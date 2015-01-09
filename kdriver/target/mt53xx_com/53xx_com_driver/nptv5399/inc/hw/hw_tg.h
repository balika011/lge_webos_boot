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
  * $RCSfile: hw_tg.h,v $
  * $Revision: #1 $
  *
  *---------------------------------------------------------------------------*/
  
#ifndef _HW_TG_H_
#define _HW_TG_H_

#define TCONIO_VIRT IO_VIRT

//TCON_TOP
#define TMGR0 (IO_VIRT + 0x34000)
    #define TMGR0_CKRST_CFG Fld(16,16,AC_FULLW32)//[31:16]
    #define TMGR0_CKEN_CFG Fld(16,0,AC_FULLW10)//[15:0]

        #define CKEN_CFG_2Port_FIFO Fld(1, 15, AC_MSKB0)  
        #define CKEN_CFG_Pixel_Reorder Fld(1, 14, AC_MSKB0)
        #define CKEN_CFG_LVDS_TX Fld(1, 13, AC_MSKB0)  
        #define CKEN_CFG_PPML Fld(1, 12, AC_MSKB0)
        #define CKEN_CFG_LVDS_ANA Fld(1, 8, AC_MSKB0) 

        #define CKEN_CFG_EPI Fld(1, 6, AC_MSKB0)  
	    #define CKEN_CFG_iDP Fld(1, 5, AC_MSKB0)
        #define CKEN_CFG_VB1 Fld(1, 4, AC_MSKB0)  
        #define CKEN_CFG_MLVDS Fld(1, 3, AC_MSKB0)
		#define CKEN_CFG_PWM_SCAN Fld(1, 2, AC_MSKB0)
        #define CKEN_CFG_TCON Fld(1, 1, AC_MSKB0) 
        #define CKEN_CFG_EH Fld(1, 0, AC_MSKB0) 

#define TMGR1 (IO_VIRT + 0x34004)
    #define TMGR1_MON_SEL Fld(8,8,AC_FULLB1)//[15:8]
    #define TMGR1_RG_TCLK_DBGSEL Fld(1,1,AC_MSKB0)//[1:1]

#define TMGR2 (IO_VIRT + 0x34008)
    #define TMGR2_TCLK_PLLRST Fld(1,31,AC_MSKB3)//[31:31]
    #define TMGR2_TCLK_PLLSEL Fld(3,28,AC_MSKB3)//[30:28]
    #define TMGR2_TCLK_PLLBP Fld(1,27,AC_MSKB3)//[27:27]
    #define TMGR2_RG_FIFO_CK_SEL Fld(3,16,AC_MSKB2)//[18:16]
    #define TMGR2_CKEN_CFG1 Fld(16,0,AC_FULLW10)//[15:0]

#define TMGR3 (IO_VIRT + 0x3400C)
    #define TMGR3_RG_TCON_OUT_SEL Fld(3,12,AC_MSKB1)//[14:12]
    #define TMGR3_TCON_HS_PWM_P Fld(1,5,AC_MSKB0)//[5:5]
    #define TMGR3_TCON_VS_PWM_P Fld(1,4,AC_MSKB0)//[4:4]
    #define TMGR3_TCON_DE_OUT_P Fld(1,2,AC_MSKB0)//[2:2]
    #define TMGR3_TCON_HS_OUT_P Fld(1,1,AC_MSKB0)//[1:1]
    #define TMGR3_TCON_VS_OUT_P Fld(1,0,AC_MSKB0)//[0:0]

#define TMGR5 (IO_VIRT + 0x34014)
    #define TMGR5_TX_CLK_CTS_INV Fld(1,29,AC_MSKB3)//[29:29]
    #define TMGR5_TX_CLK_INV Fld(1,28,AC_MSKB3)//[28:28]
    #define TMGR5_RG_FIFO_CTRL Fld(3,20,AC_MSKB2)//[22:20]
    #define TMGR5_RG_FIFO_EN Fld(1, 16, AC_MSKB2) //16

#define TMGR7 (IO_VIRT + 0x3401C)
    #define TMGR7_RG_LOOPPAT_2TO1 Fld(1,25,AC_MSKB3)//[25:25]
    #define TMGR7_RG_LOOPPAT_EN Fld(1,24,AC_MSKB3)//[24:24]
    #define TMGR7_IMP_CAL_SET Fld(1,21,AC_MSKB2)//[21:21]
    #define TMGR7_RG_RXIMP Fld(4,15,AC_MSKW21)//[18:15]
    #define TMGR7_RG_AUTOK_RX_IMP Fld(1,12,AC_MSKB1)//[12:12]
    #define TMGR7_RG_TXIMP Fld(4,8,AC_MSKB1)//[11:8]
    #define TMGR7_REG_IMP_DP_DIS Fld(1,7,AC_MSKB0)//[7:7]
    #define TMGR7_RG_IMPFREQSEL Fld(1,6,AC_MSKB0)//[6:6]
    #define TMGR7_RG_AUTOK_TX_IMP Fld(1,4,AC_MSKB0)//[4:4]
    #define TMGR7_REG_SEL_1_ALL Fld(1,2,AC_MSKB0)//[2:2]
    #define TMGR7_SAPIS_MODE_SYNC Fld(1,1,AC_MSKB0)//[1:1]
    #define TMGR7_CTL_EN_CALIBRATION Fld(1,0,AC_MSKB0)//[0:0]

