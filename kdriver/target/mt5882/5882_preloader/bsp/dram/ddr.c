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
/** @file ddr.c
 *  Dram control and setting.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "ddr.h"
#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_serial.h"
#include "x_dram.h"
#include "x_os.h"

LINT_EXT_HEADER_END

#ifdef CC_MTK_PRELOADER
#if 0
static void _DdrSetDramRefresh(void)
{
    UINT32 u4Clk = TCMGET_DDR_CLK();
    UINT32 u4MemClk;
    UINT32 u4RefreshCount;

    u4MemClk = u4Clk/2;

    // 78: tREFI = 7.8 us. 39: tREFI = 3.9 us.
    // Set 50: tREFI = 5.0 us.
    u4RefreshCount = ((u4MemClk / 1000000) * 50) / 160;       

    // Full frequency mode when DDRI, SDR and frequency < 400Mhz.
    u4RefreshCount = u4RefreshCount / 2;

    u4RefreshCount &= 0xFF;
    
    // Set REFCNT by tREFI and enable ADVREFEN.
    DRAM_WRITE32(0x08, 0x200 |u4RefreshCount);
}
#endif

UINT32 _DDR_CheckSize(BOOL fgChBActive)
{  
    register UINT32 u4DramSize = 0;   // 64 Mbytes. 
    register UINT32 u4val;


    u4val=(HAL_READ32(0xF000D748)>>12) & 0x1;
	if(u4val==0x1)
		{
			DRAM_WRITE32(0x04, DRAM_READ32(0x04)|0x00000002);//Enable Asymmetric mode
			DRAM_WRITE32(0x04, DRAM_READ32(0x04)|0x00000010);//Enable 4G+2G function
			//For CS/CSX async in dramc
			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PERFCTL0), 0x00000000);
			mcDELAY_us(1);
			
			//16bit for Low address, 32bit for high address 
			DRAM_ARB_WRITE32(0x0, DRAM_ARB_READ32(0x0)|(1<<25));
			u4DramSize = 0x30000000;
		}
	else
		{
			u4DramSize = 0x40000000;
		}

    return u4DramSize;
}


//----------------------------------------------------------------------------
/* DDR_SetDramController() to set DRAM controller parameter
 *  @param u4Clk    Data rate divided by 2
 */
//----------------------------------------------------------------------------
void DDR_SetDramController()
{    
#if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG)
    // User's chance to modify the default settings at TCM_DRAM_SETUP.
    //CLK_QueryDramSetting((UINT8 *)"scan clock-cmd/addr phase relation:", DRAM_CHANGE_CLK_PI_SETTING);
#endif /* #if defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG) */

	DramcInit();
	DramcConfig();
	//_DdrSetDramRefresh();
}

