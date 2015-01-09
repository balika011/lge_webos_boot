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
 * $RCSfile: cec_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file cec_if.c
 *  This C file implements the CEC hardware protocol functions.  
 */
//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "cec_drv_if.h"
//#define CEC_DRV_VERIFY
#ifdef SYS_MHL_SUPPORT
#include "cbus_if.h"
#include "mhl_if.h"
#ifdef CC_MT5882
#include "drv_cbus_cpi_if.h"
#else
#include "drv_cbus_if.h"
#endif
#endif
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
// CEC Buffer Management.
#define RX_Q_SIZE 32
#ifdef CC_DELAY_CEC_MESSAGE_5VIN_HPDLOW
#define TX_Q_SIZE 64
#else
#define TX_Q_SIZE 32
#endif
#ifdef SYS_MHL_SUPPORT
UINT8 buf1[16] = {0x03, 0xA7, 0xA1, 0, 0, 0 ,0 ,0 , 0, 0, 0, 0, 0, 0, 0, 0};
extern UINT8 u1CbusLa;
#endif
#define PENDING_MSG_SIZE 10
#define CEC_MAX_MESG_SIZE 16

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define EnCecInt      vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_CEC_INT0EN)
#define DisCecInt      vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x0, FLD_CEC_INT0EN)

#define IS_RX_Q_EMPTY() (CEC_rxQ_read_idx == CEC_rxQ_write_idx)
#define IS_RX_Q_FULL() (((CEC_rxQ_write_idx+1)%RX_Q_SIZE) == CEC_rxQ_read_idx)

#define IS_TX_Q_EMPTY() (CEC_txQ_read_idx == CEC_txQ_write_idx)
#define IS_TX_Q_FULL() (((CEC_txQ_write_idx+1)%TX_Q_SIZE) == CEC_txQ_read_idx)

#if defined(CC_MT5387) || defined(CC_MT5363)
#define ENABLE_PINMUX_TO_CEC()
#elif defined(CC_MT5881)
#define ENABLE_PINMUX_TO_CEC() BSP_PinSet(PIN_HDMI_CEC, 2)
#define Get_CEC_PIN_FUNCTION() BSP_PinGet(PIN_HDMI_CEC)
#else
#ifndef CC_ENABLE_HDMI_JTAG // deticated JTAG pin
#define ENABLE_PINMUX_TO_CEC() BSP_PinSet(PIN_HDMI_CEC, 1)
#define Get_CEC_PIN_FUNCTION() BSP_PinGet(PIN_HDMI_CEC)
#else 
#define ENABLE_PINMUX_TO_CEC() BSP_PinSet(PIN_HDMI_CEC, 2)
#define Get_CEC_PIN_FUNCTION() BSP_PinGet(PIN_HDMI_CEC)
#endif
#endif

#ifdef CC_DELAY_CEC_MESSAGE_5VIN_HPDLOW
#define fgCecHdmi1CanBeSend() \
    ((bHDMISwitch5VStatus(HDMI_SWITCH_1)==0) ? \
    1: bHDMISwitchHPDStatus(HDMI_SWITCH_1))

#define fgCecHdmi2CanBeSend() \
    ((bHDMISwitch5VStatus(HDMI_SWITCH_2)==0) ? \
    1: bHDMISwitchHPDStatus(HDMI_SWITCH_2))

#define fgCecHdmi3CanBeSend() \
    ((bHDMISwitch5VStatus(HDMI_SWITCH_3)==0) ? \
    1: bHDMISwitchHPDStatus(HDMI_SWITCH_3))

#define fgCecHdmi4CanBeSend() \
    ((bHDMISwitch5VStatus(HDMI_SWITCH_4)==0) ? \
    1: bHDMISwitchHPDStatus(HDMI_SWITCH_4))
    
#define fgCecMessageCanBeSend() \
    (fgCecHdmi1CanBeSend() && fgCecHdmi2CanBeSend() && \
     fgCecHdmi3CanBeSend() && fgCecHdmi4CanBeSend())
#endif

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------
UINT16 _CEC_Status;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

#ifdef CC_DELAY_CEC_MESSAGE_5VIN_HPDLOW
extern UINT8 bHDMISwitch5VStatus(UINT8 ucPort);
extern UINT8 bHDMISwitchHPDStatus(UINT8 ucPort);
#endif
extern UINT8  _bIsMhlDevice;
//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT8 u1CecTVLogicAddr =0x0;
CEC_TV_LA_t CecLa = {0xf};
UINT8 u1StandbyCtr=0;
UINT8 u1WakeUpCtr=0;
UINT8 u1StadardCmdCtr=0;
UINT8 u1SysCtrl =0;
//UINT32 u4CecArg =0;
UINT32 u4CecTxCbArg = 0;
UINT32 u4CecRxCbArg = 0;
UINT32 u4Cec5VCbArg = 0;
UINT32 u4CecTxHCbArg = 0;

UINT8 u1CecDbgLvl = 0;
UINT8 u1CecLA0AckCtr = 0;
static BOOL fgCecDrvReady = FALSE;

static CEC_FRAME_DESCRIPTION *prActiveTxFrame;
static CEC_FRAME_DESCRIPTION *prActiveRxFrame;
static UINT32 u4CecRxTag = 0;

// rx circular buffer.
static CEC_FRAME_DESCRIPTION CEC_rx_msg_queue[RX_Q_SIZE];
static UINT8 CEC_rxQ_read_idx;
static UINT8 CEC_rxQ_write_idx;

// tx circular buffer.
static CEC_FRAME_DESCRIPTION CEC_tx_msg_queue[TX_Q_SIZE];
static UINT8 CEC_txQ_read_idx;
static UINT8 CEC_txQ_write_idx;
#ifdef LG_CEC
#define RX_Q_SIZE_MTAL 32
static UINT8 CEC_rxQ_read_idx_mtal;
static UINT8 CEC_rxQ_write_idx_mtal;
static CEC_FRAME_DESCRIPTION CEC_rx_msg_queue_mtal[RX_Q_SIZE_MTAL];

#define TX_Q_SIZE_MTAL 32
static UINT8 CEC_tx_result_read_idx_mtal;
static UINT8 CEC_tx_result_write_idx_mtal;
static CEC_TX_RESULT CEC_tx_result_queue_mtal[TX_Q_SIZE_MTAL];

static UINT16 ui2_cecResponseMtal;
#endif
#ifndef LG_CEC
static BOOL fgUICECEnable = FALSE;
static BOOL fgCecMwReady = FALSE;
#else
static BOOL fgUICECEnable = TRUE;
static BOOL fgCecMwReady = TRUE;
#endif
//cec Arc enable
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)
//#if defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389) ||defined(CC_MT5398) ||defined(CC_MT5880) || defined(CC_MT5881)
//static BOOL fgCecArcEnable = FALSE;
static void _Cec_SetArcEnable(BOOL fgEnable);
//#endif
#endif
static x_cec_drv_nfy_data_arrival rCecRxHandler;
static x_cec_drv_nfy_tx_result rCecTxHandler;
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
static x_cec_drv_nfy_tx_header_result rCecTxHeaderHandler;
#endif
#ifdef CECNFY5VSTATUS
static x_cec_drv_nfy_5v_status rCec5vHandler;
static UINT8 bPre5VStatus1 = 0;
static UINT8 bPre5VStatus2 = 0;
static UINT8 bPre5VStatus3 = 0;
#if	defined(CC_MT5399) || defined(CC_MT5890)
static UINT8 bPre5VStatus4 = 0;
#endif
#endif

#ifdef CTS_TEST
static CEC_FRAME_DESCRIPTION rCtsTestFrame;
#endif

static CEC_FRAME_ACTSRCREVED rCecActiveSource;
static HANDLE_T phCecThreadHdl;
static HANDLE_T phCecInitThreadHdl;
static HANDLE_T _hCECSema = NULL_HANDLE;

static CEC_FRAME_DESCRIPTION test_txframe;
static UINT8 bInitFinish = 0;//flag for cec init finish
void CEC_HwInit(void);
static BOOL bBootUp = FALSE;

UINT8 u1IsTX_Q_Empty(void);
UINT8 u1IsRX_Q_Empty(void);

#ifdef CC_FOR_POC_DEMO
x_philips_cec_drv_nfy_5v_status pPhilips5VNfyfunc = NULL;
#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifndef LG_CEC
static void _Cec_NotifyTxResult(UINT8 result, CEC_FRAME_DESCRIPTION *frame,void *arg)
{
    if (rCecTxHandler)
    {
        rCecTxHandler(result, frame,arg);
    }
}

void _Cec_NotifyRxResult(CEC_FRAME_DESCRIPTION *frame)
{
	frame->txtag =(void *)u4CecRxCbArg;
    if (rCecRxHandler)
    {
        rCecRxHandler(frame);
    }
}
#endif
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
void _Cec_NotifyTxHeaderResult(UINT8 result, CEC_FRAME_DESCRIPTION *frame,void *arg)

{
    if (rCecTxHeaderHandler)
    {
        rCecTxHeaderHandler(result, frame,(void *)arg);
    }
}
#endif
static UINT8 _Cec_SendFrame(CEC_FRAME_DESCRIPTION *frame)
{
    UINT8 size;
    UINT8 *sendidx;
    UINT8 *blocks;
    UINT32 u4Data;
    UINT8 i, j;
	//LOG(0, "_Cec_SendFrame................\n"); 
    if (!frame)
    {
        LOG(0, "_Cec_SendFrame: frame null.\n");   
        return CEC_ERROR;
    }

    size = frame->size;
    if ((size == 0) || (size > 16))
    {
        LOG(0, "_Cec_SendFrame: Size error.\n");   
        return CEC_ERROR;
    }

    prActiveTxFrame = frame;
    sendidx = &(frame->sendidx);
    blocks = &(frame->blocks.opcode);

    // CYJ.NOTE: Leave "TX hardware error handling" to CEC_Mainloop
    if ((!IS_TX_FSM_IDLE()) ||(TX_FAIL_RECORD() > 0))
    {
        LOG(0, "_Cec_SendFrame: Detect TX FAIL.\n");
        RESET_HW_TX();
    }

    SetCECStatus(STATE_TX_SEND_FRAME);

    /* fill header */
    FILL_SRC_FIELD(frame->blocks.header.initiator);
    FILL_DST_FIELD(frame->blocks.header.destination);
    size -= 1;

    /* header-only:
         A message with the EOM bit set in the header block can be used to 
         'ping' other devices, to ascertain if they are powered on.
    */     
    if (size == 0)
    {
        SET_HW_TX_LEN(0);
		vUtDelay2us(1);
        MARK_H_EOM(1);
		vUtDelay2us(1);
        MARK_D_EOM(0);
    }

    /* fill data */
    if (size > 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(0);
    }
    else if (size == 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 3)
    {
        SET_HW_TX_LEN(0x7);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 2)
    {
        SET_HW_TX_LEN(0x3);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 1)
    {
        SET_HW_TX_LEN(0x1);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
	
    LOG(5, "_Cec_SendFrame: data size=%d.\n", size);
    
    u4Data = 0;
	if((size == 0) && (IO32ReadFldAlign(TX_HD_NEXT,WTX_H_EOM) == 0))
	{
		LOG(0,"Test,retry writting the Header EOM!\n");
		MARK_H_EOM(1);
	}
    for (i = 0, j = size; i < 4; i++)
    {
        u4Data >>= 8;

        u4Data |= (UINT32)((blocks[( *sendidx)]) << 24);
        if (i < j)
        {
            (*sendidx)++;
            size--;
        }
    }

    /* EOM */
    if (size == 0)
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(0);
        ENABLE_INT_LOW(0);
        ENABLE_INT_UN(0);
        ENABLE_INT_BS(0);
    }
    else
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(1);
        ENABLE_INT_LOW(1);
        ENABLE_INT_UN(1);
        ENABLE_INT_BS(0);

        SetCECStatus(STATE_TX_WAIT_DATA_TAKEN);
    }

    FILL_TX_DATA(u4Data);

    // CYJ.NOTE: RB_RDY interrrupt is triggered immediately after TRIGGER_TX_HW()
    // CYJ.NOTE: DONOT NEED "CRITICAL SECTION" because no access to shared resources follows TRIGGER_TX_HW()

    CLR_TX_FINISH();
    TRIGGER_TX_HW();

    LOG(5, "_Cec_SendFrame: TRIGGER_TX_HW.\n");

    return CEC_SUCCESS;
}

