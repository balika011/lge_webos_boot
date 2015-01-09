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
 * $RCSfile: vdp_hw.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_hw.h
 *  Brief of file vdp_hw.h.
 *  Details of file vdp_hw.h (optional).
 */

#ifndef VDP_HW_H
#define VDP_HW_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "x_typedef.h"
#include "x_hal_io.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define HAL_5371_B2R_MISC_LOCAL_REG         (0x300)
#define B2R_MISC_PCR_FIX_EN             (BIT0)
#define B2R_MISC_B2R_HD_RST             (BIT1)
#define B2R_MISC_PSW_HD_RST			    (BIT2)
#define B2R_MISC_PSW_SD_RST			    (BIT3)
#define B2R_MISC_27M_RST			    (BIT4)
#define B2R_MISC_DRAM_RST			    (BIT5)
#define B2R_MISC_PSW_HD_PIP			    (BIT6)
#define B2R_MISC_PSW_FB_SEL			    (BIT7)
#define B2R_MISC_RESERVED               (0xFFFFFF00)

#define HAL_5371_B2R_PCR_CTRL_LOCAL_REG     (0x308)

#define B2R_REG_NUM                         (0x44 >> 2)
#define B2R_SD_PATH_ADDR_OFFSET             (0x100)

#define B2R_REG_RHYS                        0x000       // B2R Y STARTING ADDRESS
#define B2R_REG_RHCS                        0x004       // B2R C STARTING ADDRESS
#define B2R_REG_RMAP                        0x008       // B2R HD range map
#define B2R_REG_PORCH                       0x00C       // B2R HD PORCH
#define B2R_REG_PGGC                        0x010       // PATTERN GEN GRID CONTROL
#define B2R_REG_PGCC                        0x014       // PATTERN GEN COLOR CONTROL
#define B2R_REG_HFC                         0x018       // FIXED COLOR
#define B2R_REG_HACT                        0x01C       // ACTIVE WIDTH AND HEIGHT
#define B2R_REG_HHVT                        0x020       // H/V TOTAL
#define B2R_REG_HDER                        0x024       // DATA ENABLE RATIO
#define B2R_REG_HPH                         0x028       // HORIZONTAL PITCH
#define B2R_REG_HMODE                       0x02C       // MODE CONTROL
#define B2R_REG_HITRG                       0x030       // INITIAL VALUE SETTING TRIGGER
#define B2R_REG_ROWC                        0x034       // ROW CONTROL
#define B2R_REG_BRHEN                       0x038       // BR MODE ENABLE CONTROL
#define B2R_REG_HSTA                        0x03C       // STATUS
#define B2R_REG_RHEN                        0x040       // ENABLE CONTROL
#define B2R_REG_OFFSET                      0x048       // HD frame offset
#define B2R_REG_YS_R_VIEW                   0x050       // HD R view Y addr
#define B2R_REG_CS_R_VIEW                   0x054       // HD R view C addr

#define B2R_REG_SRST                        0x300       // SOFT RESET
#define B2R_REG_PCRERR                      0x304       // PCR ERROR
#define B2R_REG_RPLL                        0x308       // B2R HD IDEAL CLOCK
#define B2R_REG_PCRT                        0x30C       // PCR RECOVERY RESYNC TRIGGER
#define B2R_REG_PIXEL_CRC                   0x310       // Pixel CRC value
#define B2R_REG_DRAM_CHKSUM                 0x314       // DRAM check sum
#define B2R_REG_SDPCRERR                    0x328

// 0x008
#define B2R_REG_RMAP_REP_CNT_M              0x00FF0000
#define B2R_REG_RMAP_REP_CNT_S              16
#define B2R_REG_RMAP_REP_FOREVER_M          0x01000000
#define B2R_REG_RMAP_REP_FORRVER_S          24
#define B2R_REG_RMAP_REP_SHIFT_M            0x02000000
#define B2R_REG_RMAP_REP_SHIFT_S            25
#define B2R_REG_RMAP_REP_NEW_MODE_M         0x04000000
#define B2R_REG_RMAP_REP_NEW_MODE_S         26

