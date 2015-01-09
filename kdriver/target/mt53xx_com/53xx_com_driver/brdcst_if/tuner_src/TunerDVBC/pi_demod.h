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
/************************************************************************* 
* 
* Filename: 
* --------- 
* 
* Description: 
* ------------ 
*    
* Author: 
* ------- 
*    JC Wu
* 
*************************************************************************/

#ifndef _MT5135C_PI_DEMOD_H_
#define _MT5135C_PI_DEMOD_H_

//#include "Demod.h"

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "ctx_driver.h"
#include "pi_dvbc_if.h"
#include "x_tuner.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define fcSHOW_HOST_CMD         (0 && fcINTERNAL_TEST)
#define MT5391_REG_TSIF_CTRL    (UINT16) 0x192
#define MT5391_REG_VARYING_AUTO_AR_ON (UINT16) 0x507
#define MT5391_REG_DVBT_PDN     (UINT16) 0x302
#define MT5391_REG_CORE_CONTROL (UINT16) 0x143



#define cMAX_READ_NUM           ccCMD_REG_NUM
#define ERR_BITS_PER_ERR_PKT    27

#define cPER_PRE_SCALE          10
#define cPER_SCALE              10000L
#define cPER_MAX                ((UINT16)(0.5 * cPER_SCALE * cPER_PRE_SCALE))

#define DVBC_TUNER_MODE_BBIQ           0x01
#define DVBC_TUNER_MODE_SELF_RFAGC     0x02
#define DVBC_TUNER_MODE_RFAGC_LNA      0x04
#define DVBC_TUNER_MODE_RFAGC_I2C_AGC  0x08

#define cCONN_TYPE_DVBC         0x01
#define cCONN_TYPE_DVBT         0x02

#define fcSHORT_LINUX_INIT      0

enum
{
    DVBC_BW_6MHz = 0,
    DVBC_BW_7MHz,
    DVBC_BW_8MHz,
    DVBC_BW_5MHz
};

//lei_code_110512;
enum
{
    cUP_LOAD_OK = 0,
    cUP_LOAD_ERR_I2C,
    cUP_LOAD_ERR_HW_RDY,
    cUP_LOAD_ERR_ROM_CHKSUM_RDY,
    cUP_LOAD_ERR_ROM_CHKSUM_OK,
    cUP_LOAD_ERR_SW_CHKSUM_OK,
    cUP_LOAD_ERR_CHKSUM_RDY,
    cUP_LOAD_ERR_CHKSUM_OK,
    cUP_LOAD_ERR_CNT
};

enum
{
	REACQ_NONE = 0,
	REACQ_AUTO
};

enum
{
	FAST_SYNC_NONE,
	FAST_SYNC_OLD,
	FAST_SYNC_AUTO,
	FAST_SYNC_MANUAL
};

//#define cIF_TARGET_LEVEL_0CCC      0
//#define cIF_TARGET_LEVEL_0A97      1

#define cMSG_UP_ERR_I2C          "I2c error!"
#define cMSG_UP_ERR_HW_RDY       "HW not ready!"
#define cMSG_UP_ERR_SW_CHKSUM_OK "SW Chksum not Ok!"
#define cMSG_UP_ERR_CHKSUM_RDY   "Chksum not ready!"
#define cMSG_UP_ERR_CHKSUM_OK    "Chksum not Ok!"

#define cMAX_DIVERSITY_NUM      8 

/*********** Register Addr. *************************************/
#define DVBC_REG_uP_Ctrl      (UINT16) 0x00
#define DVBC_REG_uP_IRA_H     (UINT16) 0x24
#define DVBC_REG_uP_IRA_L     (UINT16) 0x25
#define DVBC_REG_uP_IRData    (UINT16) 0x28
#define DVBC_REG_uP_CMDR_CFG  (UINT16) 0x07
#define DVBC_REG_uP_GP_REG00  (UINT16) 0x08
#define DVBC_REG_uP_GP_REG01  (UINT16) 0x09
#define DVBC_REG_uP_GP_REG02  (UINT16) 0x0A
#define DVBC_REG_uP_GP_REG03  (UINT16) 0x0B
#define DVBC_REG_uP_GP_REG04  (UINT16) 0x0C
#define DVBC_REG_uP_GP_REG05  (UINT16) 0x0D
#define DVBC_REG_uP_GP_REG06  (UINT16) 0x0E
#define DVBC_REG_uP_GP_REG07  (UINT16) 0x0F

