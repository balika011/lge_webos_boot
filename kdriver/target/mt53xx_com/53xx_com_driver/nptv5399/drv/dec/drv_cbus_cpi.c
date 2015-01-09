#include "drv_cbus_cpi_if.h"
#include "x_timer.h"
#include "hw_hdmi.h"
#include "drv_hdmi.h"
#include "pdwnc_if.h"
#if 1//def SYS_MHL_SUPPORT
#include "mhl_if.h"
#endif
#include "util.h"
#include "sv_const.h"
#include "video_def.h"
#include "x_os.h"

#ifdef CC_LGE_PROTO_PCBA
#define VBUS_CTRL_GPIO (0xffffffff) // LG PCBA  MHL detect pin to enable mobile charging 
#define MHL_OCP_GPIO   (0xffffffff) // LG PCBA not connected
#else
#define VBUS_CTRL_GPIO 211 //turnkey m1v1 use OPCTRL11
#define MHL_OCP_GPIO   230 //MT5882 turnkey m1v1 use ADIN3
#endif

static UINT8 u1BootUp;
static UINT8 bCbusState;
static UINT8 bInitFlg = 0;
static UINT8 u1AnReady;
static UINT8 u1HandShakingCnt =0;
static UINT8 u1DebugLvl;
static Resister_State eResist_state = Resister_None;

static UINT8 arEdid[EDID_SIZE] ;

static UINT8  _arMhlDevCap[0x10];
static UINT8  _arMhlSrcDevCap[0x10];

static UINT8 iMhlStateOld ;
static UINT8 iMhlStateNew ;

static UINT16 iDdcOffset ;
static UINT8 au1An[8];
static UINT8 au1Aksv[5];
static UINT8 au1Bksv[5] ;
static UINT8 au1Ri1[2];
static UINT8 u1Bcaps ;
static UINT8 au1Bstatus[2] ={0} ;
static UINT8 u1_rapk = 0;
static UINT16 u1ucpk[2] = {0, 0};
static HANDLE_T _hCbusThreadHdl;
static HANDLE_T _hCbusCommandHdl;
static HANDLE_T _hCbusSema = NULL_HANDLE;
#ifdef SYS_MHL_SUPPORT
#ifdef CBUS_OC_ADIN_USING
static HANDLE_T _hMhlOcpThreadHdl;
static HANDLE_T _hMhlOcpSema = NULL_HANDLE;
#endif
#endif
static HANDLE_T _hCbusCommandSema = NULL_HANDLE;
static HANDLE_T _hMhlTimer;
static HANDLE_T _hMhlCbusCommandTimer;
UINT8 u1FlgHdmiMhlSrcChged;
// send MSC message as a requester, and handshaking.
MHL_CBUS_SINK_CONTROL		stSinkCtr;
MHL_CBUS_Sink_REQUESTER 	stSinkRequster;

MHL_CBUS_TXBUF 				stCbusMscTx;
MHL_CBUS_TXBUF 				stCbusDdcTx;
MHL_CBUS_TXBUF 				stCbusTxCtr;
Sink_CBUS_TX_Timer          stBootUpTimer;
Sink_CBUS_TX_Timer			stCbusTxTimer ;
Sink_CBUS_TX_Timer  		stCbusMscTimer;
Sink_CBUS_TX_Timer  		stCbusDdcTimer ;
CBUS_COMMAND      			stCbusCmd ;

static MHL_MSG_T arMhlLogQueue[MHL_LOG_QUEUE_SIZE];

static UINT16 mhl_Log_read_idx;
static UINT16 mhl_Log_write_idx;
static BOOL  _fgIsMHLCableConnect = 0;
BOOL _fgIsMHLDiscoveryOK   = 0;

static UINT8 u1CbusMscAbortDelay2S =0;
static UINT8 u1CbusDdcAbortDelay2S =0;
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
static UINT8 u1CbusMscErrCode =0;
static UINT8 u1CbusDdcErrCode =0;
static UINT8 u1RevRcpKey = 0xff;
static UINT8 u1isRcpRev = 0;
static UINT8 u1EnVbus = 1;
//static UINT8 au1writeburst[16];
//static UINT8 u1writeburst = 0;

#define MHL_TIMER_PERIOD  10     // ms
#define MHL_BOOT_UP_TIMER_RERIOD 550/MHL_TIMER_PERIOD
#define MHL_BOOT_UP_TIMER1_RERIOD 1100/MHL_TIMER_PERIOD
#define MHL_BOOT_UP_TIMER2_RERIOD 800/MHL_TIMER_PERIOD
#define MHL_TIMER_PKT_PERIOD 170/MHL_TIMER_PERIOD
#define MHL_MSC_ABORT_DELAY  2100/MHL_TIMER_PERIOD
#define MHL_DDC_ABORT_DELAY  2100/MHL_TIMER_PERIOD
   
static void _Cbus_Interrupt(UINT16 u2Vector);

#ifdef CC_MHL_SENSE_NOT_CONNECTED
static UINT8 _u1ForceMHL = 0;
UINT8 u1SetCdsenseStatus(UINT8 u1ForceM)
{
    LOG(3, "SetCdsense = %d.\n", u1ForceM);
    _u1ForceMHL = u1ForceM;
}
#endif


void vCbusSetDeviceAttach(BOOL fgIsMhlCableConnect)
{
	_fgIsMHLCableConnect = fgIsMhlCableConnect;
}

BOOL fgCbusGetDeviceAttach(void)
{
	return _fgIsMHLCableConnect;
}

/*********************************************
//set cbus impedance
1:1K
2:100k
3:Hi-Z
4:hpd
*********************************************/
void vCbusSetImpedance(SINK_CBUS_IMPEDANCE_TYPE Impedance)
{
	switch(Impedance)
	{
		case 1: //1K
			//if(Get_CBUS_PIN_FUNCTION() !=2 )
			{
				ENABLE_PINMUX_TO_CBUS();
			}
			vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1, FLD_ZCBUS_DISCOVER_EN);
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
			//vIO32WriteFldAlign(PDWNC_PDMISC,0, FLD_CBUS_100K_ADJUST);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,3, FLD_CBUS_1K_CTRL);//cbus 1k µµ??
			
			
			break;
		case 2://100K
			//if(Get_CBUS_PIN_FUNCTION() !=2 )
			{
				ENABLE_PINMUX_TO_CBUS();
			}
			vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,4, FLD_ZCBUS_SINK_ON_CTRL);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,0, FLD_CBUS_1K_CTRL);
		
	
			break;
		case 3://high impedance
			//if(Get_CBUS_PIN_FUNCTION() !=2 )
			{
				ENABLE_PINMUX_TO_CBUS();
			}
			vIO32WriteFldAlign(PDWNC_PADCFG14,0,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_SINK_ON_CTRL);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,0, FLD_CBUS_1K_CTRL);
		
			break;
		case 4://hpd
			vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_PUPD);//280E8 bit 9 PUPD ---	SET to 1
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0, FLD_ZCBUS_DISCOVER_EN);
			//For MT5882 only.open the 100K for HDMI 8-11.
		    vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,4, FLD_ZCBUS_SINK_ON_CTRL);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_0C,0, FLD_CBUS_1K_CTRL);
			DISABLE_PINMUX_TO_CBUS();
			break;
		default:
			break;
	}
	
	
}
void vCbusEnLinkRx(BOOL fgEnable)
{
	if(fgEnable)
	{
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,1,FLD_LINKRX_EN);	
	}
	else
	{
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);	
	}
}

void vCbusEnWakeUpDetect(void)
{
	vIO32Write4B(PDWNC_CBUS_LINK_0D,1);//enable discovery pulse interrupt
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN); //xiongfeng modify to disable wakeup detect for HTC mobile phone bug
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_DISCOVERY_EN);
}
void vCbusEnDiscoveryDetect(BOOL fgEnable)
{
	if(fgEnable)
	{
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,1,FLD_DISCOVERY_EN);
		vIO32Write4B(PDWNC_CBUS_LINK_0D,1);//enable discovery pulse interrupt
		//vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN); 
	}
	else
	{
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN); 
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
		vIO32Write4B(PDWNC_CBUS_LINK_0D,0);//enable discovery pulse interrupt
	}
}

void vCbusTrackBitTimeRst(void)
{
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,1,FLD_ADP_BITIME_RST);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_01,0,FLD_ADP_BITIME_RST);
}

void vCbus_EnableInterrupt(void)
{
//0x2c8c0
	vIO32Write4B(PDWNC_CBUS_LINK_0D,0x6c8c0);
}

void vCbus_DisableInterrupt(void)
{
	vIO32Write4B(PDWNC_CBUS_LINK_0D, 0x00000000);
}

void vCBusEnHwMode(BOOL fgEnable)
{
	if(fgEnable)
	{
		vIO32WriteFldAlign(PDWNC_CBUS_TRAN_00,1,FLD_CBUS_TRAN_HW_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_TRAN_01,1,FLD_RG_CBUS_HW_SEL);
		vMHLDisDdcSwMode();
	}
	else
	{
		vIO32WriteFldAlign(PDWNC_CBUS_TRAN_00,0,FLD_CBUS_TRAN_HW_EN);
		vIO32WriteFldAlign(PDWNC_CBUS_TRAN_01,0,FLD_RG_CBUS_HW_SEL);
	}
}


void vCbusSetDiscoveryOK(BOOL fgIsMHLDiscoveryOK)
{
	_fgIsMHLDiscoveryOK = fgIsMHLDiscoveryOK;
	
	//vCbusSetImpedance(SINK_CBUS_IMPEDANCE_100K);
	vCbusEnDiscoveryDetect(FALSE);	
	vCbusTrackBitTimeRst();
	vCbus_EnableInterrupt();	
	vCbusEnLinkRx(TRUE);
	vMHLOpenRxTermination(1);
	vCBusEnHwMode(1);//enable DDC HW mode
	                    
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,1,FLD_CBUS_DISCONN_CNT_EN);// enable cbus disconnect INT  
	
	
}
BOOL fgCBUSGetDiscoveryOK(void)
{
	return _fgIsMHLDiscoveryOK;
}
void vCBusSetDiscoveryNG(void)
{
	_fgIsMHLDiscoveryOK = 0;
}
void vCbusRst(void)
{
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf0000000,0xf0000000);	
	vUtDelay2us(1);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0,0xf0000000);
}

void vCbusDisable(void)
{
	//disable cbus RX			
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
	// Enable wakeup and discovery
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_DISCOVERY_EN);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,0,FLD_WAKEUP_EN);

	// rx INT through hold and Tx bit
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,0,FLD_DUPLEX);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_FAKE_SOURCE);

	// disable cbus disconnect INT			
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0,FLD_CBUS_DISCONN_CNT_EN);

	// Enable CBUS Rx
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN); 		
				  
}

//---------------------------------------------------------------------------
void vCbus_ValInit(void)
{
    u1HDMI_GetEdidData(3, arEdid, EDID_SIZE);

	_arMhlDevCap[0]  = DEVCAP_DEVICE_STATE;
    _arMhlDevCap[1]  = DEVCAP_MHL_VERSION;
    _arMhlDevCap[2]  = DEVCAP_MHL_DEVICE_CATEGORY;
    _arMhlDevCap[3]  = DEVCAP_ADOPTER_ID_H;
    _arMhlDevCap[4]  = DEVCAP_ADOPTER_ID_L;
    _arMhlDevCap[5]  = DEVCAP_VID_LINK_MODE;
    _arMhlDevCap[6]  = DEVCAP_AUD_LINK_MODE;
    _arMhlDevCap[7]  = DEVCAP_VIDEO_TYPE;
    _arMhlDevCap[8]  = DEVCAP_LOG_DEV_MAP;
    _arMhlDevCap[9]  = DEVCAP_BANDWIDTH;
    _arMhlDevCap[10] = DEVCAP_FEATURE_FLAG;
    _arMhlDevCap[11] = DEVCAP_DEVICE_ID_H;
    _arMhlDevCap[12] = DEVCAP_DEVICE_ID_L;
    _arMhlDevCap[13] = DEVCAP_SCRATCHPAD_SIZE;
    _arMhlDevCap[14] = DEVCAP_INT_STAT_SIZE;
    _arMhlDevCap[15] = DEVCAP_RESERVED;
	
	iDdcOffset = 0;

	au1Ri1[0] = 0; 
	au1Ri1[1] = 0;//au1Ri1[2] ={0, 0};
	u1Bcaps = 0;
	au1Bstatus[0]=0; 
	au1Bstatus[1] = 0;//au1Bstatus[2] ={0x0, 0x10}; 
	
	u1_rapk = 0;
    u1ucpk[0] = 0;
	u1ucpk[1] = 0;
	
	u1AnReady = 0;
    u1DebugLvl = 0;
	eResist_state = Resister_None;

#ifdef CC_MHL_3D_SUPPORT
    vMHLParsingEDIDForMHL3D(arEdid);
#endif
    u1BootUp = 0;
    bCbusState = 1;

}