#define TMGR8 (IO_VIRT + 0x34020)
    #define TMGR8_RG_LVDSA_TERM_ECK Fld(4,28,AC_MSKB3)//[31:28]
    #define TMGR8_RG_LVDSA_TERM_O0 Fld(4,24,AC_MSKB3)//[27:24]
    #define TMGR8_RG_LVDSA_TERM_O1 Fld(4,20,AC_MSKB2)//[23:20]
    #define TMGR8_RG_LVDSA_TERM_O2 Fld(4,16,AC_MSKB2)//[19:16]
    #define TMGR8_RG_LVDSA_TERM_O3 Fld(4,12,AC_MSKB1)//[15:12]
    #define TMGR8_RG_LVDSA_TERM_O4 Fld(4,8,AC_MSKB1)//[11:8]
    #define TMGR8_RG_LVDSA_TERM_O5 Fld(4,4,AC_MSKB0)//[7:4]
    #define TMGR8_RG_LVDSA_TERM_OCK Fld(4,0,AC_MSKB0)//[3:0]

#define TMGR9 (IO_VIRT + 0x34024)
    #define TMGR9_RG_LVDSA_TERM_E0 Fld(4,20,AC_MSKB2)//[23:20]
    #define TMGR9_RG_LVDSA_TERM_E1 Fld(4,16,AC_MSKB2)//[19:16]
    #define TMGR9_RG_LVDSA_TERM_E2 Fld(4,12,AC_MSKB1)//[15:12]
    #define TMGR9_RG_LVDSA_TERM_E3 Fld(4,8,AC_MSKB1)//[11:8]
    #define TMGR9_RG_LVDSA_TERM_E4 Fld(4,4,AC_MSKB0)//[7:4]
    #define TMGR9_RG_LVDSA_TERM_E5 Fld(4,0,AC_MSKB0)//[3:0]
    
#define TMGR10 (IO_VIRT + 0x34028)
    #define TMGR10_RG_LVDSA_IMP_CALI_EN Fld(12,0,AC_MSKW10)//[11:0]
    
#define TMGR11 (IO_VIRT + 0x3402C)
    #define TMGR11_RG_TMDS_PG0 Fld(32,0,AC_FULLDW)//[31:0]
    
#define TMGR12 (IO_VIRT + 0x34030)
    #define TMGR12_RG_TMDS_PG1 Fld(32,0,AC_FULLDW)//[31:0]
    
#define TMGR13 (IO_VIRT + 0x34034)
    #define TMGR13_RG_TMDS_PG2 Fld(32,0,AC_FULLDW)//[31:0]
    
#define TMGR14 (IO_VIRT + 0x34038)
    #define TMGR14_RG_TMDS_CTRL_CK Fld(24,0,AC_MSKDW)//[23:0]
    
#define TMGR15 (IO_VIRT + 0x3403C)
    #define TMGR15_RG_TMDS_CTRL_INV Fld(24,0,AC_MSKDW)//[23:0]

#define TMGR16 (IO_VIRT + 0x34040)
    #define TMGR16_RG_TMDS_CTRL_REG_EN Fld(24,0,AC_MSKDW)//[23:0]

#define TMGR17 (IO_VIRT + 0x34044)
    #define TMGR17_RG_TMDS_REG_VALUE Fld(10,0,AC_MSKW10)//[9:0]

#define TMGR18 (IO_VIRT + 0x34048)
    #define TMGR18_RG_LOCK_VALUE Fld(1,1,AC_MSKB0)//[1:1]
    #define TMGR18_RG_LOCK_CTRL Fld(1,0,AC_MSKB0)//[0:0]
    
#define TMGR19 (IO_VIRT + 0x34060)
    #define TMGR19_DA_IMPCAL_TX_LAT Fld(4,16,AC_MSKB2)//[19:16]
    #define TMGR19_GEN_STATE Fld(3,12,AC_MSKB1)//[14:12]
    #define TMGR19_IMP_PD Fld(1,8,AC_MSKB1)//[8:8]
    #define TMGR19_IMP_CALIB_DONE Fld(1,4,AC_MSKB0)//[4:4]
    #define TMGR19_HOLD Fld(1,0,AC_MSKB0)//[0:0]



