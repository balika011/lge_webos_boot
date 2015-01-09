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
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#include "x_pdwnc_mt82.h"
#include "hw_pdwnc.h"
#include "x_bim_mt82.h"
#include "x_ckgen_mt82.h"
//#include "x_os.h"
#else
#include "x_pdwnc.h"
#include "x_bim.h"
#include "x_ckgen.h"
#include "x_os.h"
#endif
#include "drvcust_if.h"
#include "x_lint.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "pdwnc_config.h"
#include "pdwnc_debug.h"
#include "pdwnc_entry.h"
#include "pdwnc_t8032.h"
#include "pdwnc_gpio.h"
#include "pdwnc_drvif.h"
#include "ir_if.h"

#include "x_timer.h"

//#undef PDWNC_WRITE32
//#undef PDWNC_READ32

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


#define T8032_ACK_TIMEOUT 2000
#define T8032_ALIVE_CHECK_RETRY 3// 100
#define T8032_ALIVE_CHECK_INTERVAL 10 // ms
#define T8032_MSGQ_CAPACITY 16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//extern void HAL_Delay_us(UINT32 u4Micros);
//-----------------------------------------------------------------------------
// Prototypes
//-----------------------------------------------------------------------------
//INT32 _PDWNC_SetT8032(UINT32 u4State);
//INT32 PDWNC_InitT8032(UINT32 u4Addr, UINT32 u4Size);
//INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T *prCmd, PDWNC_T8032_RESPONSE_T *prResponse);
//INT32 PDWNC_ReadSwVersion(UINT32* pu4SwVersion);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
static HANDLE_T _hT8032IsrMsgq;
/*static*/ HANDLE_T _hT8032CmdMutex;
#endif
///*static*/ UINT8 _au1CecData[CEC_XDATA_SIZE];


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _CopyToT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{//for instruction ram copy
    UINT32 i;
  
    volatile UINT32* pu1Ptr = (volatile UINT32*)u4RiscAddr;
    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);
    for(i = 0; i < u4Size; i+=4)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu1Ptr);
        pu1Ptr++;
    }    
    return 0;
}

/*static*/ INT32 _CopyToT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{//for xdata copy
//though we can only write one byte data into xdata with bus operation, we still need to use DW access for the bus operation! 
    UINT32 i;
    volatile UINT8* pu4Ptr = (volatile UINT8*)u4RiscAddr;
    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);
    for(i = 0; i < u4Size; i++)
    {
        vIO32Write4B(PDWNC_UP_DATA, *pu4Ptr);
        pu4Ptr++;
    }
    return 0;
}


static INT32 _CopyFromT8032(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    volatile UINT32* pu1Ptr = (volatile UINT32*)u4RiscAddr;

    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i+=4)
    {
        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        pu1Ptr++;  
    }   
    return 0;
}


/*static*/ INT32 _CopyFromT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr)
{
    UINT32 i;
    volatile UINT8* pu1Ptr = (volatile UINT8*)u4RiscAddr;

    vIO32Write4B(PDWNC_UP_ADDR, u4XAddr);

    for(i = 0; i < u4Size; i++)
    {
//        *pu1Ptr = u4IO32Read4B(PDWNC_UP_DATA);
        *pu1Ptr = u1IO32Read1B(PDWNC_UP_DATA);
        pu1Ptr++;  
    }   
    return 0;
}

static INT32 _LoadCode(UINT32 u4Addr, UINT32 u4Size)
{
    if(u4Addr == 0)
    {
        return -1;
    }
    return _CopyToT8032(0x0, u4Size, u4Addr);
}

