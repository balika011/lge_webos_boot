#include "drv_mhlcbus.h"
#if ENABLE_CBUS

#ifdef CBUS_8032
UINT8 xdata fgCbus8032Ctrl                          _at_ HDMI_BASE+0x1EC; //0xFA01; 
#endif



//---------------------------------------------------------------------------
// MHL_ZONE
#ifdef CC_SONY_VBUS_CTRL
#define VBUS_CTRL_GPIO 223
#else 
#if defined(CC_MT5882)
#if LGE_PROTO_PCBA
#define VBUS_CTRL_GPIO (0xffffffff) // LG PCBA  MHL detect pin to enable mobile charging 
#define MHL_OCP_GPIO   (0xffffffff) // LG PCBA not connected
#else
#define VBUS_CTRL_GPIO 211 //turnkey m1v1 use OPCTRL11
#define MHL_OCP_GPIO   230 //MT5882 turnkey m1v1 use ADIN3
#endif

#else
#define VBUS_CTRL_GPIO 208
#endif
#endif
//#define CBUS_DMSG
//#define CBUS_DEBUG
#define REMY
#define ENABLE_CBUS_LOW_DISCONNECT
#define MHL_PORT_MAX 1

#define CTRL_ENABLE_CBUS_RX 0x80000000
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
static DDC_STATUS_T tDDCStatus;

static UINT8 u1ScrachpadOffset ;
static UINT8 u1ScrachpadIndex ;
static UINT8 au1ScratchPad[SCRATCHPAD_REG_SIZE];
static UINT8 u1AdopterIdH ;
static UINT8 u1AdopterIdL ;
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
static UINT8 u1BootupWaitFlg;
static UINT8 u1OcpWaitFlg;

static UINT8 u1RetryNum;
static UINT8 u1BootUp;
static UINT8 u1CableConCnt;//cable connect loop counter for sony 10ms 
void vCbusDelay2us(UINT8 bValue)
{
	UINT8 bCnt;
	for (bCnt = 0; bCnt < bValue; bCnt++) 
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

//#ifdef CBUS_DRIVER_VERIFY
static void Cbus_GpioOutput(INT16 i4Gpio, INT16 i4fgSet) reentrant
{
#ifdef CBUS_8032
	UINT32 u4Val;
	INT8 i4Idx,i4GpioNum;
	i4GpioNum = i4Gpio - 200;
	i4Idx = i4GpioNum & 0x1f;
	
#ifdef T8032_GPIO_DEBUG
	if(MGET_ARM_POWER_STATE() != ARM_POWER_OFF)
		return ;
#endif
	if ((i4GpioNum < 0) || (i4GpioNum >= TOTAL_OPCTRL_NUM))
    {
        return ;
    }

	if (i4GpioNum <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOOUT1);
    }
	
	u4Val = (i4fgSet) ?
		(u4Val | (1L << i4Idx)) :
	(u4Val & ~(1L << i4Idx));
	
	if (i4GpioNum <= 31)
    {
        vIO32Write4B(PDWNC_GPIOOUT0, u4Val);
    }
    else
    {
        vIO32Write4B(PDWNC_GPIOOUT1, u4Val);
    }
#else
    GPIO_SetOut(i4Gpio, i4fgSet);
#endif
}


static UINT8 Cbus_GpioInput(INT16 i4Gpio) reentrant
{
	UINT32 u4Val;
	INT8 i4Idx;

	i4Idx = i4Gpio - 200;
	if (i4Idx <= 31)
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOIN0);
    }
    else
    {
        u4Val = u4IO32Read4B(PDWNC_GPIOIN1);
    }
		
	i4Idx &= 0x1f;
	return ((u4Val & (1L << i4Idx)) ? 1:0);	
}


static UINT8 _Cbus_SendMsgEx(UINT16 *pMsgData, UINT8 dataSize) reentrant
{
    UINT8 i;
	UINT16 ui2TxMsg = NULL_DATA;
    for(i = 0; i < dataSize; i++)
    { 
		if( i & 1) 
		{
		   vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF1);
		}
		else
		{
		   vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF0);
		}
		
		if((*(pMsgData+i)) & 0x100)
		{
			 ui2TxMsg = (*(pMsgData+i));
		}
   }
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,dataSize,FLD_TX_NUM);	
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
    if((u2CurrTxMsg & 0x600) == 0x10)
    {
	    SetMHLStatus(STATE_MHL_TX_SEND_MSG);
	    u1RetryNum = 0;
    }
