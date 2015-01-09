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
 * $RCSfile: irtx_if.c$
 * $Revision: #1 $
 *
 *--------------------------------------------------------------------------*/

/** @file irtx_if.c
 *  irtx_if.c provides IRTX export interface functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_debug.h"
#include "x_hal_io.h"

#include "irtx_if.h"
#if defined(CC_MT5398)|| defined(CC_MT5880)|| defined(CC_MT5881)|| defined(CC_MT5399)
#include "hw_irtx.h"
#endif
#ifndef CC_MTK_LOADER
#include "x_os.h"
#else /* CC_MTK_LOADER */
extern VOID* x_memcpy (VOID*        pv_to,
                       const VOID*  pv_from,
                       SIZE_T       z_len);
#endif /* CC_MTK_LOADER */
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


#if SUPPORT_IRTX_3D

void vSet3DOpenDrain(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucMode)?1:0),FLD_IRTX1_OPEN_DR);
}

void vSet3DBitNumber(UCHAR ucNumber)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,ucNumber,FLD_IRTX1_BITNUM);
}

void vSet3DIrInver(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucMode)?1:0),FLD_IRTX1_IRINV);
}

void vSet3DIgnoreLeft(UCHAR ucLeft)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucLeft)?1:0),FLD_IRTX1_IGNORE_LEFT);
}

void vSet3DIgnoreRight(UCHAR ucRight)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucRight)?1:0),FLD_IRTX1_IGNORE_RIGHT);
}

void vSet3DLogic1AlwaysH(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucMode)?1:0),FLD_IRTX1_L1L_VALUE);
}

void vSet3DLogic0AlwaysL(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucMode)?1:0),FLD_IRTX1_L0H_VALUE);
}

void vToggle3DStart(void)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,1,FLD_IRTX1_STRT);
}

void vSet3DSWMode(UINT32 u4Data)
{
    UINT32 u4Config;

    //update data
    vIO32Write4B(IRTX_IRTX1D,u4Data);
	
    //set mode
    u4Config = (INT32)u4IO32Read4B(IRTX_IRTX1CFG);
    vIO32Write4B(IRTX_IRTX1CFG, u4Config & 0xFFFFDFDF);//clear bit 13 and bit 5
}
void vSet3DHWMode(UINT32 u4Data0,UINT32 u4Data1,UINT32 u4Data2,UINT32 u4Data3)
{
    UINT32 u4Config;

    //set data
    vIO32Write4B(IRTX_IRTX1LOD,u4Data0);
    vIO32Write4B(IRTX_IRTX1LCD,u4Data1);
    vIO32Write4B(IRTX_IRTX1ROD,u4Data2);
    vIO32Write4B(IRTX_IRTX1RCD,u4Data3);
	
    //set mode
    u4Config = (INT32)u4IO32Read4B(IRTX_IRTX1CFG);
    vIO32Write4B(IRTX_IRTX1CFG, u4Config & 0xFFFFDFDF);//clear bit 13 and bit 5
    vIO32WriteFldAlign(IRTX_IRTX1CFG,1,FLD_IRTX1_HW_MODE);//set bit 5	
}
void vset3DExtMode(UINT32 u4Data0,UINT32 u4Data1)
{
    UINT32 u4Config;


    //set data
    vIO32Write4B(IRTX_IRTX1EXTL,u4Data0);
    vIO32Write4B(IRTX_IRTX1EXTR,u4Data1);

    //set mode
    u4Config = (INT32)u4IO32Read4B(IRTX_IRTX1CFG);
    vIO32Write4B(IRTX_IRTX1CFG, u4Config | 0x2020);//set bit 13 and bit 5
}
void vSet3DLogic0Pulse(UINT32 u4Hight,UINT32 u4Low)
{
    vIO32WriteFldAlign(IRTX_IRTX1_L0H,u4Hight,FLD_IRTX1_L0H);
    vIO32WriteFldAlign(IRTX_IRTX1_L0L,u4Low,FLD_IRTX1_L0L);
}
void vSet3DLogic1Pulse(UINT32 u4Hight,UINT32 u4Low)
{
    vIO32WriteFldAlign(IRTX_IRTX1_L1H,u4Hight,FLD_IRTX1_L1H);
    vIO32WriteFldAlign(IRTX_IRTX1_L1L,u4Low,FLD_IRTX1_L1L);
}
void vSet3DSync(UINT32 u4Hight,UINT32 u4Low)
{
    vIO32WriteFldAlign(IRTX_IRTX1_SYNCH,u4Hight,FLD_IRTX1_SYNCH);
    vIO32WriteFldAlign(IRTX_IRTX1_SYNCL,u4Low,FLD_IRTX1_SYNCL);
}
void vSet3DCarrier(UCHAR ucEnable,UINT16 u2Duty,UINT16 u2Period)
{
    vIO32WriteFldAlign(IRTX_IRTX1CFG,((TRUE == ucEnable)?1:0),FLD_IRTX1_IROS);
    vIO32WriteFldAlign(IRTX_IRTX1MT,u2Duty,FLD_IRTX1_CDT);
    vIO32WriteFldAlign(IRTX_IRTX1MT,u2Period,FLD_IRTX1_CWT);
}

