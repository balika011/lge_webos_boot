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

// === INCLUDE =============================================================================
#include "drv_tdtv_drvif.h"
#include "drv_video.h"
#include "x_gpio.h"
#include "x_pinmux.h"

// === HW INCLUDE =============================================================================

// === DEFINE =============================================================================
enum
{
    VDO_2D,
    VDO_3D_FS,
    VDO_3D_SBS,
    VDO_3D_TAB,
    VDO_3D_TTD, 
    VDO_3D_SS
};

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================


// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================
extern HANDLE_T h4k2kUnMuteTimer ;
extern HANDLE_T _hFrcUnmuteSema ;
// === STATIC VARIABLE ====================================================================
static UINT8 u1Vdo3DFmt=VDO_2D;
// === END OF STATIC VARIABLE =============================================================
// === BODY ===============================================================================
#if defined( CC_SUPPORT_4K2K)||defined(CC_SUPPORT_HDMI_4K2K30)
#ifndef CC_CMI_MST_FRC_SUPPORT
#define CC_CMI_MST_FRC_SUPPORT	1
#endif
#ifndef CC_CMI_RTK_FRC_SUPPORT
#define CC_CMI_RTK_FRC_SUPPORT	1
#endif

#include "sif_if.h"
#include "drv_display.h"


r4k2kHandler *psCur4k2kHandler=NULL ;
static HANDLE_T _hFrcSetCmdSema=(HANDLE_T)(NULL);
void com4k2kSendPanelType(UINT32 u4PanelType);
void com4k2kSendPanelMode(UINT8 u1Mode);
void com4k2kSendCmd(UINT8 *pBuf, UINT16 u2Len);



#ifdef CC_CMI_MST_FRC_SUPPORT
#include "drv_cmiMst4k2k.c"
#endif
#ifdef CC_CMI_RTK_FRC_SUPPORT
#include "drv_cmiRtk4k2k.c"
#endif

#ifdef CC_CSOT_NTK_FRC_SUPPORT
#include "drv_csotNtk4k2k.c"
#endif


void com4k2kSendCmd(UINT8 *pBuf, UINT16 u2Len)
{
    UINT8 u1Checksum = 0, i, *pTmp, u1CmdLen, k ;
    UINT32 iret ;

	if(!psCur4k2kHandler)
	{
		return;
	}
	VERIFY (x_sema_lock(_hFrcSetCmdSema, X_SEMA_OPTION_WAIT) == OSR_OK);	

	pTmp = pBuf ;
	u1CmdLen = psCur4k2kHandler->u1CmdLen ;
	for(k=0; k < u2Len ; k=k+u1CmdLen)
	{
		u1Checksum = 0;
#ifdef CC_CMI_MST_FRC_SUPPORT		
		if(psCur4k2kHandler->u1Tag == E_4K2K_CMI_MST)
		{
		    for(  i=1 ; i< u1CmdLen ; i++)
		    {
				//skip data 0
					u1Checksum += pTmp[i] ;
		    }
		}    
#endif		
#ifndef NDEBUG			    
		Printf("\n[4k2k] I2c Data :");
#endif
#ifdef CC_CMI_MST_FRC_SUPPORT
		if(psCur4k2kHandler->u1Tag == E_4K2K_CMI_MST)
		{
	    	pTmp[0] = (pTmp[0] &0xF) + ((u1Checksum & 0xF) << 4) ; //upper 4 bit ;
	    }	
#endif	    
#ifndef NDEBUG			 
		 for(i=0 ; i < u1CmdLen ; i++)
		 {
			Printf("0x%x ", pTmp[i]);
		 }
		 Printf("\n");
#endif		 
	     iret = SIF_X_Write(psCur4k2kHandler->u1I2CChn, 0x100 , psCur4k2kHandler->u1I2cAddr, 0 ,0, pTmp, u1CmdLen);
	     if(iret <=0)
	     {
			Printf("[4k2k] i2c send error\n");

	     }
	     pTmp = pTmp + u1CmdLen ;
	     //command delay
	     x_thread_delay(psCur4k2kHandler->u2CmdInterval);
     }
	x_sema_unlock(_hFrcSetCmdSema);
}