static INT32 _T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse)
{
    INT32 i4Ret = 0;
    UINT8 u1Cmd, u1SubCmd;
    UINT32 u4Data;
#ifndef CC_MTK_LOADER  
	UINT16 i2MsgNum;  
    UINT16 u2Idx = 0;
    SIZE_T u4Size = sizeof(UINT32);
    UINT32 u4Msg;
#endif    

    u4Data = DRVCUST_InitGet(eT8032uPOffset);
    if (u4Data == 0)  // here needs to setup 8032uP.
    {
        Printf("Warning! T8032 is not active!");
        return 0;
    }

    VERIFY(prCmd != NULL);
	
    u1Cmd = prCmd->u1Cmd;
    u1SubCmd = prCmd->u1SubCmd;
    u4Data = prCmd->au1Data[0] | (prCmd->au1Data[1] << 8) | (prCmd->au1Data[2] << 16) | (prCmd->au1Data[3] << 24);    
     vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
#ifndef CC_MTK_LOADER
    PDWNC_T8032CMD_SET_CMD(u1Cmd | (u1SubCmd << 8));
    PDWNC_T8032CMD_SET_DATA(u4Data);
	do{
		VERIFY(x_msg_q_num_msgs(_hT8032IsrMsgq, &i2MsgNum) == OSR_OK);
		if(i2MsgNum != 0)
		{
			Printf("Warning! _hT8032IsrMsgq is not empty");
			VERIFY(x_msg_q_receive_timeout(&u2Idx, &u4Msg, &u4Size, &_hT8032IsrMsgq, 1, T8032_ACK_TIMEOUT) == OSR_OK);
			Printf("...flush Msg(%d)\n", u4Msg);				
		}
	}while(i2MsgNum != 0);        
	vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);


    // If the command is not ENTER_STANDBY, we need ACK 
    if((u1Cmd != PDWNC_CMD_ARMT8032_ENTER_STANDBY)
	#if defined(CC_SUPPORT_STR_CORE_OFF)
		&&(u1Cmd != PDWNC_CMD_ARM8032_ENTER_SUSPEND)
	#endif
	)
    {
        if (x_msg_q_receive_timeout(&u2Idx, &u4Msg, &u4Size, &_hT8032IsrMsgq, 1, T8032_ACK_TIMEOUT) != OSR_OK)
        {
            Printf("T8032 interrupt error: ack timeout!(Cmd = 0x%x, SubCmd = 0x%x)\n", u1Cmd, u1SubCmd);

            // Ack timeout
            return -2;
        }
        if((u4Msg & 0xff) == u1Cmd)
        {
            if(prResponse != NULL)
            {
                prResponse->u1Ack = (UINT8)(u4Msg & 0xff);
                prResponse->au1Data[0] = (UINT8)((u4Msg >> 8) & 0xff);
                prResponse->au1Data[1] = (UINT8)((u4Msg >> 16) & 0xff);
                prResponse->au1Data[2] = (UINT8)((u4Msg >> 24) & 0xff);            
            }
        }
        else
        {      
            Printf("T8032 interrupt error: ack mismatch!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd, (UINT8)(u4Msg & 0xff));
            // Ack mismatch                    
            i4Ret = -1;                
        }
        
    }
#else    
    PDWNC_T8032CMD_SET_CMD(u1Cmd | ((UINT32)u1SubCmd << 8));
    PDWNC_T8032CMD_SET_DATA(u4Data);
    // If the command is not ENTER_STANDBY, we need ACK 
    if(u1Cmd != PDWNC_CMD_ARMT8032_ENTER_STANDBY)
    {           
        // Wait until T8032 enter non-xdata mode
        UINT32 u4Val = 0;        
        UINT32 i = 0, j = 0;
        
        while(1)
        {          
            vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
            
            while(i < 10000)
            {
                HAL_Delay_us(100);                  
                if(IO32ReadFldAlign(PDWNC_INTSTA, FLD_AUX_INT_STA))
                {
                    u4Val = PDWNC_T8032CMD_GET_RESPONSE();
                    if((u4Val & 0xff) == u1Cmd)
                    {
                        if(prResponse != NULL)
                        {
                            prResponse->u1Ack = (UINT8)(u4Val & 0xff);
                            prResponse->au1Data[0] = (UINT8)((u4Val >> 8) & 0xff);
                            prResponse->au1Data[1] = (UINT8)((u4Val >> 16) & 0xff);
                            prResponse->au1Data[2] = (UINT8)((u4Val >> 24) & 0xff);            
                        }                            
                    }
                    else
                    {      
                        Printf("T8032 polling error: ack mismatch!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                        // Ack mismatch
                        i4Ret = -1;                
                    }
                                       
                    break;                
                }      
                i++;
            }
                                 
            if(i < 10000)
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                break;
            }            
            else
            {
                	vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
                i = 0;
            }
            
            if(++j > 2 /*T8032_ACK_TIMEOUT*/)
            {
                Printf("T8032 polling error: ack timeout!(Cmd = 0x%x, SubCmd = 0x%x, Ack = %d)\n", u1Cmd, u1SubCmd,(UINT8)(u4Val & 0xff));
                // Ack timeout
                i4Ret = -2;
                break;
            }
          
        }
    }
    else
    {
        vIO32WriteFldAlign(PDWNC_ARM_INT, 0x1, FLD_ARM_INT);
    }
#endif    
    
    return i4Ret;    

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/**  is the interrupt routine for GPIO pins of PDWNC module.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER  
void _PDWNC_T8032Isr(UINT16 u2Vector)
{
    UINT32 u4Val;
    UINT32 u4Size = sizeof(UINT32); 
    UINT16 u2Num;
    	
    x_msg_q_num_msgs (_hT8032IsrMsgq, &u2Num);
    if(u2Num >= T8032_MSGQ_CAPACITY)
    {
        return;
    }
    
    u4Val = PDWNC_T8032CMD_GET_RESPONSE();//u4IO32Read4B(PDWNC_SHREG0);
    x_msg_q_send(_hT8032IsrMsgq, &u4Val, u4Size, 100);    
    
}
//-----------------------------------------------------------------------------
/**  PDWNC_SetT8032 set T8032 to the specified state.
 *  @param u4State T8032 state to set
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_RegT8032Isr(void)
{
    void (* pfnOldIsr)(UINT16);
    VERIFY(x_msg_q_create(&_hT8032IsrMsgq, "t8032msg", sizeof(UINT32), T8032_MSGQ_CAPACITY) == OSR_OK);
	VERIFY(x_sema_create(&_hT8032CmdMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);    
    // Hook T8032 ISR
    VERIFY(PDWNC_RegIsr((UINT16)PDWNC_INTNO_AUX_INT, _PDWNC_T8032Isr, &pfnOldIsr) == PDWNC_OK);    
    return 0;
}
#endif


//-----------------------------------------------------------------------------
/**  PDWNC_SetupCEC to setup CEC wakeup before entering standby.
 *  @param void
 */
//-----------------------------------------------------------------------------

INT32 _PDWNC_SetupCEC(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data)
{

    INT32 i4Ret = 0;

#ifndef CC_MTK_LOADER
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM);

    // Copy CEC data to XData
    i4Ret |= _CopyToT8032_XDATA(u4XAddr, u4Size, (UINT32)pu1Data);//_PDWNC_TransferCECData(PDWNC_CEC_DATA_TO_T8032);

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032);


