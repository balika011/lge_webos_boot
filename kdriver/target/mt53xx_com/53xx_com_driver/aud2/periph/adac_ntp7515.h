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
 * $RCSfile: adac_apogee.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adac_apogee.h
 *  Brief of file adac_apogee.h. \n
 */

#ifndef ADAC_NTP7515_H
#define ADAC_NTP7515_H

#include "x_typedef.h"
#include "aud_if.h"

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#ifdef CC_AUD_AMP_IN_I2C_BUS_3
#define AUD_AMP_BUS_ID          3
#elif defined (CC_AUD_AMP_IN_I2C_BUS_2)
#define AUD_AMP_BUS_ID          2
#elif defined (CC_AUD_AMP_IN_I2C_BUS_1)
#define AUD_AMP_BUS_ID          1
#else
#define AUD_AMP_BUS_ID          0
#endif

#define NTP7515_ADDR                0x54    //LR
#define NTP7515_ADDR_SW         0x56    //Woofer
#define SIF_CLK_DIV                                0x100

/*
#define NTP_SM 0x0F         // Soft Mute Control 0
    #define SM_INC 0
    #define SM_DEC 1

#define NTP_POF 0x10      //PWM Switching On/Off Control
    #define POF_ON 0
    #define POF_OFFDEC 1

#define NTP_PWM_MASK0 0x11      //PWM mask control 0
    #define PWMM_H     0

#define NTP_MASTER_VOL    0x17
    #define MASTER_VOL_0DB                        (0xCF)
    #define MASTER_VOL_MUTE                      (0x0)
*/

	/****************************/

#define NTP_GF 0x66		//I2C Glitch Filter
	#define GF_GW		(0x0F)		//glitch width    0x0F

#define NTP_DC	0x5D		//Driver Control
	#define DC_SHDN_PIN_L	0	//SHND pin go to low
	#define DC_SHDN_PIN_H	1	//SHND pin go to high

#define NTP_BQFCP	0x7E	//Bi-Quad Filter Coefficient Page
	#define BQFCP_NM	(0x00)	//normal mode
	#define BQFCP_CH1	(0x01)	//Bi-Quad Filter Coefficient write enable for channel 1
	#define BQFCP_CH2	(0x02)
	#define BQFCP_CH12	(0x03)
	#define BQFCP_APEQ	(0x08)

#define NTP_L_DRC_LPF1_1	0x32
#define NTP_L_DRC_LPF1_2	0x33
#define NTP_L_DRC_LPF1_3	0x34
#define NTP_L_DRC_LPF1_4	0x35
#define NTP_L_DRC_LPF1_5	0x36

#define NTP_L_DRC_LPF2_1	0x37
#define NTP_L_DRC_LPF2_2	0x38
#define NTP_L_DRC_LPF2_3	0x39
#define NTP_L_DRC_LPF2_4	0x3A
#define NTP_L_DRC_LPF2_5	0x3B

#define NTP_H_DRC_HPF1_1	0x3C
#define NTP_H_DRC_HPF1_2	0x3D
#define NTP_H_DRC_HPF1_3	0x3E
#define NTP_H_DRC_HPF1_4	0x3F
#define NTP_H_DRC_HPF1_5	0x40

#define NTP_H_DRC_HPF2_1	0x41
#define NTP_H_DRC_HPF2_2	0x42
#define NTP_H_DRC_HPF2_3	0x43
#define NTP_H_DRC_HPF2_4	0x44
#define NTP_H_DRC_HPF2_5	0x45

#define NTP_S_DRC_SPF1_1	0x46
#define NTP_S_DRC_SPF1_2	0x47
#define NTP_S_DRC_SPF1_3	0x48
#define NTP_S_DRC_SPF1_4	0x49
#define NTP_S_DRC_SPF1_5	0x4A

#define NTP_S_DRC_SPF2_1	0x4B
#define NTP_S_DRC_SPF2_2	0x4C
#define NTP_S_DRC_SPF2_3	0x4D
#define NTP_S_DRC_SPF2_4 	0x4E
#define NTP_S_DRC_SPF2_5	0x4F

#define NTP_DRC_C_0	0x20	//DRC control
#define NTP_DRC_C_1 0x21
#define NTP_DRC_C_2 0x22
#define NTP_DRC_C_3	0x23
#define NTP_DRC_C_6	0x26
#define NTP_DRC_C_7	0x27
#define NTP_DRC_C_8	0x28
#define NTP_DRC_C_9	0x29
#define NTP_DRC_C_10	0x2A
#define NTP_DRC_C_11	0x2B
#define NTP_DRC_C_12	0x2C

