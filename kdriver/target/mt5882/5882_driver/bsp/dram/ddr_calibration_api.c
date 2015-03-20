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
 * $Date: 2015/03/20 $
 * $RCSfile: pi_calibration_api.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_calibration_api.c
 *  Basic DRAMC calibration API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "drvcust_if.h"
#include "x_printf.h"
#include "x_ckgen.h"
#include "x_assert.h"
#include "x_bim.h"
#include "ddr.h"
#include "x_dram.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#ifdef DRAM_TX_EYE_SCAN
U32 u4tx_eye_scan_buf[16][49];
#endif

U8 ucswap_table[2][32] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31}, 
						{0, 1, 2, 3, 8, 9, 10, 11, 4, 5, 6, 7, 12, 13, 14, 15, 16, 17, 18, 19, 24, 25, 26, 27, 20, 21, 22, 23, 28, 29, 30, 31}};

#ifdef DEFAULT_DDR_CHA_BUS_X8
U32 DramcDQ4bitSwap(U32 value)
{
	U32 dq7to4, dq11to8, dq23to20, dq27to24;
	U32 result;

	//value[7:4]
	dq7to4 = (value & (0xf<<4))>>4;
	//value[11:8]
	dq11to8 = (value & (0xf<<8))>>8;
	//value[23:20]
	dq23to20 = (value & (0xf<<20))>>20;
	//value[27:24]
	dq27to24 = (value & (0xf<<24))>>24;

	result = value;
	mcSET_FIELD(result, dq7to4, 0x00000f00, 8);
	mcSET_FIELD(result, dq11to8, 0x000000f0, 4);
	mcSET_FIELD(result, dq23to20, 0x0f000000, 24);
	mcSET_FIELD(result, dq27to24, 0x00f00000, 20);

	return result;
}
#endif
//-------------------------------------------------------------------------
/** DramcCheckPllDline
 *  Check PLL DLINE compare value with confidence count.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param u4addr           (U32): register address to be read.
 *  @param ucbit_pos        (U8):  bit position of DLINE.
 *  @retval status          (PLL_PHASE_CAL_STATUS_T): DLINE_0 or DLINE_1 or DLINE_TOGGLE 
 */
//-------------------------------------------------------------------------
#ifdef DRAM_Pll_PHASE_CAL
PLL_PHASE_CAL_STATUS_T DramcCheckPllDline(U32 u4addr, U8 ucbit_pos)
{
    U8 ii, ucconf_count;
    U32 u4value, u4mask;
    PLL_PHASE_CAL_STATUS_T dline_status;
    
    u4mask = (U32)1<<ucbit_pos;
    ucconf_count = 0;
    for (ii=0; ii<PLL_PHASE_CAL_CONF_COUNT; ii++)
    {
        u4value = ucDram_Register_Read(u4addr);
        u4value = mcGET_FIELD(u4value, u4mask, ucbit_pos);
        ucconf_count = ucconf_count + (U8) u4value;        
    }    

    if (ucconf_count == 0)
    {
        dline_status = DLINE_0;        
    }
    else if (ucconf_count == PLL_PHASE_CAL_CONF_COUNT)
    {
        dline_status = DLINE_1;  
    }
    else
    {
        dline_status = DLINE_TOGGLE;  
    }

    mcSHOW_DBG_MSG2("DLINE=%d , count=%2d @ confidence=%2d\n", dline_status, ucconf_count, PLL_PHASE_CAL_CONF_COUNT);

    return dline_status;
}

//-------------------------------------------------------------------------
/** DramcPllPhaseCal
 *  start PLL Phase Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcPllPhaseCal(void)
{
    U8 ii;
    U32 u4value;
    PLL_PHASE_CAL_STATUS_T dline_status1, dline_status2;

    //===============================================
    //A1
    //set REF and FBK delay to 0 at initial
    mcSHOW_DBG_MSG2("\n============== A1 PLL ==============\n");
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0cc));
    u4value = u4value & 0xffffe0e0;
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0cc), u4value);

    //check DLINE at delay 0, if 0, incr FBK, else, incr REF
    mcSHOW_DBG_MSG2("delay tap= 0    ");
    dline_status1 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x0f4), 23);
    
    if (dline_status1 == DLINE_0)
    {
        mcSHOW_DBG_MSG2("A1 PLL need to scan FBK delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0cc));
            mcSET_FIELD(u4value, ii, 0x0000001f, 0);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0cc), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x0f4), 23);
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_1 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("A1 PLL calibration done! Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else if (dline_status1 == DLINE_1)
    {
        mcSHOW_DBG_MSG2("A1 PLL need to scan REF delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0cc));
            mcSET_FIELD(u4value, ii, 0x00001f00, 8);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0cc), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x0f4), 23);            
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_0 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("A1 PLL calibration done!Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else   // dline_status1 == DLINE_TOGGLE
    {
        mcSHOW_DBG_MSG2("A1 PLL no need to scan FBK or REF delay\n");
    }
    	
    //===============================================
    //AB
    //set REF and FBK delay to 0 at initial
    mcSHOW_DBG_MSG2("\n============== AB PLL ==============\n");
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
    u4value = u4value & 0xe0e0ffff;
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

    //check DLINE at delay 0, if 0, incr FBK, else, incr REF
    mcSHOW_DBG_MSG2("delay tap= 0    ");
    dline_status1 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 5);
    
    if (dline_status1 == DLINE_0)
    {
        mcSHOW_DBG_MSG2("AB PLL need to scan FBK delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
            mcSET_FIELD(u4value, ii, 0x001f0000, 16);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 5);
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_1 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("AB PLL calibration done!Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else if (dline_status1 == DLINE_1)
    {
        mcSHOW_DBG_MSG2("AB PLL need to scan REF delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
            mcSET_FIELD(u4value, ii, 0x1f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 5);
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_0 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("AB PLL calibration done!Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else   // dline_status1 == DLINE_TOGGLE
    {
        mcSHOW_DBG_MSG2("AB PLL no need to scan FBK or REG delay\n");
    }
    

    //===============================================
    //CTL
    //set REF and FBK delay to 0 at initial
    mcSHOW_DBG_MSG2("\n============== CTL PLL ==============\n");
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b8));
    u4value = u4value & 0xffffffe0;
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b8), u4value);

    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2bc));
    u4value = u4value & 0xe0ffffff;
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2bc), u4value);

    //check DLINE at delay 0, if 0, incr FBK, else, incr REF
    mcSHOW_DBG_MSG2("delay tap= 0    ");
    dline_status1 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 15);
    
    if (dline_status1 == DLINE_0)
    {
        mcSHOW_DBG_MSG2("CTL PLL need to scan FBK delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2bc));
            mcSET_FIELD(u4value, ii, 0x1f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2bc), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 15);
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_1 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("CTL PLL calibration done!Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else if (dline_status1 == DLINE_1)
    {
        mcSHOW_DBG_MSG2("CTL PLL need to scan REF delay\n");
        for (ii=1; ii<32; ii++)
        {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2b8));
            mcSET_FIELD(u4value, ii, 0x0000001f, 0);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2b8), u4value);

            mcSHOW_DBG_MSG2("delay tap=%2d    ", ii);
            dline_status2 = DramcCheckPllDline(mcSET_PHY_REG_ADDR(0x2e8), 15);
            if (dline_status2 != dline_status1)           	
            {
                // status change: DLINE_0 or DLINE_TOGGLE, break
                mcSHOW_DBG_MSG2("CTL PLL calibration done!Final delay is %2d\n", ii);
                break;
            }
        }
    }
    else   // dline_status1 == DLINE_TOGGLE
    {
        mcSHOW_DBG_MSG2("CTL PLL no need to scan FBK or REG delay\n");
    }
	
    //log example
    /*
============== A1 PLL ==============
delay tap= 0    DLINE=0 , count= 0 @ confidence=16
A1 PLL need to scan FBK delay
delay tap= 1    DLINE=0 , count= 0 @ confidence=16
delay tap= 2    DLINE=0 , count= 0 @ confidence=16
delay tap= 3    DLINE=1 , count=16 @ confidence=16
A1 PLL calibration done! Final delay is  3

============== AB PLL ==============
delay tap= 0    DLINE=0 , count= 0 @ confidence=16
AB PLL need to scan FBK delay
delay tap= 1    DLINE=0 , count= 0 @ confidence=16
delay tap= 2    DLINE=0 , count= 0 @ confidence=16
delay tap= 3    DLINE=1 , count=16 @ confidence=16
AB PLL calibration done!Final delay is  3

============== B2 PLL ==============
delay tap= 0    DLINE=0 , count= 0 @ confidence=16
B2 PLL need to scan FBK delay
delay tap= 1    DLINE=0 , count= 0 @ confidence=16
delay tap= 2    DLINE=0 , count= 0 @ confidence=16
delay tap= 3    DLINE=1 , count=16 @ confidence=16
B2 PLL calibration done!Final delay is  3

============== CTL PLL ==============
delay tap= 0    DLINE=0 , count= 0 @ confidence=16
CTL PLL need to scan FBK delay
delay tap= 1    DLINE=0 , count= 0 @ confidence=16
delay tap= 2    DLINE=0 , count= 0 @ confidence=16
delay tap= 3    DLINE=1 , count=16 @ confidence=16
CTL PLL calibration done!Final delay is  3
   */
}
#endif


#ifdef DRAM_IMPEDANCE_CAL
//-------------------------------------------------------------------------
/** DramcImpedanceCalTxOcd
 *  start TX OCD impedance calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcImpedanceCalTxOcd(void)
{
    U8 fgcal_out;
    S8 p_drv, n_drv;
    U8 uccode_map[32] = {0x00, 0x01, 0x08, 0x10, 0x20, 0x40, 0x80, 0x12, 0x24, 0x48, \
 	                                 0x13, 0x19, 0x46, 0x89, 0x17, 0x1e, 0x4d, 0x35, 0x93, 0x4f, \
 	                                 0x8f, 0x3d, 0xb8, 0x73, 0xb6, 0xe9, 0xb7, 0xeb, 0xf3, 0xef, \
 	                                 0xfe, 0xff};
    U32 u4PHY_BASE_ADDR, u4value; 
    U32 u4prv_phy_register_0e0, u4prv_phy_register_0e8;

    u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);
    //backup registers
    u4prv_phy_register_0e8 = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe8);
    u4prv_phy_register_0e0 = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);

    //start calibrate DRVP
    //enable calibration, RG_TX_IMPx_OCD_CAL_EN (0x1?E8[31])
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe8);
    mcSET_BIT(u4value, 31);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe8, u4value);

    //select DRVP, RG_TX_IMPA_OCD_PUCMP_EN (0x1?E8[30]=1)
    mcSET_BIT(u4value, 30);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe8, u4value);

    //0x10e0[31]=1
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
    mcSET_BIT(u4value, 31);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4value);

    for (p_drv=0; p_drv<32; p_drv++)
    {
        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
        mcSET_FIELD(u4value, uccode_map[(U8)p_drv], 0x000000ff, 0);
        ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4value);

        mcDELAY_us(1);

        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xf4);
        fgcal_out = (U8) mcGET_FIELD(u4value, 0x00100000, 20);
        mcSHOW_DBG_MSG("OCD DRVP=%2d | 8-bit code=%2x    CALOUT=%d\n", p_drv, uccode_map[p_drv], fgcal_out);

        if (fgcal_out == 0)
        {
            mcSHOW_DBG_MSG2("OCD DRVP calibration OK! DRVP=%2d\n", p_drv);
            break;
        }
    }
    if (p_drv==32)
    {
        mcSHOW_ERROR_CHIP_DisplayString("No valid OCD DRVP!!\n");
        mcSHOW_ERROR_CHIP_DisplayString("Skip OCD DRVN calibration!!\n");
        goto IMP_CAL_TX_OCD_END;
    }

    //start calibrate DRVN
    //select DRVN, RG_TX_IMPA_OCD_PUCMP_EN (0x1?E8[30]=0)
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe8);
    mcCLR_BIT(u4value, 30);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe8, u4value);
    
    for (n_drv=31;n_drv>=0; n_drv--)
    {
        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
        mcSET_FIELD(u4value, uccode_map[(U8)n_drv], 0x0000ff00, 8);
        ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4value);

        mcDELAY_us(1);

        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xf4);
        fgcal_out = (U8) mcGET_FIELD(u4value, 0x00100000, 20);
        mcSHOW_DBG_MSG("OCD DRVN=%2d | 8-bit code=%2x    CALOUT=%d\n", n_drv, uccode_map[n_drv], fgcal_out);

        if (fgcal_out == 0)
        {
            mcSHOW_DBG_MSG2("OCD DRVN calibration OK! DRVN=%2d\n", n_drv);
            break;
        }
    }
    if (n_drv < 0)
    {
        mcSHOW_ERROR_CHIP_DisplayString("No valid OCD DRVN!!\n");        
        goto IMP_CAL_TX_OCD_END;
    } 	
 	
IMP_CAL_TX_OCD_END:
    //recover registers
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe8, u4prv_phy_register_0e8);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4prv_phy_register_0e0);
   
    // log example
    /*
OCD DRVP= 0 | 8-bit code= 0    CALOUT=1
OCD DRVP= 1 | 8-bit code= 1    CALOUT=1
OCD DRVP= 2 | 8-bit code= 8    CALOUT=1
OCD DRVP= 3 | 8-bit code=10    CALOUT=1
OCD DRVP= 4 | 8-bit code=20    CALOUT=1
OCD DRVP= 5 | 8-bit code=40    CALOUT=1
OCD DRVP= 6 | 8-bit code=80    CALOUT=1
OCD DRVP= 7 | 8-bit code=12    CALOUT=1
OCD DRVP= 8 | 8-bit code=24    CALOUT=1
OCD DRVP= 9 | 8-bit code=48    CALOUT=1
OCD DRVP=10 | 8-bit code=13    CALOUT=1
OCD DRVP=11 | 8-bit code=19    CALOUT=1
OCD DRVP=12 | 8-bit code=46    CALOUT=1
OCD DRVP=13 | 8-bit code=89    CALOUT=1
OCD DRVP=14 | 8-bit code=17    CALOUT=1
OCD DRVP=15 | 8-bit code=1e    CALOUT=1
OCD DRVP=16 | 8-bit code=4d    CALOUT=1
OCD DRVP=17 | 8-bit code=35    CALOUT=0
OCD DRVP calibration OK! DRVP=17
OCD DRVN=31 | 8-bit code=ff    CALOUT=1
OCD DRVN=30 | 8-bit code=fe    CALOUT=1
OCD DRVN=29 | 8-bit code=ef    CALOUT=1
OCD DRVN=28 | 8-bit code=f3    CALOUT=1
OCD DRVN=27 | 8-bit code=eb    CALOUT=1
OCD DRVN=26 | 8-bit code=b7    CALOUT=1
OCD DRVN=25 | 8-bit code=e9    CALOUT=1
OCD DRVN=24 | 8-bit code=b6    CALOUT=1
OCD DRVN=23 | 8-bit code=73    CALOUT=1
OCD DRVN=22 | 8-bit code=b8    CALOUT=1
OCD DRVN=21 | 8-bit code=3d    CALOUT=1
OCD DRVN=20 | 8-bit code=8f    CALOUT=1
OCD DRVN=19 | 8-bit code=4f    CALOUT=1
OCD DRVN=18 | 8-bit code=93    CALOUT=1
OCD DRVN=17 | 8-bit code=35    CALOUT=1
OCD DRVN=16 | 8-bit code=4d    CALOUT=1
OCD DRVN=15 | 8-bit code=1e    CALOUT=1
OCD DRVN=14 | 8-bit code=17    CALOUT=0
OCD DRVN calibration OK! DRVN=14
   */
}

//-------------------------------------------------------------------------
/** DramcImpedanceCalRxOdt
 *  start RX DDT impedance calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcImpedanceCalRxOdt(void)
{
    U8 fgcal_out;
    S8 p_drv = 0;
	S8 n_drv = 0;
    U8 uccode_map[32] = {0x00, 0x01, 0x08, 0x10, 0x20, 0x40, 0x80, 0x12, 0x24, 0x48, \
 	                                 0x13, 0x19, 0x46, 0x89, 0x17, 0x1e, 0x4d, 0x35, 0x93, 0x4f, \
 	                                 0x8f, 0x3d, 0xb8, 0x73, 0xb6, 0xe9, 0xb7, 0xeb, 0xf3, 0xef, \
 	                                 0xfe, 0xff};
    U32 u4PHY_BASE_ADDR, u4value; 
    U32 u4prv_phy_register_0e0, u4prv_phy_register_0e4;

    u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);
    //backup registers
    u4prv_phy_register_0e4 = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
    u4prv_phy_register_0e0 = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);

    //start calibrate DRVP
    //enable calibration, RG_TX_IMPx_ODT_CAL_EN (0x1?E4[31])
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
    mcSET_BIT(u4value, 31);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4value);

    //select DRVP, RG_TX_IMPA_ODT_PUCMP_EN (0x1?E4[30]=1)
    mcSET_BIT(u4value, 30);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4value);

    //0x10e0[31]=1
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
    mcSET_BIT(u4value, 31);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4value);

    for (p_drv=0; p_drv<32; p_drv++)
    {
        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
        mcSET_FIELD(u4value, uccode_map[(U8)p_drv], 0x000000ff, 0);
        ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4value);

        mcDELAY_us(1);

        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xf4);
        fgcal_out = (U8) mcGET_FIELD(u4value, 0x00200000, 21);
        mcSHOW_DBG_MSG("ODT DRVP=%2d | 8-bit code=%2x    CALOUT=%d\n", p_drv, uccode_map[p_drv], fgcal_out);

        if (fgcal_out == 0)
        {
            mcSHOW_DBG_MSG2("ODT DRVP calibration OK! DRVP=%2d\n", p_drv);
            break;
        }
    }
    if (p_drv==32)
    {
        mcSHOW_ERROR_CHIP_DisplayString("No valid ODT DRVP!!\n");
        mcSHOW_ERROR_CHIP_DisplayString("Skip ODT DRVN calibration!!\n");
        goto IMP_CAL_RX_ODT_END;
    }

    //start calibrate DRVN
    //select DRVN, RG_TX_IMPA_ODT_PUCMP_EN (0x1?E4[30]=0)
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
    mcCLR_BIT(u4value, 30);
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4value);
    
    for (n_drv=31;n_drv>=0; n_drv--)
    {
        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
        mcSET_FIELD(u4value, uccode_map[(U8)n_drv], 0x0000ff00, 8);
        ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4value);

        mcDELAY_us(1);

        u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xf4);
        fgcal_out = (U8) mcGET_FIELD(u4value, 0x00200000, 21);
        mcSHOW_DBG_MSG("ODT DRVN=%2d | 8-bit code=%2x    CALOUT=%d\n", n_drv, uccode_map[n_drv], fgcal_out);

        if (fgcal_out == 0)
        {
            mcSHOW_DBG_MSG2("ODT DRVN calibration OK! DRVN=%2d\n", n_drv);
            break;
        }
    }
    if (n_drv < 0)
    {
        mcSHOW_ERROR_CHIP_DisplayString("No valid ODT DRVN!!\n");        
        goto IMP_CAL_RX_ODT_END;
    } 	
 	
IMP_CAL_RX_ODT_END:
    //recover registers
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe4, u4prv_phy_register_0e4);
    // 0xe0 duplicate, need to apply drv_p & drv_n below if apply
    ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4prv_phy_register_0e0);
    
	// check if p_drv and n_drv valid?!
	if (p_drv == 32)
	{
	    p_drv = 31;
	    n_drv = 31;
	}
	else if (n_drv < 0)
	{
	    n_drv = 0;
	}

	u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
	mcSET_FIELD(u4value, uccode_map[(U8)p_drv], 0x000000ff, 0);
	mcSET_FIELD(u4value, uccode_map[(U8)n_drv], 0x0000ff00, 8);
	ucDram_Register_Write(u4PHY_BASE_ADDR|0xe0, u4value);
 
    // log example
    /*
ODT DRVP= 0 | 8-bit code= 0    CALOUT=1
ODT DRVP= 1 | 8-bit code= 1    CALOUT=1
ODT DRVP= 2 | 8-bit code= 8    CALOUT=1
ODT DRVP= 3 | 8-bit code=10    CALOUT=1
ODT DRVP= 4 | 8-bit code=20    CALOUT=1
ODT DRVP= 5 | 8-bit code=40    CALOUT=1
ODT DRVP= 6 | 8-bit code=80    CALOUT=1
ODT DRVP= 7 | 8-bit code=12    CALOUT=1
ODT DRVP= 8 | 8-bit code=24    CALOUT=1
ODT DRVP= 9 | 8-bit code=48    CALOUT=1
ODT DRVP=10 | 8-bit code=13    CALOUT=1
ODT DRVP=11 | 8-bit code=19    CALOUT=1
ODT DRVP=12 | 8-bit code=46    CALOUT=1
ODT DRVP=13 | 8-bit code=89    CALOUT=1
ODT DRVP=14 | 8-bit code=17    CALOUT=0
ODT DRVP calibration OK! DRVP=14
ODT DRVN=31 | 8-bit code=ff    CALOUT=1
ODT DRVN=30 | 8-bit code=fe    CALOUT=1
ODT DRVN=29 | 8-bit code=ef    CALOUT=1
ODT DRVN=28 | 8-bit code=f3    CALOUT=1
ODT DRVN=27 | 8-bit code=eb    CALOUT=1
ODT DRVN=26 | 8-bit code=b7    CALOUT=1
ODT DRVN=25 | 8-bit code=e9    CALOUT=1
ODT DRVN=24 | 8-bit code=b6    CALOUT=1
ODT DRVN=23 | 8-bit code=73    CALOUT=1
ODT DRVN=22 | 8-bit code=b8    CALOUT=1
ODT DRVN=21 | 8-bit code=3d    CALOUT=1
ODT DRVN=20 | 8-bit code=8f    CALOUT=1
ODT DRVN=19 | 8-bit code=4f    CALOUT=1
ODT DRVN=18 | 8-bit code=93    CALOUT=1
ODT DRVN=17 | 8-bit code=35    CALOUT=1
ODT DRVN=16 | 8-bit code=4d    CALOUT=1
ODT DRVN=15 | 8-bit code=1e    CALOUT=1
ODT DRVN=14 | 8-bit code=17    CALOUT=1
ODT DRVN=13 | 8-bit code=89    CALOUT=1
ODT DRVN=12 | 8-bit code=46    CALOUT=0
ODT DRVN calibration OK! DRVN=12
   */
}

