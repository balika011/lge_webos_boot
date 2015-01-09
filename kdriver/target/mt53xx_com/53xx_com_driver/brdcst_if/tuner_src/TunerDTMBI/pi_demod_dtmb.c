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
 * $RCSfile: pi_demod_dtmb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod_dtmb.c
 *  DTMB demod related API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "pi_dtmb_if.h"
#include "pi_demod.h"
#include "dtmb_com_defs.h" 
#include "pd_common.h"

#if defined(CC_MT5890)
#include "code_img_MT5861DTMB.h"
#endif
#if defined(CC_MT5882)
#include "code_img_MT5882DTMB.h"
#endif


#include "drvcust_if.h"

#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
#include"x_hal_5381.h"
#include "ctrl_bus.h"  //add by liuqu,20090429
#include "spi_if.h"
#include "tuner_interface_if.h"
#include "drvcust_if.h"
#include "fbm_drvif.h"

//-----------------------------------------------------------------------------
// Compile option define
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define REWRITETDI					1
#define HOSTCMD_UP					0

#define cRISC_DTMB_BASE          (IO_VIRT + 0x2A000)
#define cRISC_ACD_BASE           (IO_VIRT + 0x61000)
#define CKGEN_BASE               (IO_VIRT + 0x0d000)

#define MT5175_DTMB_SW_VER      "1.000"
#define fcUP_50M_CLK            1
#define fcMEASURE_DRIVER_TIME   0
#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)

#define cMAX_TRY_CNT            2
#define cMAX_WAIT_LOOP          50//10 dtmb time improve
#define cWAIT_TIME_INTERVAL     2//10         // ms dtmb time improve


#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}
#define mcGET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
    u4RegValue=mcRISC_Reg(pu4RegBase,u4RegOfst);			\
    mcSHOW_DBG_MSG(("r 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}



EXTERN UINT32 PDWNC_ReadServoADCChannelValue(UINT32 u4Channel);

static U8 u1AfterAcq = 0;
static void  vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len);
static VOID DTMB_SetTunerRel(DEMOD_CTX_T *psDemodCtx);
//static void  vSetChSearchMode(DEMOD_CTX_T *psDemodCtx);


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0

//----------------------------------------------------------------------------- 
VOID DTMB_SetTunerRel(DEMOD_CTX_T *psDemodCtx)
{
CN_TUNER_REL_REG_T * pTunerRelReg;
#if defined (CC_MT5890)
	UINT32 u4Temp;
	UINT32 u4TempRSetting;
#endif

	
	mcSHOW_DBG_MSG(("DTMB_SetTunerRel\n"));

	if (ITuner_OP(psDemodCtx->psTunerCtx, itGetCNDTMBTunerRelReg, 0, &pTunerRelReg) == ITUNER_OK)
	{
		if(pTunerRelReg)
		{
			while(pTunerRelReg->u2RegAddr!=0)
			{
				mcSHOW_DBG_MSG4(("CNDTMB RegWrite 0x%04X 0x%02X\n",pTunerRelReg->u2RegAddr,pTunerRelReg->u1RegVal));
				DTMB_SetReg(psDemodCtx,pTunerRelReg->u2RegAddr, &pTunerRelReg->u1RegVal,1);
				pTunerRelReg++;
			}
		}
	}
#if defined (CC_MT5890)

   if(ITUNER_OK == ITuner_OP(ITunerGetCtx(), itSetRSetting, MOD_DTMB, &u4TempRSetting))
   	mcSHOW_DBG_MSG(("DTMB set RSetting by tuner is %d\n", u4TempRSetting));
   u4TempRSetting = (u4TempRSetting << 16);
   u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x418);
   u4Temp &= 0xFFF8FFFF;
   u4Temp |= u4TempRSetting;   
   mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//RSetting:bit[18:16]
#endif
}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_ChipInit
 *  This function initialize DTMBrelated registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_ChipInit(DEMOD_CTX_T *psDemodCtx)
{
	UINT32 u4Tmp;
	UCHAR   ucValue;
	UINT32 u4GpioforRFAGCSwitch;
	
	u4Tmp = mcRISC_Reg(cRISC_DTMB_BASE,0x4b4);
	u4Tmp|=0x000f0000;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x4b4, u4Tmp);
	
	#if REWRITETDI
	DTMB_SetTdiAddr();
	#endif
	#if HOSTCMD_UP
	u4Tmp = mcRISC_Reg(cRISC_DTMB_BASE,0xe00);
	ucValue= u4Tmp & 0x000000ff;
	mcSHOW_DBG_MSG(("[DTMB_ChipInit]for fw re-load, get e00~e03 = 0x%x,e00=0x%x\n",u4Tmp,u4Tmp&0x000000ff));
	if(ucValue == 0x42)
	{		
		psDemodCtx->u1LoadFwOpt1=1;
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]pre-mode is atd,u1LoadFwOpt1=%d\n",psDemodCtx->u1LoadFwOpt1));
	}
	else
	{		
		psDemodCtx->u1LoadFwOpt1 = 0;
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]pre-mode is not atd,u1LoadFwOpt1=%d\n",psDemodCtx->u1LoadFwOpt1));
	}
	#else
	// re-load fw
	mcSHOW_DBG_MSG(("DTMB_ChipInit reload fw ??? \n"));
	u4Tmp = mcRISC_Reg(cRISC_DTMB_BASE,0xe00);
	ucValue= u4Tmp & 0x000000ff;
	mcSHOW_DBG_MSG(("[DTMB_ChipInit]for fw re-load, get e00~e03 = 0x%x,e00=0x%x\n",u4Tmp,u4Tmp&0x000000ff));
	if(ucValue == 0x0A)
	{		
		psDemodCtx->u1LoadFwOpt1=0;
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]pre-mode is dtmb,u1LoadFwOpt1=%d\n",psDemodCtx->u1LoadFwOpt1));
	}
	else
	{		
		psDemodCtx->u1LoadFwOpt1 =1;
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]pre-mode is not dtmb,u1LoadFwOpt1=%d\n",psDemodCtx->u1LoadFwOpt1));
	}
	#endif
// r/w reg
	//u4Tmp = 0x20c87800;
	//mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x4a0, u4Tmp);	
	mcSHOW_DBG_MSG(("[DTMB_ChipInit]0x4a3 default, use fw solution\n"));
	
	//ACD setup
	#if defined (CC_MT5890)
	u4Tmp = 0x00041533;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Tmp);//set RG_DMRX_EN =0
	u4Tmp = 0x33040912;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x404, u4Tmp);//set demod input common mode
	u4Tmp = 0x49AAA434;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x408, u4Tmp);//set LDO voltage
	u4Tmp = 0x881100B0;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Tmp);//set RG_CIC_FLT_EN =0
	u4Tmp = 0x40041533;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Tmp);//set RG_DMRX_en =1
	u4Tmp = 0x883100B0;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Tmp);//set RG_CIC_FLT_EN =1
	#elif defined (CC_MT5882)	
	u4Tmp = 0x0008A894;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x504, u4Tmp);
	u4Tmp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Tmp);
	u4Tmp = 0x00000100;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Tmp);
	#endif
	//clock setting
	u4Tmp = 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x3e0, u4Tmp);
	u4Tmp = 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x308, u4Tmp);
	u4Tmp = 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x30c, u4Tmp);
	u4Tmp = 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x388, u4Tmp);
	u4Tmp = 0x00000003;
	mcSET_SHOW_REG(CKGEN_BASE, 0x38c, u4Tmp);
	u4Tmp = 0x00000200;
	mcSET_SHOW_REG(CKGEN_BASE, 0x3dc, u4Tmp);
	u4Tmp = 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x42c, u4Tmp);
	u4Tmp = 0x00000004;
	mcSET_SHOW_REG(CKGEN_BASE, 0x460, u4Tmp);
	
	u4Tmp = 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x464, u4Tmp);
	
	//IF/RF AGC
	u4Tmp = mcRISC_Reg(CKGEN_BASE, 0x604);
	mcSHOW_DBG_MSG(("[DTMB_ChipInit]RF_AGC = 0x%x\n",u4Tmp));
	if(psDemodCtx->psTunerCtx->fgRFTuner)
	{
		#if defined (CC_MT5890)
		u4Tmp |= 0x00000880;
		#elif defined (CC_MT5882)
		u4Tmp |= 0x09000000;
		#endif
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]Sawless tuner,Enable RF_AGC\n"));
	}
	else
	{
		#if defined (CC_MT5890)
		u4Tmp |= 0x00000080;// enable IF AGC
		u4Tmp &= 0xFFFFF7FF;// dis RF AGC
		#elif defined (CC_MT5882)
		//enable IF AGC
		u4Tmp |= 0x08000000; // set bit27 1
		u4Tmp &= 0xEFFFFFFF; // set bit28 0
		//dis RF AGC
		u4Tmp &= 0xF8FFFFFF; // set bit[26:24] 000
		#endif
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]Leagacy tuner,No need to enable RF_AGC\n"));
	}
	mcSET_SHOW_REG(CKGEN_BASE, 0x604, u4Tmp);
	
	// switch to DTMB mode 
	u4Tmp = 0x0000100a;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0xe00, u4Tmp);
	mcSHOW_USER_MSG(("[DTMB_ChipInit]select DTMB Mode\n"));

	#if REWRITETDI
	//re-write tdi reg	
	mcSHOW_USER_MSG(("[DTMB_ChipInit]re-write tdi reg\n"));
	u4Tmp = 0x00010308 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9b0, u4Tmp);
	u4Tmp = 0x37ffffff  ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9b4, u4Tmp);
	u4Tmp = 0x0000307b ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9b8, u4Tmp);
	u4Tmp = 0x0000ff1f ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9bc, u4Tmp);

	u4Tmp = 0x60000000 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9c0, u4Tmp);
	u4Tmp = 0x00000000 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9c4, u4Tmp);
	u4Tmp = 0x00000000 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9c8, u4Tmp);
	u4Tmp = 0x00000000 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9cc, u4Tmp);

	//u4Tmp = 0x00000000 ;
	//mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9d0, u4Tmp);	
	mcSHOW_USER_MSG(("[DTMB_ChipInit]do not re-write 9d0~9d3 \n"));
	u4Tmp = 0x07000100 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9d4, u4Tmp);
	u4Tmp = 0x00000000 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9d8, u4Tmp);
	u4Tmp = 0x06000006 ;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9dc, u4Tmp);
	//re-write tdi reg
	//DTMB_SetTdiAddr();
	#endif
	
	//reset tdi		
#if REWRITETDI
#else
	mcSHOW_DBG_MSG(("-----[DTMB_ChipInit]reset tdi replace r-w tdi regs------\n"));
	u4Tmp = mcRISC_Reg(cRISC_DTMB_BASE,0x410);	
	mcSHOW_DBG_MSG(("-----[DTMB_ChipInit]reset tdi------ read 410=0x%x\n",u4Tmp));
	u4Tmp &= 0xfeffffff;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x410, u4Tmp);
	u4Tmp = mcRISC_Reg(cRISC_DTMB_BASE,0x410);	
	mcSHOW_DBG_MSG(("-----[DTMB_ChipInit]reset tdi------ read 410=0x%x\n",u4Tmp));
	u4Tmp |= 0xff000000;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x410, u4Tmp);
	DTMB_SetTdiAddr();
#endif
	/*
	u4Tmp = 0x00000111;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0xe04, u4Tmp);// using i2c & demod_mon_sel=17
	u4Tmp = 0x00000003;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x440, u4Tmp);//select quad4_mon to dtmb_mon
	u4Tmp = 0x00000013;
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x444, u4Tmp);// select tsif output to quad4_mon
	*/

	// from dvbt,RF AGC switch
	if(DRVCUST_OptQuery(eTunerRFAGCSwitchGpio,&u4GpioforRFAGCSwitch)==0)
	{
	  GPIO_SetOut(u4GpioforRFAGCSwitch,1);//d20110503_Lei,:Gpio set high for DTV
	  mcSHOW_DBG_MSG(("GPIO %d set high to switch RF AGC control\n",u4GpioforRFAGCSwitch));
	}

#if 0
	// from 5175
	ucValue = 0xFF;//Release DTMB Quad2/3/4
	DTMB_SetReg(psDemodCtx, DTMB_QUAD_GLOBAL_0, &ucValue, 1);
	ucValue = 0x3F;//Release DTMB Quad1_CDC,CR,STR,CRANIR,CA
	DTMB_SetReg(psDemodCtx, DTMB_QUAD1_RST_B_1, &ucValue, 1);
#endif
	DTMB_SetTunerRel(psDemodCtx);

#if HOSTCMD_UP		
		DTMB_GetReg(psDemodCtx,0x00, &ucValue,1);		
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]always,get 0x00=0x%x\n",ucValue));

		
		//ucValue=0;
		//mcSHOW_DBG_MSG(("[DTMB_ChipInit]always,dis UP,set 0x00=0x%x\n",ucValue));
		//DTMB_SetReg(psDemodCtx,0x00, &ucValue,1);	
		
		//mcDELAY(10);
		//DTMB_GetReg(psDemodCtx,0x00, &ucValue,1);	
		//mcSHOW_DBG_MSG(("[DTMB_ChipInit]always,get 0x00=0x%x\n",ucValue));
		
		//mcDELAY(10);
		ucValue=1;
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]always,en UP,set 0x00=0x%x\n",ucValue));
		DTMB_SetReg(psDemodCtx,0x00, &ucValue,1);
		
		mcDELAY(10);
		DTMB_GetReg(psDemodCtx,0x00, &ucValue,1);	
		mcSHOW_DBG_MSG(("[DTMB_ChipInit]always,get 0x00=0x%x\n",ucValue));
