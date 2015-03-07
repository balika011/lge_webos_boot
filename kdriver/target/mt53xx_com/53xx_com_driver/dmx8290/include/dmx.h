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
 * $Author: p4admin $
 * $Date: 2015/03/08 $
 * $RCSfile: dmx.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx.h
 *  Demux driver - common definitions
 */


#ifndef DMX_H
#define DMX_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "x_hal_5381.h"

#ifndef CC_SECURE_WORLD
#include "dmx_debug.h"
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

/// Enable PCR recovery
#define  __PCR_RECOVERY__



//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//
// SRAM base addresses
//

#define DMX_SECTION_PATTERN                     (DEMUX6_BASE + 0x0)
#define DMX_SECTION_MASK                        (DEMUX6_BASE + 0x400)
#define DMX_SECTION_POSNEG                      (DEMUX6_BASE + 0x800)
#define DMX_SECTION_CONTROL                     (DEMUX6_BASE + 0xC00)

#define DMX_SECTION_PATTERN2                    (DEMUX8_BASE + 0x0)
#define DMX_SECTION_MASK2                       (DEMUX8_BASE + 0x400)
#define DMX_SECTION_POSNEG2                     (DEMUX8_BASE + 0x800)
#define DMX_SECTION_CONTROL2                    (DEMUX8_BASE + 0xC00)

#define DMX_SECTION_PER_UNIT                    (96)

#if defined(CC_MT5890)
#define DMX_ONEBYTE_FILTER_BASE                 (DEMUX7_BASE + 0x174) //1C174
#else  //MT5882
#define DMX_ONEBYTE_FILTER_BASE                 (DEMUX7_BASE + 0x24) 
#endif

#if defined(CC_MT5890)
#define DMX_PATTERN_MATCH_BASE0                 (DEMUX7_BASE) //0x54000
#define DMX_PATTERN_MATCH_BASE1                 (DEMUX7_BASE + 0xA8) //0x540a8
#else //MT5882
#define DMX_PATTERN_MATCH_BASE0                 (DEMUX4_BASE + 0xC00) //1BC00
#define DMX_PATTERN_MATCH_BASE1                 (DEMUX4_BASE + 0xCA8) //1BCA8
#endif

#if defined(CC_MT5890)
#define DMX_DMEM_CA_KEY_BASE                    (DEMUX9_BASE + 0x80) //0x6A080
#define DMX_DMEM_CA_CTRL_BASE                   (DEMUX9_BASE ) //0x6A000
#else  //MT5882
#define DMX_DMEM_CA_KEY_BASE                    (DEMUX7_BASE + 0x724) 
#define DMX_DMEM_CA_CTRL_BASE                   (DEMUX7_BASE + 0x6E4) 
#endif

#define DMX_DMEM_MM_KEY_SIZE                    (18) //72 byte
#if defined(CC_MT5890)
#define DMX_DMEM_MM_KEY_BASE                    (DEMUX9_BASE + 0xE00) //6aE00
#define DMX_DMEM_MM_IV_BASE                     (DEMUX9_BASE + 0xE20) //6aE20
#define DMX_DMEM_MM_KEY_BASE1                   (DEMUX9_BASE + 0xE80) //6aE80
#define DMX_DMEM_MM_IV_BASE1                    (DEMUX9_BASE + 0xEA0) //6aEA0
#else  //MT5882
#define DMX_DMEM_MM_KEY_BASE                    (DEMUX7_BASE + 0xEA4) //54EA4
#define DMX_DMEM_MM_IV_BASE                     (DEMUX7_BASE + 0xEC4) //54EC4
#define DMX_DMEM_MM_KEY_BASE1                   (DEMUX7_BASE + 0xF24) //54F24
#define DMX_DMEM_MM_IV_BASE1                    (DEMUX7_BASE + 0xF44) //54F44
#endif

#define DMX_SECURE_PLAYBACK_PIDINDEX0           (*(volatile UINT32*)(DEMUX9_BASE + 0xF00))//6AF00
#define DMX_SECURE_PLAYBACK_PIDINDEX1           (*(volatile UINT32*)(DEMUX9_BASE + 0xF04))//6AF04
#define DMX_SECURE_PLAYBACK_PIDINDEX2           (*(volatile UINT32*)(DEMUX9_BASE + 0xF08))//6AF08
#define DMX_PIDBUFF_SECURE_CONTROL_BASE         (DEMUX9_BASE + 0xF00)

#define DMX_DMEM_MM_SECURE_BASE                 (DEMUX9_BASE + 0xF0C) //6aF0C
#define DMX_MM_SECURE_W(word)                   ((volatile UINT32*)(DMX_DMEM_MM_SECURE_BASE))[(word)]


#define IOMMU_DDI_BASE                          (IO_VIRT + 0x68280)

#define IOMMU_DMX_BASE                          (IO_VIRT + 0x68240)

//
// Base addresses
//
//
// Misc
//
#define DMX_INT_QUEUE_DEPTH                     64
#define DMX_ES_FIFO_ALIGNMENT                   64
#define DMX_HEADER_FIFO_ALIGNMENT               32
#define DMX_DMEM_ENTRY_LEN                      30  //120byte Playback PID data structure
#define DMX_DMEM_PATTERN_MATCH_LEN              42  //168byte Pattern match len
#define DMX_NULL_PIDX                           0xff
#define DMX_NULL_CHANNEL_ID                     0xff
#define DMX_PSI_BUF_TIMES                       1
#define SECTION_FILTER_ONE_APPENDDATA_SIZE      (9)  //64 bit one byte section filter match result + 1 byte serial number
#define SECTION_FILTER_APPENDDATA_SIZE          (0) //micro-code doesn't writer the bitmatch to dram.

// 8 KB - 128 bytes => To save memory allocation penalty that 8K will occupy 16KB DRAM
#define PES_HEADER_BUF_SIZE                     0x2000 - 0x80//0x1000

#define DMX_VNULL                               (120)

/* In CI+ verification, there is a high bit-rate stream test (stream replacement). The bitrate is much higher than original speed.
 * In MT5389, we found system could hang up if the total number of picture interrupt (Partial PES) is bigger than 150 per second.
 * To avoid high frequency interrupts, we start dropping B frames if picture interrupt is bigger than the threshold.
 * For two playback scenario on 60fps stream, the total number of picture interrupt could be bigger than 150 per second.
 * So, to the threshold is evaluated for each pidx
 */
#define DMX_PICTURE_INT_THRESHOLD               (120)

//
// Interrupt masks
//
#define DMX_INT_ERR_MASK                        0xffff
#define DMX_INT_ERR_DBM                         (1 << 0)
#define DMX_INT_ERR_DESCRAMBLER                 (1 << 1)
#define DMX_INT_ERR_PCR                         (1 << 2)
#define DMX_INT_ERR_RESERVED                    (1 << 3)
#define DMX_INT_ERR_STERRING                    (1 << 4)
#define DMX_INT_ERR_PES_FILTER                  (1 << 5)
#define DMX_INT_ERR_FTuP                        (1 << 6)
#define DMX_INT_ERR_MEM_ACCESS                  (1 << 7)
#define DMX_INT_ERR_PCR2                        (1 << 8)
#define DMX_INT_ERR_PVR                         (1 << 9)
#define DMX_INT_ERR_PLAYBACK_STERRING           (1 << 11)
#define DMX_INT_ERR_PLAYBACK_STERRING2          (1 << 12)
#define DMX_INT_ERR_PVR2                        (1 << 13)
#define DMX_INT_ERR_PLAYBACK_STERRING3          (1 << 14)
#define DMX_INT_ERR_PVR3                        (1<< 15)
#define DMX_INT_STATUS_MASK                     0xffff0000
#define DMX_INT_STATUS_DBM                      (1 << 16)
#define DMX_INT_STATUS_DESCRAMBLER              (1 << 17)
#define DMX_INT_STATUS_PCR                      (1 << 18)
#define DMX_INT_STATUS_PCR2                     (1 << 19)
#define DMX_INT_STATUS_STERRING                 (1 << 20)
#define DMX_INT_STATUS_PES_FILTER               (1 << 21)
#define DMX_INT_STATUS_FTuP                     (1 << 22)
#define DMX_INT_STATUS_MEM_ACCESS               (1 << 23)
#define DMX_INT_STATUS_RESERVED                 (1 << 24)
#define DMX_INT_STATUS_PVR                      (1 << 25)
#define DMX_INT_STATUS_PVR2                     (1 << 28)
#define DMX_INT_STATUS_PVR3                     (1 << 29)

#define DMX_INT_PCR_MASK                        (DMX_INT_ERR_PCR | \
                                                 DMX_INT_ERR_PCR2 | \
                                                 DMX_INT_STATUS_PCR | \
                                                 DMX_INT_STATUS_PCR2) // | DMX_INT_STATUS_RESERVED)
//
//Ckgen registers
//

#define CKGEN_BLOCK_CKENCFG1    0x1CC   //BLOCK CLOCK ENABLE CONFIGURATION1
#define CKGEN_TSCKCFG           0x238   //TS_CK_CONFIGURATION
#define CKGEN_DMX_CKCFG         0x314   //DMX_CONFIGURATION
#define CKGEN_REG_EXTTS_CKCKG   0x160   //EXTTS_CONFIGURATION
#define CKGEN_TSOUT_CKCFG       0x304   //TSOUT CONFIGURATION
#define CKGEN_TSOUT_CKCFG2      0x41C   //TSOUT CONFIGURATION2
#define CKGEN_TSOUT_CKCFG3      0x444   //TSOUT CONFIGURATION3
#define CKGEN_TSIF_CFG          0x158   //CI TSIF CONFIGURATION REGISTER
#define CKGEN_TSIF2_CFG         0x120   //CI TSIF2 CONFIGURATION REGISTER



//
// Iommu registers
//