//TCON
#define GR0 (IO_VIRT + 0x34800)
    #define GR0_TC_EN Fld(1,31,AC_MSKB3)//[31:31]
    #define GR0_TCTG_HM Fld(1,30,AC_MSKB3)//[30:30]
    #define GR0_TCTG_FCR Fld(14,16,AC_MSKW32)//[29:16]
    #define GR0_TCTG_OEN Fld(16,0,AC_FULLW10)//[15:0]
        #define GR0_TCTG_OEN_GCLK Fld(1, 3, AC_MSKB0)
        #define GR0_TCTG_OEN_GCLK_END Fld(1, 8, AC_MSKB1)
        #define GR0_TCTG_OEN_MCLK Fld(1, 4, AC_MSKB0)
        #define GR0_TCTG_OEN_GST Fld(1, 5, AC_MSKB0)
        #define GR0_TCTG_OEN_GST_END Fld(1, 9, AC_MSKB1)
        #define GR0_TCTG_OEN_EO Fld(1, 6, AC_MSKB0)
        #define GR0_TCTG_OEN_EO2 Fld(1, 10, AC_MSKB1)
        #define GR0_TCTG_OEN_VCOM_DYN Fld(1, 7, AC_MSKB0)

        #define GR0_TCTG_OEN_TCONF Fld(1,15, AC_MSKB1)
        #define GR0_TCTG_OEN_TCONE Fld(1,14, AC_MSKB1)
        #define GR0_TCTG_OEN_TCOND Fld(1,13, AC_MSKB1)
        #define GR0_TCTG_OEN_TCONC Fld(1,12, AC_MSKB1)
        #define GR0_TCTG_OEN_TCONB Fld(1,11, AC_MSKB1)
        #define GR0_TCTG_OEN_TCONA Fld(1,10, AC_MSKB1)
        #define GR0_TCTG_OEN_TCON9 Fld(1, 9, AC_MSKB1)
        #define GR0_TCTG_OEN_TCON8 Fld(1, 8, AC_MSKB1)
        #define GR0_TCTG_OEN_TCON7 Fld(1, 7, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON6 Fld(1, 6, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON5 Fld(1, 5, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON4 Fld(1, 4, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON3 Fld(1, 3, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON2 Fld(1, 2, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON1 Fld(1, 1, AC_MSKB0)
        #define GR0_TCTG_OEN_TCON0 Fld(1, 0, AC_MSKB0)
		
#define GR1 (IO_VIRT + 0x34804)
    #define GR1_DPC_SWEN Fld(1,31,AC_MSKB3)//[31:31]
    #define GR1_DPC_EN Fld(1,30,AC_MSKB3)//[30:30]
    #define GR1_TCTG_HSINV Fld(1,27,AC_MSKB3)//[27:27]
    #define GR1_TCTG_VSINV Fld(1,26,AC_MSKB3)//[26:26]
    #define GR1_DPC_SW_MODE Fld(3,23,AC_MSKW32)//[25:23]
    #define GR1_TCON_INT_CLR Fld(6,16,AC_MSKB2)//[21:16]
    #define GR1_TCON_INT_EN Fld(6,8,AC_MSKB1)//[13:8]
    #define GR1_TCON_INT Fld(6,0,AC_MSKB0)//[5:0]

#define GR2 (IO_VIRT + 0x34808)
    #define GR2_DPC_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define GR2_DPC_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define GR2_DPC_VTR Fld(12,0,AC_MSKW10)//[11:0]

#define GR3 (IO_VIRT + 0x3480C)
    #define GR3_DPC_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define GR3_DPC_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define GR3_DPC_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define GR3_FINV_SEL Fld(2,24,AC_MSKB3)//[25:24]
    #define GR3_GR_MON_SEL Fld(4,20,AC_MSKB2)//[23:20]
    #define GR3_LINE_OS Fld(3,16,AC_MSKB2)//[18:16]
    #define GR3_DPC_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define GR3_DPC_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]

#define GR4 (IO_VIRT + 0x34810)
    #define GR4_TG_MUXSEL_B15 Fld(4,28,AC_MSKB3)//[31:28]
    #define GR4_TG_MUXSEL_B14 Fld(4,24,AC_MSKB3)//[27:24]
    #define GR4_TG_MUXSEL_B13 Fld(4,20,AC_MSKB2)//[23:20]
    #define GR4_TG_MUXSEL_B12 Fld(4,16,AC_MSKB2)//[19:16]
    #define GR4_TG_MUXSEL_B11 Fld(4,12,AC_MSKB1)//[15:12]
    #define GR4_TG_MUXSEL_B10 Fld(4,8,AC_MSKB1)//[11:8]
    #define GR4_TG_MUXSEL_B9 Fld(4,4,AC_MSKB0)//[7:4]
    #define GR4_TG_MUXSEL_B8 Fld(4,0,AC_MSKB0)//[3:0]