#define DVBC_REG_PGA_CTRL_1   (UINT16) 0x1B6


#define DVBC_REG_RFA4A0       (UINT16) 0x4A0
#define DVBC_REG_RISCIF_CFG   (UINT16) 0x4A4
#define DVBC_REG_RISCIF_WDATA (UINT16) 0x4B0
#define DVBC_REG_RISCIF_CTRL  (UINT16) 0x4B4
#define DVBC_REG_RISCIF_RDATA (UINT16) 0x4B8
#define DVBC_REG_RISCIF_STAT  (UINT16) 0x4BC

#define DVBC_REG_RSICIF_CMD0  (UINT16) 0x4C0
#define DVBC_REG_RSICIF_CMD1  (UINT16) 0x4C1
#define DVBC_REG_RSICIF_CMD2  (UINT16) 0x4C2
#define DVBC_REG_RSICIF_CMD3  (UINT16) 0x4C3
#define DVBC_REG_RSICIF_CMD4  (UINT16) 0x4C4
#define DVBC_REG_RSICIF_CMD5  (UINT16) 0x4C5
#define DVBC_REG_RSICIF_CMD6  (UINT16) 0x4C6
#define DVBC_REG_RSICIF_CMD7  (UINT16) 0x4C7
#define DVBC_REG_RSICIF_RSP0  (UINT16) 0x4C8
#define DVBC_REG_RSICIF_RSP1  (UINT16) 0x4C9
#define DVBC_REG_RSICIF_RSP2  (UINT16) 0x4CA
#define DVBC_REG_RSICIF_RSP3  (UINT16) 0x4CB
#define DVBC_REG_RSICIF_RSP4  (UINT16) 0x4CC
#define DVBC_REG_RSICIF_RSP5  (UINT16) 0x4CD
#define DVBC_REG_RSICIF_RSP6  (UINT16) 0x4CE
#define DVBC_REG_RSICIF_RSP7  (UINT16) 0x4CF
#define DVBC_REG_uP_PNGP_H    (UINT16) 0x2C
#define DVBC_REG_uP_PNGP_L    (UINT16) 0x2D
#define DVBC_REG_uP_LPA_H     (UINT16) 0x2E
#define DVBC_REG_uP_LPA_L     (UINT16) 0x2F
#define DVBC_REG_I2C_Cfg      (UINT16) 0x44
#define DVBC_REG_I2C_Deglitch (UINT16) 0x45
#define DVBC_REG_I2C_Pullup_Del (UINT16) 0x46
#define DVBC_REG_I2C_IF_Cfg   (UINT16) 0x47
#define DVBC_REG_DSCRM_00    (UINT16) 0xc35
#define DVBC_REG_TS_Parallel_Serial_Cfg (UINT16)0xC38

/******************                   DVBC                   *********************/
#define DEFAULT_I2C_ADDR        0x82

#define DVBT_REG_Chip_ID   (UINT16) 0xE08
#define DVBC_REG_Chip_ID   (UINT16) 0xE09
#define MT5135_REG_Chip_ID   (UINT16) 0xE0A

//5136
//PAD
#define PADMUX_MIX_RD       0x8FB   //bit 5'1 LQFP128, bit 4'1 QFN48, 
#define PAD_PVR_TS_DATA1    0x806
#define PAD_PVR_TS_DATA0    0x808
#define PAD_PVR_TS_SYNC     0x80A
#define PAD_PVR_TS_VAL      0x80C
#define PAD_PVR_TS_CLK      0x80E
#define PAD_EXT_TS_CLK      0x828

#define PAD_EXT_TS_SYNC     0x82A
#define PAD_EXT_TS_VAL      0x82C
#define PAD_EXT_TS_DATA0    0x82E
#define PAD_EXT_TS_DATA1    0x830
#define PAD_EXT_TS_DATA2    0x832
#define PAD_EXT_TS_DATA3    0x834
#define PAD_EXT_TS_DATA4    0x836
#define PAD_EXT_TS_DATA5    0x838
#define PAD_EXT_TS_DATA6    0x83A
#define PAD_EXT_TS_DATA7    0x83C