#if defined(CC_MT5881) || defined(CC_MT5399)

void vSet3DHWDelay(UINT32 u4LODelay,UINT32 u4LCDelay,UINT32 u4RODelay,UINT32 u4RCDelay)
{
	vIO32WriteFldAlign(IRTX_IRTX1HWLOD,u4LODelay,FLD_IRTX1_HW_LODELAY);
	vIO32WriteFldAlign(IRTX_IRTX1HWLCD,u4LCDelay,FLD_IRTX1_HW_LCDELAY);
	vIO32WriteFldAlign(IRTX_IRTX1HWROD,u4RODelay,FLD_IRTX1_HW_RODELAY);
	vIO32WriteFldAlign(IRTX_IRTX1HWRCD,u4RCDelay,FLD_IRTX1_HW_RCDELAY);
}
void vSet3DSWDelay(UINT32 u4SWDelay)
{
	vIO32WriteFldAlign(IRTX_IRTX1SWD,u4SWDelay,FLD_IRTX1_SW_DELAY);
}

#endif

#endif

#if SUPPORT_IRTX_REMOTE

void vSetRemoteBitNumber(UCHAR ucNumber)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,ucNumber,FLD_IRTX2_BITNUM);
}

void vSetRemoteIrInver(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,((TRUE == ucMode)?1:0),FLD_IRTX2_IRINV);
}

void vSetRemoteRegisterOrd(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,((TRUE == ucMode)?1:0),FLD_IRTX2_RODR);
}

void vSetRemoteBitOrd(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,((TRUE == ucMode)?1:0),FLD_IRTX2_BODR);
}

void vSetRemoteSWOutbit(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,((TRUE == ucMode)?1:0),FLD_IRTX2_SWO);
}

void vSetRemoteMode(UCHAR ucMode)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG, ucMode,FLD_IRTX2_MODE);
}

void vToggleRemoteStart(void)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG, 1,FLD_IRTX2_STRT);
}

void vSetRemoteData(UINT32 u4Data0,UINT32 u4Data1,UINT32 u4Data2)
{
    vIO32Write4B(IRTX_IRTX2D0,u4Data0);
    vIO32Write4B(IRTX_IRTX2D1,u4Data1);
    vIO32Write4B(IRTX_IRTX2D2,u4Data2);
}


void vSetRemoteSync(UINT32 u4Hight,UINT32 u4Low)
{
    vIO32WriteFldAlign(IRTX_IRTX2SYNCH, u4Hight,FLD_IRTX2_SYNCH);
    vIO32WriteFldAlign(IRTX_IRTX2SYNCL, u4Low,FLD_IRTX2_SYNCL);
}

void vSetRemoteCarrier(UCHAR ucEnable,UINT16 u2Duty,UINT16 u2Period)
{
    vIO32WriteFldAlign(IRTX_IRTX2CFG,((TRUE == ucEnable)?1:0),FLD_IRTX2_IROS);
    vIO32WriteFldAlign(IRTX_IRTX2MT, u2Duty,FLD_IRTX2_CDT);
    vIO32WriteFldAlign(IRTX_IRTX2MT, u2Period,FLD_IRTX2_CWT);
}

