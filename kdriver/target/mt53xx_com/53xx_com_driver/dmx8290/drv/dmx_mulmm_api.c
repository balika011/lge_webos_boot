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
 * $Author: p4admin $
 * $Date: 2015/03/09 $
 * $RCSfile: dmx_multimm_api.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_multimm_api.c
 *  Demux multi-media driver - multiple instance
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static DMX_MUL_INSTINFO_T _arDmxMulInstInfo[DMX_MULTIPLE_INSTANCE_MAXNUM];
static DMX_MUL_PIDINFO_T _arDmxMulPidInfo[DMX_NUM_PID_INDEX];
static UINT8 _arDmxMulInstSbt[DMX_MULTIPLE_INSTANCE_MAXNUM];
static DMX_MUL_TSIDXINFO_T _arDmxTsIdxInfo[DMX_FRAMER_COUNT];

static BOOL _fgEnableMulInst = TRUE;

#ifndef CC_MTK_LOADER
static HANDLE_T _hPicSem = NULL_HANDLE;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _DMX_MUL_UpdatePVRUse(UINT8 u1TsIdx, DMX_INPUT_TYPE_T eInputType)
{
	BOOL fgPvrUse;
	
	if (u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return FALSE;
    }
	
	if(u1TsIdx == 4)
	{
		fgPvrUse = TRUE;
	}
	else if(eInputType == DMX_IN_PLAYBACK_MM)
	{
		fgPvrUse = FALSE;
	}
	else
	{
		fgPvrUse = FALSE;
	}

	_DDI_UsePVR_Update(u1TsIdx, fgPvrUse);

	return TRUE;

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

UINT8 _DMX_MUL_AllocTsIdx(UINT8 u1Inst, DMX_CONN_TYPE_T eConnType)
{
	INT32 i;
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;

	if(eConnType < DMX_CONN_TYPE_TUNER_MAX)
	{
        UINT8 u1TunerID = 0;

        //turn 0 -- TsIdx 0 , turn 1 -- TsIdex 1 , turn 2 -- TsIdex 3
        UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));

        for (i = 0; i < DMX_FRAMER_COUNT; i++)
        {
            if (prDmxFeCfgTable[i].fgEnable)
            {
                if(u1TunerID == (UINT8)eConnType)
                {
                    break;
                }
                else
                {
                    u1TunerID++;
                }
            }
			else
			{
				LOG(0, "prDmxFeCfgTable[i].fgEnable false.\n");
			}
        }

		if (i >= DMX_FRAMER_COUNT)
		{
			return 0xff;
		}
	
        _arDmxTsIdxInfo[i].fgUsed = TRUE;

		return i;
	}
	else if (eConnType < DMX_CONN_TYPE_BUFAGENT_MAX)
	{
		for (i = DMX_FRAMER_COUNT - 1; i >= 0; i--)
		{
			#if defined(CC_MT5882)
			if (i == 2)
			{
				//Capri don't have framer2.
				continue;
			}
			#endif
        
            UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));
            if (prDmxFeCfgTable[i].fgEnable)
            {
                continue;
            }
			
			if(_arDmxTsIdxInfo[i].fgUsed == FALSE)
			{
				break;
			}
		}

		if (i < 0)
		{
			return 0xff;
		}
		
		_arDmxTsIdxInfo[i].fgUsed = TRUE;
		
		return i;
	}
	
    return 0xff;
}

BOOL _DMX_MUL_FreeTsIdx(UINT8 u1Inst)
{
	UINT8 u1TsIdx = 0;

	u1TsIdx = _arDmxMulInstInfo[u1Inst].u1TsIdx;
	
	LOG(0,"_DMX_MUL_FreeTsIdx: u1Inst %d, u1TsIdx %d\n", u1Inst, u1TsIdx);

	if (u1TsIdx >= DMX_FRAMER_COUNT)
	{
		LOG(0, "_DMX_MUL_FreeTsIdx u1TsIdx = %d !!!!\n", u1TsIdx);
		return FALSE;
	}
		
	_arDmxTsIdxInfo[u1TsIdx].fgUsed = FALSE;
	_arDmxTsIdxInfo[u1TsIdx].eInputType = DMX_IN_NONE;
	_DDI_ClearAlignInfo(u1TsIdx);
	
	return TRUE;
}

