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
 * $Date: 2015/02/03 $
 * $RCSfile: drv_upscaler.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "general.h"
#include "hw_scpip_pq.h"
#include "hw_psc.h"
#include "drv_upscaler.h"
#include "drv_scaler.h"
#include "drv_scaler_psc.h"
#include "hw_tdsharp.h"
#include "vdo_misc.h"
#include "x_rand.h"
#include "nptv_debug.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

 #if defined(CC_MT5399)
REGTBL_T const CODE REGTBL_PEI[] =
{  
	{SC_PEI_00, 0x00000005, 0xE7F00005},    
	{SC_PEI_01, 0x00F04912, 0x01F0FF1F},    
	{SC_PEI_02, 0x76007040, 0xFF81FFFF},    
	{SC_PEI_03, 0x00054444, 0x000FFFFF},    
	{SC_PEI_04, 0x00064864, 0x000FFFFF},    
	{SC_PEI_05, 0x04244842, 0x0FFFFFFF},    
	{SC_PEI_08, 0x101002BF, 0x77FF07FF},    
	{SC_PEI_09, 0x001001CF, 0x07FF07FF},    
	{SC_PEI_10, 0x101002BF, 0x77FF07FF},    
	{SC_PEI_11, 0x001001CF, 0x07FF07FF},    
	{SC_PEI_12, 0x28B2CC8C, 0x7FFFFFBF},
	{SC_PEI_13, 0x7A141014, 0xFFFFFFFF},
	{REGTBL_END, 0x00000000, 0x00000000},  
};
#endif

REGTBL_T const CODE REGTBL_DE_RING[] =
{
	{SCFIR_09, 0x30310000, 0xb0fff000},		// VER_DERING
	{SCFIR_0A, 0x303013FF, 0xb0ffffff},		// HOR_Y_DERING
	{SCFIR_0B, 0x0206FF60, 0x0ffffff0},		// HOR_UV_DERING
	{REGTBL_END, 0x00000000, 0x00000000},
};

REGTBL_T const CODE REGTBL_Tap_Adap[] =
{
    {SCFIR_0F, 0x8429E508, 0xFFFFFFFC},   
    {SCFIR_10, 0x0428C508, 0x0FFFFFFC},    
    {REGTBL_END, 0x00000000, 0x00000000},
};

REGTBL_T const CODE REGTBL_VERT_CTI[] =
{
    {SCFIR_11, 0x1480CA08, 0xFFFFFFFC},   
    {SCFIR_12, 0x40000000, 0xF0000000},    
    {REGTBL_END, 0x00000000, 0x00000000},
};
#if defined(CC_MT5399)
REGTBL_T const CODE REGTBL_IRR[] =
{
    {IRR_00, 0xB00C1902, 0xBFFFFFFF},
	{IRR_01, 0x00800400, 0xFFEFF7FF},
	{IRR_02, 0x8F014063, 0xEF0FFFFF},
	{IRR_03, 0x00440000, 0xFFEFFFFF},
	{IRR_04, 0x07004040, 0xFF78F3FF},
	{IRR_05, 0x2020F000, 0xFFFFFFFF},   
    {REGTBL_END, 0x00000000, 0x00000000},
};
#endif

/**************************************************************************
 * Filter Coefficients Definition
 *************************************************************************/