#ifdef CBUS_DMSG
	for(i = 0; i < dataSize; i++)
	{
	    LogSD("tx >>", (*(pMsgData+i)));
	}
#endif	
	return 0;
}

void _Cbus_OcpStopTimer(void)
{
    vStopTimer1(MHL_OCP_TIMER_ID);
	u1OcpWaitFlg = 0;
}
void _Cbus_OcpStartTimer(void)
{
	vSetTimer1(MHL_OCP_TIMER_ID, TIMER_OCP_POLLING);
	u1OcpWaitFlg = 1;
}

static void _Cbus_OcpTimerHandling(void) reentrant
{  
    if((u1OcpWaitFlg == 1)&&(fgIsTimer1Up(MHL_OCP_TIMER_ID)))
    {
	#ifdef CBUS_OC_STB_ADIN_USING
		if(u1CbusADINGetOcpStatus() == TRUE)
	#else
		if(u1CbusGetOcpStatus()== TRUE)//ocp , nfy
	#endif
		{
#if VBUS_CTRL_GPIO
			Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif

		}
		else
		{
#if VBUS_CTRL_GPIO
			Cbus_GpioOutput(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
		}
         _Cbus_OcpStartTimer();
    }
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
#ifdef CBUS_8032
        LogS("[Cbus]cbus rx queue full\n");
#else
        LOG(0, "cbus rx queue full\n");
#endif
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
#ifdef CBUS_8032
        LogS("[Cbus]rx queue empty \n");
#else
        LOG(0,"rx queue empty \n");
#endif
#endif
    }
    
    // copy to buffer first
    (void)x_memcpy(prMsg, &(arRxMscQueue[mhl_rxQ_read_idx]), sizeof(MHL_MSG_T));

    mhl_rxQ_read_idx = (mhl_rxQ_read_idx + 1) % MHL_RX_QUEUE_SIZE;
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

void vCbus_EnableInterrupt(void)
{
	//vIO32Write4BMsk(PDWNC_CBUS_LINK_0D,0x3C0E0,0x1ffff);
#ifdef CBUS_8032
	MCBUS_EN_INT();
#endif	
	//vIO32Write4B(PDWNC_CBUS_LINK_0D,0x5E0);
	vIO32Write4B(PDWNC_CBUS_LINK_0D,0x2c7fc);
}
void vCbus_DisableInterrupt(void)
{
	//vIO32Write4BMsk(PDWNC_CBUS_LINK_0D,0x3ffff,0x3ffff);	
#ifdef CBUS_8032	
	MCBUS_DIS_INT();
#endif
	vIO32Write4B(PDWNC_CBUS_LINK_0D, 0x00000000);
}

void vCbus_ValInit(void)
{
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
	u1ScrachpadOffset = 0xff ;
	u1ScrachpadIndex = 0;
	u1AdopterIdH = 0xff;
	u1AdopterIdL = 0xff;
	
	MhlSetHpd = 0;
	MhlSetPathen = 0;
	MscReadDevCap = 0;
	u2CurrTxMsg = NULL_DATA;
	u1BootupWaitFlg = 0;
	u1OcpWaitFlg = 0;
    u1RetryNum = 0xff;    
    u1BootUp = 0;
	u1CableConCnt = 0;
}
void vCbus_HwInit(void)
{
#ifndef CBUS_8032
    _Cbus_Set8032Control(0);
#endif    
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
	// disable wakeup and discovery
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);

#ifdef LOOPBACK_TEST
	// Enable CBUS loopback
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,1,FLD_DUPLEX);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,1,FLD_FAKE_SOURCE);
#else              
	// rx INT through hold and Tx bit
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,0,FLD_DUPLEX);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_FAKE_SOURCE);
#endif

	// disable cbus disconnect INT			
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0,FLD_CBUS_DISCONN_CNT_EN);
        
	// Enable CBUS Rx
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,1,FLD_LINKRX_EN);
			
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