void _DMX_MUL_LockPicSem(UINT8 u1Inst, DMX_MM_DATA_T *prData)
{
#ifndef CC_MTK_LOADER
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        VERIFY(x_sema_create(&_hPicSem, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
        // async callback can work well, do nothing
        break;

    case DMX_IN_PLAYBACK_PS:
    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        VERIFY(x_sema_lock(_hPicSem, X_SEMA_OPTION_WAIT) == OSR_OK);
        break;

    default:
        break;
    }
#endif
}


void _DMX_MUL_UnlockPicSem(UINT8 u1Inst, DMX_MM_DATA_T *prData)
{
#ifndef CC_MTK_LOADER
    UINT32 u4PicCount;

    switch(_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:

        // async callback can work well, do nothing
        break;

    case DMX_IN_PLAYBACK_PS:
    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        _DMX_Lock();
        u4PicCount = _arDmxMulInstInfo[u1Inst].u4PicCount;
        _DMX_Unlock();
        if(u4PicCount == 0)
        {
            VERIFY(x_sema_unlock(_hPicSem) == OSR_OK);
        }
        else
        {
            // try to lock
            VERIFY(x_sema_lock(_hPicSem, X_SEMA_OPTION_WAIT) == OSR_OK);
            // unlock it if we get a lock
            VERIFY(x_sema_unlock(_hPicSem) == OSR_OK);
        }
        break;

    default:
        break;
    }
#endif
}


void _DMX_MUL_UnlockPicSemWhenFinishedCallback(void)
{
#ifndef CC_MTK_LOADER
    VERIFY(x_sema_unlock(_hPicSem) == OSR_OK);
#endif
}

//static void _DmxModifyPicCount(UINT8 u1Pidx, BOOL fbIncrement)
void _DMX_MUL_ModifyPicCount(UINT8 u1Pidx, BOOL fbIncrement)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1Inst;

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if ((prPidStruct->eInputType == DMX_IN_PLAYBACK_PS) ||
        (prPidStruct->eInputType == DMX_IN_PLAYBACK_MM) ||
        (prPidStruct->eInputType == DMX_IN_PLAYBACK_ES))
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
        if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
        {
            return;
        }

        if(fbIncrement)
        {
            _arDmxMulInstInfo[u1Inst].u4PicCount++;
        }
        else
        {
            _arDmxMulInstInfo[u1Inst].u4PicCount--;
        }

    }
    _DMX_Unlock();
}