UINT32 u4SCHYCoeff[][SCPQ_COEF_TBLSIZE] =
{
    #include "Coeff/HY_up422.txt"     // upscale coeff       
    #include "Coeff/HY_ds422_1_5.txt" // down scale 1.5 coeff
    #include "Coeff/HY_ds422_2.txt"   // down scale 2 coeff
    #include "Coeff/HY_ds422_3.txt"   // down scale 3 coeff
    #include "Coeff/HY_ds422_4.txt"   // down scale 4 coeff
    #include "Coeff/HY_ds422_5.txt"   // down scale 5 coeff
};
UINT32 u4SCHCCoeff[][SCPQ_COEF_TBLSIZE] =
{
    #include "Coeff/HC_up422.txt"     // upscale coeff       
    #include "Coeff/HC_ds422_1_5.txt" // down scale 1.5 coeff        
    #include "Coeff/HC_ds422_2.txt"   // down scale 2 coeff
    #include "Coeff/HC_ds422_3.txt"   // down scale 3 coeff
    #include "Coeff/HC_ds422_4.txt"   // down scale 4 coeff
    #include "Coeff/HC_ds422_5.txt"   // down scale 5 coeff
};
UINT32 u4SCVYCCoeff[][SCPQ_COEF_TBLSIZE] =
{
    #include "Coeff/VYC_up.txt"     // upscale coeff       
    #include "Coeff/VYC_ds_1_5.txt" // down scale 1.5 coeff        
    #include "Coeff/VYC_ds_2.txt"   // down scale 2 coeff
    #include "Coeff/VYC_ds_3.txt"   // down scale 3 coeff
    #include "Coeff/VYC_ds_4.txt"   // down scale 4 coeff
    #include "Coeff/VYC_ds_5.txt"   // down scale 5 coeff
	#include "Coeff/VYC_4Tapsup.txt"	  // upscaler coeff 4-taps for 3D
};

UINT32 dwHCOEFF_MSBVALID_MASK[3]={0x3FFFFFFF,0xFFFFFFFF,0xFFFFFFFF}; //LSB 94 bit valid in 3 dwords
UINT32 dwVCOEFF_MSBVALID_MASK[3]={0x00000000,0x0FFFFFFF,0xFFFFFFFF}; //LSB 60 bit valid in 3 dwords

//-----------------------------------------------------------------------------
/**
 * @brief vDrvUpScalerInit

 * @param void
 * @retval void
 */
//-----------------------------------------------------------------------------
void vDrvSCPQInit(void)
{  
    vDrvSCPQSramCoeffOnOff(SV_ON);
    vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y,u4SCHYCoeff[0]);
    vDrvSCPQSetSramCoeff(COEFTYP_HOR_C,u4SCHCCoeff[0]);
    vDrvSCPQSetSramCoeff(COEFTYP_VER_Y,u4SCVYCCoeff[0]);
    vDrvSCPQSetSramCoeff(COEFTYP_VER_C,u4SCVYCCoeff[0]);

    //VY, C Sram tbl separate
    vIO32WriteFldAlign(SCPIP_SCCTRL1_05, 0x1, SCCTRL1_05_SC_COEF_PP_RD_A);

    //De-ring    
    vDrvDeRingInit();

    //Tap-adap
    vDrvSCPQTapAdapInit(); 

	//Vertical CTI
    vDrvSCPQVCTIInit(); 
    vDrvSCPQVCTIOnOff(SV_ON);

	//IRR
    vDrvSCPQIRRInit(); 
    vDrvSCPQIRROnOff(SV_ON);
    
	//PEI
    vDrvUsPeiInit();	

    // SW register
    vIO32WriteFldAlign(VDP_SCPQ_01, SV_ON, SCPQ_CUST_UPCOEFF);
    vIO32WriteFldAlign(VDP_SCPQ_02, 25, SCPQ_CUST_SHARP_UI);
    vIO32WriteFldAlign(VDP_SCPQ_02, 0x30, SCPQ_VCTI_GAIN);
}

void vDrvSCPQSramCoeffOnOff(UINT8 bOnOff)
{
    vIO32WriteFldAlign(SCFIR_0C, bOnOff, H_SRAM_COEFF_EN);
    vIO32WriteFldAlign(SCFIR_0C, bOnOff, V_SRAM_COEFF_EN);
}

