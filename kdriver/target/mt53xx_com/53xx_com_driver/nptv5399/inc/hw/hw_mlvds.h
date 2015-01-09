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
  * $RCSfile: hw_mlvds.h,v $
  * $Revision: #1 $
  *
  *---------------------------------------------------------------------------*/
  
#ifndef _HW_MLVDS_H_
#define _HW_MLVDS_H_

//******************Page PANEL_INTF*******************************************
#define VINTF_B0 (IO_VIRT + 0x34600)
    #define VINTF_DISPR_BYPASS Fld(1,27,AC_MSKB3)//[27:27]
    #define VINTF_DISPR_ALLIGH_VS Fld(1,26,AC_MSKB3)//[26:26]
    #define VINTF_HV_HOLD Fld(1,23,AC_MSKB2)//[23:23]
    #define VINTF_HV_BYPASS Fld(1,22,AC_MSKB2)//[22:22]
    #define VINTF_PDP_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define VINTF_LINE_BUFFER_BYPASS Fld(1,16,AC_MSKB2)//[16:16]
    #define VINTF_EH_LATCH_HSYNC Fld(1,11,AC_MSKB1)//[11:11]
#define VINTF_B1 (IO_VIRT + 0x34604)
    #define VINTF_PTGEN_H_ACTIVE Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_PTGEN_H_TOTAL Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_B2 (IO_VIRT + 0x34608)
    #define VINTF_PTGEN_V_ACTIVE Fld(12,16,AC_MSKW32)//[27:16]
    #define VINTF_PTGEN_V_TOTAL Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B3 (IO_VIRT + 0x3460C)
    #define VINTF_PTGEN_H_START Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_PTGEN_V_START Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B4 (IO_VIRT + 0x34610)
    #define VINTF_PTGEN_H_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_PTGEN_V_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B5 (IO_VIRT + 0x34614)
    #define VINTF_PTGEN_R Fld(10,20,AC_MSKW32)//[29:20]
    #define VINTF_PTGEN_G Fld(10,10,AC_MSKW21)//[19:10]
    #define VINTF_PTGEN_B Fld(10,0,AC_MSKW10)//[9:0]
#define VINTF_B6 (IO_VIRT + 0x34618)
	#define VINTF_PTGEN_TYPE_BD_DIS Fld(1,31,AC_MSKB3)//[31:31]
	#define VINTF_PTGEN_TYPE Fld(3,24,AC_MSKB3)//[26:24]
    #define VINTF_INTF_PTGEN_EN Fld(1,21,AC_MSKB2)//[21:21]
    #define VINTF_PTGEN_LLRR Fld(1,20,AC_MSKB2)//[20:20]
    #define VINTF_PTGEN_MIRROR Fld(1,19,AC_MSKB2)//[19:19]
    #define VINTF_PTGEN_SEQ Fld(1,18,AC_MSKB2)//[18:18]
    #define VINTF_PTGEN_2CH_EN Fld(1,17,AC_MSKB2)//[17:17]
    #define VINTF_PTGEN_EN Fld(1,16,AC_MSKB2)//[16:16]
    #define VINTF_PTGEN_SET_DATA_GATE_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define VINTF_PTGEN_SET_DATA Fld(1,14,AC_MSKB1)//[14:14]
    #define VINTF_PTGEN_FIX_DISP_R_VAL Fld(1,13,AC_MSKB1)//[13:13]
    #define VINTF_PTGEN_FIX_DISP_R_EN Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_PTGEN_COLOR_BAR_TH Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B7 (IO_VIRT + 0x3461C)
    #define VINTF_LBRB_GRAY Fld(12,20,AC_MSKW32)//[31:20]
    #define VINTF_LBRB_MODE Fld(1,17,AC_MSKB2)//[17:17]
    #define VINTF_LBRB_EN Fld(1,16,AC_MSKB2)//[16:16]
#define VINTF_B8 (IO_VIRT + 0x34620)
    #define VINTF_PTGEN_BD_R Fld(10,20,AC_MSKW32)//[29:20]
    #define VINTF_PTGEN_BD_G Fld(10,10,AC_MSKW21)//[19:10]
    #define VINTF_PTGEN_BD_B Fld(10,0,AC_MSKW10)//[9:0]
#define VINTF_B9 (IO_VIRT + 0x34624)
    #define VINTF_PTGEN_H_OSD_INDEX_START Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_PTGEN_H_OSD_INDEX_END Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_B10 (IO_VIRT + 0x34628)
    #define VINTF_PTGEN_V_OSD_INDEX_START Fld(12,16,AC_MSKW32)//[27:16]
    #define VINTF_PTGEN_V_OSD_INDEX_END Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B11 (IO_VIRT + 0x3462C)
	#define VINTF_OSD_ALPHA_REV Fld(2,30,AC_MSKB3)//[31:30]
    #define VINTF_PTGEN_V_OSD_START Fld(12,16,AC_MSKW32)//[27:16]
    #define VINTF_PTGEN_V_OSD_ACTIVE Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_B12 (IO_VIRT + 0x34630)
    #define VINTF_PTGEN_OSD_TYPE Fld(8,24,AC_FULLB3)//[31:24]
	#define VINTF_PTGEN_V_RUN Fld(2,10,AC_MSKB1)//[11:10]
	#define VINTF_PTGEN_H_RUN Fld(2,8,AC_MSKB1)//[9:8]
    #define VINTF_CHESS_BOX_H Fld(2, 4, AC_MSKB0) //[5:4]
    #define VINTF_CHESS_BOX_W Fld(3,0,AC_MSKB0)//[2:0]
#define VINTF_B13 (IO_VIRT + 0x34634)
    #define VINTF_CURSOR_H Fld(12,16,AC_MSKW32)//[27:16]
    #define VINTF_CURSOR_V Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_P0 (IO_VIRT + 0x34700)
    #define VINTF_PANEL_IN_SEL Fld(2,30,AC_MSKB3)//[31:30]
    #define VINTF_PANEL_IN_B_SWAP Fld(2,12,AC_MSKB1)//[13:12]
    #define VINTF_PANEL_IN_G_SWAP Fld(2,10,AC_MSKB1)//[11:10]
    #define VINTF_PANEL_IN_R_SWAP Fld(2,8,AC_MSKB1)//[9:8]
    #define VINTF_PANEL_MONI_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_DISPR_OUT_P Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_DISPR_IN_P Fld(1,3,AC_MSKB0)//[3:3]
    #define VINTF_IF_DE_IN_P Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_IF_HSYNC_IN_P Fld(1,1,AC_MSKB0)//[1:1]
    #define VINTF_IF_VSYNC_IN_P Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P1 (IO_VIRT + 0x34704)
    #define VINTF_H_TOTAL_CLR Fld(1,31,AC_MSKB3)//[31:31]
#define VINTF_P2 (IO_VIRT + 0x34708)
    #define MAX_H_TOTAL Fld(13,16,AC_MSKW32)//[28:16]
    #define MIN_H_TOTAL Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P3 (IO_VIRT + 0x3470C)
    #define MAX_H_ACTIVE Fld(12,16,AC_MSKW32)//[27:16]
    #define MIN_H_ACTIVE Fld(12,0,AC_MSKW10)//[11:0]
#define VINTF_P4 (IO_VIRT + 0x34710)
    #define VINTF_REORDER_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
    #define VINTF_REORDER_FORMAIN_OUT Fld(2,20,AC_MSKB2)//[21:20]
     #define REORDER_FORMAIN_OUT_SEQ 0
      #define REORDER_FORMAIN_OUT_4CH_HALF 1
	  #define REORDER_FORMAIN_OUT_2CH_HALF 2
    #define VINTF_REORDER_MIDDLE_PT Fld(11,0,AC_MSKW10)//[10:0]
#define VINTF_P5 (IO_VIRT + 0x34714)
    #define TCON_3DBL_PWM Fld(1,4,AC_MSKB0)//[4:4]
    #define TCON_3DLR_PWM Fld(1,3,AC_MSKB0)//[3:3]
    #define TCON_3DR_PWM Fld(1,2,AC_MSKB0)//[2:2]
    #define TCON_3DL_PWM Fld(1,1,AC_MSKB0)//[1:1]
    #define EVEN_ODD_INFO Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P6 (IO_VIRT + 0x34718)
    #define VINTF_2EYE_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_2EYE_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P7 (IO_VIRT + 0x3471C)
    #define VINTF_2EYE_PWM_NDLY Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_2EYE_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_2EYE_OP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_2EYE_POL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_2EYE_RST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_2EYE_LRLR Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_2EYE_V_DLY Fld(2,0,AC_MSKB0)//[1:0]
#define VINTF_P8 (IO_VIRT + 0x34720)
    #define VINTF_REYE_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_REYE_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P9 (IO_VIRT + 0x34724)
    #define VINTF_REYE_PWM_NDLY Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_REYE_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_REYE_OP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_REYE_POL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_REYE_RST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_REYE_V_POS Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_REYE_LRLR Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_REYE_V_DLY Fld(2,0,AC_MSKB0)//[1:0]
#define VINTF_P10 (IO_VIRT + 0x34728)
    #define VINTF_LEYE_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_LEYE_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P11 (IO_VIRT + 0x3472C)
    #define VINTF_LEYE_PWM_NDLY Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_LEYE_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_LEYE_OP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_LEYE_POL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_LEYE_RST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_LEYE_V_POS Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_LEYE_LRLR Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_LEYE_V_DLY Fld(2,0,AC_MSKB0)//[1:0]
#define VINTF_P12 (IO_VIRT + 0x34730)
    #define VINTF_3DBL_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_3DBL_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P13 (IO_VIRT + 0x34734)
    #define VINTF_3DBL_PWM_NDLY Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_3DBL_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_3DBL_OP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_3DBL_POL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_3DBL_RST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_3DBL_LRLR Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_3DBL_V_DLY Fld(2,0,AC_MSKB0)//[1:0]
#define VINTF_P14 (IO_VIRT + 0x34738)
    #define VINTF_CRC_H_END Fld(16,16,AC_FULLW32)//[31:16]
    #define VINTF_CRC_H_START Fld(16,0,AC_FULLW10)//[15:0]
