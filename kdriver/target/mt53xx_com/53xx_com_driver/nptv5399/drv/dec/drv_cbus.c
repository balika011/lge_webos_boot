#include "drv_cbus_if.h"
#include "x_timer.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "pdwnc_if.h"
#ifdef SYS_MHL_SUPPORT
#include "mhl_if.h"
#endif

#include "util.h"
#include "sv_const.h"
#include "video_def.h"
#include "x_os.h"

//---------------------------------------------------------------------------
// MHL_ZONE
#ifdef SYS_MHL_SUPPORT
UINT8  _bIsMhlDeviceConnect;
#endif
extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
#define TEST_MSC_IN_DDC
#define TEST_ABORT_HANDLE
#define VBUS_CTRL_GPIO 208
#define CBUS_DMSG
#define CBUS_DEBUG
#define REMY
#define ENABLE_CBUS_LOW_DISCONNECT
#define HDCP_SUPPORT
#define RETRYMAX 24
extern HDMI_STATUS_T _arHdmiRx[HDMI_SWITCH_MAX];
//#define FORCE_1K_ON
#ifdef SYS_MHL_SUPPORT
HDMI_SIL_DevCap_STATUS_T tHDMI_SIL_DevCap_STATUS;
#endif

#define MHL_PORT_MAX 1

#define CTRL_ENABLE_CBUS_RX 0x80000000
#ifndef NDEBUG
static CHAR * _aszDdcState[DDC_STATE_MAX] =
{   
   "DDC_STATE_NONE",
   "DDC_STATE_SOF", 
   "DDC_STATE_ADRW",
   "DDC_STATE_OFFSET",
   "DDC_STATE_OFFSET1",
   "DDC_STATE_CONT",
   "DDC_STATE_DATAW",
   "DDC_STATE_STOP",
   "DDC_STATE_EOF",
};
static CHAR * _aszWriteBurstState[DDC_STATE_MAX] =
{   
   "MHL_WRITE_BURST_NONE",
   "MHL_WRITE_BURST_START", 
   "MHL_WRITE_BURST_OFFSET",
   "MHL_WRITE_BURSET_WDATA",
   "MHL_WRITE_BURST_EOF",
   "MHL_WRITE_BURSET_MAX",
};
static CHAR * _aszMhlRxState[DDC_STATE_MAX] =
{   
	"MHL_RX_STATE_IDLE",
	"MHL_RX_STATE_MSCMSG", 
	"MHL_RX_STATE_MSCOFFSET",
	"MHL_RX_STATE_MSCDATA0",
	"MHL_RX_STATE_MSCDATA1",
	"MHL_WRITE_BURSET_MAX",
};
static CHAR * _aszMhlState[MHL_STATE_MAX] =
{   
   "MHL_STATE_NONE",
   "MHL_STATE_INIT", 
   "MHL_STATE_BOOTUP",
   "MHL_STATE_WAIT_CABLE_CONNECT",
   "MHL_STATE_DISCOVERY",
   "MHL_STATE_CONNECTED",
   "MHL_STATE_CABLE_DISCONNECT",
};
#endif
static UINT8 u1RetryNum = 0;
static UINT8 bCbusState;
static UINT8 arEdid[EDID_SIZE] ;
static UINT16 EdidReadPoniter;

static UINT16 DdcCmd ;
static UINT16 MscCmd ;

static UINT16 MscData0 ;
static UINT16 MscData1 ;

static UINT16 DdcData0 ;

static UINT8 iMhlStateOld ;
static UINT8 iMhlStateNew ;

static UINT8 iDdcSateOld ;
static UINT8 iDdcSateNew ;

static UINT8 iWriteburstStateOld;
static UINT8 iWriteburstStateNew;

static UINT8 iMhlRxStateOld ;
static UINT8 iMhlRxStateNew ;

static UINT16 iDdcOffset ;
static UINT16 ReadDevCapOffset ;

static MHL_DEVCAP_T _arMhlDevCap;
static MHL_DEVCAP_T _arMhlSrcDevCap;
static MHL_MSG_T tCurrDDCErrorMsg;
static MHL_MSG_T tCurrMSCErrorMsg;
static UINT16 u2CurrTxMsg;
static UINT16 au2TxMsgSavedDdc[MHL_MSG_BUF_SIZE];
static UINT8 u1TxMsgSavedSizeDdc = 0;
static UINT16 au2TxMsgSavedMsc[MHL_MSG_BUF_SIZE];
static UINT8 u1TxMsgSavedSizeMsc = 0;
static UINT16 au2TxMsgRetry[MHL_MSG_BUF_SIZE];
static UINT8 u1TxMsgRetrySize = 0;

static UINT8 u1TxCurrMsgChnl=0;//
Resister_State eResist_state = Resister_None;

#ifdef HDCP_SUPPORT
static UINT8 au1An[8];
static UINT8 au1Aksv[5];
static UINT8 au1Bksv[5] ;
static UINT8 au1Ri1[2];
static UINT8 u1Bcaps ;
static UINT8 au1Bstatus[2] ; 
#endif
static DDC_STATUS_T tDDCStatus;

static UINT8 u1ScrachpadOffset ;
static UINT8 u1ScrachpadIndex ;
#ifdef SYS_MHL_SUPPORT
static UINT8 au1ScratchPad[SCRATCHPAD_REG_SIZE];
#endif
static UINT8 _MHLStatus;

static UINT8 MhlSetHpd ;
static UINT8 MhlSetPathen ;
static MHL_MSG_T arRxMscQueue[MHL_RX_QUEUE_SIZE] ;
static UINT8 mhl_rxQ_read_idx;
static UINT8 mhl_rxQ_write_idx;

static MHL_MSG_T arTxMscQueue[MHL_TX_QUEUE_SIZE] ;
static UINT8 mhl_txQ_read_idx;
static UINT8 mhl_txQ_write_idx;
static UINT16 arTxMscMsgs[MHL_MSG_BUF_SIZE] ;
static UINT8 MscReadDevCap;
static HANDLE_T phCbusHdcpThreadHdl;
static UINT8 bSetAn = 0;
static UINT8 bSetAksv = 0;
static HANDLE_T phCbusThreadHdl;
#ifdef CBUS_DMSG
static HANDLE_T phCbusDebugThreadHdl;
MHL_MSG_T *pDbgRxMsg;
static UINT32 dbg_rxQ_read_idx;
static UINT32 dbg_rxQ_write_idx;
static void _dbg_Enqueue(MHL_MSG_T *prMsg);
static UINT8 _dbg_Dequeue(MHL_MSG_T *prMsg);
#endif

void vCbusSetEdidChg(void); /*Add CC_MT5882 George.yang 2014-7-17*/


static HANDLE_T _hMhlMscTimer;
static HANDLE_T _hMhlDdcTimer;
static UINT8 u1DebugLvl;
static HANDLE_T _hMhlAbortTimer;
#ifdef SYS_MHL_SUPPORT
static HANDLE_T _hMhlOcpTimer;
#endif
static CBUS_TIMER_STATUS_T tCbusTimerState;
static HANDLE_T phCbusTimerThreadHdl;
static UINT8 u11Ktest ;
#ifdef HDCP_SUPPORT
static UINT8 u1HdcpRsd5Offset;
static UINT8 u1AnReady ;
#endif
static UINT8  GroupId;
static UINT8  u1CurGroupId;
static UINT8 u1HandshakingGroupid;

static UINT8 u1BootUp;
static HANDLE_T _hMhlBootupTimer;
static UINT8 u1CableConCnt;//cable connect loop counter for sony 10ms 
static UINT8 bInitFlg = 0;
static UINT8 _Cbus_SendMsgEx(UINT16 *pMsgData, UINT8 dataSize);
#ifdef SYS_MHL_SUPPORT
static UINT8 u1RevRcpKeyCode = 0xff;
#endif
static UINT8  u1EdidRetry = 0;

#ifdef CC_MHL_3D_SUPPORT
extern WRITE_BURST_VDO_3D_T tWriteBurstVic3D[8];
extern WRITE_BURST_VDO_3D_T tWriteBurstDtd3D[2];
extern UINT8 VicWriteburstCnt ;
extern UINT8 DtdWriteburstCnt ;
static UINT8 VicWriteburstIdx =0 ;
static UINT8 DtdWriteburstIdx =0 ;
extern MHL_3D_VIC_Structuration_T t3D_VIC_Struct;
extern MHL_3D_DTD_Structuration_T t3D_DTD_Struct;


UINT8 vCbusWriteBurst3D(void)
{
    UINT8 u1Datanum;
    UINT32 i=0;
    MHL_MSG_T t_Msg;
	if(VicWriteburstIdx < VicWriteburstCnt )
	{
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_BURST;
		arTxMscMsgs[1] = 0x440;// + (VicWriteburstIdx << 4);//40~7F
		arTxMscMsgs[2] = tWriteBurstVic3D[VicWriteburstIdx].BURST_ID_H| 0x400;
		arTxMscMsgs[3] = tWriteBurstVic3D[VicWriteburstIdx].BURST_ID_L| 0x400;
		arTxMscMsgs[4] = tWriteBurstVic3D[VicWriteburstIdx].CHECK_SUM| 0x400;
		arTxMscMsgs[5] = tWriteBurstVic3D[VicWriteburstIdx].TOT_ENT| 0x400;
		arTxMscMsgs[6] = tWriteBurstVic3D[VicWriteburstIdx].SEQ| 0x400;
		arTxMscMsgs[7] = tWriteBurstVic3D[VicWriteburstIdx].NUM_ENT| 0x400;
		arTxMscMsgs[8] = tWriteBurstVic3D[VicWriteburstIdx].VDI_0_H| 0x400;
		arTxMscMsgs[9] = tWriteBurstVic3D[VicWriteburstIdx].VDI_0_L| 0x400;
		arTxMscMsgs[10] = tWriteBurstVic3D[VicWriteburstIdx].VDI_1_H| 0x400;
		arTxMscMsgs[11] = tWriteBurstVic3D[VicWriteburstIdx].VDI_1_L| 0x400;
		arTxMscMsgs[12] = tWriteBurstVic3D[VicWriteburstIdx].VDI_2_H| 0x400;
		arTxMscMsgs[13] = tWriteBurstVic3D[VicWriteburstIdx].VDI_2_L| 0x400;
		arTxMscMsgs[14] = tWriteBurstVic3D[VicWriteburstIdx].VDI_3_H| 0x400;
		arTxMscMsgs[15] = tWriteBurstVic3D[VicWriteburstIdx].VDI_3_L| 0x400;
		arTxMscMsgs[16] = tWriteBurstVic3D[VicWriteburstIdx].VDI_4_H| 0x400;
		arTxMscMsgs[17] = tWriteBurstVic3D[VicWriteburstIdx].VDI_4_L| 0x400;
		arTxMscMsgs[18] = CBUS_MSC_CTRL_EOF;
		u1Datanum = (tWriteBurstVic3D[VicWriteburstIdx].NUM_ENT << 1) + 9;
		arTxMscMsgs[u1Datanum-1] = CBUS_MSC_CTRL_EOF;
	    //to do, for 3D
		VicWriteburstIdx ++;
		 //_Cbus_SendMsgEx(arTxMscMsgs,u1Datanum);
		for(i=0;i<u1Datanum;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg); 	  
			_Mhl_TxEnqueue(&t_Msg);		  
		}
		GroupId ++;
		return 1;
	}
	if(DtdWriteburstIdx < DtdWriteburstCnt )
	{
			arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_BURST;
			arTxMscMsgs[1] = 0x440;// + (DtdWriteburstIdx << 4);//40~7F
			arTxMscMsgs[2] = tWriteBurstDtd3D[DtdWriteburstIdx].BURST_ID_H| 0x400;
			arTxMscMsgs[3] = tWriteBurstDtd3D[DtdWriteburstIdx].BURST_ID_L| 0x400;
			arTxMscMsgs[4] = tWriteBurstDtd3D[DtdWriteburstIdx].CHECK_SUM| 0x400;
			arTxMscMsgs[5] = tWriteBurstDtd3D[DtdWriteburstIdx].TOT_ENT| 0x400;
			arTxMscMsgs[6] = tWriteBurstDtd3D[DtdWriteburstIdx].SEQ| 0x400;
			arTxMscMsgs[7] = tWriteBurstDtd3D[DtdWriteburstIdx].NUM_ENT| 0x400;
			arTxMscMsgs[8] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_0_H| 0x400;
			arTxMscMsgs[9] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_0_L| 0x400;
			arTxMscMsgs[10] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_1_H| 0x400;
			arTxMscMsgs[11] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_1_L| 0x400;
			arTxMscMsgs[12] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_2_H| 0x400;
			arTxMscMsgs[13] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_2_L| 0x400;
			arTxMscMsgs[14] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_3_H| 0x400;
			arTxMscMsgs[15] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_3_L| 0x400;
			arTxMscMsgs[16] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_4_H| 0x400;
			arTxMscMsgs[17] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_4_L| 0x400;
			arTxMscMsgs[18] = CBUS_MSC_CTRL_EOF;
			u1Datanum = (tWriteBurstDtd3D[DtdWriteburstIdx].NUM_ENT << 1) + 9;	
			arTxMscMsgs[u1Datanum-1] = CBUS_MSC_CTRL_EOF;
			//to do for 3D
			DtdWriteburstIdx ++;
 			//_Cbus_SendMsgEx(arTxMscMsgs,u1Datanum);
			for(i=0;i<u1Datanum;i++)
			{
				vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	  
				_Mhl_TxEnqueue(&t_Msg); 	  
			}
			GroupId ++;
			 return 1;
		}		
		return 0;

}
#endif
static void _Cbus_Set8032Control(BOOL fgEnable)
{
    PDWNC_T8032_CMD_T rCmd;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_CTRL_CBUS; //Notify T8032 start/stop control CEC
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

void vCbusConvertInttoMsg(UINT16 ui2Msg, MHL_MSG_T *ptMsg)
{
    x_memset(ptMsg,0,sizeof(MHL_MSG_T));
    ptMsg->u1Data = (UINT8)(ui2Msg & 0xff);
    ptMsg->channel = (ui2Msg & 0x600)>>9;
    ptMsg->ctrl = (ui2Msg & 0x100)>>8;	
    ptMsg->u1GroupId = GroupId;
}
#ifdef SYS_MHL_SUPPORT
static BOOL _Mhl_TxDequeue(MHL_MSG_T *prmsg)
{
    // copy to buffer first
    (void)x_memcpy(prmsg, &(arTxMscQueue[mhl_txQ_read_idx]), sizeof(MHL_MSG_T));
	if(prmsg->u1GroupId != u1CurGroupId)
	{
	   u1CurGroupId = prmsg->u1GroupId;
	   return MHL_TX_NEXT_GROUP;
	}
    mhl_txQ_read_idx = (mhl_txQ_read_idx + 1) % MHL_TX_QUEUE_SIZE;
    return MHL_TX_CUR_GROUP;
}
#endif
static void _Cbus_MscTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{ 
    UNUSED(ptTmHandle);
    UNUSED(pvTag);
    tCbusTimerState.u1MscTimer = CBUS_TIMER_NONE;
	tCurrMSCErrorMsg.channel = 2;
	tCurrMSCErrorMsg.u1Data = MHL_MSC_MSG_TIMEOUT;
	tCurrMSCErrorMsg.ctrl = 0;
	iWriteburstStateNew = MHL_WRITE_BURST_NONE;
	iMhlRxStateNew = MHL_RX_STATE_IDLE;
	mhl_rxQ_read_idx = 0;
	mhl_rxQ_write_idx = 0;
    _MHLStatus = 0;
    u1TxMsgRetrySize = u1TxMsgSavedSizeMsc;
    x_memcpy(au2TxMsgRetry,au2TxMsgSavedMsc,u1TxMsgRetrySize * sizeof(UINT16) );

	ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
	ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
	//if TX didnot reply ack for set HPD, msc time out, need set hpd again
	if(MhlSetHpd == 1)
	{
		SetMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
		u2CurrTxMsg = NULL_DATA;
	}
	LOG(0,"MSC time out\n");
}
void _Cbus_MscStartTimer(void)
{
    tCbusTimerState.u1MscTimer = CBUS_TIMER_WAIT_START;
}

static UINT8 _Cbus_SendMsgEx(UINT16 *pMsgData, UINT8 dataSize)
{
    UINT8 i;
	UINT16 ui2TxMsg = NULL_DATA;
	UINT16 *pu2TxMsgSaved;
#ifdef CBUS_DMSG    
	HAL_TIME_T tTimer;
#endif
#ifdef CBUS_DMSG
    MHL_MSG_T rMsg;
	HAL_GetTime(&tTimer);
#endif	
    if((*pMsgData)&0x400)//MSC
    {
        u1TxMsgSavedSizeMsc = dataSize;
        pu2TxMsgSaved = au2TxMsgSavedMsc;
		u1TxCurrMsgChnl = 2;
    }
    else
    {
		u1TxMsgSavedSizeDdc = dataSize;
		pu2TxMsgSaved = au2TxMsgSavedDdc;
		u1TxCurrMsgChnl = 0;
    }
    for(i = 0; i < dataSize; i++)
    { 
        *(pu2TxMsgSaved+i) = *(pMsgData+i);
#ifdef CBUS_DMSG
        vCbusConvertInttoMsg(*(pMsgData+i),&rMsg);
        x_memcpy(&(rMsg.time), &tTimer,sizeof(HAL_TIME_T));
        rMsg.status.dbgTxData = 1;
        _dbg_Enqueue(&rMsg);
#endif
		if( i & 1) 
		{
		   vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF1);
		}
		else
		{
		   vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF0);
		}
		
		if(((*(pMsgData+i)) & 0x500)==0x500)
		{
			 ui2TxMsg = (*(pMsgData+i));
		}
   }
    if((ui2TxMsg == CBUS_MSC_CTRL_ABORT) || (ui2TxMsg == CBUS_MSC_CTRL_ACK) || (ui2TxMsg == CBUS_MSC_CTRL_NACK) /*|| (ui2TxMsg == NONE_PACKET)*/)
    {
        ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
        ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
        u2CurrTxMsg = ui2TxMsg;
    }
    else
    if((ui2TxMsg == CBUS_MSC_CTRL_WRITE_STATE) || (ui2TxMsg == CBUS_MSC_CTRL_SET_HPD) || (ui2TxMsg == CBUS_MSC_CTRL_CLR_HPD)
    || (ui2TxMsg == CBUS_MSC_CTRL_MSC_MSG) || (ui2TxMsg == CBUS_MSC_CTRL_EOF) || (ui2TxMsg == CBUS_MSC_CTRL_WRITE_BURST))
    {   
        SetMHLStatus(STATE_MHL_TX_WAITING_ACK);
        ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
        u2CurrTxMsg = ui2TxMsg;
    }
    else
    if((ui2TxMsg == CBUS_MSC_CTRL_READ_DEVCAP))
    {    
     	SetMHLStatus(STATE_MHL_TX_WAITING_ACK);
	    SetMHLStatus(STATE_MHL_TX_WAITING_DATA);
        u2CurrTxMsg = ui2TxMsg;
    }
    else
    if((ui2TxMsg == CBUS_MSC_CTRL_GET_STATE) || (ui2TxMsg == CBUS_MSC_CTRL_GET_VENDER_ID) || (ui2TxMsg == CBUS_MSC_CTRL_GET_SC1_EC) ||
    (ui2TxMsg == CBUS_MSC_CTRL_GET_DDC_EC) || (ui2TxMsg == CBUS_MSC_CTRL_GET_MSC_EC) || (ui2TxMsg == CBUS_MSC_CTRL_GET_SC3_EC))
    {
     	 ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
	     SetMHLStatus(STATE_MHL_TX_WAITING_DATA);
		 u2CurrTxMsg = ui2TxMsg;
    }
    if((IsMHLStatus(STATE_MHL_TX_WAITING_ACK) || IsMHLStatus(STATE_MHL_TX_WAITING_DATA)) && (ui2TxMsg != NULL_DATA))
    {
	    _Cbus_MscStartTimer();
    }
    SetMHLStatus(STATE_MHL_TX_SEND_MSG);
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,dataSize,FLD_TX_NUM);	
    CBUS_TRIGGER_TX_HW();
    return 0;
}

