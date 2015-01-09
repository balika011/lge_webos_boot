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

/*
 * Hub driver definitions
 * $Revision: #1 $
 */

#ifndef _MU_HDEF_H_
#define _MU_HDEF_H_

LINT_SUPPRESS_BRACE(621)            // Warning 621: Identifier clash

/** ASSUMPTIONS: 
 * @doc 1. This Design Doesn't differentiate between root hub and Hub. So whatever 
 *    Hub is connected can be consider as root HUB. Only one difference between
 *    root hub and normal hub is; root-hub's pParentHub == NULL but normal hub's
 *    pParentHub != NULL.
 */
#define MUSB_HUB_DEBUG_DISPLAY             0x02

#define  MUSB_SUCCESS                       0x00
#define  MUSB_FAILURE                       0xFFFFFFFF
#define  MUSB_FAILURE_UNSIGNED             (uint32_t)MUSB_FAILURE

/** Setup Packet Length for control transfer.  */
#define MUSB_HUB_SETUP_PACKET_LENGTH                0x08

/** Generic defines which are used for different control transfer i.e.
 *  for class specific as well as Standard requests.
 */
#define MUSB_HUB_NO_IN_DATA_TRANSFER                0x00
#define MUSB_HUB_NO_DATA_PHASE                      0x00


#define MUSB_HUB_LOGICAL_POWER_SWITCH_MODE_BM       0x03
#define MUSB_HUB_GANGED_POWER_SWITCHING             0x00

#define MUSB_HUB_COMPOUND_DEVICE                    0x000004

        /*  Hub Specific defines. */

/** Maximum length of Hub descriptor in bytes. */
//#define MUSB_HUB_MAX_HUB_DSCR_SIZE                  0x0F

/** Root Hub is not connected to system */
#define MUSB_ROOT_HUB_NOT_CONNECTED                 ((uint8_t) 0x00)
/** Hub is connected to system */
#define MUSB_ROOT_HUB_CONNECTED                     ((uint8_t) 0x01)


/**                 State of Hub's child.    */
#define MUSB_HUB_CHILD_FREE                         0x00
#define MUSB_HUB_CHILD_PRESENT                      0x01


/** Port number 0x00 is invalid port number for any hub. */
#define  MUSB_HUB_INVALID_PORT_NUMBER               0x00

/** Hub's port number starts from 0x01 and goes till 255 as per USB 2.0
 *  Specification.
 */
#define MUSB_HUB_STARTING_PORT_NUMBER               0x01

/** Maximum Number of working Hub can be connected as chain Including Root Hub. 
 * As per 4.1.1 USB 2.0 specification. 
 */

#if defined (CONFIG_ARCH_MT85XX)
#define MUSB_MAX_HUB_CHAIN (0x06) //(0x02)
#else
#ifdef CC_USB_SUPPORT_HUB
#define MUSB_MAX_HUB_CHAIN (0x03)
#else
#define MUSB_MAX_HUB_CHAIN (0x02)
#endif /* #ifndef CC_USB_SUPPORT_HUB */
#endif

/** length of data comes in term of bytes as status of Hub. */
#define MUSB_HUB_STATUS_LENGTH                      0x04

/** length of data comes in term of bytes as status of Hub's port. */
#define MUSB_HUB_PORT_STATUS_LENGTH                 0x04

/** Speed of the connected device at hub's port. As per 11.8.2 */
#define MUSB_DEVICE_SPEED_LOW                       ((uint8_t)0x01)
#define MUSB_DEVICE_SPEED_FULL                      ((uint8_t)0x02)
#define MUSB_DEVICE_SPEED_HIGH                      ((uint8_t)0x03)

/** State of HUB       */
#define MUSB_HUB_STATE_FREE                     ((uint8_t)0x00)
#define MUSB_HUB_STATE_CONNECT                  ((uint8_t)0x01)
#define MUSB_HUB_STATE_CONFIGURED               ((uint8_t)0x02)

