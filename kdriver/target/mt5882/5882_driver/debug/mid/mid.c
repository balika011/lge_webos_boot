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

#include "mid_debug.h"
#include "x_debug.h"
#include "x_mid.h"
#include "x_dram.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_hal_arm.h"
#include "x_lint.h"
#include "drvcust_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    MID_AGENT_ID_T eAgentId;
    const CHAR* szAgentName;
} AGENT_MAPPING_T;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static PFN_MID_CALLBACK _pfnHandler = NULL;

static CRIT_STATE_T _rState;

#define DUMMY_DATA_LEN (32)
UINT32* _dummy_data = NULL;
UINT32 _last_data=0;

static BOOL _fgInit = FALSE;
static BOOL fgDMARBDynamicPri=FALSE;
static BOOL fgDMARBDynamicPriInit=FALSE;
struct AGENT_PRIORITY DMARB_PRIORITY_ORIGINE;
static AGENT_MAPPING_T _arAgentList[] =
{
    // Group 1	
    { MID_AGENT_AUDIO,            "audio" },						// 0
    { MID_AGENT_DEMUX,            "demux_gcpu_ddi" },					// 1
    { MID_AGENT_VDOIN,            "vbi_3d_tve_demod_usb2.0" },				// 2
    { MID_AGENT_OSD,              "osd_od_mmu" },					// 3
    { MID_AGENT_PSCAN,            "nr" },							// 4
    { MID_AGENT_B2R,              "b2r/nb2r" },							// 5
    { MID_AGENT_CPU,              "cpu" },							// 6
    { MID_AGENT_SCPOS,            "scpos" },				// 7
    { MID_AGENT_VDECMC,           "vdec_mc" },						// 8
    { MID_AGENT_VLD,              "vld1" },							// 9
    { MID_AGENT_3DGFX,            "3dgpu" },						// 10
    { MID_AGENT_JPEG,             "2dgraph_jpgdec_imgrsz+png" },	// 11
    { MID_AGENT_VENC,             "venc" },							// 12    
    { MID_AGENT_MJCIN,            "imgrsz(video)" },						// 13    
    { MID_AGENT_MJCOUT,           "rs232_ethernet" },				// 14
    { MID_AGENT_TEST0,            "test0_audiodsp0_arm11" },				// 15

    // Group 2
    { MID_AGENT_USB,              "usb2" },							// 16
    { MID_AGENT_ETHERNET,         "ethernet" },						// 17
    { MID_AGENT_DDI,              "demux_ddi_gcpu" },				// 18
    { MID_AGENT_USB3,             "none" },							// 19
    { MID_AGENT_DSP0,             "audio_dsp0_arm11" },					// 20
    { MID_AGENT_MSDC,             "msdc" },							// 21
    { MID_AGENT_TEST1,            "none" },							// 22
    { MID_AGENT_GDMA,             "gdma" },							// 23

    // Group 3
    { MID_AGENT_GFXLO,            "2d_graph" },						// 23
    { MID_AGENT_TCM,              "nfidma_sfalshdma_lzhs_cispi" },	// 24
    { MID_AGENT_ETHERNET_CHECKSUM,"eth_chksum" },					// 25
    { MID_AGENT_RS232,            "rs232" },						// 26
    { MID_AGENT_SPIS,             "none" },							// 27
    { MID_AGENT_NONE2,            "none" },						// 28
    { MID_AGENT_TEST2,            "test2" },							// 30
	{ MID_AGENT_NONE,             "none" },							// 31
    // Others
    { MID_AGENT_INVALID,          "invalid" }
};

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
LINT_SUPPRESS_NEXT_EXPRESSION(129)
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
LINT_SUPPRESS_NEXT_EXPRESSION(129)
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
static void _Lock(void)
{
    _rState = x_crit_start();
}

//-----------------------------------------------------------------------------
/** _Unlock() Leave the critical section
 */