void vCbusClrStatusAll(void)
{
    // clr INT
    vIO32Write4B(PDWNC_CBUS_LINK_08, 0xFFFFFFFF);
    vIO32Write4B(PDWNC_CBUS_LINK_08, 0x00000000);   
}
#ifdef CC_MHL_SENSE_NOT_CONNECTED
extern HDMI_STATUS_T _arHdmiRx[HDMI_SWITCH_MAX];
#endif
UINT8 u1GetCdsenseStatus(void)
{
#ifdef CC_MHL_SENSE_NOT_CONNECTED
	static _u1PreForceMHL = 0;
	if(_u1PreForceMHL != _u1ForceMHL)
	{
		if(_u1ForceMHL == 0) //hdmi
		{
			_arHdmiRx[HDMI_SWITCH_1]._bHDMIState = HDMI_STATE_INIT;
		}
		_u1PreForceMHL = _u1ForceMHL;
	}
    LOG(3, "u1GetCdsenseStatus = %d.\n", _u1ForceMHL);
    return (_u1ForceMHL ? 1 : 0);
#else
    if(IO32ReadFldAlign(PDWNC_CBUS_STA_01, FLD_CBUS_CDSENSE))
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

void vCbusSetVbus(UINT8 u1VbusState)
{
    if(u1EnVbus == 0)
    {
		GPIO_SetOut(VBUS_CTRL_GPIO, 0);
	}
	else
	{
	    if(u1VbusState)
	    {
		   GPIO_SetOut(VBUS_CTRL_GPIO, 1);//enable vbus
	    }
	    else
	    {
		   GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
	    }
	}
}

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


void vCbus_HwInit(void)
{
    UINT32 u4WakeUpReason;
	u4WakeUpReason = PDWNC_ReadWakeupReason();
	LOG(1, "u4WakeUpReason = %d.\n", u4WakeUpReason);
    _Cbus_Set8032Control(0);  

    vIO32WriteFldAlign(PDWNC_PADCFG14, 1,FLD_HDMI_0_HPD_CBUS_E8);
	vCbusDisable();
			
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
	vIO32WriteFldAlign(PDWNC_PADCFG14,1,FLD_HDMI_0_HPD_CBUS_SMT);//SMT ---  SET to 0

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
		GPIO_SetOut(VBUS_CTRL_GPIO, 0);//enable vbus
#endif
      }

	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0B,0x7ffff,FLD_INT_STA_MASK);
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
   vIO32WriteFldAlign(PDWNC_CBUS_LINK_05,162,FLD_WAKE_PUL_WID1_MAX);//148 //for spec it should be 132, but to cover p880 ill pulse, add anohter 5ms 
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
	
}

static void vCbusMHLCableDetect(void)
{
	UINT32 u4IntStat = 0;
	UINT32 u4ClrInt  = 0;	

	u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);	   

	if ((u4IntStat & 0x200) || u1GetCdsenseStatus())//cable detect int)
	{
		//_bMHLCableConnectCnt ++;
		u4ClrInt |= 0x200;		
		//if(_bMHLCableConnectCnt == 1)
		{
			vCbusSetDeviceAttach(TRUE);
		}
	}
	else
	{
		vCbusSetDeviceAttach(FALSE);
	}

	
	if (!(u1GetCdsenseStatus()))//cable detect int
	{
		vCbusSetDeviceAttach(FALSE);
		vCbusSetVbus(FALSE);                     
	}

	vIO32Write4B(PDWNC_CBUS_LINK_08, u4ClrInt);
	vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);	
}

void vCbusTriggerCmpInterrupt(void)
{
	UINT8 u4Stat = 0;	

	u4Stat = IO32ReadFldAlign(PDWNC_CBUS_STA_01,FLD_WAKEUP_FSM);	
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A , 0 , FLD_MON_CMP_SEL);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A , u4Stat +1 , FLD_MON_CMP_DATA);
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_0A , u4Stat, FLD_MON_CMP_DATA);	
}

static void _Cbus_MhlTimerHandle(HANDLE_T ptTmHandle, VOID* pvTag)
{
	vCbusMHLCableDetect();
	
	if(stBootUpTimer.u4MscDdcTmr)
	{
		stBootUpTimer.u4MscDdcTmr --;
		if(stBootUpTimer.u4MscDdcTmr == 55)
		{
			vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);
			vMHLOpenRxTermination(0);
			vCbusSetVbus(TRUE);
		}

		if(stBootUpTimer.u4MscDdcTmr == 0)
		{
			vCbusSetImpedance(SINK_CBUS_IMPEDANCE_1K);
			vCbusSetVbus(TRUE);
			vMHLOpenRxTermination(0);
			vCbusEnWakeUpDetect();
			iMhlStateNew = MHL_STATE_DISCOVERY;
		}
	}
	
	if(stCbusDdcTimer.u4MscDdcTmr)
	{
		stCbusDdcTimer.u4MscDdcTmr --;
		if(stCbusDdcTimer.u4MscDdcTmr == 0)
		{
			stCbusDdcTimer.fgTmrOut =TRUE;
			vCbusTriggerCmpInterrupt();
		}
	}
	
	if(stCbusTxTimer.u4MscDdcTmr)
	{
		stCbusTxTimer.u4MscDdcTmr --;
		if(stCbusTxTimer.u4MscDdcTmr == 0)
		{
			stCbusTxTimer.fgTmrOut =TRUE;
			vCbusTriggerCmpInterrupt();
		}
	}

	if(stCbusMscTimer.u4MscDdcTmr)
	{
		stCbusMscTimer.u4MscDdcTmr --;
		if(stCbusMscTimer.u4MscDdcTmr == 0)
		{
			stCbusMscTimer.fgTmrOut =TRUE;
			vCbusTriggerCmpInterrupt();
		}
	}

	if(u1CbusMscAbortDelay2S > 0)
	{
		u1CbusMscAbortDelay2S --;
	}

	if(u1CbusDdcAbortDelay2S > 0)
	{
		u1CbusDdcAbortDelay2S --;
	}
#ifdef SYS_MHL_SUPPORT
#ifndef CBUS_OC_ADIN_USING
	vOcpHandler();
#else
	VERIFY( x_sema_unlock(_hMhlOcpSema)==OSR_OK);
#endif
#endif
	VERIFY( x_sema_unlock(_hCbusSema)==OSR_OK);
	VERIFY( x_sema_unlock(_hCbusCommandSema)==OSR_OK);
}
void vCbusSetHpd(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_SET_HPD;
	stSinkRequster.u1Len =1;
	
	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
	
}