#define GR5 (IO_VIRT + 0x34814)
    #define GR5_TG_MUXSEL_B7 Fld(4,28,AC_MSKB3)//[31:28]
    #define GR5_TG_MUXSEL_B6 Fld(4,24,AC_MSKB3)//[27:24]
    #define GR5_TG_MUXSEL_B5 Fld(4,20,AC_MSKB2)//[23:20]
    #define GR5_TG_MUXSEL_B4 Fld(4,16,AC_MSKB2)//[19:16]
    #define GR5_TG_MUXSEL_B3 Fld(4,12,AC_MSKB1)//[15:12]
    #define GR5_TG_MUXSEL_B2 Fld(4,8,AC_MSKB1)//[11:8]
    #define GR5_TG_MUXSEL_B1 Fld(4,4,AC_MSKB0)//[7:4]
    #define GR5_TG_MUXSEL_B0 Fld(4,0,AC_MSKB0)//[3:0]

#define GR6 (IO_VIRT + 0x34818)
    #define GR6_TG_MODE15 Fld(2,30,AC_MSKB3)//[31:30]
    #define GR6_TG_MODE14 Fld(2,28,AC_MSKB3)//[29:28]
    #define GR6_TG_MODE13 Fld(2,26,AC_MSKB3)//[27:26]
    #define GR6_TG_MODE12 Fld(2,24,AC_MSKB3)//[25:24]
    #define GR6_TG_MODE11 Fld(2,22,AC_MSKB2)//[23:22]
    #define GR6_TG_MODE10 Fld(2,20,AC_MSKB2)//[21:20]
    #define GR6_TG_MODE9 Fld(2,18,AC_MSKB2)//[19:18]
    #define GR6_TG_MODE8 Fld(2,16,AC_MSKB2)//[17:16]
    #define GR6_TG_MODE7 Fld(2,14,AC_MSKB1)//[15:14]
    #define GR6_TG_MODE6 Fld(2,12,AC_MSKB1)//[13:12]
    #define GR6_TG_MODE5 Fld(2,10,AC_MSKB1)//[11:10]
    #define GR6_TG_MODE4 Fld(2,8,AC_MSKB1)//[9:8]
    #define GR6_TG_MODE3 Fld(2,6,AC_MSKB0)//[7:6]
    #define GR6_TG_MODE2 Fld(2,4,AC_MSKB0)//[5:4]
    #define GR6_TG_MODE1 Fld(2,2,AC_MSKB0)//[3:2]
    #define GR6_TG_MODE0 Fld(2,0,AC_MSKB0)//[1:0]

#define GR7 (IO_VIRT + 0x3481C)
    #define GR7_TCO_MUTE Fld(16,16,AC_FULLW32)//[31:16]
    #define GR7_TCO_DEFAULT Fld(16,0,AC_FULLW10)//[15:0]

#define GR8 (IO_VIRT + 0x34820)
    #define GR8_TCGPO Fld(16,16,AC_FULLW32)//[31:16]
    #define GR8_TCGPOEN Fld(16,0,AC_FULLW10)//[15:0]
        #define GR8_TCGPO_A Fld(1, 26, AC_MSKB3) //26
        #define GR8_TCGPOEN_A Fld(1, 10, AC_MSKB1) //10

#define GR9 (IO_VIRT + 0x34824)
    #define GR9_CMD_EN Fld(4,16,AC_MSKB2)//[19:16]
    #define GR9_ABNF_EN Fld(16,0,AC_FULLW10)//[15:0]
#define GR10 (IO_VIRT + 0x34828)
    #define GR10_TG_MUXSEL_C7 Fld(4,28,AC_MSKB3)//[31:28]
    #define GR10_TG_MUXSEL_C6 Fld(4,24,AC_MSKB3)//[27:24]
    #define GR10_TG_MUXSEL_C5 Fld(4,20,AC_MSKB2)//[23:20]
    #define GR10_TG_MUXSEL_C4 Fld(4,16,AC_MSKB2)//[19:16]
    #define GR10_TG_MUXSEL_C3 Fld(4,12,AC_MSKB1)//[15:12]
    #define GR10_TG_MUXSEL_C2 Fld(4,8,AC_MSKB1)//[11:8]
    #define GR10_TG_MUXSEL_C1 Fld(4,4,AC_MSKB0)//[7:4]
    #define GR10_TG_MUXSEL_C0 Fld(4,0,AC_MSKB0)//[3:0]