//0x00c
#define B2R_REG_PORCH_TAG_MODE_M            0xC0000000
#define B2R_REG_PORCH_TAG_MODE_S            30
#define B2R_REG_PORCH_TAG_LINE_NUM_M        0x20000000
#define B2R_REG_PORCH_TAG_LINE_NUM_S        29
#define B2R_REG_PORCH_TAG_TAG_POS_M         0x10000000
#define B2R_REG_PORCH_TAG_TAG_POS_S         28

// 0x018
#define B2R_REG_HFC_YCBCR_M                 0x00FFFFFF
#define B2R_REG_HFC_YCBCR_S                 0
#define B2R_REG_HFC_EN_M                    0x01000000
#define B2R_REG_HFC_EN_S                    24
#define B2R_REG_HFC_GXYM_M                  0x0E000000
#define B2R_REG_HFC_GXYM_S                  25

// 0x01C
#ifdef CC_MT5399
#define B2R_REG_HACT_RHAW_M                 0x00000FFF
#define B2R_REG_HACT_RHAW_S                 0
#define B2R_REG_HACT_HAH_M                  0xFFF00000
#define B2R_REG_HACT_HAH_S                  20
#else
#define B2R_REG_HACT_RHAW_M                 0x000007FF
#define B2R_REG_HACT_RHAW_S                 0
#define B2R_REG_HACT_HAH_M                  0x7FF00000
#define B2R_REG_HACT_HAH_S                  20
#endif

// 0x020
#define B2R_REG_HHVT_HHT_M                  0x00001FFF
#define B2R_REG_HHVT_HHT_S                  0
#ifdef CC_MT5399
#define B2R_REG_HHVT_HVT_M                  0x0FFF0000
#else
#define B2R_REG_HHVT_HVT_M                  0x07FF0000
#endif
#define B2R_REG_HHVT_HVT_S                  16
#define B2R_REG_HHVT_HTBD_M                 0x30000000
#define B2R_REG_HHVT_HTBD_S                 28
#define B2R_REG_HHVT_HBTD_M                 0xC0000000
#define B2R_REG_HHVT_HBTD_S                 30

// 0x024
#define B2R_REG_HDER_HINTD_M                0x000000FF
#define B2R_REG_HDER_HINTD_S                0
#define B2R_REG_HDER_M                      0x0003FF00
#define B2R_REG_HDER_S                      8

// 0x28
#ifdef CC_MT5399
#define B2R_REG_HPH_RHPH_M                  0x100000FF
#else
#define B2R_REG_HPH_RHPH_M                  0x000000FF
#endif
#define B2R_REG_HPH_RHPH_S                  0
#define B2R_REG_HPH_RHFS_M                  0x00000300
#define B2R_REG_HPH_RHFS_S                  8

