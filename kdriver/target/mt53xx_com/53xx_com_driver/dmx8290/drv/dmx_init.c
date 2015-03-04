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
 * $Date: 2015/03/04 $
 * $RCSfile: dmx_init.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_init.c
 *  Demux driver - initialization
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_drm_drvif.h"
#include "dmx_pcr.h"

#include "dmx_debug.h"
#include "x_lint.h"

#include "fvr.h"

#include "gcpu_if.h"
#include "drvcust_if.h"
#if defined(CC_TRUSTZONE_SUPPORT)
#include "tz_if.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_util.h"
#include "x_os.h"
#include "x_timer.h"
#include "x_bim.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Define EXT_TS2 to select the second external TS interface. By default the
// first external TS interface is selected
//
//#define EXT_TS2

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NUM_UP_TEST_RESULT_WORDS            16
#define DMX_MAX_DBM_WAIT_COUNT              10

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT32 _u4uCodeVersion = 0;

static BOOL _fgMicroProcessorStopped = FALSE;  // Is micro-processor stopped?

static DMX_FRONTEND_T _aeDmxFramerInput[DMX_FRAMER_COUNT];

static DMX_DBM_INPUT_SOURCE_T _eDmxDbm2Input = DMX_DBM_INPUT_PB;

static DMX_DBM_INPUT_SOURCE_T _eDmxDbm3Input = DMX_DBM_INPUT_FRAMER;

static DMX_DBM_INPUT_SOURCE_T _eDmxDBm4Input = DMX_DBM_INPUT_PB3;

static UINT16 _au2DmxFramerPktSize[DMX_FRAMER_COUNT];

static BOOL _afgNoTSValid[DMX_FRAMER_COUNT];
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _DdmxIsFTIRunning
 *  Query if FTI is running or not
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
static BOOL _DmxIsFTIRunning(void)
{
    UINT32 u4State;

    u4State = (DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3;
    LOG(7, "uP state %d\n", u4State);

    return 0 == u4State;
}


//-----------------------------------------------------------------------------
/** _DmxCkgenToggle
 */
//-----------------------------------------------------------------------------
static void _DmxCkgenToggle(UINT8 u1Framer)
{
//#ifndef CC_DMX_FPGA_TEST
    UINT32 i, u4Ctrl;
    UINT8 au1Toggle[4] = { 2, 10, 18, 26 };

    if (u1Framer >= DMX_FRAMER_COUNT)
    {
        return;
    }

#if defined(CC_MT5890)
    if(u1Framer == DMX_FRAMER_TOTAL_INDEX)
    {/*for dbm4 ,not need .*/
      return ;
    }
#else  //MT5882
    if(u1Framer == DMX_FRAMER_INDEX_2)
    {
      // this framer is removed.
      LOG(3, "framer %d is removed in dmx5882.\n", DMX_FRAMER_INDEX_2);
      return ;
    }
#endif
    
    // Force toggle clock to guarantee switch clock success
    u4Ctrl = CKGEN_READ32(CKGEN_TSCKCFG) & ~(0x1 << au1Toggle[u1Framer]);

    // Toggle 6 time is enough, but 10 for safe
    for (i=0; i<10; i++)
    {
        HAL_Delay_us(1);
        CKGEN_WRITE32(CKGEN_TSCKCFG, u4Ctrl | (1 << au1Toggle[u1Framer]));
        HAL_Delay_us(1);
        CKGEN_WRITE32(CKGEN_TSCKCFG, u4Ctrl);
    }
//#endif
}


//-----------------------------------------------------------------------------
/** _DmxCkgenInit
 *  Select clock and pin mux for FTI
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DmxCkgenInit(BOOL fgOn)
{
    UINT32 u4Ctrl = 0;
    UINT32 u4Reg;

#ifndef CC_DMX_FPGA_TEST
    if (fgOn)
    {
        CKGEN_WRITE32(CKGEN_BLOCK_CKENCFG1, CKGEN_READ32(CKGEN_BLOCK_CKENCFG1) | 0xC0000);
#if defined(CC_MT5890)
        CKGEN_WRITE32(CKGEN_DMX_CKCFG, 0x9);            // demux clock ,set to 432MHz
#else  //MT5882
        CKGEN_WRITE32(CKGEN_DMX_CKCFG, 0x6);            // demux clock ,set to 324MHz
#endif

        _DMX_Lock();

        u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
        u4Ctrl &= ~0x1;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);// Power on to dram clock

        // Init input source
        u4Reg = CKGEN_READ32(CKGEN_TSCKCFG);
        u4Reg &= (~0x03030303);   //choose demod input source
        CKGEN_WRITE32(CKGEN_TSCKCFG, u4Reg);
        _aeDmxFramerInput[0] = DMX_FE_INTERNAL;
        _aeDmxFramerInput[1] = DMX_FE_INTERNAL;
#if defined(CC_MT5890)
        _aeDmxFramerInput[2] = DMX_FE_INTERNAL;
#endif
        _aeDmxFramerInput[3] = DMX_FE_INTERNAL;
        _DmxCkgenToggle(0);
        _DmxCkgenToggle(1);
#if defined(CC_MT5890)
        _DmxCkgenToggle(2);
#endif
        _DmxCkgenToggle(3);

        // Set DMX SRAM used by DMX
        u4Ctrl = DMXCMD_READ32(DMX_REG_FTUPCR) & 0xFBFFFFFF;
        DMXCMD_WRITE32(DMX_REG_FTUPCR, u4Ctrl);

        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        // Turn ff DMX clock
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();

        CKGEN_WRITE32(CKGEN_BLOCK_CKENCFG1, CKGEN_READ32(CKGEN_BLOCK_CKENCFG1) & (~0xC0000));
    }
#else
    _DMX_Lock();

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
    u4Ctrl &= ~0x1;
    DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);// Power on to dram clock

    // Init input source
    u4Reg = CKGEN_READ32(CKGEN_TSCKCFG);
    u4Reg &= (~0x03030303);   //choose demod input source
    CKGEN_WRITE32(CKGEN_TSCKCFG, u4Reg);
    _aeDmxFramerInput[0] = DMX_FE_INTERNAL;
    _aeDmxFramerInput[1] = DMX_FE_INTERNAL;
#if defined(CC_MT5890)
    _aeDmxFramerInput[2] = DMX_FE_INTERNAL;
#endif
    _aeDmxFramerInput[3] = DMX_FE_INTERNAL;
    _DmxCkgenToggle(0);
    _DmxCkgenToggle(1);
#if defined(CC_MT5890)
    _DmxCkgenToggle(2);
#endif
    _DmxCkgenToggle(3);

    // Set DMX SRAM used by DMX
    u4Ctrl = DMXCMD_READ32(DMX_REG_FTUPCR) & 0xFBFFFFFF;
    DMXCMD_WRITE32(DMX_REG_FTUPCR, u4Ctrl);

    _fgMicroProcessorStopped = FALSE;
    _DMX_Unlock();

    UNUSED(_aeDmxFramerInput);
#endif // CC_DMX_FPGA_TEST
}


//-----------------------------------------------------------------------------
/** _DmxCkgenToggle
 */
//-----------------------------------------------------------------------------
static void _DmxSetAsyncFifoReset(UINT8 u1Idx, BOOL fgReset)
{
    UINT32 u4Ctrl;
    UINT8 au1Toggle[4] = { 4, 5, 7, 6 };

    if (u1Idx >= DMX_FRAMER_COUNT)
    {
        ASSERT(0);
    }

	if(u1Idx == DMX_FRAMER_TOTAL_INDEX)
	{
	   return ;
	}

    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL) & ~(0x1 << au1Toggle[u1Idx]);

    if (fgReset)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL) | (0x1 << au1Toggle[u1Idx]);
    }

    DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, u4Ctrl);
}


//-----------------------------------------------------------------------------
/** _DmxSetExtCkgen
 */
//-----------------------------------------------------------------------------
static void _DmxSetExtCkgen(UINT8 u1TsIdx, DMX_EXT_CKGEN_T eSel)
{
#if defined(CC_MT5890) || defined(CC_MT5882)
    UINT32 u4Ctrl, u4Mask;
    UINT8 au1Ext[5] =  { 0, 2, 3, 4, 1};
    UINT8 au1Ext2[5] = { 2, 0, 3, 4, 1};
    UINT8 au1Ext3[5] = { 2, 3, 0, 4, 1};
    UINT8 au1Ext4[5] = { 2, 3, 4, 0, 1};

    ASSERT(u1TsIdx < DMX_FRAMER_COUNT);
    if (u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return;
    }

	if(u1TsIdx == DMX_FRAMER_TOTAL_INDEX)
	{/*for dbm5 ,not need this one.*/
	   return ;
	}

    if (eSel >= DMX_CKGEN_NUM)
    {
        return;
    }

    if (u1TsIdx == 0)
    {
        u4Mask = ~0x70000;
    }
    else if (u1TsIdx == 1)
    {
        u4Mask = ~0x700000;
    }
    else if (u1TsIdx == 2)
    {
        u4Mask = ~0x7000000;
    }
    else if (u1TsIdx == 3)
    {
        u4Mask = ~0x70000000;
    }
    else
    {
        return;
    }

    u4Ctrl = CKGEN_READ32(CKGEN_REG_EXTTS_CKCKG) & u4Mask;

    if (u1TsIdx == 0)
    {
        u4Ctrl |= ((UINT32)au1Ext[eSel] << 16);
    }
    else if (u1TsIdx == 1)
    {
        u4Ctrl |= ((UINT32)au1Ext2[eSel] << 20);
    }
    else if (u1TsIdx == 2)
    {
        u4Ctrl |= ((UINT32)au1Ext3[eSel] << 24);
    }
    else if (u1TsIdx == 3)
    {
        u4Ctrl |= ((UINT32)au1Ext4[eSel] << 28);
    }
    else
    {
        ASSERT(0);
    }

    CKGEN_WRITE32(CKGEN_REG_EXTTS_CKCKG, u4Ctrl);

#else
    UNUSED(u1TsIdx);
    UNUSED(eSel);
#endif
}


//-----------------------------------------------------------------------------
/** _DmxSetExtPinmux
 */