void com4k2kSendPanelType(UINT32 u4PanelType)
{
    static UINT32 u4PrePanelType =0xff;
    UINT16 u2Len=0;
    UINT8 *pBuf;
    
	if(!psCur4k2kHandler)
	{
		return;
	}

    pBuf = NULL ;
#ifdef CC_CSOT_NTK_FRC_SUPPORT

	if(psCur4k2kHandler->u1Tag == E_4K2K_CSOT_NTK)
	{
		if (u1Vdo3DFmt != VDO_2D)  // NT previous mode=3D
		{
			APP_FRC_Set_314_Skyworth_Exit3D();
			Printf("[4K2K][NT]exit 3D\n");
		}
	}
#endif    
    if(u4PanelType != u4PrePanelType)
    {
    	
        if ((u4PanelType == PANEL_4K2K_FHD120)||(u4PanelType == PANEL_4K2K_FHD60))
        {
#ifdef CC_CSOT_NTK_FRC_SUPPORT        
        	if(psCur4k2kHandler->u1Tag == E_4K2K_CSOT_NTK)
        	{
				if (u4PanelType == PANEL_4K2K_FHD60)  // 2D
				{
					App_SET_4K2K_TO_FHD();
				}
				else if (u4PanelType == PANEL_4K2K_FHD120)  // 3D
				{
					APP_FRC_Set_314_Skyworth_Enter3D();
					Printf("[4K2K][NT]enter 3D\n");
				}
			}
			else
#endif			
			{
				if(psCur4k2kHandler->u13D2DTimingChg && (u4PanelType == PANEL_4K2K_FHD120))
				{
            		pBuf = psCur4k2kHandler->pu1Fhd3DBuf;            	
					u2Len = psCur4k2kHandler->u1Fhd3DBufSize;

				}
				else
				{
            		pBuf = psCur4k2kHandler->pu1FhdBuf;            	
					u2Len = psCur4k2kHandler->u1FhdBufSize;
				}
				
            }	
        	Printf("[4K2K][LVDS]I2C send FHD mode\n");
        }
        else if (u4PanelType == PANEL_4K2K_30)
        { 
#ifdef CC_CSOT_NTK_FRC_SUPPORT         
        	if(psCur4k2kHandler->u1Tag == E_4K2K_CSOT_NTK)
        	{
					App_SET_FHD_TO_4K2K();				
        	}
        	else
#endif        	
        	{
				pBuf = psCur4k2kHandler->pu14k2kBuf; 	
				u2Len = psCur4k2kHandler->u14k2kBufSize;				
			}	
        	Printf("[4K2K][LVDS]I2C send 4K2K30 mode\n");
        }
        if(psCur4k2kHandler && psCur4k2kHandler->frc_send_cmd)
        {
        	psCur4k2kHandler->frc_send_cmd(pBuf, u2Len);       
        }	
        
        u4PrePanelType = u4PanelType ;        
    }
}

void com4k2kSendPanelMode(UINT8 u1Mode)
{
    static UINT8 u1PreMode = 0xff ;
    UINT8 *pBuf ;
    UINT16 u2Len;
    BOOL fgSkipCmd=0;

	if(!psCur4k2kHandler)
	{
		return;
	}

    if((u1Mode == E_4K2K_NATIVE) || (u1Mode == E_4K2K_NORMAL_3D))
    {
		u1Mode = E_4K2K_NORMAL ;
		//mst use the default 60 or 120 panel as resume command , skip the resume command if 4k2k native mode
      	if(psCur4k2kHandler->u1Tag == E_4K2K_CMI_MST)
      	{
			fgSkipCmd = TRUE ;
      	}
    }    
    if(u1Mode != u1PreMode)
    {
        if (u1Mode == E_4K2K_SPLIT_PHOTO)
        {
        	pBuf = psCur4k2kHandler->pu1PhotoModeBuf;
        	u2Len = psCur4k2kHandler->u1PhotoBufSize;
        	Printf("[4K2K][LVDS]I2C send split photo mode #2\n");
        }
        else if (u1Mode == E_4K2K_SPLIT_VDO)
        {
        	pBuf = psCur4k2kHandler->pu1VdoModeBuf;
        	u2Len = psCur4k2kHandler->u1VdoBufSize;        	
        	Printf("[4K2K][LVDS]I2C send split vdo mode #2\n");
        }
        else // normal mode & native mode
        {        	
        		pBuf = psCur4k2kHandler->pu1ResumeBuf;
        	u2Len = psCur4k2kHandler->u1ResumeBufSize;        	        		
        		Printf("[4K2K][LVDS]I2C send non-split normal mode #2\n");
        }
        
        
        if(!fgSkipCmd)
        {
        	psCur4k2kHandler->frc_send_cmd(pBuf , u2Len);
        }
        u1PreMode = u1Mode ;
    }
}



