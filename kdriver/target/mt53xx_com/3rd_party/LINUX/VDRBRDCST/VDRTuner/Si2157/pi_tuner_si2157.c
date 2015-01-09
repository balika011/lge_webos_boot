/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: pi_tuner_SI2157.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
/* Silabs version 2.0 build 4 for Si2157-A20  8/20/2012 */
// changes tagged with Silabs_120820
/** @file pi_tuner_Si2157.c
 *  Tuner control for SS SI2157
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "tuner_interface_if.h"
#include "fe_tuner_common_if.h"
#include "x_typedef.h"
#include "tunerDebug.h"
#include "x_os.h"
#include "vdr_PD_Def.h"
#include "vdr_pi_def_dvbt.h"
#include "eq_script_si2157.h"
#include "x_hal_5381.h"

/*please add Si2157 head file here*/
#include "Si2157_L1_API.h"
#include "Si2157_firmware_3_0_build_5.h"
#include "Si2157_i2c_api.h"

#define Si2157_BYTES_PER_LINE 8






#if 0
 enum
    {
        SAW_BW_6M = 0,
        SAW_BW_7M,
        SAW_BW_8M,
        SAW_BW_5M
    };

enum
{
    cIF_TARGET_LEVEL_0CCC = 0,
    cIF_TARGET_LEVEL_0A97,
    cIF_TARGET_LEVEL_09CC,
    cIF_TARGET_LEVEL_07CC,
    cIF_TARGET_LEVEL_0CCC_0D36
};

enum
{
    DVBT_CR_12 = 0,
    DVBT_CR_23,
    DVBT_CR_34,
    DVBT_CR_56,
    DVBT_CR_78,
    DVBT_CR_Mode_Size
};
#endif
#define DAGC_TARGET_LEVEL_H_INDEX 0
#define DAGC_TARGET_LEVEL_L_INDEX 1

#define SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL 1//for SI2157 ATV AGC source selection, 0 for internal, 1 for external
#define SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL 1//for SI2157 DTV AGC source selection, 0 for internal, 1 for external
#define AD_R_setting 1      // 1:have IF attenuator, 0:No IF attenuator


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//Frequency boundary .Get from tuner spec
#define C_Si2157_FREQ_DBOUND_UPPER   859*1000*1000
#define C_Si2157_FREQ_DBOUND_LOWER    48*1000*1000
#define C_Si2157_FREQ_DBOUND_UPPER_Ana     875*1000*1000
#define C_Si2157_FREQ_DBOUND_LOWER_Ana    45*1000*1000

//PLL lock check parameters
#define C_Si2157_POLL_INTERVAL      5 //ms
#define C_Si2157_PLL_POLL_TIMETOUT      100 //ms
#define C_Si2157_PLL_POLL_CNT           C_Si2157_PLL_POLL_TIMETOUT/C_Si2157_POLL_INTERVAL //counter

#define C_Si2157_PF_TO_CF_SHIFT_BG_7M 2250 /*KHZ*/  //PF to CF is 2250 when 7M B
#define C_Si2157_PF_TO_CF_SHIFT_BG_8M 2750 /*KHZ*/  //PF to CF is 2750 when 8M G
#define C_Si2157_PF_TO_CF_SHIFT_DK 2750 /*KHZ*/
#define C_Si2157_PF_TO_CF_SHIFT_I  2750 /*KHZ*/
#define C_Si2157_PF_TO_CF_SHIFT_L  2750 /*KHZ*/
#define C_Si2157_PF_TO_CF_SHIFT_M  1750 /*KHZ*/
#define C_Si2157_PF_TO_CF_SHIFT_L1 2750 /*KHZ*/
#define C_Si2157_IF_CENTER_BG 5000 /*KHZ*/
#define C_Si2157_IF_CENTER_DK 5000 /*KHZ*/
#define C_Si2157_IF_CENTER_I  5000 /*KHZ*/
#define C_Si2157_IF_CENTER_L  5000 /*KHZ*/
#define C_Si2157_IF_CENTER_L1 5000 /*KHZ*/
#define C_Si2157_IF_CENTER_M  5000 /*KHZ*/

#define C_Si2157_IF_FREQUENCY          ((UINT16)  5000)  /* kHz */
#define C_Si2157_LO_ADDRESS           ((UINT8)  0xC0)

#define C_Si2157_LO_DIVIDER_STEP       ((UINT32) 1000)  /* Hz */
#define C_Si2157_LO_DIVIDER_STEP_ANA   ((UINT16)  1000)  /* Hz */
#define C_SILABS_Si2157_STR            "SILABS_Si2157_FW_3_0b5\n 2013-05-16\n"

#define C_Si2157_TOP_SET             ((UINT8)   0x02)
#define C_Si2157_TOP_SET_DVBC        ((UINT8)   0x04)
#define C_Si2157_TOP_SET_ANA         ((UINT8)   0x0A)
#define C_Si2157_SAW_BW              SAW_BW_8M

#if SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL
#define C_Si2157_AGC_IF_SLP           0.19
#define C_Si2157_AGC_IF_INT           -0.24
#define C_Si2157_AGC_IF_MAX           0.52
#define C_Si2157_AGC_IF_MIN           -1
#else
#define C_Si2157_AGC_IF_SLP           0.125
#define C_Si2157_AGC_IF_INT           0.5
#define C_Si2157_AGC_IF_MAX           0.499  // 0.5 will overflow  //0v
#define C_Si2157_AGC_IF_MIN          0.0
#endif
#define C_Si2157_AGC_IF_SLP_SGN  (INT8)((C_Si2157_AGC_IF_SLP>0)?1:-1)
#define C_Si2157_AGC_IF_LVL_MAX      0.4774    //-90dBm    127/256
#define C_Si2157_AGC_IF_LVL_MIN      0.08203125   //-60dBm  21/256

#define cALC_ADC_INPUT_SWING             2  //Volt
#define cALC_ADC_COMM_VOLT               1.2
#define cALC_ADC_BIAS                    (cALC_ADC_COMM_VOLT - cALC_ADC_INPUT_SWING/2)

#define C_Si2157_MAX_IF_GAIN_SET       -0.12
#define C_Si2157_MIN_IF_GAIN_SET       -0.38
#define C_Si2157_MAX_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_Si2157_MIN_RF_GAIN_SET        cALC_ADC_BIAS *2   // Volt
#define C_Si2157_MAX_IF_GAIN_POW       -100    // dBm
#define C_Si2157_MIN_IF_GAIN_POW        -50     // dBm
#define C_Si2157_MAX_RF_GAIN_POW         C_Si2157_MIN_IF_GAIN_POW
#define C_Si2157_MIN_RF_GAIN_POW        -20     // dBm
#define C_Si2157_POWER_CALIBRATE        26     // dBm

#define C_Si2157_AGC_COEF            ((UINT8)   0xBD)
#define C_Si2157_DEMOD_INPUT_POWER    cIF_TARGET_LEVEL_0CCC_0D36

// ********************************************* //

#define C_U8_Si2157_AGC_IF_SLP         (INT8)(C_Si2157_AGC_IF_SLP *64 )  // MT5135(S(1, -6)), -0.125*64 = -8 (0xF8), -8/64 = -1/8
#define C_U8_Si2157_AGC_IF_INT         (INT8)(C_Si2157_AGC_IF_INT *16 ) // MT5135(S(3, -4))  0.5*16 = 8, 8/16 =0.5
#define C_U8_Si2157_AGC_IF_MAX         (INT8)(C_Si2157_AGC_IF_MAX *128)    //0.499*256 = 0x7F
#define C_U8_Si2157_AGC_IF_MIN         (INT8)(C_Si2157_AGC_IF_MIN *128)    //0
#define C_U8_Si2157_AGC_IF_LVL_MAX        (INT8)(C_Si2157_AGC_IF_LVL_MAX *256)
#define C_U8_Si2157_AGC_IF_LVL_MIN         (INT8)(C_Si2157_AGC_IF_LVL_MIN *256)

#define C_U8_Si2157_MAX_IF_GAIN_SET    (INT8)(C_Si2157_MAX_IF_GAIN_SET *256)
#define C_U8_Si2157_MIN_IF_GAIN_SET    (INT8)(C_Si2157_MIN_IF_GAIN_SET *256)
#define C_U8_Si2157_MAX_RF_GAIN_SET    (INT8)((C_Si2157_MAX_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_Si2157_MIN_RF_GAIN_SET    (INT8)((C_Si2157_MIN_RF_GAIN_SET /2 -cALC_ADC_BIAS) /cALC_ADC_INPUT_SWING *256)
#define C_U8_Si2157_IF_GAIN_RANGE      (INT8)(C_Si2157_MIN_IF_GAIN_POW - C_Si2157_MAX_IF_GAIN_POW)
#define C_U8_Si2157_RF_GAIN_RANGE      (INT8)(C_Si2157_MIN_RF_GAIN_POW - C_Si2157_MAX_RF_GAIN_POW)

static UINT8* pSi2157[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};
static UINT8* pSi2157EqWeak[MOD_ANA_TYPE_END - MOD_ANA_TYPE_BEGIN] = {NULL};