//-----------------------------------------------------------------------------
static void _DmxSetExtPinmux(DMX_PINMUX_SEL_T ePin)
{
    INT32 i4Ret;

    switch (ePin)
    {
#if defined(CC_MT5890)
    case DMX_PINMUX_EXT_S:
        //PAD_DEMOD_TSCLK
        //PAD_DEMOD_TSVAL
        //PAD_DEMOD_TSSYNC
        //PAD_DEMOD_TSDATA0
        i4Ret = BSP_PinSet(PIN_DEMOD_TSCLK, 1);
        break;

    case DMX_PINMUX_EXT_P:
        //PAD_DEMOD_TSCLK
        //PAD_DEMOD_TSVAL
        //PAD_DEMOD_TSSYNC
        //PAD_DEMOD_TSDATA0
        //PAD_DEMOD_TSDATA1
        //PAD_DEMOD_TSDATA2
        //PAD_DEMOD_TSDATA3
        //PAD_DEMOD_TSDATA4
        //PAD_DEMOD_TSDATA5
        //PAD_DEMOD_TSDATA6
        //PAD_DEMOD_TSDATA7
        i4Ret = BSP_PinSet(PIN_DEMOD_TSCLK, 1);
        i4Ret = BSP_PinSet(PIN_DEMOD_TSDATA1, 1);
        i4Ret = BSP_PinSet(PIN_DEMOD_TSDATA5, 1);//PING ADD
        break;
        
    case DMX_PINMUX_EXT2_0_S:
        //PAD_GPIO15 --> TSCLK
        //PAD_GPIO16 --> TSVAL
        //PAD_GPIO17 --> TSSYNC
        //PAD_GPIO18 --> TSDATA0
        i4Ret = BSP_PinSet(PIN_GPIO15, 3);
        i4Ret = BSP_PinSet(PIN_GPIO18, 3);
        break;

    case DMX_PINMUX_EXT2_0_P:
        //PAD_GPIO15-->CLK
        //PAD_GPIO16-->VALID
        //PAD_GPIO17-->SYNC
        //PAD_GPIO18-->DATA0
        //PAD_GPIO19-->DATA1
        //PAD_GPIO20-->DATA2
        //PAD_GPIO21-->DATA3
        //PAD_GPIO22-->DATA4
        //PAD_GPIO23-->DATA5
        //PAD_GPIO24-->DATA6
        //PAD_GPIO25-->DATA7

        i4Ret = BSP_PinSet(PIN_GPIO15, 3);
        i4Ret = BSP_PinSet(PIN_GPIO18, 3);
        i4Ret = BSP_PinSet(PIN_GPIO19, 3);
        i4Ret = BSP_PinSet(PIN_GPIO20, 3);
        i4Ret = BSP_PinSet(PIN_GPIO21, 3);
        i4Ret = BSP_PinSet(PIN_GPIO22, 3);
        break;
        
    case DMX_PINMUX_EXT2_1_S:

        //PAD_DEMOD_TSDATA1 -->CLK
        //PAD_DEMOD_TSDATA2 -->VALID
        //PAD_DEMOD_TSDATA3 -->SYNC
        //PAD_DEMOD_TSDATA4 -->DATA0

        i4Ret = BSP_PinSet(PIN_DEMOD_TSDATA1, 2);
        break;

    case DMX_PINMUX_EXT2_2_S:

        //PAD_CI_TSCLK -->CLK
        //PAD_CI_TSVAL -->VALID
        //PAD_CI_TSSYNC -->SYNC
        //PAD_CI_TSDATA0 -->DATA0

        i4Ret = BSP_PinSet(PIN_CI_TSCLK, 1);
        i4Ret = BSP_PinSet(PIN_CI_TSDATA0, 1);
        break;

    case DMX_PINMUX_EXT2_3_S:
        //PAD_GPIO38   --> CLK
        //PAD_GPIO39   --> VALID
        //PAD_GPIO40   --> SYNC
        //PAD_GPIO41   --> DATA0
        i4Ret = BSP_PinSet(PIN_GPIO38, 4);
        break;

    case DMX_PINMUX_EXT2_4_S:
        //PAD_GPIO42   --> CLK
        //PAD_GPIO43   --> VALID
        //PAD_GPIO44   --> SYNC
        //PAD_GPIO48   --> DATA0
        i4Ret = BSP_PinSet(PIN_GPIO42, 4);
        i4Ret = BSP_PinSet(PIN_GPIO43, 4);
        i4Ret = BSP_PinSet(PIN_GPIO44, 4);
        i4Ret = BSP_PinSet(PIN_GPIO48, 4);
        break;

    case DMX_PINMUX_EXT3_0_S:
        //PAD_DEMOD_TSDATA1 -->CLK
        //PAD_DEMOD_TSDATA2 -->VALID
        //PAD_DEMOD_TSDATA3 -->SYNC
        //PAD_DEMOD_TSDATA4 -->DATA0
        i4Ret = BSP_PinSet(PIN_DEMOD_TSDATA1, 3);
        break;

    case DMX_PINMUX_EXT3_1_S:
        //PAD_GPIO49 -->CLK
        //PAD_GPIO50 -->VALID
        //PAD_GPIO51 -->SYNC
        //PAD_GPIO52 -->DATA0

        i4Ret = BSP_PinSet(PIN_GPIO49, 6);
        i4Ret = BSP_PinSet(PIN_GPIO51, 6);
        i4Ret = BSP_PinSet(PIN_GPIO52, 6);
        if(IS_IC_5861())
        {
            i4Ret = BSP_PinSet(PIN_GPIO50, 6);
        }
        else
        {
            i4Ret = BSP_PinSet(PIN_GPIO50, 7);
        }
		break;
		
    case DMX_PINMUX_EXT3_2_S:
        //PAD_GPIO63 -->CLK
        //PAD_GPIO64 -->VALID
        //PAD_GPIO65 -->SYNC
        //PAD_GPIO66 -->DATA0
        if(IS_IC_5861())
        {
            i4Ret = BSP_PinSet(PIN_GPIO63, 2);
            i4Ret = BSP_PinSet(PIN_GPIO64, 2);
            i4Ret = BSP_PinSet(PIN_GPIO65, 2);
            i4Ret = BSP_PinSet(PIN_GPIO66, 2);
        }
        else
        {
            LOG(0, "Oryx:Wrong Pinmux table please check!\n");
            return;
        }
        break;
        
    case DMX_PINMUX_EXT4_S:
        //PAD_GPIO34 -->CLK
        //PAD_GPIO35 -->VALID
        //PAD_GPIO36 -->SYNC
        //PAD_GPIO37 -->DATA0
        i4Ret = BSP_PinSet(PIN_GPIO34, 4);
        break;
//===========================================
#else  //MT5882
//===========================================
    case DMX_PINMUX_EXT_S:
        //PAD_DEMOD_TSCLK
        //PAD_DEMOD_TSVAL
        //PAD_DEMOD_TSSYNC
        //PAD_DEMOD_TSDATA0
        
        i4Ret = BSP_PinSet(PIN_DEMOD_TSCLK, 1);
        break;

    case DMX_PINMUX_EXT_P:
        if((!IS_IC_5882_ES3()) && (!IS_IC_5882_ES4()))
        {
            //For IC before Capri ES2 use this config
            //PAD_DEMOD_TSCLK    -->TSCLK
            //PAD_DEMOD_TSVAL    -->TSVAL
            //PAD_DEMOD_TSSYNC  -->TSSYNC
            //PAD_DEMOD_TSDATA0  -->TSDATA0
            //PAD_CI_TSCLK           -->TSDATA1
            //PAD_CI_TSVAL           -->TSDATA2
            //PAD_CI_TSSYNC         -->TSDATA3
            //PAD_CI_TSDATA0        -->TSDATA4
            //PAD_PVR_TSCLK          -->TSDATA5
            //PAD_PVR_TSVAL          -->TSDATA6
            //PAD_PVR_TSSYNC        -->TSDATA7
        
            i4Ret = BSP_PinSet(PIN_DEMOD_TSCLK, 1);
            i4Ret = BSP_PinSet(PIN_CI_TSCLK, 1);
            i4Ret = BSP_PinSet(PIN_CI_TSDATA0, 1);
            i4Ret = BSP_PinSet(PIN_PVR_TSCLK, 1);
        }
        else
        {
            //For Capri ES3, ES4
            //PAD_DEMOD_TSCLK    -->TSCLK
            //PAD_DEMOD_TSVAL    -->TSVAL
            //PAD_DEMOD_TSSYNC  -->TSSYNC
            //PAD_DEMOD_TSDATA0  -->TSDATA0
            //PAD_GPIO42           -->TSDATA1
            //PAD_GPIO43         -->TSDATA2
            //PAD_GPIO44        -->TSDATA3
            //PAD_GPIO45          -->TSDATA4
            //PAD_GPIO46          -->TSDATA5
            //PAD_GPIO47        -->TSDATA6
            //PAD_RF_AGC       -->TSDATA7
            i4Ret = BSP_PinSet(PIN_DEMOD_TSCLK, 1);
            i4Ret = BSP_PinSet(PIN_GPIO42, 0);
            i4Ret = BSP_PinSet(PIN_GPIO43, 0);
            i4Ret = BSP_PinSet(PIN_GPIO44, 0);
            i4Ret = BSP_PinSet(PIN_GPIO45, 0);
            i4Ret = BSP_PinSet(PIN_GPIO46, 0);
            i4Ret = BSP_PinSet(PIN_GPIO47, 0);
            i4Ret = BSP_PinSet(PIN_RF_AGC, 0);
        }
        break;

    case DMX_PINMUX_EXT_1_S:
        //PAD_GPIO15(CI_MCLKI)-->CLK
        //PAD_GPIO16(CI_VAL)-->VALID
        //PAD_GPIO17(CI_MISTRT)-->SYNC
        //PAD_GPIO18(CI_MDI0)-->DATA0
    
        i4Ret = BSP_PinSet(PIN_GPIO15, 6);
        i4Ret = BSP_PinSet(PIN_GPIO18, 6);
        break;
        
    case DMX_PINMUX_EXT_1_P:
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
    
        i4Ret = BSP_PinSet(PIN_GPIO15, 6);
        i4Ret = BSP_PinSet(PIN_GPIO18, 6);
        i4Ret = BSP_PinSet(PIN_GPIO19, 6);
        i4Ret = BSP_PinSet(PIN_GPIO20, 6);
        i4Ret = BSP_PinSet(PIN_GPIO21, 6);
        i4Ret = BSP_PinSet(PIN_GPIO22, 6);
        break;
        
    case DMX_PINMUX_EXT2_0_S:
        //PAD_GPIO15(CI_MCLKI) --> TSCLK
        //PAD_GPIO16(CI_VAL) --> TSVAL
        //PAD_GPIO17(CI_MISTRT) --> TSSYNC
        //PAD_GPIO18(CI_MDI0)--> TSDATA0
        
        i4Ret = BSP_PinSet(PIN_GPIO15, 3);
        i4Ret = BSP_PinSet(PIN_GPIO18, 3);
        break;
    
    case DMX_PINMUX_EXT2_0_P:
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
    
        i4Ret = BSP_PinSet(PIN_GPIO15, 3);
        i4Ret = BSP_PinSet(PIN_GPIO18, 3);
        i4Ret = BSP_PinSet(PIN_GPIO19, 3);
        i4Ret = BSP_PinSet(PIN_GPIO20, 3);
        i4Ret = BSP_PinSet(PIN_GPIO21, 3);
        i4Ret = BSP_PinSet(PIN_GPIO22, 3);
        break;
            
    case DMX_PINMUX_EXT2_1_S:
        //PAD_CI_TSCLK -->CLK
        //PAD_CI_TSVAL -->VALID
        //PAD_CI_TSSYNC -->SYNC
        //PAD_SPI_CLK -->DATA0   
        
        i4Ret = BSP_PinSet(PIN_CI_TSCLK, 2);
        i4Ret = BSP_PinSet(PIN_SPI_CLK,  2);
        break;
    
    case DMX_PINMUX_EXT2_2_S:
        //PAD_CI_TSCLK -->CLK
        //PAD_CI_TSVAL -->VALID
        //PAD_CI_TSSYNC -->SYNC
        //PAD_CI_TSDATA0 -->DATA0
    
        i4Ret = BSP_PinSet(PIN_CI_TSCLK,   4);
        i4Ret = BSP_PinSet(PIN_CI_TSDATA0, 4);
        break;
    
    case DMX_PINMUX_EXT2_3_S:
        //PAD_CI_TSDATA0  --> SYNC
        //PAD_PVR_TSCLK    --> VAL
        //PAD_PVR_TSSYNC  --> CLK
        //PAD_GPIO34          --> DATA0
        
        i4Ret = BSP_PinSet(PIN_CI_TSCLK, 6);
        i4Ret = BSP_PinSet(PIN_PVR_TSCLK, 6);
        i4Ret = BSP_PinSet(PIN_GPIO34, 6);
        break;
    
    case DMX_PINMUX_EXT3_S:
        //PAD_GPIO34(CI_MDO0) -->CLK
        //PAD_GPIO35(CI_MDO1) -->VALID
        //PAD_GPIO36(CI_MDO2) -->SYNC
        //PAD_GPIO37(CI_MDO3) -->DATA0
        
        i4Ret = BSP_PinSet(PIN_GPIO34, 4);
        i4Ret = BSP_PinSet(PIN_GPIO37, 4);
        break;
    
    case DMX_PINMUX_EXT4_S:
        //PAD_GPIO38(CI_MDO4) -->CLK
        //PAD_GPIO39(CI_MDO5) -->VALID
        //PAD_GPIO40(CI_MDO6) -->SYNC
        //PAD_GPIO41(CI_MDO7) -->DATA0
        
        i4Ret = BSP_PinSet(PIN_GPIO37, 4);
        break;
#endif

    default:
        LOG(0, "Wrong Pinmux table please check!\n");
        return;
    }

    UNUSED(i4Ret);
}

//-----------------------------------------------------------------------------
/** _DMX_BypassErrorHandlingTable_Enable
    TRUE: bypass error handling table
    FALSE: DO framr error handling table
 */
//-----------------------------------------------------------------------------
void _DMX_BypassErrorHandlingTable_Enable(UINT8 u1Framer,BOOL fgEnable)
{
    UINT32 u4Reg;


    ASSERT(u1Framer < DMX_FRAMER_COUNT);

    if (u1Framer == 0)
    {
        if (fgEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg |= (0x1 << 4);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg &= ~(0x1 << 4);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
    }
    else if (u1Framer == 1)
    {
        if (fgEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg |= (0x1 << 5);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg &= ~(0x1 << 5);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }

    }
    else if (u1Framer == 2)
    {
        if (fgEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg |= (0x1 << 7);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg &= ~(0x1 << 7);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }

    }
    else if (u1Framer == 3)
    {
        if (fgEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg |= (0x1 << 6);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Reg &= ~(0x1 << 6);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Reg);
        }
    }
    else
    {
        LOG(0,"Input the wrong Framer number!\n");
    }
}

//-----------------------------------------------------------------------------
/** _DMX_SetFramer

 */
//-----------------------------------------------------------------------------
void _DMX_SetFramer(UINT8 u1FramerIdx, BOOL fgEnable, BOOL fgPareallel, BOOL fgExtSync, BOOL fgPosEdge)
{
    UINT32 u4Reg, u4Val = 0x0;

    ASSERT(u1FramerIdx <= 3);

    if (u1FramerIdx == 0x2)
    {
        return;
    }

    if (u1FramerIdx == 0x3)
    {
        u1FramerIdx -= 0x1;
    }

    //disable framer first, before set framer again.
    u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Reg &= ~(0x1 << (4 * u1FramerIdx));
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);

    //set value
    u4Val |= (fgEnable ? 0x1 : 0x0); //framer enable
    u4Val |= ((fgPareallel ? 0x1 : 0x0) << 1); //parallel or serial
    u4Val |= ((fgExtSync ? 0x1 : 0x0) << 2); //external or internal
    u4Val |= ((fgPosEdge ? 0x1 : 0x0) << 3); //external or internal

    //set framer
    u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Reg &= ~(0xF << (4 * u1FramerIdx));
    u4Reg |= (u4Val << (4 * u1FramerIdx));
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Reg);

}
//-----------------------------------------------------------------------------
/** _DMX_Framer_130byteEnable

 */
