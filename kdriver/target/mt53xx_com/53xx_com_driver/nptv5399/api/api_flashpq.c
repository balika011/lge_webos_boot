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
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "drv_ycproc.h"
#include "drv_pq_cust.h"	// TODO: Remove
#include "vdo_misc.h"
#include "drv_od.h"	
#ifndef CC_FPGA
#include "drv_default.h"
#endif /* CC_FPGA */

#include "../../lzma/LzmaEnc.h"
#include "../../lzma/LzmaLib.h"

#ifndef CC_COPLAT_MT82
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#endif

// === DEFINE =============================================================================
#define ENABLE_FLASH_PQ_DEBUG       0
//#define CC_COMPRESSED_FLASH_PQ_DATA  1

#define FLASH_PQ_LOG(x)    (IO32ReadFldAlign(FLASH_PQ_00, FALSH_PQ_LOG_CFG) & x)

#define FLASH_PQ_GAMMA_CHANNEL_NUM  3
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_256  256
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_257  257

#define QTY_SCRIPT_SIZE(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT) ? TIMING_SCRIPT_SIZE:0)
#define SMP_SCRIPT_SIZE(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_SMARTPIC_DESCRIPT)? SMARTPIC_SCRIPT_SIZE:0)

#define QTY_MAX_MIN_NUM(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0)
#define QTY_MIN_INDEX(x)   (aFlashPqFileHead[x].bNumOfSmartPic)       // The end of all smart pic + 1 is MIN table.
#define QTY_MAX_INDEX(x)   (aFlashPqFileHead[x].bNumOfSmartPic + 1)   // The end of all smart pic + 2 is MAX table.


#define PANEL_LONG_NAME_SIZE(x) ((aFlashPqFileHead[x].bRes & RES_SUPPORT_LONG_PANEL_NAME) ? TBL_LONG_PANEL_NAME_SIZE : 0)
#define SCRIPT_SIZE(x) ((QTY_SCRIPT_SIZE(x))+(SMP_SCRIPT_SIZE(x)))
#define IS_SUPPORT_SUB_SCRIPT(x) ((x) & (FC_SUPPORT_SCE_BY_SOURCE))
#define GAMMA_HEAD_SIZE(index) (IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord)? sizeof(FLASH_GAMMA_SCE_TABLE_HEAD_EX):sizeof(FLASH_PQ_TABLE_HEAD))
#define SCE_HEAD_SIZE(index) (IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord)? sizeof(FLASH_GAMMA_SCE_TABLE_HEAD_EX):sizeof(FLASH_PQ_TABLE_HEAD))
#define GAMMA_SIZE(index) (GAMMA_HEAD_SIZE(index)+aFlashPqFileHead[index].wSizeOfGammaTable)
#define SCE_SIZE(index) (SCE_HEAD_SIZE(index)+aFlashPqFileHead[index].wSizeOfSCETable)

enum
{
    E_FLASH_PQ_GAMMA_LOG        = 0x01,
    E_FLASH_PQ_SCE_LOG          = 0x02,
    E_FLASH_PQ_QTY_LOG          = 0x04, 
    E_FLASH_PQ_QTY_MIN_MAX_LOG  = 0x08,
    E_FLASH_PQ_OD_LOG           = 0x10,
    E_FLASH_PQ_PANEL_LOG        = 0x20,
    E_FLASH_PQ_REG_LOG          = 0x40,
    E_FLASH_PQ_ELSE_LOG         = 0x80,    
};

enum
{
    E_OD_SUPPORT_FB     = 0x01,
    E_OD_SUPPORT_PCID   = 0x02,
    E_OD_SUPPORT_ENABLE =0x04,
};

#define FLASHPQ_INDEX_CHECK(x) \
    do{ ASSERT(x < FLASHPQ_NUM); \
        x = (x >= FLASHPQ_NUM) ? 0 : x; }while(0)
        
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================

// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================

// === STATIC VARIABLE ====================================================================
UINT8* pFlashPqData[FLASHPQ_NUM];
        
FLASH_PQ_FILE_HEAD aFlashPqFileHead[FLASHPQ_NUM];
FLASH_PQ_TABLE_HEAD aFlashPqGammaHead, aFlashPqSceHead, aFlashPqOdHead, aFlashPqPanelHead;
RAWDATA_HEADER aRawDataHead;
RAWDATA_SUBHEADER* pRawDataSubHead = NULL;

UINT8 *pRawDataTbl = NULL;
UINT32 *pRawRegTbl = NULL;
UINT8 u1RawDataInit = SV_FALSE;

enum
{
    SV_REG_DIRECT = 0,
    SV_REG_XDATA,
    SV_REG_USER_DATA
};

FLASH_GAMMA_SCE_TABLE_HEAD_EX aFlashPqGammaHead_Ex, aFlashPqSceHead_Ex;
FLASH_PQ_TABLE_HEAD aFlashPqQtyHead[FLASHPQ_NUM];
UINT64 u8PqBaseAddr[FLASHPQ_NUM];
UINT8 bFileHeadSize[FLASHPQ_NUM];
UINT8 bFlashPQInit[FLASHPQ_NUM];
UINT8 bPqExtesion[FLASHPQ_NUM]; 
UINT8 bQtyMin[FLASHPQ_NUM][CUSTOM_QTY_TBL_DIM];
UINT8 bQtyMax[FLASHPQ_NUM][CUSTOM_QTY_TBL_DIM];
UINT8 gSCE_3DMapping = FALSE;


PANEL_FLASH_DATA_ATTRIBUTE_T rFlashPanelTable;

const FLASH_PQ_SW_INFO aFlashPqSWInfo[FLASHPQ_NUM] =
{
    {   eFlagFlashPqEnableGamma,            eFlagFlashPqEnableSCE, 
        eFlagFlashPqEnableQty,              eFlagFlashPqEnableQtyMaxMin, 
        eFlagFlashPqEnableQtySmartPic,      eFlagFlashPqQtySmartPicNewMode},
#ifdef SUPPORT_2ND_FLASHPQ
    {   eFlagFlashPq2ndpartEnableGamma,         eFlagFlashPq2ndpartEnableSCE,   
        eFlagFlashPq2ndpartEnableQty,           eFlagFlashPq2ndpartEnableQtyMaxMin, 
        eFlagFlashPq2ndpartEnableQtySmartPic,   eFlagFlashPq2ndpartQtySmartPicNewMode},
#endif
};

const FLASH_PQ_FLASH_INFO aFlashPqFlashInfo[FLASHPQ_NUM] =
{
    {eFlashPqBasePartition, eFlashPqBaseAddress, eFlashPqOffset,eFlashPqBlockSize},
#ifdef SUPPORT_2ND_FLASHPQ
    {eFlashPq2ndpartBasePartition, eFlashPq2ndpartBaseAddress, eFlashPq2ndpartOffset, eFlashPq2ndpartBlockSize},
#endif
};


// === BODY ===============================================================================
#ifndef CC_MTK_LOADER
static void vDrvGammaTopPt(UINT8 * pu1Table)
{
    if(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] >= pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2])
    {
        pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
            pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 1] +
            (pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] - pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 2]);
    }
    else
    {
        pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
            pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] +
            (UINT8)((UINT16)(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1]+0x100) - 
            pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2]);
    }
}

static UINT32 dwDrvCalcFlashPqQtyNumber(UINT8 bIndex)
{
    int i;

    for (i=0; i<CUSTOM_QTY_TBL_DIM; i++)
    {
        if (READ_CUST_QTY_ITEM(bIndex, i) == QUALITY_MAX)
        {
            return i;
        }
    }

    return 0;
}

// Compare the hash between QtyItem.h and FlashPQ QtyHeader.
static UINT8 bDrvCheckFlashPqQtyHash(UINT8 bIndex)
{
	UINT32 u4QtyNumber = 0;
	UINT8 i;

	// Get the hash from QtyItem.h
	u4QtyNumber = dwDrvCalcFlashPqQtyNumber(bIndex);
    if (u4QtyNumber == 0)
    {
        LOG(0, "QTY item error!\n");
        return SV_FALSE;
    }
	u4QtyNumber ++; // Get the next position after the QUALITY_MAX entry.

	// Compare the hashs.
	for (i=0; i < TBL_HEAD_HASH; i++)
	{
		// Array index range check.
		if (u4QtyNumber+i < CUSTOM_QTY_TBL_DIM + TBL_HEAD_HASH)
		{
			if (READ_CUST_QTY_ITEM(bIndex, u4QtyNumber+i) != aFlashPqQtyHead[bIndex].bHash[i])
			{
				LOG(0, "hash error at %d\n", i);
				return SV_FALSE;
			}
		}
		else
		{
			LOG(0, "Qty index range error %d\n", u4QtyNumber+i);
			return SV_FALSE;
		}
	}
	// Compare finished.
	return SV_TRUE;
}

static UINT16 wDrvGetODTableSize(UINT16 u2OdFormat)
{
    //_ODParam.u1ODTblSizeMode = u2OdFormat;
    if (u2OdFormat == 0)
    {
        LOG(0, "FlashPQ Size for OD : 17x17 \n");
        return OD_TBL_S_SIZE;
    }
    else if (u2OdFormat == 1)
    {
        LOG(0, "FlashPQ Size for OD : 33x33 \n");
        return OD_TBL_M_SIZE;
    }
    else if (u2OdFormat == 2)
    {
        LOG(0, "FlashPQ Size for OD : 41x41 \n");
        return OD_TBL_L_SIZE;
    }
    else if (u2OdFormat == 3)
    {
        LOG(0, "FlashPQ Size for OD : 41x41 \n");
        return OD_TBL_L_SIZE;
    }
    else
    {
        LOG(0, "FlashPQ Size for OD : Error \n");
        return 0;
    }
}
#endif

