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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_demod.c
 *  DVBC demod related API implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "pi_demod.h"
#include "mt5135c_com_defs.h"
#include "pd_common.h"
#include "pd_API.h"
#if defined(CC_MT5136) 
#include "code_img_MT5136C.h"
#include "code_img_MT5136C_LEG.h"
#elif defined(CC_MT5135) || defined(CC_MT5135DVBC) 
#include "code_img_MT5135C.h"
#include "code_img_MT5135C_LEG.h"
#elif defined(CC_MT5396) || defined(CC_MT5368) //EC_code_dB330: For MT5396DVBTC demod
#include "code_img_MT5396C.h"
#include "code_img_MT5396C_LEG.h"
#elif defined(CC_MT5398)|| defined(CC_MT5880) 
#include "code_img_MT5398C.h"
#include "code_img_MT5398C_LEG.h"
#elif defined(CC_MT5881) 
#include "code_img_MT5881C.h"
#include "code_img_MT5881C_LEG.h"
#elif defined(CC_MT5399)
#include "code_img_MT5399C.h"
#include "code_img_MT5399C_LEG.h"
#elif defined(CC_MT5890)
#include "code_img_MT5890C.h"
#include "code_img_MT5890C_LEG.h"
#elif defined(CC_MT5882)
#include "code_img_MT5882C.h"
#include "code_img_MT5882C_LEG.h"
#endif

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) 
    #define fg5135_GPIOOVERWRITE_DIS    0
#else
    #define fg5135_GPIOOVERWRITE_DIS    1
#endif

#include "ctrl_bus.h"  //add by liuqu,20090429
#include "spi_if.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "eeprom_if.h"


//-----------------------------------------------------------------------------
// Compile option define
//Lei_code_dB0413: For MT5396DVBTC demod
#if defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398) || defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
     #define cRISC_DVBC_BASE          (IO_VIRT + 0x2A000) 
	 #if defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
     #define cRISC_ACD_BASE           (IO_VIRT + 0x61000) 
	 #elif defined(CC_MT5880)||defined(CC_MT5881)
	 #define cRISC_ACD_BASE           (IO_VIRT + 0x60000) //Lei_code_dB120307 
	 #endif
     #ifndef CKGEN_BASE
          #define CKGEN_BASE               (IO_VIRT + 0x0d000) 
     #endif
     #define cRISC_AGC_BASE           (IO_VIRT + 0x0d000) //weile add here for turn on/off IFAGC/RFAGC 2012.7.4
#endif

#if defined(CC_MT5881)
#ifndef REMY_PINMUX__BASE
#define REMY_PINMUX__BASE        (IO_VIRT + 0x28000) 
#endif
#endif


//-----------------------------------------------------------------------------

static BOOL fgTCSwitch = FALSE;//to define if the source is switch from T/T2
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define fcMT5136_DEV_DIS_TEMP    0
#define fcMT5136_DEV_EN_TEMP     1
#define fcLOAD_CODE_TIME        (1 && fcINTERNAL_TEST)

//-----------------------------------------------------------------------------
static void  vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len);

// Configurations
//-----------------------------------------------------------------------------
#define fcMEAS_HOST_CMD_TIME    (0 && fcINTERNAL_TEST)
#define fcSKIP_LOAD_CODE        (0 && fcINTERNAL_TEST)
#define fcADD_HOSTCMD_CHKSUM    0


/*******************************************************/
#define ESTIMATE_SYMBOL_MAX     7000       // 7MBaud/s
#define ESTIMATE_SYMBOL_MIN     4000       // 4MMBaud/s
#define ESTIMATE_SYMBOL_OFFSET  300        //300KBaud/s

#define ESTIMATE_SYMBOL_ACT_MAX  ((ESTIMATE_SYMBOL_MAX) + (ESTIMATE_SYMBOL_OFFSET))
#define ESTIMATE_SYMBOL_ACT_MIN  ((ESTIMATE_SYMBOL_MIN) - (ESTIMATE_SYMBOL_OFFSET))

/*******************************************************/
#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\


UINT32 DVBC_LOG10(UINT32 u4InputVale)
{
    UINT32 u4TempValue = u4InputVale;
    UINT32 u4ReturnValue = 0;
    UINT8  u1Index = 0;

    while(u4TempValue>=10)
    {
        u4TempValue = u4TempValue/10;
        u1Index++;
    }
    switch(u4TempValue)
    {
        case 0:
        case 1:
            u4ReturnValue = u1Index*100;
            break;
        case 2: 
            u4ReturnValue = u1Index*100+30;
            break;
        case 3: 
            u4ReturnValue = u1Index*100+48;
            break;
        case 4: 
            u4ReturnValue = u1Index*100+60;
            break;
        case 5: 
            u4ReturnValue = u1Index*100+70;
            break;
        case 6: 
            u4ReturnValue = u1Index*100+78;
            break;
        case 7: 
            u4ReturnValue = u1Index*100+85;
            break;
        case 8: 
            u4ReturnValue = u1Index*100+90;
            break;
        case 9: 
            u4ReturnValue = u1Index*100+95;
            break;
    }
    
    return u4ReturnValue;/* The Return Value Enlarge 100*/
}



static INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50);

/***********************************************************************/
/*                  Private (static) Function Prototypes                                                     */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipInit
 *  This function initialize DVB-C related registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_ChipInit(DEMOD_CTX_T *psDemodCtx)
{
#if 0//(defined(CC_MT5882) || defined(CC_MT5890)) && defined(INTERNAL_DTMB)	
UCHAR ucValue;
#endif

#if defined(CC_MT5136) //Mingsheng 120423 add for chipint
    UINT8 ucTmp;
    mcSHOW_DBG_MSG(("555136 DVBC_ChipInit\n"));
    if (!psDemodCtx)
    {
	   return 1;
    }


    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8D5, &ucTmp, 1);
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8D6, &ucTmp, 1);
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8DA, &ucTmp, 1);
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8D2, &ucTmp, 1);
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8D3, &ucTmp, 1);
    ucTmp = 0x2c;   DVBC_SetReg(psDemodCtx, 0x8DD, &ucTmp, 1);

	/************************Step I ****************************/
    // Pull-down IO CUP if NC in application
    //0x0806	=	0x60	;// PAD_PVR_TS_DATA1
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PVR_TS_DATA1, &ucTmp, 1);
    //0x0808	=	0x60	;// PAD_PVR_TS_DATA0
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PVR_TS_DATA0, &ucTmp, 1);
    //0x080A	=	0x60	;// PAD_PVR_TS_SYNC
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PVR_TS_SYNC, &ucTmp, 1);
    //0x080C	=	0x60	;// PAD_PVR_TS_VAL
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PVR_TS_VAL, &ucTmp, 1);
    //0x080E	=	0x60	;// PAD_PVR_TS_CLK
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PVR_TS_CLK, &ucTmp, 1);
    //0x0828	=	0x60	;// PAD_EXT_TS_CLK
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_CLK, &ucTmp, 1);
    //0x082A	=	0x60	;// PAD_EXT_TS_SYNC
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_SYNC, &ucTmp, 1);
    //0x082C	=	0x60	;// PAD_EXT_TS_VAL
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_VAL, &ucTmp, 1);
    //0x082E	=	0x60	;// PAD_EXT_TS_DATA0
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA0, &ucTmp, 1);
    //0x0830	=	0x60	;// PAD_EXT_TS_DATA1
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA1, &ucTmp, 1);
    //0x0832	=	0x60	;// PAD_EXT_TS_DATA2
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA2, &ucTmp, 1);
    //0x0834	=	0x60	;// PAD_EXT_TS_DATA3
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA3, &ucTmp, 1);
    //0x0836	=	0x60	;// PAD_EXT_TS_DATA4
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA4, &ucTmp, 1);
    //0x0838	=	0x60	;// PAD_EXT_TS_DATA5
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA5, &ucTmp, 1);
    //0x083A	=	0x60	;// PAD_EXT_TS_DATA6
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA6, &ucTmp, 1);
    //0x083C	=	0x60	;// PAD_EXT_TS_DATA7
    ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_EXT_TS_DATA7, &ucTmp, 1);

    // if (0x08FB, bit[5:4]=2'b01), QFN48
    DVBC_GetReg(psDemodCtx, PADMUX_MIX_RD,  &ucTmp, 1);
    ucTmp = (ucTmp & 0x30) >> 4;
    if(ucTmp == 0x01)
    {  
        mcSHOW_DBG_MSG(("QFN48 Config \n"));     
        // Pull-down NC IO CUP  
        //0x0800	=	0x60	// PAD_SPI_CLE
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_SPI_CLE, &ucTmp, 1);        
        //0x0802	=	0x60	// PAD_SPI_DATA
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_SPI_DATA, &ucTmp, 1);                
        //0x0804	=	0x60	// PAD_SPI_CLK
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_SPI_CLK, &ucTmp, 1);                        
        //0x0810	=	0x60	// PAD_CI_TS_DATA0
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CI_TS_DATA0, &ucTmp, 1);                        
        //0x0812	=	0x60	// PAD_CI_TS_SYNC
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CI_TS_SYNC, &ucTmp, 1);                        
        //0x0814	=	0x60	// PAD_CI_TS_VAL
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CI_TS_VAL, &ucTmp, 1);                        
        //0x0816	=	0x60	// PAD_CI_TS_CLK
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CI_TS_CLK, &ucTmp, 1);                        
        //0x0818	=	0x60	// PAD_DEMOD_TS_DATA0
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_DEMOD_TS_DATA0, &ucTmp, 1);                        
        //0x081A	=	0x60	// PAD_DEMOD_TS_SYNC
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_DEMOD_TS_SYNC, &ucTmp, 1);                        
        //0x081C	=	0x60	// PAD_DEMOD_TS_VAL
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_DEMOD_TS_VAL, &ucTmp, 1);                        
        //0x081E	=	0x60	// PAD_DEMOD_TS_CLK
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_DEMOD_TS_CLK, &ucTmp, 1);                        
        //0x0820	=	0x60	// PAD_TUNER_DATA
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_TUNER_DATA, &ucTmp, 1);                        
        //0x0822	=	0x60	// PAD_TUNER_CLK
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_TUNER_CLK, &ucTmp, 1);                        
        //0x0824	=	0x60	// PAD_IF_AGC
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_IF_AGC, &ucTmp, 1);                        
        //0x0826	=	0x60	// PAD_RF_AGC
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_RF_AGC, &ucTmp, 1);                        
        //0x0844	=	0x60	// PAD_CD1_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CD1_, &ucTmp, 1);                        
        //0x0846	=	0x60	// PAD_D3
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D3, &ucTmp, 1);                        
        //0x0848	=	0x60	// PAD_D11
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D11, &ucTmp, 1);                        
        //0x084A	=	0x60	// PAD_D4
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D4, &ucTmp, 1);                        
        //0x0854	=	0x60	// PAD_D14
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D14, &ucTmp, 1);                        
        //0x0856	=	0x60	// PAD_D7
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D7, &ucTmp, 1);                        
        //0x0858	=	0x60	// PAD_D15
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_D15, &ucTmp, 1);                        
        //0x085A	=	0x60	// PAD_CE1_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CE1_, &ucTmp, 1);                        
        //0x085C	=	0x60	// PAD_A10
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A10, &ucTmp, 1);                        
        //0x085E	=	0x60	// PAD_VS1_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_VS1_, &ucTmp, 1);
        //0x0860	=	0x60	// PAD_OE_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_OE_, &ucTmp, 1);        
        //0x0862	=	0x60	// PAD_IORD_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_IORD_, &ucTmp, 1);        
        //0x0864	=	0x60	// PAD_A11
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A11, &ucTmp, 1);
        //0x0866	=	0x60	// PAD_IOWR_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_IOWR_, &ucTmp, 1);        
        //0x0868	=	0x60	// PAD_A9
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A9, &ucTmp, 1);        
        //0x086A	=	0x60	// PAD_A17
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A17, &ucTmp, 1);        
        //0x086C	=	0x60	// PAD_A8
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A8, &ucTmp, 1);        
        //0x086E	=	0x60	// PAD_A18
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A18, &ucTmp, 1);        
        //0x087C	=	0x60	// PAD_READY
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_READY, &ucTmp, 1);        
        //0x0880	=	0x60	// PAD_A16
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A16, &ucTmp, 1);        
        //0x0882	=	0x60	// PAD_A23
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A23, &ucTmp, 1);        
        //0x0884	=	0x60	// PAD_A15
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A15, &ucTmp, 1);        
        //0x0886	=	0x60	// PAD_A24
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A24, &ucTmp, 1);        
        //0x0888	=	0x60	// PAD_A12
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A12, &ucTmp, 1);        
        //0x088A	=	0x60	// PAD_A25
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A25, &ucTmp, 1);        
        //0x088C	=	0x60	// PAD_A7
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A7, &ucTmp, 1);        
        //0x088E	=	0x60	// PAD_VS2_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_VS2_, &ucTmp, 1);        
        //0x0890	=	0x60	// PAD_A6
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A6, &ucTmp, 1);        
        //0x0892	=	0x60	// PAD_PCMCIA_RESET
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_PCMCIA_RESET, &ucTmp, 1);        
        //0x0894	=	0x60	// PAD_A5
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A5, &ucTmp, 1);        
        //0x0896	=	0x60	// PAD_WAIT_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_WAIT_, &ucTmp, 1);        
        //0x0898	=	0x60	// PAD_A4
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A4, &ucTmp, 1);        
        //0x089C	=	0x60	// PAD_REG_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_REG_, &ucTmp, 1);        
        //0x089E	=	0x60	// PAD_A2
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A2, &ucTmp, 1);        
        //0x08A0	=	0x60	// PAD_BVD2
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_BVD2, &ucTmp, 1);
        //0x08A2	=	0x60	// PAD_A1
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A1, &ucTmp, 1);        
        //0x08A4	=	0x60	// PAD_BVD1
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_BVD1, &ucTmp, 1);        
        //0x08A6	=	0x60	// PAD_A0
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_A0, &ucTmp, 1);        
        //0x08B4	=	0x60	// PAD_CD2_
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CD2_, &ucTmp, 1);        
        //0x08B6	=	0x60	// PAD_CI_INT
        ucTmp = 0x60;   DVBC_SetReg(psDemodCtx, PAD_CI_INT, &ucTmp, 1);        
        
    }
    
    // Peripheral soft reset
    //0x0E3B	=	0x00	;// Reset peripheral
    //ucTmp = 0x20;   DVBC_SetReg(psDemodCtx, 0xE3B, &ucTmp, 1);    
    //0x0E3C	=	0x00	;// Reset peripheral
    //ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0xE3C, &ucTmp, 1);    
    //0x0E3B	=	0xFF	;// Release peripheral
    //ucTmp = 0xFC;   DVBC_SetReg(psDemodCtx, 0xE3B, &ucTmp, 1);    
    //0x0E3C	=	0xFF	;// Release peripheral 
    //ucTmp = 0x80;   DVBC_SetReg(psDemodCtx, 0xE3C, &ucTmp, 1);

	//ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, 0x8d0, &ucTmp, 1);
	//ucTmp = 0xFF;   DVBC_SetReg(psDemodCtx, 0x8d0, &ucTmp, 1);
    /************************End Step I ****************************/


	/************************ Step II****************************/
    // IFPLL settings				
    //0x0910	=	0x9F	;// RG_ULJPLL_PWD=1; (bit[7]=1'b1)
//    ucTmp = 0x9F;   DVBC_SetReg(psDemodCtx, 0x910, &ucTmp, 1);
    //0x0918	=	0xC0	;// RG_CLKEN=1; RG_DCKEN=1; (bit[7:6]=2'b11)
    ucTmp = 0xC0;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG8, &ucTmp, 1);
    //0x0917	=	0x20	;// RG_ULJPLL_MONEN=1; (bit[5]=1b'1)
    ucTmp = 0x20;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG7, &ucTmp, 1);
    //0x0914	=	0x2F	;// RG_ULJPLL_AUTOK_VCO=1; (bit[0]=1'b1)
    ucTmp = 0x2F;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG4, &ucTmp, 1);    
    //0x0915	=	0x97	;// RG_ULJPLL_AUTOK_LOAD=1; RG_ULJPLL_LOAD_RSTB=1; (bit[7]=1'b1; bit[0]=1'b1)
    ucTmp = 0x97;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG5, &ucTmp, 1);
    //0x0910	=	0x1F	;// RG_ULJPLL_PWD=0; (bit[7]=1'b0)
    ucTmp = 0x1F;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG0, &ucTmp, 1);

    // delay 100us ==> By applying stepIII script	
    mcDELAY_MS(2);
	/************************ End Step II****************************/
    
    // post ANA regsiter after 100us delay				
    //0x0914	=	0x2E	;// RG_ULJPLL_AUTOK_VCO=0; (bit[0]=1'b0)
    ucTmp = 0x2E;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG4, &ucTmp, 1);    
    //0x0917	=	0x20	;// RG_ULJPLL_DIV3_RSTB=0; (bit[3]=1b'0)
    ucTmp = 0x20;   DVBC_SetReg(psDemodCtx, REG_IFPLL_CFG7, &ucTmp, 1);     
    // PGA power up				
    //0x0909	=	0x00	;// RG_PGAVOCM_PWD=0; RG_PGA_PWD=0; (bit[2:1]=2'b00)
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, REG_IFPGA_CFG1, &ucTmp, 1);     
    // ADC power up				
    //0x0903	=	0x82	;// RG_ADC_PWD_DEMOD=0; (bit[0]=1'b0)
    ucTmp = 0x82;   DVBC_SetReg(psDemodCtx, REG_IFADC_CFG3, &ucTmp, 1);     
    // ADC deadlock prevention				
    //0x0902	=	0xA0	;// RG_CKRATE_DEMOD[1:0]=2'b00; (bit[3:2]=2'b00)
    ucTmp = 0xA0;   DVBC_SetReg(psDemodCtx, REG_IFADC_CFG2, &ucTmp, 1);     
    //0x0902	=	0xA8	;// RG_CKRATE_DEMOD[1:0]=2'b10; (bit[3:2]=2'b10)
    ucTmp = 0xA8;   DVBC_SetReg(psDemodCtx, REG_IFADC_CFG2, &ucTmp, 1);     

    // PLLGP settings				
    //0x0921	=	0x48	;// RG_HADDS2_PWD=0; (bit[7]=1'b0)
    ucTmp = 0x48;   DVBC_SetReg(psDemodCtx, REG_PLL_CFG0, &ucTmp, 1);     
    //0x0924	=	0x27	;// RG_DDS1_DDS_PWDB=1; (bit[5]=1'b1)
    ucTmp = 0x27;   DVBC_SetReg(psDemodCtx, REG_PLL_CFG3, &ucTmp, 1);     
    //0x0924	=	0x67	;// RG_DDS1_DDS_RSTB=1; (bit[6]=1'b1)
    ucTmp = 0x67;   DVBC_SetReg(psDemodCtx, REG_PLL_CFG3, &ucTmp, 1);     
    //0x0927	=	0x27	;// RG_DDS2_DDS_PWDB=1; (bit[5]=1'b1)
    ucTmp = 0x27;   DVBC_SetReg(psDemodCtx, REG_PLL_CFG6, &ucTmp, 1);     
    //0x0927	=	0x67	;// RG_DDS2_DDS_RSTB=1; (bit[6]=1'b1)
    ucTmp = 0x67;   DVBC_SetReg(psDemodCtx, REG_PLL_CFG6, &ucTmp, 1);     

    // CKGEN				
    //0x0E50	=	0x00	;// Datapath PLL source: 0x00 for ADCPLL(432MHz); 0x10 for SYSPLL(648MHz)
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, REG_CKGEN_PLL0, &ucTmp, 1);     
    //0x0E51	=	0x01	;// 54M source: 0x01 for ADCPLL; 0x03 for SYSPLL
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_54M_0, &ucTmp, 1);     
    //0x0E52	=	0x01	;// Apply 54MHz deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_54M_1, &ucTmp, 1);     
    //0x0E55	=	0x03	;// 86.4M level 1 source: 0x03 for ADCPLL; 0x00 for SYSPLL
    ucTmp = 0x03;   DVBC_SetReg(psDemodCtx, REG_CKGEN_86P4M_0, &ucTmp, 1);     
    //0x0E56	=	0x01	;// Apply 86.4MHz level 1 deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_86P4M_1, &ucTmp, 1);     
    //0x0E57	=	0x03	;// 36M source: 0x03 for ADCPLL; 0x05 for SYSPLL
    ucTmp = 0x03;   DVBC_SetReg(psDemodCtx, REG_CKGEN_36M_0, &ucTmp, 1);     
    //0x0E58	=	0x01	;// Apply 36MHz deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_36M_1, &ucTmp, 1);     
    //0x0E6A	=	0x01	;// ADCLK Div4 (54M) source
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_ADCLK_DIV4_0, &ucTmp, 1);     
    //0x0E6B	=	0x01	;// Apply ADCLK Div4 deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_ADCLK_DIV4_1, &ucTmp, 1);     
    //0x0E6C	=	0x01	;// ADCLK source
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_ADCLK_K_0, &ucTmp, 1);     
    //0x0E6D	=	0x01	;// Apply ADCLK deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_ADCLK_K_1, &ucTmp, 1);     
    //0x0E70	=	0x01	;// 162M level 1 source
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_162M_0, &ucTmp, 1);     
    //0x0E71	=	0x01	;// Apply 162M level 1 deglitch change
    ucTmp = 0x01;   DVBC_SetReg(psDemodCtx, REG_CKGEN_162M_1, &ucTmp, 1);     

	#if 0
	DVBC_GetReg(psDemodCtx, 0xC38, &ucTmp, 1);
    mcSHOW_DBG_MSG(("read 0xC38 = 0x%02X\n", ucTmp));

	ucTmp = ucTmp|0x04;  //S->P
	DVBC_SetReg(psDemodCtx, 0xC38, &ucTmp, 1); //0xE3C bit 7,FEC logic reset,bit 6, FEC register reset

	DVBC_GetReg(psDemodCtx, 0xC38, &ucTmp, 1);
    mcSHOW_DBG_MSG(("read 0xC38 = 0x%02X\n", ucTmp));
	#endif

    // SLD ANAIF				
    //0x0F80	=	0x03	;// AD_CKPHSEL=1 (bit[1]=1'b1); AD_FMT(binary-offset)=1 (bit[0]=1'b1)
    ucTmp = 0x03;   DVBC_SetReg(psDemodCtx, REG_AD_CKPHSEL, &ucTmp, 1);     
    //0x0E01	=	0x00	;// Reset ANAIF
    ucTmp = 0x00;   DVBC_SetReg(psDemodCtx, REG_ANAIF_RESET, &ucTmp, 1);     
    //0x0E01	=	0x80	;// Release ANAIF
    ucTmp = 0x80;   DVBC_SetReg(psDemodCtx, REG_ANAIF_RESET, &ucTmp, 1);
    
    //SLD IF/RF AGC setting
    //RF_AGC_PAD_EN: 0xF62[3] = 0 (hiZ)
    //IF_AGC_PAD_EN: 0xF62[2] = 1 (Enable)
    DVBC_GetReg(psDemodCtx, 0xF62, &ucTmp, 1);
    mcCLR_BIT(ucTmp, 3);
    mcSET_BIT(ucTmp, 2);
    DVBC_SetReg(psDemodCtx, 0xF62, &ucTmp, 1);  

	//lei120427 added for larger the clk and data output driver 
	//TS Clock
    ucTmp = 0x44;   DVBC_SetReg(psDemodCtx, REG_PADMUX_TS_CLK_CFG, &ucTmp, 1);
    //TS Data 0
    ucTmp = 0x44;   DVBC_SetReg(psDemodCtx, REG_PADMUX_TS_DATA0_CFG, &ucTmp, 1);
    // System initial (CH switch) ==> Apply stepIV

    mcSHOW_DBG_MSG(("5136 DVBC_ChipInit done\n"));