//-----------------------------------------------------------------------------
BOOL _DMX_Framer_130byteEnable(UINT8 u1Framer, BOOL fgDemod130byteTs, BOOL fgInputEnable, UINT8 u1InputPktSize ,BOOL fgOutputEnable, UINT8 u1OutputPktSize)
{
    UINT32 u4Reg;

    ASSERT(u1Framer < DMX_FRAMER_COUNT);

    if (u1Framer == 0)
    {
        //set input packet size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
        u4Reg &= ~(0xFF);
        u4Reg |= u1InputPktSize;
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);

        //set input enable
        if (fgInputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
            u4Reg |= (0x1 << 8);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
            u4Reg &= ~(0x1 << 8);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
        }

        //set output packet size
        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
        u4Reg &= ~(0xFF);
        u4Reg |= (u1OutputPktSize);
        DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

        //set output enable  and dbm auto switch
        if (fgOutputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 24); //output enable
            u4Reg |= (0x1 << 27); //dbm auto switch
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 24);
            u4Reg &= ~(0x1 << 27);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

        //set Demod 130byte ts enable or disable 17920[28]
        if (fgDemod130byteTs)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 28);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 28);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

    }
    else if (u1Framer == 1)
    {
        //set input packet size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
        u4Reg &= ~(0xFF << 16);
        u4Reg |= (u1InputPktSize << 16);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);

        //set input enable
        if (fgInputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
            u4Reg |= (0x1 << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL);
            u4Reg &= ~(0x1 << 24);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
        }

        //set output packet size
        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
        u4Reg &= ~(0xFF << 8);
        u4Reg |= (u1OutputPktSize << 8);
        DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

        //set output enable  and dbm auto switch
        if (fgOutputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 25); //output enable
            u4Reg |= (0x1 << 27); //dbm auto switch
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 25);
            u4Reg &= ~(0x1 << 27);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

        //set Demod 130byte ts enable or disable 17920[29]
        if (fgDemod130byteTs)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 29);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 29);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

    }
    else if (u1Framer == 2)
    {
        //set input packet size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL2);
        u4Reg &= ~(0xFF);
        u4Reg |= u1InputPktSize;
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL2, u4Reg);

        //set input enable
        if (fgInputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg |= (0x1 << 6);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg &= ~(0x1 << 6);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }

        //set output packet size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL2);
        u4Reg &= ~(0xFF << 8);
        u4Reg |= (u1OutputPktSize << 8);
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL2, u4Reg);

        //set output enable  and dbm auto switch
        if (fgOutputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 27); //dbm auto switch
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg |= (0x1 << 5); // output enable
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 27);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg &= ~(0x1 << 5); //eoutput disable
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }

        //set Demod 130byte ts enable or disable 17920[28]
        if (fgDemod130byteTs)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg |= (0x1 << 4);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Reg &= ~(0x1 << 4);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
        }

    }
    else if (u1Framer == 3)
    {
        //set input packet size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
        u4Reg &= ~(0xFF);
        u4Reg |= (u1InputPktSize);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);

        //set input enable
        if (fgInputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
            u4Reg |= (0x1 << 8);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2);
            u4Reg &= ~(0x1 << 8);
            DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
        }

        //set output packet size
        u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
        u4Reg &= ~(0xFF << 16);
        u4Reg |= (u1OutputPktSize << 16);
        DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);

        //set output enable  and dbm auto switch
        if (fgOutputEnable)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 26); //output enable
            u4Reg |= (0x1 << 27); //dbm auto switch
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 26);
            u4Reg &= ~(0x1 << 27);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

        //set Demod 130byte ts enable or disable 17920[30]
        if (fgDemod130byteTs)
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg |= (0x1 << 30); //output enable
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }
        else
        {
            u4Reg = DMXCMD_READ32(DMX_REG_CONFIG4);
            u4Reg &= ~(0x1 << 30);
            DMXCMD_WRITE32(DMX_REG_CONFIG4, u4Reg);
        }

    }
    else
    {
        LOG(0,"Input the wrong Framer number!\n");
        return FALSE;
    }

    return TRUE;

}

//-----------------------------------------------------------------------------
/** _DMX_SetFramerMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetFramerMode(UINT8 u1Framer, DMX_FRAMER_MODE_T eMode,
                        BOOL fgExtSync, BOOL fgPosEdge)
{
    UINT32 u4Ctrl, u4RegOffset;
    UINT8 u1FramerControl;
    UINT32 u4Ctrl1;
#if defined(CC_MT5890)
    UINT32 u4Ctrl2;
#endif

    ASSERT(u1Framer < DMX_FRAMER_COUNT);

#if defined(CC_MT5890)
    if(u1Framer == DMX_FRAMER_TOTAL_INDEX)
    {
      LOG(0,"[%s]Invalid framer index(%u).\r\n", __FUNCTION__, u1Framer);
	  return ;
    }
#else  //MT5882
    if(u1Framer == DMX_FRAMER_INDEX_2)
    {
		LOG(0,"[%s]Invalid framer index(%u).\r\n", __FUNCTION__, u1Framer);
		return;
    }
#endif

    if (eMode == DMX_FRAMER_TWOBIT)
    {
        u4Ctrl1 = DMXCMD_READ32(DMX_REG_CONFIG1);
        //u4Ctrl1 &= ~(7<<26);

        if (u1Framer == 0)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
            u4Ctrl &= ~(3 << 24);                   //clear framer 0 2-bit mode format
            u4Ctrl |= (2 << 24);                    //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            u4Ctrl1 |= (1<<26);
        }
        else if (u1Framer == 1)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
            u4Ctrl &= ~(3 << 26);                   //clear framer 1 2-bit mode format
            u4Ctrl |= (2 << 26);                    //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            u4Ctrl1 |= (1<<27);
        }
#if defined(CC_MT5890)
        else if (u1Framer == 2)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Ctrl &= ~(3 << 2);                         //clear framer 2 2-bit mode format
            u4Ctrl |= (2 << 2);                          //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Ctrl);

            //framer 2 control reg 0x17924
            u4Ctrl2 = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Ctrl2 |= (1U << 7);                    //enable framer 2 two bit mode
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl2);
        }
#endif
        else if (u1Framer == 3)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
            u4Ctrl &= ~(3);                         //clear framer 3 2-bit mode format
            u4Ctrl |= (2);                          //set 2-bit synchornous TS format.
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Ctrl);
            u4Ctrl1 |= (1<<28);
        }
        else
        {
            LOG(0, "Framer index error!\n");
        }

        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl1);
    }
    else
    {
        u4Ctrl1 = DMXCMD_READ32(DMX_REG_CONFIG1);

        if (u1Framer == 0)
        {
            if (_afgNoTSValid[u1Framer] == FALSE)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
                u4Ctrl &= ~(3 << 24);                   //clear framer 0 2-bit mode format
                DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            }
            u4Ctrl1 &= ~(1<<26);
        }
        else if (u1Framer == 1)
        {
            if (_afgNoTSValid[u1Framer] == FALSE)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
                u4Ctrl &= ~(3 << 26);                   //clear framer 1 2-bit mode format
                DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
            }
            u4Ctrl1 &= ~(1<<27);
        }
#if defined(CC_MT5890)
        else if (u1Framer == 2)
        {
            if (_afgNoTSValid[u1Framer] == FALSE)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
                u4Ctrl &= ~(3 << 2);                         //clear framer 2 2-bit mode format
                DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Ctrl);
            }

            //framer 2 control reg 0x17924
            u4Ctrl2 = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
            u4Ctrl2 &= ~(1U << 7);                    //disable framer 2 two bit mode
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl2);
        }
#endif
        else if (u1Framer == 3)
        {
            if (_afgNoTSValid[u1Framer] == FALSE)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL3);
                u4Ctrl &= ~(3);                         //clear framer 3 2-bit mode format
                DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3, u4Ctrl);
            }
            u4Ctrl1 &= ~(1<<28);
        }
        else
        {
            LOG(0, "Framer index error!\n");
        }

        DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl1);
    }


    u1FramerControl = 0x1;

    if ((eMode == DMX_FRAMER_PARALLEL) || (eMode == DMX_FRAMER_TWOBIT))
    {
        u1FramerControl |= 0x2;
    }

    if (fgExtSync)
    {
        u1FramerControl |= 0x4;
    }

    if (fgPosEdge)
    {
        u1FramerControl |= 0x8;
    }

#if defined(CC_MT5890)
    //framer 2 ctrl register is far from DMX_REG_FRAMER_CONTROL, so handle this independent
    if (u1Framer == 2)
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
        u4RegOffset = 0;
        if (((u4Ctrl >> (u4RegOffset * 4)) & 0xF) != u1FramerControl)
        {
            //disable framer before set framer again
            u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset)));
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);
            //set framer again, Becarefull
            u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset))) | ((u1FramerControl) << (4*u4RegOffset));
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);

            _DMX_ResetFramer(u1Framer);
        }
    }
    else
#endif
    {
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
        u4RegOffset = (u1Framer == 3) ? 2 : (UINT32)u1Framer;
        if (((u4Ctrl >> (u4RegOffset * 4)) & 0xF) != u1FramerControl)
        {
            //disable framer before set framer again
            u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset)));
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
            //set framer again, Becarefull
            u4Ctrl = (u4Ctrl & ~((0xf) << (4*u4RegOffset))) | ((u1FramerControl) << (4*u4RegOffset));
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

            _DMX_ResetFramer(u1Framer);
        }
    }
}


//-----------------------------------------------------------------------------
/** _DmxLoadIMem
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DmxLoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT32 u4Cmd, u4Data, i;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;

    ASSERT(pu4IData != NULL);

    // Check uP status
    if (_DmxIsFTIRunning())
    {
        if (!fgForceWrite)
        {
            LOG(7, "FTI uP is running, can't load to I-mem!\n");
            return FALSE;
        }
        else
        {
            LOG(7, "FTI uP is running, now will halt it\n");
            _DMX_EnableFTI(FALSE);
            while (_DmxIsFTIRunning())
            {
                LOG(5, "wait uP stop...\n");
            }
        }
    }

    // Clear the "FTuP instruction mode" flag.
    DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= ~(1 << 25);

    // Load to I-mem
    for (i = 0; i < u4Len; i++)
    {

        _DMX_Lock();

        // Issue a write command
        DMXCMD_WRITE32(DMX_REG_MEM_DATA, pu4IData[i]);
        u4Cmd = (UINT32)((1 << 0) | (2 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD,  u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }

        _DMX_Unlock();
    }

    // Verify
    for (i = 0; i < u4Len; i++)
    {
        _DMX_Lock();

        // Issue a read command
        u4Cmd = (UINT32)((1 << 0) | (1 << 8) | (i << 16));
        DMXCMD_WRITE32(DMX_REG_MEM_CMD, u4Cmd);

        // Wait for ready
        while (1)
        {
            u4Cmd = DMXCMD_READ32(DMX_REG_MEM_CMD);
            if (((u4Cmd >> 8) & 0xf) == 0)
            {
                break;
            }
        }
        // Verify
        u4Data = DMXCMD_READ32(DMX_REG_MEM_DATA);

        _DMX_Unlock();

        if (u4Data != pu4IData[i])
        {
            LOG(1, "Load IMem error, at word %u, write 0x%08x, read 0x%08x\n",
                i, pu4IData[i], u4Data);
            return FALSE;
        }
    }

    _u4uCodeVersion = pu4IData[0];
    u2CodeVersion = (UINT16)(pu4IData[0] & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(5, "Demux uCode version: %u.%u\n", u1Major, u1Minor);

    // Reset demux (uP), so uP can run into the entry pointer of uCode
    VERIFY(_DMX_Reset());

    // Suppress warnings in lint and release build
    UNUSED(u1Major);
    UNUSED(u1Minor);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_LoadIMem
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_LoadIMem(BOOL fgForceWrite, const UINT32* pu4IData, UINT32 u4Len)
{
    UINT8 i = 0;
    BOOL fgRet;
    for (i = 0; i < 10; i++)
    {
        fgRet = _DmxLoadIMem(fgForceWrite, pu4IData, u4Len);
        if (fgRet)
        {
            return TRUE;
        }
    }
    return FALSE;
}


//-----------------------------------------------------------------------------
/** _DmxInitFVR
 */
//-----------------------------------------------------------------------------
static void _DmxInitFVR(void)
{
    UINT32 i, u4Ctrl;

    //-----------------------------------------------------
    // Reset DMX Record hardware
    //-----------------------------------------------------
    x_memset((void*)&(FVR_GBL_ARY_W(0, 0)), 0, FVR_GBL_SIZE * 4);
    x_memset((void*)&(FVR_GBL_ARY_W(1, 0)), 0, FVR_GBL_SIZE * 4);
    x_memset((void*)&(FVR_GBL_ARY_W(2, 0)), 0, FVR_GBL_SIZE * 4);
    x_memset((void*)&(FVR_GBL_ARY_W(3, 0)), 0, FVR_GBL_SIZE * 4);

    //reset DMX record hardware common region 124byte
    x_memset((void*)FVR_GBL_COMMON_REGION, 0, FVR_GBL_COMMON_SIZE * 4);

	/*reset the ci+1.4 record region buffer*/
	x_memset((void*)FVR_GBL_CI14_PID_BASE,0,FVR_CI14_GBL_SIZE*4);


    for (i = 0; i < FVR_NUM_PID_INDEX; i++)
    {
        FVR_PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(FVR_PER_PID_S(i)), 0, FVR_PID_SIZE * 4);
    }

    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF;
    u4Ctrl |= (FVR_PER_PID_OFFSET) << 16;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) & 0xFD00FEFF;
    u4Ctrl |= (1 << 25);                    // Disable record path
    u4Ctrl |= ((FVR_PID_SIZE * 4) << 16); // Set key pid size
    u4Ctrl |= (1 << 8);                     // Direct map
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Disable insert timestamp
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2) & 0xDFFFFFFF;
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);
}


