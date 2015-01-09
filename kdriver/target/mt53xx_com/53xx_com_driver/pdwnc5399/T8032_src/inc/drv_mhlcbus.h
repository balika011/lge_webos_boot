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
 * $RCSfile: drv_mhlcbus.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifndef DRV_MHLCBUS_H

#define DRV_MHLCBUS_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#define CBUS_8032
#ifdef CBUS_8032
#include "general.h"
#include "custom_def.h"
#include "hw_bim.h"
#include "hw_pdwnc.h"
#include "hw_reg.h"
#else
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_debug.h"
#include "drv_gpio.h"
LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#ifdef CC_DRIVER_PROGRAM
#define CBUS_CTS_TEST
#else
#define NOTIFY_CBUSMW
#endif
#endif
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define RETX_MAX_CNT 2
//#define LOOPBACK_TEST

#define CBUS_SUCCESS (0)

#define CBUS_ERROR (1)

//#define FPGA

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#ifdef CBUS_8032
extern UINT8 xdata fgCbus8032Ctrl;                      
#endif
enum
{
  DDC_STATE_NONE,
  DDC_STATE_SOF,
  DDC_STATE_ADRW,
  DDC_STATE_OFFSET,
  DDC_STATE_OFFSET1,
  DDC_STATE_CONT,
  DDC_STATE_DATAW,
  DDC_STATE_STOP,
  DDC_STATE_EOF,
  DDC_STATE_MAX,
};
#ifndef CBUS_8032
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
#endif

typedef struct
{
  UINT8 wAn:1;            // bit0
  UINT8 wAksv:1;      // bit1
  UINT8 rBksv: 1; //bit 2
  UINT8 rRi1: 1;       // bit3
  UINT8 rBcaps:1;  // bit4
  UINT8 rBstatus:1;  // bit5
  UINT8 rEdid :1;  // bit6
  UINT8 wAinfo:1;  // bit7
  UINT8 rRsd1:1;
  UINT8 rRsd2:1;
  UINT8 rRsd3:1;
  UINT8 rRsd4:1;
  UINT8 rRsd5:1;
  UINT8 rVh0:1;
  UINT8 rVh1:1;
  UINT8 rVh2:1;
  UINT8 rVh3:1;
  UINT8 rVh4:1;
  UINT8 rDbg:1;
  UINT8 rPj:1;
  UINT8 rKsvfifo:1;
} DDC_STATUS_T;

typedef struct
{
  UINT8 DevState;
  UINT8 MhlVersion;
  UINT8 DevCat;
  UINT8 AdopoterIdHigh;
  UINT8 AdopoterIdLow;
  UINT8 VidLinkMode;
  UINT8 AudLinkMode;
  UINT8 VideoType;  
  UINT8 LogDevMap;  
  UINT8 BandWidth; 
  UINT8 FeatureFlag;
  UINT8 DevicIdHigh;
  UINT8 DevicIdHigLow;
  UINT8 ScratchpadSize;
  UINT8 IntStateSize;  
  UINT8 reserved;    
} MHL_DEVCAP_T;

typedef struct
{
  UINT8 txOk:1;            // bit0
  UINT8 txArbFail:1;      // bit1
  UINT8 txRetryFail: 1; //bit 2
  UINT8 txWaitingAck: 1;       // bit3
  UINT8 txWaitingData:1;  // bit4
  UINT8 rxWaitingData:1;  // bit5
  UINT8 Reserved6:1;  // bit6
  UINT8 Reserved7:1;  // bit7
  UINT8 waitingDataNum;
} MHL_STATUS_T;

typedef struct
{
  UINT8 channel:2;
  UINT8 ctrl:1;
  UINT8 u1Data;
#ifndef CBUS_8032
  HAL_TIME_T time;
#endif
  MHL_STATUS_T status;
#ifdef SYS_MHL_SUPPORT
  UINT8 u1GroupId;
#endif
} MHL_MSG_T;