#ifndef LOOPBACK_TEST
	DISABLE_PINMUX_TO_CBUS();//pdmux2 //bit 29:26, function 2
	vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PU);//280e8 bit 3 PU  --- SET TO 0
	vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
	if (MGET_SYS_PRE_STANDBY_STATUS() != STBY_FROM_NORMAL_POWER)  //From AC on, need to initialize CEC register
	{
	    vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
	    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
	    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
    }
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07, 1, FLD_LDO_SWITCH_HW);//28d50 bit 20--- set to 1, control LDO ouput by h/w

    vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,32,FLD_RETRY_DISCONN_THR);//20

    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0,FLD_ZCBUS_HW);//sw control 1k&100k
    if(u1GetCdsenseStatus())//cable connected,when power on
    {
        u1BootUp = 1;
		LogS("detected the cdsense!\n");
    }
    else //cable disconnected
    {
        u1BootUp = 0;
    }
    if (MGET_SYS_PRE_STANDBY_STATUS() != STBY_FROM_NORMAL_POWER)	  //From AC on, need to initialize CEC register
    {    
#if VBUS_CTRL_GPIO
	Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
     }

//for remy es2
        #if defined(CC_MT5882)
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0x7ffff,FLD_INT_STA_MASK);
        #endif
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,0,FLD_CBUS_DRV_H_SEL);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_OE_FAST1);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_ADP_SYNC);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_OE_FAST);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_CBUS_DEGLITCH);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_LINKRX_DIS_TO_ARB23);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,1,FLD_ARB_DRIVEH);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_BAK,0xf,FLD_LINK_ACK_WIDTH_UPPER);//for 24m clock only
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,1,FLD_LINK_ACK_MANU_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,0x4,FLD_LINK_ACK_WIDTH);//for 24m clock only
#if 1
   //for 24M clock
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,24,FLD_LINKRX_BITIME);//27
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,16,FLD_ADP_BITIME_MIN);//20
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,30,FLD_ADP_BITIME_MAX);//33
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,42,FLD_CBUS_DISCONN_THR);//47
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A,24,FLD_LINKRX_ACK1_SYNC);//22 ? 27
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
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,11,FLD_LINK_ACK_WIDTH);//12
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,12,FLD_LINK_BITIME_MID);//13
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,18,FLD_LINK_RXDECISION);//24
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,70,FLD_RX_BT_TIMEOUT);//50 ? 100
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
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_04,12,FLD_LINK_ACK_WIDTH);//12
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
#endif
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf0000000,0xf0000000);
	vCbusDelay2us(1);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0,0xf0000000);
}


#ifdef CBUS_8032
void vCbus_Interrupt(void)
#else
static void _Cbus_Interrupt(UINT16 u2Vector)
#endif
{  
#ifndef POLLING
#ifndef CBUS_8032
	ASSERT(u2Vector == PDWNC_INTNO_CBUS_INT);
	UNUSED(u2Vector);
#endif
#endif	
#ifdef CBUS_DMSG		
    UINT16 test[16];
    UINT8 test_idx = 0;
#endif    
	UINT32 u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);
	UINT32 u4ClrInt = 0; //= u4IO32Read4B(PDWNC_CBUS_LINK_08);
	UINT8 u1RxMsgCnts = 0;
	UINT16 u2RxMsg = 0;
	UINT8 i = 0;
	UINT16 aRxMsg[16];	
#ifndef CBUS_8032
	HAL_TIME_T tRxTime;
    x_memset(&tRxTime, 0, sizeof(HAL_TIME_T));
#endif
#ifdef LOOPBACK_TEST
        MHL_MSG_T rRxMsg;
#endif
		if (u4IntStat & 0x1)
		{
			ENABLE_PINMUX_TO_CBUS();
			//for cts test
			vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
			#if defined(CC_MT5399)
			vIO32WriteFldAlign(PDWNC_PDMISC,1, FLD_CBUS_100K_ADJUST);
			#else
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,4, FLD_ZCBUS_SINK_ON_CTRL);
			#endif
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,0, FLD_CBUS_1K_CTRL);
#ifndef FORCE_1K_ON 
			u4ClrInt |= 0x1;