static UINT64 u8CalcFlashPqOffset(UINT8 bIndex, UINT16 nType, UINT16 nModel, UINT16 nSmartPic, UINT16 nSource, UINT16 nIdx)
{
    FLASHPQ_INDEX_CHECK(bIndex);
    
    switch (nType)
    {
        case TYPE_PQ_HEADER:
            return u8PqBaseAddr[bIndex];
        case TYPE_GAMMA_HEADER:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * nModel;
        case TYPE_SCE_HEADER:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * nModel;
        case TYPE_QTY_HEADER:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) * 
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel;
        case TYPE_QTY_DESCRIPT:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] + 
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel + 
                TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (nSmartPic * aFlashPqFileHead[bIndex].wSizeOfQtyTable *
                aFlashPqFileHead[bIndex].bNumOfTiming) + nSource * aFlashPqFileHead[bIndex].wSizeOfQtyTable;
        case TYPE_QTY_CONTENT:      
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] + 
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * nModel + 
                TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (nSmartPic * aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) +
                nSource * aFlashPqFileHead[bIndex].wSizeOfQtyTable + SCRIPT_SIZE(bIndex) + nIdx;        
        case TYPE_OD_HEADER:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * nModel;
		case TYPE_PANEL_HEADER:
			return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * aFlashPqFileHead[bIndex].wNumOfOd +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfPanelTable) * nModel;
    	case TYPE_RAWDATA_HEADER:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * aFlashPqFileHead[bIndex].wNumOfOd +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfPanelTable) * aFlashPqFileHead[bIndex].wNumOfPanelTable;
        default:
        case TYPE_FILE_END:
            return u8PqBaseAddr[bIndex] + bFileHeadSize[bIndex] +
                GAMMA_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfGamma +
                SCE_SIZE(bIndex) * aFlashPqFileHead[bIndex].wNumOfSCE +
                (TBL_HEAD_SIZE + PANEL_LONG_NAME_SIZE(bIndex) + (aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming) *
                (aFlashPqFileHead[bIndex].bNumOfSmartPic + QTY_MAX_MIN_NUM(bIndex))) * aFlashPqFileHead[bIndex].wNumOfQty +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfOdTable) * aFlashPqFileHead[bIndex].wNumOfOd +
                (TBL_HEAD_SIZE + aFlashPqFileHead[bIndex].wSizeOfPanelTable) * aFlashPqFileHead[bIndex].wNumOfPanelTable +
                (aFlashPqFileHead[bIndex].wRawDataTableSize<<10);
    }
}

UINT64 u8CalcRawDataTblOffset(UINT8 bIndex, RAWDATA_TYPE nType, int nTblIndex, int nCase)
{
	UINT64 u8RawStart = u8CalcFlashPqOffset(bIndex, TYPE_RAWDATA_HEADER, 0, 0, 0, 0);	

    if (aFlashPqFileHead == NULL)
    {
        return 0;
    }

	// Error protect !
	nTblIndex = (nTblIndex >= aFlashPqFileHead[bIndex].wNumOfRawDataTable) ? (aFlashPqFileHead[bIndex].wNumOfRawDataTable-1) : nTblIndex;
	nCase = (nCase >= pRawDataSubHead[nTblIndex].u2CaseNum) ? (pRawDataSubHead[nTblIndex].u2CaseNum-1) : nCase;
	
	switch(nType)
	{
		case RAWDATA_TBL_HEAD:
			return u8RawStart;
		case RAWDATA_REG_TBL_HEAD:
			return pRawDataSubHead[nTblIndex].u4TblOffset;
		case RAWDATA_REG_TBL_CONTENT:
			return pRawDataSubHead[nTblIndex].u4TblOffset + TBL_HEAD_ID_SIZE;
		case RAWDATA_DATA_TBL_HEAD:
			return pRawDataSubHead[nTblIndex].u4TblOffset 
				+ (TBL_HEAD_ID_SIZE + pRawDataSubHead[nTblIndex].u2RegNum * 4);
		case RAWDATA_DATA_TBL_SCRIPT:
			return pRawDataSubHead[nTblIndex].u4TblOffset
				+ (TBL_HEAD_ID_SIZE + pRawDataSubHead[nTblIndex].u2RegNum * 4)
				+ (TBL_HEAD_ID_SIZE + (TIMING_SCRIPT_SIZE + pRawDataSubHead[nTblIndex].u2DataSize) * nCase);
		case RAWDATA_DATA_TBL_CONTENT:
			return pRawDataSubHead[nTblIndex].u4TblOffset
				+ (TBL_HEAD_ID_SIZE + pRawDataSubHead[nTblIndex].u2RegNum * 4) 
				+ (TBL_HEAD_ID_SIZE + (TIMING_SCRIPT_SIZE + pRawDataSubHead[nTblIndex].u2DataSize) * nCase)
				+ TIMING_SCRIPT_SIZE;				
        case RAWDATA_TBL_END:
	    default:
		    return 0;
	}

	return 0;
}


#if ENABLE_FLASH_PQ_DEBUG 
static void vDrvDebugFileHeader(UINT8 bIndex)
{
    FLASHPQ_INDEX_CHECK(bIndex);

    LOG(1, "=== FlashPQ<%d> File Header ===\n", bIndex);
    LOG(1, "PQ base address:%X %X\n", u8PqBaseAddr[bIndex]>>32, (UINT32)u8PqBaseAddr[bIndex]);
    LOG(1, "ID %s\n", aFlashPqFileHead[bIndex].bID);
    
    LOG(1, "Gamma num       %d\n", aFlashPqFileHead[bIndex].wNumOfGamma);
    LOG(1, "Gamma size      %d\n", aFlashPqFileHead[bIndex].wSizeOfGammaTable);

    LOG(1, "SCE num         %d\n", aFlashPqFileHead[bIndex].wNumOfSCE);
    LOG(1, "SCE size        %d\n", aFlashPqFileHead[bIndex].wSizeOfSCETable);

    LOG(1, "Qty num         %d\n", aFlashPqFileHead[bIndex].wNumOfQty);
    LOG(1, "Qty size        %d\n", aFlashPqFileHead[bIndex].wSizeOfQtyTable);
    LOG(1, "Timing Num      %d\n", aFlashPqFileHead[bIndex].bNumOfTiming);
    
    LOG(1, "Descript        %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT ? 1 : 0);    
    LOG(1, "MinMax          %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_MAX_MIN ? 1 : 0);    
    LOG(1, "Hash            %d\n", aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_HASH ? 1 : 0);
    
    LOG(1, "Xml ver         %s\n", aFlashPqFileHead[bIndex].bQtyXmlVer);

    if (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0)
    {
        LOG(1, "OD num          %d\n", aFlashPqFileHead[bIndex].wNumOfOd);
        LOG(1, "OD size         %d\n", aFlashPqFileHead[bIndex].wSizeOfOdTable);
        LOG(1, "OD Format       %d\n", aFlashPqFileHead[bIndex].wConfigOfOd >> 4);
        LOG(1, "Support FB      %d\n", aFlashPqFileHead[bIndex].wConfigOfOd & E_OD_SUPPORT_FB);
        LOG(1, "Support PCID    %d\n", aFlashPqFileHead[bIndex].wConfigOfOd & E_OD_SUPPORT_PCID);
    }    

	LOG(1, "Panel num       %d\n", aFlashPqFileHead[bIndex].wNumOfPanelTable);
    LOG(1, "Panel size      %d\n", aFlashPqFileHead[bIndex].wSizeOfPanelTable);
	
	LOG(1, "RawData Table num   %d\n",	aFlashPqFileHead[bIndex].wNumOfRawDataTable); 
    LOG(1, "RawData Size(KB)    0x%08x\n",	aFlashPqFileHead[bIndex].wRawDataTableSize*1024); 
}

static void vDrvDebugSCE(UINT8 bIndex)
{
    UINT32 i;
    
    Printf("\nBASE_SCE_TBL = \n");
    for (i = 0; i < aFlashPqFileHead[bIndex].wSizeOfSCETable; i ++)
    {
        Printf("%2x ", BASE_SCE_TBL[i]);
        if ((i+1) % 28 == 0)
        {
            Printf("\n");
        }
    }
    
    Printf("\nMIN_SCE_TBL = \n");
    for (i = 0; i < aFlashPqFileHead[bIndex].wSizeOfSCETable; i ++)
    {
        Printf("%2x ", MIN_SCE_TBL[i]);
        if ((i+1) % 28 == 0)
        {
            Printf("\n");
        }
    }
}

static void vDrvDebugOD(UINT16 u2OdTblSize)
{    
    UINT32 i;
    
    Printf("\nOD_TBL = \n");
    for (i = 0; i < u2OdTblSize; i ++)
    {
        Printf("%2x ", OD_Table[i]);
        if ((i+1) % 16 == 0)
        {
            Printf("\n");
        }
    }
    Printf("\nFB_TBL = \n");
    for (i = 0; i < FB_TBL_SIZE; i ++)
    {
        Printf("%2x ", FB_Table[i]);
        if ((i+1) % FB_TBL_DIM == 0)
        {
            Printf("\n");
        }
    }

    Printf("\nPCID_TBL = \n");
    for (i = 0; i < PCID_TBL_SIZE; i ++)
    {
        Printf("%2x ", PCID_Table[i]);
        if ((i+1) % PCID_TBL_DIM == 0)
        {
            Printf("\n");
        }
    }
}

static void vDrvDebugPanel(UINT16 u2PanelTblSize)
{
	UINT32 i;
	UINT8 * PanelTableAdr = (UINT8*)(&rFlashPanelTable);

	Printf("\nPanel Table = \n");
	for (i = 0; i < u2PanelTblSize; i ++)
    {        
        Printf("%2x ", *(PanelTableAdr+i));
        if ((i+1) % 16 == 0)
        {
            Printf("\n");
        }
    }
}

static void vDrvDebugGamma(void)
{
    UINT32 i, j;    
    
    for (i = 0; i < FLASH_PQ_GAMMA_CHANNEL_NUM; i ++)
    {        
        Printf("\nGAMMA_256 [%d]= \n", i);
        for (j = 0; j < FLASH_PQ_GAMMA_CHANNEL_SIZE_257; j ++)
        {
            Printf("%2x ", GAMMA_256[i][j]);
            if ((j+1) % 16 == 0)
            {
                Printf("\n");
            }
        }
    }
}

static void vDrvDebugQtyTable(UINT8 bIndex)
{
    UINT32 i, j;

    UINT8 u1NumOfTiming = aFlashPqFileHead[bIndex].bNumOfTiming;
    UINT16 u2SizeOfQtyTable = aFlashPqFileHead[bIndex].wSizeOfQtyTable;
        
    for (j = 0; j < u1NumOfTiming; j++)
    {
        Printf("\nTiming %d\n", j);
        for (i = 0; i < u2SizeOfQtyTable; i++)
        {
            Printf("%02X ", (int)READ_CUSTOM_QTY_TBL(j, (i | (UINT16)bIndex<<QTY_TBL_REF_INDEX_SHIFT)));
            if (i % 16 == 15)
            {
                Printf("\n");
            }
        }
    }
}

static void vDrvDebugQtyMinMax(UINT8 bIndex)
{   
    int i;
    
    Printf("\nMin  Max\n");
    for (i = 0; i < aFlashPqFileHead[bIndex].wSizeOfQtyTable - SCRIPT_SIZE(bIndex); i++)
    {
        Printf("%02X  %02X\n", bQtyMin[bIndex][i], bQtyMax[bIndex][i]);
    }

    Printf("\nQty Min Max\n");
    for (i = 0; i < QUALITY_MAX; i++)
    {
        Printf("Dft[%d]  %02X  %02X\n", i, READ_DEFUALT_QTY_TBL_MIN(i), READ_DEFUALT_QTY_TBL_MAX(i));
    }
}

static void vDrvDebugRawDataSubHeader(UINT16 u2TblIndex)
{    
    if (pRawDataSubHead)
    {           
        Printf("=========Raw Data=========\n");    
        Printf("Table               0x%04x\n", u2TblIndex);    
        Printf("ID                  %s\n", pRawDataSubHead[u2TblIndex].bID);
        Printf("Case Num            %d\n", pRawDataSubHead[u2TblIndex].u2CaseNum);    
        Printf("Reg Num             %d\n", pRawDataSubHead[u2TblIndex].u2RegNum);    
        Printf("Data Size           %d\n", pRawDataSubHead[u2TblIndex].u2DataSize);    
        Printf("Offset              0x%08x\n", pRawDataSubHead[u2TblIndex].u4TblOffset);    
        Printf("Data Script         %s\n", pRawDataSubHead[u2TblIndex].bScript);
    }
}

static void vDrvDebugRawData(void)
{    
    UINT16 i, j;
    UINT32 u4Temp, u4Addr, u4Value, u4Mask, u4Pos = 0;
    UINT8 u1Type, u1Upper, u1Lower, u1Check, u1Byte;
    UINT16 u2TblIndex = u2ApiFlashPqGetRawDataTblId();
    UINT16 u2CaseIndex = u2ApiFlashPqGetRawDataCaseId();

    if ((pRawDataSubHead == NULL) || (pRawRegTbl == NULL) || (pRawDataTbl == NULL))
    {        
        Printf("Raw data Buffer not ready \n");
        return;
    }
    
    Printf("\n==================== Raw Data ===================== \n");    
    Printf("Table       %d\n", u2TblIndex);    
    Printf("Case        %d\n", u2CaseIndex);
    Printf("Index    Addr      Value    type  upper  lower  check  byte\n");

    for(i = 0; i < pRawDataSubHead[u2TblIndex].u2RegNum; i++)
    {
        u4Value = 0;
        u4Temp = pRawRegTbl[i];
        u1Type = u4Temp >> 30;
        u1Upper = (u4Temp & 0x3e000000) >> 25;
        u1Lower = (u4Temp & 0x1f00000) >> 20;
        u1Check = u4Temp & 0x3;
		u4Mask = (0xffffffff >> (32 - u1Upper - 1)) & (0xffffffff << u1Lower);
        u1Byte = (u1Upper - u1Lower + 1 + 7)/8;
        u1Byte = (u1Byte == 3) ? 4 : u1Byte;

        for(j = 0; j < u1Byte; j++)
        {
            u4Value |= pRawDataTbl[u4Pos++] << (j * 8);
        }

        switch(u1Type)
        {
            case SV_REG_DIRECT:
                u4Addr = (u4Temp & 0xffffc) | 0xf0000000;
                break;
            case SV_REG_XDATA:
                u4Addr = u4Temp & 0xffffc;
                break;
            case SV_REG_USER_DATA:
                u4Addr = 0;
            default:
                break;
        }
        
        Printf("%04d   %08x  %08x    %d     %02d     %02d      %x     %d\n", 
            i, u4Addr, u4Value, u1Type, u1Upper, u1Lower, u1Check, u1Byte);
    }    
}
#endif



void vApiFlashPqDataReadFlash(UINT8 bIndex, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    #ifndef CC_MTK_LOADER     
    STORG_SyncRead(DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBasePartition) ,u8Offset, (VOID*)u4MemPtr, u4MemLen);
	#else
	#ifdef CC_MSDC_ENABLE
	Loader_ReadMsdc((UINT32)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition), u8Offset, (VOID*)u4MemPtr, u4MemLen);
	#else
    Loader_ReadNandFlash((UINT32)DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBasePartition), u8Offset, (VOID*)u4MemPtr, u4MemLen);
    #endif
	#endif
}