//-----------------------------------------------------------------------------
static void _Unlock(void)
{
    x_crit_end(_rState);
}

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
    DRAM_ARB_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    DRAM_ARB_WRITE32(u4RegAddr, 0);

    u4RegAddr = MID_BASE + (u4Region*REGION) + AGENT;
    DRAM_ARB_WRITE32(u4RegAddr, 0);

	u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;
    DRAM_ARB_WRITE32(u4RegAddr, 0);

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
    
    u4Val = DRAM_ARB_READ32(u4RegAddr);
    u4Val |= MID_ENABLE;
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);

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
    
    u4Val = DRAM_ARB_READ32(u4RegAddr);
    u4Val &=~ MID_ENABLE;
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);

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
    BOOL fgChannelA = !IS_DRAM_CHANNELB_ACTIVE();

    if (u4Vector == VECTOR_DRAMC)
    {
        // Select DRAM channel A to set mid.
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        // Select DRAM channel B to set mid.
        TCMSET_CHANNELB_ACTIVE();
    }
    
    // Reset region states
    for (i = 0; i < MID_MAX_REGIONS; i++)
    {
        _DisableRegionProtect(i);

        if (!_ResetRegion(i))
        {
            return FALSE;
        }
    }

    // Clear interrupt
    MID_ResetStatus(u4Vector);

    // Restore to channel select.
    if (fgChannelA)
    {
        // Select DRAM channel A to set mid.
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        // Select DRAM channel B to set mid.
        TCMSET_CHANNELB_ACTIVE();
    }
    
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _GetIntruder() Find the intruder by checking hardware register states
 *  @param pu4Region: The region to be intruded
 *  @param peAgentId: The intruder
 *  @param pu4Addr: The intruded address
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
static BOOL _GetIntruder(UINT32* pu4Region, MID_AGENT_ID_T* peAgentId,
    UINT32* pu4Addr)
{
    UINT32 u4Region;   
    UINT32 u4InfoAddr;
    UINT32 u4InfoAgentAddr;
    UINT32 u4Val;
    UINT32 u4IntrudeAgentId;
    UINT32 u4IntrudeAddr;
    BOOL fgRet = FALSE;

    // Detect the region intruded
    for (u4Region = 0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        u4InfoAddr = MID_BASE + MID_INTRUD_INFO + (MID_INTRUD_INFO_OFFSET * u4Region);
        u4Val = DRAM_ARB_READ32(u4InfoAddr);
        if (u4Val & MID_INTRUD_DETECTED)
        {
            // Get intrude agent address.
            u4IntrudeAddr = u4Val & 0x7FFFFFFF;

            // Get intrude agent id.
            u4InfoAgentAddr = 
                MID_BASE + MID_INTRUD_AGENT_INFO + (MID_INTRUD_AGENT_INFO_OFFSET * (u4Region/4));
            u4Val = DRAM_ARB_READ32(u4InfoAgentAddr);
            u4IntrudeAgentId = (u4Val >> (8 * (u4Region & 0x3))) & 0x1F;

            Printf("MEMORY VIOLATION - Region-%d, AgentId: %s, Address: 0x%08X.\n", u4Region, 
                MID_AgentIdToString((MID_AGENT_ID_T)u4IntrudeAgentId), u4IntrudeAddr);

            // Report to upper layer.
            if (peAgentId)
            {
                *peAgentId = (MID_AGENT_ID_T)u4IntrudeAgentId;
            }

            if (pu4Addr)
            {
                *pu4Addr = u4IntrudeAddr;
            }

            if (pu4Region)
            {
                *pu4Region = u4Region;
            }

            fgRet = TRUE;
        }            
    }

    return fgRet;
 }

//---------------------------------------------------------------------
// Interrupt service routine

//-----------------------------------------------------------------------------
/** _MemoryIntruderIsr() This is the ISR of memory intrusion detection
 *  @param u2Vector: The IRQ vector, must be VECTOR_DRAMC
 */