static ATD_TUNER_REL_REG_T arrAtdRelReg[]={
#if !(defined(CC_MT5365) || defined(CC_MT5395))
    #if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    {0x794,0x2a},
    {0x7a4,0x2a},
    {0x7d4,0x04},
    {0x7d5,0xfb},
    {0x7d6,0xfb},
    {0x7d7,0xEC},
    {0x7d8,0xf5},
    {0x7d9,0xf5},
    {0x7da,0x7F},
    {0x7db,0x30},
    {0x7dc,0x80},
    {0x7dd,0x80},
    {0x7de,0x9f},
    {0x7df,0x11},
    {0x7b7,0x10},//Target level
    {0x7b9,0x10},//Target level
    {0x13b,0x80},
    {0x790,0x08},
    {0x1b4,0x17},   // Pilot estimator confg.
    {0x7e0,0x00}, //PGA Index
    #else
    {0x794,0x2a},
    {0x7a4,0x2a},
    {0x7d4,0x04},
    {0x7d5,0x00},
    {0x7d6,0x00},
    {0x7d7,0xEC},
    {0x7d8,0xEF},
    {0x7d9,0xEF},
    {0x7da,0x7F},
    {0x7db,0x7F},
    {0x7dc,0x80},
    {0x7dd,0x80},
    {0x7de,0x80},
    {0x7df,0x00},
    {0x7b7,0x10},
    {0x7b9,0x0e},
    {0x13b,0x80},
    {0x790,0x08},
    {0x1b4,0x17},   // Pilot estimator confg.
    #endif

#else

    #if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    {0x294,0x38},
    {0x2a4,0x28},
    {0x2d4,0x04},
    {0x2d5,0xfb},
    {0x2d6,0xfb},
    {0x2d7,0xEC},
    {0x2d8,0xf5},
    {0x2d9,0xf5},
    {0x2da,0x7F},
    {0x2db,0x30},
    {0x2dc,0x80},
    {0x2dd,0x80},
    {0x2de,0x9f},
    {0x2df,0x11},
    {0x2b7,0x10},
    {0x2b9,0x0e},
    {0x13b,0x80},
    {0x290,0x08},
    #else
    {0x294,0x38},
    {0x2a4,0x28},
    {0x2d4,0x04},
    {0x2d5,0x00},
    {0x2d6,0x00},
    {0x2d7,0xEC},
    {0x2d8,0xEF},
    {0x2d9,0xEF},
    {0x2da,0x7F},
    {0x2db,0x7F},
    {0x2dc,0x80},
    {0x2dd,0x80},
    {0x2de,0x80},
    {0x2df,0x00},
    {0x2b7,0x10},
    {0x2b9,0x0e},
    {0x13b,0x80},
    {0x290,0x08},
    #endif

#endif
{0x00,0x00}
};
static ATD_TUNER_REL_REG_T arrAtdRelRegSecam[]={
#if !(defined(CC_MT5365) || defined(CC_MT5395))
    #if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    {0x7d4,0x04},
    {0x7d5,0xfb},
    {0x7d6,0xfb},
    {0x7d7,0xEC},
    {0x7d8,0xf5},
    {0x7d9,0xf5},
    {0x7da,0x7F},
    {0x7db,0x30},
    {0x7dc,0x80},
    {0x7dd,0x80},
    {0x7de,0x80},
    {0x7df,0x11},
    {0x7b7,0x05},
    {0x7b9,0x05},
    {0x13b,0x81},
    {0x790,0x88},
    {0x1b4,0x17},    // Pilot estimator confg.
    {0x7e0,0x00},//PGA Index
    #else
    {0x7d4,0x04},
    {0x7d5,0x00},
    {0x7d6,0x00},
    {0x7d7,0xEC},
    {0x7d8,0xEF},
    {0x7d9,0xEF},
    {0x7da,0x7F},
    {0x7db,0x7F},
    {0x7dc,0x80},
    {0x7dd,0x80},
    {0x7de,0x80},
    {0x7df,0x00},
    {0x7b7,0x05},
    {0x7b9,0x03},
    {0x13b,0x81}, /*wenming 11-11-24:bit 0 set 1 for positive modulation*/
    {0x790,0x88},
    {0x1b4,0x17},    // Pilot estimator confg.
    #endif

#else

#if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    {0x2d4,0x04},
    {0x2d5,0xfb},
    {0x2d6,0xfb},
    {0x2d7,0xEC},
    {0x2d8,0xf5},
    {0x2d9,0xf5},
    {0x2da,0x7F},
    {0x2db,0x30},
    {0x2dc,0x80},
    {0x2dd,0x80},
    {0x2de,0x9f},
    {0x2df,0x11},
    {0x2b7,0x05},
    {0x2b9,0x03},
    {0x13b,0x81}, /*wenming 11-11-24:bit 0 set 1 for positive modulation*/
    {0x290,0x88},
    #else
    {0x2d4,0x04},
    {0x2d5,0x00},
    {0x2d6,0x00},
    {0x2d7,0xEC},
    {0x2d8,0xEF},
    {0x2d9,0xEF},
    {0x2da,0x7F},
    {0x2db,0x7F},
    {0x2dc,0x80},
    {0x2dd,0x80},
    {0x2de,0x80},
    {0x2df,0x00},
    {0x2b7,0x05},
    {0x2b9,0x03},
    {0x13b,0x81}, /*wenming 11-11-24:bit 0 set 1 for positive modulation*/
    {0x290,0x88},
    #endif

#endif
{0x00,0x00}
};

static CN_TUNER_REL_REG_T arrCNDTMBRelReg[]=
{
    {0xfd5,0xfb},
    {0xfd6,0xfb},
    {0xfd8,0xf5},
    {0xfd9,0xf5},
    {0xfdb,0x30},
    {0xfdd,0x80},
    {0xfde,0x80},
    {0xfdf,0x11},
    {0xfe0,0x00},
    //{0x39d,0x3a},//cold power on setting only for dtmb , the setting different with tuner 
    {0x0,0x0}//end flag
 };
static CN_TUNER_REL_REG_T arrCNDVBCRelReg[]=
{


    {0xfd5,0xfb},
    {0xfd6,0xfb},
    {0xfd8,0xf5},
    {0xfd9,0xf5},
    {0xfdb,0x30},
    {0xfdd,0x80},
    {0xfde,0x80},
    {0xfdf,0x11},
    {0xfe0,0x00},
    //{0x39d,0x3a},//cold power on setting only for dtmb , the setting different with tuner 
    {0x0,0x0}//end flag
    
 };



#if SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL
static unsigned char DVBT_IF_AMP=55;//Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
static unsigned char DVBT2_IF_AMP=55;
static unsigned char DVBC_IF_AMP=60;//Si2156_DTV_LIF_OUT_PROP_AMP_DEFAULT;
static unsigned char ATSC_8VSB_IF_AMP = 70;
static unsigned char ATSC_QAM_IF_AMP = 70;
static unsigned char DTMB_IF_AMP = 60;

#else
static unsigned char DVBT_IF_AMP=21;//Si2157_DTV_LIF_OUT_PROP_AMP_DEFAULT;
static unsigned char DVBT2_IF_AMP=21;
static unsigned char DVBC_IF_AMP=26;//Si2157_DTV_LIF_OUT_PROP_AMP_DEFAULT;
static unsigned char ATSC_8VSB_IF_AMP = 21;
static unsigned char ATSC_QAM_IF_AMP = 21;
static unsigned char DTMB_IF_AMP = 27;

#endif

static unsigned char NO_L_ANALOG_IF_AMP=160;//Si2157_ATV_LIF_OUT_PROP_AMP_DEFAULT;  //100 -->1Vpp RMS
#if AD_R_setting
static unsigned char L_ANALOG_IF_AMP=120;//SECAML/L1
#else
static unsigned char L_ANALOG_IF_AMP=100;//SECAML/L1
#endif


//static unsigned char NO_L_DIGITAL_IF_AMP=Si2157_DTV_LIF_OUT_PROP_AMP_DEFAULT;
static BOOL TunerInited = FALSE;

#ifdef    Si2157_GET_PROPERTY_STRING
static unsigned int aui2Si2157Properties[] = {
    Si2157_ATV_AFC_RANGE_PROP_CODE,
    Si2157_ATV_AGC_SPEED_PROP_CODE,
    Si2157_ATV_AGC_SPEED_LOW_RSSI_PROP_CODE,
    Si2157_ATV_ARTIFICIAL_SNOW_PROP_CODE,
    Si2157_ATV_CONFIG_IF_PORT_PROP_CODE,
    Si2157_ATV_EXT_AGC_PROP_CODE,
    Si2157_ATV_HSYNC_OUT_PROP_CODE,
    Si2157_ATV_IEN_PROP_CODE,
    Si2157_ATV_INT_SENSE_PROP_CODE,
    Si2157_ATV_LIF_FREQ_PROP_CODE,
    Si2157_ATV_LIF_OUT_PROP_CODE,
    Si2157_ATV_PGA_TARGET_PROP_CODE,
    Si2157_ATV_RF_TOP_PROP_CODE,
    Si2157_ATV_RSQ_RSSI_THRESHOLD_PROP_CODE,
    Si2157_ATV_VIDEO_MODE_PROP_CODE,
    Si2157_ATV_VSNR_CAP_PROP_CODE,
    Si2157_CRYSTAL_TRIM_PROP_CODE,
    Si2157_DTV_AGC_FREEZE_INPUT_PROP_CODE,
    Si2157_DTV_AGC_SPEED_PROP_CODE,
    Si2157_DTV_CONFIG_IF_PORT_PROP_CODE,
    Si2157_DTV_EXT_AGC_PROP_CODE,
    Si2157_DTV_IEN_PROP_CODE,
    Si2157_DTV_INITIAL_AGC_SPEED_PROP_CODE,
    Si2157_DTV_INITIAL_AGC_SPEED_PERIOD_PROP_CODE,
    Si2157_DTV_INTERNAL_ZIF_PROP_CODE,
    Si2157_DTV_INT_SENSE_PROP_CODE,
    Si2157_DTV_LIF_FREQ_PROP_CODE,
    Si2157_DTV_LIF_OUT_PROP_CODE,
    Si2157_DTV_MODE_PROP_CODE,
    Si2157_DTV_PGA_LIMITS_PROP_CODE,
    Si2157_DTV_PGA_TARGET_PROP_CODE,
    Si2157_DTV_RF_TOP_PROP_CODE,
    Si2157_DTV_RSQ_RSSI_THRESHOLD_PROP_CODE,
    Si2157_DTV_ZIF_DC_CANCELLER_BW_PROP_CODE,
    Si2157_MASTER_IEN_PROP_CODE,
    Si2157_TUNER_BLOCKED_VCO_PROP_CODE,
    Si2157_TUNER_IEN_PROP_CODE,
    Si2157_TUNER_INT_SENSE_PROP_CODE,
    Si2157_TUNER_LO_INJECTION_PROP_CODE,
    Si2157_TUNER_RETURN_LOSS_PROP_CODE
    };
    static char acSi2157ProMsg[100];
#endif

//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------
//Signal Power Sample Array Size
//-----------------------------------------------------------------------------
// pi  constant definition
//-----------------------------------------------------------------------------
//-------------------------------------------------------------static  variable----------------------------------------------------------------------------------------------------------------------
L1_Si2157_Context api_context={0};
L1_Si2157_Context *api;
UINT8 VideoStandard;

typedef enum TV_MODES
{
    ANALOG_MODE,
    DIGITAL_MODE
} TV_MODES;

typedef enum TUNER_STATUSES
{
    CHL,
    PCL,
    DTV_CHL,
    RSSI,
} TUNER_STATUSES;