UINT32 _DMX_MUL_GetPicCount(UINT8 u1Pidx)
{
    PID_STRUCT_T* prPidStruct;
    UINT8 u1Inst;
    UINT32 u4PicCount = 0;

    _DMX_Lock();
    prPidStruct = _DMX_GetPidStruct(u1Pidx);
    if ((prPidStruct->eInputType == DMX_IN_PLAYBACK_PS) ||
        (prPidStruct->eInputType == DMX_IN_PLAYBACK_MM) ||
        (prPidStruct->eInputType == DMX_IN_PLAYBACK_ES))
    {
        u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
        if(u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
        {
            return 0;
        }
        u4PicCount = _arDmxMulInstInfo[u1Inst].u4PicCount;
    }
    _DMX_Unlock();

    return u4PicCount;
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** _DMX_MUL_Init
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_Init(void)
{
    UINT8 i;

    x_memset((void*)&_arDmxMulInstInfo, 0, sizeof(_arDmxMulInstInfo));

    for (i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        _arDmxMulPidInfo[i].fgUsed = FALSE;
        _arDmxMulPidInfo[i].u1InstId = DMX_NULL_INSTINDEX;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_EnableMultipleInst
 */
//-----------------------------------------------------------------------------
void _DMX_MUL_EnableMultipleInst(BOOL fgEnable)
{
    // always in multiple intance mode
    //_fgEnableMulInst = fgEnable;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetAvailableInst
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_T eConnType)
{
    UINT8 i;

    if (eConnType != DMX_CONN_TYPE_NONE)
    {
        for (i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
        {
            if ((_arDmxMulInstInfo[i].fgUsed) && (_arDmxMulInstInfo[i].eConnType == eConnType))
            {
                break;
            }
        }

        if (i < DMX_MULTIPLE_INSTANCE_MAXNUM)
        {
            // allocated instance
            return i;
        }
    }

    // try to find a available instance
    for (i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
    {
        if (!_arDmxMulInstInfo[i].fgUsed)
        {
            break;
        }
    }

    if (i >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_MULTIPLE_NULL_INSTANCE;
    }

	_arDmxMulInstInfo[i].u1TsIdx = _DMX_MUL_AllocTsIdx(i, eConnType);

	LOG(0, "_DMX_MUL_AllocTsIdx: inst %d, u1TsIdx %d, eConnType %d\n", i, _arDmxMulInstInfo[i].u1TsIdx, eConnType);
	
	if (_arDmxMulInstInfo[i].u1TsIdx == 0xff)
	{
		return DMX_MULTIPLE_NULL_INSTANCE;
	}

    _arDmxMulInstInfo[i].fgUsed = TRUE;
    _arDmxMulInstInfo[i].eConnType = eConnType;

	//update tsindex when connect type is DMX_CONN_TYPE_BUFAGENT,
	/*if (eConnType > DMX_CONN_TYPE_TUNER_MAX)
	{
		_arDmxMulInstInfo[i].u1TsIdx = DMX_MM_MOVE_TSIDX;
	}*/

    return i;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_FreeInst
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_FreeInst(UINT8 u1Inst)
{
    BOOL u1Ret;
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }
	
	_DMX_MUL_FreeTsIdx(u1Inst);
    _arDmxMulInstInfo[u1Inst].fgUsed = FALSE;
    _arDmxMulInstInfo[u1Inst].eTSFmt = DMX_TSFMT_NONE;
    _arDmxMulInstInfo[u1Inst].u1TsIdx = 0;
	
	if (_arDmxMulInstInfo[u1Inst].eInputType == DMX_IN_PLAYBACK_PS)
    {
        u1Ret = _DMX_PS_Free_PSID(u1Inst);

        if (u1Ret == FALSE)
        {
            return FALSE;
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetPidxInstId
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetPidxInstId(UINT8 u1Pidx)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return DMX_NULL_INSTINDEX;
    }

    if (!_fgEnableMulInst)
    {
        return DMX_NULL_INSTINDEX;
    }

    if (!_arDmxMulPidInfo[u1Pidx].fgUsed)
    {
        return DMX_NULL_INSTINDEX;
    }

    return _arDmxMulPidInfo[u1Pidx].u1InstId;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetInstInputType
*/
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T _DMX_MUL_GetInstInputType(UINT8 u1Inst)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_IN_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].eInputType;
}



//-----------------------------------------------------------------------------
/** _DMX_MUL_SetInstType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetInstType(UINT8 u1Inst, DMX_INPUT_TYPE_T eInputType)
{
    UINT8 u1Psid;
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].eInputType = eInputType;
	_arDmxTsIdxInfo[_arDmxMulInstInfo[u1Inst].u1TsIdx].eInputType = eInputType;
	_DMX_MUL_UpdatePVRUse(_arDmxMulInstInfo[u1Inst].u1TsIdx, eInputType);

    if (eInputType == DMX_IN_PLAYBACK_PS)
    {
        u1Psid = _DMX_PS_GetAvailable_PSID(u1Inst);

        if (u1Psid >= DMX_MULTIPLE_INSTANCE_MAXNUM)
        {
            return FALSE;
        }
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetAvailablePidx
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetAvailablePidx(UINT8 u1Inst)
{
    UINT8 i;

    //LOG(5, "%s\n", __FUNCTION__);

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (!_arDmxMulPidInfo[i].fgUsed)
        {
            break;
        }
    }

    if (i >= DMX_NUM_PID_INDEX)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    _arDmxMulPidInfo[i].fgUsed = TRUE;
    _arDmxMulPidInfo[i].u1InstId = u1Inst;

    return i;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetSubtitlePidx
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetSubtitlePidx(UINT8 u1Inst, UINT8 u1Pidx)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    if(_arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_MM &&
	   _arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_TS &&
	   _arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_PS)
    {
    	LOG(3, "_arDmxMulInstInfo[%d].eInputType=0x%x\n", u1Inst, _arDmxMulInstInfo[u1Inst].eInputType);
        return FALSE;
    }

    _arDmxMulInstSbt[u1Inst] = u1Pidx;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetSubtitlePidx
*/
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetSubtitlePidx(UINT8 u1Inst)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    if(_arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_MM &&
	   _arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_TS &&
	   _arDmxMulInstInfo[u1Inst].eInputType != DMX_IN_PLAYBACK_PS)
    {
    	LOG(3, "_arDmxMulInstInfo[%d].eInputType=0x%x\n", u1Inst, _arDmxMulInstInfo[u1Inst].eInputType);
        return DMX_MULTIPLE_NULL_PIDX;
    }

    if (_arDmxMulInstSbt[u1Inst] >= DMX_NUM_PID_INDEX)
    {
        return DMX_MULTIPLE_NULL_PIDX;
    }

    return _arDmxMulInstSbt[u1Inst];
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetPidxInstance
 *  Temporary
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetPidxInstance(UINT8 u1Inst, UINT8 u1Pidx)
{
    if ((u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM) ||
            (u1Pidx >= DMX_NUM_PID_INDEX))
    {
        return FALSE;
    }

    _arDmxMulPidInfo[u1Pidx].fgUsed = TRUE;
    _arDmxMulPidInfo[u1Pidx].u1InstId = u1Inst;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_FreePidx
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_FreePidx(UINT8 u1Pidx)
{
    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return FALSE;
    }

    _arDmxMulPidInfo[u1Pidx].fgUsed = FALSE;
    _arDmxMulPidInfo[u1Pidx].u1InstId = DMX_NULL_INSTINDEX;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_MoveData
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_MoveData(UINT8 u1Inst, DMX_MM_DATA_T *prData)
{
    BOOL fgRet = FALSE;
    DMX_VIDEO_TYPE_T eVideoType;

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    if (!_arDmxMulInstInfo[u1Inst].fgUsed)
    {
        LOG(0,">>Instance %d is not used!<<",u1Inst);
        return FALSE;
    }

    eVideoType = _arDmxMulInstInfo[u1Inst].eVideoType;

    switch (_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
        prData->u1Idx = _DMX_MUL_GetTsIdx(u1Inst);

       /*mark it for support KR3D file play,it will set the Video type by Pid index.*/
  /*
        if (!_DMX_SetVideoType(prData->u1Idx, eVideoType))
        {
            return FALSE;
        }
  */    
        if (!_DDI_UsePVR(prData->u1Idx))
	    {
            if (_DMX_DDI_FramerSwitch(prData->u1Idx, DMX_FE_DDI))
 	        {
 	            LOG(1, "_DMX_DDI_FramerSwitch\n");
 	            prData->fgFrameHead = TRUE;
 	        }
        }
#ifdef CC_DMX_TS130
        if ((_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_130) || 
            (_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_134))
        {
            if (_DDI_UsePVR(prData->u1Idx))
	        {
                _DMX_PVRPlay_SetMoveMode(0, _arDmxMulInstInfo[u1Inst].eTSFmt, FALSE);
                if (!_DMX_PVRPlay_SetPortEx(0, prData->u1Idx, 0, DMX_PVRPLAY_PORT_DBM, FALSE, FALSE)) 
                {
                    return FALSE;
                }
            }
            else
            {
                if (prData->u1Idx == 0)
                {
                    _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER0, 0, TRUE, FALSE);
                }
                else if (prData->u1Idx == 1)
                {
                    _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, 0, TRUE, FALSE);
                }
                else if (prData->u1Idx == 2)
                {
                    _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER2, 0, TRUE, FALSE);
                }
                else if (prData->u1Idx == 3)
                {
                    _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER3, 0, TRUE, FALSE);
                }
                else
                {
                    return FALSE;
                }

                if (!_DMX_SetFramerPktSize(prData->u1Idx, 188))
                {
                    return FALSE;
                }

                if (!_DMX_DDI_SetPacketSize(prData->u1Idx, 188))
                {
                    return FALSE;
                }
            }
            
            if(_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_130)
            {
                return _DMX_TS130_MoveData(prData);
            }
            else
            {
                return _DMX_TS134_MoveData(prData);
            }
        }
#endif // CC_DMX_TS130
		if (_DDI_UsePVR(prData->u1Idx))
	    {
	    	UINT8 u1PktSize = 0;
			
            if (!_DMX_PVRPlay_SetPortEx(0, prData->u1Idx, 0, DMX_PVRPLAY_PORT_DBM, TRUE, FALSE)) {
                return FALSE ;
            }
	
			_DMX_PVRPlay_SetMoveMode(0, _arDmxMulInstInfo[u1Inst].eTSFmt, FALSE);

	       // LOG(1, "_DMX_DDI_TSMoveData\n");

		    if ((_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_192) || (_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_192_ENCRYPT) ||
            	(_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_TIMESHIFT))
		    {
		        u1PktSize = 192;
		    }
		    else if(_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_204)
		    {
		       u1PktSize = 204;
		    }
		    else
		    {
		        u1PktSize = 188;
		    }

			if (!_DMX_DDI_SetPacketSize(prData->u1Idx, u1PktSize))
	        {
	            return FALSE;
	        }
			
			fgRet = _DMX_DDI_TSMoveData(prData);
	    }
		else
		{
			if (prData->u1Idx == 0)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER0,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
	        }
	        else if (prData->u1Idx == 1)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
	        }
	        else if (prData->u1Idx == 2)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER2,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
	        }
	        else if (prData->u1Idx == 3)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER3,0, TRUE, TRUE);//avoid the DBM Bypass after Play MM.
	        }
	        else
	        {
	            //Need Add  DMX_DDI_PORT_FRAMER2_BYPASS and DMX_DDI_PORT_FRAMER3_BYPASS
	            LOG(0,"Need Add  DMX_DDI_PORT_FRAMER2_BYPASS and DMX_DDI_PORT_FRAMER3_BYPASS!\n");
	        }

			if (_DMX_SetTSFilePlayPath(prData->u1Idx, _arDmxMulInstInfo[u1Inst].eTSFmt))
	        {
	            fgRet = _DMX_DDI_TSMoveData(prData);
	        }
		}

        break;

    case DMX_IN_PLAYBACK_PS:

        //_arDmxMulInstInfo[u1Inst].u1TsIdx = DMX_MM_MOVE_TSIDX;

        if (!_DMX_SetVideoType(_arDmxMulInstInfo[u1Inst].u1TsIdx, eVideoType))
        {
            return FALSE;
        }
		
		prData->u1Idx = u1Inst;
		
		if(!_DDI_UsePVR(_arDmxMulInstInfo[u1Inst].u1TsIdx))
		{
	        if (!_DMX_SetFramerPktSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }

	        if (!_DMX_DDI_SetPacketSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }
		}
		
        fgRet = _DMX_PS_CPU_MoveData(prData);
        break;

    case DMX_IN_PLAYBACK_MM:

        //_arDmxMulInstInfo[u1Inst].u1TsIdx = DMX_MM_MOVE_TSIDX;

        if (!_DMX_SetVideoType(_arDmxMulInstInfo[u1Inst].u1TsIdx, eVideoType))
        {
            return FALSE;
        }

		if(_DDI_UsePVR(_arDmxMulInstInfo[u1Inst].u1TsIdx))
        {
	        DMX_PVR_PLAY_T rPlay;

            if (!_DMX_PVRPlay_SetPortEx(0, _arDmxMulInstInfo[u1Inst].u1TsIdx, prData->u1Idx, 
                                        DMX_PVRPLAY_PORT_DBM, FALSE, FALSE)) {
                return FALSE ;
            }
			//_DmxPVRPlay_SetFramerMode(0, TRUE, TRUE);
			
	        x_memset((void*)&rPlay, 0x0, sizeof(DMX_PVR_PLAY_T));
	        
	        rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
	        rPlay.fgContainTimeStamp = FALSE;
	        rPlay.fgIgnoreTimeStamp = TRUE;
	        rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
	        _DMX_PVRPlay_Set(0,(UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay);

	        LOG(7, "_DMX_MUL_MoveData PVR\n");
        }
        else
    	{			
	        if (_arDmxMulInstInfo[u1Inst].u1TsIdx == 3)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER3, prData->u1Idx, TRUE, FALSE);
	        }
	        else if (_arDmxMulInstInfo[u1Inst].u1TsIdx == 2)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER2, prData->u1Idx, TRUE, FALSE);
	        }
	        else if (_arDmxMulInstInfo[u1Inst].u1TsIdx == 1)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, prData->u1Idx, TRUE, FALSE);
	        }
	        else if (_arDmxMulInstInfo[u1Inst].u1TsIdx == 0)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER0, prData->u1Idx, TRUE, FALSE);
	        }
	        else
	        {
	            LOG(0,"DMX MM Move Data TS index set Error!\n");
	        }

	        if (!_DMX_SetFramerPktSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }

	        if (!_DMX_DDI_SetPacketSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }
    	}