#endif
#ifdef CBUS_DMSG
#ifdef CBUS_8032
			LogS("[Cbus]discovery pulse detected.....\n");	
#else
			LOG(0, "discovery pulse detected.....\n");	
#endif	
#endif	
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,1,FLD_ADP_BITIME_RST);
			
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,0,FLD_ADP_BITIME_RST);

			//disable discovery en
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);	
			vCbus_EnableInterrupt();
            iMhlStateNew = MHL_STATE_CONNECTED;	
			MhlSetHpd = 1;
			MhlSetPathen = 0;
			SetMHLStatus(STATE_MHL_HAND_SHAKING_FAIL);

			// enable cbus disconnect INT
#ifdef ENABLE_CBUS_LOW_DISCONNECT                        
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,1,FLD_CBUS_DISCONN_CNT_EN);
#endif
			// turn on rx sense 100k
		}
	if (u4IntStat & 0x8000)//   1 5
	{
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x8000);
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
#ifdef CBUS_DMSG
#ifdef CBUS_8032
	//	LogS("[Cbus]Interrupt for RX rbuf level meet rbuf_lvl_thr....\n");		
#else
		LOG(5, "Interrupt for RX recevied data....\n");		
#endif
#endif	
	}
	if ((u4IntStat & 0x10))//bit 4
	{
		u4ClrInt |= 0x10;
#ifdef CBUS_DMSG		
#ifdef CBUS_8032
        LogS("wake up pulse or discovery pulse timeout int");
#else
		LOG(0,"wake up pulse or discovery pulse timeout int\n");
#endif		
#endif
	}
	if ((u4IntStat & 0x8))//bit 3
	{
		u4ClrInt |= 0x8;
#ifdef CBUS_DMSG		
#ifdef CBUS_8032
		LogS("ill wake up pulse width int");
#else
		LOG(0,"ill wake up pulse width int \n");
#endif		
#endif
	}
	if ((u4IntStat & 0x4))//bit 2
	{
		u4ClrInt |= 0x4;
#ifdef CBUS_DMSG		
#ifdef CBUS_8032
		LogS("ill width from wakeup pulse to discovery pulse");
#else
		LOG(0,"ill width from wakeup pulse to discovery pulse \n");
#endif		
#endif
	}
	if ((u4IntStat & 0x20000))//bit 17
	{
		u4ClrInt |= 0x20000;
#ifdef CBUS_DMSG		
#ifdef CBUS_8032
		LogS("cbus rx bit time timeout int");
#else
		LOG(0,"cbus rx bit time timeout int\n");
#endif		
#endif
	}
	
	if ((u4IntStat & 0x400))
	{
		u4ClrInt |= 0x400;
		iMhlStateNew = MHL_STATE_INIT;
#if VBUS_CTRL_GPIO
	    Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif		
		//to do
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		LogS("[Cbus]CABLE DISCONNECTTED	\n");
#else
		LOG(0, "CABLE DISCONNECTTED	\n");
#endif
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
		u1RetryNum = 0xff;
		    //stop timer set error code
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		LogS("[Cbus]Interrupt for TX retry timeout....\n");		
#else
		LOG(0, "Interrupt for TX retry timeout....\n");		
#endif
#endif		
	}
	if (u4IntStat & 0x80)
	{	
		u4ClrInt |= 0x80;		
	    if(IsMHLStatus(STATE_MHL_TX_SEND_MSG))
	    {
	        ClrMHLStatus(STATE_MHL_TX_SEND_MSG);
	        SetMHLStatus(STATE_MHL_TX_SEND_MSG_OK);
	    }
	    u1RetryNum = 0xff;
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		//LogS("[Cbus]Interrupt for TX transmit complete......\n"); 
#else
		LOG(5, "Interrupt for TX transmit complete......\n"); 
#endif
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
	    if(u1RetryNum == 0xff) 
	    {
	        u1RetryNum = 0;
	    }
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		LogS("[Cbus]Interrupt for TX arbitration lose......\n"); 
#else
		LOG(0, "Interrupt for TX arbitration lose......\n"); 
#endif
#endif		
	}					
	if (u4IntStat & 0x200)
	{
		u4ClrInt |= 0x200;
		if(iMhlStateOld == MHL_STATE_WAIT_CABLE_CONNECT)
			iMhlStateNew = MHL_STATE_DISCOVERY;
		
#if VBUS_CTRL_GPIO
		Cbus_GpioOutput(VBUS_CTRL_GPIO, 1);//enable vbus
		_Cbus_OcpStopTimer();
		_Cbus_OcpStartTimer();
#endif
		//to do
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		LogS("[Cbus]cable connected\n"); 
#else
		LOG(0, "cable connected\n"); 
#endif
#endif
	}

	if ((u4IntStat & 0x4000))
	{
		u4ClrInt |= 0x4000;
		iMhlStateNew = MHL_STATE_INIT;
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
        
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0,FLD_CBUS_DISCONN_CNT_EN);
        vCbus_DisableInterrupt();
		//to do
