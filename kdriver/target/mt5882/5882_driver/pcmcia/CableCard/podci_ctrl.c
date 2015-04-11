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
 * $Author: p4admin $
 * $Date: 2015/04/11 $
 * $RCSfile: podci_ctrl.c,v $
 * $Revision: #3 $
 *---------------------------------------------------------------------------*/
  
 
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "drvcust_if.h"
#include "x_gpio.h"
#include "x_util.h"
#include "x_hal_926.h"
#include "x_drv_cli.h"
#include "x_timer.h"
LINT_EXT_HEADER_END

#include "../pcmcia_if.h"
#include "../pcmcia_ctrl.h"
#include "../pcmcia_hw.h"
#include "../pcmcia_isr.h"

#include "podci_ctrl.h"
#include "podci_cis.h"

#ifdef PCMCIA_ENABLED

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MTK_LOADER
static UINT32               u4CisLen;
static CIS_INFO_T           rCisInfo;
static POD_ACCESS_MODE_T    eCurrentAccessMode;
static UINT16               u2DataNegotiatedBufSize;
static UINT16               u2ExtNegotiatedBufSize;
static UINT8*		 	    pu1DataBuf = NULL;
static UINT32               u1CmdRegister = 0;

#define MAX_CIS_MAP_SIZE                256
#define TIMEOUT                         2000
#ifndef DATA_CH_BUF_SIZE
#define DATA_CH_BUF_SIZE 4096
#endif

//-----------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------
#if 0
static UINT8 * pu1CurCisMap = NULL;
static UINT16 u2addrcor;
static UINT8 u1cor;
static BOOL error = FALSE;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
INT32 PODCI_GetDaFrIntStatus(POD_ACCESS_MODE_T eAccessMode, UINT8* pu1DaFrSts);

//-----------------------------------------------------------------------------
/** This API implements SDA read operation.
 */
//-----------------------------------------------------------------------------
static INT32 _PCMCIAHW_ReadCard(UINT8* pu1Data, UINT16* pu2DataLen, BOOL bUseDMA)
{
    UINT16              u2I;
    UINT16              u2DataLen;
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4DataRegAddr;
//    UINT32              u4CtrlStsRegAddr;
    UINT32              u4SizeRegLsAddr;
    UINT32              u4SizeRegMsAddr;

    ASSERT((pu1Data != NULL) && (pu2DataLen != NULL));

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG & (UINT32)DATA_CHANNEL;
//            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS & (UINT32)DATA_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG | (UINT32)EXT_CHANNEL;
  //          u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS | (UINT32)EXT_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "_PCMCIAHW_ReadCard: POD_INVALID_ACCESS_MODE \n");
            return POD_INVALID_ACCESS_MODE;
    }

    if ( !PCMCIA_DetectCard() )
    {
        printf("_PCMCIAHW_ReadCard: Card no exist !\n");
        return POD_READ_ERROR;
    }
    
    /* Get data length for this read operation */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));

    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_ReadCard err1\n");
        return POD_READ_ERROR;
    }
    u2DataLen = (UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ);

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));

    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_ReadCard err2\n");
        return POD_READ_ERROR;
    }
    u2DataLen |= (UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ) << 8;

    if (*pu2DataLen < u2DataLen)
    {
        LOG(0, "_PCMCIAHW_ReadCard: POD_EXCEED_MAX_BUF_SIZE : %d<%d\n", *pu2DataLen, u2DataLen);
        return POD_EXCEED_MAX_BUF_SIZE;
    }
    else
    {
        *pu2DataLen = u2DataLen;
    }

    if ( u2DataLen > 64 )
    {
        bUseDMA = TRUE;
    }
    
    if ( bUseDMA )
    {
        UINT16 u2TmpDataLen = 0;
        UINT8* pu1TmpData = pu1Data;

        LOG(0, "DMA Read : size = %d : %d\n", u2DataLen, *pu2DataLen);

        while ( u2DataLen > 0 )
        {
            if ( u2DataLen > DATA_CH_BUF_SIZE )
            {
                u2TmpDataLen = DATA_CH_BUF_SIZE;
            }
            else
            {
                u2TmpDataLen = u2DataLen;
            }
            
            if ( !PCMCIA_DetectCard() )
            {
                printf("_PCMCIAHW_ReadCard: Card no exist !\n");
                return POD_READ_ERROR;
            }

            HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
            PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
            PCMCIAHW_WRITE32(REG_DRAM_ADDR, PHYSICAL((UINT32)pu1DataBuf));
            PCMCIAHW_WRITE32(REG_BYTE_CNT, u2TmpDataLen);
            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE | SWIA)); /* Lock device output address */
            if ( PCMCIA_GetCmdStatusDone() != TRUE )
            {
                LOG(0, "_PCMCIAHW_DmaReadCard err3\n");
                return POD_READ_ERROR;
            }

            x_memcpy((void*)pu1TmpData, (void*)pu1DataBuf, u2DataLen);
			
            if ( u2DataLen > DATA_CH_BUF_SIZE )
            {
                u2DataLen -= DATA_CH_BUF_SIZE;
                pu1TmpData += DATA_CH_BUF_SIZE;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        /* Read the data in sequentially */
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        for (u2I = 0; u2I < *pu2DataLen; u2I++)
        {
            if ( !PCMCIA_DetectCard() )
            {
                printf("_PCMCIAHW_ReadCard: Card no exist !\n");
                return POD_READ_ERROR;
            }
						
            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
            if ( PCMCIA_GetCmdStatusDone() != TRUE )
            {
                LOG(0, "_PCMCIAHW_ReadCard err4\n");
                return POD_READ_ERROR;
            }
            pu1Data[u2I] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
        }
    }

    return POD_OK;
}

//-----------------------------------------------------------------------------
/** This API implements SDA write operation.
 */