void _Cbus_DDCStopTimer(void)
{
    tCbusTimerState.u1DdcTimer = CBUS_TIMER_WAIT_STOP;
}

void _Cbus_MscStopTimer(void)
{
   tCbusTimerState.u1MscTimer = CBUS_TIMER_WAIT_STOP;
}
void _Cbus_AbortStopTimer(void)
{
    tCbusTimerState.u1AbortTimer = CBUS_TIMER_WAIT_STOP;
}
static void _Cbus_AbortTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{   
    MHL_MSG_T t_Msg;
    UINT32 i=0;
    UNUSED(ptTmHandle);
    UNUSED(pvTag);
    tCbusTimerState.u1AbortTimer = CBUS_TIMER_NONE;
    _MHLStatus = 0;
    if((u1RetryNum < RETRYMAX)&&(u1TxMsgRetrySize>0))
	{
		for(i=0; i<u1TxMsgRetrySize; i++)
		{
			vCbusConvertInttoMsg(au2TxMsgRetry[i], &t_Msg);	
			_Mhl_TxEnqueue(&t_Msg); 	 
		}
		if(MhlSetPathen < 4)
		{
			u1HandshakingGroupid = GroupId;
		}
		GroupId ++;
		u1RetryNum ++;
	}
	else
	{
		u1RetryNum = 0;
	}
    LOG(0,"get abort reason time out \n");
}

void _Cbus_AbortStartTimer(void)
{
   tCbusTimerState.u1AbortTimer = CBUS_TIMER_WAIT_START;
}

static void _Cbus_MscErrorHandling(UINT8 u1ErrorCode)
{
#ifdef TEST_ABORT_HANDLE //for 2.0 fail
	UINT8 u1RxMsgCnts =0;
	u1RxMsgCnts = IO32ReadFldAlign(PDWNC_CBUS_STA_00,FLD_RBUF_LVL_LAT);
#endif	
   _Cbus_MscStopTimer();
#ifdef TEST_ABORT_HANDLE
   while (u1RxMsgCnts != 0) 
   {
	   u1RxMsgCnts --;
	   u4IO32Read4B(PDWNC_CBUS_RBUF);
   }
#endif	
  // _Cbus_AbortStartTimer();
   arTxMscMsgs[0] = CBUS_MSC_CTRL_ABORT;
   _Cbus_SendMsgEx(arTxMscMsgs,1);
   tCurrMSCErrorMsg.channel = 2;
   tCurrMSCErrorMsg.u1Data = u1ErrorCode;
   tCurrMSCErrorMsg.ctrl = 0;
   iWriteburstStateNew = MHL_WRITE_BURST_NONE;
   iMhlRxStateNew = MHL_RX_STATE_IDLE;
   mhl_rxQ_read_idx = 0;
   mhl_rxQ_write_idx = 0;
   _MHLStatus = 0;
   u1TxMsgRetrySize = u1TxMsgSavedSizeMsc;
   x_memcpy(au2TxMsgRetry,au2TxMsgSavedMsc,u1TxMsgRetrySize * sizeof(UINT16) );
#ifdef CBUS_DMSG
   LOG(0,"MSC message error abort: 0x%x \n", u1ErrorCode);
#endif
}
static void _Cbus_DDCErrorHandling(UINT8 u1ErrorCode) reentrant
{
   _Cbus_DDCStopTimer();
   //_Cbus_AbortStartTimer();
   arTxMscMsgs[0] = CBUS_DDC_CTRL_ABORT;
   _Cbus_SendMsgEx(arTxMscMsgs,1);
   tCurrDDCErrorMsg.channel = 2;
   tCurrDDCErrorMsg.u1Data = u1ErrorCode;
   tCurrDDCErrorMsg.ctrl = 0;
   iDdcSateNew = DDC_STATE_NONE;
   u1TxMsgRetrySize = 0;
   //when read edid happened abort, need set hpd again 
   if(tDDCStatus.rEdid == 1)
	{
		EdidReadPoniter = 0;
		vCbusSetEdidChg();
		printf("ddc abort\n");
	}
#ifdef CBUS_DMSG
   LOG(0,"DDC message error abort: 0x%x \n", u1ErrorCode);
#endif
}
static void _Cbus_DDCTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{    
    UNUSED(ptTmHandle);
    UNUSED(pvTag);
    tCbusTimerState.u1DdcTimer = CBUS_TIMER_NONE;
   _Cbus_DDCErrorHandling(MHL_MSC_MSG_TIMEOUT);
}
void _Cbus_DDCStartTimer(void)
{
    tCbusTimerState.u1DdcTimer = CBUS_TIMER_WAIT_START;
}

#ifdef SYS_MHL_SUPPORT
void _Cbus_OcpTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{
    if((u1CbusGetOcpStatus(1)) == TRUE)//ocp , nfy
    {
        Mhl_NFYOCPStatus(1, 1);
#if VBUS_CTRL_GPIO
		GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
    }
	else
	{
        Mhl_NFYOCPStatus(1, 0);
#if VBUS_CTRL_GPIO
		GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
	}
}

void _Cbus_OcpStartTimer(void)
{
    x_timer_stop(_hMhlOcpTimer);
    x_timer_start(_hMhlOcpTimer ,TIMER_OCP_POLLING, X_TIMER_FLAG_REPEAT,_Cbus_OcpTimerHandle,NULL);
}
void _Cbus_OcpStopTimer(void)
{
    x_timer_stop(_hMhlOcpTimer);
}

#endif        

static UINT8 _Cbus_SendMsg(void)
{
    UINT8 ctrl;
	UINT8 channel=3;
    UINT8 u1data;
	UINT16 ui2data = 0;
#ifdef CBUS_DMSG
    UINT16 aui2data[MHL_TX_QUEUE_SIZE];
    UINT8 u1dataidx = 0;
#endif
    UINT8 i = 0;
	MHL_MSG_T rMsg;
	MHL_MSG_T *prMsg;
	UINT16 ui2TxMsg = NULL_DATA;

	prMsg = &rMsg;
	while(!IS_MHL_TX_Q_EMPTY())
	{
#ifdef SYS_MHL_SUPPORT		
	    if(_Mhl_TxDequeue(prMsg) == MHL_TX_NEXT_GROUP)
	    {
	       break;
	    }
#endif		
		if(prMsg == NULL)
		{
#ifdef CBUS_DMSG
		    LOG(0, "_Cbus_SendMsg, Msg null1.\n");
#endif	
		}
#ifdef CBUS_DMSG
		else
		{
            prMsg->status.dbgTxData = 1;
	        HAL_GetTime(&(prMsg->time));
		    _dbg_Enqueue(prMsg);
		}
#endif		
		ctrl = prMsg->ctrl;
	    channel = prMsg->channel;
	    u1data = prMsg->u1Data;
        ui2data = ((((channel<<1)|ctrl)<<8)| u1data);
		if(prMsg->ctrl == 1)
		{
            if(ui2TxMsg == NULL_DATA)
            {
			    ui2TxMsg = ui2data;
            }
		}
#ifdef CBUS_DMSG
        aui2data[i] = ui2data;
#endif
        if(channel == 2)
        {
		    au2TxMsgSavedMsc[i] = ui2data;
		}
		else
		{
	    	au2TxMsgSavedDdc[i] = ui2data;
		}
		if( i & 1) 
		{
			vRegWriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,ui2data,FLD_CBUS_WBUF1);
		}
		else
		{
		    vRegWriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,ui2data,FLD_CBUS_WBUF0);
		}
		i++;
	}
#ifdef CBUS_DMSG
    u1dataidx = i;
#endif
	if(channel == 2)
    {
		u1TxMsgSavedSizeMsc = i;
		u1TxCurrMsgChnl = 2;
	}
	else 
	if(channel == 0)
	{
		u1TxMsgSavedSizeDdc = i;
		u1TxCurrMsgChnl = 0;
	}
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,i,FLD_TX_NUM);
    if(i>0)
    {	    
	    CBUS_TRIGGER_TX_HW();
	    if((ui2TxMsg == CBUS_MSC_CTRL_ABORT) || (ui2TxMsg == CBUS_MSC_CTRL_ACK) || (ui2TxMsg == CBUS_MSC_CTRL_NACK) || (ui2TxMsg == NONE_PACKET))
	    {
	        ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
	        ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
	        u2CurrTxMsg = NONE_PACKET;
	    }
	    else
	    if((ui2TxMsg == CBUS_MSC_CTRL_WRITE_STATE) || (ui2TxMsg == CBUS_MSC_CTRL_SET_HPD) || (ui2TxMsg == CBUS_MSC_CTRL_CLR_HPD)
	    || (ui2TxMsg == CBUS_MSC_CTRL_MSC_MSG) || (ui2TxMsg == CBUS_MSC_CTRL_EOF) || (ui2TxMsg == CBUS_MSC_CTRL_WRITE_BURST))
	    {   
	        SetMHLStatus(STATE_MHL_TX_WAITING_ACK);
	        ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
	        u2CurrTxMsg = ui2TxMsg;
	    }
	    else
	    if((ui2TxMsg == CBUS_MSC_CTRL_READ_DEVCAP))
	    {    
	     	SetMHLStatus(STATE_MHL_TX_WAITING_ACK);
		    SetMHLStatus(STATE_MHL_TX_WAITING_DATA);
	        u2CurrTxMsg = ui2TxMsg;
	    }
	    else
	    if((ui2TxMsg == CBUS_MSC_CTRL_GET_STATE) || (ui2TxMsg == CBUS_MSC_CTRL_GET_VENDER_ID) || (ui2TxMsg == CBUS_MSC_CTRL_GET_SC1_EC) ||
	    (ui2TxMsg == CBUS_MSC_CTRL_GET_DDC_EC) || (ui2TxMsg == CBUS_MSC_CTRL_GET_MSC_EC) || (ui2TxMsg == CBUS_MSC_CTRL_GET_SC3_EC))
	    {
	     	 ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
		     SetMHLStatus(STATE_MHL_TX_WAITING_DATA);
			 u2CurrTxMsg = ui2TxMsg;
	    }
	    if(IsMHLStatus(STATE_MHL_TX_WAITING_ACK) || IsMHLStatus(STATE_MHL_TX_WAITING_DATA))
	    {
		    _Cbus_MscStartTimer();
	    }
	    SetMHLStatus(STATE_MHL_TX_SEND_MSG);
	}
    return MHL_SUCCESSFUL;
}
static void _Mhl_InitQueue(void)
{
    mhl_rxQ_read_idx = 0;
    mhl_txQ_read_idx = 0;
    mhl_rxQ_write_idx = 0;
    mhl_txQ_write_idx = 0;
}

static void _Mhl_RxEnqueue(MHL_MSG_T *prMsg)
{  
// check if driver ready

// check if msg is correct

// check
    if (IS_MHL_RX_Q_FULL())
    {
        //return MHL_RX_ERR_QUEUE_FULL;
#ifdef CBUS_DMSG
        LOG(0, "cbus rx queue full\n");
#endif
        return;
    }

    // copy to buffer first
    (void)x_memcpy(&(arRxMscQueue[mhl_rxQ_write_idx]), prMsg, sizeof(MHL_MSG_T));
    // rx write pointer add one
    mhl_rxQ_write_idx = (mhl_rxQ_write_idx + 1) % MHL_RX_QUEUE_SIZE;
}

static void _Mhl_RxDequeue(MHL_MSG_T *prMsg)
{
// check if queue is empty
    if (IS_MHL_RX_Q_EMPTY())
    {
        //return MHL_RX_QUEUE_EMPTY;
#ifdef CBUS_DMSG
        LOG(0,"rx queue empty \n");
#endif
    }
    
    // copy to buffer first
    (void)x_memcpy(prMsg, &(arRxMscQueue[mhl_rxQ_read_idx]), sizeof(MHL_MSG_T));

    mhl_rxQ_read_idx = (mhl_rxQ_read_idx + 1) % MHL_RX_QUEUE_SIZE;
}
UINT8 _Mhl_TxEnqueue(MHL_MSG_T *msg)
{
// check if driver ready

// check if msg is correct

// check
    if (IS_MHL_TX_Q_FULL())
    {
#ifdef CBUS_DMSG
        LOG(0,"tx queue full\n");
#endif
        return MHL_TX_ERR_QUEUE_FULL;
    }

// check tx queue write pointer
    if (mhl_txQ_write_idx >= MHL_TX_QUEUE_SIZE)
    {
        return MHL_TX_ERR_WRITE_POINTER;    
    }

// critical section ?

    // copy to buffer first
    (void)x_memcpy(&(arTxMscQueue[mhl_txQ_write_idx]), msg, sizeof(MHL_MSG_T));
    // tx write pointer add one
    mhl_txQ_write_idx = (mhl_txQ_write_idx + 1) % MHL_TX_QUEUE_SIZE;

// critical section ?

    return MHL_SUCCESSFUL;
}
void vCbusEnableRx(void)
{
    UINT32 dBuf = 0;
    dBuf = u4IO32Read4B(PDWNC_CBUS_LINK_02);
    dBuf |= CTRL_ENABLE_CBUS_RX;
    vIO32Write4B(PDWNC_CBUS_LINK_02,dBuf);
}

void vCbusDisableRx(void)
{
    UINT32 dBuf = 0;
    dBuf = u4IO32Read4B(PDWNC_CBUS_LINK_02);
    dBuf &= ~CTRL_ENABLE_CBUS_RX;
    vIO32Write4B(PDWNC_CBUS_LINK_02,dBuf);
}

void vCbusClrStatusAll(void)
{
    // clr INT
    vIO32Write4B(PDWNC_CBUS_LINK_08, 0xFFFFFFFF);
    vIO32Write4B(PDWNC_CBUS_LINK_08, 0x00000000);   
}

#define EVENT_CABLE_CONNECT 0x200
UINT8 u1MhlCableConnected(void)
{
    UINT32 dBuf = 0;
//    UINT32 dEvent = EVENT_CABLE_CONNECT;
    dBuf = u4IO32Read4B(PDWNC_CBUS_STA_00);

    if (dBuf & 0x200)
    {
    	// clr status
    	vIO32Write4B(0xf0028d54, 0x200);
    	vIO32Write4B(0xf0028d54, 0x0);
    	return 1;
    } 
    else 
    {
        return 0;
    }
}

#define EVENT_CABLE_DISCONNECT 0x400
UINT8 u1MhlCableDisConnected(void)
{
    UINT32 dBuf = 0;
    //UINT32 dEvent = EVENT_CABLE_DISCONNECT;
    dBuf = u4IO32Read4B(PDWNC_CBUS_STA_00);

    if (dBuf & 0x400) 
    {
    	// clr status
    	vIO32Write4B(0xf0028d54, 0x400);
    	vIO32Write4B(0xf0028d54, 0x0);
    	return 1;
    } 
    else 
    {
        return 0;
    }
}

void vCbus_EnableInterrupt(void)
{
	vIO32Write4B(PDWNC_CBUS_LINK_0D,0x2c7fc);
}
void vCbus_DisableInterrupt(void)
{
	vIO32Write4B(PDWNC_CBUS_LINK_0D, 0x00000000);
}


void vCbus_ValInit(void)
{
	x_memset(&tCbusTimerState, 0, sizeof(CBUS_TIMER_STATUS_T));
    u1HDMI_GetEdidData(3, arEdid, EDID_SIZE);
	EdidReadPoniter = 0;
	
	DdcCmd = NONE_PACKET;
	MscCmd = NONE_PACKET;
	
	MscData0 = NULL_DATA;
	MscData1 = NULL_DATA;
	
	DdcData0 = NULL_DATA;

	iMhlStateOld = MHL_STATE_NONE;
	iMhlStateNew = MHL_STATE_NONE;
	
	iDdcSateOld = DDC_STATE_NONE;
	iDdcSateNew = DDC_STATE_NONE;
	
	iWriteburstStateOld = MHL_WRITE_BURST_NONE;
	iWriteburstStateNew = MHL_WRITE_BURST_NONE;
	
	iMhlRxStateOld = MHL_RX_STATE_IDLE;
	iMhlRxStateNew = MHL_RX_STATE_IDLE;
	
	iDdcOffset = 0;
	ReadDevCapOffset = NULL_DATA;
#ifdef HDCP_SUPPORT	
	au1Ri1[0] = 0; 
	au1Ri1[1] = 0;//au1Ri1[2] ={0, 0};
	u1Bcaps = 0;
	au1Bstatus[0]=0; 
	au1Bstatus[1] = 0;//au1Bstatus[2] ={0x0, 0x10}; 
#endif	
	u1ScrachpadOffset = 0xff ;
	u1ScrachpadIndex = 0;	
	MhlSetHpd = 0;
	MhlSetPathen = 0;
	MscReadDevCap = 0;
	u2CurrTxMsg = NULL_DATA;
    u1DebugLvl = 0;
    u11Ktest = 0;
#ifdef HDCP_SUPPORT	
	u1HdcpRsd5Offset= 0;
	u1AnReady = 0;
#endif	
#ifdef SYS_MHL_SUPPORT
    x_memset(&tHDMI_SIL_DevCap_STATUS, 0, sizeof(HDMI_SIL_DevCap_STATUS_T));
#endif	
    GroupId = 0;
	u1CurGroupId= 0;
#ifdef CC_MHL_3D_SUPPORT
    vMHLParsingEDIDForMHL3D(arEdid);
#endif
    u1BootUp = 0;
	u1CableConCnt = 0;
    bCbusState = 1;
    u1HandshakingGroupid = 0;
	u1EdidRetry = 0;
	eResist_state = Resister_None;
#ifdef CBUS_DMSG
    dbg_rxQ_read_idx = 0;
    dbg_rxQ_write_idx=0;
#endif
}
void vCbus_HwInit(void)
{
    UINT32 u4WakeUpReason;
	u4WakeUpReason = PDWNC_ReadWakeupReason();
	LOG(1, "u4WakeUpReason = %d.\n", u4WakeUpReason);
    _Cbus_Set8032Control(0);  
    _arMhlDevCap.DevState = DEVCAP_DEVICE_STATE;
    _arMhlDevCap.MhlVersion = DEVCAP_MHL_VERSION;
    _arMhlDevCap.DevCat = DEVCAP_MHL_DEVICE_CATEGORY;
    _arMhlDevCap.AdopoterIdHigh = DEVCAP_ADOPTER_ID_H;
    _arMhlDevCap.AdopoterIdLow = DEVCAP_ADOPTER_ID_L;
    _arMhlDevCap.VidLinkMode = DEVCAP_VID_LINK_MODE;
    _arMhlDevCap.AudLinkMode = DEVCAP_AUD_LINK_MODE;
    _arMhlDevCap.VideoType = DEVCAP_VIDEO_TYPE;
    _arMhlDevCap.LogDevMap = DEVCAP_LOG_DEV_MAP;
    _arMhlDevCap.BandWidth = DEVCAP_BANDWIDTH;
    _arMhlDevCap.FeatureFlag = DEVCAP_FEATURE_FLAG;
    _arMhlDevCap.DevicIdHigh = DEVCAP_DEVICE_ID_H;
    _arMhlDevCap.DevicIdHigLow = DEVCAP_DEVICE_ID_L;
    _arMhlDevCap.ScratchpadSize = DEVCAP_SCRATCHPAD_SIZE;
    _arMhlDevCap.IntStateSize = DEVCAP_INT_STAT_SIZE;
    _arMhlDevCap.reserved = DEVCAP_RESERVED;
    x_memset(&tCurrDDCErrorMsg , 0,sizeof(MHL_MSG_T));
    x_memset(&tCurrMSCErrorMsg , 0,sizeof(MHL_MSG_T));
    vIO32WriteFldAlign(PDWNC_PADCFG14, 1,FLD_HDMI_0_HPD_CBUS_E8);
	//disable cbus RX			
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
	// reset hw
	// disable wakeup and discovery
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);
             
	// rx INT through hold and Tx bit
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,0,FLD_DUPLEX);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_FAKE_SOURCE);

	// disable cbus disconnect INT			
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0,FLD_CBUS_DISCONN_CNT_EN);
        
	// Enable CBUS Rx
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
			
    // Disable auto bit time
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,1,FLD_ADP_BITIME_EN);
    
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,0x1B,FLD_LINKRX_BITIME);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01, 0x1B, FLD_LINKTX_BITIME);	
    //init read buffer threshold to trigger interrupt 
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,1,FLD_RBUF_LVL_THR);
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,1,FLD_TX_NUM);
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,32,FLD_NRETRY);
    vCbus_DisableInterrupt();
    vCbusClrStatusAll();

	DISABLE_PINMUX_TO_CBUS();
	vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PU);//280e8 bit 3 PU  --- SET TO 0
    //cbus test 
    if((u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER) || 
	   (u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG))
    {
	    vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high Z state
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
    }	
    if(IS_IC_5399_ES2())
    {
		vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_SMT);//SMT ---  SET to 0		
    }
    else
    {
		vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_SMT);//SMT ---  SET to 0		
	}
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_07, 1, FLD_LDO_SWITCH_HW);//28d50 bit 20--- set to 1, control LDO ouput by h/w

    vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,32,FLD_RETRY_DISCONN_THR);//20

    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0,FLD_ZCBUS_HW);//sw control 1k&100k    
    if(u1GetCdsenseStatus())//cable connected,when power on
    {
        _bIsMhlDevice = 1;
        u1BootUp = 1;//for sony 550ms timing sequence
    }
    else //cable disconnected
    {
        _bIsMhlDevice = 0;
        u1BootUp = 0;
    }
    if((u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER) || 
    (u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG))
     {
#if VBUS_CTRL_GPIO
	GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
      }