//-------------------------------------------------------------------------
/** DramcImpedanceCalApply
 *  TX/RX OCD/ODT impedance calibration and apply to driving registers.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcImpedanceCalApply(void)
{
    U32 u4PHY_BASE_ADDR, u4value; 
    U8 uctx_ocd_drvp, uctx_ocd_drvn, ucrx_odt_drvp, ucrx_odt_drvn;

    u4PHY_BASE_ADDR = mcSET_PHY_REG_ADDR(0x000);

    DramcImpedanceCalTxOcd();
    DramcImpedanceCalRxOdt();

    //read TX OCD P/N DRV
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe4);
    uctx_ocd_drvp = (U8) mcGET_FIELD(u4value, 0x000000ff, 0);
    uctx_ocd_drvn = (U8) mcGET_FIELD(u4value, 0x0000ff00, 8);

    //read RX ODT P/N DRV
    u4value = ucDram_Register_Read(u4PHY_BASE_ADDR|0xe0);
    ucrx_odt_drvp = (U8) mcGET_FIELD(u4value, 0x000000ff, 0);
    ucrx_odt_drvn = (U8) mcGET_FIELD(u4value, 0x0000ff00, 8);

    DramcDqDriving(uctx_ocd_drvp, uctx_ocd_drvn, ucrx_odt_drvp, ucrx_odt_drvn);
    DramcDqsDriving(uctx_ocd_drvp, uctx_ocd_drvn, ucrx_odt_drvp, ucrx_odt_drvn);
    DramcDqmDriving(uctx_ocd_drvp, uctx_ocd_drvn, ucrx_odt_drvp, ucrx_odt_drvn);
    // CLK driving is different from others (based on waveform result), may need offset?!
    DramcClkDriving(uctx_ocd_drvp, uctx_ocd_drvn);
    DramcCaDriving(uctx_ocd_drvp, uctx_ocd_drvn);
    /*
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x08c));
    mcSET_FIELD(u4value, 0xff, 0x000000ff, 0);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x08c), u4value);

    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x090));
    mcSET_FIELD(u4value, 0xff, 0xff000000, 24);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x090), u4value);
    
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0a8));
    mcSET_FIELD(u4value, 0xff, 0x000000ff, 0);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0a8), u4value);

    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0ac));
    mcSET_FIELD(u4value, 0xff, 0xff000000, 24);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0ac), u4value);
*/
}
#endif

#ifdef DRAM_WRITE_LEVELING_CAL
//-------------------------------------------------------------------------
/** DramcWriteLeveling
 *  start Write Leveling Calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcWriteLeveling(void)
{
    U32 u4value, u4temp, u4dq_o1,u4stay[DQS_NUMBER];
    U32 u4prv_register_1dc, u4prv_register_044, u4prv_register_0e4, u4prv_register_07c, u4prv_register_0d8, u4prv_register_340;
    U8 ucdqs_final_delay[DQS_NUMBER], ucsample_status[DQS_NUMBER], ucdq_o1_perbyte[DQS_NUMBER];
    U8 ii, byte_i;
	//U8 ucsample_count;

   
    // backup mode settings
    u4prv_register_1dc = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    u4prv_register_044 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
    u4prv_register_0e4 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4));
    u4prv_register_07c = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL));
    u4prv_register_0d8 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY));
    u4prv_register_340 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV));

	//write leveling mode initialization
    //disable auto refresh: REFCNT_FR_CLK = 0 (0x1dc[23:16]), ADVREFEN = 0 (0x44[30])
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL));
    mcCLR_MASK(u4value, MASK_DRAMC_PD_CTRL_REFCNT_FR_CLK);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4value);

    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
    mcCLR_BIT(u4value, POS_TEST2_3_ADVREFEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4value);

    //Make CKE fixed at 1 (Put this before issuing MRS): CKEFIXON = 1 (0xe4[2])
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4));
    mcSET_BIT(u4value, POS_PADCTL4_CKEFIXON);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);

    //Enable Write ODT: WOEN = 1 (0x7c[3])
    //may no need to set here, initial value
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL));
    mcSET_BIT(u4value, POS_DDR2CTL_WOEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);

    //ODT, DQIEN fixed at 1; FIXODT = 1 (0xd8[23]), FIXDQIEN = 1111 (0xd8[15:12])
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY));
    mcSET_BIT(u4value, POS_MCKDLY_FIXODT);
    mcSET_MASK(u4value, MASK_MCKDLY_FIXDQIEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4value);

    //Enable SMT_EN: chA-> 0x104c[31:16] / 0x124c[31:16]; chB-> 0x129c[31:16] / 0x144c[31:16]
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x04c));
    mcSET_MASK(u4value, 0xffff0000);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x04c), u4value);

    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x24c));
    mcSET_MASK(u4value, 0xffff0000);
    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x24c), u4value);

    //Set {R_DQS_B3_G R_DQS_B2_G R_DQS_B1_G R_DQS_B0_G}=~DA_TX_CMDACLK_D*X : 0x340[4:1]
    //Enable Write leveling: 0x340[0]
    u4temp = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xa4c));
    u4temp = mcGET_FIELD(u4temp, 0x0000000f, 0);
    u4temp = (~u4temp)&0xf;

    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV));
    mcSET_FIELD(u4value, u4temp, MASK_WRLEV_DQS_Bx_G, POS_WRLEV_DQS_Bx_G);
    mcSET_BIT(u4value, POS_WRLEV_WRITE_LEVEL_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);

	mcDELAY_us(1);

    //issue MR1 to enable write leveling
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), 0x00002084);
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD));
    mcSET_BIT(u4value, POS_SPCMD_MRWEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);

    //wait 10 XTAL cycles after issuing MR1, 1ms here
    mcDELAY_us(1);

    //Proceed write leveling...
    //Initilize sw parameters
    for (ii=0; ii < DQS_NUMBER; ii++)
    {
        ucsample_status[ii] = 0;
        ucdqs_final_delay[ii] = 0;
    }
    //used for early break
    //ucsample_count = 0;
	for (ii=0; ii < DQS_NUMBER; ii++)
    {
        u4stay[ii] = 0;
    }

    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG2("\n        dramc_write_leveling_swcal\n");
    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG2("delay  byte0  byte1  byte2  byte3\n");
    mcSHOW_DBG_MSG2("-----------------------------\n");

    for (ii=0; ii<128; ii++)
    {
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
        mcSET_FIELD(u4value, ii, 0x0000007f, 0);
        mcSET_FIELD(u4value, ii, 0x007f0000, 16);        
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
        mcSET_FIELD(u4value, ii, 0x0000007f, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
        mcSET_FIELD(u4value, ii, 0x007f0000, 16);        
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);


        //wait for PI settling (100 XTAL cycles), 1ms here
        //Can be ignored by only 1-step tuning (need to delay if large step) by SP
        //#after 1
        mcDELAY_us(10);
		
        //Trigger DQS pulse, R_DQS_WLEV: 0x340[8] from 1 to 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV));
        mcSET_BIT(u4value, POS_WRLEV_DQS_WLEV);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);
        mcCLR_BIT(u4value, POS_WRLEV_DQS_WLEV);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4value);

        //wait 10 XTAL cycles, 1ms here
        mcDELAY_us(1);

        //Read DQ_O1 from register, 0x380 for chA, 0x384 for chB

            u4dq_o1 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(0x380));
		
#ifdef DEFAULT_DDR_CHA_BUS_X8
		u4dq_o1 = DramcDQ4bitSwap(u4dq_o1);
#endif

#ifdef DEFAULT_DDR_CHA_BUS_X8
			//DQS0: DQ3, DQS1: DQ13, DQS2: DQ18. DQS3: DQ28
            ucdq_o1_perbyte[0] = (U8)((u4dq_o1>>3)&0x00000001);
            ucdq_o1_perbyte[1] = (U8)((u4dq_o1>>14)&0x00000001);
            ucdq_o1_perbyte[2] = (U8)((u4dq_o1>>18)&0x00000001);
            ucdq_o1_perbyte[3] = (U8)((u4dq_o1>>29)&0x00000001);
#else
			//DQS0: DQ3, DQS1: DQ13, DQS2: DQ18. DQS3: DQ28
            // JC: check DQ mapping? OK
			/*
            ucdq_o1_perbyte[0] = (U8)((u4dq_o1>>3)&0x00000001);
            ucdq_o1_perbyte[1] = (U8)((u4dq_o1>>13)&0x00000001);
            ucdq_o1_perbyte[2] = (U8)((u4dq_o1>>18)&0x00000001);
            ucdq_o1_perbyte[3] = (U8)((u4dq_o1>>28)&0x00000001);*/
            
			ucdq_o1_perbyte[0] = (U8)(((u4dq_o1>>0)&0x1)|((u4dq_o1>>1)&0x1)|((u4dq_o1>>2)&0x1)|((u4dq_o1>>3)&0x1)|
													((u4dq_o1>>4)&0x1)|((u4dq_o1>>5)&0x1)|((u4dq_o1>>6)&0x1)|((u4dq_o1>>7)&0x1));

			ucdq_o1_perbyte[1] = (U8)(((u4dq_o1>>8)&0x1)|((u4dq_o1>>9)&0x1)|((u4dq_o1>>10)&0x1)|((u4dq_o1>>11)&0x1)|
													((u4dq_o1>>12)&0x1)|((u4dq_o1>>13)&0x1)|((u4dq_o1>>14)&0x1)|((u4dq_o1>>15)&0x1));
			ucdq_o1_perbyte[2] = (U8)(((u4dq_o1>>16)&0x1)|((u4dq_o1>>17)&0x1)|((u4dq_o1>>18)&0x1)|((u4dq_o1>>19)&0x1)|
													((u4dq_o1>>20)&0x1)|((u4dq_o1>>21)&0x1)|((u4dq_o1>>22)&0x1)|((u4dq_o1>>23)&0x1));

			ucdq_o1_perbyte[3] = (U8)(((u4dq_o1>>24)&0x1)|((u4dq_o1>>25)&0x1)|((u4dq_o1>>26)&0x1)|((u4dq_o1>>27)&0x1)|
													((u4dq_o1>>28)&0x1)|((u4dq_o1>>29)&0x1)|((u4dq_o1>>30)&0x1)|((u4dq_o1>>31)&0x1));
#endif

        mcSHOW_DBG_MSG("%3d %02x %02x %02x %02x\n", ii, u4dq_o1&0x000000ff, (u4dq_o1&0x0000ff00)>>8, (u4dq_o1&0x00ff0000)>>16, (u4dq_o1&0xff000000)>>24);

        //sample from 0 to 1
        for (byte_i = 0; byte_i < DQS_NUMBER;  byte_i++)
        {
			#if 1
			if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==1))
            {
                ucsample_status[byte_i] = 0;//1-->1
                //used for early break
                //ucsample_count++;
                //record delay value
                //ucdqs_final_delay[byte_i] = 0;
            }
            else if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==0))
            {
            	if(u4stay[byte_i]<3)
					{
						u4stay[byte_i]++;
						continue;
					}
				u4stay[byte_i]=0;
                ucsample_status[byte_i] = 1;//1-->0
            }
            else if ((ucsample_status[byte_i]==1) && (ucdq_o1_perbyte[byte_i]==1))
            {
            	if(u4stay[byte_i]<3)
					{
						u4stay[byte_i]++;
						continue;
					}
				u4stay[byte_i]=0;
                ucsample_status[byte_i] = 3;//0-->1
                //used for early break
                //ucsample_count++;
                //record delay value
                ucdqs_final_delay[byte_i] = ii-3;
            }
			else if((ucsample_status[byte_i]==1) && (ucdq_o1_perbyte[byte_i]==0))
			{
            	if(u4stay[byte_i]<3)
					{
						u4stay[byte_i]++;
						continue;
					}
				u4stay[byte_i]=0;
				ucsample_status[byte_i] = 2;//0-->0
			}
			else if((ucsample_status[byte_i]==2) && (ucdq_o1_perbyte[byte_i]==1))
			{
            	if(u4stay[byte_i]<3)
					{
						u4stay[byte_i]++;
						continue;
					}
				u4stay[byte_i]=0;
				ucsample_status[byte_i] = 3;//0-->1
                //used for early break
                //ucsample_count++;
                //record delay value
                ucdqs_final_delay[byte_i] = ii-3;
			}
			#else
			if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==1))
            {
                ucsample_status[byte_i] = 2;
                //used for early break
                //ucsample_count++;
                //record delay value
                ucdqs_final_delay[byte_i] = 0;
            }
            else if ((ucsample_status[byte_i]==0) && (ucdq_o1_perbyte[byte_i]==0))
            {
                ucsample_status[byte_i] = 1;
            }
            else if ((ucsample_status[byte_i]==1) && (ucdq_o1_perbyte[byte_i]==1))
            {
                ucsample_status[byte_i] = 2;
                //used for early break
                //ucsample_count++;
                //record delay value
                ucdqs_final_delay[byte_i] = ii;
            }
			#endif
        }
        //early break, may be marked for debug use
        //if (ucsample_count==DQS_NUMBER)
        //{
        //    break;
        //}        
    }

	mcDELAY_us(1);	

    //mcSHOW_DBG_MSG("pass bytecount = %d\n", ucsample_count);
    mcSHOW_DBG_MSG2("byte_i    status    best delay\n");
    for (byte_i = 0; byte_i < DQS_NUMBER;  byte_i++)
    {
        mcSHOW_DBG_MSG2("%d    %d    %d\n", byte_i, ucsample_status[byte_i], ucdqs_final_delay[byte_i]);
#ifdef DRAM_LOAD_BOARD
		if(ucsample_status[byte_i]!=3)
		{
			LoadBoardShowResult(FLAG_WRITE_LEVELING,\
									FLAG_CALIBRATION_FAIL,\
									FLAG_NOT_COMPLETE_OR_FAIL);
			while(1);
		}
#endif
    }    
    mcSHOW_DBG_MSG("========================================\n");

    // write leveling done, mode settings recovery if necessary
    // recover MR1, refer to initial value (with dynamic ODT: disable RTT_Nom off, 2013/1/3)

    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MRS), DEFAULT_MR1_VALUE);

    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD));
    mcSET_BIT(u4value, POS_SPCMD_MRWEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_SPCMD_MRWEN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_SPCMD), u4value);

    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_WRLEV), u4prv_register_340);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DRAMC_PD_CTRL), u4prv_register_1dc);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4prv_register_044);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4prv_register_0e4);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4prv_register_07c);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_MCKDLY), u4prv_register_0d8);

    
    //Disable SMT_EN: chA-> 0x104c[31:16] / 0x124c[31:16]; chB-> 0x129c[31:16] / 0x144c[31:16]
    //for power saving

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x04c));
        mcCLR_MASK(u4value, 0xffff0000);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x04c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x24c));
        mcCLR_MASK(u4value, 0xffff0000);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x24c), u4value);


	// set best delay value

	    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
	    mcSET_FIELD(u4value, ucdqs_final_delay[1], 0x0000007f, 0);
	    mcSET_FIELD(u4value, ucdqs_final_delay[0], 0x007f0000, 16);        
	    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);

	    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
	    mcSET_FIELD(u4value, ucdqs_final_delay[2], 0x0000007f, 0);
	    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

	    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
	    mcSET_FIELD(u4value, ucdqs_final_delay[3], 0x007f0000, 16);        
	    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);


    // log example
/*
===================================================================

                dramc_write_leveling_swcal
                apply=1 channel=2(2:cha, 3:chb)
===================================================================
delay  byte0  byte1  byte2  byte3
-----------------------------
  0    0    0    0    1
  1    0    0    0    1
  2    0    0    1    1
  3    0    0    1    1
  4    0    0    1    1
  5    0    0    1    1
  6    0    0    1    1
  7    0    0    1    1
  8    0    0    1    1
  9    0    0    1    1
 10    0    0    1    1
 11    1    1    1    1
pass bytecount = 4
byte_i    status    best delay
0         2         11
1         2         11
2         2         2
3         2         0
*/
}
#endif

#ifdef DRAM_MiockJmeter
//-------------------------------------------------------------------------
/** DramcMiockJmeter
 *  start MIOCK jitter meter.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param block_no         (U8): block 0 or 1.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
void DramcMiockJmeter(void)
{
    U8 ucsearch_state, ucdqs_dly, fgcurrent_value, fginitial_value, ucstart_period, ucend_period;
    U32 u4value, u4addr_array[4], u4sample_cnt, u4ones_cnt, u4frequency_hex;
    U16 u2real_freq, u2real_period, u2delay_cell_ps;
	U32 u4mempll_prediv_hex; 
	U32 u4mempll_predivider[3] = {1, 2, 4};
	U32 u4clock,u4frequency1;
	U32 u4counter,half_T_count;

	fginitial_value = 0;
	ucstart_period = 0;
	ucend_period = 0;
	half_T_count=0;

	u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
	u4addr_array[1] = mcSET_PHY_REG_ADDR(0x04c);


	u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
	u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQS_ERR_CNT);

    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN 
    //RG_??_RX_VREF_OP_EN 
    //RG_??_RX_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x0000e000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value | 0xffff0000;
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Enable MIOCK jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=1, RG_RX_MIOCK_JIT_EN=1)
    //RG_??_RX_DQS_MIOCK_SEL=1
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x00001000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=1
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    ucsearch_state = 0;

    //Disable DQ eye scan (b'1), for counter clear
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {
        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, ucdqs_dly, 0x007f0000, 16);
        ucDram_Register_Write(u4addr_array[0], u4value);

		u4counter=0;
		do
		{
			//Reset eye scan counters (reg_sw_rst): 1 to 0
	        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
	        mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
	        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
	        mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
	        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	        //Enable DQ eye scan (b'1)
	        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
	        mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
	        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	        // 2ns/sample, here we delay 1ms about 500 samples
	        mcDELAY_us(1000);

	        //Disable DQ eye scan (b'1), for counter latch
	        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
	        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
	        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

			//mcDELAY_us(1000);

	        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*)
	        u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
	        u4ones_cnt = ucDram_Register_Read(u4addr_array[3]);
			
	        mcSHOW_DBG_MSG("%3d | %8x --%8x \n", ucdqs_dly, u4sample_cnt, u4ones_cnt);

			u4counter++;
			if(u4counter>5)
			{
				mcSHOW_ERROR_CHIP_DisplayString("Toggle_cnt has mistake in function DramcMiockJmeter()!\n");
				while(1);
			}
		}while(((u4sample_cnt+10)<u4ones_cnt) || (u4sample_cnt==0));

		//change to boolean value
        if (u4ones_cnt < (u4sample_cnt/2))
        {
            fgcurrent_value = 0;
        }
        else
        {
            fgcurrent_value = 1;
        }
		
        
		
        if (ucsearch_state==0)
        {
            //record initial value at the beginning
            fginitial_value = fgcurrent_value;            
            ucsearch_state = 1;
        }
        else if (ucsearch_state==1)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
				if(half_T_count<3)
					{
						half_T_count++;
						continue;
					}
                // start of the period
                fginitial_value = fgcurrent_value;
                ucstart_period = ucdqs_dly-3;
                ucsearch_state = 2;
				half_T_count=0;
            }
        }
        else if (ucsearch_state==2)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
            	if(half_T_count<3)
					{
						half_T_count++;
						continue;
					}
                fginitial_value = fgcurrent_value;
                ucsearch_state = 3;
				half_T_count=0;
            }
        }
        else if (ucsearch_state==3)
        {
            // check if change value
            if (fgcurrent_value != fginitial_value)
            {
                if(half_T_count<3)
					{
						half_T_count++;
						continue;
					}
                // end of the period, break the loop
                ucend_period = ucdqs_dly-3;
                ucsearch_state = 4;
				half_T_count=0;
                break;
            }
        }
        else
        {
            //nothing
        }
    }

    //Calculate 1 delay cell = ? ps
    // 1T = ? delay cell
	TCMSET_DLYCELL_PERT(ucend_period-ucstart_period);
    // 1T = ? ps
    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2a0));
    u4frequency_hex = mcGET_FIELD(u4value, 0x00007f00, 8);
	u4mempll_prediv_hex = mcGET_FIELD(u4value, 0x00000003, 0);

	u2real_freq = (U16) ((u4frequency_hex+1)*(XTAL_MHZ/u4mempll_predivider[u4mempll_prediv_hex])*2);

	u4clock = (TCMGET_DDR_CLK()/BASE_DDR_CLK)/2;
	u4frequency1 = u4clock / 2;
	u2real_freq = u2real_freq + (U16)((u4frequency1-(u4frequency_hex+1)*XTAL_MHZ)/u4mempll_predivider[u4mempll_prediv_hex]*2);

	u2real_period = (U16) ((U32)1000000/(U32)u2real_freq);

	//calculate delay cell time
    u2delay_cell_ps = u2real_period / TCMGET_DLYCELL_PERT;

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG2("    MIOCK jitter meter \n");
    mcSHOW_DBG_MSG2("===============================================================================\n");
    CHIP_DisplayString("1T = (");
	CHIP_DisplayInteger(ucend_period);
	CHIP_DisplayString(" - ");
	CHIP_DisplayInteger(ucstart_period);
	CHIP_DisplayString(") = ");
	CHIP_DisplayInteger(TCMGET_DLYCELL_PERT);
	CHIP_DisplayString(" delay cells\n");

	CHIP_DisplayString("Clock frequency = ");
	CHIP_DisplayInteger(u2real_freq);
	CHIP_DisplayString(" MHz, Clock period = ");
	CHIP_DisplayInteger(u2real_period);
	CHIP_DisplayString(" ps, 1 delay cell =");
	CHIP_DisplayInteger(u2delay_cell_ps);
	CHIP_DisplayString(" ps \n");


	UNUSED(u2delay_cell_ps);
 	
    // log example
    /*
  0 |   d91b60 --       0
  1 |   d91b71 --       0
  2 |   e018e4 --       0
  3 |   d9228c --       0
  4 |   d90d08 --       0
  5 |   d91434 --       0
  6 |   d9568b --       0
  7 |   e030cc --       0
  8 |   d91bf9 --       0
  9 |   d8dfad --       0
 10 |   e0197d --       0
 11 |   d92f2a --       0
 12 |   d91bf9 --  cc0f01
 13 |   d902df --  d902df
 14 |   d91a2e --  d91a2e
 15 |   e01ac0 --  e01ac0
 16 |   d92e80 --  d92e80
 17 |   d908d9 --  d908d9
 18 |   e01aaf --  e01aaf
 19 |   d90a0b --  d90a0b
 20 |   d91f07 --  d91f07
 21 |   d91302 --  d91302
 22 |   d92d4e --  d92d4e
 23 |   e01be1 --  e01be1
 24 |   d8d7e8 --  d8d7e8
 25 |   d92468 --  d92468
 26 |   e021db --  e021db
 27 |   d94cfb --  d94cfb
 28 |   d9291f --  d9291f
 29 |   d94701 --  d94701
 30 |   d913ac --  d913ac
 31 |   e01be1 --  e01be1
 32 |   d91d2b --  d91d2b
 33 |   d90c6f --  d90c6f
 34 |   e01262 --  e01262
 35 |   d923be --    6bdc
 36 |   d91b60 --       0
 37 |   d92fb2 --       0
 38 |   d92c1c --       0
 39 |   dff34e --       0
 40 |   d90c6f --       0
 41 |   d91269 --       0
 42 |   e03891 --       0
 43 |   d92e80 --       0
 44 |   d92de7 --       0
 45 |   d8ca18 --       0
 46 |   d923be --       0
 47 |   dff937 --       0
 48 |   d90d19 --       0
 49 |   d92457 --       0
 50 |   e014b5 --       0
 51 |   d92cb5 --       0
 52 |   d92e80 --       0
 53 |   e015f8 --       0
 54 |   d927ed --       0
 55 |   e012ea --       0
 56 |   d91bf9 --       0
 57 |   d92e80 --       0
 58 |   e01e56 --       0
 59 |   d9291f --       0
 60 |   d91c92 --    8fc5
 61 |   e00a8c --  e00a8c
========================================================================
    MIOCK jitter meter - channel=2(2:cha, 3:chb), block_0
========================================================================
1T = (61-12) = 49 delay cells
Clock frequency = 918 MHz, Clock period = 1089 ps, 1 delay cell = 22 ps
   */
}
#endif