#define VINTF_P15 (IO_VIRT + 0x3473C)
    #define VINTF_CRC_VCNT Fld(16,16,AC_FULLW32)//[31:16]
    #define VINTF_CRC_ALG_SEL Fld(1,10,AC_MSKB1)//[10:10]
    #define VINTF_CRC_MODE Fld(2,8,AC_MSKB1)//[9:8]
    #define VINTF_CRC_CH_SEL Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_CRC_SRC_SEL Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_CRC_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define VINTF_CRC_START Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P16 (IO_VIRT + 0x34740)
    #define ST_INTF_CRC_OUT_23_0 Fld(24,8,AC_MSKDW)//[31:8]
    #define ST_INTF_CRC_ERR Fld(1,1,AC_MSKB0)//[1:1]
    #define ST_INTF_CRC_RDY Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P17 (IO_VIRT + 0x34744)
    #define VINTF_3D_DISP_SHIFT_VALUE_R Fld(8,16,AC_FULLB2)//[23:16]
    #define VINTF_3D_DISP_SHIFT_VALUE_L Fld(8,8,AC_FULLB1)//[15:8]
    #define VINTF_3D_DISP_SHIFT_CTL Fld(3,4,AC_MSKB0)//[6:4]
    #define VINTF_3D_DISP_SHIFT_FRAME Fld(1,3,AC_MSKB0)//[3:3]
    #define VINTF_3D_DISP_SHIFT_SAME_REG Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_3D_DISP_SHIFT_INV Fld(1,1,AC_MSKB0)//[1:1]
    #define VINTF_3D_DISP_SHIFT_EN Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P18 (IO_VIRT + 0x34748)
    #define VINTF_3D_DISP_SHIFT_B Fld(8,16,AC_FULLB2)//[23:16]
    #define VINTF_3D_DISP_SHIFT_G Fld(8,8,AC_FULLB1)//[15:8]
    #define VINTF_3D_DISP_SHIFT_R Fld(8,0,AC_FULLB0)//[7:0]
#define VINTF_P19 (IO_VIRT + 0x3474C)
    #define VINTF_DISP_R_ALLIGN_TYPE Fld(2,16,AC_MSKB2)//[17:16]
    #define VINTF_DISPR_SEL_FOR_DELAY Fld(1, 13, AC_MSKB1) //13
    #define VINTF_DISP_R_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P20 (IO_VIRT + 0x34750)
    #define VINTF_IS_DELAY_H Fld(16,16,AC_FULLW32)//[31:16]
    #define VINTF_IS_DIV_H Fld(4,12,AC_MSKB1)//[15:12]
    #define VINTF_IS_MUL_H Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_IS_FORCE_DELAY Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_IS_SYNC_SEL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_IS_USE_SC_SET_DATA Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_IS_JIT_FIL_OFF_H Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_IS_HOLD_EN_H Fld(1,3,AC_MSKB0)//[3:3]
    #define VINTF_IS_POL_H Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_SYNC_TOTAL_PLUS_1_H Fld(1,1,AC_MSKB0)//[1:1]
    #define VINTF_INV_SYNC_START Fld(1,0,AC_MSKB0)//[0:0]
#define VINTF_P21 (IO_VIRT + 0x34754)
    #define VINTF_IS_LEN_PLUS_1_H Fld(4,24,AC_MSKB3)//[27:24]
    #define VINTF_IS_MASK_LEN_H Fld(8,16,AC_FULLB2)//[23:16]
    #define VINTF_IS_PWM_SYNC_TOTAL_H Fld(16,0,AC_FULLW10)//[15:0]
#define VINTF_P22 (IO_VIRT + 0x34758)
    #define VINTF_IS_DELAY_V Fld(16,16,AC_FULLW32)//[31:16]
    #define VINTF_IS_DIV_V Fld(4,12,AC_MSKB1)//[15:12]
    #define VINTF_IS_MUL_V Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_IS_JIT_FIL_OFF_V Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_IS_HOLD_EN_V Fld(1,3,AC_MSKB0)//[3:3]
    #define VINTF_IS_POL_V Fld(1,2,AC_MSKB0)//[2:2]
    #define VINTF_SYNC_TOTAL_PLUS_1_V Fld(1,1,AC_MSKB0)//[1:1]
#define VINTF_P23 (IO_VIRT + 0x3475C)
    #define VINTF_IS_LEN_PLUS_1_V Fld(4,24,AC_MSKB3)//[27:24]
    #define VINTF_IS_MASK_LEN_V Fld(8,16,AC_FULLB2)//[23:16]
    #define VINTF_IS_PWM_SYNC_TOTAL_V Fld(16,0,AC_FULLW10)//[15:0]
#define VINTF_P24 (IO_VIRT + 0x34760)
    #define VINTF_SYNCO_POL_V Fld(1, 14, AC_MSKB1) //14
    #define VINTF_PWM_LEN_SEL_V Fld(2,12,AC_MSKB1)//[13:12]
    #define VINTF_SYNCO_POL_H Fld(1,10,AC_MSKB1)//[10:10]
    #define VINTF_PWM_LEN_SEL_H Fld(2,8,AC_MSKB1)//[9:8]
#define VINTF_P25 (IO_VIRT + 0x34764)
    #define VINTF_3DBL_2_WIDTH Fld(13,16,AC_MSKW32)//[28:16]
    #define VINTF_3DBL_2_H_DLY Fld(13,0,AC_MSKW10)//[12:0]
#define VINTF_P26 (IO_VIRT + 0x34768)
    #define VINTF_3DBL_2_PWM_NDLY Fld(1,12,AC_MSKB1)//[12:12]
    #define VINTF_3DBL_2_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define VINTF_3DBL_2_OP_EN Fld(1,7,AC_MSKB0)//[7:7]
    #define VINTF_3DBL_2_POL Fld(1,6,AC_MSKB0)//[6:6]
    #define VINTF_3DBL_2_RST_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define VINTF_3DBL_2_LRLR Fld(1,4,AC_MSKB0)//[4:4]
    #define VINTF_3DBL_2_V_DLY Fld(2,0,AC_MSKB0)//[1:0]

//******************Page LVDS*******************************************
#define LVDSB_REG00 (IO_VIRT+0x34C00)
	#define RG_LVDSTX_MON_SEL Fld(8,22,AC_MSKW32)//[29:22]
	#define RG_DISPR_P Fld(1, 15, AC_MSKB1) //[15:15]  
    #define RG_DE_P Fld(1, 14, AC_MSKB1) //[14:14]
    #define RG_VSYNC_P Fld(1, 13, AC_MSKB1) //[13:13]
    #define RG_HSYNC_P Fld(1, 12, AC_MSKB1) //[12:12]
    #define RG_LVDSTX_2CH_ARCH Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_LVDSTX_ANA_TEST Fld(1,10,AC_MSKB1)//[10:10]
	#define RG_LVDSTX_ANA_FORCE Fld(10,0,AC_MSKW10)//[9:0]
#define LVDSB_REG01 (IO_VIRT+0x34C04)
	#define RG_IBUF_BYPASS Fld(1,31,AC_MSKB3)//[31:31]
	#define RG_IBUF_SIZE Fld(11,20,AC_MSKW32)//[30:20]
	#define RG_AMST_TR Fld(11,9,AC_MSKW21)//[19:9]
	#define RG_AMST_UF_CLR Fld(1, 8, AC_MSKB1) //[8:8]
	#define RG_AMST_OF_CLR Fld(1, 7, AC_MSKB0) //[7:7]
	#define RG_IBUF_RST_SEL Fld(1, 6, AC_MSKB0) //[6:6]
#define LVDSB_REG02 (IO_VIRT+0x34C08)
    #define RG_LLV7_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LLV6_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_LLV5_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_LLV4_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_LLV3_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_LLV2_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_LLV1_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_LLV0_SEL Fld(4, 0, AC_MSKB0) //3:0	
#define LVDSB_REG03 (IO_VIRT+0x34C0C)
    #define RG_RLV7_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_RLV6_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_RLV5_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_RLV4_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_RLV3_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_RLV2_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_RLV1_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_RLV0_SEL Fld(4, 0, AC_MSKB0) //3:0
#define LVDSB_REG04 (IO_VIRT+0x34C10)
    #define RG_LLV_CK1_SEL Fld(4, 28, AC_MSKB3) //31:28
    #define RG_LLV_CK0_SEL Fld(4, 24, AC_MSKB3) //27:24
    #define RG_RLV_CK1_SEL Fld(4, 20, AC_MSKB2) //23:20
    #define RG_RLV_CK0_SEL Fld(4, 16, AC_MSKB2) //19:16
    #define RG_LLV9_SEL Fld(4, 12, AC_MSKB1) //15:12
    #define RG_LLV8_SEL Fld(4, 8, AC_MSKB1) //11:8
    #define RG_RLV9_SEL Fld(4, 4, AC_MSKB0) //7:4
    #define RG_RLV8_SEL Fld(4, 0, AC_MSKB0) //3:0
#define LVDSB_REG05 (IO_VIRT+0x34C14)
	#define RG_TOP_LLV_LINK_INV Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_TOP_LLV_PN Fld(12, 16, AC_MSKW32) //27:16
	    #define RG_LLVCK1_PN_SWAP Fld(1,27,AC_MSKB3)//[27:27]
	    #define RG_LLVCK0_PN_SWAP Fld(1,26,AC_MSKB3)//[26:26]
	    #define RG_LLV9_PN_SWAP Fld(1,25,AC_MSKB3)//[25:25]
	    #define RG_LLV8_PN_SWAP Fld(1,24,AC_MSKB3)//[24:24]
	    #define RG_LLV7_PN_SWAP Fld(1,23,AC_MSKB2)//[23:23]
	    #define RG_LLV6_PN_SWAP Fld(1,22,AC_MSKB2)//[22:22]
	    #define RG_LLV5_PN_SWAP Fld(1,21,AC_MSKB2)//[21:21]
	    #define RG_LLV4_PN_SWAP Fld(1,20,AC_MSKB2)//[20:20]
	    #define RG_LLV3_PN_SWAP Fld(1,19,AC_MSKB2)//[19:19]
	    #define RG_LLV2_PN_SWAP Fld(1,18,AC_MSKB2)//[18:18]
	    #define RG_LLV1_PN_SWAP Fld(1,17,AC_MSKB2)//[17:17]
	    #define RG_LLV0_PN_SWAP Fld(1,16,AC_MSKB2)//[16:16]
    #define RG_TOP_RLV_LINK_INV Fld(1, 12, AC_MSKB1) //[12:12]
    #define RG_TOP_RLV_PN Fld(12, 0, AC_MSKW10) //11:0
	    #define RG_RLVCK1_PN_SWAP Fld(1, 11, AC_MSKB1) //[11:11]
	    #define RG_RLVCK0_PN_SWAP Fld(1, 10, AC_MSKB1) //[10:10]
	    #define RG_RLV9_PN_SWAP Fld(1, 9, AC_MSKB1) //[9:9]
	    #define RG_RLV8_PN_SWAP Fld(1, 8, AC_MSKB1) //[8:8]
	    #define RG_RLV7_PN_SWAP Fld(1, 7, AC_MSKB0) //[7:7]
	    #define RG_RLV6_PN_SWAP Fld(1, 6, AC_MSKB0) //[6:6]
	    #define RG_RLV5_PN_SWAP Fld(1, 5, AC_MSKB0) //[5:5]
	    #define RG_RLV4_PN_SWAP Fld(1, 4, AC_MSKB0) //[4:4]
	    #define RG_RLV3_PN_SWAP Fld(1, 3, AC_MSKB0) //[3:3]
	    #define RG_RLV2_PN_SWAP Fld(1, 2, AC_MSKB0) //[2:2]
	    #define RG_RLV1_PN_SWAP Fld(1, 1, AC_MSKB0) //[1:1]
	    #define RG_RLV0_PN_SWAP Fld(1, 0, AC_MSKB0) //[0:0]