#elif defined(CC_MT5396)|| defined(CC_MT5368) //Lei_code_dB0413: For MT5396DVBTC demod
	UINT32 u4Temp;
    UINT32 u4GpioforRFAGCSwitch;

// R/W register mode :in-direct  to direct
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	u4Temp|=0x000f0000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);
//ACD  settting
//w 0xf0061500 0x00000100
//w 0xf0061504 0x0008A694
//0x0008A894--->0x0008A694,change for CC Hou,just for the THD of the low power(3V) more margin
//w 0xf0061400 0x00AAA882
	u4Temp = 0x00000100;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b00,PGA power on
	u4Temp = 0x0008A694;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x504, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b0,ADC power on

	//For ADC issue(change TV repeatly,SNR become lower and ber larger,or no signal),lei20120202;
	mcDELAY_MS(1);
	u4Temp = 0x00AAA082;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);

//Demod clock  settting
//w 0xf000d204 0x00000100
//w 0xf000d304 0x00000001
//w 0xf000d218 0x00000001
//w 0xf000d21c 0x00000002
//w 0xf000d300 0x00000002
//w 0xf000d308 0x00000003
//w 0xf000d30c 0x00000104
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x204);
	u4Temp &= 0x8FFFF8FF;
	u4Temp |= 0x00000100;
	mcSET_SHOW_REG(CKGEN_BASE, 0x204, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x304);
	u4Temp &= 0xFFFFFF0C;
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x304, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x218);
	u4Temp &= 0xFFFFFF08;
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x218, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x21c);
	u4Temp &= 0xFFFFFF0c;
	u4Temp |= 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x21c, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x300);
	u4Temp &= 0xFFFFFF0c;
	u4Temp |= 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x300, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x308);
	u4Temp &= 0xFFFFFF08;
	u4Temp |= 0x00000003;
	mcSET_SHOW_REG(CKGEN_BASE, 0x308, u4Temp);
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x30c);
	u4Temp &= 0xFFFFFE00;
	u4Temp |= 0x00000104;
	mcSET_SHOW_REG(CKGEN_BASE, 0x30c, u4Temp);
//MT5396
// IFAGC pin mux2 [27] =  1, pinmux2=0x408
// RFAGC, pin mux1[19] = 1, pinmux1=0x404
//MT568
// IFAGC, pin mux2 [20:19] = 01, pinmux2=0x408
// RFAGC, pin mux2[22:21] = 01, pinmux2=0x408
		
	#if defined(CC_MT5396)
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp |= 0x08000000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x404);
			u4Temp |= 0x00080000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x404, u4Temp);		
	#elif defined(CC_MT5368)
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp &= 0xFFEFFFFF;
			u4Temp |= 0x00080000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
			u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
			u4Temp &= 0xFFBFFFFF;
			u4Temp |= 0x00200000;
			mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);
	#endif

//Select DVBC
//w 0xf002ae00 0x00001001  //EU mode DVBC
    u4Temp = 0x00001003;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xe00, u4Temp);
	mcSHOW_USER_MSG(("select DVBC Mode\n"));
//RF AGC switch
	if(DRVCUST_OptQuery(eTunerRFAGCSwitchGpio,&u4GpioforRFAGCSwitch)==0)
	{
		 GPIO_SetOut(u4GpioforRFAGCSwitch,1);//d20110503_Lei,:Gpio set high for DTV
		 mcSHOW_DBG_MSG(("GPIO %d set high to switch RF AGC control\n",u4GpioforRFAGCSwitch));
	}
	

// RFAGC internal control by tuner, set RFAGC pin to be HiZ
#if 0	
	UINT8 ucData;
	DVBT_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	mcSHOW_DBG_MSG(("0xf62=%02x\n",ucData));
    mcCLR_BIT(ucData, 3);
	mcSET_BIT(ucData, 2);
    DVBT_SetReg(psDemodCtx, 0xf62, &ucData, 1);
	DVBT_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	mcSHOW_DBG_MSG(("0xf62=%02x\n",ucData));
#endif

	//do CDC reset after ADC and Demod clock setting,lei120326
	//0xE01[4]=1'b0,reset;=1'b1,enable; CDC ,lei110901;
	//first set 0,reset
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp &= 0xFFFFEFFF;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	//sencond set 1,enable
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp |= 0x00001000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	mcSHOW_DBG_MSG(("Have reset and enable CDC.\n"));
#elif defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	UINT32 u4Temp;
	UINT32 u4GpioforRFAGCSwitch;
		
// R/W register mode :in-direct  to direct
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	u4Temp|=0x000f0000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);

//ACD Setup
//ACD settings to enable ADC/PGA/PLL
//Cobra:
//w 0xf0061500 0x00000100
//w 0xf0061504 0x0008A694
//0x0008A894--->0x0008A694,change for CC Hou,just for the THD of the low power(3V) more margin
//w 0xf0061400 0x00AAA882
//Viper:
//w 0xf0060600 0x00000100
//w 0xf0060604 0x0008A694
//w 0xf0060500 0x00AAA882

	#if defined(CC_MT5398)||defined(CC_MT5399)||defined(CC_MT5882)
	u4Temp = 0x00000100;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);
	#if defined(CC_MT5398)
	u4Temp = 0x0008A694;
	#elif defined(CC_MT5399)||defined(CC_MT5882)
	u4Temp = 0x0008A894;
	#endif
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x504, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);
	
	//For ADC issue(change TV repeatly,SNR become lower and ber larger,or no signal),lei20120202;
	mcDELAY_MS(1);
	u4Temp = 0x00AAA082;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);
	#elif defined(CC_MT5880)||defined(CC_MT5881)
	u4Temp = 0x00000100;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x600, u4Temp);
	#if defined(CC_MT5880)
	u4Temp = 0x0008A694;
	#elif defined(CC_MT5881)
	u4Temp = 0x0008A894;//for ACD refine,lei121123
	#endif
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x604, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);

	//For ADC issue(change TV repeatly,SNR become lower and ber larger,or no signal),lei20120202;
	mcDELAY_MS(1);
	u4Temp = 0x00AAA082;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);
	u4Temp = 0x00AAA882;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);
	#endif

//140517_XJ for Oryx New ADC setting
	#if defined(CC_MT5890)
	u4Temp = 0x00041533;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//set RG_DMRX_EN =0
	u4Temp = 0x33040912;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x404, u4Temp);//set demod input common mode
	u4Temp = 0x49AAA434;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x408, u4Temp);//set LDO voltage
	u4Temp = 0x881100B0;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//set RG_CIC_FLT_EN =0
	u4Temp = 0x40041533;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//set RG_DMRX_en =1
	u4Temp = 0x883100B0;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//set RG_CIC_FLT_EN =1
	#endif
	
		
//DEMOD clock setting
//w 0xf000d3e0 0x00000001   ->c27M / 27M
//w 0xf000d308 0x00000001   ->atdad / 216M
//w 0xf000d30c 0x00000002	   ->clk54M / 54M
//w 0xf000d388 0x00000002   ->clk216M / 216M
//w 0xf000d38c 0x00000003   ->dvbtci / 54M
//w 0xf000d3dc 0x00000200   ->dvbtfec / xtal
	u4Temp = 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x3e0, u4Temp);
	u4Temp = 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x308, u4Temp);
	u4Temp = 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x30c, u4Temp);
	u4Temp = 0x00000002;
	mcSET_SHOW_REG(CKGEN_BASE, 0x388, u4Temp);
	u4Temp = 0x00000003;
	mcSET_SHOW_REG(CKGEN_BASE, 0x38c, u4Temp);
	u4Temp = 0x00000200;
	mcSET_SHOW_REG(CKGEN_BASE, 0x3dc, u4Temp);
	//Remy
	//w 0xf000d428 0x00000002  ->demod_vdac_ck/54M
	//Mustang
	//w 0xf000d42c 0x00000002  ->demod_vdac_ck/54M
	#if defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	u4Temp = 0x00000002;
	#if defined(CC_MT5881)
	mcSET_SHOW_REG(CKGEN_BASE, 0x428, u4Temp);
	#elif defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	mcSET_SHOW_REG(CKGEN_BASE, 0x42c, u4Temp);
	#endif
	#endif

	#if defined(CC_MT5882)
	u4Temp = 0x00000004;
	mcSET_SHOW_REG(CKGEN_BASE, 0x460, u4Temp);//140729 dstx_ck 108Mhz
	#endif
	
//IF/RF AGC Pinmux Enable
//Cobra/Mustang/Oryx
//w 0xf000d604[7]=1
//w 0xf000d604[11]=1
//Viper
//w 0xf000d608[26]=1
//w 0xf000d608[27]=1
//Remy
//w 0xf00280c0[24:21]=1
//w 0xf00280c0[28:25]=2

	#if defined(CC_MT5398)||defined(CC_MT5399)||defined(CC_MT5890)
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x604);
	if(psDemodCtx->psTunerCtx->fgRFTuner)
    {
        u4Temp |= 0x00000880;
		mcSHOW_DBG_MSG(("[XJ]Sawless tuner,Enable RF_AGC\n"));
    }
    else
    {
        u4Temp |= 0x00000080;
		mcSHOW_DBG_MSG(("[XJ]Leagacy tuner,No need to enable RF_AGC\n"));
    }
	mcSET_SHOW_REG(CKGEN_BASE, 0x604, u4Temp);
	#elif defined(CC_MT5880)
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x608);
	if(psDemodCtx->psTunerCtx->fgRFTuner)
    {
        u4Temp |= 0x0C000000;
		mcSHOW_DBG_MSG(("[XJ]Sawless tuner,Enable RF_AGC\n"));
    }
    else
    {
        u4Temp |= 0x04000000;
		mcSHOW_DBG_MSG(("[XJ]Leagacy tuner,No need to enable RF_AGC\n"));
    }
	mcSET_SHOW_REG(CKGEN_BASE, 0x608, u4Temp);
	#elif defined(CC_MT5881)
	u4Temp = mcRISC_Reg(REMY_PINMUX__BASE, 0x0c0);
	u4Temp &= 0xE01FFFFF;
	if(psDemodCtx->psTunerCtx->fgRFTuner)
    {
        u4Temp |= 0x04200000;
		mcSHOW_DBG_MSG(("[XJ]Sawless tuner,Enable RF_AGC\n"));
    }
    else
    {
        u4Temp |= 0x00200000;
		mcSHOW_DBG_MSG(("[XJ]Leagacy tuner,No need to enable RF_AGC\n"));
    }
	mcSET_SHOW_REG(REMY_PINMUX__BASE, 0x0c0, u4Temp);
	#elif defined(CC_MT5882)
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x604);
	if(psDemodCtx->psTunerCtx->fgRFTuner)
    {
        u4Temp &= 0xE9FFFFFF;
		mcSHOW_DBG_MSG(("[XJ]Sawless tuner,Enable RF_AGC\n"));
    }
    else
    {
        u4Temp &= 0xE8FFFFFF;
		mcSHOW_DBG_MSG(("[XJ]Leagacy tuner,No need to enable RF_AGC\n"));
    }
	mcSET_SHOW_REG(CKGEN_BASE, 0x604, u4Temp);
	#endif
		
//Select DVBC
//w 0xf002ae00 0x00001001  //EU mode DVBC
	u4Temp = 0x00001003;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xe00, u4Temp);
	mcSHOW_USER_MSG(("select DVBC Mode\n"));
//RF AGC switch
	if(DRVCUST_OptQuery(eTunerRFAGCSwitchGpio,&u4GpioforRFAGCSwitch)==0)
	{
		GPIO_SetOut(u4GpioforRFAGCSwitch,1);//d20110503_Lei,:Gpio set high for DTV
		mcSHOW_DBG_MSG(("GPIO %d set high to switch RF AGC control\n",u4GpioforRFAGCSwitch));
	}
	
	//do CDC reset after ADC and Demod clock setting,lei120326
	//0xE01[4]=1'b0,reset;=1'b1,enable; CDC ,lei110901;
	//first set 0,reset
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp &= 0xFFFFEFFF;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	//sencond set 1,enable
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0xE00);
	u4Temp |= 0x00001000;
	mcSET_SHOW_REG(cRISC_DVBC_BASE, 0xE00, u4Temp);
	mcSHOW_DBG_MSG(("Have reset and enable CDC.\n"));
	#if 0//(defined(CC_MT5882) || defined(CC_MT5890)) && defined(INTERNAL_DTMB)		
		DVBC_GetReg(psDemodCtx,0x00, &ucValue,1);		
		mcSHOW_DBG_MSG(("[DVBC_ChipInit]always,get 0x00=0x%x\n",ucValue));

		ucValue=1;
		mcSHOW_DBG_MSG(("[DVBC_ChipInit]always,en UP,set 0x00=0x%x\n",ucValue));
		DVBC_SetReg(psDemodCtx,0x00, &ucValue,1);
		
		//mcDELAY_MS(10);
		DVBC_GetReg(psDemodCtx,0x00, &ucValue,1);	
		mcSHOW_DBG_MSG(("[DVBC_ChipInit]always,get 0x00=0x%x\n",ucValue));
	#endif
#else
UINT8 ucData;
    if (!psDemodCtx)
    {
       return 1;
    }

#if 1
    /* Temp mark. Hui @ 20100308 */
    // PowerOn setting
    ucData = 0x20;
    DVBC_SetReg(psDemodCtx, 0xE90, &ucData, 1);
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE91, &ucData, 1);
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x09;
    //DVBC_SetReg(psDemodCtx, 0xE9B, &ucData, 1);
    //ucData = 0x27;
    //DVBC_SetReg(psDemodCtx, 0xE9E, &ucData, 1);

    ucData = 0x45;
    DVBC_SetReg(psDemodCtx, 0xEAA, &ucData, 1);
    ucData = 0x45;
    DVBC_SetReg(psDemodCtx, 0xEAC, &ucData, 1);
//    ucData = 0x00;
//    DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
//    ucData = 0x01;
//    DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE50, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5D, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5E, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);
#endif

#if 0
    DVBC_GetReg(psDemodCtx, 0x880, &ucData, 1);
    if(ucData == 0x02)
    {
        ucData = 0x03;
        DVBC_SetReg(psDemodCtx, 0x880, &ucData, 1);
    }
    else if(ucData == 0x03)
    {
    }
    else