#ifdef SYS_MHL_SUPPORT
    _bIsMhlDeviceConnect =0;
#endif
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,0,FLD_CBUS_DRV_H_SEL);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_OE_FAST1);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_ADP_SYNC);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_OE_FAST);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_DEGLITCH);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_LINKRX_DIS_TO_ARB23);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_ARB_DRIVEH);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,0xf,FLD_LINK_ACK_WIDTH_UPPER);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,1,FLD_LINK_ACK_MANU_EN);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,0x4,FLD_LINK_ACK_WIDTH);
	vMHLOpenRxTermination(0);
#if 1
   //for 24M clock
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,24,FLD_LINKRX_BITIME);//27
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,16,FLD_ADP_BITIME_MIN);//20
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,30,FLD_ADP_BITIME_MAX);//33
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,42,FLD_CBUS_DISCONN_THR);//47
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A,16,FLD_LINKRX_ACK1_SYNC);//22 ? 27
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,58,FLD_TREQ_ARB_CONT);//65
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,7,FLD_CBUS_DRV_H_PRD);//8
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,455,FLD_DISC_WID_CNT_TIMEOUT);//452 ? 512
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,583,FLD_WAKE_CNT_TIMEOUT);//656
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,78,FLD_DISC_PUL_WID_MIN);//88
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,249,FLD_DISC_PUL_WID_MAX);//280
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,313,FLD_WAKE_PUL_WID2_MIN);//352
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,146,FLD_WAKE_TO_DISC_MIN);//164
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,1458,FLD_WAKE_TO_DISC_MAX);//1640
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,502,FLD_WAKE_PUL_WID2_MAX);//436////for spec it should be 388, but to cover p880 ill pulse, add anohter 10ms
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,100,FLD_WAKE_PUL_WID1_MIN);//112
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,220,FLD_WAKE_PUL_WID1_MAX);//148 //for spec it should be 132, but to cover p880 ill pulse, add anohter 5ms 
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,12,FLD_LINK_BITIME_MID);//13
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,18,FLD_LINK_RXDECISION);//24
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,100,FLD_RX_BT_TIMEOUT);//50 ? 100
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,33,FLD_LINK_BITIME_MAX);//37
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,16,FLD_LINK_BITIME_MIN);//18
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,15,FLD_LINK_SYNCDUTY_MAX);//17
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,9,FLD_LINK_SYNCDUTY_MIN);//10
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,63,FLD_TREQ_ACK_CONT);//27
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,20,FLD_LINK_TXDECISION);//23
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,18,FLD_LINK_HALFTRAN_MAX);//20
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,5,FLD_LINK_HALFTRAN_MIN);//10, cts
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,24,FLD_LINKTX_BITIME);//29
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,9,FLD_CBUS_ACK_0_MIN);//10
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,16,FLD_CBUS_ACK_0_MAX);//19
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,24,FLD_CBUS_ACK_FALL_MAX);//25   	
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,11,FLD_TWAIT); //12//
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,4,FLD_TRESP_HOLD);//4//
#else
	//for 27M clock
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,27,FLD_LINKRX_BITIME);//27
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,18,FLD_ADP_BITIME_MIN);//20
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,33,FLD_ADP_BITIME_MAX);//33
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,47,FLD_CBUS_DISCONN_THR);//47
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A,27,FLD_LINKRX_ACK1_SYNC);//22 ? 27
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,65,FLD_TREQ_ARB_CONT);//65
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,8,FLD_CBUS_DRV_H_PRD);//8
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,512,FLD_DISC_WID_CNT_TIMEOUT);//452 ? 512
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_09,656,FLD_WAKE_CNT_TIMEOUT);//656
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,88,FLD_DISC_PUL_WID_MIN);//88
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,280,FLD_DISC_PUL_WID_MAX);//280
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,352,FLD_WAKE_PUL_WID2_MIN);//352
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,164,FLD_WAKE_TO_DISC_MIN);//164
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_06,1640,FLD_WAKE_TO_DISC_MAX);//1640
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,564,FLD_WAKE_PUL_WID2_MAX);//436///for spec it should be 436, but to cover p880 ill pulse, add another 10ms
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,112,FLD_WAKE_PUL_WID1_MIN);//112
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,182,FLD_WAKE_PUL_WID1_MAX);//148///for spec it should be 436, but to cover p880 ill pulse, add another 10ms
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,13,FLD_LINK_BITIME_MID);//13
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,20,FLD_LINK_RXDECISION);//24
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,79,FLD_RX_BT_TIMEOUT);//50 ? 100
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,37,FLD_LINK_BITIME_MAX);//37
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,18,FLD_LINK_BITIME_MIN);//18
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,17,FLD_LINK_SYNCDUTY_MAX);//17
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,10,FLD_LINK_SYNCDUTY_MIN);//10
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,71,FLD_TREQ_ACK_CONT);//27
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,23,FLD_LINK_TXDECISION);//23
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,20,FLD_LINK_HALFTRAN_MAX);//20
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,6,FLD_LINK_HALFTRAN_MIN);//10
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,27,FLD_LINKTX_BITIME);//27
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,10,FLD_CBUS_ACK_0_MIN);//10
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,18,FLD_CBUS_ACK_0_MAX);//19
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,27,FLD_CBUS_ACK_FALL_MAX);//25   
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,12,FLD_TWAIT); //12//
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,4,FLD_TRESP_HOLD);//4//
#endif
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf0000000,0xf0000000);	
	vUtDelay2us(1);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0,0xf0000000);
    VERIFY(x_timer_create(&_hMhlMscTimer) == OSR_OK);
    VERIFY(x_timer_create(&_hMhlDdcTimer) == OSR_OK);
    VERIFY(x_timer_create(&_hMhlAbortTimer) == OSR_OK);
#ifdef SYS_MHL_SUPPORT	
    VERIFY(x_timer_create(&_hMhlOcpTimer) == OSR_OK);
