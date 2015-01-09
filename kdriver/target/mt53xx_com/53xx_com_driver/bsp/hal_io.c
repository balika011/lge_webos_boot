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
 * $RCSfile: hal_io.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal_io.c
 *  hal_1176.c provides functions of register access
 */


#include "x_hal_io.h"
#include "x_assert.h"

#if defined(CC_MT5890)
#include "x_hal_5381.h"
#include "hw_mlvds.h"
#include "hw_vbyone.h"
#include "hw_tg.h"
#endif

#ifdef CC_MTK_LOADER
    #undef x_crit_start
    #undef x_crit_end
    #define x_crit_start() 0
    #define x_crit_end(x) UNUSED(x)
    #define CRIT_STATE_T UINT8
#else
    #include "x_os.h"
#endif

#if defined(CC_MT5890)
static UINT32 VByOne_Register_Mapping_E3(UINT32 u4Register)
{

    switch (u4Register)
    {
    case VB1SYS0: return(MLVDS_CTRL_32);
        break;
    case VB1SYS1: return(MLVDS_CTRL_33);
        break;
    case VB1SYS2: return(MLVDS_CTRL_34);
        break;
	case VB1SYS3: return(MLVDS_CTRL_35);
		break;
	case VB1SYS4: return(MLVDS_CTRL_36);
		break;
	case VB1SYS6: return(MLVDS_CTRL_37);
		break;
	case VB1SYS8: return(MLVDS_CTRL_38);
		break;
	case VB1ANA0: return(MLVDS_CTRL_39);
		break;
	case VB1RES:  return(MLVDS_CTRL_40);
		break;
	case VB1ANA2: return(MLVDS_CTRL_41);
		break;
	case VB1ANA3: return(MLVDS_CTRL_42);
		break;
	case VB1ANA4: return(MLVDS_CTRL_43);
		break;
	case VB1ANA5: return(MLVDS_CTRL_44);
		break;
	case VB1ANA6: return(MLVDS_CTRL_45);
		break;
	case VB1ANA7: return(MLVDS_CTRL_46);
		break;
	case VB1ANA8: return(MLVDS_CTRL_47);
		break;
	case VB1OSDSYS0: return(MLVDS_CTRL_48);
		break;
	case VB1OSDSYS1: return(MLVDS_CTRL_49);
		break;
	case VB1OSDSYS2: return(MLVDS_CTRL_50);
		break;
	case VB1OSDSYS3: return(MLVDS_CTRL_51);
		break;
	case VB1OSDSYS4: return(MLVDS_CTRL_52);
		break;
	case VB1OSDSYS6: return(MLVDS_CTRL_53);
		break;
	case VB1OSDSYS8: return(MLVDS_CTRL_54);
		break;
	case VB1OSDANA0: return(MLVDS_CTRL_55);
		break;
	case VB1OSDRES:  return(MLVDS_CTRL_56);
		break;
	case VB1OSDANA2: return(MLVDS_CTRL_57);
		break;
	case VB1OSDANA3: return(MLVDS_CTRL_58);
		break;
	case VB1OSDANA4: return(MLVDS_CTRL_59);
		break;			
	case GR0: return(MLVDS_CTRL_60);
		break;	
	case GR1: return(MLVDS_CTRL_61);
		break;	
	case GR12: return(MLVDS_CTRL_62);
		break;	
		
    default:
        return (u4Register);		
    }
}
#endif

UINT16 u2IO32Read2B(UINT32 reg32)
{
    UINT32 addr=reg32&~3;
    switch(reg32&3)
    {
        default:
        case 0:
        case 2:
            return (*(volatile UINT16 *)(reg32));
        case 1:
            return  ((*(volatile UINT32 *)(addr))>>8)&0xffff;
        case 3:
            ASSERT((reg32&3)<3);
            return  ((*(volatile UINT32 *)(addr))>>24)&0xff;
    }
}