#define MUSB_HUB_STATE_RESET_TT                 ((uint8_t)0x03)
#define MUSB_HUB_STATE_RESET_TT_COMPLETE        ((uint8_t)0x04)
#define MUSB_HUB_STATE_GOT_HUB_DSCR             ((uint8_t)0x05)
#define MUSB_HUB_STATE_POWER_ON                 ((uint8_t)0x06)
#define MUSB_HUB_STATE_POWER_ON_COMPLETE        ((uint8_t)0x07)
#define MUSB_HUB_STATE_GOT_HUB_STATUS           ((uint8_t)0x08)
#define MUSB_HUB_STATE_C_LOCAL_POWER_COMPLETED ((uint8_t)0x09)
#define MUSB_HUB_STATE_C_HUB_OVER_CURRENT       ((uint8_t)0x0A)
#define MUSB_HUB_STATE_INIT                     ((uint8_t)0x0B)
#define MUSB_HUB_STATE_CHECK_OVER_CURRENT_EXISTANCE ((uint8_t)0x0E)
#define MUSB_HUB_STATE_DISCONNECT_DEVICE       ((uint8_t)0x10)

/**             State of PORT       */ 
#define MUSB_HUB_PORT_STATE_DEFAULT                    ((uint8_t)0x00)
#define MUSB_HUB_PORT_STATE_GOT_STATUS_P1      ((uint8_t)0x01)
#define MUSB_HUB_PORT_STATE_CLEAR_CONNECT      ((uint8_t)0x02)
#define MUSB_HUB_PORT_STATE_CLEAR_ENABLE      ((uint8_t)0x03)
#define MUSB_HUB_PORT_STATE_GOT_STATUS              ((uint8_t)0x04)
#define MUSB_HUB_PORT_STATE_C_CONNECT_COMPLETE      ((uint8_t)0x05)
#define MUSB_HUB_PORT_STATE_C_ENABLE_COMPLETE       ((uint8_t)0x06)
#define MUSB_HUB_PORT_STATE_C_SUSPEND_COMPLETE      ((uint8_t)0x07)
#define MUSB_HUB_PORT_STATE_C_RESET_COMPLETE        ((uint8_t)0x08)
#define MUSB_HUB_PORT_STATE_C_OVERCURRENT           ((uint8_t)0x09)
#define MUSB_HUB_PORT_STATE_C_OVERCURRENT_CONTINUE  ((uint8_t)0x0A)
#define MUSB_HUB_PORT_STATE_C_OVERCURRENT_COMPLETE  ((uint8_t)0x0B)

#define MUSB_HUB_PORT_STATE_WAIT_FOR_DEBOUNCE       ((uint8_t)0x0C)
#define MUSB_HUB_PORT_STATE_DEBOUNCE_GOT_STATUS     ((uint8_t)0x0D)
#define MUSB_HUB_PORT_STATE_START_RESET             ((uint8_t)0x0E)
#define MUSB_HUB_PORT_STATE_RESET_GOT_STATUS        ((uint8_t)0x0F)
#define MUSB_HUB_PORT_STATE_RESET_COMPLETE          ((uint8_t)0x10)
#define MUSB_HUB_PORT_STATE_RESET_COMPLETE_GOT_STATUS  ((uint8_t)0x11)
#define MUSB_HUB_PORT_STATE_DISCONNECT              ((uint8_t)0x12)
#define MUSB_HUB_PORT_WAIT_FOR_ENUMERATE            ((uint8_t)0x13)

/** 
 * Hub Power types, this is based on the bmAttributes.Self-Power
 * and Local Power Status of HubStatus 
 */
typedef enum {
    /** Bus Powered Hub */
    MUSB_HUB_BUS_POWERED             = 0x00,
    /** Self Powred Hub */
    MUSB_HUB_SELF_POWERED            = 0x01,
    /** Hub can operate in both the above modes and
        present mode is indicated by MUSB_HubStatusPowerTypes */
    MUSB_HUB_SELF_AND_BUS_POWERED    = 0x02
}MUSB_HubPowerTypes;

/** Local Power status of Hub */
typedef enum {
    /** Hub Local power is not good */
    MUSB_HUB_NOT_LOCAL_POWERED        = 0x00,
    /** Hub Local Power is Good */
    MUSB_HUB_LOCAL_POWERED            = 0x01
}MUSB_HubStatusPowerTypes;

/** 
 * Power Updated event types where Allocated power has 
 * to be sheduled/updated
 */