#ifndef CC_MTK_LOADER
    x_sema_unlock(_hT8032CmdMutex);
#endif
    
    return i4Ret; 
}

INT32 _PDWNC_GetT8302CECData(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data)
{

    INT32 i4Ret = 0;

#ifndef CC_MTK_LOADER
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT); //make sure no other thread can interrupt T8032 in this critical time
#endif
    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM);

    // Copy CEC data to XData
    i4Ret |= _CopyFromT8032_XDATA(u4XAddr, u4Size, (UINT32)pu1Data);//_PDWNC_TransferCECData(PDWNC_CEC_DATA_TO_ARM);

    i4Ret |= _PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032);

#ifndef CC_MTK_LOADER
    x_sema_unlock(_hT8032CmdMutex);
#endif
    
    return i4Ret; 
}


//-----------------------------------------------------------------------------
/**   _PDWNC_RequestT8032TurnOffSys request T8032 to entern standby mode
 */
//-----------------------------------------------------------------------------
static INT32 _PDWNC_RequestT8032TurnOffSys(void)
{
    INT32 i4Ret = 0;
    PDWNC_T8032_CMD_T rCmd;
	UINT32 i;
   // x_memset(&rCmd, 0, sizeoof(PDWNC_T8032_CMD_T));
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;
   if(IO32ReadFldAlign(PDWNC_UP_CFG, FLD_XDATA_ACC))
   {
      vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC); //let T8032 control xdata
   }
   //david : Switch operation of  CEC, UART, ENGINE are not necessary in MT5387/63
   vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);

		
#if defined(CC_SUPPORT_STR_CORE_OFF)
	if(PDWNC_IsSuspend())
	{		
		rCmd.u1Cmd = PDWNC_CMD_ARM8032_ENTER_SUSPEND;		
	}
	else
#endif
	{
    	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_STANDBY;
	}
#ifdef CC_MTK_LOADER
    rCmd.u1SubCmd = 0x1; // AC_ON->Standby
#else
    rCmd.u1SubCmd = 0x2; // AC_ON->Normal->Standby