UINT8 * bApiFlashPqGetGammaHeadAddr(UINT8 index)
{
	if(IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord))
	{
		return (UINT8 *)(&aFlashPqGammaHead_Ex);
	}
	else
	{
		return (UINT8 *)(&aFlashPqGammaHead);
	}
}

UINT8 * bApiFlashPqGetGammaInfo(UINT8 index, UINT8 bType)
{
	if(IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord))
	{
		switch(bType)
		{
			case HD_ID:
				return (UINT8 *)(aFlashPqGammaHead_Ex.bID);
			case HD_SCRIPT:
				return (UINT8 *)(aFlashPqGammaHead_Ex.bScript);
			case HD_SUB_SCRIPT:
				return (UINT8 *)(aFlashPqGammaHead_Ex.bSubScript);
			case HD_DATE:
				return (UINT8 *)(aFlashPqGammaHead_Ex.bDate);
			default:
				return NULL;
		}
	}
	else
	{
		switch(bType)
		{
			case HD_ID:
				return (UINT8 *)(aFlashPqGammaHead.bID);
			case HD_SCRIPT:
				return (UINT8 *)(aFlashPqGammaHead.bScript);
			case HD_DATE:
				return (UINT8 *)(aFlashPqGammaHead.bDate);
			default:
				return NULL;
		}
	}
}

UINT8 * bApiFlashPqGetSceHeadAddr(UINT8 index)
{
	if(IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord))
	{
		return (UINT8 *)(&aFlashPqSceHead_Ex);
	}
	else
	{
		return (UINT8 *)(&aFlashPqSceHead);
	}
}

UINT8 * bApiFlashPqGetSceInfo(UINT8 index, UINT8 bType)
{
	if(IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[index].dwConfigWord))
	{
		switch(bType)
		{
			case HD_ID:
				return (UINT8 *)(aFlashPqSceHead_Ex.bID);
			case HD_SCRIPT:
				return (UINT8 *)(aFlashPqSceHead_Ex.bScript);
			case HD_SUB_SCRIPT:
				return (UINT8 *)(aFlashPqSceHead_Ex.bSubScript);
			case HD_DATE:
				return (UINT8 *)(aFlashPqSceHead_Ex.bDate);
			default:
				return NULL;
		}
	}
	else
	{
		switch(bType)
		{
			case HD_ID:
				return (UINT8 *)(aFlashPqSceHead.bID);
			case HD_SCRIPT:
				return (UINT8 *)(aFlashPqSceHead.bScript);
			case HD_DATE:
				return (UINT8 *)(aFlashPqSceHead.bDate);
			default:
				return NULL;
		}
	}
}

UINT8 bApiFlashPqIsSupportSCEbySource(UINT8 index)
{
	if(aFlashPqFileHead[index].dwConfigWord & FC_SUPPORT_SCE_BY_SOURCE)
	{
		return SV_TRUE;
	}
	return SV_FALSE;
}

UINT8 uApiFlashPqGetSceSrcNum(UINT8 index)
{
	if(aFlashPqFileHead[index].dwConfigWord & FC_SUPPORT_SCE_BY_SOURCE)
	{
		return aFlashPqFileHead[index].bNumOfSCESource;
	}
	else
	{
		return (1);
	}
}


void vApiFlashPqReadFlash(UINT8 bIndex, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    #ifdef CC_COMPRESSED_FLASH_PQ_DATA   
    if (pFlashPqData[bIndex]!=NULL)
    {
        x_memcpy((VOID*)u4MemPtr, (VOID*)(pFlashPqData[bIndex]+u8Offset-u8PqBaseAddr[bIndex]), u4MemLen);
    }
    #else
    vApiFlashPqDataReadFlash(bIndex, u8Offset, u4MemPtr, u4MemLen);
    #endif
}