enum
{
    MHL_WRITE_BURST_NONE,
    MHL_WRITE_BURST_START,
    MHL_WRITE_BURST_OFFSET,
    MHL_WRITE_BURSET_WDATA,
    MHL_WRITE_BURST_EOF,
    MHL_WRITE_BURSET_MAX,
};
#ifndef CBUS_8032
static CHAR * _aszWriteBurstState[DDC_STATE_MAX] =
{   
   "MHL_WRITE_BURST_NONE",
   "MHL_WRITE_BURST_START", 
   "MHL_WRITE_BURST_OFFSET",
   "MHL_WRITE_BURSET_WDATA",
   "MHL_WRITE_BURST_EOF",
   "MHL_WRITE_BURSET_MAX",
};
#endif
enum
{
    MHL_RX_STATE_IDLE,
    MHL_RX_STATE_MSCMSG,
    MHL_RX_STATE_MSCOFFSET,
    MHL_RX_STATE_MSCDATA0,
    MHL_RX_STATE_MSCDATA1,
    MHL_RX_STATE_MAX,
};
#ifndef CBUS_8032
static CHAR * _aszMhlRxState[DDC_STATE_MAX] =
{   
	"MHL_RX_STATE_IDLE",
	"MHL_RX_STATE_MSCMSG", 
	"MHL_RX_STATE_MSCOFFSET",
	"MHL_RX_STATE_MSCDATA0",
	"MHL_RX_STATE_MSCDATA1",
	"MHL_WRITE_BURSET_MAX",
};
#endif
#if 1
typedef enum
{
    STATE_MHL_TX_SEND_MSG = 0x01,
    STATE_MHL_TX_WAITING_ACK = 0x02,
	STATE_MHL_TX_WAITING_DATA = 0x04,
    STATE_MHL_TX_SEND_MSG_OK = 0x08,  /*Tx with data, not ping frame.*/
    STATE_MHL_TX_SEND_MSG_FAIL = 0x10,  //arbitrate fail     
    STATE_MHL_TX_NOACK = 0x20,
    STATE_MHL_HAND_SHAKING_FAIL = 0x40,
} MHL_SW_STATE;

enum
{
  MHL_STATE_NONE,
  MHL_STATE_INIT,
  MHL_STATE_BOOTUP,
  MHL_STATE_WAIT_CABLE_CONNECT,  
  MHL_STATE_DISCOVERY,
  MHL_STATE_CONNECTED,
  MHL_STATE_CABLE_DISCONNECT,  
  MHL_STATE_MAX,
};
#ifndef CBUS_8032
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
// DDC channel control packet
#define CBUS_DDC_CTRL_SOF 0x130
#define CBUS_DDC_CTRL_EOF 0x132
#define CBUS_DDC_CTRL_ACK 0x133
#define CBUS_DDC_CTRL_NACK 0x134
#define CBUS_DDC_CTRL_ABORT 0x135
#define CBUS_DDC_CTRL_CONT 0x150
#define CBUS_DDC_CTRL_STOP 0x151

#define CBUS_DDC_DATA_SEGW 0x060
#define CBUS_DDC_DATA_ADRW 0x0A0
#define CBUS_DDC_DATA_ADRR 0x0A1

#define CBUS_DDC_DATA_HDCP_ADRW 0x074
#define CBUS_DDC_DATA_HDCP_ADRR 0x075
#define CBUS_DDC_DATA_HDCP_BKSV_OFFSET 0x00
#define CBUS_DDC_DATA_HDCP_RI1_OFFSET 0x08
#define CBUS_DDC_DATA_HDCP_AKSV_OFFSET 0x10
#define CBUS_DDC_DATA_HDCP_AINFO_OFFSET 0x15
#define CBUS_DDC_DATA_HDCP_AN_OFFSET 0x18
#define CBUS_DDC_DATA_HDCP_BCAPS_OFFSET 0x40
#define CBUS_DDC_DATA_HDCP_BSTATUS_OFFSET 0x41

#define CBUS_DDC_DATA_HDCP_RSVD1 0X05 //3///////
#define CBUS_DDC_DATA_HDCP_PJ 0X0A // 1////////
#define CBUS_DDC_DATA_HDCP_RSVD2 0X0B ///////5////////
#define CBUS_DDC_DATA_HDCP_RSVD3 0X16 //////2 /////////
#define CBUS_DDC_DATA_HDCP_VH0 0X20/////////////4
#define CBUS_DDC_DATA_HDCP_VH1 0X24///////////////////4
#define CBUS_DDC_DATA_HDCP_VH2 0X28///////////////////4
#define CBUS_DDC_DATA_HDCP_VH3 0X2C///////////////////4
#define CBUS_DDC_DATA_HDCP_VH4 0X30///////////////////4
#define CBUS_DDC_DATA_HDCP_RSVD4 0X34///////////////////12
#define CBUS_DDC_DATA_HDCP_KSVFIFO 0X43////////1
#define CBUS_DDC_DATA_HDCP_RSVD5 0X44////////124
#define CBUS_DDC_DATA_HDCP_DBG 0XC0 /////////64



