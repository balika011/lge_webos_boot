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
 * $RCSfile: pi_dvbc_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pi_dvbc_if.h
 *  DVB-T Demod pi primitive API.
 */

#ifndef PI_DVBC_IF_H
#define PI_DVBC_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "u_handle.h"
#include "dvbc_tuner_if.h"
#include "tuner_interface_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define cMAX_TS_CNT     40
//#define SEMA_T                      CSingleLock *


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
enum
{
    DVBC_DEMOD_STATE_UNLOCK = 0,
    DVBC_DEMOD_STATE_LOCK,
    DVBC_DEMOD_STATE_UNSTABLE,
    DVBC_DEMOD_STATE_UNKNOWN
};

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct _DEMOD_CTX_T
{
// Hardware Configure
    UINT8           I2cAddress;
    INT32           Frequency ;             /* carrier frequency (in Hz)   */
    UINT8           u1ChipGen;
    UINT8           u1ChipRev;
    UINT32          SymbolRate;             /* Symbol Rate (in Baud)       */
    UINT8           Modulation;             /* Modulation (8VSB, 64QAM, 256QAM) */

    // Add for DVB-T. HUI
    UINT8           ucEnFpga;
    UINT8           u1Bw;
    UCHAR           ucConnType;

// Status
    UINT16          u2AcqTime;
    UINT8           u1QSpecInv;
    BOOL            fgAutoQam;
    UINT16          u2PecRefreshTime;
    UINT32          u4PecObserveTime;
    UINT8           u1PecErrSec;
    UINT8           u1UpChkCnt;
    UINT8           u1UpNakCnt;
    UINT8           u1UpMisCnt;
    UINT8           u1TunerAcqType;

    INT32           s4Cfo;
    INT32            s4QCR_CFO;
    INT32           s4TimingOffset;
    UCHAR           m_TunerMode;
    BOOL            fgTsLock;
    INT16           s2Snr;   // SNR*512
    UINT16          u2ErrorPkt;
    UINT16          u2TotalPkt;
    UINT32          u4CorrBit;
    //lei140701
    UINT32          u4UecPktErr;//Acc error packet
    UINT32          u4UecErrCnt;//unlock counter
    INT8            s1IfAgc;
    INT8            s1RfAgc;
    UINT8           ucTsIfFormat;
    UINT8           ucTsIfCtrl;
    UINT8           ucTsIfRate;
    UINT8           u1ChSearchMode;

    struct _DEMOD_CTX_T *psNextDemodCtx;
    //RF_TUNER_CTX_T  sTunerCtx;
    ITUNER_CTX_T*   psTunerCtx;
    void            *psI2cCtx;
    //SEMA_T          hHostCmdLock;
    //SEMA_T          hRiscIfLock;
    //SEMA_T          hRiscIfAtdLock;
    //SEMA_T          hLogLock;
    //SEMA_T          hDemodAcq;
    UINT32          u4DrvCustTunerCtrl;
    UINT8           u1DrvCustTunerDefType;

    /* Add for MT5135. */
    BOOL            bHighSampling;
    UINT32             u4SysClk;
    UCHAR           ucRegion;
    UCHAR           ucQamsize;
    UINT32             u4SymbolRate;
    UINT8           ucRSHState;
    UINT8           ucPraLock;
    UINT8           ucQNIRState;
    UINT8           ucQTRState;
    UINT8           ucQCRFftState;
    UINT8           ucQCRState;
    INT16           s2AgcGain;
    UINT8           ucQamMode;
    UINT16          u2SymbolRate;
    UINT8           fgAsrReady;
    UINT8           ucDsmSel;
    UINT8           ucCurrentAcqState;
    UINT8           ucSysCtrlState;
    UINT8           ucQcnState;
    UINT8           ucQcrReturnRC;
    UINT8           ucFftSearchResult;
    UINT8           ucPraState;
    UINT8           ucBNflag;
    INT32           s4ASR_CFO;
    UINT8           ucASR_CFOflag;
    UINT8           ucDev_AWGN_PN;
    UINT8           ucDev_AWGN;
    UINT8           ucCor_AWGN;
    UINT8           ucAQM_Seq[5];
    UINT8           ucAQM_ID;
    UINT16          u2JTD_Result;
    UINT8           ucSramSize;
    UINT8           ucASRFailState;
    UINT8           ucQCNFailState;
    UINT8           ucReAcqCnt;
    // sawless PRA state
    UINT8           ucSldPraState;
    INT16           s2SldPraRfagcCtrlWord;
    INT16           s2SldPraIfagcCtrlWord;
    UINT8           ucSldPraGainIdx;
    INT16           s2SldPraOut;

    // RF AGC
    INT8            m_s1AciPow;
    INT8            m_s1InbPow;

	//Symbol rate
	UINT16          u2SymbolrateMax;
	UINT16          u2SymbolrateMin;

    INT32           i4BerTh;
	INT32			i4TFO;
} DEMOD_CTX_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------- 
/** vDVBCGetInfo
 *  Get demod information and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateStatus(DEMOD_CTX_T *psDemodCtx);


//----------------------------------------------------------------------------- 
/** DVBC_UpdateSymbol
 *  Get demod information and update demod symbol rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void DVBC_UpdateSymbol(DEMOD_CTX_T *psDemodCtx);


//---------------------------------------------------------------------------------
void DVBC_SetIF(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_Initialize
 *  Initialize demod context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  ucI2cAddress  Demod I2C address.
 *  @param  fgLoadCode  Flag to check if need to downlaod firmware code. Unused.
 *  @retval   DRVAPI_TUNER_OK : OK.
 *  @retval   DRVAPI_TUNER_ERROR : Fail.   
 */