static INT8 Adjust_Si2157_i4Power_rec_val = 2;
static INT32 fgParaA_DVBC = 2;
static INT32 fgParaA_DVBT2 = 2;
/************************************************************************************************************************
  NAME: PCFreq_To_CenterFreq
  DESCRIPTION: Convert Picture Carrier frequency to Center Frequency
  Parameter:  System
  Parameter:  PC Frequency
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
UINT32 PCFreq_To_CenterFreq(UINT8 u1SubSysIdx,UINT32 pc,UINT8 fgAutoSearch)
{
    UINT32 cf=0;
    switch (u1SubSysIdx)
    {
        case MOD_PAL_BG:  /*wenming 11-12-2: BG PC to Center is by 7M or 8M BW*/
            {
                if(ATV_CONN_MODE_CHAN_SCAN == fgAutoSearch) /*wenming 12-4-16: fgAutoSearch get from MW, ignore driver fgAutoSearch setting*/
                {
                    cf = pc + C_Si2157_PF_TO_CF_SHIFT_BG_8M*1000;
                    mcDBG_MSG_L2(("TestLog:Now set 8M PC-CENTER in PC when scan mode\n"));
                }
                else
                {
                    if(pc<300000000)  //pal B system
                    {
                        cf = pc + C_Si2157_PF_TO_CF_SHIFT_BG_7M*1000;
                        mcDBG_MSG_L2(("TestLog:Now set 7M PC-CENTER in PC"));
                    }
                    else    //pal G system
                    {
                        cf = pc + C_Si2157_PF_TO_CF_SHIFT_BG_8M*1000;
                        mcDBG_MSG_L2(("TestLog:Now set 8M PC-CENTER in PC"));
                    }
                }
            }
            break;
        case MOD_PAL_DK:
            cf = pc + C_Si2157_PF_TO_CF_SHIFT_DK*1000;
            break;
        case MOD_PAL_I:
            cf = pc + C_Si2157_PF_TO_CF_SHIFT_I*1000;
            break;
        case MOD_NTSC_M:
            cf = pc + C_Si2157_PF_TO_CF_SHIFT_M*1000;
            break;
        case MOD_SECAM_L:
            cf = pc + C_Si2157_PF_TO_CF_SHIFT_L*1000;
            break;
        case MOD_SECAM_L1:
            cf = pc - C_Si2157_PF_TO_CF_SHIFT_L1*1000;
            break;
    }
    return cf;
}
/* _load_firmware_16_insertion_start */
 /************************************************************************************************************************
  NAME: Si2157_LoadFirmware_16
  DESCRIPTION: Load firmware from firmware_struct array in Si2157_Firmware_x_y_build_z.h file into Si2157
              Requires Si2157 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to firmware_struct array
  Parameter:  number of lines in firmware table array (size in bytes / firmware_struct)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadFirmware_16        (L1_Si2157_Context *api, firmware_struct fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2157_ERROR;

    mcDBG_MSG_L2 (("Si2157_LoadFirmware_16 starting...\n"));
    mcDBG_MSG_L2 (("Si2157_LoadFirmware_16 nbLines %d\n", nbLines));

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
        if (fw_table[line].firmware_len > 0)  /* don't download if length is 0 , e.g. dummy firmware */
        {
            /* send firmware_len bytes (up to 16) to Si2157 */
            if ((return_code = Si2157_L1_API_Patch(api, fw_table[line].firmware_len, fw_table[line].firmware_table)) != NO_Si2157_ERROR)
            {
                mcDBG_MSG_ERR(("Si2157_LoadFirmware_16 error 0x%02x patching line %d: %s\n", return_code, line, Si2157_L1_API_ERROR_TEXT(return_code) ));
                if (line == 0)
                {
                    mcDBG_MSG_L2(("The firmware is incompatible with the part!\n"));
                }
                return ERROR_Si2157_LOADING_FIRMWARE;
            }
            if (line==3)
                mcDBG_MSG_L2(("traces suspend"));
        }
    }
    mcDBG_MSG_L2(("traces resume"));
    mcDBG_MSG_L2 (("Si2157_LoadFirmware_16 complete...\n"));
    return NO_Si2157_ERROR;
}