#define LVDSB_REG06 (IO_VIRT + 0x34C18)
    #define RG_REG_EVEN_ODD Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_CNTLF Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_CNTLE Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_RES Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_DE_SEL Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_VS_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_HS_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_RES_FLD Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_RGB_444_MERGE Fld(1,23,AC_MSKB2)//[23:23]
    #define RG_8BIT_DUAL Fld(1,22,AC_MSKB2)//[22:22]
    #define RG_2CH_MERGE Fld(1,21,AC_MSKB2)//[21:21]
    #define RG_MERGE_OSD Fld(1,20,AC_MSKB2)//[20:20]
    #define RG_TEST_TX_EN Fld(1,18,AC_MSKB2)//[18:18]
    #define RG_TEST_TX_EYE Fld(1,17,AC_MSKB2)//[17:17]
    #define RG_TEST_TX_DEN Fld(1,16,AC_MSKB2)//[16:16]
    #define RG_TMDS_PG_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_4CH Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_DUAL Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_ODD_SW Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_RS_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_CH_SW Fld(2,8,AC_MSKB1)//[9:8]
    #define RG_DE_DELAY_SEL Fld(1, 7, AC_MSKB0) //[7:7]
    #define RG_CLK_DCBAL Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_DCBAL Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_5381_10B_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_12B_EN Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_10B_EN Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_NS_VESA_EN Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_SPECIAL_NS Fld(1,0,AC_MSKB0)//[0:0]
#define LVDSB_REG07 (IO_VIRT + 0x34C1C)
    #define RG_CLK_CTRL_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_CLK_CTRL Fld(7,24,AC_MSKB3)//[30:24]
    #define RG_PD Fld(24,0,AC_MSKDW)//[23:0]
#define LVDSB_REG08 (IO_VIRT + 0x34C20)
    #define RG_OSD_3D_IN_HS Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_OSD_3D_IN_VS Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_PNSWAP Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_LVDS_INV Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_LVDS_7TO10FIFO_EN Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_DPMODE Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_C_LINE_EXT Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_LPF_EN Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_YUV2YC_EN Fld(1,9,AC_MSKB1)//[9:9]
    #define RG_UVINV Fld(1,8,AC_MSKB1)//[8:8]
    #define RG_D_SW Fld(2,6,AC_MSKB0)//[7:6]
    #define RG_C_SW Fld(2,4,AC_MSKB0)//[5:4]
    #define RG_B_SW Fld(2,2,AC_MSKB0)//[3:2]
    #define RG_A_SW Fld(2,0,AC_MSKB0)//[1:0]
#define LVDSB_REG09 (IO_VIRT + 0x34C24)
    #define RG_TMDS_SEL Fld(32,0,AC_FULLDW)//[31:0]
#define LVDSB_REG10 (IO_VIRT + 0x34C28)
    #define RG_CRC_VCNT Fld(16,8,AC_FULLW21)//[23:8]
    #define RG_CRC_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define RG_CRC_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_CRC_START Fld(1,0,AC_MSKB0)//[0:0]
#define LVDSB_REG11 (IO_VIRT + 0x34C2C)
    #define ST_VGA_CRC_OUT Fld(24,8,AC_MSKDW)//[31:8]
    #define ST_VGA_CRC_ERR Fld(1,1,AC_MSKB0)//[1:1]
    #define ST_VGA_CRC_RDY Fld(1,0,AC_MSKB0)//[0:0]
#define LVDSB_REG12 (IO_VIRT + 0x34C30)
    #define ST_LVDS_CRC_OUT_23_0 Fld(24,8,AC_MSKDW)//[31:8]
    #define ST_LVDS_CRC_ERR Fld(1,1,AC_MSKB0)//[1:1]    
    #define ST_LVDS_CRC_RDY Fld(1,0,AC_MSKB0)//[0:0]
#define LVDSB_REG13 (IO_VIRT + 0x34C34)
    #define ST_LVDS_CRC_OUT_41_24 Fld(18,0,AC_MSKDW)//[17:0]   
#define LVDSB_REG14 (IO_VIRT + 0x34C38)
    #define ST_FIFO_LEFT_CNT Fld(16,16,AC_FULLW32)//[31:16]
    #define ST_FIFO_OVER Fld(1, 11, AC_MSKB1) //11
    #define ST_FIFO_UNDER Fld(1, 10, AC_MSKB1) //10
    #define ST_FIFO_FULL Fld(1,9,AC_MSKB1)//[9:9]
    #define ST_FIFO_EMPTY Fld(1,8,AC_MSKB1)//[8:8]
    #define ST_LVDS_DISP_R Fld(1, 0, AC_MSKB0) //0
#define LVDSB_REG15 (IO_VIRT + 0x34C3C)
    #define RG_TMDS_PG0 Fld(32,0,AC_FULLDW)//[31:0]      
#define LVDSB_REG16 (IO_VIRT + 0x34C40)
    #define RG_TMDS_PG1 Fld(32,0,AC_FULLDW)//[31:0]  
#define LVDSB_REG17 (IO_VIRT + 0x34C44)
    #define RG_TMDS_PG2 Fld(32,0,AC_FULLDW)//[31:0]  
#define LVDSB_REG18 (IO_VIRT + 0x34C48)
    #define RG_RES_EVEN_ODD_L Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_CNTLF_L Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_CNTLE_L Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_RES_L Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_DE_SEL_L Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_VS_SEL_L Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_HS_SEL_L Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_RES_FIELD_L Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_TEST_TTL_DAT Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_TEST_LC_DAT Fld(7,0,AC_MSKB0)//[6:0]
#define LVDSB_REG19 (IO_VIRT + 0x34C4C)
    #define RG_RES2_FLD Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_RES2_FLD_L Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_RES2_FLD_L_4CH Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_RES2_FLD_U_4CH Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_OSD_INDEX_TEST_ENABLE Fld(1,20,AC_MSKB2)//[20:20]
    #define RG_RES_OSD_INDEX Fld(1,19,AC_MSKB2)//[19:19]
    #define RG_RES_OSD_INDEX_L Fld(1,18,AC_MSKB2)//[18:18]
    #define RG_RES_OSD_INDEX_U_4CH Fld(1,17,AC_MSKB2)//[17:17]
    #define RG_RES_OSD_INDEX_L_4CH Fld(1,16,AC_MSKB2)//[16:16]
    #define RG_RES_EVEN_ODD_L_4CH Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_CNTLF_L_4CH Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_CNTLE_L_4CH Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_RES_L_4CH Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_DE_SEL_L_4CH Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_VS_SEL_L_4CH Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_HS_SEL_L_4CH Fld(1,9,AC_MSKB1)//[9:9]
    #define RG_RES_FIELD_L_4CH Fld(1,8,AC_MSKB1)//[8:8]
    #define RG_RES_EVEN_ODD_U_4CH Fld(1,7,AC_MSKB0)//[7:7]
    #define RG_CNTLF_U_4CH Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_CNTLE_U_4CH Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_RES_U_4CH Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_DE_SEL_U_4CH Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_VS_SEL_U_4CH Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_HS_SEL_U_4CH Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_RES_FIELD_U_4CH Fld(1,0,AC_MSKB0)//[0:0]
#define LVDSB_REG20 (IO_VIRT + 0x34C50)
    #define RG_LVDSRX_FIFO_EN Fld(1,23,AC_MSKB2)//[23:23]
    #define RG_LVDSRX_CRC_SEL Fld(2,21,AC_MSKB2)//[22:21]
    #define RG_LVDSRX_CRC_START Fld(1,20,AC_MSKB2)//[20:20]
    #define RG_LVDSRX_CRC_CLR Fld(1,19,AC_MSKB2)//[19:19]
    #define RG_LVDSRX_CRC_MODE Fld(2,17,AC_MSKB2)//[18:17]
    #define RG_LVDSRX_CRC_ALG_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define RG_LVDSRX_CRC_VCNT Fld(16,0,AC_FULLW10)//[15:0]
#define LVDSB_REG21 (IO_VIRT + 0x34C54)
    #define RG_LVDSRX_CRC_H_END Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_LVDSRX_CRC_H_START Fld(16,0,AC_FULLW10)//[15:0]
#define LVDSB_REG22 (IO_VIRT + 0x34C58)
    #define LVDSRX_CRC_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define LVDSRX_CRC_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #ifdef CC_MT5399
    #define LVDSRX_CRC_VALUE Fld(30,0,AC_MSKDW)//[29:0]
    #else
	#define LVDSRX_CRC_VALUE Fld(14,16,AC_MSKW32)//[29:16]
	#define LVDSTX_FIFO_WRPTR_ESD Fld(4,12,AC_MSKB1)//[12:15]
	#define LVDSTX_FIFO_RDPTR_ESD Fld(4,8,AC_MSKB1)//[8:11]
    #define LVDSTX_FIFO_WRPTR Fld(4,4,AC_MSKB0)//[7:4]
    #define LVDSTX_FIFO_RDPTR Fld(4,0,AC_MSKB0)//[3:0]
	#endif
#define LVDSB_REG24 (IO_VIRT + 0x34C60)
	#define OSD_IN_46_32 Fld(15,0,AC_MSKW10)//[14:0]