#ifdef CC_COMPRESSED_FLASH_PQ_DATA  
void vApiFlashPqToDram(UINT8 bIndex)
{
    UINT8* pImageAddr = NULL;
    UINT32 u4BlockSize = 0;
    UINT32 i;
    UINT32 u4DstSize, u4DstAddr, u4SrcAddr, u4SrcSize;    
    LZMA_FILE_HEADER* prLzmaHdr = (LZMA_FILE_HEADER*)pImageAddr;
    UINT32 u4Ret;

    LOG(0, "====-- Flash PQ to Dram ======\n");
    if(pFlashPqData[bIndex] != NULL)
    {
        LOG(5, "pFlashPqData[%d] is already set\n", bIndex);
        return;
    }

    #ifndef CC_MTK_LOADER  
    if (DRVCUST_OptGet(eFlagFlashPqEnable))
    #else
    if (DRVCUST_InitGet(eFlagFlashPqEnable))
    #endif
    {
        // Check if flash PQ address & block size definition.
        
        #ifndef CC_MTK_LOADER  
        if ((((UINT64)DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
            (DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
        #else
		if ((((UINT64)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
            (DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
        #endif 
        {
            LOG(0, "FlashPQ base address is zero !\n");
            return;
        }

        #ifdef CC_MTK_LOADER
        if (DRVCUST_InitGet(eFlashPqUseNandFalsh))
        #else
        if (DRVCUST_OptGet(eFlashPqUseNandFalsh))
        #endif
        {
            #ifdef CC_MTK_LOADER
            u4BlockSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)));
            #else
            u4BlockSize = DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)));
            #endif
        }
        else
        {        
            #ifdef CC_MTK_LOADER
            u4BlockSize = DRVCUST_InitGet((QUERY_TYPE_T)(eNorFlashPartSize0+DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)));
            #else
            u4BlockSize = DRVCUST_OptGet((QUERY_TYPE_T)(eNorFlashPartSize0+DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)));
            #endif
        }
    
        // Get PQ base address.
        #ifndef CC_MTK_LOADER 
        u8PqBaseAddr[bIndex] = //((UINT64)DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#else
		u8PqBaseAddr[bIndex] = //((UINT64)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#endif

        // PQ base address must be 32-bit alignment.
        if ((u8PqBaseAddr[bIndex] % 4) != 0)
        {
            LOG(0, "FlashPQ base address is not 32-bit align !\n");
            return;
        }

        pImageAddr = (UINT8*)x_mem_alloc(u4BlockSize);

        if (pImageAddr == NULL)
        {            
            Printf("Flash PQ Allocate Memory failed\n");
            return;
        }
        
        vApiFlashPqDataReadFlash(bIndex, u8PqBaseAddr[bIndex], (UINT32)pImageAddr, u4BlockSize-u8PqBaseAddr[bIndex]);
    }
    else
    {
        Printf("Flash PQ not support Dram mode\n");
        return;
    }

	Printf("======  Flash PQ Read to dram Size 0x%x ======\n", u4BlockSize);

	for (i=0; i < sizeof(LZMA_FILE_HEADER); i++)
	{            
		Printf("%02x ", pImageAddr[i]);
		if ((i+1) % 16 == 0) Printf("\n");
    }
    
    pFlashPqData[bIndex] = (UINT8*)x_mem_alloc(prLzmaHdr->org_size);
    if (pFlashPqData[bIndex] == NULL)
    {
        Printf("Flash PQ Allocate Memory %x failed\n", prLzmaHdr->org_size);
        // Flash PQ data in dram
        pFlashPqData[bIndex] = pImageAddr;        
        return;        
    }

    u4DstSize = prLzmaHdr->org_size;
    u4DstAddr = (UINT32)pFlashPqData[bIndex];
    u4SrcAddr = (UINT32)pImageAddr + sizeof(LZMA_FILE_HEADER);
    u4SrcSize = prLzmaHdr->compressed_size;

    Printf("\n==========  LZMA decode ==========\n");
    Printf("Src  addr=0x%08x   len=%d(0x%08x) \n", u4SrcAddr, u4SrcSize, u4SrcSize);    
    Printf("Dst  addr=0x%08x   len=%d(0x%08x) \n", u4DstAddr, u4DstSize, u4DstSize);

    u4Ret = (UINT32)LzmaUncompress((unsigned char*)u4DstAddr, (size_t*)&u4DstSize,
        (unsigned char*)u4SrcAddr, (size_t*)&u4SrcSize,
        (unsigned char *)&prLzmaHdr->props, sizeof(CLzmaEncProps));
    
    if (u4Ret != SZ_OK)
    {
        Printf("LZMA fail, code=%d\n", u4Ret);
        // Flash PQ data in dram
        pFlashPqData[bIndex] = pImageAddr;        
        return;        
    }
    else
    {
    	pFlashPqData[bIndex] = (UINT8*)u4DstAddr; 
    	Printf("LZMA decode ok\n");
    }
    
    x_mem_free(pImageAddr);
}
#endif

UINT8 * bApiFlashPqGetVersion(UINT8 bIndex, UINT8 bType)
{
    switch (bType)
    {
        default:
        case FLASHPQ_XML_VER:
            return (UINT8 *)(aFlashPqFileHead[bIndex].bQtyXmlVer);
        case FLASHPQ_GAMMA_VER:
            return (UINT8 *)(bApiFlashPqGetGammaInfo(bIndex, HD_SCRIPT));
        case FLASHPQ_SCE_VER:
            return (UINT8 *)(bApiFlashPqGetSceInfo(bIndex, HD_SCRIPT));
        case FLASHPQ_QTY_VER:
            return (UINT8 *)(aFlashPqQtyHead[bIndex].bScript);
        case FLASHPQ_OD_VER:
            return (UINT8 *)(aFlashPqOdHead.bScript);            
    }
}

UINT8 bApiFlashPqVerifyHeader(UINT8 bIndex)
{
    FLASHPQ_INDEX_CHECK(bIndex);

    #ifndef CC_MTK_LOADER  
    if (DRVCUST_OptGet(eFlagFlashPqEnable))
    #else
    if (DRVCUST_InitGet(eFlagFlashPqEnable))
    #endif
    {
        // Check if flash PQ address & block size definition.
        #ifndef CC_MTK_LOADER  
        if ((((UINT64)DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
            (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
        #else
		if ((((UINT64)DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) == 0) ||
            (DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBlockSize) == 0))
		#endif
        {
            LOG(0, "FlashPQ base address is zero !\n");
            return (SV_FALSE);
        }
        
        #ifdef CC_COMPRESSED_FLASH_PQ_DATA 
        vApiFlashPqToDram(bIndex);
        #endif
        
        // Get PQ base address.
        #ifndef CC_MTK_LOADER 
        u8PqBaseAddr[bIndex] = //((UINT64)DRVCUST_OptGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#else
		u8PqBaseAddr[bIndex] = //((UINT64)DRVCUST_InitGet(aFlashPqFlashInfo[bIndex].dwFlashBasePartition)<<32) +
            DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashBaseAddress) + DRVCUST_InitGet((QUERY_TYPE_T)aFlashPqFlashInfo[bIndex].dwFlashOffset);
		#endif

        // PQ base address must be 32-bit alignment.
        if ((u8PqBaseAddr[bIndex] % 4) != 0)
        {
            LOG(0, "FlashPQ base address is not 32-bit align !\n");
            ASSERT(0);
        }

        vApiFlashPqReadFlash(bIndex, u8PqBaseAddr[bIndex], 
            (UINT32)&aFlashPqFileHead[bIndex], (UINT32)(sizeof(struct tagFileHead)));

        if ((x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID) == 0) ||
            (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0))
        {            
            if (x_strcmp((CHAR *) (aFlashPqFileHead[bIndex].bID), (CHAR *) FLASHPQ_ID_EX) == 0)
            {
                bFileHeadSize[bIndex] = sizeof(struct tagFileHead);
                bPqExtesion[bIndex] = SV_TRUE;
            }
            else
            {  
                bFileHeadSize[bIndex] = sizeof(struct tagFileHead) - 32;                   
                bPqExtesion[bIndex] = SV_FALSE;
                LOG(0, "Old version Flash PQ binary \n");
            }
            
			if ((aFlashPqFileHead[bIndex].wNumOfGamma>>8) == 48)
			{
				aFlashPqFileHead[bIndex].wNumOfGamma = aFlashPqFileHead[bIndex].wNumOfGamma & 0xFF;
			}
			if ((aFlashPqFileHead[bIndex].wNumOfSCE>>8) == 48)
			{
				aFlashPqFileHead[bIndex].wNumOfSCE = aFlashPqFileHead[bIndex].wNumOfSCE & 0xFF;
			}
			if ((aFlashPqFileHead[bIndex].wNumOfQty>>8) == 48)
			{
				aFlashPqFileHead[bIndex].wNumOfQty = aFlashPqFileHead[bIndex].wNumOfQty & 0xFF;
			}

            LOG(0, "FlashPQ ID OK\n");
            bFlashPQInit[bIndex] = SV_TRUE;
                        
            #if ENABLE_FLASH_PQ_DEBUG
            vDrvDebugFileHeader(bIndex);
            #endif
            
            return (SV_TRUE);
        }
        else
        {
            bFlashPQInit[bIndex] = SV_FALSE;
            LOG(0, "FlashPQ ID NG\n");
            return (SV_FALSE);
        }
    }
    else
    {
        LOG(0, "Not Support FlashPQ\n");
        return (SV_FALSE);
    }
}

#ifndef CC_MTK_LOADER
UINT16 wApiFlashPqGetSceIndexBySrcTiming(UINT16 u1SrcTypTmg)
{
	UINT16 bCustomSrcTypTmg;

	for(bCustomSrcTypTmg=0;
		READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN)!=
			SOURCE_TYPE_TIMING_MAX
		&&READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)!=
			SOURCE_TYPE_TIMING_MAX;
		bCustomSrcTypTmg++)
	{
		if(READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN)
			>READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END))
		{
        	WRITE_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END,
            READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN));

		}
		if((u1SrcTypTmg>=READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN))
			&&(u1SrcTypTmg<=READ_Sce_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)))
		{
			return bCustomSrcTypTmg;
		}
	}
	return SOURCE_TYPE_TIMING_MAX;
}

UINT8 u1DrvCheckGammaFlashPQSel(UINT16 u2GammaIndex)
{
    UINT8 i;
    
    for (i=0; i< FLASHPQ_NUM; i++)
    {
        if (bFlashPQInit[i] != SV_TRUE)
        {
            LOG(0, "Update Gamma Check init failed ----  %d\n", i);
        }
        else if (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[i].dwSwGammaEnable) == 0)
        {
            LOG(0, "FlashPQ[%d] not support Gamma ---- \n", i);
        }
        else if (u2GammaIndex >= aFlashPqFileHead[i].wNumOfGamma)
        {
            LOG(0, "FlashPQ[%d] Gamma Index Error! %d > %d\n", i, u2GammaIndex, aFlashPqFileHead[i].wNumOfGamma);
        }
        else
        {
            return i;
        }    
    }

    return 0xFF;
}