#define PAD_SPI_CLE         0x800        
#define PAD_SPI_DATA        0x802                
#define PAD_SPI_CLK         0x804                       
#define PAD_CI_TS_DATA0     0x810                      
#define PAD_CI_TS_SYNC      0x812                        
#define PAD_CI_TS_VAL       0x814                        
#define PAD_CI_TS_CLK       0x816                     
#define PAD_DEMOD_TS_DATA0  0x818                      
#define PAD_DEMOD_TS_SYNC   0x81A                        
#define PAD_DEMOD_TS_VAL    0x81C                       
#define PAD_DEMOD_TS_CLK    0x81E                        
#define PAD_TUNER_DATA      0x820                       
#define PAD_TUNER_CLK       0x822                       
#define PAD_IF_AGC          0x824                        
#define PAD_RF_AGC          0x826                      
#define PAD_CD1_            0x844                        
#define PAD_D3              0x846                       
#define PAD_D11             0x848                      
#define PAD_D4              0x84A                      
#define PAD_D14             0x854                     
#define PAD_D7              0x856                        
#define PAD_D15             0x858                      
#define PAD_CE1_            0x85A                     
#define PAD_A10             0x85C                        
#define PAD_VS1_            0x85E
#define PAD_OE_             0x860     
#define PAD_IORD_           0x862     
#define PAD_A11             0x864
#define PAD_IOWR_           0x866        
#define PAD_A9              0x868        
#define PAD_A17             0x86A       
#define PAD_A8              0x86C        
#define PAD_A18             0x86E       
#define PAD_READY           0x87C        
#define PAD_A16             0x880        
#define PAD_A23             0x882       
#define PAD_A15             0x884        
#define PAD_A24             0x886        
#define PAD_A12             0x888        
#define PAD_A25             0x88A        
#define PAD_A7              0x88C        
#define PAD_VS2_            0x88E        
#define PAD_A6              0x890       
#define PAD_PCMCIA_RESET    0x892        
#define PAD_A5              0x894        
#define PAD_WAIT_           0x896        
#define PAD_A4              0x898        
#define PAD_REG_            0x89C        
#define PAD_A2              0x89E        
#define PAD_BVD2            0x8A0
#define PAD_A1              0x8A2      
#define PAD_BVD1            0x8A4       
#define PAD_A0              0x8A6      
#define PAD_CD2_            0x8B4       
#define PAD_CI_INT          0x8B6       
        
//IFADC
#define REG_IFADC_CFG0      0x900
#define REG_IFADC_CFG1      0x901
#define REG_IFADC_CFG2      0x902
#define REG_IFADC_CFG3      0x903
#define REG_IFADC_CFG4      0x904
#define REG_IFADC_CFG5      0x905

//IFPGA
#define REG_IFPGA_CFG0      0x908
#define REG_IFPGA_CFG1      0x909
#define REG_IFPGA_CFG2      0x90A
#define REG_IFPGA_CFG3      0x90B
#define REG_IFPGA_CFG4      0x90C
#define REG_IFPGA_CFG5      0x90D



// IFPLL settings	
#define REG_IFPLL_CFG0      0x910
#define REG_IFPLL_CFG1      0x911
#define REG_IFPLL_CFG2      0x912
#define REG_IFPLL_CFG3      0x913
#define REG_IFPLL_CFG4      0x914
#define REG_IFPLL_CFG5      0x915
#define REG_IFPLL_CFG6      0x916
#define REG_IFPLL_CFG7      0x917
#define REG_IFPLL_CFG8      0x918
#define REG_IFPLL_CFG9      0x919

// RGS IFPLL settings	
#define RGS_IFPLL_CFG0      0x91A
#define RGS_IFPLL_CFG1      0x91B

//IFTOP
#define REG_IFTOP_CFG0      0x91C
#define REG_IFTOP_CFG1      0x91D

//RSSIADC
#define REG_RSSIADC_CFG0    0x91E
#define REG_RSSIADC_CFG1    0x91F
#define REG_RSSIADC_CFG2    0x920

//SYSPLL
#define REG_PLL_CFG0        0x921
#define REG_PLL_CFG1        0x922
#define REG_PLL_CFG2        0x923
#define REG_PLL_CFG3        0x924
#define REG_PLL_CFG4        0x925
#define REG_PLL_CFG5        0x926
#define REG_PLL_CFG6        0x927


//CKGEN

#define REG_CKGEN_PLL0      0xE50
#define REG_CKGEN_54M_0     0xE51
#define REG_CKGEN_54M_1     0xE52
#define REG_CKGEN_86P4M_0   0xE55
#define REG_CKGEN_86P4M_1   0xE56
#define REG_CKGEN_36M_0     0xE57
#define REG_CKGEN_36M_1     0xE58