//==============================================================================
/* Init function pointer table via panel id  */

void vExt4k2kFrcInit(void)
{

	switch(GetCurrentPanelIndex())
	{
#ifdef CC_CMI_RTK_FRC_SUPPORT		
	case PANEL_CMI_4K2K_FHD120:
		psCur4k2kHandler = &cmiRtk4k2kHandler;	
		Printf("[4k2k]CMI+RTK 4k2k FRC handler Init\n");
		break;
#endif		
#ifdef CC_CMI_MST_FRC_SUPPORT			
	case PANEL_CMO_V580DK1_LS1_INX:
		psCur4k2kHandler = &cmiMst1204k2kHandler;
		Printf("[4k2k]CMI+MST 4k2k FRC 120 handler Init\n");
		break;
	case PANEL_CMO_V580DK1_LS1_INX_VB1:		
		psCur4k2kHandler = &cmiMst120VB14k2kHandler;
		Printf("[4k2k]CMI+MST 4k2k FRC 120 VB1 handler Init\n");
		break;
	case PANEL_AUO_4K2K_FHD60:
		psCur4k2kHandler = &cmiMst4k2kHandler;
		Printf("[4k2k]CMI+MST 4k2k FRC 60 handler Init\n");		
		break;
	case PANEL_CMO_V580DK1_LS1_INX_60_VB1:		
		psCur4k2kHandler = &cmiMst60VB14k2kHandler;
		Printf("[4k2k]CMI+MST 4k2k FRC 60 VB1 handler Init\n");		
		break;
#endif		
#ifdef CC_CSOT_NTK_FRC_SUPPORT
	case PANEL_CSOT_4K2K_FHD60:
		psCur4k2kHandler = &csotNtk4k2kHandler;
		Printf("[4k2k]CSOT+NTK 4k2k FRC 60 handler Init\n");
		break;
#endif
	default:
		psCur4k2kHandler = NULL ;
		Printf("[4k2k]none handler Init\n");
		break;
	}
	if(psCur4k2kHandler && psCur4k2kHandler->frc_init)
	{
		if(!_hFrcSetCmdSema)
		{
			VERIFY(x_sema_create(&_hFrcSetCmdSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
		}	
		psCur4k2kHandler->frc_init();
	}
	
}

/* Dispatch the function handler */

void vSend4k2kFrcCmd(UINT8 *pBuf, UINT16 u2Len)
{
	if(psCur4k2kHandler && psCur4k2kHandler->frc_send_cmd)
	{
		psCur4k2kHandler->frc_send_cmd(pBuf, u2Len);
	}	
}
void vSend4k2kFrcPanelTypeCmd(UINT32 u4PanelType) 
{
	if(psCur4k2kHandler && psCur4k2kHandler->frc_snd_pnl_type)
	{
		psCur4k2kHandler->frc_snd_pnl_type(u4PanelType);
	}	
}    

void vSend4k2kFrcModeCmd(UINT8 u1Mode)
{
	if(psCur4k2kHandler && psCur4k2kHandler->frc_snd_pnl_mode)
	{
		psCur4k2kHandler->frc_snd_pnl_mode(u1Mode);
	}	
}
void vExt4k2kFrcMute(UINT8 bOn)    
{
	if(psCur4k2kHandler && psCur4k2kHandler->frc_snd_mute)
	{
		x_timer_stop(h4k2kUnMuteTimer);
		psCur4k2kHandler->frc_snd_mute(bOn);
	}	
}	
static VOID _FRCUnMuteTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
	x_sema_unlock(_hFrcUnmuteSema);
}
void vExt4k2kFrcUmMuteTimer(UINT16 u2Timer)    
{	
	//u2Timer ms 
	if(psCur4k2kHandler && psCur4k2kHandler->frc_snd_mute)
	{
	VERIFY(x_timer_start(h4k2kUnMuteTimer, u2Timer, X_TIMER_FLAG_ONCE,
            _FRCUnMuteTimeout, NULL) == OSR_OK);		
    }        
}	

BOOL fgExt4k2k2D3DTimingChg(void)
{
	if(psCur4k2kHandler)
	{
		return (BOOL) psCur4k2kHandler->u13D2DTimingChg;
	}
	return FALSE ;
}

void  DRVCUST_SetExtFrcBypassWindow(UINT8 bWId, UINT16 u2Width, UINT16 u2Height,  UINT16 u2StartX,  UINT16 u2StartY)
{
	if(psCur4k2kHandler && psCur4k2kHandler->frc_snd_protect_window)
	{
		psCur4k2kHandler->frc_snd_protect_window(bWId, u2Width, u2Height, u2StartX, u2StartY);
	}	
}

void DRVCUST_SplitPotoGpioSet(BOOL bOnOff)
{
	INT32 i4Val = 0;
	
#if defined(CC_MT5399)	
        BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION0);
        GPIO_Output(GPIO(53), &i4Val);
#elif defined(CC_MT5882)
        BSP_PinSet(PIN_OPCTRL5, PINMUX_FUNCTION0); //GPIO, org is L/R
        GPIO_Output(GPIO(204), &i4Val);
#endif
}
UINT16 u2Ext4k2kGetMuteTime(void)
{
	if(psCur4k2kHandler)
	{
		return psCur4k2kHandler->u2MuteTime ;
	}
	return 0 ;
}
#endif //CC_SUPPORT_4K2K