#if defined(FORCE_IO32_1BMSK_FUNCTION) || defined(CC_MTK_LOADER) || defined(CC_UBOOT) || !defined(__KERNEL__)
void vIO32Write1BMsk(UINT32 reg32, UINT32 val8, UINT8 msk8)
{
    CRIT_STATE_T csState;
    UINT32 u4Val, u4Msk;
    UINT8 bByte;
	#if defined(CC_MT5890)
	UINT32 u4MapAddress;
	#endif

    bByte = reg32&3;
    reg32 &= ~3;
    val8 &= msk8;
    u4Msk = ~(UINT32)(msk8<<((UINT32)bByte<<3));

    csState = x_crit_start();
	#if defined(CC_MT5890)
	if((((reg32>=VB1SYS0)&&(reg32<=VB1OSDANA4))||((reg32>=GR0)&&(reg32<=TIM5R7))) && IS_IC_5890_ES3())
	{
		u4MapAddress = VByOne_Register_Mapping_E3(reg32);
		u4MapAddress &= ~3;
	    u4Val = (*(volatile UINT32 *)(u4MapAddress));
	    u4Val = ((u4Val & u4Msk) | ((UINT32)val8 << (bByte<<3)));		
	    (*(volatile UINT32 *)(u4MapAddress)=(u4Val));
	}
	else
	#endif
	{
    u4Val = (*(volatile UINT32 *)(reg32));
    u4Val = ((u4Val & u4Msk) | ((UINT32)val8 << (bByte<<3)));
	}	
    (*(volatile UINT32 *)(reg32)=(u4Val));
    x_crit_end(csState);

}
#endif

void vIO32Write2BMsk(UINT32 reg32, UINT32 val16, UINT16 msk16)
{
    CRIT_STATE_T csState;
    UINT32 u4Val, u4Msk;
    UINT8 bByte;
	#if defined(CC_MT5890)
	UINT32 u4MapAddress;
	#endif

    bByte = reg32&3;
    ASSERT(bByte<3);

    reg32 &= ~3;
    val16 &= msk16;
    u4Msk = ~(UINT32)(msk16<<((UINT32)bByte<<3));

    csState = x_crit_start();
	#if defined(CC_MT5890)
	if((((reg32>=VB1SYS0)&&(reg32<=VB1OSDANA4))||((reg32>=GR0)&&(reg32<=TIM5R7))) && IS_IC_5890_ES3())
	{
		u4MapAddress = VByOne_Register_Mapping_E3(reg32);
		u4MapAddress &= ~3;
	    u4Val = (*(volatile UINT32 *)(u4MapAddress));
	    u4Val = ((u4Val & u4Msk) | ((UINT32)val16 << (bByte<<3)));
	    (*(volatile UINT32 *)(u4MapAddress)=(u4Val));	
	}
	else
	#endif
	{
    u4Val = (*(volatile UINT32 *)(reg32));
    u4Val = ((u4Val & u4Msk) | ((UINT32)val16 << (bByte<<3)));
	}
    (*(volatile UINT32 *)(reg32)=(u4Val));
    x_crit_end(csState);

}

void vIO32Write4BMsk(UINT32 reg32, UINT32 val32, UINT32 msk32)
{
    CRIT_STATE_T csState;
	#if defined(CC_MT5890)
	UINT32 u4MapAddress;
	#endif

    ASSERT((reg32&3)==0);
    val32 &=msk32;

    csState = x_crit_start();
	#if defined(CC_MT5890)
	if((((reg32>=VB1SYS0)&&(reg32<=VB1OSDANA4))||((reg32>=GR0)&&(reg32<=TIM5R7))) && IS_IC_5890_ES3())
	{
		u4MapAddress = VByOne_Register_Mapping_E3(reg32);
    	(*(volatile UINT32 *)(reg32)=((*(volatile UINT32 *)(u4MapAddress))&~msk32)|val32);		
		(*(volatile UINT32 *)(u4MapAddress)=((*(volatile UINT32 *)(u4MapAddress))&~msk32)|val32);		
	}
	else
	#endif
	{
    (*(volatile UINT32 *)(reg32)=((*(volatile UINT32 *)(reg32))&~msk32)|val32);
	}
    x_crit_end(csState);
}