#ifdef  DMX_SUPPORT_PDDRM
        if(prData->fgPDDRM)
        {
            fgRet = _DMX_PDDRM_MoveData(prData->u1Idx,prData,5000);
			if (prData->fgMoveComplete)
            {
            	UNUSED(_DMX_SendMoveCompleteMessage(prData->u1Idx));
			}
            return fgRet;
        }
#endif

        fgRet = _DMX_MM_MoveData(prData->u1Idx, prData, 5000);
     	if (prData->fgMoveComplete)
        {
        	UNUSED(_DMX_SendMoveCompleteMessage(prData->u1Idx));
		}
        break;

    case DMX_IN_PLAYBACK_ES:
        //_arDmxMulInstInfo[u1Inst].u1TsIdx = DMX_MM_MOVE_TSIDX;

        if (!_DMX_SetVideoType(_arDmxMulInstInfo[u1Inst].u1TsIdx, eVideoType))
        {
            return FALSE;
        }

		if(_DDI_UsePVR(_arDmxMulInstInfo[u1Inst].u1TsIdx))
        {
	        DMX_PVR_PLAY_T rPlay;

            if (!_DMX_PVRPlay_SetPortEx(0, _arDmxMulInstInfo[u1Inst].u1TsIdx, prData->u1Idx, 
                                        DMX_PVRPLAY_PORT_DBM, FALSE, FALSE)) {
                return FALSE ;
            }
			//_DmxPVRPlay_SetFramerMode(0, TRUE, TRUE);
			
	        x_memset((void*)&rPlay, 0x0, sizeof(DMX_PVR_PLAY_T));
	        
	        rPlay.eMode = DMX_PVR_PLAY_SINGLEMOVE;
	        rPlay.fgContainTimeStamp = FALSE;
	        rPlay.fgIgnoreTimeStamp = TRUE;
	        rPlay.u2TimeStampFreqDiv = DMX_PVRPLAY_TIMESTAMP_DIV_BASE;
	        _DMX_PVRPlay_Set(0,(UINT32)(PVRPLAY_FLAGS_MODE | PVRPLAY_FLAGS_TIMESTAMP), &rPlay);

	        LOG(7, "_DMX_MUL_MoveData ES PVR\n");
        }
		else
		{			
	        if (_arDmxMulInstInfo[u1Inst].u1TsIdx == 3)
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER3, prData->u1Idx, TRUE, FALSE);
	        }
	        else
	        {
	            _DMX_DDI_SetPortEx(DMX_DDI_PORT_FRAMER1, prData->u1Idx, TRUE, FALSE);
	        }

	        if (!_DMX_SetFramerPktSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }

	        if (!_DMX_DDI_SetPacketSize(_arDmxMulInstInfo[u1Inst].u1TsIdx, 188))
	        {
	            return FALSE;
	        }
		}
		
        fgRet = _DMX_ES_MoveData(prData->u1Idx, prData);
     	if (prData->fgMoveComplete)
        {
        	UNUSED(_DMX_SendMoveCompleteMessage(prData->u1Idx));
		}
        break;

    default:
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_RequestReset(UINT8 u1Inst)
{
    BOOL fgRet = TRUE;
    UINT8 i;

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    switch (_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
#ifdef CC_DMX_TS130
        if ((_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_130) ||
            (_arDmxMulInstInfo[u1Inst].eTSFmt == DMX_TSFMT_134))
        {
            fgRet = _DMX_TS130_RequestReset(_arDmxMulInstInfo[u1Inst].u1TsIdx);
        }
        else
#endif // CC_DMX_TS130
        {
            fgRet = _DMX_DDI_TSRequestReset(_arDmxMulInstInfo[u1Inst].u1TsIdx);
        }
        break;

    case DMX_IN_PLAYBACK_PS:
        fgRet = _DMX_PS_CPU_Reset(u1Inst);
        break;

    case DMX_IN_PLAYBACK_MM:
        fgRet = _DMX_MM_Reset(u1Inst);
        break;

    case DMX_IN_PLAYBACK_ES:
        for (i=0; i<DMX_NUM_PID_INDEX ; i++)
        {
            if ((_arDmxMulPidInfo[i].u1InstId == u1Inst) && _arDmxMulPidInfo[i].fgUsed)
            {
                fgRet = _DMX_ES_Reset(i);
            }
        }
        break;

    default:
        fgRet = FALSE;
        break;
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetEmptySize
*/
//-----------------------------------------------------------------------------
UINT32 _DMX_MUL_GetEmptySize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    UINT32 u4EmptySize = 0;

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    switch (_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
    case DMX_IN_PLAYBACK_PS:
        u4EmptySize = _DMX_GetMinFreeBufferSize(u1Inst, eType);
        break;

    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        if (u1Pidx >= DMX_NUM_PID_INDEX)
        {
            return 0;
        }
        if (_arDmxMulPidInfo[u1Pidx].fgUsed && (_arDmxMulPidInfo[u1Pidx].u1InstId == u1Inst))
        {
            u4EmptySize = _DMX_MM_GetBufEmptySize(u1Pidx);
        }
        break;

    default:
        break;
    }

    return u4EmptySize;
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSize
 *  Get free buffer: dram buffer
 */
//-----------------------------------------------------------------------------
UINT32 _DMX_MUL_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    UINT32 u4EmptySize = 0;

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return 0;
    }

    switch (_arDmxMulInstInfo[u1Inst].eInputType)
    {
    case DMX_IN_BROADCAST_TS:
    case DMX_IN_PLAYBACK_TS:
    case DMX_IN_PLAYBACK_PS:
        u4EmptySize = _DMX_GetMinFreeDRAMBufferSize(u1Inst, eType);
        break;

    case DMX_IN_PLAYBACK_MM:
    case DMX_IN_PLAYBACK_ES:
        if (u1Pidx >= DMX_NUM_PID_INDEX)
        {
            return 0;
        }
        if (_arDmxMulPidInfo[u1Pidx].fgUsed && (_arDmxMulPidInfo[u1Pidx].u1InstId == u1Inst))
        {
            u4EmptySize = _DMX_MM_GetBufEmptySize(u1Pidx);
        }
        break;

    default:
        break;
    }

    return u4EmptySize;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetDecoderCallbacks
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetDecoderCallbacks(UINT8 u1Inst, const DMX_DECODER_CALLBACKS_T* prCallbacks)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].rDecoderCallback = *prCallbacks;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetInstDecoderCallback
 */