void vCbusClrHpd(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_CLR_HPD;
	stSinkRequster.u1Len =1;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void vCbusSetDevCapReady(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_STATE;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_READY_BITS;
	stSinkRequster.u2ReqBuf[2] = 0x401;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
	
}


void vCbusSetDevCapChange(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_STATE;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_REGCHANGE_INT;
	stSinkRequster.u2ReqBuf[2] = 0x401;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void vCbusSetPathEn(void)
{
	if(u1CbusMscAbortDelay2S)
			return;

	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_STATE;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_ACTIVE_LINK_MODE;
	stSinkRequster.u2ReqBuf[2] = 0x408;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
	
}

void vCbusGratWriteBurst(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_SET_INT;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_REGCHANGE_INT;
	stSinkRequster.u2ReqBuf[2] = 0x400 | INT_GRT_WRT;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}
void vCbusRequestWriteBurst(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_SET_INT;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_REGCHANGE_INT;
	stSinkRequster.u2ReqBuf[2] = 0x400 | INT_REQ_WRT;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}
#ifdef CC_MHL_3D_SUPPORT

extern WRITE_BURST_VDO_3D_T tWriteBurstVic3D[8];
extern WRITE_BURST_VDO_3D_T tWriteBurstDtd3D[2];
extern UINT8 VicWriteburstCnt ;
extern UINT8 DtdWriteburstCnt ;
static UINT8 VicWriteburstIdx =0 ;
static UINT8 DtdWriteburstIdx =0 ;
extern MHL_3D_VIC_Structuration_T t3D_VIC_Struct;
extern MHL_3D_DTD_Structuration_T t3D_DTD_Struct;

void vCbusSendWriteBurst(void)
{
	UINT8 u1Datanum;
	
	if(u1CbusMscAbortDelay2S)
		return;
	
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	if(VicWriteburstIdx < VicWriteburstCnt )
	{
		Printf("_____3d write burst___________\n");
		stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_BURST;
		stSinkRequster.u2ReqBuf[1] = 0x440;// + (VicWriteburstIdx << 4);//40~7F
		stSinkRequster.u2ReqBuf[2] = tWriteBurstVic3D[VicWriteburstIdx].BURST_ID_H| 0x400;
		stSinkRequster.u2ReqBuf[3] = tWriteBurstVic3D[VicWriteburstIdx].BURST_ID_L| 0x400;
		stSinkRequster.u2ReqBuf[4] = tWriteBurstVic3D[VicWriteburstIdx].CHECK_SUM| 0x400;
		stSinkRequster.u2ReqBuf[5] = tWriteBurstVic3D[VicWriteburstIdx].TOT_ENT| 0x400;
		stSinkRequster.u2ReqBuf[6] = tWriteBurstVic3D[VicWriteburstIdx].SEQ| 0x400;
		stSinkRequster.u2ReqBuf[7] = tWriteBurstVic3D[VicWriteburstIdx].NUM_ENT| 0x400;
		stSinkRequster.u2ReqBuf[8] = tWriteBurstVic3D[VicWriteburstIdx].VDI_0_H| 0x400;
		stSinkRequster.u2ReqBuf[9] = tWriteBurstVic3D[VicWriteburstIdx].VDI_0_L| 0x400;
		stSinkRequster.u2ReqBuf[10] = tWriteBurstVic3D[VicWriteburstIdx].VDI_1_H| 0x400;
		stSinkRequster.u2ReqBuf[11] = tWriteBurstVic3D[VicWriteburstIdx].VDI_1_L| 0x400;
		stSinkRequster.u2ReqBuf[12] = tWriteBurstVic3D[VicWriteburstIdx].VDI_2_H| 0x400;
		stSinkRequster.u2ReqBuf[13] = tWriteBurstVic3D[VicWriteburstIdx].VDI_2_L| 0x400;
		stSinkRequster.u2ReqBuf[14] = tWriteBurstVic3D[VicWriteburstIdx].VDI_3_H| 0x400;
		stSinkRequster.u2ReqBuf[15] = tWriteBurstVic3D[VicWriteburstIdx].VDI_3_L| 0x400;
		stSinkRequster.u2ReqBuf[16] = tWriteBurstVic3D[VicWriteburstIdx].VDI_4_H| 0x400;
		stSinkRequster.u2ReqBuf[17] = tWriteBurstVic3D[VicWriteburstIdx].VDI_4_L| 0x400;
		stSinkRequster.u2ReqBuf[18] = CBUS_MSC_CTRL_EOF;
		u1Datanum = (tWriteBurstVic3D[VicWriteburstIdx].NUM_ENT << 1) + 9;
		stSinkRequster.u2ReqBuf[u1Datanum-1] = CBUS_MSC_CTRL_EOF;
		stSinkRequster.u1Len =u1Datanum;
		//to do, for 3D
		VicWriteburstIdx ++;

		while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
		{
			vCbusTriggerCmpInterrupt();
			x_thread_delay(4);
		}

		return;
	}
	
	if(DtdWriteburstIdx < DtdWriteburstCnt )
	{
		stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_BURST;
		stSinkRequster.u2ReqBuf[1] = 0x440;// + (DtdWriteburstIdx << 4);//40~7F
		stSinkRequster.u2ReqBuf[2] = tWriteBurstDtd3D[DtdWriteburstIdx].BURST_ID_H| 0x400;
		stSinkRequster.u2ReqBuf[3] = tWriteBurstDtd3D[DtdWriteburstIdx].BURST_ID_L| 0x400;
		stSinkRequster.u2ReqBuf[4] = tWriteBurstDtd3D[DtdWriteburstIdx].CHECK_SUM| 0x400;
		stSinkRequster.u2ReqBuf[5] = tWriteBurstDtd3D[DtdWriteburstIdx].TOT_ENT| 0x400;
		stSinkRequster.u2ReqBuf[6] = tWriteBurstDtd3D[DtdWriteburstIdx].SEQ| 0x400;
		stSinkRequster.u2ReqBuf[7] = tWriteBurstDtd3D[DtdWriteburstIdx].NUM_ENT| 0x400;
		stSinkRequster.u2ReqBuf[8] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_0_H| 0x400;
		stSinkRequster.u2ReqBuf[9] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_0_L| 0x400;
		stSinkRequster.u2ReqBuf[10] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_1_H| 0x400;
		stSinkRequster.u2ReqBuf[11] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_1_L| 0x400;
		stSinkRequster.u2ReqBuf[12] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_2_H| 0x400;
		stSinkRequster.u2ReqBuf[13] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_2_L| 0x400;
		stSinkRequster.u2ReqBuf[14] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_3_H| 0x400;
		stSinkRequster.u2ReqBuf[15] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_3_L| 0x400;
		stSinkRequster.u2ReqBuf[16] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_4_H| 0x400;
		stSinkRequster.u2ReqBuf[17] = tWriteBurstDtd3D[DtdWriteburstIdx].VDI_4_L| 0x400;
		stSinkRequster.u2ReqBuf[18] = CBUS_MSC_CTRL_EOF;
		u1Datanum = (tWriteBurstDtd3D[DtdWriteburstIdx].NUM_ENT << 1) + 9;	
		stSinkRequster.u2ReqBuf[u1Datanum-1] = CBUS_MSC_CTRL_EOF;
		//to do for 3D
		DtdWriteburstIdx ++;
		stSinkRequster.u1Len = u1Datanum;
		while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
		{
			vCbusTriggerCmpInterrupt();
			x_thread_delay(4);
		}
		 return;
	}	
	
}
#endif
void vCbusDscrChange(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_WRITE_STATE;
	stSinkRequster.u2ReqBuf[1] = CBUS_MSC_REG_REGCHANGE_INT;
	stSinkRequster.u2ReqBuf[2] = INT_DSCR_CHG | 0x400;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}
void vCBusSetRapK(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = MHL_MSC_MSG_RAPK;
	stSinkRequster.u2ReqBuf[2] = 0x400;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void vCBusSetUcpK(void)
{
	if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = MHL_MSC_MSG_UCPK;
	stSinkRequster.u2ReqBuf[2] = 0x400 | u1ucpk[1];
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void _Mhl_LogEnqueue(MHL_MSG_T *prMsg)
{
	if (IS_MHL_LOG_Q_FULL())
    {
        return;
    }
	
    // copy to buffer first
    (void)x_memcpy(&(arMhlLogQueue[mhl_Log_write_idx]), prMsg, sizeof(MHL_MSG_T));
    // rx write pointer add one
    mhl_Log_write_idx = (mhl_Log_write_idx + 1) % MHL_LOG_QUEUE_SIZE;
}

void _Mhl_LogDequeue(MHL_MSG_T *prMsg)
{
	// check if queue is empty
    if (IS_MHL_LOG_Q_EMPTY())
    {
       return;
    }
    
    // copy to buffer first
    (void)x_memcpy(prMsg, &(arMhlLogQueue[mhl_Log_read_idx]), sizeof(MHL_MSG_T));

    mhl_Log_read_idx = (mhl_Log_read_idx + 1) % MHL_LOG_QUEUE_SIZE;
}


UINT8 u4CbusGetRxBufLen(void)
{
	return IO32ReadFldAlign(PDWNC_CBUS_STA_00,FLD_RBUF_LVL_LAT);
}

BOOL fgCBUSRxBufEmpty(void)
{
	if(u4CbusGetRxBufLen() == 0)
		return TRUE;
	else
		return FALSE;
}

BOOL fgCBUSTxBufEmpty(void)
{
	if((stCbusMscTx.tx_sate == SINK_CBUS_TX_IDLE)&&(stCbusDdcTx.tx_sate == SINK_CBUS_TX_IDLE))
		return TRUE;
	else
		return FALSE;
}

BOOL fgCbusHwTxRxIdle(void)
{
	UINT32 u4RxFSM = 0;	
	UINT32 u4TxFSM = 0;

	u4RxFSM = IO32ReadFldAlign(PDWNC_CBUS_STA_01,FLD_LINKRX_FSM);
	u4TxFSM = IO32ReadFldAlign(PDWNC_CBUS_STA_01,FLD_LINKTX_FSM);
	if((u4RxFSM == 0x00) && (u4TxFSM ==0x00))
	{
		return TRUE;
	}

	return FALSE;
}

void vCBusSetBootUpTimer(UINT8 u1Timer)
{
	stBootUpTimer.u4MscDdcTmr = u1Timer;
	stBootUpTimer.fgTmrOut =FALSE;
}

void vCBusCLrBootUpTimer(void)
{
	stBootUpTimer.u4MscDdcTmr = 0;
	stBootUpTimer.fgTmrOut =FALSE;
}

BOOL fgCBusGetBootUpTimeOut(void)
{
	return stBootUpTimer.fgTmrOut;
}

void vCbusSetTxTimer(UINT8 u1Timer)
{
	stCbusTxTimer.u4MscDdcTmr = u1Timer;
	stCbusTxTimer.fgTmrOut =FALSE;
}

void vCBusCLrTxTimer(void)
{
	stCbusTxTimer.u4MscDdcTmr = 0;
	stCbusTxTimer.fgTmrOut =FALSE;
}

BOOL fgCBusGetTxTimeOut(void)
{
	return stCbusTxTimer.fgTmrOut;
}

void vCbusSetMscTimer(UINT8 u1Timer)
{
	stCbusMscTimer.u4MscDdcTmr = u1Timer;
	stCbusMscTimer.fgTmrOut =FALSE;
}

void vCBusCLrMscTimer(void)
{
	stCbusMscTimer.u4MscDdcTmr = 0;
	stCbusMscTimer.fgTmrOut =FALSE;
}

BOOL fgCBusGetMscTimeOut(void)
{
	return stCbusMscTimer.fgTmrOut;
}

void vCbusSetDdcTimer(UINT8 u1Timer)
{
	stCbusDdcTimer.u4MscDdcTmr = u1Timer;
	stCbusDdcTimer.fgTmrOut =FALSE;
}

void vCBusCLrDdcTimer(void)
{
	stCbusDdcTimer.u4MscDdcTmr = 0;
	stCbusDdcTimer.fgTmrOut =FALSE;
}

BOOL fgCBusGetDdcTimeOut(void)
{
	return stCbusDdcTimer.fgTmrOut;
}


BOOL fgCbusTxEvent(UINT32 u2IntSta)
{
	UINT32 tmp = 0;
	
	tmp = u2IntSta & (SINK_TX_OK_INT_MASK|SINK_TX_ARB_FAIL_INT_MASK|SINK_TX_RETRY_TO_INT_MASK|SINK_TX_TRIG_FAIL_INT_MASK);
	if(tmp == 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL fgCbusTxTrigFail(UINT32 u2IntSta)
{
	if(u2IntSta & SINK_TX_TRIG_FAIL_INT_MASK)
	{
		return TRUE;
	}

	return FALSE;
}
BOOL fgCbusTxArbiFail(UINT32 u2IntSta)
{
	if(u2IntSta & SINK_TX_ARB_FAIL_INT_MASK)
	{
		return TRUE;
	}

	return FALSE;
}
BOOL fgCbusTxOK(UINT32 u2IntSta)
{

	if(u2IntSta & SINK_TX_OK_INT_MASK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL fgCbusTxErr(UINT32 u2IntSta)
{
	
	if(u2IntSta & SINK_TX_RETRY_TO_INT_MASK)
	{
		return TRUE;
	}

	return FALSE;
}


BOOL fgCbusRxErr(UINT32 u2IntSta)
{
	if(u2IntSta & SINK_LINKRX_TIMEOUT_INT_MASK)
	{
		return TRUE;
	}

	return FALSE;
}



BOOL fgCbusMscIdle(void)
{
	if(stCbusCmd.u2CbusMscMode == SINK_CBUS_STATE_IDLE)
		return TRUE;
	else
		return FALSE;
}


void vCbusSendMscMsg(UINT16 *pMscMsg, UINT8 u1MsgLen)
{
	UINT8 i=0;

	for(i=0;i< u1MsgLen;i++)
	{
		stCbusMscTx.u2TxBuf[i] = pMscMsg[i];
	}

	stCbusMscTx.u2Len = u1MsgLen;
	stCbusMscTx.tx_sate = SINK_CBUS_TX_VALID;

	if(stCbusMscTx.u2Len > SINK_TX_HW_BUF_MAX )
	{
		LOG(0,"tx buffer full\n");
	}
}

void vCbusSendDdcMsg(UINT16 *pMscMsg, UINT8 u1MsgLen)
{
	UINT8 i=0;
	

	for(i=0;i< u1MsgLen;i++)
	{
		stCbusDdcTx.u2TxBuf[i] = pMscMsg[i];
	}

	stCbusDdcTx.u2Len = u1MsgLen;
	stCbusDdcTx.tx_sate = SINK_CBUS_TX_VALID;

	if(stCbusDdcTx.u2Len > SINK_TX_HW_BUF_MAX )
	{
		LOG(0,"tx buffer full\n");
	}
	
}

static UINT8 u1CbusSendMsg(UINT16 *pMsgData, UINT8 dataSize)
{
    UINT8 i;
	HAL_TIME_T tRxTime;
	MHL_MSG_T t_Msg;

	for(i = 0; i < dataSize; i++)
	{
		HAL_GetTime(&tRxTime);
		t_Msg.u2MhlMsg = *(pMsgData+i);
		t_Msg.u1GroupId =2;// 2 indicate transmit command.
		x_memcpy(&(t_Msg.time),&tRxTime,sizeof(HAL_TIME_T));
		_Mhl_LogEnqueue(&t_Msg);

		if( i & 1) 
		{
			vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF1);
		}
		else
		{
			vIO32WriteFldAlign(PDWNC_CBUS_WBUF0 + (i>>1)*4,*(pMsgData+i),FLD_CBUS_WBUF0);
		}	

	}
   	
    vIO32WriteFldAlign(PDWNC_CBUS_LINK_00,dataSize,FLD_TX_NUM);	
    CBUS_TRIGGER_TX_HW();

    return 0;
}

void vCbusRstDdcState(void);
void vCbusRstMscState(void);

void vCbusStateReset(void)
{
	iMhlStateOld = MHL_STATE_NONE;
	iMhlStateNew = MHL_STATE_NONE;
	
	iDdcOffset = 0;

	au1Ri1[0] = 0; 
	au1Ri1[1] = 0;//au1Ri1[2] ={0, 0};
	u1Bcaps = 0;
	au1Bstatus[0]=0; 
	au1Bstatus[1] = 0;//au1Bstatus[2] ={0x0, 0x10}; 
	
	u1_rapk = 0;
    u1ucpk[0] = 0;
	u1ucpk[1] = 0;
	
	u1AnReady = 0;
	
	u1CbusMscAbortDelay2S =0;
	u1CbusDdcAbortDelay2S =0;
		
	u1CbusMscErrCode =0;
	u1CbusDdcErrCode =0;
	
	mhl_Log_read_idx =0;
    mhl_Log_write_idx =0;
	#ifdef CC_MHL_3D_SUPPORT
	VicWriteburstIdx = 0;
	DtdWriteburstIdx = 0;
	#endif
	stSinkRequster.req_state = SINK_CBUS_REQ_IDLE;
	stCbusMscTx.tx_sate = SINK_CBUS_TX_IDLE;
	stCbusDdcTx.tx_sate = SINK_CBUS_TX_IDLE;
	stCbusCmd.u2CbusMscMode = SINK_CBUS_STATE_IDLE;

	vCbusRstDdcState();
	vCbusRstMscState();
	vCBusSetDiscoveryNG();
	memset((void *)&stSinkCtr,0,sizeof(stSinkCtr));
	
 
}
void _vPatchForHPDCTSFail(void)
{
        if((!u1GetCdsenseStatus()) 
			&& bHDMIPort5VStatus(HDMI_SWITCH_1)
			&& (eResist_state!= Resister_PullLow))
        {   
                eResist_state =Resister_PullLow;
				vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HPD);
        }
        else
        {
			if((!u1GetCdsenseStatus())
				&&(bHDMIPort5VStatus(HDMI_SWITCH_1)==0)
				&& (eResist_state != Resister_NotPullLow))
	        {
	                eResist_state = Resister_NotPullLow;
					vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);
	        }
        }
}

void vCbusMainLoop(void)
{
	UINT32 u4WakeUpReason;
	
	if (iMhlStateOld != iMhlStateNew)
	{
		LOG(6,"iMhlState %s --> %s \n,",_aszMhlState[iMhlStateOld],_aszMhlState[iMhlStateNew]);
		iMhlStateOld = iMhlStateNew;
	}
	
	switch (iMhlStateOld)
	{
		case MHL_STATE_NONE:
				iMhlStateNew = MHL_STATE_INIT;
		break;
		case MHL_STATE_INIT:
			
			u1HandShakingCnt=0;
			vCbusStateReset();
			vCbusDisable();
			vCbusRst();
			vCbus_DisableInterrupt();
			vCbusClrStatusAll();
			
			if(u1GetCdsenseStatus() && u1BootUp)
			{	
			   iMhlStateNew = MHL_STATE_BOOTUP;
			}
			else
			{
			   iMhlStateNew = MHL_STATE_WAIT_CABLE_CONNECT;  
			   vCBusEnHwMode(0);
			   //vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);//4.3.23.2
			}
			
		break;
		case MHL_STATE_BOOTUP:

			if(u1BootUp)
			{
				u1BootUp =0;
				u4WakeUpReason = PDWNC_ReadWakeupReason();
				LOG(6, "u4WakeUpReason = %d.\n", u4WakeUpReason);
				if((u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_AC_POWER) || 
				  (u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_WATCHDOG))
				{
				   vCbusSetImpedance(SINK_CBUS_IMPEDANCE_1K);
				   vCBusSetBootUpTimer(MHL_BOOT_UP_TIMER1_RERIOD);
				}
				else if(u4WakeUpReason == (UINT32)PDWNC_WAKE_UP_REASON_IRRC)
				{
					vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);  
					vCBusSetBootUpTimer(MHL_BOOT_UP_TIMER2_RERIOD);//for disable wakeup pulse detect so add the delay
				}
				else
				{
				   vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);	
				   vCBusSetBootUpTimer(MHL_BOOT_UP_TIMER_RERIOD);
				}
				vCbusSetVbus(TRUE);
				vMHLOpenRxTermination(0);
			}
			
			break;
			
		case MHL_STATE_WAIT_CABLE_CONNECT:	
			_bIsMhlDevice =0;
			if (fgCbusGetDeviceAttach() == TRUE)//cable detect int
			{
				vCbusSetImpedance(SINK_CBUS_IMPEDANCE_1K);
				vCbusSetVbus(TRUE);
				
				//vCbusEnDiscoveryDetect(TRUE);
				vCbusEnWakeUpDetect();
				iMhlStateNew = MHL_STATE_DISCOVERY;
				_bIsMhlDevice = 1;			
			}
			else
			{
				_vPatchForHPDCTSFail();// No MHL Cable Connected.
			}
			break;

		case MHL_STATE_DISCOVERY:
			//working at cbus state			   
			vMHLOpenRxTermination(0);

			if(fgCbusGetDeviceAttach() == FALSE)
			{
				iMhlStateNew = MHL_STATE_INIT; 
				_bIsMhlDevice = 0;
				_bIsMhlPPMode = 0;
				vCbusSetVbus(FALSE);
				LOG(0,"CABLE DISCONNECT @  MHL_STATE_DISCOVERY \n"); 
			}
			break;

		case MHL_STATE_CONNECTED:

			if(fgCbusGetDeviceAttach() == FALSE)
			{
				iMhlStateNew = MHL_STATE_INIT; 
				_bIsMhlDevice = 0;
				_bIsMhlPPMode = 0;
				vCbusSetVbus(FALSE);
				LOG(0,"CABLE DISCONNECT @  MHL_STATE_CONNECTED \n"); 
			}
			
			if(fgCBUSGetDiscoveryOK() &&(u1HandShakingCnt <= 3))
			{
				if(u1HandShakingCnt == 0)
				{
					stSinkCtr.skIsCRdy.u1DcapRdy =1;
				}
				else if(u1HandShakingCnt == 1)
				{
					stSinkCtr.skIsRChg.u1DcpChg  =1;
				}
				else if(u1HandShakingCnt == 2)
				{
					stSinkCtr.skIsLMod.u1PathEn =1;
				}
				else if(u1HandShakingCnt == 3)
				{
					stSinkCtr.skIsHpd.u1Hpd = 1;
					stSinkCtr.skIsHpd.u1SetClrHpd = 1;
				}
				
			}
			if(u1FlgHdmiMhlSrcChged)
			{
				if(stSinkCtr.skIsRap.u1RapPol == 0)
				{
					stSinkCtr.skIsRap.u1RapPol = 1;
				}
			}
			break;
		default:
			break;
	}
}

UINT8 vCbusGetMHLState(void)
{
	return iMhlStateOld;
}
void vCbusCommandLoop(void)
{

	MHL_MSG_T rRxMsg;

	//printf cbus log
	if(bCbusState == 1)
    {
		while(!IS_MHL_LOG_Q_EMPTY())
		{
			x_memset(&rRxMsg, 0, sizeof(MHL_MSG_T));
			_Mhl_LogDequeue(&rRxMsg);
			
			if(u1DebugLvl == 1)
			{
				if(rRxMsg.u1GroupId == 1)
				{
					LOG(0," >> rx: [%d,%d] data: ",rRxMsg.time.u4Seconds, rRxMsg.time.u4Micros);					   
				}
				else if(rRxMsg.u1GroupId == 2)
				{
					LOG(0," << tx: [%d,%d] data: ",rRxMsg.time.u4Seconds, rRxMsg.time.u4Micros);	
				}
				
				LOG(0,"0x%x \n",rRxMsg.u2MhlMsg);
			}
			
		}

    }	
	
	if(vCbusGetMHLState() == MHL_STATE_CONNECTED)
	{
		
		
		if(stSinkCtr.skIsCRdy.u1DcapRdy)
		{
			vCbusSetDevCapReady();
			stSinkCtr.skIsCRdy.u1DcapRdy =0;
		}

		if(stSinkCtr.skIsRChg.u1DcpChg)
		{
			vCbusSetDevCapChange();
			stSinkCtr.skIsRChg.u1DcpChg=0;
		}

		if(stSinkCtr.skIsLMod.u1PathEn)
		{
			vCbusSetPathEn();
			stSinkCtr.skIsLMod.u1PathEn = 0;
		}
		
		if(stSinkCtr.skIsHpd.u1Hpd)
		{
			if(stSinkCtr.skIsHpd.u1SetClrHpd)
			{
				vCbusSetHpd();
			}
			else
			{
				vCbusClrHpd();
			}
			
			stSinkCtr.skIsHpd.u1Hpd = 0;
			stSinkCtr.skIsHpd.u1SetClrHpd =0;
			
		}
		if(stSinkCtr.skIsRap.u1RapPol == 1)
		{
			stSinkCtr.skIsRap.u1RapPol = 0;
			vCbusSendRapKeys(0x00);
			u1FlgHdmiMhlSrcChged = 0;//@bo.zhao add for 4.3.17.2@
		}
		if(stSinkCtr.skIsRChg.u1GrtWrt)
		{
			vCbusGratWriteBurst();
			stSinkCtr.skIsRChg.u1GrtWrt =0;
		}
		else if(stSinkCtr.skIsRChg.u13DQeq)
		{
			stSinkCtr.skIsRChg.u13DQeq =0;
			vCbusRequestWriteBurst();
		}
		else if(stSinkCtr.skIsSendWriteBurst == 1)
		{
		#ifdef CC_MHL_3D_SUPPORT
			Printf("___-send write burst,VicWriteburstIdx =%d,VicWriteburstCnt=%d,DtdWriteburstIdx=%d,DtdWriteburstCnt=%d_____________\n",VicWriteburstIdx,VicWriteburstCnt,DtdWriteburstIdx,DtdWriteburstCnt);
			vCbusSendWriteBurst();
		#endif
			
			stSinkCtr.skIsSendWriteBurst =0;
		}
		else if(stSinkCtr.skIsRChg.u1DscrChg)
		{
			vCbusDscrChange();
		#ifdef CC_MHL_3D_SUPPORT
			stSinkCtr.skIsRChg.u1DscrChg =0;
			if((VicWriteburstIdx < VicWriteburstCnt) || (DtdWriteburstIdx < DtdWriteburstCnt ))
			{
				
				stSinkCtr.skIsSendWriteBurst =2;
			}
			else
			{
				VicWriteburstIdx = 0;
				DtdWriteburstIdx = 0;
			}
		#endif
		}
		else if(stSinkCtr.skIsSendWriteBurst == 2)
		{
			Printf("____write burst again_________________\n");
			vCbusRequestWriteBurst();
			stSinkCtr.skIsSendWriteBurst =0;
		}
	}

    if(u1_rapk ==1)
    {
    	u1_rapk =0;
    	vCBusSetRapK();
    }

	if(u1ucpk[0] ==1)
	{
		u1ucpk[0] =0;
		vCBusSetUcpK();
	}
	
}

static void _Cbus_Thread( void * pvArgs )
{
    UNUSED(pvArgs);
    while(1)
    {
    
		VERIFY( x_sema_lock( _hCbusSema, X_SEMA_OPTION_WAIT) == OSR_OK);
		
		if(bCbusState == 1)
		{
		    vCbusMainLoop();		
		}
    }
}
#ifdef SYS_MHL_SUPPORT
#ifdef CBUS_OC_ADIN_USING
static void _Mhl_OCP_Thread( void * pvArgs )
{
    UNUSED(pvArgs);
    while(1)
    {
    
		VERIFY( x_sema_lock( _hMhlOcpSema, X_SEMA_OPTION_WAIT) == OSR_OK);
		
		if(bCbusState == 1)
		{
		    vOcpHandler();		
		}
    }
}
#endif
#endif
static void _Cbus_Command_Thread( void * pvArgs )
{
    UNUSED(pvArgs);
    while(1)
    {
    
		VERIFY( x_sema_lock( _hCbusCommandSema, X_SEMA_OPTION_WAIT) == OSR_OK);
		
		if(bCbusState == 1)
		{
		    vCbusCommandLoop();		
		}
    }
}

void vCbusInit(void)
{
    x_os_isr_fct pfnOldIsr;

    if(bInitFlg == 0)
    {
        bInitFlg = 1;
	    vCbus_ValInit();
	    vCbus_HwInit(); 
		VERIFY(x_timer_create(&_hMhlTimer) == OSR_OK);
		VERIFY( x_sema_create(&_hCbusSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
	    VERIFY(x_thread_create(&_hCbusThreadHdl,"CbusThread", 2048,  60, _Cbus_Thread, 0, NULL) == OSR_OK);
		VERIFY( x_sema_create(&_hCbusCommandSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
		VERIFY(x_thread_create(&_hCbusCommandHdl,"CbusCommandThread", 2048,  61, _Cbus_Command_Thread, 0, NULL) == OSR_OK);
#ifdef SYS_MHL_SUPPORT
#ifdef CBUS_OC_ADIN_USING
		VERIFY( x_sema_create(&_hMhlOcpSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
	    VERIFY(x_thread_create(&_hMhlOcpThreadHdl,"MhlOCP", 2048,  60, _Mhl_OCP_Thread, 0, NULL) == OSR_OK);
#endif
#endif
		VERIFY(PDWNC_RegIsr(PDWNC_INTNO_CBUS, _Cbus_Interrupt, &pfnOldIsr) == PDWNC_OK);
		VERIFY(x_timer_create(&_hMhlCbusCommandTimer) == OSR_OK);
		VERIFY(x_timer_start(_hMhlTimer,MHL_TIMER_PERIOD,X_TIMER_FLAG_REPEAT,_Cbus_MhlTimerHandle,NULL) == OSR_OK);
    }
}

BOOL fgCbusMscAck(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_MSC_CTRL_ACK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL fgCbusMscNack(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_MSC_CTRL_NACK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL fgCbusMscAbort(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_MSC_CTRL_ABORT)
	{
		return TRUE;
	}

	return FALSE;
}

static BOOL fgCbusValidMscCmd(UINT16 u2RxMsg)
{
	if((u2RxMsg == CBUS_MSC_CTRL_GET_STATE)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_VENDER_ID)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_DDC_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_SC3_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_MSC_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_SC1_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_WRITE_STATE)
	|| (u2RxMsg == CBUS_MSC_CTRL_READ_DEVCAP)
	|| (u2RxMsg == CBUS_MSC_CTRL_MSC_MSG)
	|| (u2RxMsg == CBUS_MSC_CTRL_WRITE_BURST))
	{
		return TRUE;
	}

	return FALSE;
}
#if 0
static BOOL fgCbusMscOpInvalid(UINT16 u2data)
{
	if((u2data & 0x100) == 0)	return FALSE;
	
	if((u2data == CBUS_MSC_CTRL_ACK)
		||(u2data == CBUS_MSC_CTRL_ABORT)
		||(u2data == CBUS_MSC_CTRL_NACK)
		||(u2data == CBUS_MSC_CTRL_WRITE_STATE)
		||(u2data == CBUS_MSC_CTRL_READ_DEVCAP)
		||(u2data == CBUS_MSC_CTRL_GET_STATE)
		||(u2data == CBUS_MSC_CTRL_GET_VENDER_ID)
		||(u2data == CBUS_MSC_CTRL_SET_HPD)
		||(u2data == CBUS_MSC_CTRL_CLR_HPD)
		||(u2data == CBUS_MSC_CTRL_MSC_MSG)
		||(u2data == CBUS_MSC_CTRL_GET_SC1_EC)
		||(u2data == CBUS_MSC_CTRL_GET_DDC_EC)
		||(u2data == CBUS_MSC_CTRL_GET_MSC_EC)
		||(u2data == CBUS_MSC_CTRL_WRITE_BURST)
		||(u2data == CBUS_MSC_CTRL_GET_SC3_EC)
		||(u2data == CBUS_MSC_CTRL_EOF)
	)
		return TRUE;
	else
		return FALSE;
}
#endif

BOOL fgCbusMscDate(UINT16 u2Data)
{
	if((u2Data & 0x700) ==0x400)
	{
		return TRUE;
	}
	return FALSE;
}

UINT8 vCbusGetMscData(UINT16 u2Data)
{
	return u2Data & 0xff;
}

void vCbusMscResAbort(void)
{
	UINT16 u2Data[1];
	u2Data[0] = CBUS_MSC_CTRL_ABORT;
	vCbusSendMscMsg(u2Data,1);
}

void vCbusMscResAck(void)
{
	UINT16 u2Data[1];
	u2Data[0] = CBUS_MSC_CTRL_ACK;
	vCbusSendMscMsg(u2Data,1);
}


static void _Cbus_MscErrorHandling(UINT8 u1ErrorCode)
{
	vCbusMscResAbort();//6.3.6.2
	u1CbusMscErrCode = u1ErrorCode;
	u1CbusMscAbortDelay2S = MHL_MSC_ABORT_DELAY;
   	LOG(0,"MSC message error abort: 0x%x \n", u1ErrorCode);
}

void vCbusRstMscState(void)
{
	stCbusCmd.u2RXBufIndex = 0;
	stCbusCmd.stReqMsc.u1MscSta = SINK_CBUS_STATE_NONE;
	stCbusCmd.stResMsc.u1MscSta= SINK_CBUS_STATE_NONE;
	stCbusCmd.u2CbusMscMode = SINK_CBUS_STATE_IDLE;
	vCBusCLrMscTimer();
}

void vCBusHandleReqAckCmd(UINT16 u2RxMsg)
{
	if(fgCbusMscAck(u2RxMsg))
	{
		//set path en
		if(stCbusCmd.stReqMsc.u2MscCmd == CBUS_MSC_CTRL_SET_HPD)
		{
			u1HandShakingCnt ++;
		}
		if(stCbusCmd.stReqMsc.u2MscCmd == CBUS_MSC_CTRL_WRITE_STATE)
		{
			u1HandShakingCnt ++;
		}

		if(stCbusCmd.stReqMsc.u2MscCmd == CBUS_MSC_CTRL_WRITE_BURST)
		{
			stSinkCtr.skIsRChg.u1DscrChg =1;
		}
	}
	else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
	{
		
	}
	else
	{
		_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
	}
}
#if 0
void vCBusHandleReqAckCmd(UINT16 u2RxMsg)
{
	if(fgCbusMscAck(u2RxMsg))
	{
		//set path en
		if(stCbusCmd.stReqMsc.u2MscCmd == CBUS_MSC_CTRL_SET_HPD)
		{
			stSinkCtr.skIsCRdy.u1DcapRdy =1;
			stSinkCtr.skIsRChg.u1DcpChg  =1;
			stSinkCtr.skIsLMod.u1PathEn =1;
		}

	}
	else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
	{
		
	}
	else
	{
		_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
	}
}
#endif

UINT8 vCBusHandleReqDataCmd(UINT16 u2RxMsg)
{
	UINT8 tmp =0;
	if(fgCbusMscDate(u2RxMsg))
	{
		tmp = vCbusGetMscData(u2RxMsg);
	}
	else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
	{
		
	}
	else
	{
		_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
	}

	return tmp;
}
static UINT16 ReadDevCapOffset =0;

UINT8 vCBusHandleReqAckDataCmd(UINT16 u2RxMsg)
{
	switch(stCbusCmd.stReqMsc.u1MscSta)
	{
		case SINK_CBUS_STATE_NONE:
			if(fgCbusMscAck(u2RxMsg))
			{
				stCbusCmd.stReqMsc.u1MscSta = SINK_CBUS_STATE_OFFSET;
				vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
			}
			break;
		case SINK_CBUS_STATE_OFFSET:
			if(fgCbusMscDate(u2RxMsg))
			{
				_arMhlSrcDevCap[ReadDevCapOffset] = vCbusGetMscData(u2RxMsg);
				ReadDevCapOffset ++;
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
			}
			break;
		default:
			break;
	}

	return 0;
	
}


void vCBusResGetStateCmd(void)
{
	UINT16 u2Data = 0x400;

	vCbusSendMscMsg(&u2Data,1);
	vCbusRstMscState();
}

void vCBusResVendorIdCmd(void)
{
	UINT16 u2Data = 0x400;

	vCbusSendMscMsg(&u2Data,1);
	vCbusRstMscState();
}

void vCBusResMscEcCmd(void)
{
	UINT16 u2Data = 0x400;
	u2Data |= u1CbusMscErrCode;

	vCbusSendMscMsg(&u2Data,1);
}

void vCBusResSc3ErrCmd(void)
{
	UINT16 u2Data = 0x400;
	vCbusSendMscMsg(&u2Data,1);
}

void vCBusResDdcEcCmd(void)
{
	UINT16 u2Data = 0x400;

	vCbusSendMscMsg(&u2Data,1);
}

void vCBusResSc1EcCmd(void)
{
	UINT16 u2Data = 0x400;

	vCbusSendMscMsg(&u2Data,1);
	vCbusRstMscState();
}

void vCBusResWriteStateCmd(UINT16 u2RxMsg)
{
	UINT16 u1Data0;
	UINT16 u1Data1;
	
	switch(stCbusCmd.stResMsc.u1MscSta)
	{
		case SINK_CBUS_STATE_NONE:
			stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_OFFSET;
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			break;
		case SINK_CBUS_STATE_OFFSET:
			if(fgCbusMscDate(u2RxMsg))
			{
				if((u2RxMsg >= MIN_INT_REG_OFFSET) && ((u2RxMsg <= MAX_STAT_REG_OFFSET)))
				{
					stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_DATA;
					stCbusCmd.u2RXBuf[1] = u2RxMsg;
					vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
				}
				else
				{
					_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
					vCbusRstMscState();
				}
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
			
			break;
		case SINK_CBUS_STATE_DATA:
			if(fgCbusMscDate(u2RxMsg))
			{
				u1Data0 = stCbusCmd.u2RXBuf[1];
				u1Data1 = u2RxMsg;

				if(u1Data0 == CBUS_MSC_REG_READY_BITS)
				{
					if(u1Data1 & 0x1) //Dcap_rdy
					{
						//stSinkCtr.skIsCRdy.
					}
				}
				else if( u1Data0 == CBUS_MSC_REG_ACTIVE_LINK_MODE)
				{
					if((u1Data1 & 0x7) == 0x2)
					{
						_bIsMhlPPMode = 1;
					}
					else
					{
						// 24bit mode
						_bIsMhlPPMode = 0;
					}

					if(u1Data1 & 0x8)	
					{
						//path en
					}
					else
					{
						//not path en
					}

					if(u1Data1 & 0x10)
					{
						//mute
					}
					else
					{
						//unmute
					}
					
				}
				else if(u1Data0 == CBUS_MSC_REG_REGCHANGE_INT)
				{
					 //to do
					 if(u1Data1 & INT_DCAP_CHG)//DCAP_CHG
					 {	
					 }
					 else
					 if(u1Data1 & INT_REQ_WRT)
					 {
					 	stSinkCtr.skIsRChg.u1GrtWrt =1;
						
					 }
					 else
					 if(u1Data1 & INT_DSCR_CHG)
					 {
					    //write burst finished

					 }
					 else
					 if(u1Data1 & INT_GRT_WRT)
					 {
				 		//write burst finished
						stSinkCtr.skIsSendWriteBurst =1;
					 }						
					 else
					 if(u1Data1 & INT_3D_REQ)
					 {
						stSinkCtr.skIsRChg.u13DQeq =1;
					 }
				}
				else if(u1Data0 == CBUS_MSC_REG_DDCHANGE_INT)
				{
					
				}
				vCbusMscResAck();
				vCbusRstMscState();
				
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();

			}
			
			break;
		default:
			break;
	}
}

void vCBusResWriteBurstCmd(UINT16 u2RxMsg)
{
	switch(stCbusCmd.stResMsc.u1MscSta)
	{
		case SINK_CBUS_STATE_NONE:
			stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_OFFSET;
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			break;
		case SINK_CBUS_STATE_OFFSET:
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			if(fgCbusMscDate(u2RxMsg) && ((u2RxMsg < MIN_SCRACHPAD_REG_OFFSET) || (u2RxMsg > MAX_SCRACHPAD_REG_OFFSET)))
			{
				_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
				vCbusRstMscState();
			}
			else if(fgCbusMscDate(u2RxMsg))
			{
				stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_DATA;
				stCbusCmd.u2RXBuf[1] = u2RxMsg &0xff;
				stCbusCmd.u2RXBufIndex = 0;
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
			
			break;
		case SINK_CBUS_STATE_DATA:
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			if(u2RxMsg == CBUS_MSC_CTRL_EOF)
			{
				if((stCbusCmd.u2RXBufIndex <=2) || (stCbusCmd.u2RXBufIndex - 2 + 0x40) > ((MAX_SCRACHPAD_REG_OFFSET & 0xff)))
				{
					_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				}
				else
				{
					if(stCbusCmd.u2RXBufIndex > 2)
	                {
#ifdef SYS_MHL_SUPPORT	                
					    Mhl_NFYWriteBurstData(0, ((UINT16 *)(stCbusCmd.u2RXBuf + 2)), stCbusCmd.u2RXBufIndex);
#endif
				    }

					vCbusMscResAck();
				}

				vCbusRstMscState();
			}
			else if(fgCbusMscDate(u2RxMsg))
			{
				if(((stCbusCmd.u2RXBuf[1] + stCbusCmd.u2RXBufIndex) > ((MAX_SCRACHPAD_REG_OFFSET & 0xff) + 2))
				||(stCbusCmd.u2RXBufIndex > (FIELD_DEV_SCRATCHPAD_SIZE - 1)))
				{
					//abort
					_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
				}
				else
				{		        
					stCbusCmd.u2RXBuf[ 2 + stCbusCmd.u2RXBufIndex] = u2RxMsg & 0xff;
					stCbusCmd.u2RXBufIndex ++;
				}
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
			
			break;
		default:
			break;
	}
}

void vCBusResMscMsgCmd(UINT16 u2RxMsg)
{
	UINT16 u1Data0;
	UINT16 u1Data1;
	UINT16 u2tmp =0;
	
	switch(stCbusCmd.stResMsc.u1MscSta)
	{
		case SINK_CBUS_STATE_NONE:
			stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_OFFSET;
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			break;
		case SINK_CBUS_STATE_OFFSET:
			if(fgCbusMscDate(u2RxMsg))
			{
				stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_DATA;
				stCbusCmd.u2RXBuf[1] = u2RxMsg;
				vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
			
			break;
		case SINK_CBUS_STATE_DATA:
			if(fgCbusMscDate(u2RxMsg))
			{
				u1Data0 = stCbusCmd.u2RXBuf[1];
				u1Data1 = u2RxMsg;

				if(u1Data0 == MHL_MSC_MSG_RAP)
				{
					u1_rapk =1;
					if(u1Data1 == MHL_RAP_CMD_CONTENTON)
					{
						vMHLSETRAPContentOnoff(1);
					}
					else if(u1Data1 == MHL_RAP_CMD_CONTENTOFF)
					{
						vMHLSETRAPContentOnoff(0);
					}
								       
#ifdef SYS_MHL_SUPPORT					       
					Mhl_NFYRAP((UINT8)(u1Data1 & 0xff));
#endif					   
				}
				else
				if(u1Data0 == MHL_MSC_MSG_RAPK) 
				{
#ifdef SYS_MHL_SUPPORT
				   Mhl_NFYRAPK((UINT8)(u1Data1 & 0xff));
#endif					   					       
				}
				else
				if(u1Data0 == MHL_MSC_MSG_RCP)
				{
				   //check key code
                  u1isRcpRev = 1;
				  u1RevRcpKey = (UINT8)(u1Data1 & 0xff);							       
				}
				else
				if(u1Data0 == MHL_MSC_MSG_RCPK)
				{
#ifdef SYS_MHL_SUPPORT
				  Mhl_NFYRcpK((UINT8)(u1Data1 & 0xff));
#endif		
				}
				else
				if(u1Data0 == MHL_MSC_MSG_RCPE)
				{
#ifdef SYS_MHL_SUPPORT
				 Mhl_NFYRcpE((UINT8)(u1Data1 & 0xff));
#endif		
				}
				else
				if(u1Data0 == MHL_MSC_MSG_UCP)
				{
					u1ucpk[0] = 1;
					u1ucpk[1] = u1Data1;
					
#ifdef SYS_MHL_SUPPORT
					 Mhl_NFYUcp((UINT8)(u1Data1 & 0xff));
#endif		
				}					  
				else
				if(u1Data0 == MHL_MSC_MSG_UCPK)
				{
#ifdef SYS_MHL_SUPPORT
				   Mhl_NFYUcpK((UINT8)(u1Data1 & 0xff));
#endif							  
				}
				else
				if(u1Data0 == MHL_MSC_MSG_UCPE)
				{
#ifdef SYS_MHL_SUPPORT
				   Mhl_NFYUcpE((UINT8)(u1Data1 & 0xff));
#endif							  
				}	

				u2tmp = CBUS_MSC_CTRL_ACK;
				vCbusSendMscMsg(&u2tmp,1);
				vCbusRstMscState();
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
	
			break;
		default:
			break;
	}
}

void vCBusResReadDevCapCmd(UINT16 u2RxMsg)
{
	UINT16 u1Data[2];
	
	switch(stCbusCmd.stResMsc.u1MscSta)
	{
		case SINK_CBUS_STATE_NONE:
			stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_OFFSET;
			vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			break;
		case SINK_CBUS_STATE_OFFSET:
			if(fgCbusMscDate(u2RxMsg))
			{
				stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_DATA;
				stCbusCmd.u2RXBuf[1] = u2RxMsg;
				vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);

				if(vCbusGetMscData(u2RxMsg) > 0xf)
				{
					_Cbus_MscErrorHandling(MHL_MSC_BAD_OFFSET);
				}
				else
				{
					u1Data[0] = CBUS_MSC_CTRL_ACK;
					u1Data[1] = _arMhlDevCap[vCbusGetMscData(u2RxMsg)] | 0x400;
					vCbusSendMscMsg(u1Data,2);
				}
				vCbusRstMscState();
			}
			else if(fgCbusMscNack(u2RxMsg)||fgCbusMscAbort(u2RxMsg))
			{
				vCbusRstMscState();
			}
			else
			{
				_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
				vCbusRstMscState();
			}
			
			break;
		default:
			break;
	}
}



static void vCbusMSCloop(UINT16 u2RxMsg)
{
	//6.3.6.5
	if(fgCbusMscAbort(u2RxMsg))
	{
		u1CbusMscAbortDelay2S = MHL_MSC_ABORT_DELAY;
	}
	switch(stCbusCmd.u2CbusMscMode)
	{
		case SINK_CBUS_STATE_IDLE:
			if(!fgCbusMscAbort(u2RxMsg))
			{
				if(fgCbusValidMscCmd(u2RxMsg))
				{
					stCbusCmd.u2CbusMscMode = SINK_CBUS_STATE_RESPONDER;
					stCbusCmd.stResMsc.u1MscSta = SINK_CBUS_STATE_NONE;
					stCbusCmd.stResMsc.u2MscCmd = u2RxMsg;
					stCbusCmd.u2RXBuf[0] = u2RxMsg;// To SINK_CBUS_STATE_RESPONDER
				}
				else
				{
					_Cbus_MscErrorHandling(MHL_MSC_BAD_OPCODE);
#if 0
					if(u2RxMsg != NONE_PACKET)
					{
						if(fgCbusMscOpInvalid(u2RxMsg))
						{
							_Cbus_MscErrorHandling(MHL_MSC_BAD_OPCODE);
						}
						else
						{
							_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
						}
					}
					else
					{
						_Cbus_MscErrorHandling(MHL_MSC_PROTOCOL_ERR);
					}
#endif
					break;// end of case
				}
			}
			else
			{
				break;// end of case
			}
		case SINK_CBUS_STATE_RESPONDER:
			switch(stCbusCmd.stResMsc.u2MscCmd)
			{
				case CBUS_MSC_CTRL_WRITE_STATE:
					vCBusResWriteStateCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_READ_DEVCAP:
					vCBusResReadDevCapCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_WRITE_BURST:
					vCBusResWriteBurstCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_MSC_MSG:
					vCBusResMscMsgCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_GET_STATE:
					vCBusResGetStateCmd();
					break;
				case CBUS_MSC_CTRL_GET_VENDER_ID:
					vCBusResVendorIdCmd();
					break;
				case CBUS_MSC_CTRL_GET_MSC_EC:
					vCBusResMscEcCmd();
					vCbusRstMscState();
					break;
				case CBUS_MSC_CTRL_GET_SC3_EC:
					vCBusResSc3ErrCmd();
					vCbusRstMscState();
				case CBUS_MSC_CTRL_GET_DDC_EC:
					vCBusResDdcEcCmd();
					vCbusRstMscState();
					break;
				case CBUS_MSC_CTRL_GET_SC1_EC:
					vCBusResSc1EcCmd();
					break;
				default:
					break;
			}
			break;
		case SINK_CBUS_STATE_REQUESTER:	
			switch(stCbusCmd.stReqMsc.u2MscCmd)
			{
				case CBUS_MSC_CTRL_SET_HPD:
				case CBUS_MSC_CTRL_CLR_HPD:
				case CBUS_MSC_CTRL_WRITE_STATE:
				case CBUS_MSC_CTRL_MSC_MSG:
				case CBUS_MSC_CTRL_WRITE_BURST:
					vCBusHandleReqAckCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_READ_DEVCAP:
					vCBusHandleReqAckDataCmd(u2RxMsg);
					break;
				case CBUS_MSC_CTRL_GET_STATE:
				case CBUS_MSC_CTRL_GET_VENDER_ID:
				case CBUS_MSC_CTRL_GET_MSC_EC:
					vCBusHandleReqDataCmd(u2RxMsg);
					break;
				default:
					break;
			}
			vCbusRstMscState();
			break;
		default:
			break;
	}
}


BOOL fgCbusDdcAck(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_DDC_CTRL_ACK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL fgCbusDdcNack(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_DDC_CTRL_NACK)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL fgCbusDdcAbort(UINT16 u2RxMsg)
{
	if(u2RxMsg == CBUS_DDC_CTRL_ABORT)
	{
		return TRUE;
	}

	return FALSE;
}

#if 0
static BOOL fgCbusValidMscCmd(UINT16 u2RxMsg)
{
	if((u2RxMsg == CBUS_MSC_CTRL_GET_STATE)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_VENDER_ID)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_DDC_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_SC3_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_MSC_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_GET_SC1_EC)
	|| (u2RxMsg == CBUS_MSC_CTRL_WRITE_STATE)
	|| (u2RxMsg == CBUS_MSC_CTRL_READ_DEVCAP)
	|| (u2RxMsg == CBUS_MSC_CTRL_MSC_MSG)
	|| (u2RxMsg == CBUS_MSC_CTRL_WRITE_BURST))
	{
		return TRUE;
	}

	return FALSE;
}

static BOOL fgCbusMscOpInvalid(UINT16 u2data)
{
	if((u2data & 0x100) == 0)	return FALSE;
	
	if((u2data == CBUS_MSC_CTRL_ACK)
		||(u2data == CBUS_MSC_CTRL_ABORT)
		||(u2data == CBUS_MSC_CTRL_NACK)
		||(u2data == CBUS_MSC_CTRL_WRITE_STATE)
		||(u2data == CBUS_MSC_CTRL_READ_DEVCAP)
		||(u2data == CBUS_MSC_CTRL_GET_STATE)
		||(u2data == CBUS_MSC_CTRL_GET_VENDER_ID)
		||(u2data == CBUS_MSC_CTRL_SET_HPD)
		||(u2data == CBUS_MSC_CTRL_CLR_HPD)
		||(u2data == CBUS_MSC_CTRL_MSC_MSG)
		||(u2data == CBUS_MSC_CTRL_GET_SC1_EC)
		||(u2data == CBUS_MSC_CTRL_GET_DDC_EC)
		||(u2data == CBUS_MSC_CTRL_GET_MSC_EC)
		||(u2data == CBUS_MSC_CTRL_WRITE_BURST)
		||(u2data == CBUS_MSC_CTRL_GET_SC3_EC)
		||(u2data == CBUS_MSC_CTRL_EOF)
	)
		return FALSE;
	else
		return TRUE;
}


BOOL fgCbusMscDate(UINT16 u2Data)
{
	if(u2Data & 0x700 ==0x400)
	{
		return TRUE;
	}
	return FALSE;
}

UINT8 vCbusGetMscData(UINT16 u2Data)
{
	return u2Data & 0xff;
}
#endif
void vCbusDdcResAck(void)
{
	UINT16 u2Data[1];
	u2Data[0] = CBUS_DDC_CTRL_ACK;
	vCbusSendDdcMsg(u2Data,1);
	//u1CbusSendMsg(u2Data,1);
}

void vCBusDdcResNack(void)
{
	UINT16 u2Data[1];
	u2Data[0] = CBUS_DDC_CTRL_NACK;
	vCbusSendDdcMsg(u2Data,1);
	//u1CbusSendMsg(u2Data,1);
	
}

BOOL fgCbusDdcDate(UINT16 u2Data)
{
	if((u2Data & 0x700) ==0x0)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL fgCbusValidHdcpOffset(UINT8 u1Offset)
{
	if( (u1Offset == CBUS_DDC_DATA_HDCP_BKSV_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_RI1_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_AKSV_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_AINFO_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_AN_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_BCAPS_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_BCAPS_OFFSET)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_VH0) 
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_VH1)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_VH2)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_VH3)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_VH4)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_KSVFIFO)
	  ||(u1Offset == CBUS_DDC_DATA_HDCP_DBG)
	  )
	{
		return TRUE;
	}
	return FALSE;
}

static void _Cbus_DDCErrorHandling(UINT8 u1ErrorCode) reentrant
{
#if 0
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
#endif
}



void vCbusRstDdcState(void)
{
	stCbusCmd.stDdc.u1IsEdid =0;
	stCbusCmd.stDdc.u1Offset =0;
	stCbusCmd.stDdc.u2DdcCmd =0;
	stCbusCmd.stDdc.u1DdcSta = DDC_STATE_NONE;
	vCBusCLrDdcTimer();
}



static void vCbusDdcLoop(UINT16 u2RxMsg)
{
	UINT16 u2Data=0;
	UINT32 u4An =0;
	UINT32 u4Aksv =0;
	
	if(fgCbusDdcAbort(u2RxMsg))
	{
		u1CbusDdcAbortDelay2S = MHL_DDC_ABORT_DELAY;
	}


	switch(stCbusCmd.stDdc.u1DdcSta)
	{
		case DDC_STATE_NONE:

			stCbusCmd.stDdc.u1IsEdid =0x0;
			stCbusCmd.stDdc.u1Offset =0xff;
			stCbusCmd.stDdc.u2DdcCmd =0x0;
			if(CBUS_DDC_CTRL_SOF == u2RxMsg)
			{
				stCbusCmd.stDdc.u1DdcSta =  DDC_STATE_SOF;
				stCbusCmd.stDdc.u1IsEdid =0x0;
				stCbusCmd.stDdc.u1Offset =0xff;
				stCbusCmd.stDdc.u2DdcCmd =0x0;
				vCbusSetDdcTimer(MHL_TIMER_PKT_PERIOD);
			}
			break;
		case DDC_STATE_SOF:
			iDdcOffset = 0 ;
			vCbusSetDdcTimer(MHL_TIMER_PKT_PERIOD);
			 if ((CBUS_DDC_DATA_ADRW== u2RxMsg) || (CBUS_DDC_DATA_HDCP_ADRW == u2RxMsg)) 
			{
				vCbusDdcResAck();
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_OFFSET;
			
				if(CBUS_DDC_DATA_ADRW == u2RxMsg)
				{
					stCbusCmd.stDdc.u1IsEdid = 1;
				}
				else
				{
					stCbusCmd.stDdc.u1IsEdid = 0;
				}
			} 
			else if ((CBUS_DDC_DATA_ADRR == u2RxMsg) || (CBUS_DDC_DATA_HDCP_ADRR== u2RxMsg)) 
			{
				vCbusDdcResAck();
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_CONT;
				
			}
			else if(CBUS_DDC_CTRL_SOF == u2RxMsg)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_SOF;
				//break;
			}
			else
			{
				vCBusDdcResNack();
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_EOF;
			} 
			
			break;
		case DDC_STATE_OFFSET:
			iDdcOffset = 0 ;
			vCbusSetDdcTimer(MHL_TIMER_PKT_PERIOD);
			if ((u2RxMsg == CBUS_DDC_CTRL_STOP) || (u2RxMsg == CBUS_DDC_CTRL_EOF)) 
			{
			   stCbusCmd.stDdc.u1DdcSta = DDC_STATE_EOF;
			   break;
			} 
			if(u2RxMsg == CBUS_DDC_CTRL_SOF)
			{
			   stCbusCmd.stDdc.u1DdcSta = DDC_STATE_SOF;
			   break;
			}
			if ((u2RxMsg & 0x700) ==0x0) 
			{
				if(stCbusCmd.stDdc.u1IsEdid == 1)
				{
				    stCbusCmd.stDdc.u1Offset = u2RxMsg;
					stCbusCmd.stDdc.u1DdcSta = DDC_STATE_DATAW;
				}				
				else
				{
					stCbusCmd.stDdc.u1Offset = u2RxMsg;
					if(fgCbusValidHdcpOffset(stCbusCmd.stDdc.u1Offset))
					{
						stCbusCmd.stDdc.u1DdcSta = DDC_STATE_DATAW;

						if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_BKSV_OFFSET)
						{
							vMHLGetHdcpBksv(au1Bksv);
						}
					}
					else
					{
						vCBusDdcResNack();
						stCbusCmd.stDdc.u1DdcSta = DDC_STATE_NONE;
						vCBusCLrDdcTimer();
					}
				}
				vCbusDdcResAck();
				break;
			 }
			_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
			break;
		case DDC_STATE_DATAW:
			vCbusSetDdcTimer(MHL_TIMER_PKT_PERIOD);
			if( u2RxMsg == CBUS_DDC_CTRL_SOF)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_SOF;
				break;
			}
			if(u2RxMsg == CBUS_DDC_CTRL_STOP)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_EOF;
				break;
			}
			if(u2RxMsg == CBUS_DDC_CTRL_EOF)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_NONE;
				break;
			}
			if((u2RxMsg & 0x700) ==0x0)
			{
				vCbusDdcResAck();
				if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_AN_OFFSET)
				{
					if(iDdcOffset < 8)
					{
						au1An[iDdcOffset] = u2RxMsg;
						u4An =  ((0x18+ iDdcOffset)<<8) + au1An[iDdcOffset];
				    	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0x20, RISC_HDCP_CLK_DIV);
				    	vIO32WriteFldAlign((MHL_HDCP_DDC + 0xc00), 0x1, RISC_ADDR_PAGE);
			            vIO32Write4B(MHL_HDCP_REG + 0xc00,u4An);
				         
						iDdcOffset ++;

						if(iDdcOffset == 8)
						{
							iDdcOffset =0;
						//	vMHLSetHdcpAn(au1An);
 
							if(u1AnReady < 2)
							{
								u1AnReady ++;
							}
						}
					}
					else
					{
						_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
					}
				}
				else if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_AINFO_OFFSET)
				{
					
				}
				else if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_AKSV_OFFSET)
				{
					if(iDdcOffset < 5)
					{
						au1Aksv[iDdcOffset] = u2RxMsg;
						u4Aksv =  ((0x10+iDdcOffset)<<8) + au1Aksv[iDdcOffset];
             			vIO32Write4B(MHL_HDCP_REG + 0xc00, u4Aksv);
						iDdcOffset ++;

						if(iDdcOffset == 5)
						{
							iDdcOffset =0;

							if(u1AnReady < 2)
							{
								u1AnReady ++;
							}
						}
					}
					else
					{
						_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
					}
				}
				break;
			}
			_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
			break;
		case DDC_STATE_CONT:
			vCbusSetDdcTimer(MHL_TIMER_PKT_PERIOD);
			if( u2RxMsg == CBUS_DDC_CTRL_SOF)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_SOF;
				break;
			}
			if(u2RxMsg == CBUS_DDC_CTRL_STOP)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_EOF;
				break;
			}
			if(u2RxMsg == CBUS_DDC_CTRL_EOF)
			{
				stCbusCmd.stDdc.u1DdcSta = DDC_STATE_NONE;
				break;
			}
			if(u2RxMsg == CBUS_DDC_CTRL_CONT)
			{
				//if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_AN_OFFSET)
				if(stCbusCmd.stDdc.u1IsEdid == 1)
				{
					if(stCbusCmd.stDdc.u1Offset < 256)
					{
						u2Data = arEdid[stCbusCmd.stDdc.u1Offset];
						vCbusSendDdcMsg(&u2Data,1);
						//u1CbusSendMsg(&u2Data,1);
						stCbusCmd.stDdc.u1Offset ++;
						
					}
					
				}
				else
				{
					if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_BKSV_OFFSET)
					{
						u2Data = au1Bksv[iDdcOffset];
						vCbusSendDdcMsg(&u2Data,1);
						//u1CbusSendMsg(&u2Data,1);
						iDdcOffset ++;

					}
					else if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_BCAPS_OFFSET)
					{
						if(vGetHDMIStatus(HDMI_SWITCH_1) == 6)
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
						//u1Bcaps = 0x80;
						u2Data = u1Bcaps;
						vCbusSendDdcMsg(&u2Data, 1);
						//u1CbusSendMsg(&u2Data,1);
						
					}
					else if(stCbusCmd.stDdc.u1Offset == CBUS_DDC_DATA_HDCP_BSTATUS_OFFSET)
					{
						if(u1IO32Read1B(AUDP_STAT_0 + 0xc00) & 0x1)
						{
						    au1Bstatus[1] = 0x10;
						}
						else
						{
						    au1Bstatus[1] = 0x0;
						}

						u2Data = au1Bstatus[iDdcOffset];
						vCbusSendDdcMsg(&u2Data,1);
						//u1CbusSendMsg(&u2Data,1);
						iDdcOffset ++;
						
					}
					else 
					{
						if(iDdcOffset == 0)
						{
							vMHLGetHdcpRi(au1Ri1);
							u2Data = au1Ri1[0];
						}
						else
						{
							u2Data = au1Ri1[1];
						}

						
						//vCbusSendDdcMsg(&u2Data,1);
						u1CbusSendMsg(&u2Data,1);
						iDdcOffset ++;
						
					}
			
				}
				break;
			}
			break;
		 case DDC_STATE_EOF:	           
		   	   vCBusCLrDdcTimer();			
			   if(CBUS_DDC_CTRL_SOF == u2RxMsg)
			   {
				   stCbusCmd.stDdc.u1DdcSta = DDC_STATE_SOF;
				   break;
			   }
			   else	
				if (u2RxMsg == CBUS_DDC_CTRL_EOF) 
				{
					stCbusCmd.stDdc.u1DdcSta = DDC_STATE_NONE;
					break;
				}
				else
				{
					_Cbus_DDCErrorHandling(MHL_MSC_PROTOCOL_ERR);
				}
				break;
		default:
			break;	
	}
}