#endif
    {
#if 0
        // Move CLKOUT to MTK_Loader.
        // set 5365 clock to 5135
        //w 0xf00280dc 0x0d000084: bit25=0, bit24=1, bit2=1
        if (BSP_GetIcVersion() != IC_VER_5365_AB)   // for ECO IC, not set this register for audio test
            mcSET_SHOW_REG(0xf0028000, 0x0dc, 0x0d000084);
#endif
        // Analog AD setting
        ucData = 0x82;
        DVBC_SetReg(psDemodCtx, 0xE93, &ucData, 1);
        ucData = 0xA8;
        DVBC_SetReg(psDemodCtx, 0xE94, &ucData, 1);
        ucData = 0x5A;
        DVBC_SetReg(psDemodCtx, 0xE95, &ucData, 1);
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE96, &ucData, 1);
        // Analog PGA power on
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE97, &ucData, 1);
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE98, &ucData, 1);

        // Ckgen
        // set demod_54m_up_ck to 54m
        ucData = 0x41;   // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD
        DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
        // set demod_216m_mux_ck to 216m
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE54, &ucData, 1);
        // set demod_86p4m_mux_ck to 54m
        ucData = 0x00;
        DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE56, &ucData, 1);
        // set demod_36m_ck to 54m
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xE58, &ucData, 1);

        {
        UINT32 u4Clock = 0;
        DRVCUST_OptQuery(eCISysClock, &u4Clock);
        switch (u4Clock)
        {
            case 144:
                ucData = 0x7; // 144 MHz
                break;
            case 108:
                ucData = 0x6;
                break;
            case 86: // 86.4
                ucData = 0x5;
                break;
            case 62: // 61.7
                ucData = 0x4;
                break;
            case 54:
                ucData = 0x3;
                break;
            case 48:
                ucData = 0x2;
                break;
            case 72:
                ucData = 0x1;
                break;
            case 36:
                ucData = 0xf;
                break;
            case 27:
                ucData = 0x13;
                break;
            default:
                ucData = 0x7;
                break;
        }
        DVBC_SetReg(psDemodCtx, 0xE59, &ucData, 1);   // CI SYS CLK
        ucData = 0x1;
        DVBC_SetReg(psDemodCtx, 0xE5A, &ucData, 1);

        u4Clock = 0;
        DRVCUST_OptQuery(eCITsClock, &u4Clock);
        switch (u4Clock)
        {
            case 48:
                ucData = 0x7; // 144 MHz
                break;
            case 62: //61.7
                ucData = 0x6;
                break;
            case 72: 
                ucData = 0x5;
                break;
            case 86:  //86.4
                ucData = 0x4;
                break;
            case 108:
                ucData = 0x3;
                break;
            case 144:
                ucData = 0x2;
                break;
            case 54:
                ucData = 0x1;
                break;
            case 36:
                ucData = 0xf;
                break;
            case 27:
                ucData = 0x13;
                break;
            default:
                ucData = 0x5;
                break;
        }
        DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);   // CI SYS CLK
        ucData = 0x1;
        DVBC_SetReg(psDemodCtx, 0xE5C, &ucData, 1);
        }

        // Padmux
        ucData = 0x01;
        DVBC_SetReg(psDemodCtx, 0xF8C, &ucData, 1);
        ucData = 0x03;
        DVBC_SetReg(psDemodCtx, 0x880, &ucData, 1);
    }

    // RFAGC internal control by tuner, Ken

    // set RFAGC pin to be HiZ
    DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcSET_BIT(ucData, 2);  // gpio enable
    mcCLR_BIT(ucData, 0);  // gpio as input mode
    DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);

#endif

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetNormalMode
 *  Set demod to normal mode.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetNormalMode(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 fgRet = 0;
    UINT8 ucData = 0x00;
    
    //U32   u4Temp;

    mcSHOW_DBG_MSG(("PI DVBC Set Normal Mode!!!\n"));
   
    // set RFAGC pin to be output (not GPIO)
    /*DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcCLR_BIT(ucData, 2);   // gpio disable
    DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);*/
if(psDemodCtx->psTunerCtx->fgRFTuner)
{    // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcSET_BIT(ucData, 2);   // gpio enable
    mcSET_BIT(ucData, 0);   // gpio as output mode
    mcSET_BIT(ucData, 1);   // gpio as output high
    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
}   
else
{    // set IFAGC pin to be normal function for legacy
    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
    mcCLR_BIT(ucData, 3);   // gpio control by demod
    mcCLR_BIT(ucData, 2);   // gpio disable
    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);

	//lei_code_db110713
	ucData=psDemodCtx->psTunerCtx->uSLD_DefaultPGAIndex;
	DVBC_SetPGAGainIndex(psDemodCtx,ucData);
	mcSHOW_DBG_MSG(("psDemodCtx->sTunerCtx->uSLD_DefaultPGAIndex = 0x%X\n",psDemodCtx->psTunerCtx->uSLD_DefaultPGAIndex));
}
    return fgRet;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_Connect
 *  Start demod module to acquire to a channel.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucBW  Channel bandwidth.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 Mode, UINT16 Rate)
{
  UINT8 fgRet = 0;  
    UINT16  u2symbolRate = 0;   
    UINT8   u1mode = 0; 

	mcSHOW_DBG_MSG(("[MS debug]DVBC_Connect\n"));
    // Chip init
    DVBC_ChipInit(psDemodCtx);
	//EC_code_dB330: For MT5396DVBTC demod
    #if defined (CC_MT5136) || defined (CC_MT5135) || defined(CC_MT5135DVBC) ||defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
    DVBC_FwSwitch(psDemodCtx);/* For Remap Use */
    #endif
    // stop driver
    DVBC_DriverStop(psDemodCtx);
    // set Parameter
    DVBC_SetNormalMode(psDemodCtx);

	//Lei130304 for Si2158 normal/ideal test.
	DVBC_SetIfAgcCurve(psDemodCtx);
	DVBC_SetIF(psDemodCtx);
	
    //Convert symbol rate
    u2symbolRate = Rate;
    //Convert QAM mode
    switch (Mode)
    {   
        case MOD_QAM_16:
            u1mode = 0x00;
            break;
        case MOD_QAM_32:
            u1mode = 0x01;
            break;
        case MOD_QAM_64:
            u1mode = 0x02;
            break;
        case MOD_QAM_128:
            u1mode = 0x03;
            break;
        case MOD_QAM_256:
            u1mode = 0x04;
            break;
        case MOD_UNKNOWN:
            u1mode = 0x05;
            break;
        default:
            u1mode = 0x02;
            mcSHOW_DBG_MSG(("Invalid input modulation(%d) to DVBC_TunerAcq!\n", u1mode));
            break;
    }    
    mcSHOW_DBG_MSG(("symbol rate = %d\n", u2symbolRate));
    mcSHOW_DBG_MSG(("Mode = %d\n", u1mode));    
    //Do Tuner Connect and Cal Cost Time        
    //ask pi to work
    // set QAM
    DVBC_SetQamMode(psDemodCtx, u1mode);
    //Set Symbol Rate
    DVBC_SetSymbolRate(psDemodCtx, u2symbolRate);
    // driver start
    DVBC_DriverStart(psDemodCtx);
    //Start Acq Start
    DVBC_AcqStart(psDemodCtx);
    // Change TS Path from Parallel to Serial
    mcDELAY_MS(10);
    DVBC_TSParallelSerialSwitch(psDemodCtx,TRUE);
    fgRet = 0;
    return fgRet;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_WO_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------

VOID DVBC_WO_DisConnect(DEMOD_CTX_T *psDemodCtx)
{   
	#if defined(CC_MT5136)
         UCHAR ucData = 0x00;
	    	    //SLD IF/RF AGC setting
		//RF_AGC_PAD_EN: 0xF62[3] = 0 (hiZ)
		//IF_AGC_PAD_EN: 0xF62[2] = 1 (Enable)
		DVBC_GetReg(psDemodCtx, 0xF62, &ucData, 1);
		mcCLR_BIT(ucData, 3);
		mcCLR_BIT(ucData, 2);
		DVBC_SetReg(psDemodCtx, 0xF62, &ucData, 1);	
	    mcSHOW_DBG_MSG(("RF AGC setting in DVBC disconnect!!\n"));
		
	    //UCHAR    ucData = 0;
		DVBC_GetReg(psDemodCtx, 0x8FB,  &ucData, 1);
	    ucData = (ucData & 0x30) >> 4;
		if(ucData == 0x01) //48pin
		{ 
			DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
		    ucData = ucData | 0xF0;
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = 0xff;
			DVBC_SetReg(psDemodCtx, 0x8E2, &ucData, 1);
			   
			mcSHOW_DBG_MSG(("48pin ts output on \n"));
		}
	    else if(ucData == 0x02)	//128 pin
		{
			DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = ucData | 0xF0;
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			mcSHOW_DBG_MSG(("128pin ts output on \n"));
	    }
		else
		{
		    mcSHOW_DBG_MSG(("Error, not 48pin and 128pin\n"));
        
		}
	#endif
	return;              //direct return for driver develpment 
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DisConnect
 *  power down demod module.  
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 */
//-----------------------------------------------------------------------------
VOID DVBC_DisConnect(DEMOD_CTX_T *psDemodCtx)
{
#if defined(CC_MT5136)
	#if 1
		UCHAR ucData = 0x00;
	    //SLD IF/RF AGC setting
		//RF_AGC_PAD_EN: 0xF62[3] = 0 (hiZ)
		//IF_AGC_PAD_EN: 0xF62[2] = 1 (Enable)
		DVBC_GetReg(psDemodCtx, 0xF62, &ucData, 1);
		mcCLR_BIT(ucData, 3);
		mcCLR_BIT(ucData, 2);
		DVBC_SetReg(psDemodCtx, 0xF62, &ucData, 1);	
	    mcSHOW_DBG_MSG(("RF AGC setting in DVBC disconnect!!\n"));
		
	    // stop driver
	    mcSHOW_DBG_MSG4(("DVBC_DriverStop(psDemodCtx);\n"));    
	    DVBC_DriverStop(psDemodCtx);

	    mcSHOW_DBG_MSG4(("DVBC_SetTsOutput(psDemodCtx, FALSE);\n"));    
	    DVBC_SetTsOutput(psDemodCtx, FALSE);
	#endif

#else
	#if defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	U32   u4Temp;
	#endif
	UCHAR ucData = 0x00;
	//Set RFAGC &IFAGC to Hiz when disconnect
	DVBC_GetReg(psDemodCtx, 0xF62, &ucData, 1);
	mcCLR_BIT(ucData, 3);
	mcCLR_BIT(ucData, 2);
	DVBC_SetReg(psDemodCtx, 0xF62, &ucData, 1); 
	mcSHOW_DBG_MSG(("Set RFGAC and IFAGC to Hiz in DVBC disconnect!!\n"));

    mcSHOW_DBG_MSG(("PI DVBC Disconnect!!\n"));

    // stop driver
    DVBC_DriverStop(psDemodCtx);

    DVBC_SetTsOutput(psDemodCtx, FALSE);

#if defined(CC_MT5396)||defined(CC_MT5368)
	//U32   u4Temp;
	//ADC and PGA power down,Lei110816;
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x500);
	u4Temp |= 0x00000006;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b11,PGA power down

	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b1,ADC power down

#elif defined(CC_MT5398)||defined(CC_MT5399)||defined(CC_MT5882)
	//ADC and PGA power down,Lei110816;
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x500);
	u4Temp |= 0x00000006;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[2:1]==2'b11,PGA power down

	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
    u4Temp |= 0x00000003;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//bit[0]=1'b1,ADC power down
    

//140417_XJ_Add New ADC power down setting
#elif defined(CC_MT5890)
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x400);
	u4Temp &= 0xBFFFFFFF;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x400, u4Temp);//Set 0xf0061400[30]=1'b0;//set RG_DMRX_EN=0
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x418);
	u4Temp &= 0xFFDFFFFF;
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//Set 0xf0061418[21]=1'b0;//set RG_CIC_FLT_EN=0


    /*
    //weile add here for ADC power down 2012.7.4
    u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x504);
    u4Temp |= 0x00080000;                         //bit[19]==1'b1,ADC power down
    mcSET_SHOW_REG(cRISC_ACD_BASE, 0x504, u4Temp);
    */
    
#if 0    
    //weile add here for set IFAGC high-z,2012.7.4
    u4Temp = mcRISC_Reg(cRISC_AGC_BASE, 0x604);
    u4Temp &= 0xFFFFF77F; //0xf000d604, bit[11:7] = 2'b00, set IFAGC high-z
    mcSET_SHOW_REG(cRISC_AGC_BASE, 0x604, u4Temp);

    //weile add here for set RFAGC high-z,2012.7.4
    u4Temp = mcRISC_Reg(cRISC_AGC_BASE, 0x72c);
    u4Temp &= 0xFFFFF3FF; //0xf000d72c, bit[11:10] = 2'b00, set RFAGC high-z
    mcSET_SHOW_REG(cRISC_AGC_BASE, 0x72c, u4Temp);
#endif

#elif defined(CC_MT5880)||defined(CC_MT5881)
	#if 1
		//ADC and PGA power down,Lei110816;
		u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x600);
		u4Temp |= 0x00000006;
		mcSET_SHOW_REG(cRISC_ACD_BASE, 0x600, u4Temp);//bit[2:1]==2'b11,PGA power down
	
		//weile modify here for ADC power down 2012.7.4
        u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x500);
        u4Temp |= 0x00000003;
        mcSET_SHOW_REG(cRISC_ACD_BASE, 0x500, u4Temp);//bit[1:0]=2'b11,ADC power down
        
        /*
        //weile add here for ADC power down 2012.7.4
        u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x604);
        u4Temp |= 0x00080000;                         //bit[19]==1'b1,ADC power down
        mcSET_SHOW_REG(cRISC_ACD_BASE, 0x604, u4Temp);
        */
	#if 0
        //weile add here for set IFAGC high-z,2012.7.4
        u4Temp = mcRISC_Reg(cRISC_AGC_BASE, 0x608);
        u4Temp &= 0xF3FFFFFF;    //0xf000d608, bit[27:26] = 2'b00, set IFAGC high-z
        mcSET_SHOW_REG(cRISC_AGC_BASE, 0x608, u4Temp);

        //weile add here for set RFAGC high-z,2012.7.4
        u4Temp = mcRISC_Reg(cRISC_AGC_BASE, 0x728);
        u4Temp &= 0xFFFFFF9F;    //0xf000d728, bit[6:5] = 2'b00, set RFAGC high-z
        mcSET_SHOW_REG(cRISC_AGC_BASE, 0x728, u4Temp);
	#endif

	#endif
#else
    // set RFAGC pin to be HiZ
    /*DVBC_GetReg(psDemodCtx, 0xf75, &ucData, 1);
       mcCLR_BIT(ucData, 3);   // gpio control by demod
       mcSET_BIT(ucData, 2);  // gpio enable
       mcCLR_BIT(ucData, 0);  // gpio as input mode
       DVBC_SetReg(psDemodCtx, 0xf75, &ucData, 1);*/
	if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
	 {   // Set IFAGC pad for GPIO control RFAGC switch due to 5365/66 HiZ not perfect
	    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
	    mcCLR_BIT(ucData, 3);   // gpio control by demod
	    mcSET_BIT(ucData, 2);  // gpio enable
	    mcSET_BIT(ucData, 0);  // gpio as output mode
	    mcCLR_BIT(ucData, 1);  // gpio as output low
	    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
	 }  
	else
	{    // set IFAGC pin to be HiZ for legacy
	    DVBC_GetReg(psDemodCtx, 0xf74, &ucData, 1);
	    mcCLR_BIT(ucData, 3);   // gpio control by demod
	    mcSET_BIT(ucData, 2);  // gpio enable
	    mcCLR_BIT(ucData, 0);  // gpio as input mode
	    DVBC_SetReg(psDemodCtx, 0xf74, &ucData, 1);
	}
	#if 1

#if fg5135_GPIOOVERWRITE_DIS
    // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD, b[1:0]: 54M clk select (XTAL/2)
    ucData = 0xC2;   // b[7]: DVBC clk PD, b[6]: DVBT clk PD, b[5]: DVBTC clk PD
    DVBC_SetReg(psDemodCtx, 0xE51, &ucData, 1);
    // apply 54M clk selection change
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE52, &ucData, 1);
    // b[7]: ci_sys_clk power down, b[6:5]: ci_sys test clock selection, b[4:0]: ci_sys clock selection
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE59, &ucData, 1);
    // apply ci_sys clock select
    ucData = 0x01;
    DVBC_SetReg(psDemodCtx, 0xE5A, &ucData, 1);
    // demod216m_ck power down
    ucData = 0x40;
    DVBC_SetReg(psDemodCtx, 0xE53, &ucData, 1);
    // demod86p4m_ck power down
    DVBC_SetReg(psDemodCtx, 0xE55, &ucData, 1);
    // demod36m_ck power down
    DVBC_SetReg(psDemodCtx, 0xE57, &ucData, 1);
    // ?? test_ck0 ??
    DVBC_SetReg(psDemodCtx, 0xE5B, &ucData, 1);

    // Power down of selected pll clock
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x01;
    //DVBC_SetReg(psDemodCtx, 0xE50, &ucData, 1);
    // Power down of selected dmsspll clock
    DVBC_SetReg(psDemodCtx, 0xE5D, &ucData, 1);
    // Power down of selected dddspll clock
    DVBC_SetReg(psDemodCtx, 0xE5E, &ucData, 1);
    // b[7]: RSSI input buffer power down
    ucData = 0xA0;
    DVBC_SetReg(psDemodCtx, 0xE90, &ucData, 1);
    // b[0]: RSSI ADC power down
    ucData = 0x00;
    DVBC_SetReg(psDemodCtx, 0xE91, &ucData, 1);
    // b[3]: IFTESTBUF power down, b[2]: IFBG power down
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0x0D;
    //DVBC_SetReg(psDemodCtx, 0xE9B, &ucData, 1);
    // b[7]: IF VCO power down, b[6]: IF PLL power down
    // not power down for CI in DVBS case, Ken, 20100609
    //ucData = 0xE7;
    //DVBC_SetReg(psDemodCtx, 0xE9E, &ucData, 1);
#endif

    // AD power down
    // b[0]: ADC PWD DEMOD
    ucData = 0x83;
    DVBC_SetReg(psDemodCtx, 0xE93, &ucData, 1);
    // b[1]: Disable ADC ckgen
    ucData = 0x02;
    DVBC_SetReg(psDemodCtx, 0xE96, &ucData, 1);
#if fg5135_GPIOOVERWRITE_DIS
    ucData = 0x57;
    DVBC_SetReg(psDemodCtx, 0xEAA, &ucData, 1);
    DVBC_SetReg(psDemodCtx, 0xEAC, &ucData, 1);
	#endif
#endif
#endif
#endif //defined(CC_MT5136)
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_DemodCtxCreate
 *  This function creates demod context and returns the context pointer.
 *  @param  void.
 *  @retval   DEMOD_CTX_T * : pointer of demod context.   
 */
//-----------------------------------------------------------------------------
DEMOD_CTX_T *DVBC_DemodCtxCreate(void)
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
 *  DVBC_DemodCtxDestroy
 *  This function destroys context you have created and releases the memory.
 *  @param  p  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DemodCtxDestroy(DEMOD_CTX_T *p)
{
    DESTROY_OBJECT(p);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDemodReg
 *  This function will read demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_GetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
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
 *  DVBC_SetDemodReg
 *  This function will write demod registers through host command.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  ucLen  Total bytes to be written. (<=4)
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetDemodReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucBuf, UCHAR ucLen)
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