//-----------------------------------------------------------------------------
static INT32 _PCMCIAHW_WriteCard(const UINT8* pu1Data, UINT16 u2DataLen, BOOL bUseDMA)
{
    UINT16              u2I;
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4DataRegAddr;
    UINT32              u4CtrlStsRegAddr;
    UINT32              u4SizeRegLsAddr;
    UINT32              u4SizeRegMsAddr;
    UINT8               u1DaFrSts;
    UINT8               pu1DaFrSts;

    ASSERT(pu1Data != NULL);

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG & (UINT32)DATA_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS & (UINT32)DATA_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG | (UINT32)EXT_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS | (UINT32)EXT_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "_PCMCIAHW_WriteCard: POD_INVALID_ACCESS_MODE \n");
            return POD_INVALID_ACCESS_MODE;
    }

    if ( !PCMCIA_DetectCard() )
    {
        printf("_PCMCIAHW_WriteCard: Card no exist !\n");
        return POD_WRITE_ERROR;
    }
						
    /* Get DAIE and FRIE status */
    if (PODCI_GetDaFrIntStatus(eAccessMode, &u1DaFrSts) != POD_OK)
    {
        LOG(0, "_PCMCIAHW_WriteCard: POD_WRITE_ERROR \n");
        return POD_WRITE_ERROR;
    }

    /* Check if there is data available */
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_POLLING )    /* Polling */
    {
        if (PODCI_WaitForModuleStatus((UINT8)DA, 1) == POD_OK)
        {
            LOG(0, "_PCMCIAHW_WriteCard: POD_DATA_AVAILABLE 1\n");
            return POD_DATA_AVAILABLE;
        }
    }
    else                                                                    /* Interrupt  */
    {
        PODCI_GetDaFrIntStatus(eAccessMode, &pu1DaFrSts);
        if (pu1DaFrSts & DA)
        {
            LOG(0, "_PCMCIAHW_WriteCard: POD_DATA_AVAILABLE 2\n");
            return POD_DATA_AVAILABLE;
        }
    }


    if (PODCI_WaitForModuleStatus((UINT8)FR, WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test FR */
    {
        LOG(0, "_PCMCIAHW_WriteCard wait FR error1\n");
        return FALSE;
    }

    /* Initiate host-to-module cycle */
    u1CmdRegister |= HC;
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* HC -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_WriteCard err1\n");
        return POD_WRITE_ERROR;
    }

    /* Set data length for this write operation */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)(u2DataLen & 0xFF));
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_WriteCard err3\n");
        return POD_WRITE_ERROR;
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)((u2DataLen >> 8)&0xFF));
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_WriteCard err4\n");
        return POD_WRITE_ERROR;
    }
	
    if ( u2DataLen > 64 )
    {
        bUseDMA = TRUE;
    }
    
    if ( bUseDMA )
    {
        UINT16 u2TmpDataLen = 0;
        const UINT8* pu1TmpData = pu1Data;
        LOG(0, "DMA Write : size = %d : %d\n", u2DataLen, u2DataLen);
        while ( u2DataLen > 0 )
        {
            if ( u2DataLen > DATA_CH_BUF_SIZE )
            {
                u2TmpDataLen = DATA_CH_BUF_SIZE;
            }
            else
            {
                u2TmpDataLen = u2DataLen;
            }
            
            if ( !PCMCIA_DetectCard() )
            {
                printf("_PCMCIAHW_WriteCard: Card no exist !\n");
                return POD_WRITE_ERROR;
            }
		
            x_memcpy((void*)pu1DataBuf, (void*)pu1TmpData, u2DataLen);
            HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
            LOG(1, "PODCI_WriteCardReg:Physical address=0x%x, virtual address=0x%x\n", PHYSICAL((UINT32)pu1DataBuf), (UINT32)pu1DataBuf);
            PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
            PCMCIAHW_WRITE32(REG_DRAM_ADDR, PHYSICAL((UINT32)pu1DataBuf));
            PCMCIAHW_WRITE32(REG_BYTE_CNT, u2DataLen);
            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_WRITE | ICMD_IE | SWIA)); /* Lock device output address */
            if ( PCMCIA_GetCmdStatusDone() != TRUE )
            {
                LOG(1, "PODCI_WriteCardReg err2\n");
                return POD_WRITE_ERROR;
            }

            if ( u2DataLen > DATA_CH_BUF_SIZE )
            {
                u2DataLen -= DATA_CH_BUF_SIZE;
                pu1TmpData += DATA_CH_BUF_SIZE;
            }
            else
            {
                break;
            }
        }
    
        // LOG(0, "_PCMCIAHW_WriteCard err5\n");
        //return POD_WRITE_ERROR;
    }
    else
    {
        /* Write the data out sequentially */
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        for (u2I = 0; u2I < u2DataLen; u2I++)
        {
            if ( !PCMCIA_DetectCard() )
            {
                printf("_PCMCIAHW_WriteCard: Card no exist !\n");
                return POD_WRITE_ERROR;
            }
        			
            PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)(pu1Data[u2I]));
            PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
            if ( PCMCIA_GetCmdStatusDone() != TRUE )
            {
                LOG(0, "_PCMCIAHW_WriteCard err6\n");
                return POD_WRITE_ERROR;
            }
        }
	}
		
    u1CmdRegister &= (0xFF-HC);
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* HC -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PCMCIAHW_WriteCard err7\n");
        return POD_WRITE_ERROR;
    }

    return POD_OK;
}

#if 0
//-----------------------------------------------------------------------------
/** This API implements DMA read operation.
 *  For external CI, the DMA should be two steps : CAM->8295, 8295->main chip DRAM
 *  For internal CI, the DAM is only one step : CAM->main chip DRAM
 */