static UINT8 _Cec_SendNextDataFrame(void)
{
    CEC_FRAME_DESCRIPTION *frame;
    UINT8 size;
    UINT8 *sendidx;
    UINT8 *blocks;
    UINT32 u4Data;
    UINT8 i, j;

    // CYJ.NOTE: Leave "TX hardware error handling" to CEC_Mainloop
    if (IS_TX_FSM_FAIL() || (TX_FAIL_RECORD() > 0))
    {
        LOG(0, "_Cec_SendRemain: Detect TX FAIL.\n");
        return CEC_ERROR;
    }

    if (!IsCECStatus(STATE_TX_WAIT_DATA_TAKEN))
    {
        LOG(0, "_Cec_SendRemain: state error.\n");
        return CEC_ERROR;
    }

    ClrCECStatus(STATE_TX_WAIT_DATA_TAKEN);

    /* request current active TX frame */
    frame = prActiveTxFrame;

    size = frame->size;
    sendidx = &(frame->sendidx);
    blocks = &(frame->blocks.opcode);
    size -= (( *sendidx)+1);

    if (size == 0)
    {
        LOG(0, "_Cec_SendNextDataFrame: size error.\n");    
        return CEC_ERROR;
    }

    /* CYJ:TODO duplicate (as _Cec_SendFrame())! */
    /* fill data */
    if (size > 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(0);
    }
    else if (size == 4)
    {
        SET_HW_TX_LEN(0xf);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 3)
    {
        SET_HW_TX_LEN(0x7);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 2)
    {
        SET_HW_TX_LEN(0x3);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }
    else if (size == 1)
    {
        SET_HW_TX_LEN(0x1);

        MARK_H_EOM(0);
        MARK_D_EOM(1);
    }

    LOG(5, "_Cec_SendNextDataFrame: size=%d.\n", size);

    u4Data = 0;

    for (i = 0, j = size; i < 4; i++)
    {
        u4Data >>= 8;

        u4Data |= (UINT32)((blocks[( *sendidx)]) << 24);
        if (i < j)
        {
            (*sendidx)++;
            size--;
        }
    }

    /* EOM */
    if (size == 0)
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(0);
        ENABLE_INT_LOW(0);
        ENABLE_INT_UN(0);
        ENABLE_INT_BS(0);
    }
    else
    {
        ENABLE_INT_FAIL(1);

        ENABLE_INT_RB(1);
        ENABLE_INT_LOW(1);
        ENABLE_INT_UN(1);
        ENABLE_INT_BS(0);

        SetCECStatus(STATE_TX_WAIT_DATA_TAKEN);
    }

    FILL_TX_DATA(u4Data);

    CLR_TX_FINISH();
    TRIGGER_TX_HW();

    LOG(5, "_Cec_SendNextDataFrame: TRIGGER_TX_HW.\n");

    return CEC_SUCCESS;
}

static void _Cec_InitQueue(void)
{
    CEC_rxQ_write_idx = 0;
    CEC_rxQ_read_idx = 0;
    CEC_txQ_write_idx = 0;
    CEC_txQ_read_idx = 0;
#ifdef LG_CEC
    CEC_rxQ_read_idx_mtal = 0;
    CEC_rxQ_read_idx_mtal = 0;
    CEC_tx_result_read_idx_mtal = 0;
    CEC_tx_result_write_idx_mtal = 0;
#endif
}

static CEC_FRAME_DESCRIPTION *_Cec_GetRxWriteFrame(void)
{
    if (IS_RX_Q_FULL())
    {
        LOG(0, "rx queue full.\n");
        return NULL;
    }

    if (CEC_rxQ_write_idx >= RX_Q_SIZE)
    {
        LOG(0, "rx queue index error=%d.\n", CEC_rxQ_write_idx);
        return NULL;
    }
    
    return (&(CEC_rx_msg_queue[CEC_rxQ_write_idx]));
}

static CEC_FRAME_DESCRIPTION *_Cec_GetRxReadFrame(void)
{
    if (IS_RX_Q_EMPTY())
    {
        return NULL;
    }

    if (CEC_rxQ_read_idx >= RX_Q_SIZE)
    {
        LOG(0, "rx queue index error=%d.\n", CEC_rxQ_read_idx);
        return NULL;
    }
    
    return (&(CEC_rx_msg_queue[CEC_rxQ_read_idx]));
}

#ifdef LG_CEC
void _Cec_EnqueueRxMtal(CEC_FRAME_DESCRIPTION *prFrame)
{
    if((CEC_rxQ_write_idx_mtal + 1) %RX_Q_SIZE_MTAL== CEC_rxQ_read_idx_mtal)
    {
        LOG(0,"error: rx queue full (mtal) \n");
        return;
    }
    x_memcpy(&CEC_rx_msg_queue_mtal[CEC_rxQ_write_idx_mtal], prFrame, sizeof(CEC_FRAME_DESCRIPTION) );
    CEC_rxQ_write_idx_mtal = (CEC_rxQ_write_idx_mtal + 1) %RX_Q_SIZE_MTAL;
    return;
}

void _Cec_EnqueueTxResultMtal(UINT8 u1_result, CEC_FRAME_DESCRIPTION *prFrame)
{
    if((CEC_tx_result_write_idx_mtal+ 1) %TX_Q_SIZE_MTAL== CEC_tx_result_read_idx_mtal)
    {
        LOG(0,"error: tx result queue full (mtal) \n");
        return;
    }
    x_memcpy(&(CEC_tx_result_queue_mtal[CEC_tx_result_write_idx_mtal].tFrame), prFrame, sizeof(CEC_FRAME_DESCRIPTION) );
    CEC_tx_result_queue_mtal[CEC_tx_result_write_idx_mtal].ui1_txResult = u1_result;
    CEC_tx_result_write_idx_mtal = (CEC_tx_result_write_idx_mtal + 1) %RX_Q_SIZE_MTAL;
    return;
}
#endif

static void _Cec_EnqueueRx(CEC_FRAME_DESCRIPTION *prFrame)
{
    // AP need to know if active source is ever happened ?
    if (prFrame->blocks.opcode == (UINT32)OPCODE_ACTIVE_SOURCE)
    {
        rCecActiveSource.fgActSrcReved = TRUE;
#ifdef CC_FOR_POC_DEMO
x_memcpy((void *)&(rCecActiveSource.eFrameDescription),(void *)prFrame, sizeof(CEC_FRAME_DESCRIPTION));
rCecActiveSource.eFrameDescription.txtag = NULL;
#else
rCecActiveSource.u1PhyAddr[0] = prFrame->blocks.operand[0];
rCecActiveSource.u1PhyAddr[1] = prFrame->blocks.operand[1];
#endif		
    }

    CEC_rxQ_write_idx = (CEC_rxQ_write_idx + 1) % RX_Q_SIZE;   
    x_sema_unlock(_hCECSema);
}

static void _Cec_DequeueRx(void)
{
	    CEC_rxQ_read_idx = (CEC_rxQ_read_idx + 1) % RX_Q_SIZE;
}

static CEC_FRAME_DESCRIPTION *_Cec_GetTxReadFrame(void)
{
    if (IS_TX_Q_EMPTY())
    {
        return NULL;
    }

    if (CEC_txQ_read_idx >= TX_Q_SIZE)
    {
        LOG(0, "tx queue index error=%d.\n", CEC_txQ_read_idx);
        return NULL;
    }
    
    return (&(CEC_tx_msg_queue[CEC_txQ_read_idx]));
}

static void _Cec_DequeueTx(void)
{
	    CEC_txQ_read_idx = (CEC_txQ_read_idx + 1) % TX_Q_SIZE;
}

static void _Cec_ProcessRxIsr(void)
{
    UINT32 data;
    UINT8 i, rxlen, is_d_eom;
    UINT32 u4DstAddr;
        
    /* new incoming message */
    if (IsCECStatus(STATE_RX_GET_NEW_HEADER))
    {
        ClrCECStatus(STATE_RX_GET_NEW_HEADER);

        u4DstAddr = GET_DST_FIELD();
        if ((u4DstAddr != u1CecTVLogicAddr) &&
             (u4DstAddr != (UINT32)CEC_LOG_BROADCAST))
        {
            LOG(0, "Error RX Header, u4DstAddr=%d.\n", u4DstAddr);
            return;
        }

        if (IsCECStatus(STATE_RX_WAIT_COMPLETE))
        {
            LOG(0, "Error Lost EOM:2\n");
        }

        SetCECStatus(STATE_RX_WAIT_COMPLETE);
		if(u1CecDbgLvl == 2)
        {
        	LOG(0, "RX:Header, initiator=0x%X, destination=0x%X.\n",GET_SRC_FIELD(), u4DstAddr);
		}
        prActiveRxFrame = _Cec_GetRxWriteFrame();
        if (!prActiveRxFrame)
        {
            return;
        }

        prActiveRxFrame->blocks.header.initiator = GET_SRC_FIELD();
        prActiveRxFrame->blocks.header.destination = u4DstAddr;
        prActiveRxFrame->size = 1;
        
        // Rx do not use txtag, use this as a sequential driver tag.    
        prActiveRxFrame->txtag = (void *)(u4CecRxTag ++);        
    }

    if (!IsCECStatus(STATE_RX_WAIT_COMPLETE))
    {
        LOG(0, "Error Lost HEADER.\n");    
        return;
    }

    rxlen = GET_HW_RX_LEN();
    data = GET_HW_RX_DATA();
    is_d_eom = IS_RX_D_EOM();

    if (rxlen == 0x3)
    {
        rxlen = 2;
    }
    else if (rxlen == 0x7)
    {
        rxlen = 3;
    }
    else if (rxlen == 0xf)
    {
        rxlen = 4;
    }
    else if (rxlen != 0x1)
    {
        LOG(0, "Error rxlen = %d.\n", rxlen);
    }
    
    LOG(5, "rxlen = %d.\n", rxlen);

    if (!prActiveRxFrame)
    {
        LOG(0, "Error prActiveRxFrame=Null.\n");
        return;
    }

    // Get opcode
    if (prActiveRxFrame->size == 1)
    {
        prActiveRxFrame->blocks.opcode = data & 0xff;
        prActiveRxFrame->size++;
        data >>= 8;
        rxlen--;
    }

    // Get operand.
    for (i=0; i<rxlen; i++)
    {
        if ((prActiveRxFrame->size < 2) || (prActiveRxFrame->size >= 16))
        {
            LOG(0, "Error Receive Data Length = %d.\n", prActiveRxFrame->size);
            break;
        }
    
        prActiveRxFrame->blocks.operand[(prActiveRxFrame->size-2)] = data & 0xff;
        LOG(7, "Rx operand[%d] = 0x%X.\n", (prActiveRxFrame->size-2), (data & 0xff));
        prActiveRxFrame->size++;        
        data >>= 8;        
    }

    // Check if data complete.
    if (is_d_eom)
    {
        if (prActiveRxFrame->size < 16)
        {
            // Fill 0 to the unused operand.
            for (i=(prActiveRxFrame->size-2); i<14; i++)
            {
                prActiveRxFrame->blocks.operand[i] = 0;
            }
        }
        
        ClrCECStatus(STATE_RX_WAIT_COMPLETE);
        if(fgCecMwReady == TRUE)
        {
        // push into rx_queue.
        _Cec_EnqueueRx(prActiveRxFrame);
        }
        else
        if((prActiveRxFrame->blocks.opcode == OPCODE_ACTIVE_SOURCE)||
        	(prActiveRxFrame->blocks.opcode == OPCODE_REPORT_PHYSICAL_ADDRESS))
        {
        // push into rx_queue.
        _Cec_EnqueueRx(prActiveRxFrame);
        }
         prActiveRxFrame = NULL;
    }
}