#define GR11 (IO_VIRT + 0x3482C)
    #define GR11_TG_MUXSEL_D7 Fld(4,28,AC_MSKB3)//[31:28]
    #define GR11_TG_MUXSEL_D6 Fld(4,24,AC_MSKB3)//[27:24]
    #define GR11_TG_MUXSEL_D5 Fld(4,20,AC_MSKB2)//[23:20]
    #define GR11_TG_MUXSEL_D4 Fld(4,16,AC_MSKB2)//[19:16]
    #define GR11_TG_MUXSEL_D3 Fld(4,12,AC_MSKB1)//[15:12]
    #define GR11_TG_MUXSEL_D2 Fld(4,8,AC_MSKB1)//[11:8]
    #define GR11_TG_MUXSEL_D1 Fld(4,4,AC_MSKB0)//[7:4]
    #define GR11_TG_MUXSEL_D0 Fld(4,0,AC_MSKB0)//[3:0]
#define GR12 (IO_VIRT + 0x34830)
    #define GR12_TCTG_VCNT_INT Fld(12,16,AC_MSKW32)//[27:16]
    #define GR12_TCTG_FCNT_INT Fld(14,0,AC_MSKW10)//[13:0]

#define TCSEL0 (IO_VIRT + 0x34838)
    #define TCSEL0_TG_OUTSEL15 Fld(4,28,AC_MSKB3)//[31:28]
    #define TCSEL0_TG_OUTSEL14 Fld(4,24,AC_MSKB3)//[27:24]
    #define TCSEL0_TG_OUTSEL13 Fld(4,20,AC_MSKB2)//[23:20]
    #define TCSEL0_TG_OUTSEL12 Fld(4,16,AC_MSKB2)//[19:16]
    #define TCSEL0_TG_OUTSEL11 Fld(4,12,AC_MSKB1)//[15:12]
    #define TCSEL0_TG_OUTSEL10 Fld(4,8,AC_MSKB1)//[11:8]
    #define TCSEL0_TG_OUTSEL9 Fld(4,4,AC_MSKB0)//[7:4]
    #define TCSEL0_TG_OUTSEL8 Fld(4,0,AC_MSKB0)//[3:0]
#define TCSEL1 (IO_VIRT + 0x3483C)
    #define TCSEL1_TG_OUTSEL7 Fld(4,28,AC_MSKB3)//[31:28]
    #define TCSEL1_TG_OUTSEL6 Fld(4,24,AC_MSKB3)//[27:24]
    #define TCSEL1_TG_OUTSEL5 Fld(4,20,AC_MSKB2)//[23:20]
    #define TCSEL1_TG_OUTSEL4 Fld(4,16,AC_MSKB2)//[19:16]
    #define TCSEL1_TG_OUTSEL3 Fld(4,12,AC_MSKB1)//[15:12]
    #define TCSEL1_TG_OUTSEL2 Fld(4,8,AC_MSKB1)//[11:8]
    #define TCSEL1_TG_OUTSEL1 Fld(4,4,AC_MSKB0)//[7:4]
    #define TCSEL1_TG_OUTSEL0 Fld(4,0,AC_MSKB0)//[3:0]

#define TIM0R0 (IO_VIRT + 0x34840)
    #define TIM0R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM0R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM0R1 (IO_VIRT + 0x34844)
    #define TIM0R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM0R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM0R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM0R2 (IO_VIRT + 0x34848)
    #define TIM0R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM0R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM0R3 (IO_VIRT + 0x3484C)
    #define TIM0R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM0R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM0R4 (IO_VIRT + 0x34850)
    #define TIM0R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM0R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM0R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM0R5 (IO_VIRT + 0x34854)
    #define TIM0R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM0R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM0R6 (IO_VIRT + 0x34858)
    #define TIM0R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM0R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM0R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM0R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM0R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM0R7 (IO_VIRT + 0x3485C)
    #define TIM0R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM0R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM0R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM0R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM0R7_FTR Fld(14,0,AC_MSKW10)//[13:0]



#define TIM1R0 (IO_VIRT + 0x34860)
    #define TIM1R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM1R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM1R1 (IO_VIRT + 0x34864)
    #define TIM1R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM1R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM1R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM1R2 (IO_VIRT + 0x34868)
    #define TIM1R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM1R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM1R3 (IO_VIRT + 0x3486C)
    #define TIM1R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM1R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM1R4 (IO_VIRT + 0x34870)
    #define TIM1R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM1R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM1R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM1R5 (IO_VIRT + 0x34874)
    #define TIM1R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM1R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM1R6 (IO_VIRT + 0x34878)
    #define TIM1R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM1R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM1R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM1R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM1R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM1R7 (IO_VIRT + 0x3487C)
    #define TIM1R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM1R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM1R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM1R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM1R7_FTR Fld(14,0,AC_MSKW10)//[13:0]