#define DDC_PACKET 0
#define VSI_PACKET 1
#define MSC_PACKET 2
#define REV_PACKET 3
#define NONE_PACKET 0xffff
#define NULL_DATA 0xffff

#define DDC_EVET_NONE 0
#define DDC_EVET_WAIT_ACK 1
#define DDC_EVET_ACKED 2
#define DDC_EVET_WAIT_DATA 3
#define DDC_EVET_DATA_RECEIVED 4
#define DDC_EVET_DATA_END 5

#define DDC_SRC_IDLE 0
#define DDC_SRC_SOF 1
#define DDC_SRC_ADRW_WAIT_ACK 2
#define DDC_SRC_OFFSET_WAIT_ACK 3
#define DDC_SRC_ADRR_WAIT_ACK 4
#define DDC_SRC_DDC_CONT 5
#define DDC_SRC_STOP 6
#define DDC_SRC_EOF 7

//C1, C3
#define CBUS_C1_CTRL_ABORT 0x135
#define CBUS_C3_CTRL_ABORT 0x335


// MSC control packets
#define CBUS_MSC_CTRL_ACK 0x533
#define CBUS_MSC_CTRL_NACK 0x534
#define CBUS_MSC_CTRL_ABORT 0x535

#define CBUS_MSC_CTRL_WRITE_STATE 0x560////type 3, source should send 3 packet,need ack
#define CBUS_MSC_CTRL_SET_INT 0x560////type 3, source should send 3 packet,need ack
#define CBUS_MSC_CTRL_READ_DEVCAP 0x561////type 1, source should send 2 packet,need ack, and 1 value
#define CBUS_MSC_CTRL_GET_STATE 0x562////////type 0, no ack , only value
#define CBUS_MSC_CTRL_GET_VENDER_ID 0x563////////type 0, no ack , only value
#define CBUS_MSC_CTRL_SET_HPD 0x564////////type 0, no ack , only value
#define CBUS_MSC_CTRL_CLR_HPD 0x565////////type 0, no ack , only value

#define CBUS_MSC_CTRL_MSC_MSG 0x568//////////////type 3, source should send 3 packet,need ack,
                                       //////////////type 2, source should send 3 packet,need ack,and 3 value

#define CBUS_MSC_CTRL_GET_SC1_EC 0x569////////type 0, no ack , only value
#define CBUS_MSC_CTRL_GET_DDC_EC 0x56A////////type 0, no ack , only value
#define CBUS_MSC_CTRL_GET_MSC_EC 0x56B////////type 0, no ack , only value

#define CBUS_MSC_CTRL_WRITE_BURST 0x56C
#define CBUS_MSC_CTRL_GET_SC3_EC 0x56D////////type 0, no ack , only value

#define CBUS_MSC_CTRL_EOF 0x532

// device capability registers
#define CBUS_MSC_DEVCAP_DEV_STATE 0x400
#define CBUS_MSC_DEVCAP_MHL_VERSION 0x401
#define CBUS_MSC_DEVCAP_DEV_CAT 0x402
#define CBUS_MSC_DEVCAP_ADOPTER_ID_H 0x403
#define CBUS_MSC_DEVCAP_ADOPTER_ID_L 0x404
#define CBUS_MSC_DEVCAP_VID_LINK_MODE 0x405
#define CBUS_MSC_DEVCAP_AUD_LINK_MODE 0x406
#define CBUS_MSC_DEVCAP_VIDEO_TYPE 0x407
#define CBUS_MSC_DEVCAP_LOG_DEV_MAP 0x408
#define CBUS_MSC_DEVCAP_BANDWIDTH 0x409
#define CBUS_MSC_DEVCAP_FEATURE_FLAG 0x40A
#define CBUS_MSC_DEVCAP_DEVICE_ID_H 0x40B
#define CBUS_MSC_DEVCAP_DEVICE_ID_L 0x40C
#define CBUS_MSC_DEVCAP_SCRATCHPAD_SIZE 0x40D
#define CBUS_MSC_DEVCAP_INT_STATE_SIZE 0x40E
#define CBUS_MSC_DEVCAP_RESERVED 0x40F
// device capability registers
#define CBUS_MSC_REG_REGCHANGE_INT 0x420
#define CBUS_MSC_REG_DDCHANGE_INT 0x421

