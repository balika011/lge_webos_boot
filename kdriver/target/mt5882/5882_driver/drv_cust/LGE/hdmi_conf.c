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

/**
 * @file HDMI customization file:
 *
 * @author josh.pan@mediatek.com
 */
#include "x_gpio.h"
#include "x_printf.h"

#include "hw_vdoin.h"
#include "hw_hdmi.h"
#include "hw_pdwnc.h"

#include "drv_hdmi.h"
#include "drv_video.h"
#include "drv_adcset.h"

#include "sv_const.h"
#include "source_table.h"
#if defined(CC_MT5882)
#include "drv_cbus_cpi_if.h"
#endif
#define HDMI_GPIO_HPD0 (217)
#define HDMI_GPIO_HPD1 (222)
#define HDMI_GPIO_HPD2 (225)
//#define HDMI_GPIO_HPD3 (229)

#ifdef CC_LGE_PROTO_PCBA
#define HDMI_5V_DETECT0 (219)
#define HDMI_5V_DETECT1 (OPCTRL(1))
#define HDMI_5V_DETECT2 (OPCTRL(0))
#endif


extern E_HDMI_SWITCH_NUM eActiveHdmiPort;
//====================//
void vHDMIHPDHigh(E_HDMI_SWITCH_NUM eSwitch)
{ 
	vHdmiSetHpdStatus(eSwitch, 0);
       
	switch(eSwitch)
	{
#ifdef HDMI_GPIO_HPD0
		case HDMI_SWITCH_1:
#ifdef CC_LGE_PROTO_PCBA
			GPIO_SetOut(HDMI_GPIO_HPD0, 0);
#else
			if(_bIsMhlDevice == 0)
			{
				GPIO_GetIn(HDMI_GPIO_HPD0);
			}			
#endif
			break;

#endif
#ifdef HDMI_GPIO_HPD1			
		case HDMI_SWITCH_2:
#ifdef CC_LGE_PROTO_PCBA
			GPIO_SetOut(HDMI_GPIO_HPD1, 0);
#else
			GPIO_GetIn(HDMI_GPIO_HPD1);
#endif
			
			break;
#endif
#ifdef HDMI_GPIO_HPD2
		case HDMI_SWITCH_3:
#ifdef CC_LGE_PROTO_PCBA
			GPIO_SetOut(HDMI_GPIO_HPD2, 0);
#else
            GPIO_GetIn(HDMI_GPIO_HPD2);
#endif
			break;
#endif
#ifdef HDMI_GPIO_HPD3
		case HDMI_SWITCH_4:
			GPIO_GetIn(HDMI_GPIO_HPD3);
			break;
#endif			
		default: /* disable all */
			break;
	}
}

//====================//
void vHDMIHPDLow(E_HDMI_SWITCH_NUM eSwitch)
{

#ifndef CC_LGE_PROTO_PCBA
	INT32 i4Val = 0;
#endif
    vHdmiSetHpdStatus(eSwitch, 1);
       
	switch(eSwitch)
	{
#ifdef HDMI_GPIO_HPD0
		case HDMI_SWITCH_1:			
#ifdef CC_LGE_PROTO_PCBA
            GPIO_SetOut(HDMI_GPIO_HPD0, 1);
#else
			if(_bIsMhlDevice == 0)
			{
				GPIO_SetOut(HDMI_GPIO_HPD0, i4Val);
			}			
#endif
            break;
#endif			
#ifdef HDMI_GPIO_HPD1
		case HDMI_SWITCH_2:	
#ifdef CC_LGE_PROTO_PCBA
            GPIO_SetOut(HDMI_GPIO_HPD1, 1);
#else
            GPIO_SetOut(HDMI_GPIO_HPD1, i4Val);
#endif

			break;
#endif				
#ifdef HDMI_GPIO_HPD2
		case HDMI_SWITCH_3:	
#ifdef CC_LGE_PROTO_PCBA
 			GPIO_SetOut(HDMI_GPIO_HPD2, 1);
#else
			GPIO_SetOut(HDMI_GPIO_HPD2, i4Val);
#endif
			break;
#endif			
#ifdef HDMI_GPIO_HPD3
		case HDMI_SWITCH_4:
 			GPIO_SetOut(HDMI_GPIO_HPD3, i4Val);
			break;
#endif			
		default: /* disable all */
			break;
	}
}

//====================//
UINT8 bHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 input = 0;

    if (u1IsHdmiHpdPulse(ePort) == 1)
    {
        // hpd pulse period
        return (input);
    }
    
    switch(ePort)
    {
#ifdef HDMI_GPIO_HPD0
        case HDMI_SWITCH_1: 
			if(_bIsMhlDevice == 1)
			{
				input = 1;
				_bIsMhlDevice = 1;
			}
			else
			{
				_bIsMhlDevice = 0;
#ifdef CC_LGE_PROTO_PCBA
			input = GPIO_GetIn(HDMI_5V_DETECT0);
#else
			input = GPIO_GetIn(HDMI_GPIO_HPD0);
#endif 				
			}
	        break;
#endif
#ifdef HDMI_GPIO_HPD1
        case HDMI_SWITCH_2: 
#ifdef CC_LGE_PROTO_PCBA
			input = GPIO_GetIn(HDMI_5V_DETECT1);
#else
			input = GPIO_GetIn(HDMI_GPIO_HPD1);
#endif
			
			break;
#endif

#ifdef HDMI_GPIO_HPD2
        case HDMI_SWITCH_3: 
#ifdef CC_LGE_PROTO_PCBA
					input = GPIO_GetIn(HDMI_5V_DETECT2);
#else
					input = GPIO_GetIn(HDMI_GPIO_HPD2); 
#endif 
	         break;
#endif
#ifdef HDMI_GPIO_HPD3
        case HDMI_SWITCH_4: 
	          input = GPIO_GetIn(HDMI_GPIO_HPD3);
	         break;
#endif
	  default:
	         break;
	}
    
    return input;
}