#endif
    VERIFY(x_timer_create(&_hMhlBootupTimer) == OSR_OK);
}
static void _Cbus_DDCloop(UINT16 *aRxMsgTest, UINT8 u1RxMsgCnts, HAL_TIME_T* ptRxTime)
{
    UINT8 i = 0;
    UINT8 j = 0;
    UINT16 u2RxMsg;
    MHL_MSG_T rRxMsg;
    UINT16 aRxMsg[u1RxMsgCnts];
    UINT8 u1TmpCnt = u1RxMsgCnts;
    while(u1TmpCnt)
    {
         u1TmpCnt -- ;
         if((aRxMsgTest[i] & 0x600) == 0)
         {
             aRxMsg[j] = aRxMsgTest[i];
             j++;
         }
         i++;
    }
    u1RxMsgCnts = j;
    i = 0;   
	while(u1RxMsgCnts)
    //include edid & hdcp
    {
        x_memset(&rRxMsg,0,sizeof(MHL_MSG_T));
		u1RxMsgCnts --;
		u2RxMsg = aRxMsg[i];
		i++;		
		rRxMsg.channel = (u2RxMsg>> 9) & 0x3;
		rRxMsg.ctrl = (u2RxMsg >> 8) & 0x1;
		rRxMsg.u1Data = u2RxMsg & 0xff;
		x_memcpy(&(rRxMsg.time),ptRxTime,sizeof(HAL_TIME_T));
		DdcCmd = NONE_PACKET;
		DdcData0 = NULL_DATA;
		if (rRxMsg.channel == 0)
		{	
			if (rRxMsg.ctrl == 1)
			{
				DdcCmd =(((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
			}
			else if(rRxMsg.ctrl == 0 && DdcData0 == NULL_DATA)
			{
				DdcData0 =	(((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
			}
		}
		if (iDdcSateOld != iDdcSateNew)
		{
#ifdef CBUS_DMSG
#ifndef NDEBUG
			LOG(3,"iDdcState %s --> %s, \n",_aszDdcState[iDdcSateOld],_aszDdcState[iDdcSateNew]);
#endif			
#endif			
			iDdcSateOld = iDdcSateNew;
		}
		if((DdcCmd == NONE_PACKET)&&(DdcData0 == NULL_DATA))
		    return;
		switch (iDdcSateOld)
		{
			case DDC_STATE_NONE:
			    _Cbus_DDCStopTimer();
			    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
				if(CBUS_DDC_CTRL_SOF == u2RxMsg)
				{
					iDdcSateNew = DDC_STATE_SOF;
					_Cbus_DDCStartTimer();
				}
				break;
			 
			case DDC_STATE_SOF:
#ifdef TEST_MSC_IN_DDC			
				if((u2RxMsg & 0x400)==0x400)
				{
					LOG(6,"MSC1 in DDC \n");
					break;
				}
#endif				
				_Cbus_DDCStartTimer();
				iDdcOffset = 0 ;
				//for asus only
				
#if 1				
				if(CBUS_DDC_CTRL_SOF == u2RxMsg)
				{
					iDdcSateNew = DDC_STATE_SOF;
					break;
				}
				else
#endif				
				if (CBUS_DDC_DATA_SEGW == u2RxMsg) 
				{
					arTxMscMsgs[0] = CBUS_DDC_CTRL_NACK;
					iDdcSateNew = DDC_STATE_EOF;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				} 
				else if (CBUS_DDC_DATA_ADRW== u2RxMsg) 
				{
					DdcData0 = NULL_DATA;
					iDdcSateNew = DDC_STATE_OFFSET;
					arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK;
					x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
					tDDCStatus.rEdid = 1;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				} 
				else if (CBUS_DDC_DATA_ADRR == u2RxMsg) 
				{
					DdcData0 = NULL_DATA;
					iDdcSateNew = DDC_STATE_CONT;					
					arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				}
				else if (CBUS_DDC_DATA_HDCP_ADRR== u2RxMsg) 
				{
					DdcData0 = NULL_DATA;
					iDdcSateNew = DDC_STATE_CONT;					
					arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				}
				else if(CBUS_DDC_DATA_HDCP_ADRW == u2RxMsg)
				{
				    tDDCStatus.rEdid = 0;
					DdcData0 = NULL_DATA;
					iDdcSateNew = DDC_STATE_OFFSET;
					arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				}
				else if((u2RxMsg & 0x700)== 0)
				{
					arTxMscMsgs[0] = CBUS_DDC_CTRL_NACK;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
					iDdcSateNew = DDC_STATE_NONE;
				}
				else
				{
				    //abort: protocol error
#ifdef CBUS_DEBUG  
				    LOG(0,"abort 1: 0x%x \n", u2RxMsg);
#endif
					_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
				}
				break;
			 
		   case DDC_STATE_ADRW:
				 break;
			 
		   case DDC_STATE_OFFSET:
#ifdef TEST_MSC_IN_DDC		   
				if((u2RxMsg & 0x400)==0x400)
				{
				   LOG(6,"MSC2 in DDC \n");
				   break;
				}
#endif				
				_Cbus_DDCStartTimer();
				if ((u2RxMsg == CBUS_DDC_CTRL_STOP) || (u2RxMsg == CBUS_DDC_CTRL_EOF)) 
				{
				   iDdcSateNew = DDC_STATE_EOF;
				   break;
				} 
				if(u2RxMsg == CBUS_DDC_CTRL_SOF)
				{
				   iDdcSateNew = DDC_STATE_SOF;
				   break;
				}
				if (DdcData0!= NULL_DATA) 
				{
					iDdcOffset = 0 ;
					if(tDDCStatus.rEdid == 1)
					{
					    EdidReadPoniter = u2RxMsg;
						iDdcSateNew = DDC_STATE_OFFSET1;
					}
#ifdef HDCP_SUPPORT					
					else
					if(CBUS_DDC_DATA_HDCP_AN_OFFSET == u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
					    tDDCStatus.wAn = 1;
					    iDdcSateNew = DDC_STATE_DATAW;
					}
					else
					if(CBUS_DDC_DATA_HDCP_AINFO_OFFSET == u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
						tDDCStatus.wAinfo = 1;
						iDdcSateNew = DDC_STATE_DATAW;
					}
					else
					if(CBUS_DDC_DATA_HDCP_AKSV_OFFSET == u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
					    tDDCStatus.wAksv = 1;
					    iDdcSateNew = DDC_STATE_DATAW;
					}
					else
					if(CBUS_DDC_DATA_HDCP_BKSV_OFFSET == u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
					    iDdcSateNew = DDC_STATE_OFFSET1;
					    tDDCStatus.rBksv = 1;
                        vMHLGetHdcpBksv(au1Bksv);
					}
					else
					if(CBUS_DDC_DATA_HDCP_BCAPS_OFFSET == u2RxMsg)
					{
					     x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
						iDdcSateNew = DDC_STATE_OFFSET1;
					    tDDCStatus.rBcaps= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_BSTATUS_OFFSET== u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
						iDdcSateNew = DDC_STATE_OFFSET1;
					    tDDCStatus.rBstatus= 1;
					}                    
					else
					if(CBUS_DDC_DATA_HDCP_RI1_OFFSET == u2RxMsg)
					{
					    x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
					    iDdcSateNew = DDC_STATE_OFFSET1;
					    tDDCStatus.rRi1 = 1;

					}
					else
					if(CBUS_DDC_DATA_HDCP_RSVD1 == u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rRsd1= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_RSVD2 == u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rRsd2= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_RSVD3 == u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rRsd3= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_RSVD4 == u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rRsd4= 1;
					}
					else
					//if(CBUS_DDC_DATA_HDCP_RSVD5 == u2RxMsg)
					if((CBUS_DDC_DATA_HDCP_RSVD5 <= u2RxMsg) &&(CBUS_DDC_DATA_HDCP_RSVD5 + 124 >= u2RxMsg))
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						u1HdcpRsd5Offset = u2RxMsg;
						tDDCStatus.rRsd5= 1;
						if(u1DebugLvl)
						{
							LOG(0,"u1HdcpRsd5Offset = %d \n", u1HdcpRsd5Offset);
						}
					}
					else
					if(CBUS_DDC_DATA_HDCP_VH0== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rVh0= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_VH1== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rVh1= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_VH2== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rVh2= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_VH3== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rVh3= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_VH4== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rVh4= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_DBG== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rDbg= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_PJ== u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rPj= 1;
					}
					else
					if(CBUS_DDC_DATA_HDCP_KSVFIFO == u2RxMsg)
					{
						x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));  
						iDdcSateNew = DDC_STATE_OFFSET1;
						tDDCStatus.rKsvfifo = 1;
					}
#endif					
					else
					{
						 //abort
						 //wrong offset
						 //_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
					   _Cbus_DDCStopTimer();
					   arTxMscMsgs[0] = CBUS_DDC_CTRL_NACK;
					   _Cbus_SendMsgEx(arTxMscMsgs,1);
					   iDdcSateNew = DDC_STATE_NONE;
                       break;
					}
					DdcData0 = NULL_DATA;
					arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK; 
					_Cbus_SendMsgEx(arTxMscMsgs,1);
					break;
				}
				//abort
				//abort: protocol error
#ifdef CBUS_DEBUG  
				LOG(0,"abort 2: 0x%x \n", u2RxMsg);
#endif
				_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
				break;			
		   case DDC_STATE_OFFSET1:
#ifdef TEST_MSC_IN_DDC		   
			   if((u2RxMsg & 0x400)==0x400)
			   {
				   LOG(6,"MSC3 in DDC \n");
				   break;
			   }		   
#endif			   
				_Cbus_DDCStartTimer();
				if (DdcCmd == CBUS_DDC_CTRL_STOP) 
				{
					iDdcSateNew = DDC_STATE_EOF;
				} 	
				else				
				if (DdcCmd == CBUS_DDC_CTRL_SOF) 
				{
					iDdcSateNew = DDC_STATE_SOF;
				} 	
				else
				if (DdcCmd == CBUS_DDC_CTRL_EOF) 
				{
					iDdcSateNew = DDC_STATE_NONE;
					DdcData0 = NULL_DATA;
					DdcCmd = NONE_PACKET;
				}
				else
				{
				    //abort
				    //abort: protocol error
#ifdef CBUS_DEBUG  
				    LOG(0,"abort 3: 0x%x \n", u2RxMsg);
#endif
					_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
				}
				break;
		   case DDC_STATE_DATAW:
#ifdef TEST_MSC_IN_DDC		   
			   if((u2RxMsg & 0x400)==0x400)
			   {
				   LOG(6,"MSC4 in DDC \n");
				   break;
			   }
#endif			   
		       {	
				   _Cbus_DDCStartTimer();
		       	   if(DdcCmd == CBUS_DDC_CTRL_SOF)
		           {
		               iDdcSateNew = DDC_STATE_SOF;
						break;
				   }
		       	   if(DdcCmd == CBUS_DDC_CTRL_STOP)
		           {
		               iDdcSateNew = DDC_STATE_EOF;
					   break;
		           }
		       	   if(DdcCmd == CBUS_DDC_CTRL_EOF)
		           {
		               iDdcSateNew = DDC_STATE_NONE;
					   break;
		           }
		           if(DdcData0 != NULL_DATA)
		           {
#ifdef HDCP_SUPPORT		           
   			           if(tDDCStatus.wAn)
			           {
			               if(iDdcOffset < 8)
			               {
			                   au1An[iDdcOffset] =  DdcData0;
			                   DdcData0 = NULL_DATA;
						       arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK; 
						       _Cbus_SendMsgEx(arTxMscMsgs,1);
						       iDdcOffset ++;
						       if(iDdcOffset == 8)
						       {
						           bSetAn = 1;
						           //vMHLSetHdcpAn(au1An);
						           if(u1AnReady < 2)
						           {
							           u1AnReady ++;
						           }
						       }
						   }
						   else
						   {
							   //abort: protocol error
#ifdef CBUS_DEBUG  
							   LOG(0,"abort 4: 0x%x \n", u2RxMsg);
#endif
							   _Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
						   }
			           }
			           else if(tDDCStatus.wAksv)
			           {
			               if(iDdcOffset < 5)
			               {
			                   au1Aksv[iDdcOffset] = DdcData0;			                   
			                   DdcData0 = NULL_DATA;
						       arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK; 
						       _Cbus_SendMsgEx(arTxMscMsgs,1);
						       iDdcOffset ++;
							  if(iDdcOffset == 5)
							  {
							      bSetAksv = 1;
								 // vMHLSetHdcpAksv(au1Aksv);
								  if(u1AnReady < 2)
								  {
									  u1AnReady ++;
								  }
							  }
			               }
			               else
			               {
							   //abort: protocol error
#ifdef CBUS_DEBUG  
							   LOG(0,"abort 5: 0x%x \n", u2RxMsg);
#endif
							   _Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
			               }
			           }
			           else if(tDDCStatus.wAinfo)
			           {
			               if(iDdcOffset == 0)
			               {
			                   DdcData0 = NULL_DATA;
						       arTxMscMsgs[0] = CBUS_DDC_CTRL_ACK; 
						       _Cbus_SendMsgEx(arTxMscMsgs,1);
						       iDdcOffset ++;
			               }
			               else
			               {
							   //abort: protocol error
#ifdef CBUS_DEBUG  
							   LOG(0,"abort 6: 0x%x \n", u2RxMsg);
#endif
							   _Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
			               }
			           }
#endif			           			           
			           break;
		           }
				   //abort: protocol error
#ifdef CBUS_DEBUG  
				   LOG(0,"abort 7: 0x%x \n", u2RxMsg);
#endif
				   _Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
		       }
		        break;
		   case DDC_STATE_CONT:
#ifdef TEST_MSC_IN_DDC 		   
			   if((u2RxMsg & 0x400)==0x400)
			   {
				   LOG(6,"MSC5 in DDC \n");
				   break;
			   }
#endif			   
				_Cbus_DDCStartTimer();
				//for asus only
#if 1				
				if(CBUS_DDC_CTRL_SOF == DdcCmd)
				{
					iDdcSateNew = DDC_STATE_SOF;
					break;
				}
				else
#endif		
                if(DdcCmd == CBUS_DDC_CTRL_EOF)
                {
					iDdcSateNew = DDC_STATE_NONE;
					DdcData0 = NULL_DATA;
					DdcCmd = NONE_PACKET;
                }
                else
				if (DdcCmd == CBUS_DDC_CTRL_STOP) 
				{
					iDdcSateNew = DDC_STATE_EOF;
				} 
				else if (DdcCmd == CBUS_DDC_CTRL_CONT) 
				{
#ifdef HDCP_SUPPORT				
				    if(tDDCStatus.rBcaps)
				    {
#if 0				    
						if(iDdcOffset == 1)
						{
							//abort: protocol error
							
							LOG(0,"abort 8: 0x%x \n", u2RxMsg);
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
#endif		
						if(_arHdmiRx[HDMI_SWITCH_1]._bHDMIState == 6)
						{
							if(u1AnReady == 2)
							{
							    u1Bcaps = 0xa1;
							}
							else
							{
								u1Bcaps = 0x80;
							}
						}
						else
						{
						    u1Bcaps = 0x80;
						    u1AnReady = 0;
						}
				        arTxMscMsgs[0] = u1Bcaps;
						_Cbus_SendMsgEx(arTxMscMsgs,1);
						if(u1DebugLvl)
						{
						LOG(0,"tx Bcaps 0x%x, %d\n", u1Bcaps, iDdcOffset);
						}
						DdcCmd = NONE_PACKET;
						iDdcOffset++;
				    }
                    else                 
                    if(tDDCStatus.rRsd1)
                    {
                        if(iDdcOffset == 3)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 9: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rRsd2)
                    {
                        if(iDdcOffset == 5)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 10: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rRsd3)
                    {
                        if(iDdcOffset == 2)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 11: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rRsd4)
                    {
                        if(iDdcOffset == 12)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 12: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rVh0)
                    {
                        if(iDdcOffset == 4)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 13: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rVh1)
                    {
                        if(iDdcOffset == 4)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 14: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rVh2)
                    {
                        if(iDdcOffset == 4)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 15: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rVh3)
                    {
                        if(iDdcOffset == 4)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 16: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rVh4)
                    {
                        if(iDdcOffset == 4)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 17: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
					else					
					if(tDDCStatus.rKsvfifo)
					{
#if 0					
						if(iDdcOffset == 1)
						{
							//abort: protocol error
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
#endif						
						arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1); 	
#ifdef CBUS_DEBUG  
						LOG(0,"tx: 0x%x, offset: %d \n", arTxMscMsgs[0],iDdcOffset);
#endif
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
					}
                    else                    
                    if(tDDCStatus.rDbg)
                    {
                        if(iDdcOffset == 64)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 18: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rPj)
                    {
                        if(iDdcOffset == 1)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 19: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else                    
                    if(tDDCStatus.rRsd5)
                    {
                        if(u1HdcpRsd5Offset == 192)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 20: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = 0;
						_Cbus_SendMsgEx(arTxMscMsgs,1);		
						u1HdcpRsd5Offset ++;
						DdcCmd = NONE_PACKET;
                    }
                    else
                    if(tDDCStatus.rBksv)
                    {
                        if(iDdcOffset == 5)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 21: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
                        arTxMscMsgs[0] = iDdcOffset < 5 ? au1Bksv[iDdcOffset] : au1Bksv[4];
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else
                    if(tDDCStatus.rBstatus)
                    {   
                        if(iDdcOffset == 2)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 22: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
						if(u1IO32Read1B(AUDP_STAT_0 + 0xc00) & 0x1)
						{
						    au1Bstatus[1] = 0x10;
						}
						else
						{
						    au1Bstatus[1] = 0x0;
						}
						arTxMscMsgs[0] = iDdcOffset < 2 ? au1Bstatus[iDdcOffset] : au1Bstatus[1];
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else
                    if(tDDCStatus.rRi1)
                    {
                        if(iDdcOffset == 0)
						{
                            vMHLGetHdcpRi(au1Ri1);
						}
						if(iDdcOffset == 2)
						{
							//abort: protocol error
#ifdef CBUS_DEBUG  
							LOG(0,"abort 23: 0x%x \n", u2RxMsg);
#endif
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
						arTxMscMsgs[0] = iDdcOffset < 2 ? au1Ri1[iDdcOffset] : au1Ri1[1];
						_Cbus_SendMsgEx(arTxMscMsgs,1);			
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
                    }
                    else
#endif                    
					if (EdidReadPoniter <= 255) 
					{

						arTxMscMsgs[0] = EdidReadPoniter < 255 ? arEdid[EdidReadPoniter] : arEdid[255]; 	
						_Cbus_SendMsgEx(arTxMscMsgs,1);
						 EdidReadPoniter++;

						 DdcCmd = NONE_PACKET;
					}	
#ifdef HDCP_SUPPORT					
					else
					{
						//abort: protocol error
						//_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
						//short read and DDC current Read, the offset is automatically set to 0x08
                        if(iDdcOffset == 0)
						{
							vMHLGetHdcpRi(au1Ri1);
						}
						if(iDdcOffset == 2)
						{
							//abort: protocol error
							LOG(0,"abort 24: 0x%x \n", u2RxMsg);
							_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
							break;
						}
						arTxMscMsgs[0] = iDdcOffset < 2 ? au1Ri1[iDdcOffset] : au1Ri1[1];
						_Cbus_SendMsgEx(arTxMscMsgs,1);						
						iDdcOffset++;
						DdcCmd = NONE_PACKET;
						tDDCStatus.rEdid = 0;
					}
#endif					
					break;
				}
				//abort
				break;

		   case DDC_STATE_STOP:
				break;
			 
		   case DDC_STATE_EOF:
#ifdef TEST_MSC_IN_DDC		   
			   if((u2RxMsg & 0x400)==0x400)
			   {
				   LOG(6,"MSC6 in DDC \n");
				   break;
			   }
#endif		           
		   	    _Cbus_DDCStopTimer();
#if 1				
			   if(CBUS_DDC_CTRL_SOF == DdcCmd)
			   {
				   iDdcSateNew = DDC_STATE_SOF;
				   break;
			   }
			   else
#endif		
				if (DdcCmd == CBUS_DDC_CTRL_EOF) 
				{
					iDdcSateNew = DDC_STATE_NONE;
					DdcData0 = NULL_DATA;
					DdcCmd = NONE_PACKET;		
					u1EdidRetry = 0;
				}
				else
				{
				    //abort: protocol error
#ifdef CBUS_DEBUG  
				    LOG(0,"abort 25: 0x%x \n", u2RxMsg);
#endif
					_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
				}
				break;
			default:
			break;
		 }						
	}
}
#ifdef CC_MHL_3D_SUPPORT	
static UINT8 u1write_burst = 0;
#endif
static UINT8 u1_rapk = 0;
static UINT8 u1ucpk[2] = {0, 0};
static UINT8 u1StartReadDev = 0;//cannot set path en when read devicecapiability

void _vCbusHandleRxTask(void)
{
	// process Rx message RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
	MHL_MSG_T t_Msg;
	UINT32 i = 0;
	MHL_MSG_T rRxMsg;
#ifdef CBUS_DMSG
	UINT16 arRxTmpBuf[MHL_MSG_BUF_SIZE];
	UINT8 iCnt = 0;
	x_memset(arRxTmpBuf,0,sizeof(arRxTmpBuf));
#endif	
	while (!IS_MHL_RX_Q_EMPTY()) 
	{
		if((iMhlRxStateNew == MHL_RX_STATE_IDLE) && (!IS_MHL_RX_Q_EMPTY()))
		{
			MscCmd = NULL_DATA;
			MscData0 = NULL_DATA;
			MscData1 = NULL_DATA;
		iMhlRxStateNew = MHL_RX_STATE_MSCMSG;
		}

	    x_memset(&rRxMsg, 0, sizeof(MHL_MSG_T));
		_Mhl_RxDequeue(&rRxMsg);
		if (rRxMsg.ctrl == 1) 
		{
		    if(iMhlRxStateNew != MHL_RX_STATE_MSCMSG)
		    {
		        _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
		        return;
		    }
			MscCmd = (((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
		}
		else if ((rRxMsg.ctrl ==0) && MscData0 == NULL_DATA) 
		{
			MscData0 =	(((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
		}
		else if ((rRxMsg.ctrl ==0) && MscData1 == NULL_DATA ) 
		{
			MscData1 =	(((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
		}
		
#ifdef CBUS_DMSG
		arRxTmpBuf[iCnt] = (((UINT16)rRxMsg.channel)<< 9) + (((UINT16)rRxMsg.ctrl) << 8) + (UINT16)(rRxMsg.u1Data);
		if(u1DebugLvl)
		{
			LOG(3, ">>> msg: %d 0x%x, 0x%x,0x%x,0x%x,0x%x,\n", iCnt, arRxTmpBuf[0],arRxTmpBuf[1],arRxTmpBuf[2],arRxTmpBuf[3],arRxTmpBuf[4]); 	
		}
         	iCnt++;		
#endif		
		if (iMhlRxStateOld != iMhlRxStateNew)
		{
#ifdef CBUS_DMSG
#ifndef NDEBUG
			if(u1DebugLvl)
			{
				LOG(3,"iMhlRxState %s --> %s, \n",_aszMhlRxState[iMhlRxStateOld],_aszMhlRxState[iMhlRxStateNew]);
			}
#endif				
#endif			
			iMhlRxStateOld = iMhlRxStateNew;
		}
		switch (iMhlRxStateOld)
		{
			case MHL_RX_STATE_IDLE:
			    MscCmd = NULL_DATA;
			    MscData0 = NULL_DATA;
			    MscData1 = NULL_DATA;
				break;
			case MHL_RX_STATE_MSCMSG:
				_Cbus_MscStartTimer();
				if((!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
				{
				   if((rRxMsg.channel == 0x01) || (rRxMsg.channel == 0x3))
				   {
				       iMhlRxStateNew = MHL_RX_STATE_IDLE;
				       break;
				   }
				   if(MscCmd != NULL_DATA)
					{
						if((MscCmd == CBUS_MSC_CTRL_ACK)|| (MscCmd == CBUS_MSC_CTRL_NACK) || (MscCmd == CBUS_MSC_CTRL_ABORT)||
						(MscCmd == CBUS_MSC_CTRL_SET_HPD) || (MscCmd == CBUS_MSC_CTRL_CLR_HPD) || (MscCmd == CBUS_MSC_CTRL_EOF)
						|| (MscCmd == CBUS_DDC_CTRL_ACK) || (MscCmd == CBUS_DDC_CTRL_NACK) || (MscCmd == CBUS_DDC_CTRL_ABORT)
						|| (MscCmd == CBUS_DDC_CTRL_EOF))
						{
							//abort,idle
							_Cbus_MscErrorHandling(MHL_MSC_BAD_OPCODE);
#ifdef CBUS_DMSG
							LOG(0,"abort for bad opcode@MHL_RX_STATE_MSCMSG 1, 0x%x", MscCmd);
#endif
							iMhlRxStateNew = MHL_RX_STATE_IDLE;
							MscCmd = NONE_PACKET;
							break;
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_STATE) 
						{
							_Cbus_MscStopTimer();
							arTxMscMsgs[0] = 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get state 0x%08x", MscCmd); 
#endif						  
						  iMhlRxStateNew = MHL_RX_STATE_IDLE;
              break;
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_VENDER_ID)
						{
							_Cbus_MscStopTimer();
							arTxMscMsgs[0] = 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get vendor id 0x%08x", MscCmd); 
#endif						  
			              iMhlRxStateNew = MHL_RX_STATE_IDLE;
              break;						
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_DDC_EC)
						{
							_Cbus_MscStopTimer();
							//to do get ddc error code
							arTxMscMsgs[0] = (((UINT16)tCurrDDCErrorMsg.ctrl) << 8) + (UINT16)(tCurrDDCErrorMsg.u1Data);
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get ddc ecc 0x%08x", MscCmd); 
#endif						  
				             iMhlRxStateNew = MHL_RX_STATE_IDLE;
             break;
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_SC3_EC)
						{
							//to do get sc3 error code
							_Cbus_MscStopTimer();
							arTxMscMsgs[0] = 0x400;
							
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get ddc ecc 0x%08x", MscCmd); 
#endif						  
					        iMhlRxStateNew = MHL_RX_STATE_IDLE;
             break;
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_MSC_EC)
						{
							_Cbus_MscStopTimer();
							//to do get ddc error code
							arTxMscMsgs[0] = 0x400 + (((UINT16)tCurrMSCErrorMsg.ctrl) << 8) + (UINT16)(tCurrMSCErrorMsg.u1Data);;
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get msc ecc 0x%08x", MscCmd); 
#endif
				            iMhlRxStateNew = MHL_RX_STATE_IDLE;
             break;
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_GET_SC1_EC)
						{
							_Cbus_MscStopTimer();
							//to do get ddc error code
							arTxMscMsgs[0] = 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs,1);
#ifdef CBUS_DMSG
							LOG(3, "reply get ddc ecc 0x%08x", MscCmd); 
#endif						  
				            iMhlRxStateNew = MHL_RX_STATE_IDLE;
             break;
						}						
						else
						if((MscCmd == CBUS_MSC_CTRL_READ_DEVCAP) || (MscCmd == CBUS_MSC_CTRL_WRITE_STATE) /*|| (MscCmd == CBUS_MSC_CTRL_SET_INT)*/)
						{    
							iMhlRxStateNew = MHL_RX_STATE_MSCOFFSET;
#ifdef CBUS_DMSG
				           LOG(3,"read devcap or write state received @ MHL_RX_STATE_MSCMSG 0x%x\n", MscCmd);
#endif
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_MSC_MSG) 
						{
#ifdef CBUS_DMSG
							LOG(3,"MSCMSG received @ MHL_RX_STATE_MSCMSG 0x%x \n",MscCmd);
#endif
							iMhlRxStateNew = MHL_RX_STATE_MSCDATA0;
						}	
						else
						{
						   _Cbus_MscErrorHandling(MHL_MSC_BAD_OPCODE);
#ifdef CBUS_DMSG
							LOG(0,"abort for bad opcode@MHL_RX_STATE_MSCMSG 3, 0x%x", MscCmd);
#endif
							MscCmd = NONE_PACKET;
	                        iMhlRxStateNew = MHL_RX_STATE_IDLE;
	                        break;
						}
					}
					else//MscCmd = null_data
					{
					   //abort
					   MscCmd = NONE_PACKET;
					   _Cbus_MscErrorHandling(MHL_MSC_BAD_OPCODE);
#ifdef CBUS_DMSG
						LOG(0,"abort for bad opcode@MHL_RX_STATE_MSCMSG 2, 0x%x", MscData0);
#endif
                        iMhlRxStateNew = MHL_RX_STATE_IDLE;
                        break;
					}
				}
				else
				if((IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
				{
					_Cbus_MscStopTimer();		
					ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
			        if(MscCmd == CBUS_MSC_CTRL_ACK)
					{	
#ifdef CBUS_DMSG
						LOG(3,"ACK for 0x%x @MHL_RX_STATE_MSCMSG\n", u2CurrTxMsg);
#endif
						if((u2CurrTxMsg == CBUS_MSC_CTRL_SET_HPD)&&(MhlSetHpd == 1))
						{
						    ///////////////////////
							MhlSetHpd = 0;
							MhlSetPathen = 1;
							SetMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
						}	
						else
						if(MhlSetPathen)
						{
						    if(MhlSetPathen < 4)
						    {
						        if(u1HandshakingGroupid == u1CurGroupId)
						        {
								    //MhlSetPathen ++;
								    SetMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
							    }
							}
							else
							{
							    ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
							}
						}
#ifdef CC_MHL_3D_SUPPORT		
						if(u2CurrTxMsg == CBUS_MSC_CTRL_WRITE_BURST)
						{
						    u1write_burst = 2;
						}
						else
						if(u2CurrTxMsg == CBUS_MSC_CTRL_WRITE_STATE)
						{
						    if((au2TxMsgSavedMsc[1] == CBUS_MSC_REG_REGCHANGE_INT) && (au2TxMsgSavedMsc[2] == (INT_DSCR_CHG | 0x400)))
						    {
								u1write_burst = 3;
						    }
				        }
#endif				        
				        iMhlRxStateNew = MHL_RX_STATE_IDLE;
                     break;
					}
					else
					if(MscCmd == CBUS_MSC_CTRL_NACK)
					{
						if((MhlSetHpd == 1) || ((MhlSetPathen > 0) && (MhlSetPathen < 4)))
						{
							ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
							//_Cbus_AbortStartTimer();
							break;
						}
#ifdef CBUS_DMSG
			            LOG(3,"NACK for 0x%x @MHL_RX_STATE_MSCMSG\n", u2CurrTxMsg);
#endif
						iMhlRxStateNew = MHL_RX_STATE_IDLE;
			            break;
					}
					else
					if(MscCmd == CBUS_MSC_CTRL_ABORT)
					{
					    if((MhlSetHpd == 1) || ((MhlSetPathen > 0) && (MhlSetPathen < 4)))
					    {
							ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
							//_Cbus_AbortStartTimer();
					        break;
					    }
#ifdef CBUS_DMSG

						LOG(3,"ABORT for 0x%x @MHL_RX_STATE_MSCMSG\n", u2CurrTxMsg);
#endif
						iMhlRxStateNew = MHL_RX_STATE_IDLE;
                        break;
					}
					else
					{
						//abort
						 iMhlRxStateNew = MHL_RX_STATE_IDLE;
						_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG

						LOG(0,"abort for protocolerror@MHL_RX_STATE_MSCMSG 3, 0x%x", MscData0);
#endif
						break;
					}
					
				}
				else
				if(!(IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
				{
				   _Cbus_MscStopTimer();
				   if(MscCmd != NULL_DATA)
				   {

						if(MscCmd == CBUS_MSC_CTRL_ABORT)
						{
							ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
							iMhlRxStateNew = MHL_RX_STATE_IDLE;
							break;
						}
				      //abort 
				        ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
		                iMhlRxStateNew = MHL_RX_STATE_IDLE;
					    _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
						LOG(0,"abort MHL_MSC_PROTOCOL_ERR@MHL_RX_STATE_MSCMSG 4, 0x%x", MscData0);
#endif
					  break;
				   }
				   else
				   if(MscData0 != NULL_DATA)
				   {
				       if((MscData0 & 0x400) != 0x400)
				       {  
				           //abort
			               iMhlRxStateNew = MHL_RX_STATE_IDLE;
						   ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
							_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
							LOG(0,"abort MHL_MSC_PROTOCOL_ERR@MHL_RX_STATE_MSCMSG 5, 0x%x", MscData0);
#endif
							break;
					     }
					     else
					     {
						       ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
						       iMhlRxStateNew = MHL_RX_STATE_IDLE;
                   break;
						   //to do this data can be processed
					     }
				   }
				}
				else
				if((IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
				{
					if((MscCmd == CBUS_MSC_CTRL_ACK))
					{
						MscCmd = NULL_DATA;
						ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
						iMhlRxStateNew = MHL_RX_STATE_MSCDATA0;
#ifdef CBUS_DMSG
			            LOG(3,"ack received @ MHL_RX_STATE_MSCMSG \n");
#endif
					}
					else
					if((MscCmd == CBUS_MSC_CTRL_NACK) || (MscCmd == CBUS_MSC_CTRL_ABORT))
					{
						_Cbus_MscStopTimer();
					    ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
					    ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
						iMhlRxStateNew = MHL_RX_STATE_IDLE;
						//to do this data can be processed
						if(MscCmd == CBUS_MSC_CTRL_NACK)
						{
#ifdef CBUS_DMSG
							LOG(3,"NACK for 0x%x @MHL_RX_STATE_MSCMSG\n", u2CurrTxMsg);
#endif
						}
						else
						if(MscCmd == CBUS_MSC_CTRL_ABORT)
						{
#ifdef CBUS_DMSG
							LOG(3,"ABORT for 0x%x @MHL_RX_STATE_MSCMSG\n", u2CurrTxMsg);
#endif
						}
						break;
					}
					else
					{
					    //abort
					    ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
					    ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
					    iMhlRxStateNew = MHL_RX_STATE_IDLE;
						_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
						LOG(0,"abort for protocol error@MHL_RX_STATE_MSCMSG 7");
#endif
						break;
					}
				    
				}	
				break;
			case MHL_RX_STATE_MSCOFFSET:
				_Cbus_MscStartTimer();
			    if(MscCmd == CBUS_MSC_CTRL_READ_DEVCAP) 
			    {
					_Cbus_MscStopTimer();
				    switch(MscData0)
				    {
						case CBUS_MSC_DEVCAP_DEV_STATE:
							u1StartReadDev = 1;
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.DevState) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						
						case CBUS_MSC_DEVCAP_MHL_VERSION:

							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.MhlVersion) | 0x400; 				
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_DEV_CAT:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.DevCat) | 0x400; 
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_ADOPTER_ID_H:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.AdopoterIdHigh) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_ADOPTER_ID_L:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.AdopoterIdLow) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_VID_LINK_MODE:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.VidLinkMode) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_AUD_LINK_MODE:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.AudLinkMode) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_VIDEO_TYPE:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.VideoType) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);									
						break;
						case CBUS_MSC_DEVCAP_LOG_DEV_MAP:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.LogDevMap) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_BANDWIDTH:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.BandWidth) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_FEATURE_FLAG:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.FeatureFlag) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_DEVICE_ID_H:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.DevicIdHigh) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_DEVICE_ID_L:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.DevicIdHigLow) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_SCRATCHPAD_SIZE:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.ScratchpadSize) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_INT_STATE_SIZE:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.IntStateSize) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
						break;
						case CBUS_MSC_DEVCAP_RESERVED:
							arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
							arTxMscMsgs[1] = (_arMhlDevCap.reserved) | 0x400;
							_Cbus_SendMsgEx(arTxMscMsgs, 2);
							u1StartReadDev = 2;
						break;
						default:
						   //send abort message
						   if((MscData0& 0x400) == 0x400)
						   {
						       _Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
#ifdef CBUS_DMSG
				               LOG(0,"abort bad offset for DEV_CAP@MHL_RX_STATE_MSCOFFSET 1, 0x%x", MscData0);
#endif
						   }
						   else
						   {
						       _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
							   LOG(0,"abort other instead of data for DEV_CAP,protcol error@MHL_RX_STATE_MSCOFFSET 2 0x%x", MscData0);
#endif
						   }
						break;
					  }					
					  iMhlRxStateNew = MHL_RX_STATE_IDLE;
					  break;
			    }
			    else
			    if((MscCmd == CBUS_MSC_CTRL_WRITE_STATE)/* || (MscCmd == CBUS_MSC_CTRL_SET_INT)*/)
			    {
					if((MscData0 >= MIN_INT_REG_OFFSET) && (MscData0 <= MAX_STAT_REG_OFFSET))
					{
						iMhlRxStateNew = MHL_RX_STATE_MSCDATA0;
#ifdef CBUS_DMSG
						LOG(3,"valid offset for CBUS_MSC_CTRL_WRITE_STATE@MHL_RX_STATE_MSCOFFSET 0x%x \n",MscData0);
#endif
						break;
					}
			        else
					{
						//send abort message
						if((MscData0& 0x400) == 0x400)
						{
							iMhlRxStateNew = MHL_RX_STATE_IDLE;

							_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
#ifdef CBUS_DMSG
							LOG(0,"abort bad offset for DEV_CAP@MHL_RX_STATE_MSCOFFSET,3 0x%x", MscData0);
#endif
							break;
						}
						else
						{
							iMhlRxStateNew = MHL_RX_STATE_IDLE;

							_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
							LOG(0,"abort other instead of data for DEV_CAP,protcol error@MHL_RX_STATE_MSCOFFSET 4 0x%x", MscData0);
#endif
							break;     
						}
					}
			    }
				break;
			case MHL_RX_STATE_MSCDATA0:
				_Cbus_MscStartTimer();
			    if(IsMHLStatus(STATE_MHL_TX_WAITING_DATA))
			    {
					  _Cbus_MscStopTimer();
					  ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
				    if(MscCmd == NULL_DATA)
				    {
				        if((MscData0 & 0x400) == 0x400)
				        {
							    if(u2CurrTxMsg == CBUS_MSC_CTRL_READ_DEVCAP)
							    {
								    MscReadDevCap = 1;
								    switch(ReadDevCapOffset)
								    {
										case CBUS_MSC_DEVCAP_DEV_STATE: 		
											_arMhlSrcDevCap.DevState = (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_MHL_VERSION:
											_arMhlSrcDevCap.MhlVersion= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_DEV_CAT:
											_arMhlSrcDevCap.DevCat= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_ADOPTER_ID_H:
											_arMhlSrcDevCap.AdopoterIdHigh= (UINT8)(MscData0 & 0xff);
#ifdef SYS_MHL_SUPPORT
											tHDMI_SIL_DevCap_STATUS.DevCapValue_ADOPTER_ID_H = _arMhlSrcDevCap.AdopoterIdHigh;
#endif									
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_ADOPTER_ID_L:
											_arMhlSrcDevCap.AdopoterIdLow= (UINT8)(MscData0 & 0xff);
#ifdef SYS_MHL_SUPPORT
											tHDMI_SIL_DevCap_STATUS.DevCapValue_ADOPTER_ID_L = _arMhlSrcDevCap.AdopoterIdLow;
#endif									
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_VID_LINK_MODE:
											_arMhlSrcDevCap.VidLinkMode= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_AUD_LINK_MODE:
											_arMhlSrcDevCap.AudLinkMode= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_VIDEO_TYPE:
											_arMhlSrcDevCap.VideoType= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_LOG_DEV_MAP:
											_arMhlSrcDevCap.LogDevMap= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_BANDWIDTH:
											_arMhlSrcDevCap.BandWidth= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_FEATURE_FLAG:
											_arMhlSrcDevCap.FeatureFlag= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_DEVICE_ID_H:
											_arMhlSrcDevCap.DevicIdHigh= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_DEVICE_ID_L:
											_arMhlSrcDevCap.DevicIdHigLow= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_SCRATCHPAD_SIZE:
											_arMhlSrcDevCap.ScratchpadSize = (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_INT_STATE_SIZE:
											_arMhlSrcDevCap.IntStateSize = (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset ++;
										break;
										case CBUS_MSC_DEVCAP_RESERVED:
											_arMhlSrcDevCap.reserved= (UINT8)(MscData0 & 0xff);
											ReadDevCapOffset = NULL_DATA;
											MscReadDevCap = 0;
#ifdef SYS_MHL_SUPPORT
	                                        Mhl_NTFDevCap(&tHDMI_SIL_DevCap_STATUS);
#endif
										break;
										default:
										break;
									}
								}							
							    MscData0 = NULL_DATA;
							    iMhlRxStateNew = MHL_RX_STATE_IDLE;
							    break;
				        }
				    }
				   //send abort message
				   iMhlRxStateNew = MHL_RX_STATE_IDLE;
				   _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
				   LOG(0,"abort other instead of data for DEV_CAP,protcol error@MHL_RX_STATE_MSCDATA0 0 0x%x", MscData0);
#endif
			    }
			    else
			    if(MscCmd == CBUS_MSC_CTRL_WRITE_STATE)
			    {
				    _Cbus_MscStopTimer();
				     iMhlRxStateNew = MHL_RX_STATE_IDLE;
			        if((MscData0 & 0x400) == 0x400)
			        {
					    arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK; 	
					    _Cbus_SendMsgEx(arTxMscMsgs, 1);

						if(MscData0 == CBUS_MSC_REG_READY_BITS) 
						{
							 if(MscData1 & 1)//DCAP_RDY
							 {		
#ifdef CBUS_DMSG
								LOG(3,"capability register values are stable\n");
#endif
								 //to do
							 }				 
						}
						else
						if(MscData0 == CBUS_MSC_REG_ACTIVE_LINK_MODE)
						{
							switch(MscData1 & 0x7)
							{
								case 2:
#ifdef CBUS_DMSG
									LOG(3,"packet pixel clk mode\n");//reserved
#endif
								    _bIsMhlPPMode = 1;
									arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
									arTxMscMsgs[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;
									arTxMscMsgs[2] = 0x40a;  
									//_Cbus_SendMsgEx(arTxMscMsgs,3);
									 for(i=0;i<3;i++)
									 {
										 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		 
										 _Mhl_TxEnqueue(&t_Msg);		 
									 }
									 GroupId ++;

									break;
								case 3:
#ifdef CBUS_DMSG
									LOG(3,"normal24-bit clk mode\n");//reserved
#endif
								    _bIsMhlPPMode = 0;

									break;
								default:
#ifdef CBUS_DMSG
									LOG(3,"reserved link mode\n");//reserved
#endif
									break;		
								
							}
							if(MscData1 & 0x8)
							{
#ifdef CBUS_DMSG

								LOG(3,"path enable\n");//reserved
#endif
							}
							if(MscData1 & 0x10)
							{
#ifdef CBUS_DMSG
								LOG(3,"content muted\n");//reserved
#endif
							}
						}
						else		
						if(MscData0 == CBUS_MSC_REG_REGCHANGE_INT)
						{
							 //to do
							 if(MscData1 & INT_DCAP_CHG)//DCAP_CHG
							 {	
#if 0							 
							     MscReadDevCap = 1;
							     ReadDevCapOffset = CBUS_MSC_DEVCAP_DEV_STATE;
#endif							     
							     //start to read devcap
							 }
							 else
							 if(MscData1 & INT_REQ_WRT)
							 {
								 if(!(IS_CBUS_BUSY()))
								 {
									 arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
									 arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
									 arTxMscMsgs[2] = 0x400 | INT_GRT_WRT;
									 //_Cbus_SendMsgEx(arTxMscMsgs,3);
									 for(i=0;i<3;i++)
									 {
										 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		 
										 _Mhl_TxEnqueue(&t_Msg);		 
									 }
	                                 GroupId ++;
								 }
							 }
							 else
							 if(MscData1 & INT_DSCR_CHG)
							 {
							    //write burst finished
#ifdef CBUS_DMSG
								LOG(3,"write burst finished\n");
#endif
							 }
							 else
							 if(MscData1 & INT_GRT_WRT)
							 {
							 //init write burst command
							 //write data byts into scratchpad registers
#ifdef CBUS_DMSG
								 LOG(3,"Write burst@MHL_RX_STATE_MSCDATA0\n");
#endif
#ifdef CC_MHL_3D_SUPPORT		
                                u1write_burst = 1;
#endif
							 }
#ifdef CC_MHL_3D_SUPPORT						
							 else
							 if(MscData1 & INT_3D_REQ)
							 {
								 arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
								 arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
								 arTxMscMsgs[2] = 0x400 | INT_REQ_WRT;
								 for(i=0;i<3;i++)
								 {
									 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		 
									 _Mhl_TxEnqueue(&t_Msg);		 
								 }
                                 GroupId ++;
							 }
#endif						    
						}
						else
						if(MscData0 == CBUS_MSC_REG_DDCHANGE_INT)
						{
							//to do
						} 
					}
					else
					{
					    //abort
						_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
					   LOG(0,"abort other instead of data for DEV_CAP,protcol error@MHL_RX_STATE_MSCDATA0 5 0x%x", MscData0);
#endif
					}
          break;
			    }
			    else
			    if(MscCmd == CBUS_MSC_CTRL_MSC_MSG)
			    {	
					if((MscData0 & 0x700) == 0x400)
			       {
					   iMhlRxStateNew = MHL_RX_STATE_MSCDATA1;
					   break;
			       }
			       else
			       {
					   iMhlRxStateNew = MHL_RX_STATE_IDLE;
					   //abort
					  _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
					  break;
			       }			        
			    }
				break;
			case MHL_RX_STATE_MSCDATA1:	
					_Cbus_MscStopTimer();
					iMhlRxStateNew = MHL_RX_STATE_IDLE;
					if((MscData1 & 0x700) == 0x400)
					{	
						arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK;
						_Cbus_SendMsgEx(arTxMscMsgs,1);
					}
#if   1				
					if((MscData1 & 0x700) == 0x400)
					{
					   if(MscData0 == MHL_MSC_MSG_RAP)
					   {
                          if(MscData1 == MHL_RAP_CMD_CONTENTON)
						  {
							 vMHLSETRAPContentOnoff(1);
						  }
						  else
						   if(MscData1 == MHL_RAP_CMD_CONTENTOFF)
						   {
							 vMHLSETRAPContentOnoff(0);
						   }
						  u1_rapk = 1;
						  /*arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
						  arTxMscMsgs[1] = MHL_MSC_MSG_RAPK;
						  arTxMscMsgs[2] = 0x400 | (Mhl_GetSystemPortStatus(0));
						  _Cbus_SendMsgEx(arTxMscMsgs,3);*/
					       //reply with rapk					       
#ifdef SYS_MHL_SUPPORT					       
				           Mhl_NFYRAP((UINT8)(MscData1 & 0xff));
#endif					   
					   }
					   else
					   if(MscData0 == MHL_MSC_MSG_RAPK) 
					   {
#ifdef SYS_MHL_SUPPORT
					       Mhl_NFYRAPK((UINT8)(MscData1 & 0xff));
#endif					   					       
					   }
					   else
					   if(MscData0 == MHL_MSC_MSG_RCP)
					   {
					       //check key code
#ifdef SYS_MHL_SUPPORT
						  u1RevRcpKeyCode = (UINT8)(MscData1 & 0xff);
#endif								       
					   }
					   else
					   if(MscData0 == MHL_MSC_MSG_RCPK)
					   {
#ifdef SYS_MHL_SUPPORT
				          Mhl_NFYRcpK((UINT8)(MscData1 & 0xff));
#endif		
					   }
					   else
					   if(MscData0 == MHL_MSC_MSG_RCPE)
					   {
#ifdef SYS_MHL_SUPPORT
						 Mhl_NFYRcpE((UINT8)(MscData1 & 0xff));
#endif		
					   }
					   else
                       if(MscData0 == MHL_MSC_MSG_UCP)
						{
							u1ucpk[0] = 1;
							u1ucpk[1] = MscData1;
							 /*arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
							 arTxMscMsgs[1] = MHL_MSC_MSG_UCPK;
							 arTxMscMsgs[2] = MscData1;
							_Cbus_SendMsgEx(arTxMscMsgs,3);*/
							
#ifdef SYS_MHL_SUPPORT
							 Mhl_NFYUcp((UINT8)(MscData1 & 0xff));
#endif		
						}					  
					  else
					  if(MscData0 == MHL_MSC_MSG_UCPK)
					  {
#ifdef SYS_MHL_SUPPORT
						   Mhl_NFYUcpK((UINT8)(MscData1 & 0xff));
#endif							  
					  }
					  else
					  if(MscData0 == MHL_MSC_MSG_UCPE)
					  {
#ifdef SYS_MHL_SUPPORT
						   Mhl_NFYUcpE((UINT8)(MscData1 & 0xff));
#endif							  
					  }		
#if 0					  
					   else
					   {
					       //abort
						  // _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
					   }
#endif					   
					}
#endif					
					else
					{
						//abort
                       _Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
#ifdef CBUS_DMSG
					   LOG(3,"abort other instead of data for DEV_CAP,protcol error@MHL_RX_STATE_MSCDATA1 0x%x", MscData0);
#endif
					}
				break;
			default:
				break;
			}
		}
	}


//this function is used to handle reply data which need some delay
void vCbusReplyData(void)
{
    MHL_MSG_T t_Msg;
    UINT32 i =0;
	if((u1_rapk == 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)) && (u1StartReadDev != 1))
	{
		u1_rapk = 0;
		arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		arTxMscMsgs[1] = MHL_MSC_MSG_RAPK;
		arTxMscMsgs[2] = 0x400;
		//_Cbus_SendMsgEx(arTxMscMsgs,3);
		for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	  
			_Mhl_TxEnqueue(&t_Msg); 	  
		}
		GroupId ++;
	}
#ifdef CC_MHL_3D_SUPPORT				
	if((u1write_burst == 1) && (u1StartReadDev != 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
	{
		u1write_burst = 0;
		x_thread_delay(2);
		vCbusWriteBurst3D();
	}
	else
	if((u1write_burst == 2) && (u1StartReadDev != 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
	{
	    x_thread_delay(2);	
		u1write_burst = 0;
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
		arTxMscMsgs[2] = INT_DSCR_CHG | 0x400;	
		//_Cbus_SendMsgEx(arTxMscMsgs,3);
		for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	  
			_Mhl_TxEnqueue(&t_Msg); 	  
		}
		GroupId ++;
	}
	else
	if((u1write_burst == 3) && (u1StartReadDev != 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
	{
	    u1write_burst = 0;
		x_thread_delay(2);
		if((VicWriteburstIdx < VicWriteburstCnt) || (DtdWriteburstIdx < DtdWriteburstCnt))
		{
			arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
			arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
			arTxMscMsgs[2] = INT_REQ_WRT | 0x400;	
			//_Cbus_SendMsgEx(arTxMscMsgs,3);
			for(i=0;i<3;i++)
			{
				vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	  
				_Mhl_TxEnqueue(&t_Msg); 	  
			}
			GroupId ++;
		}
		else
		{
			VicWriteburstIdx = 0;
			DtdWriteburstIdx = 0;
		}
	}	
#endif
	if((u1ucpk[0] == 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (!IsMHLStatus(STATE_MHL_TX_WAITING_DATA))  && (u1StartReadDev != 1))
	{
		 u1ucpk[0] = 0;
		 //need to confirm it can work well
		 arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		 arTxMscMsgs[1] = MHL_MSC_MSG_UCPK;
		 arTxMscMsgs[2] = 0x400 + u1ucpk[1];
		 for(i=0;i<3;i++)
		 {
			 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	   
			 _Mhl_TxEnqueue(&t_Msg);	   
		 }
		 GroupId ++;
	}
#ifdef SYS_MHL_SUPPORT
	if(u1RevRcpKeyCode != 0xff)
	{
		if(Mhl_NFYRcp(u1RevRcpKeyCode))
		{
		//rcpk
		  arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		  arTxMscMsgs[1] = MHL_MSC_MSG_RCPK;
		  arTxMscMsgs[2] = u1RevRcpKeyCode + 0x400;
		  for(i=0;i<3;i++)
		  {
			  vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg); 	
			  _Mhl_TxEnqueue(&t_Msg);		
		  }
		  GroupId ++;
		}
		else
		{
		//rcpe
		   arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		   arTxMscMsgs[1] = MHL_MSC_MSG_RCPE;
		   arTxMscMsgs[2] = 0x401;
		   for(i=0;i<3;i++)
		   {
			   vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);	 
			   _Mhl_TxEnqueue(&t_Msg);		 
		   }
		   GroupId ++;
		}
		u1RevRcpKeyCode = 0xff;
	}
#endif								       
	

}

static void _Cbus_MSCloop(UINT16 *aRxMsg, UINT8 u1RxMsgCnts, HAL_TIME_T *ptRxTime)
{
   //include devcap
   //include write burst
   UINT8 i = 0;
   UINT16 u2RxMsg;
   MHL_MSG_T rRxMsg;
   while(u1RxMsgCnts)
   {
	
		u1RxMsgCnts --;
		u2RxMsg = aRxMsg[i];
		i++;	
		x_memset(&rRxMsg, 0, sizeof(MHL_MSG_T));
		rRxMsg.channel = (u2RxMsg>> 9) & 0x3;
		rRxMsg.ctrl = (u2RxMsg >> 8) & 0x1;
		rRxMsg.u1Data = u2RxMsg & 0xff;
			//MscMsgReply = 0x100;
		if (iWriteburstStateOld != iWriteburstStateNew)
		{
#ifdef CBUS_DMSG
#ifndef NDEBUG
			LOG(3,"iMhlWriterBurstState %s --> %s, \n",_aszWriteBurstState[iWriteburstStateOld],_aszWriteBurstState[iWriteburstStateNew]);
#endif
#endif			
			iWriteburstStateOld = iWriteburstStateNew;
		}
			
		switch (iWriteburstStateOld)
		{
			//au1ScratchPad[]//to do
			case MHL_WRITE_BURST_NONE:
			    if(u2RxMsg == CBUS_MSC_CTRL_WRITE_BURST)
			    {
					_Cbus_MscStartTimer();
			        iWriteburstStateNew = MHL_WRITE_BURST_START;
			    }
			    break;
			case MHL_WRITE_BURST_START:
				_Cbus_MscStartTimer();
			    if((u2RxMsg & 0x100)== 0x100)
			    {
					//abort control instead of data
					_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
					iWriteburstStateNew = MHL_WRITE_BURST_EOF;
			    }
			    else
                if((u2RxMsg >= MIN_SCRACHPAD_REG_OFFSET) && (u2RxMsg <= MAX_SCRACHPAD_REG_OFFSET))
			    {
					u1ScrachpadOffset = rRxMsg.u1Data;
			        u1ScrachpadIndex = 0;
			        iWriteburstStateNew = MHL_WRITE_BURSET_WDATA;
			    }
			    else
			    {
					//invalid offset
					_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
					iWriteburstStateNew = MHL_WRITE_BURST_EOF;
			    }
			    break;
			case MHL_WRITE_BURSET_WDATA:
				_Cbus_MscStartTimer();
				if(u2RxMsg == CBUS_MSC_CTRL_EOF)
				{
				    if(u1ScrachpadIndex < 2)
				    {
				        //abort bad offset
						_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
						iWriteburstStateNew = MHL_WRITE_BURST_EOF;
						break;
				    }
					_Cbus_MscStopTimer();
					iWriteburstStateNew = MHL_WRITE_BURST_EOF;
					arTxMscMsgs[0] = CBUS_MSC_CTRL_ACK;
					_Cbus_SendMsgEx(arTxMscMsgs,1);
				}
				else
			    if((u2RxMsg & 0x700)== 0x400)
			    {
			        if((u1ScrachpadIndex + u1ScrachpadOffset) > (MAX_SCRACHPAD_REG_OFFSET & 0xff)
			        ||(u1ScrachpadIndex > (FIELD_DEV_SCRATCHPAD_SIZE - 1)))
			        {
			            //abort
						_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
						iWriteburstStateNew = MHL_WRITE_BURST_EOF;
			        }
			        else
			        {
#ifdef SYS_MHL_SUPPORT			        
					   au1ScratchPad[u1ScrachpadIndex] = rRxMsg.u1Data;
#endif					   
					   u1ScrachpadIndex ++;
			        }
			    }
			    else
				{
					//abort control instead of data
					_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
					iWriteburstStateNew = MHL_WRITE_BURST_EOF;
				}
			    break;
			case MHL_WRITE_BURST_EOF:	
#ifdef SYS_MHL_SUPPORT
                if(u1ScrachpadIndex)
                {
				    Mhl_NFYWriteBurstData(0, au1ScratchPad, u1ScrachpadIndex);
				    u1ScrachpadIndex = 0;
			    }
#endif			    
				if(u2RxMsg == CBUS_MSC_CTRL_WRITE_BURST)
				{
					_Cbus_MscStartTimer();
					iWriteburstStateNew = MHL_WRITE_BURST_START;
				}
				else
				{
				    iWriteburstStateNew = MHL_WRITE_BURST_NONE;
			    }
			    break;
			default:
			    break;
		}
		if(iWriteburstStateNew != MHL_WRITE_BURST_NONE)
		{
			break;
		}
		if(rRxMsg.channel == 0)
		{
		    if((DDC_STATE_NONE == iDdcSateNew) &&(DDC_STATE_NONE == iDdcSateOld))
		    {
		       _Cbus_MscStopTimer();
		        _Mhl_RxEnqueue(&rRxMsg);
		    }
		}
		else
		{
		    _Cbus_MscStopTimer();
			if((u2RxMsg == CBUS_MSC_CTRL_GET_STATE)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = 0x400;
				_Cbus_SendMsgEx(arTxMscMsgs,1);  
			}
			else
			if((u2RxMsg == CBUS_MSC_CTRL_GET_VENDER_ID)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = 0x401;
				_Cbus_SendMsgEx(arTxMscMsgs,1);  
			
			}
			else
			if((u2RxMsg == CBUS_MSC_CTRL_GET_DDC_EC)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = (((UINT16)tCurrDDCErrorMsg.ctrl) << 8) + (UINT16)(tCurrDDCErrorMsg.u1Data);
				_Cbus_SendMsgEx(arTxMscMsgs,1);
			}
			else
			if((u2RxMsg == CBUS_MSC_CTRL_GET_SC3_EC)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = 0x400;
				_Cbus_SendMsgEx(arTxMscMsgs,1);
			}
			else
			if((u2RxMsg == CBUS_MSC_CTRL_GET_MSC_EC)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = 0x400 + (((UINT16)tCurrMSCErrorMsg.ctrl) << 8) + (UINT16)(tCurrMSCErrorMsg.u1Data);;
				_Cbus_SendMsgEx(arTxMscMsgs,1);
			}
			else
			if((u2RxMsg == CBUS_MSC_CTRL_GET_SC1_EC)&&(iMhlRxStateNew == MHL_RX_STATE_IDLE))
			{
				arTxMscMsgs[0] = 0x400;
				_Cbus_SendMsgEx(arTxMscMsgs,1);
			}	
			else
			{
				_Mhl_RxEnqueue(&rRxMsg);
			}
		}
   }
   _vCbusHandleRxTask();   
 }
static void _Cbus_Interrupt(UINT16 u2Vector)
{  
#ifdef CBUS_DMSG
    MHL_MSG_T t_Msg;
#endif      
	UINT32 u4IntStat = 0;
	UINT32 u4ClrInt = 0; //= u4IO32Read4B(PDWNC_CBUS_LINK_08);
	UINT8 u1RxMsgCnts = 0;
	UINT16 u2RxMsg = 0;
	UINT8 i = 0;
	UINT8 j = 0;
	UINT16 aRxMsg[16];	
	HAL_TIME_T tRxTime;
    x_memset(&tRxTime, 0, sizeof(HAL_TIME_T));
	ASSERT(u2Vector == PDWNC_INTNO_CBUS);
	UNUSED(u2Vector);
     if(bCbusState == 0)
     {
         return;
     }
     u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);

#if 1
	if (u4IntStat & 0x1)
	{
		//for cts test
		if(IS_IC_5399_ES2())
		{
			vIO32WriteFldAlign(PDWNC_PDMISC,1, FLD_CBUS_100K_ADJUST);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);			
		}
		else
		{
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,5, FLD_ZCBUS_SINK_ON_CTRL);
		}
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);	
#ifndef FORCE_1K_ON 
		u4ClrInt |= 0x1;
#endif
#ifdef CBUS_DMSG

		LOG(0, "discovery pulse detected.....\n");	
#endif	
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,1,FLD_ADP_BITIME_RST);

		vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,0,FLD_ADP_BITIME_RST);

		//disable discovery en
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
		vCbus_EnableInterrupt();
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,1,FLD_LINKRX_EN);			

		// pull rx sense
		// TODO
		iMhlStateNew = MHL_STATE_CONNECTED;
#ifdef SYS_MHL_SUPPORT		
		_bIsMhlDeviceConnect = 1;
#endif		
		vMHLOpenRxTermination(1);
		MhlSetHpd = 1;
		MhlSetPathen = 0;
		SetMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);

		// enable cbus disconnect INT
#ifdef ENABLE_CBUS_LOW_DISCONNECT                        
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,1,FLD_CBUS_DISCONN_CNT_EN);
#endif
		// turn on rx sense 100k
	}
#endif			
	if (u4IntStat & 0x8000)//   1 5
	{
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x8000);
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
#ifdef CBUS_DMSG
		HAL_GetTime(&tRxTime);
		LOG(5, "Interrupt for RX recevied data....\n");		
#endif
	}
	if ((u4IntStat & 0x10))//bit 4
	{
		u4ClrInt |= 0x10;
#ifdef CBUS_DMSG		
		LOG(0,"wake up pulse or discovery pulse timeout int\n");
#endif		
	}
	if ((u4IntStat & 0x8))//bit 3
	{
		u4ClrInt |= 0x8;
#ifdef CBUS_DMSG		
		LOG(0,"ill wake up pulse width int \n");
#endif		
	}
	if ((u4IntStat & 0x4))//bit 2
	{
		u4ClrInt |= 0x4;
#ifdef CBUS_DMSG		
		LOG(0,"ill width from wakeup pulse to discovery pulse \n");
#endif		
	}
	if ((u4IntStat & 0x20000))//bit 17
	{
		u4ClrInt |= 0x20000;
#ifdef CBUS_DMSG		
		LOG(0,"cbus rx bit time timeout int\n");
#endif		
	}
	
	if(u4IntStat & 0x400)
	{
		u4ClrInt |= 0x400;
		_bIsMhlDevice = 0;
#ifdef SYS_MHL_SUPPORT
		_bIsMhlDeviceConnect =0;
#endif
		vMHLOpenRxTermination(0);
		iMhlStateNew = MHL_STATE_INIT;
#if VBUS_CTRL_GPIO
#ifdef SYS_MHL_SUPPORT
		_Cbus_OcpStopTimer();
#endif	
		GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
		//to do
#ifdef CBUS_DMSG
		LOG(0, "CABLE DISCONNECTTED	\n");
#endif
	}
	if (u4IntStat & 0x40)// 	6
	{
		u4ClrInt |= 0x40;
	    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG))
	    {
	        ClrMHLStatus(STATE_MHL_TX_SEND_MSG);
	        SetMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
	    }
#ifdef CBUS_DMSG
		LOG(0, "Interrupt for TX retry timeout....\n");		
#endif
	}
	if (u4IntStat & 0x100)
	{	
		u4ClrInt |= 0x100;		
	    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG))
	    {
	        ClrMHLStatus(STATE_MHL_TX_SEND_MSG);
	        SetMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
	    }
#ifdef CBUS_DMSG
		LOG(0, "Interrupt for TX arbitration lose......\n"); 
#endif
	}
	if (u4IntStat & 0x80)
	{	
		u4ClrInt |= 0x80;		
		if(IsMHLStatus(STATE_MHL_TX_SEND_MSG))
		{
			ClrMHLStatus(STATE_MHL_TX_SEND_MSG);
			ClrMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
			SetMHLStatus(STATE_MHL_TX_SEND_MSG_OK);
			u1RetryNum = 0;
		}
#ifdef CBUS_DMSG
		LOG(5, "Interrupt for TX transmit complete......\n"); 
#endif
	}
	if(IsMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL))
	{
		if(u1TxCurrMsgChnl == 2)//msc
		{		
			ClrMHLStatus(STATE_MHL_TX_WAITING_ACK);
			ClrMHLStatus(STATE_MHL_TX_WAITING_DATA);
			_Cbus_MscStopTimer();
			/*if(u1RetryNum < RETRYMAX)
			{
				for(i=0; i<u1TxMsgSavedSizeMsc; i++)
				{
					vCbusConvertInttoMsg(au2TxMsgSavedMsc[i], &t_Msg);  
					_Mhl_TxEnqueue(&t_Msg); 	 
				}
				GroupId ++;
				u1RetryNum ++;
			}
			else
			{
			    ClrMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
				u1RetryNum = 0;
			}*/
			ClrMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
		}		
		else//DDC
		{
			_Cbus_DDCStopTimer();
			/*if(u1RetryNum < RETRYMAX)
			{
				for(i=0; i<u1TxMsgSavedSizeDdc; i++)
				{
					vCbusConvertInttoMsg(au2TxMsgSavedDdc[i], &t_Msg);	
					_Mhl_TxEnqueue(&t_Msg); 	 
				}
				GroupId ++;
				u1RetryNum ++;
			}
			else
			{
			    ClrMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
				u1RetryNum = 0;
			}*/
			ClrMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL);
		}		
	}
	if (u4IntStat & 0x200)
	{
		u4ClrInt |= 0x200;
		if(iMhlStateOld == MHL_STATE_WAIT_CABLE_CONNECT)
			iMhlStateNew = MHL_STATE_DISCOVERY;
		//to do
#ifdef CBUS_DMSG
		LOG(0, "cable connected\n"); 
#endif
	}

	if ((u4IntStat & 0x4000))
	{
		u4ClrInt |= 0x4000;
#if 1		
		iMhlStateNew = MHL_STATE_INIT;		
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
		// disable cbus disconnect INT			
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0,FLD_CBUS_DISCONN_CNT_EN);
		vCbus_DisableInterrupt();