#endif
    
    // Signal 8032 to enter standby
    i4Ret = _T8032Cmd(&rCmd, NULL);

    LOG(7, "Signal T8032 to write power down entry code!\n"); 
	
	#if defined(CC_SUPPORT_STR_CORE_OFF)
	if(!PDWNC_IsSuspend())
	#endif
	for(i=0;i<T8032_ACK_TIMEOUT;i++)
	{
   		 HAL_Delay_us(1000);
	}

    return i4Ret;
}

//-----------------------------------------------------------------------------
/**  PDWNC_SetT8032 set T8032 to the specified state.
 *  @param u4State T8032 state to set
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_SetT8032(UINT32 u4Set)
{
    INT32 i4Ret = 0;
    UINT32 u4Val;   
    PDWNC_T8032_CMD_T rCmd;
	
	x_memset((void *)&rCmd, 0, sizeof(PDWNC_T8032_CMD_T));
	
    // PDWNC_T8032_RESPONSE_T rResponse;    
    if(u4Set == PDWNC_T8032_SET_STANDBY)
    {//david: this path will not go through OS API (x_sema_lock)
        return _PDWNC_RequestT8032TurnOffSys();
    }    
#ifndef CC_MTK_LOADER
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT);
#endif
    
    if(u4Set == PDWNC_T8032_SET_XDATA_ARM)    
    {       
        u4Val = IO32ReadFldAlign(PDWNC_UP_CFG,FLD_T8032_RST);
        
        // If T8032 is inactive (reset)             
        if(u4Val)
        {
            LOG(7, "XDATA belongs to ARM already!\n");                        
#ifndef CC_MTK_LOADER
            x_sema_unlock(_hT8032CmdMutex);
#endif             
            return 0;
        }
        // If T8032 is active (reset released)    
        else
        {        
          rCmd.u1Cmd = PDWNC_CMD_ARMT8032_XDATA_SWITCH;
          rCmd.u1SubCmd = 0x1;

          MSET_XDATA_TO_ARM_FLAG();

          //Printf("Set XDATA to ARM!\n");
          // Signal 8032 to change to non-xdata mode
          _T8032Cmd(&rCmd, NULL);
            // Really set xdata access right to ARM
          vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
          LOG(7, "Switch XDATA to ARM!\n");            
        }

    }
    else if(u4Set == PDWNC_T8032_SET_XDATA_T8032)
    {

        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);//switch xdata to T8032
        MCLR_XDATA_TO_ARM_FLAG();
        LOG(7, "Switch XDATA to T8032!\n");    

        
    }    	
    else if(u4Set == PDWNC_T8032_SET_ENGINE_ARM)//useless in MT5387
    {
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENGINE_SWITCH;
        rCmd.u1SubCmd = 0x1;

        // Signal 8032 to release other engine control right
        if(_T8032Cmd(&rCmd, NULL) != PDWNC_OK)
        {
            LOG(5, "Switch other engine to ARM error: T8032 not response!\n");
            i4Ret = -1;
        }   
   	  // Disable 8032 engine access   	  

        u4Val = u4IO32Read4B(PDWNC_UP_CFG);
        u4Val &= ~(1U << 16);   	    	 
        vIO32Write4B(PDWNC_UP_CFG, u4Val);
        LOG(7, "Switch other engine to ARM successfully!\n");        
    }    
    else if(u4Set == PDWNC_T8032_SET_ENGINE_T8032)////useless in MT5387
    {
   	  // Disable 8032 engine access   	  
        u4Val = u4IO32Read4B(PDWNC_UP_CFG);
        u4Val |= (1U << 16);   	    	 
        vIO32Write4B(PDWNC_UP_CFG, u4Val);
        
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENGINE_SWITCH;
        rCmd.u1SubCmd = 0x2;    
        
        // Signal 8032 to get other engine control right        
        if(_T8032Cmd(&rCmd, NULL) != PDWNC_OK)
        {
            LOG(5, "Switch other engine to T8032 error: T8032 not response!\n");
            i4Ret = -1;
        }

        LOG(7, "Switch other engine to T8032 successfully!\n");        
    }        
    else if(u4Set == PDWNC_T8032_SET_NORMAL)
    {
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_NORMAL;
        rCmd.u1SubCmd = 0x2;
        
        // Signal 8032 to enter normal mode
        _T8032Cmd(&rCmd, NULL);
        
        LOG(7, "Signal T8032 to switch to normal mode!\n"); 
    }
    else
    {
        i4Ret = -1;
    }        

#ifndef CC_MTK_LOADER
    x_sema_unlock(_hT8032CmdMutex);
#endif
    
    return i4Ret;
}

//-----------------------------------------------------------------------------
/**  PDWNC_InitT8032 to initialize T8032.
 *  @param void.
 */