#define LVDSB_REG25 (IO_VIRT + 0x34C64)
    #define OSD_IN_31_0 Fld(32,0,AC_FULLDW)//[31:0]  
#define LVDSB_REG26 (IO_VIRT + 0x34C68)
	#define RG_OSD_SWITCH_CH Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_OSD_ENCODE_EN Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_OSD_HSYNC_P Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_OSD_VSYNC_P Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_OSD_DE_P Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_OSD_HS_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_OSD_VS_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_OSD_DE_SEL Fld(1,23,AC_MSKB2)//[23:23]
    #define RG_OSD_RES1_SEL Fld(1,22,AC_MSKB2)//[22:22]
    #define RG_OSD_RES2_SEL Fld(1,21,AC_MSKB2)//[21:21]
	#define RG_OSD_RES Fld(1,20,AC_MSKB2)//[20:20]
	#define RG_OSD_CNTLE Fld(1, 19, AC_MSKB2) //19
	#define RG_OSD_CNTLF Fld(1, 18, AC_MSKB2) //18
	#define RG_OSD_RES1 Fld(1, 17, AC_MSKB2) //17
    #define RG_OSD_RES2 Fld(1, 16, AC_MSKB2) //16
    #define RG_OSD_IN_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_OSD_IN_46_32 Fld(15,0,AC_MSKW10)//[14:0]
#define LVDSB_REG27 (IO_VIRT + 0x34C6C)
    #define RG_OSD_IN_31_0 Fld(32,0,AC_FULLDW)//[31:0] 


//******************Page mLVDS*******************************************
#define FIFO_CTRL_01 (IO_VIRT + 0x35000)
    #define RG_IPS_REORDER Fld(1,7,AC_MSKB0)//[7:7]
    #define RG_MLVDSM_6BIT_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define RG_MLVDSM_10BIT_LG Fld(1,5,AC_MSKB0)//[5:5]
    #define TCLK_FIFO_START Fld(1,4,AC_MSKB0)//[4:4]
    #define TCLK_FIFO_ACTIVE_PORT Fld(4,0,AC_MSKB0)//[3:0]
    
#define MLVDS_CTRL_01 (IO_VIRT + 0x35004)
    #define RG_MLVDS_RST0 Fld(8,24,AC_FULLB3)//[31:24]
    #define RG_MLVDS_RST1 Fld(8,16,AC_FULLB2)//[23:16]
    #define RG_MLVDS_RST2 Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_MLVDS_RST3 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_02 (IO_VIRT + 0x35008)
    #define RG_MLVDS_RST4 Fld(8,24,AC_FULLB3)//[31:24]
    #define RG_MLVDS_RST5 Fld(8,16,AC_FULLB2)//[23:16]
    #define RG_MLVDS_RST6 Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_MLVDS_RST7 Fld(8,0,AC_FULLB0)//[7:0]
		
#define MLVDS_CTRL_03 (IO_VIRT + 0x3500C)
    #define RG_MLVDS_RST8 Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_MLVDS_RST9 Fld(8,0,AC_FULLB0)//[7:0]
    
#define MLVDS_CTRL_04 (IO_VIRT + 0x35010)
    #define RG_CH0_RST_OFF Fld(8,24,AC_FULLB3)//[31:24]
    #define RG_CH1_RST_OFF Fld(8,16,AC_FULLB2)//[23:16]
    #define RG_CH2_RST_OFF Fld(8,8,AC_FULLB1)//[15:8]
    #define RG_CH3_RST_OFF Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_05 (IO_VIRT + 0x35014)
    #define RG_PAIR_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_SS_GATE Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_VSYNC_LATCH Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_EMBEDDED_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_SS_GATE_CNT Fld(9,16,AC_MSKW32)//[24:16]
    #define RG_ABNOR_V_CNT Fld(11,0,AC_MSKW10)//[10:0]

#define MLVDS_CTRL_06 (IO_VIRT + 0x35018)
    #define RG_OUT_DIS_CH0 Fld(10,16,AC_MSKW32)//[25:16]
    #define RG_OUT_DIS_CH1 Fld(10,0,AC_MSKW10)//[9:0]

#define MLVDS_CTRL_07 (IO_VIRT + 0x3501C)
    #define RG_OUT_DIS_CH2 Fld(10,16,AC_MSKW32)//[25:16]
    #define RG_OUT_DIS_CH3 Fld(10,0,AC_MSKW10)//[9:0]

#define MLVDS_CTRL_08 (IO_VIRT + 0x35020)
    #define RG_CH3_SEL Fld(2,6,AC_MSKB0)//[7:6]
    #define RG_CH2_SEL Fld(2,4,AC_MSKB0)//[5:4]
    #define RG_CH1_SEL Fld(2,2,AC_MSKB0)//[3:2]
    #define RG_CH0_SEL Fld(2,0,AC_MSKB0)//[1:0]

#define MLVDS_CTRL_09 (IO_VIRT + 0x35024)
    #define ASYNC_FIFO_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_MLVDSM_10BIT_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_MLVDSM_ANA_TEST Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_FIFO_SEL Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_MLVDSM_MON_SEL Fld(7,16,AC_MSKB2)//[22:16] 
    #define RG_MLVDSM_ANA_FORCE Fld(10,0,AC_MSKW10)//[9:0]

#define MLVDS_CTRL_10 (IO_VIRT + 0x35028)
    #define MLVDS_CTRL_10_RG_LLV7_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define MLVDS_CTRL_10_RG_LLV6_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define MLVDS_CTRL_10_RG_LLV5_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define MLVDS_CTRL_10_RG_LLV4_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define MLVDS_CTRL_10_RG_LLV3_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define MLVDS_CTRL_10_RG_LLV2_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define MLVDS_CTRL_10_RG_LLV1_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define MLVDS_CTRL_10_RG_LLV0_SEL Fld(4,0,AC_MSKB0)//[3:0]

#define MLVDS_CTRL_11 (IO_VIRT + 0x3502C)
    #define MLVDS_CTRL_11_RG_RLV7_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define MLVDS_CTRL_11_RG_RLV6_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define MLVDS_CTRL_11_RG_RLV5_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define MLVDS_CTRL_11_RG_RLV4_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define MLVDS_CTRL_11_RG_RLV3_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define MLVDS_CTRL_11_RG_RLV2_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define MLVDS_CTRL_11_RG_RLV1_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define MLVDS_CTRL_11_RG_RLV0_SEL Fld(4,0,AC_MSKB0)//[3:0]

#define MLVDS_CTRL_12 (IO_VIRT + 0x35030)
    #define MLVDS_CTRL_12_RG_LLV_CK1_SEL Fld(4,28,AC_MSKB3)//[31:28]
    #define MLVDS_CTRL_12_RG_LLV_CK0_SEL Fld(4,24,AC_MSKB3)//[27:24]
    #define MLVDS_CTRL_12_RG_RLV_CK1_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define MLVDS_CTRL_12_RG_RLV_CK0_SEL Fld(4,16,AC_MSKB2)//[19:16]
    #define MLVDS_CTRL_12_RG_LLV9_SEL Fld(4,12,AC_MSKB1)//[15:12]
    #define MLVDS_CTRL_12_RG_LLV8_SEL Fld(4,8,AC_MSKB1)//[11:8]
    #define MLVDS_CTRL_12_RG_RLV9_SEL Fld(4,4,AC_MSKB0)//[7:4]
    #define MLVDS_CTRL_12_RG_RLV8_SEL Fld(4,0,AC_MSKB0)//[3:0]

#define MLVDS_CTRL_13 (IO_VIRT + 0x35034)
    #define MLVDS_CTRL_13_TOP_LLV_PN Fld(12,16,AC_MSKW32)//[27:16]
    #define MLVDS_CTRL_13_RG_TOP_RLV_PN Fld(12,0,AC_MSKW10)//[11:0]

    #define RG_LLVCK1_PN_SWAP Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_LLVCK0_PN_SWAP Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_LLV9_PN_SWAP Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_LLV8_PN_SWAP Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_LLV7_PN_SWAP Fld(1,23,AC_MSKB2)//[23:23]
    #define RG_LLV6_PN_SWAP Fld(1,22,AC_MSKB2)//[22:22]
    #define RG_LLV5_PN_SWAP Fld(1,21,AC_MSKB2)//[21:21]
    #define RG_LLV4_PN_SWAP Fld(1,20,AC_MSKB2)//[20:20]
    #define RG_LLV3_PN_SWAP Fld(1,19,AC_MSKB2)//[19:19]
    #define RG_LLV2_PN_SWAP Fld(1,18,AC_MSKB2)//[18:18]
    #define RG_LLV1_PN_SWAP Fld(1,17,AC_MSKB2)//[17:17]
    #define RG_LLV0_PN_SWAP Fld(1,16,AC_MSKB2)//[16:16]
    
    #define RG_RLVCK1_PN_SWAP Fld(1, 11, AC_MSKB1) //[11:11]
    #define RG_RLVCK0_PN_SWAP Fld(1, 10, AC_MSKB1) //[10:10]
    #define RG_RLV9_PN_SWAP Fld(1, 9, AC_MSKB1) //[9:9]
    #define RG_RLV8_PN_SWAP Fld(1, 8, AC_MSKB1) //[8:8]
    #define RG_RLV7_PN_SWAP Fld(1, 7, AC_MSKB0) //[7:7]
    #define RG_RLV6_PN_SWAP Fld(1, 6, AC_MSKB0) //[6:6]
    #define RG_RLV5_PN_SWAP Fld(1, 5, AC_MSKB0) //[5:5]
    #define RG_RLV4_PN_SWAP Fld(1, 4, AC_MSKB0) //[4:4]
    #define RG_RLV3_PN_SWAP Fld(1, 3, AC_MSKB0) //[3:3]
    #define RG_RLV2_PN_SWAP Fld(1, 2, AC_MSKB0) //[2:2]
    #define RG_RLV1_PN_SWAP Fld(1, 1, AC_MSKB0) //[1:1]
    #define RG_RLV0_PN_SWAP Fld(1, 0, AC_MSKB0) //[0:0]

#define MLVDS_CTRL_14 (IO_VIRT + 0x35038)
	#define RG_TOP_CRC_ALG_SEL Fld(1,24,AC_MSKB3)//[24:24]
	#define RG_TOP_CRC_NEW Fld(1,18,AC_MSKB2)//[18:18]
	#define RG_TOP_CRC_MODE Fld(2,16,AC_MSKB2)//[17:16]
	#define RG_TOP_CRC_V_CNT Fld(16,0,AC_FULLW10)//[15:0]
	