#ifdef DRAM_GATING_SCAN
//-------------------------------------------------------------------------
/** DramcRxdqsGatingCal (v2 version)
 *  start the dqsien software calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  bl_type         (U8): Burst length type. 4 or 8 (4 is just used for lpddr2 ,if bl_type is 4 ,we will change BL to 4 to do this calibraion ,write back to 8 when exit).
 *  @param  test2_0         (U32): 16bits,set pattern1 [31:24] and set pattern0 [23:16].
 *  @param  test2_1         (U32): 28bits,base address[27:0].
 *  @param  test2_2         (U32): 28bits,offset address[27:0].
 *  @param  gold_counter    (U32): the dqs counter value base on test2_1 and test2_2 you give.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @param  test_pattern    (DRAM_TEST_PATTERN_T): used test pattern. 0: ISI, 1: AUDIO, 2: TESTPAT4, 3: TESTPAT4_3
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxdqsGatingCal(void)
{
    U8 ucmin, ucmax;
    U32 u4value, u4all_result_R, u4all_result_F;
    U8 ucpass_begin[DQS_NUMBER], ucpass_count[DQS_NUMBER];
    U8 ucmin_coarse_tune2T[DQS_NUMBER]={0}, ucmin_coarse_tune0p5T[DQS_NUMBER]={0}, ucmin_fine_tune[DQS_NUMBER]={0};
    U8 ucpass_count_1[DQS_NUMBER]={0}, ucmin_coarse_tune2T_1[DQS_NUMBER]={0}, ucmin_coarse_tune0p5T_1[DQS_NUMBER]={0}, ucmin_fine_tune_1[DQS_NUMBER]={0};
    U8 dqs_i, ucRX_DLY_DQSIENSTB_LOOP, ucdly_coarse_2T, ucdly_coarse_0p5T, ucdly_fine_xT;
    U8 ucdqs_result_R, ucdqs_result_F, ucerr_cnt, uctmp_offset, uctmp_value;
    U8 ucbest_fine_tune[DQS_NUMBER], ucbest_coarse_tune0p5T[DQS_NUMBER], ucbest_coarse_tune2T[DQS_NUMBER];
    U8 ucfinal_fine_tune[DQS_NUMBER], ucfinal_coarse_tune0p5T[DQS_NUMBER], ucfinal_coarse_tune2T;
    U16 u2one_hot_dly;
    int u4Avg[DQS_NUMBER],u4Min[DQS_NUMBER],u4Max[DQS_NUMBER];
    U32 test2_0 = 0xaa550000, test2_1 = 0x30000000, test2_2 = 0x00000406;
	U8 fgfail = DRAM_CALIBRATION_PASS;
	
	//power up dqs when in idle state when gating calibration

		//RG_A1_TX_ARDQS0_R75KP=0; 0x5801c[21]=0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x01c));
        mcCLR_BIT(u4value, 21);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), u4value);
		//RG_A1_TX_ARDQS1_R75KP=0; 0x58020[5]=0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x020));
        mcCLR_BIT(u4value, 5);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), u4value);
		//RG_A2_TX_ARDQS2_R75KP=0; 0x5821c[21]=0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x21c));
        mcCLR_BIT(u4value, 21);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), u4value);
		//RG_A2_TX_ARDQS3_R75KP=0; 0x58220[5]=0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x220));
        mcCLR_BIT(u4value, 5);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), u4value);		


	// begin 
    // enable RG_RX_*_DLY_DQSIENSTB_*_SWEN
    // Use JMeter to find delay cell measurement, ?T
    // transfer all coarse/fine tune steps into the same unit (say, ?T)
    // scan all possible range and find pass window and middle value per byte
    // since coarse_2T is per channel, select the min value between 4 bytes, use coarse_0.5T and fine_xT to get the best value
    // disable RG_RX_*_DLY_DQSIENSTB_*_SWEN (no need for A60807 since there is no HW gating)

    //enable RG_RX_*_DLY_DQSIENSTB_*_SWEN (switch to SW fine-tuning)

        //RG_A1_RX_DLY_DQSIENSTB_0_SWEN: 0x103c[23] = 1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x03c));
        mcSET_BIT(u4value, 23);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), u4value);
        //RG_A1_RX_DLY_DQSIENSTB_1_SWEN: 0x1040[23] = 1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x040));
        mcSET_BIT(u4value, 23);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x040), u4value);
        //RG_A2_RX_DLY_DQSIENSTB_2_SWEN: 0x123c[23] = 1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x23c));
        mcSET_BIT(u4value, 23);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), u4value);
        //RG_A2_RX_DLY_DQSIENSTB_3_SWEN: 0x1240[23] = 1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x240));
        mcSET_BIT(u4value, 23);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x240), u4value);		


    //Initialize variables
    for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
    {
        ucpass_begin[dqs_i] = 0;
        ucpass_count[dqs_i] = 0;
    }

    //coarse tune delay is (reg_DLY_DQSIENSTB_*)x2 + (RG_**_RX_DLY_DQSIENSTB_*)x0.5  DRAM clock cycle.
    // loop number of 2T and 0.5T coarse tune are defined @ pi_api.h
    //fine tune delay, depends on JMeter result
    ucRX_DLY_DQSIENSTB_LOOP = TCMGET_DLYCELL_PERT / 2;

    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG2("x = dqs result \ny = coarse_2T  coarse_0.5T  finetune\n");
    mcSHOW_DBG_MSG2("-------------------------------------------------------------------------------\n"); 
    mcSHOW_DBG_MSG2("y  |  dqs0f   dqs0r   dqs1f   dqs1r   dqs2f   dqs2r   dqs3f   dqs3r\n");
    mcSHOW_DBG_MSG2("-------------------------------------------------------------------------------\n");

    for (ucdly_coarse_2T=4; ucdly_coarse_2T<=DLY_DQSIENSTB_LOOP; ucdly_coarse_2T++)
    {
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xa4c));
            mcSET_FIELD(u4value, ucdly_coarse_2T, 0x00000700, 8);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa4c), u4value);	

        for (ucdly_coarse_0p5T=1; ucdly_coarse_0p5T<=RX_DQS_CTL_LOOP; ucdly_coarse_0p5T++)
        {
            u2one_hot_dly = (U16) 1<<(ucdly_coarse_0p5T-1);


                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x038));
                mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x038), u4value);

                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x03c));
                mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), u4value);

                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x238));
                mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x238), u4value);

                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x23c));
                mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), u4value);

            for (ucdly_fine_xT=0; ucdly_fine_xT<=ucRX_DLY_DQSIENSTB_LOOP; ucdly_fine_xT++)
            {

                    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x03c));
                    mcSET_FIELD(u4value, ucdly_fine_xT, 0x003f0000, 16);
                    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), u4value);

                    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x040));
                    mcSET_FIELD(u4value, ucdly_fine_xT, 0x003f0000, 16);
                    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x040), u4value);

                    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x23c));
                    mcSET_FIELD(u4value, ucdly_fine_xT, 0x003f0000, 16);
                    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), u4value);

                    u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x240));
                    mcSET_FIELD(u4value, ucdly_fine_xT, 0x003f0000, 16);
                    ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x240), u4value);


                //ok we set a coarse/fine tune value already
                //reset the read counters in both DRAMC and DDRPHY (R_DMPHYRST: 0x0f0[28])
                //enable test engine
                //record the counter value

                //reset phy R_DMPHYRST: 0xf0[28] 
                // 0x0f0[28] = 1 -> 0
                u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
                mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
                //delay 10ns, 1ms here
                mcDELAY_us(1);
                mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

                // read data counter reset
                // 0x0f4[25] = 1 -> 0
                u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
                mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
                //delay 10ns, 1ms here
                mcDELAY_us(1);                
                mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
                ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);


					u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
					mcCLR_BIT(u4value, 0);
					ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
					mcDELAY_us(1);
					mcSET_BIT(u4value, 0);
					ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);

					u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
					mcCLR_BIT(u4value, 0);
					ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);
					mcDELAY_us(1);
					mcSET_BIT(u4value, 0);
					ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);

                // enable TE2, audio pattern
                DramcEngine2(TE_OP_READ_CHECK, test2_0, test2_1, test2_2);

                u4all_result_R = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_STBENERR_R));
                u4all_result_F = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_STBENERR_F));

#ifdef DEFAULT_DDR_CHA_BUS_X8
				u4all_result_R = DramcDQ4bitSwap(u4all_result_R);
				u4all_result_F = DramcDQ4bitSwap(u4all_result_F);
#endif				
                //mcSHOW_DBG_MSG(("--------------------------------\n"));
                //mcSHOW_DBG_MSG(("%2d  %2d  %2d | %8x  %8x\n", ucdly_coarse_2T, ucdly_coarse_0p5T, ucdly_fine_xT, u4all_result_R, u4all_result_F));

                mcSHOW_DBG_MSG("%2d  %2d  %2d  |  %2x  %2x  %2x  %2x  %2x  %2x  %2x  %2x  \n", ucdly_coarse_2T, ucdly_coarse_0p5T, ucdly_fine_xT, \
                                                     (u4all_result_F)&0xff,         (u4all_result_R)&0xff,         (u4all_result_F>>8)&0xff,   (u4all_result_R>>8)&0xff, \
                                                     (u4all_result_F>>16)&0xff, (u4all_result_R>>16)&0xff, (u4all_result_F>>24)&0xff, (u4all_result_R>>24)&0xff);                	

                //find gating window pass range per DQS separately
                for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
                {
                    //get dqs error result
                    ucdqs_result_R = (U8) ((u4all_result_R>>(8*dqs_i))&0x000000ff);
                    ucdqs_result_F = (U8) ((u4all_result_F>>(8*dqs_i))&0x000000ff);

                    //if current tap is pass 
                    if ((ucdqs_result_R==0) && (ucdqs_result_F==0))
                    {
                        if (ucpass_begin[dqs_i]==0)
                        {
                            //no pass tap before , so it is the begining of pass range
                            ucpass_begin[dqs_i] = 1;
                            ucpass_count_1[dqs_i] = 0;
                            ucmin_coarse_tune2T_1[dqs_i] = ucdly_coarse_2T;
                            ucmin_coarse_tune0p5T_1[dqs_i] = ucdly_coarse_0p5T;
                            ucmin_fine_tune_1[dqs_i] = ucdly_fine_xT;                            
                        }

                        if (ucpass_begin[dqs_i]==1)
                        {
                            //incr pass tap number
                            ucpass_count_1[dqs_i]++;
                        }
                    }
                    else
                    {
                        if (ucpass_begin[dqs_i]==1)
                        {
                            //at the end of pass range
                            ucpass_begin[dqs_i] = 0;

                            //save the max range settings, to avoid glitch
                            if (ucpass_count_1[dqs_i] > ucpass_count[dqs_i])
                            {
                                ucmin_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T_1[dqs_i];
                                ucmin_coarse_tune0p5T[dqs_i] = ucmin_coarse_tune0p5T_1[dqs_i];
                                ucmin_fine_tune[dqs_i] = ucmin_fine_tune_1[dqs_i];
                                ucpass_count[dqs_i] = ucpass_count_1[dqs_i];
                            }
                        }
                    }
                }                                 
            }
        }
    }
    
    //reset phy R_DMPHYRST: 0xf0[28] 
    // 0x0f0[28] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
    mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

    // read data counter reset
    // 0x0f4[25] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
    mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);                
    mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);


		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
		mcCLR_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
		mcDELAY_us(1);
		mcSET_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);

		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
		mcCLR_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);
		mcDELAY_us(1);
		mcSET_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);


    //check if there is no pass taps for each DQS
    ucerr_cnt = 0;
    for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
    {
        if (ucpass_count[dqs_i]==0)
        {
            mcSHOW_ERROR_CHIP_DisplayString("error, no pass taps in DQS_");
			mcSHOW_ERROR_CHIP_DisplayInteger(dqs_i);
			mcSHOW_ERROR_CHIP_DisplayString(" !!!\n");
            ucerr_cnt++;
        }
    }

    if (ucerr_cnt != 0)
    {
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_GATING_CLIBRATION,\
							FLAG_CALIBRATION_FAIL,\
							FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
        while (1);
#endif
		return DRAM_FAIL;
    }


	for(dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
	{
		if((ucpass_count[dqs_i]*2)<TCMGET_DLYCELL_PERT)//gating window size<UI
		{
			mcSHOW_ERROR_CHIP_DisplayString("Gating calibration byte ");
			mcSHOW_ERROR_CHIP_DisplayInteger(dqs_i);
			mcSHOW_ERROR_CHIP_DisplayString(" window size is too small!!\n");
			fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL;
		}
		else if((ucpass_count[dqs_i]*4)>(TCMGET_DLYCELL_PERT*5))//gating window size>2.5UI
		{
			mcSHOW_ERROR_CHIP_DisplayString("Gating calibration byte ");
			mcSHOW_ERROR_CHIP_DisplayInteger(dqs_i);
			mcSHOW_ERROR_CHIP_DisplayString(" window size is too big!!\n");
			fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG;
		}
	}
	if(fgfail==DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_GATING_CLIBRATION,\
								FLAG_WINDOW_TOO_SMALL,\
								FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif
		return DRAM_FAIL;
	}
	else if(fgfail==DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_GATING_CLIBRATION,\
								FLAG_WINDOW_TOO_BIG,\
								FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif
		return DRAM_FAIL;
	}

    //find center of each byte
    for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
    {
        //note: confirm SoC can use "/" and "%" operations
        uctmp_offset = (ucpass_count[dqs_i]-1)/2;
        uctmp_value = ucmin_fine_tune[dqs_i]+uctmp_offset;
        ucbest_fine_tune[dqs_i] = uctmp_value%(ucRX_DLY_DQSIENSTB_LOOP+1);

        uctmp_offset = uctmp_value / (ucRX_DLY_DQSIENSTB_LOOP+1);
        //coase tune 0.5T : 1~4 -> 0~3 first, after modulo will recover
        uctmp_value = (ucmin_coarse_tune0p5T[dqs_i]-1)+uctmp_offset;
        ucbest_coarse_tune0p5T[dqs_i] = (uctmp_value%RX_DQS_CTL_LOOP)+1;

        uctmp_offset = uctmp_value/RX_DQS_CTL_LOOP;
        ucbest_coarse_tune2T[dqs_i] = ucmin_coarse_tune2T[dqs_i]+uctmp_offset;
        // may no need to check, it is impossible to exceed if pass_count is correct
        //ucbest_coarse_tune2T[dqs_i] = ((ucbest_coarse_tune2T[dqs_i] > (DLY_DQSIENSTB_MAX)) ? (DLY_DQSIENSTB_MAX) : ucbest_coarse_tune2T[dqs_i]);
    }

    //check if coarse tune 2T the same per channel
    //find min and max
    ucmin = 255;
    ucmax = 0;
    for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
    {
        if (ucbest_coarse_tune2T[dqs_i]<ucmin)
        {
            ucmin = ucbest_coarse_tune2T[dqs_i];
        }

        if (ucbest_coarse_tune2T[dqs_i]>ucmax)
        {
            ucmax = ucbest_coarse_tune2T[dqs_i];
        }
    }

    if (ucmin == ucmax)
    {
        //all DQS are the same
        mcSHOW_DBG_MSG("Best Coarse Tune for 2T are the same...Done!!\n");
        ucfinal_coarse_tune2T = ucmin;
        for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
        {
            ucfinal_coarse_tune0p5T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i];
            ucfinal_fine_tune[dqs_i] = ucbest_fine_tune[dqs_i];
        }
    }
    else
    {
        //different values between Coarse tune 2T (per channel setting)
        //set final coarse tune 2T to min 
        ucfinal_coarse_tune2T = ucmin;
        for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
        {
            if (ucbest_coarse_tune2T[dqs_i] != ucfinal_coarse_tune2T)
            {
                // 1 x coarse_tune2T = 4 x coarse_tune0p5T
                ucfinal_coarse_tune0p5T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i] + RX_DQS_CTL_LOOP*(ucbest_coarse_tune2T[dqs_i]-ucfinal_coarse_tune2T);
                if (ucfinal_coarse_tune0p5T[dqs_i]>RX_DQS_CTL_MAX)
                {
                    //Exceed max taps of coarse_tune0p5T
                    // 1 x fine_tune = ? x coarse_tune0p5T (need to get from JMeter, assume fine_tune is 0.1T here)
                    ucfinal_fine_tune[dqs_i] = ucbest_fine_tune[dqs_i]+ucRX_DLY_DQSIENSTB_LOOP*(ucfinal_coarse_tune0p5T[dqs_i]-RX_DQS_CTL_MAX);
                    ucfinal_fine_tune[dqs_i] = ((ucfinal_fine_tune[dqs_i] > (RX_DLY_DQSIENSTB_MAX)) ? (RX_DLY_DQSIENSTB_MAX) : ucfinal_fine_tune[dqs_i]);
                    ucfinal_coarse_tune0p5T[dqs_i] = RX_DQS_CTL_MAX;
                }
                else
                {
                    ucfinal_fine_tune[dqs_i] = ucbest_fine_tune[dqs_i];
                }
            }
            else
            {
                ucfinal_coarse_tune0p5T[dqs_i] = ucbest_coarse_tune0p5T[dqs_i];
                ucfinal_fine_tune[dqs_i] = ucbest_fine_tune[dqs_i];
            }
        }
    }

    //switch to hw calibration RG_RX_*_DLY_DQSIENSTB_*_SWEN (no need for A60807)
    //if {[string compare -nocase $channel chA]==0} {
    //    RG_A1_RX_DLY_DQSIENSTB_0_SWEN: 0x103c[23] = 0
    //    RG_A1_RX_DLY_DQSIENSTB_1_SWEN: 0x1040[23] = 0
    //    RG_A2_RX_DLY_DQSIENSTB_2_SWEN: 0x123c[23] = 0
    //    RG_A2_RX_DLY_DQSIENSTB_3_SWEN: 0x1240[23] = 0
    //} elseif {[string compare -nocase $channel chB]==0} {
    //     RG_B1_RX_DLY_DQSIENSTB_0_SWEN: 0x128c[23] = 0
    //     RG_B1_RX_DLY_DQSIENSTB_1_SWEN: 0x1290[23] = 0
    //     RG_B2_RX_DLY_DQSIENSTB_2_SWEN: 0x143c[23] = 0
    //     RG_B2_RX_DLY_DQSIENSTB_3_SWEN: 0x1440[23] = 0
    //}
    for (dqs_i=0; dqs_i<DQS_NUMBER; dqs_i++)
    {
	    
	   u4Min[dqs_i] = 0 - (ucpass_count[dqs_i] - 1) / 2;
	   u4Max[dqs_i] = (ucpass_count[dqs_i] - 1) - (ucpass_count[dqs_i] - 1) / 2;

	   CHIP_DisplayString("Byte ");
	   CHIP_DisplayInteger(dqs_i);
	   CHIP_DisplayString(" : Gating(");
	   CHIP_DisplayInteger(u4Min[dqs_i]);
	   CHIP_DisplayString(" ~ ");
	   CHIP_DisplayInteger(u4Max[dqs_i]);
	   CHIP_DisplayString("), Size=");
	   CHIP_DisplayInteger(ucpass_count[dqs_i]);
	   CHIP_DisplayString(" .\n");

    }

    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG2("    dqs input gating widnow, final delay value\n)");
    mcSHOW_DBG_MSG2("===============================================================================\n");
    mcSHOW_DBG_MSG("final Coarse 2T = %d\n", ucfinal_coarse_tune2T);
    mcSHOW_DBG_MSG("final DQS0 Coarse 0.5T = %d\n", ucfinal_coarse_tune0p5T[0]);
    mcSHOW_DBG_MSG("final DQS1 Coarse 0.5T = %d\n", ucfinal_coarse_tune0p5T[1]);
    mcSHOW_DBG_MSG("final DQS2 Coarse 0.5T = %d\n", ucfinal_coarse_tune0p5T[2]);
    mcSHOW_DBG_MSG("final DQS3 Coarse 0.5T = %d\n", ucfinal_coarse_tune0p5T[3]);
    mcSHOW_DBG_MSG("__________________________________________________\n");
    mcSHOW_DBG_MSG("final DQS0 fine tune = %d\n", ucfinal_fine_tune[0]);
    mcSHOW_DBG_MSG("final DQS1 fine tune = %d\n", ucfinal_fine_tune[1]);
    mcSHOW_DBG_MSG("final DQS2 fine tune = %d\n", ucfinal_fine_tune[2]);
    mcSHOW_DBG_MSG("final DQS3 fine tune = %d\n", ucfinal_fine_tune[3]);
    mcSHOW_DBG_MSG2("===============================================================================\n");
	
	mcSHOW_DBG_MSG2("\n    dqs input gating widnow, best delay value\n");
    mcSHOW_DBG_MSG2("===============================================================================\n"); 
    mcSHOW_DBG_MSG2("best DQS0 Coarse 2T = %d\n", ucbest_coarse_tune2T[0]);
    mcSHOW_DBG_MSG2("best DQS1 Coarse 2T = %d\n", ucbest_coarse_tune2T[1]);
    mcSHOW_DBG_MSG2("best DQS2 Coarse 2T = %d\n", ucbest_coarse_tune2T[2]);
    mcSHOW_DBG_MSG2("best DQS3 Coarse 2T = %d\n", ucbest_coarse_tune2T[3]);
    mcSHOW_DBG_MSG2("__________________________________________________\n");
    mcSHOW_DBG_MSG2("best DQS0 Coarse 0.5T = %d\n", ucbest_coarse_tune0p5T[0]);
    mcSHOW_DBG_MSG2("best DQS1 Coarse 0.5T = %d\n", ucbest_coarse_tune0p5T[1]);
    mcSHOW_DBG_MSG2("best DQS2 Coarse 0.5T = %d\n", ucbest_coarse_tune0p5T[2]);
    mcSHOW_DBG_MSG2("best DQS3 Coarse 0.5T = %d\n", ucbest_coarse_tune0p5T[3]);
    mcSHOW_DBG_MSG2("__________________________________________________\n");
    mcSHOW_DBG_MSG2("best DQS0 fine tune = %d\n", ucbest_fine_tune[0]);
    mcSHOW_DBG_MSG2("best DQS1 fine tune = %d\n", ucbest_fine_tune[1]);
    mcSHOW_DBG_MSG2("best DQS2 fine tune = %d\n", ucbest_fine_tune[2]);
    mcSHOW_DBG_MSG2("best DQS3 fine tune = %d\n", ucbest_fine_tune[3]);
    mcSHOW_DBG_MSG2("===============================================================================\n");
	

        //set final coarse tune 2T delay value
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0xa4c));
        mcSET_FIELD(u4value, ucfinal_coarse_tune2T, 0x00000700, 8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0xa4c), u4value);	

        //set final coarse tune 0.5T delay value
        u2one_hot_dly = (U16) 1<<(ucfinal_coarse_tune0p5T[0]-1);
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x038));
        mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x038), u4value);

        u2one_hot_dly = (U16) 1<<(ucfinal_coarse_tune0p5T[1]-1);
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x03c));
        mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), u4value);

        u2one_hot_dly = (U16) 1<<(ucfinal_coarse_tune0p5T[2]-1);
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x238));
        mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x238), u4value);

        u2one_hot_dly = (U16) 1<<(ucfinal_coarse_tune0p5T[3]-1);
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x23c));
        mcSET_FIELD(u4value, u2one_hot_dly, 0x00000fff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), u4value);

        //set final fine tune delay value
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x03c));
        mcSET_FIELD(u4value, ucfinal_fine_tune[0], 0x003f0000, 16);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x03c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x040));
        mcSET_FIELD(u4value, ucfinal_fine_tune[1], 0x003f0000, 16);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x040), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x23c));
        mcSET_FIELD(u4value, ucfinal_fine_tune[2], 0x003f0000, 16);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x23c), u4value);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x240));
        mcSET_FIELD(u4value, ucfinal_fine_tune[3], 0x003f0000, 16);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x240), u4value);



		//RG_A1_TX_ARDQS0_R75KP=1; 0x5801c[21]=1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x01c));
        mcSET_BIT(u4value, 21);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), u4value);
		//RG_A1_TX_ARDQS1_R75KP=1; 0x58020[5]=1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x020));
        mcSET_BIT(u4value, 5);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), u4value);
		//RG_A2_TX_ARDQS2_R75KP=1; 0x5821c[21]=1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x21c));
        mcSET_BIT(u4value, 21);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), u4value);
		//RG_A2_TX_ARDQS3_R75KP=1; 0x58220[5]=1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x220));
        mcSET_BIT(u4value, 5);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), u4value);		

	
    //temp solution, dummy read/write before reset
    DramcEngine2(TE_OP_READ_CHECK, test2_0, test2_1, test2_2);

    //Reset after scan to avoid error gating counter due to DQS glitch
    //reset phy R_DMPHYRST: 0xf0[28] 
    // 0x0f0[28] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
    mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

    // read data counter reset
    // 0x0f4[25] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
    mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);                
    mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);


		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
		mcCLR_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
		mcDELAY_us(1);
		mcSET_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);

		u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
		mcCLR_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);
		mcDELAY_us(1);
		mcSET_BIT(u4value, 0);
		ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);


	UNUSED(u4Avg);
	UNUSED(u4Min);
	UNUSED(u4Max);
    
	return DRAM_OK;

    // log example
    /*
======================================================================
x = dqs result
y = coarse_2T  coarse_0.5T  finetune
----------------------------------------------------------------------
y  |  dqs0f   dqs0r   dqs1f   dqs1r   dqs2f   dqs2r   dqs3f   dqs3r
----------------------------------------------------------------------
 6   1   0  |  ff  ff   0   0  ff  ff  ff  ff
 6   1   1  |  ff  ff   0   0  ff  ff  ff  ff
 6   1   2  |  ff  ff   0   0  ff  ff  ff  ff
 6   1   3  |  ff  ff   0   0  ff  ff  ff  ff
 6   1   4  |  ff  ff   0   0  ff  ff  7f  7f
 6   1   5  |  ff  ff   0   0  ff  ff  fe  fe
 6   1   6  |  ff  ff  c7  c7  ff  ff  ff  ff
 6   1   7  |  ff  ff  ff  ff  7f  7f  ff  ff
 6   1   8  |  ff  ff  ff  ff  ff  ff  ff  ff
 6   1   9  |  ff  ff  ff  ff  ff  ff  ff  ff
 6   1  10  |  ff  ff  ff  ff  ff  ff  ff  ff
 6   1  11  |  ff  ff  ff  ff   0   0   0   0
 6   1  12  |  ff  ff  ff  ff   0   0   0   0
 6   1  13  |  ff  ff  ff  ff   0   0   0   0
 6   1  14  |  ff  ff  ff  ff   0   0   0   0
 6   1  15  |  ff  ff  ff  ff   0   0   0   0
 6   1  16  |  ff  ff  ff  ff   0   0   0   0
 6   1  17  |  ff  ff  ff  ff   0   0   0   0
 6   1  18  |  ff  ff  df  df   0   0   0   0
 6   1  19  |  ff  ff  ff  ff   0   0   0   0
 6   1  20  |  ff  ff  ff  ff   0   0   0   0
 6   1  21  |  14  14  ff  ff   0   0   0   0
 6   1  22  |  ff  ff  ff  ff   0   0   0   0
 6   1  23  |  ff  ff  ff  ff   0   0   0   0
 6   1  24  |  ff  ff  ff  ff   0   0   0   0
 6   2   0  |   0   0   0   0   0   0   0   0
 6   2   1  |   0   0   0   0   0   0   0   0
 6   2   2  |   0   0   0   0   0   0   0   0
 6   2   3  |   0   0   0   0   0   0   0   0
 6   2   4  |   0   0   0   0   0   0   0   0
 6   2   5  |   0   0   0   0   0   0   0   0
 6   2   6  |   0   0   0   0   0   0   0   0
 6   2   7  |   0   0   0   0   0   0   0   0
 6   2   8  |   0   0   0   0   0   0   0   0
 6   2   9  |   0   0   0   0   0   0   0   0
 6   2  10  |   0   0   0   0   0   0   0   0
 6   2  11  |   0   0   0   0   0   0   0   0
 6   2  12  |   0   0   0   0   0   0   0   0
 6   2  13  |   0   0   0   0   0   0   0   0
 6   2  14  |   0   0   0   0   0   0   0   0
 6   2  15  |   0   0   0   0   0   0   0   0
 6   2  16  |   0   0   0   0   0   0   0   0
 6   2  17  |   0   0   0   0   0   0   0   0
 6   2  18  |   0   0   0   0   0   0   0   0
 6   2  19  |   0   0   0   0   0   0   0   0
 6   2  20  |   0   0   0   0   0   0   0   0
 6   2  21  |   0   0   0   0   0   0   0   0
 6   2  22  |   0   0   0   0   0   0   0   0
 6   2  23  |   0   0   0   0   0   0   0   0
 6   2  24  |   0   0   0   0   0   0   0   0
 6   3   0  |   0   0   0   0   0   0   0   0
 6   3   1  |   0   0   0   0   0   0   0   0
 6   3   2  |   0   0   0   0   0   0   0   0
 6   3   3  |   0   0   0   0   0   0   0   0
 6   3   4  |   0   0   0   0   0   0   0   0
 6   3   5  |   0   0   0   0   0   0   0   0
 6   3   6  |   0   0   0   0   0   0   0   0
 6   3   7  |   0   0   0   0   0   0   0   0
 6   3   8  |   0   0   0   0   0   0   0   0
 6   3   9  |   0   0   0   0   0   0   0   0
 6   3  10  |   0   0   0   0   0   0   0   0
 6   3  11  |   0   0   0   0  ff  ff   0   0
 6   3  12  |   0   0   0   0  ff  ff  ff  ff
 6   3  13  |   0   0   0   0  ff  ff  ff  ff
 6   3  14  |   0   0   0   0  ff  ff  ff  ff
 6   3  15  |   0   0   0   0  ff  ff  ff  ff
 6   3  16  |   0   0   0   0  ff  ff  ff  ff
 6   3  17  |   0   0   0   0  ff  ff  ff  ff
 6   3  18  |   0   0   0   0  ff  ff  ff  ff
 6   3  19  |   0   0   0   0  ff  ff  ff  ff
 6   3  20  |   0   0   0   0  ff  ff  ff  ff
 6   3  21  |   0   0   0   0  ff  ff  ff  ff
 6   3  22  |   0   0   0   0  ff  ff  ff  ff
 6   3  23  |   0   0   0   0  ff  ff  ff  ff
 6   3  24  |   0   0  ff  ff  ff  ff  ff  ff
 6   4   0  |  ff  ff  ff  ff  ff  ff  ff  ff
 6   4   1  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   2  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   3  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   4  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   5  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   6  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   7  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   8  |  ff  ff  ff  ff  ff  ff   0   0
 6   4   9  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  10  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  11  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  12  |  ff  ff  ff  ff  ff  ff  ff  ff
 6   4  13  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  14  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  15  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  16  |  ff  ff  ff  ff  ff  ff   0   0
 6   4  17  |  fb  fb  ff  ff  ff  ff   0   0
 6   4  18  |  c1  c1  ff  ff  ff  ff   0   0
 6   4  19  |  80   0  ff  ff  ff  ff   0   0
 6   4  20  |  80   0  ff  ff  ff  ff   0   0
 6   4  21  |  80   0  ff  ff  ff  ff   0   0
 6   4  22  |  80   0  ff  ff  ff  ff   0   0
 6   4  23  |  80   0  ff  ff  ff  ff   0   0
 6   4  24  |  80   0  ff  ff  ff  ff   0   0
Best Coarse Tune for 2T are the same...Done!!
======================================================================

    dqs input gating widnow, final delay value
    apply =1
======================================================================
bl_type: 1 test2_1: 1426063360 test2_2: -1442840573 counter: 20202020
final Coarse 2T = 6
final DQS0 Coarse 0.5T = 2
final DQS1 Coarse 0.5T = 2
final DQS2 Coarse 0.5T = 2
final DQS3 Coarse 0.5T = 2
__________________________________________________
final DQS0 fine tune = 24
final DQS1 fine tune = 24
final DQS2 fine tune = 10
final DQS3 fine tune = 11
======================================================================

    dqs input gating widnow, best delay value
======================================================================
best DQS0 Coarse 2T = 6
best DQS1 Coarse 2T = 6
best DQS2 Coarse 2T = 6
best DQS3 Coarse 2T = 6
__________________________________________________
best DQS0 Coarse 0.5T = 2
best DQS1 Coarse 0.5T = 2
best DQS2 Coarse 0.5T = 2
best DQS3 Coarse 0.5T = 2
__________________________________________________
best DQS0 fine tune = 24
best DQS1 fine tune = 24
best DQS2 fine tune = 10
best DQS3 fine tune = 11
======================================================================
   */
}
#endif