#endif
	
	
#if defined (CC_MT5175)
UCHAR   ucValue;

	mcSHOW_DRVERR_MSG(("DTMB_ChipInit\n"));

	// register setup
	//ANALOG Setting 
	ucValue = 0x5A; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG1, &ucValue, 1); // ADC setting        
	ucValue = 0xA8; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG2, &ucValue, 1); // ADC setting        
	ucValue = 0x82; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG3, &ucValue, 1); // ADC setting        
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG0, &ucValue, 1); // PGA power on       
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG1, &ucValue, 1); // PGA power on       
	ucValue = 0x09; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG3, &ucValue, 1); // Enable BandGap     
	ucValue = 0x17; DTMB_SetReg(psDemodCtx, REG_SYSPLL_CFG2, &ucValue, 1); // set feedback divider=2*12
	ucValue = 0x78; DTMB_SetReg(psDemodCtx, REG_SYSPLL_CFG0, &ucValue, 1); // Enable bias circuit
	ucValue = 0x18; DTMB_SetReg(psDemodCtx, REG_SYSPLL_CFG0, &ucValue, 1); // SYSPLL power on
	ucValue = 0xAA; DTMB_SetReg(psDemodCtx, REG_SYSPLL_CFG6, &ucValue, 1); // DDS power on, frequency=27M

	//Clock Setting   
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, CKGEN_01, &ucValue, 1); //use ADCPLL
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_02, &ucValue, 1); //set demod_54m_ck to 54m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_03, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_08, &ucValue, 1); //set demod_36m_ck to 54m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_09, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_04, &ucValue, 1); //set demod_216m_ck to 216m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_05, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_31, &ucValue, 1); //set demod_108m_ck to 108m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_32, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_29, &ucValue, 1); //set adclk to 216m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_30, &ucValue, 1); //trigger
	ucValue = 0x20; DTMB_SetReg(psDemodCtx, CKGEN_06, &ucValue, 1); //set demod_86p4m_ck to 54m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_07, &ucValue, 1); //trigger level 1
	ucValue = 0x04; DTMB_SetReg(psDemodCtx, CKGEN_07, &ucValue, 1); //trigger level 2
	ucValue = 0x05; DTMB_SetReg(psDemodCtx, CKGEN_10, &ucValue, 1); //set ci_sys_ck to 86.4m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_11, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_12, &ucValue, 1); //set ci_tso_ck to 54m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_13, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_16, &ucValue, 1); //set spd0_ck to dmss_ck_mux
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_17, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_18, &ucValue, 1); //set spd1_ck to dmss_ck_mux
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_19, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_23, &ucValue, 1); //set spd2_ck to dmss_ck_mux
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_24, &ucValue, 1); //trigger
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_27, &ucValue, 1); //set adclk_div4_ck to 54m
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_28, &ucValue, 1); //trigger
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, MT5175_GPM_E01, &ucValue, 1); 
	ucValue = 0x80; DTMB_SetReg(psDemodCtx, MT5175_GPM_E01, &ucValue, 1);

	//Padmux and GPM Setting   
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F129, &ucValue, 1); // set tstmd_en[7:0] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F130, &ucValue, 1); // set tstmd_en[15:8] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F131, &ucValue, 1); // set tstmd_en[23:16] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F132, &ucValue, 1); // set tstmd_en[31:24] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F133, &ucValue, 1); // set tstmd_en[39:32] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F134, &ucValue, 1); // set tstmd_en[47:40] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F135, &ucValue, 1); // set tstmd_en[55:48] to 0
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F136, &ucValue, 1); // set tstmd_en[63:56] to 0
	ucValue = 0x20; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F138, &ucValue, 1); // Enable parallel TSIF output PAD;  Mingsheng 20120228 PVR TS output->input 0x21->0x20
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F109, &ucValue, 1); // Enable RF AGC control


	ucValue = 0xFF;//Release DTMB Quad2/3/4
	DTMB_SetReg(psDemodCtx, DTMB_QUAD_GLOBAL_0, &ucValue, 1);
	ucValue = 0x3F;//Release DTMB Quad1_CDC,CR,STR,CRANIR,CA
	DTMB_SetReg(psDemodCtx, DTMB_QUAD1_RST_B_1, &ucValue, 1);

	////bypass CI when MT5175 and set TS out put fmt as serial
#if defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881) || defined (CC_MT5399))   // //huiyan 20120314 for 5175+mt5398 ,LL for 5175+5881
	DTMB_GetReg(psDemodCtx, MT5175_GPM_E00, &ucValue, 1);
	ucValue &=0xEF;
	DTMB_SetReg(psDemodCtx, MT5175_GPM_E00, &ucValue, 1);            // bypass CI
	ucValue |=0x02;
	DTMB_SetReg(psDemodCtx, MT5175_GPM_E00, &ucValue, 1);
	ucValue =0x00;
	DTMB_SetReg(psDemodCtx, 0xDD0, &ucValue, 1);
	ucValue =0x00;
	DTMB_SetReg(psDemodCtx, 0xDD1, &ucValue, 1);
	
	DTMB_GetReg(psDemodCtx, REG_MT5175_PADMUX_F138, &ucValue, 1);

#if defined(CC_MT5398)||defined(CC_MT5396)
	ucValue &= 0xFC;
	DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F138, &ucValue, 1);    ///serial mode
	DTMB_GetReg(psDemodCtx,DTMB_SB_CFG0,&ucValue,1);
	ucValue &= 0xf7;
	DTMB_SetReg(psDemodCtx,DTMB_SB_CFG0,&ucValue,1);
	ucValue=0x01;
	DTMB_SetReg(psDemodCtx,DTMB_TSIF_CTRL3,&ucValue,1);	
	ucValue = 0x04;                                          //set data1~data7 as High Z  when TS output mode is serial     2012.4.14 huiyan 
	DTMB_SetReg(psDemodCtx,0x871,&ucValue,1);	
	DTMB_SetReg(psDemodCtx,0x872,&ucValue,1);	
	DTMB_SetReg(psDemodCtx,0x873,&ucValue,1);	
	DTMB_SetReg(psDemodCtx,0x874,&ucValue,1);	
	DTMB_SetReg(psDemodCtx,0x875,&ucValue,1);	
	DTMB_SetReg(psDemodCtx,0x876,&ucValue,1);
	DTMB_SetReg(psDemodCtx,0x877,&ucValue,1);
#endif	

#if defined(CC_MT5880) || defined(CC_MT5881) ||defined(CC_MT5399)//LL for 75+5881
	ucValue &= 0xFC;
	ucValue |= 0x01;
	DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F138, &ucValue, 1);    ///parallel mode for Viper and Python

	DTMB_GetReg(psDemodCtx,DTMB_SB_CFG0,&ucValue,1);
	if( ucValue & 0x80 )
	{
		ucValue |=0x08;
		DTMB_SetReg(psDemodCtx,DTMB_SB_CFG0,&ucValue,1);
		
	}
	else	//fix:change channel stress test fail. 3 regs is changed.
	{	
		ucValue = 0x8F;
		DTMB_SetReg(psDemodCtx,DTMB_SB_CFG0,&ucValue,1);
		ucValue = 0x0A;
		DTMB_SetReg(psDemodCtx,0x5B1,&ucValue,1);
		ucValue = 0x09;
		DTMB_SetReg(psDemodCtx,0x5B8,&ucValue,1);
	}
#endif
	
#endif

	//AGC crrve setting
	DTMB_SetTunerRel(psDemodCtx);
#endif
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if(ICtrlBus_DemodWrite(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG(("Demod Write Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    if(ICtrlBus_DemodRead(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" Demod Read Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_DemodCtxCreate
 *  This function creates demod context and returns the context pointer.
 *  @param  void.
 *  @retval   DEMOD_CTX_T * : pointer of demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *DTMB_DemodCtxCreate(void)
{
    DEMOD_CTX_T *p = NULL;

    CREATE_OBJECT(p, DEMOD_CTX_T);

    // error handling
    if (!p)
       return NULL;
    
    // zero memory
    memset(p, 0, sizeof(DEMOD_CTX_T));

    return p;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_DemodCtxDestroy
 *  This function destroys context you have created and releases the memory.
 *  @param  p  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_DemodCtxDestroy(DEMOD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetDemodReg
 *  This function will read demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DTMB_GetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucLen - 1, ccOFST_REG1_SIZE+1))
    {
        for (ii = 0; ii < ucLen; ii++)
             *(pucBuf+ii) = mccGET_QUEUE(aucData, ccOFST_REG1_DATA+ii);
         return 0;
    }

    return 1;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetDemodReg
 *  This function will write demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DTMB_SetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii = 0;

    if (!psDemodCtx)
        return 1;
    if ( (ucLen == 0) || (ucLen > 4) )
        return 1;
    if (!pucBuf)
        return 1;

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_REG;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucLen;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE( u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    for (ii = 0; ii < ucLen; ii++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA+ii) = *(pucBuf+ii);

    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE+ucLen-1+1))
         return 0;

    return 1;
}



//justin
//----------------------------------------------------------------------------- 
/** download image

 *  @param  
 *  @param  
 *  @retval   others : Error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_Loadcode(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen)
{

if(ICtrlBus_DemodLoadCode(psDemodCtx->I2cAddress, 	DTMB_REG_uP_IRData, pu1Buffer, u2CurRomLen))
{
	/*fail case*/
	mcSHOW_POP_MSG(("Download Code fail!\n"));
	return 1;
}

return 0;

}
/************************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  fgHostCmdDTMB
 *  Host command interface API between host and demod.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  aucCmdData  Data buffer.
 *  @param  ucReadParacnt  Read parameter byte counts.
 *  @param  ucWriteCmdcnt  Write command byte counts.
 *  @retval   TRUE : Host command success.
 *  @retval   FALSE : Host command fail.   
 */