UINT8 bApiFlashPqUpdateGamma(void)
{
    UINT8 bi;
    UINT8* u1GammaBufPtr;
    UINT64 u8GammaAddr;
    UINT16 u2GammaMdlSize;
    UINT16 u2GammaChlSize;
    UINT16 u2GammaIndex = vApiFlashPqGetGammaId();
    UINT8 u1Index = 0;
    
    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }

    u1Index = u1DrvCheckGammaFlashPQSel(u2GammaIndex);
    
    if (u1Index == 0xFF)
    {
        return (SV_FALSE);
    }

    // Calculate start address of Gamma table.
    u8GammaAddr = u8CalcFlashPqOffset(u1Index, TYPE_GAMMA_HEADER, u2GammaIndex, 0, 0, 0);

    // Gamma head base address must be 32-bit alignment.
    if ((u8GammaAddr % 4) != 0)
    {
        LOG(0, "FlashPQ gamma address error !(%d)\n", u8GammaAddr);
        return (SV_FALSE);
    }

    // Check Gamma table size in FlashPQ bin file: 256 or 257
    if (aFlashPqFileHead[u1Index].wSizeOfGammaTable == 
        FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM)
    {
        u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_256;
    }
    else if (aFlashPqFileHead[u1Index].wSizeOfGammaTable == 
        FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM + 16)
    {
        u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_257;
    }
    else
    {
        LOG(0, "FlashPQ Gamma size error !\n");
        return (SV_FALSE);
    }
    
    u2GammaMdlSize = GAMMA_HEAD_SIZE(u1Index) + aFlashPqFileHead[u1Index].wSizeOfGammaTable;
    if ((u1GammaBufPtr = (UINT8*)x_mem_alloc(u2GammaMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ Gamma memory request fail !\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u1Index, u8GammaAddr, (UINT32)u1GammaBufPtr, (UINT32)u2GammaMdlSize);

    x_memcpy(bApiFlashPqGetGammaHeadAddr(u1Index), u1GammaBufPtr, GAMMA_HEAD_SIZE(u1Index));

    LOG(1, "\nGamma ID        %s\n", (bApiFlashPqGetGammaInfo(u1Index, HD_ID)));
    LOG(1, "PANEL NO        %d\n", u2GammaIndex);
    LOG(1, "DATE            %s\n", (bApiFlashPqGetGammaInfo(u1Index, HD_DATE)));
    LOG(1, "SCRIPT          %s\n", (bApiFlashPqGetGammaInfo(u1Index, HD_SCRIPT)));

    // Check Gamma ID
    if (x_strcmp((CHAR *) (bApiFlashPqGetGammaInfo(u1Index, HD_ID)), (CHAR *) GAMMA_ID) != 0)
    {
        LOG(0, "Gamma ID [%d]  Error  %s\n", u2GammaIndex, (bApiFlashPqGetGammaInfo(u1Index, HD_ID)));
        x_mem_free(u1GammaBufPtr);
        return (SV_FALSE);
    }
    
    for (bi = 0; bi < FLASH_PQ_GAMMA_CHANNEL_NUM; bi ++)
    {
        x_memcpy(&GAMMA_256[bi], 
            u1GammaBufPtr + GAMMA_HEAD_SIZE(u1Index) + u2GammaChlSize * bi, 
            u2GammaChlSize);

        // Calculate point 257 if bin file only store 256 points
        if (u2GammaChlSize == FLASH_PQ_GAMMA_CHANNEL_SIZE_256)
        {
            vDrvGammaTopPt(GAMMA_256[bi]);
        }
    }

    #if ENABLE_FLASH_PQ_DEBUG
    if (FLASH_PQ_LOG(E_FLASH_PQ_GAMMA_LOG))
    {
        vDrvDebugGamma();
    }
    #endif
    
    x_mem_free(u1GammaBufPtr);
    LOG(1, "FlashPQ Gamma OK\n");
    return (SV_TRUE);
}

UINT8 u1DrvCheckSCEFlashPQSel(UINT16 u2SCEIndex)
{
    UINT8 i;
    
    for (i=0; i< FLASHPQ_NUM; i++)
    {
        if (bFlashPQInit[i] != SV_TRUE)
        {
            LOG(0, "Update SCE Check init failed  ----  %d\n", i);
        }    
        else if (DRVCUST_OptGet((QUERY_TYPE_T)(aFlashPqSWInfo[i].dwSwSceEnable)) == 0)
        {
            LOG(0, "FlashPQ[%d] not support SCE\n", i);
        }
        else if (u2SCEIndex >= aFlashPqFileHead[i].wNumOfSCE)
        {
            LOG(0, "FlashPQ[%d] SCE Index Error! %d >= %d\n", i, u2SCEIndex, aFlashPqFileHead[i].wNumOfSCE);
        }
        else
        {
            return i;
        }
    }

    return 0xFF;    
}

UINT8 bApiFlashPqUpdateSCE(void)
{
    UINT8* u1SceBufPtr;
    UINT64 u8SceAddr;
    UINT16 u2SceMdlSize;
    UINT16 u2PanelIndex=0;
    UINT8 u1Index = 0;

	UINT16 u2SceIndex;
	UINT16 u1SrcTypTmg = 0, u1SceSrcIdx=0;

    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }
	
	u2PanelIndex = vApiFlashPqGetSceId();

    u1Index = u1DrvCheckSCEFlashPQSel(u2PanelIndex);
	
    if (u1Index == 0xFF)
    {
        return (SV_FALSE);
    }

	if(bApiFlashPqIsSupportSCEbySource(u1Index))
	{
		u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);

		u1SceSrcIdx = wApiFlashPqGetSceIndexBySrcTiming(u1SrcTypTmg);
		
		if(u1SceSrcIdx >= uApiFlashPqGetSceSrcNum(0))
		{
			u1SceSrcIdx = (uApiFlashPqGetSceSrcNum(0)-1);
		}	
		
		u2SceIndex = u2PanelIndex * uApiFlashPqGetSceSrcNum(0)+ u1SceSrcIdx;		
	}
	else
	{
		u2SceIndex = u2PanelIndex;
	}
	
    if (u1Index == 0xFF)
    {
        return (SV_FALSE);
    }    

    // Calcuate start address of SCE table.
    u8SceAddr = u8CalcFlashPqOffset(u1Index, TYPE_SCE_HEADER, u2SceIndex, 0, 0, 0);

    // SCE head base address must be 32-bit alignment.
    if ((u8SceAddr % 4) != 0)
    {
        LOG(0, "FlashPQ SCE address error!!!(%d)\n", u8SceAddr);
        return (SV_FALSE);
    }

    u2SceMdlSize = SCE_HEAD_SIZE(u1Index) + aFlashPqFileHead[u1Index].wSizeOfSCETable;

    if ((u1SceBufPtr = (UINT8*)x_mem_alloc(u2SceMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ SCE memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u1Index, u8SceAddr, (UINT32)u1SceBufPtr, (UINT32)u2SceMdlSize);

    x_memcpy(bApiFlashPqGetSceHeadAddr(u1Index), u1SceBufPtr, SCE_HEAD_SIZE(u1Index));

    LOG(1, "\nDef SCE ID      %s\n", bApiFlashPqGetSceInfo(u1Index, HD_ID));
    LOG(1, "Def SCE NO      %d\n", u2PanelIndex);
    LOG(1, "DATE            %s\n", bApiFlashPqGetSceInfo(u1Index, HD_DATE));
    LOG(1, "SCRIPT          %s\n", bApiFlashPqGetSceInfo(u1Index, HD_SCRIPT));
	
    if(IS_SUPPORT_SUB_SCRIPT(aFlashPqFileHead[u1Index].dwConfigWord))
    {
    	LOG(1, "SUB SCRIPT          %s\n", (bApiFlashPqGetSceInfo(u1Index, HD_SUB_SCRIPT)));
    	LOG(1, "Sce Source Index    %d,Sce_panel_source Index		%d\n", u1SceSrcIdx,u2SceIndex);
    }

    // Check SCE ID
    if (x_strcmp((CHAR *) bApiFlashPqGetSceInfo(u1Index, HD_ID), (CHAR *) SCE_ID) != 0)
    {
        LOG(0, "Def SCE ID [%d]  Error  %s\n", u2SceIndex, bApiFlashPqGetSceInfo(u1Index, HD_ID));
        x_mem_free(u1SceBufPtr);
        return (SV_FALSE);
    }

    if (aFlashPqFileHead[u1Index].wSizeOfSCETable > 1216)
    {
        LOG(0, "SCE table size %d is too big\n", aFlashPqFileHead[u1Index].wSizeOfSCETable);
        x_mem_free(u1SceBufPtr);
        return (SV_FALSE);
    }
	
  	if (aFlashPqFileHead[u1Index].wSizeOfSCETable <= 960)
    {
		gSCE_3DMapping = FALSE;
    }
	else
	{
		gSCE_3DMapping = TRUE;
	}  
    
    x_memcpy(&DEFAULT_SCE_TBL, (u1SceBufPtr + SCE_HEAD_SIZE(u1Index)), aFlashPqFileHead[u1Index].wSizeOfSCETable);
    x_memcpy(&BASE_SCE_TBL, &DEFAULT_SCE_TBL, aFlashPqFileHead[u1Index].wSizeOfSCETable);

    if (DRVCUST_OptGet(eFlagFlashPqSCEMin) == SV_TRUE) // Normanl Mode
    {        
//        u2PanelIndex = vApiFlashPqGetMinSceId();
		if(bApiFlashPqIsSupportSCEbySource(u1Index))
		{
			u2SceIndex = u2PanelIndex * uApiFlashPqGetSceSrcNum(0)+ (uApiFlashPqGetSceSrcNum(0)+1);
		}
		else
		{
			u2SceIndex = u2PanelIndex+1;
		}
        
        if (u2SceIndex >= aFlashPqFileHead[u1Index].wNumOfSCE)
        {
            LOG(0, "SCE Index Error! %d >= %d \n", u2SceIndex, aFlashPqFileHead[u1Index].wNumOfSCE);
            x_mem_free(u1SceBufPtr);
            return (SV_FALSE);
        }
        
        u8SceAddr = u8CalcFlashPqOffset(u1Index, TYPE_SCE_HEADER, u2SceIndex, 0, 0, 0);

        if ((u8SceAddr % 4) != 0)
        {
            LOG(0, "FlashPQ sce address error!!!(%d)\n", u8SceAddr);
            x_mem_free(u1SceBufPtr);
            return (SV_FALSE);
        }
        
        vApiFlashPqReadFlash(u1Index, u8SceAddr, (UINT32)u1SceBufPtr, (UINT32)u2SceMdlSize);
        
        x_memcpy(bApiFlashPqGetSceHeadAddr(u1Index), u1SceBufPtr, SCE_HEAD_SIZE(u1Index));
        
        LOG(1, "\nMin SCE ID      %s\n", bApiFlashPqGetSceInfo(u1Index, HD_ID));
        LOG(1, "Min SCE NO      %d\n", u2SceIndex);
        LOG(1, "DATE            %s\n", bApiFlashPqGetSceInfo(u1Index, HD_DATE));
        LOG(1, "SCRIPT          %s\n", bApiFlashPqGetSceInfo(u1Index, HD_SCRIPT));

        if (x_strcmp((CHAR *) bApiFlashPqGetSceInfo(u1Index, HD_ID), (CHAR *) SCE_ID) != 0)
        {
            LOG(0, "Min SCE ID [%d]  Error  %s\n", u2SceIndex, bApiFlashPqGetSceInfo(u1Index, HD_ID));
            x_mem_free(u1SceBufPtr);
            return (SV_FALSE);
        }
        
        x_memcpy(&MIN_SCE_TBL, (u1SceBufPtr + SCE_HEAD_SIZE(u1Index)), aFlashPqFileHead[u1Index].wSizeOfSCETable);
    }
    
    #if ENABLE_FLASH_PQ_DEBUG
    if (FLASH_PQ_LOG(E_FLASH_PQ_SCE_LOG))
    {
        vDrvDebugSCE(u1Index);
    }
    #endif
    
    x_mem_free(u1SceBufPtr);    
    LOG(1, "FlashPQ SCE OK\n");
    return (SV_TRUE);
}

UINT8 bApiFlashPqSystemInfo(void)
{
    // support flashpq info only when flashpq enabled
    if(DRVCUST_OptGet(eFlagFlashPqEnable))
    {
        LOG(0, "[FLASHPQ] SUPPORT_FLASH_PQ\n");            
        LOG(0, "[FLASHPQ] FLASH_PQ_BASE_PARTITION = %d\n", DRVCUST_OptGet(eFlashPqBasePartition));
        #ifdef SUPPORT_2ND_FLASHPQ
        LOG(0, "[FLASHPQ] FLASH_PQ_2ndpart_BASE_PARTITION = %d\n", DRVCUST_OptGet(eFlashPq2ndpartBasePartition));
        #endif
        
        if(DRVCUST_OptGet(eFlashPqUseNandFalsh))
        {
            LOG(0, "[FLASHPQ] FLASH_PQ_USE_NAND_FLASH\n");                        
            LOG(0, "[FLASHPQ] NAND_PART_SIZE = 0x%x\n", 
                DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPqBasePartition))));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPqBaseAddress));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPqOffset));                                           
            
            #ifdef SUPPORT_2ND_FLASHPQ
            LOG(0, "\n[FLASHPQ] NAND_2nd_PART_SIZE = 0x%x\n", 
                DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPq2ndpartBasePartition))));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartBaseAddress));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartOffset));                                           
            #endif
        }
        else
        {
            LOG(0, "[FLASHPQ] FLASH_PQ_USE_NOR_FLASH\n");                
            LOG(0, "[FLASHPQ] NOR_PART_SIZE = 0x%x\n", 
                DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPqBasePartition))));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPqBaseAddress));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPqOffset));                                                       

            #ifdef SUPPORT_2ND_FLASHPQ
            LOG(0, "\n[FLASHPQ] NAND_2nd_PART_SIZE = 0x%x\n", 
                DRVCUST_OptGet((QUERY_TYPE_T)(eNANDFlashPartSize0+DRVCUST_OptGet(eFlashPq2ndpartBasePartition))));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_ADDRESS = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartBaseAddress));                    
            LOG(0, "[FLASHPQ] FLASH_PQ_BASE_OFFSET = 0x%x\n", DRVCUST_OptGet(eFlashPq2ndpartOffset));                                           
            #endif
        }
    }
    else
    {
        LOG(0, "[FLASHPQ] NOT_SUPPORT_FLASH_PQ\n");            
    }
    return 0;
}