#endif
		vMHLOpenRxTermination(0);
#ifdef SYS_MHL_SUPPORT
		_bIsMhlDeviceConnect =0;
#endif
		//to do
#ifdef CBUS_DMSG
		LOG(0, "low disconnect detected	\n");
#endif
	}
	// handle CBUS low, Tx device disconneted
	// TODO
	if (u4IntStat & 0x20)//	5
	{
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x20);
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
#ifdef CBUS_DMSG
		LOG(5, "Interrupt for RX rbuf level meet rbuf_lvl_thr....\n");		
#endif
	}
	HAL_GetTime(&tRxTime);
	//vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
	vIO32Write4B(PDWNC_CBUS_LINK_08, 0xffffffff);
	vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
	u1RxMsgCnts = IO32ReadFldAlign(PDWNC_CBUS_STA_00,FLD_RBUF_LVL_LAT);
	// Rx data enqueue
	while (u1RxMsgCnts != 0) 
	{
		u1RxMsgCnts --;
		u2RxMsg = u4IO32Read4B(PDWNC_CBUS_RBUF);
 #ifdef CBUS_DMSG
        vCbusConvertInttoMsg(u2RxMsg,&t_Msg);
        x_memcpy(&(t_Msg.time),&tRxTime,sizeof(HAL_TIME_T));
        t_Msg.status.dbgTxData = 0;//rx data
        _dbg_Enqueue(&t_Msg);
#endif        
        if((u2RxMsg & 0x500) == 0x500) //control data
        {
            j ++;
        }
        if(j > 2)
        {
            i = 0;
        }
		aRxMsg[i] = u2RxMsg;
		i++;
        if(u2RxMsg == CBUS_DDC_CTRL_ABORT) 
        {
			//send abort or do nothing
			if(tDDCStatus.rEdid == 1)
			{
			  iDdcSateNew = DDC_STATE_NONE;
			  LOG(0,"cbus: edid read fail ddc abort received %d\n", EdidReadPoniter);
			  EdidReadPoniter = 0;
			  vCbusSetEdidChg();
			}
            if(u1RxMsgCnts == 0)
            {
                return;
            }
            else
            {
               i--;
            }
        }
	}
	u1RxMsgCnts = i;
	_Cbus_DDCloop(aRxMsg,u1RxMsgCnts,&tRxTime);	
	_Cbus_MSCloop(aRxMsg,u1RxMsgCnts,&tRxTime);

}