//-----------------------------------------------------------------------------
static INT32 _PCMCIAHW_DmaReadCard(UINT8* pu1Data, UINT16* pu2DataLen)
{
    UINT16              u2I;
    UINT16              u2DataLen;
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4DataRegAddr;
    UINT32              u4CtrlStsRegAddr;
    UINT32              u4SizeRegLsAddr;
    UINT32              u4SizeRegMsAddr;

    ASSERT((pu1Data != NULL) && (pu2DataLen != NULL));

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG & (UINT32)DATA_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS & (UINT32)DATA_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG | (UINT32)EXT_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS | (UINT32)EXT_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "_PCMCIAHW_DmaReadCard: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    /* Get data length for this read operation */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaReadCard err1\n");
        return POD_READ_ERROR;
    }
    u2DataLen = (UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ);

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaReadCard err2\n");
        return POD_READ_ERROR;
    }
    u2DataLen |= (UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ) << 8;

    if (*pu2DataLen < u2DataLen)
    {
        LOG(0, "_PCMCIAHW_DmaReadCard: POD_EXCEED_MAX_BUF_SIZE\n");
        return POD_EXCEED_MAX_BUF_SIZE;
    }
    else
    {
        *pu2DataLen = u2DataLen;
    }

    /* Read the data out sequentially */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        PCMCIAHW_WRITE32(REG_DRAM_ADDR, 0);
    }
    else
    {
        PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)pu1DataBuf);
    }

    PCMCIAHW_WRITE32(REG_BYTE_CNT, *pu2DataLen);
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE | SWIA)); /* Lock device output address */
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaReadCard err3\n");
        return POD_READ_ERROR;
    }

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {
        UINT32 *pu4Buf = (UINT32 *)pu1DataBuf;
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|(1<<17));
        PCMCIAHW_DMA_READ32((UINT32)pu4Buf, 0, u2DataLen);
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~(1<<17)));
    }
    else
    {
    	HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, *pu2DataLen);
    }

    x_memcpy((void*)pu1Data, (void*)pu1DataBuf, *pu2DataLen);
    /* Make sure the transaction is completed successfully */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    for (u2I = 0; u2I < RE_CHECK_COUNT; u2I++)
    {
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
        LOG(1, "_PCMCIAHW_DmaReadCard err4\n");
            return POD_READ_ERROR;
        }
        if (!(PCMCIAHW_READ32(REG_SDA_DATA_READ) & RE))
        {
            return POD_OK;
        }
        x_thread_delay(1);
    }

    if (u2I == RE_CHECK_COUNT)
    {
        LOG(0, "_PCMCIAHW_DmaReadCard: POD_READ_ERROR\n");
        return POD_READ_ERROR;
    }

    return POD_OK;
}

//-----------------------------------------------------------------------------
/** This API implements DMA write operation.
 */
//-----------------------------------------------------------------------------
static INT32 _PCMCIAHW_DmaWriteCard(const UINT8* pu1Data, UINT16 u2DataLen)
{
    UINT16              u2I;
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4DataRegAddr;
    UINT32              u4CtrlStsRegAddr;
    UINT32              u4SizeRegLsAddr;
    UINT32              u4SizeRegMsAddr;
    UINT8               u1DaFrSts;
    UINT8               pu1DaFrSts;     

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG & (UINT32)DATA_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS & (UINT32)DATA_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG | (UINT32)EXT_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS | (UINT32)EXT_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "_PCMCIAHW_DmaWriteCard: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    /* Get DAIE and FRIE status */
    if (PODCI_GetDaFrIntStatus(eAccessMode, &u1DaFrSts) != POD_OK)
    {
        LOG(0, "_PCMCIAHW_DmaWriteCard: POD_WRITE_ERROR\n");
        return POD_WRITE_ERROR;
    }

    /* Check if there is data available */
    if( tPcmciaDriverConfig.u1CmdDoneMethod == PCMCIA_CMD_DONE_POLLING )              /* Polling */
    {
        if (PODCI_WaitForModuleStatus((UINT8)DA, 1) == POD_OK)
        {
            LOG(0, "_PCMCIAHW_DmaWriteCard: POD_DATA_AVAILABLE\n");
            return POD_DATA_AVAILABLE;
        }
    }
    else                                                                                           /* Interrupt  */
    {
        PODCI_GetDaFrIntStatus(eAccessMode, &pu1DaFrSts);
        if (pu1DaFrSts & DA)
        {
            LOG(0, "_PCMCIAHW_DmaWriteCard: POD_DATA_AVAILABLE 2\n");
            return POD_DATA_AVAILABLE;
        }
    }


    /* Initiate host-to-module cycle */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    for (u2I = 0; u2I < HOST_CONTROL_TRY; u2I++)
    {
        PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)(HC | u1DaFrSts)); /* HC -> 1 */
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(1, "_PCMCIAHW_DmaWriteCard err1\n");
            return POD_WRITE_ERROR;
        }

        if (PODCI_WaitForModuleStatus((UINT8)FR, WAIT_FOR_MODULE_STATUS_COUNT) == POD_OK) /* Test FR */
        {
            break;
        }

        /* Module is not available, retest FR after a period */
        PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, 0 | u1DaFrSts); /* HC -> 0 */
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(1, "_PCMCIAHW_DmaWriteCard err2\n");
            return POD_WRITE_ERROR;
        }
        x_thread_delay(RETEST_FR_INTERVAL);
    }

    /* Module is still not available, report error */
    if (u2I == HOST_CONTROL_TRY)
    {
        LOG(0, "_PCMCIAHW_DmaWriteCard: POD_WRITE_ERROR\n");
        return POD_WRITE_ERROR;
    }

    /* Set data length for this write operation */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u2DataLen & 0xFF);
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaWriteCard err3\n");
        return POD_WRITE_ERROR;
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u2DataLen >> 8);
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaWriteCard err4\n");
        return POD_WRITE_ERROR;
    }

    /* Write the data out sequentially */
    x_memcpy((void*)pu1DataBuf, (void*)pu1Data, u2DataLen);
    HalFlushDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);

    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
    {   
        UINT32 *pu4Buf = (UINT32 *)pu1DataBuf;
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|(1<<17));
        PCMCIAHW_DMA_WRITE32((UINT32)pu4Buf, 0, u2DataLen);
        PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~(1<<17)));
        PCMCIAHW_WRITE32(REG_DRAM_ADDR, 0);
    }
    else
    {
        PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)pu1DataBuf);
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
    PCMCIAHW_WRITE32(REG_BYTE_CNT, u2DataLen);
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_WRITE | ICMD_IE | SWIA)); /* Lock device output address */
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaWriteCard err5\n");
        return POD_WRITE_ERROR;
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, 0 | u1DaFrSts); /* HC -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PCMCIAHW_DmaWriteCard err6\n");
        return POD_WRITE_ERROR;
    }

    /* Make sure the transaction is completed successfully */
    for (u2I = 0; u2I < WE_CHECK_COUNT; u2I++)
    {
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(1, "_PCMCIAHW_DmaWriteCard err7\n");
            return POD_WRITE_ERROR;
        }
        if (!(PCMCIAHW_READ32(REG_SDA_DATA_READ) & WE))
        {
            return POD_OK;
        }
        x_thread_delay(1);
    }

    if (u2I == WE_CHECK_COUNT)
    {
        LOG(0, "_PCMCIAHW_DmaWriteCard: POD_WRITE_ERROR\n");
        return POD_WRITE_ERROR;
    }

    return POD_OK;
}
#endif