UINT8 bApiFlashPqUpdateQtyDft(UINT8 bIndex)
{
    UINT8* u1QtyPtr;
    UINT8* u1QtyBufPtr;
    UINT8 bSmartPicIndex;
    UINT8 bQtyScriptSize = SCRIPT_SIZE(bIndex);
    UINT16 u2QtyMdlSize, i;
    UINT16 u2QtyIndex = vApiFlashPqGetQtyId(bIndex);
    UINT64 u8QtyAddr;

    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }

    if (bFlashPQInit[bIndex] != SV_TRUE)
    {
        LOG(0, "Update Qty Check init failed\n");
        return (SV_FALSE);
    }

    if (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[bIndex].dwSwQtyEnable) == 0)
    {
        LOG(0, "FlashPQ not support QtyTable\n");
        return (SV_FALSE);
    }
 
    if (u2QtyIndex >= aFlashPqFileHead[bIndex].wNumOfQty)
    {
        LOG(0, "Qty Index Error %d >= %d!\n", u2QtyIndex, aFlashPqFileHead[bIndex].wNumOfQty);
        return (SV_FALSE);
    }

    // Check SmartPic ID
    bSmartPicIndex = DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[bIndex].dwSwQtySmartPicEnable) ? vApiFlashPqGetSmartPicId() : 0;
    
    if (bSmartPicIndex >= aFlashPqFileHead[bIndex].bNumOfSmartPic)
    {
        LOG(0, "SmartPic Index Error!\n");
        return (SV_FALSE);
    }

    // Read Qty header only
    // Calculate start address of Qty table header.
    u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_HEADER, u2QtyIndex, 0, 0, 0);
    
    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
        LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
        return SV_FALSE;
    }
    
    if ((u1QtyBufPtr = (UINT8*)x_mem_alloc(TBL_HEAD_SIZE)) == NULL)
    {
        LOG(0, "FlashPQ Qty memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)u1QtyBufPtr, (UINT32)TBL_HEAD_SIZE);

    x_memcpy(&aFlashPqQtyHead[bIndex], u1QtyBufPtr, TBL_HEAD_SIZE);
    x_mem_free(u1QtyBufPtr);

    LOG(1, "\nQTY NO      %d\n", u2QtyIndex);
    LOG(1, "DATE        %s\n", aFlashPqQtyHead[bIndex].bDate);
    LOG(1, "PIC ID      %d\n", bSmartPicIndex);
    LOG(1, "SCRIPT      %s\n", aFlashPqQtyHead[bIndex].bScript);

    // Read Qty table content
    u2QtyMdlSize = aFlashPqFileHead[bIndex].wSizeOfQtyTable * aFlashPqFileHead[bIndex].bNumOfTiming;

    // Calculate start address of Qty table content.
    u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_DESCRIPT, u2QtyIndex, bSmartPicIndex, 0, 0);
    
    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
        LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
        return SV_FALSE;
    }
    
    if ((u1QtyBufPtr = (UINT8*)x_mem_alloc(u2QtyMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ Qty memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)u1QtyBufPtr, (UINT32)u2QtyMdlSize);

    // Check Qty ID
    if (x_strcmp((CHAR *) (aFlashPqQtyHead[bIndex].bID), (CHAR *) QTY_ID) != 0)
    {
        LOG(0, "QTY ID NG\n");
        x_mem_free(u1QtyBufPtr);
        return (SV_FALSE);
    }

    // Check the hash info inside the Qty header.
    if (aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_HASH)
    {
        if (!bDrvCheckFlashPqQtyHash(bIndex))
        {
            LOG(0, "QTY hash error!\n");
            x_mem_free(u1QtyBufPtr);
            return (SV_FALSE);
        }
    }

    // Check QTY table size.
    if (aFlashPqFileHead[bIndex].wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
    {
        LOG(0, "FlashPQ qty table size error!!!(%d!=%d)\n",
            aFlashPqFileHead[bIndex].wSizeOfQtyTable, CUSTOM_QTY_TBL_DIM);
            x_mem_free(u1QtyBufPtr);
        return (SV_FALSE);
    }

    u1QtyPtr = u1QtyBufPtr;

    for (i = 0; i < aFlashPqFileHead[bIndex].bNumOfTiming; i ++)
    {           
        x_memcpy(&(bCustomQtyTbl[bIndex][i]), u1QtyPtr + bQtyScriptSize, 
                aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);

        u1QtyPtr += aFlashPqFileHead[bIndex].wSizeOfQtyTable;
    }

    #if ENABLE_FLASH_PQ_DEBUG
    if (FLASH_PQ_LOG(E_FLASH_PQ_QTY_LOG))
    {
        vDrvDebugQtyTable(bIndex);
    }
    #endif
    
    x_mem_free(u1QtyBufPtr);
    LOG(0, "FlashPQ Qty OK\n");
    return (SV_TRUE);
}

UINT8 bApiFlashPqUpdateQtyMinMax(UINT8 u1VidPath, UINT8 bIndex)
{
    UINT64 u8QtyAddr;
    UINT16 i;
    UINT8 u1SrcTypTmg;
    UINT8 bQtyScriptSize = SCRIPT_SIZE(bIndex);
    UINT16 u2QtyIndex = vApiFlashPqGetQtyId(bIndex);

    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }

    if (_fgAutoSearch == TRUE)
    {
        LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
        return SV_FALSE;
    }

    if (bFlashPQInit[bIndex] != SV_TRUE)
    {
        LOG(0, "Update MinMax Check init failed\n");
        return (SV_FALSE);
    }

    // Check QtyTable Enable
    if (((aFlashPqFileHead[bIndex].bRes & RES_SUPPORT_QTY_MAX_MIN) == 0) ||
        (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[bIndex].dwSwQtyEnable) == 0) ||
        (DRVCUST_OptGet((QUERY_TYPE_T)aFlashPqSWInfo[bIndex].dwSwQtyMinMaxEnable) == 0))
    {
        LOG(1, "FlashPQ not support Qty Min/Max\n");
        return (SV_FALSE);
    }

    // Check QTY table size.
    if (aFlashPqFileHead[bIndex].wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
    {
        LOG(0, "FlashPQ qty table size error!!!(%d!=%d)\n",
            aFlashPqFileHead[bIndex].wSizeOfQtyTable, CUSTOM_QTY_TBL_DIM);
        return (SV_FALSE);
    }

    for (i=0; i< CUSTOM_QTY_TBL_DIM; i++)
    {
        bQtyMin[bIndex][i] = 0;
        bQtyMax[bIndex][i] = 0;
    }

    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);

    // Transfrom the QtyIndex.
    // Find a match SourceTypeTiming in CustomQtyIdx[].
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);

    if (u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
    {
        return (SV_FALSE);
    }

    // Calculate start address of Qty table.
    u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic, u1SrcTypTmg, 0);

    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
		LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
		return (SV_FALSE);
    }

    vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMin[bIndex],
        (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);
    
    // Calculate start address of Qty table.
    u8QtyAddr = u8CalcFlashPqOffset(bIndex, TYPE_QTY_CONTENT, u2QtyIndex, aFlashPqFileHead[bIndex].bNumOfSmartPic+1, u1SrcTypTmg, 0);

    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
		LOG(0, "FlashPQ qty table address error!!!(%d)\n", u8QtyAddr);
		return (SV_FALSE);
    }
    
    vApiFlashPqReadFlash(bIndex, u8QtyAddr, (UINT32)&bQtyMax[bIndex],
        (UINT32)aFlashPqFileHead[bIndex].wSizeOfQtyTable - bQtyScriptSize);

    for (i=0; i < QUALITY_MAX; i++)
    {
        if ((READ_DEFUALT_QTY_TBL_MIN(i) == 0xFF) &&
            (READ_DEFUALT_QTY_TBL_MAX(i) == 0xFF) &&
            (READ_DEFUALT_QTY_TBL_DFT(i) == 0xFF))
        {
            break;
        }

        if (((READ_DEFUALT_QTY_TBL_REF(i) != FROM_DFT) &&
            ((READ_DEFUALT_QTY_TBL_REF(i) & (0x7FFF)) < CUSTOM_QTY_TBL_DIM)) &&
            ((UINT8)(READ_DEFUALT_QTY_TBL_REF(i)>>16) == bIndex))
        {
            WRITE_DEFUALT_QTY_TBL_MIN(i, bQtyMin[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x7FFF)]);
            WRITE_DEFUALT_QTY_TBL_MAX(i, bQtyMax[bIndex][(READ_DEFUALT_QTY_TBL_REF(i) & 0x7FFF)]);
        }
    }

    #if ENABLE_FLASH_PQ_DEBUG
    if (FLASH_PQ_LOG(E_FLASH_PQ_QTY_MIN_MAX_LOG))
    {
        vDrvDebugQtyMinMax(bIndex);
    }
    #endif

    LOG(1, "FlashPQ Qty Min/Max OK\n");
    return (SV_TRUE);
}