static void _Cbus_Interrupt(UINT16 u2Vector)
{  
    UINT8 i = 0;
	UINT8 j = 0;
	UINT32 u4IntStat = 0;
	UINT32 u4ClrInt = 0; //= u4IO32Read4B(PDWNC_CBUS_LINK_08);
	UINT8 u1RxMsgCnts = 0;
	UINT16 u2RxMsg = 0;
	//UINT16 aRxMsg[32];	
	HAL_TIME_T tRxTime;
	MHL_MSG_T t_Msg;
	
    x_memset(&tRxTime, 0, sizeof(HAL_TIME_T));
	ASSERT(u2Vector == PDWNC_INTNO_CBUS);
	UNUSED(u2Vector);
	
	if(bCbusState == 0)
	{
		return;
	}
	
	u4IntStat = u4IO32Read4B(PDWNC_CBUS_STA_00);
	//LOG(1,"u4IntStat =0x%x\n",u4IntStat);
	
	if (u4IntStat & 0x1)
	{
		vCbusSetImpedance(SINK_CBUS_IMPEDANCE_100K);
		vCbusSetDiscoveryOK(TRUE);
		iMhlStateNew = MHL_STATE_CONNECTED;	
#ifdef SYS_ANDROID_TV_TIF_SUPPORT		
		Mhl_NotifyCbusMode(1,1);
#endif
		LOG(0,"discovery pulse detected............\n");
	}

	if(u4IntStat & 0x2)
	{
		vCbusEnDiscoveryDetect(TRUE);
		LOG(0,"wake up pulse detected..............\n");
		u4ClrInt |= 0x2;
	}
	if ((u4IntStat & 0x4000))
	{
		u4ClrInt |= 0x4000;
		
		vCbusSetImpedance(SINK_CBUS_IMPEDANCE_1K);
		// disable cbus disconnect INT			
		vIO32WriteFldAlign(PDWNC_CBUS_LINK_07,0,FLD_CBUS_DISCONN_CNT_EN);
		vCbus_DisableInterrupt();

		vMHLOpenRxTermination(0);
		_bIsMhlPPMode = 0;
       
		iMhlStateNew = MHL_STATE_INIT;
		vCBusSetDiscoveryNG();
		LOG(0, "low disconnect detected	\n");

	}

	if(u4IntStat & 0x400)
	{
		u4ClrInt |= 0x400;
		_bIsMhlDevice = 0;

		vMHLOpenRxTermination(0);
		iMhlStateNew = MHL_STATE_INIT;
		vCBusSetDiscoveryNG();
		GPIO_SetOut(VBUS_CTRL_GPIO, 0);//disable vbus
		LOG(0, "CABLE DISCONNECTTED	\n");

	}

	vIO32Write4B(PDWNC_CBUS_LINK_08, 0xffffffff);
	vIO32Write4B(PDWNC_CBUS_LINK_08, 0x0);

	if((stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND) || (stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND))
	{
		if(fgCbusTxTrigFail(u4IntStat))
		{
			vCBusCLrTxTimer();
			if(stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND)
			{
				stCbusMscTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND)
			{
				stCbusDdcTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			LOG(0,"SINK CBUS tx trigger fail,send again\n");
		}
		else if(fgCbusTxArbiFail(u4IntStat))
		{
			vCBusCLrTxTimer();
			if(stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND)
			{
				stCbusMscTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND)
			{
				stCbusDdcTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			LOG(0,"SINK CBUS tx arbitration fail,send again\n");
			
		}
		else if(fgCbusTxEvent(u4IntStat))
		{
			vCBusCLrTxTimer();
		}
		else if(fgCBusGetTxTimeOut())
		{
			vCBusCLrTxTimer();
		#if 0
			if(stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND)
			{
			   stCbusMscTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND)
			{
			   stCbusDdcTx.tx_sate = SINK_CBUS_TX_VALID;
			}
			LOG(0,"SINK CBUS tx time out,send again\n");
		#endif
		}
	}
	//requester first send data
	if(stSinkRequster.req_state == SINK_CBUS_REQ_WAIT)
	{
		if(fgCbusTxEvent(u4IntStat) || fgCBusGetTxTimeOut())
		{
			stSinkRequster.req_state = SINK_CBUS_REQ_IDLE;
			//do not send req msc first data
			stCbusMscTx.tx_sate = SINK_CBUS_TX_IDLE;
			if(fgCBusGetTxTimeOut())
			{
			}	
			else
			{
				stCbusCmd.u2CbusMscMode= SINK_CBUS_STATE_REQUESTER;
				stCbusCmd.stReqMsc.u2MscCmd = stSinkRequster.u2ReqBuf[0];
				stCbusCmd.stReqMsc.u1MscSta = SINK_CBUS_STATE_NONE;	
				vCbusSetMscTimer(MHL_TIMER_PKT_PERIOD);
			}
		}
	}

	if((fgCbusTxErr(u4IntStat)) ||(fgCbusRxErr(u4IntStat)) )
	{		
		if(!fgCbusTxArbiFail(u4IntStat))
		{
			stCbusMscTx.tx_sate = SINK_CBUS_TX_IDLE;
			stCbusDdcTx.tx_sate = SINK_CBUS_TX_IDLE;
		}
		vCbusRstMscState();
		vCbusRstDdcState();
		vCBusCLrTxTimer();
		if(fgCbusTxErr(u4IntStat))
		{
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,1,FLD_SW_RESET_TX);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,0,FLD_SW_RESET_TX);
			LOG(0,"tx err,reset HW\n");
		}
		else
		{
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,1,FLD_SW_RESET_RX);
			vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,0,FLD_SW_RESET_RX);
			//vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,1,FLD_SW_RESET_TX);
			//vIO32WriteFldAlign(PDWNC_CBUS_LINK_03,0,FLD_SW_RESET_TX);
			LOG(0,"rx err,reset HW\n");
		}
	}
	
	if(fgCbusTxOK(u4IntStat))
	{
		//LOG(0,"tx ok\n");
		vCBusCLrTxTimer();
		if(stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND)
		{
			stCbusMscTx.tx_sate = SINK_CBUS_TX_IDLE;
		}
		if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND)
		{
			stCbusDdcTx.tx_sate = SINK_CBUS_TX_IDLE;
		}
	}
	if(fgCBusGetDdcTimeOut())
	{
		_Cbus_DDCErrorHandling(MHL_MSC_MSG_TIMEOUT); 
		vCbusRstDdcState();			
		LOG(0,"timer_tri,ddc timer out\n");
	}
	if(fgCBusGetMscTimeOut())
	{
		u1CbusMscErrCode = MHL_MSC_MSG_TIMEOUT;
		vCbusRstMscState();		

		LOG(0,"mto\n");
	}

	//if(u4IntStat & SINK_RBUF_PULS1_INT_MASK)
	{
		u1RxMsgCnts = u4CbusGetRxBufLen();
		j= u1RxMsgCnts;
		while(j)
		{
		
			HAL_GetTime(&tRxTime);
			u2RxMsg = u4IO32Read4B(PDWNC_CBUS_RBUF) & 0x7FF;
			switch((u2RxMsg & 0x600) >> 9) //Get the Header 0110,0000,0000
			{
				case SINK_CBUS_RX_DDC_CHANNEL://Handle Rvd DDC Command
					if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_SEND)
					{
						stCbusDdcTx.tx_sate = SINK_CBUS_TX_IDLE;
					}
					vCbusDdcLoop(u2RxMsg);	
					break;
				case SINK_CBUS_RX_MSC_CHANNEL://Handle Rvd MSC Command 
					if(stCbusMscTx.tx_sate == SINK_CBUS_TX_SEND)
					{
						stCbusMscTx.tx_sate = SINK_CBUS_TX_IDLE;
						if(stSinkRequster.req_state == SINK_CBUS_REQ_WAIT)
						{
							stSinkRequster.req_state = SINK_CBUS_REQ_IDLE;
						}
					}
					vCbusMSCloop(u2RxMsg);
					break;
				case SINK_CBUS_RX_VDRSPEC_CHANNEL://implement in the future.
				case SINK_CBUS_RX_RSRVD_CHANNEL://implement in the future.
				default:
					break;
			}
			t_Msg.u2MhlMsg = u2RxMsg;
			t_Msg.u1GroupId = 1;// 1 indicate received command.
			x_memcpy(&(t_Msg.time),&tRxTime,sizeof(HAL_TIME_T));
			_Mhl_LogEnqueue(&t_Msg);
			j--;
		}
	}
	
	if((stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)&& fgCbusMscIdle())
	{
		if(fgCBUSRxBufEmpty() && fgCBUSTxBufEmpty() && fgCbusHwTxRxIdle())
		{
			//Printf("req begain\n");
			stSinkRequster.req_state = SINK_CBUS_REQ_WAIT;
			vCbusSendMscMsg(stSinkRequster.u2ReqBuf,stSinkRequster.u1Len);
		}
	}
	
	stCbusTxCtr.u2Len = 0;
	if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_VALID)
	{
		//LOG(0,"DDC tx\n");
		for(i=0;i<stCbusDdcTx.u2Len;i++)
		{
			stCbusTxCtr.u2TxBuf[ i] = stCbusDdcTx.u2TxBuf[i];
		}
		stCbusTxCtr.u2Len = stCbusDdcTx.u2Len;
	}
	//msc tx buf
	if(stCbusMscTx.tx_sate == SINK_CBUS_TX_VALID)
	{
		//LOG(0,"MSC tx\n");
		for(i=0;i<stCbusMscTx.u2Len;i++)
		{
			stCbusTxCtr.u2TxBuf[stCbusTxCtr.u2Len + i] = stCbusMscTx.u2TxBuf[i];
		}
		stCbusTxCtr.u2Len = stCbusMscTx.u2Len + stCbusTxCtr.u2Len;
	}

	
	//send tx data
	if(((stCbusMscTx.tx_sate == SINK_CBUS_TX_VALID)||(stCbusDdcTx.tx_sate == SINK_CBUS_TX_VALID))
		&&(stCbusMscTx.tx_sate != SINK_CBUS_TX_SEND)
		&&(stCbusDdcTx.tx_sate != SINK_CBUS_TX_SEND))
	{		
		u1CbusSendMsg(stCbusTxCtr.u2TxBuf,stCbusTxCtr.u2Len);	
		if(stCbusMscTx.tx_sate == SINK_CBUS_TX_VALID)
		{
			//Printf("MSC send\n");
			stCbusMscTx.tx_sate = SINK_CBUS_TX_SEND;
		}
		if(stCbusDdcTx.tx_sate == SINK_CBUS_TX_VALID)
		{
			//printf("DDC send\n");
			stCbusDdcTx.tx_sate = SINK_CBUS_TX_SEND;
		}
		vCbusSetTxTimer(30/MHL_TIMER_PERIOD);
	}
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
	if(u1GetCdsenseStatus())
    {
        u1BootUp = 1;
		_bIsMhlDevice = 1;
    }
    else   
    {
        u1BootUp = 0;
    }
    iMhlStateNew = MHL_STATE_INIT;
}
#endif