static INT32 _PODCI_CheckCis(void)
{
    INT32   i4Return;
    UINT8*  pu1CisMap = NULL;

    pu1CisMap = (UINT8*)x_mem_alloc(MAX_CIS_MAP_SIZE);
    if ( pu1CisMap == NULL )
    {
        LOG(0, "_PODCI_CheckCis allocate memory failed !\n");
		x_mem_free((void*)pu1CisMap);
        return POD_OK;
    }
    
    (void)x_memset((void*)pu1CisMap, 0, MAX_CIS_MAP_SIZE);    
    
    u4CisLen = MAX_CIS_MAP_SIZE;
    i4Return = PODCI_GetCis(POD_SDA, pu1CisMap, &u4CisLen);
    if (i4Return != POD_OK)
    {
        LOG(0, "_PODCI_CheckCis: Failed1 %d\n", i4Return);
		x_mem_free((void*)pu1CisMap);
        return i4Return;
    }

    i4Return = PODCI_ParseCis(pu1CisMap, &rCisInfo);
    if (i4Return != POD_OK)
    {
        LOG(0, "_PODCI_CheckCis: Failed2 %d\n", i4Return);
		x_mem_free((void*)pu1CisMap);
        return i4Return;
    }

    x_mem_free((void*)pu1CisMap);

    LOG(1, "COR address: 0x%x\n", rCisInfo.u2CorAddr);
    LOG(1, "COR value: 0x%x\n", rCisInfo.u1CorValue);
    return i4Return;
}

static void _PODCI_ChangePersonality(void)
{
    LOG(0, "_PODCI_ChangePersonality : %d %d\n", rCisInfo.u2CorAddr, rCisInfo.u1CorValue);
    VERIFY(PODCI_WriteCor(rCisInfo.u2CorAddr, rCisInfo.u1CorValue) == POD_OK);

    /* Configure hardware controller to support POD interface */
    /* Disable BVD1 and BVD2 interrupts since the pins are used as TS data pins after personality change */
    PCMCIAHW_WRITE32(REG_CTRL, PCMCIAHW_READ32(REG_CTRL) & ~PC_BVD1_IE & ~PC_BVD2_IE);
    PCMCIAHW_WRITE32(REG_CTRL, (PCMCIAHW_READ32(REG_CTRL) & 0x3FFFFFFF) | (UINT32)PODCI);
}

