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
 * $RCSfile: mid.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file mid.c
 *  mid.c Memory intrusion detection module
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_mid.h"
#include "x_dram.h"
#include "x_bim.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


#define MID_TZ_BASE (IO_VIRT + 0x37000)

#define MID_TZ_CHB_BASE (IO_VIRT + 0x38000)


#define MID_TZ_READ32(offset)			(IS_DRAM_CHANNELB_ACTIVE() ? \
                                                                   IO_READ32(MID_TZ_CHB_BASE, (offset)) : \
                                                                   IO_READ32(MID_TZ_BASE, (offset)))
#define MID_TZ_WRITE32(offset, value)   (IS_DRAM_CHANNELB_ACTIVE() ? \
																	   IO_WRITE32(MID_TZ_CHB_BASE, (offset), (value)) : \
																	   IO_WRITE32(MID_TZ_BASE, (offset), (value)))
#undef MID_BASE
#define MID_BASE (0)
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static BOOL _fgInit = FALSE;
//-----------------------------------------------------------------------------
// Inline functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _IsRegionValid() Check if the given region is valid
 *  @param u4Region: The region number
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
INLINE static BOOL _IsRegionValid(UINT32 u4Region)
{
    return (u4Region < MID_MAX_REGIONS);
}

//-----------------------------------------------------------------------------
/** _IsAgentValid() Check if an agent id is valid
 *  @param eAgentId: The agent id to be checked
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
INLINE static BOOL _IsAgentValid(MID_AGENT_ID_T eAgentId)
{
    return (eAgentId < MID_AGENT_MAX);
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _Align() Return the nearest aligned address equal to or greater than the \n
 *      given address
 *  @param u4Addr: The address
 *  @param u4Alignment: The alignment requirement in byte
 *  @return The aligned address
 */
//-----------------------------------------------------------------------------
static UINT32 _Align(UINT32 u4Addr, UINT32 u4Alignment)
{
    UINT32 u4Unaligned;

    if (u4Alignment <= 1)
    {
        return u4Addr;
    }

    u4Unaligned = u4Addr % u4Alignment;
    if (u4Unaligned != 0)
    {
        u4Addr += u4Alignment - u4Unaligned;
    }

    return u4Addr;
}

//-----------------------------------------------------------------------------
/** _IsAligned() Check if an address is aligned to the given alignment
 *  @param u4Addr: The address to be checked
 *  @param u4Alignment: The alignment
 *  @retval TRUE: The address is aligned
 *  @retval FALSE: Otherwise
 */
//-----------------------------------------------------------------------------
static BOOL _IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
	return u4Addr == _Align(u4Addr, u4Alignment);
}

//-----------------------------------------------------------------------------
/** _Lock() Enter the critical section to access global data
 */
//-----------------------------------------------------------------------------
static void _Lock(void) {}

//-----------------------------------------------------------------------------
/** _Unlock() Leave the critical section
 */
//-----------------------------------------------------------------------------
static void _Unlock(void) {}