/************************************************************************************************************************
  NAME: Si2157_StartFirmware
  DESCRIPTION: Start Si2157 firmware (put the Si2157 into run mode)
  Parameter:   Si2157 Context (I2C address)
  Parameter (passed by Reference):   ExitBootloadeer Response Status byte : tunint, atvint, dtvint, err, cts
  Returns:     I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_StartFirmware       (L1_Si2157_Context *api)
{

    if (Si2157_L1_EXIT_BOOTLOADER(api, Si2157_EXIT_BOOTLOADER_CMD_FUNC_TUNER, Si2157_EXIT_BOOTLOADER_CMD_CTSIEN_OFF) != NO_Si2157_ERROR)
    {
        return ERROR_Si2157_STARTING_FIRMWARE;
    }

    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
  NAME: Si2157_LoadFirmware
  DESCRIPTON: Load firmware from FIRMWARE_TABLE array in Si2157_Firmware_x_y_build_z.h file into Si2157
              Requires Si2157 to be in bootloader mode after PowerUp
  Programming Guide Reference:    Flowchart A.3 (Download FW PATCH flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to firmware table array
  Parameter:  number of lines in firmware table array (size in bytes / BYTES_PER_LINE)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadFirmware        (L1_Si2157_Context *api, unsigned char fw_table[], int nbLines)
{
    int return_code;
    int line;
    return_code = NO_Si2157_ERROR;

   mcDBG_MSG_L2(("Si2157_LoadFirmware starting...\n"));
    mcDBG_MSG_L2 (("Si2157_LoadFirmware nbLines %d\n", nbLines));

    /* for each line in fw_table */
    for (line = 0; line < nbLines; line++)
    {
        /* send Si2157_BYTES_PER_LINE fw bytes to Si2157 */
        if ((return_code = Si2157_L1_API_Patch(api, Si2157_BYTES_PER_LINE, fw_table + Si2157_BYTES_PER_LINE*line)) != NO_Si2157_ERROR)
        {
            mcDBG_MSG_ERR(("Si2157_LoadFirmware error 0x%02x patching line %d: %s\n", return_code, line, Si2157_L1_API_ERROR_TEXT(return_code) ));
            if (line == 0) {
                mcDBG_MSG_L2(("The firmware is incompatible with the part!\n"));
            }
            return ERROR_Si2157_LOADING_FIRMWARE;
        }
        if (line==3)
            mcDBG_MSG_L2(("traces suspend"));
    }
    mcDBG_MSG_L2(("traces resume"));
    mcDBG_MSG_L2 (("Si2157_LoadFirmware complete...\n"));
    return NO_Si2157_ERROR;
}
 /************************************************************************************************************************
  NAME: Si2157_LoadVideofilter
  DESCRIPTION:        Load video filters from vidfiltTable in Si2157_write_xTV_video_coeffs.h file into Si2157
  Programming Guide Reference:    Flowchart A.4 (Download Video Filters flowchart)

  Parameter:  Si2157 Context (I2C address)
  Parameter:  pointer to video filter table array
  Parameter:  number of lines in video filter table array(size in bytes / atv_vid_filt_struct)
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_LoadVideofilter     (L1_Si2157_Context *api, vid_filt_struct vidFiltTable[], int lines)
{
    int line;
    /* for each line in VIDFILT_TABLE  (max 16 bytes) */
    for (line = 0; line < lines; line++)
    {
        /* send up to 16 byte I2C command to Si2157 */
        if (Si2157_L1_API_Patch(api, vidFiltTable[line].vid_filt_len, vidFiltTable[line].vid_filt_table) != NO_Si2157_ERROR)
        {
            return ERROR_Si2157_SENDING_COMMAND;
        }
    }
    return NO_Si2157_ERROR;
}
/************************************************************************************************************************
  NAME: configureSi2157
  DESCRIPTION: Setup Si2157 video filters, GPIOs/clocks, Common Properties startup, Tuner startup, ATV startup, and DTV startup.
  Parameter:  Pointer to Si2157 Context (I2C address)
  Returns:    I2C transaction error code, 0 if successful
************************************************************************************************************************/
int Si2157_Configure           (L1_Si2157_Context *api)
{
    int return_code;
    return_code=0;

    /* Set Properties startup configuration         */
    Si2157_storePropertiesDefaults (api->propShadow);

    /* Edit the procedure below if you have any properties settings different from the standard defaults */
	#if 0
	// Silabs_130322 -- commented out the following property settings.  They are the defaults and writing them is unnecessary.
    api->prop->tuner_ien.tcien = Si2157_TUNER_IEN_PROP_TCIEN_ENABLE;
    api->prop->tuner_ien.rssilien = Si2157_TUNER_IEN_PROP_RSSILIEN_DISABLE;
    api->prop->tuner_ien.rssihien = Si2157_TUNER_IEN_PROP_RSSIHIEN_DISABLE;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_TUNER_IEN_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
    api->prop->dtv_ien.chlien = Si2157_DTV_IEN_PROP_CHLIEN_ENABLE;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_IEN_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
    api->prop->atv_ien.chlien = Si2157_ATV_IEN_PROP_CHLIEN_ENABLE;
    api->prop->atv_ien.pclien = Si2157_ATV_IEN_PROP_PCLIEN_DISABLE;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_IEN_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
	#endif
    api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;
    api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_DVBT;
    api->prop->dtv_mode.invert_spectrum = Si2157_DTV_MODE_PROP_INVERT_SPECTRUM_NORMAL;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_MODE_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }

    api->prop->atv_video_mode.color= Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
    api->prop->atv_video_mode.invert_spectrum= Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_NORMAL;
     // Silabs_120425 Removed. api->prop->atv_video_mode.trans = Si2157_ATV_VIDEO_MODE_PROP_TRANS_TERRESTRIAL;
    api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_VIDEO_MODE_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
   	#if 0
   // Silabs_130322 -- commented out the following property settings.  They are the defaults and writing them is unnecessary.
    api->prop->atv_agc_speed.if_agc_speed= Si2157_ATV_AGC_SPEED_PROP_IF_AGC_SPEED_185;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_AGC_SPEED_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
	#endif
    #if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    // Silabs_120425 was ATV_AGC_3DB    (renamed to AGC2_3DB)
    api->prop->atv_config_if_port.atv_agc_source=Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_AGC2_3DB;
    api->prop->atv_ext_agc.max_10mv = 200;
    api->prop->atv_ext_agc.min_10mv = 50;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_EXT_AGC_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
    #else
    api->prop->atv_config_if_port.atv_agc_source = Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_AGC_SOURCE_INTERNAL;
    #endif
    api->prop->atv_config_if_port.atv_out_type=Si2157_ATV_CONFIG_IF_PORT_PROP_ATV_OUT_TYPE_LIF_DIFF_IF1;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_CONFIG_IF_PORT_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
    #if SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL
    // Silabs_120425 was DTV_AGC_3DB    (renamed to AGC2_3DB)
    api->prop->dtv_config_if_port.dtv_agc_source=Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_AGC2_3DB;
    api->prop->dtv_ext_agc.max_10mv = 200;
    api->prop->dtv_ext_agc.min_10mv = 50;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_EXT_AGC_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
    #else
    api->prop->dtv_config_if_port.dtv_agc_source = Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_AGC_SOURCE_INTERNAL;
    #endif
    api->prop->dtv_config_if_port.dtv_out_type=Si2157_DTV_CONFIG_IF_PORT_PROP_DTV_OUT_TYPE_LIF_IF1;
    if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_CONFIG_IF_PORT_PROP)) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
	#if 0
	// Silabs_130322 -- commented out the following property settings.  They are the defaults and writing them is unnecessary.
    //Silabs_120425 added set dtv_pga_limits min=24, max=56 for all DTV standards
    api->prop->dtv_pga_limits.min = 24;
    api->prop->dtv_pga_limits.max = 56;
    if (Si2157_L1_SetProperty2(api, Si2157_DTV_PGA_LIMITS_PROP) != 0)
    {
        SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
        return return_code;
    }
	#endif
    //wenming 2012-07-31: FEF config
    if((return_code = Si2157_L1_CONFIG_PINS (api,
       Si2157_CONFIG_PINS_CMD_GPIO1_MODE_DISABLE,
       Si2157_CONFIG_PINS_CMD_GPIO1_READ_DO_NOT_READ,
       Si2157_CONFIG_PINS_CMD_GPIO2_MODE_DRIVE_0,
       Si2157_CONFIG_PINS_CMD_GPIO2_READ_DO_NOT_READ,
       Si2157_CONFIG_PINS_CMD_RESERVED1_RESERVED,
       Si2157_CONFIG_PINS_CMD_RESERVED2_RESERVED,
       Si2157_CONFIG_PINS_CMD_RESERVED3_RESERVED)) != 0)
    {
        SiTRACE ("Si2157_L1_CONFIG_PINS error 0x%02x\n", return_code);
        return return_code;
    }
    //end FEF config
    return 0;
}
/************************************************************************************************************************
  NAME: Si2157_PowerUpWithPatch
  DESCRIPTION: Send Si2157 API PowerUp Command with PowerUp to bootloader,
  Check the Chip rev and part, and ROMID are compared to expected values.
  Load the Firmware Patch then Start the Firmware.
  Programming Guide Reference:    Flowchart A.2 (POWER_UP with patch flowchart)

  Parameter:  pointer to Si2157 Context
  Returns:    Si2157/I2C transaction error code, NO_Si2157_ERROR if successful
************************************************************************************************************************/
int Si2157_PowerUpWithPatch    (L1_Si2157_Context *api)
{
    int return_code;
    return_code = NO_Si2157_ERROR;

    /* always wait for CTS prior to POWER_UP command */
    if ((return_code = Si2157_pollForCTS  (api)) != NO_Si2157_ERROR) {
        SiTRACE ("Si2157_pollForCTS error 0x%02x\n", return_code);
        return return_code;
    }

    if ((return_code = Si2157_L1_POWER_UP (api,
                            Si2157_POWER_UP_CMD_SUBCODE_CODE,
                            api->cmd->power_up.clock_mode,
                            api->cmd->power_up.en_xout,
                            Si2157_POWER_UP_CMD_PD_LDO_LDO_POWER_UP,
                            Si2157_POWER_UP_CMD_RESERVED2_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED3_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED4_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED5_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED6_RESERVED,
                            Si2157_POWER_UP_CMD_RESERVED7_RESERVED,
                            Si2157_POWER_UP_CMD_RESET_RESET,
                            Si2157_POWER_UP_CMD_CLOCK_FREQ_CLK_24MHZ,
                            Si2157_POWER_UP_CMD_RESERVED8_RESERVED,
                            Si2157_POWER_UP_CMD_FUNC_BOOTLOADER,
                            Si2157_POWER_UP_CMD_RESERVED9_RESERVED,
                            Si2157_POWER_UP_CMD_CTSIEN_DISABLE,
                            Si2157_POWER_UP_CMD_WAKE_UP_WAKE_UP
                            )) != NO_Si2157_ERROR)
    /* _power_up_call_insertion_point */
    {
        SiTRACE ("Si2157_L1_POWER_UP error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    /* Get the Part Info from the chip.   This command is only valid in Bootloader mode */
    if ((return_code = Si2157_L1_PART_INFO(api)) != NO_Si2157_ERROR) {
        SiTRACE ("Si2157_L1_PART_INFO error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }
    SiTRACE("chiprev %d\n",        api->rsp->part_info.chiprev);
    SiTRACE("part    Si21%d\n",    api->rsp->part_info.part   );
    SiTRACE("pmajor  %d\n",        api->rsp->part_info.pmajor );
    if (api->rsp->part_info.pmajor >= 0x30) {
    SiTRACE("pmajor '%c'\n",       api->rsp->part_info.pmajor );
    }
    SiTRACE("pminor  %d\n",        api->rsp->part_info.pminor );
    if (api->rsp->part_info.pminor >= 0x30) {
    SiTRACE("pminor '%c'\n",       api->rsp->part_info.pminor );
    }
    SiTRACE("pbuild %d\n",         api->rsp->part_info.pbuild );
    SiTRACE("romid %3d/0x%02x\n",  api->rsp->part_info.romid,  api->rsp->part_info.romid );
    /* Load the Firmware */

    if (api->rsp->part_info.romid == 0x50) {

      /* Only load the Firmware if we are NOT a Si2157-A30 part*/
      if ((api->rsp->part_info.pmajor == '3') && (api->rsp->part_info.pminor == '0') && (api->rsp->part_info.pbuild == 5))
      {
          SiTRACE ("No firmware to download. Loading from NVM only\n" );
      }
      else
      {
           if ((return_code = Si2157_LoadFirmware_16(api, Si2157_FW_3_0b5, FIRMWARE_LINES_3_0b5)) != NO_Si2157_ERROR) {
             SiTRACE ("Si2157_LoadFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
             return return_code;
           }
      }
    }

    /*Start the Firmware */
    if ((return_code = Si2157_StartFirmware(api)) != NO_Si2157_ERROR) { /* Start firmware */
        SiTRACE ("Si2157_StartFirmware error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        return return_code;
    }

    return NO_Si2157_ERROR;
}
/** Si2157_TunerInit
 *  Tuner initialzation forSi2157.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval void
 */

//-----------------------------------------------------------------------------
VOID Si2157_TunerInit(
    ITUNER_CTX_T *pTCtx
    )
{
    ITUNER_CTX_T *psTunerCtx = pTCtx;
    SPECIFIC_MEMBER_US_CTX * pUSCtx= &(pTCtx->specific_member.us_ctx);
    SPECIFIC_MEMBER_EU_CTX * pEUCtx= &(pTCtx->specific_member.eu_ctx);

    TunerInited = FALSE;

    psTunerCtx->u2LO_Step=1000;  //*1Khz

    psTunerCtx->I2cAddress = C_Si2157_LO_ADDRESS;
    psTunerCtx->u2IF_Freq = C_Si2157_IF_FREQUENCY;
    psTunerCtx->u2IF_Freq_A = 0; // C_Si2157_IF_FREQUENCY_ANA;//analog if frequency for ATD
    psTunerCtx->u4RF_Freq = 0;

     // AGC para
    psTunerCtx->sAtdAgcPara.AgcRfBias = 0x00;
    psTunerCtx->sAtdAgcPara.AgcRfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcIfMin  = 0x00;
    psTunerCtx->sAtdAgcPara.AgcDrSel  = 0x0B;

    psTunerCtx->u2RetryTVDTm = 100;  /*wenming 11-11-16:retry TVD lock for secam loss issue*/

    //ATD patch selection
    psTunerCtx->u1AtdPatchSwitch =0x8D;//MASK_PATCH_CR|MASK_PATCH_SENS|MASK_PATCH_CCI|MASK_PATCH_SELECTIVITY
    psTunerCtx->fgRFAGC = FALSE;//indicate if tuner need extern RF_AGC
    #if SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL
    psTunerCtx->fgRFTuner =FALSE;//Digital tuner
    #else
    psTunerCtx->fgRFTuner =TRUE;//d120210_Haibo:PGA shound be close loop
    #endif
    psTunerCtx->u1EqIndex=0;//EQ index select

    pEUCtx->m_SAW_BW  = C_Si2157_SAW_BW;
    pEUCtx->m_Ana_Top = C_Si2157_TOP_SET_ANA;
    pUSCtx->u2LO_Step = 1000; //*1KHZ
    pUSCtx->m_aucPara_SLD[2] = (U8)C_U8_Si2157_AGC_IF_SLP;
    pUSCtx->m_aucPara_SLD[3] = (U8)C_U8_Si2157_AGC_IF_INT;
    pUSCtx->m_aucPara_SLD[4] = (U8)C_U8_Si2157_AGC_IF_MAX;
    pUSCtx->m_aucPara_SLD[5] = (U8)C_U8_Si2157_AGC_IF_MIN;

    //Tuner frequency range
    pEUCtx->m_s4FreqBoundUpper = C_Si2157_FREQ_DBOUND_UPPER;
    pEUCtx->m_s4FreqBoundLower = C_Si2157_FREQ_DBOUND_LOWER;
    pEUCtx->m_s4AnaFreqBoundUpper = C_Si2157_FREQ_DBOUND_UPPER_Ana;
    pEUCtx->m_s4AnaFreqBoundLower = C_Si2157_FREQ_DBOUND_LOWER_Ana;
    // for Signal Level display
    pEUCtx->m_ifAgc_lvl_max = (UINT8)C_U8_Si2157_AGC_IF_LVL_MAX;
    pEUCtx->m_ifAgc_lvl_min = (UINT8)C_U8_Si2157_AGC_IF_LVL_MIN;

    pEUCtx->m_SigLvTh = 0;
    pEUCtx->m_SigLvScan = 0;

    pEUCtx->m_aucPara[0] = C_Si2157_TOP_SET;
    pEUCtx->m_aucPara[4] = (UINT8)C_U8_Si2157_AGC_IF_MAX;
    pEUCtx->m_aucPara[1] = 0;

    pEUCtx->m_aucPara[2] = (UINT8)C_U8_Si2157_AGC_IF_SLP;
    pEUCtx->m_aucPara[3] = (UINT8)C_U8_Si2157_AGC_IF_INT;
    pEUCtx->m_aucPara[5] = (UINT8)C_U8_Si2157_AGC_IF_MIN;

    pEUCtx->m_aucPara[6] = C_Si2157_AGC_COEF;
    pEUCtx->m_aucPara[7] = C_Si2157_DEMOD_INPUT_POWER;

    pEUCtx->m_aucPara[ 8] = (UINT8)C_U8_Si2157_MAX_IF_GAIN_SET;
    pEUCtx->m_aucPara[ 9] = (UINT8)C_U8_Si2157_MIN_IF_GAIN_SET;
    pEUCtx->m_aucPara[10] = (UINT8)C_U8_Si2157_MAX_RF_GAIN_SET;
    pEUCtx->m_aucPara[11] = (UINT8)C_U8_Si2157_MIN_RF_GAIN_SET;
    pEUCtx->m_aucPara[12] = (UINT8)C_U8_Si2157_IF_GAIN_RANGE;
    pEUCtx->m_aucPara[13] = (UINT8)C_U8_Si2157_RF_GAIN_RANGE;
    pEUCtx->m_aucPara[14] = C_Si2157_POWER_CALIBRATE;

    //IF MAX setting!
    pEUCtx->m_aucPara[21] = 0x7F;
	#if defined(CC_MT5890)
	if(IS_IC_5861())
	{
    pSi2157[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG_5861;
    pSi2157[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK_5861;
    pSi2157[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I_5861;
    pSi2157[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L_5861;
    pSi2157[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1_5861;
    pSi2157[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M_5861;

    pSi2157EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG_5861;
    pSi2157EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK_5861;
    pSi2157EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I_5861;
    pSi2157EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L_5861;
    pSi2157EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1_5861;
    pSi2157EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M_5861;	
	}
	else
	{
    pSi2157[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG;
    pSi2157[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK;
    pSi2157[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I;
    pSi2157[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L;
    pSi2157[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1;
    pSi2157[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M;

    pSi2157EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG;
    pSi2157EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK;
    pSi2157EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I;
    pSi2157EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L;
    pSi2157EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1;
    pSi2157EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M;
    }
	#else
	pSi2157[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG;
    pSi2157[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK;
    pSi2157[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I;
    pSi2157[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L;
    pSi2157[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1;
    pSi2157[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M;

    pSi2157EqWeak[MOD_PAL_BG - MOD_ANA_TYPE_BEGIN]   = EQSi2157BG;
    pSi2157EqWeak[MOD_PAL_DK - MOD_ANA_TYPE_BEGIN]   = EQSi2157DK;
    pSi2157EqWeak[MOD_PAL_I - MOD_ANA_TYPE_BEGIN]    = EQSi2157I;
    pSi2157EqWeak[MOD_SECAM_L - MOD_ANA_TYPE_BEGIN]  = EQSi2157L;
    pSi2157EqWeak[MOD_SECAM_L1 - MOD_ANA_TYPE_BEGIN] = EQSi2157L1;
    pSi2157EqWeak[MOD_NTSC_M- MOD_ANA_TYPE_BEGIN]    = EQSi2157M;
	#endif


         /* add Silabs Software Init here*/
    if(!TunerInited)
    {
        mcDBG_MSG_L1(("Init " C_SILABS_Si2157_STR "(Cust tuner)\n"));
        api = &api_context;
        /* Software Init */
        Si2157_L1_API_Init(api,C_Si2157_LO_ADDRESS);
    }
}

/* Setup properties to switch standards. */
void SetIfDemod(ITUNER_CTX_T *pTCtx,UINT8 subSysIdx,UINT32 Freq,UINT8 fgAutoSearch)
{
    // set a global here to save the video standard,  because it's not always available from the caller.
    VideoStandard =subSysIdx;
    //Default disable the AGC freeze function
    api->prop->dtv_agc_freeze_input.level=Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_LOW;
    api->prop->dtv_agc_freeze_input.pin=Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_NONE;
    if (Si2157_L1_SetProperty2(api, Si2157_DTV_AGC_FREEZE_INPUT_PROP) != 0)
    {
        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_AGC_FREEZE_INPUT_PROP\n"));
    }
    switch (subSysIdx)
    {
        case MOD_PAL_BG:
        /*wenming 11-12-05:always set sys as G when scan mode to fix DK detected as M/N issue*/
            if(ATV_CONN_MODE_CHAN_SCAN == fgAutoSearch)
            {
                api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH;
                mcDBG_MSG_L2(("TestLog:Now set as GH sys when autosearch\n"));
            }
            else
            {
                if(Freq<300000)  //pal B system
                {
                    api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_B;
                    mcDBG_MSG_L2(("TestLog:Now set as B sys\n"));
                }
                else
                {        //pal G system
                    api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_GH;
                    mcDBG_MSG_L2(("TestLog:Now set as GH sys\n"));
                }
            }

            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            // Silabs_120425 Removed. api->prop->atv_video_mode.trans = Si2157_ATV_VIDEO_MODE_PROP_TRANS_CABLE;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_BG;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
              api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
              api->prop->atv_lif_out.amp = NO_L_ANALOG_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
            }
            break;
        case MOD_PAL_DK:
            api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_DK;
            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_DK;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->atv_lif_out.amp = NO_L_ANALOG_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
            }
            break;
        case MOD_PAL_I:
            api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_I;
            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_I;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->atv_lif_out.amp = NO_L_ANALOG_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
            }
            break;
        case MOD_NTSC_M:
            api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_M;
            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_PAL_NTSC;
            api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_M;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->atv_lif_out.amp = NO_L_ANALOG_IF_AMP; // mark to check cli

            mcDBG_MSG_L2((" =====NTSC IF AMP=%d \n",api->prop->atv_lif_out.amp));
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
            }
            break;
        case MOD_SECAM_L:
            api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L;
            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
            api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_L;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->atv_lif_out.amp = L_ANALOG_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
            }
            break;
        case MOD_SECAM_L1:
            if(fgAutoSearch)
            {
                api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_L;
                api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;
            }
            else
            {
                api->prop->atv_video_mode.video_sys = Si2157_ATV_VIDEO_MODE_PROP_VIDEO_SYS_LP;
                api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_NORMAL;
            }
            api->prop->atv_video_mode.color = Si2157_ATV_VIDEO_MODE_PROP_COLOR_SECAM;
            //api->prop->atv_video_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_NORMAL;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_VIDEO_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_VIDEO_MODE_PROP\n"));
            }
            api->prop->atv_lif_freq.offset = C_Si2157_IF_CENTER_L1;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_lif_out.offset = Si2157_ATV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->atv_lif_out.amp = L_ANALOG_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
            }
            api->prop->atv_rf_top.atv_rf_top= Si2157_ATV_RF_TOP_PROP_ATV_RF_TOP_M6DB;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_RF_TOP_PROP\n"));
			}
            break;
       case MOD_DVBT:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_DVBT;
            if(pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_6M)
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_6MHZ;
            else if (pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_7M)
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_7MHZ;
            else
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;

            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =DVBT_IF_AMP;
            mcDBG_MSG_L2(("DVBT IF AMP =%d \n",api->prop->dtv_lif_out.amp ));
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_rf_top.dtv_rf_top= Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 was M2DB
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.dvbt = Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_ZIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            break;
       case MOD_DVBT2:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_DVBT;
            if(pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_6M)
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_6MHZ;
            else if (pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_7M)
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_7MHZ;
            else if (pTCtx->specific_member.eu_ctx.m_SAW_BW == SAW_BW_8M)
                api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;
            else
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =DVBT2_IF_AMP;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 was M2DB
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.dvbt = Si2157_DTV_INTERNAL_ZIF_PROP_DVBT_ZIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            api->prop->dtv_agc_freeze_input.level=Si2157_DTV_AGC_FREEZE_INPUT_PROP_LEVEL_HIGH;
            api->prop->dtv_agc_freeze_input.pin=Si2157_DTV_AGC_FREEZE_INPUT_PROP_PIN_GPIO1;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_AGC_FREEZE_INPUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_AGC_FREEZE_INPUT_PROP\n"));
            }
            break;
       case MOD_DVBC:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_DVBC;
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =DVBC_IF_AMP;
            mcDBG_MSG_L2(("XXXXXXXXXX DVBC IF AMP =%d \n",api->prop->dtv_lif_out.amp ));
            //api->prop->dtv_lif_out.amp =32;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 was M2DB
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.dvbc = Si2157_DTV_INTERNAL_ZIF_PROP_DVBC_ZIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            break;
      case MOD_ATSC_8VSB:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_ATSC;
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =ATSC_8VSB_IF_AMP;
            mcDBG_MSG_L2(("XXXXXXXXXX 8VSB IF AMP =%d \n",api->prop->dtv_lif_out.amp ));
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 added
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.atsc = Si2157_DTV_INTERNAL_ZIF_PROP_ATSC_ZIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            break;
        case MOD_J83B_AUTO_QAM:
        case MOD_J83B_256QAM:
        case MOD_J83B_64QAM:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_QAM_US;
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =ATSC_QAM_IF_AMP;
            mcDBG_MSG_L2(("XXXXXXXXXX QAM IF AMP =%d \n",api->prop->dtv_lif_out.amp ));
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            #if 0
            // Silabs_130322 -- commented out the following property settings.  They are the defaults and writing them is unnecessary.
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 added
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.qam_us = Si2157_DTV_INTERNAL_ZIF_PROP_QAM_US_ZIF ;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            #endif
            break;
        case MOD_DTMB:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_DTMB;
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_8MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =DTMB_IF_AMP;
			mcDBG_MSG_ERR((" [Silabs]: DTMB IF_AMP=%d\n",api->prop->dtv_lif_out.amp));
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }

            // Silabs_20130913 -- Change from ZIF to LIF for better sensitivity performance.
            api->prop->dtv_internal_zif.dtmb = Si2157_DTV_INTERNAL_ZIF_PROP_DTMB_LIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }

            #if 0
            // Silabs_130322 -- commented out the following property settings.  They are the defaults and writing them is unnecessary.
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 added
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            #endif
            break;
       case MOD_ISDBT:
            api->prop->dtv_mode.modulation = Si2157_DTV_MODE_PROP_MODULATION_ISDBT;
            api->prop->dtv_mode.bw = Si2157_DTV_MODE_PROP_BW_BW_6MHZ;
            api->prop->dtv_mode.invert_spectrum = Si2157_ATV_VIDEO_MODE_PROP_INVERT_SPECTRUM_INVERTED;

            if (Si2157_L1_SetProperty2(api, Si2157_DTV_MODE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_MODE_PROP\n"));
            }
            api->prop->dtv_lif_freq.offset=C_Si2157_IF_FREQUENCY;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_FREQ_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_lif_out.offset = Si2157_DTV_LIF_OUT_PROP_OFFSET_DEFAULT;
            api->prop->dtv_lif_out.amp =Si2157_DTV_LIF_OUT_PROP_AMP_DEFAULT;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
            }
            api->prop->dtv_rf_top.dtv_rf_top = Si2157_DTV_RF_TOP_PROP_DTV_RF_TOP_P4DB; //Silabs_120425 added
             if (Si2157_L1_SetProperty2(api, Si2157_DTV_RF_TOP_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_RF_TOP_PROP\n"));
            }
            api->prop->dtv_internal_zif.isdbt = Si2157_DTV_INTERNAL_ZIF_PROP_ISDBT_ZIF;
            if (Si2157_L1_SetProperty2(api, Si2157_DTV_INTERNAL_ZIF_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_INTERNAL_ZIF_PROP\n"));
            }
            break;
    }
}
//-----------------------------------------------------------------------------
/** Si2157_TunerSetFreq
 *  Set Tuner PLL forSi2157/Si2157 to tune RF frequency.
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param     param    Pointer of structure of parameters
 *  @retval  0 success
                    1  fail   out of range
                   -1 fail  I2C error
 */
//-----------------------------------------------------------------------------
INT16 Si2157_TunerSetFreq(
    ITUNER_CTX_T *pTCtx,
    PARAM_SETFREQ_T* param
    )
{
    UINT32 Freq = param->Freq; // transfer to video freq(KHz) digital center freq ,analog picture freq
    UINT8 Mode = param->Modulation;    //tv system
    UINT8 AutoSearch=param->fgAutoSearch;
    int timeout =36;
    UINT32 freqHz =0;
    UINT32 u2TickStart_eu;
    UINT32 u2TickTUNINTStart_eu;
    UINT32 u2TickxTVINTStart_eu;
    UINT32 TUNINT_duration =timeout;

    //UINT32 xTVINT_duration =110;
    UINT32 xTVINT_duration =115;//yuan add 5ms for cover "mcGET_SYS_TICK()" resolution
    TV_MODES tvMode;
    u2TickStart_eu = mcGET_SYS_TICK();
    pTCtx->u4RF_Freq = Freq;
    // SPECIFIC_MEMBER_EU_CTX* pEUCtx= &( pTunerCtx->specific_member.eu_ctx);
    mcDBG_MSG_L2 (("Si2157_TunerSetFreq freq = %d;Mode=%d\n", Freq,Mode ));
#if SI2157_ENABLE_EXTERNAL_ATV_AGC_CONTROL
    mcDBG_MSG_L2(("SI2157:Now is external ATV AGC control!!!\n"));
#endif
#if SI2157_ENABLE_EXTERNAL_DTV_AGC_CONTROL
    mcDBG_MSG_L2(("SI2157:Now is external DTV AGC control!!!\n"));
#endif



    if(!TunerInited)
    {
        TunerInited=TRUE;

        api->cmd->power_up.clock_mode = Si2157_POWER_UP_CMD_CLOCK_MODE_XTAL;
        api->cmd->power_up.en_xout = Si2157_POWER_UP_CMD_EN_XOUT_DIS_XOUT;

        Si2157_PowerUpWithPatch (api);
        Si2157_Configure (api);
    }

    SetIfDemod(pTCtx,Mode,Freq,AutoSearch);

    if(Mode == MOD_PAL_BG) /*wenming 11-12-2: BG PC to Center is by 7M or 8M BW*/
    {
        if(ATV_CONN_MODE_CHAN_SCAN == AutoSearch)
        {
            pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_BG+C_Si2157_PF_TO_CF_SHIFT_BG_8M;
            mcDBG_MSG_L2(("TestLog:Now set 8M PC-CENTER when scan mode\n"));
        }
        else
        {
            if(Freq<300000)  //pal B system
            {
                pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_BG+C_Si2157_PF_TO_CF_SHIFT_BG_7M;
                mcDBG_MSG_L2(("TestLog:Now set 7M PC-CENTER"));
            }
            else            //pag G system
            {
                pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_BG+C_Si2157_PF_TO_CF_SHIFT_BG_8M;
                mcDBG_MSG_L2(("TestLog:Now set 8M PC-CENTER"));
            }
        }
    }
    if(Mode == MOD_PAL_DK)
        pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_DK+C_Si2157_PF_TO_CF_SHIFT_DK;
    if(Mode == MOD_PAL_I)
        pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_I+C_Si2157_PF_TO_CF_SHIFT_I;
    if(Mode == MOD_NTSC_M)
        pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_M+C_Si2157_PF_TO_CF_SHIFT_M;
    if(Mode == MOD_SECAM_L)
        pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_L+C_Si2157_PF_TO_CF_SHIFT_L;
    if(Mode == MOD_SECAM_L1)
        pTCtx->u2IF_Freq_A = C_Si2157_IF_CENTER_L1-C_Si2157_PF_TO_CF_SHIFT_L1;

    if (Mode < MOD_TYPE_END)
        tvMode = DIGITAL_MODE;
    else
    {
        tvMode=ANALOG_MODE;
        if(AutoSearch)
        {
            api->prop->atv_afc_range.range_khz= 1000;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_AFC_RANGE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_AFC_RANGE_PROP\n"));
            }
            else
            {
                mcDBG_MSG_L2(("[Silabs]:-->>> Si2157_L1_SetProperty2 set Si2157_ATV_AFC_RANGE_PROP OK\n"));
            }
        }
        else
        {
            api->prop->atv_afc_range.range_khz= 1000;
            if (Si2157_L1_SetProperty2(api, Si2157_ATV_AFC_RANGE_PROP) != 0)
            {
                mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_AFC_RANGE_PROP\n"));
            }
            else
            {
                mcDBG_MSG_L2(("[Silabs]:-->>> Si2157_L1_SetProperty2 set Si2157_ATV_AFC_RANGE_PROP OK\n"));
            }
        }
    }

    /* Use Picture Carrier Frequency if ATV and Center Frequency if DTV */
    if (tvMode==ANALOG_MODE)
        freqHz = PCFreq_To_CenterFreq( Mode,Freq * 1000, AutoSearch);
    else
        freqHz = Freq * 1000;

    if (freqHz < Si2157_TUNER_TUNE_FREQ_CMD_FREQ_MIN || freqHz > Si2157_TUNER_TUNE_FREQ_CMD_FREQ_MAX)
    {
		mcDBG_MSG_L2(("[Silabs]:<= Response out of Range\n"));
        return ERROR_Si2157_PARAMETER_OUT_OF_RANGE; // =1
    }
    if (tvMode==ANALOG_MODE)
    {
        /* set Si2157_TUNER_LO_INJECTION_PROP property for ATV */
	if(Mode == MOD_NTSC_M)
		{
				api->prop->tuner_lo_injection.band_1	= Si2157_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE;
				api->prop->tuner_lo_injection.band_2	= Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE;
				api->prop->tuner_lo_injection.band_3	= Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE;
		}
	else
		{
				api->prop->tuner_lo_injection.band_1	= Si2157_TUNER_LO_INJECTION_PROP_BAND_1_LOW_SIDE;
				api->prop->tuner_lo_injection.band_2	= Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE;
				api->prop->tuner_lo_injection.band_3	= Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE;
		}
        if ((Si2157_L1_SetProperty2(api,Si2157_TUNER_LO_INJECTION_PROP)) != 0)
        {
            SiTRACE ("Si2157_L1_SetProperty2 Si2157_TUNER_LO_INJECTION_PROP error \n");
        }
        else
        {
            mcDBG_MSG_L2(("[Silabs]:-->>> Si2157_L1_SetProperty2 set Si2157_TUNER_LO_INJECTION_PROP OK"));
        }
    }
    else
    {
        /* set Si2157_TUNER_LO_INJECTION_PROP property for DTV */
        api->prop->tuner_lo_injection.band_1  = Si2157_TUNER_LO_INJECTION_PROP_BAND_1_HIGH_SIDE;
        api->prop->tuner_lo_injection.band_2  = Si2157_TUNER_LO_INJECTION_PROP_BAND_2_LOW_SIDE;
        api->prop->tuner_lo_injection.band_3  = Si2157_TUNER_LO_INJECTION_PROP_BAND_3_LOW_SIDE;
        if ((Si2157_L1_SetProperty2(api,Si2157_TUNER_LO_INJECTION_PROP)) != 0)
        {
            SiTRACE ("Si2157_L1_SetProperty2 Si2157_TUNER_LO_INJECTION_PROP error \n");
        }
    }

    if (Si2157_L1_TUNER_TUNE_FREQ (api,(tvMode==ANALOG_MODE) ? Si2157_TUNER_TUNE_FREQ_CMD_MODE_ATV : Si2157_TUNER_TUNE_FREQ_CMD_MODE_DTV,  freqHz) != 0)
    {
        mcDBG_MSG_ERR(("[Silabs]:Error Si2157_L1_TUNER_TUNE_FREQ\n"));
        return -1;
    }
    else
    {
        mcDBG_MSG_L2(("[Silabs]:Si2157_L1_TUNER_TUNE_FREQ OK\n"));
    }

    /* wait for TUNINT, timeout is 36ms */
    u2TickTUNINTStart_eu = mcGET_SYS_TICK();
    while (( mcGET_TICK_PERIOD() * (mcGET_SYS_TICK() - u2TickTUNINTStart_eu)) < (timeout+5) ) //yuan add 5ms for cover "mcGET_SYS_TICK()" resolution
    {
        if (( Si2157_L1_CheckStatus(api)) != 0)
        {
            mcDBG_MSG_ERR(("[Silabs]:Error Si2157_L1_CheckStatus\n"));
            return ERROR_Si2157_SENDING_COMMAND;
        }

        if (api->status->tunint)
        {
            /* below line for debug only, should be commented in normal mode */
            TUNINT_duration = (mcGET_SYS_TICK() - u2TickTUNINTStart_eu) * mcGET_TICK_PERIOD();
            break;
        }
    }

    /*wait for ATVINT, timeout is 110ms for ATVINT and 10 ms for DTVINT */
    if (tvMode==ANALOG_MODE)
    {
    /* Silabs_121115 if in SCAN mode then issue the ATV_CW_TEST command which will freeze the AFC and pass the IF */
    /* This is not to be used for picture viewing just for channel scan detection */
        if (AutoSearch)
        {
            if (Si2157_L1_ATV_CW_TEST(api,Si2157_ATV_CW_TEST_CMD_PC_LOCK_UNLOCK) != NO_Si2157_ERROR)
            {
                mcDBG_MSG_ERR(("[Silabs]:Error Si2157_L1_ATV_CW_TEST\n"));
                return -1;
            }
            else
                return NO_Si2157_ERROR;
            mcDBG_MSG_L2(("[Silabs]:Si2157_L1_ATV_CW_TEST\n"));
        }
        timeout = 110;
    }
    else
    {
        timeout = 10;
    }
    u2TickxTVINTStart_eu = mcGET_SYS_TICK();
    while (( mcGET_TICK_PERIOD() *( mcGET_SYS_TICK() - u2TickxTVINTStart_eu)) < (timeout+5) ) //yuan add 5ms for cover "mcGET_SYS_TICK()" resolution
    {
        if (( Si2157_L1_CheckStatus(api)) != 0)
        {
            mcDBG_MSG_ERR(("[Silabs]:Error Si2157_L1_CheckStatus\n"));
            return ERROR_Si2157_SENDING_COMMAND;
        }

        if (tvMode==ANALOG_MODE)
        {
            if (api->status->atvint)
            {
                /* below line for debug only, should be commented in normal mode */
                xTVINT_duration = (mcGET_SYS_TICK() - u2TickxTVINTStart_eu) * mcGET_TICK_PERIOD();
                break;
            }
        }
        else
        {
            if (api->status->dtvint)
            {
                /* below line for debug only, should be commented in normal mode */
                xTVINT_duration = (mcGET_SYS_TICK() - u2TickxTVINTStart_eu) * mcGET_TICK_PERIOD();
                break;
            }
        }
    }

    mcDBG_MSG_L2(("[Silabs]:Si2157TunerSetFreq use time = (%3u ms) tun=%3u atv=%3u\n", (mcGET_SYS_TICK() - u2TickStart_eu) * mcGET_TICK_PERIOD(),TUNINT_duration,xTVINT_duration));

    return (0);
}


/* Get Status information from part */
UCHAR GetStatus( L1_Si2157_Context *api,TUNER_STATUSES status)
{
    int return_code;
    if (status==CHL)
    {
        if ((return_code=Si2157_L1_ATV_STATUS(api,Si2157_ATV_STATUS_CMD_INTACK_CLEAR))!=0) {
             SiTRACE ("Si2157_L1_ATV_STATUS error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        }
        return api->rsp->atv_status.chl;
    }
    else if (status == DTV_CHL)
    {
        if ((return_code=Si2157_L1_DTV_STATUS(api,Si2157_DTV_STATUS_CMD_INTACK_CLEAR))!=0) {
         SiTRACE ("Si2157_L1_DTV_STATUS error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );
        }
        return api->rsp->dtv_status.chl;
    }
    else
        return 0;
}

/* Get RSSI  from part */
CHAR GetSignalLevel(L1_Si2157_Context *api)
{
    int return_code;
    if ((return_code=Si2157_L1_TUNER_STATUS(api,Si2157_TUNER_STATUS_CMD_INTACK_CLEAR))!=0) {
        SiTRACE ("Si2157_L1_TUNER_STATUS error 0x%02x: %s\n", return_code, Si2157_L1_API_ERROR_TEXT(return_code) );

    }

    return api->rsp->tuner_status.rssi;
}
static VOID Si2157_SetSawBw(ITUNER_CTX_T * pTCtx, UINT8 sawbw)
{
    pTCtx->specific_member.eu_ctx.m_SAW_BW=sawbw;
    mcDBG_MSG_L2(("Si2157_SetSawBw=%d\n",sawbw));
}

//-----------------------------------------------------------------------------
/** Si2157_TunerGetVer
 *  Get Tuner type version
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @retval tuner model name
 */
//-----------------------------------------------------------------------------
static CHAR *Si2157_TunerGetVer(VOID)
{
    return ((CHAR*)C_SILABS_Si2157_STR);
}



UINT32 Si2157_GetSSI(ITUNER_CTX_T* pTunerCtx)
{
    INT16 Power_Ref[] =
    {
        -93,
        -91,
        -90,
        -89,
        -88,
        -87,
        -85,
        -84,
        -83,
        -82,
        -82,
        -80,
        -78,
        -77,
        -76,
    };
    INT32 i4Power_ref = 0;
    INT8 i4Power_rec = 0;
    INT32 i4Power_rel = 0;

    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;

    /* Get Power Ref Value */
    mcDBG_MSG_L2(("DVBT: Mod=%d, Code Rate=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR));

    /* Get Power_Rec Value */
    i4Power_rec=GetSignalLevel(api);

    /*Get Ref Value-Start*/
    if(pTunerCtx->sSSIPara.i2Mod==DVBT_Qam_Mode_Unknow)
    {
        //mcSHOW_DBG_MSG1(("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
        //printf("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel);
        return u4SSI_Indicator;
    }
    else
    {
        i4Power_ref = Power_Ref[(pTunerCtx->sSSIPara.i2Mod)*DVBT_CR_Mode_Size + pTunerCtx->sSSIPara.i2CR];
    }

    /* Get Power Rel Value */
    if(pTunerCtx->sSSIPara.i2Bw == SAW_BW_8M)
    {
        mcDBG_MSG_L2(("DVBT:8M Real_i4Power_rec = %d\n",i4Power_rec));
        i4Power_rec = i4Power_rec - Adjust_Si2157_i4Power_rec_val;
    }
    else
    {
        mcDBG_MSG_L2(("DVBT:7M Real_i4Power_rec = %d\n",i4Power_rec));
        i4Power_rec = i4Power_rec - Adjust_Si2157_i4Power_rec_val;
    }
    i4Power_rel = i4Power_rec - i4Power_ref;
    mcDBG_MSG_L2(("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));

    //printf("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel);
    /* Cal SSI_Indicator Value */
    if(i4Power_rel < -15)
    {
        u4SSI_Indicator = 0;
    }
    else if((i4Power_rel >= -15) && (i4Power_rel < 0))
    {
        u4SSI_Indicator = 2*(i4Power_rel+15)/3;
    }
    else if((i4Power_rel >= 0) && (i4Power_rel < 20))
    {
        u4SSI_Indicator = 4*i4Power_rel+10;
    }
    else if((i4Power_rel >= 20) && (i4Power_rel < 35))
    {
        u4SSI_Indicator = 2*(i4Power_rel-20)/3+90;
    }
    else if(i4Power_rel >= 35)
    {
        u4SSI_Indicator = 100;
    }

    // update signal level for best mux, Ken, 100607
    pCtx->m_SigLvScan = (INT8) u4SSI_Indicator;
    mcDBG_MSG_L2(("u4SSI_Indicator = %d",u4SSI_Indicator));
    return u4SSI_Indicator;
}


UINT32 Si2157_GetSSI_T2(ITUNER_CTX_T* pTunerCtx)
{
    INT16 Power_Ref[] =
    {
        -930,
        -923,
        -916,
        -909,
        -902,
        -895,
        -888,
        -881,
        -874,
        -867,
        -860,
        -853,
        -846,
        -839,
        -832,
        -825,
        -818,
        -811,
        -794,
        -787,
        -780,
        -772,
        -765,
        -758,
    };
    INT32 i4Power_ref = 0;
    INT32 i4Power_rec = 0;
    INT32 i4Power_rel = 0;

    UINT32 u4SSI_Indicator = 0;
    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);

    // initial level for best mux, Ken, 100607
    pCtx->m_SigLvScan = 0;

    /* Get Power Ref Value */
    mcDBG_MSG_L2(("DVBT: Mod=%d, Code Rate=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR));

    /* Get Power_Rec Value */
    i4Power_rec=((INT32)GetSignalLevel(api))*10;

    /*Get Ref Value-Start*/
    if(pTunerCtx->sSSIPara.i2Mod==DVBT2_Qam_Mode_Unknow)
    {
        //mcDBG_MSG_L21(("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
        //printf("DVBT: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel);
        return u4SSI_Indicator;
    }
    else
    {
        i4Power_ref = Power_Ref[(pTunerCtx->sSSIPara.i2Mod)*DVBT2_CR_Mode_Size + pTunerCtx->sSSIPara.i2CR];
    }

    /* Get Power Rel Value */
    i4Power_rec = i4Power_rec - 20;  //adjust bu test result;
    i4Power_rel = i4Power_rec - i4Power_ref;
    mcDBG_MSG_L2(("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));

    //printf("DVBT: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel);
    /* Cal SSI_Indicator Value */
    if(i4Power_rel < -150)
    {
        u4SSI_Indicator = 0;
    }
    else if((i4Power_rel >= -150) && (i4Power_rel < 0))
    {
        u4SSI_Indicator = (i4Power_rel+150)/15;
    }
    else if((i4Power_rel >= 0) && (i4Power_rel < 200))
    {
        u4SSI_Indicator = 4*i4Power_rel/10+10;
    }
    else if((i4Power_rel >= 200) && (i4Power_rel < 350))
    {
        u4SSI_Indicator = (i4Power_rel-200)/15+90;
    }
    else if(i4Power_rel >= 350)
    {
        u4SSI_Indicator = 100;
    }

    // update signal level for best mux, Ken, 100607
    pCtx->m_SigLvScan = (INT8) u4SSI_Indicator;
    mcDBG_MSG_L2(("u4SSI_Indicator = %d",u4SSI_Indicator));
    return u4SSI_Indicator;
}

/*wenming 11-10-13:DVBC SSI*/
UINT32 Si2157_GetSSI_DVBC(ITUNER_CTX_T* pTunerCtx)
{
    INT16 Power_Ref[] =
    {
        -82,//16QAM
        -79,//32QAM
        -76,//64
        -73,//128QAM
        -70,//256QAM
    };
    INT32 i4Power_ref = 0;
    INT32 i4Power_rec = 0;
    INT32 i4Power_rel = 0;
    INT16 refIndex=0;
    UINT32 u4SSI_Indicator = 0;

    SPECIFIC_MEMBER_EU_CTX * pCtx= &(pTunerCtx->specific_member.eu_ctx);
    pCtx->m_SigLvScan = 0;
    mcDBG_MSG_L2(("DVBC: Mod=%d, Code Rate=%d\n",pTunerCtx->sSSIPara.i2Mod,pTunerCtx->sSSIPara.i2CR));

    i4Power_rec=GetSignalLevel(api);;
    i4Power_rec = i4Power_rec - fgParaA_DVBC;
     //i4Power_rec+=6;//compensate
        refIndex=pTunerCtx->sSSIPara.i2Mod;
        mcDBG_MSG_L2(("refIndex=%d",refIndex));
    if(refIndex>4)
    {
        mcDBG_MSG_L2(("DVBC: QAM mode unknown; Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
        return u4SSI_Indicator;
    }
    else
    {
        i4Power_ref = Power_Ref[refIndex];
        mcDBG_MSG_L2(("i4Power_ref=%d\n",i4Power_ref));
    }
    i4Power_rel = i4Power_rec - i4Power_ref;
    mcDBG_MSG_L2(("DVBC: Power_rec=%d, Power_ref=%d, Power_rel=%d\n",i4Power_rec,i4Power_ref,i4Power_rel));
    if(i4Power_rel < -15)
    {
        u4SSI_Indicator = 0;
    }
    else if((i4Power_rel >= -15) && (i4Power_rel < 0))
    {
        u4SSI_Indicator = 2*(i4Power_rel+15)/3;
    }
    else if((i4Power_rel >= 0) && (i4Power_rel < 20))
    {
        u4SSI_Indicator = 4*i4Power_rel+10;
    }
    else if((i4Power_rel >= 20) && (i4Power_rel < 35))
    {
        u4SSI_Indicator = 2*(i4Power_rel-20)/3+90;
    }
    else if(i4Power_rel >= 35)
    {
        u4SSI_Indicator = 100;
    }
    pCtx->m_SigLvScan = (S8) u4SSI_Indicator;
    return u4SSI_Indicator;
}
//-----------------------------------------------------------------------------
/** Si2157_SetSSICondition
 *  Set SSI Condition for TH2603
 *  @param
 *  @retval void
 */
//-----------------------------------------------------------------------------
void Si2157_SetSSICondition(ITUNER_CTX_T* pTunerCtx, VOID * pInOutVal)
{
    pTunerCtx->sSSIPara.i2Mod = *((INT16*) pInOutVal);
    pTunerCtx->sSSIPara.i2CR = *((INT16*) pInOutVal+1);
    pTunerCtx->sSSIPara.i2Bw = *((INT16*) pInOutVal+5);
}

//-----------------------------------------------------------------------------
/** Si2157_TunerOP
 * Set/Get function
 *  @param  pTCtx       Pointer of pi tuner driver context.
 *  @param  eOperation       To Get/Set operation id
 *  @retval  ITUNER_OK    Operation Success
 *  @retval  ITUNER_NOT_OK    Operation fail
 *  @retval  ITUNER_NOT_SUPPORTED Operation not supported in  driver
 */
//-----------------------------------------------------------------------------
INT16 Si2157_TunerOP(ITUNER_CTX_T * pTCtx, ITUNEROP_T eOperation, UINT8 SetVal,  VOID * pInOutVal){

    //L1_Si2157_Context *api;
    int return_code =0;

    //api->prop->dtv_mode.bw = 8;
    switch(eOperation){
    case itSetSSICond:
        Si2157_SetSSICondition(pTCtx, pInOutVal);
        break;
    case itGetEqScriptNormal:
        *(UINT8**)pInOutVal = pSi2157[SetVal];;
        break;
    case itGetEqScriptWeak:
        *(UINT8**)pInOutVal = pSi2157EqWeak[SetVal];
        break;
    case itGetTunerRelReg://Get ATD Tuner Rel register setting
        {
            if((SetVal==MOD_SECAM_L)||(SetVal==MOD_SECAM_L1)){
                *(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelRegSecam;}
            else{
                *(ATD_TUNER_REL_REG_T **)pInOutVal = arrAtdRelReg;
            }
        }
    break;

    case itGetVer:
        {
            CHAR ** RetStr=(CHAR **)pInOutVal;
            *RetStr=Si2157_TunerGetVer();
        }
        break;
    case itGetTop:
        if (SetVal >= MOD_ANA_TYPE_BEGIN)
        {
            if ((return_code=Si2157_L1_GetProperty2(api,Si2157_ATV_RF_TOP_PROP))!=0)
            {
                SiTRACE ("Si2157_L1_GetProperty2 error 0x%02x\n", return_code);
                return ITUNER_NOT_OK;
            }
            *(UINT8 *)pInOutVal= api->prop->atv_rf_top.atv_rf_top;
        }
        else
        {
            if ((return_code=Si2157_L1_GetProperty2(api,Si2157_DTV_RF_TOP_PROP))!=0)
            {
                SiTRACE ("Si2157_L1_GetProperty2 error 0x%02x\n", return_code);
                return ITUNER_NOT_OK;
            }
            *(UINT8 *)pInOutVal= api->prop->dtv_rf_top.dtv_rf_top;
        }
        break;
    case itSetIfDemod:
        //    SetIfDemod(SetVal);
        break;
    case itSetTop:
        {
            if(VideoStandard >= MOD_ANA_TYPE_BEGIN)
            {
                api->prop->atv_rf_top.atv_rf_top=SetVal;
                if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_RF_TOP_PROP)) != 0)
                {
                    SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
                    return ITUNER_NOT_OK;
                }
            }
            else
            {
                api->prop->dtv_rf_top.dtv_rf_top=SetVal;
                if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_RF_TOP_PROP)) != 0)
                {
                    SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
                    return ITUNER_NOT_OK;
                }
            }
        }
        break;
    case itSetSawBw://Set Channel Bandwidth
        {
            Si2157_SetSawBw(pTCtx,SetVal);
        }
        break;
        case itGetStatus: // return channel locked status.
        if (SetVal >= MOD_ANA_TYPE_BEGIN)
            *(UINT8 *)pInOutVal = GetStatus( api,CHL);
        else
            *(UINT8 *)pInOutVal = GetStatus( api,DTV_CHL);
        break;
    case itGetSSIIndex: // Get RSSI
        //    INT32 *RetVal= NULL;
        //   RetVal = (UINT32 *)pInOutVal;
        if (SetVal==0)
        {
            *(INT32 *)pInOutVal = Si2157_GetSSI(pTCtx);
        }
        else
        {
            *(INT32 *)pInOutVal = Si2157_GetSSI_DVBC(pTCtx);
        }
        break;
    case itGetSSIIndex_T2: // Get RSSI
        *(INT32 *)pInOutVal = Si2157_GetSSI_T2(pTCtx);//(INT16)GetSignalLevel(api);
        //SiTRACE("Si2157_GetSignalLevel= %d\n",(GetSignalLevel(api)));
        break;
    case itGetRSSI:
        {
            INT32 * Retval = (INT32*)pInOutVal;
            *Retval = GetSignalLevel(api) - fgParaA_DVBT2;
        }
        break;
    case itGetSignalLevel: // Get RSSI
        *(INT32 *)pInOutVal = (108+(INT8)GetSignalLevel(api));// use dBuV to show signal level
        //SiTRACE("Si2157_GetSignalLevel= %d\n",(108 +(INT8) GetSignalLevel(api)));
        break;
    case itSetIF: // set ATV/DTV LIF FREQ
        if (VideoStandard >= MOD_ANA_TYPE_BEGIN)
        {
            api->prop->atv_lif_freq.offset = SetVal;
            if ((return_code=Si2157_L1_SetProperty2(api,Si2157_ATV_LIF_FREQ_PROP)) != 0)
            {
                SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
                return ITUNER_NOT_OK;
            }
        }
        else
        {
            api->prop->dtv_lif_freq.offset = SetVal;
            if ((return_code=Si2157_L1_SetProperty2(api,Si2157_DTV_LIF_FREQ_PROP)) != 0)
            {
                SiTRACE ("Si2157_L1_SetProperty2 error 0x%02x\n", return_code);
                return ITUNER_NOT_OK;
            }
        }
        break;
	case itGetCNDTMBTunerRelReg://Get CNDTMB Tuner Rel register setting
			 {
				 *(CN_TUNER_REL_REG_T **)pInOutVal=arrCNDTMBRelReg;
			 }
		 break;
		 
	case itGetCNDVBCTunerRelReg://Get CNDVBC Tuner Rel register setting
			 {
				 *(CN_TUNER_REL_REG_T **)pInOutVal=arrCNDVBCRelReg;
			 }
		 break;
		 


		
    case itSetIFAMP:
        {
            mcDBG_MSG_L2(("---------IF_AMP set as %d \n",SetVal));
			mcDBG_MSG_L2(("VideoStandard=%d\n",VideoStandard));

            if(SetVal<=255)
            {
                if(VideoStandard== MOD_DVBT)
                {
                    DVBT_IF_AMP=SetVal;
                    api->prop->dtv_lif_out.amp =DVBT_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L1(("DVBT_IF_AMP set as %d \n",DVBT_IF_AMP));
                }
                else if(VideoStandard== MOD_DVBT2)
                {
                    DVBT2_IF_AMP=SetVal;
                    api->prop->dtv_lif_out.amp =DVBT2_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L2(("DVBT2_IF_AMP set as %d \n",DVBT2_IF_AMP));
                }
                else if(VideoStandard== MOD_DVBC)
                {
                    DVBC_IF_AMP=SetVal;
                    api->prop->dtv_lif_out.amp =DVBC_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L1(("DVBC_IF_AMP set as %d \n",DVBC_IF_AMP));
                }
                else if((VideoStandard== MOD_SECAM_L)||(VideoStandard==MOD_SECAM_L1))
                {
                    L_ANALOG_IF_AMP=SetVal;
                    api->prop->atv_lif_out.amp =L_ANALOG_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L2(("L_ANALOG_IF_AMP set as %d \n",NO_L_ANALOG_IF_AMP));
                }
                else if((VideoStandard==MOD_J83B_64QAM)||(VideoStandard==MOD_J83B_256QAM)||(VideoStandard==MOD_J83B_AUTO_QAM))
                {
                    ATSC_QAM_IF_AMP=SetVal;
                    api->prop->dtv_lif_out.amp =ATSC_QAM_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L2(("QAM NO_L_DIGITAL_IF_AMP set as %d \n",api->prop->dtv_lif_out.amp));
                }
                else if((VideoStandard==MOD_ATSC_8VSB))
                {
                    ATSC_8VSB_IF_AMP=SetVal;
                    api->prop->dtv_lif_out.amp =ATSC_8VSB_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_DTV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_DTV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L2(("8VSB NO_L_DIGITAL_IF_AMP set as %d \n",api->prop->dtv_lif_out.amp));
                }
                else if((VideoStandard>=MOD_ANA_TYPE_BEGIN)||(VideoStandard<MOD_ANA_TYPE_END))
                {
                    NO_L_ANALOG_IF_AMP=SetVal;
                    api->prop->atv_lif_out.amp =NO_L_ANALOG_IF_AMP;
                    if (Si2157_L1_SetProperty2(api, Si2157_ATV_LIF_OUT_PROP) != 0)
                    {
                        mcDBG_MSG_ERR((" [Silabs]: ERROR_SETTING_PROPERTY Si2157_ATV_LIF_FREQ_PROP\n"));
                    }
                    mcDBG_MSG_L2(("NO_L_ANALOG_IF_AMP set as %d \n",NO_L_ANALOG_IF_AMP));
                }
                else
                    mcDBG_MSG_L2(("TVStandard unknown pls set a channel at first"));
            }
        }
        break;
    case itGetTunerAFCKhz:
        {
            #if 0
            unsigned char intack=Si2157_ATV_STATUS_CMD_INTACK_OK;
            S32 TunerAFCKhz = 5000;
            if ((Si2157_L1_ATV_STATUS(api,intack) != 0))
            {
                 mcDBG_MSG_ERR(("[Silabs]:Error reading STATUS\n"));
            }
            else
            {
                //if(api->status->atvint)
                if(api->rsp->atv_status.pcl)  /*wenming 11-11-25: for weak signal loss issue, side effect: scan may slow down*/
                {
                  TunerAFCKhz = (S32)api->rsp->atv_status.afc_freq;
                  mcDBG_MSG_L2(("[Silabs]:Tuner ATV lock and TunerAFCKhz = %d\n",TunerAFCKhz));
                }
                else
                {
                  mcDBG_MSG_L2(("[Silabs]:Tuner ATV unlock set TunerAFCKhz = %d,to let demod go ,,\n",TunerAFCKhz));
                }
            }
            #endif
            S32 TunerAFCKhz = 0;
            *(S32*)pInOutVal=TunerAFCKhz;
        }
        break;
    #ifdef      Si2157_GET_PROPERTY_STRING
    case itGetReg:
        {
            UINT8 ucTemp = 0;
            UINT8 ucDumpSettingCount = 0;

            SiTRACE ("Si2157_itGetReg ...........\n");

            ucDumpSettingCount = sizeof(aui2Si2157Properties)/sizeof(unsigned int);
            mcDBG_MSG_L1(("\n"));
            for(ucTemp=0;ucTemp<ucDumpSettingCount;ucTemp++)
            {
                Si2157_L1_GetPropertyString(api,aui2Si2157Properties[ucTemp],(char*)"\n",acSi2157ProMsg);
                mcDBG_MSG_L1(("%s\n",acSi2157ProMsg));
            }
        }
        break;
    #endif
	case itGetAnaFreqBound:
        {
            TUNER_ANA_ATTRIBUTE_T   *psTunerAnaAttribute;

            psTunerAnaAttribute = (TUNER_ANA_ATTRIBUTE_T *) pInOutVal;
            psTunerAnaAttribute->ui4_lower_bound_freq = C_Si2157_FREQ_DBOUND_LOWER_Ana;
            psTunerAnaAttribute->ui4_upper_bound_freq = C_Si2157_FREQ_DBOUND_UPPER_Ana;
        }
        break;
	case itSetRSetting:
		   {
			   switch(SetVal)
				   {
			   #if AD_R_setting    //have IF attenuator
					   case MOD_PAL_BG: 
					   case MOD_PAL_DK: 
					   case MOD_PAL_I:
					   case MOD_NTSC_M:
						   *(UINT32 *)pInOutVal=0x06 ;
						   break;
					   case MOD_SECAM_L:
					   case MOD_SECAM_L1:
						   *(UINT32 *)pInOutVal=0x07 ;
						   break;
					   case MOD_ATSC_8VSB:
					   case MOD_J83B_64QAM:
					   case MOD_J83B_256QAM:
					   case MOD_J83B_AUTO_QAM:
						   *(UINT32 *)pInOutVal=0x06 ;
						   break;
					   case MOD_DVBT:
					   case MOD_DVBC:
					   case MOD_DTMB:
						   *(UINT32 *)pInOutVal=0x06 ;
						   break;
			   #else			   //No IF attenuator
					   case MOD_PAL_BG: 
					   case MOD_PAL_DK: 
					   case MOD_PAL_I:
					   case MOD_NTSC_M:
						   *(UINT32 *)pInOutVal=0x00 ;
						   break;
					   case MOD_SECAM_L:
					   case MOD_SECAM_L1:
						   *(UINT32 *)pInOutVal=0x00 ;
						   break;
					   case MOD_ATSC_8VSB:
					   case MOD_J83B_64QAM:
					   case MOD_J83B_256QAM:
					   case MOD_J83B_AUTO_QAM:
						   *(UINT32 *)pInOutVal=0x00 ;
						   break;
					   case MOD_DVBT:
					   case MOD_DVBC:
					   case MOD_DTMB:
						   *(UINT32 *)pInOutVal=0x00 ;
						   break;
			   #endif
					   default:
						   *(UINT32 *)pInOutVal=0x06 ;
						   break;
	
			   }
				mcDBG_MSG_L1(("Tuner R_setting Value=%d\n", *(UINT32 *)pInOutVal));
		   }
		   break;

    default:
        return ITUNER_NOT_SUPPORTED;
    }

    return ITUNER_OK;
}