void vCbusSendRcp(UINT8 ui1KeyCode)
{
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = MHL_MSC_MSG_RCP;
	stSinkRequster.u2ReqBuf[2] = (UINT16)(ui1KeyCode | 0x400);
	stSinkRequster.u1Len =3;
	
	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
	
}

void vCbusSendUcp(UINT8 ui1data )
{
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = MHL_MSC_MSG_UCP;
	stSinkRequster.u2ReqBuf[2] = (UINT16)(ui1data | 0x400);	
	stSinkRequster.u1Len =3;
	
	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void vCbusRcpTest(UINT8 ui1KeyCode)
{
	vCbusSendRcp(ui1KeyCode);
	return;
}

void vCbusUcpTest(UINT8 ui1data)
{
	vCbusSendUcp(ui1data);
	return;
}

void vCbusUcpKTest(UINT8 ui1KeyCode)
{
	
}


void vCbusStop(void)
{
	vIO32WriteFldAlign(PDWNC_CBUS_LINK_02,0,FLD_LINKRX_EN);
	vIO32Write4BMsk(PDWNC_CBUS_LINK_03,0xf,0xf0000000);
	vCbus_DisableInterrupt();

	bCbusState = 0;
	//disable rx
}

void vCbusStatusQuery(void)
{
    LOG(0,"cbus current status \n");
	LOG(0,"iMhlState pre status %s -->current status %s,\n",_aszMhlState[iMhlStateOld],_aszMhlState[iMhlStateNew]);	
}


void vCbusResume(void)
{
    bCbusState = 1;
}

void vCbusSetVersion(UINT8 u1Version )
{
    //_arMhlDevCap.MhlVersion = u1Version;
}

UINT8 u1CbusGetVersion(void)
{
    return 0;//_arMhlDevCap.MhlVersion;
}

void vCbusSetState(UINT8 ui1_state)
{
   // iMhlStateNew = ui1_state;
    return;
}

UINT8 vCbusGetState(void)
{
    return iMhlStateNew;
}

void vCbusSetHpdFlag(UINT8 u1SetHpd)
{
   // MhlSetHpd = u1SetHpd;
   // MhlSetPathen = 1-MhlSetHpd;
}
#ifdef SYS_ANDROID_TV_TIF_SUPPORT
CBUS_MSG_DESCRIPTION_T scratchPadData;
void vCbusRequestSendSratchPad(CBUS_MSG_DESCRIPTION_T *msg)
{
	x_memset((void *)&scratchPadData,0,sizeof(CBUS_MSG_DESCRIPTION_T));
	x_memcpy((void *)&scratchPadData,(void *)msg,sizeof(CBUS_MSG_DESCRIPTION_T));
	vCbusRequestWriteBurst();
	stSinkCtr.skIsSendWriteBurstByAp =1;
}

void vCbusSendMsg(UINT8 opcode,UINT8 data)
{
    //_Cbus_SendMsg();
    if(u1CbusMscAbortDelay2S)
		return;
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = 0x400 | opcode;
	stSinkRequster.u2ReqBuf[2] = 0x400 | data;
	stSinkRequster.u1Len =3;

	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}
#else
void vCbusSendMsg(void)
{
    //_Cbus_SendMsg();
}
#endif
BOOL u1CbusGetOcpStatus(UINT8 port)
{
	//if(Cbus_GpioInput(MHL_OCP_GPIO))//3.3v normal, 0v ocp
	if(GPIO_GetIn(MHL_OCP_GPIO))//Modify by wangliang.wang
	{
	   return FALSE;
	}
	return TRUE;
}
#ifdef SYS_MHL_SUPPORT
#ifdef CBUS_OC_ADIN_USING
/*
For MT5882 Turnkey, the MHL OC pin is shared with the CI OC pin.so we should dectect the OC event
by juding the voltage.
*/
BOOL u1CbusADINGetOcpStatus(void)
{
	UINT32 getvalue,tagtvalue1,tagtvalue2;
	tagtvalue1 = (UINT32)(1.93*255)/2.8;
	tagtvalue2 = (UINT32)(1.56*255)/2.8;
	/*target value = (measured voltage * 255)/(max ADIN voltage), 255 is the maximum return value for PDWNC_ReadServoADCChannelValue */
	getvalue = PDWNC_ReadServoADCChannelValue(ADIN3);
	if(((getvalue < tagtvalue1 + 10) && (getvalue > tagtvalue1 - 10))
		|| ((getvalue < tagtvalue2 + 10) && (getvalue > tagtvalue2 - 10)))//10 reprsent 0.1v tolerance.
	{
		return TRUE;//OC happen.
	}
	else
	{
		return FALSE;//NO OC happen.
	}
	
}
#endif
#endif
void vCbusNtfMscCmd(UINT8 port, UINT8 MscCmd, UINT8 OpCode, UINT8 Data)
{
}

void vCbusOpenDebuglog(UINT8 u1debug)
{
    u1DebugLvl = u1debug;
}


void vCbusSendWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size)
{
	
	#if 0
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
	 #endif
}
void vCbusSendRcpKeys(UINT8 Key, UINT8 event)
{
	vCbusSendRcp(Key);
}
void vCbusSendRapKeys(UINT8 Key)
{
	stSinkRequster.req_state = SINK_CBUS_REQ_BEGIN;
	stSinkRequster.u2ReqBuf[0] = CBUS_MSC_CTRL_MSC_MSG;
	stSinkRequster.u2ReqBuf[1] = MHL_MSC_MSG_RAP;
	stSinkRequster.u2ReqBuf[2] = (UINT16)(Key | 0x400);	
	stSinkRequster.u1Len =3;
	
	while(stSinkRequster.req_state == SINK_CBUS_REQ_BEGIN)
	{
		vCbusTriggerCmpInterrupt();
		x_thread_delay(4);
	}
}