//-----------------------------------------------------------------------------
//BOOL fgHostCmd(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
//rename to fix build error "API multiply defined" when build with DTD on iDTV
BOOL fgHostCmdDTMB(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
{
UCHAR   ucWriteParacnt;
UINT16  ii;
UINT8   ucValue;
UCHAR   ucParaStart, ucRegSetOfst;
BOOL    fgRetStatus = TRUE;
#if fcMEAS_HOST_CMD_TIME
UINT32  u4TickStart;
UINT32  u4TickCnt;
#endif
UCHAR   ucReadChkCnt;
/*#if fcCHIP_TYPE
UCHAR   ucChipID;
#endif*/
#if fcADD_HOSTCMD_CHKSUM
UCHAR   ucChkSum;
#endif
UCHAR   jj;

#if fcMEAS_HOST_CMD_TIME
    u4TickStart = mcGET_SYS_TICK();
#endif

    if (DTMB_GetReg(psDemodCtx, MT5391_REG_uP_Ctrl, &ucValue, 1))
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG(("[fgHostCmdDTMB]I2C fail _I2_ "));

        return FALSE;                       // I2C fail
    }
    if (!mcTEST_BIT(ucValue, ccpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));
        mcSHOW_HW_MSG(("[fgHostCmdDTMB]uP not enabled _UE_ "));
        
        return FALSE;
    }
    if (DTMB_GetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1))
    {
        return FALSE;                       // I2C fail
    }
    if (ucValue != ccFW_RUN_TAG)
    {
        psDemodCtx->u1UpChkCnt ++;
        if (ucValue == ccFW_WDT_TAG)
        {
        //    mcSHOW_HW_MSG(("Watchdog reset!\n"));
            mcSHOW_HW_MSG((" [fgHostCmdDTMB]Watchdog reset _WR_ "));
        }
        else
        {
        //    mcSHOW_HW_MSG(("Fw not running!\n"));
            mcSHOW_HW_MSG(("[fgHostCmdDTMB]Fw not running! _FR_ "));
        }
        return FALSE;
    }

    /*
    #if fcCHIP_TYPE
        // Get Chip Type
        DVBT_GetReg(psDemodCtx, MT5391_REG_Chip_ID, &ucChipID, 1);
        ucChipID &= mcMASKS(4, 4);
    #endif
    */
        if (ucWriteCmdcnt)
        {
            ucWriteParacnt = ucWriteCmdcnt - 1;
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
            {
            #if 1
                DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
            #else
                if (DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1))
                {                               // I2C fail
                    if (ii < (cMAX_WAIT_LOOP - 1))
                    {
                        mcDELAY_MS(cWAIT_TIME_INTERVAL);
                        continue;
                    }
                
                    return FALSE;
                }
            #endif
            // This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
            #if 1
                jj = 0;
                while (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
                {
                /*#if fcCHIP_TYPE
                    if (ucChipID == CHIP_TYPE_1)
                        DVBT_GetReg(psDemodCtx, MT5391_REG_RFA4CF, &ucValue, 1);
                    else
                #endif*/
                    DTMB_GetReg(psDemodCtx, MT5391_REG_RSICIF_RSP7, &ucValue, 1);
                    DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
                    jj ++;
                    if (jj >= cMAX_WAIT_LOOP)
                        break;
                }
            #endif
                if (!mcTEST_BIT(ucValue, ccpUP_CMD_RX))
                    break;
                mcDELAY_MS(cWAIT_TIME_INTERVAL);
            }
        #if fcMEAS_HOST_CMD_TIME
            u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
            mcSHOW_TIME_MSG((" ** Cmd%02X %d (%3u)", mccSET_CMD2(aucCmdData),
                            ii, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif
            if (ucWriteParacnt > ccCMD_REG_NUM - 1)
            {
                mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d > %d!",
                                mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
                ucWriteParacnt = ccCMD_REG_NUM - 1;
            }
            #if fcADD_HOSTCMD_CHKSUM
            if (ucWriteParacnt == (ccCMD_REG_NUM - 1))
            {
                mcSHOW_HW_MSG(("Cmd %02X Para Cnt %d == %d!",
                                mccSET_CMD2(aucCmdData), ucWriteParacnt, ccCMD_REG_NUM - 1));
            }
            else
            {
                ucChkSum = 0;
                for (ii = 0; ii < ccCMD_REG_NUM - 1; ii ++)
                {
                    ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii + 1];
                }
                aucCmdData[1] = ucChkSum;
                ucWriteParacnt = ccCMD_REG_NUM - 1;
            }
            #endif
            if (ii == cMAX_WAIT_LOOP)
                psDemodCtx->u1UpNakCnt ++;
    
            ucParaStart = ccCMD_REG_NUM - ucWriteParacnt - 1;
            ucRegSetOfst = 1;

        /*#if fcCHIP_TYPE
            if (ucChipID == CHIP_TYPE_1)
                DVBT_SetReg(psDemodCtx, MT5391_REG_RFA4C0 + ucParaStart,
                              aucCmdData + ucParaStart + ucRegSetOfst,
                              ucWriteParacnt + 1);
            else
        #endif*/
                DTMB_SetReg(psDemodCtx, (UINT16) (MT5391_REG_RSICIF_CMD0 + ucParaStart),
                              aucCmdData + ucParaStart + ucRegSetOfst,
                              ucWriteParacnt + 1);
        #if fcMEAS_HOST_CMD_TIME
            u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
            mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif
        #if 0//fcSHOW_HOST_CMD
            if (fgShowHostCmd)
            {
            //    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
                mcSHOW_USER_MSG(("HostCmd -->"));
                for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
                    mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
                mcSHOW_USER_MSG(("\n           "));
                DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR00 + ucParaStart,
                          aucCmdData + ucParaStart + ucRegSetOfst,
                          ucWriteParacnt + 1);
                for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
                    mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
                mcSHOW_USER_MSG(("\n"));
            }
        #endif
        }

        if (ucReadParacnt)
        {
        UCHAR   ucCmdId;

            ucCmdId = mccSET_CMD2(aucCmdData);
            fgRetStatus = FALSE;
            for (jj = 0; jj < cMAX_TRY_CNT; jj ++)
    //        for (jj = 0; jj < 1; jj ++)
            {
                for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
                {
                #if 1
                    DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1);
                #else
                    if (DTMB_GetReg(psDemodCtx, MT5391_REG_uP_CMDR_CFG, &ucValue, 1))
                    {                           // I2C fail
                        if (ii < (cMAX_WAIT_LOOP - 1))
                        {
                            mcDELAY_MS(cSHORT_SLEEP_TIME);
                            continue;
                        }
                        return FALSE;
                    }
                #endif
                    if (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
                        break;
                    //mcDELAY_MS(cSHORT_SLEEP_TIME);
                    mcDELAY_MS(1);//dtmb time improve
                    //HAL_Delay_us(10000);// temp5391
                }
            //    Get(MT5391_REG_uP_RSPR00 + cPARA_NUM_POS, &ucReadParacnt, 1);
                if (ucReadParacnt > ccCMD_REG_NUM - 1)
                {
                    mcSHOW_HW_MSG(("Rsp %02X Para Cnt %d > %d!",
                                    mccSET_CMD2(aucCmdData), ucReadParacnt, ccCMD_REG_NUM - 1));
                    ucReadParacnt = ccCMD_REG_NUM - 1;
                }
                ucReadChkCnt = ucReadParacnt;
            #if fcADD_HOSTCMD_CHKSUM
                if (ucReadChkCnt < ccCMD_REG_NUM - 1)
                    ucReadChkCnt ++;
            #endif
                if (ii < cMAX_WAIT_LOOP)
                {
                UINT16  u2AddrOfst = 0;

                #if fcMEAS_HOST_CMD_TIME
                    u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                    mcSHOW_TIME_MSG((" Rsp %d_%d (%3u)", jj, ii, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif
                    ucParaStart = ccCMD_REG_NUM - ucReadChkCnt - 1;

                /*#if fcCHIP_TYPE
                    if (ucChipID == CHIP_TYPE_1)
                        u2AddrOfst = MT5391_REG_RFA4C8 - MT5391_REG_uP_RSPR00;
                #endif*/
                    if (DTMB_GetReg(psDemodCtx,
                                      (UINT16) (MT5391_REG_RSICIF_RSP0 + u2AddrOfst + ucParaStart),
                                      aucCmdData + ucParaStart, ucReadChkCnt + 1))
                    {
                        return FALSE;
                    }
                #if fcMEAS_HOST_CMD_TIME
                    u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
                    mcSHOW_TIME_MSG((" Para %d (%3u)", ucReadChkCnt, u4TickCnt * mcGET_TICK_PERIOD()));
                #endif
                    if (mccGET_CMD(aucCmdData) == ucCmdId)
                    {
                        fgRetStatus = TRUE;
                #if 0//fcSHOW_HOST_CMD
                        if (fgShowHostCmd)
                        {
                            mcSHOW_USER_MSG(("HostRsp -->"));
                            for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                            mcSHOW_USER_MSG(("\n"));
                        }
                #endif
                #if fcADD_HOSTCMD_CHKSUM
                        if (ucReadParacnt < ccCMD_REG_NUM - 1)
                        {
                            ucChkSum = 0;
                            for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                            {
                                ucChkSum ^= aucCmdData[ccCMD_REG_NUM - 1 - ii];
                            }
                            if (ucChkSum)
                            {
                                fgRetStatus = FALSE;
                                mcSHOW_USER_MSG((" _@CS_ "));
                                DTMB_GetReg(psDemodCtx, MT5391_REG_uP_RSPR00 + ucParaStart,
                                              aucCmdData + ucParaStart, ucReadChkCnt + 1);
                                mcSHOW_USER_MSG(("-->"));
                                for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                                {
                                    mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                                }
                                mcSHOW_USER_MSG(("\n"));
                                psDemodCtx->u1UpMisCnt ++;
                            }
                        }
                #endif
                        break;
                    }
                    else
                        psDemodCtx->u1UpMisCnt ++;
                }
                else
                {
                    psDemodCtx->u1UpNakCnt ++;
                    break;
                }
            }
        //    mcSHOW_HW_MSG(("Error uP Rsp!"));
        }

    #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
    //    mcSHOW_DBG_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
        mcSHOW_TIME_MSG((" %3uH ", u4TickCnt * mcGET_TICK_PERIOD()));
        mcSHOW_TIME_MSG(("\n"));
    #endif

        return fgRetStatus;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDtmbGetFwVars
 *  Get firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDtmbGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (ucCnt > 4) { // for klocwork warning
        return FALSE;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_GET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    {
        for (ii = 0; ii < ucCnt; ii ++)
            pucData[ii] = mccGET_QUEUE(aucData, ccOFST_REG1_DATA + ii);
    }
    else
        return FALSE;
    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  ucGetFwVarDTMB
 *  Get firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @retval   1-byte data.   
 */
//-----------------------------------------------------------------------------
//rename to fix build error "API multiply defined" when build with DTD on iDTV
UINT8 ucGetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
UCHAR   ucValue = 0;

    fgDtmbGetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}

//----------------------------------------------------------------------------- 
/*
 *  fgDtmbSetFwVars
 *  Set firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDtmbSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if (ucCnt > 4) { // for klocwork warning
        return FALSE;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DEMOD_SET_VAR;
    mccSET_QUEUE2(aucData, ccOFST_REG_NUM) = ucCnt;
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_L) = mcLOW_BYTE(u2Addr);
    //mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = mcHIGH_BYTE(u2Addr);
    mccSET_QUEUE2(aucData, ccOFST_REG1_ADDR_H) = 0;
    for (ii = 0; ii < ucCnt; ii ++)
        mccSET_QUEUE2(aucData, ccOFST_REG1_DATA + ii) = pucData[ii];
    //if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_REG1_SIZE + ucCnt - 1, ccOFST_REG1_SIZE + 1))
    if (mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_REG1_SIZE + ucCnt -1 + 1))
    {
        return TRUE;
    }
    return FALSE;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetFwVar
 *  Set firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  ucValue  data to be set
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vSetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UINT8 ucValue)
{
    fgDtmbSetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
}

/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/

/**********************************************************************/
/*                  Initialization APIs                               */
/**********************************************************************/

 BOOL fgDTMB_FwStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
	UINT8 ii;
	UINT8 ucValue;
    mccSET_CMD2(aucData) = ccCMD_ID_DMBTC_SWITCH;
	mccSET_QUEUE2(aucData, ccOFST_DMBTC_SWITCH) = cCONN_TYPE_DTMB;
    if(mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DMBTC_SWITCH_SIZE+1))
    {
		
        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DTMB_GetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }
        mcSHOW_USER_MSG(("[DTMB_FwSwitch]FW switch to DTMB!\n"));

    }

    return TRUE;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_MemoryRepair
 *  This function perform memory repair before DTMB_ChipInit & s4LoadCode.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
/*
UINT8 DTMB_MemoryRepair(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR   ucValue;

	UCHAR	ucCheck[3];
	
	//ANALOG Setting 
	ucValue = 0x5A; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG1, &ucValue, 1); // ADC setting        
	ucValue = 0xA8; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG2, &ucValue, 1); // ADC setting        
	ucValue = 0x82; DTMB_SetReg(psDemodCtx, REG_IFADC_CFG3, &ucValue, 1); // ADC setting        
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG0, &ucValue, 1); // PGA power on       
	ucValue = 0x00; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG1, &ucValue, 1); // PGA power on       
	ucValue = 0x09; DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG3, &ucValue, 1); // Enable BandGap     
	ucValue = 0x7C; DTMB_SetReg(psDemodCtx, REG_SYSPLL_CFG0, &ucValue, 1); // Enable bias circuit

	//Set Clock		
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_02, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_03, &ucValue, 1);
	ucValue = 0x03; DTMB_SetReg(psDemodCtx, CKGEN_06, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_07, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_31, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_32, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_04, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_05, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_10, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_11, &ucValue, 1);
	ucValue = 0x05; DTMB_SetReg(psDemodCtx, CKGEN_23, &ucValue, 1);
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, CKGEN_24, &ucValue, 1);
	ucValue = 0x60; DTMB_SetReg(psDemodCtx, 0xDD2, &ucValue, 1);
			
	// Mbist Reset Release		
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E10, &ucValue, 1);
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E11, &ucValue, 1);
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E12, &ucValue, 1);
			
	//Enable Hardware Repair Mode		
	ucValue = 0xf6; DTMB_SetReg(psDemodCtx, 0xAA1, &ucValue, 1);
	ucValue = 0xf6; DTMB_SetReg(psDemodCtx, 0xAB3, &ucValue, 1);
			
	//Mbist enable
	ucValue = 0x03; DTMB_SetReg(psDemodCtx, 0xAB4, &ucValue, 1);
	ucValue = 0x03; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E10, &ucValue, 1);
	ucValue = 0xD0; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E11, &ucValue, 1);
			
	//Start Hardware Repair		
	ucValue = 0xfe; DTMB_SetReg(psDemodCtx, 0xAA1, &ucValue, 1);
	ucValue = 0xfe; DTMB_SetReg(psDemodCtx, 0xAB3, &ucValue, 1);

	DTMB_GetReg(psDemodCtx, MT5175_MBIST_E1A, &ucCheck[0], 1);
	DTMB_GetReg(psDemodCtx, MT5175_MBIST_E1D, &ucCheck[1], 1);
	DTMB_GetReg(psDemodCtx, MT5175_MBIST_E20, &ucCheck[2], 1);

	while(!mcTEST_BIT(ucCheck[0], 7) || !mcTEST_BIT(ucCheck[1], 7) || !mcTEST_BIT(ucCheck[2], 7))
	{
		DTMB_GetReg(psDemodCtx, MT5175_MBIST_E1A, &ucCheck[0], 1);
		DTMB_GetReg(psDemodCtx, MT5175_MBIST_E1D, &ucCheck[1], 1);
		DTMB_GetReg(psDemodCtx, MT5175_MBIST_E20, &ucCheck[2], 1);
	}

	//MBist disable
	ucValue = 0x01; DTMB_SetReg(psDemodCtx, 0xAB4, &ucValue, 1);
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E10, &ucValue, 1);
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E11, &ucValue, 1);
	ucValue = 0x55; DTMB_SetReg(psDemodCtx, MT5175_MBIST_E12, &ucValue, 1);

	return 0;

}
*/
//----------------------------------------------------------------------------- 
/** DTMB_SetEarlyBreak
 *  Set early break time.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DTMB_SetEarlyBreak(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_SET_EARLY_BREAK;
    mccSET_QUEUE2(aucData, ccOFST_DTMB_SET_EARLY_BREAK_TIME_H) = 0x01;
	mccSET_QUEUE2(aucData, ccOFST_DTMB_SET_EARLY_BREAK_TIME_L) = 0x5E; //350ms
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DTMB_SET_EARLY_BREAK_SIZE+1);
}

//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode
 *  Download firmware code.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u1CodeImage  Pointer of firmware code image.
 *  @param  u2CurRomLen  Firmware code size in byte.
 *  @param  u2MaxI2cLen  Maximum I2C length per transfer.
 *  @param  fgClk50  Flag to switch if 48MHz or 24MHz clock is used.
 *  @retval   cUP_LOAD_OK : Load code OK.
 *  @retval   cUP_LOAD_ERR_I2C : I2C error.
 *  @retval   cUP_LOAD_ERR_HW_RDY : HW not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_RDY : Checksum not ready.
 *  @retval   cUP_LOAD_ERR_CHKSUM_OK : Checksum not OK.   
 */
//-----------------------------------------------------------------------------
static INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage, UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
//#if (!defined(CC_MT5175))||(defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399))) //LL for 5175+5881
	UCHAR	ucI2cAuto;
//#endif
    UCHAR   ucValue, ucUpCtrl, ii;
    UINT16  u2CurRamAdr;
#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif
    UINT16  u2SramSize;


    DTMB_GetReg(psDemodCtx, DTMB_REG_uP_GP_REG06, &ucUpCtrl, 1);
    if(ucUpCtrl == ccFW_RUN_TAG)
    {
        mcSHOW_POP_MSG(("[DTMB]FW has download,force load dtmb fw\n"));
        //return cUP_LOAD_OK;
    }
    if (DTMB_GetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
	 mcSHOW_USER_MSG(("[DTMB]s4LoadCodeI2C OK \n"));

	 ///////////////// Option 1:check sum before load bin, no more need this
	 /*
	 if(psDemodCtx->u1LoadFwOpt1)
	 {
	 ucUpCtrl = 0;
	 DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
	 
	 ucUpCtrl = mcBIT(ccpUP_RESTART_CHKSUM) | mcBIT(ccpUP_ROMCODE_CHKSUM);
	 DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
	 for (ii = 0; ii < 8; ii++)
	 {
		 mcDELAY_MS(10);						// Unit: ms
		 DTMB_GetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
		 if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
			 break;
	 }
	 if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
	 {
		 mcSHOW_USER_MSG(("[DTMB s4LoadCode]Load FW ROM checksum not ready\n"));
		 return cUP_LOAD_ERR_ROM_CHKSUM_RDY;
	 }
	 if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
	 {
		 mcSHOW_USER_MSG(("[DTMB s4LoadCode]Load FW ROM checksum not OK\n"));
		 return cUP_LOAD_ERR_ROM_CHKSUM_OK;
	 }
	 }
	 //////////////////////////////////////
	 */

	 ////////////Option 2:sram clear,soc need this
	 if(1/*psDemodCtx->u1LoadFwOpt2*/)
	 {
	ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
	 }
	 /////////////////////////////////
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
		mcSHOW_USER_MSG(("[DTMB]s4LoadCode HW NOT READY!!!\n"));
        return cUP_LOAD_ERR_HW_RDY;
    }
	mcSHOW_USER_MSG(("[DTMB]s4LoadCode HW READY\n"));

    ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_GP_REG06, &ucValue, 1);

    for (ii = 0; ii < 4; ii ++)
    {
        DTMB_GetReg(psDemodCtx, (UINT16) (DTMB_REG_uP_PNGP_H + ii), &ucValue, 1);
        DTMB_SetReg(psDemodCtx, (UINT16) (DTMB_REG_uP_PNGP_H + ii), &ucValue, 1);
    }

    // set max code size
//#if defined(CC_MT5175) 
  // u2SramSize = 32 * 1024 - 1;//from Hans toolx pi_demod.c code
   u2SramSize = u2CurRomLen;
//#endif
    //u2SramSize = (UINT16)(psDemodCtx->ucSramSize) * 1024 - 1;
    ucValue = mcHIGH_BYTE(u2SramSize);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_LPA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2SramSize);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_LPA_L, &ucValue, 1);
