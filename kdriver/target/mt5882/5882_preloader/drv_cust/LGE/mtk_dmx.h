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
#ifndef MTK_DMX_H
#define MTK_DMX_H

#include "dmx_if.h"

// The input pin set
/*************************************************************
1: DMX_PINMUX_EXT_S = 0x10            //Serial
        //PAD_DEMOD_TSCLK
        //PAD_DEMOD_TSVAL
        //PAD_DEMOD_TSSYNC
        //PAD_DEMOD_TSDATA0

2. DMX_PINMUX_EXT_P = 0x11            //Parallel
        //PAD_DEMOD_TSCLK    -->TSCLK
        //PAD_DEMOD_TSVAL    -->TSVAL
        //PAD_DEMOD_TSSYNC  -->TSSYNC
        //PAD_CI_TSCLK           -->TSDATA1
        //PAD_CI_TSVAL           -->TSDATA2
        //PAD_CI_TSSYNC         -->TSDATA3
        //PAD_CI_TSDATA0        -->TSDATA4
        //PAD_PVR_TSCLK          -->TSDATA5
        //PAD_PVR_TSVAL          -->TSDATA6
        //PAD_PVR_TSSYNC        -->TSDATA7
    
3: DMX_PINMUX_EXT_1_S = 0x20      //Serial
        //PAD_GPIO15(CI_MCLKI)-->CLK
        //PAD_GPIO16(CI_VAL)-->VALID
        //PAD_GPIO17(CI_MISTRT)-->SYNC
        //PAD_GPIO18(CI_MDI0)-->DATA0
    
4: DMX_PINMUX_EXT_1_P = 0x21          //Parallel
        //PAD_GPIO15(CI_MCLKI)-->CLK
        //PAD_GPIO16(CI_VAL)-->VALID
        //PAD_GPIO17(CI_MISTRT)-->SYNC
        //PAD_GPIO18(CI_MDI0)-->DATA0
        //PAD_GPIO19(CI_MDI1)-->DATA1
        //PAD_GPIO20(CI_MDI2)-->DATA2
        //PAD_GPIO21(CI_MDI3)-->DATA3
        //PAD_GPIO22(CI_MDI4)-->DATA4
        //PAD_GPIO23(CI_MDI5)-->DATA5
        //PAD_GPIO24(CI_MDI6)-->DATA6
        //PAD_GPIO25(CI_MDI7)-->DATA7
        
5: DMX_PINMUX_EXT2_0_S = 0x30       //Serial
        //PAD_GPIO15(CI_MCLKI) --> TSCLK
        //PAD_GPIO16(CI_VAL) --> TSVAL
        //PAD_GPIO17(CI_MISTRT) --> TSSYNC
        //PAD_GPIO18(CI_MDI0)--> TSDATA0
            
6: DMX_PINMUX_EXT2_0_P = 0x31     //Serial
        //PAD_GPIO15(CI_MCLKI)-->CLK
        //PAD_GPIO16(CI_VAL)-->VALID
        //PAD_GPIO17(CI_MISTRT)-->SYNC
        //PAD_GPIO18(CI_MDI0)-->DATA0
        //PAD_GPIO19(CI_MDI1)-->DATA1
        //PAD_GPIO20(CI_MDI2)-->DATA2
        //PAD_GPIO21(CI_MDI3)-->DATA3
        //PAD_GPIO22(CI_MDI4)-->DATA4
        //PAD_GPIO23(CI_MDI5)-->DATA5
        //PAD_GPIO24(CI_MDI6)-->DATA6
        //PAD_GPIO25(CI_MDI7)-->DATA7
            
7: DMX_PINMUX_EXT2_1_S = 0x40     //Serial
        //PAD_CI_TSCLK -->CLK
        //PAD_CI_TSVAL -->VALID
        //PAD_CI_TSSYNC -->SYNC
        //PAD_SPI_CLK -->DATA0   
        
8: DMX_PINMUX_EXT2_2_S = 0x50     //Serial
        //PAD_CI_TSCLK -->CLK
        //PAD_CI_TSVAL -->VALID
        //PAD_CI_TSSYNC -->SYNC
        //PAD_CI_TSDATA0 -->DATA0
    
9: DMX_PINMUX_EXT2_3_S = 0x60     //Serial
        //PAD_CI_TSDATA0  --> SYNC
        //PAD_PVR_TSCLK    --> VAL
        //PAD_PVR_TSSYNC  --> CLK
        //PAD_GPIO34          --> DATA0
    
10: DMX_PINMUX_EXT3_S = 0x70    //Serial
        //PAD_GPIO34(CI_MDO0) -->CLK
        //PAD_GPIO35(CI_MDO1) -->VALID
        //PAD_GPIO36(CI_MDO2) -->SYNC
        //PAD_GPIO37(CI_MDO3) -->DATA0
         
11: DMX_PINMUX_EXT4_S = 0x80        //Serial
        //PAD_GPIO38(CI_MDO4) -->CLK
        //PAD_GPIO39(CI_MDO5) -->VALID
        //PAD_GPIO40(CI_MDO6) -->SYNC
        //PAD_GPIO41(CI_MDO7) -->DATA0
**************************************************************/