#ifdef CBUS_DMSG
#ifdef CBUS_8032
		LogS("[Cbus]low disconnect detected	\n");
#else
		LOG(0, "low disconnect detected	\n");
#endif
#endif
	}
	// handle CBUS low, Tx device disconneted
	// TODO
	if (u4IntStat & 0x20)//	5
	{
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x20);
		vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
#ifdef CBUS_DMSG
#ifdef CBUS_8032
	//	LogS("[Cbus]Interrupt for RX rbuf level meet rbuf_lvl_thr....\n");		
#else
		LOG(5, "Interrupt for RX rbuf level meet rbuf_lvl_thr....\n");		
#endif
#endif	
	}
	vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
	vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);
	u1RxMsgCnts = IO32ReadFldAlign(PDWNC_CBUS_STA_00,FLD_RBUF_LVL_LAT);
	
	// Rx data enqueue
	while (u1RxMsgCnts != 0) 
	{
		u1RxMsgCnts --;
#ifdef CBUS_8032
		u2RxMsg = 0;
		u2RxMsg |= ((UINT16)(u1RegRead1B(PDWNC_CBUS_RBUF+1)))<<8;	
		u2RxMsg |= (UINT16)(u1RegRead1B(PDWNC_CBUS_RBUF));		
#else
		u2RxMsg = u4IO32Read4B(PDWNC_CBUS_RBUF);
#endif	
#ifdef CBUS_DMSG
        if(test_idx < 16)
        {
		test[test_idx] = u2RxMsg;
		test_idx ++;
		}
#endif
        if((u2RxMsg & 0x500) == 0x500) //control data
        {
            i = 0;
        }
		aRxMsg[i] = u2RxMsg;
		i++;
        if(u2RxMsg == CBUS_DDC_CTRL_ABORT) 
        {
            if(u1RxMsgCnts == 0)
            {
                //send abort or do nothing
                return;
            }
            else
            {
               i--;
            }
        }
#ifdef CBUS_DMSG
#ifdef POLLING
#ifdef CBUS_8032
        LogSD("[Cbus]rx msg \n",u2RxMsg);
#else
        LOG(0,"rx msg 0x%x \n", u2RxMsg);
#endif
#else
#ifdef CBUS_8032
        LogSD("rx <<",u2RxMsg);
#endif
#endif		
#endif		
#ifdef LOOPBACK_TEST
		x_memset(&rRxMsg, 0, sizeof(MHL_MSG_T));
		rRxMsg.channel = (u2RxMsg>> 9) & 0x3;
		rRxMsg.ctrl = (u2RxMsg >> 8) & 0x1;
		rRxMsg.u1Data = u2RxMsg & 0xff;
		_Mhl_RxEnqueue(&rRxMsg);
#endif
	}
	u1RxMsgCnts = i;
#if 1	
	if((u1RetryNum < NMAX))
	{
	    u1RetryNum ++;
		CBUS_TRIGGER_TX_HW();
		return;
	}
#endif	

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