void vDrvSCPQSetSramCoeff(UINT8 bType, UINT32 dwCoeff[SCPQ_COEF_TBLSIZE])
{
    UINT16 i;

	if(bType>=COEFTYP_MAX)
	{
        LOG(3,"Set Scaler PQ SRAM Coeff Type: %d,  Wrong !!\n", bType);
	}

	//initial setting before writing coefficient
	//vDrvSCPQSramCoeffOnOff(SV_OFF);
    
    vIO32WriteFldMulti(SCFIR_00, P_Fld(0x1, RD_AUTO_INC_ADDR)|
		  						 P_Fld(0x1, WR_AUTO_INC_ADDR)|
    							 P_Fld(0x0, PIO_MODE)|
		  						 P_Fld(0x0, FORCE_WR_NO_WAIT)|
		  						 P_Fld(0xFF, MAX_WAIT_CYCLE)); 	
	vIO32WriteFldAlign(SCFIR_01, (bType!=3), CPURW_ACTIVE);
    vIO32WriteFldAlign(SCFIR_01, (bType==3), CPURW_ACTIVE_VPP);	
	vIO32WriteFldMulti(SCFIR_02, P_Fld(bType, COE_SEL)|
		  						 P_Fld(0x0, CPU_SRAM_ADDR)); 

    LOG(3, "Set Scaler PQ SRAM Coeff Type: %d ,\n", bType);
	
	//writing sram coeff
	for (i = 0; i < SCPQ_PHASE_NUM; i++)
	{
		vIO32Write4B(SCFIR_05, dwCoeff[i*SCPQ_COEF_LEN+0]);
		vIO32Write4B(SCFIR_04, dwCoeff[i*SCPQ_COEF_LEN+1]);
		vIO32Write4B(SCFIR_03, dwCoeff[i*SCPQ_COEF_LEN+2]);
        LOG(3, "Set Scaler PQ SRAM Coeff Type: %d , dwCoeff = 0x%x 0x%x 0x%x\n", bType, 
            dwCoeff[i*SCPQ_COEF_LEN+0], dwCoeff[i*SCPQ_COEF_LEN+1], dwCoeff[i*SCPQ_COEF_LEN+2]);
        
	}

	//finish writing sram coeff
	vIO32WriteFldAlign(SCFIR_01, 0, CPURW_ACTIVE);
	vIO32WriteFldAlign(SCFIR_01, 0, CPURW_ACTIVE_VPP);
}

void vDrvSCPQCoeffRead(UINT8 bType, UINT32 dwResult[SCPQ_COEF_TBLSIZE])
{
    UINT16 i;
    
	if(bType>=COEFTYP_MAX)
	{
        LOG(3,"Scaler PQ Coeff Type: %d,  Wrong !!\n", bType);
	}
    
    //vDrvSCPQSramCoeffOnOff(SV_OFF);
    
	//initial setting before reading coefficient
    vIO32WriteFldMulti(SCFIR_00, P_Fld(0x1, RD_AUTO_INC_ADDR)|
		  						 P_Fld(0x1, WR_AUTO_INC_ADDR)|
    							 P_Fld(0x0, PIO_MODE)|
		  						 P_Fld(0x0, FORCE_WR_NO_WAIT)); 
	
	vIO32WriteFldAlign(SCFIR_01, (bType!=3), CPURW_ACTIVE);
	vIO32WriteFldAlign(SCFIR_01, (bType==3), CPURW_ACTIVE_VPP);
	vIO32WriteFldMulti(SCFIR_02, P_Fld(bType, COE_SEL)|
		  						 P_Fld(0x0, CPU_SRAM_ADDR)); 
    
    LOG(3, "Scaler PQ Coeff Type: %d !!\n", bType);
	//reading sram coeff
	for (i = 0; i < SCPQ_PHASE_NUM; i++)
	{
        dwResult[i*SCPQ_COEF_LEN+2]=IO32ReadFldAlign(SCFIR_06, RDATA_00);
        dwResult[i*SCPQ_COEF_LEN+1]=IO32ReadFldAlign(SCFIR_07, RDATA_01);
        dwResult[i*SCPQ_COEF_LEN+0]=IO32ReadFldAlign(SCFIR_08, RDATA_02);
		LOG(3, "0x%08x, 0x%08x, 0x%08x,\n", dwResult[i*SCPQ_COEF_LEN+0], dwResult[i*SCPQ_COEF_LEN+1], dwResult[i*SCPQ_COEF_LEN+2]);
	}
	
	//finish reading sram coeff
	vIO32WriteFldAlign(SCFIR_01, 0, CPURW_ACTIVE);
	vIO32WriteFldAlign(SCFIR_01, 0, CPURW_ACTIVE_VPP);

}