//-----------------------------------------------------------------------------
/** _ResetRegion() Reset a region to normal state
 *  @param u4Region: The region to be reset
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _ResetRegion(UINT32 u4Region)
{
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;    
    MID_TZ_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    MID_TZ_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;
    MID_TZ_WRITE32(u4RegAddr, 0);

	u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;
    MID_TZ_WRITE32(u4RegAddr, 0);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _EnableRegionProtect() Enable the intrusion detection of a region
 *  @param u4Region: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _EnableRegionProtect(UINT32 u4Region)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;
    
    u4Val = MID_TZ_READ32(u4RegAddr);
    u4Val |= MID_ENABLE;
    MID_TZ_WRITE32(u4RegAddr, u4Val);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _DisableRegionProtect() Disable the intrusion detection of a region
 *  @param u4Retion: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _DisableRegionProtect(UINT32 u4Region)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;
    
    u4Val = MID_TZ_READ32(u4RegAddr);
    u4Val &=~ MID_ENABLE;
    MID_TZ_WRITE32(u4RegAddr, u4Val);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _Reset() Reset all regions to normal state
 *  @param u4Vector: dram interrupt vector.
 *  @reval TRUE: Succeed
 *  @reval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _Reset(UINT32 u4Vector)
{
    UINT32 i;

	UNUSED(u4Vector);
    // Reset region states
    for (i = 0; i < MID_MAX_REGIONS; i++)
    {
        _DisableRegionProtect(i);

        if (!_ResetRegion(i))
        {
            return FALSE;
        }
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** MID_Init() Initialize MID module
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Init(void)
{
    if (_fgInit)
    {
        return TRUE;
    }

	MID_SetDRAMChannel(TRUE);
    // Reset Channel A MID hardware
    if (!_Reset(VECTOR_DRAMC))
    {
        return FALSE;
    }

	MID_SetDRAMChannel(FALSE);
    // Reset Channel B MID hardware
    if (!_Reset(VECTOR_DRAMC_CHB))
    {
        return FALSE;
    }

	MID_SetDRAMChannel(TRUE);

    _fgInit = TRUE;

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_Exit() Shutdown MID
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Exit(void)
{

    if (!_fgInit)
    {
        return FALSE;
    }

    _fgInit = FALSE;

	MID_SetDRAMChannel(TRUE);
    // Reset Channel A MID hardware
    if (!_Reset(VECTOR_DRAMC))
    {
        return FALSE;
    }

	MID_SetDRAMChannel(FALSE);
    // Reset Channel B MID hardware
    if (!_Reset(VECTOR_DRAMC_CHB))
    {
        return FALSE;
    }

	MID_SetDRAMChannel(TRUE);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_Reset() Reset all regions to initial states
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_Reset(void)
{
    if (!_fgInit)
    {
        return FALSE;
    }

    _Lock();

    // Reset Channel A MID hardware
    if (!_Reset(VECTOR_DRAMC))
    {
        _Unlock();
        return FALSE;
    }

    // Reset Channel B MID hardware
    if (!_Reset(VECTOR_DRAMC_CHB))
    {
        _Unlock();    
        return FALSE;
    }

    _Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_SetRegionEx() Setup a region with flags
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @param u4Flags: The Inverse flag & CPU access flag.
 *  @param u4AgentListMask: Agent write on/off list mask.
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegionEx3(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags, UINT32 u4AgentListMask,BOOL readable,BOOL writable)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;
    UINT32 u4MidMaxVal = 0x1FFFFFFC;
	UINT32 u4MidMinVal = 0x0;

    if (!_fgInit)
    {
        return FALSE;
    }

    // Check region number and agent id
    if (!_IsRegionValid(u4Region) || !_IsAgentValid(eAgentId))
    {
        return FALSE;
    }

    // Check address range
    if (u4LowAddr > u4HighAddr)
    {
        return FALSE;
    }

    // Check address alignments
    if (!_IsAligned(u4LowAddr, MID_ADDR_ALIGNMENT) ||
        !_IsAligned(u4HighAddr, MID_ADDR_ALIGNMENT))
    {
        return FALSE;
    }

    // Over MID boundary fixup.
    if (!IS_DRAM_CHANNELB_ACTIVE())
    {
        u4MidMaxVal = (TCMGET_CHANNELA_SIZE() * 0x100000)-4;
        u4LowAddr = (u4LowAddr > u4MidMaxVal) ? u4MidMaxVal : (u4LowAddr & u4MidMaxVal);
        u4HighAddr = (u4HighAddr > u4MidMaxVal) ? u4MidMaxVal : (u4HighAddr & u4MidMaxVal);
    }
    else
    {
    	u4MidMinVal = (TCMGET_CHANNELA_SIZE() * 0x100000);
        u4MidMaxVal = (TCMGET_CHANNELB_SIZE() * 0x100000)-4;
		if(u4LowAddr > u4MidMinVal + u4MidMaxVal)
		{
        	 u4LowAddr = u4MidMinVal + u4MidMaxVal;
		}
		else if(u4LowAddr >= u4MidMinVal)
		{
			u4LowAddr = u4LowAddr;
		}
		else
		{
			u4LowAddr = u4MidMinVal;
		}

		if(u4HighAddr > u4MidMinVal + u4MidMaxVal)
		{
        	 u4HighAddr = u4MidMinVal + u4MidMaxVal;
		}
		else if(u4HighAddr >= u4MidMinVal)
		{
			u4HighAddr = u4HighAddr;
		}
		else
		{
			u4HighAddr = u4MidMinVal;
		}      
    }

    // Check region state?
    _Lock();

    // Setup region on lower address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    u4Val = MID_TZ_READ32(u4RegAddr);
    u4Val = u4LowAddr;
    MID_TZ_WRITE32(u4RegAddr, u4Val);

    // Setup write allow agent id on this region.
    u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;    
    u4Val = 0;

    if (eAgentId < MID_AGENT_NONE)
    {        
        if (eAgentId >= GROUP3_FIRST_AGENT_ID)
        {
            /* 
                All group 3 need to shift one bit. 
                Because gfx id = 23 but h/w control is at bit 24.
            */
            eAgentId <<= 1;
        }
        u4Val |= 1 << (UINT32)eAgentId;
    }

    // Allow cpu to write this region.
    if (u4Flags & MID_FLAG_CPUACCESS)    
    {
        u4Val |= 1 << (UINT32)MID_AGENT_CPU;        
    }

    // Support multiple agent list to write this region.
    if (u4AgentListMask > 0)
    {
        u4Val |= u4AgentListMask;
    }    
    MID_TZ_WRITE32(u4RegAddr, u4Val);

    // Setup region on upper address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;    
    u4Val = MID_TZ_READ32(u4RegAddr);
    u4Val |= u4HighAddr;
	MID_TZ_WRITE32(u4RegAddr, u4Val);

	u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;    
    u4Val = MID_TZ_READ32(u4RegAddr);
	if (u4Flags & MID_FLAG_INVERSE)
    {
        // Enable (region >= upper address)  || (region < lower address).
        u4Val |= MID_MODE;    
    }
	if(!readable)
	{
		u4Val |= MID_READPROTECT;   
	}

	if(writable)
	{
		u4Val |= MID_WRITEABLE;
	}
    MID_TZ_WRITE32(u4RegAddr, u4Val);

    _Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_ResetRegion() Reset a region to initial state
 *  @param u4Region: The region to be reset
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_ResetRegion(UINT32 u4Region)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _ResetRegion(u4Region);

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_EnableRegionProtect() Enable the intrusion detection of a region
 *  @param u4Retion: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_EnableRegionProtect(UINT32 u4Region)
{
	BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _EnableRegionProtect(u4Region);

    _Unlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** MID_DisableRegionProtect() Disable the intrusion detection of a region
 *  @param u4Region: The region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_DisableRegionProtect(UINT32 u4Region)
{
    BOOL fgRet;

    if (!_fgInit)
    {
        return FALSE;
    }

    if (!_IsRegionValid(u4Region))
    {
        return FALSE;
    }

    _Lock();

    fgRet = _DisableRegionProtect(u4Region);

    _Unlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** MID_SetDRAMChannel() Setup DRAM channel A or B.
 *  @param fgChannelA: TRUE channel A, FALSE channel B.
 *  @retval void
 */
//-----------------------------------------------------------------------------
void MID_SetDRAMChannel(BOOL fgChannelA)
{
    if (fgChannelA)
    {
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {  
        TCMSET_CHANNELB_ACTIVE();
    }        
}

#define SEC_AGENT_MASK (1<<MID_AGENT_VDOIN 	| 1<<MID_AGENT_OSD 	| \
	  					1<<MID_AGENT_PSCAN 	| 1<<MID_AGENT_B2R 	|  \
	  					1<<MID_AGENT_SCPOS 	| 1<<MID_AGENT_VDECMC | 1<<MID_AGENT_VLD | \
	  					1<<MID_AGENT_MJCIN 	| 1<<MID_AGENT_MJCOUT )
	  					
#define SPEC_AGENT_FOR_DEV (1<< MID_AGENT_JPEG)

#define CPU_GCPU_AGENT_MASK (1<<MID_AGENT_CPU | 1<<MID_AGENT_DEMUX | 1<<MID_AGENT_DDI)

UINT32 sec_agent_mask = (UINT32)SEC_AGENT_MASK;
	
UINT32 normal_agent_mask = (~(UINT32)SEC_AGENT_MASK)&(~CPU_GCPU_AGENT_MASK);

UINT32 cpu_gcpu_agent_mask = CPU_GCPU_AGENT_MASK;

UINT32 sec_fbm_start = 0;
UINT32 sec_fbm_end = 0;
UINT32 sec_heap_start = 0;
UINT32 sec_heap_end = 0;

void EnableSeucreMID(void)
{
	//channel A
	MID_SetDRAMChannel(TRUE);
	//non-secure buffer,secure agent cannot write them
	MID_SetRegionEx3(0,MID_AGENT_NONE,sec_fbm_start,sec_fbm_end,MID_FLAG_INVERSE,normal_agent_mask|cpu_gcpu_agent_mask,TRUE,TRUE);
	//secure fbm,normal agent can write ,can not read
	MID_SetRegionEx3(1,MID_AGENT_NONE,sec_fbm_start,sec_fbm_end,0,sec_agent_mask|cpu_gcpu_agent_mask|SPEC_AGENT_FOR_DEV,TRUE,TRUE);
	//secure heap can only access by cpu/gcpu,other agenets write-protected,read-protected
	MID_SetRegionEx3(2,MID_AGENT_NONE,sec_heap_start,sec_heap_end,0,cpu_gcpu_agent_mask,TRUE,TRUE);
	MID_EnableRegionProtect(0);
	MID_EnableRegionProtect(1);
	MID_EnableRegionProtect(2);

	//channel B
	MID_SetDRAMChannel(FALSE);
	MID_SetRegionEx3(0,MID_AGENT_NONE,sec_fbm_start,sec_fbm_end,MID_FLAG_INVERSE,normal_agent_mask|cpu_gcpu_agent_mask,TRUE,TRUE);
	//secure fbm,normal agent can write ,can not read
	MID_SetRegionEx3(1,MID_AGENT_NONE,sec_fbm_start,sec_fbm_end,0,sec_agent_mask|cpu_gcpu_agent_mask|SPEC_AGENT_FOR_DEV,TRUE,TRUE);
	//secure heap can only access by cpu/gcpu,other agenets write-protected,read-protected
	MID_SetRegionEx3(2,MID_AGENT_NONE,sec_heap_start,sec_heap_end,0,cpu_gcpu_agent_mask,TRUE,TRUE);
	MID_EnableRegionProtect(0);
	MID_EnableRegionProtect(1);
	MID_EnableRegionProtect(2);

	MID_SetDRAMChannel(TRUE);
}

void DisableSecureMID(void)
{
	//channel A
	MID_SetDRAMChannel(TRUE);
	MID_DisableRegionProtect(0);
	MID_DisableRegionProtect(1);
	MID_DisableRegionProtect(2);

	//channel B
	MID_SetDRAMChannel(FALSE);
	MID_DisableRegionProtect(0);
	MID_DisableRegionProtect(1);
	MID_DisableRegionProtect(2);

	MID_SetDRAMChannel(TRUE);
}