#define MLVDS_CTRL_15 (IO_VIRT + 0x3503C)
	#define RG_TOP_CRC_H_END Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_TOP_CRC_H_START Fld(16,0,AC_FULLW10)//[15:0]

#define MLVDS_CTRL_16 (IO_VIRT + 0x35040)
	#define RG_CRC_HLEN_NEW Fld(14,16,AC_MSKW32)//[29:16]
    #define RG_TOP_CRC_SEL_L Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_TOP_CRC_CLR Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_TOP_CRC_START Fld(1,0,AC_MSKB0)//[0:0]

#define MLVDS_CTRL_17 (IO_VIRT + 0x35044)
	#define ST_TOP_CRC1_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define ST_TOP_CRC1_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #define ST_TOP_CRC1_VALUE Fld(30,0,AC_MSKDW)//[29:0]

#define MLVDS_CTRL_18 (IO_VIRT + 0x35048)
	#define ST_TOP_CRC2_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define ST_TOP_CRC2_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #define ST_TOP_CRC2_VALUE Fld(30,0,AC_MSKDW)//[29:0]

#define MLVDS_CTRL_19 (IO_VIRT + 0x3504C)
	#define ST_TOP_CRC3_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define ST_TOP_CRC3_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #define ST_TOP_CRC3_VALUE Fld(30,0,AC_MSKDW)//[29:0]

#define MLVDS_CTRL_20 (IO_VIRT + 0x35050)
	#define ST_TOP_CRC4_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define ST_TOP_CRC4_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #define ST_TOP_CRC4_VALUE Fld(30,0,AC_MSKDW)//[29:0]

#define MLVDS_CTRL_21 (IO_VIRT + 0x35054)
	#define RG_MLVDS_MLT0 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_MLT1 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_MLT2 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_MLT3 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_22 (IO_VIRT + 0x35058)
	#define RG_MLVDS_MLT4 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_MLT5 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_MLT6 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_MLT7 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_23 (IO_VIRT + 0x3505C)
	#define RG_MLVDS_MLT8 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_MLT9 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_24 (IO_VIRT + 0x35060)
	#define RG_MLVDS_TCON0_SEL0 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_TCON0_SEL1 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_TCON0_SEL2 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON0_SEL3 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_25 (IO_VIRT + 0x35064)
	#define RG_MLVDS_TCON0_SEL4 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_TCON0_SEL5 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_TCON0_SEL6 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON0_SEL7 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_26 (IO_VIRT + 0x35068)
	#define RG_MLVDS_TCON0_SEL8 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON0_SEL9 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_27 (IO_VIRT + 0x3506C)
	#define RG_MLVDS_TCON1_SEL0 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_TCON1_SEL1 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_TCON1_SEL2 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON1_SEL3 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_28 (IO_VIRT + 0x35070)
	#define RG_MLVDS_TCON1_SEL4 Fld(8,24,AC_FULLB3)//[31:24]
	#define RG_MLVDS_TCON1_SEL5 Fld(8,16,AC_FULLB2)//23:16]
	#define RG_MLVDS_TCON1_SEL6 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON1_SEL7 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_29 (IO_VIRT + 0x35074)
	#define RG_MLVDS_TCON1_SEL8 Fld(8,8,AC_FULLB1)//[15:8]
	#define RG_MLVDS_TCON1_SEL9 Fld(8,0,AC_FULLB0)//[7:0]

#define MLVDS_CTRL_30 (IO_VIRT + 0x35078)
	#define RG_CH0_LV0_RST_SEL Fld(2,30,AC_MSKB3)//[31:30]
	#define RG_CH0_LV1_RST_SEL Fld(2,28,AC_MSKB3)//[29:28]
	#define RG_CH0_LV2_RST_SEL Fld(2,26,AC_MSKB3)//[27:26]
	#define RG_CH0_LV3_RST_SEL Fld(2,24,AC_MSKB3)//[25:24]
	#define RG_CH0_LV4_RST_SEL Fld(2,22,AC_MSKB2)//[23:22]
	#define RG_CH0_LV5_RST_SEL Fld(2,20,AC_MSKB2)//[21:20]
	#define RG_CH0_LV6_RST_SEL Fld(2,18,AC_MSKB2)//[19:18]
	#define RG_CH0_LV7_RST_SEL Fld(2,16,AC_MSKB2)//[17:16]
	#define RG_CH1_LV0_RST_SEL Fld(2,14,AC_MSKB1)//[15:14]
	#define RG_CH1_LV1_RST_SEL Fld(2,12,AC_MSKB1)//[13:12]
	#define RG_CH1_LV2_RST_SEL Fld(2,10,AC_MSKB1)//[11:10]
	#define RG_CH1_LV3_RST_SEL Fld(2,8,AC_MSKB1)//[9:8]
	#define RG_CH1_LV4_RST_SEL Fld(2,6,AC_MSKB0)//[7:6]
	#define RG_CH1_LV5_RST_SEL Fld(2,4,AC_MSKB0)//[5:4]
	#define RG_CH1_LV6_RST_SEL Fld(2,2,AC_MSKB0)//[3:2]
	#define RG_CH1_LV7_RST_SEL Fld(2,0,AC_MSKB0)//[1:0]
	
#define MLVDS_CTRL_31 (IO_VIRT + 0x3507C)
	#define RG_CH2_LV0_RST_SEL Fld(2,30,AC_MSKB3)//[31:30]
	#define RG_CH2_LV1_RST_SEL Fld(2,28,AC_MSKB3)//[29:28]
	#define RG_CH2_LV2_RST_SEL Fld(2,26,AC_MSKB3)//[27:26]
	#define RG_CH2_LV3_RST_SEL Fld(2,24,AC_MSKB3)//[25:24]
	#define RG_CH2_LV4_RST_SEL Fld(2,22,AC_MSKB2)//[23:22]
	#define RG_CH2_LV5_RST_SEL Fld(2,20,AC_MSKB2)//[21:20]
	#define RG_CH2_LV6_RST_SEL Fld(2,18,AC_MSKB2)//[19:18]
	#define RG_CH2_LV7_RST_SEL Fld(2,16,AC_MSKB2)//[17:16]
	#define RG_CH3_LV0_RST_SEL Fld(2,14,AC_MSKB1)//[15:14]
	#define RG_CH3_LV1_RST_SEL Fld(2,12,AC_MSKB1)//[13:12]
	#define RG_CH3_LV2_RST_SEL Fld(2,10,AC_MSKB1)//[11:10]
	#define RG_CH3_LV3_RST_SEL Fld(2,8,AC_MSKB1)//[9:8]
	#define RG_CH3_LV4_RST_SEL Fld(2,6,AC_MSKB0)//[7:6]
	#define RG_CH3_LV5_RST_SEL Fld(2,4,AC_MSKB0)//[5:4]
	#define RG_CH3_LV6_RST_SEL Fld(2,2,AC_MSKB0)//[3:2]
	#define RG_CH3_LV7_RST_SEL Fld(2,0,AC_MSKB0)//[1:0]	

#define MLVDS_CTRL_32 (IO_VIRT + 0x35080)
	#define RG_CH0_LLV0_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_33 (IO_VIRT + 0x35084)
	#define RG_CH0_LLV1_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_34 (IO_VIRT + 0x35088)
	#define RG_CH0_LLV2_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_35 (IO_VIRT + 0x3508C)
	#define RG_CH0_LLV3_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_36 (IO_VIRT + 0x35090)
	#define RG_CH0_LLV4_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_37 (IO_VIRT + 0x35094)
	#define RG_CH0_LLV5_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_38 (IO_VIRT + 0x35098)
	#define RG_CH0_LLV6_REG Fld(32,0,AC_FULLDW)//[31:0]
	
#define MLVDS_CTRL_39 (IO_VIRT + 0x3509C)
	#define RG_CH0_LLV7_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_40 (IO_VIRT + 0x350A0)
	#define RG_CH1_LLV0_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_41 (IO_VIRT + 0x350A4)
	#define RG_CH1_LLV1_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_42 (IO_VIRT + 0x350A8)
	#define RG_CH1_LLV2_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_43 (IO_VIRT + 0x350AC)
	#define RG_CH1_LLV3_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_44 (IO_VIRT + 0x350B0)
	#define RG_CH1_LLV4_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_45 (IO_VIRT + 0x350B4)
	#define RG_CH1_LLV5_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_46 (IO_VIRT + 0x350B8)
	#define RG_CH1_LLV6_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_47 (IO_VIRT + 0x350BC)
	#define RG_CH1_LLV7_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_48 (IO_VIRT + 0x350C0)
	#define RG_CH2_LLV0_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_49 (IO_VIRT + 0x350C4)
	#define RG_CH2_LLV1_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_50 (IO_VIRT + 0x350C8)
	#define RG_CH2_LLV2_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_51 (IO_VIRT + 0x350CC)
	#define RG_CH2_LLV3_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_52 (IO_VIRT + 0x350D0)
	#define RG_CH2_LLV4_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_53 (IO_VIRT + 0x350D4)
	#define RG_CH2_LLV5_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_54 (IO_VIRT + 0x350D8)
	#define RG_CH2_LLV6_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_55 (IO_VIRT + 0x350DC)
	#define RG_CH2_LLV7_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_56 (IO_VIRT + 0x350E0)
	#define RG_CH3_LLV0_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_57 (IO_VIRT + 0x350E4)
	#define RG_CH3_LLV1_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_58 (IO_VIRT + 0x350E8)
	#define RG_CH3_LLV2_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_59 (IO_VIRT + 0x350EC)
	#define RG_CH3_LLV3_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_60 (IO_VIRT + 0x350F0)
	#define RG_CH3_LLV4_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_61 (IO_VIRT + 0x350F4)
	#define RG_CH3_LLV5_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_62 (IO_VIRT + 0x350F8)
	#define RG_CH3_LLV6_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_63 (IO_VIRT + 0x350FC)
	#define RG_CH0_LLV7_REG Fld(32,0,AC_FULLDW)//[31:0]