static void _Cec_HandleTxResult(void)
{
    if (IsCECStatus(STATE_TX_RESEND))
    {
        ClrCECStatus(STATE_TX_RESEND);
        SetCECStatus(STATE_TX_NOACK);

        if (IS_TX_FSM_FAIL())
        {
            LOG(5, "TX NO ACK\n");
        }
        else
        {
            LOG(1, "other TX error.\n");
        }

        LOG(7, "HACK=%d, DACK=%d\n", GET_FOLLOWER_H_ACK(), GET_FOLLOWER_D_ACK());

        RESET_HW_TX();
        return;
    }

    if ((prActiveTxFrame->sendidx + 1) == (prActiveTxFrame->size))
    {
        if (IS_TX_FSM_IDLE() && IS_TX_FINISH())
        {
            DISABLE_ALL_TX_INT();
            SetCECStatus(STATE_TX_SEND_FRAME_OK);
            return;
        }
    }
}

#ifndef SYS_ANDROID_TV_TIF_SUPPORT
static UINT8 _Cec_CheckFrame(const CEC_FRAME_DESCRIPTION *frame)
{
    UINT8 size;
    UINT8 u1Ret = TRUE;

    if (!frame)
    {
        return FALSE;
    }

    size = frame->size;

    /* opcode-aware */
    /* CYJ.NOTE: code size issue */
    switch (frame->blocks.opcode)
    {
    // length == 2
    case OPCODE_ARC_INIT:
    case OPCODE_ARC_REPORT_INIT:    
    case OPCODE_ARC_REPORT_TERMINATE:    
    case OPCODE_ARC_REQUEST_INIT:
    case OPCODE_ARC_REQUEST_TERMINATE:
    case OPCODE_ARC_TERMINATE:
    case OPCODE_IMAGE_VIEW_ON:
    case OPCODE_TEXT_VIEW_ON:
    case OPCODE_REQUEST_ACTIVE_SOURCE:
    case OPCODE_STANDBY:
    case OPCODE_RECORD_OFF:
    case OPCODE_RECORD_TV_SCREEN:
    case OPCODE_GET_CEC_VERSION:
    case OPCODE_GIVE_PHYSICAL_ADDRESS:
    case OPCODE_GET_MENU_LANGUAGE:
    case OPCODE_TUNER_STEP_DECREMENT:
    case OPCODE_TUNER_STEP_INCREMENT:
    case OPCODE_GIVE_DEVICE_VENDOR_ID:
    case OPCODE_VENDOR_REMOTE_BUTTON_UP:
    case OPCODE_GIVE_OSD_NAME:
    case OPCODE_USER_CONTROL_RELEASED:
    case OPCODE_GIVE_DEVICE_POWER_STATUS:
    case OPCODE_ABORT:
    case OPCODE_GIVE_AUDIO_STATUS:
    case OPCODE_GIVE_SYSTEM_AUDIO_MODE_STATUS:
        if (size != 2)
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_SYSTEM_AUDIO_MODE_REQUEST:
        if ((size != 2) && (size != 4))
        {
            u1Ret = FALSE;
        }
        break;

    // length == 3
    case OPCODE_RECORD_STATUS:
    case OPCODE_TIMER_CLEARED_STATUS:
    case OPCODE_CEC_VERSION:
    case OPCODE_DECK_CONTROL:
    case OPCODE_DECK_STATUS:
    case OPCODE_GIVE_DECK_STATUS:
    case OPCODE_PLAY:
    case OPCODE_GIVE_TUNER_DEVICE_STATUS:
    case OPCODE_MENU_REQUEST:
    case OPCODE_MENU_STATUS:
    case OPCODE_REPORT_POWER_STATUS:
    case OPCODE_REPORT_AUDIO_STATUS:
    case OPCODE_SET_SYSTEM_AUDIO_MODE:
    case OPCODE_SYSTEM_AUDIO_MODE_STATUS:
    case OPCODE_SET_AUDIO_RATE:
        if (size != 3)
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_USER_CONTROL_PRESSED:
        if ((size != 3) && (size != 4))
        {
            u1Ret = FALSE;
        }
        break;

    // length == 4
    case OPCODE_ACTIVE_SOURCE:
    case OPCODE_INACTIVE_SOURCE:
    case OPCODE_ROUTING_INFORMATION:
    case OPCODE_SET_STREAM_PATH:
    case OPCODE_FEATURE_ABORT:
#ifdef SYS_MHL_SUPPORT
    case OPCODE_MHL_CONTENT_ON:
    case OPCODE_MHL_CONTENT_OFF:    
#endif	
        if (size != 4)
        {
            u1Ret = FALSE;
        }
        break;

    // length == 5
    case OPCODE_REPORT_PHYSICAL_ADDRESS:
    case OPCODE_SET_MENU_LANGUAGE:
    case OPCODE_DEVICE_VENDOR_ID:
        if (size != 5)
        {
            u1Ret = FALSE;
        }
        break;

    // length == 6
    case OPCODE_ROUTING_CHANGE:
    case OPCODE_SELECT_ANALOGUE_SERVICE:
        if (size != 6)
        {
            u1Ret = FALSE;
        }
        break;

    // length == 9
    case OPCODE_SELECT_DIGITAL_SERVICE:
        if (size != 9)
        {
            u1Ret = FALSE;
        }
        break;

    // length == 13
    case OPCODE_CLEAR_ANALOGUE_TIMER:
    case OPCODE_SET_ANALOGUE_TIMER:
        if (size != 13)
        {
            u1Ret = FALSE;
        }
        break;

    // length == 16
    case OPCODE_CLEAR_DIGITAL_TIMER:
    case OPCODE_SET_DIGITAL_TIMER:
        if (size != 16)
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_RECORD_ON:
        if ((size < 3) || (size > 10))
        {
            u1Ret = FALSE;
        }
        break;

    // length == 10 ~ 11
    case OPCODE_CLEAR_EXTERNAL_TIMER:
    case OPCODE_SET_EXTERNAL_TIMER:
        if ((size < 10) || (size > 11))
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_TIMER_STATUS:
        if ((size != 3) && (size != 5))
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_TUNER_DEVICE_STATUS:
        if ((size != 7) && (size != 10))
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_VENDOR_COMMAND:
    case OPCODE_VENDOR_COMMAND_WITH_ID:
    case OPCODE_VENDOR_REMOTE_BUTTON_DOWN:
        if (size > 16)
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_SET_OSD_STRING:
        if ((size < 3) || (size > 16))
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_SET_TIMER_PROGRAM_TITLE:
    case OPCODE_SET_OSD_NAME:
        if ((size < 3) || (size > 16))
        {
            u1Ret = FALSE;
        }
        break;

    case OPCODE_CDC_MESSAGE:
        u1Ret = TRUE;
        break;

     default:
        u1Ret = FALSE;
        break;
    }

    if (!u1Ret)
    {
        LOG(0, "Error size=%d, opcode=0x%X\n", size, frame->blocks.opcode);
        CEC_PrintFrame(frame, CEC_RX_FRAME);
    }

    return u1Ret;
}
#endif
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
static BOOL _Cec_CheckLA(const CEC_FRAME_DESCRIPTION *frame)
{
	
	if(frame->blocks.header.destination == 0xF)
	{
		return TRUE;
	}

	if((frame->blocks.header.destination == CecLa.tvLA1) || (frame->blocks.header.destination == CecLa.tvLA2))
	{
		return TRUE;
	}
	
	return FALSE;
}
#endif

#ifdef CTS_TEST

static void _Cec_HandleCTSRx(CEC_FRAME_DESCRIPTION *frame)
{
    if (frame->blocks.opcode == OPCODE_ABORT)
    {
        rCtsTestFrame.size = 4;

        rCtsTestFrame.sendidx = 0;
        rCtsTestFrame.reTXcnt = 0;
        rCtsTestFrame.txtag = NULL;
        rCtsTestFrame.blocks.header.destination = frame->blocks.header.initiator;
        rCtsTestFrame.blocks.header.initiator = CEC_LOG_TV;
        rCtsTestFrame.blocks.opcode = OPCODE_FEATURE_ABORT;
        rCtsTestFrame.blocks.operand[0] = OPCODE_ABORT;
        rCtsTestFrame.blocks.operand[1] = 4;

        /////////////////////// Test//////////////////////////////////
        rCtsTestFrame.blocks.operand[2] = 2;
        rCtsTestFrame.blocks.operand[3] = 3;
        rCtsTestFrame.blocks.operand[4] = 4;
        rCtsTestFrame.blocks.operand[5] = 5;
        rCtsTestFrame.blocks.operand[6] = 6;
        rCtsTestFrame.blocks.operand[7] = 7;
        rCtsTestFrame.blocks.operand[8] = 8;
        rCtsTestFrame.blocks.operand[9] = 9;
        rCtsTestFrame.blocks.operand[10] = 10;
        rCtsTestFrame.blocks.operand[11] = 11;
        rCtsTestFrame.blocks.operand[12] = 12;
        rCtsTestFrame.blocks.operand[13] = 13;
        ///////////////////////Test////////////////////////////////////

        CEC_EnqueueTx(&rCtsTestFrame);
        LOG(5, "CTS Send: OPCODE_FEATURE_ABORT\n");

        return;
    }

    if (frame->blocks.opcode == OPCODE_GIVE_PHYSICAL_ADDRESS)
    {
        rCtsTestFrame.size = 5;

        rCtsTestFrame.sendidx = 0;
        rCtsTestFrame.reTXcnt = 0;
        rCtsTestFrame.txtag = NULL;
        rCtsTestFrame.blocks.header.destination = CEC_LOG_BROADCAST;
        rCtsTestFrame.blocks.header.initiator = CEC_LOG_TV;
        rCtsTestFrame.blocks.opcode = OPCODE_REPORT_PHYSICAL_ADDRESS;
        rCtsTestFrame.blocks.operand[0] = 0;
        rCtsTestFrame.blocks.operand[1] = 0;
        rCtsTestFrame.blocks.operand[2] = 0;

        CEC_EnqueueTx(&rCtsTestFrame);
        LOG(5, "CTS Send: OPCODE_REPORT_PHYSICAL_ADDRESS\n");
        return;
    }
}
#endif /* CTS_TEST */
#ifndef LG_CEC
static void _Cec_Get8032Data(UINT8 *pu1Data, UINT32 u4Size)
{
    ASSERT(pu1Data != NULL);
    
// Get data from XDATA with 8032 address (Xdata base address + 0x200).
/*
	Xdata base address mapping table.
					ARM address <----------> 8032 address
	5301A/5881		   0x8000					 0xD000
----------------------------------------------------------
	5399/5890
	/5882/5861		   0x8000					 0xF400
----------------------------------------------------------
	Other			   0x8000					 0xF800		 
	@Bo,8032 address: defined as XDATA(xxxx-xxxx) in "system.cfg" where 53xx_com_driver\pdwncxxxx\T8032_src\workspace.
*/
    (void)_PDWNC_GetT8302CECData(XDATA_ADDR + CEC_XDATA_OFFSET, u4Size, pu1Data);
}