UINT8 u1DrvCheckODFlashPQSel(UINT16 u2ODIndex)
{
    UINT8 i;
    
    for (i=0; i < FLASHPQ_NUM; i++)
    {
        if (bFlashPQInit[i] != SV_TRUE)
        {
            LOG(0, "Update OD Check init failed  ----  %d\n", i);
        }
        else if (bPqExtesion[i] == 0)
        {
            LOG(0, "FlashPQ[%d] not support OD\n", i);
        }
        else if (u2ODIndex >= aFlashPqFileHead[i].wNumOfOd)
        {
            LOG(0, "FlashPQ[%d] OD Index Error %d >= %d!\n", i, u2ODIndex, aFlashPqFileHead[i].wNumOfOd);
        }
        else
        {
            return i;
        }
    }
    
    return 0xFF;    
}


UINT8 bApiFlashPqUpdateOD(void)
{
    UINT8* u1OdBufPtr;
    UINT64 u8OdAddr;
    UINT16 u2OdMdlSize;
    UINT16 u2OdFormat = aFlashPqOdHead.bConfig[0];
    UINT16 u2OdIndex = vApiFlashPqGetOdId();
    UINT16 u2OdTblSize = 0;
    UINT8 u1Index = 0;
    
    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }

    u1Index = u1DrvCheckODFlashPQSel(u2OdIndex);    

    if (u1Index == 0xFF)
    {
        return SV_FALSE;
    }
    
    u8OdAddr = u8CalcFlashPqOffset(u1Index, TYPE_OD_HEADER, u2OdIndex, 0, 0, 0);

    if ((u8OdAddr % 4) != 0)
    {
        LOG(0, "FlashPQ OD address error!!!(%d)\n", u8OdAddr);
        return (SV_FALSE);
    }
    
    u2OdMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfOdTable;
    if ((u1OdBufPtr = (UINT8*)x_mem_alloc(u2OdMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ OD memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u1Index, u8OdAddr, (UINT32)u1OdBufPtr, (UINT32)u2OdMdlSize);

    x_memcpy(&aFlashPqOdHead, u1OdBufPtr, TBL_HEAD_SIZE);

    LOG(1, "\nOD ID       %s\n", aFlashPqOdHead.bID);
    LOG(1, "OD NO           %d\n", u2OdIndex);
    LOG(1, "DATE            %s\n", aFlashPqOdHead.bDate);
    LOG(1, "SCRIPT          %s\n", aFlashPqOdHead.bScript);
    LOG(1, "Format          %d\n", aFlashPqOdHead.bConfig[0]>4);
    LOG(1, "Support OD Enable      %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_ENABLE) ? 1 : 0);
    LOG(1, "Support FB      %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_FB) ? 1 : 0);
    LOG(1, "Support PCID    %d\n", (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_PCID) ? 1 : 0);
    
    if (x_memcmp((void *) (aFlashPqOdHead.bID), (void *) OD_ID, TBL_HEAD_ID_SIZE) != 0)
    {
        LOG(0, "OD ID [%d] Error  %s\n", u2OdIndex, aFlashPqOdHead.bID);
        x_mem_free(u1OdBufPtr);
        return (SV_FALSE);
    }

    _ODParam.u1ODTblSizeMode = aFlashPqOdHead.bConfig[0] >>4;
    u2OdFormat = aFlashPqFileHead[u1Index].wConfigOfOd>>4;
    u2OdTblSize = wDrvGetODTableSize(u2OdFormat);
    
    x_memcpy(OD_Table, (u1OdBufPtr + TBL_HEAD_SIZE), u2OdTblSize);

    _ODParam.u1FB_ON =0;
    _ODParam.u1PCID_ON =0;
    _ODParam.u1OD_ON =0;
    
    if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_FB)
    {
        _ODParam.u1FB_ON =1;
        x_memcpy(FB_Table, (u1OdBufPtr + TBL_HEAD_SIZE + u2OdTblSize + 15), FB_TBL_SIZE);
    }
    
    if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_PCID)
    {
        _ODParam.u1PCID_ON =1;
        x_memcpy(PCID_Table, (u1OdBufPtr + TBL_HEAD_SIZE + u2OdTblSize + 15 + PCID_TBL_SIZE + 15), 
            PCID_TBL_SIZE);
    }
    
    if (aFlashPqOdHead.bConfig[0] & E_OD_SUPPORT_ENABLE)
    {
        _ODParam.u1OD_ON =1;
    }
    
    #if ENABLE_FLASH_PQ_DEBUG
    if (FLASH_PQ_LOG(E_FLASH_PQ_OD_LOG))
    {
        vDrvDebugOD(u2OdTblSize);
    }
    #endif
    
    x_mem_free(u1OdBufPtr);    
    LOG(1, "FlashPQ OD OK\n");
    return (SV_TRUE);
}

UINT8 u1DrvCheckRawDataFlashPQSel(UINT16 u2TblIndex)
{
    UINT8 i;
    
    for (i=0; i< FLASHPQ_NUM; i++)
    {
        if (bFlashPQInit[i] != SV_TRUE)
        {
            LOG(0, "Update Register Data table Check init failed  ----  %d\n", i);
        }    
        else if (u2TblIndex >= aFlashPqFileHead[i].wNumOfRawDataTable)
        {
            LOG(0, "FlashPQ[%d] Register Data table Index Error! %d >= %d\n", 
                i, u2TblIndex, aFlashPqFileHead[i].wNumOfRawDataTable);
        }
        else
        {
            return i;
        }
    }

    return 0xFF;    
}

UINT8 bApiFlashPqSetRawData(UINT16 u2RegNum, UINT32 *pRegTbl, UINT8* pDataTbl)
{
    UINT16 i, j;
    UINT32 u4Temp, u4Addr, u4Value, u4Mask, u4Pos = 0;
    UINT8 u1Type, u1Upper, u1Lower, u1Check, u1Byte;    
    
    for(i = 0; i < u2RegNum; i++)    
    {
        u4Value = 0;
        u4Temp = pRegTbl[i];
                
        u1Type = u4Temp >> 30;
        u1Upper = (u4Temp & 0x3e000000) >> 25;
        u1Lower = (u4Temp & 0x1f00000) >> 20;
        u1Check = u4Temp & 0x3;        
        u1Byte = (u1Upper - u1Lower + 1 + 7)/8;
        u1Byte = (u1Byte == 3) ? 4 : u1Byte;
		u4Mask = (0xffffffff >> (32 - u1Upper - 1)) & (0xffffffff << u1Lower);

        for(j = 0; j < u1Byte; j++)
        {
            //u4Value |= pDataTbl[u4Pos++] << (j * 8);	
			u4Value |= ((UINT32)pDataTbl[u4Pos++]) << (j * 8 + u1Lower); //add lower byte 
        }

        switch(u1Type)
        {
            case SV_REG_DIRECT:
                u4Addr = (u4Temp & 0xffffc) | 0xf0000000;
                vIO32Write4BMsk(u4Addr, u4Value, u4Mask);
                break;
            case SV_REG_XDATA:
                u4Addr = u4Temp & 0xffffc;
				vIO32Write4BMsk(u4SWReg(u4Addr), u4Value, u4Mask);
                break;
            case SV_REG_USER_DATA:
            default:
                break;
        }
    }

    return (SV_TRUE);
}

UINT8 bApiFlashPqGetRawDataInfo(RAWDATA_INFO* pstInfo)
{   
    UINT16 u2TblIndex = u2ApiFlashPqGetRawDataTblId();
    UINT8 u1Index = u1DrvCheckRawDataFlashPQSel(u2TblIndex); 
    
    if(0xFF == u1Index)
    {
        return (SV_FALSE);
    }

    if ((u2TblIndex < aFlashPqFileHead[u1Index].wNumOfRawDataTable) && (0xFF != u1Index))
    {
        pstInfo->u2RegNum = pRawDataSubHead[u2TblIndex].u2RegNum;    
        pstInfo->u2DataSize = pRawDataSubHead[u2TblIndex].u2DataSize;
        pstInfo->pRegTbl = pRawRegTbl;
        pstInfo->pDataTbl = pRawDataTbl;
        
        LOG(2, "Reg Num             %d\n", pstInfo->u2RegNum);
        LOG(2, "Data Size           %d\n", pstInfo->u2DataSize); 
        LOG(2, "Reg  Addr           0x%08x\n", pRawRegTbl);    
        LOG(2, "Data Addr           0x%08x\n", pRawDataTbl);    

        return (SV_TRUE);
    }
    else
    {
        return (SV_FALSE);
    }
}

UINT8 bApiFlashPqRawDataInit(void)
{
    UINT64 u8Addr;
    UINT8 u1Index, bID[TBL_HEAD_ID_SIZE+1];
    UINT32 i, u4RegSize = 0, u4DataSize = 0;    
	UINT32 u4SubHeadSize;

    if (DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }
   
    u1Index = u1DrvCheckRawDataFlashPQSel(0);    
    if(0xFF == u1Index)
    {
        return (SV_FALSE);
    }

    if ((aFlashPqFileHead[u1Index].wNumOfRawDataTable == 0)
        || (aFlashPqFileHead[u1Index].wRawDataTableSize == 0))
    {
        return (SV_TRUE);
    }

    u8Addr = u8CalcFlashPqOffset(u1Index, TYPE_RAWDATA_HEADER, 0, 0, 0, 0);
    
    if((u8Addr % 4) != 0)
    {
        LOG(0, "FlashPQ RAWDATA_HEADER address error!!!(%d)\n", u8Addr);
        return (SV_FALSE);
    }    
    vApiFlashPqReadFlash(u1Index, u8Addr, (UINT32)&aRawDataHead, sizeof(RAWDATA_HEADER));    
    x_memcpy(bID, aRawDataHead.bID, TBL_HEAD_ID_SIZE);
    
    #if ENABLE_FLASH_PQ_DEBUG        
    LOG(0, "RawData ID          %s\n", bID);
    LOG(0, "RawData Version     %s\n", aRawDataHead.bDataVersion);    
    #endif
    
    if (x_memcmp((void *)aRawDataHead.bID, (void *)RAW_ID, TBL_HEAD_ID_SIZE) != 0)
    {
        LOG(0, "RAW_ID  Error  %s\n", bID);
        return (SV_FALSE);
    }

    // read sub header 
    u4SubHeadSize = sizeof(RAWDATA_SUBHEADER) * aFlashPqFileHead[u1Index].wNumOfRawDataTable;
    pRawDataSubHead = (RAWDATA_SUBHEADER*)x_mem_alloc(u4SubHeadSize); 

    if (pRawDataSubHead == NULL)
    {
        LOG(0, "FlashPQ RawData Sub Header memory request fail\n");
        return SV_FALSE;
    }
    
    x_memset((void*)pRawDataSubHead, 0, u4SubHeadSize);  
    vApiFlashPqReadFlash(u1Index, u8Addr+sizeof(RAWDATA_HEADER), (UINT32)pRawDataSubHead, u4SubHeadSize);

    for (i=0; i<aFlashPqFileHead[u1Index].wNumOfRawDataTable; i++)
    {
        u4RegSize = (pRawDataSubHead[i].u2RegNum > u4RegSize) ? pRawDataSubHead[i].u2RegNum : u4RegSize;    
        u4DataSize = (pRawDataSubHead[i].u2DataSize > u4DataSize) ? pRawDataSubHead[i].u2DataSize : u4DataSize;  
        
        #if ENABLE_FLASH_PQ_DEBUG        
        vDrvDebugRawDataSubHeader(i);
        #endif
    }

    u4RegSize *= sizeof(UINT32);
    
    #if ENABLE_FLASH_PQ_DEBUG        
    LOG(0, "RawData Max Reg  Size       %x\n", u4RegSize);
    LOG(0, "RawData Max Data Size       %x\n", u4DataSize);
    #endif
    
    // Allocat Reg table and Data table memory first     
    if((pRawRegTbl = (UINT32*)x_mem_alloc(u4RegSize)) == NULL)
    {
        LOG(0, "Register table memory request fail\n");
        return (SV_FALSE);
    }    

    if((pRawDataTbl = (UINT8*)x_mem_alloc(u4DataSize)) == NULL)
    {
        LOG(0, "Data table memory request fail\n");
        return (SV_FALSE);
    }
    
    x_memset((void*)pRawRegTbl, 0, u4RegSize);        
    x_memset((void*)pRawDataTbl, 0, u4DataSize);  

	u1RawDataInit = SV_TRUE;
   
    LOG(0, "FlashPQ RawData Init OK\n");  
    return (SV_TRUE);
}

UINT8 bApiFlashPqUpdateRawData(void)
{
    UINT8 u1Index, u1Script[TIMING_SCRIPT_SIZE+1];
    UINT64 u8Addr;
    UINT32 u4Size;    
    UINT16 u2TblIndex = u2ApiFlashPqGetRawDataTblId();
    UINT16 u2CaseIndex = u2ApiFlashPqGetRawDataCaseId();

    if(DRVCUST_OptGet(eFlagFlashPqEnable) == 0)
    {
        return (SV_FALSE);
    }

	if(!u1RawDataInit)
	{
		LOG(0, "Raw Data have NOT init!\n");
		return (SV_FALSE);
	}
    
    u1Index = u1DrvCheckRawDataFlashPQSel(u2TblIndex);    
    if(0xFF == u1Index)
    {
        return (SV_FALSE);
    }

    // Update register table  
    u8Addr = u8CalcRawDataTblOffset(u1Index, RAWDATA_REG_TBL_CONTENT, u2TblIndex, 0);    
    u4Size = pRawDataSubHead[u2TblIndex].u2RegNum * sizeof(UINT32);       
    
    x_memset((void*)pRawRegTbl, 0, u4Size);
    vApiFlashPqReadFlash(u1Index, u8Addr, (UINT32)pRawRegTbl, u4Size);

    // Update data table Script   
    u8Addr = u8CalcRawDataTblOffset(u1Index, RAWDATA_DATA_TBL_SCRIPT, u2TblIndex, u2CaseIndex);        
    x_memset((void*)u1Script, 0, TIMING_SCRIPT_SIZE+1);
    
    vApiFlashPqReadFlash(u1Index, u8Addr, (UINT32)u1Script, TIMING_SCRIPT_SIZE);   
    
    LOG(1, "Table           %d\n", u2TblIndex);
    LOG(1, "Case            %d\n", u2CaseIndex);
    LOG(1, "Script          %s\n", u1Script);    
    LOG(1, "Case Num        %d\n", pRawDataSubHead[u2TblIndex].u2CaseNum);    
    LOG(1, "Reg  Num        %d\n", pRawDataSubHead[u2TblIndex].u2RegNum);    
    LOG(1, "Data Size       %d\n", pRawDataSubHead[u2TblIndex].u2DataSize);    
    
    // Update data table    
    u8Addr = u8CalcRawDataTblOffset(u1Index, RAWDATA_DATA_TBL_CONTENT, u2TblIndex, u2CaseIndex);    
    u4Size = pRawDataSubHead[u2TblIndex].u2DataSize;
            
    x_memset((void*)pRawDataTbl, 0, u4Size);    
    vApiFlashPqReadFlash(u1Index, u8Addr, (UINT32)pRawDataTbl, u4Size);
    
    LOG(1, "FlashPQ Update Raw data OK\n");
    
    #if ENABLE_FLASH_PQ_DEBUG        
    if (FLASH_PQ_LOG(E_FLASH_PQ_REG_LOG))
    {
        vDrvDebugRawData();
    }
    #endif

    return (SV_TRUE);
}
#endif

UINT8 u1DrvCheckPanelFlashPQSel(UINT16 u2PanleIndex)
{
    UINT8 i;
    
    for (i=0; i < FLASHPQ_NUM; i++)
    {
        if (bFlashPQInit[i] != SV_TRUE)
        {
            LOG(0, "Update FlashPanel Check init failed  ----  %d\n", i);
        }
        else if (u2PanleIndex >= aFlashPqFileHead[i].wNumOfPanelTable)
        {
            LOG(0, "FlashPQ[%d] FlashPanel Index Error %d >= %d!\n", 
                i, u2PanleIndex, aFlashPqFileHead[i].wNumOfPanelTable);
        }
        else
        {
            return i;
        }
    }
    
    return 0xFF;    
}

UINT8 bApiFlashPqUpdatePanelTable(void)
{
    UINT8* u1PanelBufPtr;
    UINT64 u8PanelAddr;
    UINT16 u2PanelMdlSize;
    UINT16 u2PanelIndex = wApiFlashPqGetPanelId();
    UINT8 u1Index = 0;
    
#ifndef CC_MTK_LOADER 
    if (DRVCUST_OptGet(eFlagFlashPanelEnable) == 0)
#else
    if (DRVCUST_InitGet(eFlagFlashPanelEnable) == 0)
#endif        
	{
		LOG(0, "Not support FlashPanel\n");
		return (SV_FALSE);
	}

    if (u2PanelIndex == PANEL_TOTAL_NUMBER)
    {
        return SV_FALSE;
    }

    u1Index = u1DrvCheckPanelFlashPQSel(u2PanelIndex);    

    if (u1Index == 0xFF)
    {
        return SV_FALSE;
    }
    
    u8PanelAddr = u8CalcFlashPqOffset(u1Index, TYPE_PANEL_HEADER, u2PanelIndex, 0, 0, 0);

    if ((u8PanelAddr % 4) != 0)
    {
        LOG(0, "FlashPQ Panel address error!!!(%d)\n", u8PanelAddr);
        return (SV_FALSE);
    }
    
    u2PanelMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead[u1Index].wSizeOfPanelTable;
    if ((u1PanelBufPtr = (UINT8*)x_mem_alloc(u2PanelMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ Panel memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u1Index, u8PanelAddr, (UINT32)u1PanelBufPtr, (UINT32)u2PanelMdlSize);

    x_memcpy(&aFlashPqPanelHead, u1PanelBufPtr, TBL_HEAD_SIZE);

    LOG(1, "\nPanel ID       %s\n", aFlashPqPanelHead.bID);
    LOG(1, "Panel NO           %d\n", u2PanelIndex);
    LOG(1, "DATE            %s\n", aFlashPqPanelHead.bDate);
    LOG(1, "SCRIPT          %s\n", aFlashPqPanelHead.bScript);

    if (x_memcmp((void *) (aFlashPqPanelHead.bID), (void *) PANEL_ID, TBL_HEAD_ID_SIZE) != 0)
    {
        LOG(0, "Panel ID [%d] Error  %s\n", u2PanelIndex, aFlashPqPanelHead.bID);
        x_mem_free(u1PanelBufPtr);
        return (SV_FALSE);
    }
    
    if (*((UINT32 *)(aFlashPqPanelHead.bHash)) != FLASH_PANEL_HASH)
    {
        LOG(0, "Panel HASH = %x Error, should be %x\n", *((UINT32 *)(aFlashPqPanelHead.bHash)), FLASH_PANEL_HASH);
        return (SV_FALSE);
    }
    
    x_memcpy(&rFlashPanelTable, (u1PanelBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead[u1Index].wSizeOfPanelTable);

    #if ENABLE_FLASH_PQ_DEBUG
    //if (FLASH_PQ_LOG(E_FLASH_PQ_PANEL_LOG))
    {
        vDrvDebugPanel(aFlashPqFileHead[u1Index].wSizeOfPanelTable);
    }
    #endif
    
    x_mem_free(u1PanelBufPtr);    
    LOG(0, "FlashPQ Panel OK\n");
    return (SV_TRUE);
}