void vCbusSendMsg(void)
{
    _Cbus_SendMsg();
}
void vCbusCmdTest(UINT8 cmd_id, UINT8 sub_cmd_id)
{
	switch(cmd_id)
	{
	case 1:
	  // set hpd
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_HPD;
	   _Cbus_SendMsgEx(arTxMscMsgs, 1);
	   LOG(0, "CBUS_MSC_CTRL_SET_HPD \n"); 			 
	break;

	case 2:
	// clr hpd
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_CLR_HPD;
	   _Cbus_SendMsgEx(arTxMscMsgs,1);
	   LOG(0,"CBUS_MSC_CTRL_CLR_HPD \n"); 
	break;

	case 3:
	// set path en
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
	   arTxMscMsgs[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;
	   arTxMscMsgs[2] = 0x40B;	
	   _Cbus_SendMsgEx(arTxMscMsgs,3);
	   LOG(0,"SET PATH EN \n"); 
	break;

	case 4:
		// set DEV_CAP ready
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
	   arTxMscMsgs[1] = CBUS_MSC_REG_READY_BITS;
	   arTxMscMsgs[2] = 0x401;		
	   _Cbus_SendMsgEx(arTxMscMsgs,3);
	   LOG(0,"SET CBUS_MSC_REG_READY_BITS \n"); 
	break;

	case 5:
	// DCAP_CHG
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
	   arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
	   arTxMscMsgs[2] = 0x401;		
	   _Cbus_SendMsgEx(arTxMscMsgs,3);
	  LOG(0, "SET CBUS_MSC_REG_REGCHANGE_INT \n");
		  
	break;

	case 6:
	// set EDID change
	   arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
	   arTxMscMsgs[1] = CBUS_MSC_REG_DDCHANGE_INT;
	   arTxMscMsgs[2] = 0x402;	
	   _Cbus_SendMsgEx(arTxMscMsgs,3);
	    LOG(0, "SET CBUS_MSC_REG_DDCHANGE_INT \n");
	break;

	case 7:
		// read CAP_DEV register
		 LOG(0, "SET CBUS_MSC_CTRL_READ_DEVCAP \n"); 
		MscReadDevCap = 1;
		ReadDevCapOffset = CBUS_MSC_DEVCAP_DEV_STATE;
	break;
	case 9:
		//write burst command
		if(_arMhlSrcDevCap.FeatureFlag & 0x4)
		{
			LOG(0,"send REQ_WRT\n");
			arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
			arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;//40~7F
			arTxMscMsgs[2] = INT_REQ_WRT | 0x400;
			_Cbus_SendMsgEx(arTxMscMsgs,3);
		}
		else
		{
		    LOG(0,"source do not support scratch pad register: 0x%x \n",_arMhlSrcDevCap.FeatureFlag);
		}
		break;
	case 10:
		//write burst command
		break;
	case 11:
	    LOG(0,"RAP pollihg test \n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		arTxMscMsgs[1] = MHL_MSC_MSG_RAP;
		arTxMscMsgs[2] = MHL_RAP_CMD_POLL;
		_Cbus_SendMsgEx(arTxMscMsgs,3);
	case 12:	
		LOG(0,"RAP content on test \n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		arTxMscMsgs[1] = MHL_MSC_MSG_RAP;
		arTxMscMsgs[2] = MHL_RAP_CMD_CONTENTON;
		_Cbus_SendMsgEx(arTxMscMsgs,3);
	    break;
	case 13:
		LOG(0,"RAP content off test \n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
		arTxMscMsgs[1] = MHL_MSC_MSG_RAP;
		arTxMscMsgs[2] = MHL_RAP_CMD_CONTENTOFF;		
		_Cbus_SendMsgEx(arTxMscMsgs,3);
	    break;
    case 15:
	  LOG(0,"1k test 0 \n");
	  u11Ktest = 0;
	  break;
#ifdef CC_MHL_3D_SUPPORT
	case 16:
		LOG(0,"3D edid config \n");
		vMHLParsingEDIDForMHL3D(arEdid);
	    //Mhl_3D_EDID_Configuration_VIC_DTD(&t3D_VIC_Struct,&t3D_DTD_Struct);
		break;
	case 17:
		LOG(0,"3D write burst \n");
		vCbusWriteBurst3D();
		break;
#endif
	case 18:
	    LOG(0,"get vendor id\n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_GET_VENDER_ID;
		_Cbus_SendMsgEx(arTxMscMsgs,1);
	    break;
	case 19:
		LOG(0,"get msc error code\n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_GET_MSC_EC;
		_Cbus_SendMsgEx(arTxMscMsgs,1);
		break;
	case 20:
		LOG(0,"PATH DISABLE\n");
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;//40~7F
		arTxMscMsgs[2] = 0x400;
		_Cbus_SendMsgEx(arTxMscMsgs,3);
		break;
    case 21:
#ifdef CC_MHL_3D_SUPPORT	
         LOG(0,"3D REQ TEST\n");
		 arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
		 arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
		 arTxMscMsgs[2] = 0x400 | INT_REQ_WRT;
		 _Cbus_SendMsgEx(arTxMscMsgs,3);
#endif		
		break;
#ifdef SYS_MHL_SUPPORT		
    case 22:
        LOG(0,"rcp receiver test, 0x%x \n",sub_cmd_id);
        Mhl_NFYRcp(sub_cmd_id);
        break;
#endif        
    default:
    break;
    }
}

void vCbusRcpTest(UINT8 ui1KeyCode)
{
	LOG(0,"RCP test \n");
	arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
	arTxMscMsgs[1] = MHL_MSC_MSG_RCP;
	arTxMscMsgs[2] = (UINT16)(ui1KeyCode | 0x400);		
	_Cbus_SendMsgEx(arTxMscMsgs,3);
	return;
}

void vCbusUcpTest(UINT8 ui1data)
{
	LOG(0,"UCP test \n");
	arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
	arTxMscMsgs[1] = MHL_MSC_MSG_UCP;
	arTxMscMsgs[2] = (UINT16)(ui1data | 0x400);		
	_Cbus_SendMsgEx(arTxMscMsgs,3);
	return;
}
void vCbusUcpKTest(UINT8 ui1data)
{
	LOG(0,"UCP test \n");
	arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
	arTxMscMsgs[1] = MHL_MSC_MSG_UCPK;
	arTxMscMsgs[2] = (UINT16)(ui1data | 0x400);		
	_Cbus_SendMsgEx(arTxMscMsgs,3);
	return;
}

void vCbusSetHpd(void)
{
    //MHL_MSG_T t_Msg;
	if(IsMHLStatus(STATE_MHL_HAND_SHAKING_FAIL))
    {
		arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_HPD;
		
		_Cbus_SendMsgEx(arTxMscMsgs,1);
		/*vCbusConvertInttoMsg(arTxMscMsgs[0], &t_Msg);
		_Mhl_TxEnqueue(&t_Msg);
		GroupId ++;*/
#ifdef CBUS_DMSG
		LOG(3, "CBUS_MSC_CTRL_SET_HPD \n"); 
#endif	
       ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
	}
}
void vCbusClrHpd(void)
{
    MHL_MSG_T t_Msg;
	arTxMscMsgs[0] = CBUS_MSC_CTRL_CLR_HPD;
	vCbusConvertInttoMsg(arTxMscMsgs[0], &t_Msg);
	_Mhl_TxEnqueue(&t_Msg);
	GroupId ++;
#ifdef CBUS_DMSG
	LOG(3, "CBUS_MSC_CTRL_CLR_HPD \n"); 
#endif
}

//enquene, then send is too slow, sometimes, tx start read dev capibility, we didnot get data from buffer in time, this will lead to read dev cap fail
void vCbusSetPathEn(void)
{
	//set path en: 
	//560 430 401
	//560 420 401
	//560 431 408
	//UINT8 i = 0;
	//MHL_MSG_T t_Msg;
	
    if((MhlSetPathen == 1) && (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (u1StartReadDev != 1))
    {  
    
	if(IsMHLStatus(STATE_MHL_HAND_SHAKING_FAIL))
	{
		MhlSetPathen = 2;
	    ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_READY_BITS;
		arTxMscMsgs[2] = 0x401;  
		_Cbus_SendMsgEx(arTxMscMsgs,3);
		/*for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		
			_Mhl_TxEnqueue(&t_Msg);			
		}*/
		u1HandshakingGroupid = GroupId;
       // GroupId ++;
#ifdef CBUS_DMSG
		LOG(3,"SET PATH EN 1 \n"); 
#endif	
}
    }
    else
	if((MhlSetPathen == 2)&& (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (u1StartReadDev != 1))
    {  
    
	if(IsMHLStatus(STATE_MHL_HAND_SHAKING_FAIL))
	{
		MhlSetPathen = 3;
	    ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
		arTxMscMsgs[2] = 0x401;  
		
		_Cbus_SendMsgEx(arTxMscMsgs,3);
		/*for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		
			_Mhl_TxEnqueue(&t_Msg);
		}*/
		u1HandshakingGroupid = GroupId;
		//GroupId ++;
#ifdef CBUS_DMSG	  
		LOG(3,"SET PATH EN 2\n"); 
#endif	
}
    }
    else
	if((MhlSetPathen == 3)&& (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && ((u1StartReadDev != 1)))
    {
    
	if(IsMHLStatus(STATE_MHL_HAND_SHAKING_FAIL))
	{
		MhlSetPathen = 4;
		//x_thread_delay(1);
	    ClrMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;
		arTxMscMsgs[2] = 0x408;  
		
		_Cbus_SendMsgEx(arTxMscMsgs,3);
		u1StartReadDev = 3;
		/*for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		
			_Mhl_TxEnqueue(&t_Msg);
		}*/	
		u1HandshakingGroupid = GroupId;
		//GroupId ++;
    }
    }
	else
	if((!IsMHLStatus(STATE_MHL_TX_WAITING_DATA))&& (!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) && (u1StartReadDev == 2))
	{
		x_thread_delay(1);
		arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
		arTxMscMsgs[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;
		arTxMscMsgs[2] = 0x408;  
			
		_Cbus_SendMsgEx(arTxMscMsgs,3);
		/*for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		
			_Mhl_TxEnqueue(&t_Msg);
		}*/	
		GroupId ++;
		u1StartReadDev = 3;
	}
}

void _Cbus_ReadDevCap(void)
{
    if(MscReadDevCap)
    {
        if(ReadDevCapOffset != NULL_DATA)
        {
			arTxMscMsgs[0] = CBUS_MSC_CTRL_READ_DEVCAP; 				
			arTxMscMsgs[1] = ReadDevCapOffset;		
			MscReadDevCap = 0;
			_Cbus_SendMsgEx(arTxMscMsgs,2);
			/*for(i=0; i<2; i++)
			{
				vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);
				_Mhl_TxEnqueue(&t_Msg);
			}*/
		}
    }
}

UINT8 u1GetCdsenseStatus(void)
{
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_01, FLD_CBUS_CDSENSE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void _vCbusHandleTxTask(void)
{
    _Cbus_ReadDevCap();
    if((IsMHLStatus(STATE_MHL_TX_WAITING_ACK)) || (IsMHLStatus(STATE_MHL_TX_WAITING_DATA)))
    {
        return;
    }
	if(!IS_MHL_TX_Q_EMPTY())
	{
		_Cbus_SendMsg();
	}
}

void _vCbusStateReset(void)
{
	iMhlStateOld = MHL_STATE_NONE;
	iMhlStateNew = MHL_STATE_NONE;
	
	iDdcSateOld = DDC_STATE_NONE;
	iDdcSateNew = DDC_STATE_NONE;
	
	iWriteburstStateOld = MHL_WRITE_BURST_NONE;
	iWriteburstStateNew = MHL_WRITE_BURST_NONE;
	
	iMhlRxStateOld = MHL_RX_STATE_IDLE;
	iMhlRxStateNew = MHL_RX_STATE_IDLE;
	_MHLStatus = 0;
	x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
#ifdef HDCP_SUPPORT	
	u1AnReady = 0;
	u1Bcaps = 0x80;
#endif	
    GroupId = 0;
    u1CurGroupId = 0;
    u1CableConCnt = 0;
#ifdef CC_MHL_3D_SUPPORT
	VicWriteburstIdx =0 ;
	DtdWriteburstIdx =0 ;
	u1write_burst = 0;
#endif	
	u1_rapk = 0;
	u1ucpk[0] = 0;
	u1StartReadDev = 0;

	EdidReadPoniter = 0;
	iDdcOffset = 0;
	u1RetryNum = 0;
	u1HandshakingGroupid = 0;
#ifdef CBUS_DMSG
    dbg_rxQ_read_idx = 0;
    dbg_rxQ_write_idx=0;
#endif    
}

void _Cbus_BootupTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{
	Printf("[cbus] boot up timer out, %d\n", u1BootUp);

    if(u1BootUp == 2)
    {
        u1BootUp = 3;
       DISABLE_PINMUX_TO_CBUS();
	   vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high Z state
       vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
#if VBUS_CTRL_GPIO
  	   GPIO_SetOut(VBUS_CTRL_GPIO, 1);
#endif
	    vMHLOpenRxTermination(0);
		x_timer_start(_hMhlBootupTimer ,TIMER_BOOT_UP, X_TIMER_FLAG_ONCE,_Cbus_BootupTimerHandle,NULL);
    }
    else
    {
        u1BootUp = 0;
	    vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
		ENABLE_PINMUX_TO_CBUS();
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_WAKEUP_EN); 	
#if VBUS_CTRL_GPIO
		GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
		vMHLOpenRxTermination(0);
#ifdef SYS_MHL_SUPPORT		
        _Cbus_OcpStartTimer();
#endif
		iMhlStateNew = MHL_STATE_DISCOVERY;
    }
}

void _vPatchForHPDCTSFail(void)
{
    if(!u1GetCdsenseStatus())
    {
        if(bHDMIPort5VStatus(HDMI_SWITCH_1) && (eResist_state!= Resister_PullLow)&&(!u1GetCdsenseStatus()))
        {   
                eResist_state =Resister_PullLow;
            	vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_SINK_ON_CTRL);			
                vIO32WriteFldAlign(PDWNC_PDMISC,1, FLD_CBUS_100K_ADJUST);
        }
        else
        if((bHDMIPort5VStatus(HDMI_SWITCH_1)==0) && (eResist_state != Resister_NotPullLow)&&(!u1GetCdsenseStatus()))
        {
                eResist_state = Resister_NotPullLow;
                vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);			
                vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
        }
    }
}
void vCbusMainLoop(void)
{
	// Sink state machine +++++++++++++++++++++++++++++++++++
    UINT32 u4IntStat = 0;
    UINT32 u4ClrInt = 0;
    UINT32 u4WakeUpReason;
	if (iMhlStateOld != iMhlStateNew)
	{
#ifdef CBUS_DMSG
#ifndef NDEBUG
		LOG(2,"iMhlState %s --> %s,",_aszMhlState[iMhlStateOld],_aszMhlState[iMhlStateNew]);
#endif		
#endif	
	        iMhlStateOld = iMhlStateNew;
	}
	_vPatchForHPDCTSFail();
	switch (iMhlStateOld)
	{
		case MHL_STATE_NONE:
				iMhlStateNew = MHL_STATE_INIT;
		break;
		case MHL_STATE_INIT:
#ifdef SYS_MHL_SUPPORT		
		    Mhl_NTF_CBUS_Disconnected();
			_Cbus_OcpStopTimer();
#endif		
			_Cbus_MscStopTimer();
			_Cbus_DDCStopTimer();
		    _vCbusStateReset();	
			//disable cbus RX			
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
			// Enable wakeup and discovery
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);
             
			// rx INT through hold and Tx bit
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,0,FLD_DUPLEX);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_FAKE_SOURCE);

			// disable cbus disconnect INT			
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0,FLD_CBUS_DISCONN_CNT_EN);

			// Enable CBUS Rx
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);			
            vCbus_DisableInterrupt();     
            
			vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf0000000,0xf0000000);
			vUtDelay2us(1);
			vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0,0xf0000000);
			
		    _Mhl_InitQueue();
            vCbusClrStatusAll();
            if(!u1GetCdsenseStatus())
			{
                
                vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
		        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);			
                vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
			}
		    if(u1BootUp == 0)
		    {
		       iMhlStateNew = MHL_STATE_WAIT_CABLE_CONNECT;
		    }
		    else
		    {
			   iMhlStateNew = MHL_STATE_BOOTUP;    
			}
			
		break;
        case MHL_STATE_BOOTUP:
            if(u1BootUp == 1)
            {
			   u4WakeUpReason = PDWNC_ReadWakeupReason();
			   LOG(5, "u4WakeUpReason = %d.\n", u4WakeUpReason);
			   if((u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER) || 
                  (u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG))
			  {
	               u1BootUp = 2;
				   ENABLE_PINMUX_TO_CBUS();
				   vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
#if VBUS_CTRL_GPIO
				   GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
				   vMHLOpenRxTermination(0);
               }
			   else
			   {
				   u1BootUp = 3;
#if VBUS_CTRL_GPIO
		           GPIO_SetOut(VBUS_CTRL_GPIO, 1);
#endif
				   vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
				   vMHLOpenRxTermination(0);				   
				   DISABLE_PINMUX_TO_CBUS();
			   }

		       x_timer_start(_hMhlBootupTimer,TIMER_BOOT_UP, X_TIMER_FLAG_ONCE,_Cbus_BootupTimerHandle,NULL);
            }
        break;
		case MHL_STATE_WAIT_CABLE_CONNECT:

			u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);	   
			if (u4IntStat & 0x400)
			{
				u4ClrInt |= 0x400;
#ifdef CBUS_DMSG
				LOG(5,"Clear Cable out NOISE !!!\n"); 
#endif
			}
			_bIsMhlDevice =0;
			
			if (!(u1GetCdsenseStatus()))//cable detect int
			{
#if VBUS_CTRL_GPIO
#ifdef SYS_MHL_SUPPORT
                _Cbus_OcpStopTimer();
#endif				
				GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
#endif                      
			}
			if ((u4IntStat & 0x200) || (u1GetCdsenseStatus()))//cable detect int
			{
				u1CableConCnt ++;//polling 10 times for cable connect,
				u4ClrInt |= 0x200;		
				if(u1CableConCnt == 10)
				{
					iMhlStateNew = MHL_STATE_DISCOVERY;
					ENABLE_PINMUX_TO_CBUS();
					_bIsMhlDevice = 1;			

                    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);                    
		            vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
		            vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
		            vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);	  
		            
                    vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_WAKEUP_EN);	