static void _Cec_Process8032Data(UINT32 u4MsgNum)
{
    UINT8 *pu1Data;
    UINT32 i;
    UINT32 u4Offset;    
    UINT32 u4Size;
    INT32 i4MsgNum;
    const UINT32 u48032FrameSize = 22; // CEC_FRAME_DESCRIPTION size at 8032 is 22 bytes.

    if ((u4MsgNum == 0) || (u4MsgNum > PENDING_MSG_SIZE))
    {
        return;
    }  
    
    u4Size = (u4MsgNum * u48032FrameSize) + 16;
    
    pu1Data = (UINT8 *)x_mem_alloc(u4Size);
    if (!pu1Data)
    {
        LOG(0, "8032Data x_mem_alloc fail.\n");   
        return;
    }

    _Cec_Get8032Data(pu1Data, u4Size); //from 0xfa00    

    // Init Rx tag with message length.
    u4CecRxTag = u4MsgNum;
    
    // 8032 XDATA at 0xFA10. 
    u4Offset = 0x10;
    i4MsgNum = (INT32)u4MsgNum;

    while (i4MsgNum > 0)        
    {
        u4Size = pu1Data[u4Offset];
        LOG(1, "(%d) 8032Data size: %d.\n", i4MsgNum, u4Size);

        // Skip if frame size = 0.
        if ((u4Size > 0) && (u4Size <= 16))
        {
            prActiveRxFrame = _Cec_GetRxWriteFrame();
            if (!prActiveRxFrame)
            {
                break;
            }
            
            // Copy 8032 data to DRAM.
            prActiveRxFrame->size = u4Size;
            prActiveRxFrame->sendidx = pu1Data[u4Offset + 1];
            prActiveRxFrame->reTXcnt = pu1Data[u4Offset + 2];
            // Notice: 8032 pointer length is 24 bits not 32 bits.
            prActiveRxFrame->txtag = (void *)((pu1Data[u4Offset + 3]<<16) |
                (pu1Data[u4Offset + 4]<<8) | pu1Data[u4Offset + 5]);

            prActiveRxFrame->blocks.header.initiator = (pu1Data[u4Offset + 6] >> 4);
            prActiveRxFrame->blocks.header.destination = (pu1Data[u4Offset + 6] & 0xFF);

            // if this frame contain opcode.
            if (u4Size > 1)
            {
                prActiveRxFrame->blocks.opcode = pu1Data[u4Offset + 7];
            }

            LOG(1, "Frame size: %d.\n", prActiveRxFrame->size);
            LOG(1, "initiator: 0x%X.\n", prActiveRxFrame->blocks.header.initiator);
            LOG(1, "destination: 0x%X.\n", prActiveRxFrame->blocks.header.destination);
            LOG(1, "opcode: 0x%X.\n", prActiveRxFrame->blocks.opcode);

            // if this frame contain operand.
            if (u4Size > 2)
            {
                for (i=0; i<(prActiveRxFrame->size-2); i++)
                {
                    prActiveRxFrame->blocks.operand[i] = pu1Data[u4Offset+8+i];
                    LOG(1, "operand[i]: 0x%X.\n", prActiveRxFrame->blocks.operand[i]);                    
                }
            }
#ifndef SYS_ANDROID_TV_TIF_SUPPORT
            if (_Cec_CheckFrame(prActiveRxFrame))
#endif// sony's requestion.
            {
                // Enqueue valid frame only.
                _Cec_EnqueueRx(prActiveRxFrame);
            }        

            prActiveRxFrame = NULL;
        }
        
        u4Offset += u48032FrameSize;
        i4MsgNum --;
    }

    x_mem_free(pu1Data);
}
#endif
static void _Cec_HandleRxTask(void)
{
    CEC_FRAME_DESCRIPTION *frame;

    // Must wait middleware become ready to handle Rx.
    if (!fgCecMwReady)
    {
		if(u1CecDbgLvl == 3)
		{
       		LOG(0, "_Cec_HandleRxTask: Mw not ready.\n");  
    	}
        return;
    }

#ifdef SUPPORT_WELTREND_UP
    // query the pending message
    vWTNotifyPendingData();
#endif

    while (1)
    {
        frame = _Cec_GetRxReadFrame();
        if(frame == NULL)
        {
            //empty or error happen.
            return;
        }
#ifndef SYS_ANDROID_TV_TIF_SUPPORT
        if(_Cec_CheckFrame(frame))
#endif //sony's requestion.
		{
			if(u1CecDbgLvl == 1)
            {
            	LOG(0, "RxTask: Valid frame.\n");
			}

            CEC_PrintFrame(frame, CEC_RX_FRAME);
#ifdef LG_CEC
            _Cec_EnqueueRxMtal(frame);
            ui2_cecResponseMtal = ui2_cecResponseMtal  |0xA200;
#else             
	#ifdef SYS_ANDROID_TV_TIF_SUPPORT
		if(_Cec_CheckLA(frame))
	#endif
         _Cec_NotifyRxResult(frame);
	
	
#endif

#ifdef CTS_TEST
            _Cec_HandleCTSRx(frame);
#endif

        }

        _Cec_DequeueRx();    
    }
}

static UINT8 _Cec_InitTxFrame(CEC_FRAME_DESCRIPTION *frame)
{
    if (!frame)
    {
        return CEC_ERROR;
    }

    if ((frame->size > CEC_MAX_MESG_SIZE) || (frame->size == 0))
    {
        LOG(0, "_Cec_InitTxFrame: tx size error=%d.\n", frame->size);
    
        return CEC_ERROR;
    }

    // valid tx frame
    frame->reTXcnt = 0;
    frame->sendidx = 0;

    return CEC_SUCCESS;
}