#define CBUS_MSC_REG_READY_BITS 0x430
#define CBUS_MSC_REG_ACTIVE_LINK_MODE 0x431


//fields of capability register
#define	FIELD_MHL_VER_MAJOR_2_0                0x20				
#define	FIELD_MHL_VER_MINOR_1_2		      0x12	
#define	FIELD_MHL_VER_MINOR_1_1		      0x11
#define	FIELD_MHL_VER_MINOR_1_0			0x10

#define	FIELD_DEV_CAT_SINK		              0x01
#define	FIELD_DEV_CAT_SOURCE				0x02
#define	FIELD_DEV_CAT_DONGLE				0x03
#define   FIELD_DEV_CAT_POWER				0x10
#define   FIELD_DEV_CAT_PLIM_500MA			0x00  // 500mA
#define   FIELD_DEV_CAT_PLIM_900MA			0x20  // 900mA
#define   FIELD_DEV_CAT_PLIM_1_5A			0x40  // 1.5 A
#define   FIELD_DEV_CAT_PLIM_RESERVED		0x60  // reserved

#define	FIELD_DEV_SUPP_RGB444			0x01
#define	FIELD_DEV_SUPP_YCBCR444			0x02
#define	FIELD_DEV_SUPP_YCBCR422			0x04
#define	FIELD_DEV_SUPP_PPIXEL				0x08
#define	FIELD_DEV_SUPP_ISLANDS			0x10   // support data islands
#define   FIELD_DEV_SUPP_VGA				0x20   // 640x480 / 60Hz, VGA timing
// bit 6,7 reserved

#define	FIELD_DEV_AUD_2CH					0x01
#define	FIELD_DEV_AUD_8CH					0x02
// bit 2 ~ 7 reserved

#define   FIELD_DEV_VT_GRAPHIC                       0x01
#define   FIELD_DEV_VT_PHOTO                           0x02
#define   FIELD_DEV_VT_CINEMA                         0x04
#define   FIELD_DEV_VT_GAME                             0x08
// bit 5~7 reserved
#define   FIELD_DEV_SUPP_VT                              0x80

#define	FIELD_DEV_LD_DISPLAY				0x01
#define	FIELD_DEV_LD_VIDEO				0x02
#define	FIELD_DEV_LD_AUDIO				0x04
#define	FIELD_DEV_LD_MEDIA				0x08
#define	FIELD_DEV_LD_TUNER				0x10
#define	FIELD_DEV_LD_RECORD				0x20
#define	FIELD_DEV_LD_SPEAKER				0x40
#define	FIELD_DEV_LD_GUI					0x80
 
#define 	FIELD_DEV_RCP_SUPPORT		       0x01
#define 	FIELD_DEV_RAP_SUPPORT		       0x02
#define 	FIELD_DEV_SP_SUPPORT			       0x04
#define 	FIELD_DEV_SP_UCP_SEND_SUPPORT	0x08
#define 	FIELD_DEV_SP_UCP_RECV_SUPPORT	0x10

#define	FIELD_DEV_SCRATCHPAD_SIZE	       0x40

#define	FIELD_DEV_INTERRUPT_SIZE		       0xF
#define	FIELD_DEV_STATUS_SIZE			       0xF

//devic capability valus
//offset = 0x00
#define 	DEVCAP_DEVICE_STATE				0x00

//offset = 0x01
#define	DEVCAP_MHL_VERSION				(FIELD_MHL_VER_MAJOR_2_0)

//Address = 0x02
#define	DEVCAP_MHL_DEVICE_CATEGORY		(FIELD_DEV_CAT_SINK | FIELD_DEV_CAT_POWER)

//Address = 0x03
#define 	DEVCAP_ADOPTER_ID_H				0x01  // SONY

//Address = 0x04
#define	DEVCAP_ADOPTER_ID_L				0x43  // SONY