#define REG_IOMMU_CFG0              0x0//0x000        // basic setting
#define REG_IOMMU_CFG1              0x1//0x004        // page table index
#define REG_IOMMU_CFG2              0x2//0x008        // agnet_0~1 setting
#define REG_IOMMU_CFG4              0x4//0x010        // interrupt, monitor and debug
#define REG_IOMMU_CFG5              0x5//0x014       // performance meter
#define REG_IOMMU_CFG6              0x6//0x018      //monitor result 0
#define REG_IOMMU_CFG7              0x7//0x01c        //monitor result 1
#define REG_IOMMU_CFG8              0x8//0x020        //monitor result 2
#define REG_IOMMU_CFG9              0x9//0x024         // overread 0
#define REG_IOMMU_CFGA              0xA//0x028         //overread 1
#define REG_IOMMU_CFGB              0xb//0x02C          //overread 2
#define REG_IOMMU_CFGC              0xc//0x030        //overread 3
#define REG_IOMMU_CFGD              0xd//0x034         //overread 4




//
// Demux registers
//
#define DMX_REG_CONTROL                         0       // 0x0
#define DMX_REG_CONFIG1                         1       // 0x4
#define DMX_REG_CONFIG2                         2       // 0x8
#define DMX_REG_MEM_CMD                         3       // 0xC
#define DMX_REG_MEM_DATA                        4       // 0x10
#define DMX_FTI_SECURITY_ACCESS                 5       //0x14
#define DMX_REG_REC_CONFIG1                     6       // 0x18
#define DMX_REG_REC_CONFIG2                     7       // 0x1C
#define DMX_REG_FRAMER_CONTROL                  8       // 0x20
#define DMX_REG_FRAMER_ERROR_HANDLE             9       // 0x24
#define DMX_REG_DBM_CONTROL                     10      // 0x28
#define DMX_REG_DBM_BYPASS_PID                  11      // 0x2C


#define DMX_REG_DBM_BYPASS_PID_2                12      // 0x30
#define DMX_REG_DBM_BYPASS_PID_3                13      // 0x34
#define DMX_REG_DBM_BYPASS_PID_4                14      // 0x38
#define DMX_REG_DBM_BUF_CTRL                    15      // 0x3C

#define DMX_REG_PES_CONTRL                      46      // 0xB8
#define DMX_REG_FTuP_CONTROL                    48      // 0xC0
#define DMX_REG_INT_STAUS                       64      // 0x100
#define DMX_REG_INT_MASK                        65      // 0x104
#define DMX_REG_CA_CTRL                         16      // 0x40
#define DMX_REG_CA_IN_BUF_THRESHOLD             17      // 0x44
#define DMX_REG_CA_IN_BUF_START                 19      // 0x4C
#define DMX_REG_CA_IN_BUF_END                   20      // 0x50
#define DMX_REG_CA_IN_BUF_RP                    21      // 0x54
#define DMX_REG_CA_IN_BUF_WP                    22      // 0x58
#define DMX_REG_CA_OUT_BUF_THRESHOLD            18      // 0x48
#define DMX_REG_CA_OUT_BUF_START                23      // 0x5C
#define DMX_REG_CA_OUT_BUF_END                  24      // 0x60
#define DMX_REG_CA_OUT_BUF_RP                   25      // 0x64
#define DMX_REG_CA_OUT_BUF_WP                   26      // 0x68
#define DMX_REG_TS0_LOCAL_TS_ID_VALID           27      // 0x6C
#define DMX_REG_TS1_LOCAL_TS_ID_VALID           28      // 0x70
#define DMX_REG_TS2_LOCAL_TS_ID_VALID           29      // 0x74
#define DMX_REG_TS3_LOCAL_TS_ID_VALID           30      // 0x78
#define DMX_REG_DBM_MULTI_STREAM_MODE           31      // 0x7C
#define DMX_REG_DBM_BYPASS_PID_5                32      // 0x80
#define DMX_REG_M2_CFG                          33      // 0x84
#define DMX_REG_FTUPCR                          48      // 0xC0
#define DMX_REG_PES_DBM_STEER_CTRL              50      // 0xC8
#define DMX_REG_FTuP_FULL_STATUS                52      // 0xD0
#define DMX_REG_PID_STRUCT_OFFSET               53      // 0xD4
#define DMX_REG_M2_SYS_KEY0                     54      // 0xD8
#define DMX_REG_DBM_ERROR_STATUS_REG            80      // 0x140
#define DMX_REG_DBM_NONERR_STATUS_REG           81      // 0x144
#define DMX_REG_DESCRAMBLER_ERROR_STATUS_REG    88      // 0x160
#define DMX_REG_DESCRAMBLER_NONERR_STATUS_REG   89      // 0x164
#define DMX_REG_STEER_ERROR_STATUS_REG          96      // 0x180
#define DMX_REG_STEER_NONERR_STATUS_REG         97      // 0x184
#define DMX_REG_PES_ERROR_STATUS_REG            104     // 0x1A0
#define DMX_REG_PES_NONERR_STATUS_REG1          105     // 0x1A4
#define DMX_REG_PES_NONERR_STATUS_REG2          106     // 0x1A8
#define DMX_REG_FTuP_ERROR_STATUS_REG           112     // 0x1C0
#define DMX_REG_FTuP_NONERR_STATUS_REG1         113     // 0x1C4
#define DMX_REG_FTuP_NONERR_STATUS_REG2         114     // 0x1C8
#define DMX_REG_DES_IV_LO_ODD                   118     // 0x1d8
#define DMX_REG_DES_IV_HI_ODD                   119     // 0x1dc
#define DMX_REG_AES_CTRL                        120
#define DMX_REG_AES_IV_0_EVEN                   121
#define DMX_REG_AES_IV_1_EVEN                   122
#define DMX_REG_AES_IV_2_EVEN                   123
#define DMX_REG_AES_IV_3_EVEN                   124
#define DMX_REG_AES_IV_0_ODD                    125
#define DMX_REG_AES_IV_1_ODD                    126
#define DMX_REG_AES_IV_2_ODD                    127
#define DMX_REG_AES_IV_3_ODD                    128
#define DMX_REG_PCR_NUM_CONTROL                 129
#define DMX_REG_PCR_CTRL                        130
#define DMX_REG_PCR_ERR_THRESHOLD               131
#define DMX_REG_PCR_FIR_FILTER_COEFF0           132
#define DMX_REG_PCR_FIR_FILTER_COEFF1           133
#define DMX_REG_PCR_FIR_FILTER_COEFF2           134
#define DMX_REG_PCR_FIR_FILTER_COEFF3           135
#define DMX_REG_PCR_EXTENSION                   136
#define DMX_REG_PCR_BASE                        137
#define DMX_REG_STC_SNAP_EXTENSION              138
#define DMX_REG_STC_SNAP_BASE                   139
#define DMX_REG_PCR_ERR_TERM_LOWER              140
#define DMX_REG_PCR_ERR_TERM_UPPER              141
#define DMX_REG_STC_EXTENSION                   142
#define DMX_REG_STC_BASE                        143
#define DMX_REG_PCR_IIR_XN1                     144
#define DMX_REG_PCR_IIR_YN1                     145
#define DMX_REG_PCR_IIR_YN2                     146
#define DMX_REG_PCR_DV_CTRL                     147

#define DMX_REG_PCR2_CTRL                       150
#define DMX_REG_PCR2_ERR_THRESHOLD              151
#define DMX_REG_PCR2_FIR_FILTER_COEFF0          152
#define DMX_REG_PCR2_FIR_FILTER_COEFF1          153
#define DMX_REG_PCR2_FIR_FILTER_COEFF2          154
#define DMX_REG_PCR2_FIR_FILTER_COEFF3          155
#define DMX_REG_PCR2_EXTENSION                  156
#define DMX_REG_PCR2_BASE                       157
#define DMX_REG_STC2_SNAP_EXTENSION             158
#define DMX_REG_STC2_SNAP_BASE                  159
#define DMX_REG_PCR2_ERR_TERM_LOWER             160
#define DMX_REG_PCR2_ERR_TERM_UPPER             161
#define DMX_REG_STC2_EXTENSION                  162
#define DMX_REG_STC2_BASE                       163
#define DMX_REG_PCR2_IIR_XN1                    164
#define DMX_REG_PCR2_IIR_YN1                    165
#define DMX_REG_PCR2_IIR_YN2                    166

#define DMX_REG_PCR_ERROR_STATUS_REG            180
#define DMX_REG_PCR_NONERR_STATUS_REG1          181
#define DMX_REG_PCR_NONERR_STATUS_REG2          182
#define DMX_REG_PCR2_ERROR_STATUS_REG           190
#define DMX_REG_PCR2_NONERR_STATUS_REG1         191
#define DMX_REG_PCR2_NONERR_STATUS_REG2         192
#define DMX_REG_DV_CLOCK_RECOVERY               147
#define DMX_REG_PVR_CONTROL                     210     // 0x348
#define DMX_REG_PVR_PLAY_BUF_THRESHOLD          211     // 0x34C
#define DMX_REG_PVR_RECORD_BUF_THRESHOLD        212     // 0x350
#define DMX_REG_PVR_PLAY_BUF_START              213     // 0x354
#define DMX_REG_PVR_PLAY_BUF_END                214     // 0x358
#define DMX_REG_PVR_PLAY_BUF_RP                 215     // 0x35C
#define DMX_REG_PVR_PLAY_BUF_WP                 216     // 0x360
#define DMX_REG_PVR_RECORD_BUF_START            217     // 0x364
#define DMX_REG_PVR_RECORD_BUF_END              218     // 0x368
#define DMX_REG_PVR_RECORD_BUF_RP               219     // 0x36C
#define DMX_REG_PVR_RECORD_BUF_WP               220     // 0x370
#define DMX_REG_PVR_LOCAL_TIMESTAMP             222     // 0x378
#define DMX_REG_PVR_TIMESTAMP_THRESHOLD         223     // 0x37C
#define DMX_REG_PVR_READ_BUFFER_STATUS          224     // 0X380
#define DMX_REG_PVR_ERROR_STATUS_REG            230     // 0x398
#define DMX_REG_PVR_NONERR_STATUS_REG           231     // 0x39C
#define DMX_REG_FTuP_TS_PACKET_FIFO_STATUS      259     // 0x40C
#define DMX_REG_DBM_STATUS_REG_3                260     // 0x410
#define DMX_REG_FRAMER0_FSM                     261     // 0x414
#define DMX_REG_FRAMER1_FSM                     262     // 0x418
#define DMX_REG_FRAMER3_FSM                     263     // 0x41c
#define DMX_REG_FTuP_PROGRAM_COUNTER            265     // 0x424
#define DMX_REG_FRAMER0_STATUS                  269     //0x434
#define DMX_REG_FRAMER1_STATUS                  270     //0x438
#define DMX_REG_FRAMER3_STATUS                  271     //0x43c

