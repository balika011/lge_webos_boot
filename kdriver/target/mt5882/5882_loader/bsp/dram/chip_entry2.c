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

#include "x_typedef.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_dram.h"
#include "x_serial.h"
#include "drvcust_if.h"
#include "ddr.h"
#include "chip_txt.h"

extern void CHIP_DisplayString(const CHAR *szString);
extern void CHIP_DisplayInteger(INT32 i);
extern UINT32 _DDR_CheckSize(BOOL fgChBActive);
extern UINT32 CHIP_IsRemap(void);

#ifdef CC_MTK_PRELOADER
static UINT32 _Chip_CalibrateDdr(void)
{
	UINT32 u4Ret = 0;

#ifdef DRAM_RX_EYE_SCAN
	U8 dq_no=0;
#endif

#ifdef DRAM_Pll_PHASE_CAL
	DramcPllPhaseCal();
#endif

#ifdef DRAM_IMPEDANCE_CAL
	DramcImpedanceCalApply();
#endif

#ifdef DRAM_WRITE_LEVELING_CAL
	DramcWriteLeveling();
#endif

#ifdef DRAM_MiockJmeter	
	DramcMiockJmeter();
#endif

#ifdef DRAM_GATING_SCAN
	u4Ret = DramcRxdqsGatingCal();
#endif

#ifdef DRAM_RX_ODT_SCAN
	DramcRxOdtScan();
#endif

#ifdef DRAM_RX_WINDOW_PERBIT_CAL
	u4Ret |= DramcRxWindowPerbitCal(0);
#endif

#ifdef DRAM_RX_DATLAT_CAL
	DramcRxdatlatCal();
#endif

#ifdef DRAM_TX_OCD_DRV_SCAN
	DramcTxOcdDrvScan();
#endif

#ifdef DRAM_TX_VREF_CAL
	DramcTxVrefCal();
#endif

#ifdef DRAM_TX_WINDOW_PERBIT_CAL
	u4Ret |= DramcTxWindowPerbitCal(0);
#endif

#ifdef DRAM_RX_EYE_SCAN
	for(dq_no =0;dq_no<32;dq_no++)
		DramcRxEyeScan(dq_no);
#endif

#ifdef DRAM_TX_EYE_SCAN
	DramcTxEyeScan();
#endif

    return u4Ret;
}
#endif /* #ifndef CC_MTK_PRELOADER */

void CHIP_BootEntry2(void)
{   

#ifdef CC_MTK_PRELOADER
#ifdef DRAM_WRITE_READ_LOOP_AFTER_CALIBRATION
	volatile UINT32 i;
#endif	
	//volatile UINT32 DummyRead[16];
	UINT32 u4Ret = 0;
	UINT32 u4Val;
	
#ifdef DRAM_WRITE_READ_LOOP_AFTER_CALIBRATION	
	UINT32 u4err_value;
	UINT32 test2_0 = DEFAULT_TEST2_0_CAL;
    UINT32 test2_1 = DEFAULT_TEST2_1_CAL;
    UINT32 test2_2 = DEFAULT_TEST2_2_CAL;
#endif

	if (!CHIP_IsRemap())
	{
		CHIP_DisplayString("DRAM Channel A Calibration.\n");	

		TCMSET_CHANNELA_ACTIVE();
	   
		u4Ret = _Chip_CalibrateDdr();
		
		if (u4Ret == DRAM_OK)
        {
            TCMSET_CHANNELA_SIZE(_DDR_CheckSize(FALSE)/0x100000);
            CHIP_DisplayString("DRAM A Size = ");       
            CHIP_DisplayInteger(TCMGET_CHANNELA_SIZE());
            CHIP_DisplayString(" Mbytes.\n");            
        }
		
		if (IS_DRAM_CHANNELB_SUPPORT())
		{
			CHIP_DisplayString("DRAM Channel B Calibration.\n");

			TCMSET_CHANNELB_ACTIVE();
			
			u4Ret |= _Chip_CalibrateDdr();
			
            if (u4Ret == DRAM_OK)
            {
                TCMSET_CHANNELB_SIZE(_DDR_CheckSize(TRUE)/0x100000);
                CHIP_DisplayString("DRAM B Size = ");       
                CHIP_DisplayInteger(TCMGET_CHANNELB_SIZE());
                CHIP_DisplayString(" Mbytes.\n");            
            }
		
			// Back to channel A.
			TCMSET_CHANNELA_ACTIVE();

			#if defined(CC_CHA_CHB_NO_GAP)
            // Set CHSEL channel B according to channel A Mbytes.
            u4Val = ucDram_Register_Read(mcSET_ARBITER_ADDR(0x0));
            u4Val &= ~0x700000;
            if(TCMGET_CHANNELA_SIZE()==0x40)     
                ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4Val |0x100000);//channel A 64Mbytes
            else if(TCMGET_CHANNELA_SIZE()==0x80)    
                ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4Val |0x200000);//channel A 128Mbytes
            else if(TCMGET_CHANNELA_SIZE()==0x100)    
                ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4Val |0x300000);//channel A 256Mbytes
            else if(TCMGET_CHANNELA_SIZE()==0x200)
                ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4Val |0x400000);//channel A 512Mbytes
            else
                ucDram_Register_Write(mcSET_ARBITER_ADDR(0x0), u4Val |0x500000);//channel A 1Gbytes
            #endif 

		}
		