/***********************************************************************/
/*                  Private (static) Function Prototypes               */
/***********************************************************************/

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetReg
 *  This function set demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemWrite(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))  
    if(ICtrlBus_DemodWrite(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG(("Demod Write Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetReg
 *  This function get demod register.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2RegAddr  Demod register address (16-bit).
 *  @param  pu1Buffer  Pointer of data buffer.
 *  @param  u2ByteCount  Total bytes to be written.
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount)
{
    //if (u1I2cDemRead(psDemodCtx, psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    if(ICtrlBus_DemodRead(psDemodCtx->I2cAddress, u2RegAddr, pu1Buffer, u2ByteCount))
    {
        mcSHOW_DBG_MSG((" Demod Read Error: %02X!\n", u2RegAddr));
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------- 
/** download image

 *  @param  
 *  @param  
 *  @retval   others : Error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_Loadcode(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen)
{
    if(ICtrlBus_DemodLoadCode(psDemodCtx->I2cAddress, DVBC_REG_uP_IRData, pu1Buffer, u2CurRomLen))
    {
        /*fail case*/
        mcSHOW_POP_MSG(("Download Code fail!\n"));
        return 1;
    }

    return 0;
}

/************************************************************************/
#define cMAX_TRY_CNT            2
//lei121119
#define cMAX_WAIT_LOOP          50
#define cWAIT_TIME_INTERVAL     2         // ms

//----------------------------------------------------------------------------- 
/*
 *  fgHostCmd
 *  Host command interface API between host and demod.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  aucCmdData  Data buffer.
 *  @param  ucReadParacnt  Read parameter byte counts.
 *  @param  ucWriteCmdcnt  Write command byte counts.
 *  @retval   TRUE : Host command success.
 *  @retval   FALSE : Host command fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgHostCmdDVBC(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt)
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
#if fcADD_HOSTCMD_CHKSUM
UCHAR   ucChkSum;
#endif
UCHAR   jj;

#if fcMEAS_HOST_CMD_TIME
    u4TickStart = mcGET_SYS_TICK();
#endif

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucValue, 1))
    {
    //    psDemodCtx->u1I2cNakCnt ++;
        mcSHOW_HW_MSG((" _I2_ "));
        mcSHOW_HW_MSG((" Cmd%02x ", mccSET_CMD2(aucCmdData)));
        return FALSE;                       // I2C fail
    }
    if (!mcTEST_BIT(ucValue, ccpUP_ENABLE))
    {
        psDemodCtx->u1UpChkCnt ++;
    //    mcSHOW_HW_MSG(("uP not enabled!\n"));
        mcSHOW_HW_MSG((" _UE_ "));
        mcSHOW_HW_MSG(("  Cmd%02x ", mccSET_CMD2(aucCmdData)));
        return FALSE;
    }
    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1))
    {
        return FALSE;                       // I2C fail
    }
    if (ucValue != ccFW_RUN_TAG)
    {
        psDemodCtx->u1UpChkCnt ++;
        if (ucValue == ccFW_WDT_TAG)
        {
        //    mcSHOW_HW_MSG(("Watchdog reset!\n"));
            mcSHOW_HW_MSG((" _WR_ "));
        }
        else
        {
        //    mcSHOW_HW_MSG(("Fw not running!\n"));
            mcSHOW_HW_MSG((" _FR_ "));
        }

        return FALSE;
    }   
    if (ucWriteCmdcnt)
    {
        ucWriteParacnt = ucWriteCmdcnt - 1;
        for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
        {
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
            // This while loop will cause dead-lock for Combo-Tuner (when I2C_Block = TRUE)
        #if 1
            jj = 0;
            while (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_RSICIF_RSP7, &ucValue, 1);
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
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

        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_RSICIF_CMD0 + ucParaStart),
                                      aucCmdData + ucParaStart + ucRegSetOfst,
                                                          ucWriteParacnt + 1);
        #if fcMEAS_HOST_CMD_TIME
        u4TickCnt = mcGET_SYS_TICK() - u4TickStart;
        mcSHOW_TIME_MSG((" Para %d (%3u)", ucWriteParacnt, u4TickCnt * mcGET_TICK_PERIOD()));
        #endif
        #if fcSHOW_HOST_CMD
        if (fgShowHostCmd)
        {
            //    mcSHOW_USER_MSG(("HostCmd: (%d) -->", ucParaStart));
            mcSHOW_USER_MSG(("HostCmd -->"));
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
            {
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            }
            mcSHOW_USER_MSG(("\n           "));
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR00 + ucParaStart,
                                                         aucCmdData + ucParaStart + ucRegSetOfst,
                                                         ucWriteParacnt + 1);
            for (ii = 0; ii < ucWriteParacnt + 1; ii ++)
            {
                mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ucRegSetOfst + ii]));
            }
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
        {
            for (ii = 0; ii < cMAX_WAIT_LOOP; ii ++)
            {
                DVBC_GetReg(psDemodCtx, DVBC_REG_uP_CMDR_CFG, &ucValue, 1);
                if (mcTEST_BIT(ucValue, ccpUP_RSP_TX))
                    break;
                mcDELAY_MS(cSHORT_SLEEP_TIME);
            }

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

                if (DVBC_GetReg(psDemodCtx,
                                            (UINT16) (DVBC_REG_RSICIF_RSP0 + u2AddrOfst + ucParaStart),
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
                    #if fcSHOW_HOST_CMD
                    if (fgShowHostCmd)
                    {
                        mcSHOW_USER_MSG(("HostRsp -->"));
                        for (ii = 0; ii < ucReadChkCnt + 1; ii ++)
                        {
                            mcSHOW_USER_MSG((" %02X", aucCmdData[ucParaStart + ii]));
                        }
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
                            mcSHOW_HW_MSG((" _@CS_ "));
                            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_RSPR00 + ucParaStart,
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
 *  fgDVBCGetFwVars
 *  Get firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if(ucCnt>(ccCMD_REG_NUM-2-ccOFST_REG1_DATA+1))/*Prevent aucData Buffer Overflow  */ 
    {
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
 *  DVBC_GetDvbcNotExist
 *  Check if really no DVB-C signal exists.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE  : It's confirmed that no DVB-C signal exists.   
 *  @retval   FALSE : DVB-C signal exists or it's not confirmed yet.
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetDvbcNotExist(DEMOD_CTX_T *psDemodCtx)
{
    if (!psDemodCtx)
        return FALSE;

    mcSHOW_DBG_MSG((".DVBC ReAcq Cnt %d, QCN Fail Cnt %d, ASR Fail Cnt %d, CurState %d!\n", 
        psDemodCtx->ucReAcqCnt, psDemodCtx->ucQCNFailState, psDemodCtx->ucASRFailState,psDemodCtx->ucCurrentAcqState));
	mcSHOW_DBG_MSG(("DVBC esimate QAM: %d , RS: %d!\n", psDemodCtx->ucQamMode, psDemodCtx->u2SymbolRate));

//    if ( (psDemodCtx->ucASRFailState>0) || (psDemodCtx->ucQCNFailState>0) )
    if ( (psDemodCtx->ucASRFailState>0) )  // removed  the QCN fail for early break
    {
        return TRUE;
    }
	else if ((psDemodCtx->ucReAcqCnt > 1 ))
	{    
        DVBC_UpdateSymbol(psDemodCtx);

        mcSHOW_DBG_MSG(("DVBC ASR Ready : %d\n", psDemodCtx->fgAsrReady));

        if (psDemodCtx->fgAsrReady)
        {
		
            mcSHOW_DBG_MSG(("DVBC esimate QAM: %d , RS: %d!\n", psDemodCtx->ucQamMode, psDemodCtx->u2SymbolRate));
            if (psDemodCtx->u2SymbolRate == 0)
            {
                mcSHOW_DBG_MSG(("Esimate symbol rate is 0, not yet\n"));
                return FALSE;
            }
            else if ((psDemodCtx->u2SymbolRate<(psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET))||
                       (psDemodCtx->u2SymbolRate>(psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)))
            {   
                mcSHOW_DBG_MSG(("Esimate symbol rate is  %dkbaud/s, out of range [%dkbaud/s, %dKbaud/s], need early break\n",
                                                 psDemodCtx->u2SymbolRate, (psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET), (psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)));
				//lei130308 for symbol rate unnormal;
				#if defined(CC_MT5136)
				if((psDemodCtx->u2SymbolRate >0x8000)&&(psDemodCtx->u2SymbolRate <0xC000)) 
				{
					mcSHOW_DBG_MSG(("[Lei]@@@@@@@@@@@@@@@@@@@@@@\n"));
					return FALSE;
				}
				else
				{
                    return TRUE;
                }
				#else
				return TRUE;
				#endif
            }
            else
            {
                mcSHOW_DBG_MSG(("Esimate symbol rate is  %dkbaud/s, in range [%dkbaud/s, %dKbaud/s], ok!\n",
                                                 psDemodCtx->u2SymbolRate, (psDemodCtx->u2SymbolrateMin-ESTIMATE_SYMBOL_OFFSET), (psDemodCtx->u2SymbolrateMax+ESTIMATE_SYMBOL_OFFSET)));
                return FALSE;
            }
        }
        else
        {
            mcSHOW_DBG_MSG(("DVBC ASR Not Ready\n"));
            return FALSE;
        }
    }
    else
        return FALSE;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetDvbcTsNotExist
 *  Check if RSH lock but PER>80% or TotalPkt=0.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Lock   : DVB-C TS really ok
 *  @retval   Unknown: DVB-C TS not confirmed yet.
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetDvbcTsNotExist(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1TsState = DVBC_DEMOD_STATE_UNKNOWN;

    if (!psDemodCtx)
        return FALSE;

    if (psDemodCtx->ucRSHState)
    {
        UCHAR   aucData[cMAX_READ_NUM + 1];
        UCHAR   uc_data[2];
        UINT16  u2ErrPkt=0;
        UINT16  u2TotPkt=0;

        mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO0;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO0_SIZE, 1))
        {
            //total error packets
            uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT0);
            uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT1);
            u2ErrPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8); 
            //total packets
            uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT0);
            uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT1);
            u2TotPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        }
		mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO2;
		if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO2_SIZE, 1))
		{
			uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR0);
			uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR1);
			psDemodCtx->s2Snr = mcSIGN_2BYTE(uc_data[1], uc_data[0], 7, 8);   // SNR * 256
			psDemodCtx->u1Bw = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_QCR_DD_BW);
			mcSHOW_USER_MSG(("HC_snr = %d/10 dB\n", psDemodCtx->s2Snr));
			mcSHOW_USER_MSG(("HC_SNR = %d/10 dB\n", (psDemodCtx->s2Snr) / 25));
		}
        // If TS is locked, but error rate is high; not report locked, it may be QAM error (eg: 16QAM signal locked with 256QAM)
        if ((u2TotPkt != 0) && ((u2TotPkt * 9) > (u2ErrPkt * 10)))
        {
            u1TsState = DVBC_DEMOD_STATE_LOCK;
        }
        else
        {
            u1TsState = DVBC_DEMOD_STATE_UNSTABLE;
            mcSHOW_DBG_MSG(("..DVBC TS lock, but TotalPkt %d, ErrorPkt %d. Continue try..!\n", u2TotPkt, u2ErrPkt));
        }
    }

    return u1TsState;
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSync
 *  Get demod lock status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   DVBC_DEMOD_STATE_UNLOCK : Demod unlock.
 *  @retval   DVBC_DEMOD_STATE_LOCK   : Demod locked.   
 *  @retval   DVBC_DEMOD_STATE_UNKNON : Demod status unknown.
 */
//-----------------------------------------------------------------------------

UINT8 DVBC_GetSync(DEMOD_CTX_T *psDemodCtx)
{
    UINT8 u1DvbcDemodState = DVBC_DEMOD_STATE_UNKNOWN;
#if 0
    // for debug
    UCHAR   aucData[cMAX_READ_NUM + 1];
#endif

    DVBC_UpdateStatus(psDemodCtx); // update SC state, FFT & TS lock
   
	//Mingsheng 0802. Remove to tuner acq to decrease the scan time ;
    //DVBC_UpdateInfo(psDemodCtx); // Iven, for Symbol rate update 20110317
    if (DVBC_GetDvbcNotExist(psDemodCtx))
    {
        u1DvbcDemodState = DVBC_DEMOD_STATE_UNLOCK;
    }
    else
    {
        u1DvbcDemodState = DVBC_GetDvbcTsNotExist(psDemodCtx);
    }

#if 0
    // for debug
    if (u1DbgLevel > 1)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO6;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO6_SIZE, 1))
        {
            psDemodCtx->ucAQM_Seq[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_00);
            psDemodCtx->ucAQM_Seq[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_01);
            psDemodCtx->ucAQM_Seq[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_02);
            psDemodCtx->ucAQM_Seq[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_03);
            psDemodCtx->ucAQM_Seq[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_04);
            psDemodCtx->ucAQM_ID = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_Index);
        }
        mcSHOW_DBG_MSG(("AQM Sequence: %d-%d-%d-%d-%d, %d\n", psDemodCtx->ucAQM_Seq[0], psDemodCtx->ucAQM_Seq[1], psDemodCtx->ucAQM_Seq[2], psDemodCtx->ucAQM_Seq[3], psDemodCtx->ucAQM_Seq[4], psDemodCtx->ucAQM_ID));
    }
#endif
    
    return u1DvbcDemodState;
}

//----------------------------------------------------------------------------- 
/*
 *  ucGetFwVar
 *  Get firmware one-byte variable.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @retval   1-byte data.   
 */
//-----------------------------------------------------------------------------
UINT8 ucDVBCGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr)
{
    UCHAR   ucValue = 0;

    fgDVBCGetFwVars(psDemodCtx, u2Addr, &ucValue, 1);
    return ucValue;
}
//----------------------------------------------------------------------------- 
/*
 *  fgDVBCSetFwVars
 *  Set firmware variables.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u2Addr  firmware variable address.
 *  @param  pucData  Pointer of data buffer.
 *  @param  ucCnt  Length of data (ucCnt<=4).
 *  @retval   TRUE : Success.
 *  @retval   FALSE : Fail.   
 */
//-----------------------------------------------------------------------------
BOOL fgDVBCSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt)
{
UCHAR   ii;
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd

    if(ucCnt>(ccCMD_REG_NUM-2-ccOFST_REG1_DATA+2))/*Prevent aucData Buffer Overflow  */ 
    {
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
 *  s4LoadCode_patch
 *  Download firmware code for MT5396 up sram share sen.
 *  @param  psDemodCtx  Pointer of context created by DVBT_DemodCtxCreate.
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

#if defined(CC_MT5396)|| defined(CC_MT5368)
#if (ATD_NOT_USE==FALSE)
static INT32 s4LoadCode_patch(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
    UCHAR   ucValue, ucUpCtrl, ii;
	UINT16 u2SramSize;
#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
    UCHAR   ucI2cAuto;
#endif
    UINT16  /*u2CurI2cLen,*/ u2CurRamAdr;
    /*UCHAR   ucI2cAddr;*/
#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif

#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    /* Confirm Load FW Source Code */

	// for 5396 ES SRAM sharing patch
    #if defined(CC_MT5135) || defined (CC_MT5135DVBC)
	    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
	    if(ucUpCtrl == 0x66)
	    {
			mcSHOW_POP_MSG(("Have not download code,direct return!\n"));
			return cUP_LOAD_OK;
	    }
	#elif defined(CC_MT5396)|| defined(CC_MT5368)
		ucUpCtrl = 0;	
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		
		ucUpCtrl = 0x00;
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
	#endif
	
	ucUpCtrl = 0;
	DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
	ucUpCtrl = mcBIT(ccpUP_RESTART_CHKSUM) | mcBIT(ccpUP_ROMCODE_CHKSUM);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not ready\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not OK\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_OK;
    }
	
	
#endif

//#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368)
    // SRAM clear
    ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368) 
    // set ad clock phase for more margin case
    DVBC_GetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    mcCLR_BIT(ucUpCtrl, 5);
    DVBC_SetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }
#endif

    ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);

    for (ii = 0; ii < 4; ii ++)
    {
        DVBC_GetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
    }

// set max code size.
#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    ucValue = 0x9F;
#else
    ucValue = 0x6F;
#endif
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_H, &ucValue, 1);
    ucValue = 0xFF;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_L, &ucValue, 1);

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn off I2C auto-increment
    DVBC_GetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    //UINT16 u2SramSize;
#if defined(CC_MT5135) || defined(CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)
    u2SramSize = 110*1024/10;
#else
    u2SramSize = 0;
#endif
    u2CurRamAdr = u2SramSize;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    // for 40k SRAM, IR_RD is 0x00[7]
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_L, &ucValue, 1);

    /*ucI2cAddr = psDemodCtx->I2cAddress;*/
    if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("Download FW fail! (%d)\n"));
        return 1;
    }
	mcSHOW_POP_MSG(("Download FW code finished!\n"));

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        return cUP_LOAD_ERR_CHKSUM_OK;
    }

    ucUpCtrl = mcBIT(ccpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);    // 25 MHz Clock
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);

    // delay for waiting FW initialization done, may have I2C timeout at first
    for (ii = 0; ii < 5; ii++)
    {
        mcDELAY_MS(3);
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == ccFW_RUN_TAG)
            break;
    }

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) //|| defined(CC_MT5396)
    // enable HOST_EN for host to be able to access cmd/rsp register
    ucValue = 0x01;
    DVBC_SetReg(psDemodCtx, DVBC_REG_RFA4A0, &ucValue, 1);
#endif

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
#endif
#endif

//----------------------------------------------------------------------------- 
BOOL DVBC_FwSwitch(DEMOD_CTX_T *psDemodCtx)
{
#if defined(CC_MT5136)
	UINT8 *pCodeImage=NULL;
    UINT16 u2CodeImageLen;
    UCHAR ucValue, ucData;
    UCHAR aucData[cMAX_READ_NUM + 1], ii;
	DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG07, &ucValue, 1);
	mcSHOW_USER_MSG(("[MS DEBUG]0x00f is %d!\n", ucValue));
    if(0x01 != ucValue)
    //if(1)
    {   
		DVBC_SetTCSwitch(psDemodCtx, TRUE);
		mcSHOW_DBG_MSG(("----[XJ DEBUG]-----TC switch flag = TRUE!\n"))
        DVBC_SetEEPromBrdcstType(psDemodCtx);
    	//rest SLD before load FW.lei11080
    	//UCHAR ucData = 0x00;
    	//Reset SLD REG 
		ucData = 0x00;
		DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);
		ucData = 0xff;
		DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);

		ucData = 0x00;
		DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
		ucData = 0xff;
		DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);

        DVBC_ChipInit(psDemodCtx);
        
		if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
		{     
		    mcSHOW_USER_MSG(("download 5136 RF tuner FW !!\n"));
		    pCodeImage=ucDvbcCodeImage;
		    u2CodeImageLen=sizeof(ucDvbcCodeImage);
		 }
		 else//legacy tuner
		 {     
		    mcSHOW_USER_MSG(("download 5136 legacy tuner FW !!\n"));
		    pCodeImage=ucDvbcCodeImageLeg;
		    u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
		 }				
				
		 if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
		 {
		    mcSHOW_USER_MSG(("download 5136 FW fail!!\n"));
            return FALSE;
		 }
		 #if 0 // LL test for db10-2
         	#if  (defined(CC_MT5136) && defined (CC_MT5881)) // LL for 5136+5881
   DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);
			mcSHOW_USER_CRI_MSG(("[DVBC_FwSwitch]DVBC_SetTsIf\n"));
	#endif
	#endif
         mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_SWITCH;
         mccSET_QUEUE2(aucData, ccOFST_DVBTC_SWITCH) = 0x01;//0x01:DVBC, 0x02: DVBT
         mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBTC_SWITCH_RSIZE, ccOFST_DVBTC_SWITCH_SIZE+1);
   
         // delay for waiting FW initialization done, may have I2C timeout at first
         for (ii = 0; ii < 5; ii++)
         {
            mcDELAY_MS(3);
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
         }
         mcSHOW_DBG_MSG2(("FW switch to DVBC!"));
		 
    }
	else
	{
		DVBC_SetTCSwitch(psDemodCtx, FALSE);
		mcSHOW_DBG_MSG(("----[XJ DEBUG]-----TC switch flag = FALSE!\n"));
	}
#if 0	
	else
	{
		mcSHOW_DBG_MSG2(("ucValue not 1!"));
	}
    mcSHOW_DBG_MSG2(("FW switch to 5136 DVBC!"));
#endif	
	//SLD IF/RF AGC setting
	//RF_AGC_PAD_EN: 0xF62[3] = 0 (hiZ)
	//IF_AGC_PAD_EN: 0xF62[2] = 1 (Enable)
	DVBC_GetReg(psDemodCtx, 0xF62, &ucData, 1);
	mcCLR_BIT(ucData, 3);
	mcSET_BIT(ucData, 2);
	DVBC_SetReg(psDemodCtx, 0xF62, &ucData, 1);	

	return TRUE;              //direct return for driver develpment 
#else