//-----------------------------------------------------------------------------
DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetInstDecoderCallback(UINT8 u1Inst)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return NULL;
    }

    return &(_arDmxMulInstInfo[u1Inst].rDecoderCallback);
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetPidxDecoderCallback
 */
//-----------------------------------------------------------------------------
DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetPidxDecoderCallback(UINT8 u1Pidx)
{
    UINT8 u1Inst;

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        return NULL;
    }

    u1Inst = _arDmxMulPidInfo[u1Pidx].u1InstId;

    if (!_arDmxMulPidInfo[u1Pidx].fgUsed || (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM))
    {
        return NULL;
    }

    return &(_arDmxMulInstInfo[u1Inst].rDecoderCallback);
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetVideoType
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetVideoType(UINT8 u1Inst, DMX_VIDEO_TYPE_T eVideoType)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    _arDmxMulInstInfo[u1Inst].eVideoType = eVideoType;

    if (!_DMX_SetVideoType(_arDmxMulInstInfo[u1Inst].u1TsIdx, eVideoType))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetVideoTypec
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T _DMX_MUL_GetVideoType(UINT8 u1Inst)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_VIDEO_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].eVideoType;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetTSMode
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetTSMode(UINT8 u1Inst, DMX_TSFMT_T eFmt)
{
    //UINT8 i;
#if 0
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    if (eFmt == DMX_TSFMT_TIMESHIFT)
    {
        _arDmxMulInstInfo[u1Inst].u1TsIdx = 2;
    }
    else
    {
        // check if there are other ts occupied - assume there are two TS instance at most
        for (i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
        {
            if ((i != u1Inst) && (_arDmxMulInstInfo[i].fgUsed == TRUE) &&
                    (_arDmxMulInstInfo[i].eTSFmt != DMX_TSFMT_NONE))
            {
                break;
            }
        }

#if defined(CC_DUAL_TUNER_SUPPORT)
		_arDmxMulInstInfo[u1Inst].u1TsIdx = DMX_MM_MOVE_TSIDX;
#else
		 if (i != DMX_MULTIPLE_INSTANCE_MAXNUM)
		 {
			 // assign another ts index
			 _arDmxMulInstInfo[u1Inst].u1TsIdx = (_arDmxMulInstInfo[i].u1TsIdx == 1)?0:1;
		 }
		 else
		 {
			 // there is no occupied, assign 1 as default
			 _arDmxMulInstInfo[u1Inst].u1TsIdx = 1;
		 }
 #endif

    }
	
#endif
    _arDmxMulInstInfo[u1Inst].eTSFmt = eFmt;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetTSMode
 */
//-----------------------------------------------------------------------------
DMX_TSFMT_T _DMX_MUL_GetTSMode(UINT8 u1Inst)
{
#ifndef CC_DMX_EMULATION
    ASSERT(u1Inst < DMX_MULTIPLE_INSTANCE_MAXNUM);
#endif

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_TSFMT_NONE;
    }

    return _arDmxMulInstInfo[u1Inst].eTSFmt;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetTSIdx
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetTSIdx(UINT8 u1Inst)
{
#if 0
    UINT8 i;
    DMX_FRONTEND_CONFIG_T* prDmxFeCfgTable;
    UINT8 u1TunerID = 0;

    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return FALSE;
    }

    //Play back ts and pvr ts don't set ts index ,use swdmx set ts mode
    if (_arDmxMulInstInfo[u1Inst].eConnType > DMX_CONN_TYPE_TUNER_MAX)
    {
        return TRUE;
    }
    //turn 0 -- TsIdx 0 , turn 1 -- TsIdex 1 , turn 2 -- TsIdex 3
    UNUSED(DRVCUST_OptQuery(eDmxFrontendConfigTable, (UINT32 *)(void *)&prDmxFeCfgTable));

	for (i = 0; i < DMX_FRAMER_COUNT; i++)
	{
		if (prDmxFeCfgTable[i].fgEnable)
		{
			if(u1TunerID == (UINT8)_arDmxMulInstInfo[u1Inst].eConnType)
			{
				break;
			}
			else
			{
				u1TunerID++;
			}
		}
	}

    _arDmxMulInstInfo[u1Inst].u1TsIdx = i;
#endif

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetTsIdx
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetTsIdx(UINT8 u1Inst)
{
    if (u1Inst >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        return DMX_FRAMER_COUNT;
    }

    return _arDmxMulInstInfo[u1Inst].u1TsIdx;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_ForceToFreeVdec0
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_ForceToFreeVdec0(BOOL fgEnable)
{
    static UINT8 u1VideoPidx = 0;
    static UINT8 u1ChannelId = 0;
    static BOOL fgVideoPidxKept = FALSE;
    DMX_PID_T rPid;
    UINT32 i;
    BOOL fgRet = TRUE;

    if (fgEnable)
    {
        if (fgVideoPidxKept)
        {
            LOG(5, "we already keep a video PID\n");
            ASSERT(0);
            return FALSE;
        }

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!_DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
            {
                LOG(5, "failed to query pidx: %d\n", i);
                return FALSE;
            }

            if ((rPid.fgEnable == TRUE) && (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO))
            {
                //There is a video pid, disable it
                DMXPIDLOG(DMX_LOG_MUL DMX_LOG_AV 6, i, "force to diable pidx: %d\n", i);
                rPid.fgEnable = FALSE;
                u1VideoPidx = i;
                fgVideoPidxKept = TRUE;
                u1ChannelId = rPid.u1ChannelId;
                fgRet = _DMX_SetPid(i, DMX_PID_FLAG_VALID, &rPid, TRUE);
                _DMX_FreeVideoChannel(i);
                break;
            }
        }
    }
    else
    {
        if (fgVideoPidxKept)
        {
            //enalbe pidx that is disable before
            DMXPIDLOG(DMX_LOG_MUL DMX_LOG_AV 6, u1VideoPidx, "force to enable pidx: %d\n", u1VideoPidx);
            if (!_DMX_AllocateVideoChannel(u1VideoPidx, u1ChannelId))
            {
                LOG(5, "failed to alloc video channel pidx: %d\n", u1VideoPidx);
            }
            rPid.fgEnable = TRUE;
            if (!_DMX_SetPid(u1VideoPidx, DMX_PID_FLAG_VALID, &rPid, TRUE))
            {
                fgRet = FALSE;
                LOG(5, "failed to enable pidx: %d\n", u1VideoPidx);
            }
            fgVideoPidxKept = FALSE;
        }
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_ForceToFreePCR
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_ForceToFreePCR(BOOL fgEnable)
{
    static UINT8 u1PCRPidx = 0;
    static BOOL fgPCRPidxKept = FALSE;
    DMX_PID_T rPid;
    UINT32 i;
    BOOL fgRet = TRUE;

    if (fgEnable)
    {
        if (fgPCRPidxKept)
        {
            LOG(5, "we already keep a video PID\n");
            ASSERT(0);
            return FALSE;
        }

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!_DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
            {
                LOG(5, "failed to query pidx: %d\n", i);
                return FALSE;
            }

            if ((rPid.fgEnable == TRUE) && (rPid.ePidType == DMX_PID_TYPE_NONE))
            {
                //There is a PCR pid, disable it
                DMXPIDLOG(DMX_LOG_MUL DMX_LOG_PCR 6, i, "force to diable pidx: %d\n", i);
                rPid.fgEnable = FALSE;
                u1PCRPidx = i;
                fgPCRPidxKept = TRUE;
                fgRet = _DMX_SetPid(i, DMX_PID_FLAG_VALID, &rPid, TRUE);
                break;
            }
        }
    }
    else
    {
        if (fgPCRPidxKept)
        {
            //enalbe pidx that is disable before
            DMXPIDLOG(DMX_LOG_MUL DMX_LOG_PCR 6, u1PCRPidx, "force to enable pidx: %d\n", u1PCRPidx);
            rPid.fgEnable = TRUE;
            if (!_DMX_SetPid(u1PCRPidx, DMX_PID_FLAG_VALID, &rPid, TRUE))
            {
                fgRet = FALSE;
                LOG(5, "failed to enable pidx: %d\n", u1PCRPidx);
            }
            fgPCRPidxKept = FALSE;
        }
    }

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetFifoOutput
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_SetFifoOutput(UINT8 u1Inst, DMX_PID_TYPE_T ePidType, UINT8 u1ChannelId, BOOL fgEnable)
{
    DMX_PID_T rPid;
    UINT32 i;

    // we don't consider about u1Inst now.

    if ((ePidType != DMX_PID_TYPE_ES_VIDEO) && (ePidType != DMX_PID_TYPE_ES_AUDIO))
    {
        DMXLOG(DMX_LOG_MUL 6, "%s Only support audio and video type\n", __FUNCTION__);
        return FALSE;
    }

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        if (!_DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
        {
            LOG(5, "failed to query pidx: %d\n", i);
            return FALSE;
        }

        if ((rPid.fgEnable == TRUE) && (ePidType == rPid.ePidType))
        {
            if (((rPid.ePidType == DMX_PID_TYPE_ES_VIDEO) && (rPid.u1ChannelId == u1ChannelId)) ||
                    ((rPid.ePidType == DMX_PID_TYPE_ES_AUDIO) && (rPid.u1DeviceId == u1ChannelId)))
            {
                // found pidx
                break;
            }
        }
    }

    if (i == DMX_NUM_PID_INDEX)
    {
        DMXLOG(DMX_LOG_MUL 6, "%s can't find any matched pidx.\n", __FUNCTION__);
        return FALSE;
    }

    rPid.fgDisableFifoOutput = fgEnable?FALSE:TRUE;
    if (!_DMX_SetScrambleCheckPid(i, DMX_PID_FLAG_DISABLE_OUTPUT, &rPid))
    {
        LOG(5, "failed to set FIFO output: %d\n", i);
        return FALSE;
    }

    if (ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        if (fgEnable)
        {
            if (!_DMX_AllocateVideoChannel(i, u1ChannelId))
            {
                LOG(3, "failed to alloc video channel pidx: %d\n", i);
                ASSERT(0);
            }
        }
        else
        {
            _DMX_FreeVideoChannel(i);
        }
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetMulPidInfo
 *  Get a MulPidInfo
 *
 *  @param  u1Pidx         Pid instance
 *  @param  prMulPid       The PID structure of the MUlpidInfo
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_GetMulPidInfo(UINT8 u1Pidx, DMX_MUL_PIDINFO_T* prMulPid)
{

    ASSERT(prMulPid != NULL);

    if (u1Pidx >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "Invalid Pid index: %u\n", u1Pidx);
        return FALSE;
    }

    x_memcpy(prMulPid, &_arDmxMulPidInfo[u1Pidx], sizeof(DMX_MUL_PIDINFO_T));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_GetMulInstInfo
 *  Get a MulInstInfo
 *
 *  @param  u1Inst          Inst id
 *  @param  prMulPid       The PID structure of the MUlpidInfo
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL _DMX_MUL_GetMulInstInfo(UINT8 u1InstId, DMX_MUL_INSTINFO_T* prMulInst)
{

    ASSERT(prMulInst != NULL);

    if (u1InstId >= DMX_MULTIPLE_INSTANCE_MAXNUM)
    {
        LOG(3, "Invalid Instance index: %u\n", u1InstId);
        return FALSE;
    }

    x_memcpy(prMulInst, &_arDmxMulInstInfo[u1InstId], sizeof(DMX_MUL_INSTINFO_T));

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DMX_MUL_GetDtvInstFromTsIdx
 */
//-----------------------------------------------------------------------------
UINT8 _DMX_MUL_GetDtvInstFromTsIdx(UINT8 u1TsIdx)
{
    UINT8 i;

    if (u1TsIdx >= DMX_FRAMER_COUNT)
    {
        return DMX_MULTIPLE_NULL_INSTANCE;
    }

    for(i = 0; i < DMX_MULTIPLE_INSTANCE_MAXNUM; i++)
    {
        if((_arDmxMulInstInfo[i].fgUsed == TRUE) &&
           (_arDmxMulInstInfo[i].eConnType < DMX_CONN_TYPE_TUNER_MAX) &&
           (_arDmxMulInstInfo[i].u1TsIdx == u1TsIdx))
        {
            // We found match instance and return it.
            return i;
        }
    }

    // not found
    return DMX_MULTIPLE_NULL_INSTANCE;
}