static void _Cec_HandleTxTask(void)
{
    UINT32 u4Val;
    CEC_FRAME_DESCRIPTION *frame;

    /* if the tx message queue is empty */
    if (IS_TX_Q_EMPTY())
    {
        return;
    }

    /* if the tx is active, check the result */
    if (IsCECStatus(STATE_TX_SEND_FRAME))
    {  
        _Cec_HandleTxResult();

        // Prevent TX lost power down interrupt.
        u4Val = u4IO32Read4B(PDWNC_INTSTA) & _PINT(PDWNC_INTNO_CEC);
        if ((u4Val == 0) &&
             (IS_INT_FAIL()) && (IS_INT_FAIL_ENABLE()) &&
             (TX_FAIL_RECORD() !=0) && (IS_TX_FSM_FAIL()))
        {
			if(u1CecDbgLvl == 1)
            {
				LOG(0, "CEC handle interrupt stauts error.\n");
			}
		#ifdef SYS_ANDROID_TV_TIF_SUPPORT
			if(IS_INT_FAIL() && IS_INT_HEADER_ACK())
			{
				_Cec_NotifyTxHeaderResult(CEC_NACK, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
			}
			else
			{
				_Cec_NotifyTxHeaderResult(CEC_BUSY, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
			}
		#endif
            DISABLE_ALL_TX_INT();
            SetCECStatus(STATE_TX_NOACK);
            RESET_HW_TX();
        }   
        if (IsCECStatus(STATE_TX_SEND_FRAME_OK))
        {
            ClrCECStatus(STATE_TX_SEND_FRAME_OK);
            ClrCECStatus(STATE_TX_SEND_FRAME);

            if (!prActiveTxFrame)
            {
                LOG(0, "Error prActiveTxFrame=Null.\n");
                ASSERT(prActiveTxFrame);                
            }
            else
            {
            	
				if(u1CecDbgLvl == 1)
               	{
				
 	LOG(0, "Send frame with tag=0x%08X ok.\n", prActiveTxFrame->txtag);
				}
#ifdef LG_CEC
                _Cec_EnqueueTxResultMtal(CEC_SUCCESS, prActiveTxFrame);
                ui2_cecResponseMtal = ui2_cecResponseMtal  |0xA3;
#else
		#ifdef SYS_ANDROID_TV_TIF_SUPPORT


                if(prActiveTxFrame->blocks.header.destination == 0 && prActiveTxFrame->blocks.header.initiator == 0 && u1CecLA0AckCtr < 2)
                {
                    LOG(0,"The (0->0) %d times,WAIT...\n",(u1CecLA0AckCtr  + 1));
                    u1CecLA0AckCtr++;
                }
                else
                {
                    _Cec_NotifyTxHeaderResult(CEC_ACK, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
                    _Cec_NotifyTxResult(CEC_SUCCESS,prActiveTxFrame, (void *)u4CecTxCbArg);
                }
		#else
                _Cec_NotifyTxResult(CEC_SUCCESS,prActiveTxFrame, prActiveTxFrame->txtag);
		#endif
#endif
                _Cec_DequeueTx();

                // Set by next _Cec_SendFrame().
                prActiveTxFrame = NULL;
            }            
        }
        
        if (IsCECStatus(STATE_TX_NOACK))
        {
            ClrCECStatus(STATE_TX_NOACK);
            ClrCECStatus(STATE_TX_SEND_FRAME);
            ClrCECStatus(STATE_TX_WAIT_DATA_TAKEN);            

            if (!prActiveTxFrame)
            {
                LOG(0, "Error prActiveTxFrame=Null.\n");
                ASSERT(prActiveTxFrame);                
            }
            else
            {
                LOG(5, "TX_NOACK, reTXcnt = %d.\n", prActiveTxFrame->reTXcnt);
                // CYJ.NOTE: retransmission
                prActiveTxFrame->reTXcnt++;
                prActiveTxFrame->sendidx = 0;

                if (prActiveTxFrame->reTXcnt >= RETX_MAX_CNT)
                {
                
				if(u1CecDbgLvl == 1)
                {
                    LOG(0, "reTXcnt reach MAX=%d.\n", prActiveTxFrame->reTXcnt);
				}
#ifdef LG_CEC
                    _Cec_EnqueueTxResultMtal(CEC_ERROR, prActiveTxFrame);
                    ui2_cecResponseMtal = ui2_cecResponseMtal  |0xA3;
#else
			#ifdef SYS_ANDROID_TV_TIF_SUPPORT
					_Cec_NotifyTxResult(CEC_ERROR,prActiveTxFrame, (void *)u4CecTxCbArg);
			#else
                    _Cec_NotifyTxResult(CEC_ERROR,prActiveTxFrame, prActiveTxFrame->txtag);
			#endif
#endif
                    _Cec_DequeueTx();

                    // Set by next _Cec_SendFrame().
                    prActiveTxFrame = NULL;                    
                }
            }
        }
    }
    else
    {
#ifdef CC_DELAY_CEC_MESSAGE_5VIN_HPDLOW
        if (!fgCecMessageCanBeSend())
        {
            LOG(0, "There is an active source but HPD is low!\n");
            return;
        }
#endif

        // Handle retransmission first.
        if (prActiveTxFrame)
        {
            LOG(5, "Retx, reTXcnt=%d.\n", prActiveTxFrame->reTXcnt);            
            frame = prActiveTxFrame;
        }
        else
        {
            /* if the tx is not active, send the next message */
            frame = _Cec_GetTxReadFrame();
        }
        if (frame)
        {
#ifdef SYS_MHL_SUPPORT     
			if (_IsMhlDevice()) 
            {
	            if((frame->blocks.header.destination == 0xd) && (frame->size > 1))
	                return;
            }
#endif            
            CEC_PrintFrame(frame, CEC_TX_FRAME);

            (void)_Cec_SendFrame(frame);
        }
    }
}

static void _Cec_Interrupt(UINT16 u2Vector)
{  
    ASSERT(u2Vector == PDWNC_INTNO_CEC);
    UNUSED(u2Vector);

	
	if(u1CecDbgLvl == 2)
    {
	    LOG(0, "CEC ISR.\n");
	    LOG(0, "ISR RX_EVENT.\n");
	}
    //RX_EVENT
    if (IS_INT_OV())
    {
        CLR_INT_OV();
        LOG(0, "Error Rx Overflow.\n");        
    }

    if (IS_INT_SB_RDY())
    {
        LOG(0, "Error Rx fifo full.\n");
    }
   
    // Check header interrupt when header ready or fifo full 
    if (IS_INT_HEADER_RDY())
    {
        CLR_INT_HEADER_RDY();    
        /*
            Caution: do not use GET_SRC_FIELD() and GET_DST_FIELD()
            Because it will get the transient unstable value.
            Using GET_SRC_FIELD_RECEIVING() and GET_DST_FIELD_RECEIVING() to handle.
        */
        SetCECStatus(STATE_RX_GET_NEW_HEADER);
    }

    // Check data interrupt.
    if (IS_INT_BR_RDY())
    {
        if (GET_HW_RX_LEN() == 0)
        {        
			
			LOG(7, "RX:Ping.\n");
            ClrCECStatus(STATE_RX_GET_NEW_HEADER);
        }
        else
        {
        	
            LOG(7, "RX:Data\n");
            _Cec_ProcessRxIsr();
        }

        // Clear to receive next data.
        NOTIFY_RX_HW_DATA_TAKEN();
    }

    //TX_EVENT
    if (IsCECStatus(STATE_TX_SEND_FRAME))
    {
        LOG(7, "ISR TX_EVENT.\n");

        if (IS_INT_UN())
        {
            LOG(0, "Error Tx Underrun.\n");
        }
		
        if (IS_INT_LOW())
        {
            // If h/w buffer empty and data EOM not be set. H/w will trigger buffer low flag.
            LOG(5, "Tx Buffer Low\n");
        }

        // CYJ.NOTE TX Failure Detection
        if (IS_INT_FAIL_ENABLE() && ((TX_FAIL_RECORD() != 0) || IS_TX_FSM_INIT()))
        {
        	#ifdef SYS_ANDROID_TV_TIF_SUPPORT
        	if(IS_INT_FAIL() && IS_INT_HEADER_ACK())
			{
				_Cec_NotifyTxHeaderResult(CEC_NACK, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
			}
			else
			{
				_Cec_NotifyTxHeaderResult(CEC_BUSY, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
			}
			#endif
            DISABLE_ALL_TX_INT();

            SetCECStatus(STATE_TX_RESEND);
			if(u1CecDbgLvl == 2)
            {
				LOG(0, "[Trace Flow]%s,TX FAIL RECORD:0x%x,FSM STATUS:0x%x.\n",__FUNCTION__,TX_FAIL_RECORD(),TX_FSM_STATUS());
			}
            // CYJ.NOTE: Substitute TX_FAIL_MAX() with IS_TX_FSM_FAIL()!
            if (TX_FAIL_MAX() || IS_TX_FSM_FAIL())
            {
                LOG(5, "TX Fail: 0x%X, FSM: 0x%X.\n", TX_FAIL_RECORD(), TX_FSM_STATUS());
            }
            return;
        }
		#ifdef SYS_ANDROID_TV_TIF_SUPPORT

		//_Cec_NotifyTxHeaderResult(CEC_ACK, prActiveTxFrame,(void *)(UINT32)(UINT8)prActiveTxFrame->blocks.header.destination);
		#endif
        // Send next data frame.
        if (IS_INT_RB_ENABLE() && IS_TX_DATA_TAKEN())
        {
            LOG(7, "TX Data Taken.\n");
            (void)_Cec_SendNextDataFrame();
        }
    }
}

static void _Cec_EnableInterrupt(void)
{
    // TV_EVENT
    vIO32Write4BMsk(TX_EVENT, 0x00, 0xff);
    vIO32Write4BMsk(RX_EVENT, 0x00, 0xff);
    
    // RX_EVENT
    ENABLE_INT_OV(1);
    ENABLE_INT_BR_RDY(1);
    ENABLE_INT_HEADER_RDY(1);
    // TX_EVENT
    ENABLE_INT_UN(0);
    ENABLE_INT_LOW(0);
    ENABLE_INT_FAIL(0);
    ENABLE_INT_BS(0);
    ENABLE_INT_RB(0);
}

#ifdef CC_FOR_POC_DEMO
#define CABLE_CHANGE 0
#define SOURCE_CHANGE 1
void _Cec_PhilipsNotify5VStatus(UINT8 bSwitch, BOOL fgIs5VOn)
{	
	static UINT8 bPre5VStatus[5];
	if(bPre5VStatus[bSwitch] != fgIs5VOn)
	{	
		bPre5VStatus[bSwitch] = fgIs5VOn;
		if(pPhilips5VNfyfunc != NULL)
		{
			pPhilips5VNfyfunc(CABLE_CHANGE,bSwitch,fgIs5VOn);	
		}
	}
}
#endif 
#ifdef CECNFY5VSTATUS
void _Cec_Notify5VStatus(UINT8 u1Port, UINT8 u15vStatus)
{
	if(fgCecMwReady == FALSE)
       return;
	switch(u1Port)
	{
		case HDMI_SWITCH_1:
			
		#ifdef SYS_MHL_SUPPORT // add for CR DTV00594014
			if(_bIsMhlDevice)// MHL cable disconnect, not chage the value "u15vStatus"
			{
				if(vCbusGetState() == MHL_STATE_CONNECTED)
				{
					u15vStatus = 1;// MHL cable connected, connect the source
				}
				else
				{
					u15vStatus = 0;// MHL cable connected, disconnect the source
				}
			}
		#endif // only for MHL Included Project
			if(bPre5VStatus1 != u15vStatus)
			{
				bPre5VStatus1 = u15vStatus;
				
			#ifdef SYS_ANDROID_TV_TIF_SUPPORT
				if(_bIsMhlDevice)
				{
					Mhl_NotifyHotPlug(u1Port,u15vStatus);
				}
				else
				{
			#endif
					if (rCec5vHandler)
				    {
				    	LOG(2, "_Cec_Check5VStatus port %d: %d \n", u1Port,u15vStatus);
				        rCec5vHandler(HDMI_SWITCH_1, bPre5VStatus1,(void *)u4Cec5VCbArg);
				    }
			#ifdef SYS_ANDROID_TV_TIF_SUPPORT
				}
			#endif
			}
			break;
		case HDMI_SWITCH_2:
			if(bPre5VStatus2 != u15vStatus)
			{
				bPre5VStatus2 = u15vStatus;
				if (rCec5vHandler)
			    {
			    	LOG(2, "_Cec_Check5VStatus port %d: %d \n", u1Port,u15vStatus);
					
				#if defined(CC_SOEM_BOARD)
					rCec5vHandler(HDMI_SWITCH_1, bPre5VStatus2,(void *)u4Cec5VCbArg);
				#else
			        rCec5vHandler(HDMI_SWITCH_2, bPre5VStatus2,(void *)u4Cec5VCbArg);
				#endif
			    }
			}
			break;
		case HDMI_SWITCH_3:
			if(bPre5VStatus3 != u15vStatus)
			{
				bPre5VStatus3 = u15vStatus;
				if (rCec5vHandler)
			    {
			    	LOG(2, "_Cec_Check5VStatus port %d: %d \n", u1Port,u15vStatus);
					
					#if defined(CC_SOEM_BOARD)
					rCec5vHandler(HDMI_SWITCH_2, bPre5VStatus2,(void *)u4Cec5VCbArg);
					#else
			        rCec5vHandler(HDMI_SWITCH_3, bPre5VStatus3,(void *)u4Cec5VCbArg);
					#endif
			    }
			}
			break;
#if defined(CC_MT5399) || defined(CC_MT5890)
		case HDMI_SWITCH_4:
			if(bPre5VStatus4 != u15vStatus)
			{
				bPre5VStatus4 = u15vStatus;
				if (rCec5vHandler)
			    {
			    	LOG(2, "_Cec_Check5VStatus port %d: %d \n", u1Port,u15vStatus);
			        rCec5vHandler(HDMI_SWITCH_4, bPre5VStatus4,(void *)u4Cec5VCbArg);
			    }
			}
			break;
#endif
		default:
			break;
			
	}	
}
#if 0
static void _Cec_Check5VStatus(void)
{
    if(fgCecMwReady == FALSE)
        return;
    if (bPre5VStatus1 != bHDMIPort5VStatus(HDMI_SWITCH_1))
    {
        bPre5VStatus1 = bHDMIPort5VStatus(HDMI_SWITCH_1);
        if (rCec5vHandler)
        {
	    LOG(5, "_Cec_Check5VStatus port 1: %d \n", bPre5VStatus1);
            rCec5vHandler(HDMI_SWITCH_1, bPre5VStatus1);
        }
    }

    if (bPre5VStatus2 != bHDMIPort5VStatus(HDMI_SWITCH_2))
    {
        bPre5VStatus2 = bHDMIPort5VStatus(HDMI_SWITCH_2);
        if (rCec5vHandler)
        {        
	        LOG(5, "_Cec_Check5VStatus port2: %d \n", bPre5VStatus2);
            rCec5vHandler(HDMI_SWITCH_2, bPre5VStatus2);
        }        
    }

    if (bPre5VStatus3 != bHDMIPort5VStatus(HDMI_SWITCH_3))
    {
        bPre5VStatus3 = bHDMIPort5VStatus(HDMI_SWITCH_3);
        if (rCec5vHandler)
        {
	        LOG(5, "_Cec_Check5VStatus port3: %d \n", bPre5VStatus3);
            rCec5vHandler(HDMI_SWITCH_3, bPre5VStatus3);
        }        
    }
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5861)
    if (bPre5VStatus4 != bHDMIPort5VStatus(HDMI_SWITCH_4))
    {
        bPre5VStatus4 = bHDMIPort5VStatus(HDMI_SWITCH_4);
        if (rCec5vHandler)
        {
	        LOG(5, "_Cec_Check5VStatus port4: %d \n", bPre5VStatus4);
            rCec5vHandler(HDMI_SWITCH_4, bPre5VStatus4);
        }        
    }
#endif    
}
#endif
#endif

#ifndef LG_CEC
static void _Cec_Set8032Control(BOOL fgEnable)
{
    PDWNC_T8032_CMD_T rCmd;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_T8032_CTL_CEC; //Notify T8032 start/stop control CEC
    rCmd.u1SubCmd = 0;

    if (fgEnable)
    {
        rCmd.au1Data[0] = 1; //T8032 control CEC
    }
    else
    {
        rCmd.au1Data[0] = 0; //T8032 stop control CEC
    }

    (void)PDWNC_T8032Cmd(&rCmd, NULL);
}
#endif
#ifdef SYS_MHL_SUPPORT
extern void vMhlSetRapOnOff(UINT8 port, UINT8 OnOff);
#endif
static void _Cec_SetEnable(BOOL fgEnable)
{
#ifdef SYS_MHL_SUPPORT
    vMhlSetRapOnOff(0,fgEnable);
#endif
    if (fgEnable)
    {
        #if !defined(CC_MT5387) && !defined(CC_MT5363)
        (void)vIO32Write4B(TR_CONFIG, 0x0FF01151 | ((UINT32)u1CecTVLogicAddr << 16));
        #else
        (void)vIO32Write4B(TR_CONFIG, 0x0ff01101 | ((UINT32)u1CecTVLogicAddr << 16));
        #endif

        // TV_EVENT && RX_EVENT
        (void)vIO32Write4BMsk(TX_EVENT, 0x00, 0xff);
        (void)vIO32Write4BMsk(RX_EVENT, 0x00, 0xff);
        
        //init rx queue
        //_Cec_InitQueue();
                
        // RX_EVENT
        ENABLE_INT_OV(1);
        ENABLE_INT_BR_RDY(1);
        ENABLE_INT_HEADER_RDY(1);
        // TX_EVENT
        ENABLE_INT_UN(0);
        ENABLE_INT_LOW(0);
        ENABLE_INT_FAIL(0);
        ENABLE_INT_BS(0);
        ENABLE_INT_RB(0);
        LOG(0, "CEC Driver ON \n");
    }
    else
    {
        (void)vIO32Write4B(TR_CONFIG, 0x0ff00001 | ((UINT32)u1CecTVLogicAddr << 16));
        _CEC_Status = 0;
        _Cec_InitQueue();
	    if(prActiveTxFrame)
	    {
		    prActiveTxFrame ->sendidx = 0;
		    prActiveTxFrame ->reTXcnt = 0;
			prActiveTxFrame = NULL;
	    }
        LOG(0, "CEC Driver OFF \n");
    }
}
static void _Cec_InitThread( void * pvArgs )
{
    UNUSED(pvArgs);
    bInitFinish = 0;
    CEC_HwInit();
    bInitFinish = 1;
}
static void _Cec_Thread( void * pvArgs )
{
    UNUSED(pvArgs);

    while(1)
    {
    	x_thread_delay(10);
		x_sema_lock( _hCECSema, X_SEMA_OPTION_WAIT);
        if(bInitFinish)
        {
            CEC_Mainloop();
        }
		if(!(u1IsTX_Q_Empty() && u1IsRX_Q_Empty()))
		{
			x_sema_unlock(_hCECSema);
		}
    }
}
void CECSetDbgLvl(UINT8 u1dbglvl)
{
	u1CecDbgLvl = u1dbglvl;
}
//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------
void CEC_PrintFrame(const CEC_FRAME_DESCRIPTION *pFrame, BOOL fgTxFrame)
{
    UINT8 i;

    if (!pFrame)
    {
        return;
    }
	
	if(u1CecDbgLvl == 1)
	{
		LOG(0, "%s Frame start ========>.\n", ((fgTxFrame) ? "Tx": "Rx"));
	    LOG(0, "%s Size=%d, sendidx=%d, reTXcnt=%d, txtag=0x%08X.\n",
	        ((fgTxFrame) ? "Tx": "Rx"),
	        pFrame->size, pFrame->sendidx, pFrame->reTXcnt, (UINT32)pFrame->txtag);
	    LOG(0, "%s Initiator=0x%X, Destination=0x%X.\n",
	        ((fgTxFrame) ? "Tx": "Rx"),
	        pFrame->blocks.header.initiator, pFrame->blocks.header.destination);
	    if (pFrame->size > 1)
	    {
	        LOG(0, "%s Opcode=0x%X.\n", 
	            ((fgTxFrame) ? "Tx": "Rx"), pFrame->blocks.opcode);
	    }
	    if ((pFrame->size > 2) && (pFrame->size <= 16))
	    {
	        for (i = 0; i < (pFrame->size - 2); i++)
	        {
	            LOG(0, "%s Operand[%d]=0x%02X.\n", 
	                ((fgTxFrame) ? "Tx": "Rx"), i, pFrame->blocks.operand[i]);
	        }
	    }
	    LOG(0, "%s Frame end  <========.\n", ((fgTxFrame) ? "Tx": "Rx"));
	}

}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
void CEC_SetRxCallback(x_cec_drv_nfy_data_arrival nfyfp,void *arg)
{
    rCecRxHandler = nfyfp;
	u4CecRxCbArg = (UINT32)arg;
}

void CEC_SetTxCallback(x_cec_drv_nfy_tx_result nfyfp,void *arg)
{
    rCecTxHandler = nfyfp;
	u4CecTxCbArg = (UINT32)arg;
}

#ifdef CECNFY5VSTATUS
void CEC_Set5VCallback(x_cec_drv_nfy_5v_status nfyfp,void *arg)
{
    rCec5vHandler = nfyfp;
	u4Cec5VCbArg = (UINT32)arg;
}
#endif
#ifdef CC_FOR_POC_DEMO
void CEC_SetPhilips5VCallback(x_philips_cec_drv_nfy_5v_status nfyfp)
{
    pPhilips5VNfyfunc = nfyfp;
}
#endif
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
void CEC_SetTxHeaderCallback(x_cec_drv_nfy_tx_header_result nfyfp,void *arg)
{
    rCecTxHeaderHandler = nfyfp;
	u4CecTxHCbArg = (UINT32)arg;
	u1CecLA0AckCtr = 0;
}
#endif

#if defined(SYS_ANDROID_TV_TIF_SUPPORT)
void CEC_HALSetMWReady(UINT8 mwrdy_val)
{
	LOG(0,"%s:%d",__FUNCTION__,mwrdy_val);
	fgCecMwReady = mwrdy_val;
}
#else
void CEC_SetMWReady(void)
{
    LOG(5, "CEC_SetMWReady.\n");
    //_Cec_InitQueue();
    fgCecMwReady = TRUE;
}
#endif
void CEC_SetLogicAddress(UINT8 u1LogicalAddr)
{
    LOG(0, "CEC_SetLogicAddress = %d.\n", u1LogicalAddr);
	SET_TV_LA(u1LogicalAddr);// set TV LA to the register.
 	if(u1LogicalAddr == 0)
 	{
 		CecLa.tvLA1 = u1LogicalAddr;
 	}
	else
	{
		CecLa.tvLA2 = u1LogicalAddr;
	}
}
void CEC_ClrLogicAddress(void)
{
   LOG(0, "CEC_ClrLogicAddress .\n");
   CecLa.tvLA1 = 0xf;
   CecLa.tvLA2 = 0xf;

}

void CEC_SendStandby(UINT8 u1Standby)
{
	u1StandbyCtr = u1Standby;
}
void CEC_SetWakeUp(UINT8 u1WakeUp)
{
	u1WakeUpCtr = u1WakeUp;
}

void CEC_SendStandardCmd(UINT8 u1StandardCmd)
{
	u1StadardCmdCtr = u1StandardCmd;
}

void CEC_SetSysVtrl(UINT8 u1Onoff)
{
	u1SysCtrl = u1Onoff;
}

void CEC_SetEnable(BOOL fgEnable)
{
    LOG(0, "CEC_SetEnable = %d.\n", fgEnable);

    if (fgUICECEnable != fgEnable)        
    {
        _Cec_SetEnable(fgEnable);
        fgUICECEnable = fgEnable;
    }
}

UINT8 CEC_GetLogicalAddr(void)
{
    return ((UINT8)bApiEepromReadByte(EEP_CEC_LOG_ADDR));
}

// Check if active source op code has been received.
void CEC_CheckActiveSource(CEC_FRAME_ACTSRCREVED *prActiveSrc)
{  
    if (!prActiveSrc)
    {
        return;
    }
#ifdef CC_FOR_POC_DEMO
	x_memcpy((void *)prActiveSrc,(void *)&rCecActiveSource, sizeof(CEC_FRAME_ACTSRCREVED));
#else
    if (rCecActiveSource.fgActSrcReved)
    {
        prActiveSrc->fgActSrcReved = TRUE;
        prActiveSrc->u1PhyAddr[0] = rCecActiveSource.u1PhyAddr[0];
        prActiveSrc->u1PhyAddr[1] = rCecActiveSource.u1PhyAddr[1];
    }
    else
    {
        prActiveSrc->fgActSrcReved = FALSE;
        prActiveSrc->u1PhyAddr[0] = 0;
        prActiveSrc->u1PhyAddr[1] = 0;
    }

    Printf("[CEC] ActiveSource = %d, 0x%X, 0x%X.\n", 
        prActiveSrc->fgActSrcReved, prActiveSrc->u1PhyAddr[0], prActiveSrc->u1PhyAddr[1]);
#endif
}

void CEC_HwInit(void)
{
#ifndef LG_CEC
    UINT32 u4WakeUpReason;
#ifndef CEC_DRV_VERIFY
    UINT32 u4Addr;
    INT32 i4Ret;
    UINT32 u4Count;
    UINT8 u18032Xdata[16];
#endif
#endif
    UINT32 u4Val = 1;

    /* Software initialization */
    _CEC_Status = 0;
    _Cec_InitQueue();
#ifdef LG_CEC	
    ui2_cecResponseMtal = 0;
#endif

	u1StandbyCtr=0;
    u1WakeUpCtr=0;
    u1StadardCmdCtr =0;

#ifndef LG_CEC
    // Use AC power on for FPGA emulation.
    //u4WakeUpReason = (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER;
    u4WakeUpReason = PDWNC_ReadWakeupReason();

    LOG(0, "HDMI CEC u4WakeUpReason = %d.......................\n", u4WakeUpReason);
    if ((u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER) || 
         (u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG))
#endif
    {
        ENABLE_PINMUX_TO_CEC();
#ifndef LG_CEC
        _Cec_Set8032Control(FALSE); //Stop T8032 Control CEC
#endif
        u1CecTVLogicAddr = (UINT8)CEC_LOG_TV;

        (void)vIO32WriteFldAlign(CKGEN, 0x00, PDN);
#if defined(CC_MT5387) ||defined(CC_MT5363) ||defined(CC_MT5301A)
        // Power down clock run at 3 Mhz.
        (void)vIO32WriteFldAlign(CKGEN, 0xF, DIV_SEL); 
#elif defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
		// Power down clock run at 24 Mhz.
		(void)vIO32WriteFldAlign(CKGEN, 0x78, DIV_SEL); 
#else
        // Power down clock run at 27 Mhz.
        (void)vIO32WriteFldAlign(CKGEN, 0x87, DIV_SEL); 
#endif

#if defined(CC_MT5387) ||defined(CC_MT5363) ||defined(CC_MT5301A)
	// HDMI_CEC pad control.
        (void)vIO32WriteFldAlign(PDWNC_PADCFG5, 0, FLD_HDMI_CEC_SMT); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG5, 1, FLD_HDMI_CEC_PU); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG5, 0, FLD_HDMI_CEC_SR); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG5, 1, FLD_HDMI_CEC_E4); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG5, 1, FLD_HDMI_CEC_E2);       
		
#elif defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389) || defined(CC_MT5398)|| defined(CC_MT5880) || defined(CC_MT5399)\
	|| defined(CC_MT5890) || defined(CC_MT5882)
        (void)vIO32WriteFldAlign(PDWNC_PADCFG0, 0, FLD_HDMI_CEC_SMT); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG0, 1, FLD_HDMI_CEC_PU); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG0, 0, FLD_HDMI_CEC_SR); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG0, 1, FLD_HDMI_CEC_E4); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG0, 1, FLD_HDMI_CEC_E2);	
        