#ifdef DRAM_RX_WINDOW_PERBIT_CAL
#ifdef DRAM_RX_ODT_SCAN
	U32 u4rx_bitwindowsum;
#endif
//-------------------------------------------------------------------------
/** DramcRxWindowPerbitCal (v2 version)
 *  start the rx dqs perbit sw calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxWindowPerbitCal(U8 only_get_bitwindowsum)
{
    U8 ii, jj;
    U32 u4value, u4err_value, u4fail_bit;
    RXDQS_PERBIT_DLY_T dqdqs_perbit_dly[DQ_DATA_WIDTH];
    U8 ucbit_first, ucbit_last;
	U8 uchold_pass_number[DQ_DATA_WIDTH];
	U8 ucsetup_pass_number[DQ_DATA_WIDTH];
    U8 ucmax_dqsdly_byte[DQS_NUMBER];
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;
	S8 s1size_byte[DQ_DATA_WIDTH]={0};
	U8 fgfail = DRAM_CALIBRATION_PASS;
	
    // 1.delay DQ ,find the pass widnow (left boundary).
    // 2.delay DQS find the pass window (right boundary). 
    // 3.Find the best DQ / DQS to satify the middle value of the overall pass window per bit
    // 4.Set DQS delay to the max per byte, delay DQ to de-skew

    // 1
    // set DQS delay to 0 first
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), 0);

    // initialize parameters
    for (ii = 0; ii < DQ_DATA_WIDTH; ii++)
    {
        dqdqs_perbit_dly[ii].first_dqdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqdly_pass = -2;
        dqdqs_perbit_dly[ii].first_dqsdly_pass = -1;
        dqdqs_perbit_dly[ii].last_dqsdly_pass = -2;
    }

	if(!only_get_bitwindowsum)
	{
		mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
		mcSHOW_DBG_MSG2("Start DQ delay to find pass range, DQS delay fixed to 0...\n");
		mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
		mcSHOW_DBG_MSG2("x-axis is bit #; y-axis is DQ delay (%d~%d)\n", 0, MAX_RX_DQDLY_TAPS-1);
	}
    // delay DQ from 0 to 15 to get the setup time
    for (ii = 0; ii < MAX_RX_DQDLY_TAPS; ii++)
    {
        for (jj=0; jj<DQ_DATA_WIDTH; jj=jj+4)
        {
            //every 4bit dq have the same delay register address
            u4value = ((U32) ii) + (((U32)ii)<<8) + (((U32)ii)<<16) + (((U32)ii)<<24);  
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), u4value);            
        }

        //Reset after scan to avoid error gating counter due to DQS glitch
        //reset phy R_DMPHYRST: 0xf0[28] 
        // 0x0f0[28] = 1 -> 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
        mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_us(1);
        mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

        // read data counter reset
        // 0x0f4[25] = 1 -> 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
        mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_us(1);                
        mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);       


        // enable TE2, audio pattern
        u4err_value = 0;
        for (jj = 0; jj < 1; jj++)
        {
            u4err_value |= DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);
        }            
      

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < DQ_DATA_WIDTH; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqdly_pass == -1)
                {
                    // first DQ pass delay tap
                    dqdqs_perbit_dly[jj].first_dqdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqdly_pass == -2)
                {
                    if (ii == (MAX_RX_DQDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqdly_pass = ii;
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqdly_pass = ii -1;
                }
            }
			if(!only_get_bitwindowsum)
			{
	            if (u4fail_bit == 0)
	            {
	                mcSHOW_DBG_MSG2("o");
	            }
	            else
	            {
	                mcSHOW_DBG_MSG2("x");
	            }
        	}
        }
		if(!only_get_bitwindowsum)
        	mcSHOW_DBG_MSG2("\n");
    }

    // 2
    //set dq delay to 0
    for (ii = 0; ii < 8; ii++)
    {
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+4*ii), 0x00000000);
    }

    //dqs from 1
    if(!only_get_bitwindowsum)
    {
	    mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
	    mcSHOW_DBG_MSG2("Start DQS delay to find pass range, DQ delay fixed to 0...\n");
	    mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
	    mcSHOW_DBG_MSG2("x-axis is bit #; y-axis is DQS delay (%d~%d)\n", 1, MAX_RX_DQSDLY_TAPS-1);
    }
    // because the tap DQdly=0 DQSdly=0 will be counted when we delay dq ,so we don't count it here
    // so we set first dqs delay to 1
    for (ii = 1; ii < MAX_RX_DQSDLY_TAPS; ii++)
    {
        // 0x18
        u4value = ((U32) ii) + (((U32)ii)<<8) + (((U32)ii)<<16) + (((U32)ii)<<24);            
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), u4value);

        //Reset after scan to avoid error gating counter due to DQS glitch
        //reset phy R_DMPHYRST: 0xf0[28] 
        // 0x0f0[28] = 1 -> 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
        mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_us(1);
        mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

        // read data counter reset
        // 0x0f4[25] = 1 -> 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
        mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
        //delay 10ns, 1ms here
        mcDELAY_us(1);                
        mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);     
        
        // enable TE2, audio pattern
        u4err_value = 0;
        for (jj = 0; jj < 1; jj++)
        {
            u4err_value |= DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);
        }            

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < DQ_DATA_WIDTH; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqsdly_pass == -1)
                {
                    // first DQS pass delay tap
                    dqdqs_perbit_dly[jj].first_dqsdly_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqsdly_pass == -2)
                {
                    if (ii == (MAX_RX_DQSDLY_TAPS-1))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqsdly_pass = ii;
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqsdly_pass != -1)&&(dqdqs_perbit_dly[jj].last_dqsdly_pass == -2))
                {
                    dqdqs_perbit_dly[jj].last_dqsdly_pass = ii -1;
                }
            }
			if(!only_get_bitwindowsum)
			{
		        if (u4fail_bit == 0)
		        {
		            mcSHOW_DBG_MSG2("o");
		        }
		        else
		        {
		            mcSHOW_DBG_MSG2("x");
		        }
			}
        }
		if(!only_get_bitwindowsum)
        	mcSHOW_DBG_MSG2("\n");
    }

    // 3
    if(!only_get_bitwindowsum)
    {
		mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
		mcSHOW_DBG_MSG2("Start calculate dq time and dqs time / \n");
		mcSHOW_DBG_MSG2("Find max DQS delay per byte / Adjust DQ delay to align DQS...\n");
		mcSHOW_DBG_MSG2("------------------------------------------------------\n"); 
    }
	//As per byte, check max DQS delay in 8-bit. Except for the bit of max DQS delay, delay DQ to fulfill setup time = hold time
    for (ii = 0; ii < DQS_NUMBER; ii++)
    {
        ucbit_first = 8*ii;
        ucbit_last = 8*ii+7;
        ucmax_dqsdly_byte[ii] = 0;
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // hold time = DQS pass taps
            uchold_pass_number[jj] = dqdqs_perbit_dly[jj].last_dqsdly_pass - dqdqs_perbit_dly[jj].first_dqsdly_pass + 1;
            // setup time = DQ pass taps
            ucsetup_pass_number[jj] = dqdqs_perbit_dly[jj].last_dqdly_pass - dqdqs_perbit_dly[jj].first_dqdly_pass + 1;

            if (uchold_pass_number[jj] > ucsetup_pass_number[jj])
            {
                if (ucsetup_pass_number[jj] != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxoooooooo|ooooooooooooooooooooxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number[jj] - ucsetup_pass_number[jj]) / 2;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxooooooooooxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    dqdqs_perbit_dly[jj].best_dqsdly = (uchold_pass_number[jj] - ucsetup_pass_number[jj]) / 2 + dqdqs_perbit_dly[jj].first_dqsdly_pass;

                    if (dqdqs_perbit_dly[jj].best_dqsdly > ucmax_dqsdly_byte[ii])
                    {
                        ucmax_dqsdly_byte[ii] = dqdqs_perbit_dly[jj].best_dqsdly;
                    }
                }
            }
            else if (uchold_pass_number[jj] < ucsetup_pass_number[jj])
            {
                if (uchold_pass_number[jj] != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooooo|ooooooooxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number[jj] - uchold_pass_number[jj]) / 2;                    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxoooooooooooooooxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = (ucsetup_pass_number[jj] - uchold_pass_number[jj]) / 2 + dqdqs_perbit_dly[jj].first_dqdly_pass;                    
                }
            }
            else   // hold time == setup time
            {
                if (uchold_pass_number[jj] != 0)
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxoooooo|ooooooxxxxxxxxxxxxxxxxxxx
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;    
                }
                else
                {
                    // like this:
                    // setup time(dq delay)     hold time(dqs delay)
                    // 15                  0 1                       15 tap
                    // xxxxxxxxxxxxxxxxxxxxx|xxxxxxxxxxxxxxxxxxxxxxxxx
                    // mean this bit is error
                    mcSHOW_ERROR_CHIP_DisplayString("error on bit ");
					mcSHOW_ERROR_CHIP_DisplayInteger(jj);
					mcSHOW_ERROR_CHIP_DisplayString(" ,setup_time =hold_time =0!!\n ");
                    dqdqs_perbit_dly[jj].best_dqsdly = 0;
                    dqdqs_perbit_dly[jj].best_dqdly = 0;
                    fgfail = DRAM_CALIBRATION_FAIL;
                }
            } 
			if(!only_get_bitwindowsum)
            	mcSHOW_DBG_MSG("bit#%2d : dq time=%2d dqs time=%2d\n", jj, ucsetup_pass_number[jj], uchold_pass_number[jj]);                        
        }

        //mcSHOW_DBG_MSG("----seperate line----\n");

        // we delay DQ or DQS to let DQS sample the middle of tx pass window for all the 8 bits,
        for (jj = ucbit_first; jj <= ucbit_last; jj++)
        {
            // set DQS to max for 8-bit
            if (dqdqs_perbit_dly[jj].best_dqsdly < ucmax_dqsdly_byte[ii])
            {
                // delay DQ to compensate extra DQS delay
                dqdqs_perbit_dly[jj].best_dqdly = dqdqs_perbit_dly[jj].best_dqdly + (ucmax_dqsdly_byte[ii] - dqdqs_perbit_dly[jj].best_dqsdly);
                // max limit to 15
                dqdqs_perbit_dly[jj].best_dqdly = ((dqdqs_perbit_dly[jj].best_dqdly > 15) ? 15 : dqdqs_perbit_dly[jj].best_dqdly);
            }            
        }
    }
    if(!only_get_bitwindowsum)   
	{
		mcSHOW_DBG_MSG("==================================================\n");
	    mcSHOW_DBG_MSG("    dramc_rxdqs_perbit_swcal_v2\n");
	    mcSHOW_DBG_MSG("==================================================\n");
	    mcSHOW_DBG_MSG("DQS Delay :\n DQS0 = %d DQS1 = %d DQS2 = %d DQS3 = %d\n", ucmax_dqsdly_byte[0], ucmax_dqsdly_byte[1], ucmax_dqsdly_byte[2], ucmax_dqsdly_byte[3]);
	    mcSHOW_DBG_MSG2("DQ Delay :\n");
	    for (ii = 0; ii < 32; ii++)
	    {
	        mcSHOW_DBG_MSG2("DQ%2d = %2d \n", ii, dqdqs_perbit_dly[ii].best_dqdly);
	    }
	    mcSHOW_DBG_MSG("________________________________________________________________________\n");
	}
#ifdef DRAM_RX_ODT_SCAN
	u4rx_bitwindowsum = 0;
	for(jj=0;jj<32;jj++)
		u4rx_bitwindowsum = u4rx_bitwindowsum + (uchold_pass_number[jj] + ucsetup_pass_number[jj]);

	if(only_get_bitwindowsum)
	{
		return DRAM_OK;
	}
#endif
    // set dqs delay
    u4value = ((U32) ucmax_dqsdly_byte[0]) + (((U32)ucmax_dqsdly_byte[1])<<8) + (((U32)ucmax_dqsdly_byte[2])<<16) + (((U32)ucmax_dqsdly_byte[3])<<24);            
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DELDLY1), u4value);
    // set dq delay
    for (jj=0; jj<DQ_DATA_WIDTH; jj=jj+4)
    {
		//every 4bit dq have the same delay register address
		u4value = ((U32) dqdqs_perbit_dly[jj].best_dqdly) + (((U32)dqdqs_perbit_dly[jj+1].best_dqdly)<<8) + (((U32)dqdqs_perbit_dly[jj+2].best_dqdly)<<16) + (((U32)dqdqs_perbit_dly[jj+3].best_dqdly)<<24);  
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQIDLY1+jj), u4value);            
    }              

    // Log example
    /*
------------------------------------------------------
Start calculate dq time and dqs time /
Find max DQS delay per byte / Adjust DQ delay to align DQS...
------------------------------------------------------
bit# 0 : dq time=11 dqs time= 8
bit# 1 : dq time=11 dqs time= 8
bit# 2 : dq time=11 dqs time= 6
bit# 3 : dq time=10 dqs time= 8
bit# 4 : dq time=11 dqs time= 8
bit# 5 : dq time=10 dqs time= 8
bit# 6 : dq time=11 dqs time= 8
bit# 7 : dq time= 9 dqs time= 6
----seperate line----
bit# 8 : dq time=12 dqs time= 7
bit# 9 : dq time=10 dqs time= 8
bit#10 : dq time=11 dqs time= 8
bit#11 : dq time=10 dqs time= 8
bit#12 : dq time=11 dqs time= 8
bit#13 : dq time=11 dqs time= 8
bit#14 : dq time=11 dqs time= 8
bit#15 : dq time=12 dqs time= 8
----seperate line----
bit#16 : dq time=11 dqs time= 7
bit#17 : dq time=10 dqs time= 8
bit#18 : dq time=11 dqs time= 7
bit#19 : dq time=11 dqs time= 6
bit#20 : dq time=10 dqs time= 9
bit#21 : dq time=11 dqs time=10
bit#22 : dq time=11 dqs time=10
bit#23 : dq time= 9 dqs time= 9
----seperate line----
bit#24 : dq time=12 dqs time= 6
bit#25 : dq time=13 dqs time= 6
bit#26 : dq time=13 dqs time= 7
bit#27 : dq time=11 dqs time= 7
bit#28 : dq time=12 dqs time= 8
bit#29 : dq time=10 dqs time= 8
bit#30 : dq time=13 dqs time= 7
bit#31 : dq time=11 dqs time= 8
----seperate line----
==================================================
    dramc_rxdqs_perbit_swcal_v2
    channel=2(2:cha, 3:chb) apply = 1
==================================================
DQS Delay :
 DQS0 = 0 DQS1 = 0 DQS2 = 0 DQS3 = 0
DQ Delay :
DQ 0 =  1 DQ 1 =  1 DQ 2 =  2 DQ 3 =  1
DQ 4 =  1 DQ 5 =  1 DQ 6 =  1 DQ 7 =  1
DQ 8 =  2 DQ 9 =  1 DQ10 =  1 DQ11 =  1
DQ12 =  1 DQ13 =  1 DQ14 =  1 DQ15 =  2
DQ16 =  2 DQ17 =  1 DQ18 =  2 DQ19 =  2
DQ20 =  0 DQ21 =  0 DQ22 =  0 DQ23 =  0
DQ24 =  3 DQ25 =  3 DQ26 =  3 DQ27 =  2
DQ28 =  2 DQ29 =  1 DQ30 =  3 DQ31 =  1
_______________________________________________________________
   */

    for (ii = 0; ii < DQS_NUMBER; ii++)
    {
		s1size_byte[ii] = 64;
        for (jj = 8*ii; jj <= 8*ii+7; jj++)
        {
			if(s1size_byte[ii]>(ucsetup_pass_number[jj]+uchold_pass_number[jj]))
				s1size_byte[ii] = ucsetup_pass_number[jj]+uchold_pass_number[jj];
		}
    }

    for (ii = 0; ii < DQS_NUMBER; ii++)
    {
		u4value = HAL_READ32(RECORD_DQS_REG + ((ii /2)*4));
		u4value &= ~ (0xFFFF << ((ii & 0x1)*16));
		u4value |= (((UINT32)((s1size_byte[ii]-1)/2) << 8) | ((UINT32)(s1size_byte[ii]/2))) << ((ii & 0x1)*16);
		HAL_WRITE32((RECORD_DQS_REG + ((ii /2)*4)), u4value);
		CHIP_DisplayString("HW Byte ");
		CHIP_DisplayInteger(ii);
		CHIP_DisplayString(" : DQS(");
		CHIP_DisplayInteger((ucmax_dqsdly_byte[ii]-s1size_byte[ii]/2));
		CHIP_DisplayString(" ~ ");
		CHIP_DisplayInteger((ucmax_dqsdly_byte[ii]+(s1size_byte[ii]-1)/2));
		CHIP_DisplayString("), Size ");
		CHIP_DisplayInteger(s1size_byte[ii]);
		CHIP_DisplayString(" , Set ");
		CHIP_DisplayInteger(ucmax_dqsdly_byte[ii]);
		CHIP_DisplayString(". \n");
    }

    if (fgfail == DRAM_CALIBRATION_FAIL)
    {
        mcSHOW_ERROR_CHIP_DisplayString("RX DQ/DQS calibration fail!\n");
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_RX_DQS_CALIBRATION,\
								FLAG_CALIBRATION_FAIL,\
								FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif
        return DRAM_FAIL;
    }

		for(ii=0;ii<DQS_NUMBER;ii++)
		{
#if defined(DRAM_LOAD_BOARD) || defined(__MODEL_slt__)
			if((s1size_byte[ii]*4)<TCMGET_DLYCELL_PERT)//rx window size<0.5UI
#else
			if((s1size_byte[ii]*5)<TCMGET_DLYCELL_PERT)//rx window size<0.4UI
#endif
			{
				mcSHOW_ERROR_CHIP_DisplayString("RX calibration byte ");
				mcSHOW_ERROR_CHIP_DisplayInteger(ii);
				mcSHOW_ERROR_CHIP_DisplayString(" window size is too small!!\n");
				fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL;
			}
			else if((s1size_byte[ii]*2)>(TCMGET_DLYCELL_PERT))//rx window size>UI
			{
				mcSHOW_ERROR_CHIP_DisplayString("RX calibration byte ");
				mcSHOW_ERROR_CHIP_DisplayInteger(ii);
				mcSHOW_ERROR_CHIP_DisplayString(" window size is too big!!\n");
				fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG;
			}
		}

	if(fgfail == DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_RX_DQS_CALIBRATION,\
							FLAG_WINDOW_TOO_SMALL,\
							FLAG_NOT_COMPLETE_OR_FAIL);

		while(1);				
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif
		return DRAM_FAIL;
	}
	else if(fgfail == DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_RX_DQS_CALIBRATION,\
							FLAG_WINDOW_TOO_BIG,\
							FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif

		return DRAM_FAIL;
	}
	
	return DRAM_OK;

}
#endif