typedef enum {
    /** Device/Hub connected at a port of the hub */
    MUSB_HUB_PORT_CONNECTED            = 0x00,
    /** Device/Hub disconnected at a port of the hub */
    MUSB_HUB_PORT_DISCONNECTED         = 0x01,
    /** Port suspended */
    MUSB_HUB_PORT_SUSPENDED            = 0x02,
    /** Port Resumed */
    MUSB_HUB_PORT_RESUMED              = 0x03
}MUSB_HubPowerUpdateTypes;

/** Maximum number of load in terms of units 
  1 unit = 100mA, Bus powered hub can draw from upstream port */
#define MUSB_HUB_BUS_POWER_MAX_UNITS            0x05

/* Bit mask for Hub Status local power */
#define MUSB_HUB_STATUS_LOCAL_POWER_BM          0x01

/* Config.BmAttributes.Self-Power Bit mask */
#define MUSB_HUB_CONFIG_SELF_POWER_BM           0x40
/* Config.bmAttributes.RemoteWakeup */
#define MUSB_HUB_REMOTE_WAKEUP_BM               0x20

/** Hub TT class specific bmrequest values */
/** Hub Clear TT Buffer Request */
#define MUSB_HUB_CLEAR_TT_BUFFER_BMREQUEST      0x23
/** Hub Reset TT Request */
#define MUSB_HUB_RESET_TT_BMREQUEST             0x23
/** Hub Get TT state Request */
#define MUSB_HUB_GET_TT_STATE_BMREQUEST         0xA3
/** Hub Stop TT Request*/
#define MUSB_HUB_STOP_TT_BMREQUEST              0x23

/** Hub TT class specifc bRequest values */
/** Hub Clear TT Buffer Request */
#define MUSB_HUB_REQ_CLEAR_TT_BUFFER            0x08
/** Hub Reset TT Request */            
#define MUSB_HUB_REQ_RESET_TT                   0x09
/** Hub Get TT state Request */
#define MUSB_HUB_REQ_GET_TT_STATE               0x0A
/** Hub Stop TT Request*/
#define MUSB_HUB_REQ_STOP_TT                    0x0B

/** Bit masks for Hub-TT type */
#define MUSB_HUB_TT_TYPE_BM                     0x30
/** Current TT-Type */
#define MUSB_HUB_CURRENT_TT_TYPE_BM             0x03 


/** This tells that connected hub is root hub or a normal hub in the system.
 *  Root Hub: First hub connected to the system is called root hub.
 *  Normal hub : Hub which is connected to the system after root hub called 
 *  normal hub.
 */
typedef enum {
    MUSB_ROOT_HUB  = 0x01 ,
    MUSB_NORMAL_HUB = 0x02
}MUSB_HubType;

/** De-bounce time As per 7.1.7.3 : Minimum De-bounce interval is 100ms */
typedef enum {
    MUSB_HUB_MIN_DEBOUNCE_TIME          =  50, 
    MUSB_HUB_MAX_DEBOUNCE_TIME          =  100, 
    MUSB_HUB_INCREMENT_DEBOUNCE_TIME   =  50,
    MUSB_HUB_DEBOUNCE_MAX_ERROR_COUNT   =  0x03
}MUSB_DebounceDefines;

/** Reset Parameters like min, max reset time as well as num. of tries.
 * These are as per 10.2.8.1, 11.5.1.5, 7.1.7.5 and 11.10 in 
 * USB specification 2,0. All the timings in ms
 */
typedef enum {
    MUSB_HUB_MIN_RESET_TIME            = 50, 
    MUSB_HUB_INCREMENT_RESET_TIME     = 20,
    MUSB_HUB_MAX_RESET_TIME            = 100, 
    MUSB_HUB_MAX_RESET_TRIES           = 1
} MUSB_ResetDefines;


#define MUSB_FILL_CTRL_IRP(pCtrlIrp, BusAddress, pSetupPacket, OutLength, InBuffer, InLength, IrpComplete, CompleteParam) \
    {\
    (pCtrlIrp)->pOutBuffer  = (const uint8_t*) pSetupPacket;\
    (pCtrlIrp)->dwOutLength = OutLength;\
    (pCtrlIrp)->pInBuffer   = InBuffer;\
    (pCtrlIrp)->dwInLength  = InLength;\
    (pCtrlIrp)->pfIrpComplete = IrpComplete;\
    (pCtrlIrp)->pCompleteParam= CompleteParam;}

#endif