//-----------------------------------------------------------------------------
INT32 _PDWNC_InitT8032(UINT32 u4Addr, UINT32 u4Size)
{
    INT32 i4Ret = 0;
    UINT32 u4Val;    
    UINT32 u4IrrxType;
    UINT32 i = 0;
    vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_AUX_INTEN);

    // Enable IR interrupt & wakeup

    i4Ret = DRVCUST_InitQuery(eIrrxType, &u4IrrxType);   
    i4Ret = IRRX_InitMtkIrRegSetup(u4IrrxType);   

    vIO32WriteFldAlign(PDWNC_ARM_INTEN, 0x1, FLD_IR_INTEN);
    
    vIO32WriteFldAlign(PDWNC_WAKEN, 0x1, FLD_IR_WAKEN);
    // Set reset count from 16/27 seconds to 5/27 seconds
    vIO32Write4B(PDWNC_WKRSC, DRVCUST_InitGet(eSysResetTimer));
	Printf("IR DATA register : 0x%8x\n",u4IO32Read4B(PDWNC_IRDATA0));
    // If A/C on, load uCode to T8032 except for mt8223 
    u4Val = PDWNC_ReadWakeupReason();

    if((u4Val == PDWNC_WAKE_UP_REASON_AC_POWER) || (u4Val == PDWNC_WAKE_UP_REASON_WATCHDOG))

    {    
        UINT32 u4Val;
        UINT8* pu1Buf = x_mem_alloc(u4Size);
        
        Printf("T8032 init A/C on case loader stage...\n");      
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
        vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
        // Load uP code              
        VERIFY(_LoadCode(u4Addr, u4Size) == 0);        
        
        _CopyFromT8032(0x0, u4Size, (UINT32)pu1Buf);
        if(x_memcmp((void*)u4Addr, (void*)pu1Buf, u4Size) != 0)
        {
            Printf("Load T8032 FW fail!!\n");
            return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
        
        Printf("Load T8032 FW (addr: 0x%8x, size: %d)success!!\n", u4Addr, u4Size);

#if PDWNC_T8032_UART_PD_ENABLE
		vIO32Write4B(PDWNC_PINMUX0, (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff) | 0x00003000);
		
        //Set UART transparent mode
        vIO32Write4B(PDWNC_STAB, 0xe2);//david: need to confirm...     
#endif        
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_XDATA_ACC);
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_VIR_EN);
	
        vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_T8032_RST);
        vIO32WriteFldAlign(PDWNC_INTCLR, 0x1, FLD_AUX_INTCLR);
        HAL_Delay_us(500); //wait T8032 init ready
        for(i = 0; i < T8032_ALIVE_CHECK_RETRY; i++)
        {
            if((i4Ret = PDWNC_ReadSwVersion(&u4Val)) == PDWNC_OK)
            {
                Printf("T8032 FW version: %d\n", u4Val);
                break;
            }
            Printf("T8032 FW version read error!\n");                        
            HAL_Delay_us(T8032_ALIVE_CHECK_INTERVAL);                                        
        }                     
    }

    if( i == T8032_ALIVE_CHECK_RETRY)
    {
    	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
    	vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
    	vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
        Printf("Read veriosn fail, set t8032 as reset state!\n");                        	
	    return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
    }