#define REG_CKGEN_ADCLK_DIV4_0     0xE6A
#define REG_CKGEN_ADCLK_DIV4_1     0xE6B
#define REG_CKGEN_ADCLK_K_0        0xE6C
#define REG_CKGEN_ADCLK_K_1        0xE6D

#define REG_CKGEN_162M_0           0xE70
#define REG_CKGEN_162M_1           0xE71



#define REG_AD_CKPHSEL             0xF80
#define REG_ANAIF_RESET            0xE01

#define REG_PADMUX_TS_DATA0_CFG    0x818
#define REG_PADMUX_TS_SYNC_CFG     0x81A
#define REG_PADMUX_TS_VALID_CFG    0x81C
#define REG_PADMUX_TS_CLK_CFG    0x81E



/***********************************************************************/
//#define TUNER_SOURCE_LABEL      ""
//#define cMAX_I2C_LEN            64

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define mcHOST_CMD(pDemd, Cmd, Rcnt, Wcnt)      fgHostCmdDVBC((DEMOD_CTX_T *) pDemd, Cmd, Rcnt, Wcnt)
#define mcCTMR_DBG_MSG(_x_)     mcSHOW_DBG_MSG2(_x_)
#define mcCTMR_DBG_MSG2(_x_)    mcSHOW_DBG_MSG2(_x_)

#define mcCHECK_BLOCK_I2C(ucRetSts)
#define mcCHECK_BLOCK_I2C0()
#define mcBLOCK_I2C(ucBlockSts)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT8  DVBC_SetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8  DVBC_GetReg(DEMOD_CTX_T *psDemodCtx, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
UINT8  ucDVBCI2cWriteSpeed(DEMOD_CTX_T *psDemodCtx, UINT8 *pu1Buffer, UINT16 u2CurRomLen);
BOOL   fgHostCmdDVBC(DEMOD_CTX_T *psDemodCtx, UCHAR *aucCmdData, UCHAR ucReadParacnt, UCHAR ucWriteCmdcnt);
BOOL   fgDVBCGetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt);
UINT8  ucDVBCGetFwVar(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr);
BOOL   fgDVBCSetFwVars(DEMOD_CTX_T *psDemodCtx, UINT16 u2Addr, UCHAR *pucData, UCHAR ucCnt);
UINT8  DVBC_SetArCoef(DEMOD_CTX_T *psDemodCtx, UINT8 ucData);
//INT32  s4LoadCode(DEMOD_CTX_T *psDemodCtx, UINT8 *u1CodeImage,
//                  UINT16 u2CurRomLen, UINT16 u2MaxI2cLen, BOOL fgClk50);

UINT8 DVBC_ChipInit(DEMOD_CTX_T *psDemodCtx);
BOOL  DVBC_FwSwitch(DEMOD_CTX_T *psDemodCtx);
VOID  DVBC_TSParallelSerialSwitch(DEMOD_CTX_T *psDemodCtx, BOOL IsSerial);
UINT8 DVBC_SetNormalMode(DEMOD_CTX_T *psDemodCtx);
UINT8 DVBC_Connect(DEMOD_CTX_T *psDemodCtx, UINT8 Mode, UINT16 Rate);
VOID  DVBC_DisConnect(DEMOD_CTX_T *psDemodCtx);
VOID  DVBC_WO_DisConnect(DEMOD_CTX_T *psDemodCtx);
INT32  DVBC_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgLoadCode);
INT32  DVBC_GetPER(DEMOD_CTX_T *psDemodCtx);
UINT32 DVBC_GetTotPkt(DEMOD_CTX_T *psDemodCtx);
UINT32 DVBC_GetErrPkt(DEMOD_CTX_T *psDemodCtx);
BOOL   DVBC_GetTsLock(DEMOD_CTX_T *psDemodCtx);
void   DVBC_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate);
INT16  DVBC_TunerI2c(DEMOD_CTX_T *psDemodCtx, BOOL fgSwitchOn);
void   DVBC_DriverStart(DEMOD_CTX_T *psDemodCtx);
void   DVBC_DriverStop(DEMOD_CTX_T *psDemodCtx);
void   DVBC_SetConnType(DEMOD_CTX_T *psDemodCtx, UINT8 type);
UCHAR  DVBC_SetPowerMode(DEMOD_CTX_T *psDemodCtx, UCHAR ucMode);
INT32  DVBC_GetPostVBer(DEMOD_CTX_T *psDemodCtx);
INT32 DVBC_GetPostVBer_E7(DEMOD_CTX_T *psDemodCtx);
UINT8  DVBC_DumpRegisters(DEMOD_CTX_T *psDemodCtx, UINT8 *buffer, UINT8 bank_no, UINT16 max_i2c_len);
void DVBC_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel);
void DVBC_SetQTREarlyONOFF(DEMOD_CTX_T *psDemodCtx, BOOL fgQTREarly);
void DVBC_SetTsPathMode(DEMOD_CTX_T *psDemodCtx, BOOL fgEnable);
UINT8 DVBC_GetSync(DEMOD_CTX_T *psDemodCtx);
UINT8 DVBC_GetArCoef(DEMOD_CTX_T *psDemodCtx, UINT8 *pucData);