// 0x2C
#define B2R_REG_HMODE_RHRQTH_M              0x00000003
#define B2R_REG_HMODE_RHRQTH_S              0
#define B2R_REG_HMODE_HBFST_M               0x00000010
#define B2R_REG_HMODE_HBFST_S               4
//#define B2R_REG_HMODE_HCRPT_M               0x00000020
//#define B2R_REG_HMODE_HCRPT_S               5
//#define B2R_REG_HMODE_RHFAUT_M              0x00000040
//#define B2R_REG_HMODE_RHFAUT_S              6
#define B2R_REG_HMODE_LEFT_VT_DEC_M         0x00000020
#define B2R_REG_HMODE_LEFT_VT_DEC_S         5
#define B2R_REG_HMODE_LEFT_VT_DEC_EN_M      0x00000040
#define B2R_REG_HMODE_LEFT_VT_DEC_EN_S      6
#define B2R_REG_HMODE_RHFINI_M              0x00000080
#define B2R_REG_HMODE_RHFINI_S              7
#define B2R_REG_HMODE_FLIP_M                0x00000100
#define B2R_REG_HMODE_FLIP_S                8
#define B2R_REG_HMODE_MIRROR_M              0x00000200
#define B2R_REG_HMODE_MIRROR_S              9
#define B2R_REG_HMODE_H422_M                0x00000800
#define B2R_REG_HMODE_H422_S                11
#define B2R_REG_HMODE_RHI_M                 0x00001000
#define B2R_REG_HMODE_RHI_S                 12
#define B2R_REG_HMODE_HVTC_M                0x00002000
#define B2R_REG_HMODE_HVTC_S                13
#define B2R_REG_HMODE_RHCIS_M               0x00004000
#define B2R_REG_HMODE_RHCIS_S               14
#define B2R_REG_HMODE_FMON_M                0x00008000
#define B2R_REG_HMODE_FMON_S                15
//#define B2R_REG_HMODE_HTBM_M                0x00010000
//#define B2R_REG_HMODE_HTBM_S                16
//#define B2R_REG_HMODE_HBTM_M                0x00020000
//#define B2R_REG_HMODE_HBTM_S                17
#define B2R_REG_HMODE_HFI_M                 0x00040000
#define B2R_REG_HMODE_HFI_S                 18
#define B2R_REG_HMODE_3D_INTERLACE_M        0x00080000
#define B2R_REG_HMODE_3D_INTERLACE_S        19
#define B2R_REG_HMODE_LINEAR_M              0x00100000
#define B2R_REG_HMODE_LINEAR_S              20
#define B2R_REG_HMODE_BURST_M               0x0800000
#define B2R_REG_HMODE_BURST_S               23
#define B2R_REG_HMODE_ADDR_M                0x03000000
#define B2R_REG_HMODE_ADDR_S                24
#define B2R_REG_HMODE_TWO_ADDR_EN_M         0x04000000
#define B2R_REG_HMODE_TWO_ADDR_EN_S         26
#define B2R_REG_HMODE_DISP_R_VIEW_INV_M     0x08000000
#define B2R_REG_HMODE_DISP_R_VIEW_INV_S     27
#define B2R_REG_HMODE_DISP_R_VIEW_M         0x10000000
#define B2R_REG_HMODE_DISP_R_VIEW_S         28
#define B2R_REG_HMODE_DISP_R_VIEW_MODE_M    0x60000000
#define B2R_REG_HMODE_DISP_R_VIEW_MODE_S    29
#define B2R_REG_HMODE_AUTO_OFFSET_M         0x80000000
#define B2R_REG_HMODE_AUTO_OFFSET_S         31


// 0x30
#define B2R_REG_HITRG_RHFT_M                0x00000001
#define B2R_REG_HITRG_RHFT_S                0
#define B2R_REG_HITRG_RHTT_M                0x00000004
#define B2R_REG_HITRG_RHTT_S                2
#define B2R_REG_HITRG_REAP_SET_M            0x00000008
#define B2R_REG_HITRG_REAP_SET_S            3

// 0x34
#define B2R_REG_ROWC_MAXR_M                 0x0000007F
#define B2R_REG_ROWC_MAXR_S                 0
#define B2R_REG_ROWC_CRM_M                  0x00000080
#define B2R_REG_ROWC_CRM_S                  7
#define B2R_REG_ROWC_FLD_M                  0x00000200
#define B2R_REG_ROWC_FLD_S                  9
#define B2R_REG_ROWC_FIE_M                  0x00000400
#define B2R_REG_ROWC_FIE_S                  10
#define B2R_REG_ROWC_LPE_M                  0x00000800
#define B2R_REG_ROWC_LPE_S                  11
#define B2R_REG_ROWC_YCPT_M                 0x00004000
#define B2R_REG_ROWC_YCPT_S                 14
#define B2R_REG_ROWC_CCPT_M                 0x00008000
#define B2R_REG_ROWC_CCPT_S                 15
#define B2R_REG_ROWC_LSTR_M                 0x007F0000
#define B2R_REG_ROWC_LSTR_S                 16
#define B2R_REG_ROWC_LNP_M                  0x00800000
#define B2R_REG_ROWC_LNP_S                  23

// 0x38
#define B2R_REG_BRHEN_BRPE_M                0x00000001
#define B2R_REG_BRHEN_BRPE_S                0
#define B2R_REG_BRHEN_BRWP_M                0x00000002
#define B2R_REG_BRHEN_BRWP_S                1