#elif defined(CC_MT5881)        
		(void)vIO32WriteFldAlign(PDWNC_PADCFG6, 0, FLD_HDMI_CEC_SMT); 
		(void)vIO32WriteFldAlign(PDWNC_PADCFG6, 1, FLD_HDMI_CEC_PU); 
		(void)vIO32WriteFldAlign(PDWNC_PADCFG6, 0, FLD_HDMI_CEC_SR); 
		(void)vIO32WriteFldAlign(PDWNC_PADCFG6, 1, FLD_HDMI_CEC_E4); 
		(void)vIO32WriteFldAlign(PDWNC_PADCFG6, 1, FLD_HDMI_CEC_E2);	

#else
	// HDMI_CEC pad control.
        (void)vIO32WriteFldAlign(PDWNC_PADCFG3, 0, FLD_HDMI_CEC_SMT); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG3, 1, FLD_HDMI_CEC_PU); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG3, 0, FLD_HDMI_CEC_SR); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG3, 1, FLD_HDMI_CEC_E4); 
        (void)vIO32WriteFldAlign(PDWNC_PADCFG3, 1, FLD_HDMI_CEC_E2);       
#endif

        (void)vIO32Write4B(RX_T_START_R, 0x0189015b);
        (void)vIO32Write4B(RX_T_START_F, 0x01db01ae);
        (void)vIO32Write4B(RX_T_DATA, 0x006e00c8);
        (void)vIO32Write4B(RX_T_ACK, 0x00000096);
        (void)vIO32Write4B(RX_T_ERROR, 0x01680212);
        (void)vIO32Write4B(TX_T_START, 0x01c20172);
        (void)vIO32Write4B(TX_T_DATA_R, 0x003c0096);
        (void)vIO32Write4B(TX_T_DATA_F, 0x00f000f0);

        (void)vIO32WriteFldAlign(TX_ARB, 6, BCNT_NEW_INIT);
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
        (void)vIO32WriteFldAlign(TX_ARB, 0xc, BCNT_NEW_MSG);