/*
#if (!defined(CC_MT5175))||(defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)))//LL for 5175+5881
// Turn off I2C auto-increment
    DTMB_GetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DTMB_SetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif
*/
// dtmb soc 
DTMB_GetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);
mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
DTMB_SetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    u2CurRamAdr = 0;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_IRA_L, &ucValue, 1);
    u2CurRamAdr = 0;

	if (DTMB_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("[DTMB s4LoadCode]Download I2C fail! (%d)\n"));
        return 1;
    }
	mcSHOW_USER_MSG(("[DTMB]s4LoadCode Download I2C OK\n"));

/*
#if (!defined(CC_MT5175))||(defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)))//LL for 5175+5881
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DTMB_SetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif
*/
	// dtmb soc 
	mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DTMB_SetReg(psDemodCtx, DTMB_REG_I2C_Cfg, &ucI2cAuto, 1);

    ucUpCtrl = 0;
    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DTMB_GetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }
	 mcSHOW_USER_MSG(("[DTMB]s4LoadCode CHKSUM READY\n"));
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_OK;
    }
	mcSHOW_USER_MSG(("[DTMB]s4LoadCode CHKSUM OK\n"));

    ucUpCtrl = mcBIT(ccpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);    // 25 MHz Clock

    DTMB_SetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucUpCtrl, 1);
	DTMB_GetReg(psDemodCtx, DTMB_REG_uP_Ctrl, &ucValue, 1);

    // delay for waiting FW initialization done, may have I2C timeout at first
    for (ii = 0; ii < 5; ii++)
    {
        mcDELAY_MS(3);
        DTMB_GetReg(psDemodCtx, DTMB_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == ccFW_RUN_TAG)
            break;
    }

    // enable HOST_EN for host to be able to access cmd/rsp register
  //  ucValue = 0x01;
 //   DTMB_SetReg(psDemodCtx, DTMB_REG_RFA4A0, &ucValue, 1);

	//fgDTMB_FwStart(psDemodCtx);

#if fcLOAD_CODE_TIME
    mcGET_SYS_TIME(u4TickCur);
    mcGET_DIFF_TIME(u4TickCnt, u4TickStart, u4TickCur);
    u4TickCnt = mcCONV_SYS_TIME(u4TickCnt);
    mcSHOW_DBG_MSG((" (%3u ms) ", u4TickCnt));
    if (u4TickCnt > 0)
        return -((INT32) u4TickCnt);
    else
#endif

    return cUP_LOAD_OK;


}