//offset = 0x05
#define	DEVCAP_VID_LINK_MODE				(FIELD_DEV_SUPP_RGB444 | FIELD_DEV_SUPP_YCBCR444 | FIELD_DEV_SUPP_YCBCR422 | FIELD_DEV_SUPP_ISLANDS | FIELD_DEV_SUPP_VGA)

//offset = 0x06
#define	DEVCAP_AUD_LINK_MODE				(FIELD_DEV_AUD_2CH)

//offset = 0x07
#define	DEVCAP_VIDEO_TYPE					(FIELD_DEV_VT_GRAPHIC | FIELD_DEV_VT_PHOTO | FIELD_DEV_VT_CINEMA | FIELD_DEV_VT_GAME | FIELD_DEV_SUPP_VT)

//offset = 0x08
#define	DEVCAP_LOG_DEV_MAP              		(FIELD_DEV_LD_DISPLAY)

//offset = 0x09
#define	DEVCAP_BANDWIDTH					0x0F	// 75 MHz, 75/5=15

//offset = 0x0A
#define	DEVCAP_FEATURE_FLAG				(FIELD_DEV_RCP_SUPPORT | FIELD_DEV_RAP_SUPPORT | FIELD_DEV_SP_SUPPORT /*| FIELD_DEV_SP_UCP_SEND_SUPPORT | FIELD_DEV_SP_UCP_RECV_SUPPORT*/)

//offset = 0x0B
#define	DEVCAP_DEVICE_ID_H				0x00

//offset = 0x0C
#define	DEVCAP_DEVICE_ID_L				0x01

//offset = 0x0D
#define	DEVCAP_SCRATCHPAD_SIZE			(FIELD_DEV_SCRATCHPAD_SIZE)

//offset = 0x0E
#define	DEVCAP_INT_STAT_SIZE				((FIELD_DEV_INTERRUPT_SIZE<<4)|FIELD_DEV_STATUS_SIZE)

//offset = 0x0F
#define	DEVCAP_RESERVED                               	0x00 //reserved

#define MHL_RX_QUEUE_SIZE 16
#define MHL_TX_QUEUE_SIZE 24
#define MHL_MSG_BUF_SIZE 24
#define SCRATCHPAD_REG_SIZE (FIELD_DEV_SCRATCHPAD_SIZE + 1)

#define MIN_INT_REG_OFFSET 0x420
#define MAX_INT_REG_OFFSET 0x42f
#define MIN_STAT_REG_OFFSET 0x430
#define MAX_STAT_REG_OFFSET 0x43f
#define MIN_SCRACHPAD_REG_OFFSET 0x440
#define MAX_SCRACHPAD_REG_OFFSET (0x400 | ( 0x40 + FIELD_DEV_SCRATCHPAD_SIZE))

#define INT_DCAP_CHG 1
#define INT_DSCR_CHG 0x2
#define INT_REQ_WRT 0x4
#define INT_GRT_WRT 0x8
#define INT_3D_REQ 0x10
typedef enum eHDMI_SWITCH_NO {
	HDMI_SWITCH_INIT = 0,
	HDMI_SWITCH_1,
	HDMI_SWITCH_2,
	HDMI_SWITCH_3,
	HDMI_SWITCH_4,
	HDMI_SWITCH_MAX
}E_HDMI_SWITCH_NUM;
typedef enum
{
 Resister_None = 0x00,
 Resister_PullLow = 0x01,
 Resister_NotPullLow = 0x02,
} Resister_State;


enum//msc command reactions
{
    MHL_MSC_BAD_OFFSET = 0x10,//bad offset, send abort
    MHL_MSC_BAD_OPCODE   = 0x08,//bad opcode, send abort
    MHL_MSC_MSG_TIMEOUT = 0X04,//MHL_MSC_TOO_FEW_PKT  = 0x04,//time out
                                   //MHL_MSC_INCOMPLETE_PKT = 0x04,//time out
    MHL_MSC_DEV_BUSY = 0x20,//nack
    MHL_MSC_PROTOCOL_ERR = 0x02,//abort
    MHL_MSC_RETRY_EXCEED = 0x01,//time out
};
enum
{
    MHL_MSC_MSG_RCP             = 0x410,     // RCP sub-command
    MHL_MSC_MSG_RCPK            = 0x411,     // RCP Acknowledge sub-command
    MHL_MSC_MSG_RCPE            = 0x412,     // RCP Error sub-command
    MHL_MSC_MSG_RAP             = 0x420,     // RAP sub-command
    MHL_MSC_MSG_RAPK            = 0x421,     // RAP Acknowledge sub-command
    MHL_MSC_MSG_UCP             = 0x430,
    MHL_MSC_MSG_UCPK            = 0x431,
    MHL_MSC_MSG_UCPE            = 0x432,
};