#ifdef DRAM_RX_ODT_SCAN
void DramcRxOdtScan(void)
{
    U8 uccode_map[32] = {0x00, 0x01, 0x08, 0x10, 0x20, 0x40, 0x80, 0x12, 0x24, 0x48, \
 	                                 0x13, 0x19, 0x46, 0x89, 0x17, 0x1e, 0x4d, 0x35, 0x93, 0x4f, \
 	                                 0x8f, 0x3d, 0xb8, 0x73, 0xb6, 0xe9, 0xb7, 0xeb, 0xf3, 0xef, \
 	                                 0xfe, 0xff};
	U8 ii;
	U32 u4rx_odt_best_index=11;
	U32	u4bitwindowsum_max=0;
	
	mcSHOW_DBG_MSG2("==============================================================\n");
    mcSHOW_DBG_MSG2("    Rx Odt Scan \n");
    mcSHOW_DBG_MSG2("==============================================================\n");	
	
	mcSHOW_DBG_MSG("rx odt code | bitwindowsum\n");
	for(ii=0;ii<26;ii++)
	{
		
		DramcDqDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[ii], uccode_map[ii]);
    	DramcDqsDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[ii], uccode_map[ii]);
    	DramcDqmDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[ii], uccode_map[ii]);
		DramcRxWindowPerbitCal(1);
		
		mcSHOW_DBG_MSG("0x%02x		%d.\n", uccode_map[ii], u4rx_bitwindowsum);

		if(u4bitwindowsum_max < u4rx_bitwindowsum)
		{
			u4bitwindowsum_max = u4rx_bitwindowsum;
			u4rx_odt_best_index = ii;
		}		
	}

	mcSHOW_DBG_MSG2("-----Best rx odt code 0x%02x, bitwindowsum %d.\n", uccode_map[u4rx_odt_best_index], u4bitwindowsum_max);

	DramcDqDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[u4rx_odt_best_index], uccode_map[u4rx_odt_best_index]);
	DramcDqsDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[u4rx_odt_best_index], uccode_map[u4rx_odt_best_index]);
	DramcDqmDriving(DEFAULT_OCDP_DRIVING, DEFAULT_OCDN_DRIVING, uccode_map[u4rx_odt_best_index], uccode_map[u4rx_odt_best_index]);
}
#endif


#ifdef DRAM_RX_DATLAT_CAL
//-------------------------------------------------------------------------
/** DramcRxdatlatCal
 *  scan the pass range of DATLAT for DDRPHY read data window.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxdatlatCal(void)
//DRAM_STATUS_T DramcRxdatlatCal(DRAMC_CTX_T *p, U8 apply, U16 low_freq_wr)
{
    U8 ii;
    U32 u4prv_register_07c, u4prv_register_0e4;
    U32 u4value, u4err_value;
    U8 ucfirst, ucbegin, ucsum, ucbest_step;
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;

#ifdef CC_SUPPORT_STR_CORE_OFF
	if (IS_DDR_SUSPENDSTATE())
	{
		if(!IS_DRAM_CHANNELB_ACTIVE())	//channel trust zone 0x1000000, 16M
			test2_1 = ((((TCMGET_CHANNELA_SIZE()*0x100000)-0x1000000)>>4)-DEFAULT_TEST2_2_CAL)| 0x30000000;
		else
			test2_1 = ((((TCMGET_CHANNELB_SIZE()*0x100000)-0x1000000)>>4)-DEFAULT_TEST2_2_CAL)| 0x30000000;
	}
#endif

    mcSHOW_DBG_MSG("==============================================================\n");
    CHIP_DisplayString("DATLAT calibration \n");
    mcSHOW_DBG_MSG("==============================================================\n");
    
    // pre-save
    // 0x07c[6:4]   DATLAT bit2-bit0
    u4prv_register_07c = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL));
    // 0x0e4[4]     DALLAT bit3
    u4prv_register_0e4 = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4));

    ucbest_step = ((u4prv_register_07c>>4)&0x7) | ((u4prv_register_0e4>>1)&0x8);
    mcSHOW_DBG_MSG("DATLAT Default value = 0x%x\n", ((u4prv_register_07c>>4)&0x7) | ((u4prv_register_0e4>>1)&0x8));

    // 1.set DATLAT 0-15
    // 2.enable engine1 or engine2 
    // 3.check result  ,2-3 taps pass 
    // 4.set DATLAT 1st value when 2 taps pass ,2nd value when 3taps pass

    // Initialize
    ucfirst = 0xff;
    ucbegin = 0;
    ucsum = 0;
	u4value = u4prv_register_07c;
    mcCLR_MASK(u4value, MASK_DDR2CTL_DATLAT);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);

    u4value = u4prv_register_0e4;
    mcCLR_BIT(u4value, POS_PADCTL4_DATLAT3);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);
    
    for (ii = 8; ii < DATLAT_TAP_NUMBER; ii++)
    {        
        // 1
        if (ii == 8)
        {
            // bit3=1 ,bit2=bit1=bit0 =0
            u4value = u4prv_register_0e4;
            mcSET_BIT(u4value, POS_PADCTL4_DATLAT3);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);

            u4value = u4prv_register_07c;
            mcCLR_MASK(u4value, MASK_DDR2CTL_DATLAT);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);
        }
        else
        {
            u4value = u4prv_register_07c;
            mcSET_FIELD(u4value, (ii&0x7), MASK_DDR2CTL_DATLAT, POS_DDR2CTL_DTALAT);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);
        }

        // 2

        // enable TE2, audio pattern
        u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);
       
        
        if (u4err_value == 0)
        {
            if (ucbegin == 0)
            {
                // first tap which is pass
                ucfirst = ii;
                ucbegin = 1;
            }
            if (ucbegin == 1)
            {
                ucsum++;
            }
        }
        else
        {
            if (ucbegin == 1)
            {
                // pass range end
                ucbegin = 0xff;
            }
        }

        mcSHOW_DBG_MSG2("TAP=%2d, err_value=0x%8x, begin=%d, first=%3d, sum=%d\n", ii, u4err_value, ucbegin, ucfirst, ucsum);
    }

    // 4
    if (ucsum == 0)
    {
        mcSHOW_ERROR_CHIP_DisplayString("no DATLAT taps pass\n");
		return DRAM_FAIL;
    }    
    else if (ucsum > 1)
    {
        //if test engine2, DLE return not so density, once pass, always pass
        //so we choose the second one as the best
        ucbest_step = ucfirst + 1;
    }
    else
    {
        ucbest_step = ucfirst + (ucsum-1)/2;
    }

	CHIP_DisplayString("first_step= ");
	CHIP_DisplayInteger(ucfirst);
	CHIP_DisplayString(" total pass= ");
	CHIP_DisplayInteger(ucsum);
	CHIP_DisplayString(" best_step=");
	CHIP_DisplayInteger(ucbest_step);
	CHIP_DisplayString(" .\n");
    

    if (ucsum == 0)
    {
        mcSHOW_ERROR_CHIP_DisplayString("DATLAT calibration fail, write back to default values!\n");
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4prv_register_07c);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4prv_register_0e4);
    }
	else
	{
		if (ucbest_step >= 8)
		{
			u4value = u4prv_register_0e4;
			mcSET_BIT(u4value, POS_PADCTL4_DATLAT3);
			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);
		}
		else
		{
			u4value = u4prv_register_0e4;
			mcCLR_BIT(u4value, POS_PADCTL4_DATLAT3);
			ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PADCTL4), u4value);
		}
		u4value = u4prv_register_07c;
		mcSET_FIELD(u4value, (ucbest_step&0x7), MASK_DDR2CTL_DATLAT, POS_DDR2CTL_DTALAT);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DDR2CTL), u4value);

	}

    

    // Log Example
/*
==============================================================
    DATLAT calibration
    channel=2(2:cha, 3:chb)    apply = 1
==============================================================
DATLAT Default value = 0xe
TAP= 0, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 1, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 2, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 3, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 4, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 5, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 6, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 7, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 8, err_value=0xffffffff, begin=0, first=255, sum=0
TAP= 9, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=10, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=11, err_value=0xffffffff, begin=0, first=255, sum=0
TAP=12, err_value=0x       0, begin=1, first= 12, sum=1
TAP=13, err_value=0x       0, begin=1, first= 12, sum=2
TAP=14, err_value=0x       0, begin=1, first= 12, sum=3
TAP=15, err_value=0x       0, begin=1, first= 12, sum=4
pattern=1(0: ISI, 1: AUDIO, 2: TA4, 3: TA4-3) first_step=12 total pass=4 best_step=13
*/

	return DRAM_OK;

}
#endif

#ifdef DRAM_TX_WINDOW_PERBIT_CAL

#define fcTX_TEST_PATTERN 0   // 0: audio, 1: TE1, 2: XTALK
#define fcTX_PERBIT_DQM 1
#define fcTX_PERBIT_DQ 1

#define fcTX_PI_OFFSET -2   // based on LGE window data
//#define fcTX_PI_OFFSET -3   // based on waveform measurement

#define DEFAULT_FIRST_DQDLY_L_PASS	-15
#define DEFAULT_LAST_DQDLY_L_PASS 	-16
#define DEFAULT_FIRST_DQDLY_R_PASS  -15
#define DEFAULT_LAST_DQDLY_R_PASS	-16

void DramcTxSetDqDelayCell(U8 *pdq_dly)
{
    U32 u4value, u4temp;

       
        //bit 0~3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x004));
        u4temp = ((U32)pdq_dly[3]) |((U32)pdq_dly[2]<<4) | ((U32)pdq_dly[1]<<8) |((U32)pdq_dly[0]<<12);
        mcSET_FIELD(u4value, u4temp, 0x0000ffff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x004), u4value);
        //bit 4~11
        u4value = ((U32)pdq_dly[11]) | ((U32)pdq_dly[10]<<4) | ((U32)pdq_dly[9]<<8) |((U32)pdq_dly[8]<<12) | \
                        ((U32)pdq_dly[7]<<16) |((U32)pdq_dly[6]<<20) | ((U32)pdq_dly[5]<<24) |((U32)pdq_dly[4]<<28);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x008), u4value);
        //bit 12~15
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x00c));
        u4temp = ((U32)pdq_dly[15]<<16) |((U32)pdq_dly[14]<<20) | ((U32)pdq_dly[13]<<24) |((U32)pdq_dly[12]<<28);
        mcSET_FIELD(u4value, u4temp, 0xffff0000, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x00c), u4value);
        //bit 16~19
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x204));
        u4temp = ((U32)pdq_dly[19]) |((U32)pdq_dly[18]<<4) | ((U32)pdq_dly[17]<<8) |((U32)pdq_dly[16]<<12);
        mcSET_FIELD(u4value, u4temp, 0x0000ffff, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x204), u4value);
        //bit 20~27
        u4value = ((U32)pdq_dly[27])|((U32)pdq_dly[26]<<4) | ((U32)pdq_dly[25]<<8) |((U32)pdq_dly[24]<<12) | \
                        ((U32)pdq_dly[23]<<16) |((U32)pdq_dly[22]<<20) | ((U32)pdq_dly[21]<<24) |((U32)pdq_dly[20]<<28);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x208), u4value);
        //bit 28~31
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x20c));
        u4temp = ((U32)pdq_dly[31]<<16) |((U32)pdq_dly[30]<<20) | ((U32)pdq_dly[29]<<24) |((U32)pdq_dly[28]<<28);
        mcSET_FIELD(u4value, u4temp, 0xffff0000, 0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x20c), u4value);

}