/*_DMX_SetDbmChannel4:enable channel4*/
BOOL _DMX_SetDbmChannel4(BOOL fg_playback_enable, BOOL fg_record_enable)
{
    UINT32 u4Ctrl;

	u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE)&~(0xC0);
	if(fg_playback_enable == TRUE)
	{
	  u4Ctrl = u4Ctrl|(1<<7) ;
	}

	if(fg_record_enable == TRUE)
	{
	   u4Ctrl = u4Ctrl|(1<<6) ;
	}

	DMXCMD_WRITE32(DMX_REG_DBM_MULTI_STREAM_MODE,u4Ctrl);

	return TRUE;

}

/*get the dbmchannel5 status: true:enable , false:disable*/
BOOL _DMX_GetDbmChannel4(BOOL* fg_playback_enable, BOOL* fg_record_enable)
{
   if(fg_playback_enable==NULL ||fg_record_enable==NULL)
   {
     LOG(0,"NULL Pointer.\r\n");
	 return FALSE;
   }

   (*fg_playback_enable) = (DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE)>>7)&0x1;
   (*fg_record_enable) = (DMXCMD_READ32(DMX_REG_DBM_MULTI_STREAM_MODE)>>6)&0x1;

   if((*fg_playback_enable)== TRUE || (*fg_playback_enable)== TRUE)
   {
     return TRUE;
   }
   else
   {
     return FALSE;
   }
}

//-----------------------------------------------------------------------------
/** _DmxTSSoftInit
 *  Soft-initialize demux
 */
//-----------------------------------------------------------------------------
static BOOL _DmxTSSoftInit(void)
{
    UINT32 u4Ctrl, i ,j;
	UINT32 u4_dmx_filter_index = DMX_NUM_FILTER_INDEX/2;

    _DMX_Lock();

    //---------------------------------------------------------
    // in oryx ,remove this pid replace region.
    //---------------------------------------------------------
    //x_memset((void*)PID_REPLACED_BASE, 0, PID_REPLACED_SIZE * 4 );
    //---------------------------------------------------------
    //clear 130 byte golbal region
    //---------------------------------------------------------
    x_memset((void*)RV_SETTING_BASE, 0, RV_SETTING_SIZE * 4);

    //---------------------------------------------------------
    // Clear Play back global region
    //---------------------------------------------------------
    x_memset((void*)PLAYBACK_GBL_BASE, 0, PLAYBACK_GBL_SIZE * 4 );

    //---------------------------------------------------------
    // Clear DMX SRAM and struct
    //---------------------------------------------------------
    // Clear HW PID index table and PID DMEM
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_INDEX_TABLE(i) = 0;
        x_memset((void*)&(PID_S(i)), 0, DMX_DMEM_ENTRY_LEN * 4);
        // Debug, reset continuity counter to 0xff
        PID_S_W(i, 3) = 0xffff0000;
    }

    // PID structures
    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        PID_STRUCT_T* prPidStruct;
        UINT8 u1SerialNum;

        prPidStruct = _DMX_GetPidStruct(i);
        u1SerialNum = prPidStruct->u1SerialNumber;
        x_memset((void*)prPidStruct, 0, sizeof(PID_STRUCT_T));
        prPidStruct->u1SerialNumber = u1SerialNum;
    }

    // Clear section filter
    //First 96 section filter
    x_memset((void*)DMX_SECTION_POSNEG, 0, 8 * u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_CONTROL, 0, 4 * u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_PATTERN, 0, 8 * u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_MASK, 0, 8 *u4_dmx_filter_index);

    //second 96 section filter
    x_memset((void*)DMX_SECTION_POSNEG2, 0, 8 * u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_CONTROL2, 0, 4 *u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_PATTERN2, 0, 8 * u4_dmx_filter_index);
    x_memset((void*)DMX_SECTION_MASK2, 0, 8 * u4_dmx_filter_index);

    x_memset((void*)DMX_ONEBYTE_FILTER_BASE, 0, 2 * DMX_NUM_ONEBYTE_FILTER_INDEX);

    //use the share section filter ,remove it .but bit[8-0] is used for ts_idx4.
    u4Ctrl = SECTIONFILTER_SETTING | (0xFF);
    SECTIONFILTER_SETTING = u4Ctrl;

    //CT setting init
    VERIFY(_DMX_CTInit());

    //PID structure offset
    u4Ctrl = DMXCMD_READ32(DMX_REG_PID_STRUCT_OFFSET) & 0xFFFF0000;
    DMXCMD_WRITE32(DMX_REG_PID_STRUCT_OFFSET, u4Ctrl);

    // Clear section filters
    for (j = 0; j < DMX_FRAMER_COUNT; j++)
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            FILTER_STRUCT_T *prFilterStruct;

            prFilterStruct = _DMX_GetFilterStruct( j,i);
            x_memset((void*)prFilterStruct, 0, sizeof(FILTER_STRUCT_T));
        }
    }
    _DMX_Unlock();

	_DMX_SetDuplicateScheme(TRUE);
    // Initialize interrupt handler
    _DMX_InitISR();

    // Initialize common routines
    VERIFY(_DMX_InitAPI());

    _DMX_Lock();

    //-----------------------------------------------------
    //Framer Error Table
    //-----------------------------------------------------
    //enable error handling
    _DMX_SetFramerPacketErrorHandling(0, TRUE, 0xF0C0);
    _DMX_SetFramerPacketErrorHandling(1, TRUE, 0xF0C0);
#if defined(CC_MT5890)
    _DMX_SetFramerPacketErrorHandling(2, TRUE, 0xF0C0);
#endif
    _DMX_SetFramerPacketErrorHandling(3, TRUE, 0xF0C0);

    //-----------------------------------------------------
    // Record default setting
    //-----------------------------------------------------
    // Disable record
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1) | (1 << 25);
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    // Set packet length to 188 bytes
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG2);
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0x08;          // maximum burst size
    u4Ctrl = (u4Ctrl & 0xFFFF00FF) | (188 << 8);    // packet len = 188 bytes
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x47 << 16);  // TS packet sync byte
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Ctrl);

    // Setup framer control , default enable framer 0 and framer 1,
    //framer 2 and framer 3 deault disable,
    _DMX_SetFramerMode(0, DMX_FRAMER_PARALLEL, TRUE, TRUE);
    _DMX_SetFramerMode(1, DMX_FRAMER_PARALLEL, TRUE, TRUE);

    // Set DBM to normal mode and enable DBM
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Ctrl = (u4Ctrl & 0xcfffffff) | (1 << 30);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
	//enable dbm channel5 default
	_DMX_SetDbmChannel4(TRUE,TRUE);

    // Set PID structure size
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG1);
    u4Ctrl &= 0xffffEf00;                       // Select Group 1 DRAM agent
    u4Ctrl |= (DMX_DMEM_ENTRY_LEN * 4);         // Size of each PID data
    u4Ctrl |= (1 << 9);                         // DMA delay ack, debug
    u4Ctrl |= (1 << 10);                        // Enable multi-PID channel
    DMXCMD_WRITE32(DMX_REG_CONFIG1, u4Ctrl);

    _DMX_Unlock();

    //Reset Framer 0 and Framer 1
    _DMX_ResetFramer(0);
    _DMX_ResetFramer(1);

    //Set SECURE Control
#ifndef CC_TRUSTZONE_SUPPORT
    DMX_MM_SECURE_W(0) = 0x00;
#endif
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxInitBroadcastTS
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitBroadcastTS(void)
{
    UINT32 u4Ctrl;

#ifdef DMX_MEASURE_PSI_TIME
    _DMX_PSI_ResetMaxTime();
#endif  // DMX_MEASURE_PSI_TIME
    // Load default uCode (for normal transport stream)
    if (!_DMX_LoaduCode())
    {
        return FALSE;
    }

    //Setup 17014 Ftup's data memory security access disable before ts soft init
    #if defined(CC_TRUSTZONE_SUPPORT)
    UNUSED(TZ_DMX_Init());
#else
    _DMX_DMEM_CA_Init();
#endif

    // Soft init
    if (!_DmxTSSoftInit())
    {
        return FALSE;
    }

    _DmxInitFVR();

    _DMX_Lock();
    u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    // Set DBM output spacing
    u4Ctrl = (u4Ctrl & 0xff00ffff) | (8 << 16);
    // Set DBM max playback TS packets
    u4Ctrl = (u4Ctrl & 0xffff00ff) | (1 << 8);
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
    _DMX_Unlock();

    // Important: Set framer after calling _DmxTSSoftInit().
    // Be sure to set the register while the FTI is disabled.
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x03 << 16);  // Sync Lock
    u4Ctrl = (u4Ctrl & 0x00FFFFFF) | (0x01 << 24);  // Sync Loss
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    //Playback 1 Framer Control
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CTRL2);
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x47 << 16);  // Sync byte pattern
    u4Ctrl = (u4Ctrl & 0xFFFF00FF) | (0x3 << 8);    // Sync Lock
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0x1;           // Sync Loss
    DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL2, u4Ctrl);

    //Playback 2 Framer Control
    u4Ctrl = DMXCMD_READ32(DMX_REG_PLAYBACKFRAMER2_CTRL2);
    u4Ctrl = (u4Ctrl & 0xFF00FFFF) | (0x47 << 16);  // Sync byte pattern
    u4Ctrl = (u4Ctrl & 0xFFFF00FF) | (0x3 << 8);    // Sync Lock
    u4Ctrl = (u4Ctrl & 0xFFFFFF00) | 0x1;           // Sync Loss
    DMXCMD_WRITE32(DMX_REG_PLAYBACKFRAMER2_CTRL2, u4Ctrl);

    //enable framer pre-full to control DDI and play back speed
    u4Ctrl = DMXCMD_READ32(DMX_REG_CONFIG4);
    u4Ctrl |= ((UINT32)0x1 << 31);
    DMXCMD_WRITE32(DMX_REG_CONFIG4,u4Ctrl);


    //enable bit to reset the DBM some framer releated register
    u4Ctrl = DMXCMD_READ32(DMX_REG_PCR_NUM_CONTROL);
    u4Ctrl |= ((UINT32)0x1 << 5);
    DMXCMD_WRITE32(DMX_REG_PCR_NUM_CONTROL, u4Ctrl);


    //enalbe framer unlock check
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL) & 0xFFFE0000;
    u4Ctrl |= ((UINT32)0x1 << 16);//enable
    u4Ctrl |= ((UINT32)0x3E8);//threshold 1000 packet
    DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Ctrl);


    _DMX_SetFramerMode(0, DMX_FRAMER_PARALLEL, TRUE, TRUE);
    _DMX_SetFramerMode(1, DMX_FRAMER_PARALLEL, TRUE, TRUE);

    // Enable FTI
    _DMX_EnableFTI(TRUE);

	/**
     Temp solution: Disable WDLE check
     Phenomenon:
       Netflix hang up
       AES offset: 5, ChunkLen: 0x352, EncryptLen: 0x34D
 
     Cause:
       WDLE check logic crashs
       1. Timing issue due to DMX clock is faster
       2. ALE count is not equal to WDLE
     
     Side effect:
       DMX cannot guarantee data is completely written to DRAM when CPU receives interrupts
       But, DMX dram priority is higher than CPU
       So, CPU read will be done after DMX write       
    */
    u4Ctrl = DMXCMD_READ32(DMX_REG_REG_FILE_ADDR_REG);
    u4Ctrl |= 0x10000000;
    DMXCMD_WRITE32(DMX_REG_REG_FILE_ADDR_REG, u4Ctrl);
	
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxFrontEndCust
 */
//-----------------------------------------------------------------------------
BOOL _DmxCustFrontEnd(void)
{
    BOOL fgRet = TRUE;
    UINT8 i;
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;

    UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));

    for (i = 0; i < DMX_FRAMER_TOTAL_INDEX; i++)
    {
        if (prDmxFeCfgTable[i].fgEnable)
        {
            DMX_FRONTEND_T eFrontEnd;
            if (prDmxFeCfgTable[i].ePinSet == DMX_PINSET_INTERNAL)
            {
                eFrontEnd = DMX_FE_INTERNAL;
            }
            else
            {
                eFrontEnd = (DMX_FRONTEND_T)((UINT32)prDmxFeCfgTable[i].ePinSet | (UINT32)prDmxFeCfgTable[i].eMode);
            }

            if (!_DMX_SetFrontEndEx(i, eFrontEnd))
            {
                fgRet = FALSE;
            }
        }
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DmxInitHWPath
 */
//-----------------------------------------------------------------------------
static BOOL _DmxInitHWPath(void)
{
#if 0   // Enable framer2
    UINT32 u4Reg;

    // Framer2 size
    u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_CTRL1);
    u4Reg |= 0x1;   // Enable framer2
    DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL1, u4Reg);
    _DMX_ResetFramer(2);