void _vCbusStateReset(void)
{
	//iMhlStateOld = MHL_STATE_NONE;
	//iMhlStateNew = MHL_STATE_NONE;
	
	iDdcSateOld = DDC_STATE_NONE;
	iDdcSateNew = DDC_STATE_NONE;
	
	iWriteburstStateOld = MHL_WRITE_BURST_NONE;
	iWriteburstStateNew = MHL_WRITE_BURST_NONE;
	
	iMhlRxStateOld = MHL_RX_STATE_IDLE;
	iMhlRxStateNew = MHL_RX_STATE_IDLE;
	_MHLStatus = 0;
	x_memset(&tDDCStatus,0,sizeof(DDC_STATUS_T));
	u1RetryNum = 0xff;
    u1CableConCnt = 0;
}
void _Cbus_BootupStopTimer(void)
{
#ifdef CBUS_8032
    vStopTimer1(MHL_BOOTUP_TIMER_ID);
	u1BootupWaitFlg = 0;
#else
    x_timer_stop(_hMhlBootupTimer);
#endif
}

void _Cbus_BootupStartTimer(void)
{
#ifdef CBUS_8032
	vSetTimer1(MHL_BOOTUP_TIMER_ID, TIMER_BOOT_UP);
	u1BootupWaitFlg ++;
#else
    x_timer_start(_hMhlBootupTimer,TIMER_BOOT_UP,X_TIMER_FLAG_ONCE,_Cbus_BootupTimerHandling,NULL);
#endif
}

static void _Cbus_BootupTimerHandling(void) reentrant
{  
    if((u1BootupWaitFlg >0)&&(u1BootupWaitFlg < 3)&&(fgIsTimer1Up(MHL_BOOTUP_TIMER_ID)))
    {
		_Cbus_BootupStartTimer();
		return;
    }
    if((u1BootupWaitFlg == 3)&&(fgIsTimer1Up(MHL_BOOTUP_TIMER_ID)))
	{
		if(u1BootUp == 2)
		{
			u1BootUp = 3;

			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		    vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
#if VBUS_CTRL_GPIO
			Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
			_Cbus_BootupStartTimer();
		}
		return;
	}	
    if((u1BootupWaitFlg >3)&&(u1BootupWaitFlg <6)&&(fgIsTimer1Up(MHL_BOOTUP_TIMER_ID)))
	{
	    _Cbus_BootupStartTimer();
	    return;
	}
    if((u1BootupWaitFlg == 6)&&(fgIsTimer1Up(MHL_BOOTUP_TIMER_ID)))
    {
		u1BootUp = 0;
		_Cbus_BootupStopTimer();
		vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
		ENABLE_PINMUX_TO_CBUS();
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_WAKEUP_EN); 	
#if VBUS_CTRL_GPIO
		Cbus_GpioOutput(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
		
		_Cbus_OcpStopTimer();
		_Cbus_OcpStartTimer();
		iMhlStateNew = MHL_STATE_DISCOVERY;
		return;
	}
}
#define HDMI_GPIO_HPD0 (217)
#define HDMI_GPIO_HPD1 (221)
#define HDMI_GPIO_HPD2 (225)
#define HDMI_GPIO_HPD3 (229)

UINT8 bHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 input = 0;

    switch(ePort)
    {
        case HDMI_SWITCH_1: 
				input = GPIO_GetIn(HDMI_GPIO_HPD0); 				
	        break;
        case HDMI_SWITCH_2: 
	          input = GPIO_GetIn(HDMI_GPIO_HPD1);
	         break;
        case HDMI_SWITCH_3: 
	          input = GPIO_GetIn(HDMI_GPIO_HPD2);
	         break;
        case HDMI_SWITCH_4: 
	          input = GPIO_GetIn(HDMI_GPIO_HPD3);
	         break;
	  default:
	         break;
	}
    
    return input;
}
Resister_State eResist_state;

void _vPatchForHPDCTSFail(void)
{
    if(!u1GetCdsenseStatus())
    {
        if(bHDMIPort5VStatus(HDMI_SWITCH_1) && (eResist_state!= Resister_PullLow)&&(!u1GetCdsenseStatus()))
        {   
                eResist_state =Resister_PullLow;
            	vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
			#if defined(CC_MT5399)
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_SINK_ON_CTRL);	
                vIO32WriteFldAlign(PDWNC_PDMISC,1, FLD_CBUS_100K_ADJUST);
			#else
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,4, FLD_ZCBUS_SINK_ON_CTRL);	
			#endif
        }
        else
        if((bHDMIPort5VStatus(HDMI_SWITCH_1)==0) && (eResist_state != Resister_NotPullLow)&&(!u1GetCdsenseStatus()))
        {
                eResist_state = Resister_NotPullLow;
                vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
				#if defined(CC_MT5399)
                vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
				#endif
        }
    }
}