#if VBUS_CTRL_GPIO
					GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
#ifdef SYS_MHL_SUPPORT
                    _Cbus_OcpStartTimer();
#endif
				}
			}

			if (u4IntStat & 0x1)
			{
#ifndef FORCE_1K_ON
				u4ClrInt |= 0x1;
#endif			
#ifdef CBUS_DMSG
				LOG(0,"discovery pulse detected@MHL_STATE_WAIT_CABLE_CONNECT.....\n");  
#endif			
             	iMhlStateNew = MHL_STATE_DISCOVERY;
			}
			vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
			vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	   					
		break;

		case MHL_STATE_DISCOVERY:
		   //working at cbus state 			   
		   vMHLOpenRxTermination(0);
			u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);

			if (u4IntStat & 0x200)
			{
			u4ClrInt |= 0x200;
#ifdef CBUS_DMSG
			LOG(5,"Clear Cable in NOISE !!!\n"); 
#endif	

			}

			if ((u4IntStat & 0x400) || (!u1GetCdsenseStatus()))
			{
				u4ClrInt |= 0x400;
				iMhlStateNew = MHL_STATE_INIT; 
				_bIsMhlDevice = 0;
				#ifdef SYS_MHL_SUPPORT
				_bIsMhlDeviceConnect =0;
				#endif
#if VBUS_CTRL_GPIO
#ifdef SYS_MHL_SUPPORT
                _Cbus_OcpStopTimer();
#endif	
				GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
#ifdef CBUS_DMSG

				LOG(0,"CABLE DISCONNECT @  MHL_STATE_DISCOVERY \n"); 
#endif	
            }

			if (u4IntStat & 0x2)
			{	     
				u4ClrInt |= 0x2;
#ifdef CBUS_DMSG
				LOG(0,"WAKEUP pulse detected.....\n");	
#endif	
                //disable wake up en
				//vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);			
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_DISCOVERY_EN);
				vIO32Write4B(PDWNC_CBUS_LINK_0D,1);//enable discovery pulse interrupt
			    vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
			    vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);
			    x_thread_delay(600);//for cts test 2.0
			}		
			vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
			vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);
		break;

		case MHL_STATE_CONNECTED:
		    if((MHL_RX_STATE_MSCMSG != iMhlRxStateNew) && (MHL_RX_STATE_IDLE != iMhlRxStateNew))
		    {
		        return;
		    }
			vCbusReplyData();
			if(!u1GetCdsenseStatus())
			{
				iMhlStateNew = MHL_STATE_INIT;
#if VBUS_CTRL_GPIO
#ifdef SYS_MHL_SUPPORT
                _Cbus_OcpStopTimer();
#endif	
				GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
				_bIsMhlDevice = 0;
#ifdef SYS_MHL_SUPPORT
				_bIsMhlDeviceConnect =0;
#endif 
				vMHLOpenRxTermination(0);
				break;
			}
#ifdef SYS_MHL_SUPPORT			
			Mhl_NTF_CBUS_Connected();
#endif			
#ifdef SYS_MHL_SUPPORT   
            if(tCbusTimerState.u1AbortTimer != CBUS_TIMER_GOING)
            {
                _vCbusHandleTxTask();
            }