#endif

    /*for ts_idx3 ,set it to framer->dbm channel.*/
	DMX_SetDbm_InputSource(3,DMX_DBM_INPUT_FRAMER);


    if (!_DmxCustFrontEnd())
    {
        return FALSE;
    }

#ifdef ENABLE_MULTIMEDIA
    //----------------------------------
    // DDI path
    //----------------------------------
    _DMX_DDI_SetPort(DDI_DEFAULT_PORT, TRUE);

    //----------------------------------
    // PVR path
    //----------------------------------
    DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_FRAMER2_BYPASS); // PVR for TS file
    UNUSED(_DMX_PVRPlay_SetMoveMode(0,DMX_TSFMT_188, TRUE));
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxSetTSInputCkgen
 */
//-----------------------------------------------------------------------------
static void _DmxSetTSInputCkgen(UINT8 u1Idx, DMX_FRONTEND_T eFrontEnd)
{
    BOOL fgToggle;
    DMX_FRONTEND_T ePreType;
    UINT32 u4Reg, u4OrigReg;
    UINT8 au1Offset[4] = { 0, 8, 16, 24 };
    UINT8 i;

    ASSERT(u1Idx < DMX_FRAMER_COUNT);

    if (u1Idx >= DMX_FRAMER_COUNT)
    {
        return;
    }

	if(u1Idx == DMX_FRAMER_TOTAL_INDEX)
	{//fro dbm5 ,not need .
	   return ;
	}

    //--------------------------
    // Set Framer Ckgen
    //--------------------------
    ePreType = _aeDmxFramerInput[u1Idx];
    fgToggle = FALSE;

    /**
     * ckgen toggle will generate 10 clk to guarantee switch clock success,but toggle is only
     * needed in no clk input, such as internal demod and 8-bits mode TS_OUT.
     */
    if ((ePreType == DMX_FE_INTERNAL) || (ePreType == DMX_FE_TSOUT))
    {
        fgToggle = TRUE;
    }

    u4OrigReg = CKGEN_READ32(CKGEN_TSCKCFG);
    u4Reg = u4OrigReg;

    //// Double confirm
    //ASSERT((u4Reg&0x3) == _aeDmxFramerInput[0]);
    //ASSERT(((u4Reg>>au1Offset[1])&0x3) == _aeDmxFramerInput[1]);
    //ASSERT(((u4Reg>>au1Offset[3])&0x3) == _aeDmxFramerInput[3]);

    u4Reg &= ~(0x3 << au1Offset[u1Idx]);

    // DDI can only input one framer
    if (eFrontEnd == DMX_FE_DDI)
    {
        for (i=0; i<4; i++)
        {
            if (i == u1Idx)
            {
                continue;
            }
		#ifdef CC_DMX_EMULATION
		    if(_aeDmxFramerInput[0]==DMX_FE_DDI || _aeDmxFramerInput[1]==DMX_FE_DDI)
		    {/*framer0,1,2,3  can connnect to DDI at the same ,modify it for ciplus1.4 test.*/
		      continue;
		    }
        #endif
            if (_aeDmxFramerInput[i] == DMX_FE_DDI)
            {
                u4Reg &= ~(0x3 << au1Offset[i]);
                u4Reg |= ((UINT32)DMX_FE_TSOUT << au1Offset[i]);
                _aeDmxFramerInput[i] = DMX_FE_TSOUT;
            }

        }
    }

    if ((eFrontEnd == DMX_FE_INTERNAL) || (eFrontEnd == DMX_FE_DDI) ||
            (eFrontEnd == DMX_FE_TSOUT))
    {
        u4Reg |= (((UINT32)eFrontEnd & 0x3) << au1Offset[u1Idx]);
    }
    else
    {
        u4Reg |= (1 << au1Offset[u1Idx]);
    }

    _aeDmxFramerInput[u1Idx] = eFrontEnd;

    if (u4Reg != u4OrigReg)
    {
        //To avoid affecting framer logic, we disable framer before changing clock source.
        _DMX_SetFramerEnabled(u1Idx, FALSE);

        CKGEN_WRITE32(CKGEN_TSCKCFG, u4Reg);

        if (fgToggle)
        {
            _DmxCkgenToggle(u1Idx);
        }

        _DMX_SetFramerEnabled(u1Idx, TRUE);
    }
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
BOOL _DMX_InitDiag(void)
{
    // Load test uCode
    if (!_DMX_LoaduTestCode())
    {
        return FALSE;
    }

    // Reset FTI
    if (!_DmxTSSoftInit())
    {
        return FALSE;
    }

    // Enable FTI
    _DMX_EnableFTI(TRUE);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_ActivateDbmReset
 *
 *  Disable DBM and pull reset to high
 *
 *  Note:
 *    This API is used to resolve a hardware bug. If framer3 is disabled and
 *    enabled frequently, the sync fifo of framer3 could be overflow (DBM error 32).
 *    The workaround is we have to soft reset framer3 after disabling it.
 *    However, the soft reset (0x17204[5]) has a problem on Cobra and Vipher.
 *    The alternative workaround is we reset DBM before disabling framer3.
 *    And, we need to keep reset signal until framer3 is disabled completely.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ActivateDbmReset(VOID)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg, u4Counter;

    // Disable DBM
    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= ~(1 << 30);                            // Clear "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    _DMX_Unlock();

    // Polling the value of 0x20017410
    //      if bit[16..12] != 0x10
    //          then reset DBM
    u4Counter = 0;
    while (u4Counter < DMX_MAX_DBM_WAIT_COUNT)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
        if (((u4Reg >> 12) & 0x1F) == 0x10)
        {
            x_thread_delay(1);
            u4Counter++;
        }
        else
        {
            break;
        }
    }

    if (u4Counter == DMX_MAX_DBM_WAIT_COUNT)
    {
        LOG(2, "Failed to reset DBM safely (still outputing TS packets)!\n");
        fgRet = FALSE;
    }

    // Reset DBM
    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg |= 0x80000000;                            // Set "Reset" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    _DMX_Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_ReleaseDbmReset
 *
 *  Pull reset to low
 *
 *  Note:
 *    This API is used to resolve a hardware bug. If framer3 is disabled and
 *    enabled frequently, the sync fifo of framer3 could be overflow (DBM error 32).
 *    The workaround is we have to soft reset framer3 after disabling it.
 *    However, the soft reset (0x17204[5]) has a problem on Cobra and Vipher.
 *    The alternative workaround is we reset DBM before disabling framer3.
 *    And, we need to keep reset signal until framer3 is disabled completely.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ReleaseDbmReset(VOID)
{
    UINT32 u4Reg;

    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= 0x7FFFFFFF;                            // Clear "Reset" bit
    u4Reg |= (1 << 30);                             // Set "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
	 _DMX_SetDbmChannel4(TRUE,TRUE); // enable dbm channel4 default.
    _DMX_Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_ResetDbmSafely
 *
 *  Reset DBM safely (at packet boundary) in three steps.
 */
//-----------------------------------------------------------------------------
BOOL _DMX_ResetDbmSafely(VOID)
{
    BOOL fgRet = TRUE;
    UINT32 u4Reg, u4Counter;

    // 1/3. Disable DBM
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg &= ~(1 << 30);                            // Clear "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);

    // 2/3. Polling the value of 0x20017410
    //      if bit[16..12] != 0x10
    //          then reset DBM
    u4Counter = 0;
    while (u4Counter < DMX_MAX_DBM_WAIT_COUNT)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
        if (((u4Reg >> 12) & 0x1F) == 0x10)
        {
            x_thread_delay(1);
            u4Counter++;
        }
        else
        {
            break;
        }
    }

    // 3/3. Reset DBM which also resets Framer and PID index table. Enable DBM.
    if (u4Counter == DMX_MAX_DBM_WAIT_COUNT)
    {
        LOG(3, "Failed to reset DBM safely (still outputing TS packets)!\n");
        fgRet = FALSE;
    }
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
    u4Reg |= 0x80000000;                            // Set "Reset" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
    u4Reg &= 0x7FFFFFFF;                            // Clear "Reset" bit
    u4Reg |= (1 << 30);                             // Set "Enable" bit
    DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Reg);
     _DMX_SetDbmChannel4(TRUE,TRUE); // enable dbm channel4 default.
    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_ClearInterruptQueue
 */
//-----------------------------------------------------------------------------
VOID _DMX_ClearInterruptQueue(VOID)
{
    UINT32 i;

    _DMX_Lock();

    DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
    DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFTuP
 *
 *  This function resets the uP, and sets its "boot address" to 0.
 */
//-----------------------------------------------------------------------------
VOID _DMX_ResetFTuP(VOID)
{
    // Reset uP and its booting address.
    UINT32 u4Reg;
    _DMX_Lock();
    _fgMicroProcessorStopped = TRUE;
    _DMX_Unlock();
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg |= 0x81000000;                        // Disable "FTuP Stalling"
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    while (1)
    {
        if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
        {
            break;
        }
    }

    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0xFeFF0000;        // Enable "FTuP Stalling"; Clear "Boot address"
    u4Reg |= 0x1;               // Change "Boot address" to 1.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);        // Write it twice!

    _DMX_ClearInterruptQueue();

    // Enable uP
    u4Reg = DMXCMD_READ32(DMX_REG_FTuP_CONTROL);
    u4Reg &= 0x7FFF0000;        // Change "Boot address" to 0.
    DMXCMD_WRITE32(DMX_REG_FTuP_CONTROL, u4Reg);
    _DMX_Lock();
    _fgMicroProcessorStopped = FALSE;
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_EnableFTI
 *  Enable or disable FTI
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _DMX_EnableFTI(BOOL fgEnable)
{
    UINT32 u4Ctrl, i;

    u4Ctrl = DMXCMD_READ32(DMX_REG_CONTROL);
    if (fgEnable)
    {
        // Enable FTI
        u4Ctrl &= 0x3fffffff;
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        // Enable uP
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0x7fffffff;
        _DMX_Lock();
        _fgMicroProcessorStopped = FALSE;
        _DMX_Unlock();
    }
    else
    {
        // Halt uP
        //DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x80000000;
        // MT5391, turn on "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) |= 0x81000000;   // MT5391
        while (1)
        {
            if ((DMXCMD_READ32(DMX_REG_FTuP_CONTROL) & (1 << 30)) != 0)
            {
                break;
            }
        }

        // MT5391, turn off "bit[24]: disble uP stall"
        DMXCMD_REG32(DMX_REG_FTuP_CONTROL) &= 0xfeffffff;

        // Clear all interrupts
        for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
        {
            DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
            DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
        }

        // Halt FTI
        _DMX_Lock();
        _fgMicroProcessorStopped = TRUE;
        _DMX_Unlock();
        u4Ctrl = ((u4Ctrl & 0x3fffffff) | 0x40000000);
        DMXCMD_WRITE32(DMX_REG_CONTROL, u4Ctrl);

        while (1)
        {
            // Check if FTI is halted
            if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
            {
                x_thread_delay(1);
            }
            else
            {
                break;
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** _DMX_Reset
 *  Reset FTI
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Reset(void)
{
    UINT32 i;

    _DMX_Lock();

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    // Reset FTI
    _fgMicroProcessorStopped = TRUE;
    DMXCMD_WRITE32(DMX_REG_CONTROL, 0x8f000000);

    _DMX_Unlock();

    // Wait for ready
    while (1)
    {
        // Check if FTI is halted
        if (0x1 != ((DMXCMD_READ32(DMX_REG_CONTROL) >> 28) & 0x3))
        {
            x_thread_delay(1);
        }
        else
        {
            break;
        }
    }

#ifndef CC_DMX_FPGA_TEST
#ifdef __PCR_RECOVERY__
    // Initialize PCR whenever demux is reset
    if (!_DMX_GetIsPanicReset())
    {
        _DMX_PcrInit();
    }
    else
    {
        _DMX_PcrReInitHW();
    }
#endif  // __PCR_RECOVERY__
#endif

    // Set to previous value before reset

    //Framer 2  input source
    _DMX_SetDbm_InputSource(2, _eDmxDbm2Input);

    //Framer 3 input source
    _DMX_SetDbm_InputSource(3, _eDmxDbm3Input);

	_DMX_SetDbm_InputSource(4,_eDmxDBm4Input);

	//set FTI record buffer register to 27MHZ(432/16)
	DMXCMD_WRITE32(DMX_REG_REC_CONFIG1,0x10);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetFrontEnd
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T _DMX_GetFrontEnd(UINT8 u1TsIdx)
{
    DMX_FRONTEND_T eFE;

    if (u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return DMX_FE_NULL;
    }

	if(u1TsIdx == DMX_FRAMER_TOTAL_INDEX)
	{
	   return DMX_FE_NULL;
	}

    _DMX_Lock();
    eFE = _aeDmxFramerInput[u1TsIdx];
    _DMX_Unlock();

    return eFE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFrontEndEx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFrontEndEx(UINT8 u1Idx, DMX_FRONTEND_T eFrontEnd)
{
    DMX_PINMUX_SEL_T ePinmux;
#if defined(CC_MT5890) || defined(CC_MT5882)
    DMX_EXT_CKGEN_T eCkgen = DMX_CKGEN_EXT_0;
#endif
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;
    DMX_EDGE_T eEdge;
    DMX_SYNC_T eSync;

    if (u1Idx >= DMX_FRAMER_COUNT)
    {
        LOG(0, "Tsindex 2 has no framer or wrong tsindex!\n");
        return FALSE;
    }

	if(u1Idx == DMX_FRAMER_TOTAL_INDEX)
	{
	   LOG(0,"for dbm5, not need config the frontend.\r\n");
	   return FALSE;
	}

    if (eFrontEnd == DMX_FE_NO_TSVALID)
    {
        // this is for sepecial two tuner config, there is no TS_VAILD input
        // set it to 2 bit mode with serial input for ts index 0 and 1
        _DMX_Lock();
        _afgNoTSValid[u1Idx] = TRUE;
        _DMX_Unlock();

        if (!_DMX_InitMonThread())
        {
            LOG(3, "Fail to start monitor thread\n");
        }

        if (u1Idx == 0)
        {
            DMXCMD_WRITE32(DMX_REG_CONFIG2,
                           (DMXCMD_READ32(DMX_REG_CONFIG2) & (0xFCFFFFFF)) | 0x02000000);
        }
        else if (u1Idx == 1)
        {
            DMXCMD_WRITE32(DMX_REG_CONFIG2,
                           (DMXCMD_READ32(DMX_REG_CONFIG2) & (0xF3FFFFFF)) | 0x08000000);
        }
#if defined(CC_MT5890)
        else if (u1Idx == 2)
        {
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3,
                           (DMXCMD_READ32(DMX_REG_FRAMER_CTRL3) & (0xFFFFFFF3)) | 0x00000008);
        }
#endif
        else if (u1Idx == 3)
        {
            DMXCMD_WRITE32(DMX_REG_FRAMER_CTRL3,
                           (DMXCMD_READ32(DMX_REG_FRAMER_CTRL3) & (0xFFFFFFFC)) | 0x00000002);
        }
        else
        {
            LOG(0, "[%s]Invalid TS index %d\n", __FUNCTION__, u1Idx);
        }
        return TRUE;
    }


    //--------------------
    // Set TSin bonding check
    //--------------------
#ifndef CC_DMX_EMULATION
    if (eFrontEnd > 0x4) //external ts
    {
        if (!_DMX_Support_Tsin())
        {
            LOG(0, "Dmx don't support external ts in.\n\n");
            return FALSE;
        }
    }
#endif  // CC_DMX_EMULATION


    //--------------------
    // Set async fifo reset
    //--------------------
    if (eFrontEnd >= 0x4)    // External + CI
    {
        _DmxSetAsyncFifoReset(u1Idx, FALSE);
    }
    else
    {
        _DmxSetAsyncFifoReset(u1Idx, TRUE);
    }

    //--------------------
    // Set pinmux
    //--------------------
    if (eFrontEnd >= 0x10)   // External
    {
        ePinmux = (DMX_PINMUX_SEL_T)eFrontEnd;
        if (((UINT32)eFrontEnd & 0xF) == 2)
        {
            ePinmux = (DMX_PINMUX_SEL_T)((UINT32)eFrontEnd - 2);
        }
        _DmxSetExtPinmux(ePinmux);
    }

    if ((u1Idx == 2) || (u1Idx == 3))
    {
        if ((eFrontEnd == DMX_FE_CI) ||
            (eFrontEnd == DMX_FE_INTERNAL) ||
            (eFrontEnd >= 0x10))
        {
            _DMX_SetDbm_InputSource(u1Idx, DMX_DBM_INPUT_FRAMER);
        }
        else if (eFrontEnd == DMX_FE_DDI)
        {/*set to the framer.*/
           _DMX_SetDbm_InputSource(u1Idx,DMX_DBM_INPUT_FRAMER);
        }
		else if(eFrontEnd == DMX_FE_TSOUT)
		{/*set the framer.*/
		   _DMX_SetDbm_InputSource(u1Idx,DMX_DBM_INPUT_FRAMER);
		}
    }

    //--------------------
    // Set Ext ckgen
    //--------------------
    if (eFrontEnd == DMX_FE_CI)
    {
        _DmxSetExtCkgen(u1Idx, DMX_CKGEN_CI);
    }

    if (eFrontEnd >= 0x10)   // External
    {
        switch ((UINT32)(eFrontEnd >> 4) & 0xF)
        {
#if defined(CC_MT5890)
        case 1:
        default:
            eCkgen = DMX_CKGEN_EXT_0;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            eCkgen = DMX_CKGEN_EXT_2;
            break;
        case 7:
        case 8:
		case 9:
            eCkgen = DMX_CKGEN_EXT_3;
            break;
        case 10:
		    eCkgen = DMX_CKGEN_EXT_4;
            break;
		case 11:
            eCkgen = DMX_CKGEN_CI;
            break;
#else //5882
		case 1:
        default:
        case 2:
			eCkgen = DMX_CKGEN_EXT_0;
			break;
        case 3:
        case 4:
        case 5:
        case 6:
            eCkgen = DMX_CKGEN_EXT_2;
            break;
        case 7:
            eCkgen = DMX_CKGEN_EXT_3;
            break;
        case 8:
            eCkgen = DMX_CKGEN_EXT_4;
            break;
        case 10:
            eCkgen = DMX_CKGEN_CI;
            break;

#endif		
        }
        _DmxSetExtCkgen(u1Idx, eCkgen);
    }

    //--------------------
    // Set framer mode
    //--------------------
    UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));
    eEdge = prDmxFeCfgTable[u1Idx].eEdge;
    eSync = prDmxFeCfgTable[u1Idx].eSync;

    // force to use external sync for no TS_VAL signal
    if (_afgNoTSValid[u1Idx] == TRUE)
    {
        eSync = DMX_SYNC_EXTERNAL;
    }

    if (eFrontEnd == DMX_FE_INTERNAL)
    {
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL,
                           (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
    }
    else if (eFrontEnd == DMX_FE_DDI)
    {
#ifdef CC_DMX_EMULATION
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, TRUE, TRUE);
#else
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, FALSE, TRUE);
#endif // CC_DMX_EMULATION
    }
    else if (eFrontEnd == DMX_FE_TSOUT)
    {
#ifdef CC_DMX_EMULATION
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, TRUE, TRUE);
#else
        _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL, FALSE, TRUE);
#endif // CC_DMX_EMULATION
    }
    else
    {
        switch ((UINT32)eFrontEnd & 0xF)
        {
        case 0:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_SERIAL,
                               (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        case 1:
        default:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_PARALLEL,
                               (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        case 2:
            _DMX_SetFramerMode(u1Idx, DMX_FRAMER_TWOBIT,
                               (eSync == DMX_SYNC_EXTERNAL), (eEdge == DMX_EDGE_POSITIVE));
            break;
        }
    }

    //--------------------
    // Set framer input
    //--------------------
    _DmxSetTSInputCkgen(u1Idx, eFrontEnd);

    //--------------------
    // set framer 2 and framer 3 dbm input source
    //--------------------

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_ResetFramer
 */
//-----------------------------------------------------------------------------
void _DMX_ResetFramer(UINT8 u1Framer)
{
//#if !defined(CC_DMX_EMULATION) && !defined(__MODEL_slt__)
    UINT32 u4Ctrl;
    UINT8 i;

	if(u1Framer == DMX_FRAMER_TOTAL_INDEX)
	{
	  return ;
	}

    // Reset framer - bit12 for framer0 reset, bi13 for framer 1 reset
    if (u1Framer == 0)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER0_FSM);
            if ((u4Ctrl & 0xFFFFF) == 0x10101)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
            if ((u4Ctrl & 0x3) == 0x0)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x1000);
        u4Ctrl |= 0x1000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x1000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else if (u1Framer == 1)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x10);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER1_FSM);
            if ((u4Ctrl & 0xFFFFF) == 0x10101)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
            if ((u4Ctrl & 0xC) == 0x0)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x2000);
        u4Ctrl |= 0x2000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x2000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x10);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else if (u1Framer == 2)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) & (~0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_FSM);
            if ((u4Ctrl & 0xFFFFF) == 0x10101)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
            if ((u4Ctrl & 0x30) == 0x0)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x4000);
        u4Ctrl |= 0x4000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x4000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) | (0x1);
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);
    }
    else if (u1Framer == 3)
    {
        // disable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x100);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // wait for framer state to idle
        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER3_FSM);
            if ((u4Ctrl & 0xFFFFF) == 0x10101)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        for (i = 0; i < 100; i++)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
            if ((u4Ctrl & 0xC0) == 0x0)
            {
                // in idle state
                break;
            }
            HAL_Delay_us(10);
        }

        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x8000);
        u4Ctrl |= 0x8000;
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        //x_thread_delay(1);
        HAL_Delay_us(1);
        u4Ctrl &= (~0x8000);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

        // enable framer
        u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x100);
        DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
    }
    else
    {
        ASSERT(0);
    }

    LOG(9, "Reset framer\n");