static BOOL _PODCI_NegotiateBufSize(UINT16* pu2Buf)
{
    UINT8               u1I;
    UINT16              u2DataLen;
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4DataRegAddr;
    UINT32              u4CtrlStsRegAddr;
    UINT32              u4SizeRegLsAddr;
    UINT32              u4SizeRegMsAddr;
    UINT16*             pu2BufSize;
    UINT16              u2Tmp;
    
    LOG(3, "_PODCI_NegotiateBufSize\n");
    
    if ( !PCMCIA_DetectCard() )
    {
        printf("_PODCI_NegotiateBufSize: Card no exist !\n");
        return FALSE;
    }
    
    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG & (UINT32)DATA_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS & (UINT32)DATA_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS & (UINT32)DATA_CHANNEL;
            pu2BufSize = &u2DataNegotiatedBufSize;
            break;

        case POD_EXTENDED_CHANNEL:
            u4DataRegAddr   = (UINT32)POD_DATA_REG | (UINT32)EXT_CHANNEL;
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            u4SizeRegLsAddr = (UINT32)POD_SIZE_REG_LS | (UINT32)EXT_CHANNEL;
            u4SizeRegMsAddr = (UINT32)POD_SIZE_REG_MS | (UINT32)EXT_CHANNEL;
            pu2BufSize = &u2ExtNegotiatedBufSize;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "_PODCI_NegotiateBufSize: POD_INVALID_ACCESS_MODE\n");
            return FALSE;
    }

    *pu2Buf = 16;
    
    //
    u1CmdRegister = RS|DAIE;
    LOG(3,"[N] u1CmdRegister is 0x%x\n", u1CmdRegister);
    /* Reset channel */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* RS -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "PODCI_ResetChannel err1\n");
        return FALSE;
    }
    x_thread_delay(5); /* At least 40us */

    u1CmdRegister &= (0xFF-RS); 
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* RS -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "PODCI_ResetChannel err2\n");
        return FALSE;
    }
    x_thread_delay(5); /* At least 40us */

    if (PODCI_WaitForModuleStatus((UINT8)FR, (UINT16)WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test FR */
    {
        return FALSE;
    }

    /* Size read */
    u1CmdRegister |= SR;
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)u1CmdRegister); /* SR -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "_PODCI_NegotiateBufSize err1\n");
        return FALSE;
    }
    if (PODCI_WaitForModuleStatus((UINT8)DA, WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test DA */
    {
        return FALSE;
    }

    u2DataLen = 0;
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err2\n");
        return FALSE;
    }
    u2Tmp = (UINT16)(PCMCIAHW_READ8(REG_SDA_DATA_READ));
    u2DataLen = (u2Tmp & 0xFF);
    LOG(3, "LS = %d\n", u2Tmp);

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err3\n");
        return FALSE;
    }
    u2Tmp = (UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ);
    u2DataLen = u2DataLen | ((u2Tmp<<8) & 0xFF00);
    LOG(3, "MS = %d\n", u2Tmp);
    LOG(3, "Data Length = %d\n" ,u2DataLen);

    /* Data length cannot be more than 2 */
    if (u2DataLen > 2)
    {
        LOG(0, "_PODCI_NegotiateBufSize: u2DataLen > 2\n");
        return FALSE;
    }

    if ( u2DataLen == 1 )
    {
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(0, "_PODCI_NegotiateBufSize err4\n");
            return FALSE;
        }
        *pu2BufSize = (UINT16)(PCMCIAHW_READ8(REG_SDA_DATA_READ) & 0xFF);
    }
    else if ( u2DataLen == 2 )
    {
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(0, "_PODCI_NegotiateBufSize err4.1\n");
            return FALSE;
        }
        *pu2BufSize = (((UINT16)PCMCIAHW_READ8(REG_SDA_DATA_READ)<<8) & 0xFF00);
        
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(0, "_PODCI_NegotiateBufSize err4.1\n");
            return FALSE;
        }
        *pu2BufSize |= (UINT16)(PCMCIAHW_READ8(REG_SDA_DATA_READ) & 0xFF);
    }
    else
    {
        LOG(0, "_PODCI_NegotiateBufSize err4.3\n");
        return FALSE;
    }

    u1CmdRegister &= (0xFF-SR);
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* SR -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err5\n");
        return FALSE;
    }

    LOG(3, "Buffer size from CAM : %d\n", *pu2BufSize);
    /* Compare the buffer size with our buffer size */
    *pu2BufSize = MIN_VALUE(*pu2BufSize, 65535);//change nego buffsize to max for some CI+ issue
    LOG(3, "Buffer size decided : %d\n", *pu2BufSize);

    /* Size write */
    u1CmdRegister |= SW;
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* SW -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err6\n");
        return FALSE;
    }
    // wait FR
    if (PODCI_WaitForModuleStatus((UINT8)FR, WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test FR */
    {
        LOG(0, "_PODCI_NegotiateBufSize wait FR error\n");
        return FALSE;
    }
    
    u1CmdRegister |= HC;
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* HC -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err6.1\n");
        return FALSE;
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegLsAddr); /* LS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)(u2DataLen & 0xFF));
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err8\n");
        return FALSE;
    }

    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4SizeRegMsAddr); /* MS */
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)((u2DataLen >> 8)&0xFF));
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err9\n");
        return FALSE;
    }
    for (u1I = (UINT8)u2DataLen; u1I > 0; u1I--)
    {
        PCMCIAHW_WRITE32(REG_DEV_ADDR, u4DataRegAddr);
        PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, (UINT32)(*pu2BufSize >> ((u1I-1)*8)) & 0xFF);
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            return FALSE;
        }
    }

    u1CmdRegister &= (0xFF-HC);
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* HC -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err10\n");
        return FALSE;
    }

    u1CmdRegister &= (0xFF-SW);
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* SW -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(0, "_PODCI_NegotiateBufSize err10\n");
        return FALSE;
    }

    
    *pu2Buf = *pu2BufSize;

    if (PODCI_WaitForModuleStatus((UINT8)FR, WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test FR */
    {
        LOG(0, "_PODCI_NegotiateBufSize wait FR error1\n");
        return FALSE;
    }

    LOG(0, "_PODCI_NegotiateBufSize end\n");
    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
INT32 PODCI_GetDaFrIntStatus(POD_ACCESS_MODE_T    eAccessMode,
                            UINT8*               pu1DaFrSts)
{
    UINT32              u4CtrlStsRegAddr;
    
    ASSERT(pu1DaFrSts != NULL);

    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4CtrlStsRegAddr= (UINT32)POD_CTRL_STS_REG & (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_GetDaFrIntStatus: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    if ( !PCMCIA_DetectCard() )
    {
        printf("PODCI_GetDaFrIntStatus: Card no exist !\n");
        return POD_CMD_ERROR;
    }
    
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "PODCI_GetDaFrIntStatus err1\n");
        return POD_CMD_ERROR;
    }
    *pu1DaFrSts = PCMCIAHW_READ8(REG_SDA_DATA_READ);
                
    return POD_OK;
}
#if 0
INT32 PODCI_LowLevelInit(void)
{
	UINT32 i;
		UINT32 u4TimeCount= 0 ;
		UINT16 addrcor;
		UINT8 cor;
		 error = FALSE;
		LOG(0,"_MTCI_CheckCIS\n");
		 
    pu1CurCisMap = (UINT8*)x_mem_alloc(MAX_CIS_MAP_SIZE);
    if ( pu1CurCisMap == NULL )
    {
        LOG(0, "_PODCI_CheckCis allocate memory failed !\n");
        return POD_OK;
    }
	
		if ( pu1CurCisMap == NULL )
		{
			return -1;
		}
	
		// Reset CAM
		x_thread_delay(200);
		PCMCIA_Reset(1);
		x_thread_delay(10);
		PCMCIA_Reset(0);
	
		while( (PCMCIA_TestCardReady()!=1) )
		{
			if ( ++u4TimeCount > TIMEOUT)
			{
				LOG(0,"Ready pin is low... (card not reset?) \n");
				//for one DIBOLO CAM(from LG) issue
				x_thread_delay(200);
				PCMCIA_Reset(1);
				x_thread_delay(10);
				PCMCIA_Reset(0);
				break;
			}
			x_thread_delay(5);
		}
	
		PCMCIA_PcmciaToPod( FALSE );
		x_memset((void*)pu1CurCisMap, 0, MAX_CIS_MAP_SIZE);
		for( i = 0 ; i < MAX_CIS_MAP_SIZE ; i ++)
		{
			pu1CurCisMap[i] = PCMCIA_ReadCor( i *2);
			
		}
	
		if (PCMCIA_CheckCIS(pu1CurCisMap, &addrcor, &cor) != 1)
		{
			LOG(0,"_MTCI_CheckCIS Failed!\n");
			return -1;
		}
		
  	PCMCIA_PcmciaEnableTS(FALSE);
	
		LOG(0,"_MTCI_CheckCIS OK!\n");

		u2addrcor = addrcor;
		u1cor = cor;
		PCMCIA_WriteCor(u2addrcor, u1cor);
		 LOG(0,"PCMCIA_WriteCor : %d %d\n", u2addrcor, u1cor);

		  LOG(0,"_MTCI_ResetCam\n");
		
    PCMCIA_SetPodCiAccessMode(POD_DATA_CHANNEL);
	x_thread_delay(20);
    PCMCIA_ResetChannel();
	x_thread_delay(20);

	UINT16 u2BufSize = 65536;
   

   
    if (PCMCIA_NegotiatedBufSize(&u2BufSize))
    {
        LOG(0,"_MTCI_NegoBuf Failed ! pu2BufSize = %d\n", u2BufSize);
        return -1;
    	}
		return POD_OK;
}
#else
INT32 PODCI_LowLevelInit(void)
{
    INT32   i4Return;
    UINT16  u2NegoBufSize;

    PCMCIA_ResetCableCard();
    
    /* Read and parse CIS */
    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_CI )
    {
        CLI_Parser("p.ci.cis");
        i4Return = POD_OK;
    }
    else
    {
        i4Return = _PODCI_CheckCis();
    }

    if (i4Return != POD_OK)
    {
        LOG(0, "PODCI_LowLevelInit: i4Return != POD_OK\n");
        return i4Return;
    }

    /* Perform personality change */
    LOG(1, "POD personality change\n");
    _PODCI_ChangePersonality();

    // Work around, Mororola POD CAM will failed in PODCI_ResetChannel().
    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_CI )
    {
        /* Reset data channel */
        LOG(0, "Reset data channel\n");
        VERIFY(PODCI_SetPodCiAccessMode(POD_DATA_CHANNEL) == POD_OK);
        #ifdef PCMCIA_WORK_AROUND_DRIVER_BUILD_FOR_CAM026_CAM023
        x_thread_delay(3000);
        #endif
        if (!PODCI_ResetChannel())
        {
            LOG(0, "PODCI_LowLevelInit: Reset Data Channel Failed\n");
            return POD_CHANNEL_RESET_FAILED;
        }

        x_thread_delay(1);
        
        //CI does not have extended channel
        if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
        {
            /* Reset extended channel */
            LOG(0, "Reset extended channel\n");
            VERIFY(PODCI_SetPodCiAccessMode(POD_EXTENDED_CHANNEL) == POD_OK);
            if (!PODCI_ResetChannel())
            {
                LOG(0, "PODCI_LowLevelInit: Reset Extended Channel Failed\n");
                return POD_CHANNEL_RESET_FAILED;
            }

            x_thread_delay(1);
        }
    }
    LOG(0, "Reset data channel done\n");
    /* Negotiate data channel buffer size */
    VERIFY(PODCI_SetPodCiAccessMode(POD_DATA_CHANNEL) == POD_OK);
    if (!_PODCI_NegotiateBufSize(&u2NegoBufSize))
    {
        LOG(0, "Negotiate data channel buffer size failed !\n");
        return POD_BUF_SIZE_NGO_FAILED;
    }
    LOG(1, "Negotiate data channel buffer size : %d\n", u2NegoBufSize);

    x_thread_delay(1);

    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        /* Negotiate extended channel buffer size */
        VERIFY(PODCI_SetPodCiAccessMode(POD_EXTENDED_CHANNEL) == POD_OK);
        if (!_PODCI_NegotiateBufSize(&u2NegoBufSize))
        {
            LOG(0, "Negotiate extended channel buffer size failed !\n");
            return POD_BUF_SIZE_NGO_FAILED;
        }
        LOG(1, "Negotiate extended channel buffer size : %d\n", u2NegoBufSize);

        /* Enable data channel DA interrupt */
        LOG(1, "Enable data channel DA interrupt\n");