#define MLVDS_CTRL_64 (IO_VIRT + 0x35100)
	#define RG_TCON0_SEL Fld(4,28,AC_MSKB3)//[31:28]
	#define RG_TCON1_SEL Fld(4,24,AC_MSKB3)//[27:24]
	#define RG_CH0_TCON_XOR Fld(2,22,AC_MSKB2)//[23:22]
	#define RG_CH1_TCON_XOR Fld(2,20,AC_MSKB2)//[21:20]
	#define RG_CH2_TCON_XOR Fld(2,18,AC_MSKB2)//[19:18]
	#define RG_CH3_TCON_XOR Fld(2,16,AC_MSKB2)//[17:16]
	#define RG_MLT_EN Fld(1,10,AC_MSKB1)//[10:10]
	#define RG_MLT_HSWITCH Fld(10,0,AC_MSKW10)//[9:0]

#define MLVDS_CTRL_65 (IO_VIRT + 0x35104)
	#define RG_CH0_TCON_XOR_7 Fld(1,31,AC_MSKB3)//[31:31]
	#define RG_CH0_TCON_XOR_8 Fld(1,30,AC_MSKB3)//[30:30]
	#define RG_CH0_TCON_XOR_9 Fld(1,29,AC_MSKB3)//[29:29]
	#define RG_CH0_CTRL_BIT_SEL Fld(2,20,AC_MSKDW)//[23:0]

#define MLVDS_CTRL_66 (IO_VIRT + 0x35108)
	#define RG_TCON2_SEL Fld(4,28,AC_MSKB3)//[31:28]
	#define RG_TCON3_SEL Fld(4,24,AC_MSKB3)//[27:24]
	#define RG_TCON4_SEL Fld(4,20,AC_MSKB2)//[23:20]
	#define RG_TCON5_SEL Fld(4,16,AC_MSKB2)//[19:16]
	#define RG_TCON6_SEL Fld(4,12,AC_MSKB1)//[15:12]
	#define RG_TCON7_SEL Fld(4,8,AC_MSKB1)//[11:8]
	#define RG_TCON8_SEL Fld(4,4,AC_MSKB0)//[7:4]
	#define RG_TCON9_SEL Fld(4,0,AC_MSKB0)//[3:0]

#define MLVDS_CTRL_67 (IO_VIRT + 0x3510C)
	#define RG_CH1_TCON_XOR_7 Fld(1,31,AC_MSKB3)//[31:31]
	#define RG_CH1_TCON_XOR_8 Fld(1,30,AC_MSKB3)//[30:30]
	#define RG_CH1_TCON_XOR_9 Fld(1,29,AC_MSKB3)//[29:29]
	#define RG_CH1_CTRL_BIT_SEL Fld(2,20,AC_MSKDW)//[23:0]

#define MLVDS_CTRL_68 (IO_VIRT + 0x35110)
	#define RG_TCONA_SEL Fld(4,28,AC_MSKB3)//[31:28]
	#define RG_TCONB_SEL Fld(4,24,AC_MSKB3)//[27:24]
	#define RG_TCONC_SEL Fld(4,20,AC_MSKB2)//[23:20]
	#define RG_TCOND_SEL Fld(4,16,AC_MSKB2)//[19:16]
	#define RG_TCONE_SEL Fld(4,12,AC_MSKB1)//[15:12]
	#define RG_TCONF_SEL Fld(4,8,AC_MSKB1)//[11:8]

#define DPC_CTRL_01 (IO_VIRT + 0x35180)
    #define RG_DPC_TABLE07 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE06 Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE05 Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE04 Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE03 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE02 Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE01 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE00 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_02 (IO_VIRT + 0x35184)
    #define RG_DPC_TABLE0F Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE0E Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE0D Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE0C Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE0B Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE0A Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE09 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE08 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_03 (IO_VIRT + 0x35188)
    #define RG_DPC_TABLE17 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE16 Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE15 Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE14 Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE13 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE12 Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE11 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE10 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_04 (IO_VIRT + 0x3518C)
    #define RG_DPC_TABLE1F Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE1E Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE1D Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE1C Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE1B Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE1A Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE19 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE18 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_05 (IO_VIRT + 0x35190)
    #define RG_DPC_TABLE27 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE26 Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE25 Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE24 Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE23 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE22 Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE21 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE20 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_06 (IO_VIRT + 0x35194)
    #define RG_DPC_TABLE2F Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE2E Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE2D Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE2C Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE2B Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE2A Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE29 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE28 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_07 (IO_VIRT + 0x35198)
    #define RG_DPC_TABLE37 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE36 Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE35 Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE34 Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE33 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE32 Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE31 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE30 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_08 (IO_VIRT + 0x3519C)
    #define RG_DPC_TABLE3F Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_DPC_TABLE3E Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_TABLE3D Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TABLE3C Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TABLE3B Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TABLE3A Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TABLE39 Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TABLE38 Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_09 (IO_VIRT + 0x351A0)
    #define RG_DPC_ORDER_WR Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_DPC_ORDER Fld(3,24,AC_MSKB3)//[26:24]
    #define RG_DPC_MODE_EN Fld(6,16,AC_MSKB2)//[21:16]
    #define RG_DPC_CMODE Fld(5,8,AC_MSKB1)//[12:8]
    #define RG_DPC_MODE_SEL Fld(2,5,AC_MSKB0)//[6:5]
    #define RG_DPC_COEF Fld(4,1,AC_MSKB0)//[4:1]
    #define RG_DPC_DUAL Fld(1,0,AC_MSKB0)//[0:0]
    
#define DPC_CTRL_10 (IO_VIRT + 0x351A4)
    #define RG_DPC_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
    
#define DPC_CTRL_11 (IO_VIRT + 0x351A8)
    #define RG_DPC_ROI_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_DPC_ROI_HST Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_DPC_ROI_HEND Fld(12,0,AC_MSKW10)//[11:0]
    
#define DPC_CTRL_12 (IO_VIRT + 0x351AC)
    #define RG_DPC_ROI_VST Fld(11,16,AC_MSKW32)//[26:16]
    #define RG_DPC_ROI_VEND Fld(11,0,AC_MSKW10)//[10:0]
    
#define DPC_CTRL_13 (IO_VIRT + 0x351B0)
    #define RG_DPC_ROW0_MD3 Fld(12,16,AC_MSKW32)//[27:16]
    #define DPC_V_SHIFT_REG3 Fld(2,14,AC_MSKB1)//[15:14]
    #define DPC_H_SHIFT_REG3 Fld(2,12,AC_MSKB1)//[13:12]
    #define DPC_V_SHIFT_REG2 Fld(2,10,AC_MSKB1)//[11:10]
    #define DPC_H_SHIFT_REG2 Fld(2,8,AC_MSKB1)//[9:8]
    #define DPC_V_SHIFT_REG1 Fld(2,6,AC_MSKB0)//[7:6]
    #define DPC_H_SHIFT_REG1 Fld(2,4,AC_MSKB0)//[5:4]
    #define DPC_V_SHIFT_REG0 Fld(2,2,AC_MSKB0)//[3:2]
    #define DPC_H_SHIFT_REG0 Fld(2,0,AC_MSKB0)//[1:0]
    
#define DPC_CTRL_14 (IO_VIRT + 0x351B4)
    #define DPC_R_MON0 Fld(10,20,AC_MSKW32)//[29:20]
    #define DPC_G_MON0 Fld(10,10,AC_MSKW21)//[19:10]
    #define DPC_B_MON0 Fld(10,0,AC_MSKW10)//[9:0]
    
#define DPC_CTRL_15 (IO_VIRT + 0x351B8)
    #define DPC_R_MON1 Fld(10,20,AC_MSKW32)//[29:20]
    #define DPC_G_MON1 Fld(10,10,AC_MSKW21)//[19:10]
    #define DPC_B_MON1 Fld(10,0,AC_MSKW10)//[9:0]
    
#define DPC_CTRL_16 (IO_VIRT + 0x351BC)
    #define RG_DPC_TH0_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_17 (IO_VIRT + 0x351C0)
    #define RG_DPC_TH1_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_18 (IO_VIRT + 0x351C4)
    #define RG_DPC_TH2_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_19 (IO_VIRT + 0x351C8)
    #define RG_DPC_TH3_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_20 (IO_VIRT + 0x351CC)
    #define RG_DPC_TH4_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_21 (IO_VIRT + 0x351D0)
    #define RG_DPC_TH5_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_22 (IO_VIRT + 0x351D4)
    #define RG_DPC_TH5_MSB Fld(3,20,AC_MSKB2)//[22:20]
    #define RG_DPC_TH4_MSB Fld(3,16,AC_MSKB2)//[18:16]
    #define RG_DPC_TH3_MSB Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_DPC_TH2_MSB Fld(3,8,AC_MSKB1)//[10:8]
    #define RG_DPC_TH1_MSB Fld(3,4,AC_MSKB0)//[6:4]
    #define RG_DPC_TH0_MSB Fld(3,0,AC_MSKB0)//[2:0]
    
#define DPC_CTRL_23 (IO_VIRT + 0x351D8)
    #define RG_DPC_V_SEL Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_DPC_H_SEL Fld(12,0,AC_MSKW10)//[11:0]
    
#define DPC_CTRL_24 (IO_VIRT + 0x351DC)
    #define DPC_SC0_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_25 (IO_VIRT + 0x351E0)
    #define DPC_SC1_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_26 (IO_VIRT + 0x351E4)
    #define DPC_SC2_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_27 (IO_VIRT + 0x351E8)
    #define DPC_SC3_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_28 (IO_VIRT + 0x351EC)
    #define DPC_SC4_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_29 (IO_VIRT + 0x351F0)
    #define DPC_SC5_LSB Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_30 (IO_VIRT + 0x351F4)
    #define DPC_MSB Fld(24,0,AC_MSKDW)//[23:0]
    
#define DPC_CTRL_31 (IO_VIRT + 0x351F8)
    #define DPC_MON Fld(32,0,AC_FULLDW)//[31:0]
    
#define DPC_CTRL_32 (IO_VIRT + 0x351FC)
    #define RG_DPC_MONSEL Fld(4,0,AC_MSKB0)//[3:0]
    
//PPML
#define PPML_CONTROL01 (IO_VIRT + 0x35200)
    #define PPML_PAIR Fld(1,8,AC_MSKB1)//[8:8]
    #define PPML_PORT_EN Fld(8,0,AC_FULLB0)//[7:0]
    
#define PPML_CONTROL02 (IO_VIRT + 0x35204)
    #define PPML_TCODE_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define PPML_TCODE1 Fld(10,16,AC_MSKW32)//[25:16]
    #define PPML_TCODE2 Fld(10,0,AC_MSKW10)//[9:0]
    