VOID DRVCUST_SetExtFrc3DFormat(E_TDTV_UI_3D_PACKING eUIPackingSel)
{
	 UINT32 u4Ext3DEnableGPIO;
	 static UINT8 u1PreVdo3DFmt=VDO_2D ;

    //UINT8 u1ErrMsg;
    static E_TDTV_UI_3D_PACKING ePreFMT=E_TDTV_UI_3D_MODE_END;

    if(bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
    {
        eUIPackingSel=E_TDTV_UI_3D_MODE_OFF;
    }
    
    if(ePreFMT!=eUIPackingSel)
    {    
        if(E_TDTV_UI_3D_MODE_RD==eUIPackingSel)
        {
            vOSTGSetBorderColor(VDP_1, 0x0); /*set color ==> black*/
            vOSTGSetBorderParam(VDP_1, 0, PANEL_GetPanelHeight()-4, PANEL_GetPanelWidth(), 4, 2);
            vOSTGSetBorderOnOff(VDP_1, SV_ON);
        }
        else
        {
            vOSTGSetBorderOnOff(VDP_1, SV_OFF);
        }    
        switch(eUIPackingSel)
        {
            case E_TDTV_UI_3D_MODE_FS:
            case E_TDTV_UI_3D_MODE_SBS:
            case E_TDTV_UI_3D_MODE_RD:     
            case E_TDTV_UI_3D_MODE_SS:
            case E_TDTV_UI_3D_MODE_LI:                
            case E_TDTV_UI_3D_MODE_CB:
            case E_TDTV_UI_3D_MODE_DA:
            case E_TDTV_UI_3D_MODE_TB:
            case E_TDTV_UI_3D_MODE_TTD:
                u1Vdo3DFmt=VDO_3D_FS;
                break;            
            default:
                u1Vdo3DFmt=VDO_2D;                
        }
		if(fgExt4k2k2D3DTimingChg())
		{
        	SetPanel_4k2k((u1Vdo3DFmt == VDO_2D)?E_4K2K_NORMAL:E_4K2K_NORMAL_3D);
        	if(bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE)
        	{
        		Printf("[4k2k] 3d mode change mode change/done\n");
        		vSetMainFlg(MAIN_FLG_MODE_CHG);
        		vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
        	}	
        }	
        else
        {
			//mute
			if(u1PreVdo3DFmt != u1Vdo3DFmt)
			{
				vExt4k2kFrcMute(SV_ON);
				vExt4k2kFrcUmMuteTimer(u2Ext4k2kGetMuteTime());
			}
			
        }
        u1PreVdo3DFmt = u1Vdo3DFmt ;
        ePreFMT=eUIPackingSel;
    }


    DRVCUST_OptQuery(e3DEnableGPIO,&u4Ext3DEnableGPIO);

    if((E_TDTV_UI_3D_MODE_OFF!=eUIPackingSel)&&
     (bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE))
    {        
        //Enable 3D Enable Signal
        GPIO_SetOut(u4Ext3DEnableGPIO, 1);
        
        //Enable 3D LR Signal
#if defined(CC_MT5399)  
        BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION1);
#elif defined(CC_MT5882)
        BSP_PinSet(PIN_OPCTRL5, PINMUX_FUNCTION7);
#endif

    }
    else
    {
        //Disable LR Signal Sync
#if defined(CC_MT5399)  
        BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION0);
#elif defined(CC_MT5882)
        BSP_PinSet(PIN_OPCTRL5, PINMUX_FUNCTION0);
#endif


        //Disable 3D Enable Signal    
        GPIO_SetOut(u4Ext3DEnableGPIO, 0);
    }
}