#define TIM2R0 (IO_VIRT + 0x34880)
    #define TIM2R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM2R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM2R1 (IO_VIRT + 0x34884)
    #define TIM2R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM2R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM2R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM2R2 (IO_VIRT + 0x34888)
    #define TIM2R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM2R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM2R3 (IO_VIRT + 0x3488C)
    #define TIM2R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM2R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM2R4 (IO_VIRT + 0x34890)
    #define TIM2R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM2R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM2R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM2R5 (IO_VIRT + 0x34894)
    #define TIM2R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM2R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM2R6 (IO_VIRT + 0x34898)
    #define TIM2R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM2R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM2R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM2R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM2R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM2R7 (IO_VIRT + 0x3489C)
    #define TIM2R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM2R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM2R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM2R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM2R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM3R0 (IO_VIRT + 0x348A0)
    #define TIM3R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM3R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM3R1 (IO_VIRT + 0x348A4)
    #define TIM3R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM3R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM3R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM3R2 (IO_VIRT + 0x348A8)
    #define TIM3R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM3R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM3R3 (IO_VIRT + 0x348AC)
    #define TIM3R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM3R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM3R4 (IO_VIRT + 0x348B0)
    #define TIM3R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM3R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM3R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM3R5 (IO_VIRT + 0x348B4)
    #define TIM3R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM3R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM3R6 (IO_VIRT + 0x348B8)
    #define TIM3R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM3R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM3R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM3R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM3R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM3R7 (IO_VIRT + 0x348BC)
    #define TIM3R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM3R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM3R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM3R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM3R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM4R0 (IO_VIRT + 0x348C0)
    #define TIM4R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM4R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM4R1 (IO_VIRT + 0x348C4)
    #define TIM4R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM4R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM4R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM4R2 (IO_VIRT + 0x348C8)
    #define TIM4R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM4R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM4R3 (IO_VIRT + 0x348CC)
    #define TIM4R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM4R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM4R4 (IO_VIRT + 0x348D0)
    #define TIM4R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM4R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM4R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM4R5 (IO_VIRT + 0x348D4)
    #define TIM4R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM4R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM4R6 (IO_VIRT + 0x348D8)
    #define TIM4R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM4R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM4R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM4R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM4R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM4R7 (IO_VIRT + 0x348DC)
    #define TIM4R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM4R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM4R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM4R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM4R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM5R0 (IO_VIRT + 0x348E0)
    #define TIM5R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM5R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM5R1 (IO_VIRT + 0x348E4)
    #define TIM5R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM5R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM5R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM5R2 (IO_VIRT + 0x348E8)
    #define TIM5R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM5R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM5R3 (IO_VIRT + 0x348EC)
    #define TIM5R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM5R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM5R4 (IO_VIRT + 0x348F0)
    #define TIM5R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM5R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM5R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM5R5 (IO_VIRT + 0x348F4)
    #define TIM5R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM5R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM5R6 (IO_VIRT + 0x348F8)
    #define TIM5R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM5R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM5R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM5R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM5R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM5R7 (IO_VIRT + 0x348FC)
    #define TIM5R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM5R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM5R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM5R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM5R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM6R0 (IO_VIRT + 0x34900)
    #define TIM6R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM6R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM6R1 (IO_VIRT + 0x34904)
    #define TIM6R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM6R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM6R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM6R2 (IO_VIRT + 0x34908)
    #define TIM6R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM6R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM6R3 (IO_VIRT + 0x3490C)
    #define TIM6R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM6R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM6R4 (IO_VIRT + 0x34910)
    #define TIM6R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM6R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM6R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM6R5 (IO_VIRT + 0x34914)
    #define TIM6R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM6R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM6R6 (IO_VIRT + 0x34918)
    #define TIM6R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM6R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM6R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM6R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM6R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM6R7 (IO_VIRT + 0x3491C)
    #define TIM6R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM6R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM6R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM6R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM6R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM7R0 (IO_VIRT + 0x34920)
    #define TIM7R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM7R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM7R1 (IO_VIRT + 0x34924)
    #define TIM7R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM7R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM7R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM7R2 (IO_VIRT + 0x34928)
    #define TIM7R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM7R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM7R3 (IO_VIRT + 0x3492C)
    #define TIM7R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM7R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM7R4 (IO_VIRT + 0x34930)
    #define TIM7R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM7R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM7R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM7R5 (IO_VIRT + 0x34934)
    #define TIM7R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM7R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM7R6 (IO_VIRT + 0x34938)
    #define TIM7R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM7R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM7R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM7R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM7R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM7R7 (IO_VIRT + 0x3493C)
    #define TIM7R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM7R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM7R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM7R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM7R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM8R0 (IO_VIRT + 0x34940)
    #define TIM8R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM8R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM8R1 (IO_VIRT + 0x34944)
    #define TIM8R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM8R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM8R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM8R2 (IO_VIRT + 0x34948)
    #define TIM8R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM8R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM8R3 (IO_VIRT + 0x3494C)
    #define TIM8R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM8R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM8R4 (IO_VIRT + 0x34950)
    #define TIM8R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM8R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM8R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM8R5 (IO_VIRT + 0x34954)
    #define TIM8R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM8R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM8R6 (IO_VIRT + 0x34958)
    #define TIM8R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM8R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM8R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM8R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM8R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM8R7 (IO_VIRT + 0x3495C)
    #define TIM8R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM8R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM8R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM8R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM8R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIM9R0 (IO_VIRT + 0x34960)
    #define TIM9R0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM9R0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIM9R1 (IO_VIRT + 0x34964)
    #define TIM9R1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM9R1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM9R1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM9R2 (IO_VIRT + 0x34968)
    #define TIM9R2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIM9R2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIM9R3 (IO_VIRT + 0x3496C)
    #define TIM9R3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM9R3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIM9R4 (IO_VIRT + 0x34970)
    #define TIM9R4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIM9R4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIM9R4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIM9R5 (IO_VIRT + 0x34974)
    #define TIM9R5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIM9R5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIM9R6 (IO_VIRT + 0x34978)
    #define TIM9R6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIM9R6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIM9R6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIM9R6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIM9R6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIM9R7 (IO_VIRT + 0x3497C)
    #define TIM9R7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIM9R7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIM9R7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIM9R7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIM9R7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIMAR0 (IO_VIRT + 0x34980)
    #define TIMAR0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMAR0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMAR1 (IO_VIRT + 0x34984)
    #define TIMAR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMAR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMAR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMAR2 (IO_VIRT + 0x34988)
    #define TIMAR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMAR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMAR3 (IO_VIRT + 0x3498C)
    #define TIMAR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMAR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMAR4 (IO_VIRT + 0x34990)
    #define TIMAR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMAR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMAR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMAR5 (IO_VIRT + 0x34994)
    #define TIMAR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMAR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMAR6 (IO_VIRT + 0x34348)
    #define TIMAR6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMAR6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMAR6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMAR6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMAR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMAR7 (IO_VIRT + 0x3499C)
    #define TIMAR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMAR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMAR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMAR7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIMAR7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIMBR0 (IO_VIRT + 0x349A0)
    #define TIMBR0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMBR0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMBR1 (IO_VIRT + 0x349A4)
    #define TIMBR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMBR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMBR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMBR2 (IO_VIRT + 0x349A8)
    #define TIMBR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMBR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMBR3 (IO_VIRT + 0x349AC)
    #define TIMBR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMBR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMBR4 (IO_VIRT + 0x349B0)
    #define TIMBR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMBR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMBR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMBR5 (IO_VIRT + 0x349B4)
    #define TIMBR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMBR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMBR6 (IO_VIRT + 0x349B8)
    #define TIMBR6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMBR6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMBR6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMBR6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMBR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMBR7 (IO_VIRT + 0x349BC)
    #define TIMBR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMBR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMBR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMBR7_FTF Fld(14,14,AC_MSKDW)//[27:14]
    #define TIMBR7_FTR Fld(14,0,AC_MSKW10)//[13:0]