// Framer Mode
//  DMX_FRAMER_SERIAL: 1 bit data
//  DMX_FRAMER_PARALLEL: 8 bits data
//  DMX_FRAMER_TWOBIT: 2 bits data for MT5135

// Edge Setting
//  DMX_EDGE_POSITIVE: positive edge sampling
//  DMX_EDGE_NEGATIVE: negative edge sampling

// Sync Control
//  DMX_SYNC_INTERNAL: sync by data is 0x47
//  DMX_SYNC_EXTERNAL: sync by sync singal

static const DMX_FRONTEND_CONFIG_T _arMtkFeCfgTable[] =
{
#ifdef CC_EXTERNAL_DEMOD
#ifdef CC_ISDB_SUPPORT
    // Frontend input path 0
    { TRUE, DMX_PINSET_EXT1, DMX_FRAMER_SERIAL, DMX_EDGE_NEGATIVE, DMX_SYNC_INTERNAL},
#else
    // Frontend input path 0/***L5L?***/
    { TRUE, DMX_PINSET_EXT1, DMX_FRAMER_SERIAL, DMX_EDGE_POSITIVE, DMX_SYNC_INTERNAL},
#endif
#else
    // Frontend input path 0
    { TRUE, DMX_PINSET_INTERNAL, DMX_FRAMER_PARALLEL, DMX_EDGE_POSITIVE, DMX_SYNC_INTERNAL},
#endif
    // Frontend input path 1
    { FALSE, DMX_PINSET_EXT2_0, DMX_FRAMER_PARALLEL, DMX_EDGE_POSITIVE, DMX_SYNC_INTERNAL},
    // PVR path
    { FALSE, DMX_PINSET_INTERNAL, DMX_FRAMER_SERIAL, DMX_EDGE_POSITIVE, DMX_SYNC_INTERNAL},
    // Frontend input path 2
    { FALSE, DMX_PINSET_EXT1, DMX_FRAMER_TWOBIT, DMX_EDGE_POSITIVE, DMX_SYNC_INTERNAL},
};

#define DMX_FRONTEND_CONFIG_TABLE     (&_arMtkFeCfgTable)
#define DMX_FRONTEND_CONFIG_SIZE      (sizeof(_arMtkFeCfgTable)/sizeof(DMX_FRONTEND_CONFIG_T))

static const DMX_SCRAMBLE_CONFIG_T _rMtkDmxScrambleConfig =
{
    DMX_SCRAMBLE_PESSTART_ONLY,   // DMX_SCRAMBLE_TSFLAG_ONLY
    0,                            // If
};

#define DMX_SCRAMBLE_CONFIG_TABLE     (&_rMtkDmxScrambleConfig)

#endif /* MTK_DMX_H */