UINT8 bDrvSCPQCoeffValidate(UINT8 bType, UINT32 dwCoeffTblR[SCPQ_COEF_TBLSIZE], UINT32 dwCoeffTblW[SCPQ_COEF_TBLSIZE])
{
    UINT16 i, wIdx;
    UINT32* dwCoeffMsk;
    UINT8 bRet = SV_TRUE;

	if(bType>=COEFTYP_MAX)
	{
        LOG(3,"Scaler PQ Coeff Type: %d,  Wrong !!\n", bType);
	}
    
    dwCoeffMsk=(bType<=COEFTYP_HOR_C)?dwHCOEFF_MSBVALID_MASK:dwVCOEFF_MSBVALID_MASK;
    
    for(i=0; i<SCPQ_PHASE_NUM; i++)
    {
        wIdx=i*SCPQ_COEF_LEN;
	    if (dwCoeffTblR[wIdx+0]!=(dwCoeffTblW[wIdx+0]&dwCoeffMsk[0])||
	        dwCoeffTblR[wIdx+1]!=(dwCoeffTblW[wIdx+1]&dwCoeffMsk[1])||
	        dwCoeffTblR[wIdx+2]!=(dwCoeffTblW[wIdx+2]&dwCoeffMsk[2]))
	    {
	    	Printf("Scaler PQ Coeff Type: %d,  Wrong !!\n", bType);
			Printf("[0] Read [%d]  Write [%d]\n", dwCoeffTblR[wIdx+0], dwCoeffTblW[wIdx+0]);
			Printf("[1] Read [%d]  Write [%d]\n", dwCoeffTblR[wIdx+1], dwCoeffTblW[wIdx+1]);
			Printf("[2] Read [%d]  Write [%d]\n", dwCoeffTblR[wIdx+2], dwCoeffTblW[wIdx+2]); 
            bRet = SV_FALSE;
	    }
    }
    
    return bRet;
}

UINT8 vDrvSCPQSetHCoeff(UINT8 bPath)
{
    UINT8 bHCoeffIdx, IsScalerInput444;
	UINT8 bCustUPCoeff = IO32ReadFldAlign(VDP_SCPQ_01, SCPQ_CUST_UPCOEFF);
	UINT32 dwScHIn = u4IO32Read4B(SCALER_COEF_MAIN_H_IN);
	UINT32 dwScHOut= u4IO32Read4B(SCALER_COEF_MAIN_H_OUT);
	
    static UINT8 bPreHCoeffIdx = 0xFF;
	static UINT8 bPreIsScalerInput444 = 0xFF;
	
    if(bPath!=SV_VP_MAIN)
        return SV_FALSE;

	/*	bHCoeffIdx
			0: up scaling
			1: down scaling 1.5
			2: down scaling 2
			3: down scaling 3
			4: down scaling 4
			5: down scaling 5
		*/ 
	bHCoeffIdx = (dwScHOut>=dwScHIn)?0: // upscaling only 1 hw default tbl
			  (dwScHOut*9<dwScHIn*2)?5: // DS ratio > 4.5 use 5
			  (dwScHOut*7<dwScHIn*2)?4: // DS ratio > 3.5 use 4
			  (dwScHOut*5<dwScHIn*2)?3: // DS ratio > 2.5 use 3
			  (dwScHOut*3<dwScHIn*2)?2: // DS ratio > 1.75 use 2
			  (1);						// DS ratio < 1.75 use 1.5
	IsScalerInput444 = bIsScalerInput444(bPath);
    
	//Use Sram coeff tbl
	if((!bCustUPCoeff))
	{ 
		if((bPreHCoeffIdx!=bHCoeffIdx)||(IsScalerInput444!=bPreIsScalerInput444))
		{
			bPreHCoeffIdx = bHCoeffIdx;
			bPreIsScalerInput444 = IsScalerInput444;
			
	        vDrvSCPQSetSramCoeff(COEFTYP_HOR_Y,u4SCHYCoeff[bHCoeffIdx]);
			if(!IsScalerInput444) 
			{
				// 422 mode Y/C needs different coeff
        		vDrvSCPQSetSramCoeff(COEFTYP_HOR_C,u4SCHCCoeff[bHCoeffIdx]);
			}
			else
			{
				vDrvSCPQSetSramCoeff(COEFTYP_HOR_C,u4SCHYCoeff[bHCoeffIdx]);
			}
		}
    }
	else
	{
		DRVCUST_HUPSCaleCoeff(bPath);
	}

	//Turn off dering when down-scaling
    vDrvDeRingOnOff(bPath, SCPQ_DIR_H, !bHCoeffIdx);
    vDrvSCPQTapAdapOnOff(bPath, SCPQ_DIR_H, !bHCoeffIdx);
    vDrvUsPeiOnOff(bPath, !bHCoeffIdx);

    return SV_TRUE;
}