#define TIMCR0 (IO_VIRT + 0x349C0)
    #define TIMCR0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMCR0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMCR1 (IO_VIRT + 0x349C4)
    #define TIMCR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMCR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMCR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMCR2 (IO_VIRT + 0x349C8)
    #define TIMCR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMCR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMCR3 (IO_VIRT + 0x349CC)
    #define TIMCR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMCR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMCR4 (IO_VIRT + 0x349D0)
    #define TIMCR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMCR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMCR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMCR5 (IO_VIRT + 0x349D4)
    #define TIMCR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMCR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMCR6 (IO_VIRT + 0x349D8)
    #define TIMCR6_TC_CMD Fld(16,16,AC_FULLW32)//[31:16]
    #define TIMCR6_TC_CMD_ABNF Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMCR6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMCR6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMCR6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMCR6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMCR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMCR7 (IO_VIRT + 0x349DC)
    #define TIMCR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMCR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMCR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMCR7_FT_CNT Fld(4,24,AC_MSKB3)//[27:24]
    #define TIMCR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMCR7_FTR Fld(11,0,AC_MSKW10)//[10:0]

#define TIMDR0 (IO_VIRT + 0x349E0)
    #define TIMDR0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMDR0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMDR1 (IO_VIRT + 0x349E4)
    #define TIMDR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMDR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMDR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMDR2 (IO_VIRT + 0x349E8)
    #define TIMDR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMDR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMDR3 (IO_VIRT + 0x349EC)
    #define TIMDR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMDR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMDR4 (IO_VIRT + 0x349F0)
    #define TIMDR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMDR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMDR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMDR5 (IO_VIRT + 0x349F4)
    #define TIMDR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMDR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMDR6 (IO_VIRT + 0x349F8)
    #define TIMDR6_TC_CMD Fld(16,16,AC_FULLW32)//[31:16]
    #define TIMDR6_TC_CMD_ABNF Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMDR6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMDR6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMDR6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMDR6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMDR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMDR7 (IO_VIRT + 0x349FC)
    #define TIMDR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMDR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMDR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMDR7_FT_CNT Fld(4,24,AC_MSKB3)//[27:24]
    #define TIMDR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMDR7_FTR Fld(11,0,AC_MSKW10)//[10:0]