#define PPML_CONTROL03 (IO_VIRT + 0x35208)
    #define PPML_TCODE1_VEND Fld(12,16,AC_MSKW32)//[27:16]
    #define PPML_TCODE1_VST Fld(12,0,AC_MSKW10)//[11:0]
    
#define PPML_CONTROL04 (IO_VIRT + 0x3520C)
    #define PPML_TCODE2_VEND Fld(12,16,AC_MSKW32)//[27:16]
    #define PPML_TCODE2_VST Fld(12,0,AC_MSKW10)//[11:0]
    
#define PPML_CONTROL05 (IO_VIRT + 0x35210)
    #define PPML_TCODE1_HST Fld(13,16,AC_MSKW32)//[28:16]
    #define PPML_TCODE1_HEND Fld(13,0,AC_MSKW10)//[12:0]
    
#define PPML_CONTROL06 (IO_VIRT + 0x35214)
    #define PPML_TCODE2_HST Fld(13,16,AC_MSKW32)//[28:16]
    #define PPML_TCODE2_HEND Fld(13,0,AC_MSKW10)//[12:0]
    
#define PPML_CONTROL07 (IO_VIRT + 0x35218)
    #define PPML_RST_OFF Fld(16,0,AC_FULLW10)//[15:0]
    
#define PPML_CONTROL08 (IO_VIRT + 0x3521C)
    #define PPML_RST0 Fld(8,24,AC_FULLB3)//[31:24]
    #define PPML_RST1 Fld(8,16,AC_FULLB2)//[23:16]
    #define PPML_RST2 Fld(8,8,AC_FULLB1)//[15:8]
    #define PPML_RST3 Fld(8,0,AC_FULLB0)//[7:0]
    
#define PPML_CONTROL09 (IO_VIRT + 0x35220)
    #define PPML_RST4 Fld(8,24,AC_FULLB3)//[31:24]
    #define PPML_RST5 Fld(8,16,AC_FULLB2)//[23:16]
    #define PPML_RST6 Fld(8,8,AC_FULLB1)//[15:8]
    #define PPML_RST7 Fld(8,0,AC_FULLB0)//[7:0]
    
#define PPML_CONTROL10 (IO_VIRT + 0x35224)
    #define PPML_RST8 Fld(8,24,AC_FULLB3)//[31:24]
    #define PPML_RST9 Fld(8,16,AC_FULLB2)//[23:16]
    #define PPML_RSTA Fld(8,8,AC_FULLB1)//[15:8]
    #define PPML_RSTB Fld(8,0,AC_FULLB0)//[7:0]
    
#define PPML_CONTROL11 (IO_VIRT + 0x35228)
    #define PPML_ANA_CIRBUF_RST Fld(1,31,AC_MSKB3)//[31:31]
    #define PPML_10BIT_INV Fld(1,30,AC_MSKB3)//[30:30]
    #define PPML_ANA_TEST Fld(1,29,AC_MSKB3)//[29:29]
    #define PPML_ANA_FORCE Fld(10,0,AC_MSKW10)//[9:0]
    
#define PPML_CONTROL12 (IO_VIRT + 0x3522C)
    #define PPML_CH0_PORT_SWAP Fld(3,28,AC_MSKB3)//[30:28]
    #define PPML_CH1_PORT_SWAP Fld(3,24,AC_MSKB3)//[26:24]
    #define PPML_CH2_PORT_SWAP Fld(3,20,AC_MSKB2)//[22:20]
    #define PPML_CH3_PORT_SWAP Fld(3,16,AC_MSKB2)//[18:16]
    #define PPML_CH4_PORT_SWAP Fld(3,12,AC_MSKB1)//[14:12]
    #define PPML_CH5_PORT_SWAP Fld(3,8,AC_MSKB1)//[10:8]
    #define PPML_CH6_PORT_SWAP Fld(3,4,AC_MSKB0)//[6:4]
    #define PPML_CH7_PORT_SWAP Fld(3,0,AC_MSKB0)//[2:0]	

#if 0
//LVDS need to modify	
#define MLVDS_P0 (IO_VIRT+0x35280)          
    #define RG_WXGA_MIRROR_EN Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_WXGA_MID_PXL Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_LVDS_IBUF_RST_SEL Fld(1, 15, AC_MSKB1) //15
    #define RG_LVDS_IBUF_RST_EN Fld(1,14,AC_MSKB1)//[14:14]
    #define RG_LVDS_DISPR_P Fld(1, 11, AC_MSKB1) //11
    #define RG_LVDS_DE_P Fld(1, 10, AC_MSKB1) //10
    #define RG_LVDS_VSYNC_P Fld(1, 9, AC_MSKB1) //9
    #define RG_LVDS_HSYNC_P Fld(1, 8, AC_MSKB1) //8
    #define RG_WXGA_DUMY_MODE Fld(3,3,AC_MSKB0)//[5:3]
    #define RG_DE_IN_P Fld(1, 2, AC_MSKB0) //2
    #define RG_HSYNC_IN_P Fld(1, 1, AC_MSKB0) //1
    #define RG_VSYNC_IN_P Fld(1, 0, AC_MSKB0) //0
#define MLVDS_P1 (IO_VIRT+0x35284)
    #define MLVDSM_IN_DETECT_CLR Fld(1, 31, AC_MSKB3) //31
    #define RG_PCLK_RESET Fld(1, 0, AC_MSKB0) //0
#endif

//Page PIXEL_REORDER
#define PIXEL_REORDER_01 (IO_VIRT + 0x35C00)
    #define RG_PIXEL_REORDER_INPUT_SEL Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_OUTPUT_PORT_NUMBER Fld(4,20,AC_MSKB2)//[23:20]
    #define RG_READ_COUNT_NUMBER Fld(4,16,AC_MSKB2)//[19:16]
    #define RG_WRITE_COUNT_NUMBER Fld(2,14,AC_MSKB1)//[15:14]
    #define RG_H_HOLD Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_V_HOLD Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_HS_OUT_POL Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_VS_OUT_POL Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_BACK_SIDE_DUMMY_DATA_REG Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_02 (IO_VIRT + 0x35C04)
    #define RG_DUMMY_PIXEL1_ENABLE Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_DUMMY_PIXEL2_ENABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_DMY_PXL1_R Fld(10,20,AC_MSKW32)//[29:20]
    #define RG_DMY_PXL1_G Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_DMY_PXL1_B Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_03 (IO_VIRT + 0x35C08)
    #define RG_DMY_PXL2_R Fld(10,20,AC_MSKW32)//[29:20]
    #define RG_DMY_PXL2_G Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_DMY_PXL2_B Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_04 (IO_VIRT + 0x35C0C)
    #define RG_EXT_DMY_PXL_R_LEN Fld(5,26,AC_MSKB3)//[30:26]
    #define RG_DMY_PXL1_R_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_DMY_PXL1_G_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_DMY_PXL1_R_POS Fld(12,12,AC_MSKW21)//[23:12]
    #define RG_DMY_PXL1_G_POS Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_05 (IO_VIRT + 0x35C10)
    #define RG_EXT_DMY_PXL_G_LEN Fld(5,26,AC_MSKB3)//[30:26]
    #define RG_DMY_PXL1_B_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_DMY_PXL2_R_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_DMY_PXL1_B_POS Fld(12,12,AC_MSKW21)//[23:12]
    #define RG_DMY_PXL2_R_POS Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_06 (IO_VIRT + 0x35C14)
    #define RG_EXT_DMY_PXL_B_LEN Fld(5,26,AC_MSKB3)//[30:26]
    #define RG_DMY_PXL2_G_SEL Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_DMY_PXL2_B_SEL Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_DMY_PXL2_G_POS Fld(12,12,AC_MSKW21)//[23:12]
    #define RG_DMY_PXL2_B_POS Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_07 (IO_VIRT + 0x35C18)
    #define RG_PIXEL_REORDER_SHARE_SRAM Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_EXTENDED_DUMMY_PIXEL_ENABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_EXT_DMY_PXL_R Fld(10,20,AC_MSKW32)//[29:20]
    #define RG_EXT_DMY_PXL_G Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_EXT_DMY_PXL_B Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_08 (IO_VIRT + 0x35C1C)
    #define RG_BLUE_SHIFT_MODE Fld(4,26,AC_MSKB3)//[29:26]
    #define RG_BS_FNT_DMY_DATA_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define RG_BS_FNT_DMY_DATA_REG Fld(10,14,AC_MSKW21)//[23:14]
    #define RG_BS_TO_FIRST Fld(1,13,AC_MSKB1)//[13:13]
    #define RG_FP_MODE Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_FP_FNT_DMY_DATA_SEL Fld(2,10,AC_MSKB1)//[11:10]
    #define RG_FP_FNT_DMY_DATA_REG Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_09 (IO_VIRT + 0x35C20)
    #define RG_TRD_EN Fld(1,27,AC_MSKB3)//[27:27]
    #define RG_DRD_EN Fld(1,26,AC_MSKB3)//[26:26]
    #define RG_BS_MODE_EXT_DE Fld(1,25,AC_MSKB3)//[25:25]
    #define RG_BS2_MODE Fld(1,24,AC_MSKB3)//[24:24]
    #define RG_TRD_MODE Fld(6,18,AC_MSKB2)//[23:18]
    #define RG_TRD_FNT_DMY_DATA_SEL Fld(3,13,AC_MSKB1)//[15:13]
    #define RG_BS_MODE_EXT_ADD_R Fld(1,12,AC_MSKB1)//[12:12]
    #define RG_BS_MIRROR_MODE Fld(1,11,AC_MSKB1)//[11:11]
    #define RG_FP_MIRROR_MODE Fld(1,10,AC_MSKB1)//[10:10]
    #define RG_BS_OFFSET_DMY_PXL1_R Fld(1,5,AC_MSKB0)//[5:5]
    #define RG_BS_OFFSET_DMY_PXL1_G Fld(1,4,AC_MSKB0)//[4:4]
    #define RG_BS_OFFSET_DMY_PXL1_B Fld(1,3,AC_MSKB0)//[3:3]
    #define RG_BS_OFFSET_DMY_PXL2_R Fld(1,2,AC_MSKB0)//[2:2]
    #define RG_BS_OFFSET_DMY_PXL2_G Fld(1,1,AC_MSKB0)//[1:1]
    #define RG_BS_OFFSET_DMY_PXL2_B Fld(1,0,AC_MSKB0)//[0:0]