//-----------------------------------------------------------------------------
static VOID _MemoryIntrudeIsr(UINT16 u2Vector)
{
    MID_AGENT_ID_T eAgentId;
    UINT32 u4Region = 0, u4Addr = 0;
    BOOL fgChannelA = !IS_DRAM_CHANNELB_ACTIVE();

    ASSERT((u2Vector == VECTOR_DRAMC) || (u2Vector == VECTOR_DRAMC_CHB));

    if (u2Vector == VECTOR_DRAMC)
    {
        Printf("\nDRAM channel A memory intrude.\n");
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        Printf("\nDRAM channel B memory intrude.\n");
        TCMSET_CHANNELB_ACTIVE();
    }

    // Always check memory intrude.
    VERIFY(_GetIntruder(&u4Region, &eAgentId, &u4Addr));

    // Invoke user registered handler
    if (_pfnHandler != NULL)
    {
        _pfnHandler(u4Region, eAgentId, u4Addr);
    }

    // Clear interrupt
    while (BIM_IsIrqPending(u2Vector))
    {
        MID_ResetStatus(u2Vector);
    }

    // Restore to channel select.
    if (fgChannelA)
    {
        // Select DRAM channel A to set mid.
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        // Select DRAM channel B to set mid.
        TCMSET_CHANNELB_ACTIVE();
    }
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
    x_os_isr_fct pfnOldIsr;

    if (_fgInit)
    {
        return TRUE;
    }
    
    // Reset Channel A MID hardware
    if (!_Reset(VECTOR_DRAMC))
    {
        return FALSE;
    }

    // Reset Channel B MID hardware
    if (!_Reset(VECTOR_DRAMC_CHB))
    {
        return FALSE;
    }

    // Register Channel A MID ISR handler
    if (x_reg_isr(VECTOR_DRAMC, _MemoryIntrudeIsr, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    // Register Channel B MID ISR handler
    if (x_reg_isr(VECTOR_DRAMC_CHB, _MemoryIntrudeIsr, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    _dummy_data = x_mem_alloc(DUMMY_DATA_LEN);

    _pfnHandler = NULL;
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
    x_os_isr_fct pfnOldIsr;

    if (!_fgInit)
    {
        return FALSE;
    }

    _fgInit = FALSE;

    // Uninstall channel A ISR handler
    if (x_reg_isr(VECTOR_DRAMC, NULL, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }

    // Uninstall channel B ISR handler
    if (x_reg_isr(VECTOR_DRAMC_CHB, NULL, &pfnOldIsr) != OSR_OK)
    {
        return FALSE;
    }
    
    // Reset Channel A MID hardware
    if (!_Reset(VECTOR_DRAMC))
    {
        return FALSE;
    }

    // Reset Channel B MID hardware
    if (!_Reset(VECTOR_DRAMC_CHB))
    {
        return FALSE;
    }

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
/** MID_RegisterHandler() Register a callback handler. The handler will be \n
 *      called when memory intrusion happened.
 *  @param pfnNewHandler: The new handler to be registered
 *  @return The old handler
 */
//-----------------------------------------------------------------------------
PFN_MID_CALLBACK MID_RegisterHandler(PFN_MID_CALLBACK pfnNewHandler)
{
    PFN_MID_CALLBACK pfnOldHandler;

    ASSERT(_fgInit);

    _Lock();

    pfnOldHandler = _pfnHandler;
    _pfnHandler = pfnNewHandler;

    _Unlock();

    return pfnOldHandler;
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
BOOL MID_SetRegionEx2(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags, UINT32 u4AgentListMask)
{
    UINT32 u4Val;    
    UINT32 u4RegAddr;
    UINT32 u4MidMaxVal = 0x1FFFFFFC;

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
        u4MidMaxVal = (TCMGET_CHANNELB_SIZE() * 0x100000)-4;
        u4LowAddr = (u4LowAddr > u4MidMaxVal) ? u4MidMaxVal : (u4LowAddr & u4MidMaxVal);
        u4LowAddr += (TCMGET_CHANNELA_SIZE() * 0x100000);
        u4HighAddr = (u4HighAddr > u4MidMaxVal) ? u4MidMaxVal : (u4HighAddr & u4MidMaxVal);
        u4HighAddr += (TCMGET_CHANNELA_SIZE() * 0x100000);        
    }
#if 0
    if (u4HighAddr == 0x3FFFFFFC)
    {
	u4HighAddr = 0x5FFFFFFC; // dirty hack: to protect all chB (0x20000000 ~ 0x5FFFFFFC)
    }
#endif

    // Check region state?
    _Lock();

    // Setup region on lower address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + LOWADDR;    
    u4Val = DRAM_ARB_READ32(u4RegAddr);
    u4Val = u4LowAddr;
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);

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
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);

    // Setup region on upper address register.
    u4RegAddr = MID_BASE + (u4Region*REGION) + UPADDR;    
    u4Val = DRAM_ARB_READ32(u4RegAddr);
    u4Val = u4HighAddr;
	DRAM_ARB_WRITE32(u4RegAddr, u4Val);

	u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;    
    u4Val = DRAM_ARB_READ32(u4RegAddr);
	if (u4Flags & MID_FLAG_INVERSE)
    {
        // Enable (region >= upper address)  || (region < lower address).
        u4Val |= MID_MODE;    
    }
    if (u4Flags & MID_FLAG_PROTRD)
    {
	u4Val |= MID_READPROTECT;
    }
#if 0 // by default, to protect write-access
    if (u4Flags & MID_FLAG_PROTWR)
    {
	u4Val &= ~MID_WRITEABLE;
    }
#endif
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);

    _Unlock();

    return TRUE;
}

//-----------------------------------------------------------------------------
/** MID_SetRegionEx() Setup a region with flags
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @param u4Flags: THe Inverse flag & CPU access flag.
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegionEx(UINT32 u4Region, MID_AGENT_ID_T eAgentId,
    UINT32 u4LowAddr, UINT32 u4HighAddr, UINT32 u4Flags)
{
    return MID_SetRegionEx2(u4Region, eAgentId, u4LowAddr, u4HighAddr, u4Flags, 0);
}

//-----------------------------------------------------------------------------
/** MID_SetRegion() Setup a region
 *  @param u4Region: The region to be setup
 *  @param eAgentid: The only agent allowed to write the region
 *  @param u4LowAddr: The low address of the region
 *  @param u4HighAddr: The high address of the region
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
BOOL MID_SetRegion(UINT32 u4Region, MID_AGENT_ID_T eAgentId, UINT32 u4LowAddr,
    UINT32 u4HighAddr)
{
    return MID_SetRegionEx(u4Region, eAgentId, u4LowAddr, u4HighAddr, 0);
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
/** MID_AllocateFreeRegion() Find a free region
 *  @param fgReadOnly -input- If the region is a read-only one
 *  @param pu4Region -output- The region allocated
 *  @retval TRUE: A free region is found
 *  @retval FALSE: No free region for the given condition
 */
//-----------------------------------------------------------------------------
BOOL MID_AllocateFreeRegion(BOOL fgReadOnly, UINT32* pu4Region)
{
    BOOL fgRet = FALSE;
    UINT32 u4Region;
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    UNUSED(fgReadOnly);
    
    if (!_fgInit)
    {
        return FALSE;
    }

    _Lock();

    for (u4Region=0; u4Region < MID_MAX_REGIONS; u4Region++)
    {
        u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;        
        u4Val = DRAM_ARB_READ32(u4RegAddr);
        
        if ((u4Val & MID_ENABLE) == 0)
        {
            if (pu4Region != NULL)
            {
                *pu4Region = u4Region;
            }

            fgRet = TRUE;
            break;
        }
    }

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

    HalFlushInvalidateDCacheMultipleLine((UINT32)_dummy_data, DUMMY_DATA_LEN);
    _last_data = (*((UINT32*)_dummy_data));
    HalFlushWriteBuffer();
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
/** MID_ShowAgentList() show agent status
 *  @param void
 *  @return void
 */
//-----------------------------------------------------------------------------
void MID_ShowAgentList(void)
{
    UINT32 i;
    UINT32 u4NumAgents;
    
    Printf("DRAM agent list\n");
    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        Printf("  %2u: %s\n", (UINT32)_arAgentList[i].eAgentId, _arAgentList[i].szAgentName);    
    }    
}

//-----------------------------------------------------------------------------
/** MID_AgentIdToString() Convert an agent id to a readable string
 *  @param eAgentId: The agent id
 *  @return The string
 */
//-----------------------------------------------------------------------------
const CHAR* MID_AgentIdToString(MID_AGENT_ID_T eAgentId)
{
    UINT32 u4NumAgents, i;
    const CHAR* szAgentName = "unused";

    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        if (_arAgentList[i].eAgentId == eAgentId)
        {
            szAgentName = _arAgentList[i].szAgentName;
            break;
        }
    }

    return szAgentName;
}

//-----------------------------------------------------------------------------
/** MID_StringToAgentId() Convert a string to an agent id
 *  @param szAgentName The string of agent name
 *  @return The agent id
 */
//-----------------------------------------------------------------------------
MID_AGENT_ID_T MID_StringToAgentId(const CHAR* szAgentName)
{
    UINT32 i, u4NumAgents;
    MID_AGENT_ID_T eAgentId = MID_AGENT_INVALID;

    u4NumAgents = (sizeof (_arAgentList) / sizeof (AGENT_MAPPING_T)) - 1;

    for (i = 0; i < u4NumAgents; i++)
    {
        if (x_strcmp(_arAgentList[i].szAgentName, szAgentName) == 0)
        {
            eAgentId = _arAgentList[i].eAgentId;
            break;
        }
    }

    return eAgentId;
}

//-----------------------------------------------------------------------------
/** MID_ResetStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
VOID MID_ResetStatus(UINT32 u4Vector)
{
    UINT32 u4RegAddr;
    UINT32 u4InfoAddr;
    UINT32 u4Val;
    BOOL fgLoop;
    UINT32 i;
    BOOL fgChannelA = !IS_DRAM_CHANNELB_ACTIVE();

    if (u4Vector == VECTOR_DRAMC)
    {
        // Select DRAM channel A to set mid.
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        // Select DRAM channel B to set mid.
        TCMSET_CHANNELB_ACTIVE();
    }
    
    u4RegAddr = MID_BASE + MID_CLEAR_STATUS;
    
    DRAM_ARB_WRITE32(u4RegAddr, 0x1);    

    // Check each region status.
    do
    {
        fgLoop = FALSE;
        for (i = 0; i < MID_MAX_REGIONS; i++)
        {
            u4InfoAddr = MID_BASE + MID_INTRUD_INFO + (MID_INTRUD_INFO_OFFSET * i);
            u4Val = DRAM_ARB_READ32(u4InfoAddr);
            if (u4Val & MID_INTRUD_DETECTED)
            {
                fgLoop = TRUE;
            }
        }
    }
    while (fgLoop);

    DRAM_ARB_WRITE32(u4RegAddr, 0x0);

    BIM_ClearIrq(u4Vector);

    // Restore to channel select.
    if (fgChannelA)
    {
        // Select DRAM channel A to set mid.
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        // Select DRAM channel B to set mid.
        TCMSET_CHANNELB_ACTIVE();
    }
    
}

//-----------------------------------------------------------------------------
/** MID_QueryIntrusionStatus() Reset all intrusion status
 *  @retval None
 */
//-----------------------------------------------------------------------------
UINT32 MID_QueryIntrusionStatus(void)
{
    UINT32 ret = 0;
    if (!_GetIntruder(NULL, NULL, NULL))
    {
        Printf("No CHA memory violation.\n");
    }
    else
    {
        ret |= 0x01;
    }

    if(IS_DRAM_CHANNELB_SUPPORT())
    {
        TCMSET_CHANNELB_ACTIVE();
        if (!_GetIntruder(NULL, NULL, NULL))
        {
            Printf("No CHB memory violation.\n");
        }
        else
        {
            ret |= 0x02;
        }
        TCMSET_CHANNELA_ACTIVE();
    }

    return ret;
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
        Printf("Select DRAM channel A.\n");
        TCMSET_CHANNELA_ACTIVE();
    }
    else
    {
        Printf("Select DRAM channel B.\n");    
        TCMSET_CHANNELB_ACTIVE();
    }        
}

#ifdef CC_SUPPORT_STR_CORE_OFF

static BOOL _EnableRegionReadProtect(UINT32 u4Region, BOOL onoff)
{
#if 0
    UINT32 u4Val;    
    UINT32 u4RegAddr;

    u4RegAddr = MID_BASE + (u4Region*REGION) + EN_MODE_RWPRT;
    
    u4Val =  DRAM_ARB_READ32(u4RegAddr);
    if (onoff)
    {
        u4Val |= MID_READPROTECT;
    }
    else
    {
        u4Val &= ~MID_READPROTECT;
    }
    DRAM_ARB_WRITE32(u4RegAddr, u4Val);
#endif

    return TRUE;
}

BOOL MID_EnableRegionReadProtect(UINT32 u4Region, BOOL onoff)
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

    fgRet = _EnableRegionReadProtect(u4Region, onoff);

    _Unlock();

    return fgRet;
}
//Add Agent flag option
int _mt53xx_protect_all_dramEx(UINT32 u4AgentFlags)
{
    UINT32 u4LowAddr, u4HighAddr, u4Region;

 	Printf("_mt53xx_protect_all_dramEx.\n"); 
	Printf("Select DRAM channel %s.\n", IS_DRAM_CHANNELB_ACTIVE()? "B":"A");

    u4LowAddr = 0;
	if(!IS_DRAM_CHANNELB_ACTIVE())
		u4HighAddr = ((UINT32)(TCMGET_CHANNELA_SIZE()*0x100000) - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);
	else
		u4HighAddr = ((UINT32)(TCMGET_CHANNELB_SIZE()*0x100000) - MID_ADDR_ALIGNMENT + 1) & ~(MID_ADDR_ALIGNMENT - 1);
	
	if (!MID_AllocateFreeRegion(TRUE, &u4Region))
    {
        return FALSE;
    }
    //Printf("Protect readonly memory from 0x%08x to 0x%08x\n", u4LowAddr, u4HighAddr);

    // Set All dram to read-only
    //if (!MID_SetRegion(u4Region, u4AgentFlags, u4LowAddr, u4HighAddr))//MID_AGENT_CPU
    if (!MID_SetRegionEx(u4Region, u4AgentFlags, u4LowAddr, u4HighAddr, MID_FLAG_PROTWR|MID_FLAG_PROTRD))
    {
        return FALSE;
    }
    Printf("Protect readonly memory from 0x%08x to 0x%08x, region: %d\n", u4LowAddr, u4HighAddr, u4Region);

#if 0
    if(!MID_EnableRegionReadProtect(u4Region, 1))
    {
        return FALSE;
    }
#endif

    // Enable the region detection
    if (!MID_EnableRegionProtect(u4Region))
    {
        return FALSE;
    }
	return TRUE;
}