#ifndef CC_IR_POWER_BUTTON_RECEIVE_PRELOADER      
    else
    {

#if PDWNC_T8032_UART_PD_ENABLE
		vIO32Write4B(PDWNC_PINMUX0, (u4IO32Read4B(PDWNC_PINMUX0) & 0xffff8fff) | 0x00003000);
		
        //Set UART transparent mode
        vIO32Write4B(PDWNC_STAB, 0xe2);//david: need to confirm...     
#endif            
        PDWNC_T8032_CMD_T rCmd;
        rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENTER_NORMAL;
        rCmd.u1SubCmd = 1; // loader stage
        rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;  
        if(_T8032Cmd(&rCmd, NULL))
        {
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
            vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST);	
            vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
            Printf("Enter normal state fail, set t8032 as reset state!\n");                        	
            return 1;// keep vir_en = 0 && t8032_rst = 1 for T8032_ALIVE macro in pdwnc_drvif.h
        }
        else
        {
            Printf("T8032 change to loader stage...\n");             
         }    
    }       
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/* PDWNC_InitT8032() To initial T8032 uP with 8032 binary info.
 *
 * @param u4Offset  8032 binary pointer offset.
 * @param u4Size    8032 binary size.
 * @retval 0        SUCCESS.
 *  @retval   1		    FAIL.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_InitT8032(UINT32 u4Offset, UINT32 u4Size)
{
    return _PDWNC_InitT8032(u4Offset, u4Size);

}

//-----------------------------------------------------------------------------
/** PDWNC_T8032Cmd to issue cmd to T8032.
 *  @param u1Flag the power down flag to set.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse)
{
    INT32 i4Ret = 0;
#ifndef CC_MTK_LOADER
    x_sema_lock(_hT8032CmdMutex, X_SEMA_OPTION_WAIT);
#endif
    
    i4Ret = _T8032Cmd(prCmd, prResponse);

#ifndef CC_MTK_LOADER
    x_sema_unlock(_hT8032CmdMutex);
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** PDWNC_ReadSwVersion reads SW version from T8032.
 *  @param u2Gpio is the gpio number in PDWNC module to trigger interrupt.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ReadSwVersion(UINT32* pu4SwVersion)
{
    INT32 i4Ret = 0;
    PDWNC_T8032_CMD_T rCmd;
    PDWNC_T8032_RESPONSE_T rResponse;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_QUERY_SW_VERSION;
#ifdef CC_PDWNCONOFF_PATCH
	rCmd.u1SubCmd = 0xaa;
#else
    rCmd.u1SubCmd = 0x0;
#endif
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    // Signal 8032 to enter normal mode
    i4Ret = PDWNC_T8032Cmd(&rCmd, &rResponse);

    *pu4SwVersion = (UINT32)rResponse.au1Data[0];

    return i4Ret;
}
//-----------------------------------------------------------------------------
/** PDWNC_ReadT8032WakeupReason reads wakeup reason & subreason from T8032.
 */
//-----------------------------------------------------------------------------
INT32 PDWNC_ReadT8032WakeupReason(UINT32* pu4WakReason , UINT32* pu4SubReason)
{
    INT32 i4Ret = 0;
    PDWNC_T8032_CMD_T rCmd;
    PDWNC_T8032_RESPONSE_T rResponse;

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_READ_WAKEUP_REASON;
    rCmd.u1SubCmd = 0x0;
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    // Signal 8032 to enter normal mode
    i4Ret = PDWNC_T8032Cmd(&rCmd, &rResponse);

    *pu4WakReason = (UINT32)rResponse.au1Data[0];
	if(pu4SubReason != NULL)
	{
		*pu4SubReason = (UINT32)rResponse.au1Data[1];
	}
    return i4Ret;
}


//-----------------------------------------------------------------------------
/** PDWNC_SetUartPDBaudrate to set system error code.
 *  @param u1ErrorCode the error code.
 */
//-----------------------------------------------------------------------------

INT32 PDWNC_SetUartPDBaudrate(UINT8 u1baudrate)
{
    PDWNC_T8032_CMD_T rCmd;
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_UART_PD_BAUDRATE;
    rCmd.u1SubCmd = u1baudrate;
    rCmd.au1Data[0] = rCmd.au1Data[1]= rCmd.au1Data[2] = rCmd.au1Data[3] = 0;    
    return PDWNC_T8032Cmd(&rCmd, NULL);
}

#if 0 //defined(CC_ENABLE_REBOOT_REASON) && defined(ANDROID) && defined(CC_EMMC_BOOT)

INT32 PDWNC_Write_T8032_XData(UINT32 u4XAddr, UINT32 u4Size, const void *u4RiscAddr)
{
    int ret = 1;

    if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
    {
            return 0;
    }

    // Copy data to XData
    if(_CopyToT8032_XDATA(u4XAddr, u4Size, (UINT32)u4RiscAddr))
    {
            ret = 0;
    }

    if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
    {
            ret = 0;
    }

    return ret;
}

INT32 PDWNC_Read_T8032_XData(UINT32 u4XAddr, UINT32 u4Size, const void *u4RiscAddr)
{
    int ret = 1;

    if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_ARM))
    {
            return 0;
    }

    // Copy data to XData
    if(_CopyFromT8032_XDATA(u4XAddr, u4Size, (UINT32)u4RiscAddr))
    {
            ret = 0;
    }

    if(_PDWNC_SetT8032(PDWNC_T8032_SET_XDATA_T8032))
    {
            ret = 0;
    }

    return ret;
}
#endif