#else
		(void)vIO32WriteFldAlign(TX_ARB, 9, BCNT_NEW_MSG);
#endif
        (void)vIO32WriteFldAlign(TX_ARB, 4, BCNT_RETRANSMIT);
        (void)vIO32WriteFldAlign(TX_ARB, 0, MAX_RETRANSMIT);
        (void)vIO32WriteFldAlign(TX_ARB, 0x19, TX_INPUT_DELAY);
		
		FLOW_CONTROL_ACK(1);
		
    }  
//#ifndef LG_CEC
#ifndef CEC_DRV_VERIFY
    else
    {   
	    if(Get_CEC_PIN_FUNCTION() !=2)
	    {
	    	ENABLE_PINMUX_TO_CEC();
	    }
        (void)x_memset((void *)u18032Xdata, 0, 16);
        /*
        8032 Xdata define.

        UINT8 xdata u1CecPendingMsgIdx               _at_ 0xFA00;
        UINT8 xdata fgCec8032Ctrl                          _at_ 0xFA01; 
        UINT8 xdata fgCecEnRx                                _at_ 0xFA02;
        UINT8 xdata fgCecEnOneTouchPlay               _at_ 0xFA03;

        UINT8 xdata fgCec8032Run                           _at_ 0xFA04;
        UINT8 xdata fgCecStandbyToOn                    _at_ 0xFA05;
        UINT8 xdata u1CecTVLogicAddr                     _at_ 0xFA06;
        UINT8 xdata u1CecVersion		            _at_ 0xFA07;

        UINT8 xdata u1CecMenuLanguage[3] 	    _at_ 0xFA08;
        UINT8 xdata u1CecVendorID[3] 		    _at_ 0xFA0B;
        */                
        _Cec_Get8032Data(u18032Xdata, 16);
        
        LOG(5, "u18032Xdata[0~ 7]: %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X.\n",
            u18032Xdata[0], u18032Xdata[1], u18032Xdata[2], u18032Xdata[3], 
            u18032Xdata[4], u18032Xdata[5], u18032Xdata[6], u18032Xdata[7]);
        LOG(5, "u18032Xdata[8~15]: %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X.\n",
            u18032Xdata[8], u18032Xdata[9], u18032Xdata[10], u18032Xdata[11], 
            u18032Xdata[12], u18032Xdata[13], u18032Xdata[14], u18032Xdata[15]);

        // Set fgCec8032Ctrl.        
        if (u18032Xdata[1] != 0)
        {
            _Cec_Set8032Control(FALSE); //Stop T8032 Control CEC
        }

        // Wait fgCec8032Run = 0.
        u4Count = 0;
        while ((u18032Xdata[4] != 0) && (u4Count < 40))
        {
            x_thread_delay(50);        
            _Cec_Get8032Data(u18032Xdata, 16);
            u4Count ++;
        }        
        if ((u18032Xdata[4] != 0) && (u4Count >= 40))
        {
            LOG(0, "Error T8032_CEC_BUSY.\n");
        }

        LOG(1, "_Cec_Process8032Data msg num = %d.\n", u18032Xdata[0]);

        // Check if 8032 pending RX data.
        if ((u18032Xdata[0] > 0) && (u18032Xdata[0] < 16))
        {
            _Cec_Process8032Data((UINT32)u18032Xdata[0]);
        }        

        // Use 8032 LA directly.
        u1CecTVLogicAddr = GET_DEVICE_LA();

        if (u1CecTVLogicAddr != CEC_LOG_TV)
        {
            LOG(0, "Error, u1CecTVLogicAddr=%d.\n", u1CecTVLogicAddr);            
        }
    }
#endif
//Bo modify for the issue:tv can't receive commad from standby by one touch play.
//CL:1641794 begin.

	// CYJ.NOTE TX_EN, RX_EN: disable it
	(void)vIO32Write4B(TR_CONFIG, 0x00000001);
	// Check if CEC is disable by AP. Without ack to initiator.
#ifndef CEC_DRV_VERIFY		
	if ((u4Addr = DRVCUST_InitGet(eEepromCECEnableOffset)) != 0x00)
	{
		i4Ret = EEPROM_Read(u4Addr, (UINT32)&u4Val, 1);
		if (i4Ret != 0)
		{
			LOG(0, "EEPROM error. Turn on CEC.\n");
			// Default to turn on CEC when EEPROM read error.
			u4Val = 1;
		}
		else
		{
			LOG(0, "EEPROM ok. u4Val = %d.\n", u4Val);
		}				 
	}
#endif
#ifdef SYS_MHL_SUPPORT
	vMhlSetRapOnOff(0,(u4Val ? 1 : 0));
#endif
	if(u4Val == 255)
	{
		u4Val = 1;
	}// verification use.
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
	u4Val = 1; // google request , force cec on .
#endif
	if (u4Val == 1)
	{
    #if !defined(CC_MT5387) && !defined(CC_MT5363)
		u4Val = 0x0FF01151;
    #else
		u4Val = 0x0FF01101;
    #endif        
		(void)vIO32Write4B(TR_CONFIG, u4Val | ((UINT32)u1CecTVLogicAddr << 16));
	}
	else
	{
		// Turn off CEC.
		(void)vIO32Write4B(TR_CONFIG, 0x0ff00001 | ((UINT32)u1CecTVLogicAddr << 16));
	}
	
	// Enable interrupt.
	_Cec_EnableInterrupt();
//CL:1641794 end.
    if (CHECK_RX_EN() == 1)
    {
        fgUICECEnable = TRUE;
        LOG(5, "CEC Enalbe.\n");
    }
    else
    {
        fgUICECEnable = FALSE;
        LOG(3, "CEC Disalbe.\n");
    }
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)
    _Cec_SetArcEnable(FALSE);
#endif    
    fgCecDrvReady = TRUE;
    return;
}


UINT8 u1IsTX_Q_Empty(void)
{
    return IS_TX_Q_EMPTY();
}

UINT8 u1IsTX_Q_Full(void)
{
    return IS_TX_Q_FULL();
}

UINT8 u1IsRX_Q_Empty(void)
{
    return IS_RX_Q_EMPTY();
}

UINT8 u1IsRX_Q_Full(void)
{
    return IS_RX_Q_FULL();
}
#if defined(SYS_MHL_SUPPORT)

CEC_FRAME_DESCRIPTION *_Cbus_GetTxReadFrame(void)
{
    return ((CEC_FRAME_DESCRIPTION *)_Cec_GetTxReadFrame());
}
CEC_FRAME_DESCRIPTION *_Cbus_GetRxReadFrame(void)
{
    return ((CEC_FRAME_DESCRIPTION *)_Cec_GetRxReadFrame());
}

void _Cbus_NotifyTxResult(UINT8 result, CEC_FRAME_DESCRIPTION *frame,void *arg)
{
    _Cec_NotifyTxResult(result, frame,arg);
}
void _Cbus_NotifyRxResult(CEC_FRAME_DESCRIPTION *frame)
{
    _Cec_NotifyRxResult(frame);
}

void _Cbus_DequeueTx(void)
{
   _Cec_DequeueTx();
}
void _Cbus_EnqueueRx(CEC_FRAME_DESCRIPTION *prFrame)
{

   x_memcpy(&CEC_rx_msg_queue[CEC_rxQ_write_idx],prFrame,sizeof(CEC_FRAME_DESCRIPTION));
   _Cec_EnqueueRx(prFrame);
}

void _Cbus_DequeueRx(void)
{
   _Cec_DequeueRx();
}

#endif

