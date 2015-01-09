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

#ifdef CC_Factory2K12
#include "x_tuner.h"
#include "TDIS.h"
#include "PI_Def_dbg.h"

#ifdef CC_SUPPORT_STR
#include "drvapi_tuner.h"
extern STATE_INFO_T*           g_ptStateInfo;
#endif
extern BOOL fgHwTvdVPres(void);
BOOL Tuner_IsSignalLoss(VOID){
    #ifdef CC_SUPPORT_STR
    TUNER_CTX_T*    ptTunerCtx=NULL;
    if(g_ptStateInfo){
        ptTunerCtx = (TUNER_CTX_T*)g_ptStateInfo->ptParentTunerCtx;

        switch(g_ptStateInfo->e_conn_type)//ATV.Check VPress
        {
            case TUNER_CONN_TYPE_SAT_ANA: 
            case TUNER_CONN_TYPE_TER_ANA:
            case TUNER_CONN_TYPE_CAB_ANA:
            #ifdef TUNER_SUPPORT_SCART_OUT
            case TUNER_CONN_TYPE_SAT_ANA_SCART_OUT:
            case TUNER_CONN_TYPE_CAB_ANA_SCART_OUT:
            case TUNER_CONN_TYPE_TER_ANA_SCART_OUT:
            #endif
            mcSHOW_DBG_MSG(("ATV case:%d %d\n",fgHwTvdVPres(),(IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4));
            return !((IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4);//VPRESS 3D
            default:
            break;
        }
        return !TunerGetSync(ptTunerCtx->TunerID, g_ptStateInfo->e_conn_type);
    }else{
       return TRUE;
    }
    #else
    return TRUE;
    #endif
}

#ifdef SYS_DVBS_SUPPORT//factory command for DVBS

#define DEMOD_MOUNT_STATUS_MSK_BIT 0x01
#define LNB_MOUNT_STATUS_MSK_BIT 0x02

extern void SetRemapConnType(UINT16 TunerID,DRV_CONN_TYPE_T e_conn_type);
#ifndef CC_SONYDEMOD_CXD2842 //Adding this define is only for build error, Salvia_EU sysbuild run on Turnkey(MT5565 + DB10_V3)
extern UINT8 DVBS_DemodMountStatus(UINT8 TunerID);
#endif
 
enum
{
   FAC_LNB_POWER_OFF,
   FAC_LNB_POWER_13V,
   FAC_LNB_POWER_14V,
   FAC_LNB_POWER_18V,
   FAC_LNB_POWER_19V,
};
 
static void TunerDVBSFac_InitDisecCmd(DISEQC_BUS_CMD_T * pcmd,TUNER_SAT_DIG_LNB_STATUS_INFO_T * pLnbInfo)
{
    pcmd->ui1_polarity_13v_18v=pLnbInfo->ui1_polarity_13v_18v;
    pcmd->b_22k=pLnbInfo->b_22k;
    pcmd->ui1_lnb_power=pLnbInfo->ui1_lnb_power;
}
void TunerDVBSFac_SetLnbVol(UINT8 TunerID, UINT8 u1Vol)
{
    TUNER_SAT_DIG_LNB_STATUS_INFO_T t_lnb_info;
    SIZE_T z_len=sizeof(TUNER_SAT_DIG_LNB_STATUS_INFO_T);
    DISEQC_BUS_CMD_T disec_cmd;

    SetRemapConnType((UINT16)TunerID,TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK==TunerGetAttribute(TunerID,TUNER_GET_TYPE_LNB_STATUS,&t_lnb_info,&z_len)){
        TunerDVBSFac_InitDisecCmd(&disec_cmd,&t_lnb_info);
        disec_cmd.ui4_control_mask=DISEQC_CMD_CTRL_POLARIZATION_MASK|DISEQC_CMD_CTRL_LNB_POWER_MASK;
        if((FAC_LNB_POWER_13V==u1Vol) || (FAC_LNB_POWER_14V==u1Vol)){
            disec_cmd.ui1_polarity_13v_18v=0;
        }
        if((FAC_LNB_POWER_18V==u1Vol) || (FAC_LNB_POWER_19V==u1Vol)){
            disec_cmd.ui1_polarity_13v_18v=1;
        }
        if(FAC_LNB_POWER_OFF==u1Vol){
            disec_cmd.ui1_lnb_power=0;
        }
        if((FAC_LNB_POWER_13V==u1Vol) || (FAC_LNB_POWER_18V==u1Vol)){
            disec_cmd.ui1_lnb_power=1;
        }
        if((FAC_LNB_POWER_14V==u1Vol) || (FAC_LNB_POWER_19V==u1Vol)){
            disec_cmd.ui1_lnb_power=2;
        }
        if(DRVAPI_TUNER_OK!=TunerSetAttribute(TunerID,TUNER_SET_TYPE_DISEQC_CMD,&disec_cmd,sizeof(DISEQC_BUS_CMD_T))){
            mcSHOW_DRVERR_MSG(("[TunerFac]Set Diseqc command fail\n"));;
        }
    }else{
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }	
}
 
void TunerDVBSFac_GetLnbVol(UINT8 TunerID, UINT8 *pu1Vol)
{
    TUNER_SAT_DIG_LNB_STATUS_INFO_T t_lnb_info;
    SIZE_T z_len=sizeof(TUNER_SAT_DIG_LNB_STATUS_INFO_T);

    SetRemapConnType((UINT16)TunerID, TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK==TunerGetAttribute(TunerID,TUNER_GET_TYPE_LNB_STATUS,&t_lnb_info,&z_len)){
        if(0==t_lnb_info.ui1_lnb_power){
    	  *pu1Vol=FAC_LNB_POWER_OFF;
        }else if(1==t_lnb_info.ui1_lnb_power){//13/18v
    	    if(t_lnb_info.ui1_polarity_13v_18v){//18v
    		   *pu1Vol=FAC_LNB_POWER_18V;
            }else{
                *pu1Vol=FAC_LNB_POWER_13V;
            }
        }else if(2==t_lnb_info.ui1_lnb_power){//14/19v
            if(t_lnb_info.ui1_polarity_13v_18v){//19v
                *pu1Vol=FAC_LNB_POWER_19V;
            }else{
                *pu1Vol=FAC_LNB_POWER_14V;
            }
    	}
    }else{
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }	
}
void TunerDVBSFac_GetLnbShortStatus(UINT8 TunerID, UINT8 *pbshort)
{
    TUNER_SAT_DIG_LNB_STATUS_INFO_T t_lnb_info;
    SIZE_T z_len=sizeof(TUNER_SAT_DIG_LNB_STATUS_INFO_T);

    SetRemapConnType((UINT16)TunerID, TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK==TunerGetAttribute(TunerID,TUNER_GET_TYPE_LNB_SHORT_STATUS,&t_lnb_info,&z_len)){
        *pbshort=t_lnb_info.b_short_status;
        mcSHOW_DBG_MSG(("[%d] Shorts %d\n",TunerID, *pbshort));
    }else{
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }
}
void TunerDVBSFac_Set22K(UINT8 TunerID, UINT8 u1On)
{
    TUNER_SAT_DIG_LNB_STATUS_INFO_T t_lnb_info;
    SIZE_T z_len=sizeof(TUNER_SAT_DIG_LNB_STATUS_INFO_T);
    DISEQC_BUS_CMD_T disec_cmd;

    SetRemapConnType((UINT16)TunerID, TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK==TunerGetAttribute(TunerID,TUNER_GET_TYPE_LNB_STATUS,&t_lnb_info,&z_len)){
        TunerDVBSFac_InitDisecCmd(&disec_cmd,&t_lnb_info);
        disec_cmd.ui4_control_mask=DISEQC_CMD_CTRL_22K_MASK;
        disec_cmd.b_22k=u1On;
        if(DRVAPI_TUNER_OK!=TunerSetAttribute(TunerID,TUNER_SET_TYPE_DISEQC_CMD,&disec_cmd,sizeof(DISEQC_BUS_CMD_T))){
            mcSHOW_DRVERR_MSG(("[TunerFac]Set Diseqc command fail\n"));;
        }
    }else{
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }	
}
void TunerDVBSFac_GetLnb22KStatus(UINT8 TunerID, UINT8 *pb22kOn)
{
    TUNER_SAT_DIG_LNB_STATUS_INFO_T t_lnb_info;
    SIZE_T z_len=sizeof(TUNER_SAT_DIG_LNB_STATUS_INFO_T);

    SetRemapConnType((UINT16)TunerID, TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK==TunerGetAttribute(TunerID,TUNER_GET_TYPE_LNB_STATUS,&t_lnb_info,&z_len)){
        *pb22kOn=t_lnb_info.b_22k;
        mcSHOW_DBG_MSG(("[%d] 22K is %d\n",TunerID, *pb22kOn));
    }else{
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }
}
void TunerDVBSFac_SetDemodClock(UINT8 TunerID, UINT8 u1ClockOn)
{
    SetRemapConnType((UINT16)TunerID, TUNER_CONN_TYPE_SAT_DIG);
    if(DRVAPI_TUNER_OK!=TunerSetAttribute(TunerID,TUNER_SET_TYPE_DEMOD_CLOCK,&u1ClockOn,sizeof(u1ClockOn))){
        mcSHOW_DRVERR_MSG(("[TunerFac]Get LNB Status fail\n"));
    }
}
BOOL TunerDVBSFac_GetDemodMountStatus(UINT8 TunerID)
{
    #ifndef CC_SONYDEMOD_CXD2842 //Adding this define is only for build error, Salvia_EU sysbuild run on Turnkey(MT5565 + DB10_V3)
    return (DEMOD_MOUNT_STATUS_MSK_BIT==(DVBS_DemodMountStatus(TunerID) & DEMOD_MOUNT_STATUS_MSK_BIT));
    #else
    return TRUE;
    #endif
}
BOOL TunerDVBSFac_GetLNBMountStatus(UINT8 TunerID)
{
    #ifndef CC_SONYDEMOD_CXD2842 //Adding this define is only for build error, Salvia_EU sysbuild run on Turnkey(MT5565 + DB10_V3)
    return (LNB_MOUNT_STATUS_MSK_BIT==(DVBS_DemodMountStatus(TunerID) & LNB_MOUNT_STATUS_MSK_BIT));
    #else
    return TRUE;
    #endif
}
#endif
#endif
 