#define NTP_CH1_VC	0x17	//ch1 volume control
#define NTP_CH2_VC	0x18
	#define CVC_V	(0xBD)	//Channel volume value

#define NTP_M_PWM_C	0x43	//Miscellaneous PWM Control
	#define M_PWM_C_MD 	(0x02)

#define NTP_CPVC	0x3C	//CH1&CH2 Prescaler Value Control
	#define CPVC_PS12	(0x7C)

#define NTP_BQ1_1	0x00
#define NTP_BQ1_2	0x01
#define NTP_BQ1_3	0x02
#define NTP_BQ1_4	0x03
#define NTP_BQ1_5	0x04

#define NTP_BQ2_1	0x05
#define NTP_BQ2_2	0x06
#define NTP_BQ2_3	0x07
#define NTP_BQ2_4	0x08
#define NTP_BQ2_5	0x09

#define NTP_BQ3_1	0x0A
#define NTP_BQ3_2	0x0B
#define NTP_BQ3_3	0x0C
#define NTP_BQ3_4	0x0D
#define NTP_BQ3_5	0x0E

#define NTP_BQ4_1	0x0F
#define NTP_BQ4_2	0x10
#define NTP_BQ4_3	0x11
#define NTP_BQ4_4	0x12
#define NTP_BQ4_5	0x13

#define NTP_BQ5_1	0x14
#define NTP_BQ5_2	0x15
#define NTP_BQ5_3	0x16
#define NTP_BQ5_4	0x17
#define NTP_BQ5_5	0x18

#define NTP_BQ6_1	0x19
#define NTP_BQ6_2	0x1A
#define NTP_BQ6_3	0x1B
#define NTP_BQ6_4	0x1C
#define NTP_BQ6_5	0x1D

#define NTP_BQ7_1	0x1E
#define NTP_BQ7_2	0x1F
#define NTP_BQ7_3	0x20
#define NTP_BQ7_4	0x21
#define NTP_BQ7_5	0x22

#define NTP_BQ8_1	0x23
#define NTP_BQ8_2	0x24
#define NTP_BQ8_3	0x25
#define NTP_BQ8_4	0x26
#define NTP_BQ8_5	0x27

#define NTP_BQ9_1	0x28
#define NTP_BQ9_2	0x29
#define NTP_BQ9_3	0x2A
#define NTP_BQ9_4	0x2B
#define NTP_BQ9_5	0x2C

#define NTP_BQ10_1	0x2D
#define NTP_BQ10_2	0x2E
#define NTP_BQ10_3	0x2F
#define NTP_BQ10_4	0x30
#define NTP_BQ10_5	0x31

#define NTP_BQ11_1	0x5C
#define NTP_BQ11_2	0x5D
#define NTP_BQ11_3	0x5E
#define NTP_BQ11_4	0x5F
#define NTP_BQ11_5	0x60

#define NTP_BQ12_1	0x61
#define NTP_BQ12_2	0x62
#define NTP_BQ12_3	0x63
#define NTP_BQ12_4	0x64
#define NTP_BQ12_5	0x65

#define NTP_LG	0x50	//Loudness Gain	0x50~0x52

#define NTP_APEQ_C0_1	0x00	//Coefficient 0 of APEQ 1~6
#define NTP_APEQ_C0_2	0x01
#define NTP_APEQ_C0_3	0x02
#define NTP_APEQ_C0_4	0x03
#define NTP_APEQ_C0_5	0x04
#define NTP_APEQ_C0_6	0x05

#define NTP_APEQ_C1_1	0x06
#define NTP_APEQ_C1_2	0x07
#define NTP_APEQ_C1_3	0x08
#define NTP_APEQ_C1_4	0x09
#define NTP_APEQ_C1_5	0x0A
#define NTP_APEQ_C1_6	0x0B

#define NTP_APEQ_C2_1	0x0C
#define NTP_APEQ_C2_2	0x0D
#define NTP_APEQ_C2_3	0x0E
#define NTP_APEQ_C2_4	0x0F
#define NTP_APEQ_C2_5	0x10
#define NTP_APEQ_C2_6	0x11

#define NTP_APEQ_C3_1	0x12
#define NTP_APEQ_C3_2	0x13
#define NTP_APEQ_C3_3	0x14
#define NTP_APEQ_C3_4	0x15
#define NTP_APEQ_C3_5	0x16
#define NTP_APEQ_C3_6	0x17

#define NTP_APEQ_C4_1	0x1F
#define NTP_APEQ_C4_2	0x20
#define NTP_APEQ_C4_3	0x21
#define NTP_APEQ_C4_4	0x22
#define NTP_APEQ_C4_5	0x23
#define NTP_APEQ_C4_6	0x24