UINT32 MID_WarmBootRegionProtect(UINT32 onoff)
{
	//UINT32 i;
    if (onoff)
    {  
		TCMSET_CHANNELA_ACTIVE();
		if (_mt53xx_protect_all_dramEx(MID_AGENT_CPU))
			Printf("MID enable all except CPU - success\n");
		else
			Printf("MID enable all except CPU - fail\n");
		
		if(IS_DRAM_CHANNELB_SUPPORT())
		{
			TCMSET_CHANNELB_ACTIVE();
			//if (_mt53xx_protect_all_dramEx(MID_AGENT_CPU)) // for checksum of chB
			//if (_mt53xx_protect_all_dramEx(MID_AGENT_NONE))
			if (_mt53xx_protect_all_dramEx(MID_AGENT_CPU)) // for checksum of chB
				Printf("CHB MID enable all except CPU - success\n");
			else
				Printf("CHB MID enable all - fail\n");
			TCMSET_CHANNELA_ACTIVE();	
		}
    }
	else
	{
		MID_QueryIntrusionStatus();
	}
	return TRUE;

}	

static UINT32* pm_save_mid_regs= NULL;
static UINT32* pm_save_pri_regs= NULL;

void DMARB_mid_suspend(void)
{
	UINT32 i;

	pm_save_mid_regs = x_mem_alloc(0x70);//0x6200~0x6270
	pm_save_pri_regs = x_mem_alloc(12 * sizeof(UINT32));//0x6200~0x6270
		
	Printf("DMARB_mid_suspend addr : 0x%x .\n",pm_save_mid_regs);
	TCMSET_CHANNELA_ACTIVE();
	if (pm_save_mid_regs!=NULL && pm_save_pri_regs!=NULL)
    {
	    for(i=0;i<0x1C;i++) // 0x70%4 = 0x1C
	    {
	    	*(pm_save_mid_regs + i) = DRAM_ARB_READ32(MID_BASE+i*4);
	    }
		pm_save_pri_regs[0] = DRAM_ARB_READ32(0x00);
		pm_save_pri_regs[1] = DRAM_ARB_READ32(0x04);
		pm_save_pri_regs[2] = DRAM_ARB_READ32(0x10);
		pm_save_pri_regs[3] = DRAM_ARB_READ32(0x14);
		pm_save_pri_regs[4] = DRAM_ARB_READ32(0x18);
		pm_save_pri_regs[5] = DRAM_ARB_READ32(0x1C);
		pm_save_pri_regs[6] = DRAM_ARB_READ32(0x20);
		pm_save_pri_regs[7] = DRAM_ARB_READ32(0x24);
		pm_save_pri_regs[8] = DRAM_ARB_READ32(0x28);
		pm_save_pri_regs[9] = DRAM_ARB_READ32(0x2C);
		pm_save_pri_regs[10] = DRAM_ARB_READ32(0x60);
		pm_save_pri_regs[11] = DRAM_ARB_READ32(0x6c);
		
	}
}