//        if (!_PODCI_EnableDaFrInt(DAIE, 0))
ASSERT(0);
        {
            LOG(0, "PODCI_LowLevelInit: POD_DA_FR_INT_ENABLE_FAILED\n");
            return POD_DA_FR_INT_ENABLE_FAILED;
        }
    }

    PCMCIA_EnableTS(TRUE);
    
    return POD_OK;
}
#endif
void PODCI_Init()
{
    /* Set the initial access mode */
    VERIFY(PODCI_SetPodCiAccessMode(PCMCIA_ATTRIBUTE_MEM) == POD_OK);

    /* Init parameters */
    u2DataNegotiatedBufSize = 0;
    u2ExtNegotiatedBufSize = 0;
    u4CisLen = 0;
}

void PODCI_SetCISInfo(UINT16 addrcor, UINT8 cor)
{
    rCisInfo.u2CorAddr = addrcor ;
    rCisInfo.u1CorValue = cor;
}

INT32 PODCI_GetPodCiAccessMode(POD_ACCESS_MODE_T* peAccessMode)
{
    ASSERT(peAccessMode != NULL);

    *peAccessMode = eCurrentAccessMode;
    return POD_OK;
}

INT32 PODCI_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode)
{
    eCurrentAccessMode = eAccessMode;
    return POD_OK;
}

void PODCI_SetBypass(BOOL fgBypass)
{
    if (fgBypass)
    {
        PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & (~(0x80000000))) | 0x80000000);   //Bypass (MT8295)
    }
    else
    {
        PCMCIAHW_WRITE32( REG_POD_GPR_CNTL_SET0, (PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0) & (~(0x80000000))) | 0x00000000);		//Non-Bypass (MT8295)
    }
}