//----------------------------------------------------------------------------- 
/*
 *  vDTMBShowDbgMsg
 *  Show Some Debug Msg .
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------

void vDTMBShowDbgMsg(DEMOD_CTX_T *psDemodCtx)
{

	switch (psDemodCtx->u1Mod)
	{
		case constellation_qam4NR: mcSHOW_DBG_MSG3(("QamMode = 4QAM_NR\n"));	 break;
		case constellation_qam4: mcSHOW_DBG_MSG3(("QamMode = 4QAM\n"));    break;
		case constellation_qam16: mcSHOW_DBG_MSG3(("QamMode = 16QAM\n"));	 break;
		case constellation_qam32: mcSHOW_DBG_MSG3(("QamMode = 32QAM\n"));	 break;
		case constellation_qam64: mcSHOW_DBG_MSG3(("QamMode = 64QAM\n"));	 break;
		default: mcSHOW_DBG_MSG3(("QamMode = unknow QAM\n")); break;
	}
	switch (psDemodCtx->u1GI)
	{
		case GuardIntvl_PN420: mcSHOW_DBG_MSG3(("PN mode = PN420\n")); break;
		case GuardIntvl_PN595: mcSHOW_DBG_MSG3(("PN mode = PN595\n")); break;
		case GuardIntvl_PN945: mcSHOW_DBG_MSG3(("PN mode = PN945\n"));	break;
	
		default: mcSHOW_DBG_MSG3(("PN mode = unknow\n")); break;
	}
	switch (psDemodCtx->u1CR)
	{
		case FecRateLow: mcSHOW_DBG_MSG3(("Code Rate = 0.4\n"));  break;
		case FecRateMiddle: mcSHOW_DBG_MSG3(("Code Rate = 0.6\n"));  break;
		case FecRateHigh: mcSHOW_DBG_MSG3(("Code Rate = 0.8\n"));  break;
		default: mcSHOW_DBG_MSG3(("Code Rate = unknow\n")); break;
	}
	switch (psDemodCtx->ucCarrMode)
	{
		case CarrierMode_MC: mcSHOW_DBG_MSG3(("Carrier Mode = 3780\n"));  break;
		case CarrierMode_SC: mcSHOW_DBG_MSG3(("Carrier Mode = 1\n"));  break; 
		default: mcSHOW_DBG_MSG3(("Carrier Mode =  unknow\n")); break;
	}
	switch (psDemodCtx->ucItlvMode)
	{
		case TimDeInterl_240: mcSHOW_DBG_MSG3(("ItlvMode Mode = 240\n"));  break;
		case TimDeInterl_720: mcSHOW_DBG_MSG3(("ItlvMode Mode = 720\n"));  break; 
		default: mcSHOW_DBG_MSG3(("ItlvMode Mode =	unknow\n")); break;
	}

	if (psDemodCtx->fgShowDbgMsg)
	{
		mcSHOW_DBG_MSG3(("CFO = %dKHz\n", DTMB_GetFreqOffset_kHz(psDemodCtx)));
		mcSHOW_DBG_MSG3(("TFO = %5dppm\n", (psDemodCtx->s4Tfo)/256));
	    DTMB_GetSignalSNR(psDemodCtx);
		mcSHOW_DBG_MSG3(("Total Pkt = %7d\n",psDemodCtx->u2TsifTotalPkt));
		mcSHOW_DBG_MSG3(("Err   Pkt = %7d\n",psDemodCtx->u2TsifErrorPkt));
		mcSHOW_DBG_MSG3(("LDPC Iter = %d\n", psDemodCtx->ucLdpc_Iter));	
		mcSHOW_DBG_MSG3(("SLD PRA State = %d\n",psDemodCtx->ucSldPraState));
		mcSHOW_DBG_MSG3(("SLD RFAGC CW = %d / %d\n",psDemodCtx->s2SldPraRfagcCtrlWord, (psDemodCtx->s2SldPraRfagcCtrlWord)/256));
		mcSHOW_DBG_MSG3(("SLD IFAGC CW = %d / %d\n",psDemodCtx->s2SldPraIfagcCtrlWord, (psDemodCtx->s2SldPraIfagcCtrlWord)/256));
		mcSHOW_DBG_MSG3(("SLD Gain Idx = %d\n",psDemodCtx->ucSldPraGainIdx));
		mcSHOW_DBG_MSG3(("PRA Mag = %d / %d\n",psDemodCtx->s2LegPraOut, psDemodCtx->s2SldPraOut));

		if(psDemodCtx->u1DagcGainShift <= 128)
		{
			mcSHOW_DBG_MSG3(("LDAGC Gain Shift = %d\n",psDemodCtx->u1DagcGainShift));
		}
		else
		{
			mcSHOW_DBG_MSG3(("LDAGC Gain Shift = %d\n",(psDemodCtx->u1DagcGainShift)-256));
		}

		mcSHOW_DBG_MSG3(("LDAGC Gain Norm = %d\n",(psDemodCtx->u1DagcGainNorm)/64));


		switch (psDemodCtx->ucCaStat)
		{
			case CA_State_PMS: mcSHOW_DBG_MSG3(("CA State : PMS\n"));  break;
			case CA_State_CFO_Seek: mcSHOW_DBG_MSG3(("CA State : CFO_Seek\n"));  break;
			case CA_State_CFO_Check: mcSHOW_DBG_MSG3(("CA State : CFO_Check\n"));  break;
			case CA_State_PN_Transient: mcSHOW_DBG_MSG3(("CA State : PN_Transient\n"));  break;
			case CA_State_PN_Peak_Centering: mcSHOW_DBG_MSG3(("CA State : PN_Peak_Centering\n"));  break;
			case CA_State_PN_Phase_Acquire: mcSHOW_DBG_MSG3(("CA State : PN_Phase_Acquire\n"));  break;
			case CA_State_PN_Check: mcSHOW_DBG_MSG3(("CA State : PN_Check\n"));  break;
			case CA_State_PN_Track: mcSHOW_DBG_MSG3(("CA State : PN_Track\n"));  break;
			default: mcSHOW_DBG_MSG3(("CA State : unknow\n")); break;
		}

		switch (psDemodCtx->ucCrStat)
		{
			case CR_State_NCR_CA_CFO: mcSHOW_DBG_MSG3(("CR State : NCR_CA_CFO\n"));  break;
			case CR_State_NCR_FFT_CFO: mcSHOW_DBG_MSG3(("CR State : NCR_FFT_CFO\n"));  break;
			case CR_State_NCR_Intra_Frame: mcSHOW_DBG_MSG3(("CR State : NCR_Intra_Frame\n"));  break;
			case CR_State_NCR_Inter_Frame: mcSHOW_DBG_MSG3(("CR State : NCR_Inter_Frame\n"));  break;
			default: mcSHOW_DBG_MSG3(("CR State : unknow\n")); break;
		}

		switch (psDemodCtx->ucChSpeed)
		{
			case CH_Speed_Static: mcSHOW_DBG_MSG3(("CH Speed : Static\n"));  break;
			case CH_Speed_Low: mcSHOW_DBG_MSG3(("CH Speed : Low\n"));  break;
			case CH_Speed_Medium: mcSHOW_DBG_MSG3(("CH Speed : Medium\n"));  break;
			case CH_Speed_High: mcSHOW_DBG_MSG3(("CH Speed : High\n"));  break;
			default: mcSHOW_DBG_MSG3(("CH Speed : unknow\n")); break;
		}

		if ( (!mcTEST_BIT(psDemodCtx->ucDoppInd, 7)) && (mcTEST_BIT(psDemodCtx->ucDoppInd, 4)))
		{
			mcSHOW_DBG_MSG3(("Dopp Ind : High\n"));

		}
		else if ( (!mcTEST_BIT(psDemodCtx->ucDoppInd, 6)) && (mcTEST_BIT(psDemodCtx->ucDoppInd, 3)))
		{
			mcSHOW_DBG_MSG3(("Dopp Ind : Medium\n"));

		}
		else if ( (!mcTEST_BIT(psDemodCtx->ucDoppInd, 5)) && (mcTEST_BIT(psDemodCtx->ucDoppInd, 2)))
		{
			mcSHOW_DBG_MSG3(("Dopp Ind : Low\n"));

		}
		else
		{
			mcSHOW_DBG_MSG3(("Dopp Ind : Static\n"));
		}

		mcSHOW_DBG_MSG3(("Pre  Echo : %d\n",psDemodCtx->s2PreEcho));
		mcSHOW_DBG_MSG3(("Post Echo : %d\n",psDemodCtx->s2PostEcho));

		switch (psDemodCtx->ucSel_Chir)
		{
			case Sel_Chir_Avg: mcSHOW_DBG_MSG3(("Sel Chir : Avg\n"));  break;
			case Sel_Chir_Cur: mcSHOW_DBG_MSG3(("Sel Chir : Cur\n"));  break;
			default: mcSHOW_DBG_MSG3(("Sel Chir : unknow\n")); break;
		}

		mcSHOW_DBG_MSG3(("ReACQ Count = %d\n",psDemodCtx->u1ReacqCnt));
	    
		if (psDemodCtx->ucCarrMode == 1) // C1 mode
	    { 
			mcSHOW_DBG_MSG3(("FFF_SP_NUM = %d\n",psDemodCtx->ucFffSpNum));
			mcSHOW_DBG_MSG3(("FBF_SP_NUM = %d\n",psDemodCtx->ucFbfSpNum));
			mcSHOW_DBG_MSG3(("DD_SNR     = %d\n",(psDemodCtx->u2Dd_Snr)/64));
	    }

	}


}


//----------------------------------------------------------------------------- 
/*
 *  vDTMBGetStatus
 *  Get demod status and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDTMBGetStatus(DEMOD_CTX_T *psDemodCtx)
{

    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ucData[2];

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_STATUS0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_STATUS0_RSIZE, 1))
    {
        ucData[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_STATUS00);

        psDemodCtx->fgPraLock    = mcTEST_BIT(ucData[0], 0);
        psDemodCtx->fgLDAGCLock  = mcTEST_BIT(ucData[0], 1);
        psDemodCtx->fgCALock	 = mcTEST_BIT(ucData[0], 2);
        psDemodCtx->fgEQLock	 = mcTEST_BIT(ucData[0], 3);
        psDemodCtx->fgLDPCLock   = mcTEST_BIT(ucData[0], 5);
        psDemodCtx->fgTRKStatus  = mcTEST_BIT(ucData[0], 6);

		ucData[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_STATUS01);
		psDemodCtx->fgNoSignal   = mcTEST_BIT(ucData[0], 0);

		ucData[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_STATUS02);
		psDemodCtx->fgTsLock     = mcTEST_BIT(ucData[0], 0);;


    }
	else		
	{
		mcSHOW_DBG_MSG(("[vDTMBGetStatus]hostcmd fail\n"));
	}

	mcSHOW_DBG_MSG3(("fgPraLock=0x%0x\n",psDemodCtx->fgPraLock ));
	mcSHOW_DBG_MSG3(("fgLDAGCLock=0x%0x\n",psDemodCtx->fgLDAGCLock ));
	mcSHOW_DBG_MSG3(("fgCALock=0x%0x\n",psDemodCtx->fgCALock ));
	mcSHOW_DBG_MSG3(("fgEQLock=0x%0x\n",psDemodCtx->fgEQLock ));
	mcSHOW_DBG_MSG3(("fgLDPCLock=0x%0x\n",psDemodCtx->fgLDPCLock ));
	mcSHOW_DBG_MSG3(("fgTRKStatus=0x%0x\n",psDemodCtx->fgTRKStatus ));
	mcSHOW_DBG_MSG3(("fgNoSignal=0x%0x\n",psDemodCtx->fgNoSignal ));
	mcSHOW_DBG_MSG3(("fgTsLock=0x%0x\n",psDemodCtx->fgTsLock ));

}

//----------------------------------------------------------------------------- 
/*
 *  vDTMBGetInfo
 *  Get demod information and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void vDTMBGetInfo(DEMOD_CTX_T *psDemodCtx)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   uc_data[7];
U32 	u4Tmp;
U8		ucTmp;


    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO0_RSIZE, 1))
    {
        //CA State
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CA_STATUS);
        //uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CR_STATUS);
        //uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_TPS_INFO);
        psDemodCtx->u1GI = mcGET_FIELD(uc_data[0], 0xC0, 6);
        psDemodCtx->ucCaStat = mcGET_FIELD(uc_data[0], 0x1C, 2);


		//CR State
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CR_STATUS);
        psDemodCtx->ucCrStat = mcGET_FIELD(uc_data[0], 0x07, 0);

		//TPS Real Time Info
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_TPS_INFO);
        psDemodCtx->ucItlvMode = mcTEST_BIT(uc_data[0], 0);			
        psDemodCtx->u1CR = mcGET_FIELD(uc_data[0], 0x06, 1);	
        psDemodCtx->u1Mod = mcGET_FIELD(uc_data[0], 0x38, 3);
        //psDemodCtx->ucCarrMode = mcTEST_BIT(uc_data[0], 6);	
         psDemodCtx->ucCarrMode = mcGET_FIELD(uc_data[0], 0x40, 6);
		//TPS Code Word
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_TPS_CODEWORD);
        psDemodCtx->ucTPSCodeWord = mcGET_FIELD(uc_data[0], 0x3F, 0);

		//Spec Inv
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SPECINV1);
        //psDemodCtx->ucNRDSpecInv = mcTEST_BIT(uc_data[0], 3);	
		//psDemodCtx->ucDEMAPSpecInv = mcTEST_BIT(uc_data[0], 2);
		//psDemodCtx->ucC3780SpecInv = mcTEST_BIT(uc_data[0], 1);
		psDemodCtx->ucNRDSpecInv = mcGET_FIELD(uc_data[0], 0x08, 3);	
		psDemodCtx->ucDEMAPSpecInv = mcGET_FIELD(uc_data[0], 0x04, 2);
		psDemodCtx->ucC3780SpecInv = mcGET_FIELD(uc_data[0], 0x02, 1);

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SPECINV2);
        //psDemodCtx->ucDCSpecInv = mcTEST_BIT(uc_data[0], 2);
		psDemodCtx->ucDCSpecInv = mcGET_FIELD(uc_data[0], 0x04, 2);	

    }

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO1_RSIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_TFO_07);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_TFO_15);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_TFO_23);
		uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_TFO_25);
        psDemodCtx->s4Tfo = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        psDemodCtx->s4Tfo += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 2, 8) * 256 * 256;

    }

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO2;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO2_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CFO_07);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CFO_15);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_CFO_23);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_CFO_28);

        psDemodCtx->s4Cfo = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        psDemodCtx->s4Cfo += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;

    }

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO3_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CH_SPEED);
        psDemodCtx->ucChSpeed = mcGET_FIELD(uc_data[0], 0x1C, 2);
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_DOPP_IND);
        psDemodCtx->ucDoppInd = mcGET_FIELD(uc_data[0], 0xE0, 5);

		#if 0
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_ERR_POW_H);
        psDemodCtx->ucSel_Chir = mcTEST_BIT(uc_data[0], 7);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_ERR_POW_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_ERR_POW_H);
        psDemodCtx->u2ErrPow = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 7, 8);
		#endif

		psDemodCtx->ucLdpc_Iter = mccGET_QUEUE(aucData, ccOFST_DTMB_LDPC_ITER);

		//Sparse
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_FFF_SP_GRP_NUM);
        psDemodCtx->ucFffSpNum = mcGET_FIELD(uc_data[0], 0x1F, 0);
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_FBF_SP_GRP_NUM);
        psDemodCtx->ucFbfSpNum = mcGET_FIELD(uc_data[0], 0x1F, 0);

    }


    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO4;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO4_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_PN_SNR_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_PN_SNR_H);
		psDemodCtx->u2Pn_Snr = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		psDemodCtx->u2Pn_Snr = psDemodCtx->u2Pn_Snr;

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LONG_PN_SNR_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_LONG_PN_SNR_H);
		psDemodCtx->u2Long_Pn_Snr = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		psDemodCtx->u2Long_Pn_Snr = psDemodCtx->u2Long_Pn_Snr;

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_DD_SNR_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_DD_SNR_H);
		psDemodCtx->u2Dd_Snr = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		psDemodCtx->u2Dd_Snr = psDemodCtx->u2Dd_Snr;
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO5;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO5_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_TSIF_PKT_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_TSIF_PKT_H);
		psDemodCtx->u2TsifTotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_TSIF_ERR_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_TSIF_ERR_H);
		psDemodCtx->u2TsifErrorPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

		psDemodCtx->ucTps_Ind = mccGET_QUEUE(aucData, ccOFST_DTMB_TPS_IND);  
    }



    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO6;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO6_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDPC_PKT_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDPC_PKT_H);
		psDemodCtx->u2LdpcTotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDPC_ERR_L);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDPC_ERR_H);
		psDemodCtx->u2LdpcErrorPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

    }


	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO9;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO9_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_ECHO1);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_ECHO2);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_ECHO3);
        psDemodCtx->s2PreEcho = (S16)mcSIGN_2BYTE(uc_data[0], uc_data[1], 8, 4) ;
		psDemodCtx->s2PostEcho = (S16)mcSIGN_2BYTE(uc_data[1], uc_data[2], 4, 8) ;
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_REACQCNT);
		psDemodCtx->u1ReacqCnt = uc_data[0];
    }

	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO10;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO10_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDAGC_GAIN_SHIFT);
		psDemodCtx->u1DagcGainShift = uc_data[0];
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDAGC_GAIN_NORM);
		psDemodCtx->u1DagcGainNorm = uc_data[0];		
    }


    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO11;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO11_RSIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_STATUS_MONITOR);
        psDemodCtx->ucSldPraState = mcGET_FIELD(uc_data[0], 0x60, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_RFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_RFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraRfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_IFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_IFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraIfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_PGA_GAIN_INDEX);
        psDemodCtx->ucSldPraGainIdx = mcGET_FIELD(uc_data[0], 0x1f, 0);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO12;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO12_RSIZE, 1))
    {
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_MAG_REF_OUT_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_SLD_PRA_MAG_REF_OUT_L);
        psDemodCtx->s2SldPraOut = mcSIGN_2BYTE(uc_data[1], uc_data[0], 3, 8);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_LEGACY_PRAD_Cur_Level_sf_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LEGACY_PRAD_Cur_Level_sf_L);
        psDemodCtx->s2LegPraOut = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }
	
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_INFO13;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_INFO13_RSIZE, 1))
    {
        //uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_SIGNAL_POW_H);
        //uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_SIGNAL_POW_L);
        //psDemodCtx->u2SigPow = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_9);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_10);
		psDemodCtx->u2SigPow = (U16)mcUNSIGN_2BYTE(uc_data[0], uc_data[1], 8, 8);
		
		// work around (_TBD_)
		//psDemodCtx->u2SigPow = 1336;

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_11);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_12);
		psDemodCtx->ucSel_Chir = mcTEST_BIT(uc_data[0], 7);
		psDemodCtx->u2ErrPow = (U16)mcUNSIGN_2BYTE(uc_data[0], uc_data[1], 7, 8);

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_13);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CE_STATUS_14);

    }

	#if DTMB_SOC
		/*****new ldagc**************/
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_NEWLDAGC;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_NEWLDAGC_RSIZE, 1))
    {
 		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEWLDAGC_BITSHIFT);
		if(mcTEST_BIT(uc_data[0],7))// neg
		{
		//psDemodCtx->u1NLdagcBitShift = (S32)(uc_data[0]-pow(2,8))*pow(2,0);
		
		psDemodCtx->u1NLdagcBitShift = (S32)(uc_data[0]-256);
		}
		else
		{
			//psDemodCtx->u1NLdagcBitShift = (S32)(uc_data[0]*pow(2,0));
			
			psDemodCtx->u1NLdagcBitShift = (S32)(uc_data[0]);
		}

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEWLDAGC_GAINNORM);
		if(mcTEST_BIT(uc_data[0],7))
		{
		//psDemodCtx->u1NLdagcGainNorm = (S32)((uc_data[0]-pow(2,8))*pow(2,-6));//(INT8)(~uc_data[0]+1);
		psDemodCtx->u1NLdagcGainNorm = (S32)((uc_data[0]-256));//norm*64
		}
		else
		{
			psDemodCtx->u1NLdagcGainNorm = (S32)uc_data[0];////norm*64
		}
		
		///input
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEWLDAGC_INPUTLEVEL);
		if(mcTEST_BIT(uc_data[0],7))
		{
		//psDemodCtx->u1NLdagcInputLevel =(S32)( (uc_data[0]-pow(2,8))*pow(2,-4)); //toolx
		//psDemodCtx->u1NLdagcInputLevel=psDemodCtx->u1NLdagcInputLevel+log(512/420)/16.0f;//toolx
		
		//psDemodCtx->u1NLdagcInputLevel =(S32)( (uc_data[0]-256)/16);//driver
		//psDemodCtx->u1NLdagcInputLevel=psDemodCtx->u1NLdagcInputLevel+0.08602/16;//driver
		}
		else
		{
			//psDemodCtx->u1NLdagcInputLevel =(S32)uc_data[0]/16.0f;//toolx
			//psDemodCtx->u1NLdagcInputLevel=psDemodCtx->u1NLdagcInputLevel+log(512/420)/16.0f;//toolx
			
			//psDemodCtx->u1NLdagcInputLevel =(S32)uc_data[0]/16;//driver
			//psDemodCtx->u1NLdagcInputLevel=psDemodCtx->u1NLdagcInputLevel+0.08602/16;//driver
		}
		///output
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEWLDAGC_OUTPUTLEVEL);
		if(mcTEST_BIT(uc_data[0],7))
		{
		//psDemodCtx->u1NLdagcOutputLevel = (S32)( (uc_data[0]-pow(2,8))*pow(2,-4));
		
		psDemodCtx->u1NLdagcOutputLevel = (S32)( (uc_data[0]-256)/16);
		}
		else
		{
			psDemodCtx->u1NLdagcOutputLevel = (S32)uc_data[0]/16;
		}

		// need complete in kernel
		//psDemodCtx->u1NLdagcGain = (S32)pow(2,psDemodCtx->u1NLdagcBitShift)*(psDemodCtx->u1NLdagcGainNorm);
    }

	
	/******ldagc PMS*************/
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_LDAGC_PMS;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_LDAGC_PMS_RSIZE, 1))
    {
 		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_LDAGC_PMS_DECISION);
		psDemodCtx->u1LdagcPmsReg = uc_data[0];
		psDemodCtx->u1LdagcPmsDecisionReady = uc_data[0]>>7;
		psDemodCtx->u1LdagcPmsDecisionPN = uc_data[0]&0x03;
		psDemodCtx->u1LdagcPmsConfidence = mccGET_QUEUE(aucData, ccOFST_DTMB_LDAGC_PMS_CONFIDENCE);
    }

	
	/*sync time*/
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_SYNC_TIME;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_SYNC_TIME_RSIZE, 1))
    {
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CA_TIMEH);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CA_TIMEL);
		psDemodCtx->u1CaLockTime = (U16)mcUNSIGN_2BYTE(uc_data[0], uc_data[1], 8, 8);

		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_CR_TIMEH);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_CR_TIMEL);
		psDemodCtx->u1CrLockTime = (U16)mcUNSIGN_2BYTE(uc_data[0], uc_data[1], 8, 8);
    }

	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_NEW_DOPPLER1;
	if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_NEW_DOPPLER1_RSIZE, 1))
	{
		uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_PERIOD_ESTIMATE_CNT_DOWN);
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_PERIOD1);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_PERIOD2);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_PERIOD3);
		if((uc_data[3]&0x03) == 0)
		{
			
			uc_data[2] = uc_data[2]&0x03;
			u4Tmp = (((uc_data[0]<<8) | uc_data[1])<<2)|uc_data[2];
			if(mcTEST_BIT(uc_data[0],7))
			{
				//dataf2 = (u4Tmp-262144)/64;
				psDemodCtx->u4NewDopPeriod=u4Tmp-262144;//need divide 64
			}
			else
			{
				//dataf2 = u4Tmp/64;
				psDemodCtx->u4NewDopPeriod=u4Tmp;//need divide 64
			}
			//u4NewDopFreq
			uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_FREQUENCY);
			ucTmp = (uc_data[3] & 0x18)>>3;
			ucTmp = ucTmp +1;
			psDemodCtx->u4NewDopFreq = 756000/(psDemodCtx->u4NewDopPeriod*(ucTmp*210*23)/64);
		}
	}

	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_NEW_DOPPLER2;
	if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_NEW_DOPPLER2_RSIZE, 1))
	{
		
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF0_1);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF0_2);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF0_3);

		uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF1_1);
		uc_data[4] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF1_2);
		uc_data[5] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF1_3);
	}

	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_NEW_DOPPLER3;
	if(mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_NEW_DOPPLER3_RSIZE, 1))
	{
		uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF2_1);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF2_2);
		uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_MEAN_CURR_LEVEL_IN_DIFF2_3);

		uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_LARGE_DELAY_DIFF0_1);
		uc_data[4] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_LARGE_DELAY_DIFF0_2);
		uc_data[5] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_LARGE_DELAY_DIFF1_1);
		uc_data[6] = mccGET_QUEUE(aucData, ccOFST_DTMB_NEW_DOP_LARGE_DELAY_DIFF1_2);
	}
	#endif
	vDTMBShowDbgMsg(psDemodCtx);


}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetLdpcTotPkt
 *  Get total LDPC packets.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval  (UINT32)total LDPC packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetLdpcTotPkt(DEMOD_CTX_T *psDemodCtx)
{
	return (UINT32)psDemodCtx->u2LdpcTotalPkt;
}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetLdpcErrPkt
 *  Get error LDPC packets.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval  (UINT32)error LDPC packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetLdpcErrPkt(DEMOD_CTX_T *psDemodCtx)
{
	return (UINT32)psDemodCtx->u2LdpcErrorPkt;
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetLdpcERR
 *  Get LDPC packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DTMB_GetLdpcERR(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 LDPCTotPkt, LDPCErrPkt;
        
    LDPCTotPkt = (INT32)psDemodCtx->u2LdpcTotalPkt;
    LDPCErrPkt = (INT32)psDemodCtx->u2LdpcErrorPkt;

    if (LDPCTotPkt <= 0)
        return -1;
    else
        return (INT32) ((LDPCErrPkt * 100000) / LDPCTotPkt);
}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetTsifERR
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DTMB_GetPER(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 TSIFTotPkt, TSIFErrPkt;
        
    TSIFTotPkt = (INT32)psDemodCtx->u2TsifTotalPkt;
    TSIFErrPkt = (INT32)psDemodCtx->u2TsifErrorPkt;

    if (TSIFTotPkt <= 0)
        return -1;
    else
        return (INT32) ((TSIFErrPkt * 100000) / TSIFTotPkt);
}

INT16 DTMB_GetSldRfAgc(DEMOD_CTX_T *psDemodCtx)
{
    return psDemodCtx->s2SldPraRfagcCtrlWord;
}

INT16 DTMB_GetSldIfAgc(DEMOD_CTX_T *psDemodCtx)
{
    return psDemodCtx->s2SldPraIfagcCtrlWord;
}

UINT8 DTMB_GetSldGainIdx(DEMOD_CTX_T *psDemodCtx)
{
    return psDemodCtx->ucSldPraGainIdx;
}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetSB
 *  Set SB configuration
 *  TSIF mode: 0:srial/ 1: parallel
 *  CPE_EN: 0: disable/ 1: enable
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval  
 */
//-----------------------------------------------------------------------------
void DTMB_SetSB(DEMOD_CTX_T *psDemodCtx, BOOL tsif_data_mode, BOOL cpe_en)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_SET_SB;
    mccSET_QUEUE2(aucData, ccOFST_DTMB_SET_TSIF_SP) = tsif_data_mode;
    mccSET_QUEUE2(aucData, ccOFST_DTMB_SET_CPE_EN) =  cpe_en;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_DTMB_SET_SB_RSIZE, 1);
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_UpdateInfo
 *  Get demod status, information and system parameters 
 *  and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{
    vDTMBGetStatus(psDemodCtx);
    vDTMBGetInfo(psDemodCtx);
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_TunerI2c
 *  Switch tuner bypass mode 
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  fgSwitchOn  Flag to switch bypass mode on/off (TRUE: ON, FALSE: OFF)
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{

    UCHAR   aucData[cMAX_READ_NUM + 1]; 
    //U8      status;

    if (fgSwitchOn)
    {
   		mcSHOW_DBG_MSG2(("DTMB_TunerI2c:%d!!!!\n",fgSwitchOn));
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 1;
        if (psDemodCtx->ucEnFpga)
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x04;   //huiyan 20120314 for 5175+mt5398
        else
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50;    // //huiyan 20120314 for 5175+mt5398
    }
    else 
    {
    	mcSHOW_DBG_MSG2(("DTMB_TunerI2c:%d!!!!\n",fgSwitchOn));
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 0;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50;
    }


   // status = mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_TUNER_I2C_ONOFF_SIZE+1);
   mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_TUNER_I2C_ONOFF_SIZE+1);
   mcDELAY_MS(20);
   

	#if 0
    if (status)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
            return 0;
        else
            return -1;
    }
    else
        return -1;