void DMARB_mid_resume(void)
{
	UINT32 i;
	if (pm_save_mid_regs!=NULL && pm_save_pri_regs!=NULL)
    {
		TCMSET_CHANNELA_ACTIVE();
		for(i=0;i<0x1C;i++) // 0x70%4 = 0x1C
	    {
	    	DRAM_ARB_WRITE32((MID_BASE+i*4), *(pm_save_mid_regs + i));
	    }
		DRAM_ARB_WRITE32(0x00 , pm_save_pri_regs[0]);
		DRAM_ARB_WRITE32(0x04 , pm_save_pri_regs[1]);
		DRAM_ARB_WRITE32(0x10 , pm_save_pri_regs[2]);
		DRAM_ARB_WRITE32(0x14 , pm_save_pri_regs[3]);
		DRAM_ARB_WRITE32(0x18 , pm_save_pri_regs[4]);
		DRAM_ARB_WRITE32(0x1C , pm_save_pri_regs[5]);
		DRAM_ARB_WRITE32(0x20 , pm_save_pri_regs[6]);
		DRAM_ARB_WRITE32(0x24 , pm_save_pri_regs[7]);
		DRAM_ARB_WRITE32(0x28 , pm_save_pri_regs[8]);
		DRAM_ARB_WRITE32(0x2C , pm_save_pri_regs[9]);
		DRAM_ARB_WRITE32(0x60 , pm_save_pri_regs[10]);
		DRAM_ARB_WRITE32(0x6c , pm_save_pri_regs[11]|0x1);
		
		x_mem_free(pm_save_mid_regs);
		x_mem_free(pm_save_pri_regs);
    }
    else
    {
		Printf("MID restore mid mem fail\n");
    }
}
#endif 
//-----------------------------------------------------------------------------
/** MID_Init() Initialize MID module
 *  @retval TRUE: Succeed
 *  @retval FALSE: Fail
 */