#else /* #ifdef CC_MTK_PRELOADER */
//-----------------------------------------------------------------------------
/** DDR_SetAgentPriority() Set dram group 1 agent priorities
 *  @param pu1Priorities   Priorities of aud, dmx, fci, vbi, osd, pscan,
 *                         b2r, cpu, scpos
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetAgentPriority(const UINT8 *pu1Prio)
{
    UINT64 u8Priority = 0;
    INT32 i;
    UINT8 u1Prio;

    ASSERT(pu1Prio != NULL);

    for (i = 0; i < 16; i++)
    {
        u1Prio = pu1Prio[i];
        // '1' ~ '9' ==> Pri 1 ~ 9
        if ((u1Prio <= (UINT8)'9') && (u1Prio >= (UINT8)'0'))
        {
            u1Prio -= (UINT8)'0';
        }
        // 'A' ==> Pri 10
        else if ((u1Prio == (UINT8)'A') || (u1Prio == (UINT8)'a'))
        {
            u1Prio = 10;
        }
        // 'B' ==> Pri 11
        else if ((u1Prio == (UINT8)'B') || (u1Prio == (UINT8)'b'))
        {
            u1Prio = 11;
        }
        // 'C' ==> Pri 12
        else if ((u1Prio == (UINT8)'C') || (u1Prio == (UINT8)'c'))
        {
            u1Prio = 12;
        }
        else if ((u1Prio == (UINT8)'D') || (u1Prio == (UINT8)'d'))
        {
            u1Prio = 13;
        }
        else if ((u1Prio == (UINT8)'E') || (u1Prio == (UINT8)'e'))
        {
            u1Prio = 14;
        }
        else if ((u1Prio == (UINT8)'F') || (u1Prio == (UINT8)'f'))
        {
            u1Prio = 15;
        }
    //    else if (i == 9)    // backward compatible, for only 9 agents
    //    {
   //         u1Prio = 9;
    //    }
        else
        {
            ASSERT(0);
            return;
        }

        u8Priority |= (((UINT64)u1Prio) << (4 * i));
    }

    // Set agent 0 ~ 7 of group 1 static priority.
    IO_WRITE32(DRAM_DMARB_BASE, 0x10, (UINT32)(u8Priority & 0xffffffff));

    // Set agent 8 ~ 15 of group 1 static priority.
    IO_WRITE32(DRAM_DMARB_BASE, 0x14, (UINT32)((u8Priority>>32) & 0xffffffff));

	IO_WRITE32(DRAM_DMARB_BASE, 0x6c, IO_READ32(DRAM_DMARB_BASE, 0x6c) | 0x1);

    // Set dynamic priority control.
    // dynamic priority on NR: 0x20007128[13:0] <= 14'h2031
    // DRAM_WRITE32(0x128, (DRAM_READ32(0x128) & ~(0x3fffU << 0)) | (0x2031U << 0));
    // dynamic priority on Scalar: 0x2000712c[29:16] <= 14'h2031
    // DRAM_WRITE32(0x12c, (DRAM_READ32(0x12c) & ~(0x3fffU << 16)) | (0x2031U << 16));
    // dynamic priority on VDEC: 0x20007130[13:0] <= 14'h2031
    // DRAM_WRITE32(0x130, (DRAM_READ32(0x130) & ~(0x3fffU << 0)) | (0x2031U << 0));
    // Enable dynamic control.
    //DRAM_WRITE32(0x150, (DRAM_READ32(0x150) | 0x10000));
}

//-----------------------------------------------------------------------------
/** DDR_SetBurstLen() Set dram group 1 agent burst length
 *  @param u4BurstLen1     Burst length of agent 0 ~ 7
 *  @param u4BurstLen2     Burst length of agent 8 ~ 16
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetBurstLen(UINT32 u4BurstLen1, UINT32 u4BurstLen2)
{
	//Channel A burst length setting
	IO_WRITE32(DRAM_DMARB_BASE, 0x20, u4BurstLen1);
    IO_WRITE32(DRAM_DMARB_BASE, 0x24, u4BurstLen2);
	//Channel A burst length setting
}

//-----------------------------------------------------------------------------
/** DDR_SetArbitorTime() Set dram group arbiter time
 *  @param u1Group         Dram agent group (1,2,3)
 *  @param u1Time          Arbitor time (0 ~ 15)
 *  @retval none.
 */
//-----------------------------------------------------------------------------
void DDR_SetArbiterTime(UINT8 u1Group, UINT8 u1Time)
{
    UINT32 u4RegVal;

    if (u1Time > 15)
    {
        u1Time = 15;
    }

    u4RegVal = IO_READ32(DRAM_DMARB_BASE, 0x60);
    switch(u1Group)
    {
    case 1:
        u4RegVal &= 0xff0fffff;
        u4RegVal |= (((UINT32)u1Time) << 20);
        break;
    case 2:
        u4RegVal &= 0xf0ffffff;
        u4RegVal |= (((UINT32)u1Time) << 24);
        break;
    case 3:
        u4RegVal &= 0x0fffffff;
        u4RegVal |= (((UINT32)u1Time) << 28);
        break;
    default:
        return;
    }
    IO_WRITE32(DRAM_DMARB_BASE, 0x60,u4RegVal);
}

#endif