#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_DriverStart(DEMOD_CTX_T *psDemodCtx, UINT8 u1Mon1Sel, UINT8 u1Mon2Sel)
{
#ifndef DTMB_pidriver
UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_START_DRIVER;
    // 0:none 
    // 1:Adv ReAcq
    mccSET_QUEUE2(aucData, ccOFST_SET_DRIVER_MON1) = u1Mon1Sel;   
    // 0:none
    // 1:TPS sync
    // 2:SPS sync auto
    // 3:SPS sync manual
    mccSET_QUEUE2(aucData, ccOFST_SET_DRIVER_MON2) = u1Mon2Sel;   
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_SET_DRIVER_RSIZE, ccOFST_SET_DRIVER_SIZE + 1);
#else
	UCHAR	aucData[cMAX_READ_NUM + 1];


	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_REACQ_START;
	mcHOST_CMD(psDemodCtx, aucData, 0, 1);//dtmb time improve
	

#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DTMB_DriverStop(DEMOD_CTX_T *psDemodCtx)
{
#ifndef DTMB_pidriver
UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_STOP_DRIVER;
    mcHOST_CMD(psDemodCtx, aucData, ccOFST_STOP_DRIVER_RSIZE, 1);
#else

    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_REACQ_STOP;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);//dtmb time improve

#endif
}
//----------------------------------------------------------------------------- 
/** DTMB_AcqStart
 *  Start Acquistion.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DTMB_AcqStart(DEMOD_CTX_T *psDemodCtx)
{
	//set fw state to reset
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DTMB_ACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);//dtmb time improve
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetIF
 *  set demod IF input.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.  
 */
//-----------------------------------------------------------------------------
void DTMB_SetIF(DEMOD_CTX_T *psDemodCtx)
{
#ifndef DTMB_pidriver
    UCHAR   ucPara[ccCMD_REG_NUM];
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    // set agc parameters
    ucPara[0] = ccCMD_ID_SET_TUNER_PARA1;
    ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, 080826, JC
    ucPara[2] = 0x7F;//(psDemodCtx->sTunerCtx.m_aucPara[6] & 0x3F) | 0x40;            // kp, ki; LF_CTRL
    ucPara[3] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
    ucPara[4] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
    vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);

#else

UCHAR ucPara[4];

ucPara[0] = ccCMD_ID_DTMB_SET_TUNER_PARA1;
ucPara[1] = 0;// dtmb no need to set target in driver from Jostan and Hans,so do not care ucPara[1]
ucPara[2] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
ucPara[3] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
mcSHOW_USER_MSG(("[DTMB_SetIF]DTMB_IF high byte 0x%2x!\n", ucPara[3]));
mcSHOW_USER_MSG(("[DTMB_SetIF]DTMB_IF low byte 0x%2x!\n", ucPara[2]));

vSetIntfPara(psDemodCtx, ucPara, ccOFST_DTMB_SET_TUNER_PARA1_SIZE+1);


#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DTMB_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{
//UCHAR   ucPara[ccCMD_REG_NUM];
//UCHAR   ii;
//UCHAR   aucData[cMAX_READ_NUM + 1];
UINT8 * pCodeImage=NULL;
UINT16 u2CodeImageLen;
//SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    psDemodCtx->u1Bw = MT5391_BW_8MHz;
    psDemodCtx->fgEn24MClk = FALSE;
    psDemodCtx->ucEnFpga = FALSE;
    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->ucConnType = PORT_CFG_MTK;
    psDemodCtx->ucTsIfFormat = 0x8E;   // change to Parallel TS output.
    psDemodCtx->ucTsIfCtrl = 0x00;
    psDemodCtx->ucTsIfRate = 0x07;
    psDemodCtx->u1ChSearchMode = CH_SEARCH_SCAN;
    psDemodCtx->u1TunerAcqType = ACQ_ACTION_DEFAULT;
	psDemodCtx->fgShowDbgMsg = FALSE;
	//////////test
	psDemodCtx->u1LoadFwOpt1=0;
	psDemodCtx->u1LoadFwOpt2=0;

// dtmb soc
/*
#if defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)|| defined (CC_MT5399))    //huiyan 20120314 for 5175+mt5398 ,LL for 5175+5881
	DTMB_ResetDemod(100);
#endif

	#ifdef MT5175_MEM_REPAIR
	DTMB_MemoryRepair(psDemodCtx);
	#endif
*/
	
	//DTMB_SetTdiAddr();// befor module release
	DTMB_ChipInit(psDemodCtx);
	mcSHOW_USER_MSG(("[DTMB_Initialize]DTMB_chipInit finished\n"));

    if (fgLoadCode)
    {    
        if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
        {
            pCodeImage=ucDtmbCodeImage;
            u2CodeImageLen=sizeof(ucDtmbCodeImage);
			mcSHOW_DBG_MSG(("-----[DTMB_Initialize]Load sawless fw------\n"));
        }
        else//legacy tuner
        {
            pCodeImage=ucDtmbCodeImage;//ucDtmbCodeImageLeg;
            u2CodeImageLen=sizeof(ucDtmbCodeImage/*ucDtmbCodeImageLeg*/);
			mcSHOW_DBG_MSG(("-----[DTMB_Initialize]Load lowif fw------\n"));
        }

		
        if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
        {
			mcSHOW_USER_MSG(("[DTMB_Initialize]load dtmb fw fail !\n"));
            return (DRVAPI_TUNER_ERROR);
        }

		// use to fine tune early break time 
        DTMB_SetEarlyBreak(psDemodCtx);

        // Set TS IF
        //DTMB_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);//dtmb time improve

        // AGC crrve setting  dtmb no need to set from Jostan
       // ucPara[0] = ccCMD_ID_SET_TUNER_PARA0;
        //for (ii = 0; ii < 4; ii++)
            //ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
        //vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA0_SIZE+1);

        // set agc parameters
        //DTMB_SetIF(psDemodCtx);//dtmb time improve

    }

    return (DRVAPI_TUNER_OK);
}

/**********************************************************************/
/*                  Query APIs                                        */
/**********************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetIfAgcVol
 *  Get IF AGC voltage.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (UINT16) IF AGC voltage in mV.   
 */