enum
{
    MHL_RAP_CMD_POLL            = 0x400,     // Poll
    MHL_RAP_CMD_CONTENTON       = 0x410,     // change to content-on state
    MHL_RAP_CMD_CONTENTOFF      = 0x411,     // change to content-off state
};//RAP action code

enum
{
    MHL_RAP_NO_ERROR           = 0x00,     // Poll
    MHL_RAP_UNRECOGNIZED_CODE       = 0x01,     // unrecognized action code RAP action code
    MHL_RAP_UNSUPPORT_CODE      = 0x02,     // unsupport RAP Action code
    MHL_RAP_RESPONDER_BUSY      = 0x03,     // responder is busy
};//RAP Error code

enum
{
    MHL_MSC_MSG_NO_ERROR        = 0x00,     // RCP No Error
    MHL_MSC_MSG_ERROR_KEY_CODE  = 0x01,     // RCP Unrecognized Key Code
    MHL_MSC_MSG_BUSY            = 0x02,     // RCP Response busy
};//RCP error code

enum
{
    MHL_RCP_CMD_SELECT            = 0x00,
    MHL_RCP_CMD_UP            = 0x01,
    MHL_RCP_CMD_DOWN           = 0x02,
    MHL_RCP_CMD_LEFT            = 0x03,
    MHL_RCP_CMD_RIGHT     = 0x04,
    MHL_RCP_CMD_RIGHTUP          = 0x05,
    MHL_RCP_CMD_RIGHTDOWN           = 0x06,
    MHL_RCP_CMD_LEFT_UP        = 0x07,
    MHL_RCP_CMD_LEFT_DOWN        = 0x08,
    MHL_RCP_CMD_ROOT_MENU        = 0x09,
    MHL_RCP_CMD_SETUP_MENU        = 0x0a,
    MHL_RCP_CMD_CONTENTS_MENU            = 0xb,
    MHL_RCP_CMD_FAVORITE_MENU            = 0xc,
    MHL_RCP_CMD_EXIT              = 0xd,
    
    MHL_RCP_CMD_NUM_0            = 0x20,
    MHL_RCP_CMD_NUM_1           = 0x21,
    MHL_RCP_CMD_NUM_2              = 0x22,
    MHL_RCP_CMD_NUM_3            = 0x23,

    MHL_RCP_CMD_NUM_4     = 0x24,
    MHL_RCP_CMD_NUM5      = 0x25,

    MHL_RCP_CMD_NUM_6          = 0x26,
    MHL_RCP_CMD_NUM_7         = 0x27,
    MHL_RCP_CMD_NUM_8         = 0x28,
    MHL_RCP_CMD_NUM_9        = 0x29,
    MHL_RCP_CMD_DOT            = 0x2A,
    MHL_RCP_CMD_ENTER         = 0x2B,
    MHL_RCP_CMD_CLEAR         = 0x2c,

    MHL_RCP_CMD_CHANNEL_UP           = 0x30,
    MHL_RCP_CMD_CHANNEL_DOWN           = 0x31,
    MHL_RCP_CMD_PREVIOUS_CHANNEL          = 0x32,
    MHL_RCP_CMD_SOUND_SELECT          = 0x33,
    MHL_RCP_CMD_INPUT_SELECT           = 0x34,
    MHL_RCP_CMD_SHOWINFORMATION           = 0x35,
    MHL_RCP_CMD_HELP           = 0x36,
    MHL_RCP_CMD_PAGE_UP          = 0x37,
    MHL_RCP_CMD_PAGE_DOWN           = 0x38,

    //MHL_RCP_CMD_NUM_0           = 0x40,
    MHL_RCP_CMD_VOL_UP           = 0x41,
    MHL_RCP_CMD_VOL_DOWN          = 0x42,
    MHL_RCP_CMD_MUTE          = 0x43,
    MHL_RCP_CMD_PLAY           = 0x44,
    MHL_RCP_CMD_STOP           = 0x45,
    MHL_RCP_CMD_PAUSE           = 0x46,
    MHL_RCP_CMD_RECORD          = 0x47,
    MHL_RCP_CMD_REWIND           = 0x48,
    MHL_RCP_CMD_FASTFORWARD           = 0x49,
    MHL_RCP_CMD_EJECT          = 0x4A,
    MHL_RCP_CMD_FORWARD          = 0x4B,
    MHL_RCP_CMD_BACKWARD          = 0x4C,