void vCbusMainLoop(void)
{
	// Sink state machine +++++++++++++++++++++++++++++++++++
    UINT32 u4IntStat = 0;
    UINT32 u4ClrInt = 0;
#ifdef CBUS_8032
	if(fgCbus8032Ctrl==0)
	{
		MCBUS_DIS_INT();
		return;
	}
#endif
	if (iMhlStateOld != iMhlStateNew)
	{
#ifdef CBUS_DMSG
#ifdef CBUS_8032
#ifdef DEBUG_CBUS_8032
		LogS("[Cbus]iMhlState changed,");
		LogD(iMhlStateOld);
		LogD(iMhlStateNew);
#endif
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
            _Cbus_OcpStopTimer();
		    _vCbusStateReset();
            vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf0000000,0xf0000000);//hw reset
			vCbusDelay2us(1);
            vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0,0xf0000000);
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
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,1,FLD_LINKRX_EN);
	
			//when cbus floating, set default input to 0
            if((u1BootUp == 1)||(!u1GetCdsenseStatus()))
            {
              DISABLE_PINMUX_TO_CBUS();
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
			  vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
            }		
            if(!u1GetCdsenseStatus())
			{
                vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
		        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		        vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);	
				#if defined(CC_MT5399)
                vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
				#endif
			}
            vCbus_DisableInterrupt();     
		    _Mhl_InitQueue();
            vCbusClrStatusAll();
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
               if(MGET_SYS_PRE_STANDBY_STATUS()== STBY_FROM_NORMAL_POWER)//IR power off
               {
                   u1BootUp =2;
                   u1BootupWaitFlg = 3;
#if VBUS_CTRL_GPIO
				   Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//enable vbus
#endif
				   vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 1
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
				   vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
                   DISABLE_PINMUX_TO_CBUS();
               }
               else
               {
                  u1BootUp = 2;
                  u1BootupWaitFlg = 0;
                  ENABLE_PINMUX_TO_CBUS();
				  vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
				  vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
#if VBUS_CTRL_GPIO
				 Cbus_GpioOutput(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
			   _Cbus_OcpStopTimer();
			   _Cbus_OcpStartTimer();

               }
                _Cbus_BootupStartTimer();
			}
#ifdef CBUS_8032
			_Cbus_BootupTimerHandling();
#endif
        break;
		case MHL_STATE_WAIT_CABLE_CONNECT:

			u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);	   
			if (u4IntStat & 0x400)
			{
				u4ClrInt |= 0x400;
#ifdef CBUS_DMSG
#ifdef CBUS_8032
				//LogS("[Cbus]Clear Cable out NOISE !!!\n"); 
#else
				LOG(5,"Clear Cable out NOISE !!!\n"); 
#endif
#endif
			}
			if ((u4IntStat & 0x200) || (u1GetCdsenseStatus()))//cable detect int
			{
				u1CableConCnt ++;//polling 10 times for cable connect,
				u4ClrInt |= 0x200;			
				if(u1CableConCnt == 10)
				{
				iMhlStateNew = MHL_STATE_DISCOVERY;               
                vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);                    
                vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---  SET to 0, high z state
                #if defined(CC_MT5399)
                vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
				#endif
                vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);     
#ifndef FPGA				
				ENABLE_PINMUX_TO_CBUS();
#endif
			    vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_WAKEUP_EN);
#if VBUS_CTRL_GPIO
				Cbus_GpioOutput(VBUS_CTRL_GPIO, 1);//enable vbus