//EC_code_dB330: For MT5396DVBTC demod
#if defined (CC_MT5135) || defined(CC_MT5135DVBC) ||defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	UCHAR aucsldData[cMAX_READ_NUM + 1];
	UCHAR aucData[cMAX_READ_NUM + 1], ii;
    UCHAR ucValue;
    UCHAR ucData = 0x00;
    /*
    UCHAR ucUpCtrl = 1;// Enable uP //EC_debug_dB506
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcSHOW_DBG_MSG(("enable DVBC UP!\n"));    
    mcDELAY_MS(10);
	*/

	#if defined(CC_MT5396)|| defined(CC_MT5368)// for 5396 ES SRAM sharing patch 

    #if (ATD_NOT_USE==FALSE)	
	UINT8 * pCodeImage=NULL;
	UINT16 u2CodeImageLen;
    //UCHAR ucData = 0x00;
	
	//Reset SLD REG //Mingsheng110616
	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);

	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	
    if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
    {
          pCodeImage=ucDvbcCodeImage;
          u2CodeImageLen=sizeof(ucDvbcCodeImage);
    }
    else//legacy tuner
    {
          pCodeImage=ucDvbcCodeImageLeg;
          u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
    }

		mcSHOW_USER_MSG(("Re-download FW start!!\n"));
		
    if (s4LoadCode_patch(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
    {
            mcSHOW_USER_MSG(("Re-download FW fail!!\n"));
    }
	#endif
	
	#elif defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
			mccSET_CMD2(aucsldData) = ccCMD_ID_DVBTC_SLD_RESET;
			mccSET_QUEUE2(aucsldData, ccOFST_DVBTC_SLD_RESET_MODE) = 0x01;//0x01:DVBC, 0x02: DVBT
			mcHOST_CMD(psDemodCtx, aucsldData, 0, ccOFST_DVBTC_SLD_RESET_TSIZE+1);//lei121119
			mcSHOW_USER_MSG(("[DVBC]SLD Reset in FwSwitch()!\n"));
    
	#endif
    
    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG07, &ucValue, 1);
    if(0x01 != ucValue)
    {
		#ifdef INTERNAL_DTMB
		UINT8 * pCodeImage=NULL;
		UINT16 u2CodeImageLen;

		mcSHOW_DBG_MSG(("[XJ]Use internal DTMB, needs load DVBC fw!\n "));

		if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
		{
			  pCodeImage=ucDvbcCodeImage;
			  u2CodeImageLen=sizeof(ucDvbcCodeImage);
		}
		else//legacy tuner
		{
			  pCodeImage=ucDvbcCodeImageLeg;
			  u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
		}

		if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
		{
			mcSHOW_USER_MSG(("[XJ]download DVBC FW fail!!\n"));
		}
		//XJ141008 for switch from DTMB sld reset hostcmd re-execute
		mccSET_CMD2(aucsldData) = ccCMD_ID_DVBTC_SLD_RESET;
		mccSET_QUEUE2(aucsldData, ccOFST_DVBTC_SLD_RESET_MODE) = 0x01;//0x01:DVBC, 0x02: DVBT
		mcHOST_CMD(psDemodCtx, aucsldData, 0, ccOFST_DVBTC_SLD_RESET_TSIZE+1);
		mcSHOW_USER_MSG(("[DTMB2DVBC]SLD Reset in FwSwitch()!\n"));
		#endif

        mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_SWITCH;
        mccSET_QUEUE2(aucData, ccOFST_DVBTC_SWITCH) = cCONN_TYPE_DVBC;//0x01:DVBC, 0x02: DVBT
        mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBTC_SWITCH_SIZE+1);//lei121119

        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }

        mcSHOW_DBG_MSG2(("FW switch to DVBC!"));
    }

	#if defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
    // RFAGC internal control by tuner, set RFAGC pin to be HiZ
    if(!psDemodCtx->psTunerCtx->fgRFTuner)//legacy tuner
    {
		DVBC_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	    mcCLR_BIT(ucData, 3);
		mcSET_BIT(ucData, 2);
	    DVBC_SetReg(psDemodCtx, 0xf62, &ucData, 1);
    }
	else
	{//Sawless RF by Tuner,IF PGA close loop
		DVBC_GetReg(psDemodCtx, 0xf62, &ucData, 1);
	    mcCLR_BIT(ucData, 3);
		mcCLR_BIT(ucData, 2);
	    DVBC_SetReg(psDemodCtx, 0xf62, &ucData, 1);
		mcSHOW_DBG_MSG(("-----[DVBC]SET RFIF AGC in FW swith!------\n"));
	}
	#endif
	
    // set agc parameters
    //DVBC_SetIF(psDemodCtx);     // set IF when FW switch
#endif

    return TRUE;
#endif    
}

//-----------------------------------------------------------------------------
VOID DVBC_TSParallelSerialSwitch(DEMOD_CTX_T *psDemodCtx, BOOL IsSerial)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_TS_MODE;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_TS_MODE) = (UCHAR)IsSerial;
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_TS_MODE_SIZE+1);//lei121119
}

/***********************************************************************/
/*                  Public Functions (not static)                      */
/***********************************************************************/
//----------------------------------------------------------------------------- 
/*
 *  s4LoadCode
 *  Download firmware code.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u4I2cClkDiv  Unused.
 *  @param  u1CodeImage  Pointer of fimware code image.
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
static INT32 s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
                 UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50)
{
    UCHAR   ucValue = 0; 
	UCHAR   ucUpCtrl = 0;
	UCHAR   ii = 0;
	UINT16 u2SramSize;
#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
    UCHAR   ucI2cAuto;
#endif
    UINT16  /*u2CurI2cLen,*/ u2CurRamAdr;
    /*UCHAR   ucI2cAddr;*/
#if fcLOAD_CODE_TIME
    TIME_TICK_T u4TickStart, u4TickCur;
    TIME_DIFF_T u4TickCnt;
#endif

#if defined(CC_MT5136)  // for 5136 development
		
		DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("[MS debug] DVBC_REG_uP_GP_REG06 is 0x%x!\n", ucUpCtrl));
		
	    //if(ucUpCtrl == 0x66)
	    //{
	    //    mcSHOW_POP_MSG(("Have not download code,direct return!\n"));
	    //    return cUP_LOAD_OK;
	    //}
		if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1))
		{
		    mcSHOW_POP_MSG(("[MS debug] cUP_LOAD_ERR_I2C!\n"));
			return cUP_LOAD_ERR_I2C;
		}
		else
		{
		    mcSHOW_POP_MSG(("[MS debug] reg 0x00 is 0x%0x!\n",ucUpCtrl));
		}



	    // SRAM clear
       ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
         DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        mcDELAY_MS(10);
         DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
		{
	        mcSHOW_POP_MSG(("[MS debug] cUP_LOAD_ERR_HW_RDY!\n"));
			return cUP_LOAD_ERR_HW_RDY;
		}
		///
	    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("[MS debug] 0 reg 0x00 is 0x%0x!\n",ucUpCtrl));
			
		ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		ucValue = 0;
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);

        ///
		DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("[MS debug] 1 reg 0x00 is 0x%0x!\n",ucUpCtrl));
		
		for (ii = 0; ii < 4; ii ++)
		{
			DVBC_GetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
			DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
		}
	
		// set max code size
		mcSHOW_POP_MSG(("[MS debug] psDemodCtx->ucSramSize is %d!\n", psDemodCtx->ucSramSize));
		u2SramSize = (UINT16)(psDemodCtx->ucSramSize) * 1024 - 1;
		ucValue = mcHIGH_BYTE(u2SramSize);
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_H, &ucValue, 1);
		ucValue = mcLOW_BYTE(u2SramSize);
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_L, &ucValue, 1);
	
       #if 1
	// Turn off I2C auto-increment
		DVBC_GetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
		mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
		DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
		#endif

	// Set Load code RAM Starting Addr
		#if fcLOAD_CODE_TIME
		mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
		mcGET_SYS_TIME(u4TickStart);
		ii = 0;
		#endif
	
		u2CurRamAdr = 0;
		//u2CurRamAdr = psDemodCtx->m_u2RomSize;
		ucValue = mcHIGH_BYTE(u2CurRamAdr);
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_H, &ucValue, 1);
		ucValue = mcLOW_BYTE(u2CurRamAdr);
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_L, &ucValue, 1);
		u2CurRamAdr = 0;
	
	
	    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("[MS debug] 2 reg 0x00 is 0x%0x!\n",ucUpCtrl));

		if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
		{
			mcSHOW_POP_MSG(("Download I2C fail! (%d)\n"));
			return 1;
		}
		else
		{
			mcSHOW_POP_MSG(("Load code ok!\n"));
		}
		#if 0
		ucI2cAddr = psDemodCtx->I2cAddress; 
		mcSHOW_POP_MSG(("ucI2cAddr is 0x%x\n", ucI2cAddr)); 
		while (u2CurRomLen)    
		{        
			u2CurI2cLen = u2CurRomLen;        
			if (u2CurI2cLen > u2MaxI2cLen)            
				u2CurI2cLen = u2MaxI2cLen;        
			if (ucI2cWriteSpeed(psDemodCtx, 33, ucI2cAddr, DVBC_REG_uP_IRData, u1CodeImage + u2CurRamAdr, u2CurI2cLen))     
			{           
				mcSHOW_POP_MSG(("Download I2C fail!\n"));            
				break;        
			}        
			u2CurRamAdr += u2CurI2cLen;       
			u2CurRomLen -= u2CurI2cLen;   
		}
		#endif
		
	    #if 1
	// Turn on I2C auto-increment
		mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
		DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
	    #endif
	    ///
	    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("[MS debug] 3 reg 0x00 is 0x%0x!\n",ucUpCtrl));
		
		ucUpCtrl = mcBIT(ccpUP_ENABLE); 		 // 50 MHz Clock
		if (!fgClk50)
			ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);	  // 25 MHz Clock
	
		DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		
	
		// delay for waiting FW initialization done, may have I2C timeout at first
		for (ii = 0; ii < 5; ii++)
		{
			mcDELAY_MS(3);
			DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
			if (ucValue == ccFW_RUN_TAG)
			{
				mcSHOW_POP_MSG(("[MS debug] DVBC_REG_uP_GP_REG06 is ccFW_RUN_TAG!, break\n"));
				break;
			}
		}
	    mcSHOW_DBG_MSG(("uP Startup!\n"));
		
		DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
		mcSHOW_POP_MSG(("\nREG 0x00 is 0x%x!\n",ucUpCtrl )); 
		
		// enable HOST_EN for host to be able to access cmd/rsp register
		ucValue = 0x01;
		DVBC_SetReg(psDemodCtx, DVBC_REG_RFA4A0, &ucValue, 1);
	   
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

#else
	
#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)|| defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
    /* Confirm Load FW Source Code */
    DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucUpCtrl, 1);
	mcSHOW_DBG_MSG(("------[xx2863]DVBC_REG_uP_GP_REG06 =0x%x-----\n",ucUpCtrl));
    //if(ucUpCtrl == 0x66)
    //{
    //    mcSHOW_POP_MSG(("Have not download code,direct return!\n"));
    //    return cUP_LOAD_OK;
    //}
	ucUpCtrl = 0;
	DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
	ucUpCtrl = mcBIT(ccpUP_RESTART_CHKSUM) | mcBIT(ccpUP_ROMCODE_CHKSUM);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not ready\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
        mcSHOW_USER_MSG(("Load FW ROM checksum not OK\n"));
        return cUP_LOAD_ERR_ROM_CHKSUM_OK;
    }
#endif

//#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368)
    // SRAM clear
    ucUpCtrl = mcBIT(ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    mcDELAY_MS(10);
#if !defined(CC_MT5135) && !defined(CC_MT5135DVBC) && !defined(CC_MT5396)&& !defined(CC_MT5368)&& !defined(CC_MT5398)&& !defined(CC_MT5880)&& !defined(CC_MT5881)&& !defined(CC_MT5399)&& !defined(CC_MT5890)&& !defined(CC_MT5882)
    // set ad clock phase for more margin case
    DVBC_GetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);
    mcCLR_BIT(ucUpCtrl, 5);
    DVBC_SetReg(psDemodCtx, DVBC_REG_PGA_CTRL_1, &ucUpCtrl, 1);

    if (DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1))
        return cUP_LOAD_ERR_I2C;
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE))
    {
        return cUP_LOAD_ERR_HW_RDY;
    }
#endif

    ucUpCtrl = mcBIT(ccpUP_DN_FREQ) | mcBIT(ccpUP_RESET_CHKSUM);  // Disable uP
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    ucValue = 0;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);

    for (ii = 0; ii < 4; ii ++)
    {
        DVBC_GetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
        DVBC_SetReg(psDemodCtx, (UINT16) (DVBC_REG_uP_PNGP_H + ii), &ucValue, 1);
    }

// set max code size.
#if defined(CC_MT5135) || defined (CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)
    ucValue = 0x9F;
#elif defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
    ucValue = 0xAF;
#else
    ucValue = 0x6F;
#endif
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_H, &ucValue, 1);
    ucValue = 0xFF;
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_LPA_L, &ucValue, 1);

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn off I2C auto-increment
    DVBC_GetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
    mcCLR_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

// Set Load code RAM Starting Addr
#if fcLOAD_CODE_TIME
    mcSHOW_DBG_MSG(("Code Size = %d ", u2CurRomLen));
    mcGET_SYS_TIME(u4TickStart);
    ii = 0;
#endif

    //UINT16 u2SramSize;
#if defined(CC_MT5135) || defined(CC_MT5135DVBC) || defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
    u2SramSize = 110*1024/10;
#else
    u2SramSize = 0;
#endif
    u2CurRamAdr = u2SramSize;
    ucValue = mcHIGH_BYTE(u2CurRamAdr);
    // for 40k SRAM, IR_RD is 0x00[7]
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_H, &ucValue, 1);
    ucValue = mcLOW_BYTE(u2CurRamAdr);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_IRA_L, &ucValue, 1);

    /*ucI2cAddr = psDemodCtx->I2cAddress;*/
    if (DVBC_Loadcode(psDemodCtx, u1CodeImage, u2CurRomLen))
    {
        mcSHOW_POP_MSG(("Download FW fail! (%d)\n"));
        return 1;
    }
	mcSHOW_POP_MSG(("Download FW code finished!\n"));

#if !defined(CC_MT5135) //&& !defined(CC_MT5396)
// Turn on I2C auto-increment
    mcSET_BIT(ucI2cAuto, ccpI2C_AUTO_INC);
    DVBC_SetReg(psDemodCtx, DVBC_REG_I2C_Cfg, &ucI2cAuto, 1);
#endif

    mcSET_BIT(ucUpCtrl, ccpUP_RESTART_CHKSUM);
    mcCLR_BIT(ucUpCtrl, ccpUP_MEM_INIT_DONE);
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
    for (ii = 0; ii < 8; ii++)
    {
        mcDELAY_MS(10);                        // Unit: ms
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);
        if (mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
            break;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_DONE))
    {
	mcSHOW_DBG_MSG(("------[xx]cUP_LOAD_ERR_CHKSUM_RDY-----\n"));
        return cUP_LOAD_ERR_CHKSUM_RDY;
    }
    if (!mcTEST_BIT(ucUpCtrl, ccpUP_CHKSUM_OK))
    {
	mcSHOW_DBG_MSG(("------[xx]cUP_LOAD_ERR_CHKSUM_OK-----\n"));
        return cUP_LOAD_ERR_CHKSUM_OK;
    }

    ucUpCtrl = mcBIT(ccpUP_ENABLE);          // 50 MHz Clock
    if (!fgClk50)
        ucUpCtrl = mcBIT(ccpUP_ENABLE) | mcBIT(ccpUP_DN_FREQ);    // 25 MHz Clock
    DVBC_SetReg(psDemodCtx, DVBC_REG_uP_Ctrl, &ucUpCtrl, 1);

    // delay for waiting FW initialization done, may have I2C timeout at first
    for (ii = 0; ii < 5; ii++)
    {
        mcDELAY_MS(3);
        DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
        if (ucValue == ccFW_RUN_TAG)
            break;
    }

#if defined(CC_MT5135) || defined(CC_MT5135DVBC) //|| defined(CC_MT5396)
    // enable HOST_EN for host to be able to access cmd/rsp register
    ucValue = 0x01;
    DVBC_SetReg(psDemodCtx, DVBC_REG_RFA4A0, &ucValue, 1);
#endif

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
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStart
 *  Start demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  u1Mon1Sel  Monitor1 driver select. (0: None, 1: Auto-Reacquisition).
 *  @param  u1Mon2Sel  Monitor2 driver select. (0: None, 1: TPS sync, 2: SPS sync auto, 3: SPS sync manual
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);//lei121119
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_DriverStop(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_REACQ_STOP;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);//lei121119
}


void DVBC_SetIF(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   ucPara[ccCMD_REG_NUM];
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    // set agc parameters
    ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA1;
    ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, reserved
    ucPara[2] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
    ucPara[3] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
    vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 ucI2cAddress, BOOL fgLoadCode)
{
    //UCHAR   ucPara[ccCMD_REG_NUM];
    //UCHAR   ii;
    //UCHAR   aucData[cMAX_READ_NUM + 1];
    //SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);
    UINT8 *pCodeImage=NULL;
    UINT16 u2CodeImageLen;
	#if defined(INTERNAL_DTMB)
	UCHAR aucData[cMAX_READ_NUM + 1], ii;
	UCHAR ucValue =0;
	#endif
	#if (ATD_NOT_USE==TRUE)
    UCHAR ucData = 0x00;
	#endif
	
	
	#if defined(CC_MT5136)
	//UCHAR ucData = 0x00;
	//UINT8 ucTmp = 0;
	UINT8 u1Val = 0 ;
	//UINT32 u4Ofst = 0;
	#endif

	mcSHOW_USER_MSG(("[MS DEBUG]DVBC_Initialize!\n"));
	
    psDemodCtx->u2ErrorPkt = 0;
    psDemodCtx->u2TotalPkt = 0;
    psDemodCtx->u4CorrBit = 0;
    psDemodCtx->ucEnFpga = FALSE;
    psDemodCtx->I2cAddress = ucI2cAddress;
    psDemodCtx->ucConnType = PORT_CFG_MTK;
    psDemodCtx->ucTsIfFormat = 0x8C;   //changed from 0x0C to 0x8C due to MT5360B, and no impact to MT539x and MT5360A
    psDemodCtx->ucTsIfCtrl = 0x00;
    psDemodCtx->ucTsIfRate = 0x07;
    psDemodCtx->ucSramSize = 32;
    psDemodCtx->m_TunerMode = 0;
    psDemodCtx->i4BerTh = 430;   // JC, 090202
    psDemodCtx->u2SymbolrateMax = ESTIMATE_SYMBOL_ACT_MAX;
	psDemodCtx->u2SymbolrateMin = ESTIMATE_SYMBOL_ACT_MIN;
	#if defined(CC_MT5136)
	psDemodCtx->ucSramSize = 56;
	#endif

//rest SLD before load FW.lei110805
#if defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
#if (ATD_NOT_USE==TRUE)

    //UCHAR ucData = 0x00;
    //Reset SLD REG 
	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf61, &ucData, 1);

	ucData = 0x00;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);
	ucData = 0xff;
	DVBC_SetReg(psDemodCtx, 0xf60, &ucData, 1);

	