void DramcTxSetDqmDelayCell(U8 *pdqm_dly)
{
    U32 u4value;


        //DQM0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x028));
        mcSET_FIELD(u4value, pdqm_dly[0], 0xf0000000, 28);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x028), u4value);
        //DQM1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x02c));
        mcSET_FIELD(u4value, pdqm_dly[1], 0x0000f000, 12);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x02c), u4value);
        //DQM2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x228));
        mcSET_FIELD(u4value, pdqm_dly[2], 0xf0000000, 28);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x228), u4value);
        //DQM3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x22c));
        mcSET_FIELD(u4value, pdqm_dly[3], 0x0000f000, 12);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x22c), u4value);

    
}

#if fcTX_TEST_PATTERN == 2
U32 DramcXtalkEngine(void)
{
U32 u4Val; 
U32 u4Err = 0xffffffff;

    //u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3));
    //mcSET_FIELD0(u4Val, 0xf, 0x0000000f, 0);
    //mcCLR_BIT(u4Val,7);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_3), u4Val);

    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4));
    mcSET_BIT(u4Val,16);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TEST2_4), u4Val);

    // enable				
    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcSET_FIELD(u4Val, (U32) 4, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);

    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
    while(mcCHK_BIT1(u4Val, POS_TESTRPT_DM_CMP_CPT) == 0)
    {
        u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        mcDELAY_us(1);
    }

    // disable write
    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcSET_FIELD(u4Val, (U32) 0, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);

    // enable read
    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcSET_FIELD(u4Val, (U32) 2, MASK_CONF2_TE12_ENABLE, POS_CONF2_TEST1);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);

    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
    while(mcCHK_BIT1(u4Val, POS_TESTRPT_DM_CMP_CPT) == 0)
    {
        u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_TESTRPT));
        mcDELAY_us(1);
    }

    mcDELAY_us(1);

    u4Err = 0;
    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CMP_ERR));
    u4Err |= u4Val;
    //mcSHOW_DBG_MSG("XtalkErr=%8x\n", u4Err);

    // disable read
    u4Val = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2));
    mcCLR_MASK(u4Val, MASK_CONF2_TE12_ENABLE);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_CONF2), u4Val);   

    return u4Err;
					
}
#endif

//-------------------------------------------------------------------------
/** DramcTxWindowPerbitCal (v3)
 *  TX DQS per bit SW calibration.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param  apply           (U8): 0 don't apply the register we set  1 apply the register we set ,default don't apply.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
#ifdef DRAM_TX_VREF_CAL
U32 u4tx_bitwindowsum_perdram[2];
#endif

DRAM_STATUS_T DramcTxWindowPerbitCal(U8 only_get_bitwindowsum)
{
    U8 ucpass_bit, iwrap, jj, ucindex;
    S8 ii,s1pi_dq_delay[DQS_NUMBER];
    U32 u4value, u4err_value = 0xffffffff, u4fail_bit;
    TXDQS_PERBIT_DLY_T dqdqs_perbit_dly[DQ_DATA_WIDTH];   
    S16 s2sum_dly;
    S8 s1temp1, s1temp2;
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;
	U8 fgfail = DRAM_CALIBRATION_PASS;
#ifdef DRAM_TX_VREF_CAL
	S8 dram_no;
#endif
    U16 u2win_size;
    U8  ucdly_temp[DQ_DATA_WIDTH];
	S8 s1min_pi_dqdly_center[DQS_NUMBER];
#ifdef DRAM_TX_PERBIT_DQM_DESKEW
    S8 dqm_dly_r[DQS_NUMBER], dqm_dly_l[DQS_NUMBER];
	U8 ucwin_size[DQS_NUMBER], dqm_dly_done[DQS_NUMBER], ucdly_dqdqm_offset[DQS_NUMBER], ucfail_cnt;
#endif    //A.set RX DQ/DQS in the middle of the pass region from read DQ/DQS calibration
	S8 ucmax_leftbound_byte[DQS_NUMBER];
	S8 ucmin_rightbound_byte[DQS_NUMBER];
	U8 ucdefault_first_dqdly_lr_pass;
    //B.Fix DQS (RG_PI_**_PBYTE*) at degree from write leveling. 
    //   Move DQ (per byte) gradually from 90 to -45 degree to find the left boundary
    //   Move DQ (per byte) gradually from 90 to 225 degree to find the right boundary
    //C.For each DQ delay in step B, start engine test
    //D.After engine test, read per bit results from registers.
    //E.Set RG_PI_**_DQ* to lie in the average of the middle of the pass region in the same byte
    
    // set DQ's delay cell to 0
    for (jj=0; jj<DQ_DATA_WIDTH; jj++)
    {
        ucdly_temp[jj] = 0;        
    }
    // set to registers
    DramcTxSetDqDelayCell(ucdly_temp); 

    // set DQM delay cell to 0
    for (jj=0; jj<DQS_NUMBER; jj++)
    {
            ucdly_temp[jj] = 0;              
    }    
    // set to registers
    DramcTxSetDqmDelayCell(ucdly_temp);

        //-----------------------------------------------
        //set per byte DQS delay to 0x0: RG_*_TX_*RDQS*_DLY
        //A1 DQS0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x01c));
        u4value = u4value & 0x0fffffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), u4value);

        //A1 DQS1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x020));
        u4value = u4value & 0xffff0fff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), u4value);

        //A2 DQS2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x21c));
        u4value = u4value & 0x0fffffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), u4value);

        //A2 DQS3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x220));
        u4value = u4value & 0xffff0fff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), u4value);        

    

    // initialize parameters
    for (ii = 0; ii < DQ_DATA_WIDTH; ii++)
    {
        dqdqs_perbit_dly[(U8)ii].first_dqdly_l_pass = DEFAULT_FIRST_DQDLY_L_PASS;
        dqdqs_perbit_dly[(U8)ii].last_dqdly_l_pass = DEFAULT_LAST_DQDLY_L_PASS;
        dqdqs_perbit_dly[(U8)ii].first_dqdly_r_pass = DEFAULT_FIRST_DQDLY_R_PASS;
        dqdqs_perbit_dly[(U8)ii].last_dqdly_r_pass = DEFAULT_LAST_DQDLY_R_PASS;
    }
	ucdefault_first_dqdly_lr_pass=(IS_DRAM_CHANNELB_ACTIVE()?DEFAULT_PI_B1_DQA:DEFAULT_PI_A1_DQA);

    ucpass_bit = 0;

    //Move DQ (total 0x00~0x3F) down from 0x10 (default: 90 degree) to 0x38 (-45 degree) to find the left boundary
    for (ii = ucdefault_first_dqdly_lr_pass; ii >= -8; ii--)
    {
        // double check if compiler have "%" operation
        //iwrap = ii % 64;
        if (ii >= 0)
        {
            iwrap = ii;
        }
        else
        {
            iwrap = ii + 64;
        }


            //A1 byte A&B
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
            mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
            mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);

            //A2 byte A
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
            mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

            //A2 byte B
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
            mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);           		

#if fcTX_TEST_PATTERN == 0   // audio
        // enable TE2, audio pattern
        u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2); 
#elif fcTX_TEST_PATTERN == 1   // TE1
        // test engine1 offset must be 0x7ff
        u4err_value = DramcEngine1(test2_0, test2_1, 0x000007ff, 0, 0);
        //mcSHOW_DBG_MSG("PI DQ delay: %4d    err_value: %8x\n", ii, u4err_value);
        u4temp = u4err_value & 0x00004000;
        if (u4temp == 0)
        {
            u4err_value = 0;
        }
        else
        {
            u4err_value = 0xffffffff;
        } 
#else   // XTALK
        u4err_value = DramcXtalkEngine();
        //mcSHOW_DBG_MSG("Using XTALK pattern to calibrate TX window. %8x\n", u4err_value);
#endif

        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < DQ_DATA_WIDTH; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqdly_l_pass == DEFAULT_FIRST_DQDLY_L_PASS)
                {
                    // first DQ pass delay tap
                    dqdqs_perbit_dly[jj].first_dqdly_l_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqdly_l_pass == DEFAULT_LAST_DQDLY_L_PASS)
                {
                    if (ii == (-8))
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqdly_l_pass = ii;
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqdly_l_pass != DEFAULT_FIRST_DQDLY_L_PASS)&&(dqdqs_perbit_dly[jj].last_dqdly_l_pass == DEFAULT_LAST_DQDLY_L_PASS))
                {
                    dqdqs_perbit_dly[jj].last_dqdly_l_pass = ii + 1;
                    //may early break for applictions
                    ucpass_bit++;
                }
            }            
        }
        //early break if every bit is ok to find boundary
        if (ucpass_bit == DQ_DATA_WIDTH)
        {
			if(!only_get_bitwindowsum)
				mcSHOW_DBG_MSG("TX calibration finding left boundary early break. PI DQ delay=0x%2x\n", iwrap);
            break;
        }
    }

    ucpass_bit = 0;
    //Move DQ (total 0x00~0x3F) up from 0x10 (default: 90 degree) to 0x28 (225 degree) to find the right boundary
    for (ii = ucdefault_first_dqdly_lr_pass; ii <= 40; ii++)
    {

            //A1 byte A&B
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
            mcSET_FIELD(u4value, ii, 0x00003f00, 8);
            mcSET_FIELD(u4value, ii, 0x3f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);

            //A2 byte A
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
            mcSET_FIELD(u4value, ii, 0x00003f00, 8);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

            //A2 byte B
            u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
            mcSET_FIELD(u4value, ii, 0x3f000000, 24);
            ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);           		

        

#if fcTX_TEST_PATTERN == 0   // audio
        // enable TE2, audio pattern
        u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);                        
#elif fcTX_TEST_PATTERN == 1   // TE1
        // test engine1 offset must be 0x7ff
        u4err_value = DramcEngine1(test2_0, test2_1, 0x000007ff, 0, 0);
        //mcSHOW_DBG_MSG("PI DQ delay: %4d    err_value: %8x\n", ii, u4err_value);
        u4temp = u4err_value & 0x00004000;
        if (u4temp == 0)
        {
            u4err_value = 0;
        }
        else
        {
            u4err_value = 0xffffffff;
        } 
#else   // XTALK
        u4err_value = DramcXtalkEngine();
        //mcSHOW_DBG_MSG("Using XTALK pattern to calibrate TX window. %8x\n", u4err_value);
#endif
        // check fail bit ,0 ok ,others fail
        for (jj = 0; jj < DQ_DATA_WIDTH; jj++)
        {
            u4fail_bit = u4err_value&((U32)1<<jj);

            if (u4fail_bit == 0)
            {
                if (dqdqs_perbit_dly[jj].first_dqdly_r_pass == DEFAULT_FIRST_DQDLY_R_PASS)
                {
                    // first DQ pass delay tap
                    dqdqs_perbit_dly[jj].first_dqdly_r_pass = ii;
                }
                if (dqdqs_perbit_dly[jj].last_dqdly_r_pass == DEFAULT_LAST_DQDLY_R_PASS)
                {
                    if (ii == 40)
                    {
                        // pass to the last tap
                        dqdqs_perbit_dly[jj].last_dqdly_r_pass = ii;
                    }
                }
            }
            else
            {
                if ((dqdqs_perbit_dly[jj].first_dqdly_r_pass != DEFAULT_FIRST_DQDLY_R_PASS)&&(dqdqs_perbit_dly[jj].last_dqdly_r_pass == DEFAULT_LAST_DQDLY_R_PASS))
                {
                    dqdqs_perbit_dly[jj].last_dqdly_r_pass = ii - 1;
                    //may early break for applictions
                    ucpass_bit++;
                }
            }            
        }
        //early break if every bit is ok to find boundary
        if (ucpass_bit == DQ_DATA_WIDTH)
        {
			if(!only_get_bitwindowsum)
				mcSHOW_DBG_MSG("TX calibration finding right boundary early break. PI DQ delay=0x%2x\n", ii);
            break;
        }
    }

    //Calculate the center of DQ pass window
    //average the center delay
    for (ii=0; ii<DQS_NUMBER; ii++)
    {
		s2sum_dly = 0;
        u2win_size = 0;
        s1min_pi_dqdly_center[(U8)ii] = 127; // init to max number
        for (jj=0; jj<DQS_BIT_NUMBER; jj++)
        {
            ucindex = ii * DQS_BIT_NUMBER + jj;
            dqdqs_perbit_dly[ucindex].pi_dqdly_ok_center = (dqdqs_perbit_dly[ucindex].last_dqdly_l_pass + dqdqs_perbit_dly[ucindex].last_dqdly_r_pass) / 2;
            s2sum_dly = s2sum_dly + dqdqs_perbit_dly[ucindex].pi_dqdly_ok_center;
            u2win_size = u2win_size + (dqdqs_perbit_dly[ucindex].last_dqdly_r_pass - dqdqs_perbit_dly[ucindex].last_dqdly_l_pass + 1);
            //mcSHOW_DBG_MSG("jj=%d, win_size=%d, %d, %d\n", jj, u2win_size, dqdqs_perbit_dly[ucindex].last_dqdly_r_pass, dqdqs_perbit_dly[ucindex].last_dqdly_l_pass);

            // find min center
            if (dqdqs_perbit_dly[ucindex].pi_dqdly_ok_center < s1min_pi_dqdly_center[(U8)ii])
            {
                s1min_pi_dqdly_center[(U8)ii] = dqdqs_perbit_dly[ucindex].pi_dqdly_ok_center;
            }            
        }
        // check if SoC compiler support "/" operation?
        // 'round' operation
        s1temp1 = (S8) (s2sum_dly/DQS_BIT_NUMBER);
        s1temp2 = s1temp1+1;
        if ((s2sum_dly-s1temp1*DQS_BIT_NUMBER) > (s1temp2*DQS_BIT_NUMBER-s2sum_dly))
        {
            s1pi_dq_delay[(U8)ii] = s1temp2;
        }
        else
        {
            s1pi_dq_delay[(U8)ii] = s1temp1;
        }

        //wrap to write to registers
        // check if SoC compiler support "%" operation?
        //s1pi_dq_delay[ii] = s1pi_dq_delay[ii]%64;
        if (s1pi_dq_delay[(U8)ii] < 0)
        {
            s1pi_dq_delay[(U8)ii] = s1pi_dq_delay[(U8)ii] + 64;
        }
		
#ifdef DRAM_TX_PERBIT_DQM_DESKEW
        // calculate average window size
        ucwin_size[ii] = (U8) (u2win_size/DQS_BIT_NUMBER);
        //mcSHOW_DBG_MSG("ii=%d, win_size=%d\n", ii, ucwin_size[ii]);
#endif
    }

	if(!only_get_bitwindowsum)
	{
		mcSHOW_DBG_MSG("==================================================================\n");
	    mcSHOW_DBG_MSG("    TX DQS perbit delay software calibration v3 \n"); 
	    mcSHOW_DBG_MSG("==================================================================\n");
	    mcSHOW_DBG_MSG("PI DQ (per byte) window\nx=pass dq delay value (min~max)center \ny=0-7bit DQ of every group\n");
#ifdef DRAM_TX_PERBIT_DQ_DESKEW
	    mcSHOW_DBG_MSG("input delay (per bit de-skew):\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", s1min_pi_dqdly_center[0], s1min_pi_dqdly_center[1], s1min_pi_dqdly_center[2], s1min_pi_dqdly_center[3]);
#else 
		mcSHOW_DBG_MSG("input delay (w/o per bit de-skew):\n Byte0 = %d\n Byte1 = %d\n Byte2 = %d\n Byte3 = %d\n", s1pi_dq_delay[0], s1pi_dq_delay[1], s1pi_dq_delay[2], s1pi_dq_delay[3]);
#endif
	    mcSHOW_DBG_MSG("TX PI OFFSET : %d\n", fcTX_PI_OFFSET);
	    mcSHOW_DBG_MSG("==================================================================\n");
	    mcSHOW_DBG_MSG("bit window center \n");	
	}
    for (ii = 0; ii < 32; ii++)
    {
		if(!only_get_bitwindowsum)
			mcSHOW_DBG_MSG("%2d   (%2d~%2d) %2d\n", \
            	ii, dqdqs_perbit_dly[ii].last_dqdly_l_pass, dqdqs_perbit_dly[ii].last_dqdly_r_pass, dqdqs_perbit_dly[ii].pi_dqdly_ok_center);
	}
	for (ii = 0; ii < 32; ii++)
	{
		if((dqdqs_perbit_dly[(U8)ii].first_dqdly_l_pass!=ucdefault_first_dqdly_lr_pass) || (dqdqs_perbit_dly[(U8)ii].first_dqdly_r_pass!=ucdefault_first_dqdly_lr_pass))
		{
			mcSHOW_ERROR_CHIP_DisplayString("default tx dq PI delay value(");
			mcSHOW_ERROR_CHIP_DisplayInteger(ucdefault_first_dqdly_lr_pass);
			mcSHOW_ERROR_CHIP_DisplayString(") is fail on bit ");
			mcSHOW_ERROR_CHIP_DisplayInteger(ii);
			mcSHOW_ERROR_CHIP_DisplayString("!!\n");
			fgfail = DRAM_CALIBRATION_FAIL;
		}
	}
	if(!only_get_bitwindowsum)
		mcSHOW_DBG_MSG("\n==================================================================\n");

#ifdef DRAM_TX_VREF_CAL
	for(dram_no=0;dram_no<2;dram_no++)
	{
		u4tx_bitwindowsum_perdram[dram_no] = 0;
		for(ii=0;ii<16;ii++)
			if(((dqdqs_perbit_dly[ii].first_dqdly_l_pass!=ucdefault_first_dqdly_lr_pass)\
				|| (dqdqs_perbit_dly[ii].first_dqdly_r_pass!=ucdefault_first_dqdly_lr_pass))\
				&&((dqdqs_perbit_dly[ii].first_dqdly_r_pass-dqdqs_perbit_dly[ii].first_dqdly_l_pass+1)<12))//bit calibration fail or bitwindow size<12
			{
				u4tx_bitwindowsum_perdram[dram_no] = 0;
				break;
			}
			else
				u4tx_bitwindowsum_perdram[dram_no] += (dqdqs_perbit_dly[ii+16*dram_no].last_dqdly_r_pass - dqdqs_perbit_dly[ii+16*dram_no].last_dqdly_l_pass +1);
	}

	if(only_get_bitwindowsum)
	{
		return DRAM_OK;
	}
#endif

#ifdef DRAM_TX_PERBIT_DQM_DESKEW
		//============================================================
		// DQM calibration
		// 1. Delay all DQ's using delay cell by average win_size/2 to make DQ's behind to DQM
		// transfer PI to delay cell
		for (ii=0; ii<DQS_NUMBER; ii++)
		{
			//mcSHOW_DBG_MSG("1. ii=%d, win_size=%d\n", ii, ucwin_size[ii]);
			ucdly_dqdqm_offset[ii] = (U8) ((TCMGET_DLYCELL_PERT*(U16)(ucwin_size[ii]/2))/64);
			if (ucdly_dqdqm_offset[ii] > 15)
			{
				ucdly_dqdqm_offset[ii] = 15;
			}
			mcSHOW_DBG_MSG("DQS:%d, win_size=%2d, dqdqm delay cell offset=%2d\n", ii, ucwin_size[ii], ucdly_dqdqm_offset[ii]);
			
			ucdly_temp[DQS_BIT_NUMBER*ii] = ucdly_dqdqm_offset[ii];
			
			for (jj=1; jj < DQS_BIT_NUMBER; jj++)
			{
				ucdly_temp[DQS_BIT_NUMBER*ii+jj] = ucdly_temp[DQS_BIT_NUMBER*ii];
			}
		}
		//set to registers
		// DQ delay cell
		DramcTxSetDqDelayCell(ucdly_temp);
	
		for (jj=0; jj<DQS_NUMBER; jj++)
		{
			ucdly_temp[jj] = 0; 	   
		}
		DramcTxSetDqmDelayCell(ucdly_temp);
	
		// 2. Adjust DQ/DQM PI to find DQM right boundary
		// for early break
		ucfail_cnt = 0;
		for (jj = 0; jj < DQS_NUMBER; jj++)
		{
			dqm_dly_done[jj] = 0;
		}
		
		for (ii = -8; ii <= 32; ii++)
		{
			// double check if compiler have "%" operation
			//iwrap = ii % 64;
			if (ii >= 0)
			{
				iwrap = ii;
			}
			else
			{
				iwrap = ii + 64;
			}
	
			
				//A1 byte A&B
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
				mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
				mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);
	
				//A2 byte A
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
				mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);
	
				//A2 byte B
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
				mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);					
			
			
#if fcTX_TEST_PATTERN == 0   // audio
			// enable TE2, audio pattern
			u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2); 
#elif fcTX_TEST_PATTERN == 1   // TE1
			// test engine1 offset must be 0x7ff
			u4err_value = DramcEngine1(test2_0, test2_1, 0x000007ff, 0, 0);
			u4temp = u4err_value & 0x00004000;
			if (u4temp == 0)
			{
				u4err_value = 0;
			}
			else
			{
				u4err_value = 0xffffffff;
			}
#else   // XTALK
			u4err_value = DramcXtalkEngine();
			//mcSHOW_DBG_MSG("Using XTALK pattern to calibrate TX window. %8x\n", u4err_value);
#endif        
			mcSHOW_DBG_MSG2("PI DQ delay: %4d	 err_value: %8x\n", ii, u4err_value);
			
			for (jj=0; jj<DQS_NUMBER; jj++)
			{
				u4fail_bit = u4err_value&((U32)0xff<<(jj*DQS_BIT_NUMBER));
	
				if ((u4fail_bit == 0) && (dqm_dly_done[jj] == 0))
				{
					dqm_dly_r[jj] = ii;
					dqm_dly_done[jj] = 1;
					ucfail_cnt++;
				}
			}
	
			if (ucfail_cnt == DQS_NUMBER)
			{
				// early break
				break;
			}		 
		}
	
		if (ucfail_cnt == DQS_NUMBER)
		{
			mcSHOW_DBG_MSG("DQM delay right boundary: %3d : %3d : %3d : %3d\n", dqm_dly_r[0], dqm_dly_r[1],dqm_dly_r[2],dqm_dly_r[3]);
		}
		else
		{
			mcSHOW_ERROR_CHIP_DisplayString("error: cannot find right boundary of DQM\n");
		}
	
		// 3. set DQ's delay cell to 0; set DQM using delay cell by average win_size/2 to make DQM behind to DQ's
		for (jj=0; jj<DQ_DATA_WIDTH; jj++)
		{
			ucdly_temp[jj] = 0;
		}
		DramcTxSetDqDelayCell(ucdly_temp);
	
		for (jj=0; jj<DQS_NUMBER; jj++)
		{
			ucdly_temp[jj] = ucdly_dqdqm_offset[jj];		
		}
		DramcTxSetDqmDelayCell(ucdly_temp);
	
		// 4. Adjust DQ/DQM PI to find DQM left boundary
		// for early break
		ucfail_cnt = 0;
		for (jj = 0; jj < DQS_NUMBER; jj++)
		{
			dqm_dly_done[jj] = 0;
		}
		
		for (ii = 40; ii >= 0; ii--)
		{
			// double check if compiler have "%" operation
			//iwrap = ii % 64;
			if (ii >= 0)
			{
				iwrap = ii;
			}
			else
			{
				iwrap = ii + 64;
			}
	
			
				//A1 byte A&B
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
				mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
				mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);
	
				//A2 byte A
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
				mcSET_FIELD(u4value, iwrap, 0x00003f00, 8);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);
	
				//A2 byte B
				u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
				mcSET_FIELD(u4value, iwrap, 0x3f000000, 24);
				ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);					
			
			
#if fcTX_TEST_PATTERN == 0   // audio
			// enable TE2, audio pattern
			u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2); 
#elif fcTX_TEST_PATTERN == 1   // TE1
			// test engine1 offset must be 0x7ff
			u4err_value = DramcEngine1(test2_0, test2_1, 0x000007ff, 0, 0);
			u4temp = u4err_value & 0x00004000;
			if (u4temp == 0)
			{
				u4err_value = 0;
			}
			else
			{
				u4err_value = 0xffffffff;
			}	   
#else   // XTALK
			u4err_value = DramcXtalkEngine();
			//mcSHOW_DBG_MSG("Using XTALK pattern to calibrate TX window. %8x\n", u4err_value);
#endif        
			mcSHOW_DBG_MSG2("PI DQ delay: %4d	 err_value: %8x\n", ii, u4err_value);
			
			for (jj=0; jj<DQS_NUMBER; jj++)
			{
				u4fail_bit = u4err_value&((U32)0xff<<(jj*DQS_BIT_NUMBER));
	
				if ((u4fail_bit == 0) && (dqm_dly_done[jj] == 0))
				{
					// add offset to compensate delay cell
					dqm_dly_l[jj] = ii + (ucwin_size[jj]/2);
					dqm_dly_done[jj] = 1;
					ucfail_cnt++;
				}
			}
	
			if (ucfail_cnt == DQS_NUMBER)
			{
				// early break
				break;
			}		 
		}
	
		if (ucfail_cnt == DQS_NUMBER)
		{
			mcSHOW_DBG_MSG("DQM delay left boundary: %3d : %3d : %3d : %3d\n", dqm_dly_l[0], dqm_dly_l[1],dqm_dly_l[2],dqm_dly_l[3]);
			mcSHOW_DBG_MSG2("DQM offset: %3d : %3d : %3d : %3d\n", ucwin_size[0]/2, ucwin_size[1]/2, ucwin_size[2]/2, ucwin_size[3]/2);
		}
		else
		{
			mcSHOW_ERROR_CHIP_DisplayString("error: cannot find left boundary of DQM\n");
		}
	
		// 4. DQ/DQM deskew
		// DQM de-skew
		for (jj=0; jj<DQS_NUMBER; jj++)
		{
			s1temp1 = ((dqm_dly_l[jj]+dqm_dly_r[jj])/2);
#ifdef DRAM_TX_PERBIT_DQ_DESKEW
			s1temp2 = s1min_pi_dqdly_center[jj];
#else
			s1temp2 = s1pi_dq_delay[jj];
#endif
			// not sure min or average center to compare?!
			if (s1temp1 < s1temp2)
			{
				ucdly_temp[jj] = 0;
			}
			else
			{
				ucdly_temp[jj] = s1temp1 - s1temp2;
			}
			mcSHOW_DBG_MSG("DQM:%d de-skew: PI -> %d; ", jj, ucdly_temp[jj]);
			// change PI delay to delay cells
			ucdly_temp[jj] = (U8) ((TCMGET_DLYCELL_PERT*(U16)ucdly_temp[jj])/64) + 1; // add 1 for favor more delay of DQM
			mcSHOW_DBG_MSG("Delay cell -> %d\n", ucdly_temp[jj]);
		}
		// set to registers
		DramcTxSetDqmDelayCell(ucdly_temp);    
		
		//============================================================
#else
		for (jj=0; jj<DQS_NUMBER; jj++)
		{
			ucdly_temp[jj] = 0;  
			mcSHOW_DBG_MSG("DQM:%d w/o de-skew: delay cell -> %d\n", jj, ucdly_temp[jj]);
		}	 
		// set to registers
		DramcTxSetDqmDelayCell(ucdly_temp);  
#endif
	
	//============================================================
#ifdef DRAM_TX_PERBIT_DQ_DESKEW
		// DQ de-skew
		for (jj=0; jj<DQ_DATA_WIDTH; jj++)
		{
			ucindex = jj /DQS_BIT_NUMBER;
			ucdly_temp[jj] = dqdqs_perbit_dly[jj].pi_dqdly_ok_center - s1min_pi_dqdly_center[ucindex];
			mcSHOW_DBG_MSG("DQ:%2d de-skew: PI -> %d; ", jj, ucdly_temp[jj]);
			// change PI delay to delay cells
			ucdly_temp[jj] = (U8) ((TCMGET_DLYCELL_PERT*(U16)ucdly_temp[jj])/64);
			mcSHOW_DBG_MSG("Delay cell -> %d\n", ucdly_temp[jj]);
		}
		// set to registers
		DramcTxSetDqDelayCell(ucdly_temp); 
#else
		for (jj=0; jj<DQ_DATA_WIDTH; jj++)
		{
			ucdly_temp[jj] = 0;
			mcSHOW_DBG_MSG("DQ:%2d w/o de-skew: delay cell -> %d\n", jj, ucdly_temp[jj]);
		}
		// set to registers
		DramcTxSetDqDelayCell(ucdly_temp); 
#endif
	//============================================================
	
		//set to registers
		
			//PI DQ (per byte)
			//A1 byte A&B
			u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
#ifndef DRAM_TX_PERBIT_DQ_DESKEW // if TX per bit de-skew, PI set to min
			mcSET_FIELD(u4value, s1pi_dq_delay[1]+fcTX_PI_OFFSET, 0x00003f00, 8);
			mcSET_FIELD(u4value, s1pi_dq_delay[0]+fcTX_PI_OFFSET, 0x3f000000, 24);
#else
			mcSET_FIELD(u4value, s1min_pi_dqdly_center[1]+fcTX_PI_OFFSET, 0x00003f00, 8);
			mcSET_FIELD(u4value, s1min_pi_dqdly_center[0]+fcTX_PI_OFFSET, 0x3f000000, 24);
#endif
			ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);
	
			//A2 byte A
			u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
#ifndef DRAM_TX_PERBIT_DQ_DESKEW // if TX per bit de-skew, PI set to min
			mcSET_FIELD(u4value, s1pi_dq_delay[2]+fcTX_PI_OFFSET, 0x00003f00, 8);
#else
			mcSET_FIELD(u4value, s1min_pi_dqdly_center[2]+fcTX_PI_OFFSET, 0x00003f00, 8);
#endif
			ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);
	
			//A2 byte B
			u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
#ifndef DRAM_TX_PERBIT_DQ_DESKEW // if TX per bit de-skew, PI set to min
			mcSET_FIELD(u4value, s1pi_dq_delay[3]+fcTX_PI_OFFSET, 0x3f000000, 24);
#else
			mcSET_FIELD(u4value, s1min_pi_dqdly_center[3]+fcTX_PI_OFFSET, 0x3f000000, 24);
#endif
			ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);	
		
		

#if 1
    for (ii = 0; ii < DQS_NUMBER; ii++)
    {
		ucmax_leftbound_byte[(U8)ii]=-16;
		ucmin_rightbound_byte[(U8)ii]=32;

		//left
		for (jj = 8*ii; jj <= 8*ii+7; jj++)
	    {
			if (dqdqs_perbit_dly[jj].last_dqdly_l_pass > ucmax_leftbound_byte[(U8)ii])
			{
			   ucmax_leftbound_byte[(U8)ii] = dqdqs_perbit_dly[jj].last_dqdly_l_pass;
			}
	    }
		//right
        for (jj = 8*ii; jj <= 8*ii+7; jj++)
        {
			if (dqdqs_perbit_dly[jj].last_dqdly_r_pass < ucmin_rightbound_byte[(U8)ii])
			{
			  ucmin_rightbound_byte[(U8)ii] = dqdqs_perbit_dly[jj].last_dqdly_r_pass;
			}
        }
    }

    for (ii = 0; ii < DQS_NUMBER; ii++)
    {
       CHIP_DisplayString("TX HW Byte ");
	   CHIP_DisplayInteger(ii);
	   CHIP_DisplayString(" : DQS(");
	   CHIP_DisplayInteger(ucmax_leftbound_byte[ii]);
	   CHIP_DisplayString(" ~ ");
	   CHIP_DisplayInteger(ucmin_rightbound_byte[ii]);
	   CHIP_DisplayString("), Size ");
	   CHIP_DisplayInteger((ucmin_rightbound_byte[ii] - ucmax_leftbound_byte[ii] + 1));
	   CHIP_DisplayString(", Set ");
	   CHIP_DisplayInteger(s1pi_dq_delay[ii]);
	   CHIP_DisplayString(". \n");
    }
#endif

	if(fgfail == DRAM_CALIBRATION_FAIL)
	{
		mcSHOW_ERROR_CHIP_DisplayString("TX DQ/DQS calibration fail!\n");
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_TX_DQS_CALIBRATION,\
								FLAG_CALIBRATION_FAIL,\
								FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif

		return DRAM_FAIL;
	}


	for (ii = 0; ii < DQS_NUMBER; ii++)
    {
#if defined(DRAM_LOAD_BOARD) || defined(__MODEL_slt__)
		if((ucmin_rightbound_byte[ii]-ucmax_leftbound_byte[ii]+1)<16)//tx window size<0.5UI
#else
		if((ucmin_rightbound_byte[(U8)ii]-ucmax_leftbound_byte[(U8)ii]+1)<12)//tx window size<0.375UI
#endif
		{
			mcSHOW_ERROR_CHIP_DisplayString("TX calibration byte ");
			mcSHOW_ERROR_CHIP_DisplayInteger(ii);
			mcSHOW_ERROR_CHIP_DisplayString(" window size is too small!!\n");
			fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL;

		}
		else if((ucmin_rightbound_byte[(U8)ii]-ucmax_leftbound_byte[(U8)ii]+1)>32)//tx window size>UI
		{
			mcSHOW_ERROR_CHIP_DisplayString("TX calibration byte ");
			mcSHOW_ERROR_CHIP_DisplayInteger(ii);
			mcSHOW_ERROR_CHIP_DisplayString(" window size is too big!!\n");
			fgfail = DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG;
		}
    }

	if(fgfail == DRAM_CALIBRATION_WINDOW_SIZE_TOO_SMALL)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_TX_DQS_CALIBRATION,\
							FLAG_WINDOW_TOO_SMALL,\
							FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif

		return DRAM_FAIL;
	}
	else if(fgfail == DRAM_CALIBRATION_WINDOW_SIZE_TOO_BIG)
	{
#ifdef DRAM_LOAD_BOARD
		LoadBoardShowResult(FLAG_TX_DQS_CALIBRATION,\
							FLAG_WINDOW_TOO_BIG,\
							FLAG_NOT_COMPLETE_OR_FAIL);
		while(1);
#endif

#ifdef __MODEL_slt__
		CHIP_DisplayString(" system halt !!!\n");
		// Lock system and wait to check this ic.
		while (1);
#endif

		return DRAM_FAIL;
	}
	

	return DRAM_OK;

    // log example
    /*
TX calibration finding left boundary early break. PI DQ delay=0x3e
TX calibration finding right boundary early break. PI DQ delay=0x1c
==================================================================
    TX DQS perbit delay software calibration v3
    channel=2(2:cha, 3:chb)  apply = 1
==================================================================
PI DQ (per byte) window
x=pass dq delay value (min~max)center
y=0-7bit DQ of every group
input delay:Byte0 = 13 Byte1 = 13 Byte2 = 12 Byte3 = 13
==================================================================
bit    Byte0    bit    Byte1    bit    Byte2    bit    Byte3
 0   ( 1~26)13,  8   ( 3~26)14, 16   ( 2~27)14, 24   ( 3~26)14
 1   ( 2~26)14,  9   ( 1~24)12, 17   ( 0~25)12, 25   ( 4~26)15
 2   ( 3~25)14, 10   ( 2~26)14, 18   ( 2~25)13, 26   ( 3~27)15
 3   ( 2~24)13, 11   ( 1~23)12, 19   ( 3~25)14, 27   ( 1~23)12
 4   ( 3~26)14, 12   ( 2~26)14, 20   ( 0~24)12, 28   ( 1~25)13
 5   ( 3~25)14, 13   ( 2~25)13, 21   (-1~25)12, 29   ( 2~24)13
 6   ( 2~26)14, 14   ( 2~24)13, 22   (-1~26)12, 30   ( 3~27)15
 7   ( 1~25)13, 15   ( 2~26)14, 23   (-1~22)10, 31   ( 2~26)14

==================================================================
   */
}
#endif