// 0x3C
#define B2R_REG_HSTA_HHACT_M                0x00000001
#define B2R_REG_HSTA_HHACT_S                0
#define B2R_REG_HSTA_HVBLK_M                0x00000002
#define B2R_REG_HSTA_HVBLK_S                1
#define B2R_REG_HSTA_HVFLD_M                0x00000004
#define B2R_REG_HSTA_HVFLD_S                2
#define B2R_REG_HSTA_RHFUDF_M               0x00000008
#define B2R_REG_HSTA_RHFUDF_S               3
#define B2R_REG_HSTA_VEIF_M                 0x00000020
#define B2R_REG_HSTA_VEIF_S                 5
#define B2R_REG_HSTA_ADIF_M                 0x00000040
#define B2R_REG_HSTA_ADIF_S                 6
#define B2R_REG_HSTA_INT_STATUS_M           0x00000380
#define B2R_REG_HSTA_INT_STATUS_S           7
#define B2R_REG_HSTA_LR_STATUS_M            0x00000800
#define B2R_REG_HSTA_LR_STATUS_S            11
#define B2R_REG_HD_DISP_R_ADJI_M            0x00001000
#define B2R_REG_HD_DISP_R_ADJI_S            12
#define B2R_REG_HSTA_HD_FLD_ADJ_M           0x00004000
#define B2R_REG_HSTA_HD_FLD_ADJ_S           14
#define B2R_REG_HSTA_HTGL_M                 0x07FF0000
#define B2R_REG_HSTA_HTGL_S                 16

// 0x40
#define B2R_REG_RHEN_M                      0x00000001
#define B2R_REG_RHEN_S                      0
#define B2R_REG_RHEN_VPTE_M                 0x00000004
#define B2R_REG_RHEN_VPTE_S                 2
#define B2R_REG_RHEN_ADIE_M                 0x00000010
#define B2R_REG_RHEN_ADIE_S                 4
#define B2R_REG_RHEN_VEIE_M                 0x00000020
#define B2R_REG_RHEN_VEIE_S                 5

// 0x48
#define B2R_REG_HOFFSET_WIDTH_M             0x000007FC
#define B2R_REG_HOFFSET_WIDTH_S             0
#define B2R_REG_HOFFSET_HEIGHT_M            0x07FE0000
#define B2R_REG_HOFFSET_HEIGHT_S            16

//0x05C


// 0x300
#define B2R_REG_SRST_PCREN_M                0x00000001
#define B2R_REG_SRST_PCREN_S                0
#define B2R_REG_SRST_HDRST_M                0x00000002
#define B2R_REG_SRST_HDRST_S                1
#define B2R_REG_SRST_SDRST_M                0x00000004
#define B2R_REG_SRST_SDRST_S                2
#define B2R_REG_SRST_SDPCREN_M              0x00000008
#define B2R_REG_SRST_SDPCREN_S              3
#define B2R_REG_SRST_F27RST_M               0x00000010
#define B2R_REG_SRST_F27RST_S               4
#define B2R_REG_SRST_DMRST_M                0x00000020
#define B2R_REG_SRST_DMRST_S                5
#define B2R_REG_SRST_DMEN_M                 0x00000080
#define B2R_REG_SRST_DMEN_S                 7
#define B2R_REG_RR_HDY_RREN_M               0x00000100
#define B2R_REG_RR_HDY_RREN_S               8
#define B2R_REG_RR_HDY_RRTP_M               0x00000200
#define B2R_REG_RR_HDY_RRTP_S               9
#define B2R_REG_RR_HDC_RREN_M               0x00000400
#define B2R_REG_RR_HDC_RREN_S               10
#define B2R_REG_RR_HDC_RRTP_M               0x00000800
#define B2R_REG_RR_HDC_RRTP_S               11
#define B2R_REG_RR_BYPASS_M                 0x00010000
#define B2R_REG_RR_BYPASS_S                 16
#define B2R_REG_RR_NEWMODEY_M               0x01000000
#define B2R_REG_RR_NEWMODEY_S               24
#define B2R_REG_RR_CACHEY_M                 0x02000000
#define B2R_REG_RR_CACHEY_S                 25
#define B2R_REG_RR_NEWMODEC_M               0x04000000
#define B2R_REG_RR_NEWMODEC_S               26
#define B2R_REG_RR_CACHEC_M                 0x08000000
#define B2R_REG_RR_CACHEC_S                 27