UINT8 vDrvSCPQSetVCoeff(UINT8 bPath)
{
    UINT8 bVCoeffIdx;
    UINT8 bLineInterleave;
	UINT32 dwScVIn = u4IO32Read4B(SCALER_COEF_MAIN_V_IN);
	UINT32 dwScVOut= u4IO32Read4B(SCALER_COEF_MAIN_V_OUT);	
    static UINT8 bPreVCoeffIdx = 0xFF;
	UINT8 bPingPongMode = IO32ReadFldAlign(SCPIP_SCCTRL1_04, SCCTRL1_04_SC_COEF_PP_ENABLE);
    UINT8 bPingPongWRTVC = IO32ReadFldAlign(SCPIP_SCCTRL1_05, SCCTRL1_05_SC_COEF_PP_RD_A);
    UINT32   u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);
	UINT8 bCustUPCoeff = IO32ReadFldAlign(VDP_SCPQ_01, SCPQ_CUST_UPCOEFF);
	UINT8 bTTD_Coeff_Blur = ((dwScVIn>=720)&&
								(u4DrvTDTV3DModeQuery()==E_TDTV_UI_3D_MODE_TTD)&&
								IS_LVDS_DISP_3D_POLARIZED);
    if(bPath!=SV_VP_MAIN)
        return SV_FALSE;

	//scaler out line interleave or not
	bLineInterleave =((E_TD_OUT_3D_LI==eSCLOut)||
					 ((E_TD_IN_LI_P==eSCLIn)&&(E_TD_OUT_NATIVE==eSCLOut))); 

     /*  bVCoeffIdx
        0: up scaling
        1: down scaling 1.5
        2: down scaling 2
        3: down scaling 3
        4: down scaling 4
        5: down scaling 5
    */
    bVCoeffIdx = ((dwScVOut>=dwScVIn) && (!bLineInterleave) && (!bTTD_Coeff_Blur))?0: // upscaling hw default tbl
#if !defined(CC_MT5399)||!defined(CC_MT5882)
	//Viper TTD needs differenct coeff to remove aliasing effect
    			 ((dwScVOut>=dwScVIn) && (!bLineInterleave))?2:
#endif    			 
    		  	  (dwScVOut>=dwScVIn)?6: // upscaling use 4 taps tbl
	              (dwScVOut*9<dwScVIn*2)?5: // DS ratio > 4.5 use 5
	              (dwScVOut*7<dwScVIn*2)?4: // DS ratio > 3.5 use 4
	              (dwScVOut*5<dwScVIn*2)?3: // DS ratio > 2.5 use 3
	              (dwScVOut*3<dwScVIn*2)?2: // DS ratio > 1.75 use 2
	              (1);                      // DS ratio < 1.75 use 1.5	
    //Use Sram coeff tbl
	if((!bCustUPCoeff))
	{       
        if(bPingPongMode)
        {
            //Ping-pong R/W VYC same tbl         
            vDrvSCPQSetSramCoeff(((bPingPongWRTVC)?COEFTYP_VER_C:COEFTYP_VER_Y), 
                                                          u4SCVYCCoeff[bVCoeffIdx]);
            vIO32WriteFldAlign(SCPIP_SCCTRL1_05, (!bPingPongWRTVC), SCCTRL1_05_SC_COEF_PP_RD_A);
        }
        else
        {
    		if(bPreVCoeffIdx!=bVCoeffIdx)
    		{
    			bPreVCoeffIdx = bVCoeffIdx;
    			vDrvSCPQSetSramCoeff(COEFTYP_VER_Y, u4SCVYCCoeff[bVCoeffIdx]);
    			vDrvSCPQSetSramCoeff(COEFTYP_VER_C, u4SCVYCCoeff[bVCoeffIdx]);					
    		}
		}
    }
	else
	{
		DRVCUST_VUPSCaleCoeff(bPath);
	}

	//Turn off dering when down-scaling
    vDrvDeRingOnOff(bPath, SCPQ_DIR_V, !bVCoeffIdx);
    vDrvSCPQTapAdapOnOff(bPath, SCPQ_DIR_V, !bVCoeffIdx);
    vDrvUsPeiOnOff(bPath, !bVCoeffIdx);

    return SV_TRUE;
}

/*****************************************************************************************/
/*************************************   Tap-adap   *******************************************/
/*****************************************************************************************/
void vDrvSCPQTapAdapInit(void)
{
    vDrvLoadRegTbl(REGTBL_Tap_Adap);
}

void vDrvSCPQTapAdapOnOff(UINT32 u4Path, UINT32 u4Dir, UINT32 u4OnOff)
{
    if (u4Path == SV_VP_MAIN)
    {
        if (bIsScalerInput444(SV_VP_MAIN))
        {
            u4OnOff = SV_OFF;
        }
        else
        {
            u4OnOff = (u4OnOff > 0) ? SV_ON : SV_OFF;
        }

        if (u4Dir == SCPQ_DIR_H)
        {
            vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, u4OnOff, SCCTRL1_0C_SC_TAP_ADAPT_H_EN);
        }
        else if (u4Dir == SCPQ_DIR_V)
        {
            vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, u4OnOff, SCCTRL1_0C_SC_TAP_ADAPT_V_EN);
        }
    }
}

/*****************************************************************************************/
/*******************************     Vertical CTI    *******************************************/
/*****************************************************************************************/
void vDrvSCPQVCTIInit(void)
{
    vDrvLoadRegTbl(REGTBL_VERT_CTI);
}

void vDrvSCPQVCTIOnOff(UINT8 bOnOff)
{
	VRM_INFO_T in,out;
    UINT8 bLineInterleave;

    UINT32   u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);

    UINT32 dwScVIn = u4IO32Read4B(SCALER_COEF_MAIN_V_IN);
	UINT32 dwScVOut= u4IO32Read4B(SCALER_COEF_MAIN_V_OUT);	
	UINT32 dwScHIn = u4IO32Read4B(SCALER_COEF_MAIN_H_IN);
	UINT32 dwScHOut= u4IO32Read4B(SCALER_COEF_MAIN_H_OUT);

    //scaler out line interleave or not
    bLineInterleave =((E_TD_OUT_3D_LI==eSCLOut)||
                     ((E_TD_IN_LI_P==eSCLIn)&&(E_TD_OUT_NATIVE==eSCLOut))); 

    //get fsc in & fsc out info
    VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC, &in, &out);

    //Turn off this function when  front scaler out is
    //a. line interleave
    //b. width > 1920
    //c. height > 1080
    bOnOff &= (!(out.u2Width > 1920 || 
                 out.u2Height> 1080 || 
                 in.u2Height > 2000 ||
                 bLineInterleave||
                 (dwScVOut < dwScVIn)||
                 (dwScHOut < dwScHIn)));
	
	vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, bOnOff, SCCTRL1_0C_SC_VERT_CTI_EN);
	vDrvSetImportWaitFlag(SV_VP_MAIN, VRM_IMPORT_WAIT_FLAG_TRIG_START);	
}

/*****************************************************************************************/
/****************************************      IRR     ****************************************/
/*****************************************************************************************/
void vDrvSCPQIRRInit(void)
{
#if defined(CC_MT5399)
     vDrvLoadRegTbl(REGTBL_IRR);
#endif
}