//-----------------------------------------------------------------------------
UINT16 DTMB_GetIfAgcVol(DEMOD_CTX_T *psDemodCtx)
{
#if 0
    UINT16 u2IfAgcVol = 0;
    INT32  s4Temp;

    s4Temp = (INT32)(psDemodCtx->s1IfAgc) * 3300;
    s4Temp /= 256;
    u2IfAgcVol = (UINT16)(s4Temp + 1650);

    return u2IfAgcVol;
#else
	return 0;
#endif
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{
    INT8 s1AgcMax;
    INT8 s1AgcMin;
    INT16 s2SignalStrength;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

  
    //s1AgcMax = (INT8)psDemodCtx->sTunerCtx.m_aucPara[4];
    //s1AgcMin = (INT8)psDemodCtx->sTunerCtx.m_aucPara[5];
    s1AgcMax = (INT8)pEuTunerCtx->m_ifAgc_lvl_max;
    s1AgcMin = (INT8)pEuTunerCtx->m_ifAgc_lvl_min;

    if (s1AgcMax != s1AgcMin)
#if defined(CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368)||defined(CC_MT5175)||defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881) ||defined(CC_MT5399)//LL for 5175+5881 EC_code_dB3330: For MT5396DVBTC demod
        s2SignalStrength = ((INT16)(psDemodCtx->ucSldPraGainIdx-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#else
        s2SignalStrength = ((INT16)(psDemodCtx->s1IfAgc-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#endif
    else
        s2SignalStrength = 0;


    if ( ((INT8)pEuTunerCtx->m_aucPara[2]) > 0)
        s2SignalStrength = 100 - s2SignalStrength;


    if ((s2SignalStrength==0) && (DTMB_GetPER(psDemodCtx)==0))
		s2SignalStrength = 10;
	else if ((s2SignalStrength==0) && (DTMB_GetPER(psDemodCtx)>0) && (DTMB_GetPER(psDemodCtx)<20))
		s2SignalStrength = 5;


    //if (psDemodCtx->fgTsLock == TRUE)
    {
        if      (s2SignalStrength < 0)
            return 0;
        else if (s2SignalStrength > 100)
            return 100;
        else
            return (UINT8)s2SignalStrength;
    }
    //return 0;
}
STATIC UINT16 SNR_log_table_int[]=
{
	0, 100,
		2, 104,
		4, 109,
		6, 114,
		8, 120,
		10, 125,
		12, 131,
		14, 138,
		16, 144,
		18, 151,
		20, 158,
		22, 166,
		24, 173,
		26, 182,
		28, 190,
		30, 199,
		32, 208	
};

/****************************************************************************
Function:    Left_Table
Parameters:  InValue
Return:      OutValue  

  Description:
  This is a sub function called by Log10_Convert.
******************************************************************************/
STATIC UINT16 Left_Table(UINT16 InValue)
{
	UINT16 OutValue = 0;
	UINT16 TableSize = sizeof(SNR_log_table_int)/sizeof(UINT16);
	UINT16 i;	
	for(i=0; i<TableSize; i+=2)
	{	
		if(InValue<=SNR_log_table_int[i+1])
		{		
			OutValue = SNR_log_table_int[i];	
		//	mcSHOW_USER_MSG(("InValue = %d, OutValue = %d\n", InValue, OutValue));
			return OutValue;
		}	
	}
	OutValue = 32;
	return OutValue;
}

/****************************************************************************
Function:    Log10_Convert
Parameters:  InValue
Return:      log_ret  

  Description:
  Calculate log value
******************************************************************************/
UINT32 Log10_Convert(UINT32 InValue)
{
	UINT32		u32_temp = 1;
	UINT8		indx = 0;
	UINT32             InValue_left;
	UINT32 log_ret;    

	do {	
		u32_temp = u32_temp << 1;
		if (InValue < u32_temp)
			break;		
	}while(++indx < 32);
	
	log_ret = (301*indx+50)/100;

	InValue_left = InValue*100/(1<<indx);
	
	log_ret = log_ret + Left_Table(InValue_left)/10;
	
	//mcSHOW_USER_MSG((" InValue = %d  log ret = %d\n", InValue,  log_ret));
	return log_ret;	
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (UINT16) SNR in 0.1dB.   
 */
//-----------------------------------------------------------------------------
UINT16 DTMB_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
    INT16 s2_data;
	UINT32 u2Data;
    
	if (psDemodCtx->ucCarrMode == 1) // C1 mode
    {
        s2_data = psDemodCtx->u2Long_Pn_Snr / 64;  
		mcSHOW_DBG_MSG3(("LONG_PN_SNR(C1) = %d\n",s2_data));
    }
	else //if (psDemodCtx->ucCarrMode == 0)	// C3780 mode
	{

		if ( (psDemodCtx->u2ErrPow==0) && (psDemodCtx->fgTsLock==0))
			return 0;
		if ( (psDemodCtx->u2ErrPow==0) && (psDemodCtx->fgTsLock==1))
			return 30;
		     
		
		u2Data=psDemodCtx->u2SigPow /(psDemodCtx->u2ErrPow*2);

		mcSHOW_DBG_MSG4(("u2Data=0x%04x\n",u2Data));
		mcSHOW_DBG_MSG4(("u2SigPow=0x%04x\n",psDemodCtx->u2SigPow));
		mcSHOW_DBG_MSG4(("u2ErrPow=0x%04x\n",psDemodCtx->u2ErrPow));

		//psDemodCtx->s2Snr = 10*log10(u2Data);
		psDemodCtx->s2Snr = (INT16)Log10_Convert(u2Data);
		s2_data = psDemodCtx->s2Snr;
		mcSHOW_DBG_MSG3(("CE SNR(C3780) = %d\n",s2_data));
		
	}
	
    if (s2_data < 0)
        return 0;
    else
        return (UINT16) s2_data;
    
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetTotPkt(DEMOD_CTX_T *psDemodCtx)  
{
	return psDemodCtx->u2TsifTotalPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DTMB_GetErrPkt(DEMOD_CTX_T *psDemodCtx)  
{
	return psDemodCtx->u2TsifErrorPkt;

}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DTMB_GetTsLock(DEMOD_CTX_T *psDemodCtx)
{
    return psDemodCtx->fgTsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetFreqOffset_kHz
 *  Get frequency offset in kHz.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   (INT32) freq_offset * 256.   
 */
//-----------------------------------------------------------------------------
INT32 DTMB_GetFreqOffset_kHz(DEMOD_CTX_T *psDemodCtx)
{
	return ( (psDemodCtx->s4Cfo/10000/1000)*141 );

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetBW
 *  Get channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   0 : 6M.
 *  @retval   1 : 7M.
 *  @retval   2 : 8M.
 *  @retval   3 : 5M.
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetBW(DEMOD_CTX_T *psDemodCtx)
{
    return (UINT8) psDemodCtx->u1Bw;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetIntfPara
 *  Set host command interface API.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  para  Pointer of parameters buffer. (para[0] : CmdId, para[1~(len-1)] : parameters).
 *  @param  len  Length of parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
static void vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii;

    if (len > 8) { // for klocwork warning
        return;
    }

    mccSET_CMD2(aucData) = para[0];
    if (len > 1)
    {
        for (ii = 0; ii < (len-1); ii++)
            mccSET_QUEUE2(aucData, ii) = para[ii+1];
    }

    mcHOST_CMD(psDemodCtx, aucData, 0, len);

}

//----------------------------------------------------------------------------- 
/*
 *  vSetChSearchMode
 *  Set channel search mode according to demod context.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
#if 0
static void vSetChSearchMode(DEMOD_CTX_T *psDemodCtx)
{
#ifndef DTMB_pidriver
//TBD
#endif
}
#endif

//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetBW
 *  Set channel bandwidth.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DTMB_SetBW(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW)
{

//need to do


}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  tsif_format  TS interface format.
 *  TsIfFormat:
 *  b7  Reserve
 *  b6  VALID   Active polarity selection of tsif_val
 *      0 -> tsif_val is high active
 *      1 -> tsif_val is low active
 *  b5  ERR Active polarity selection of tsif_err
 *      0 -> tsif_err is high active
 *      1 -> tsif_err is low active
 *  b4  SYNC    Active polarity selection of tsif_sync
 *      0 -> tsif_sync is high active
 *      1 -> tsif_sync is low active
 *  b3  TEI Transport stream error indicator
 *      0 -> TEI bit will not be changed by tsif
 *      1 -> TEI bit will automatic set by tsif 
 *  b2  SP  Parrallel / Serial mode selection
 *      0 -> Serial mode
 *      1 -> Parralle mode
 *  b1  CLK_GATED   Output clock gated selection of tsif_clk
 *      0 -> tsif_clk is gated
 *      1 -> tsif_clk is non-gated
 *  b0  CLK_INV Output clock edge selection of tsif_clk
 *      0 -> tsif_data is valid at positive edge clock
 *      1 -> tsif_data is valid at negative edge clock
 *  @param  tsif_ctrl  TS interface control.
 *  TsIfControl:
 *  b7  Reserve
 *  b6  FRACTIONAL_PKT  Allow MPEG fractional packet generated
 *      0 -> Output packet is always 188 bytes
 *      1 -> Output packet may not be 188 bytes when channel changed
 *  b5  SERIAL_SYNC  Control the width of TSIF_SYNC signal. Only valid in serial mode
 *      0 -> SYNC duration = first 8 bit
 *      1 -> SYNC duration = first 1 bit
 *  b4  DISABLE_HDR  Disable output header 'h47
 *      0 -> Normal
 *      1 -> Disable header 'h47 in TS
 *  b3  DATA_FLIP  Control data flip of TSIF_DATA[7:0]
 *      0 -> Normal
 *      1 -> Data flip of TSIF_DATA[7:0]
 *  b2  ERR_DROP_PKT  Drop packet if error packet occur
 *      0 -> Normal
 *      1 -> Drop packet if TSIF_ERR = 1'b1
 *  b1  ERR_SHORT  Define TSIF_ERR duration
 *      0 -> Normal, TSIF_ERR duration is 188 bytes
 *      1 -> TSIF_ERR duration is 1 byte
 *  b0  ERR_GATED  Define TSIF_ERR gated
 *      0 -> TSIF_ERR is gated by data enable
 *      1 -> TSIF_ERR is not gated between packet
 *  @param  tsif_rate  TS interface clock rate. Output data rate = (48MHz/(TSIF_RATE+1)). The value should be odd number.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DTMB_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate)
{
//TBD
}

//----------------------------------------------------------------------------- 
/** DTMB_SetTsOutputMode
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DTMB_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{

	UCHAR ucData;

	mcSHOW_DBG_MSG3(("DTMB_SetTsOutputMode: fgParallel = %d\n", fgParallel));
	DTMB_GetReg(psDemodCtx,DTMB_SB_CFG0,&ucData,1);

	if(fgParallel)
	{
		ucData |=0x08;
	    DTMB_SetReg(psDemodCtx,DTMB_SB_CFG0,&ucData,1);
	}

    else
    {
	  	ucData &= 0xf7;
	    DTMB_SetReg(psDemodCtx,DTMB_SB_CFG0,&ucData,1);
		ucData=0x01;
		DTMB_SetReg(psDemodCtx,DTMB_TSIF_CTRL3,&ucData,1);
    }

}

//----------------------------------------------------------------------------- 
/** DTMB_SetTsPathMode
 *  Set TS path mode
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable)
{

    return DTMB_DEMOD_STATE_LOCK;
}
/**********************************************************************/
// This function will get SNR immediately from MT5133, you don't need
// to call DTMB_GetInfo in advance
// return value ranges from 0~255 (mapping from 0~31.5dB)
//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSnrInfo
 *  Get SNR value, need not to call DTMB_GetInfo in advance.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   SNR * 8(dB).   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_GetSnrInfo(DEMOD_CTX_T *psDemodCtx)
{
    INT16 s2_data;
	UINT32 u2Data;
    
	if (psDemodCtx->ucCarrMode == 1) // C1 mode
    {
        s2_data = psDemodCtx->u2Long_Pn_Snr / 64;        
    }
	else //if (psDemodCtx->ucCarrMode == 0)	// C3780 mode
	{

		if ( (psDemodCtx->u2ErrPow==0) && (psDemodCtx->fgTsLock==0))
			return 0;
		if ( (psDemodCtx->u2ErrPow==0) && (psDemodCtx->fgTsLock==1))
			return 30;
		     
		
		u2Data=psDemodCtx->u2SigPow /(psDemodCtx->u2ErrPow*2);

		mcSHOW_DBG_MSG3(("u2Data=0x%04x\n",u2Data));
		mcSHOW_DBG_MSG3(("u2SigPow=0x%04x\n",psDemodCtx->u2SigPow));
		mcSHOW_DBG_MSG3(("u2ErrPow=0x%04x\n",psDemodCtx->u2ErrPow));

		//psDemodCtx->s2Snr = 10*log10(u2Data);
		psDemodCtx->s2Snr = (INT16)Log10_Convert(u2Data);
		s2_data = psDemodCtx->s2Snr;
		mcSHOW_DBG_MSG3(("s2_data=0x%04x\n",s2_data));
		
	}
	
    if (s2_data < 0)
        return 0;
    else
        return (UINT16) s2_data;



}
//----------------------------------------------------------------------------- 
/*
 *  DTMB_DumpRegisters
 *  Dump Demod registers.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  bank_no  Register bank number.
 *  @param  max_i2c_len  Max I2C read byte at one time
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len)
{
UINT16  u2CurI2cLen, u2CurAddr, u2StartAddr = 0;
UINT16  u2CurLen = 256;

    if (!psDemodCtx)
        return 1;
    if (!buffer)
        return 1;
    if (bank_no > 2)
        return 1;

    u2CurAddr = 0;
    u2StartAddr = (UINT16) bank_no << 8;
    while (u2CurLen)
    {
        u2CurI2cLen = u2CurLen;
        if (u2CurI2cLen > max_i2c_len)
            u2CurI2cLen = max_i2c_len;

        if (DTMB_GetReg(psDemodCtx, u2StartAddr + u2CurAddr,
                          buffer + u2CurAddr, u2CurI2cLen))
        {
            return 1;
        }
        u2CurAddr += u2CurI2cLen;
        u2CurLen  -= u2CurI2cLen;
    }

    return 0;
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_SetNormalMode
 *  Set demod to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_SetNormalMode(DEMOD_CTX_T *psDemodCtx)
{
UCHAR ucData = 0x00;

    if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
    {   // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
        //0x86E=0x00 means IFAGC mode
        //0x86E=0x05 means GPIO output low mode
        //0x86E=0x07 means GPIO output high mode
        ucData = 0x07;
        DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F110, &ucData, 1);
    }
    else
    {    // set IFAGC pin to be normal function for legacy
    	ucData = 0x00;
        DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F110, &ucData, 1);

    }

    u1AfterAcq = 1;
    mcSHOW_DBG_MSG(("DTMB_SetNormalMode : u1AfterAcq = %d\n", u1AfterAcq));


	return 0;

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DTMB_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 ucBW)
{
    UINT8 fgRet = 0;

    mcSHOW_DBG_MSG(("PI DTMB Connect!!\n"));

    // Chip init
    DTMB_ChipInit(psDemodCtx);
	DTMB_FwSwitch(psDemodCtx);
	//DTMB/DVBC switch
	//fgDTMB_FwStart(psDemodCtx);

    // stop driver
    DTMB_DriverStop(psDemodCtx);

    fgRet = DTMB_SetNormalMode(psDemodCtx);

    // stop driver
    //DTMB_DriverStop(psDemodCtx);   
    // set BW
    DTMB_SetBW(psDemodCtx, ucBW);


#ifdef DTMB_pidriver
		DTMB_AcqStart(psDemodCtx);
#endif


    // driver start
    DTMB_DriverStart(psDemodCtx, REACQ_AUTO, FAST_SYNC_AUTO);

    return fgRet;
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------
VOID DTMB_DisConnect(DEMOD_CTX_T *psDemodCtx)
{
#if defined (CC_MT5175)
#endif

UCHAR ucData = 0x00;
UINT32 u4Temp;
#if HOSTCMD_UP
UINT32 u4Tmp;
#endif

// from dvbt
// RFAGC internal control by tuner, set RFAGC pin to be HiZ
DTMB_GetReg(psDemodCtx, 0xf62, &ucData, 1);
mcCLR_BIT(ucData, 3);
mcCLR_BIT(ucData, 2);
DTMB_GetReg(psDemodCtx, 0xf62, &ucData, 1);
mcSHOW_USER_MSG(("[DTMB_DisConnect]Set RFGAC and IFAGC to Hiz in DTMB disconnect!!\n"));

//from 5175
DTMB_DriverStop(psDemodCtx);
mcDELAY_MS(10);
ucData = 0xAB;//Reset DTMB Quad2/3/4
DTMB_SetReg(psDemodCtx, DTMB_QUAD_GLOBAL_0, &ucData, 1);
ucData = 0x01;//Reset DTMB Quad1_CDC,CR,STR,CRANIR,CA
DTMB_SetReg(psDemodCtx, DTMB_QUAD1_RST_B_1, &ucData, 1);


#if HOSTCMD_UP


IO_WRITE32(0xF002A000, 0x4B4, 0x000F0000);		 
//u4Tmp = mcRISC_Reg(0xf002a000,0xe00);//0xf002a000	
//ucData= u4Tmp & 0x000000ff;
//mcSHOW_DBG_MSG(("[ATD_ChipInit]pre-mode, get e00~e03 = 0x%x,e00=0x%x\n",u4Tmp,ucData));
//if(ucData==0x0a)
//{
	u4Tmp = mcRISC_Reg(0xf002a000,0x000);//get 0x00
	mcSHOW_DBG_MSG(("[DTMB_DisConnect]get 0xf002a000=0x%x\n",u4Tmp));
	u4Tmp &= 0xffffff00;
	IO_WRITE32(0xF002a000, 0x000, u4Tmp);
	mcSHOW_DBG_MSG(("[DTMB_DisConnect]dis UP set 0xf002a000=0x%x\n",u4Tmp));
	mcDELAY(10);
	u4Tmp = mcRISC_Reg(0xf002a000,0x000);
	mcSHOW_DBG_MSG(("[DTMB_DisConnect]get 0xf002a000=0x%x\n",u4Tmp));
//}
//else
//{		
//	mcSHOW_DBG_MSG(("[ATD_ChipInit]pre mode is not dtmb\n"));
//}
#endif


#if defined (CC_MT5882)// from dvbt 5399
u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x500);
u4Temp |= 0x00000006;
mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b11,PGA power down

u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
u4Temp |= 0x00000003;
mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b1,ADC power down
#endif

#if defined (CC_MT5890)// from dvbt 5890
u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
u4Temp &= 0xBFFFFFFF;
mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//	Set 0xf0061400[30]=1'b0;//set RG_DMRX_EN=0
u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x418);
u4Temp &= 0xFFDFFFFF;
mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//Set 0xf0061418[21]=1'b0;//set RG_CIC_FLT_EN=0
#endif


#if defined (CC_MT5175)

	UCHAR ucData = 0x00;
    //UCHAR   aucData[cMAX_READ_NUM + 1];

    // stop driver
    DTMB_DriverStop(psDemodCtx);

    //DTMB_SetTsOutput(psDemodCtx, FALSE);//dtmb time improve


    // delay for SB reset, at least one complete TS packet
    mcDELAY_MS(10);

    if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
    {    // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
         //0x86E=0x00 means IFAGC mode
         //0x86E=0x05 means GPIO output low mode
         //0x86E=0x07 means GPIO output high mode
        ucData = 0x05;
        DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F110, &ucData, 1);
    }
    else
    {    // set IFAGC pin to be HiZ for legacy
        DTMB_GetReg(psDemodCtx, REG_MT5175_PADMUX_F110, &ucData, 1);
        mcCLR_BIT(ucData, 3);   // gpio control by demod
        mcSET_BIT(ucData, 2);  // gpio enable
        mcCLR_BIT(ucData, 0);  // gpio as input mode
        DTMB_SetReg(psDemodCtx, REG_MT5175_PADMUX_F110, &ucData, 1);
    }

	ucData = 0xAB;//Reset DTMB Quad2/3/4
	DTMB_SetReg(psDemodCtx, DTMB_QUAD_GLOBAL_0, &ucData, 1);
	ucData = 0x01;//Reset DTMB Quad1_CDC,CR,STR,CRANIR,CA
	DTMB_SetReg(psDemodCtx, DTMB_QUAD1_RST_B_1, &ucData, 1);

	//Aanlog Power Down
	DTMB_GetReg(psDemodCtx, REG_IFADC_CFG3, &ucData, 1);//dtmb time improve
	mcSET_BIT(ucData, 1); 
	mcSET_BIT(ucData, 0); 
	DTMB_SetReg(psDemodCtx, REG_IFADC_CFG3, &ucData, 1);
	
	DTMB_GetReg(psDemodCtx, REG_IFPGA_CFG1, &ucData, 1);
	mcSET_BIT(ucData, 2); 
	mcSET_BIT(ucData, 1); 
	DTMB_SetReg(psDemodCtx, REG_IFPGA_CFG1, &ucData, 1);
#endif

}

//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   DTMB_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DTMB_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DTMB_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DTMB_GetSync(DEMOD_CTX_T *psDemodCtx)
{

	UINT8 u1DTMBState = DTMB_DEMOD_STATE_UNKNOWN;


	if (psDemodCtx->fgNoSignal)
	{
		u1DTMBState = DTMB_DEMOD_STATE_UNLOCK;
	}
	 if (psDemodCtx->fgTsLock)
	{
		u1DTMBState = DTMB_DEMOD_STATE_LOCK;
	}

	return u1DTMBState;

}
//----------------------------------------------------------------------------- 
/** DTMB_GetSwVer
 *  Get SW version.
 *  @retval   string of SW version.
 */
//-----------------------------------------------------------------------------
CHAR*  DTMB_GetSwVer(void)
{
    return ((CHAR*)MT5175_DTMB_SW_VER);
}


//----------------------------------------------------------------------------- 
/*
 *  DTMB_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DTMB_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DTMB_GetTsFmt(DEMOD_CTX_T *psDemodCtx)
{
TS_FMT_T fgTsFmt;
UINT8 ucData;
	
	DTMB_GetReg(psDemodCtx, DTMB_REG_SB_CFG0, &ucData, 1);
	
    if (ucData & 0x08)
        fgTsFmt.SerOrPar = SP_PARALLEL;
    else
        fgTsFmt.SerOrPar = SP_SERIAL;


	DTMB_GetReg(psDemodCtx, DTMB_REG_TSIF_CTRL, &ucData, 1);

    if (ucData & 0x40)
        fgTsFmt.fgGatedClock = BOOL_FALSE;
    else
        fgTsFmt.fgGatedClock = BOOL_TRUE;

    if (ucData & 0x20)
        fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
    else
        fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;	


	DTMB_GetReg(psDemodCtx, DTMB_REG_SB_RATE, &ucData, 1);

    if (fgTsFmt.SerOrPar == SP_PARALLEL)
    {
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 54000/(ucData+1);
    }
    else
    {
        /* Serial */
        fgTsFmt.u4TsClockInKHz = 54000*8/(ucData+1);
    }
// external
/*
#if defined(CC_MT5396) || defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5860)||defined(CC_MT5881)||defined(CC_MT5399)//LL for 5175+5881
	fgTsFmt.fgExternalDemod = BOOL_TRUE;
#else
	fgTsFmt.fgExternalDemod = BOOL_FALSE;
#endif
*/
//internal
	fgTsFmt.fgExternalDemod = BOOL_FALSE;

	fgTsFmt.fgTristate = BOOL_TRUE;

	mcSHOW_DBG_MSG3(("GetTsFmt: SerOrPar=%d,fgExternalDemod=%d\n",fgTsFmt.SerOrPar,fgTsFmt.fgExternalDemod));

    return fgTsFmt;
}


/***********************************************************************/
CHAR *DTMB_GetVer(DEMOD_CTX_T *psDemodCtx)
{
#ifndef DTMB_pidriver

   CHAR * cString1;
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = TRUE;
#endif
    vDTMB_GetAsicVer(psDemodCtx);
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#endif
    ITuner_OP(psDemodCtx->psTunerCtx,itGetVer,0,&cString1);
    mcSHOW_USER_MSG(("%s\n", cString1));
	
#endif

    return NULL;
}
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
VOID vDTMB_GetAsicVer(DEMOD_CTX_T *psDemodCtx)
{
#ifndef DTMB_pidriver

UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
#if defined (CC_MT5135) || defined (CC_MT5135DVBT)
    DTMB_GetDemodReg(psDemodCtx, MT5135_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("MT5135 Chip ID: %02x\n", aucData[0]));
#endif

    DTMB_GetDemodReg(psDemodCtx, DVBT_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("DTMB Chip ID: %02x\n", aucData[0]));


    mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
    {
        mcSHOW_USER_MSG(("FW ver: %02X%02X-%02X%02X\n",
                         mccGET_QUEUE(aucData, ccOFST_VER_YEAR_MONTH), mccGET_QUEUE(aucData, ccOFST_VER_DATE),
                         mccGET_QUEUE(aucData, ccOFST_VER_HOUR), mccGET_QUEUE(aucData, ccOFST_VER_MINUTE)));
    }
#endif
}

VOID DTMB_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable)
{

//need to do
UCHAR ucData;
mcSHOW_DBG_MSG(("[DTMB_SetTsOutput]\n"));

DTMB_GetReg(psDemodCtx, 0x5D2, &ucData, 1);

mcSHOW_DBG_MSG(("[DTMB_SetTsOutput]: 0x5D2 = 0x%02x\n", ucData));
if(fgOutputEnable)//on
{
	mcSET_BIT(ucData,2);
	mcCLR_BIT(ucData,1);
	mcSET_BIT(ucData,0);
	DTMB_SetReg(psDemodCtx, 0x5D2, &ucData, 1);
}
else				//off
{
	mcCLR_BIT(ucData,2);
	mcSET_BIT(ucData,1);
	mcCLR_BIT(ucData,0);
	DTMB_SetReg(psDemodCtx, 0x5D2, &ucData, 1);
}


}

// dtmb soc
/*
#if defined (CC_MT5175) &&(defined(CC_MT5396)|| defined(CC_MT5398)|| defined(CC_MT5880) ||defined(CC_MT5881) ||defined (CC_MT5399))    // //huiyan 20120314 for 5175+mt5398,LL for 5175+5881
VOID DTMB_ResetDemod(UINT16 u2DelayTime)
{
UINT32 u4RstPinNum;
    // Config GPIO to reset demod
    mcSHOW_DRVAPI_MSG(("DTMB_ResetDemod\n"));
    if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
        GPIO_SetOut((INT16)u4RstPinNum, 0);
        mcDELAY_MS(u2DelayTime);
        GPIO_SetOut((INT16)u4RstPinNum, 1);
        mcDELAY_MS(100); // Delay 20 ms for CR[DTV00030459]. Or it will init fail in rel image
        mcSHOW_DBG_MSG(("DTMB Reset Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime));
    }
}
#endif
*/

#ifdef CC_SUPPORT_STR
/* close dmtb clk  */
VOID DTMB_CloseClk(DEMOD_CTX_T *psDemodCtx)
{
UCHAR ucData = 0x00;
mcSHOW_DBG_MSG(("DTMB_CloseClkSet\n"));

//Abist clk pwd
DTMB_GetReg(psDemodCtx, CKGEN_20, &ucData, 1);
mcSET_BIT(ucData, 5); 
DTMB_SetReg(psDemodCtx, CKGEN_20, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_22, &ucData, 1);
mcSET_BIT(ucData, 2); 
DTMB_SetReg(psDemodCtx, CKGEN_22, &ucData, 1);

/// close other clk
DTMB_GetReg(psDemodCtx, CKGEN_29, &ucData, 1);
mcSET_BIT(ucData, 6); 
DTMB_SetReg(psDemodCtx, CKGEN_29, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_27, &ucData, 1);
mcSET_BIT(ucData, 6);
DTMB_SetReg(psDemodCtx, CKGEN_27, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_04, &ucData, 1);
mcSET_BIT(ucData, 6);
DTMB_SetReg(psDemodCtx, CKGEN_04, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_31, &ucData, 1);
mcSET_BIT(ucData, 6);
DTMB_SetReg(psDemodCtx, CKGEN_31, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_06, &ucData, 1);
mcSET_BIT(ucData, 7);
DTMB_SetReg(psDemodCtx, CKGEN_06, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_02, &ucData, 1);
mcSET_BIT(ucData, 6);
DTMB_SetReg(psDemodCtx, CKGEN_02, &ucData, 1);

DTMB_GetReg(psDemodCtx, CKGEN_08, &ucData, 1);
mcSET_BIT(ucData, 6);
DTMB_SetReg(psDemodCtx, CKGEN_08, &ucData, 1);  
}

#endif
BOOL fgSetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR ucValue)
{	
	UCHAR	aucData[cMAX_READ_NUM + 1]; 		// For Host Cmd
	//CString sztmp;
	//UCHAR ucData;
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_SET_VAR;
	
	mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_SET_VAR_ADDRH) = mcHIGH_BYTE(u2Addr);
	mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_SET_VAR_ADDRL) = mcLOW_BYTE(u2Addr);
	mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_SET_VAR_VALUE) = ucValue;
	mcSHOW_DBG_MSG1(("[fgSetFwVarDTMB]addr=0x%x,value=0x%x",u2Addr,ucValue));
	if(mcHOST_CMD(psDemodCtx, aucData, 0, ccCMD_ID_DTMB_SET_VAR_RSIZE+1))
	{
		//ucData=mccGET_QUEUE(aucData, ccCMD_ID_DTMB_SET_VAR_VALUE);
		
		//sztmp.Format("fgSetFwVarDTMB,addr=0x%x,value=0x%x,read = 0x%x",u2Addr,mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_SET_VAR_VALUE),ucData);
		//AfxMessageBox(sztmp);
		return TRUE;
	}
	else
		return FALSE;
}

//BOOL fgGetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData,UCHAR ucCnt)
BOOL fgGetFwVarDTMB(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{	
	UCHAR	aucData[cMAX_READ_NUM + 1]; 		// For Host Cmd
//	UCHAR 	ii=0;
//	if((ucCnt<1) || (ucCnt>4))
//		ucCnt = 1;
	
	mccSET_CMD2(aucData) = ccCMD_ID_DTMB_GET_VAR;	
	mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_GET_VAR_ADDRH) = mcHIGH_BYTE(u2Addr);
	mccSET_QUEUE2(aucData,ccCMD_ID_DTMB_GET_VAR_ADDRL) = mcLOW_BYTE(u2Addr);

	//AfxMessageBox("get var hostcmd=??");
	if(mcHOST_CMD(psDemodCtx, aucData, 4, 3))
	{
		/*
		for(ii=0;ii<ucCnt;ii++)
		{
			pucData[ii] = mccGET_QUEUE(aucData, ccCMD_ID_DTMB_GET_VAR_VALUE1 + ii);
			mcSHOW_DBG_MSG1(("the %d th data = 0x%x",ii,mccGET_QUEUE(aucData, ccCMD_ID_DTMB_GET_VAR_VALUE1 + ii)));
		}
		*/
		//pucData[0] = mccGET_QUEUE(aucData, ccCMD_ID_DTMB_GET_VAR_VALUE1);
		mcSHOW_DBG_MSG1(("[fgGetFwVarDTMB]addr=0x%x,Value=0x%x\n",u2Addr,mccGET_QUEUE(aucData, ccCMD_ID_DTMB_GET_VAR_VALUE1)));
		//AfxMessageBox("pucdata=??");
		return TRUE;
	}
	else
		return FALSE;
}

void DTMB_SetTdiAddr(void)
{
	FBM_POOL_T* prFbmPool;    
	// get addr
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_HW_DEMOD);

	if(NULL == prFbmPool)
	{
		mcSHOW_DBG_MSG(("[DTMB_GetTdiAddr]NULL == prFbmPool\n"));
		return;
	}

	if(0==prFbmPool->u4Addr)
	{
		mcSHOW_DBG_MSG(("[DTMB_GetTdiAddr]NULL == tdi addr\n"));
		return;
	}

	mcSHOW_DBG_MSG(("[DTMB_GetTdiAddr]addr = 0x%x, size = 0x%x\n",prFbmPool->u4Addr, prFbmPool->u4Size));

	// set tdi addr 
	mcSET_SHOW_REG(cRISC_DTMB_BASE, 0x9d0, prFbmPool->u4Addr);
}


BOOL DTMB_FwSwitch(DEMOD_CTX_T *psDemodCtx)//not ready
{
	
//	UCHAR aucsldData[cMAX_READ_NUM + 1];
    UCHAR ucValue;
    UCHAR ucData = 0x00;
	UINT8 * pCodeImage=NULL;
	UINT16 u2CodeImageLen;

	if(!psDemodCtx->psTunerCtx->fgRFTuner)//Legacy tuner
    {
		DTMB_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	    mcCLR_BIT(ucData, 3);
		mcSET_BIT(ucData, 2);
	    DTMB_SetReg(psDemodCtx, 0xf62, &ucData, 1);
		mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]dis RF,en IF agc------\n"));
    }
	else
	{//Sawless RF by Tuner,IF PGA close loop
		DTMB_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	    mcCLR_BIT(ucData, 3);
		mcCLR_BIT(ucData, 2);
	    DTMB_SetReg(psDemodCtx, 0xf62, &ucData, 1);
		mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]SET RFIF AGC in FW swith!------\n"));
	}

	//reset sld
	
	mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]reset sld------\n"));
	ucData=0x00;
	DTMB_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData=0xFF;
	DTMB_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData=0x00;
	DTMB_SetReg(psDemodCtx, 0xf61, &ucData, 1);
	ucData=0xFF;
	DTMB_SetReg(psDemodCtx, 0xf61, &ucData, 1);


    DTMB_GetReg(psDemodCtx, 0x0F/*MT5391_REG_uP_GP_REG07*/, &ucValue, 1);
	mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]0x0F=0x%x------\n",ucValue));