//-----------------------------------------------------------------------------
void DMARB_Dynamic_Priority_Enable(BOOL u4Enable)
{
	if(u4Enable)
		{
			fgDMARBDynamicPri=TRUE;
			//Printf("Dynamic priority enabled!\n");
		}
	else
		{
			fgDMARBDynamicPri=FALSE;
			//Printf("Dynamic priority disabled!\n");
		}
}

void DMARB_Dynamic_Priority_Init(void)
{
	if(!fgDMARBDynamicPriInit)
		{
			DMARB_PRIORITY_ORIGINE.u4prityL=DRAM_ARB_READ32(0x10);
			DMARB_PRIORITY_ORIGINE.u4prityH=DRAM_ARB_READ32(0x14);
			DMARB_PRIORITY_ORIGINE.u4prityjumpL=DRAM_ARB_READ32(0x18);
			DMARB_PRIORITY_ORIGINE.u4prityjumpH=DRAM_ARB_READ32(0x1c);
			Printf("Dynamic priority Init!\n");
			Printf("Default value is u4prityL=0x%x , u4prityH=0x%x \n",DMARB_PRIORITY_ORIGINE.u4prityL,DMARB_PRIORITY_ORIGINE.u4prityH);
			Printf("                 u4prityjumpL=0x%x , u4prityjumpH=0x%x \n",DMARB_PRIORITY_ORIGINE.u4prityjumpL,DMARB_PRIORITY_ORIGINE.u4prityjumpH);
		}
	fgDMARBDynamicPriInit=TRUE;
}


