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
 * Description:
 *      Memory test module
 *
 *---------------------------------------------------------------------------*/


#include "x_printf.h"
// #include "x_pinmux.h"
#include "x_assert.h"
#include "x_bim.h"
#include "memtest.h"

//---------------------------------------------------------------------------
// Constant definitions

#define EXT_GPIO_NUM                63


//---------------------------------------------------------------------------
// External declarations

extern void memtest(unsigned int uStart, unsigned int uSize, unsigned int uSeed);
extern void memintf(unsigned int uStart, unsigned int uSize);
extern void MEM_Test(UINT32 u4Round, UINT32 u4Addr, UINT32 u4Size);


//---------------------------------------------------------------------------
// Static variables


//---------------------------------------------------------------------------
// Static functions

static void _TickGpio(void)
{
#if 0
    static BOOL _fgOn = FALSE;
    INT32 fgEnable, fgOutput;
    // Select pin mux
    VERIFY(BSP_PinGpioSel(EXT_GPIO_NUM) == 0);

    // Enable output
    fgEnable = 1;
    GPIO_Enable(EXT_GPIO_NUM, &fgEnable);

    // Toggle GPIO output
    _fgOn = !_fgOn;
    fgOutput = _fgOn;
    GPIO_Output(EXT_GPIO_NUM, &fgOutput);
#endif
}


//---------------------------------------------------------------------------
// Interface

/*----------------------------------------------------------------------------
 * Function: MEM_Test
 *
 * Description:
 *      Memory test
 *
 * Inputs:
 *      u4Round: The number of rounds the test performs
 *      u4Addr: The address to be tested
 *      u4Size: The size to be tested
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void MEM_Test(UINT32 u4Round, UINT32 u4Addr, UINT32 u4Size)
{
    UINT32 u4Loop;

    BIM_WRITE32(0x158, 0x0);
    BIM_WRITE32(0x158, 0x11);

    for (u4Loop = 0; u4Loop < u4Round; u4Loop++)
    {
        Printf("\nRound %u\n", u4Loop);

        _TickGpio();

        memintf(u4Addr, u4Size);
        memtest(u4Addr, u4Size, u4Loop);

        if (!IS_SRAM(u4Addr))
        {
            memLineBounceTest(u4Addr, u4Size, 1);
            memButterflyTest(u4Addr, u4Size, 1);
        }
    }
    BIM_WRITE32(0x158, 0x0);

    Printf("ICache {request=%u, miss=%u, penalty=%u}\n",
           BIM_READ32(0x160), BIM_READ32(0x164), BIM_READ32(0x168));
    Printf("DCache {request=%u, miss=%u, penalty=%u}\n",
           BIM_READ32(0x16c), BIM_READ32(0x170), BIM_READ32(0x174));
}