#define PIXEL_REORDER_10 (IO_VIRT + 0x35C24)
    #define RG_TRD_FNT_DMY_R_DATA_REG Fld(10,20,AC_MSKW32)//[29:20]
    #define RG_TRD_FNT_DMY_G_DATA_REG Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_TRD_FNT_DMY_B_DATA_REG Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_11 (IO_VIRT + 0x35C28)
    #define RG_HTOTAL_SEL Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_HTOTAL_R Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_HSTART_R Fld(13,0,AC_MSKW10)//[12:0]
#define PIXEL_REORDER_12 (IO_VIRT + 0x35C2C)
    #define RG_HWIDTH_R Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_HACTIVE_R Fld(13,0,AC_MSKW10)//[12:0]
#define PIXEL_REORDER_13 (IO_VIRT + 0x35C30)
    #define RG_DE_HSTART_R Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_DE_HWIDTH_R Fld(13,0,AC_MSKW10)//[12:0]
#define PIXEL_REORDER_14 (IO_VIRT + 0x35C34)
    #define RG_PRE_DE_HSTART_R Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_PRE_DE_HWIDTH_R Fld(13,0,AC_MSKW10)//[12:0]
#define PIXEL_REORDER_15 (IO_VIRT + 0x35C38)
    #define RG_VSTART_R Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_VWIDTH_R Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_16 (IO_VIRT + 0x35C3C)
    #define RG_VDE_VSTART_R Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_VACTIVE_SEL Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_VACTIVE_R Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_17 (IO_VIRT + 0x35C40)
    #define RG_OUTPUT_DUMMY_ENABLE Fld(1,30,AC_MSKB3)//[30:30]
    #define RG_OUT_DMY_R Fld(10,20,AC_MSKW32)//[29:20]
    #define RG_OUT_DMY_G Fld(10,10,AC_MSKW21)//[19:10]
    #define RG_OUT_DMY_B Fld(10,0,AC_MSKW10)//[9:0]
#define PIXEL_REORDER_18 (IO_VIRT + 0x35C44)
    #define RG_OUT_DMY_LOW_BOUND Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_OUT_DMY_HIGH_BOUND Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_19 (IO_VIRT + 0x35C48)
    #define RG_SPLIT_HS_OUT_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_SPLIT_HWIDTH_R Fld(13,16,AC_MSKW32)//[28:16]
    #define RG_MASK_BACK_SIDE_EN Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_MASK_BACK_SIDE_TH Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_20 (IO_VIRT + 0x35C4C)
    #define RG_W_LENGTH Fld(13,0,AC_MSKW10)//[12:0]
#define PIXEL_REORDER_21 (IO_VIRT + 0x35C50)
    #define RG_W_CNT_MODE Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_W_OFFSET1 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_W_ST_ADDR1 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_W_LN_NUM1 Fld(2,14,AC_MSKB1)//[15:14]
    #define RG_W_PL_NUM1 Fld(2,12,AC_MSKB1)//[13:12]
    #define RG_W_LENGTH1 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_22 (IO_VIRT + 0x35C54)
    #define RG_W_OFFSET2 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_W_ST_ADDR2 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_W_LN_NUM2 Fld(2,14,AC_MSKB1)//[15:14]
    #define RG_W_PL_NUM2 Fld(2,12,AC_MSKB1)//[13:12]
    #define RG_W_LENGTH2 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_23 (IO_VIRT + 0x35C58)
    #define RG_W_OFFSET3 Fld(3,28,AC_MSKB3)//[30:28]
    #define RG_W_ST_ADDR3 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_W_LN_NUM3 Fld(2,14,AC_MSKB1)//[15:14]
    #define RG_W_PL_NUM3 Fld(2,12,AC_MSKB1)//[13:12]
    #define RG_W_LENGTH3 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_24 (IO_VIRT + 0x35C5C)
    #define RG_R_LN_NUM Fld(3,29,AC_MSKB3)//[31:29]
    #define RG_R_CNT_MODE Fld(1,28,AC_MSKB3)//[28:28]
    #define RG_MON_SEL Fld(8,16,AC_FULLB2)//[23:16]
#define PIXEL_REORDER_25 (IO_VIRT + 0x35C60)
    #define RG_R_OFFSET1 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR11 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE1 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM1 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR12 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_26 (IO_VIRT + 0x35C64)
    #define RG_R_OFFSET2 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR21 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE2 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM2 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR22 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_27 (IO_VIRT + 0x35C68)
    #define RG_R_OFFSET3 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR31 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE3 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM3 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR32 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_28 (IO_VIRT + 0x35C6C)
    #define RG_R_OFFSET4 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR41 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE4 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM4 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR42 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_29 (IO_VIRT + 0x35C70)
    #define RG_R_OFFSET5 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR51 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE5 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM5 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR52 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_30 (IO_VIRT + 0x35C74)
    #define RG_R_OFFSET6 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR61 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE6 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM6 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR62 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_31 (IO_VIRT + 0x35C78)
    #define RG_R_OFFSET7 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR71 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE7 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM7 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR72 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_32 (IO_VIRT + 0x35C7C)
    #define RG_R_OFFSET8 Fld(4,28,AC_MSKB3)//[31:28]
    #define RG_R_ST_ADDR81 Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_R_TWO_CNT_MODE8 Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_R_LN_NUM8 Fld(3,12,AC_MSKB1)//[14:12]
    #define RG_R_ST_ADDR82 Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_37 (IO_VIRT + 0x35C90)
    #define RG_PIXEL_REORDER_CRC_INPUT_SEL Fld(5,24,AC_MSKB3)//[28:24]
    #define RG_PIXEL_REORDER_START Fld(1,20,AC_MSKB2)//[20:20]
    #define RG_PIXEL_REORDER_CLEAR Fld(1,19,AC_MSKB2)//[19:19]
    #define RG_PIXEL_REORDER_CRC_MODE Fld(2,17,AC_MSKB2)//[18:17]
    #define RG_PIXEL_REORDER_ALG_SEL Fld(1,16,AC_MSKB2)//[16:16]
    #define RG_PIXEL_REORDER_CRC_VCNT Fld(16,0,AC_FULLW10)//[15:0]
#define PIXEL_REORDER_38 (IO_VIRT + 0x35C94)
    #define RG_PIXEL_REORDER_H_START Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_PIXEL_REORDER_H_END Fld(16,0,AC_FULLW10)//[15:0]
#define PIXEL_REORDER_39 (IO_VIRT + 0x35C98)
    #define PIXEL_REORDER_CRC_DONE Fld(1,31,AC_MSKB3)//[31:31]
    #define PIXEL_REORDER_CRC_FAIL Fld(1,30,AC_MSKB3)//[30:30]
    #define PIXEL_REORDER_CRC_CAL Fld(30,0,AC_MSKDW)//[29:0]
#define PIXEL_REORDER_41 (IO_VIRT + 0x35CA0)
    #define RG_DIS_BLK_MD_OUT Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_VSTART_BLANK Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_VWIDTH_BLANK Fld(12,0,AC_MSKW10)//[11:0]
#define PIXEL_REORDER_42 (IO_VIRT + 0x35CA4)
    #define RG_DIS_BLK_PRE_DE_OUT Fld(1,31,AC_MSKB3)//[31:31]
    #define RG_VDE_VSTART_BLANK Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_DIS_BLK_DE_OUT Fld(1,15,AC_MSKB1)//[15:15]
    #define RG_VACTIVE_BLANK Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_PATTERN_01 (IO_VIRT + 0x35CC0)
    #define DIS_SW_MUTE_PATTERN Fld(1,29,AC_MSKB3)//[29:29]
    #define RG_MUTE_F_CNT Fld(3,26,AC_MSKB3)//[28:26]
    #define RG_NOR_F_CNT Fld(3,23,AC_MSKW32)//[25:23]
    #define MANUAL_PATTERN_ENABLE Fld(1,22,AC_MSKB2)//[22:22]
    #define RG_ABNOR_TCTG_F_CNT Fld(4,0,AC_MSKB0)//[3:0]
#define MUTE_PATTERN_02 (IO_VIRT + 0x35CC4)
    #define MUTE_H_TOTAL Fld(13,16,AC_MSKW32)//[28:16]
    #define MUTE_H_WIDTH Fld(13,0,AC_MSKW10)//[12:0]
#define MUTE_PATTERN_03 (IO_VIRT + 0x35CC8)
    #define MUTE_H_START Fld(13,16,AC_MSKW32)//[28:16]
    #define MUTE_H_ACTIVE Fld(13,0,AC_MSKW10)//[12:0]
#define MUTE_PATTERN_04 (IO_VIRT + 0x35CCC)
    #define MUTE_V_TOTAL Fld(12,16,AC_MSKW32)//[27:16]
    #define MUTE_V_WIDTH Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_PATTERN_05 (IO_VIRT + 0x35CD0)
    #define MUTE_V_START Fld(12,16,AC_MSKW32)//[27:16]
    #define MUTE_V_ACTIVE Fld(12,0,AC_MSKW10)//[11:0]
#define MUTE_PATTERN_06 (IO_VIRT + 0x35CD4)
    #define RG_BIST_TIME_TH Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_BIST_TIME Fld(4,4,AC_MSKB0)//[7:4]
    #define RG_BIST_CNT Fld(4,0,AC_MSKB0)//[3:0]
#define MUTE_PATTERN_07 (IO_VIRT + 0x35CD8)
    #define RG_BIST0 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_BIST1 Fld(16,0,AC_FULLW10)//[15:0]
#define MUTE_PATTERN_08 (IO_VIRT + 0x35CDC)
    #define RG_BIST2 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_BIST3 Fld(16,0,AC_FULLW10)//[15:0]
#define MUTE_PATTERN_09 (IO_VIRT + 0x35CE0)
    #define RG_BIST4 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_BIST5 Fld(16,0,AC_FULLW10)//[15:0]
#define MUTE_PATTERN_10 (IO_VIRT + 0x35CE4)
    #define RG_BIST6 Fld(16,16,AC_FULLW32)//[31:16]
    #define RG_BIST7 Fld(16,0,AC_FULLW10)//[15:0]
#define MUTE_PATTERN_11 (IO_VIRT + 0x35CE8)
    #define RG_H_COLOR_BAR_TH Fld(12,16,AC_MSKW32)//[27:16]
    #define RG_V_COLOR_BAR_TH Fld(12,0,AC_MSKW10)//[11:0]
	

#endif