void DVBC_SetQamMode(DEMOD_CTX_T *psDemodCtx, UINT8 Mode);
void DVBC_SetSymbolRate(DEMOD_CTX_T *psDemodCtx, UINT16 Rate);
void DVBC_AcqStart(DEMOD_CTX_T *psDemodCtx);
void DVBC_UpdateStatus(DEMOD_CTX_T *psDemodCtx);
void DVBC_UpdateInfo(DEMOD_CTX_T *psDemodCtx);
INT32 DVBC_GetCFO(DEMOD_CTX_T *psDemodCtx);
INT32 DVBC_GetTFO(DEMOD_CTX_T *psDemodCtx);
TS_FMT_T DVBC_GetTsFmt(DEMOD_CTX_T *psDemodCtx);
UINT8 DVBC_GetSQI(DEMOD_CTX_T *psDemodCtx);
BOOL  DVBC_CheckLocked(DEMOD_CTX_T *psDemodCtx);
VOID  DVBC_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable);
void DVBC_SetPGAGainIndex(DEMOD_CTX_T *psDemodCtx, UCHAR ucPGAGainIndex);
void DVBC_SetIfAgcCurve(DEMOD_CTX_T *psDemodCtx);
UCHAR DVBC_GetTsFormat(DEMOD_CTX_T *psDemodCtx);
VOID DVBC_SetTsFormat(DEMOD_CTX_T *psDemodCtx, UCHAR u1TsFmt);
void DVBC_SetTsClkFreq(DEMOD_CTX_T *psDemodCtx, UCHAR ucManual);
void DVBC_Set_R_Setting(DEMOD_CTX_T *psDemodCtx);

void DVBC_SetTsOutputDelay(DEMOD_CTX_T *psDemodCtx, UINT16 ucDelay);




CHAR *DVBC_GetVer(DEMOD_CTX_T *psDemodCtx);
VOID vDVBC_GetAsicVer(DEMOD_CTX_T *psDemodCtx);
void DVBC_AdvAQM(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvAQM);
void DVBC_AdvASR(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvASR);
VOID DVBC_SetDescramblerLock(DEMOD_CTX_T *psDemodCtx, BOOL fgDetect);

#ifdef CC_SUPPORT_STR
VOID DVBC_CloseClk(DEMOD_CTX_T *psDemodCtx);
VOID DVBC_OpenClk(DEMOD_CTX_T *psDemodCtx);
#endif

/**********************************************************************/
/*      I2C Interface                                                 */
/**********************************************************************/
/*UINT8 ucI2cRead(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);
//UINT8 ucI2cWrite(UINT8 ucI2cAddr, UINT8 ucRegAddr, UINT8 *pucBuffer, UINT16 ucByteCount);

//UINT8 u1I2cDemWrite(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
//UINT8 u1I2cDemRead(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddr, UINT16 u2RegAddr, UINT8 *pu1Buffer, UINT16 u2ByteCount);
//#define ucI2cWriteSpeed(psDemodCtx, u4I2cClkDiv, ucI2cAddr, ucRegAddr, pucBuffer, ucByteCount) 
//        u1I2cDemWrite(psDemodCtx, ucI2cAddr, ucRegAddr, pucBuffer, ucByteCount)*/

DEMOD_CTX_T *DVBC_DemodCtxCreate(void);
void DVBC_DemodCtxDestroy(DEMOD_CTX_T *p);

#endif // _MT5135C_PI_DEMOD_H_