//-----------------------------------------------------------------------------
INT32  DVBC_Initialize(DEMOD_CTX_T *psDemodCtx, UINT8 u1I2cAddress, BOOL fgLoadCode);

//----------------------------------------------------------------------------- 
/** DVBC_UpdateInfo
 *  Get demod status, information and system parameters 
 *  and update demod related context.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBC_UpdateInfo(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_DriverStop
 *  Stop demod driver in firmware.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBC_DriverStop(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_SetTsIf
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
void   DVBC_SetTsIf(DEMOD_CTX_T *psDemodCtx, UCHAR tsif_format, UCHAR tsif_ctrl, UCHAR tsif_rate);

//----------------------------------------------------------------------------- 
/** DVBC_GetSignalLevel
 *  Get signal Level.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) 0-100.
 */
//-----------------------------------------------------------------------------
UINT8  DVBC_GetSignalLevel(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_GetSignalSNR
 *  Get signal SNR.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (UINT8) SNR in 0.1dB.   
 */
//-----------------------------------------------------------------------------
UINT16  DVBC_GetSignalSNR(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_GetPER
 *  Get TS packet error rate.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   (INT32) PER*10^5.   
 */
//-----------------------------------------------------------------------------
INT32  DVBC_GetPER(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_GetTsLock
 *  Get TS lock.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   TRUE : Lock.
 *  @retval   FALSE : Unlock.   
 */
//-----------------------------------------------------------------------------
BOOL   DVBC_GetTsLock(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/*
 *  DVBC_GetPreVBer
 *  Get pre Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Pre Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPreVBer(DEMOD_CTX_T *psDemodCtx);

// ----------------------------------------------------------------------------- 
/** DVBC_GetPostVBer
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^5.   
 */
//-----------------------------------------------------------------------------
INT32  DVBC_GetPostVBer(DEMOD_CTX_T *psDemodCtx);
// ----------------------------------------------------------------------------- 
/** DVBC_GetPostVBer_E7
 *  Get post Viterbi BER.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval   Post Viterbi BER * 10^7.   
 */
//-----------------------------------------------------------------------------
INT32 DVBC_GetPostVBer_E7(DEMOD_CTX_T *psDemodCtx);
//----------------------------------------------------------------------------- 
/** DVBC_GetSearchParam
 *  Get and update system parameters of channel scan or channel up/down mode.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  mdMode  Pointer of context created by DVBC_DemodCtxCreate.
 *  @param  tpsCr  Pointer of code rate value.
 *  @param  tpsCons  Pointer of TPS value.
 *  @param  tpsHier  Pointer of hierarchical mode value.
 *  @param  freqBias_40kHz  Pointer of frequency offset in 40kHz .
 *  @retval   void.   
 */
//-----------------------------------------------------------------------------
void   DVBC_GetSearchParam(DEMOD_CTX_T *psDemodCtx, UCHAR *mdMode, UCHAR *tpsCr, 
                           UCHAR *tpsCons, UCHAR *tpsHier, INT8 *freqBias_40kHz);

//----------------------------------------------------------------------------- 
/** DVBC_GetTotPkt
 *  Get total TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)total TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetTotPkt(DEMOD_CTX_T *psDemodCtx);


//----------------------------------------------------------------------------- 
/** DVBC_GetErrPkt
 *  Get error TS packets.
 *  @param  psDemodCtx  Pointer of context created by DVBC_DemodCtxCreate.
 *  @retval  (UINT32)error TS packet count.   
 */
//-----------------------------------------------------------------------------
UINT32 DVBC_GetErrPkt(DEMOD_CTX_T *psDemodCtx);

//----------------------------------------------------------------------------- 
/** DVBC_SetTSInterface
 *  Set TS interface
 *  @param  fgParallel  set interface to parallel or serial mode. TRUE: Parallel, FALSE: serial
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTSInterface(BOOL fgParallel);

//----------------------------------------------------------------------------- 
/** DVBC_SetTSPath
 *  Set TS routing path
 *  @param  fgEnable  set TS path to tri-state or pass-through. TRUE: pass-through, FALSE: tri-state
 *  @retval   void.
 */
//-----------------------------------------------------------------------------
void DVBC_SetTSPath(BOOL fgEnable);


UINT8 DVBC_GetSync(DEMOD_CTX_T *psDemodCtx);
VOID DVBC_SetTsOutput(DEMOD_CTX_T *psDemodCtx, BOOL fgOutputEnable);
UINT8 DVBC_ChipInit(DEMOD_CTX_T *psDemodCtx);
BOOL DVBC_FwSwitch(DEMOD_CTX_T *psDemodCtx);
UINT8 DVBC_SetNormalMode(DEMOD_CTX_T *psDemodCtx);
void DVBC_SetIfAgcCurve(DEMOD_CTX_T *psDemodCtx);
void DVBC_SetQamMode(DEMOD_CTX_T *psDemodCtx, UINT8 Mode);
void DVBC_SetSymbolRate(DEMOD_CTX_T *psDemodCtx, UINT16 Rate);
void DVBC_DriverStart(DEMOD_CTX_T *psDemodCtx);
void DVBC_AcqStart(DEMOD_CTX_T *psDemodCtx);
void DVBC_AdvAQM(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvAQM);
void DVBC_AdvASR(DEMOD_CTX_T *psDemodCtx, BOOL bfgAdvASR);
void DVBC_SetTsOutputMode(DEMOD_CTX_T *psDemodCtx, BOOL fgParallel);
VOID DVBC_DisConnect(DEMOD_CTX_T *psDemodCtx);
INT32 DVBC_GetCFO(DEMOD_CTX_T *psDemodCtx);
void DVBC_SetEEPromBrdcstType(DEMOD_CTX_T *psDemodCtx);
UINT8 DVBC_GetEEPromBrdcstType(DEMOD_CTX_T *psDemodCtx);
UCHAR DVBC_GetTsFormat(DEMOD_CTX_T *psDemodCtx);
VOID DVBC_SetTsFormat(DEMOD_CTX_T *psDemodCtx, UCHAR u1TsFmt);
UINT8 DVBC_GetSQI(DEMOD_CTX_T *psDemodCtx);

void DVBC_SetTsClkFreq(DEMOD_CTX_T *psDemodCtx, UCHAR ucManual);
void DVBC_Set_R_Setting(DEMOD_CTX_T *psDemodCtx);


BOOL DVBC_GetTCSwitch(DEMOD_CTX_T *psDemodCtx);
void DVBC_SetTCSwitch(DEMOD_CTX_T *psDemodCtx, BOOL fgTCSwitchFlag);

#if defined(CC_MT5136) 
    void DVBC_DisConnect_HWConfig(DEMOD_CTX_T *psDemodCtx);
    void DVBC_Connect_HWConfig(DEMOD_CTX_T *psDemodCtx);
#endif

//liuqu,AUTO ATP,20090302
#ifdef EU_AUTO_ATP
extern UINT8 gATP_TEST_ITEM_BIT ;
extern UINT8 gATPFailBitMsk ;
#endif
#endif /* PI_DVBC_IF_H */