INT32 PODCI_WaitForModuleStatus(UINT8    u1StatusBit,
                              UINT16   u2TimeOut)
{
    UINT16  u2Count = 0;
    UINT32  u4CtrlStsRegAddr;
    UINT32  u4Status = 0;
    POD_ACCESS_MODE_T   eAccessMode;

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4CtrlStsRegAddr = (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4CtrlStsRegAddr = (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_WaitForModuleStatus: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    if ( !PCMCIA_DetectCard() )
    {
        printf("PODCI_WaitForModuleStatus stage1: Card no exist !\n");
        return POD_CMD_ERROR;
    }
				
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    while (u2Count < u2TimeOut)
    {
        if ( !PCMCIA_DetectCard() )
        {
            printf("PODCI_WaitForModuleStatus stage2: Card no exist !\n");
            return POD_CMD_ERROR;
        }
	
        PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
        if ( PCMCIA_GetCmdStatusDone() != TRUE )
        {
            LOG(0, "PODCI_WaitForModuleStatus err1\n");
            return POD_CMD_ERROR;
        }
        u4Status = (UINT8)PCMCIAHW_READ8(REG_SDA_DATA_READ);
        if ( u4Status & u1StatusBit )
        {
            break;
        }
        LOG(1, "PODCI_WaitForModuleStatus 0x%x\n", u4Status);
        x_thread_delay(5);
        u2Count++;
    }

    if (u2Count == u2TimeOut)
    {
        LOG(0, "PODCI_WaitForModuleStatus timeout, status:0x%x, statusbit:0x%x\n",u4Status,  u1StatusBit);
        return POD_TIME_OUT;
    }

    return POD_OK;
}

INT32 PODCI_GetNegotiatedBufSize(UINT16* pu2BufSize)
{
    POD_ACCESS_MODE_T   eAccessMode;

    ASSERT(pu2BufSize != NULL);

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            *pu2BufSize = u2DataNegotiatedBufSize;
            break;

        case POD_EXTENDED_CHANNEL:
            *pu2BufSize = u2ExtNegotiatedBufSize;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_GetNegotiatedBufSize: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    return POD_OK;
}

BOOL PODCI_NegotiateBufSize(UINT16* pu2Buf)
{
    return _PODCI_NegotiateBufSize(pu2Buf);
}

BOOL PODCI_ResetChannel(void)
{
    POD_ACCESS_MODE_T   eAccessMode;
    UINT32              u4CtrlStsRegAddr;

    LOG(0, "Enter LGE PODCI_ResetChannel!\n");
    
    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            u4CtrlStsRegAddr = (UINT32)POD_CTRL_STS_REG & (UINT32)DATA_CHANNEL;
            break;

        case POD_EXTENDED_CHANNEL:
            u4CtrlStsRegAddr = (UINT32)POD_CTRL_STS_REG | (UINT32)EXT_CHANNEL;
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_ResetChannel: POD_INVALID_ACCESS_MODE\n");
            return FALSE;
    }

    u1CmdRegister = RS|FRIE|DAIE;
    /* Reset channel */
    PCMCIAHW_WRITE32(REG_DEV_ADDR, u4CtrlStsRegAddr);
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* RS -> 1 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "PODCI_ResetChannel err1\n");
        return FALSE;
    }
    x_thread_delay(5); /* At least 40us */

    u1CmdRegister &= (0xFF-RS); 
    PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, u1CmdRegister); /* RS -> 0 */
    PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
    if ( PCMCIA_GetCmdStatusDone() != TRUE )
    {
        LOG(1, "PODCI_ResetChannel err2\n");
        return FALSE;
    }
    x_thread_delay(5); /* At least 40us */

    if (PODCI_WaitForModuleStatus((UINT8)FR, (UINT16)WAIT_FOR_MODULE_STATUS_COUNT) != POD_OK) /* Test FR */
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** This API reads a number of bytes from POD data register. (Register on CAM)
 *  (Auto handle control/status/size(LS/MS) register.)
 *  (Middleware will not invoke this API.)
 */
//-----------------------------------------------------------------------------
INT32 PODCI_ReadCard(POD_IO_STATE_T   eIoState, UINT8* pu1Data, UINT16* pu2DataLen)
{
    INT32               i4Return = POD_OK;
    
    switch (eIoState)
    {
        case POD_SDA:
            i4Return = _PCMCIAHW_ReadCard(pu1Data, pu2DataLen, FALSE);
            break;

        case POD_DMA:
            i4Return = _PCMCIAHW_ReadCard(pu1Data, pu2DataLen, TRUE);
            break;
    }

    return i4Return;
}

//-----------------------------------------------------------------------------
/** This API writes a number of bytes into POD data register.
 *  (Auto handle control/status/size(LS/MS) register.)
 *  (Middleware will not invoke this API.)
 */
//-----------------------------------------------------------------------------
INT32 PODCI_WriteCard(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen)
{
    UINT16  u2NgoBufSize=0;
    INT32   i4Return;
    
    i4Return = PODCI_GetNegotiatedBufSize(&u2NgoBufSize);
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    if (u2DataLen > u2NgoBufSize)
    {
        LOG(0, "PODCI_WriteCard: POD_EXCEED_MAX_BUF_SIZE\n");
       return POD_EXCEED_MAX_BUF_SIZE;
    }

    switch (eIoState)
    {
        case POD_SDA:
            i4Return = _PCMCIAHW_WriteCard(pu1Data, u2DataLen, FALSE);
            break;

        case POD_DMA:
            i4Return = _PCMCIAHW_WriteCard(pu1Data, u2DataLen, FALSE);
            break;
    }

    return i4Return;
}

//-----------------------------------------------------------------------------
/** This API writes a number of bytes from one of POD registers.
 *  (Doesn't handle control/status/size(LS/MS) register.)
 *  (Middleware will invoke this API.)
 */
//-----------------------------------------------------------------------------
INT32 PODCI_ReadCardReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr, 
                        UINT8* pu1Data, UINT16 u2DataLen)
{
    POD_ACCESS_MODE_T   eAccessMode;
    UINT16              u2I;
#ifdef PCMCIA_MEASURE_TIME
	HAL_TIME_T time1, time2, result_time;
	UINT32 u4Micros;
	HAL_GetTime(&time1);
#endif

    ASSERT( pu1Data != NULL );

    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            PCMCIAHW_WRITE32(REG_DEV_ADDR, (UINT32)(u4RegAddr & (UINT32)DATA_CHANNEL));
            break;

        case POD_EXTENDED_CHANNEL:
            PCMCIAHW_WRITE32(REG_DEV_ADDR, (UINT32)(u4RegAddr | (UINT32)EXT_CHANNEL));
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_ReadCardReg: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    switch (eIoState)
    {
        case POD_SDA:
            for (u2I = 0; u2I < u2DataLen; u2I++)
            {
                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_READ | ICMD_IE));
                if ( PCMCIA_GetCmdStatusDone() != TRUE )
                {
                    LOG(0, "PODCI_ReadCardReg err1\n");
                    return POD_READ_ERROR;
                }
                pu1Data[u2I] = PCMCIAHW_READ8(REG_SDA_DATA_READ);
            }
            break;

        case POD_DMA:
            if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
            {
                // DMA : CI Card -> MT8295
                PCMCIAHW_WRITE32(REG_DRAM_ADDR, 0);
                PCMCIAHW_WRITE32(REG_BYTE_CNT, u2DataLen);
                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE | SWIA)); /* Lock device output address */
                if ( PCMCIA_GetCmdStatusDone() != TRUE )
                {
                    LOG(0, "PODCI_ReadCardReg err2\n");
                    return POD_READ_ERROR;
                }

                // DMA : MT8295 -> Main Chip
                {
                    UINT32 *pu4Buf = (UINT32 *)pu1DataBuf;
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)|(1<<17));
                    PCMCIAHW_DMA_READ32((UINT32)pu4Buf, 0, 512);
                    PCMCIAHW_WRITE32(REG_POD_GPR_CNTL_SET0, PCMCIAHW_READ32(REG_POD_GPR_CNTL_SET0)&(~(1<<17)));
                }
                x_memcpy((void*)pu1Data, (void*)pu1DataBuf, u2DataLen);
            }
            else
            {
            HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
            LOG(1, "PODCI_ReadCardReg:Physical address=0x%x, virtual address=0x%x\n", PHYSICAL((UINT32)pu1DataBuf), (UINT32)pu1DataBuf);
            PCMCIAHW_WRITE32(REG_DRAM_ADDR, PHYSICAL((UINT32)pu1DataBuf));
                //PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)pu1DataBuf);
                PCMCIAHW_WRITE32(REG_BYTE_CNT, u2DataLen);
                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_READ | ICMD_IE | SWIA)); /* Lock device output address */
                if ( PCMCIA_GetCmdStatusDone() != TRUE )
                {
                    LOG(1, "PODCI_ReadCardReg err2\n");
                    return POD_READ_ERROR;
                }
                HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
                x_memcpy((void*)pu1Data, (void*)pu1DataBuf, u2DataLen);
                break;
            }
    }