#ifdef DRAM_TX_VREF_CAL
void DramcTxVrefCal(void)
{
    U8	s1vref;
    U32	u4value;
    U32	u4addr_array[2];
	U8	dram_no;
	U32 u4tx_vref_bitwindowsum_perdram[16][2]={{0,0}};
	U8	ucbest_vref[2];
	U32	ucbitwindowsum_max_perdram[2]={0};

	mcSHOW_DBG_MSG("==============================================================\n");
    mcSHOW_DBG_MSG("   Tx Vref calibration \n");
    mcSHOW_DBG_MSG("==============================================================\n");
	
	if (!IS_DRAM_CHANNELB_ACTIVE())
	{
        // assign address array
        u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
        u4addr_array[1] = mcSET_PHY_REG_ADDR(0x248);

		//MUX settings for VREF_GEN_SCAN out
        //RG_*_RX_VREF_OUT_SCAN_SEL=1
        //RG_*_RX_VREF_OUT_SEL=1
        //0x1044[2], [9] for A1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
        //0x1244[2], [9] for A2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);
	}
	else
	{
		// assign address array
        u4addr_array[0] = mcSET_PHY_REG_ADDR(0x298);
        u4addr_array[1] = mcSET_PHY_REG_ADDR(0x448);

		//MUX settings for VREF_GEN_SCAN out
        //RG_*_RX_VREF_OUT_SCAN_SEL=1
        //RG_*_RX_VREF_OUT_SEL=1
        //0x1294[2], [9] for B1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x294));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x294), u4value);
        //0x1444[2], [9] for B2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x444));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x444), u4value);
	}

	mcSHOW_DBG_MSG("tx vref | dram No.0 BitWindSum | dram No.1 BitWindowSum\n");
	for (s1vref=6; s1vref<=9; s1vref++)
    {
        //Set Vref voltage
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0)
        ucDram_Register_Write(u4addr_array[0], u4value); 

        u4value = ucDram_Register_Read(u4addr_array[1]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0)
        ucDram_Register_Write(u4addr_array[1], u4value); 

        //Wait for Vref settles down, 1ms is enough
        mcDELAY_us(10);

		DramcTxWindowPerbitCal(1);

		for(dram_no=0;dram_no<2;dram_no++)
		{		
			u4tx_vref_bitwindowsum_perdram[s1vref][dram_no] = u4tx_bitwindowsum_perdram[dram_no];
			if((u4tx_vref_bitwindowsum_perdram[s1vref][dram_no])>=(ucbitwindowsum_max_perdram[dram_no]))
			{
				ucbest_vref[dram_no] = s1vref;
				ucbitwindowsum_max_perdram[dram_no] = u4tx_vref_bitwindowsum_perdram[s1vref][dram_no];
			}
		}
		mcSHOW_DBG_MSG("%d	%d	%d \n", s1vref,  u4tx_vref_bitwindowsum_perdram[s1vref][0], u4tx_vref_bitwindowsum_perdram[s1vref][1]);
	}

	mcSHOW_DBG_MSG("-----Best tx vref for per dram------ \n");
	mcSHOW_DBG_MSG("dram_no   best_tx_vref    bit_window_sum \n");
	for(dram_no=0;dram_no<2;dram_no++) 
	{
		mcSHOW_DBG_MSG("%d   %d   %d \n", dram_no, ucbest_vref[dram_no], ucbitwindowsum_max_perdram[dram_no]);
	}

	//Set Vref voltage
	for(dram_no=0;dram_no<2;dram_no++)
	{
		u4value = ucDram_Register_Read(u4addr_array[dram_no]);
		mcSET_FIELD(u4value, ucbest_vref[dram_no], 0x0000000f, 0);
		ucDram_Register_Write(u4addr_array[dram_no], u4value); 
	}
}
#endif