#endif
#endif


    #if defined(CC_MT5136)
	//Mingsheng test bradcust type
	//u4Ofst = 0x51e;
	//u4Ofst = DRVCUST_OptGet(eBrdcstSrcOfst0);
	//VERIFY(0 == EEPROM_Read((UINT64)u4Ofst, (UINT32)(void *)&u1Val, 1));
	//mcSHOW_USER_MSG(("[MS debug]DVBC_Initialize EEPROM_Read 0x%0x = 0x%0x!\n",u4Ofst, u1Val));
	u1Val = DVBC_GetEEPromBrdcstType(psDemodCtx);
	

	 mcSHOW_USER_MSG(("[MS debug] u1Val is 0x%x , cDEMOD_DVBC_MODE is 0x%x!\n", u1Val, cDEMOD_DVBC_MODE));
	    
	if (u1Val != cDEMOD_DVBC_MODE)
	{    
	    mcSHOW_USER_MSG(("[MS debug]No  DVBC_Initialize, RETUEN\n"));
		return DRVAPI_TUNER_OK;
	}
	else 
	{  
	   DVBC_SetEEPromBrdcstType(psDemodCtx);
	   mcSHOW_USER_MSG(("[MS debug]Need initial in DVBC_Initialize\n"));
	}

	mcSHOW_USER_MSG(("[MS debug]5136 DVBC_ChipInit!\n"));
	DVBC_ChipInit(psDemodCtx);
    #endif
	
	#if defined(INTERNAL_DTMB)
	DVBC_ChipInit(psDemodCtx);
	#endif
    if (fgLoadCode)
    {
        if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
        {   
            mcSHOW_USER_MSG(("[MS debug1]DVBC_Initialize RF tuner!\n"));
            pCodeImage=ucDvbcCodeImage;
            u2CodeImageLen=sizeof(ucDvbcCodeImage);
        }
        else//legacy tuner
        {    
             mcSHOW_USER_MSG(("[MS DEBUG1]DVBC_Initialize LEGACY tuner!\n"));
             pCodeImage=ucDvbcCodeImageLeg;
             u2CodeImageLen=sizeof(ucDvbcCodeImageLeg);
        }

       //DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);
	 //  mcSHOW_USER_MSG(("[DVBC_Initialize]DVBC_SetTsIf has been done.\n"));
        if (s4LoadCode(psDemodCtx, pCodeImage,u2CodeImageLen, cMAX_I2C_LEN, TRUE))
        {
            return (DRVAPI_TUNER_ERROR);
        }

		
		#if defined(INTERNAL_DTMB)
        mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_SWITCH;
        mccSET_QUEUE2(aucData, ccOFST_DVBTC_SWITCH) = cCONN_TYPE_DVBC;//0x01:DVBC, 0x02: DVBT
        mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBTC_SWITCH_SIZE+1);//lei121119

        // delay for waiting FW initialization done, may have I2C timeout at first
        for (ii = 0; ii < 5; ii++)
        {
            mcDELAY_MS(3);
            DVBC_GetReg(psDemodCtx, DVBC_REG_uP_GP_REG06, &ucValue, 1);
            if (ucValue == ccFW_RUN_TAG)
                break;
        }

        mcSHOW_DBG_MSG(("[xx]Has internal DTMB,Need swith fw to DVBC!\n"));
		#endif
		
        // Set TS IF
        DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);
        #if 0
        #if  (defined(CC_MT5136) && defined (CC_MT5881))
        DVBC_SetTsIf(psDemodCtx, psDemodCtx->ucTsIfFormat, psDemodCtx->ucTsIfCtrl, psDemodCtx->ucTsIfRate);
		mcSHOW_USER_CRI_MSG(("[DVBC_Initialize]DVBC_SetTsIf\n"));
#endif
#endif
        // AGC crrve setting
        //ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA0;
        //for (ii = 0; ii < 4; ii++)
        //    ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
        //vSetIntfPara(psDemodCtx, ucPara, ccOFST_DVBC_SET_TUNER_PARA0_SIZE+1);

        // set agc parameters
       // DVBC_SetIF(psDemodCtx);
        /*
        ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA1;
        ucPara[1] = pEuTunerCtx->m_aucPara[7];   // IF taget level, reserved
        ucPara[2] = mcLOW_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);   // IF_FREQ_LOW
        ucPara[3] = mcHIGH_BYTE(psDemodCtx->psTunerCtx->u2IF_Freq);  // IF_FREQ_HIGH
        vSetIntfPara(psDemodCtx, ucPara, ccOFST_SET_TUNER_PARA1_SIZE+1);
        */
        // set to blind search mode
        //vSetChSearchMode(psDemodCtx); /* Zhang Chi mark, and will be added in the later time */

        // Ken, GetVersion for previous host command complete, 090114
        #if 0
        mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
        if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
        {
            mcSHOW_USER_MSG(("DVBC_Initialize OK!\n"));
        }
        #endif
        mcSHOW_USER_MSG(("DVBC_Initialize OK!\n"));
    }     
	
	
	#if (!defined(CC_MT5136)) && (!defined(INTERNAL_DTMB))
    DVBC_ChipInit(psDemodCtx);
	#else
	mcSHOW_USER_MSG(("Had chipinit done, so skip!\n"));
	#endif

    return (DRVAPI_TUNER_OK);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetTotPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2TotalPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetErrPkt(DEMOD_CTX_T *psDemodCtx)  
{
    return (UINT32)psDemodCtx->u2ErrorPkt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPER(DEMOD_CTX_T *psDemodCtx)  // return value = PER*10^5
{
    INT32 RSTotPkt, RSErrPkt;
        
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;

    if (RSTotPkt <= 0)
        return -1;
    else
        return (INT32) ((RSErrPkt * 100000) / RSTotPkt);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetTsLock(DEMOD_CTX_T *psDemodCtx)
{
    if (psDemodCtx->ucRSHState)
        psDemodCtx->fgTsLock = TRUE;
    else
        psDemodCtx->fgTsLock = FALSE;

    return psDemodCtx->fgTsLock;
}

//----------------------------------------------------------------------------- 
/*
 *  vDVBCSetTunerSSICond
 *  Set Tuner SSI Condition.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------

void vDVBCSetTunerSSICond(DEMOD_CTX_T *psDemodCtx)
{
    INT16 InOutVal[5];

    //if(DVBT_GetSQI(psDemodCtx) > 2)/* have signal */
	//DVBC_UpdateInfo(psDemodCtx);

	
    if (psDemodCtx->fgTsLock)
    {
        InOutVal[0]= (INT16)psDemodCtx->ucQamMode;
        InOutVal[1]= 0;//(INT16)psDemodCtx->u1CR;
        InOutVal[2]= (INT16)psDemodCtx->s2SldPraRfagcCtrlWord;
		if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
		{
			InOutVal[3]= (INT16)psDemodCtx->ucSldPraGainIdx;
		}
		else
		{
			InOutVal[3]= (INT16)psDemodCtx->s2SldPraIfagcCtrlWord;
		}
        InOutVal[4]= psDemodCtx->s2SldPraOut;
    }
    else/* no signal */
    {
        InOutVal[0]= 5;
        InOutVal[1]= 0;//(INT16)psDemodCtx->u1CR;

        InOutVal[2]= (INT16)psDemodCtx->s2SldPraRfagcCtrlWord;
		if(psDemodCtx->psTunerCtx->fgRFTuner)//Sawless tuner
		{
			InOutVal[3]= (INT16)psDemodCtx->ucSldPraGainIdx;
		}
		else
		{
			InOutVal[3]= (INT16)psDemodCtx->s2SldPraIfagcCtrlWord;
		}
        InOutVal[4]= psDemodCtx->s2SldPraOut;

        mcSHOW_DBG_MSG(("vDVBCSetTunerSSICond: TS unlock, no signal\n"));
    }
    mcSHOW_DBG_MSG(("DVBC: Mod=%d, Code Rate=%d, SLD_RFAGC=%d, SLD_IFAGC=%d, Legacy_Index=%d\n",InOutVal[0],InOutVal[1],InOutVal[2],InOutVal[3],InOutVal[4]));
    ITuner_OP( ITunerGetCtx(), itSetSSICond,0, &InOutVal);
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalLevel
 *  Get signal level from 0 (lowest) to 100 (highest).
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) 0~100.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSignalLevel(DEMOD_CTX_T *psDemodCtx)
{
    INT8 s1AgcMax;
    INT8 s1AgcMin;
    INT32 s2SignalStrength;
	INT16 s2Temp = 0;
    SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

    vDVBCSetTunerSSICond(psDemodCtx);
	DigTunerBypassI2C(TRUE);
	s2Temp = ITuner_OP( ITunerGetCtx(), itGetSSIIndex,1, &s2SignalStrength);
	DigTunerBypassI2C(FALSE);
    if(s2Temp == ITUNER_OK)
    {
        if(psDemodCtx->fgTsLock ==TRUE)
        {
            return (UINT8)s2SignalStrength;
        }
        else 
        {    
            return 0;
        }
    }
    else
    {      
        //s1AgcMax = (INT8)psDemodCtx->sTunerCtx.m_aucPara[4];
        //s1AgcMin = (INT8)psDemodCtx->sTunerCtx.m_aucPara[5];
        s1AgcMax = (INT8)pEuTunerCtx->m_ifAgc_lvl_max;
        s1AgcMin = (INT8)pEuTunerCtx->m_ifAgc_lvl_min;

        if (s1AgcMax != s1AgcMin)
#if defined(CC_MT5135) || defined(CC_MT5396)|| defined(CC_MT5368)|| defined(CC_MT5398)||defined(CC_MT5880) ||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
            s2SignalStrength = ((INT16)(psDemodCtx->ucSldPraGainIdx-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#else
            s2SignalStrength = ((INT16)(psDemodCtx->s1IfAgc-s1AgcMin))*100 / (INT16)(s1AgcMax-s1AgcMin); // IF AGC gain
#endif
        else
            s2SignalStrength = 0;

        if ( ((INT8)pEuTunerCtx->m_aucPara[2]) > 0)
            s2SignalStrength = 100 - s2SignalStrength;

        //if (psDemodCtx->fgTsLock == TRUE)
        {
            if      (s2SignalStrength < 0)
                return 0;
            else if (s2SignalStrength > 100)
                return 100;
            else
                return (UINT8)s2SignalStrength;
        }
    }
    //return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) SNR in dB.   
 */
//-----------------------------------------------------------------------------
UINT16 DVBC_GetSignalSNR(DEMOD_CTX_T *psDemodCtx)
{
    INT16 s2_data;

    if (psDemodCtx->fgTsLock == TRUE)
    {
//        s2_data = (psDemodCtx->s2Snr) / 256;
        s2_data = (psDemodCtx->s2Snr) / 25; // return 0.1 dB
        if (s2_data < 0)
            return 0;
        else
            return  s2_data;
    }
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  vSetIntfPara
 *  Set host command interface API.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  para  Pointer of parameters buffer. (para[0] : CmdId, para[1~(len-1)] : parameters).
 *  @param  len  Length of parameters
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
static void vSetIntfPara(DEMOD_CTX_T *psDemodCtx, UCHAR *para, UCHAR len)
{
UCHAR   aucData[cMAX_READ_NUM + 1];
UCHAR   ii;

    if(len>(cMAX_READ_NUM +1))/*Prevent aucData Buffer Overflow*/
    {
        return;
    }
    mccSET_CMD2(aucData) = para[0];
    if (len > 1)
    {
        for (ii = 0; ii < (len-1); ii++)
            mccSET_QUEUE2(aucData, ii) = para[ii+1];
    }
    
    mcHOST_CMD(psDemodCtx, aucData, 0, len);//lei130823
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetTsIf
 *  Set TS interface parameters.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
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
void DVBC_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate)
{
#if 0
	UCHAR ucData;
	   DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
	   mcSHOW_USER_CRI_MSG(("[DVBC_SetTsIf] Get  DVBC_REG_TS_Parallel_Serial_Cfg = 0x%02x \n", ucData));
	   mcCLR_BIT(ucData, 2);
	   mcSHOW_USER_CRI_MSG(("[DVBC_SetTsIf] Set  DVBC_REG_TS_Parallel_Serial_Cfg = 0x%02x \n", ucData));
	   DVBC_SetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
	   #endif

}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsOutputMode
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel)
{
    UCHAR ucData;

    //DVBC_GetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    mcSHOW_DBG_MSG(("DVBC_SetTsOutputMode: fgParallel = %d\n", fgParallel));
    mcSHOW_DBG_MSG(("DVBC_SetTsOutputMode: read 0xC38 = 0x%02X\n", ucData));

    if (fgParallel)
        ucData |= 0x04;
    else
        ucData &= 0xfB;

    mcSHOW_DBG_MSG(("DVBC_SetTsOutputMode: write 0xC38 = 0x%02X\n", ucData));
    //DVBC_SetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    DVBC_SetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    //Wait TS mode change
    mcDELAY_MS(1);
}

//----------------------------------------------------------------------------- 
/** DVBC_SetQTREarlyONOFF
 *  Set TS output mode
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetQTREarlyONOFF(DEMOD_CTX_T *psDemodCtx, BOOL fgQTREarly)
{
	UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_QTRER_EN;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_QTRER_ENABLE) = fgQTREarly;

    if(mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_QTRER_TSIZE+1)== FALSE)//lei121119
    {
        mcSHOW_USER_MSG(("DVBC Set QTR Earlybreak enable flag!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_SetTsPathMode
 *  Set TS path mode
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable)
{
#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
	UINT32 u4Temp;

    SPI_SDARead(&u4Temp, 0x0001);
    if (fgEnable)
        u4Temp |= 0x000C0000;
    else
        u4Temp &= 0xFFF3FFFF;
    SPI_SDAWrite(&u4Temp, 0x0001);
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPostVBer(DEMOD_CTX_T *psDemodCtx) // return value = PostVBer * 10^5
{
    INT32 RSTotPkt, RSErrPkt, RSCorrBit;

    if (!psDemodCtx)
        return -1;

    // PostVBer = (RSCorBit + RSErrPkt * ERR_BITS_PER_ERR_PKT) / (RSTotPkt *188*8)
    // 66 = 10^5 / (188*8)
    
    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;
    RSCorrBit = (INT32)psDemodCtx->u4CorrBit;
    
    if (RSTotPkt <= 0)
        return (-1);
    else
        return ( (RSCorrBit + RSErrPkt*ERR_BITS_PER_ERR_PKT) * 66 / RSTotPkt );
}
//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^7.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPostVBer_E7(DEMOD_CTX_T *psDemodCtx) // return value = PostVBer * 10^7
{
    INT32 RSTotPkt, RSErrPkt, RSCorrBit;

    if (!psDemodCtx)
        return -1;

    // PostVBer = (RSCorBit + RSErrPkt * ERR_BITS_PER_ERR_PKT) / (RSTotPkt *188*8)
    // 66 = 10^5 / (188*8)    
    //6649 = 10^ 7 / (188*8)=50*133

    RSTotPkt = (INT32)psDemodCtx->u2TotalPkt;
    RSErrPkt = (INT32)psDemodCtx->u2ErrorPkt;
    RSCorrBit = (INT32)psDemodCtx->u4CorrBit;


	mcSHOW_DBG_MSG(("RSTotPkt = %d    RSErrPkt = %d    RSCorrBit = %d\n",RSTotPkt,RSErrPkt,RSCorrBit));

    if (RSTotPkt <= 0)
        return (-1);
    else
        return ( (INT32)(((UINT32)((RSCorrBit + RSErrPkt*ERR_BITS_PER_ERR_PKT) * 6649) )/ RSTotPkt ));

}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetConnType
 *  Set connection type.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  type  Connection type. (Default is 0).
 *  @retval   void. 
 */
//-----------------------------------------------------------------------------
void DVBC_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type)
{
    psDemodCtx->ucConnType = type;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetPowerMode
 *  Set power mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucMode  Unused.
 *  @retval   0. 
 */
//-----------------------------------------------------------------------------
UCHAR DVBC_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode)
{
    return 0;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_DumpRegisters
 *  Dump Demod registers.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  pucBuf  Pointer of data buffer.
 *  @param  bank_no  Register bank number.
 *  @param  max_i2c_len  Max I2C read byte at one time
 *  @retval   1 : I2C error.
 *  @retval   0 : I2C OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len)
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

        if (DVBC_GetReg(psDemodCtx, u2StartAddr + u2CurAddr,
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
/** DVBC_SetQamMode
 *  Set QAM Mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Mode  QAM mode selection.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetQamMode(DEMOD_CTX_T *psDemodCtx, UINT8 Mode)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_QAM;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_QAM_MODE) = Mode;

    if(mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_QAM_TSIZE+1)== FALSE)//lei121119
    {
        mcSHOW_USER_MSG(("DVBC Set Qam Mode Fail!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_SetSymbolRate
 *  Set Symbol Rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  Rate  symbol rate in kSym/sec.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetSymbolRate(DEMOD_CTX_T *psDemodCtx, UINT16 Rate)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_SYMBOL_RATE;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_SYMBOL_RATE_H) = mcHIGH_BYTE(Rate);
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_SYMBOL_RATE_L) =  mcLOW_BYTE(Rate);

    if(mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_SYMBOL_RATE_TSIZE+1) == FALSE)//lei121119
    {
        mcSHOW_USER_MSG(("DVBC Set Symbol Rate Fail!\n"));
    }
}

//----------------------------------------------------------------------------- 
/** DVBC_AcqStart
 *  Start Acquistion.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_AcqStart(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_ACQ_START;
    mcHOST_CMD(psDemodCtx, aucData, 0, 1);//lei121119
}

void DVBC_AdvAQM(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvAQM)
{
#if 1
    UCHAR   aucData[cMAX_READ_NUM + 1];

    /* bfgAdvAQM=TRUE, do AdvAQM. */
    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_ADV_AQM;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_ADV_AQM_ENABLE) = (UINT8) bfgAdvAQM;
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_ADV_AQM_TSIZE+1);//lei121119
#endif
}

void DVBC_AdvASR(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvASR)
{
#if 1
    UCHAR   aucData[cMAX_READ_NUM + 1];

    /* bfgAdvASR=TRUE, do AdvASR. */
    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_ADV_ASR;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_ADV_ASR_ENABLE) = (UINT8) bfgAdvASR;
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_ADV_ASR_TSIZE+1);//lei121119
#endif
}

//----------------------------------------------------------------------------- 
/** DVBC_TunerI2c
 *  Switch Tuner I2C.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  fgSwitchOn  Switch ON/OFF of tuner I2C
 *  @retval   S16.
 */
//-----------------------------------------------------------------------------
S16 DVBC_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn)
{
    UCHAR   aucData[cMAX_READ_NUM + 1]; 
    U8      status;

    if (!psDemodCtx)
        return -2;

    if (fgSwitchOn)
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 1;
        if (psDemodCtx->ucEnFpga)
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x04;
        else
            mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50; 
    }
    else 
    {
        mccSET_CMD2(aucData) = ccCMD_ID_TUNER_I2C_ONOFF;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_ONOFF) = 0;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_DEGLITCH) = 0x07;
        mccSET_QUEUE2(aucData, ccOFST_TUNER_I2C_PULLUP_DEL) = 0x50;
    }

    status = mcHOST_CMD(psDemodCtx, aucData, ccOFST_TUNER_I2C_ONOFF_RSIZE, ccOFST_TUNER_I2C_ONOFF_SIZE+1);
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
}