#define DMX_REG_FRAMER2_FSM                     280     // 0x460
#define DMX_REG_FRAMER2_STATUS                  281     //0x464

#define DMX_REG_PES_DBM_STEER2_CTRL             384     // 0x600
#define DMX_REG_PLAYBACKFRAMER2_CTRL1           385     //0X604
#define DMX_REG_PLAYBACKFRAMER2_CTRL2           386     // 0x608

#define DMX_REG_PVR_CONTROL2                    390     // 0x618
#define DMX_REG_PVR_PLAY_BUF_THRESHOLD2         391     // 0x61C


#define DMX_REG_PVR_PLAY_BUF_START2             393     // 0x624
#define DMX_REG_PVR_PLAY_BUF_END2               394     // 0x628
#define DMX_REG_PVR_PLAY_BUF_RP2                395     // 0x62C
#define DMX_REG_PVR_PLAY_BUF_WP2                396     // 0x630

#define DMX_REG_PVR_LOCAL_TIMESTAMP2            402     // 0x648
#define DMX_REG_PVR_TIMESTAMP_THRESHOLD2        403     // 0x64c
#define DMX_REG_PVR_READ_BUFFER_STATUS2         404     // 0X650

#define DMX_REG_PVR_ERROR_STATUS_REG2           410     // 0x668
#define DMX_REG_PVR_NONERR_STATUS_REG2          411     // 0x66C


#define DMX_REG_REG_FILE_ADDR_REG               512     // 0x800
#define DMX_REG_LOCAL_ARBITOR_CTRL1             513     // 0x804
#define DMX_REG_LOCAL_ARBITOR_CTRL2             514     // 0x808
#define DMX_REG_LOCAL_ARBITOR_MONITOR           515     // 0x80C

#define DMX_REG_MDMX_CONTROL                    528     // 0x840
#define DMX_REG_MDMX_PACKET_STATUS              529     // 0x844
#define DMX_REG_MDMX_ERR_STATUS                 530     // 0x848
#define DMX_REG_MDMX_NONERR_STATUS              531     // 0x84C
#define DMX_REG_PES_DBM_STEER3_CTRL             536     //0x860
/*for mini-pvr3*/
#define DMX_REG_PLAYBACKFRAMER3_CTRL1               537    //0x864
#define DMX_REG_PLAYBACKFRAMER3_CTRL2              538    //0x868
#define DMX_REG_PVR3_FRAMER_ERROR_STATUS       539    //0x86c
#define DMX_REG_PVR_CONTROL3                   540    //0x870
#define DMX_REG_PVR_PLAY_BUF_THRESHOLD3        541     // 0x874,buffer threshold


#define DMX_REG_PVR_PLAY_BUF_START3            543     // 0x87c
#define DMX_REG_PVR_PLAY_BUF_END3              544    // 0x880
#define DMX_REG_PVR_PLAY_BUF_RP3               545     // 0x884
#define DMX_REG_PVR_PLAY_BUF_WP3               546   // 0x888

#define DMX_REG_PVR_RECEIVED_TIMESTAMP3        551     //0x89c
#define DMX_REG_PVR_LOCAL_TIMESTAMP3           552     //0x8A0

#define DMX_REG_PVR_TIMESTAMP_THRESHOLD3       553     // 0x8A4
#define DMX_REG_PVR_READ_BUFFER_STATUS3         554     // 0X8A8

#define DMX_REG_PVR_ERROR_STATUS_REG3          558     // 0x8B8
#define DMX_REG_PVR_NONERR_STATUS_REG3         559     // 0x8BC

#define DMX_REG_CONFIG5                         587     //0x92c


#define DMX_REG_FRAMER_PREBYTE_CTRL             576     // 0x900
#define DMX_REG_FRAMER_CTRL1                    577     // 0x904
#define DMX_REG_FRAMER_CTRL2                    578     // 0x908
#define DMX_REG_PLAYBACK_FRAMER_ERROR_STATUS    579     //0x90c
#define DMX_REG_FRAMER_PREBYTE_CTRL2            580     // 0x910
#define DMX_REG_FRAMER_CTRL3                    581     // 0x914
#define DMX_REG_PCRSPEED_CTRL0                  582     //0x918
#define DMX_REG_PCRSPEED_CTRL1                  583     //0x91C
#define DMX_REG_CONFIG4                         584      //0x920
#define DMX_REG_FRAMER2_CTRL                    585      //0X924
#define DMX_REG_FRAMER2_CTRL2                   586      //0x928

#define DMX_REG_FRAMER_UNLOCK_CONTROL           588      //0X930
#define DMX_REG_SECURE_DRAM_START_ADDR          590      //0X938
#define DMX_REG_SECURE_DRAM_END_ADDR            591      //0X93c
#define DMX_REG_SECURE_PLAYBACK_PIDINDEX0       592      //0X940
#define DMX_REG_SECURE_PLAYBACK_PIDINDEX1       593      //0X944
#define DMX_REG_SECURE_PLAYBACK_PATTERN         594      //0X948





#define DMX_FMR0                                (0)
#define DMX_FMR1                                (1)
#define DMX_FMR2                                (2)
#define DMX_FMR3                                (3)
#define DMX_FMR_DISABLE                         (0 << 0)
#define DMX_FMR_ENABLE                          (1 << 0)
#define DMX_FMR_SERIAL_INPUT                    (0 << 1)
#define DMX_FMR_PARALLEL_INPUT                  (1 << 1)
#define DMX_FMR_INTERNAL_SYNC                   (0 << 2)
#define DMX_FMR_EXTERNAL_SYNC                   (1 << 2)
#define DMX_FMR_NEGATIVE_EDGE                   (0 << 3)
#define DMX_FMR_POSITIVE_EDGE                   (1 << 3)

//
// Demux message queue attributes
//
#define DMX_MSG_PRIORITY_HIGH                   0
#define DMX_MSG_PRIORITY_NORMAL                 128
#define DMX_MSG_PRIORITY_LOW                    255

#define DDI_TS_INDEX_FOR_NO_FRAMER              3

#define DMX_STARTCODE_PATTERN_SIZE              4
#define DMX_STARTCODE_BUFFER_SIZE               ((DMX_STARTCODE_PATTERN_SIZE + 2 + 31) & ~31u)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//// DMX input mode
typedef enum
{
    DMX_PUSH,
    DMX_HALF_PUSH,
    DMX_PULL
} DMX_INPUT_MODE_T;

typedef enum
{
#if defined(CC_MT5890) || defined(CC_MT5882)
    DMX_CKGEN_EXT_0 = 0,
    DMX_CKGEN_EXT_2 = 1,
    DMX_CKGEN_EXT_3 = 2,
    DMX_CKGEN_EXT_4 = 3,
    DMX_CKGEN_CI = 4,
#endif
    DMX_CKGEN_NUM
} DMX_EXT_CKGEN_T;


typedef enum
{   
#if defined(CC_MT5890)
    DMX_PINMUX_EXT_S = 0x10,           // Serial
    DMX_PINMUX_EXT_P = 0x11,           // Parallel

    DMX_PINMUX_EXT2_0_S = 0x20,        // Serial
    DMX_PINMUX_EXT2_0_P = 0x21,        // Parallel

    DMX_PINMUX_EXT2_1_S = 0x30,        // Serial

    DMX_PINMUX_EXT2_2_S = 0x40,        // Serial

    DMX_PINMUX_EXT2_3_S = 0x50,        // Serial

    DMX_PINMUX_EXT2_4_S = 0x60,        // Serial

    DMX_PINMUX_EXT3_0_S = 0x70,          // Serial

    DMX_PINMUX_EXT3_1_S = 0x80,          // Serial

    DMX_PINMUX_EXT3_2_S = 0x90,          //Serial
    
    DMX_PINMUX_EXT4_S = 0xA0,          // Parallel
//=============================================
#else  //MT5882
//=============================================
    DMX_PINMUX_EXT_S = 0x10,           // Serial
    DMX_PINMUX_EXT_P = 0x11,           // Parallel
    
    DMX_PINMUX_EXT_1_S = 0x20,           // Serial
    DMX_PINMUX_EXT_1_P = 0x21,           // Parallel

    DMX_PINMUX_EXT2_0_S = 0x30,        // Serial
    DMX_PINMUX_EXT2_0_P = 0x31,        // Parallel

    DMX_PINMUX_EXT2_1_S = 0x40,        // Serial

    DMX_PINMUX_EXT2_2_S = 0x50,        // Serial

    DMX_PINMUX_EXT2_3_S = 0x60,        // Serial

    DMX_PINMUX_EXT3_S = 0x70,          // Serial

    DMX_PINMUX_EXT4_S = 0x80,          // Serial
#endif
} DMX_PINMUX_SEL_T;