void CEC_Init(void)
{
	x_os_isr_fct pfnOldIsr;
	if(bBootUp == FALSE)
	{
		bBootUp = TRUE;

	
	x_sema_create(&_hCECSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    /* CEC Init Thread */
    VERIFY(x_thread_create(&phCecInitThreadHdl, 
        "CECInitThread", 1024,  60, _Cec_InitThread, 0, NULL) == OSR_OK);

    /* CEC Thread */
    VERIFY(x_thread_create(&phCecThreadHdl, 
        "CECThread", 2048,  60, _Cec_Thread, 0, NULL) == OSR_OK);
        
    //CEC_HwInit();

    VERIFY(PDWNC_RegIsr(PDWNC_INTNO_CEC, _Cec_Interrupt, &pfnOldIsr) == PDWNC_OK);   
  }
#ifdef SYS_MHL_SUPPORT
    Cbus_Init();
#endif
    return;
}
#if 1//ndef RAP_OSD_NAME_ROLL_BACK
#ifdef SYS_MHL_SUPPORT
UINT8 u1ContentOnRetryCnt=0;
//UINT8 u1ContentOffRetryCnt=0;
extern UINT8 uIsHaveAckFlag;
extern UINT8 u1RapOnOff;
#endif
#endif

void CEC_Mainloop(void)
{
#ifdef SYS_MHL_SUPPORT
    if (_IsMhlDevice()) {
        _Cbus_HandleTxTask();
    }
#endif
    _Cec_HandleTxTask();

#ifdef SYS_MHL_SUPPORT
    if (_IsMhlDevice() && fgCecMwReady) {
          _Cbus_HandleRxTask();
    }
#endif

    // NOTE: the priority between tx and rx
    if (!IsCECStatus(STATE_TX_SEND_FRAME))
    {
        _Cec_HandleRxTask();
    }
}

UINT8 CEC_EnqueueTx(CEC_FRAME_DESCRIPTION *frame)
{
    if (!fgCecDrvReady)
    {
        return CEC_ERROR;
    }
    if (_Cec_InitTxFrame(frame))
    {
        return CEC_ERROR;
    }

    if (IS_TX_Q_FULL())
    {
		if(u1CecDbgLvl == 0)//Sony request, default open ,if it always log,please set u1CecDbgLvl to above 4.
		{
        	LOG(0, "CEC_EnqueueTx: tx queue full.\n");
		}
        return CEC_ERROR;
    }
	#ifdef SYS_ANDROID_TV_TIF_SUPPORT
	if( IS_INT_FAIL_ENABLE() && IS_TX_FSM_INIT())
	{
		_Cec_NotifyTxHeaderResult(CEC_BUSY, frame,(void *)(UINT32)(UINT8)frame->blocks.header.destination);
		if(u1CecDbgLvl == 3)
		{
			LOG(0,"%s:CEC Bus is busy,notify to the TOP LAYER.\n",__FUNCTION__);
		}
		return CEC_ERROR;
	}
	#endif
    if (CEC_txQ_write_idx >= TX_Q_SIZE)
    {
        LOG(0, "CEC_EnqueueTx: tx queue index error=%d.\n", CEC_txQ_write_idx);
        return CEC_ERROR;    
    }
    
    (void)x_memcpy(&(CEC_tx_msg_queue[CEC_txQ_write_idx]), 
        frame, sizeof(CEC_FRAME_DESCRIPTION));
    
    CEC_txQ_write_idx = (CEC_txQ_write_idx + 1) % TX_Q_SIZE;
	x_sema_unlock(_hCECSema);
    return CEC_SUCCESS;
}

#ifdef CC_SUPPORT_STR//CC_FAST_INIT
void CEC_pm_suspend(void)
{
#if !defined(SYS_ANDROID_TV_TIF_SUPPORT)
	fgCecMwReady = FALSE;
#endif
	rCecActiveSource.fgActSrcReved = FALSE;
#ifdef CC_FOR_POC_DEMO
	
#else
	rCecActiveSource.u1PhyAddr[0] = 0x0;
	rCecActiveSource.u1PhyAddr[1] = 0x0;
#endif
}

void CEC_pm_resume(void)
{
    CEC_HwInit();
    //CEC_Init();
    // Only enable CEC pdwnc interrupt.
    PDWNC_INTEN(PDWNC_INTNO_CEC);
}
#endif

void _CEC_TestCmd(UINT8 cmd)
{
    UINT8 errcode;
    CEC_FRAME_DESCRIPTION* frame;

    // initialization
    if (cmd == 0x01)
    {
        Printf("[CEC] Initialization\n");
        CEC_Init();
    }

    if (cmd == 0x03)
    {
        Printf("[CEC] Disable INT not support.\n");
    }

    // broadcast
    if (cmd == 0x11)
    {
        frame = &test_txframe;

        frame->size = 6;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_BROADCAST;
        frame->blocks.opcode = OPCODE_IMAGE_VIEW_ON;
        frame->blocks.operand[0] = 1;
        frame->blocks.operand[1] = 2;
        frame->blocks.operand[2] = 3;
        frame->blocks.operand[3] = 4;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }

    // dedicated
    if (cmd == 0x13)
    {
        frame = &test_txframe;

        frame->size = 3;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_PLAYBACK_DEVICE1;
        frame->blocks.opcode = OPCODE_FEATURE_ABORT;
        frame->blocks.operand[0] = 1;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }

    // maximum message size, dedicated
    if (cmd == 0x14)
    {
        frame = &test_txframe;

        frame->size = 16;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_RECORDING_DEVICE1;
        frame->blocks.opcode = OPCODE_IMAGE_VIEW_ON;
        frame->blocks.operand[0] = 0;
        frame->blocks.operand[1] = 1;
        frame->blocks.operand[2] = 2;
        frame->blocks.operand[3] = 3;
        frame->blocks.operand[4] = 4;
        frame->blocks.operand[5] = 5;
        frame->blocks.operand[6] = 6;
        frame->blocks.operand[7] = 7;
        frame->blocks.operand[8] = 8;
        frame->blocks.operand[9] = 9;
        frame->blocks.operand[0xa] = 0xa;
        frame->blocks.operand[0xb] = 0xb;
        frame->blocks.operand[0xc] = 0xc;
        frame->blocks.operand[0xd] = 0xd;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }
    // ping message
    if (cmd == 0x15)
    {
        frame = &test_txframe;

        frame->size = 1;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_PLAYBACK_DEVICE1;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }
    if (cmd == 0x16)
    {
        frame = &test_txframe;

        frame->size = 3;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_BROADCAST;
        frame->blocks.opcode = OPCODE_IMAGE_VIEW_ON;
        frame->blocks.operand[0] = 2;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }
    if (cmd == 0x17)
    {
        frame = &test_txframe;

        frame->size = 3;
        frame->sendidx = 0;
        frame->reTXcnt = 0;
        frame->blocks.header.initiator = CEC_LOG_TV;
        frame->blocks.header.destination = CEC_LOG_RECORDING_DEVICE1;
        frame->blocks.opcode = OPCODE_IMAGE_VIEW_ON;
        frame->blocks.operand[0] = 3;

        CEC_PrintFrame(frame, CEC_TX_FRAME);

        errcode = _Cec_SendFrame(frame);
        if (errcode)
        {
            Printf("_CEC_SendFrame fail: %x\n", errcode);
        }
    }
    // test flow control
    if (cmd == 0x30)
    {
        FLOW_CONTROL_ACK(1);
    }
    if (cmd == 0x31)
    {
        FLOW_CONTROL_ACK(0);
    }
}

//cec Arc control
#if defined (CC_HDMI_ARC_SPDIF_CTRL)|| defined (CC_HDMI_ARC_SPDIF_CTRL_89)
//#if defined(CC_MT5396) ||defined(CC_MT5368) || defined(CC_MT5389) ||defined(CC_MT5398) ||defined(CC_MT5880) || defined(CC_MT5881)
static void _Cec_SetArcEnable(BOOL fgEnable)
{   
    if (fgEnable)   //Arc Enable
    {
#if defined(CC_MT5396)     
        BSP_PinSet(PIN_ASPDIF1,1);
#elif defined(CC_MT5368)
        BSP_PinSet(PIN_ASPDIFO1,1);
#elif defined(CC_MT5389)
        BSP_PinSet(PIN_ASPDIFO0, 1);  
#elif defined(CC_MT5398)
        BSP_PinSet(PIN_ASPDIF1, 1);
#elif defined(CC_MT5880) || defined(CC_MT5565)
#if defined(CC_SOEM_BOARD) 
		BSP_PinSet(PIN_ALIN,2);//sony ARC pin on HW is this pin
			   /*PIN_ALIN:0,gpio;1,AIn;2,Spdifo;3,hdmi_spdifo*/
#else
		BSP_PinSet(PIN_ASPDIFO0,1);
#endif
#elif defined(CC_MT5881)
        BSP_PinSet(PIN_ASPDIFIN,3);
#elif defined(CC_MT5399) || defined(CC_MT5890)
        BSP_PinSet(PIN_ASPDIF1, 1);
#elif defined(CC_MT5882)
				BSP_PinSet(PIN_ALIN, 2);
#endif
        LOG(0, "CEC Arc Enable \n");
    }
    else   //Arc disable
    {   
#if defined(CC_MT5396)     
        BSP_PinSet(PIN_ASPDIF1,0);
#elif defined(CC_MT5368)
        BSP_PinSet(PIN_ASPDIFO1,0);
#elif defined(CC_MT5389)
        BSP_PinSet(PIN_ASPDIFO0, 0);
#elif defined(CC_MT5398)
        BSP_PinSet(PIN_ASPDIF1, 0);
#elif defined(CC_MT5880) || defined(CC_MT5565)
#if defined(CC_SOEM_BOARD)
        BSP_PinSet(PIN_ALIN,0);//sony ARC pin on HW is this pin
        /*PIN_ALIN:0,gpio;1,AIn;2,Spdifo;3,hdmi_spdifo*/
#else
        BSP_PinSet(PIN_ASPDIFO0,0);
#endif
#elif defined(CC_MT5881)
        BSP_PinSet(PIN_ASPDIFIN,0);        
#elif defined(CC_MT5399) || defined(CC_MT5890)
        BSP_PinSet(PIN_ASPDIF1, 0);
#elif defined(CC_MT5882)
				BSP_PinSet(PIN_ALIN, 0);
#endif

        LOG(0, "CEC Arc Disable \n");
    }
}

void CEC_SetArcEnable(BOOL fgEnable)
{
    LOG(5, "CEC_SetArcEnable = %d.\n", fgEnable);
    //if (fgCecArcEnable != fgEnable)        
    //{
        _Cec_SetArcEnable(fgEnable);
		//fgCecArcEnable = fgEnable;
   // }

}

//#endif
#endif

#ifdef LG_CEC
void CEC_ReadRxCommand(CEC_FRAME_DESCRIPTION *pt_frame)
{
    if(CEC_rxQ_read_idx_mtal ==CEC_rxQ_write_idx_mtal)
    {
        LOG(0, " rx queue empty (mtal)\n");
        return;
    }
    x_memcpy(pt_frame, &CEC_rx_msg_queue_mtal[CEC_rxQ_read_idx_mtal], sizeof(CEC_FRAME_DESCRIPTION));
    CEC_rxQ_read_idx_mtal = (CEC_rxQ_read_idx_mtal + 1) %RX_Q_SIZE_MTAL;
        return;
}
void CEC_ReadTxResult(CEC_TX_RESULT *pt_txResult)
{
    if(CEC_tx_result_read_idx_mtal == CEC_tx_result_write_idx_mtal)
    {
        LOG(0, "tx result queue empty (mtal)\n");
        return;
    }
    x_memcpy(pt_txResult, &CEC_tx_result_queue_mtal[CEC_tx_result_read_idx_mtal], sizeof(CEC_TX_RESULT));
    CEC_tx_result_read_idx_mtal = (CEC_tx_result_read_idx_mtal + 1)%TX_Q_SIZE_MTAL;
        return;
}
UINT16 CEC_ReadResponse(void)
{
    if(CEC_rxQ_read_idx_mtal ==CEC_rxQ_write_idx_mtal)//rx queue for mtal empty
        ui2_cecResponseMtal = ui2_cecResponseMtal & 0xff;
    if(CEC_tx_result_read_idx_mtal == CEC_tx_result_write_idx_mtal)
        ui2_cecResponseMtal = ui2_cecResponseMtal &0xff00;
    return ui2_cecResponseMtal;;
}
#endif