//#endif
}

//-----------------------------------------------------------------------------
/** _DMX_SetFramerEnabled
 */
//-----------------------------------------------------------------------------
void _DMX_SetFramerEnabled(UINT8 u1Framer, BOOL fgEnable)
{
    UINT32 u4Ctrl;
    UINT8 i;

    switch(u1Framer)
    {
    case 0:
        if (fgEnable)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x1);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        }
        else
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x1);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

            // wait for framer state to idle
            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER0_FSM);
                if ((u4Ctrl & 0xFFFFF) == 0x10101)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }

            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
                if ((u4Ctrl & 0x3) == 0x0)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }
        }
        break;
    case 1:
        if (fgEnable)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x10);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        }
        else
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x10);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

            // wait for framer state to idle
            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER1_FSM);
                if ((u4Ctrl & 0xFFFFF) == 0x10101)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }

            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
                if ((u4Ctrl & 0xC) == 0x0)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }
        }
        break;
    case 2:
        if (fgEnable)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) | (0x1);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);
        }
        else
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL) & (~0x1);
            DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);

            // wait for framer state to idle
            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_FSM);
                if ((u4Ctrl & 0xFFFFF) == 0x10101)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }

            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
                if ((u4Ctrl & 0x30) == 0x0)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }
        }
        break;
    case 3:
        if (fgEnable)
        {
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) | (0x100);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);
        }
        else
        {
            // disable framer
            u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL) & (~0x100);
            DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

            // wait for framer state to idle
            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER3_FSM);
                if ((u4Ctrl & 0xFFFFF) == 0x10101)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }

            for (i = 0; i < 100; i++)
            {
                u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_STATUS_REG_3);
                if ((u4Ctrl & 0xC0) == 0x0)
                {
                    // in idle state
                    break;
                }
                HAL_Delay_us(10);
            }
        }
        break;
    default:
        LOG(0, "%s - unsupported framer %u\n", __FUNCTION__, u1Framer);
        break;
    }
}

//-----------------------------------------------------------------------------
/** _DMX_GetFramerIndex
 *  Get DTV framer
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_GetFramerIndex(void)
{
    // Fixme, 0 use as default first
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMX_Start
 *  Start demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Start(void)
{
    UINT32 u4Ctrl;

    _DMX_Lock();

    // Enable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl |= (1 << 0) | (1 << 4);  // enalbe framer 0 and framer 1
    u4Ctrl |= (1 << 8);            // enable framer 3
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    //Enalbe framer 2
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
    u4Ctrl |= (1 << 0);
    DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Stop
 *  Stop demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Stop(void)
{
    UINT32 u4Ctrl, i;

    _DMX_Lock();

    // Disable framers
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);
    u4Ctrl &= ~((1 << 0) | (1 << 4)); // disable framer 0 and 1
    u4Ctrl &= ~(1 << 8); // disable framer 3
    DMXCMD_WRITE32(DMX_REG_FRAMER_CONTROL, u4Ctrl);

    //Disable Framer 2
    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
    u4Ctrl &= ~(1 << 0);
    DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Ctrl);

    // Clear all interrupts
    for (i = 0; i < DMX_INT_QUEUE_DEPTH; i++)
    {
        DMXCMD_WRITE32(DMX_REG_DBM_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DBM_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_DESCRAMBLER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_STEER_NONERR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_FTuP_NONERR_STATUS_REG1, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_ERROR_STATUS_REG, 1);
        DMXCMD_WRITE32(DMX_REG_PCR_NONERR_STATUS_REG1, 1);
    }

    _DMX_Unlock();

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DmxOneuCodeInit
 */