/// PID structure
typedef struct
{
    // Buffer info
#ifdef CC_FBM_SUPPORT_DMXPID
    UINT32 u4BufStartPoint;     // Buffer start
#endif

    UINT32 u4BufStart;          // Buffer start adderss mapped in kernel logical address space
    UINT32 u4BufEnd;            // Buffer end address mapped in kernel logical address space
    UINT32 u4BufLen;            // Buffer length
    UINT32 u4Rp;                // Read pointer mapped in kernel logical address space
    UINT32 u4SectionRp;         // Section read pointer mapped in kernel logical address space
    UINT32 u4PesRp;             // PES read pointer mapped in kernel logical address space
    UINT32 u4Wp;                // Write pointer mapped in kernel logical address space
    UINT32 u4PeakBufFull;       // Peak Buffer Fullness, to estimate required
    // ES FIFO size
    UINT32 u4HeaderBufAddr;     // Header buffer start address mapped in kernel logical address space
    UINT32 u4NextPic;           // Next picture number in current PES

#ifdef CC_DMX_MULTIPLE_SECTION
    //UINT32 u4SecMask;
    UINT32 au4SecMask[SEC_CNT];
    UINT8  u1SecCount;
    UINT8  u1SecIdx;            // If u1SecIdx means first secion filter index
#endif  // CC_DMX_MULTIPLE_SECTION

    // PID attributes
    BOOL fgEnable;              // Valid
    BOOL fgAllocateBuffer;      // Allocate buffer or not
    BOOL fgPrimary;             // Primary PID
    BOOL fgFifoFull;            //Pid buffer fifo full
    UINT8 u1ContainAD;          // Audio description (0: no-AD, 1: AD, 2: decided by driver)
    UINT8 u1TsIndex;            // TS index
    UINT8 u1LocalTsIndex;      //local tsindex for ci+1.4
    UINT8 u1DeviceId;           // Device ID
    UINT8 u1KeyIndex;           // Key index
    UINT8 u1SteerMode;          // Steering mode
    UINT16 u2Pid;               // PID
    DMX_PCR_MODE_T ePcrMode;    // PCR mode
    DMX_PID_TYPE_T ePidType;    // PID type
    DMX_DESC_MODE_T eDescMode;  // Descramble mode
    PFN_DMX_NOTIFY pfnNotify;   // Demux callback function
    void* pvNotifyTag;          // Tag value of callback function
    PFN_DMX_NOTIFY pfnScramble; // Scramble state callback function
    void* pvScrambleTag;        // Tag value of scramble callback

    // Statistics counters
    DMX_PID_COUNTERS_T rCounters;

    UINT8 u1SerialNumber;

#ifdef CC_DMX_PES_EXT_BUFFER
    // The buffer for storing the "extension 2 data" in the PES header.
    // The "extension 2 data" is parsed/copied for the video PES.
    // All these pointers store virtual addresses.
    UINT32 u4ExtBufStart;
    UINT32 u4ExtBufEnd;
    UINT32 u4ExtBufRp;
    UINT32 u4ExtBufWp;
#endif  // CC_DMX_PES_EXT_BUFFER

    UINT8 u1ChannelId;
    DMX_INPUT_TYPE_T eInputType;
    DMX_SOURCE_T eSource;
    void *pvInstanceTag;        // Tag for supporting multiple instances (used only by v-dec)

#ifdef CC_DMX_PS_CPU
    UINT32 u4LastWp;
    UINT8 u1StreamID;
    UINT8 u1SubstreamID;
    BOOL fgEnableSsidFilter;
#endif  // CC_DMX_PS_CPU

    BOOL fgDisableFifoOutput;

    BOOL fgIommuEnable;         //IOMMU Enable or Disable

#ifdef CC_DMX_TS130
    BOOL fgTS130;
#endif
    BOOL fgSendCompleteMsg;

    UINT32 u4FrameCntPs;        // frames per second
    HAL_TIME_T rPrevFrameTime;  // previous frame arrival time
#ifdef CC_EIT_SECTION_FILTER
    UINT32 u4LastSent2MwRp; //For last EIT data sent to MW
    UINT32 u4LastMwUpdateRp; //For last EIT data sent to MW
#endif
} PID_STRUCT_T;

typedef enum
{
    FILTER_TYPE_LEGACY,
    FILTER_TYPE_GENERIC
} FILTER_TYPE_T;

typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[8];              ///< Pattern
    UINT8 au1Mask[8];              ///< Mask
} FILTER_BANK_T;

typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
#ifdef CC_DMX_MULTIPLE_SECTION
    BOOL fgCompareSW;
#endif // CC_DMX_MULTIPLE_SECTION
    BOOL fgNegSpread;
    UINT8 u1Pidx;                   ///< PID index, 0 ~ 31
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[16];              ///< Pattern
    UINT8 au1Mask[16];              ///< Mask
    UINT8 au1PosNeg[16];            ///< Positive or negative
    DMX_GENFILTER_MODE_T eMode;
} FILTER_GENERIC_T;

typedef struct
{
    FILTER_BANK_T arBank[2];
    FILTER_GENERIC_T rGeneric;
    DMX_FILTER_MODE_T eMode;        ///< Mode
    BOOL fgIsFvr;
} FILTER_STRUCT_T;

/// Capture settings
typedef struct
{
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    PFN_DMX_CAPTURE_HANDLER pfnHandler;
    void* pvTag;
} DMX_CAPTURE_SETTINGS_T;

/// Demux states
typedef struct
{
    UINT8 au1ChannelId[DMX_NUM_PID_INDEX];
    BOOL afgNewPidx[DMX_NUM_PID_INDEX];

#ifndef CC_SECURE_WORLD
    BOOL afgUseNew[MPV_MAX_ES];
#endif

    DMX_CAPTURE_SETTINGS_T rCaptureSettings;
} DMX_STATES_T;


/// Prototype of PVR record callback handler
typedef void (*PFN_PVR_RECORD_HANDLER)(void);

/// Prototype of PVR play callback handler
typedef void (*PFN_PVR_PLAY_HANDLER)(void);


typedef struct
{
    UINT32 u4Pattern0;
    UINT32 u4Pattern1;
    UINT32 u4Mask0;
    UINT32 u4Mask1;
} DMX_STARTCODE_T;

typedef struct
{
    DMX_VIDEO_TYPE_T eVideoType;
    UINT8 au1Pattern[DMX_STARTCODE_PATTERN_SIZE];
    UINT8 au1Mask[DMX_STARTCODE_PATTERN_SIZE];
} DMX_VIDEO_STARTCODE_T;

#ifdef CC_EIT_SECTION_FILTER
//for every channel ,it use the following three key to identify it
#define  CHANNEL_MAX_NUM        (500)

#define  EIT_TABLE_NUM          (32) //for every channel ,it contains 0x50-0x5f,0x60-0x6f table id .

#define  EIT_SECTION_NUM        (32)

#define  EIT_INVALID_TABLE_VERSION (0xff)

#define EIT_FILTER_BY_TABLEID(tableid)      ((tableid)-0x50)

#define EIT_SECTION_IDX(tableid,secnum) ((((tableid)-0x50)*EIT_SECTION_NUM)+((secnum)/8))

#define EIT_GET_SECTION_MASK(secnum,value)  (((value)>>(secnum)%8)&(0x1))

typedef enum
{
    EIT_FILTER_INDEX_NOT_FOUND,
    EIT_FILTER_VERSION_NOT_SAME,
    EIT_FILTER_MATCH,
    EIT_FILTER_NOT_MATCH,
    EIT_FILTER_INVALID
}EIT_FILTER_CHECK_RESULT;

typedef struct 
{ 
    //channel key number
    CHANNEL_EIT_KEY_T t_channel;
    // for every table id ,it maps to one version number
    UINT8 aui1_cur_version[EIT_TABLE_NUM];
    //for every table id ,it at most has 32*8=256 section number ,so 
    //use 32 bytes to mask every section number.
    UINT8 aui1_sec_mask[EIT_TABLE_NUM * EIT_SECTION_NUM];
    // b_used=1 ,it indicate this item is used.
    UINT8 b_used;
}CHANNEL_EIT_S_SECTION_T;
#endif


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
EXTERN void _DMX_Panic(const CHAR* szExpress, const CHAR* szFile, INT32 i4Line);
EXTERN INT32 _i4DmxErrReason;
#define DMX_PANIC(x)        ((x) ? (void)0 : _DMX_Panic(#x, __FILE__, (INT32)__LINE__))
#define DMX_PANIC_RET(x)        if(_i4DmxErrReason != 0) { _i4DmxErrReason = 0;  return (x); }
#define DMX_PANIC_RET_VOID()    if(_i4DmxErrReason != 0) { _i4DmxErrReason = 0;  return; }

/// Get byte in a word
#define GET_BYTE(arg, n)                ((UINT8*)&(arg))[n]

///
/// Demux register access commands
///
#define DMXCMD_READ8(offset)            IO_READ8(DEMUX0_BASE, (offset))
#define DMXCMD_READ16(offset)           IO_READ16(DEMUX0_BASE, (offset))
#define DMXCMD_READ32(offset)           IO_READ32(DEMUX0_BASE, ((offset) * 4))
#define DMXCMD_WRITE8(offset, value)    IO_WRITE8(DEMUX0_BASE, (offset), (value))
#define DMXCMD_WRITE16(offset, value)   IO_WRITE16(DEMUX0_BASE, (offset), (value))
#define DMXCMD_WRITE32(offset, value)   IO_WRITE32(DEMUX0_BASE, ((offset) * 4), (value))
#define DMXCMD_REG32(offset)            DMXCMD_READ32(offset)

//DDI IOMMU Read/Write  commands
#define IOMMU_DDI_READ32(offset)           IO_READ32(IOMMU_DDI_BASE, ((offset) * 4))
#define IOMMU_DDI_WRITE32(offset, value)   IO_WRITE32(IOMMU_DDI_BASE, ((offset) * 4), (value))

//DMX IOMMU Read/Write  commands
#define IOMMU_DMX_READ32(offset)           IO_READ32(IOMMU_DMX_BASE, ((offset) * 4))
#define IOMMU_DMX_WRITE32(offset, value)   IO_WRITE32(IOMMU_DMX_BASE, ((offset) * 4), (value))


/// Read/Write DMEM commands
/// Key base