void vSetRemoteModeNEC(void)
{ 
    vSetRemoteMode(IRRX_REMOTE_MODE_NEC);
    vSetRemoteBitNumber(0x21);
    vSetRemoteIrInver(TRUE);    
    vSetRemoteBitOrd(TRUE);
	
    vIO32WriteFldAlign(IRTX_IRTX2_L0H, 0x3B10,FLD_IRTX2_L0H);
    vIO32WriteFldAlign(IRTX_IRTX2_L0L, 0x3B10,FLD_IRTX2_L0L);
    vIO32WriteFldAlign(IRTX_IRTX2_L1H, 0x3B10,FLD_IRTX2_L1H);
    vIO32WriteFldAlign(IRTX_IRTX2_L1L, 0xB23E,FLD_IRTX2_L1L);

    vSetRemoteSync(0x3B4C0,0x1DA60);
    vSetRemoteCarrier(TRUE,0xED,0x2C7);
}


void vSetRemoteModeRC5(void)
{ 
    vSetRemoteMode(IRRX_REMOTE_MODE_RC5);
    vSetRemoteBitNumber(0xE);
    vSetRemoteIrInver(TRUE);
    vSetRemoteBitOrd(TRUE);
	
    vIO32WriteFldAlign(IRTX_IRTX2_L0H, 0x2EEF,FLD_IRTX2_L0H);
    vIO32WriteFldAlign(IRTX_IRTX2_L0L, 0x2EEF,FLD_IRTX2_L0L);
    vIO32WriteFldAlign(IRTX_IRTX2_L1H, 0x2EEF,FLD_IRTX2_L1H);
    vIO32WriteFldAlign(IRTX_IRTX2_L1L, 0x2EEF,FLD_IRTX2_L1L);

    vSetRemoteSync(0,0);
    vSetRemoteCarrier(TRUE,0xBB,0x2EE);
}


void vSetRemoteModeRC6(void)
{ 
    vSetRemoteMode(IRRX_REMOTE_MODE_RC6);
    vSetRemoteBitNumber(0x14);
    vSetRemoteIrInver(TRUE);    
    vSetRemoteBitOrd(TRUE);
	
    vIO32WriteFldAlign(IRTX_IRTX2_L0H, 0x5DDE,FLD_IRTX2_L0H);
    vIO32WriteFldAlign(IRTX_IRTX2_L0L, 0x2EEF,FLD_IRTX2_L0L);
    vIO32WriteFldAlign(IRTX_IRTX2_L1H, 0x2EEF,FLD_IRTX2_L1H);
    vIO32WriteFldAlign(IRTX_IRTX2_L1L, 0x2EEF,FLD_IRTX2_L1L);

    vSetRemoteSync(0,0);
    vSetRemoteCarrier(TRUE,0xBB,0x2EE);
}

void vSetRemoteModeSW(void)
{

}

#endif

#if defined(CC_SUPPORT_STR_CORE_OFF)
#if defined(CC_MT5881)||defined(CC_MT5399)
#define IRTX_REGISTER_LEN 32
#else
#define IRTX_REGISTER_LEN 27
#endif

#if (SUPPORT_IRTX_3D||SUPPORT_IRTX_REMOTE)
static UINT32 u4IrtxSuspendRegister[IRTX_REGISTER_LEN];
#endif

void IRTX_pm_suspend(void)
{
	#if (SUPPORT_IRTX_3D||SUPPORT_IRTX_REMOTE)
    x_memcpy((void*)u4IrtxSuspendRegister,(void*)(IRTX_IRTX1CFG),sizeof(u4IrtxSuspendRegister));
	#endif
}

void IRTX_pm_resume(void)
{	
	#if (SUPPORT_IRTX_3D||SUPPORT_IRTX_REMOTE)
    x_memcpy((void*)(IRTX_IRTX1CFG),(void*)(u4IrtxSuspendRegister),sizeof(u4IrtxSuspendRegister));
	#endif
}

#endif

void vIrtxSetting(void)
{
#if SUPPORT_IRTX_3D

	#if CUST_CONFIG
	#if 0
	//how to set pinmux ???
    UINT32 PINMUXCFG;
    PINMUXCFG = u4IO32Read4B(0xF00280B4);
    PINMUXCFG = (PINMUXCFG & 0xFC000000);
    if(PINMUXCFG != 0x24000000)
    {
        LOG(0,"IRTX pinmux set not GPIO8 & GPIO9 !\n");
    }
	#endif    
    vSet3DSync(0x0,0x0);
    vSet3DLogic0Pulse(0x190,0x137);
	vSet3DLogic1Pulse(0x190,0x137);
	vSet3DLogic0AlwaysL(TRUE);
    vSet3DBitNumber(6);
    vset3DExtMode(0xE0000000,0x0);   
	
	#endif
#endif

#if SUPPORT_IRTX_REMOTE

#endif


}