//-----------------------------------------------------------------------------
static BOOL _DmxOneuCodeInit(void)
{
#ifdef ENABLE_MULTIMEDIA
    DMX_DDI_T rDDI;
#endif // ENABLE_MULTIMEDIA

    if (!_DmxInitBroadcastTS())
    {
        return FALSE;
    }


    _DMX_SetInputMode(DMX_HALF_PUSH);

#ifdef ENABLE_MULTIMEDIA
    if (!_DMX_DDI_Init())
    {
        return FALSE;
    }

    rDDI.eMode = DMX_DDI_MODE_SINGLE;
   // /* Set  M=N=0,just use delay byte to adjust speed:[(432*8)/delay byte]Mbps */
    rDDI.u4RateN = 0x0;
    rDDI.u4RateM = 0x0;
    if (!DMX_DDI_Set((UINT32)(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE), &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return FALSE;
    }

    if (!_DMX_PS_CPU_Init())
    {
        return FALSE;
    }

    if (!_DMX_MM_Init())
    {
        return FALSE;
    }
    /*
        // Set DBM to bypass mode and keep all TS packets
        u4Ctrl = DMXCMD_READ32(DMX_REG_DBM_CONTROL);
        u4Ctrl |= (1 << 30);
        DMXCMD_WRITE32(DMX_REG_DBM_CONTROL, u4Ctrl);
    */
#endif  // ENABLE_MULTIMEDIA

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_Init
 *  Initialize demux - can run multiple times
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_Init(void)
{
    UINT8 i;

    DMX_SCRAMBLE_CONFIG_T* prDmxScrmTbl;

    UNUSED(DRVCUST_OptQuery(eDmxScrambleDetectTable, (UINT32 *)(void *)&prDmxScrmTbl));

    _DMX_Lock();
    for (i=0; i<DMX_FRAMER_COUNT; i++)
    {
        _au2DmxFramerPktSize[i] = 188;
        _afgNoTSValid[i] = FALSE;
    }
    _DMX_Unlock();

    // Select clock and pin mux
    _DmxCkgenInit(TRUE);

    _DMX_SetErrLevel(DMX_ERR_RESET);

    if (!_DmxOneuCodeInit())
    {
        return FALSE;
    }

    _DMX_SetScrambleSchemeEx(prDmxScrmTbl->eType, prDmxScrmTbl->u1Flag);

    if (!_DMX_VCodeInit())
    {
        return FALSE;
    }

    // Initialize handler if integrated with VLD
    if (!_DMX_InitHandler())
    {
        return FALSE;
    }

    if (!_DMX_InitPSIHandler())
    {
        return FALSE;
    }

#ifdef ENABLE_MULTIMEDIA
#ifdef CC_DMX_AUDIO_PREBUF
    if (!_DMX_InitAudHandler())
    {
        return FALSE;
    }
#endif  // CC_DMX_AUDIO_PREBUF

    if (!GCPU_Init(0))
    {
        return FALSE;
    }

    if (!_DMX_PVRPlay_Init())
    {
        return FALSE;
    }

    if (!_DMX_GetIsPanicReset())
    {
        if (!_DMX_MUL_Init())
        {
            return FALSE;
        }
    }
#endif  // ENABLE_MULTIMEDIA

#ifdef CC_DMX_TS_LOOPBACK
    if (!_DMX_Loopback_Init())
    {
        return FALSE;
    }
#endif  // CC_DMX_TS_LOOPBACK

    if (!_DmxInitHWPath())
    {
        return FALSE;
    }

    if (!_DMX_InitRawTsHandler())
    {
        return FALSE;
    }

#ifdef CC_FBM_SUPPORT_DMXPID
    if (!_DMX_PSIBufferInit())
    {
        return FALSE;
    }
#endif

#ifdef CC_DMX_TS130
    if (!_DMX_TS130_Init())
    {
        return FALSE;
    }
#endif // CC_DMX_TS130

#ifdef CC_DMX_SUPPORT_CIPLUS14
   DMX_CiPlus14_Init();
#endif

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetInputMode
 */
//-----------------------------------------------------------------------------
void _DMX_SetInputMode(DMX_INPUT_MODE_T rMode)
{
    UINT32 u4Reg;

    _DMX_Lock();
    u4Reg = DMXCMD_READ32(DMX_REG_CONFIG2);
    _DMX_Unlock();

    u4Reg &= 0x3fffffff;

    switch (rMode)
    {
    case DMX_PUSH:
        break;

    case DMX_HALF_PUSH:
        u4Reg |= (1U << 30);
        break;

    case DMX_PULL:
        u4Reg |= (3U << 30);
        break;

    default:
        ASSERT(0);
        break;
    }

    _DMX_Lock();
    DMXCMD_WRITE32(DMX_REG_CONFIG2, u4Reg);

    // Enable "DBM input full" signal to TS input device
#if 0 // todo
    //bit31~24 -FMR3-EHT2   bit23~16 -FMR3-EHT1
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL);
    u4Reg &= ~(0x7);
    u4Reg |= 0x1;
    DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, u4Reg);
#else
    u4Reg = DMXCMD_READ32(DMX_REG_DBM_BUF_CTRL) & (~ 0x3);
    u4Reg |= 0x1;
    DMXCMD_WRITE32(DMX_REG_DBM_BUF_CTRL, u4Reg);
#endif
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_SetIgnorePESLen
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetIgnorePESLen(BOOL fgEnable)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF00);
    if (fgEnable)
    {
        CT_SETTING = u4Ctrl | 0x100;
    }
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_EnablePower
 */
//-----------------------------------------------------------------------------
VOID _DMX_EnablePower(BOOL fgEnable)
{
    _DmxCkgenInit(fgEnable);
}


//-----------------------------------------------------------------------------
/** _DMX_SetDuplicateScheme
 *  fgOldMethod - false means use new method to check duplicate packet
 *                true means use old method to check duplicate packet
 *  Old method: drop packets only if CC is the same and PUSI = 0
 *  New method: drop packets if both CC and PUSI are the same as that of last packet
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetDuplicateScheme(BOOL fgOldMethod)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
  
    u4Ctrl = SECTIONFILTER_SETTING & (~0xFF00);
    if (fgOldMethod)
    {
        // drop packets only if CC is the same and PUSI = 0
        u4Ctrl |= 0x100;
    }
    else
    {
        // drop packets if both CC and PUSI are the same as that of last packet
    }
    
    SECTIONFILTER_SETTING = u4Ctrl;
    
    _DMX_Unlock();
}


//-----------------------------------------------------------------------------
/** _DMX_SetScrambleScheme
 *  fgOrg - false means use PES header to decide scramble state
 *          true means use scramble flag to decide scramble state
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetScrambleScheme(BOOL fgOrg)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFF0000);
    if (fgOrg)
    {
        u4Ctrl |= 0x10000;
    }
    CT_SETTING = u4Ctrl;
    _DMX_Unlock();

#ifdef TIME_SHIFT_SUPPORT
    FVR_SetScrambleScheme(fgOrg);
#endif // TIME_SHIFT_SUPPORT
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleScheme
 *  return TRUE: new, FALSE: org
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetScrambleScheme(void)
{
    UINT32 u4Ctrl;

    u4Ctrl = (CT_SETTING) & 0x10000;
    if (u4Ctrl != 0) // original
    {
        return FALSE;
    }

    return TRUE;    // new
}


//-----------------------------------------------------------------------------
/** _DMX_SetScrambleSchemeEx
 * u1Flag: bit0=1, both pes=0, flag=0 as scramble
 *         bit1=1, pes=0, flag=1 as scramble
 *         bit2=1, pes=1, flag=0 as scramble
 *         bit3=1, both pes=1, flag=1 as scramble
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T eType, UINT8 u1Flag)
{
    UINT32 u4Ctrl;

    _DMX_Lock();
    u4Ctrl = (CT_SETTING) & (~0xFFFF0000);
    if (eType == DMX_SCRAMBLE_TSFLAG_ONLY)
    {
        u4Ctrl |= 0x10000;
    }
    else if (eType == DMX_SCRAMBLE_PESSTART_ONLY)
    {
        u4Ctrl |= 0x0;
    }
    else if (eType == DMX_SCRAMBLE_BOTH_TSFLAG_PESSTART)
    {
        u4Ctrl |= 0x20000;
        u4Ctrl |= ((UINT32)(u1Flag & 0xF)<<24);
    }
    CT_SETTING = u4Ctrl;
    _DMX_Unlock();

//#ifdef TIME_SHIFT_SUPPORT
//    FVR_SetScrambleScheme(fgOrg);
//#endif // TIME_SHIFT_SUPPORT
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleSchemeEx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetScrambleSchemeEx(DMX_SCRAMBLE_TYPE_T *peType, UINT8 *pu1Flag)
{
    UINT32 u4Ctrl;

    if (peType == NULL)
    {
        return FALSE;
    }

    _DMX_Lock();
    u4Ctrl = CT_SETTING;
    _DMX_Unlock();

    switch (((u4Ctrl & 0xFF0000) >> 16))
    {
    case 0:
    default:
        *peType = DMX_SCRAMBLE_PESSTART_ONLY;
        break;
    case 1:
        *peType = DMX_SCRAMBLE_TSFLAG_ONLY;
        break;
    case 2:
        *peType = DMX_SCRAMBLE_BOTH_TSFLAG_PESSTART;
        if (pu1Flag == NULL)
        {
            return FALSE;
        }
        *pu1Flag = (UINT8)((u4Ctrl >> 24) & 0xF);
        break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_GetEfuseStatus
 *  Show _DMX_GetEfuseStatus information
 */
//-----------------------------------------------------------------------------
BOOL _DMX_GetEfuseStatus(void)
{
    if ((BIM_READ32(0x664) & 0x4) == 0x4)
    {
        // Demod does not clear VCT.  DMX has the choice to clear VCT or not.
        LOG(0, "Efuse IC: YES!\n");
        LOG(0, "Demod does not clear VCT.  DMX has the choice to clear VCT or not.!\n");
        LOG(0, "define DMX_NT_UCODE_DISABLE,demux will not clear VCT.if not ,demux will clear it!\n");
        return TRUE;
    }
    else
    {
        // Demod clears VCT.
        LOG(0, "Efuse IC: NO NO!\n");
        LOG(0, "Demod clears VCT.!\n");
        return FALSE;
    }
}

//-----------------------------------------------------------------------------
/** _DMX_Version
 *  Show DMX version information
 */
//-----------------------------------------------------------------------------
VOID _DMX_Version(void)
{
    UINT32 u4Ctrl;
    UINT16 u2CodeVersion;
    UINT8 u1Major, u1Minor;
#if defined(CC_MT5890) || defined(CC_MT5882)
    UINT32 u4Reg;
#endif

    LOG(0, "----------------------------------------\n");
    LOG(0, "DMX information\n");
    LOG(0, "----------------------------------------\n");

    u2CodeVersion = (UINT16)(_u4uCodeVersion & 0xffff);
    u1Major = (UINT8)((u2CodeVersion >> 8) & 0xf);
    u1Minor = (UINT8)(u2CodeVersion & 0xff);
    LOG(0, "Demux uCode version: %x.%x\n", u1Major, u1Minor);

    u4Ctrl = DMXCMD_READ32(DMX_REG_FRAMER_CONTROL);

    //framer 0
    LOG(0, "Framer0: %s, %s, %s, %s\n",
        (u4Ctrl & 0x1) ? "Enable" : "Disable",
        (u4Ctrl & 0x2) ? "Parallel" : "Serial",
        (u4Ctrl & 0x4) ? "External sync" : "Internal sync",
        (u4Ctrl & 0x8) ? "Positive" : "Negative");
    //framer 1
    LOG(0, "Framer1: %s, %s, %s, %s\n",
        (u4Ctrl & 0x10) ? "Enable" : "Disable",
        (u4Ctrl & 0x20) ? "Parallel" : "Serial",
        (u4Ctrl & 0x40) ? "External sync" : "Internal sync",
        (u4Ctrl & 0x80) ? "Positive" : "Negative");

    //Framer 2
    u4Reg = IO_READ32(DEMUX0_BASE, 0x924);
    LOG(0,"Framer2: %s, %s, %s, %s\n",
           (u4Reg & 0x1) ? "Enable" : "Disable",
           (u4Reg & 0x2) ? "Parallel" : "Serial",
           (u4Reg & 0x4) ? "External sync" : "Internal sync",
           (u4Reg & 0x8) ? "Positive" : "Negative");

    //framer 3
    LOG(0, "Framer3: %s, %s, %s, %s\n",
        (u4Ctrl & 0x100) ? "Enable" : "Disable",
        (u4Ctrl & 0x200) ? "Parallel" : "Serial",
        (u4Ctrl & 0x400) ? "External sync" : "Internal sync",
        (u4Ctrl & 0x800) ? "Positive" : "Negative");

    LOG(0, "----------------------------------------\n");
}


//-----------------------------------------------------------------------------
/** _DMX_IsMicroProcessorStopped
 *  Return TRUE if the micro-processor is stopped according to the global
 *  variable _fgMicroProcessorStopped.  Otherwise, return FALSE.
*/
//-----------------------------------------------------------------------------
BOOL _DMX_IsMicroProcessorStopped(void)
{
    BOOL fgMicroProcessorStopped;

    _DMX_Lock();
    fgMicroProcessorStopped = _fgMicroProcessorStopped;
    _DMX_Unlock();

    return fgMicroProcessorStopped;
}


//-----------------------------------------------------------------------------
/** _DMX_SetDbm_InputSource
 */
//-----------------------------------------------------------------------------
void _DMX_SetDbm_InputSource(UINT8 u1TsIdx, DMX_DBM_INPUT_SOURCE_T eSource)
{
    UINT32 u4Reg;

    if ((u1TsIdx != 2) && (u1TsIdx != 3)&&(u1TsIdx !=4))
    {
        LOG(0, "Tsindex Error, _DMX_SetDbm_InputSource only support framer 2 and 3,4.\n");
        return;
    }

    if (u1TsIdx == 2)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
        u4Reg &= ~(0x3 << 24);

        switch(eSource)
        {
        case DMX_DBM_INPUT_DDI:
            u4Reg |= (0x2 << 24);
            break;

        case DMX_DBM_INPUT_FRAMER:
            u4Reg |= (0x1 << 24);
            break;

        case DMX_DBM_INPUT_PB:
            u4Reg |= (0x0 << 24);
            break;

        case DMX_DBM_INPUT_PB2:
            u4Reg |= (0x3 << 24);
            break;

        default:
            break;
        }

        DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);

        _eDmxDbm2Input = eSource;

        //fix hw limitation, framer2 dbm input source must different with framer3 inputsource,except framer input source,
        //example,framer 2 choose ddi, framer 3 must choose anther input source.
        if (eSource != DMX_DBM_INPUT_FRAMER) //framer 2 and 3 can receive data from framer input source.
        {
            if (eSource == _eDmxDbm3Input)   //connect same input source
            {
                u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
                u4Reg &= ~(0x3 << 26);

                u4Reg |= (0x1 << 26);  //connect framer input source.
                DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
                _eDmxDbm3Input = DMX_DBM_INPUT_FRAMER;
            }
			else if(eSource == _eDmxDBm4Input)
			{
			   u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
               u4Reg &= ~(0x3 << 28);
	           u4Reg |= (0x2 << 28);
			   DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
			   _eDmxDBm4Input = DMX_DBM_INPUT_PB3; //connect to pb3

			}
        }

    }
    else if (u1TsIdx == 3)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
        u4Reg &= ~(0x3 << 26);

        switch(eSource)
        {
        case DMX_DBM_INPUT_DDI:
            u4Reg |= (0x0 << 26);
            break;

        case DMX_DBM_INPUT_FRAMER:
            u4Reg |= (0x1 << 26);
            break;

        case DMX_DBM_INPUT_PB:
            u4Reg |= (0x2 << 26);
            break;

        case DMX_DBM_INPUT_PB2:
            u4Reg |= (0x3 << 26);
            break;

        default:
            break;
        }
        DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);

        _eDmxDbm3Input = eSource;


        //fix hw limitation, framer2 dbm input source must different with framer3 inputsource,except framer input source,
        //example,framer 2 choose ddi, framer 3 must choose anther input source.
        if (eSource != DMX_DBM_INPUT_FRAMER) //framer 2 and 3 can receive data from framer input source.
        {
            if (eSource == _eDmxDbm2Input)   //connect same input source
            {
                u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
                u4Reg &= ~(0x3 << 24);

                u4Reg |= (0x1 << 24);  //connect framer input source.
                DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
                _eDmxDbm2Input = DMX_DBM_INPUT_FRAMER;
            }
			else if(eSource == _eDmxDBm4Input)
			{
			   u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
               u4Reg &= ~(0x3 << 28);
	           u4Reg |= (0x2 << 28);
			   DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
			   _eDmxDBm4Input = DMX_DBM_INPUT_PB3; //connect to pb3

			}
        }
    }
	else if (u1TsIdx == 4)
	{
	    u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
        u4Reg &= ~(0x3 << 28);

        switch(eSource)
        {
        case DMX_DBM_INPUT_DDI:
            u4Reg |= (0x0 << 28);
            break;

        case DMX_DBM_INPUT_PB2:
            u4Reg |= (0x1 << 28);
            break;

        case DMX_DBM_INPUT_PB3:
            u4Reg |= (0x2 << 28);
            break;

        case DMX_DBM_INPUT_PB:
            u4Reg |= (0x3 << 28);
            break;

        default:
            break;
        }
        DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);

        _eDmxDBm4Input = eSource;

		if(eSource == _eDmxDbm2Input)
		{
          u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
          u4Reg &= ~(0x3 << 24);
		  u4Reg |=(0x1<<24);
	      DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
          _eDmxDbm2Input = DMX_DBM_INPUT_FRAMER;

		}
		else if(eSource == _eDmxDbm3Input)
		{
		  u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_UNLOCK_CONTROL);
          u4Reg &= ~(0x3 << 26);
          u4Reg |=(0x1<<26);
		  DMXCMD_WRITE32(DMX_REG_FRAMER_UNLOCK_CONTROL, u4Reg);
         _eDmxDbm3Input = DMX_DBM_INPUT_FRAMER;
		}
	}
    else
    {
        ASSERT(0);
    }
}