/// Read/Write DMEM commands
#define DMX_DMEM_CA_CTRL(word)          ((volatile UINT32*)(DMX_DMEM_CA_CTRL_BASE))[(word)]
#define DMX_DMEM_CA_EVEN_KEY(idx, word) ((volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + (96 * (idx))))[(word)]
#define DMX_DMEM_CA_EVEN_IV(idx, word)  ((volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + (32 + (96 * (idx)))))[(word)]
#define DMX_DMEM_CA_ODD_KEY(idx, word)  ((volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + (48 + (96 * (idx)))))[(word)]
#define DMX_DMEM_CA_ODD_IV(idx, word)   ((volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + (80 + (96 * (idx)))))[(word)]

/// Key base
#define DMX_DMEM_CA_KEY(idx, word)      ((volatile UINT32*)(DMX_DMEM_CA_KEY_BASE + (96 * (idx))))[(word)]



#define DMX_VCODE_W(word)               ((volatile UINT32*)(DMX_PATTERN_MATCH_BASE0))[(word)]
#define DMX_VCODE_S_W(idx, word)        DMX_VCODE_W((2 + (4 * (idx))) + (word))

///
/// PID data structure access commands
///
#define PID_W(word)             ((volatile UINT32*)(DEMUX2_BASE))[(word)]
#define PID_S(pidx)             PID_W(DMX_DMEM_ENTRY_LEN * (pidx))
#define PID_S_W(pidx, word)     PID_W((DMX_DMEM_ENTRY_LEN * (pidx)) + (word))

/// PID index table access command
#define PID_INDEX_TABLE(pidx)   ((volatile UINT32*)(DEMUX0_BASE + 0xc00))[(pidx)]

//-------------------------------------------------------------------

/// Filter access command
//New Section Filter access Command for share mode
#define SECTION_FILTER_BASE_CTRL(base,Filteridx)          ((volatile UINT32*)(base))[(Filteridx)]
#define SECTION_FILTER_BASE(base,Filteridx, word)          ((volatile UINT32*)(base + ((Filteridx) * 8)))[(word)]
#define SECTION_FILTER_PAT( Filteridx, word) \
        SECTION_FILTER_BASE(((Filteridx>=DMX_SECTION_PER_UNIT)? DMX_SECTION_PATTERN2: \
                             DMX_SECTION_PATTERN)  \
                             ,((Filteridx>=DMX_SECTION_PER_UNIT)? (Filteridx-DMX_SECTION_PER_UNIT): \
                             Filteridx),word)

#define SECTION_FILTER_MASK(Filteridx, word) \
	   SECTION_FILTER_BASE(((Filteridx>=DMX_SECTION_PER_UNIT)? DMX_SECTION_MASK2: \
						 DMX_SECTION_MASK)  \
						 ,((Filteridx>=DMX_SECTION_PER_UNIT)? (Filteridx-DMX_SECTION_PER_UNIT): \
						 Filteridx),word)


#define SECTION_FILTER_POSNEG(Filteridx, word) \
	SECTION_FILTER_BASE(((Filteridx>=DMX_SECTION_PER_UNIT)? DMX_SECTION_POSNEG2: \
						 DMX_SECTION_POSNEG)  \
						 ,((Filteridx>=DMX_SECTION_PER_UNIT)? (Filteridx-DMX_SECTION_PER_UNIT): \
						 Filteridx),word)


#define SECTION_FILTER_CTRL(Filteridx) \
	SECTION_FILTER_BASE_CTRL(((Filteridx>=DMX_SECTION_PER_UNIT)? DMX_SECTION_CONTROL2: \
						 DMX_SECTION_CONTROL)  \
						 ,((Filteridx>=DMX_SECTION_PER_UNIT)? (Filteridx-DMX_SECTION_PER_UNIT): \
						 Filteridx))

/// Function entry log
#define FUNC_ENTRY              DMXLOG(DMX_LOG_FUNCTION 5, "+++ Enter %s +++\n", __FUNCTION__)
/// Function exit log
#define FUNC_EXIT               DMXLOG(DMX_LOG_FUNCTION 5, "--- Exit %s ---\n", __FUNCTION__)

#define PLAYBACK_GBL_SIZE       (9)   // 36 bytes

#if defined(CC_MT5890)
#define PLAYBACK_GBL_BASE       (DEMUX7_BASE+ 0x150)
#define CT_SETTING              (*(volatile UINT32*)(DEMUX7_BASE + 0x150))
#define MICROCODE_WATCHDOG      (*(volatile UINT32*)(DEMUX7_BASE + 0x154))
#define CT_TSIDX_SETTING        (*(volatile UINT32*)(DEMUX7_BASE + 0x158))
#define SECTIONFILTER_SETTING   (*(volatile UINT32*)(DEMUX7_BASE + 0x15C))
#define TS4_VIDEO_TYPE          (*(volatile UINT32*)(DEMUX7_BASE + 0x170))

#define B_FRAME_DROP_ENABLE     (*(volatile UINT32*)(DEMUX7_BASE + 0x160))
#else  //MT5882
#define PLAYBACK_GBL_BASE       (DEMUX7_BASE)
#define CT_SETTING              (*(volatile UINT32*)(DEMUX7_BASE))
#define MICROCODE_WATCHDOG      (*(volatile UINT32*)(DEMUX7_BASE + 0x4))
#define CT_TSIDX_SETTING        (*(volatile UINT32*)(DEMUX7_BASE + 0x8))
#define SECTIONFILTER_SETTING   (*(volatile UINT32*)(DEMUX7_BASE + 0xC))
#define B_FRAME_DROP_ENABLE     (*(volatile UINT32*)(DEMUX7_BASE + 0x10))
#define VCT_SOFTFILTER_SETTING    (*(volatile UINT32*)(DEMUX7_BASE + 0x10))
#endif

#define RV_SETTING_SIZE         (33)//132 byte

#if defined(CC_MT5890)
#define RV_SETTING_BASE         (DEMUX7_BASE + 0x3B0) //0x543b0,systme b golbal region
#else  //MT5882
#define RV_SETTING_BASE         (DEMUX7_BASE + 0x260) //0x54260,systme b golbal region
#endif

#define RV_SETTING_CTRL         (*(volatile UINT32*)RV_SETTING_BASE) //for ts-130.

//#define PID_REPLACED_SIZE         (33)//132 byte
//#define PID_REPLACED_BASE         (DEMUX5_BASE + 0x3F4) //0x1C3F4


/// address increment in ring buffer
#define ADDR_INCR_IN_RING(addr, incr, ringstart, ringend)      \
    ((((addr) + (incr)) < (ringend)) ? ((addr) + (incr)) : (((addr) + (incr)) - ((ringend) - (ringstart))))

#ifdef NDEBUG
#define DMXLOG(level, str...)           LOG(level, str)
#define DMXPIDLOG(level, pidx, str...)  LOG(level, str)
#define DMXSECLOG(level, secIdx, str...)  LOG(level, str)
#else
extern UINT32 u4LogLvlMask;
extern UINT8 u1PidxMask;
extern UINT8 u1SecIdxMask;
#define DMXLOG(level, str...)  if((((level) & 0xFFFFFF00) == 0) || (((level) & u4LogLvlMask) != 0)) {LOG(((level) & 0xff), str);}
#define DMXPIDLOG(level, pidx, str...) if((u1PidxMask == 0xFF) ||  (u1PidxMask == pidx)) {DMXLOG((level), str);}
#define DMXSECLOG(level, secIdx, str...) if((u1SecIdxMask == 0xFF) ||  (u1SecIdxMask == secIdx)) {DMXLOG((level), str);}
#endif

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------
//
// Initialization
//

EXTERN BOOL _DMX_Init(void);

EXTERN VOID _DMX_SetIgnorePESLen(BOOL fgEnable);

EXTERN VOID _DMX_SetDuplicateScheme(BOOL fgOldMethod);


EXTERN VOID _DMX_SetScrambleScheme(BOOL fgOrg);

EXTERN BOOL _DMX_GetScrambleScheme(void);

EXTERN VOID _DMX_SetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T eType, UINT8 u1Flag);

EXTERN BOOL _DMX_GetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T *peType, UINT8 *pu1Flag);

EXTERN VOID _DMX_EnablePower(BOOL fgEnable);

EXTERN BOOL _DMX_Reset(void);

EXTERN void _DMX_SetFramerMode(UINT8 u1Framer, DMX_FRAMER_MODE_T eMode,
                               BOOL fgExtSync, BOOL fgPosEdge);
EXTERN BOOL _DMX_Framer_130byteEnable(UINT8 u1Framer, BOOL fgDemod130byteTs, BOOL fgInputEnable, UINT8 u1InputPktSize ,BOOL fgOutputEnable, UINT8 u1OutputPktSize);
EXTERN void _DMX_BypassErrorHandlingTable_Enable(UINT8 u1Framer,BOOL fgEnable);
EXTERN void _DMX_SetFramer(UINT8 u1FramerIdx, BOOL fgEnable, BOOL fgPareallel, BOOL fgExtSync, BOOL fgPosEdge);

EXTERN BOOL _DMX_LoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len);

EXTERN BOOL _DMX_SetFrontEndEx(UINT8 u1TsIdx, DMX_FRONTEND_T eFrontEnd);

EXTERN BOOL _DMX_SetDbmChannel4(BOOL fg_playback_enable, BOOL fg_record_enable);
EXTERN BOOL _DMX_GetDbmChannel4(BOOL* fg_playback_enable, BOOL* fg_record_enable);


EXTERN DMX_FRONTEND_T _DMX_GetFrontEnd(UINT8 u1TsIdx);

EXTERN void _DMX_ResetFramer(UINT8 u1Framer);

EXTERN void _DMX_SetFramerEnabled(UINT8 u1Framer, BOOL fgEnable);

EXTERN UINT8 _DMX_GetFramerIndex(void);

EXTERN BOOL _DMX_ActivateDbmReset(VOID);

EXTERN BOOL _DMX_ReleaseDbmReset(VOID);