void vCbusCmdTest(UINT8 cmd_id, UINT8 sub_cmd_id)
{
#if 1
	switch(cmd_id)
	{
	case 1:
	  // set hpd
	   vCbusSetHpd();
	   LOG(0, "CBUS_MSC_CTRL_SET_HPD \n"); 			 
	break;

	case 2:
	// clr hpd
	  vCbusClrHpd();
	   LOG(0,"CBUS_MSC_CTRL_CLR_HPD \n"); 
	break;

	case 3:
	// set path en
	   vCbusSetPathEn();
	   LOG(0,"SET PATH EN \n"); 
	break;

	case 4:
		// set DEV_CAP ready
	   vCbusSetDevCapReady();
	   LOG(0,"SET CBUS_MSC_REG_READY_BITS \n"); 
	break;

	case 5:
	// DCAP_CHG
	 vCbusSetDevCapChange();
	  LOG(0, "SET CBUS_MSC_REG_REGCHANGE_INT \n");
		  
	break;
	case 23:
		LOG(0,"targetvalue1 = %d,targetvalue2 = %d\n",((UINT32)(1.93*255)/2.8),((UINT32)(1.56*255)/2.8));
#ifdef SYS_MHL_SUPPORT
#ifdef CBUS_OC_ADIN_USING	
		if(u1CbusADINGetOcpStatus())
		{
			LOG(0,"OCP HAPPEN!\n");
		}
		else
		{
			LOG(0,"OCP NOT HAPPEN!\n");
		}
#endif
#endif
	break;
#if 0
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
#endif
    default:
    break;
    }
#endif
}
BOOL fgGetMHLStatus(void)
{
	return _fgIsMHLDiscoveryOK;
}
UINT8 _Mhl_TxEnqueue(MHL_MSG_T *msg)
{
    return MHL_SUCCESSFUL;
}
UINT8 vIsRcpReceive(void)
{
    return u1isRcpRev;
}

UINT8 vRcpReceiveCode(void)
{
    return u1RevRcpKey;
}

void vClearRcpState(void)
{
    u1isRcpRev = 0;
	u1RevRcpKey = 0xff;
}
void vStopMHL(void)
{
    bCbusState = 0;
	u1EnVbus = 0;
	vCbusSetVbus(FALSE);
	vCbusSetImpedance(SINK_CBUS_IMPEDANCE_HiZ);  	
}

void vResumeMHL(void)
{
    bCbusState = 1;
	u1EnVbus = 1;
	iMhlStateNew = MHL_STATE_INIT;
}