// 0x304
#define B2R_REG_PCRERR_M                    0x000000FF
#define B2R_REG_PCRERR_S                    0
#define B2R_REG_PCRERR_PCRS_M               0x00000100
#define B2R_REG_PCRERR_PCRS_S               8

// 0x308
#define B2R_REG_RPLL_PLLM_M                 0x000003FF
#define B2R_REG_RPLL_PLLM_S                 0
#define B2R_REG_RPLL_PLLN_M                 0x03FF0000
#define B2R_REG_RPLL_PLLN_S                 16

// 0x30C
#define B2R_REG_PCRT_M                      0x00000001
#define B2R_REG_PCRT_S                      0
#define B2R_REG_DRAMC_M                     0x00000002
#define B2R_REG_DRAMC_S                     1
#define B2R_REG_YCCRC_M                     0x00000004
#define B2R_REG_YCCRC_S                     2
#define B2R_REG_HDISR_M                     0x00000010
#define B2R_REG_HDISR_S                     4
#define B2R_REG_SDISR_M                     0x00000020
#define B2R_REG_SDISR_S                     5
#define B2R_REG_DRAM_CRC_EN_M               0x00000002
#define B2R_REG_DRAM_CRC_EN_S               1
#define B2R_REG_YC_CRC_EN_M                 0x00000004
#define B2R_REG_YC_CRC_EN_S                 2

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#ifdef CC_SECOND_B2R_SUPPORT
#define B2R_REG_R(reg)                      ((ucB2rId==B2R_1 || reg>=0x300) ? IO_READ32(BLK2RS_BASE, reg) : IO_READ32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET))
#define B2R_REG_W(reg, val)                 ((ucB2rId==B2R_1 || reg>=0x300) ? IO_WRITE32(BLK2RS_BASE, reg, val) : IO_WRITE32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET, val))
#define B2R_REG_R_M(reg, mask, shift)       ((ucB2rId==B2R_1 || reg>=0x300) ? ((IO_READ32(BLK2RS_BASE, reg) & mask) >> shift) : ((IO_READ32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET) & mask) >> shift))
#define B2R_REG_W_M(reg, val, mask, shift)  ((ucB2rId==B2R_1 || reg>=0x300) ? (IO_WRITE32(BLK2RS_BASE,reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | ( val << shift)))) : (IO_WRITE32(BLK2RS_BASE,reg+B2R_SD_PATH_ADDR_OFFSET, ((IO_READ32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET) & (~mask)) | ( val << shift)))))
#define B2R_REG_R_B(reg, mask)              ((ucB2rId==B2R_1 || reg>=0x300) ? (IO_READ32(BLK2RS_BASE, reg) & mask) : (IO_READ32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET) & mask))
#define B2R_REG_W_B(reg, val, mask)         ((ucB2rId==B2R_1 || reg>=0x300) ? (IO_WRITE32(BLK2RS_BASE,reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | (val)))) : (IO_WRITE32(BLK2RS_BASE,reg+B2R_SD_PATH_ADDR_OFFSET, ((IO_READ32(BLK2RS_BASE, reg+B2R_SD_PATH_ADDR_OFFSET) & (~mask)) | (val)))))
#else
#define B2R_REG_R(reg)                      (IO_READ32(BLK2RS_BASE, reg))
#define B2R_REG_W(reg, val)                 (IO_WRITE32(BLK2RS_BASE, reg, val))
#define B2R_REG_R_M(reg, mask, shift)       ((IO_READ32(BLK2RS_BASE, reg) & mask) >> shift)
#define B2R_REG_W_M(reg, val, mask, shift)       B2R_REG_W(reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | ( val << shift)))
#define B2R_REG_R_B(reg, mask)              (IO_READ32(BLK2RS_BASE, reg) & mask)
#define B2R_REG_W_B(reg, val, mask)         B2R_REG_W(reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | (val)))
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if 1
#define NB2R_H_PORCH_OFFSET                   28
#define NB2R_H_PORCH_BACK_WIDTH               148
#define NB2R_H_PORCH_SYNC_WIDTH               44
#define NB2R_H_PORCH_FRONT_WIDTH               88