#endif
				_Cbus_OcpStopTimer();
				_Cbus_OcpStartTimer();
				}
			}

			if (u4IntStat & 0x1)
			{
				u4ClrInt |= 0x1;
#ifdef CBUS_DMSG
#ifdef CBUS_8032
				LogS("[Cbus]discovery pulse detected@MHL_STATE_WAIT_CABLE_CONNECT.....\n");  
#else
				LOG(0,"discovery pulse detected@MHL_STATE_WAIT_CABLE_CONNECT.....\n");  
#endif	
#endif			
             	iMhlStateNew = MHL_STATE_DISCOVERY;
			}
			vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
			vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	   					
		break;

		case MHL_STATE_DISCOVERY:	
			u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);

			if (u4IntStat & 0x200)
			{
			u4ClrInt |= 0x200;
#ifdef CBUS_DMSG
#ifdef CBUS_8032
			LogS("[Cbus]Clear Cable in NOISE !!!\n"); 
#else

			LOG(5,"Clear Cable in NOISE !!!\n"); 
#endif	
#endif			

			}

			if ((u4IntStat & 0x400) || (!u1GetCdsenseStatus()))
			{
				u4ClrInt |= 0x400;
				iMhlStateNew = MHL_STATE_INIT; 
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);				
				DISABLE_PINMUX_TO_CBUS();
#if VBUS_CTRL_GPIO
				Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
#ifdef CBUS_DMSG
#ifdef CBUS_8032
				LogS("[Cbus]CABLE DISCONNECT @  MHL_STATE_DISCOVERY \n"); 
#else

				LOG(0,"CABLE DISCONNECT @  MHL_STATE_DISCOVERY \n"); 
#endif	
#endif						
            }

			if (u4IntStat & 0x2)
			{	     
				u4ClrInt |= 0x2;
#ifdef CBUS_DMSG
#ifdef CBUS_8032
				LogS("[Cbus]WAKEUP pulse detected.....\n");			
				LogD(u4IO32Read4B(PDWNC_CBUS_STA_00));
#else
				LOG(0,"WAKEUP pulse detected.....\n");	
#endif	
#endif				
                //disable wake up en
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);		
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_DISCOVERY_EN);
				vIO32Write4B(PDWNC_CBUS_LINK_0D,1);//enable discovery pulse interrupt				
				MCBUS_EN_INT();
			}
			vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
			vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);
			
			_Cbus_OcpTimerHandling();
		break;

		case MHL_STATE_CONNECTED:
			//_Cbus_OcpTimerHandling();
			if(!u1GetCdsenseStatus())
			{
				iMhlStateNew = MHL_STATE_INIT;
				DISABLE_PINMUX_TO_CBUS();
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
				vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
#if VBUS_CTRL_GPIO
				Cbus_GpioOutput(VBUS_CTRL_GPIO, 0);//disable vbus
#endif
				break;
			}
        break;
        default:
        break;
	}
}

void vCbusInit(void)
{
#ifdef CBUS_DMSG
#ifdef CBUS_8032
    LogS("[Cbus]cbus init \n");
#endif
#endif    
    vCbus_ValInit();
    vCbus_HwInit(); 
}

#if defined(CC_MT5882)
#define MHL_OCP_GPIO 230
#else
#define MHL_OCP_GPIO 229
#endif

UINT8 u1CbusGetOcpStatus(void)
{
	if(Cbus_GpioInput(MHL_OCP_GPIO))//3.3v normal, 0v ocp
	{
	   return FALSE;
	}
	return TRUE;
}
#endif

#ifdef CBUS_OC_STB_ADIN_USING
/*
For MT5882 Turnkey, the MHL OC pin is shared with the CI OC pin.so we should dectect the OC event
by juding the voltage.
*/
UINT8 u1CbusADINGetOcpStatus(void) 
{
	UINT16 getvalue;
	UINT16 tagtvalue1;
	UINT16 tagtvalue2;
	tagtvalue1 = (UINT16)(1.93*255)/2.8;
	tagtvalue2 = (UINT16)(1.56*255)/2.8;
	/*target value = (measured voltage * 255)/(max ADIN voltage), 255 is the maximum return value for PDWNC_ReadServoADCChannelValue */
	getvalue = MReadSrvad(3);
	if((getvalue < tagtvalue1 + 10) && (getvalue > tagtvalue1 - 10))
	{
		LogS("MHL OC HAPPEN!\n");
		return TRUE;//OC happen.
	}
	else if((getvalue < tagtvalue2 + 10) && (getvalue > tagtvalue2 - 10))//10 reprsent 0.1v tolerance.
	{
		LogS("MHL and Other OC HAPPEN!\n");
		return TRUE;//OC happen.
	}
	else
	{
		return FALSE;//OC not happen.
	}
}
#endif