#define NTP_APEQ_PO	0x19	//APEQ Path Option
	#define APEQ_PO_V	(0x00)	//Path Option Value

#define NTP_APEQ_FC2	0x1A	//APEQ Filter Control 2 for APEQ1(BQ7)
	#define APEQ_FC2_V	(0x10)
	
#define NTP_APEQ_FC3	0x1B	//APEQ Filter Control 3 for APEQ2(BQ8)
	#define APEQ_FC3_V	(0x10)
	
#define NTP_APEQ_FC4	0x1C	//APEQ Filter Control 4 for APEQ3(BQ9)
	#define APEQ_FC4_V	(0x10)
	
#define NTP_APEQ_FC5	0x1D	//APEQ Filter Control 5 for APEQ4(BQ10)
	#define APEQ_FC5_V	(0x10)
	
#define NTP_APEQ_FC6	0x1E	//APEQ Filter Control 6 for APEQ5(BQ11)
	#define APEQ_FC6_V	(0x10)
	
#define NTP_APEQ_FC7	0x1F	//APEQ Filter Control 7 for APEQ6(BQ12)
	#define APEQ_FC7_V	(0x10)
	
#define NTP_PEQ_FC0_CH1	0x0E	//PEQ Filter Control 0 for Channel 1
	#define PEQ_FC0_CH1_V	(0x00)

#define NTP_PEQ_FC0_CH2	0x0F	//PEQ Filter Control 0 for Channel 2
	#define PEQ_FC0_CH2_V	(0x00)

#define NTP_PEQ_FC1_CH1	0x10	//PEQ Filter Control 1 for Channel 1
	#define PEQ_FC1_CH1_V	(0x00)
	
#define NTP_PEQ_FC1_CH2	0x11	//PEQ Filter Control 1 for Channel 2
	#define PEQ_FC1_CH2_V	(0x00)

#define NTP_APEQ_FC0_CH1	0x12	//PEQ Filter Control 0 for Channel 1
	#define APEQ_FC0_CH1_V	(0x00)
	
#define NTP_APEQ_FC0_CH2	0x13	//PEQ Filter Control 0 for Channel 2
	#define APEQ_FC0_CH2_V	(0x00)
	
#define NTP_APEQ_FC1_CH1	0x14	//PEQ Filter Control 1 for Channel 1
	#define APEQ_FC1_CH1_V	(0x00)
		
#define NTP_APEQ_FC1_CH2	0x15	//PEQ Filter Control 1 for Channel 2
	#define APEQ_FC1_CH2_V	(0x00)

#define NTP_PWM_PC	0x68	//PWM Phase Control
	#define PWM_PC_V	(0x00)

#define NTP_SSRC_C0	0x60	//SSRC Control 0
	#define SSRC_C0_V	(0x28)

#define NTP_SSC0	0x4A	//Soft Start Control 0
	#define SSC0_V	(0x00)

#define NTP_SSC1	0x4B
	#define SSC1_V	(0x01)

#define NTP_SSRHC1	0x70
	#define SSRHC1_V	(0x00)

#define NTP_AD_DC_PC0	0x50	//AD DC Protection Control 0
	#define AD_DC_PC0_V	(0x22)

#define NTP_D_BTL_DC_PC1	0x51	//D-BTL DC Protection Control 1
	#define D_BTL_DC_PC1_V	(0x00)

#define NTP_DC_PC2	0x52	//DC Protection Control 2
	#define DC_PC2_V	(0x00)

#define NTP_PWM_MASK_C0	0x35	//PWM_MASK Control 0
	#define PWM_MASK_C0_FPMLD	(0x04)
	#define PWM_MASK_C0_PWMM_FPMLD	(0x06)

#define NTP_PWM_SOOC	0x34	//PWM Switching On/Off Control
	#define PWM_SOOC_POF1	(0x00)
	#define PWM_SOOC_POF12	(0x03)

#define NTP_SMC	0x33	//Soft Mute Control
	#define SMC_SM1	(0x00)
	#define SMC_SM12	(0x03)

#define NTP_MV_SPK_PWM_SOOC	0x0C	//Master Volume & SPK PWM Switching On/Off Control
	#define MV_SPK_PWM_SOOC_V	(0xFF)

#define NTP_MVFC	0x16	//Master Volume Fine Control
	#define MVFC_V	(0x00)

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

extern AMP_T* _AMP_GetAmpNTP7515FunTbl(void);
	
#endif /* ADAC_STA339_H*/