void vDrvSCPQIRROnOff(UINT8 bOnOff)
{
#if defined(CC_MT5399)
    vIO32WriteFldAlign(IRR_00, bOnOff, IRR_EN);
#endif
}

/*****************************************************************************************/
/*************************************   De-Ring   ******************************************/
/*****************************************************************************************/
void vDrvDeRingInit(void)
{
   	 vDrvLoadRegTbl(REGTBL_DE_RING);
}

void vDrvDeRingOnOff(UINT32 u4Path, UINT32 u4Dir, UINT32 u4OnOff)
{
    if (u4Path == SV_VP_MAIN)
    {
        if (bIsScalerInput444(SV_VP_MAIN))
        {
            u4OnOff = SV_OFF;
        }
        else
        {
            u4OnOff = (u4OnOff > 0) ? SV_ON : SV_OFF;
        }

        if (u4Dir == SCPQ_DIR_H)
        {
            vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, u4OnOff, SCCTRL1_0C_SC_DERING_H_EN);
        }
        else if (u4Dir == SCPQ_DIR_V)
        {
            vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, u4OnOff, SCCTRL1_0C_SC_DERING_V_EN);
        }
    }
}

/*****************************************************************************************/
/*******************************    Pure Edge Interpolation   **********************************/
/*****************************************************************************************/
void vDrvUsPeiInit(void)
{
#if defined(CC_MT5399)
	vDrvLoadRegTbl(REGTBL_PEI);
#endif
}

void vDrvUsPeiOnOff(UINT32 u4Path, UINT32 u4OnOff)
{
#if defined(CC_MT5399)
    VRM_INFO_T in,out;
    UINT8 bLineInterleave;

    UINT32   u4SCL  = u4DrvTDTVScalerModeQuery();
    E_TD_OUT eSCLOut= TD_SCL_OUT(u4SCL);
    E_TD_IN  eSCLIn = TD_SCL_IN(u4SCL);

    UINT32 dwScVIn = u4IO32Read4B(SCALER_COEF_MAIN_V_IN);
	UINT32 dwScVOut= u4IO32Read4B(SCALER_COEF_MAIN_V_OUT);	
	UINT32 dwScHIn = u4IO32Read4B(SCALER_COEF_MAIN_H_IN);
	UINT32 dwScHOut= u4IO32Read4B(SCALER_COEF_MAIN_H_OUT);

    if (u4Path == SV_VP_MAIN)
    {
        if (bIsScalerInput444(SV_VP_MAIN))
        {
            u4OnOff = SV_OFF;
        }
        else
        {
            u4OnOff = (u4OnOff > 0) ? SV_ON : SV_OFF;
        }

        //scaler out line interleave or not
        bLineInterleave =((E_TD_OUT_3D_LI==eSCLOut)||
                         ((E_TD_IN_LI_P==eSCLIn)&&(E_TD_OUT_NATIVE==eSCLOut))); 
        
        //get fsc in & fsc out info
        VRMGetModuleVRMInfo(SV_VP_MAIN, VRM_MODULE_FSC, &in, &out);
        
        //Turn off this function when  front scaler out is
        //a. line interleave
        //b. width > 1920
        //c. height > 1080
        u4OnOff &= (!(out.u2Width > 1920 || 
                     out.u2Height> 1080 || 
                     in.u2Height > 2000 ||
                     bLineInterleave||
                     (dwScVOut < dwScVIn)||
                     (dwScHOut < dwScHIn)||
                     (in.u2Height >= out.u2Height)));   
        
        vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, u4OnOff, SCCTRL1_0C_SC_PE_ENABLE);
        vDrvSetImportWaitFlag(SV_VP_MAIN, VRM_IMPORT_WAIT_FLAG_TRIG_START);     
    }
	
#elif defined(CC_MT5882)
	vIO32WriteFldAlign(SCPIP_SCCTRL1_0C, SV_OFF, SCCTRL1_0C_SC_PE_ENABLE);
#endif
}