#ifdef PCMCIA_MEASURE_TIME
	HAL_GetTime(&time2);
	HAL_GetDeltaTime(&result_time, &time1, &time2);
	u4Micros = (result_time.u4Seconds * 1000000 + result_time.u4Micros)/1000;
	Printf("PODCI_ReadCardReg %d bytes consumes %dms\n", u2DataLen, u4Micros);
#endif

    return POD_OK;
}

//-----------------------------------------------------------------------------
/** This API writes a number of bytes into one of POD registers.
 *  (Doesn't handle control/status/size(LS/MS) register.)
 *  (Middleware will invoke this API.)
 */
//-----------------------------------------------------------------------------
INT32 PODCI_WriteCardReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr,
                         const UINT8* pu1Data, UINT16 u2DataLen)
{
    POD_ACCESS_MODE_T   eAccessMode;
    UINT16              u2I;
#ifdef PCMCIA_MEASURE_TIME
	HAL_TIME_T time1, time2, result_time;
	UINT32 u4Micros;
	HAL_GetTime(&time1);
#endif

    ASSERT(pu1Data != NULL);
    VERIFY(PODCI_GetPodCiAccessMode(&eAccessMode) == POD_OK);
    switch (eAccessMode)
    {
        case POD_DATA_CHANNEL:
            PCMCIAHW_WRITE32(REG_DEV_ADDR, (UINT32)(u4RegAddr & (UINT32)DATA_CHANNEL));
            break;

        case POD_EXTENDED_CHANNEL:
            PCMCIAHW_WRITE32(REG_DEV_ADDR, (UINT32)(u4RegAddr | (UINT32)EXT_CHANNEL));
            break;

        case PCMCIA_ATTRIBUTE_MEM:
        case PCMCIA_COMMON_MEM:
        default:
            LOG(0, "PODCI_WriteCardReg: POD_INVALID_ACCESS_MODE\n");
            return POD_INVALID_ACCESS_MODE;
    }

    switch (eIoState)
    {
        case POD_SDA:
            for (u2I = 0; u2I < u2DataLen; u2I++)
            {
                PCMCIAHW_WRITE32(REG_SDA_DATA_WRITE, pu1Data[u2I]);
                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_SDA_WRITE | ICMD_IE));
                if ( PCMCIA_GetCmdStatusDone() == FALSE )
                {
                    LOG(1, "PODCI_WriteCardReg err1\n");
                    return POD_WRITE_ERROR;
                }
            }
            break;

        case POD_DMA:
            if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
            {
                LOG(0, "TODO : External CI DMA \n");
                ASSERT(0);
            }
            else
            {
                x_memcpy((void*)pu1DataBuf, (void*)pu1Data, u2DataLen);
				 HalFlushInvalidateDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
                LOG(1, "PODCI_WriteCardReg:Physical address=0x%x, virtual address=0x%x\n", PHYSICAL((UINT32)pu1DataBuf), (UINT32)pu1DataBuf);
                PCMCIAHW_WRITE32(REG_DRAM_ADDR, PHYSICAL((UINT32)pu1DataBuf));
                HalFlushDCacheMultipleLine((UINT32)pu1DataBuf, u2DataLen);
                //PCMCIAHW_WRITE32(REG_DRAM_ADDR, (UINT32)pu1DataBuf);
                PCMCIAHW_WRITE32(REG_BYTE_CNT, u2DataLen);
                PCMCIAHW_WRITE32(REG_CMD, (UINT32)(PC_DMA_WRITE | ICMD_IE | SWIA)); /* Lock device output address */
                if ( PCMCIA_GetCmdStatusDone() != TRUE )
                {
                    LOG(1, "PODCI_WriteCardReg err2\n");
                    return POD_WRITE_ERROR;
                }
                break;
            }
    }

#ifdef PCMCIA_MEASURE_TIME
	HAL_GetTime(&time2);
	HAL_GetDeltaTime(&result_time, &time1, &time2);
	u4Micros = (result_time.u4Seconds * 1000000 + result_time.u4Micros)/1000;
	Printf("PODCI_WriteCardReg %d bytes consumes %dms\n", u2DataLen, u4Micros);
#endif

    return POD_OK;
}

//-----------------------------------------------------------------------------
/** This API allocates a DMA buffer for DMA read and write.
 */
//-----------------------------------------------------------------------------
void PODCI_AllocDmaBuf(void)
{
    if ( pu1DataBuf == NULL )
    {
        pu1DataBuf = (UINT8*)(VIRTUAL(BSP_AllocAlignedDmaMemory(DMA_BUF_SIZE, 32))); // return physical address
        LOG(3, "PODCI_AllocDmaBuf");
        if ( pu1DataBuf == 0 )
        {
            LOG(0, "PODCI_AllocDmaBuf Failed !\n");
            ASSERT(0);
        }
        else
        {
            LOG(1, "PODCI_AllocDmaBuf pu1DataBuf = 0x%x!\n", (UINT32)pu1DataBuf);
        }
    }
}
#endif
#endif