#endif            
			if(MhlSetHpd &&(!IsMHLStatus(STATE_MHL_TX_WAITING_ACK)))
			{	
				vCbusSetHpd();
			}
			else
			if(MhlSetPathen)
			{
				 //set path en: 
				 //560 430 401
				 //560 420 401
				 //560 431 408
				 vCbusSetPathEn();
			}
        break;
        default:
        break;
	}
}
static void _Cbus_Thread( void * pvArgs )
{
    UNUSED(pvArgs);
    while(1)
    {
		x_thread_delay(1);
		if(bCbusState == 1)
		{
		    vCbusMainLoop();		
		}
    }
}

static void _Hdcp_Thread( void * pvArgs )
{
    UNUSED(pvArgs);
    while(1)
    {
		x_thread_delay(1);
		if(bSetAn)
		{    
		    bSetAn = 0;
			vMHLSetHdcpAn(au1An);
		}
		if(bSetAksv)
		{
		    bSetAksv = 0;
		    vMHLSetHdcpAksv(au1Aksv);
		}
     }
}


static void _Cbus_TimerThread( void * pvArgs )
{
	UNUSED(pvArgs);
	while(1)
	{
		x_thread_delay(1);
		if(bCbusState == 1)
		{
			if(tCbusTimerState.u1DdcTimer == CBUS_TIMER_WAIT_START)	
			{
			    tCbusTimerState.u1DdcTimer = CBUS_TIMER_GOING;
				x_timer_stop(_hMhlDdcTimer);
				x_timer_start(_hMhlDdcTimer,TIMER_PKT_SENDER,X_TIMER_FLAG_ONCE,_Cbus_DDCTimerHandle,NULL);
			}
			if(tCbusTimerState.u1MscTimer == CBUS_TIMER_WAIT_START)	
			{
			    tCbusTimerState.u1MscTimer = CBUS_TIMER_GOING;
				 x_timer_stop(_hMhlMscTimer);
				 x_timer_start(_hMhlMscTimer,TIMER_PKT_SENDER,X_TIMER_FLAG_ONCE,_Cbus_MscTimerHandle,NULL);
			}
			if(tCbusTimerState.u1AbortTimer == CBUS_TIMER_WAIT_START)	
			{
			    tCbusTimerState.u1AbortTimer = CBUS_TIMER_GOING;
				 x_timer_stop(_hMhlAbortTimer);
				 x_timer_start(_hMhlAbortTimer,TIMER_WRITE_ABORT,X_TIMER_FLAG_ONCE,_Cbus_AbortTimerHandle,NULL);
			}
			if(tCbusTimerState.u1DdcTimer == CBUS_TIMER_WAIT_STOP)	
			{
			    x_timer_stop(_hMhlDdcTimer);
			    tCbusTimerState.u1DdcTimer = CBUS_TIMER_NONE;
			}
			if(tCbusTimerState.u1MscTimer == CBUS_TIMER_WAIT_STOP)	
			{
			    x_timer_stop(_hMhlMscTimer);
			    tCbusTimerState.u1MscTimer = CBUS_TIMER_NONE;
			}
			if(tCbusTimerState.u1AbortTimer == CBUS_TIMER_WAIT_STOP)	
			{
			    x_timer_stop(_hMhlAbortTimer);
			    tCbusTimerState.u1AbortTimer = CBUS_TIMER_NONE;
			}
		}
	}
}
#ifdef CBUS_DMSG
static void _dbg_Enqueue(MHL_MSG_T *prMsg)
{  
    if (((dbg_rxQ_write_idx+1)%1024) == dbg_rxQ_read_idx)
    {
        //return MHL_RX_ERR_QUEUE_FULL;
#ifdef CBUS_DMSG
        LOG(0, "debug rx queue full\n");
#endif
        return;
    }
    (void)x_memcpy(&(pDbgRxMsg[dbg_rxQ_write_idx]), prMsg, sizeof(MHL_MSG_T));
    // rx write pointer add one
    dbg_rxQ_write_idx = (dbg_rxQ_write_idx + 1) % 1024;
}

static UINT8 _dbg_Dequeue(MHL_MSG_T *prMsg)
{
// check if queue is empty
    if (dbg_rxQ_read_idx == dbg_rxQ_write_idx)
    {
        //return MHL_RX_QUEUE_EMPTY;
#ifdef CBUS_DMSG
        LOG(0,"debug rx queue empty \n");
#endif
        return 1;
    }
    
    // copy to buffer first
    (void)x_memcpy(prMsg, &(pDbgRxMsg[dbg_rxQ_read_idx]), sizeof(MHL_MSG_T));

    dbg_rxQ_read_idx = (dbg_rxQ_read_idx + 1) % 1024;
    return 0;
}

static void _Cbus_Debug_Thread( void * pvArgs )
{
    MHL_MSG_T rMsg;
    UNUSED(pvArgs);
    while(1)
    {
		x_thread_delay(20);
		if(bCbusState == 1)
		{
		    while (dbg_rxQ_read_idx != dbg_rxQ_write_idx)
            {
                _dbg_Dequeue(&rMsg);
				if(u1DebugLvl == 2)
				{	
				    if(rMsg.status.dbgTxData == 1)
				    {
						LOG(0," >> tx [%d. %d]	data: ",
						rMsg.time.u4Seconds, rMsg.time.u4Micros);				        
				    }
                    else if(rMsg.status.dbgTxData == 0)
                    {
						LOG(0," << rx [%d. %d]	data: ",
						rMsg.time.u4Seconds, rMsg.time.u4Micros);				        
                    }
					LOG(0, "0x%x ", ((((rMsg.channel<<1)|rMsg.ctrl)<<8)| rMsg.u1Data));
					LOG(0,"\n");
        }
				else
				if(u1DebugLvl == 1)
				{	
				    if(rMsg.channel != 0)
				    {
				    if(rMsg.status.dbgTxData == 1)
				    {
						LOG(0," >> tx [%d. %d]	data: ",
						rMsg.time.u4Seconds, rMsg.time.u4Micros);				        
				    }
                    else if(rMsg.status.dbgTxData == 0)
                    {
						LOG(0," << rx [%d. %d]	data: ",
						rMsg.time.u4Seconds, rMsg.time.u4Micros);				        
                    }
					LOG(0, "0x%x ", ((((rMsg.channel<<1)|rMsg.ctrl)<<8)| rMsg.u1Data));
					LOG(0,"\n");
					}
				}
		}
		}
}
}
void vDebugInit(void)
{
    pDbgRxMsg = x_mem_alloc(1024*sizeof(MHL_MSG_T));
    VERIFY(x_thread_create(&phCbusDebugThreadHdl, 
		"CbusDebugThread", 2048,  60, _Cbus_Debug_Thread, 0, NULL) == OSR_OK);
    return;
}
#endif

void vCbusInit(void)
{
    x_os_isr_fct pfnOldIsr;   
    if(bInitFlg == 0)
    {
        bInitFlg = 1;
	    vCbus_ValInit();
	    vCbus_HwInit(); 
#ifdef CBUS_DMSG
		vDebugInit();
#endif
	    VERIFY(x_thread_create(&phCbusThreadHdl, 
	        "CbusThread", 2048,  60, _Cbus_Thread, 0, NULL) == OSR_OK);
    VERIFY(x_thread_create(&phCbusHdcpThreadHdl, 
        "HdcpThread", 2048,  59, _Hdcp_Thread, 0, NULL) == OSR_OK);
	    VERIFY(PDWNC_RegIsr(PDWNC_INTNO_CBUS, _Cbus_Interrupt, &pfnOldIsr) == PDWNC_OK);
        VERIFY(x_thread_create(&phCbusTimerThreadHdl, 
        "CbusTimerThread", 2048, 59, _Cbus_TimerThread, 0, NULL) == OSR_OK);
    }
}

void vCbusSetState(UINT8 ui1_state)
{
    iMhlStateNew = ui1_state;
    return;
}

UINT8 vCbusGetState(void)
{
    return iMhlStateNew;
}

void vCbusSetHpdFlag(UINT8 u1SetHpd)
{
    MhlSetHpd = u1SetHpd;
    MhlSetPathen = 1-MhlSetHpd;
}
void vCbusOpenDebuglog(UINT8 u1debug)
{
    u1DebugLvl = u1debug;
}

void vCbusStatusQuery(void)
{
    LOG(0,"cbus current status \n");
#ifndef NDEBUG    
	LOG(0,"iMhlState pre status %s -->current status %s,\n",_aszMhlState[iMhlStateOld],_aszMhlState[iMhlStateNew]);	
	LOG(0,"iMhlRxState pre status %s -->current status %s, \n",_aszMhlRxState[iMhlRxStateOld],_aszMhlRxState[iMhlRxStateOld]);
	LOG(0,"iMhlWriterBurstState pre status  %s -->current status  %s, \n",_aszWriteBurstState[iWriteburstStateOld],_aszWriteBurstState[iWriteburstStateNew]);
	LOG(0,"iDdcState  pre status %s --> current status %s, \n",_aszDdcState[iDdcSateOld],_aszDdcState[iDdcSateNew]);
#endif
    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG))
    {
	    LOG(0,"mhl tx status: STATE_MHL_TX_SEND_MSG \n");
    }
    if(IsMHLStatus(STATE_MHL_TX_WAITING_ACK))
    {
	    LOG(0,"mhl tx status: STATE_MHL_TX_WAITING_ACK \n");
    }
    if(IsMHLStatus(STATE_MHL_TX_WAITING_DATA))
    {
		LOG(0,"mhl tx status: STATE_MHL_TX_WAITING_DATA \n");
    }
    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG_FAIL))
    {
		LOG(0,"mhl tx status: STATE_MHL_TX_SEND_MSG_FAIL \n");
    }
    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG_OK))
    {
		LOG(0,"mhl tx status: STATE_MHL_TX_SEND_MSG_OK \n");
    }
    LOG(0,"current Edid offset: %d \n", EdidReadPoniter);    
    LOG(0,"current DDC offset: %d \n", iDdcOffset);
}

/*********************************************
//get cbus impedance
1:1K
2:100k
3:Hi-Z
4:hpd
*********************************************/
void vGetCbusImpedance(void)
{
	
	//if(Get_CBUS_PIN_FUNCTION() != 1)
	//{
	//	LOG(0,"CBUS is GPIO function\n");
	//}
	//else 
	//{
		if((IO32ReadFldAlign(PDWNC_PADCFG8,FLD_HDMI_0_HPD_CBUS_PUPD) == 0) && (IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_DISCOVER_EN) == 0) && (IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_SINK_ON_CTRL) == 0))
		{
			LOG(0,"CBUS Hi Impedance\n");
		}
		else if((IO32ReadFldAlign(PDWNC_PADCFG8,FLD_HDMI_0_HPD_CBUS_PUPD) == 1) && (IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_DISCOVER_EN) == 1) && (IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_SINK_ON_CTRL) == 0))
		{
			LOG(0,"CBUS 1K Impedance\n");
		}
		else if((IO32ReadFldAlign(PDWNC_PADCFG8,FLD_HDMI_0_HPD_CBUS_PUPD) == 1) && (IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_DISCOVER_EN) == 0) && ((IO32ReadFldAlign(PDWNC_CBUS_LINK_07,FLD_ZCBUS_SINK_ON_CTRL) & 0x1) == 0x1))
		{
			LOG(0,"CBUS 100K Impedance\n");
		}
	//}
}


void vGetCbusSatus(void)
{
	LOG(0,"cbus current status \n");
	if(u1GetCdsenseStatus()) //cdsense
	{
		LOG(0,"CD_SENCE High\n");
	}
	else
	{
		LOG(0,"CD_SENCE Low\n");
	}

	//Cbus impedance
	vGetCbusImpedance();

	//Cbus connect status
#ifdef SYS_MHL_SUPPORT
	if(_bIsMhlDeviceConnect ==1)

	{
		LOG(0,"Cbus Active\n");
	}
	else
	{
		LOG(0,"Cbus Inactive\n");
	}
#endif	
}
#ifdef SYS_MHL_SUPPORT
void vCbusSetVbus(UINT8 u1VbusState)
{
    //to do
    //vbus is auto control by hw
#ifdef CBUS_DMSG
    LOG(0,"mhl set vbus on/off %d \n", u1VbusState);
#endif    
}
#define MHL_OCP_GPIO 246

BOOL u1CbusGetOcpStatus(UINT8 port)
{
    UNUSED(port);
	if(GPIO_Input(MHL_OCP_GPIO))//3.3v normal, 0v ocp
	{
	   return FALSE;
	}
	return TRUE;
}
void vCbusWriteBurstRequest(void)
{
    UINT8 i;
	MHL_MSG_T t_Msg;
	arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_STATE;
	arTxMscMsgs[1] = CBUS_MSC_REG_REGCHANGE_INT;
	arTxMscMsgs[2] = INT_REQ_WRT | 0x400;	

	//_Cbus_SendMsgEx(arTxMscMsgs,3);
#if 1
	 for(i=0; i< 3; i++)
	 {
		 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);
		 _Mhl_TxEnqueue(&t_Msg);
	 }
	 GroupId ++;
#endif
}
void vCbusSendWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size)
{
    //request osd name
	UINT8 i;
	MHL_MSG_T t_Msg;
	UNUSED(key);
	UNUSED(pdata);
	UNUSED(size);
	arTxMscMsgs[0] = CBUS_MSC_CTRL_WRITE_BURST;
	arTxMscMsgs[1] = 0x440;//40~7F
	arTxMscMsgs[2] = _arMhlSrcDevCap.AdopoterIdHigh | 0x400;
	arTxMscMsgs[3] = _arMhlSrcDevCap.AdopoterIdLow | 0x400;
	//_arMhlSrcDevCap.ScratchpadSize
	for(i = 0; i < 17; i++)
	{
		arTxMscMsgs[4+i] = i | 0x400;
	}
	arTxMscMsgs[18] = CBUS_MSC_CTRL_EOF;
	
	 for(i=0; i< 19; i++)
	 {
		 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);
		 _Mhl_TxEnqueue(&t_Msg);
	 }
	 GroupId ++;
}
void vCbusSendRcpKeys(UINT8 Key, UINT8 event)
{
	UINT8 i;
	MHL_MSG_T t_Msg;
	arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
	arTxMscMsgs[1] = MHL_MSC_MSG_RCP ;
	arTxMscMsgs[2] = Key | 0x400;
	 for(i=0; i< 3; i++)
	 {
		 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);
		 _Mhl_TxEnqueue(&t_Msg);
	 }
	 GroupId ++;
}
void vCbusSendRapKeys(UINT8 Key)
{
	UINT8 i;
	MHL_MSG_T t_Msg;
	arTxMscMsgs[0] = CBUS_MSC_CTRL_MSC_MSG;
	arTxMscMsgs[1] = MHL_MSC_MSG_RAP ;
	arTxMscMsgs[2] = Key | 0x400;
	 for(i=0; i< 3; i++)
	 {
		 vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);
		 _Mhl_TxEnqueue(&t_Msg);
	 }
	 GroupId ++;
}
void vCbusNtfMscCmd(UINT8 port, UINT8 MscCmd, UINT8 OpCode, UINT8 Data)
{
}

#endif

void vCbusStop(void)
{
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf,0xf0000000);
	vCbus_DisableInterrupt();
	_bIsMhlDevice = 1;
	bCbusState = 0;
	//disable rx
}
void vCbusResume(void)
{
    bCbusState = 1;
}

void vCbusSetVersion(UINT8 u1Version )
{
    _arMhlDevCap.MhlVersion = u1Version;
}

UINT8 u1CbusGetVersion(void)
{
    return _arMhlDevCap.MhlVersion;
}


VOID vCbusGetDevCap(VOID)
{
	Printf("device capability registers:\n");
	Printf("device state            = 0x%2x\n",_arMhlDevCap.DevState);
	Printf("device mhl version      = 0x%2x\n",_arMhlDevCap.MhlVersion);
	Printf("device category         = 0x%2x\n",_arMhlDevCap.DevCat);
	Printf("device Adopoter Id High = 0x%2x\n",_arMhlDevCap.AdopoterIdHigh);
	Printf("device Adopoter Id Low  = 0x%2x\n",_arMhlDevCap.AdopoterIdLow);
	Printf("device Vidio LinkMode   = 0x%2x\n",_arMhlDevCap.VidLinkMode);
	Printf("device Audio LinkMode   = 0x%2x\n",_arMhlDevCap.AudLinkMode);
	Printf("device VideoType        = 0x%2x\n",_arMhlDevCap.VideoType);
	Printf("device LogDevMap        = 0x%2x\n",_arMhlDevCap.LogDevMap);
	Printf("device BandWidth        = 0x%2x\n",_arMhlDevCap.BandWidth);
	Printf("device Feature Flag     = 0x%2x\n",_arMhlDevCap.FeatureFlag);
	Printf("device Devic Id High    = 0x%2x\n",_arMhlDevCap.DevicIdHigh);
	Printf("device Devic Id Low     = 0x%2x\n",_arMhlDevCap.DevicIdHigLow);
	Printf("device Scratchpad Size  = 0x%2x\n",_arMhlDevCap.ScratchpadSize);
	Printf("device IntState Size    = 0x%2x\n",_arMhlDevCap.IntStateSize);
}
#ifdef CC_SUPPORT_STR
void MHL_pm_suspend(void)
{
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf,0xf0000000);
	vCbus_DisableInterrupt();
	bCbusState = 0;
}

void MHL_pm_resume(void)
{
	vCbus_HwInit(); 
	vCbus_ValInit();
}
#endif

void vCbusSetEdidChg(void)
{
    MHL_MSG_T t_Msg;
    UINT8 i=0;
    if(u1EdidRetry < 5)
    {
	    u1EdidRetry ++;
		arTxMscMsgs[0] = CBUS_MSC_CTRL_SET_INT;
		arTxMscMsgs[1] = CBUS_MSC_REG_DDCHANGE_INT;
		arTxMscMsgs[2] = 0x402;  
	    
		for(i=0;i<3;i++)
		{
			vCbusConvertInttoMsg(arTxMscMsgs[i], &t_Msg);		
			_Mhl_TxEnqueue(&t_Msg); 		
		}	
		GroupId ++;
		return;
	}
	else
	{
	    LOG(0,"edid retry fail, retry %d", u1EdidRetry);
	    EdidReadPoniter = 256;
	    u1EdidRetry = 0;
	    tDDCStatus.rEdid = 0;
	}
}
#if 0
static UINT32 bIsRxBusy(void)
{
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_01, FLD_LINKRX_FSM))
    {
        if(u1DebugLvl == 1)
        {
            LOG(0,"rx busy 1 \n");
        }
        return 1;
    }
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_00, FLD_CBUS_NEG_INT))
    {
		if(u1DebugLvl == 1)
        {
		    LOG(0,"rx busy 2 \n");
	    }
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_08, 1, FLD_CBUS_NEG_INT_CLR);        
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_08, 0, FLD_CBUS_NEG_INT_CLR);
        return  2;
    }
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_00, FLD_CBUS_POS_INT))
    {
		if(u1DebugLvl == 1)
		{
		    LOG(0,"rx busy 3 \n");
	    }
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_08, 1, FLD_CBUS_POS_INT_CLR);        
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_08, 0, FLD_CBUS_POS_INT_CLR);
        return  3;
    }
    if(!(IO32ReadFldAlign(PDWNC_CBUS_STA_01, FLD_CBUS_IN)))
    {
		if(u1DebugLvl == 1)
		{
		    LOG(0,"rx busy 4 \n");
	    }
        return  4;
    }    
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_01, FLD_LINKTX_FSM))
    {
		if(u1DebugLvl == 1)
		{
		    LOG(0,"rx busy 5 \n");
	    }
        return  5;
    }
    return 0;
}
#endif