#if HOSTCMD_UP
    if(0x02 != ucValue)
#else
    if(psDemodCtx->u1LoadFwOpt1)
#endif
    {
		
        if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
        {
            pCodeImage=ucDtmbCodeImage;
            u2CodeImageLen=sizeof(ucDtmbCodeImage);
			mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]Load sawless fw------\n"));
        }
        else//legacy tuner
        {
            pCodeImage=ucDtmbCodeImage;//ucDtmbCodeImageLeg;
            u2CodeImageLen=sizeof(ucDtmbCodeImage/*ucDtmbCodeImageLeg*/);
			mcSHOW_DBG_MSG(("-----[DTMB_FwSwitch]Load low if fw------\n"));
        }

		
        if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
        {
           mcSHOW_USER_MSG(("[DTMB_FwSwitch]download dtmb FW fail!!\n"));
        }
		#if 0
        UCHAR aucData[cMAX_READ_NUM + 1], ii;

        mccSET_CMD2(aucData) = ccCMD_ID_DTMBC_SWITCH;
        mccSET_QUEUE2(aucData, ccOFST_DTMBC_SWITCH) = 0x02;//0x01:DVBC, 0x02: DTMB
        mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DTMBC_SWITCH_SIZE+1);//lei121119

        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DTMB_GetReg(psDemodCtx, MT5391_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }
        mcSHOW_USER_MSG(("[DTMB_FwSwitch]FW switch to DTMB!"));
		#endif
    }
	fgDTMB_FwStart(psDemodCtx);

    // set agc parameters
    //DTMB_SetIF(psDemodCtx);     // set IF when FW switch

	DTMB_SetTunerRel(psDemodCtx);

    return TRUE;
}