#ifdef DRAM_TX_OCD_DRV_SCAN
void DramcTxOcdDrvScan(void)
{
	//S8 p_drv, n_drv;
    U8 uccode_map[32] = {	0x00, 0x01, 0x08, 0x10, 0x20, 0x40, 0x80, 0x12, 
							0x24, 0x48, 0x13, 0x19, 0x46, 0x89, 0x17, 0x1e,
							0x4d, 0x35, 0x93, 0x4f, 0x8f, 0x3d, 0xb8, 0x73,
							0xb6, 0xe9, 0xb7, 0xeb, 0xf3, 0xef, 0xfe, 0xff};
	U8 ii;

	mcSHOW_DBG_MSG("==============================================================\n");
    mcSHOW_DBG_MSG("    Tx Ocd Drv Scan \n");
    mcSHOW_DBG_MSG("==============================================================\n");

	for(ii=6;ii<32;ii++)
	{
		
		DramcDqDriving(uccode_map[ii], uccode_map[ii], DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    	DramcDqsDriving(uccode_map[ii], uccode_map[ii], DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    	DramcDqmDriving(uccode_map[ii], uccode_map[ii], DEFAULT_ODTP_DRIVING, DEFAULT_ODTN_DRIVING);
    	DramcClkDriving(uccode_map[ii], uccode_map[ii]);
    	DramcCaDriving(uccode_map[ii], uccode_map[ii]);
		mcSHOW_DBG_MSG("Current Tx OCD Driving code is 0x%02x \n", uccode_map[ii]);
		DramcTxVrefCal();
	}
}
#endif

#ifdef DRAM_RX_EYE_SCAN
//-------------------------------------------------------------------------
/** DramcRxEyeScan
 *  start the rx dq eye scan.
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param dq_no            (U8): 0~31.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcRxEyeScan(U8 dq_no)
{
    U8 ucidx_dqs, ucidx_dq, ucdqs_dly;
    S8 s1vref, s1dq_dly;
    U32 u4value, u4err_value = 0xffffffff, u4addr_array[4], u4sample_cnt, u4error_cnt;
	U32 ii;
	
    if (dq_no <= 15)
    {

            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x04c);


        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT);
        ucidx_dqs =  dq_no / 8;
    }
    else
    {
        
            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x248);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x24c);
        
        

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT_2);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQ_ERR_CNT_2);
        ucidx_dqs =  (dq_no-16) / 8;
    }

    // check if SoC platform has "%" operation?!
    ucidx_dq =  dq_no % 16;
    	
    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN 
    //RG_??_RX_VREF_OP_EN 
    //RG_??_RX_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x0000e000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value | 0xffff0000;
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Disable MIOCK jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=0, RG_RX_MIOCK_JIT_EN=0)
    //RG_??_RX_DQS_MIOCK_SEL=0
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value & 0xffffefff;
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    //select DQ to be scanned (0~15)
    //DDRPHY: RG_??_RX_DQ_EYE_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dq, 0x0f000000, 24)
    ucDram_Register_Write(u4addr_array[0], u4value); 
    //DRAMC: RG_RX_DQ_EYE_SEL (0~15)
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcSET_FIELD(u4value, ucidx_dq, 0x000000f0, 4)
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value); 
 	
    //Select DQS to be scanned (0 or 1)
    //RG_??_RX_DQS_EYE_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dqs, 0x00800000, 23);
    ucDram_Register_Write(u4addr_array[0], u4value); 	

    mcSHOW_DBG_MSG("===============================================================================\n");
    mcSHOW_DBG_MSG("    DQ RX eye scan (channel=%d(2:cha, 3:chb), dq_%d)\n", IS_DRAM_CHANNELB_ACTIVE(), dq_no);
    mcSHOW_DBG_MSG("===============================================================================\n");

    for (s1vref=15; s1vref>=0; s1vref--)
    {
        //Set Vref voltage
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0)
        ucDram_Register_Write(u4addr_array[0], u4value); 

        //Wait for Vref settles down, 1ms is enough
        mcDELAY_us(10);

        //Set DQS delay (RG_??_RX_DQS_EYE_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x007f0000, 16)
        ucDram_Register_Write(u4addr_array[0], u4value);  		
 		
        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        for (s1dq_dly=15; s1dq_dly>=0; s1dq_dly--)
        {
            //Set DQ delay (RG_??_RX_DQ_EYE_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, s1dq_dly, 0x00000f00, 8);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            // enable TE2, audio pattern
            u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        

            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            mcSHOW_DBG_MSG("%4d,", u4error_cnt);         
        }

        //Set DQ delay (RG_??_RX_DQ_EYE_DLY) to 0
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, 0, 0x00000f00, 8);
        ucDram_Register_Write(u4addr_array[0], u4value);

        //Disable DQ eye scan (b'1), for counter clear
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //for eye scan, dqs only need to scan 0~31
        for (ucdqs_dly=0; ucdqs_dly<32; ucdqs_dly++)
        {
            //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
            u4value = ucDram_Register_Read(u4addr_array[0]);
            mcSET_FIELD(u4value, ucdqs_dly, 0x007f0000, 16);
            ucDram_Register_Write(u4addr_array[0], u4value);

            //Reset eye scan counters (reg_sw_rst): 1 to 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
            mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Enable DQ eye scan (b'1)
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //reset phy R_DMPHYRST: 0xf0[28] 
            // 0x0f0[28] = 1 -> 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
            mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
            //delay 10ns, 1ms here
            mcDELAY_us(1);
            mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

            // read data counter reset
            // 0x0f4[25] = 1 -> 0
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
            mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
            //delay 10ns, 1ms here
            mcDELAY_us(1);                
            mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

            // enable TE2, audio pattern
            u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);

            //Disable DQ eye scan (b'1), for counter latch
            u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
            mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
            ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

            //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
            u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
            u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

            mcSHOW_DBG_MSG("%4d,", u4error_cnt);             
        }
        mcSHOW_DBG_MSG("\n");
    }

	//reset phy R_DMPHYRST: 0xf0[28] 
    // 0x0f0[28] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1));
    mcSET_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);
    mcCLR_BIT(u4value, POS_PHYCTL1_PHYRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_PHYCTL1), u4value);

    // read data counter reset
    // 0x0f4[25] = 1 -> 0
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1));
    mcSET_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);
    //delay 10ns, 1ms here
    mcDELAY_us(1);                
    mcCLR_BIT(u4value, POS_GDDR3CTL1_RDATRST);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_GDDR3CTL1), u4value);

	//Set Vref voltage
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 8, 0x0000000f, 0)
    ucDram_Register_Write(u4addr_array[0], u4value);
	//Set DQ delay (RG_??_RX_DQ_EYE_DLY)
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, 0, 0x00000f00, 8);
    ucDram_Register_Write(u4addr_array[0], u4value);
	//set dqs delay
	u4value = ucDram_Register_Read(u4addr_array[0]);
	mcSET_FIELD(u4value, 9, 0x007f0000, 16);
	ucDram_Register_Write(u4addr_array[0], u4value);

	mcSHOW_DBG_MSG("vref, dqs_delay, dq_delay: reg 0x%08x=0x%08x \n", u4addr_array[0], ucDram_Register_Read(u4addr_array[0]));

	for(ii=0;ii<10;ii++)
	{
		//Reset eye scan counters (reg_sw_rst): 1 to 0
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
		mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Enable DQ eye scan (b'1)
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		// enable TE2, audio pattern
		//u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, 0xaa550000, 0x30000000, 0x00000100);                        
		HAL_Delay_us(100);
		//Disable DQ eye scan (b'1), for counter latch
		u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
		mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
		ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

		//Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
		u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
		u4error_cnt = ucDram_Register_Read(u4addr_array[3]);

		mcSHOW_DBG_MSG("%4d,", u4error_cnt);         	
	}

	mcSHOW_DBG_MSG("\n");

	UNUSED(u4err_value);
	UNUSED(u4sample_cnt);
	UNUSED(u4error_cnt);

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	return DRAM_OK;
	
    // log example
    /*
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 495, 494, 227,  61,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   3, 133, 226, 354, 378, 378, 381, 424, 480, 504, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 502, 500, 500, 429, 429,  53,  18,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  73, 197, 307, 378, 378, 385, 422, 493, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 502, 500, 500, 500, 225, 228,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 182, 200, 378, 378, 411, 450, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 500, 463,  59,  56,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 102, 193, 348, 382, 436, 476, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 484, 343,  16,  15,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  17, 129, 268, 384, 444, 503, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 500, 500, 401, 177,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  84, 238, 372, 467, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 504, 500, 223,  38,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  15, 261, 365, 499, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 496,  81,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  11, 264, 372, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 478, 121,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,  45, 241, 322, 475, 506, 506, 506, 506, 506, 506, 506, 506,
 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 492, 220,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  80, 195, 308, 470, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 506, 506, 466, 324, 103,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2,  41, 156, 205, 315, 494, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 506, 496, 409, 364, 228,  17,  13,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  74, 187, 245, 349, 502, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 506, 469, 386, 373, 320,  89,  85,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6, 135, 189, 316, 373, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 412, 378, 378, 345, 178, 181,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   9, 188, 193, 365, 397, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 389, 378, 378, 368, 299, 297,  37,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  38, 191, 225, 376, 446, 506, 506, 506, 506, 506, 506, 506, 506, 506,
 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 508, 435, 378, 378, 374, 331, 332, 133,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   5,  96, 192, 264, 404, 498, 506, 506, 506, 506, 506, 506, 506, 506, 506,
   */
}
#endif

#ifdef DRAM_TX_EYE_SCAN
//-------------------------------------------------------------------------
/** DramcTxEyeScan
 *  Start the tx dq eye scan. (all 32-bit one time)
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcTxEyeScan(void)
{
    U8 ucdq_dly_wrap, ucdq_no, fgerr_dq;
    S8 s1dq_dly, s1vref;
    U32 u4value, u4err_value = 0xffffffff;
    U32 u4prev_phyreg_0d0, u4prev_phyreg_2c8, u4prev_phyreg_2cc, u4prev_phyreg_004, u4prev_phyreg_008;
    U32 u4prev_phyreg_00c, u4prev_phyreg_204, u4prev_phyreg_208, u4prev_phyreg_20c, u4prev_phyreg_048, u4prev_phyreg_248;
    U32 u4prev_phyreg_2d4, u4prev_phyreg_2d8, u4prev_phyreg_4d0, u4prev_phyreg_258;
    U32 u4prev_phyreg_25c, u4prev_phyreg_404, u4prev_phyreg_408, u4prev_phyreg_40c, u4prev_phyreg_298, u4prev_phyreg_448;
    U32 u4addr_array[2];
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;
	

        u4prev_phyreg_0d0 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
        u4prev_phyreg_2c8 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
        u4prev_phyreg_2cc = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
        u4prev_phyreg_004 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x004));
        u4prev_phyreg_008 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x008));
        u4prev_phyreg_00c = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x00c));
        u4prev_phyreg_204 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x204));
        u4prev_phyreg_208 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x208));
        u4prev_phyreg_20c = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x20c));     
        u4prev_phyreg_048 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x048));
        u4prev_phyreg_248 = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x248));

    
    

    //A.set RX DQ/DQS in the middle of the pass region from read DQ/DQS calibration
    //B.Fix DQS (RG_PI_**_PBYTE*) at degree from write leveling. 
    //   Move DQ (per byte) gradually from 90 to -45 degree to find the left boundary
    //   Move DQ (per byte) gradually from 90 to 225 degree to find the right boundary
    //C.For each DQ delay in step B, start engine test
    //D.After engine test, read per bit results from registers.

        //set per bit DQ delay to 0x0: RG_*_TX_*RDQ*_DLY
        //A1 bit 0~15
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x004));
        u4value = u4value & 0xffff0000;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x004), u4value);

        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x008), 0);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x00c));
        u4value = u4value & 0x0000ffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x00c), u4value);

        //A2 bit 16~31
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x204));
        u4value = u4value & 0xffff0000;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x204), u4value);

        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x208), 0);

        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x20c));
        u4value = u4value & 0x0000ffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x20c), u4value);
        //-----------------------------------------------
        //set per byte DQS delay to 0x0: RG_*_TX_*RDQS*_DLY
        //A1 DQS0
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x01c));
        u4value = u4value & 0x0fffffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x01c), u4value);

        //A1 DQS1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x020));
        u4value = u4value & 0xffff0fff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x020), u4value);

        //A2 DQS2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x21c));
        u4value = u4value & 0x0fffffff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x21c), u4value);

        //A2 DQS3
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x220));
        u4value = u4value & 0xffff0fff;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x220), u4value);        

        // assign address array
        u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
        u4addr_array[1] = mcSET_PHY_REG_ADDR(0x248);

        //MUX settings for VREF_GEN_SCAN out
        //RG_*_RX_VREF_OUT_SCAN_SEL=1
        //RG_*_RX_VREF_OUT_SEL=1
        //0x1044[2], [9] for A1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
        //0x1244[2], [9] for A2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
        u4value = u4value | 0x00000204;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);
        

    

    for (s1vref=0; s1vref<16; s1vref++)
    {
        //Set Vref voltage
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0)
        ucDram_Register_Write(u4addr_array[0], u4value); 

        u4value = ucDram_Register_Read(u4addr_array[1]);
        mcSET_FIELD(u4value, s1vref, 0x0000000f, 0)
        ucDram_Register_Write(u4addr_array[1], u4value); 

        //Wait for Vref settles down, 1ms is enough
        mcDELAY_us(10);

        //Move DQ (per byte) gradually from -45 to 225 degree (RG_PI_**_DQ*=-8~40 => 0x38 to 0x28)
        for (s1dq_dly=-8; s1dq_dly<=40; s1dq_dly++)
        {
            // double check if compiler have "%" operation
            //iwrap = ii % 64;
            if (s1dq_dly >= 0)
            {
                ucdq_dly_wrap = (U8) s1dq_dly;
            }
            else
            {
                ucdq_dly_wrap = (U8) (s1dq_dly + 64);
            }


                //A1 byte A&B
                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x0d0));
                mcSET_FIELD(u4value, ucdq_dly_wrap, 0x00003f00, 8);
                mcSET_FIELD(u4value, ucdq_dly_wrap, 0x3f000000, 24);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4value);

                //A2 byte A
                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2c8));
                mcSET_FIELD(u4value, ucdq_dly_wrap, 0x00003f00, 8);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4value);

                //A2 byte B
                u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x2cc));
                mcSET_FIELD(u4value, ucdq_dly_wrap, 0x3f000000, 24);
                ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4value);           		

            

            // enable TE2, audio pattern
            u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);                        

            // array index cannot be negative
            // for log
            u4tx_eye_scan_buf[s1vref][(s1dq_dly+8)] = u4err_value;
        }        
    }

    //MUX settings for Vref out recovery
    //RG_*_RX_VREF_OUT_SCAN_SEL=0
    //RG_*_RX_VREF_OUT_SEL=0

        //0x1044[2], [9] for A1
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x044));
        u4value = u4value & 0xfffffdfb;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x044), u4value);
        //0x1244[2], [9] for A2
        u4value = ucDram_Register_Read(mcSET_PHY_REG_ADDR(0x244));
        u4value = u4value & 0xfffffdfb;
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x244), u4value);

    

    mcSHOW_DBG_MSG("===============================================================================\n");
    mcSHOW_DBG_MSG("    DQ TX eye scan (channel=%d(2:cha, 3:chb))\n", IS_DRAM_CHANNELB_ACTIVE());
    mcSHOW_DBG_MSG("===============================================================================\n");
    // write log
    for (ucdq_no=0; ucdq_no<32; ucdq_no++)
    {
        mcSHOW_DBG_MSG("--DQ%d TX eye scan\n", ucdq_no);
        for (s1vref=15; s1vref>=0; s1vref--)
        {
            for (s1dq_dly=-8; s1dq_dly<=40; s1dq_dly++)
            {
                u4err_value = u4tx_eye_scan_buf[s1vref][(s1dq_dly+8)];
                u4value = u4err_value&(1<<ucdq_no);
                if (u4value == 0)
                {
                    fgerr_dq = 0;
                }
                else
                {
                    fgerr_dq = 1;
                }
                mcSHOW_DBG_MSG("%1d,", fgerr_dq);
            }
            mcSHOW_DBG_MSG("\n");
        }
        mcSHOW_DBG_MSG("\n");
        mcSHOW_DBG_MSG("\n");
    } 	


        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x0d0), u4prev_phyreg_0d0);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2c8), u4prev_phyreg_2c8);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x2cc), u4prev_phyreg_2cc);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x004), u4prev_phyreg_004);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x008), u4prev_phyreg_008);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x00c), u4prev_phyreg_00c);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x204), u4prev_phyreg_204);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x208), u4prev_phyreg_208);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x20c), u4prev_phyreg_20c);     
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x048), u4prev_phyreg_048);
        ucDram_Register_Write(mcSET_PHY_REG_ADDR(0x248), u4prev_phyreg_248);

     

	UNUSED(fgerr_dq);
    
	return DRAM_OK;

    // log example
    /*
--DQ0 TX eye scan
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,


--DQ1 TX eye scan
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
...
...
...
--DQ30 TX eye scan
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,


--DQ31 TX eye scan
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
   */
}
#endif

#ifdef DRAM_DQS_JMETER
//-------------------------------------------------------------------------
/** DramcDqsJmeter
 *  start DQS jitter meter. Eye scan block is built by 2-byte unit
 *  @param p                Pointer of context created by DramcCtxCreate.
 *  @param dqs_no           (U8): 0~3.
 *  @retval status          (DRAM_STATUS_T): DRAM_OK or DRAM_FAIL 
 */
//-------------------------------------------------------------------------
DRAM_STATUS_T DramcDqsJmeter(U8 dqs_no)
{
    U8 ucidx_dqs, ucdqs_dly;
    U32 u4value, u4err_value = 0xffffffff, u4addr_array[4], u4sample_cnt, u4ones_cnt;
	U32 test2_0 = DEFAULT_TEST2_0_CAL;
    U32 test2_1 = DEFAULT_TEST2_1_CAL;
    U32 test2_2 = DEFAULT_TEST2_2_CAL;
	
    if ((dqs_no < 0) || (dqs_no > 3))
    {
        mcSHOW_ERROR_CHIP_DisplayString("DQS number should be 0~3\n");
        return DRAM_FAIL;
    }

    if ((dqs_no == 0)||(dqs_no == 1))
    {

            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x048);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x04c);

        

        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQS_ERR_CNT);
    }
    else
    {

            u4addr_array[0] = mcSET_PHY_REG_ADDR(0x248);
            u4addr_array[1] = mcSET_PHY_REG_ADDR(0x24c);


        u4addr_array[2] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_TOGGLE_CNT_2);
        u4addr_array[3] = mcSET_DRAMC_REG_ADDR(DRAMC_REG_DQS_ERR_CNT_2);
    }

    // check if SoC platform has "%" operation?!
    ucidx_dqs =  dqs_no % 2;
    	
    //Enable DQ eye scan
    //RG_??_RX_EYE_SCAN_EN
    //RG_??_RX_VREF_EN 
    //RG_??_RX_VREF_OP_EN 
    //RG_??_RX_SMT_EN
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value | 0x0000e000;
    ucDram_Register_Write(u4addr_array[0], u4value);

    u4value = ucDram_Register_Read(u4addr_array[1]);
    u4value = u4value | 0xffff0000;
    ucDram_Register_Write(u4addr_array[1], u4value);

    //Enable DQS jitter meter mode (RG_??_RX_DQS_MIOCK_SEL=0, RG_RX_MIOCK_JIT_EN=0)
    //RG_??_RX_DQS_MIOCK_SEL=0
    u4value = ucDram_Register_Read(u4addr_array[0]);
    u4value = u4value & 0xffffefff;
    ucDram_Register_Write(u4addr_array[0], u4value);

    //RG_RX_MIOCK_JIT_EN=0 (Justin, 2012/09/28)
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    //Select DQS to be scanned (0 or 1)
    //RG_??_RX_DQS_EYE_SEL
    u4value = ucDram_Register_Read(u4addr_array[0]);
    mcSET_FIELD(u4value, ucidx_dqs, 0x00800000, 23)
    ucDram_Register_Write(u4addr_array[0], u4value); 	

    //Disable DQ eye scan (b'1), for counter clear
    u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
    mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
    ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

    mcSHOW_DBG_MSG("===============================================================================\n"));
    mcSHOW_DBG_MSG("    DQS jitter meter (channel=%d(2:cha, 3:chb), dqs_%d)\n", !IS_DRAM_CHANNELB_ACTIVE(), dqs_no);
    mcSHOW_DBG_MSG("===============================================================================\n");

    for (ucdqs_dly=0; ucdqs_dly<128; ucdqs_dly++)
    {
        //Set DQS delay (RG_??_RX_DQS_EYE_DLY)
        u4value = ucDram_Register_Read(u4addr_array[0]);
        mcSET_FIELD(u4value, ucdqs_dly, 0x007f0000, 16);
        ucDram_Register_Write(u4addr_array[0], u4value);

        //Reset eye scan counters (reg_sw_rst): 1 to 0
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);
        mcCLR_BIT(u4value, POS_DCBLN_REG_SW_RST);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //Enable DQ eye scan (b'1)
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcSET_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        // enable TE2, audio pattern
        u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);                        

        //Disable DQ eye scan (b'1), for counter latch
        u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN));
        mcCLR_BIT(u4value, POS_DCBLN_RX_EYE_SCAN_EN);
        ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

        //Read the counter values from registers (toggle_cnt*, dqs_err_cnt*); *_cnt is for (DQS0,1) and *_cnt_2 is for (DQS2,3)
        u4sample_cnt = ucDram_Register_Read(u4addr_array[2]);
        u4ones_cnt = ucDram_Register_Read(u4addr_array[3]);

        mcSHOW_DBG_MSG("%3d | %8x --%8x \n", ucdqs_dly, u4sample_cnt, u4ones_cnt);         
    }

    //RG_RX_MIOCK_JIT_EN=0
    //u4value = ucDram_Register_Read(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), &u4value);
    //mcCLR_BIT(u4value, POS_DCBLN_RX_MIOCK_JIT_EN);
    //ucDram_Register_Write(mcSET_DRAMC_REG_ADDR(DRAMC_REG_DCBLN), u4value);

	return DRAM_OK;
 	
    // log example
    /*
===================================================================

            DQS jitter meter (channel=2(2:cha, 3:chb), dqs_0)
===================================================================
  0 |      800 --       0
  1 |      800 --       0
  2 |      800 --       0
  3 |      800 --       0
  4 |      800 --       0
  5 |      800 --       0
  6 |      800 --       0
  7 |      800 --       0
  8 |      800 --       0
  9 |      800 --       0
 10 |      800 --       0
 11 |      800 --       0
 12 |      800 --       0
 13 |      800 --       0
 14 |      800 --       0
 15 |      800 --      2a
 16 |      800 --     325
 17 |      800 --     400
 18 |      800 --     400
 19 |      800 --     425
 20 |      800 --     568
 21 |      800 --     7f2
 22 |      800 --     800
 23 |      800 --     800
 24 |      800 --     800
 25 |      800 --     800
 26 |      800 --     800
 27 |      800 --     800
 28 |      800 --     800
 29 |      800 --     800
 30 |      800 --     800
 31 |      800 --     800
 32 |      800 --     800
 33 |      800 --     800
 34 |      800 --     800
 35 |      800 --     800
 36 |      800 --     800
 37 |      800 --     800
 38 |      800 --     7fe
 39 |      800 --     6ba
 40 |      800 --     46c
 41 |      800 --     400
 42 |      800 --     3ff
 43 |      800 --     380
 44 |      800 --     109
 45 |      800 --       1
 46 |      800 --       0
 47 |      800 --       0
 48 |      800 --       0
 49 |      800 --       0
 50 |      800 --       0
 51 |      800 --       0
 52 |      800 --       0
 53 |      800 --       0
 54 |      800 --       0
 55 |      800 --       0
 56 |      800 --       0
 57 |      800 --       0
 58 |      800 --       0
 59 |      800 --       0
 60 |      800 --       0
 61 |      800 --       0
 62 |      800 --       0
 63 |      800 --       4
 64 |      800 --       4
 65 |      800 --     1b6
 66 |      800 --     35f
 67 |      800 --     400
 68 |      800 --     468
 69 |      800 --     6ee
 70 |      800 --     7e4
 71 |      800 --     800
 72 |      800 --     800
 73 |      800 --     800
 74 |      800 --     800
 75 |      800 --     800
 76 |      800 --     800
 77 |      800 --     800
 78 |      800 --     800
 79 |      800 --     800
 80 |      800 --     800
 81 |      800 --     800
 82 |      800 --     800
 83 |      800 --     800
 84 |      800 --     800
 85 |      800 --     800
 86 |      800 --     800
 87 |      800 --     800
 88 |      800 --     800
 89 |      800 --     7fb
 90 |      800 --     747
 91 |      800 --     437
 92 |      800 --     217
 93 |      800 --      59
 94 |      800 --       2
 95 |      800 --       0
 96 |      800 --       0
 97 |      800 --       0
 98 |      800 --       0
 99 |      800 --       0
100 |      800 --       0
101 |      800 --       0
102 |      800 --       0
103 |      800 --       0
104 |      800 --       0
105 |      800 --       0
106 |      800 --       0
107 |      800 --       0
108 |      800 --       0
109 |      800 --       0
110 |      800 --      3f
111 |      800 --     27e
112 |      800 --     3f2
113 |      800 --     414
114 |      800 --     4ad
115 |      800 --     77e
116 |      800 --     7ea
117 |      800 --     800
118 |      800 --     800
119 |      800 --     800
120 |      800 --     800
121 |      800 --     800
122 |      800 --     800
123 |      800 --     800
124 |      800 --     800
125 |      800 --     800
126 |      800 --     800
127 |      800 --     800
   */
}
#endif