#define TIMER0 (IO_VIRT + 0x34A00)
    #define TIMER0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMER0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMER1 (IO_VIRT + 0x34A04)
    #define TIMER1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMER1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMER1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMER2 (IO_VIRT + 0x34A08)
    #define TIMER2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMER2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMER3 (IO_VIRT + 0x34A0C)
    #define TIMER3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMER3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMER4 (IO_VIRT + 0x34A10)
    #define TIMER4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMER4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMER4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMER5 (IO_VIRT + 0x34A14)
    #define TIMER5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMER5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMER6 (IO_VIRT + 0x34A18)
    #define TIMER6_TC_CMD Fld(16,16,AC_FULLW32)//[31:16]
    #define TIMER6_TC_CMD_ABNF Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMER6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMER6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMER6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMER6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMER6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMER7 (IO_VIRT + 0x34A1C)
    #define TIMER7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMER7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMER7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMER7_FT_CNT Fld(4,24,AC_MSKB3)//[27:24]
    #define TIMER7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMER7_FTR Fld(11,0,AC_MSKW10)//[10:0]
    
#define TIMFR0 (IO_VIRT + 0x34A20)
    #define TIMFR0_HE Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMFR0_HS Fld(13,0,AC_MSKW10)//[12:0]
#define TIMFR1 (IO_VIRT + 0x34A24)
    #define TIMFR1_HMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR1_HMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMFR1_HMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR1_HMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMFR1_HMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMFR2 (IO_VIRT + 0x34A28)
    #define TIMFR2_HTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR2_HTF Fld(13,16,AC_MSKW32)//[28:16]
    #define TIMFR2_HTR Fld(13,0,AC_MSKW10)//[12:0]
#define TIMFR3 (IO_VIRT + 0x34A2C)
    #define TIMFR3_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMFR3_VS Fld(12,0,AC_MSKW10)//[11:0]
#define TIMFR4 (IO_VIRT + 0x34A30)
    #define TIMFR4_VMRSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR4_VMINV Fld(1,30,AC_MSKB3)//[30:30]
    #define TIMFR4_VMNOTGL Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR4_VMLCNT Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMFR4_VMHCNT Fld(8,0,AC_FULLB0)//[7:0]
#define TIMFR5 (IO_VIRT + 0x34A34)
    #define TIMFR5_VTCSEL Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR5_VTF Fld(12,16,AC_MSKW32)//[27:16]
    #define TIMFR5_VTR Fld(12,0,AC_MSKW10)//[11:0]
#define TIMFR6 (IO_VIRT + 0x34A38)
    #define TIMFR6_TC_CMD Fld(16,16,AC_FULLW32)//[31:16]
    #define TIMFR6_TC_CMD_ABNF Fld(8,8,AC_FULLB1)//[15:8]
    #define TIMFR6_TCINV Fld(1,7,AC_MSKB0)//[7:7]
    #define TIMFR6_LOS_EN Fld(1,6,AC_MSKB0)//[6:6]
    #define TIMFR6_POL_EN Fld(1,5,AC_MSKB0)//[5:5]
    #define TIMFR6_H_CONT_EN Fld(1,4,AC_MSKB0)//[4:4]
    #define TIMFR6_TCOPR Fld(3,0,AC_MSKB0)//[2:0]
#define TIMFR7 (IO_VIRT + 0x34A3C)
    #define TIMFR7_FTEN Fld(1,31,AC_MSKB3)//[31:31]
    #define TIMFR7_FPOSTTGLEN Fld(1,29,AC_MSKB3)//[29:29]
    #define TIMFR7_FPRETGLEN Fld(1,28,AC_MSKB3)//[28:28]
    #define TIMFR7_FT_CNT Fld(4,24,AC_MSKB3)//[27:24]
    #define TIMFR7_FTF Fld(11,12,AC_MSKW21)//[22:12]
    #define TIMFR7_FTR Fld(11,0,AC_MSKW10)//[10:0]

#define CRCR0 (IO_VIRT + 0x34A40)
    #define CRC_FCNT Fld(4,16,AC_MSKB2)//[19:16]
    #define CRC_ENABLE Fld(16,0,AC_FULLW10)//[15:0]
    
#define CRCR1 (IO_VIRT + 0x34A44)
    #define CRC_VE Fld(12,16,AC_MSKW32)//[27:16]
    #define CRC_VS Fld(12,0,AC_MSKW10)//[11:0]
    
#define CRCR2 (IO_VIRT + 0x34A48)
    #define CRC_RESULT Fld(32,0,AC_FULLDW)//[31:0]

#endif