UINT32 DMARB_Dynamic_Priority_Set(struct AGENT_PRIORITY u4priority)
{
	UINT32 u4temp,i=0;

	_Lock();
	if(fgDMARBDynamicPri==FALSE)
		{
			Printf("Dynamic priority switch is not enabled!\n");
			return -1;
		}
	
	DRAM_ARB_WRITE32(0x10,u4priority.u4prityL);
	DRAM_ARB_WRITE32(0x14,u4priority.u4prityH);
	//DRAM_ARB_WRITE32(0x18,u4priority.u4prityjumpL);
	//DRAM_ARB_WRITE32(0x1c,u4priority.u4prityjumpL);
	u4temp=DRAM_ARB_READ32(0x6c);
	u4temp&=~(0x1<<31);
	DRAM_ARB_WRITE32(0x6c,u4temp);
	u4temp|=0x1;
	DRAM_ARB_WRITE32(0x6c,u4temp);
	
	_Unlock();
	while(1)
		{
			u4temp=DRAM_ARB_READ32(0x6c);
			if((u4temp&0x1)==0)
				break;
			
			if(i==100)
				{
					Printf("DMARB Dynamic priority polling time out!");
					return -1;
				}
			i++;
			x_thread_delay(100);
		}
	return 0;
}

UINT32 DMARB_Dynamic_Priority_Reset(void)
{
	UINT32 u4val;
	struct AGENT_PRIORITY u4pri;

	if(fgDMARBDynamicPri==FALSE)
		{
			Printf("Dynamic priority switch is not enabled!\n");
			return 0;
		}
	u4pri.u4prityL=DMARB_PRIORITY_ORIGINE.u4prityL;
	u4pri.u4prityH=DMARB_PRIORITY_ORIGINE.u4prityH;

	
	u4val=DMARB_Dynamic_Priority_Set(u4pri);
	Printf("reset done.\n");
	//Printf("Default value is u4prityL=%x , u4prityH=%x \n",DMARB_PRIORITY_ORIGINE.u4prityL,DMARB_PRIORITY_ORIGINE.u4prityH);
	//Printf("                 u4prityjumpL=%x , u4prityjumpH=%x \n",DMARB_PRIORITY_ORIGINE.u4prityjumpL,DMARB_PRIORITY_ORIGINE.u4prityjumpH);
	return u4val;
}

