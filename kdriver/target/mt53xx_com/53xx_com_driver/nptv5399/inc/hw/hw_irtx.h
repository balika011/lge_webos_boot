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

#include"x_hal_5381.h"


#define IO_IRTX_BASE (IO_VIRT + 0x29000)


//Page IRTX
#if defined(CC_MT5399)
#define IRTX_IRTX1CFG (IO_IRTX_BASE + 0x600)
    #define FLD_IRTX1_OPEN_DR Fld(1,16,AC_MSKB2)//[16:16]
    #define FLD_IRTX1_DATA_INV Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_IRTX1_DELAY_TYPE Fld(1,14,AC_MSKB1)//[14:14]
    #define FLD_IRTX1_IROS Fld(1,13,AC_MSKB1)//[13:13]
    #define FLD_IRTX1_BITNUM Fld(5,8,AC_MSKB1)//[12:8]
    #define FLD_IRTX1_IRINV Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_IRTX1_EXT_MODE Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_IRTX1_HW_MODE Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_IRTX1_IGNORE_LEFT Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_IRTX1_IGNORE_RIGHT Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_IRTX1_L0H_VALUE Fld(1,2,AC_MSKB0)//[2:2]
    #define FLD_IRTX1_L1L_VALUE Fld(1,1,AC_MSKB0)//[1:1]
    #define FLD_IRTX1_STRT Fld(1,0,AC_MSKB0)//[0:0]
#define IRTX_IRTX1D (IO_IRTX_BASE + 0x604)
    #define FLD_IRTX1_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1_L0H (IO_IRTX_BASE + 0x608)
    #define FLD_IRTX1_L0H Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1_L0L (IO_IRTX_BASE + 0x60C)
    #define FLD_IRTX1_L0L Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1_L1H (IO_IRTX_BASE + 0x610)
    #define FLD_IRTX1_L1H Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1_L1L (IO_IRTX_BASE + 0x614)
    #define FLD_IRTX1_L1L Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1_SYNCH (IO_IRTX_BASE + 0x618)
    #define FLD_IRTX1_SYNCH Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1_SYNCL (IO_IRTX_BASE + 0x61C)
    #define FLD_IRTX1_SYNCL Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1MT (IO_IRTX_BASE + 0x620)
    #define FLD_IRTX1_CDT Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_IRTX1_CWT Fld(16,0,AC_FULLW10)//[15:0]
#define IRTX_IRTX1LOD (IO_IRTX_BASE + 0x624)
    #define FLD_IRTX1_LO_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1LCD (IO_IRTX_BASE + 0x628)
    #define FLD_IRTX1_LC_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1ROD (IO_IRTX_BASE + 0x62C)
    #define FLD_IRTX1_RO_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1RCD (IO_IRTX_BASE + 0x630)
    #define FLD_IRTX1_RC_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1EXTL (IO_IRTX_BASE + 0x634)
    #define FLD_IRTX1_EXTL_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1EXTR (IO_IRTX_BASE + 0x638)
    #define FLD_IRTX1_EXTR_DATA Fld(32,0,AC_FULLDW)//[31:0]
#define IRTX_IRTX1SWD (IO_IRTX_BASE + 0x66C)
    #define FLD_IRTX1_SW_DELAY Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1HWLOD (IO_IRTX_BASE + 0x670)
    #define FLD_IRTX1_HW_LODELAY Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1HWLCD (IO_IRTX_BASE + 0x674)
    #define FLD_IRTX1_HW_LCDELAY Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1HWROD (IO_IRTX_BASE + 0x678)
    #define FLD_IRTX1_HW_RODELAY Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX1HWRCD (IO_IRTX_BASE + 0x67C)
    #define FLD_IRTX1_HW_RCDELAY Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2CFG (IO_IRTX_BASE + 0x640)
    #define FLD_DATA_INV Fld(1,15,AC_MSKB1)//[15:15]
    #define FLD_IRTX2_BITNUM Fld(7,8,AC_MSKB1)//[14:8]
    #define FLD_IRTX2_IRINV Fld(1,7,AC_MSKB0)//[7:7]
    #define FLD_IRTX2_IROS Fld(1,6,AC_MSKB0)//[6:6]
    #define FLD_IRTX2_RODR Fld(1,5,AC_MSKB0)//[5:5]
    #define FLD_IRTX2_BODR Fld(1,4,AC_MSKB0)//[4:4]
    #define FLD_IRTX2_SWO Fld(1,3,AC_MSKB0)//[3:3]
    #define FLD_IRTX2_MODE Fld(2,1,AC_MSKB0)//[2:1]
    #define FLD_IRTX2_STRT Fld(1,0,AC_MSKB0)//[0:0]
#define IRTX_IRTX2D0 (IO_IRTX_BASE + 0x644)
    #define FLD_IRTX2_R3 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_IRTX2_R2 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_IRTX2_R1 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_IRTX2_R0 Fld(8,0,AC_FULLB0)//[7:0]
#define IRTX_IRTX2D1 (IO_IRTX_BASE + 0x648)
    #define FLD_IRTX2_R7 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_IRTX2_R6 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_IRTX2_R5 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_IRTX2_R4 Fld(8,0,AC_FULLB0)//[7:0]
#define IRTX_IRTX2D2 (IO_IRTX_BASE + 0x64C)
    #define FLD_IRTX2_R11 Fld(8,24,AC_FULLB3)//[31:24]
    #define FLD_IRTX2_R10 Fld(8,16,AC_FULLB2)//[23:16]
    #define FLD_IRTX2_R9 Fld(8,8,AC_FULLB1)//[15:8]
    #define FLD_IRTX2_R8 Fld(8,0,AC_FULLB0)//[7:0]
#define IRTX_IRTX2_L0H (IO_IRTX_BASE + 0x650)
    #define FLD_IRTX2_L0H Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2_L0L (IO_IRTX_BASE + 0x654)
    #define FLD_IRTX2_L0L Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2_L1H (IO_IRTX_BASE + 0x658)
    #define FLD_IRTX2_L1H Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2_L1L (IO_IRTX_BASE + 0x65C)
    #define FLD_IRTX2_L1L Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2SYNCH (IO_IRTX_BASE + 0x660)
    #define FLD_IRTX2_SYNCH Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2SYNCL (IO_IRTX_BASE + 0x664)
    #define FLD_IRTX2_SYNCL Fld(24,0,AC_MSKDW)//[23:0]
#define IRTX_IRTX2MT (IO_IRTX_BASE + 0x668)
    #define FLD_IRTX2_CDT Fld(16,16,AC_FULLW32)//[31:16]
    #define FLD_IRTX2_CWT Fld(16,0,AC_FULLW10)//[15:0]
#endif