#define NB2R_V_PORCH_OFFSET                   9
#define NB2R_V_PORCH_FRONT_WIDTH               8
#define NB2R_V_PORCH_BACK_WIDTH               72
#define NB2R_V_PORCH_SYNC_WIDTH               10

#define NB2R_REG_HACT                        0x81C       // [31:20] V active ;[11:0]H active
#define NB2R_REG_HTOTAL                      0x820       // [27:16] Vtotal  ;[12:0]H total 
#define NB2R_REG_H_FRONTPORCH                0x80c       // [12:0]  HD left porch 
#define NB2R_REG_H_BACKPORCH                 0x860       // [11:0] HD path H right porch
#define NB2R_REG_V_FRONTPORCH                0x814       // [31:20] HD path vertical up porch
#define NB2R_REG_V_BACKPORCH                 0x860       // [23:13] HD vertical down porch 
#define NB2R_REG_HS_INV                      0x82C       // [24] for Hsync;[25]for Vsync
#define NB2R_REG_UFO                         0x838       // ufo Enable
#define NB2R_REG_RHEN                        0x840       // ENABLE CONTROL

#define NB2R_REG_HACT_RHAW_M                 0x00000FFF
#define NB2R_REG_HACT_RHAW_S                 0
#define NB2R_REG_HACT_HAH_M                  0xFFF00000
#define NB2R_REG_HACT_HAH_S                  20
#define NB2R_REG_HHVT_HHT_M                  0x00001FFF
#define NB2R_REG_HHVT_HHT_S                  0
#define NB2R_REG_HHVT_HVT_M                  0x0FFF0000
#define NB2R_REG_HHVT_HVT_S                  16


#define NB2R_REG_H_FRONTPORCH_M                  0x00001FFF
#define NB2R_REG_H_FRONTPORCH_S                  0 

#define NB2R_REG_H_BACKPORCH_M                   0x00000FFF
#define NB2R_REG_H_BACKPORCH_S                   0

#define NB2R_REG_V_FRONTPORCH_M                  0xFFF00000
#define NB2R_REG_V_FRONTPORCH_S                  20

#define NB2R_REG_V_BACKPORCH_M                   0xFFE000
#define NB2R_REG_V_BACKPORCH_S                   13

#define NB2R_REG_HS_INV_H                0x1000000
#define NB2R_REG_HS_INV_H_S                24
#define NB2R_REG_HS_INV_V                 0x2000000
#define NB2R_REG_HS_INV_V_S                25  

#define NB2R_REG_UFO_M               0x8000000

#define NB2R_REG_UFO_MODE_10                   0x0E//10bit UFO 4K2K enable
#define NB2R_REG_UFO_MODE_8                    0x08//8bit UFO 4K2K enable

#define NB2R_REG_UFO_MODE_M                   0xF0000000
#define NB2R_REG_UFO_MODE_S                   28


//#define NB2R_REG_UFO_MODE_8                   0x00

#define NB2R_REG_W_B(reg, val, mask)        IO_WRITE32(BLK2RS_BASE,reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | (val))) 
#define NB2R_REG_W_B_BITS(RegName,FieldName,value) vIO32WriteFldAlign((BLK2RS_BASE+(RegName)) ,(value) ,(FieldName))

#define NB2R_REG_W_M(reg, val, mask, shift)  IO_WRITE32(BLK2RS_BASE,reg, ((IO_READ32(BLK2RS_BASE, reg) & (~mask)) | ( val << shift)))
#define NB2R_REG_R(reg)                      IO_READ32(BLK2RS_BASE, reg)
#define NB2R_REG_W(reg,value)                IO_WRITE32(BLK2RS_BASE, reg,value)
#define NB2R_REG_W_VB1(reg, val, mask)        IO_WRITE32(0xF0000000,reg, ((IO_READ32(0xF0000000, reg) & (~mask)) | (val))) 
#define NB2R_REG_VB1(reg,value)               IO_WRITE32(0xF0000000, reg,value)
#endif


#endif    // VDP_HW_H