//-----------------------------------------------------------------------------
/** _DMX_GetDbm_InputSource
 */
//-----------------------------------------------------------------------------
DMX_DBM_INPUT_SOURCE_T _DMX_GetDbm_InputSource(UINT8 u1TsIdx)
{
    DMX_DBM_INPUT_SOURCE_T eSource = DMX_DBM_INPUT_FRAMER;

#if defined(CC_MT5890)
    if ((u1TsIdx != 2) && (u1TsIdx != 3) && (u1TsIdx != 4))
    {
        LOG(0, "Ts index Error, _DMX_SetDbm_InputSource only support framer 2, 3 and 4.\n");
        return DMX_DBM_INPUT_FRAMER;
    }
#else  //MT5882
    if ((u1TsIdx != 2) && (u1TsIdx != 3))
    {
        LOG(0, "Ts index Error, _DMX_SetDbm_InputSource only support framer 2 and 3.\n");
        return DMX_DBM_INPUT_FRAMER;
    }
#endif

    if (u1TsIdx == 2)
    {
        eSource = _eDmxDbm2Input;
    }
    else if (u1TsIdx == 3)
    {
        eSource = _eDmxDbm3Input;
    }
#if defined(CC_MT5890)
	else if(u1TsIdx == 4)
	{
	   eSource = _eDmxDBm4Input;
    }
#endif
    else
    {
         ASSERT(0);
    }

    return eSource;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFramerPktSize
 */
//-----------------------------------------------------------------------------
BOOL _DMX_SetFramerPktSize(UINT8 u1TsIndex, UINT16 u2PktSize)
{
    UINT32 u4Reg;

    if (u1TsIndex >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }

    if (_au2DmxFramerPktSize[u1TsIndex] == u2PktSize)
    {
        return TRUE;
    }

    if (u1TsIndex == 0)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL) & 0xFFFF0000;
        u4Reg |= 0x0100;
        u4Reg |= (UINT32)(u2PktSize & 0xFF);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
    }
    else if (u1TsIndex == 1)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL) & 0xFFFF;
        u4Reg |= 0x01000000;
        u4Reg |= ((UINT32)(u2PktSize & 0xFF) << 16);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL, u4Reg);
    }
    else if (u1TsIndex == 2)
    {
        // Framer2 size
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL2) & ~0xFF;
        u4Reg |= (UINT32)(u2PktSize & 0xFF);
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL2, u4Reg);

        // enable pre-byte scheme
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER2_CTRL);
        u4Reg |= 0x1 << 6;
        DMXCMD_WRITE32(DMX_REG_FRAMER2_CTRL, u4Reg);
    }
    else if (u1TsIndex == 3)
    {
        u4Reg = DMXCMD_READ32(DMX_REG_FRAMER_PREBYTE_CTRL2) & 0xFFFF0000;
        u4Reg |= 0x0100;
        u4Reg |= (UINT32)(u2PktSize & 0xFF);
        DMXCMD_WRITE32(DMX_REG_FRAMER_PREBYTE_CTRL2, u4Reg);
    }
    else
    {
        ASSERT(0);
    }

    _DMX_ResetFramer(u1TsIndex);

    _au2DmxFramerPktSize[u1TsIndex] = u2PktSize;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_SetSecurebufferRegion
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetSecurebufferRegion(UINT32 u4Start,  UINT32 u4End)
{
   DMXCMD_WRITE32(DMX_REG_SECURE_DRAM_START_ADDR, u4Start); //start
   DMXCMD_WRITE32(DMX_REG_SECURE_DRAM_END_ADDR, u4End); //end
}

//-----------------------------------------------------------------------------
/** _DMX_SetSecurebufferPattern
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetSecurebufferPattern(UINT32 u4Pattern)
{
   DMXCMD_WRITE32(DMX_REG_SECURE_PLAYBACK_PATTERN, u4Pattern); //pattern
}


//-----------------------------------------------------------------------------
/** _DMX_SetSecurebufferProtectEnable
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetSecurebufferProtectEnable(BOOL Enable)
{
    UINT32 u4Reg;

    u4Reg = DMXCMD_READ32(DMX_FTI_SECURITY_ACCESS);

    if (Enable)
    {
        u4Reg &= ~(0x1 << 5);
    }
    else
    {
        u4Reg |= 0x1 << 5;
    }

    DMXCMD_WRITE32(DMX_FTI_SECURITY_ACCESS, u4Reg);
}

//-----------------------------------------------------------------------------
/** _DMX_SetSecurePidIndex
 */
//-----------------------------------------------------------------------------
VOID _DMX_SetSecurePidIndex(UINT8 u1PidIdx, BOOL fgEnable)
{
   UINT32 u4Reg;

   ASSERT(u1PidIdx < DMX_NUM_PID_INDEX);

   if(u1PidIdx >= DMX_NUM_SECURE_PID_INDEX)
   {
     LOG(0,"Out of range,Hardware Support %d.\r\n",DMX_NUM_SECURE_PID_INDEX);
	 return;
   }

//For Oryx/Gazelle, secure control words are in DMEM, for capri, in REG 17940,17944
#if defined(CC_MT5890)
   if (u1PidIdx > 63)
   {
        u1PidIdx -= 64;
        u4Reg = DMX_SECURE_PLAYBACK_PIDINDEX2;
        u4Reg &= ~(0x1 << u1PidIdx);
        u4Reg |= (fgEnable ? 0x1 : 0x0) << u1PidIdx;
        DMX_SECURE_PLAYBACK_PIDINDEX2 = u4Reg;
   }
   else if (u1PidIdx < 64 && u1PidIdx > 31)
   {
        u1PidIdx -= 32;
        u4Reg = DMX_SECURE_PLAYBACK_PIDINDEX1;
        u4Reg &= ~(0x1 << u1PidIdx);
        u4Reg |= (fgEnable ? 0x1 : 0x0) << u1PidIdx;
        DMX_SECURE_PLAYBACK_PIDINDEX1 = u4Reg;
   }
   else
   {
        u4Reg = DMX_SECURE_PLAYBACK_PIDINDEX0;
        u4Reg &= ~(0x1 << u1PidIdx);
        u4Reg |= (fgEnable ? 0x1 : 0x0) << u1PidIdx;
        DMX_SECURE_PLAYBACK_PIDINDEX0 = u4Reg;
   }
#else  //MT5882
   if (u1PidIdx > 31)
   {
        u1PidIdx -= 32;
        u4Reg = DMXCMD_READ32(DMX_REG_SECURE_PLAYBACK_PIDINDEX1);
        u4Reg &= ~(0x1 << u1PidIdx);
        u4Reg |= (fgEnable ? 0x1 : 0x0) << u1PidIdx;
        DMXCMD_WRITE32(DMX_REG_SECURE_PLAYBACK_PIDINDEX1, u4Reg);
   }
   else
   {
        u4Reg = DMXCMD_READ32(DMX_REG_SECURE_PLAYBACK_PIDINDEX0);
        u4Reg &= ~(0x1 << u1PidIdx);
        u4Reg |= (fgEnable ? 0x1 : 0x0) << u1PidIdx;
        DMXCMD_WRITE32(DMX_REG_SECURE_PLAYBACK_PIDINDEX0, u4Reg);
   }
#endif
   
}

#ifdef CC_FAST_INIT
//-----------------------------------------------------------------------------
/** DMX_pm_resuem
 */
//-----------------------------------------------------------------------------
void DMX_pm_resume(void)
{
    // 1. Do init
    if (!_DMX_Init())
    {
        LOG(1, "Resume fail\n");
    }

    // 2. Enable interrupt controller
    if (!BIM_EnableIrq(VECTOR_DEMUX))
    {
        LOG(1, "Eanble DMX isr fail\n");
    }
    if (!BIM_EnableIrq(VECTOR_DDI))
    {
        LOG(1, "Eanble DDI isr fail\n");
    }
    if (!BIM_EnableIrq(VECTOR_UP0))
    {
        LOG(1, "Eanble GCPU isr fail\n");
    }
    if (!BIM_EnableIrq(VECTOR_MMU_GCPU))
    {
        LOG(1, "Eanble GCPU IOMMU isr fail\n");
    }
}
#endif  // CC_FAST_INIT

#ifdef CC_SUPPORT_STR
//-----------------------------------------------------------------------------
/** _DMX_pm_str_suspend
 */
//-----------------------------------------------------------------------------
void _DMX_pm_str_suspend(void)
{
    // 1. Disable interrupt controller
    if(!BIM_DisableIrq(VECTOR_DEMUX))
    {
        LOG(1, "Disable DMX isr fail\n");
    }
    if(!BIM_DisableIrq(VECTOR_DDI))
    {
        LOG(1, "Disable DDI isr fail\n");
    }

    //Disable DMX CLK
    CKGEN_WRITE32(CKGEN_BLOCK_CKENCFG1, CKGEN_READ32(CKGEN_BLOCK_CKENCFG1) & (~0xC0000));

}

//-----------------------------------------------------------------------------
/** _DMX_pm_str_resume
 */
//-----------------------------------------------------------------------------
void _DMX_pm_str_resume(void)
{
    // 1. Do init
    if (!_DMX_Init())
    {
        LOG(1, "Resume fail\n");
    }

    // 2. Enable interrupt controller
    if (!BIM_EnableIrq(VECTOR_DEMUX))
    {
        LOG(1, "Enable DMX isr fail\n");
    }
    if (!BIM_EnableIrq(VECTOR_DDI))
    {
        LOG(1, "Enable DDI isr fail\n");
    }
}
#endif