    MHL_RCP_CMD_KEY_RELEASED	= 0x80,
};//RCP key code

enum
{
    MSC_WRITE_STAT              = 0x60,     // share same OPCODE
    MSC_SET_INT                 = 0x60,     // share same OPCODE
    MSC_READ_DEVCAP             = 0x61,
    MSC_GET_STATE               = 0x62,
    MSC_GET_VENDOR_ID           = 0x63,
    MSC_SET_HPD                 = 0x64,
    MSC_CLR_HPD                 = 0x65,
    MSC_SET_CEC_CAP_ID          = 0x66,
    MSC_GET_CEC_CAP_ID          = 0x67,
    MSC_MSG                     = 0x68,
    MSC_GET_VS_ABORT            = 0x69,
    MSC_GET_DDC_ABORT           = 0x6A,
    MSC_GET_MSC_ABORT           = 0x6B,
    MSC_WRITE_BURST             = 0x6C,
};
#endif
#ifdef CC_MHL_3D_SUPPORT
typedef struct
{
  UINT8 BURST_ID_H;
  UINT8 BURST_ID_L;
  UINT8 CHECK_SUM;
  UINT8 TOT_ENT;
  UINT8 SEQ;
  UINT8 NUM_ENT;
  UINT8 VDI_0_H;
  UINT8 VDI_0_L;  
  UINT8 VDI_1_H;  
  UINT8 VDI_1_L; 
  UINT8 VDI_2_H;
  UINT8 VDI_2_L;
  UINT8 VDI_3_H;
  UINT8 VDI_3_L;
  UINT8 VDI_4_H;  
  UINT8 VDI_4_L;    
}  WRITE_BURST_VDO_3D_T;
#endif

#define TMIER_PKT_RECEIVER 170//ms
#define TIMER_PKT_SENDER 170
#define TIMER_CMD_RECEIVER 340
#define TIMER_CMD_SENDER 340
#define TIMER_WRITE_ABORT 2200
#define TIMER_OCP_POLLING 10
#define TIMER_BOOT_UP 140

#define IS_MHL_RX_Q_EMPTY() (mhl_rxQ_read_idx == mhl_rxQ_write_idx)
#define IS_MHL_RX_Q_FULL() (((mhl_rxQ_write_idx+1)%MHL_RX_QUEUE_SIZE) == mhl_rxQ_read_idx)

#define IS_MHL_TX_Q_EMPTY() (mhl_txQ_read_idx == mhl_txQ_write_idx)
#define IS_MHL_TX_Q_FULL() (((mhl_txQ_write_idx+1)%MHL_TX_QUEUE_SIZE) == mhl_txQ_read_idx)
#define IS_CBUS_BUSY() (0)
#ifdef CBUS_8032
#define x_memset memset
#define x_memcpy memcpy
#define SetMHLStatus(arg) { \
    (_MHLStatus |= ((UINT8)arg)); }

#define ClrMHLStatus(arg) { \
    (_MHLStatus &= (~((UINT8)arg)));  }
#else
#define SetMHLStatus(arg) { \
    CRIT_STATE_T rCritState; \
    rCritState = x_crit_start(); \
    (_MHLStatus |= ((UINT8)arg)); \
    x_crit_end(rCritState); }

#define ClrMHLStatus(arg) { \
    CRIT_STATE_T rCritState; \
    rCritState = x_crit_start(); \
    (_MHLStatus &= (~((UINT8)arg))); \
    x_crit_end(rCritState); }
#endif
#define IsMHLStatus(arg) 	((_MHLStatus & ((UINT8)arg)) > 0)
#define MHL_SUCCESSFUL 0
#define MHL_RX_QUEUE_EMPTY 1

#define MHL_TX_QUEUE_EMPTY 51

#define MHL_RX_ERR_QUEUE_FULL 101
#define MHL_RX_ERR_WRITE_POINTER 102

#define MHL_TX_ERR_QUEUE_FULL 151
#define MHL_TX_ERR_WRITE_POINTER 152