EXTERN BOOL _DMX_ResetDbmSafely(VOID);

EXTERN VOID _DMX_ClearInterruptQueue(VOID);

EXTERN VOID _DMX_ResetFTuP(VOID);

EXTERN void _DMX_EnableFTI(BOOL fgEnable);

EXTERN BOOL _DMX_Start(void);

EXTERN BOOL _DMX_Stop(void);

EXTERN void _DMX_SetInputMode(DMX_INPUT_MODE_T rMode);

EXTERN BOOL _DMX_TestFTuPLatch(void);

EXTERN UINT32 _DMX_GetMinFreeBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T ePidType);

EXTERN UINT32 _DMX_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T ePidType);

EXTERN UINT32 _DMX_GetMinFreeBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType);

EXTERN UINT32 _DMX_GetMinFreeDRAMBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType);

EXTERN VOID _DMX_Version(void);

EXTERN BOOL _DMX_GetEfuseStatus(void);

EXTERN BOOL _DMX_IsMicroProcessorStopped(void);

EXTERN void _DMX_SetDbm_InputSource(UINT8 u1TsIdx, DMX_DBM_INPUT_SOURCE_T eSource);

EXTERN DMX_DBM_INPUT_SOURCE_T _DMX_GetDbm_InputSource(UINT8 u1TsIdx);

EXTERN BOOL _DMX_SetFramerPktSize(UINT8 u1TsIndex, UINT16 u2PktSize);

EXTERN BOOL _DmxCustFrontEnd(void);  //tmp interface

EXTERN BOOL _DMX_PSIBufferInit(void);

EXTERN BOOL _DMX_SetFramerPacketErrorHandling(UINT8 u1TsIndex, BOOL fgEnable, UINT32 u4Value);
EXTERN UINT32 _DMX_GetFramerPacketErrorHandling(UINT8 u1TsIndex);

EXTERN VOID _DMX_SetSecurebufferPattern(UINT32 u4Pattern);

EXTERN VOID _DMX_SetSecurebufferRegion(UINT32 u4Start,  UINT32 u4End);

EXTERN VOID _DMX_SetSecurePidIndex(UINT8 u1PidIdx, BOOL fgEnable);

EXTERN VOID _DMX_SetSecurebufferProtectEnable(BOOL Enable);
//
// PID setup
//

EXTERN BOOL _DMX_SetBypassMode(UINT8 u1TsIndex, UINT8 u1PacketSize,
                               BOOL fgSteerToFTuP, BOOL fgReset);

EXTERN BOOL _DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid, BOOL fgKeepScramble);

EXTERN BOOL _DMX_SetScrambleCheckPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid);

EXTERN BOOL _DMX_SetSeamlessPID(UINT8 u1NewPidx, UINT8 u1OldPidx, UINT16 u2Pid);

EXTERN BOOL _DMX_SetIgnoreCCCheck(UINT8 u1Pidx, BOOL fgEnable);

EXTERN BOOL _DMX_TS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType);

EXTERN DMX_AUDIO_TYPE_T _DMX_TS_GetAudioType(UINT8 u1Pidx);

EXTERN BOOL _DMX_HDCP_EnableDistCtrCheck(BOOL fgEnable);

EXTERN BOOL _DMX_HDCP_IsInvalidCtr(UINT8 u1Pidx);

EXTERN BOOL _DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid);

EXTERN UINT32  _DMX_GetPsiMaxSize(void);

EXTERN UINT32  _DMX_GetPesMaxSize(void);

EXTERN BOOL _DMX_FreePid(UINT8 u1Pidx);

EXTERN BOOL _DMX_FreeSwitchPid(UINT8 u1Pidx);

EXTERN BOOL _DMX_FreePidBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_ResetPidBuffer(UINT8 u1Pidx, PID_STRUCT_T* prPidStruct, BOOL fgKeepScramble);

EXTERN BOOL _DMX_FlushPidBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex);

EXTERN BOOL _DMX_SetPcr(UINT8 u1PidIndex, BOOL fgEnable, BOOL fgNewMode);

EXTERN BOOL _DMX_IsVideoPid(UINT8 u1PidIndex);

EXTERN PID_STRUCT_T* _DMX_GetPidStruct(UINT32 u4PidIndex);

EXTERN BOOL _DMX_SetStartCodePattern_Ex(UINT8 u1NoIntrCount,
                                        const DMX_STARTCODE_T *prNoIntrStartCode,
                                        UINT8 u1IntrCount,
                                        const DMX_STARTCODE_T *prIntrStartCode);

EXTERN BOOL _DMX_SetBufPointer(UINT8 u1Idx, UINT32 u4BufStart, UINT32 u4BufEnd,
                               UINT32 u4Addr);

EXTERN DMX_INPUT_TYPE_T _DMX_GetPidInputType(UINT8 u1Pidx);

EXTERN UINT8 _DMX_GetPidTsIndex(UINT8 u1Pidx);

EXTERN BOOL _DMX_GetIsTSEnabled(UINT8 u1TsIdx);

EXTERN BOOL _DMX_Support_Tsin(void);

EXTERN BOOL _DMX_SetStcToDspUpdate(UINT8 u1Id, BOOL fgEnable);

//
// Descrambler
//
EXTERN void _DMX_DMEM_CA_Init(void);

EXTERN BOOL _DMX_SetCA_Mode(UINT8 u1Idx, DMX_CA_MODE_T eMode, DMX_CA_FEEDBACK_MODE_T eFBMode, BOOL fgEncrypt);

EXTERN BOOL _DMX_SetDescMode(UINT8 u1Idx, DMX_DESC_MODE_T eMode);

EXTERN void _DMX_SetCA_CTR(UINT8 u1Idx, UINT64 u8Nonce, UINT64 u8Counter, BOOL fgEven);

EXTERN void _DMX_SetCTR_Rtb(UINT8 u1Idx, BOOL fgDoRtb);

EXTERN BOOL _DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
                           const UINT32 au4Key[6]);

EXTERN void _DMX_SetDesIV(UINT8 u1Idx, UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);

EXTERN void _DMX_SetRtbMode(UINT8 u1Idx, DMX_DESC_RTB_MODE_T eMode);

EXTERN void _DMX_SetAesIV(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven);

EXTERN BOOL _DMX_SetAesKeyLen(UINT8 u1Idx, UINT32 u4KeyLen);

EXTERN BOOL _DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8]);

EXTERN BOOL _DMX_SetSMS4Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[4]);

EXTERN void _DMX_SetSMS4IV(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven);

EXTERN BOOL _DMX_VUDU_SetKey(DMX_VUDU_KEY_T *prVuduKey);

EXTERN BOOL _DMX_SetDVBKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2]);

EXTERN void _DMX_SetDVBIV(UINT8 u1Idx, UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven);

#ifdef CC_DMX_SUPPORT_MULTI2
EXTERN BOOL _DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2]);

EXTERN void _DMX_SetMulti2CbcIV(UINT8 u1Index,UINT32 u4IvHi, UINT32 u4IvLo);

EXTERN void _DMX_SetMulti2OfbIV(UINT8 u1Index,UINT32 u4IvHi, UINT32 u4IvLo);

EXTERN void _DMX_SetMulti2SysKey(UINT8 u1Idx, const UINT32 au4Key[8]);

EXTERN void _DMX_SetMulti2Config(UINT8 u1Index,BOOL fgOfbFreeRun, UINT16 u2Iteration);
#endif  // CC_DMX_SUPPORT_MULTI2

EXTERN BOOL _DMX_ReloadSecureKey(UINT8 u1KeyIndex, UINT32 u4SecureKeyIdx, DMX_CRYPTO_KEY_TYPE_T eKeyType);

EXTERN BOOL _DMX_SetHDCPKeyIv(UINT8 u1Pidx, UINT8 u1KeyIdx, UINT8 *pu1Key, UINT8 *pu1Iv);

EXTERN BOOL _DMX_SetCryptoKey(UINT8 u1KeyIdx, BOOL fgEven, DMX_CRYPTO_KEY_TYPE_T eKeyType,
                            const UINT8 *pu1Key, UINT32 u4KeyLen);

EXTERN BOOL _DMX_SetCaOutputBuffer(BOOL fgEnable, BOOL fgIsPs,
                                   UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);

EXTERN BOOL _DMX_SetCaInputBuffer(BOOL fgEnable, BOOL fgIsPs,
                                  UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);

//
// Capture function
//
EXTERN BOOL _DMX_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture);

EXTERN BOOL _DMX_UpdateCaptureReadPointer(UINT32 u4NewRp);

EXTERN BOOL _DMX_GetCaptureInfo(DMX_CAPTURE_INFO_T* prInfo);

EXTERN BOOL _DMX_DemodDataCapture(BOOL fgEnable, const UINT32 u4BufStart,
                                  const UINT32 u4Size, const UINT32 u4Threshold);

EXTERN BOOL _DMX_Capture_ChannelChangeInit(UINT32 u4Size);

EXTERN BOOL _DMX_Capture_ChannelChangeStart(void);

//
// PSI
//
EXTERN VOID _DMX_VCTSoftFilterEnable(BOOL fgEnable, UINT8 TableId);

EXTERN void _DMX_SetFilterType(FILTER_TYPE_T eType);

EXTERN FILTER_TYPE_T _DMX_GetFilterType(void);

EXTERN BOOL _DMX_SetFilter(UINT8 u1FitlerIndex, UINT8 u1Bank,
                           UINT32 u4Flags, const DMX_FILTER_T* prFilter);

EXTERN BOOL _DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                              const DMX_FILTER_GENERIC_T* prFilter);

EXTERN BOOL _DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
                           DMX_FILTER_T* prFilter);

EXTERN BOOL _DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                              DMX_FILTER_GENERIC_T* prFilter);

EXTERN BOOL _DMX_SetOneByteFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                                  const DMX_FILTER_GENERIC_T* prFilter);