//====================//
void vHDMISwitchInit(void)
{    
}

//====================//
void vHDMISetSwitch(E_HDMI_SWITCH_NUM eSwitch)
{
    E_HDMI_SWITCH_NUM i = HDMI_SWITCH_INIT;
    E_HDMI_SWITCH_NUM eHdmiRx;
	UINT8 u1PwOnInit = 0;

      // port mapping here 
      switch(eSwitch)
      {
          case HDMI_SWITCH_1:
                   eHdmiRx = HDMI_SWITCH_1;
  		break;
  				
          case HDMI_SWITCH_2:
			       eHdmiRx = HDMI_SWITCH_2;
  	       break;

          case HDMI_SWITCH_3:
                   eHdmiRx = HDMI_SWITCH_3;
  	       break;

          case HDMI_SWITCH_4:
                   eHdmiRx = HDMI_SWITCH_4;
  	       break;
  	       
          default:
          	      eHdmiRx = eSwitch;
  		break;
      }
	  
    if (eHDMICurrSwitch == HDMI_SWITCH_INIT)       
	{          		
		u1PwOnInit = 1;              
	}
#if defined(CC_MT5882)
	if(((eActiveHdmiPort == HDMI_SWITCH_1) && (eHdmiRx != HDMI_SWITCH_1))
		|| ((eActiveHdmiPort != HDMI_SWITCH_1) && (eHdmiRx == HDMI_SWITCH_1)))
	{
		u1FlgHdmiMhlSrcChged = 1;//MHL -> HDMI ->MHL, set to 1
	}
#endif	

	if (eHDMICurrSwitch != eHdmiRx)
	{	
		eHDMICurrSwitch = eHdmiRx;
              eActiveHdmiPort = eHdmiRx;
		
              if (u1IsHdmiResStable(eActiveHdmiPort) && u1IsHdmiRxShared(eActiveHdmiPort) == 0)
              {
                  vSetHdmiFastSw(eActiveHdmiPort, 1);
              }
		else
		{
		    vSetHdmiFastSw(eActiveHdmiPort, 0);
		    if (u1PwOnInit == 0)
			{
				vHDMIHPDLow(eActiveHdmiPort);
			}

		}
		
              vHDMISel(eHdmiRx);

		// HPD high for unselected ports, support CEC feature.
		for ( i = HDMI_SWITCH_1 ; i <= HDMI_SWITCH_4 ; i++) 
		{
			if (i != eHdmiRx)
			{
				vHDMIHPDHigh(i);
			}
		}
		
		#if MUTE_TEMP
		vHDMIVideoOutOff();
		#endif
		
        if (u1IsHdmiFastSw(eActiveHdmiPort) == 0)
        {
            if (u1PwOnInit == 0)
			{
	      		vSetHdmiState(HDMI_STATE_INIT);
            }

        }
         else
        {
	      vSetHdmiState(HDMI_STATE_SCDT);
        }

	}
}
UINT8 u1GetHDMIPort5VStatus(E_HDMI_SWITCH_NUM ePort)
{
    UINT32 input = 0;

	switch(ePort)
	{
#ifdef HDMI_GPIO_HPD0
	case HDMI_SWITCH_1: 
		if(_bIsMhlDevice == 1)
		{
			input = 1;
			_bIsMhlDevice = 1;
		}
		else
		{
			_bIsMhlDevice = 0;
#ifdef CC_LGE_PROTO_PCBA
			input = GPIO_GetIn(HDMI_5V_DETECT0);
#else
			input = GPIO_GetIn(HDMI_GPIO_HPD0);
#endif 			
		}
		break;
#endif
#ifdef HDMI_GPIO_HPD1
	case HDMI_SWITCH_2: 
#ifdef CC_LGE_PROTO_PCBA
			  input = GPIO_GetIn(HDMI_5V_DETECT1);
#else
			  input = GPIO_GetIn(HDMI_GPIO_HPD1);
#endif
		
		 break;
#endif

#ifdef HDMI_GPIO_HPD2
	case HDMI_SWITCH_3: 
#ifdef CC_LGE_PROTO_PCBA
		input = GPIO_GetIn(HDMI_5V_DETECT2);
#else
		input = GPIO_GetIn(HDMI_GPIO_HPD2); 
#endif 
		 break;
#endif
#ifdef HDMI_GPIO_HPD3
	case HDMI_SWITCH_4: 
		  input = GPIO_GetIn(HDMI_GPIO_HPD3);
		 break;
#endif
	default:
		 break;
	}

	return input;
}