//----------------------------------------------------------------------------- 
/** DVBC_UpdateStatus
 *  Get Status.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateStatus(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   ucData[2];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_STATUS0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_STATUS0_SIZE, 1))
    {
        ucData[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS00);

        psDemodCtx->ucPraLock     = mcGET_FIELD(ucData[0], 0x03, 0);
        psDemodCtx->ucQNIRState   = mcGET_FIELD(ucData[0], 0x04, 2);
        psDemodCtx->ucQTRState    = mcGET_FIELD(ucData[0], 0x08, 3);
        psDemodCtx->ucQCRFftState = mcGET_FIELD(ucData[0], 0x10, 4);
        psDemodCtx->ucQCRState    = mcGET_FIELD(ucData[0], 0x20, 5);
        psDemodCtx->ucRSHState    = mcGET_FIELD(ucData[0], 0x40, 6);

        psDemodCtx->ucQCNFailState = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_QCN_FAIL_CNT);
        psDemodCtx->ucASRFailState = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_ASR_FAIL_CNT);
        psDemodCtx->ucReAcqCnt     = mccGET_QUEUE(aucData, ccOFST_DVBC_STATUS_REACQ_CNT);
    }
	mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO7;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO7_SIZE, 1))
    {
        ucData[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SpecInv_QCN_State);
        psDemodCtx->u1QSpecInv = mcGET_FIELD(ucData[0], 0x80, 7);
        psDemodCtx->ucQcnState = ucData[0] & 0x07;

        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SYSC_State);
        psDemodCtx->ucCurrentAcqState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_ucCurAcqState);
        psDemodCtx->ucDev_AWGN_PN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN_PN);
        psDemodCtx->ucDev_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN);
        psDemodCtx->ucCor_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uCor_AWGN);
    }
}


//----------------------------------------------------------------------------- 
/** DVBC_UpdateSymbol
 *  Get demod information and update demod symbol rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateSymbol(DEMOD_CTX_T *psDemodCtx)

{   
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[5];
	//To compensate Symbol Rate OffSet
	INT32 SR_Old = 0;
	INT32 SR_NEW = 0;
	INT32 SR_TFO = 0;
	INT32 SR_OffSet = 0;  

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO1_SIZE, 1))
    {   
    
	    psDemodCtx->ucQamMode = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_QAM_MODE);
	   
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_H);
        psDemodCtx->u2SymbolRate = mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
		mcSHOW_DBG_MSG(("[Lei]DVBC_UpdateSymbol symbole rate is = %d\n", psDemodCtx->u2SymbolRate));   

        psDemodCtx->fgAsrReady = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_RDY);
		// Sony Tina TC : Symbol Rate is not right without CFO compensation
		SR_Old = psDemodCtx->u2SymbolRate;
		SR_NEW = SR_Old;
		SR_TFO = psDemodCtx->i4TFO;
		SR_OffSet =  (SR_Old * SR_TFO)/100000;
		if (SR_OffSet > 5)
			{
				SR_NEW = SR_Old + 1;
			}			
		if (( SR_OffSet < -5 ) && (SR_OffSet > -10))
			{
				SR_NEW = SR_Old - 1;
			}	
		mcSHOW_DBG_MSG4(("SR_Old    =	%d\n",SR_Old));
		mcSHOW_DBG_MSG4(("SR_TFO    =	%d\n",SR_TFO)); 		
		mcSHOW_DBG_MSG4(("SR_OffSet =	%d\n",SR_OffSet));
		mcSHOW_DBG_MSG4(("SR_NEW    =	%d\n",SR_NEW));
		psDemodCtx->u2SymbolRate = SR_NEW ;
		// Sony Tina TC : Symbol Rate is not right without CFO compensation
		mcSHOW_DBG_MSG3(("[MS DEBUG]ccCMD_ID_DVBC_GET_INFO1 symbole rate is = %d\n", psDemodCtx->u2SymbolRate));   
    }	
}



//----------------------------------------------------------------------------- 
/** DVBC_UpdateInfo
 *  Get Info.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateInfo(DEMOD_CTX_T *psDemodCtx)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[5];
    U32 u4temp=0;
	//To compensate Symbol Rate OffSet
	INT32 SR_Old = 0;
	INT32 SR_NEW = 0;
	INT32 SR_TFO = 0;
	INT32 SR_OffSet = 0;  

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO0;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO0_SIZE, 1))
    {
        //corrected error bits
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_COR_BIT2);
        psDemodCtx->u4CorrBit = uc_data[0];
        psDemodCtx->u4CorrBit += (U32)mcUNSIGN_2BYTE(uc_data[2], uc_data[1], 7, 8) * 256;
        //total error packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_ERR_PKT1);
        psDemodCtx->u2ErrorPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8); 
        //total packets
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO0_RS_TOT_PKT1);
        psDemodCtx->u2TotalPkt = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO1;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO1_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_AGC_GAIN_H);
        psDemodCtx->s2AgcGain = mcSIGN_2BYTE(uc_data[1], uc_data[0], 6, 8);   // Gain * 2^13

        psDemodCtx->ucQamMode = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_QAM_MODE);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_L);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_H);
        psDemodCtx->u2SymbolRate = mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
mcSHOW_DBG_MSG(("Lei DVBC_UpdateInfo symbole rate is = %d\n", psDemodCtx->u2SymbolRate));  
        psDemodCtx->fgAsrReady = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_SYMBOL_RATE_RDY);

        psDemodCtx->ucDsmSel = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO1_DSM_SEL);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO2;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO2_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_CARRIER_OFFSET3);

    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO3 = %d\n", uc_data[3]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO2 = %d\n", uc_data[2]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO1 = %d\n", uc_data[1]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_QTR_CFO0 = %d\n", uc_data[0]));    //Iven, 20110303    


       // psDemodCtx->s4Cfo = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
       // psDemodCtx->s4Cfo += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;
        psDemodCtx->s4QCR_CFO = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);    // Iven, 20110303
        psDemodCtx->s4QCR_CFO += (S32)mcSIGN_2BYTE(uc_data[3], uc_data[2], 5, 8) * 256 * 256;   

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_EQ_SNR1);
        psDemodCtx->s2Snr = mcSIGN_2BYTE(uc_data[1], uc_data[0], 7, 8);   // SNR * 256

        psDemodCtx->u1Bw = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO2_QCR_DD_BW);
    }

    

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO4;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO4_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_3);
        uc_data[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO4_TIMING_OFFSET_4);

        psDemodCtx->s4TimingOffset = ((S32)uc_data[4]) << 8;
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[3])) <<8;        
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[2])) <<8;                
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[1])) <<8;                
        psDemodCtx->s4TimingOffset = ((psDemodCtx->s4TimingOffset) + ((U32)uc_data[0])) ;                        
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO5;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO5_SIZE, 1))
    {
        psDemodCtx->ucASR_CFOflag = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CONFIDENCE);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO0);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO1);
        uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO2);
        uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO3);

        u4temp = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
        u4temp += (U32)mcUNSIGN_2BYTE(uc_data[3], uc_data[2], 8, 8) * 256 * 256;

    if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 4))    // CFO_Con = 2, valid
    {
         if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 0))
         {
             psDemodCtx->s4ASR_CFO =(S32)( 0 - u4temp );
         }
         else
         {
             psDemodCtx->s4ASR_CFO = (S32)u4temp;
         }
    }
    else
    {
        psDemodCtx->s4ASR_CFO = 0;
    }

	mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO3;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO3_SIZE, 1))
    {
        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_SYSC);
        psDemodCtx->ucQcnState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCN);
        psDemodCtx->ucQcrReturnRC = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCR_RETURN_RC);
        psDemodCtx->ucFftSearchResult = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QTR_FFT_SEARCH_RESULT);
        psDemodCtx->ucPraState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_PRA);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_BN_MONITOR);
		uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_CARRIER_INVERSE);
		mcSHOW_DBG_MSG3(("b_QCR_Spectrum_inverse = 0x%x\n", uc_data[1]));     
        psDemodCtx->ucBNflag = mcGET_FIELD(uc_data[0], 0x40, 6);
		if (uc_data[1] == 0x01)
		{
			psDemodCtx->s4QCR_CFO = 0 - psDemodCtx->s4QCR_CFO;
			psDemodCtx->s4ASR_CFO = 0 - psDemodCtx->s4ASR_CFO;
		}
				
    }

    mcSHOW_DBG_MSG3(("psDemodCtx->ucASR_CFOflag = 0x%x\n", psDemodCtx->ucASR_CFOflag));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO3 = 0x%x\n", uc_data[3]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO2 = 0x%x\n", uc_data[2]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO1 = 0x%x\n", uc_data[1]));    //Iven, 20110303
    mcSHOW_DBG_MSG3(("ccOFST_DVBC_INFO5_ASR_CFO0 = 0x%x\n", uc_data[0]));    //Iven, 20110303    
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO6;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO6_SIZE, 1))
    {
        psDemodCtx->ucAQM_Seq[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_00);
        psDemodCtx->ucAQM_Seq[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_01);
        psDemodCtx->ucAQM_Seq[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_02);
        psDemodCtx->ucAQM_Seq[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_03);
        psDemodCtx->ucAQM_Seq[4] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_04);
        psDemodCtx->ucAQM_ID = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO6_AQM_Index);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO7;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO7_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SpecInv_QCN_State);
        psDemodCtx->u1QSpecInv = mcGET_FIELD(uc_data[0], 0x80, 7);
        psDemodCtx->ucQcnState = uc_data[0] & 0x07;

        psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_SYSC_State);
        psDemodCtx->ucCurrentAcqState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_ucCurAcqState);
        psDemodCtx->ucDev_AWGN_PN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN_PN);
        psDemodCtx->ucDev_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uDev_AWGN);
        psDemodCtx->ucCor_AWGN = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO7_uCor_AWGN);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO8;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO8_SIZE, 1))
    {
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_JTD_01);
        psDemodCtx->u2JTD_Result = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_00);
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO8_LockTime_01);
        psDemodCtx->u2AcqTime = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO9;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO9_SIZE, 1))
    {
//        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_STATUS_MONITOR);
//        psDemodCtx->ucSldPraState = mcGET_FIELD(uc_data[0], 0x60, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_RFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraRfagcCtrlWord = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_IFAGC_CTRL_WORD_L);
        psDemodCtx->s2SldPraIfagcCtrlWord = mcSIGN_2BYTE(uc_data[1], uc_data[0], 8, 5);

        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO9_SLD_PRA_PGA_GAIN_INDEX);
        psDemodCtx->ucSldPraGainIdx = mcGET_FIELD(uc_data[0], 0x1f, 0);
    }

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFOA;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFOA_SIZE, 1))
    {
        uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_H);
        uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFOA_SLD_PRA_MAG_REF_OUT_L);
        psDemodCtx->s2SldPraOut = (S16) mcSIGN_2BYTE(uc_data[1], uc_data[0], 3, 8);
    }	
	// Sony Tina TC : Symbol Rate is not right without CFO compensation
	SR_Old = psDemodCtx->u2SymbolRate;
	SR_NEW = SR_Old;
	SR_TFO = DVBC_GetTFO(psDemodCtx);
	SR_OffSet =  (SR_Old * SR_TFO)/100000;
	if (SR_OffSet > 5)
		{
			SR_NEW = SR_Old + 1;
		}			
	if (( SR_OffSet < -5 ) && (SR_OffSet > -10))
		{
			SR_NEW = SR_Old - 1;
		}	
	mcSHOW_DBG_MSG4(("SR_Old	=	%d\n",SR_Old));
	mcSHOW_DBG_MSG4(("SR_TFO	=	%d\n",SR_TFO)); 		
	mcSHOW_DBG_MSG4(("SR_OffSet =	%d\n",SR_OffSet));
	mcSHOW_DBG_MSG4(("SR_NEW	=	%d\n",SR_NEW));
	psDemodCtx->u2SymbolRate = SR_NEW ;
	// Sony Tina TC : Symbol Rate is not right without CFO compensation
	psDemodCtx->i4TFO = SR_TFO;
}

//----------------------------------------------------------------------------- 
/** DVBC_GetCFO
 *  Get CFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   S32 CFO in kHz * 10^5.
 */
//-----------------------------------------------------------------------------
S32 DVBC_GetCFO(DEMOD_CTX_T *psDemodCtx)
{
    S32 s4Cfo_value;

    DVBC_UpdateInfo(psDemodCtx);// iven, 20110303
//    s4Cfo_value = psDemodCtx->s4Cfo * 10;   // (reg/2^29*54*1000) * 10^5

    psDemodCtx->s4Cfo =  psDemodCtx->s4QCR_CFO* 10 + psDemodCtx->s4ASR_CFO*10;   // (reg/2^29*54*1000) * 10^5
    s4Cfo_value = psDemodCtx->s4Cfo;
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_QCR_CFO = %dkhz\n", psDemodCtx->s4QCR_CFO/10000));    //Iven, 20110303    
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_ASR_CFO = %dkhz\n", psDemodCtx->s4ASR_CFO/10000));    //Iven, 20110303    
    mcSHOW_DBG_MSG3(("DVBC_GetCFO(s)_s4Cfo = %dkhz\n", psDemodCtx->s4Cfo/100000));    //Iven, 20110303

    return s4Cfo_value;
}

//----------------------------------------------------------------------------- 
/** DVBC_GetTFO
 *  Get TFO.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   UINT32 TFO in ppm.
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetTFO(DEMOD_CTX_T *psDemodCtx)
{
#if 1
    // Hui mark for Linux build error. 20100402
    INT32  s4TFO;

    s4TFO = ((((( (psDemodCtx->s4TimingOffset) * 37) >> 18) * ((S32)psDemodCtx->u2SymbolRate)) << (psDemodCtx->ucDsmSel)) >> 11);


    return s4TFO;
#else
    return 0;
#endif
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTsFmt
 *  Get demod TS output format.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
TS_FMT_T DVBC_GetTsFmt(DEMOD_CTX_T *psDemodCtx)
{
#if 0
UCHAR    ucData, ucClk;
TS_FMT_T fgTsFmt;

    DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
    mcSHOW_USER_CRI_MSG(("[DVBC_GetTsFmt] Get  DVBC_REG_TS_Parallel_Serial_Cfg = 0x%02x \n", ucData));
    if (ucData & 0x04){
        fgTsFmt.SerOrPar = SP_PARALLEL;
		mcSHOW_USER_CRI_MSG(("[DVBC_GetTsFmt] gTsFmt.SerOrPar = SP_PARALLE \n", ucData));
    	}
    else
       { fgTsFmt.SerOrPar = SP_SERIAL;
	mcSHOW_USER_CRI_MSG(("[DVBC_GetTsFmt] fgTsFmt.SerOrPar = SP_SERIAL \n", ucData));
    	}

    if (ucData & 0x02)
        fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
    else
        fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;

    if (ucData & 0x01)
        fgTsFmt.fgGatedClock = BOOL_FALSE;
    else
        fgTsFmt.fgGatedClock = BOOL_TRUE;

#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
    UINT32   u4Temp;
    SPI_SDARead(&u4Temp, 0x0004);
    if (u4Temp & 0x00080000)
        fgTsFmt.fgTristate = BOOL_FALSE;
    else
        fgTsFmt.fgTristate = BOOL_TRUE;
#elif defined(CC_MT5396) || defined (CC_MT5368)|| defined(CC_MT5398) ||defined(CC_MT5880)||defined(CC_MT5881)//for MT5396,lei_code_110513;
	fgTsFmt.fgTristate = BOOL_FALSE;
#else   
	fgTsFmt.fgTristate = BOOL_FALSE;
#endif
    DVBC_GetReg(psDemodCtx, 0xC39, &ucClk, 1);
    if (ucData & 0x04)
    {
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
    }
    else
    {
        /* Serial */
        if (ucClk >= 0x07)
        {
            fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
        }
        else if (0x05 == ucClk)
        {
            fgTsFmt.u4TsClockInKHz = 36000;
        }
        else
        {
            fgTsFmt.u4TsClockInKHz = 54000;
        }
    }

    /* In-house DTD demod,  fgExternalDemod is BOOL_FALSE! */
	#if (defined(CC_MT5136) && defined (CC_MT5880)) || (defined(CC_MT5136) && defined (CC_MT5881)) // LL for 5136+5881
    fgTsFmt.fgExternalDemod = BOOL_TRUE;
	#else
	fgTsFmt.fgExternalDemod = BOOL_FALSE;
	#endif
	

    return fgTsFmt;
	#endif
	UCHAR    ucData, ucClk;
TS_FMT_T fgTsFmt;

    DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);

    if (ucData & 0x04)
        fgTsFmt.SerOrPar = SP_PARALLEL;
    else
        fgTsFmt.SerOrPar = SP_SERIAL;

    if (ucData & 0x02)
        fgTsFmt.ClockPolarity = POL_LOW_ACTIVE;
    else
        fgTsFmt.ClockPolarity = POL_HIGH_ACTIVE;

    if (ucData & 0x01)
        fgTsFmt.fgGatedClock = BOOL_FALSE;
    else
        fgTsFmt.fgGatedClock = BOOL_TRUE;

#if defined(CC_MT5135) || defined (CC_MT5135DVBC)
    UINT32   u4Temp;
    SPI_SDARead(&u4Temp, 0x0004);
    if (u4Temp & 0x00080000)
        fgTsFmt.fgTristate = BOOL_FALSE;
    else
        fgTsFmt.fgTristate = BOOL_TRUE;
//for MT5396,lei_code_110513;
#elif defined(CC_MT5396) || defined (CC_MT5368)|| defined(CC_MT5398) ||defined(CC_MT5880)||defined(CC_MT5881)||defined(CC_MT5399)||defined(CC_MT5890)||defined(CC_MT5882)
	fgTsFmt.fgTristate = BOOL_FALSE;
#else   
	fgTsFmt.fgTristate = BOOL_FALSE;
#endif
    DVBC_GetReg(psDemodCtx, 0xC39, &ucClk, 1);
    if (ucData & 0x04)
    {
        /* Parallel */
        fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
    }
    else
    {
        /* Serial */
        if (ucClk >= 0x07)
        {
            fgTsFmt.u4TsClockInKHz = 54000/2/(ucClk+1);
        }
        else if (0x05 == ucClk)
        {
            fgTsFmt.u4TsClockInKHz = 36000;
        }
        else
        {
            fgTsFmt.u4TsClockInKHz = 54000;
        }
    }

    /* In-house DTD demod,  fgExternalDemod is BOOL_FALSE! */
	#if (defined(CC_MT5136) && defined (CC_MT5880)) || (defined(CC_MT5136) && defined (CC_MT5881)) ||(defined(CC_MT5136) && defined (CC_MT5398))// LL for 5136+5881
    fgTsFmt.fgExternalDemod = BOOL_TRUE;
	mcSHOW_USER_MSG(("[DVBC_GetTsFmt]fgExternalDemod = BOOL_TRUE\n"));
	#else
	fgTsFmt.fgExternalDemod = BOOL_FALSE;
	#endif
	

    return fgTsFmt;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetSQI
 *  calculate Signal Quality Indicator.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_GetSQI(DEMOD_CTX_T *psDemodCtx)
{
	UINT16 CN_NorDigP1[] = {185, 215, 252, 275, 308};
	UINT8 u1QAMMode = 0;
	UINT16 u2CN_Rec = 0; 
	INT16  i2CN_Rel = 0;
	INT32  i2Post_Viterbi_BER = 0;
	UINT32 u4BER_SQI = 0; 
	UINT32 u4SQI_Indicator = 0;   
	
	u1QAMMode = psDemodCtx->ucQamMode;
	if(u1QAMMode > 4)
	{
		u1QAMMode = 4;
	}
		
	/* Get CN_rec Value */		
	u2CN_Rec = DVBC_GetSignalSNR(psDemodCtx);
			
	/* Get Post_BER Value */			
	i2Post_Viterbi_BER = DVBC_GetPostVBer(psDemodCtx);
	/* Get CN_Rel Value */
	i2CN_Rel = u2CN_Rec - CN_NorDigP1[u1QAMMode];
  
	/* Get BER_SQI Value */
	if(i2Post_Viterbi_BER > 10000)
	{
		u4BER_SQI = 0;
	}
	else if((i2Post_Viterbi_BER > 0) && (i2Post_Viterbi_BER <= 10000))
	{
		u4BER_SQI = (UINT32)((20*DVBC_LOG10((UINT32)(10000000/i2Post_Viterbi_BER))-4000));						  
	}
	else
	{
		u4BER_SQI = 100*100;
	}
	/* Cal SQI Indicator*/
	if((i2CN_Rel) < -70)
	{
		u4SQI_Indicator = 0;
	}
	else if(((i2CN_Rel) >= -70) && ((i2CN_Rel) < 30))
	{
		u4SQI_Indicator = (UINT32)(((i2CN_Rel + 70) * u4BER_SQI)/10000);
	}
	else if((i2CN_Rel) >= 30)
	{
		u4SQI_Indicator = (UINT32)(u4BER_SQI/100);
	}

	mcSHOW_USER_MSG(("DVBC_GetSQI: u1QAMMode=%d, CN_Rec=%d, BER=%d, SQI=%d\n", u1QAMMode, u2CN_Rec, i2Post_Viterbi_BER, u4SQI_Indicator));
		
	return u4SQI_Indicator;
}