EXTERN BOOL _DMX_GetOneByteFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                                  DMX_FILTER_GENERIC_T* prFilter);

EXTERN FILTER_STRUCT_T* _DMX_GetFilterStruct(UINT8 u1Idx, UINT32 u4FilterIndex);

EXTERN BOOL _DMX_PSI_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

EXTERN BOOL _DMX_PSI_FreeBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
                                  UINT32 u4Size, UINT32 u4FrameAddr);

EXTERN BOOL _DMX_PSI_FlushBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
                               UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

#ifdef DMX_MEASURE_PSI_TIME
EXTERN VOID _DMX_PSI_GetMaxTime(HAL_TIME_T *prTime);

EXTERN VOID _DMX_PSI_SetMaxTime(const HAL_TIME_T *prTime);

EXTERN VOID _DMX_PSI_ResetMaxTime(void);
#endif  // DMX_MEASURE_PSI_TIME


//
// PES
//

EXTERN BOOL _DMX_PES_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

EXTERN BOOL _DMX_PES_FreeBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_PES_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
                                  UINT32 u4Size, UINT32 u4FrameAddr);

EXTERN BOOL _DMX_PES_FlushBuffer(UINT8 u1Pidx);

EXTERN BOOL _DMX_PES_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
                               UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

//
// ISR-related
//

EXTERN void _DMX_InitISR(void);

EXTERN BOOL _DMX_InitHandler(void);

EXTERN BOOL _DMX_InitPSIHandler(void);

EXTERN BOOL _DMX_AddPSI(DMX_PSI_MSG_T *prPSI);

EXTERN BOOL _DMX_InitMonThread(void);

EXTERN BOOL _DMX_ClearPicFlag(UINT8 u1Pidx);

//
// Handlers
//

EXTERN DMX_STATES_T* _DMX_GetStates(void);

EXTERN void _DMX_SetFramerErrorHandlingTable(UINT32 u4Value);

EXTERN UINT32 _DMX_GetFramerErrorHandlingTable(void);

EXTERN BOOL _DMX_InitAPI(void);

EXTERN BOOL _DMX_AllocateVideoChannel(UINT8 u1Pidx, UINT8 u1Channel);

EXTERN void _DMX_FreeVideoChannel(UINT8 u1Pidx);

EXTERN void _DMX_StopOldVideoChannel(UINT8 u1Pidx);

EXTERN BOOL _DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel);

EXTERN BOOL _DMX_GetVideoPidx(UINT8 u1Channel, UINT8* pu1Pidx);

#ifdef CC_DMX_PES_EXT_BUFFER
EXTERN BOOL _DMX_UpdateExtBufferReadPointer(UINT8 u1Pidx, UINT32 u4NewRp,
        UINT32 u4DataAddr);
#endif  // CC_DMX_PES_EXT_BUFFER

EXTERN BOOL _DMX_UpdateWritePointer(UINT8 u1Pidx, UINT32 u4Wp);

EXTERN BOOL _DMX_UpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp,
                                   UINT32 u4FrameAddr);

EXTERN BOOL _DMX_UpdateReadPointer2(UINT8 u1Pidx, UINT32 u4Rp,
                                    UINT32 u4FrameAddr);

EXTERN BOOL _DMX_UpdateVideoReadPointer(UINT8 u1Channel, UINT32 u4Rp,
                                        UINT32 u4FrameAddr);

EXTERN BOOL _DMX_UpdateVideoWritePointer(UINT8 u1Pidx, UINT8 u1Channel, UINT32 u4Wp);

EXTERN BOOL _DMX_SendPictureHeader(DMX_PES_MSG_T* prPes);

EXTERN BOOL _DMX_SendVideoChunk(const DMX_PES_MSG_T* prPes);

EXTERN BOOL _DMX_SendAudioPes(DMX_AUDIO_PES_T* prPes);

EXTERN BOOL _DMX_SendPurgeMessage(UINT8 u1Pidx, BOOL fgReenablePid);

EXTERN void _DMX_NotifyEnablePid(UINT8 u1Pidx, BOOL fgEnable);

EXTERN INT32 _DMX_Diag(void);

//-----------------------------------------------------------------------------
// --- Beginning (for J-Chip SLT program) ---
EXTERN INT32 _DMX_JChipDiag(UINT32 u1TsIndex);

EXTERN void _DMX_CheckTVCT(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4SecRp,
                           UINT32 u4SecLen);
// --- End (for J-Chip SLT program) ---
//-----------------------------------------------------------------------------


EXTERN BOOL _DMX_GetLockState(UINT8 u1FramerIdx);

EXTERN VOID _DMX_GetPacketStatus(UINT8 u1Tsidx, UINT32 u4MsTime, UINT32 *pu4RcvCount, UINT32 *pu4DropCount);

EXTERN void _DMX_GetDBMFramerStatus(UINT8 u1Tsidx, UINT32 *pu4DBMStatus, UINT32 *pu4FramerStatus);

EXTERN UINT32 _DMX_GetReceivedPacketNumber(void);

EXTERN UINT32 _DMX_GetDroppedPacketNumber(void);

EXTERN BOOL _DMX_SendEOSMessage(UINT8 u1Pidx);

EXTERN BOOL _DMX_SendMoveCompleteMessage(UINT8 u1Pidx);

EXTERN void _DMX_ErrHanlde(void);

//
// Video start code
//
EXTERN BOOL _DMX_VCodeInit(void);
EXTERN DMX_VIDEO_TYPE_T _DMX_GetPidxVideoType(UINT8 u1Idx);
EXTERN BOOL _DMX_SetVideoType(UINT8 u1TsIndex, DMX_VIDEO_TYPE_T eVideoType);
EXTERN DMX_VIDEO_TYPE_T _DMX_GeteVideoType(UINT8 u1TsIndex);
EXTERN INT8 _DMX_GetVCode_Offset(DMX_VIDEO_TYPE_T eVideoType, UINT8 u1Idx);
EXTERN BOOL _DMX_SetVideoTypeByPidx(UINT8 u1Pidx, DMX_VIDEO_TYPE_T eVideoType);
EXTERN DMX_VIDEO_TYPE_T _DMX_GetVideoTypeByPidx(UINT8 u1Pidx);
EXTERN BOOL _DMX_FreeVideoTypeByPidx(UINT8 u1Pidx);
//
// Utilities
//
EXTERN UINT8 _DMX_GetByte(UINT8** ppu1StartAddr, UINT32 u4EndAddr,
                          UINT32 u4BufLen, UINT32 u4Offset, UINT8 u1Forward);

EXTERN UINT32 _DMX_Align(UINT32 u4Addr, UINT32 u4Alignment);

EXTERN BOOL _DMX_IsAligned(UINT32 u4Addr, UINT32 u4Alignment);

EXTERN UINT32 _DMX_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart,
                                  UINT32 u4DstEnd, UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd,
                                  UINT32 u4Size);

EXTERN BOOL _DMX_CopyDestRingBuffer(UINT32 u4Dst, UINT32 u4BufStart,
                                    UINT32 u4BufEnd, UINT32 u4Src, UINT32 u4Size);

EXTERN void DataSwap(UINT32 *pu4Dest, UINT32 *pu4Src, UINT32 u4Size,
                     UINT32 u4Mode);

EXTERN void _DMX_GetPicQSize(UINT16* u2DmxPictureQSize,
                             UINT16* u2DmxMaxQSize);

//
// Helpers
//
#ifdef CC_DEFINE_DMX_LOCK
extern CRIT_STATE_T _rGDmxLock;
#define _DMX_Lock() do { _rGDmxLock = x_crit_start(); } while (0)
#define _DMX_Unlock() do {  x_crit_end(_rGDmxLock); } while (0)
#else /* CC_DEFINE_DMX_LOCK */
EXTERN void _DMX_Lock(void);
EXTERN void _DMX_Unlock(void);
#endif /* CC_DEFINE_DMX_LOCK */

EXTERN void _DMX_LockApi(void);

EXTERN void _DMX_UnlockApi(void);

EXTERN UINT32 _DMX_AdvanceAddr(UINT32 u4Addr, INT32 i4Increment, UINT32 u4Wp,
                               UINT32 u4FifoStart, UINT32 u4FifoEnd);

EXTERN UINT32 _DMX_CopyRingBuffer(UINT32 u4Dst, UINT32 u4DstStart,
                                  UINT32 u4DstEnd, UINT32 u4Src, UINT32 u4SrcStart, UINT32 u4SrcEnd,
                                  UINT32 u4Size);


#ifdef CC_DMX_EMULATION
EXTERN void _DMX_SetToDecoder(BOOL fgToDecoder);
#endif

EXTERN BOOL _DMX_IsToDecoder(void);

EXTERN BOOL _DMX_GetPidCounters(UINT8 u1Pidx,
                                DMX_PID_COUNTERS_T* const prCounters);

EXTERN BOOL _DMX_ResetPidCounters(UINT8 u1Pidx);

EXTERN BOOL _DMX_IsPidEnabled(UINT8 u1Pidx);

EXTERN DMX_SCRAMBLE_STATE_T _DMX_GetScrambleState(UINT8 u1Pidx);

EXTERN DMX_SCRAMBLE_STATE_T _DMX_GetRawScrambleState(UINT8 u1Pidx);

EXTERN BOOL _DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType,
                                       UINT32 u4Bytes);

EXTERN BOOL _DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType,
                                       UINT32* pu4Bytes);

EXTERN BOOL _DMX_SetTsIndex(UINT8 u1TsIdx);

EXTERN BOOL _DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable);

EXTERN BOOL _DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs);

#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
EXTERN BOOL _DMX_EMU_SyncToggleBitInBypassCtrlWord(UINT8 u1TsIndex, UINT8 u1Pidx);
EXTERN void _GenerateRandomPattern(UINT32 u4Addr, UINT32 u4Size);

void _GenerateFixedPattern(UINT32 u4Addr,UINT32 u4Size);