#ifdef DRAM_LOAD_BOARD
	LoadBoardShowResult(FLAG_TX_DQS_CALIBRATION,\
								FLAG_CALIBRATION_PASS,\
								FLAG_COMPLETE_AND_PASS);

	CHIP_DisplayString("DRAM Load board pass!.\n");
	while(1);
	
#endif

#ifdef DRAM_WRITE_READ_LOOP_AFTER_CALIBRATION
		while(1)
		{
			for(i=0;(i*DEFAULT_TEST2_2_CAL*32)<(TCMGET_CHANNELA_SIZE()*1024*1024);i++)
			{
				test2_1 =  DEFAULT_TEST2_1_CAL | (i*DEFAULT_TEST2_2_CAL);
				u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);

				if(!u4err_value)
				{	
					CHIP_DisplayString("Channel A ");
					CHIP_DisplayHex(i*DEFAULT_TEST2_2_CAL*32);
					CHIP_DisplayString(" ~ ");
					CHIP_DisplayHex((i+1)*DEFAULT_TEST2_2_CAL*32-1);
					CHIP_DisplayString(" Pass!!\n");
				}
				else
				{
					CHIP_DisplayString("Channel A ");
					CHIP_DisplayHex(i*DEFAULT_TEST2_2_CAL*32);
					CHIP_DisplayString(" ~ ");
					CHIP_DisplayHex((i+1)*DEFAULT_TEST2_2_CAL*32-1);
					CHIP_DisplayString(" Error!!\n");
					CHIP_DisplayString("Error value is ");
					CHIP_DisplayHex(u4err_value);
					CHIP_DisplayString(".\n");
					while(1);
				}
				mcDELAY_us(1000);
			}
			
			if (IS_DRAM_CHANNELB_SUPPORT())
			{
				TCMSET_CHANNELB_ACTIVE();
				
				for(i=0;(i*DEFAULT_TEST2_2_CAL*32)<(TCMGET_CHANNELB_SIZE()*1024*1024);i++)
				{
					u4err_value = DramcEngine2(TE_OP_WRITE_READ_CHECK, test2_0, test2_1, test2_2);
					if(!u4err_value)
					{
						CHIP_DisplayString("Channel B ");
						CHIP_DisplayHex(i*DEFAULT_TEST2_2_CAL*32);
						CHIP_DisplayString(" ~ ");
						CHIP_DisplayHex((i+1)*DEFAULT_TEST2_2_CAL*32-1);
						CHIP_DisplayString(" Pass!!\n");
					}
					else
					{
						CHIP_DisplayString("Channel B ");
						CHIP_DisplayHex(i*DEFAULT_TEST2_2_CAL*32);
						CHIP_DisplayString(" ~ ");
						CHIP_DisplayHex((i+1)*DEFAULT_TEST2_2_CAL*32-1);
						CHIP_DisplayString(" Error!!\n");
						CHIP_DisplayString("Error value is ");
						CHIP_DisplayHex(u4err_value);
						CHIP_DisplayString(".\n");
						while(1);
					}
					mcDELAY_us(1000);
				}
				TCMSET_CHANNELA_ACTIVE();
			}
			
		}
#endif
		// Set to download mode.
        if (u4Ret > 0)
        {               
            SerNormalMode();
            while(1);
        }
		
	#if (!defined(__MODEL_slt__)) && defined(CC_MTK_PRELOADER) && (defined(DRAM_DEBUG) || defined(DRAM_SLT_DEBUG))
		//CLK_QueryDramSetting((UINT8 *)"Dram generate test pattern after calibration:", DRAM_GEN_TEST_PATTERN_AFTER_CALIBRATION);
	#endif
        #ifdef CC_DRAM_TUNING
		DRAM_StoreParams(INIT_DRAM_B_BASE + DRAM_PARAMS_ADDR);
        #endif // CC_DRAM_TUNING
	}
#endif				
}