/***********************************************************************/
CHAR *DVBC_GetVer(DEMOD_CTX_T *psDemodCtx)
{
   CHAR * cString1;
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = TRUE;
#endif
    vDVBC_GetAsicVer(psDemodCtx);
#if fcSHORT_LINUX_INIT
    psDemodCtx->fgUserGetInfo = FALSE;
#endif
    ITuner_OP(psDemodCtx->psTunerCtx,itGetVer,0,&cString1);
    mcSHOW_USER_MSG(("%s\n", cString1));

    //return szGetDrvVer();
    return NULL;
}
/***********************************************************************/
/*              Public Functions                                       */
/***********************************************************************/
VOID vDVBC_GetAsicVer(DEMOD_CTX_T *psDemodCtx)
{
     #if !defined(CC_MT5136)
UCHAR   aucData[cMAX_READ_NUM + 1];         // For Host Cmd
     #endif

    #if defined(CC_MT5135)|| defined(CC_MT5135DVBC)
    DVBC_GetDemodReg(psDemodCtx, MT5135_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("MT5135 Chip ID: %02x\n", aucData[0]));
	#endif

    #if !defined(CC_MT5136)
    DVBC_GetDemodReg(psDemodCtx, DVBC_REG_Chip_ID, aucData, 1);
    mcSHOW_USER_MSG(("DVBC Chip ID: %02x\n", aucData[0]));


    mccSET_CMD2(aucData) = ccCMD_ID_GET_VER;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_VER_SIZE, 1))
    {
        mcSHOW_USER_MSG(("FW build ver: %02X%02X-%02X%02X\n",
                         mccGET_QUEUE(aucData, ccOFST_VER_YEAR_MONTH), mccGET_QUEUE(aucData, ccOFST_VER_DATE),
                         mccGET_QUEUE(aucData, ccOFST_VER_HOUR), mccGET_QUEUE(aucData, ccOFST_VER_MINUTE)));
    }
	//Mingsheng 110802; add it for FW version control;
	mccSET_CMD2(aucData) = ccCMD_ID_GET_LABEL;
    if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_LABEL_SIZE, 1))
    {
        mcSHOW_USER_MSG(("FW update ver: %c%02X%02X\n",
                         mccGET_QUEUE(aucData, ccOFST_LABEL_BRANCH_TYPE), mccGET_QUEUE(aucData, ccOFST_LABEL_ID1),
                         mccGET_QUEUE(aucData, ccOFST_LABEL_ID2)));
    }
	#endif

}

BOOL DVBC_CheckLocked(DEMOD_CTX_T *psDemodCtx)
{

	INT32 s4Freq;
	#if defined(CC_MT5136)
    UCHAR   aucData[cMAX_READ_NUM + 1];
    UCHAR   uc_data[4];
    U32 u4temp=0;
	#endif

    /* If ASR_CFO<+-500kHz, Locked. */ 
    DVBC_UpdateInfo(psDemodCtx);

    s4Freq = (INT32)(psDemodCtx->s4ASR_CFO)*27/(1024*16)*1000/4/4096;

    mcSHOW_USER_MSG(("DVBC_CheckLocked(s)_u4Freq = %dkhz\n", s4Freq));            
    

    if((s4Freq<501) && (s4Freq>-501))
    {
        return TRUE;
    }
    else
    {
		#if defined(CC_MT5136)
		mcSHOW_DBG_MSG(("[Lei]####################\n"));
		mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO5;
		if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO5_SIZE, 1))
		{
			psDemodCtx->ucASR_CFOflag = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CONFIDENCE);
			uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO0);
			uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO1);
			uc_data[2] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO2);
			uc_data[3] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO5_ASR_CFO3);
		
			u4temp = (U16)mcUNSIGN_2BYTE(uc_data[1], uc_data[0], 8, 8);
			u4temp += (U32)mcUNSIGN_2BYTE(uc_data[3], uc_data[2], 8, 8) * 256 * 256;
			mcSHOW_DBG_MSG(("[XJ2]ASR_CFO_No FLAG = %d\n", u4temp));    
		
		if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 4))	// CFO_Con = 2, valid
		{
			 if(mcCHK_BIT1(psDemodCtx->ucASR_CFOflag, 0))
			 {
				 psDemodCtx->s4ASR_CFO =(S32)( 0 - u4temp );
			 }
			 else
			 {
				 psDemodCtx->s4ASR_CFO = (S32)u4temp;
			 }
		}
		else
		{
			psDemodCtx->s4ASR_CFO = 0;
		}
		
		mcSHOW_DBG_MSG(("[XJ2]ASR_CFO With FLAG = %d\n", psDemodCtx->s4ASR_CFO));	   
		
		mccSET_CMD2(aucData) = ccCMD_ID_DVBC_GET_INFO3;
		if (mcHOST_CMD(psDemodCtx, aucData, ccOFST_DVBC_INFO3_SIZE, 1))
		{
			psDemodCtx->ucSysCtrlState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_SYSC);
			psDemodCtx->ucQcnState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCN);
			psDemodCtx->ucQcrReturnRC = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QCR_RETURN_RC);
			psDemodCtx->ucFftSearchResult = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_QTR_FFT_SEARCH_RESULT);
			psDemodCtx->ucPraState = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_PRA);
			uc_data[0] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_BN_MONITOR);
			uc_data[1] = mccGET_QUEUE(aucData, ccOFST_DVBC_INFO3_CARRIER_INVERSE);
			mcSHOW_DBG_MSG(("b_QCR_Spectrum_inverse = 0x%x\n", uc_data[1]));	 
			psDemodCtx->ucBNflag = mcGET_FIELD(uc_data[0], 0x40, 6);
			if (uc_data[1] == 0x01)
			{
				psDemodCtx->s4QCR_CFO = 0 - psDemodCtx->s4QCR_CFO;
				psDemodCtx->s4ASR_CFO = 0 - psDemodCtx->s4ASR_CFO;
				mcSHOW_DBG_MSG(("[XJ2]ASR_CFO Inverse = %d\n", psDemodCtx->s4ASR_CFO));
				mcSHOW_DBG_MSG(("[XJ2]QCR_CFO Inverse = %d\n", psDemodCtx->s4QCR_CFO));
			}
					
		}
		
		}
		s4Freq = (INT32)(psDemodCtx->s4ASR_CFO)*27/(1024*16)*1000/4/4096;
		if((s4Freq<501) && (s4Freq>-501))
	    {
			mcSHOW_DBG_MSG(("[Lei]!!!!!!!!!!!!!!!!!!\n"));
			return TRUE;
	    }
	    else
	    {
        return FALSE;
    }
	#else
	return FALSE;
	#endif
}
}

VOID DVBC_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable)
{
    UCHAR    aucData[cMAX_READ_NUM + 1];
	#if defined(CC_MT5396) // Mingsheng 0929; Set TS Pad output mode for TS high clk issue.
        UINT32	 u4Temp = 0;	
    #elif defined(CC_MT5136)
	    UCHAR    ucData = 0;
    #endif
	

    if (fgOutputEnable) //// Enable  ts pad
    {     
    
	#if defined(CC_MT5396) // Mingsheng 0929; Set TS Pad output mode for TS high clk issue.
        //UINT32	 u4Temp = 0;
        u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	    u4Temp|=0x000f0000;
	    mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);

		u4Temp = mcRISC_Reg(CKGEN_BASE, 0x40c);//mingsheng add
		u4Temp&=0xffffff3f;
		u4Temp|=0x000f0040;
		mcSET_SHOW_REG(CKGEN_BASE, 0x40c, u4Temp);
	#elif defined(CC_MT5136)
	    //UCHAR    ucData = 0;
		DVBC_GetReg(psDemodCtx, 0x8FB,  &ucData, 1);
	    ucData = (ucData & 0x30) >> 4;
		if(ucData == 0x01) //48pin
		{ 
			DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
		    ucData = ucData | 0xF0;
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = 0xff;
			DVBC_SetReg(psDemodCtx, 0x8E2, &ucData, 1);
			   
			mcSHOW_DBG_MSG(("48pin ts output on \n"));
		}
	    else if(ucData == 0x02)	//128 pin
		{
			DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = ucData | 0xF0;
			#if defined(CC_MT5398)
			ucData = ucData & 0xF0;//lei131212 for Cobra+MT5136 128pin demux can't receive packet issue
			#endif
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			mcSHOW_DBG_MSG(("128pin ts output on \n"));
	    }
		else
		{
		    mcSHOW_DBG_MSG(("Error, not 48pin and 128pin\n"));
        
		}
	#endif
    }

	

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_SET_TS_OUTPUT;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_SET_TS_OUTPUT_ENABLE) = (UINT8) fgOutputEnable;
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_SET_TS_OUTPUT_TSIZE+1);//lei121119

    if (!fgOutputEnable)  // closed ts pad
    {
	#if defined(CC_MT5396) // Mingsheng 0929; Set TS Pad input mode for TS high clk issue.

		u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x4b4);
	      u4Temp|=0x000f0000;
	      mcSET_SHOW_REG(cRISC_DVBC_BASE, 0x4b4, u4Temp);

    	      u4Temp = mcRISC_Reg(CKGEN_BASE, 0x40c);//mingsheng add
		u4Temp&=0xffffff3f;
		mcSET_SHOW_REG(CKGEN_BASE, 0x40c, u4Temp);
	#elif defined(CC_MT5136)
	    DVBC_GetReg(psDemodCtx, 0x8FB,  &ucData, 1);
	    ucData = (ucData & 0x30) >> 4;
	    if(ucData == 0x01) //48pin
	    { 
	    	DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = ucData & 0x0F;
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = 0;
			DVBC_SetReg(psDemodCtx, 0x8E2, &ucData, 1);
			mcSHOW_DBG_MSG((" 48pin ts output disable\n"));
	    }
		else if(ucData == 0x02)  //128 pin
		{
			DVBC_GetReg(psDemodCtx, 0x8E1, &ucData, 1);
			ucData = ucData & 0x0F;
			#if defined(CC_MT5398)
			ucData = ucData | 0x0F;//lei131212 for Cobra+MT5136 128pin demux can't receive packet issue
			#endif
			DVBC_SetReg(psDemodCtx, 0x8E1, &ucData, 1);
			mcSHOW_DBG_MSG(("128pin ts output disable\n"));
		}
		else
		{
		    mcSHOW_DBG_MSG(("Error, not 48pin and 128pin\n"));
        
		}
	#endif
    }
}

VOID DVBC_SetDescramblerLock(DEMOD_CTX_T *psDemodCtx, BOOL fgDetect)
{
UINT8 ucData;

    if (fgDetect)
    {
        // Restart Descrambler, always detect
        DVBC_GetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
        mcCLR_BIT(ucData, 4);
        DVBC_SetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
    }
    else
    {
        // lock one time
        DVBC_GetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
        mcSET_BIT(ucData, 4);
        DVBC_SetReg(psDemodCtx, DVBC_REG_DSCRM_00, &ucData, 1);
    }
}

void DVBC_SetPGAGainIndex(DEMOD_CTX_T *psDemodCtx, UCHAR ucPGAGainIndex)
{
    UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_SET_PGAGAININDEX;
    mccSET_QUEUE2(aucData, ccOFST_FAST_SET_PGAGAININDEX) = ucPGAGainIndex;   
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_FAST_SET_PGAGAININDEX_TSIZE + 1);//lei121119
}

// AGC crrve setting

void DVBC_SetIfAgcCurve(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR   ucPara[ccCMD_REG_NUM];
	UCHAR   ii;
	SPECIFIC_MEMBER_EU_CTX* pEuTunerCtx = &(psDemodCtx->psTunerCtx->specific_member.eu_ctx);

	ucPara[0] = ccCMD_ID_DVBC_SET_TUNER_PARA0;
	for (ii = 0; ii < 4; ii++)
		ucPara[ii+1] = pEuTunerCtx->m_aucPara[ii+2];  // write m_aucPara[2-5]
	vSetIntfPara(psDemodCtx, ucPara, ccOFST_DVBC_SET_TUNER_PARA0_SIZE+1);
	
}

UCHAR DVBC_GetTsFormat(DEMOD_CTX_T *psDemodCtx)
{
	UCHAR ucData = 0;
	UCHAR ucTemp = 0;
	//DVBC_GetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
	DVBC_GetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucData, 1);
	mcSHOW_DBG_MSG(("DVBC_GetTsFormat: read 0xC38 = 0x%02X\n", ucData));
	ucTemp = ucData;
	
	if((ucData & 0x01) == 0x01) ucTemp = ucTemp|0x02;
	else ucTemp = ucTemp & 0xFD;
	
	if((ucData & 0x02) == 0x02) ucTemp = ucTemp|0x01;
	else ucTemp = ucTemp & 0xFE;

	if((ucData & 0x80) == 0x80) ucTemp = ucTemp|0x40;
	else ucTemp = ucTemp & 0xBF;
	
	mcSHOW_DBG_MSG(("DVBC TsFormat word = 0x%02X\n", ucTemp));
    return ucTemp;
}

VOID DVBC_SetTsFormat(DEMOD_CTX_T *psDemodCtx, UCHAR u1TsFmt)
{

	UCHAR ucData = 0;
	UCHAR ucTemp = 0;
	ucData = u1TsFmt;
	ucTemp = ucData;
	mcSHOW_DBG_MSG(("DVBC TsFormat word = 0x%02X\n", u1TsFmt));
	
	if((ucData & 0x01) == 0x01) ucTemp = ucTemp|0x02;
	else ucTemp = ucTemp & 0xFD;
	
	if((ucData & 0x02) == 0x02) ucTemp = ucTemp|0x01;
	else ucTemp = ucTemp & 0xFE;

	if((ucData & 0x40) == 0x40) ucTemp = ucTemp|0x80;
	else ucTemp = ucTemp & 0x7F;

	//DVBC_SetDemodReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucTemp, 1);
	DVBC_SetReg(psDemodCtx, DVBC_REG_TS_Parallel_Serial_Cfg, &ucTemp, 1);
	mcSHOW_DBG_MSG(("DVBC_SetTsFormat: write 0xC38 = 0x%02X\n", ucTemp));
	
}

//lei_code_dB120905
void DVBC_SetTsClkFreq(DEMOD_CTX_T *psDemodCtx, UCHAR ucManual)
{
#if defined(CC_MT5890)||defined(CC_MT5882)
UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBC_TS_CLKFREQ;
    mccSET_QUEUE2(aucData, ccOFST_DVBC_TS_CLKFREQ_MANUAL) = ucManual;
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBC_TS_CLKFREQ_TSIZE + 1);//lei121119
#endif
}


//XJ140523_Add Set ADC R-setting by Tuner
void DVBC_Set_R_Setting(DEMOD_CTX_T *psDemodCtx)
{
	#if defined(CC_MT5890)
	UINT32 u4Temp;
	UINT32 u4TempRSetting;
	if(ITUNER_OK == ITuner_OP(ITunerGetCtx(), itSetRSetting, MOD_DVBC, &u4TempRSetting))
	mcSHOW_DBG_MSG(("[XJ]DVBC set RSetting by tuner is %d\n", u4TempRSetting));
	u4TempRSetting = (u4TempRSetting << 16);
	u4Temp = mcRISC_Reg(cRISC_ACD_BASE, 0x418);
	u4Temp &= 0xFFF8FFFF;
	u4Temp |= u4TempRSetting;	
	mcSET_SHOW_REG(cRISC_ACD_BASE, 0x418, u4Temp);//RSetting:bit[18:16]
	#endif
}

void DVBC_SetTsOutputDelay(DEMOD_CTX_T *psDemodCtx, UINT16 ucDelay)
{
UCHAR   aucData[cMAX_READ_NUM + 1];

    mccSET_CMD2(aucData) = ccCMD_ID_DVBTC_TS_OUTPUT_DELAY;
    mccSET_QUEUE2(aucData, ccOFST_DVBTC_TS_OUTPUT_DELAY_Loop0) = mcLOW_BYTE(ucDelay);
	mccSET_QUEUE2(aucData, ccOFST_DVBTC_TS_OUTPUT_DELAY_Loop1) = mcHIGH_BYTE(ucDelay);
    mcHOST_CMD(psDemodCtx, aucData, 0, ccOFST_DVBTC_TS_OUTPUT_DELAY_TSIZE + 1);

}


void DVBC_SetEEPromBrdcstType(DEMOD_CTX_T *psDemodCtx)
{   
    #if defined(CC_MT5136)
	UINT8					u1Val = 0 ;
	UINT32					u4Ofst = 0;

	u4Ofst = DRVCUST_OptGet(eBrdcstSrcOfst0);
	u1Val = cDEMOD_DVBC_MODE;
	VERIFY(0 == EEPROM_Write((UINT64)u4Ofst, (UINT32)(void *)&u1Val, 1));
	mcSHOW_USER_MSG(("[MS debug]DVBC Set brdcst source  0x%0x = 0x%0x!\n",u4Ofst, u1Val));
	#endif

}

UINT8 DVBC_GetEEPromBrdcstType(DEMOD_CTX_T *psDemodCtx)
{   
	UINT8					u1Val = 0 ;

	#if defined(CC_MT5136)
	UINT32					u4Ofst = 0;
	u4Ofst = DRVCUST_OptGet(eBrdcstSrcOfst0);
	VERIFY(0 == EEPROM_Read((UINT64)u4Ofst, (UINT32)(void *)&u1Val, 1));
	mcSHOW_USER_MSG(("[MS debug]DVBC Get brdcst source 0x%0x = 0x%0x!\n",u4Ofst, u1Val));
	#endif
	return u1Val;
}

#ifdef CC_SUPPORT_STR
VOID DVBC_CloseClk(DEMOD_CTX_T *psDemodCtx)
{
	UINT32 u4Temp;
	mcSHOW_DBG_MSG(("DVBC_CloseClkSet\n"));

	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x11c);
	u4Temp &= 0xFFFFFBFF;
	mcSET_SHOW_REG(CKGEN_BASE, 0x11C, u4Temp);//close ATD clk
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x11c);
	u4Temp &= 0xFFFFF7FF;
	mcSET_SHOW_REG(CKGEN_BASE, 0x11C, u4Temp);//close DTD clk
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x302);
	u4Temp |= 0x00000001;
	mcSET_SHOW_REG(CKGEN_BASE, 0x302, u4Temp);//close DVBT clk

}

VOID DVBC_OpenClk(DEMOD_CTX_T *psDemodCtx)
{
	UINT32 u4Temp;
	mcSHOW_DBG_MSG(("DVBC_Re-OpenClkSet\n"));

	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x11c);
	u4Temp |= 0x00000400;
	mcSET_SHOW_REG(CKGEN_BASE, 0x11C, u4Temp);//Open ATD clk
	u4Temp = mcRISC_Reg(CKGEN_BASE, 0x11c);
	u4Temp |= 0x00000800;
	mcSET_SHOW_REG(CKGEN_BASE, 0x11C, u4Temp);//Open DTD clk
	u4Temp = mcRISC_Reg(cRISC_DVBC_BASE, 0x302);
	u4Temp &= 0xFFFFFFFE;
	mcSET_SHOW_REG(CKGEN_BASE, 0x302, u4Temp);//Open DVBT clk

}


#endif
//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetTCSwitch
 *  GetTCSwitch result.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : True.
 *  @retval   FALSE : False.   
 */
//-----------------------------------------------------------------------------
BOOL DVBC_GetTCSwitch(DEMOD_CTX_T *psDemodCtx)
{
	return fgTCSwitch;
}

//----------------------------------------------------------------------------- 
/*
 *  DVBC_SetTCSwitch
 *  SetTCSwitch result.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : True.
 *  @retval   FALSE : False.   
 */
//-----------------------------------------------------------------------------
void DVBC_SetTCSwitch(DEMOD_CTX_T *psDemodCtx, BOOL fgTCSwitchFlag)
{
	fgTCSwitch = fgTCSwitchFlag;

}