EXTERN BOOL _MemCmp(UINT32 u4S1, UINT32 u4S2, UINT32 u4Size, UINT32* pu4DiffS1,
                    UINT32* pu4DiffS2);

EXTERN UINT32 _MemCmpRing(UINT32 u4S1, UINT32 u4Buf1Start, UINT32 u4Buf1End,
                          UINT32 u4S2, UINT32 u4Buf2Start, UINT32 u4Buf2End, UINT32 u4Size);

EXTERN BOOL _GenerateSimpleTsHeader(UINT32 u4Addr, UINT32 u4Size, UINT16 u2Pid,
                                    UINT32 u4PesAddr, UINT32* pu4PesSize);

EXTERN BOOL _GenerateSimpleTsHeaderEx(UINT32 u4TsAddr, UINT32 u4TsSize, UINT16 u2Pid,
                             UINT32 u4PesAddr, UINT32 *pu4PesSize, UINT8 *pu1ContinuityCounter);

EXTERN void _DumpBuffer(const UINT8 au1Buf[], UINT32 u4Size,
                        UINT32 u4BytesPerLine);
#endif
EXTERN void _GenerateRandomPattern2(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _GenerateSimpleTsHeader2(UINT32 u4Addr, UINT32 u4Size, UINT16 u2Pid,
                                    UINT32 u4PesAddr, UINT32* pu4PesSize);

EXTERN BOOL _DMX_CTInit(void);
EXTERN BOOL _DMX_CTSetPid(UINT8 u1Pidx, UINT32 u4Flags, PID_STRUCT_T *prPidStruct,
                          const DMX_PID_T* prPid, UINT32 *pu4PidEntry);
EXTERN BOOL _DMX_CTConfig(UINT8 u1Pidx, PID_STRUCT_T *prPidStruct, UINT32 *pu4PidEntry, BOOL fgEnable);
EXTERN void _DMX_CTSetIndex(UINT8 u1Pidx, UINT8 u1TsIdx, DMX_PID_T* prPid);
EXTERN BOOL _DMX_LoaduCode(void);
EXTERN BOOL _DMX_LoaduTestCode(void);
EXTERN BOOL _DMX_InitDiag(void);
EXTERN BOOL _DMX_GetMtdSecureKey(UINT8 *pu1Key);

// For checking AW support in the driver and the uCode.
EXTERN BOOL _DMX_DriverHasAW(void);
EXTERN BOOL _DMX_uCodeHasAW(void);

//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
EXTERN void _DMX_AwEraserCheck(UINT32 u4BufStart, UINT32 u4BufEnd,
                               UINT32 u4SecRp, UINT32 u4SecLen);
EXTERN void _DMX_AwEraserCheck_Init(void);
EXTERN void _DMX_AwEraserCheck_Free(void);
EXTERN void _DMX_AwEraserCheck_Set(BOOL fgEnable);
EXTERN INT32 _DMX_AwEraserCheck_Get(UINT32 u4Timeout,UINT8 u1TsIndex);
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

//
// LOG
//
EXTERN BOOL _DMX_LogOperation(UINT8 u1Flag, UINT32 u4SetInfo, UINT32 *pu4GetInfo);

EXTERN BOOL _DMX_TraceLog(UINT32 u4CallerAddr, CHAR *pszCaller,
                          UINT32 u4CalleeAddr, CHAR *pszCallee, UINT32 flag);

EXTERN BOOL _DMX_GetTraceLogFiles(UINT32 u4Tracex, DMX_TRACE_LOGFILE_T *prReturnTrace);

#ifdef CC_FBM_SUPPORT_DMXPID
EXTERN void* _Dmx_AllocPsiBuffer(UINT32 u4Size);
EXTERN BOOL _Dmx_FreePsiBuffer(void *pBuf);
#endif
//
// Debug error interrupts
//
#ifdef CC_DMX_TRACK_ERROR_INTS
EXTERN BOOL _DMX_RecordErrorInt(UINT8 u1TsIndex, UINT16 u2Pid, DMX_PID_TYPE_T ePidType, UINT32 u4ErrorInt, UINT32 u4ErrorStatus);

EXTERN BOOL _DMX_DumpErrIntRecords(void);

EXTERN BOOL _DMX_ResetErrIntRecords(void);

EXTERN BOOL _DMX_SaveDBMStatus(UINT32 u4Reason);

EXTERN BOOL _DMX_DumpDBMStatus(void);
#endif

//
// Loopback
//
EXTERN BOOL _DMX_Loopback_Init(void);
EXTERN BOOL _DMX_Loopback_Set(void);
EXTERN BOOL _DMX_Loopback_Free(void);
EXTERN BOOL _DMX_Loopback_Enable(BOOL fgEnable);

// dmx_recovery.h
EXTERN void _DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel);
EXTERN void _DMX_PanicMicroCode(void);
EXTERN void _DMX_PanicReset(void);
EXTERN BOOL _DMX_GetIsPanicReset(void);
EXTERN void _DMX_PanicDisplay(void);

// raw ts
EXTERN BOOL _DMX_InitRawTsHandler(void);
EXTERN BOOL _DMX_RawTsEnable(UINT8 u1Pidx, BOOL fgEnable);
EXTERN BOOL _DMX_RawTs_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
                                 UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);
EXTERN BOOL _DMX_RawTs_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size, UINT32 u4FrameAddr);

// DMX PID buffer
EXTERN void _DMX_PSIBufferStatus(void);

#ifdef CC_EIT_SECTION_FILTER
EXTERN VOID _DMX_Enable_EIT_FILTER(BOOL fgEnable);
EXTERN VOID _DMX_ENABLE_EIT_Filter_Auto_Insert(BOOL fgEnable);
EXTERN VOID _DMX_Print_EIT_Filter(VOID);
EXTERN BOOL _DMX_SetEITSection(SECTION_KEY_SET_T *prSetSection);
EXTERN EIT_FILTER_CHECK_RESULT _DMX_CheckEITFilter(SECTION_KEY_T *prSection, UINT32* pu4FltIndex);
EXTERN BOOL _DMX_EITFilter_VersionChange(UINT32 u4FltIndex, UINT8 u1TableId, UINT8 u1Version);
#endif

#ifdef CC_SUPPORT_STR
EXTERN void _DMX_pm_str_suspend(void);
EXTERN void _DMX_pm_str_resume(void);
#endif

//ciplus1.4 interface
EXTERN VOID _DMX_CI_Set_StreamMode(DMX_CI_STREAM_MODE_T e_StreamMode);
EXTERN VOID _DMX_CI_Set_MultiStream_TsIdx(UINT8 u1Tsidx);
EXTERN VOID _DMX_CI_Set_MultiStream_LocalTs(DMX_CI_LOCAL_TS_STRUCT_T* prLocalTs);
EXTERN VOID _DMX_CI_GetRecordBuffer(UINT32* pu4Addr, UINT32* pu4Size);
EXTERN BOOL _DMX_CI_Set_PvrBuffer(UINT8 u1TsoutIdx);
EXTERN VOID _DMX_CI_Record_Start(VOID);
EXTERN VOID _DMX_CI_Record_Stop(VOID);
EXTERN VOID _DMX_CiPlus14_Init(VOID);
EXTERN VOID _DMX_CI_SetTsoutPath(UINT8 u1TsoutIdx);
EXTERN BOOL _DMX_CI_OutputTwobitsTest(UINT8 u1TsIn, UINT8 u1TsOut, UINT8 u1DataWidth, UINT32 u4Size);


//Ts out interface

EXTERN BOOL _DMX_TsOut_SetLatchEdge(UINT8 u1TsOutIdx, DMX_TSOUT_LATCH_EDGE_T eEdge);
EXTERN BOOL _DMX_TsOut_SetValidCycle(UINT8 u1TsOutIdx, UINT32 u4Cycle);
EXTERN BOOL _DMX_TsOut_SetSyncCycle(UINT8 u1TsOutIdx, DMX_TSOUT_SYNC_CYCLE_T eSyncCycle);
EXTERN BOOL _DMX_TsOut_SetDataWidth(UINT8 u1TsOutIdx, DMX_TSOUT_DATAWIDTH_T eDataWidth);
EXTERN BOOL _DMX_TsOut_SetClockInvertGating(UINT8 u1TsOutIdx, BOOL fgInvert, BOOL fgGating);
EXTERN BOOL _DMX_TsIn_SetDataWidth(UINT8 u1TsIndex, DMX_TSOUT_DATAWIDTH_T eDataWidth);
EXTERN BOOL _DMX_TsOut_SetEndianess(UINT8 u1TsOutIdx, DMX_TSOUT_ENDIAN_T eEndian);
EXTERN BOOL _DMX_TsOut_SetRountDBMTsout(UINT8 u1TsOutIdx, BOOL Enable_DBM, BOOL Enable_Tsout);
EXTERN VOID _DMX_TsOut_SetClock(UINT8 u1TsOutIdx, DMX_PVRPLAY_OUTPUT_CLOCK_T eClock);
EXTERN BOOL _DMX_TsIn_SetRountMux(UINT8 u1TsIdx, UINT8 u1TsOutIdx);
EXTERN BOOL _DMX_SetTsOut(UINT8 u1TsIndex, DMX_TSOUT_PATH_T ePath);

#if defined(CC_DMX_EMULATION) || defined(__MODEL_slt__)
EXTERN BOOL _DMX_TsOut_TransmitData(UINT8 u1TsIndex, UINT8 u1TsoutIdx, UINT32 u4Size);
EXTERN BOOL _DMX_TsOut_SetRoute(UINT8 u1TsIndex, UINT8 u1TsoutIdx, DMX_TSOUT_ROUTE_T eRoute);
EXTERN BOOL _DMX_TsOut_Config(UINT8 u1TsOutSel, UINT8 u1TsOutindex);
EXTERN BOOL _DMX_PVRPlay_SetTSOut(UINT8 u1TsOutIdx, BOOL fgEnable);
#endif //(CC_DMX_EMULATION) || defined(__MODEL_slt__)
#endif  // DMX_H