#define CBUS_TRIGGER_TX_HW() ((void)vRegWriteFldAlign(PDWNC_CBUS_LINK_00, 1, FLD_TX_TRIG));\
	                            ((void)vRegWriteFldAlign(PDWNC_CBUS_LINK_00, 0, FLD_TX_TRIG))

#ifdef FPGA
#define ENABLE_PINMUX_TO_CBUS()      (0)
#else
#if defined(CC_MT5882)
#define CBUS_OC_STB_ADIN_USING 
#define FLD_HDMI_2_HPD_CBUS Fld(2,26,AC_MSKB3)//[2]
#else
#define FLD_HDMI_2_HPD_CBUS Fld(1,2,AC_MSKB0)//[2]
#endif
#define ENABLE_PINMUX_TO_CBUS() ((void)vRegWriteFldAlign(PDWNC_PINMUX1,1,FLD_HDMI_2_HPD_CBUS))
#define DISABLE_PINMUX_TO_CBUS() ((void)vRegWriteFldAlign(PDWNC_PINMUX1,0,FLD_HDMI_2_HPD_CBUS)) 
#endif
#define EDID_SIZE 256
#define NMAX 24

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
extern void vCbusInit(void);
#ifndef CBUS_8032
extern UINT8 _Mhl_TxEnqueue(MHL_MSG_T *msg);
extern void vCbusCmdTest(UINT8 cmd_id);
extern void vCbusOpenDebuglog(UINT8 u1debug);
extern void vCbusRcpTest(UINT8 ui1KeyCode);
extern void vCbusSendMsg(void);
extern void vCbusSetHpdFlag(UINT8 u1SetHpd);
extern void vCbusSetState(UINT8 ui1_state);
extern void vCbusStatusQuery(void);
extern void vCbusUcpTest(UINT8 ui1data);
extern void vCbusConvertInttoMsg(UINT16 ui2Msg, MHL_MSG_T *ptMsg);
#endif
extern UINT8 u1GetCdsenseStatus(void);
extern void vCbusSetState(UINT8 ui1_state);
extern void vCbusStatusQuery(void);
#ifdef SYS_MHL_SUPPORT
#define MHL_TX_NEXT_GROUP 0
#define MHL_TX_CUR_GROUP 1
extern void vCbusSendWriteBurstData(UINT8 key, UINT8 *pdata, UINT8 size);
extern void vCbusSendRcpKeys(UINT8 Key, UINT8 event);
extern void vCbusNtfMscCmd(UINT8 port, UINT8 MscCmd, UINT8 OpCode, UINT8 Data);
extern void vCbusSendRapKeys(UINT8 key);
extern void vCbusSetVBus(UINT8 u1VbusState);
extern BOOL u1CbusGetOcpStatus(UINT8 port);
#endif
#ifndef FPGA
#ifndef CBUS_8032
extern UINT8  _bIsMhlDevice;//0:HDMI   1:MHL
extern UINT8 u1HDMI_GetEdidData(UINT8 u1Port, UINT8 *pData, UINT16 u2Length);
#endif
#endif

#ifdef CBUS_8032
extern void vCbus_Interrupt(void);
extern void vCbusMainLoop(void);
extern void vCbusInit(void);
#else
#ifndef SYS_MHL_SUPPORT
typedef void (*x_cbus_drv_nfy_data_arrival) (MHL_MSG_T* prMsg);
typedef void (*x_cbus_drv_nfy_tx_result) (UINT8 result, void* txtag);
static x_cbus_drv_nfy_data_arrival rCbusRxHandler;
static x_cbus_drv_nfy_tx_result rCbusTxHandler;
extern void CBUS_SetRxCallback(x_cbus_drv_nfy_data_arrival nfyinfo);
extern void CBUS_SetTxCallback(x_cbus_drv_nfy_tx_result nfyinfo);
#endif
#endif

#ifdef CC_MHL_3D_SUPPORT
UINT8 vCbusWriteBurst3D(void);
#endif

UINT8 u1CbusGetOcpStatus();
#endif // DRV_MHLCBUS_H
void vCbus_DisableInterrupt(void);
void vCbusClrStatusAll(void);
#ifdef CBUS_OC_STB_ADIN_USING
extern UINT8 u1CbusADINGetOcpStatus(void);
#endif

