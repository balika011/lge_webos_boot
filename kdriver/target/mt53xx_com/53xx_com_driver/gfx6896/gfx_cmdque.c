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
 * $RCSfile: gfx_cmdque.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_cmdque.c
 *  Brief of file gfx_cmdque.c.
 *  Details of file gfx_cmdque.c (optional).
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_cmdque.h"
#include "gfx_dif.h"
#include "gfx_hw.h"


#if defined(CC_ENABLE_GRAPHIC_DUMP)
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include <linux/file_ops.h>
#include "osd_drvif.h"
#endif
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// que capacity
#define E_GFX_CQ_QCAPACITY      (_prGfxCmdQue->i4QueCapacity)

/// que align mask
//  gfx command que should align on dram word (128 bits = 16 bytes)
// #define E_GFX_CQ_ALIGN          16

// min of que capacity is 2 (mt5351 is 4)
#define GFX_CMQ_MIN_SIZE        2

#define GFX_CMD_MARGIN          2

#define GFX_ONE_CMD_SIZE        8   // one cmd = 8 bytes

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#if defined(CC_ENABLE_GRAPHIC_DUMP)
#define GFX_BMP_OFFSET(buffer, pitch, x, y, cm)                  \
    (buffer + pitch * y + _GFX_ColorModeTraslateByte(cm) * x)

#define GFX_FILE_OPEN(path, flags, mode)          FILE_Open(path, flags, mode)
#define GFX_FILE_CLOSE(file)                      FILE_Close(file)
#define GFX_FILE_WRITE(file, data, size, offset)  FILE_Write(file, data, size, offset)         
#define GFX_FILE_ACCESS(name, mode)               _GFX_AcessFile(name)

#define GFX_DUMPCHECK(fun)   \
    do                       \
    {                        \
         if((fun) < 0)         \
            return -1;       \
    }while(0)                \

#endif

#define GFX_DUMP_WIDTH      960
#define GFX_DUMP_HEIGHT     540

//---------------------------------------------------------------------------
// Structure definitions
//---------------------------------------------------------------------------
#if defined(CC_ENABLE_GRAPHIC_DUMP)

typedef struct file *  GFX_FILE_HANDLE;

typedef struct _GFX_COMMANDQUE_INFO_T_
{
     UINT32                   u4OP;
     
     UINT32                   pvSrc;
     UINT32                   u4SrcPitch;
     UINT32                   u4SrcWidth;
     UINT32                   u4SrcHeight;
     UINT32                   u4SrcCM;
     
     UINT32                   pvDst;
     UINT32                   u4DstPitch;
     UINT32                   u4DstWidth;
     UINT32                   u4DstHeight;
     UINT32                   u4DstCM;
}GFX_COMMANDQUE_INFO_T;

#endif
//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
#if defined(CC_ENABLE_GRAPHIC_DUMP)
INT32 GFX_DumpDebug(VOID);
INT32 GFX_DumpSrc(VOID);
INT32 GFX_DumpDst(VOID);
#endif
//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static INT32 _i4GfxCmdqConfig = (INT32) E_GFX_CMDQ_CFG_256KB;

static INT32 _i4GfxCmdqNewConfig = (INT32) E_GFX_CMDQ_CFG_256KB;


static volatile UINT64 *_pu8GfxCmdqueBuf = NULL;


static volatile GFX_CMDQUE_T _rGfxCmdQue;

static volatile GFX_CMDQUE_T *_prGfxCmdQue = NULL;

static BOOL _fgExtCmdQue   = FALSE;
static UINT32 _fgGfx_Dbg_Lvl = (UINT32)0x0;


#if defined(GFX_ENABLE_SW_MODE)
static volatile MI_DIF_UNION_T *_prRegBase = NULL;
#endif  //#if defined(GFX_ENABLE_SW_MODE)

#if defined(GFX_RISC_MODE)  // risc mode   
static INT32 _i4GfxCmdqueBufExist = (INT32)TRUE;
#else                       // cmdque mode
static INT32 _i4GfxCmdqueBufExist = (INT32) FALSE;
#endif

#ifdef __KERNEL__ 
static UINT32 _u4CmdQSize = 0;
#endif

#if defined(CC_ENABLE_GRAPHIC_DUMP)
static UINT32 _u4UserSetWidth;
static UINT32 _u4UserSetHeight;
static UINT32 _u4DumpWidth;
static UINT32 _u4DumpHeight;
static UINT32 _u4DumpPitch;
static UINT32 _u4DumpAddr;
static UINT32 _u4DumpBufferFlags = 0;
static UINT32 _u4DumpBufferOPFlags = 0;
static UINT32 _u4OsdCnt = 0;
static CHAR  _gDumpPath[100];
#endif

static UINT32 _au4_gfx_dbg_info[E_DFB_DBG_INFO_MAX] = {0x0,0x0,0x0,0x0};  
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GfxRecoverEngine
 *  Recover gfx engine when gfx engine crashed
 */
//-------------------------------------------------------------------------


UINT32 Gfx_Cmd_Get_Task_SrcAddr(void)
{
    return (UINT32)VIRTUAL((UINT32)_pu8GfxCmdqueBuf);
}

#if defined(GFX_ENABLE_SW_MODE)
//-------------------------------------------------------------------------
/** _GfxSwRealIsr
 *  gfx software interrupt routine
 */
//-------------------------------------------------------------------------
static void _GfxSwRealIsr(void)
{
    // unlock cmdque resource
    GFX_UnlockCmdque();
}



//-------------------------------------------------------------------------
/** _GfxSwQueAction
 *  start sw engine
 */
//-------------------------------------------------------------------------
static void _GfxSwQueAction(void)
{
    INT32 i4Element, i4CurIdx;
    UINT64 *pu8QueTop;
    UINT32 *pu4RegBase;
    UINT32 u4RegAddr, u4RegValue;

    pu4RegBase = (UINT32 *)(_prRegBase);

    i4Element = (INT32)(_prRegBase->rField.fg_DRAMQ_LEN >> 3);
    ASSERT(i4Element == _prGfxCmdQue->i4QueSize);

    pu8QueTop = (UINT64 *)_prGfxCmdQue->pu8QueTop;
    i4CurIdx = _prGfxCmdQue->i4ReadIndex;

    while (i4Element--)
    {
        u4RegAddr = (UINT32)((pu8QueTop[i4CurIdx] >> 34) & 0x3ff);
        u4RegValue = (UINT32)(pu8QueTop[i4CurIdx] & 0xffffffff);

        pu4RegBase[u4RegAddr] = u4RegValue;

        // only direct mode by now
        if ((u4RegAddr == (UINT32)GREG_G_MODE) &&
                ((u4RegValue & 0x00000800) != 0))
        {
            pfnGFX_DifAction();
        }

        i4CurIdx++; // cmdque current index

        // If current index >= bottom of cmdque then
        // set current index to top of cmdque.
        if (i4CurIdx >= E_GFX_CQ_QCAPACITY)
        {
            i4CurIdx = 0;
        }
    }

    _GfxSwRealIsr();
}
#endif // #if defined(GFX_ENABLE_SW_MODE)

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** GFX_CmdQueInit
 *  init gfx command queue
 *    allocate cmdq buffer
 *    get hw register address (sw mod)
 *    set command queue info
 *    reset command queue hw
 */
//-------------------------------------------------------------------------
void GFX_CmdQueInit(void)
{
    UINT32 u4Size  = 0;
    UINT32 u4Align = 0;

    _prGfxCmdQue = &_rGfxCmdQue;

    if (_i4GfxCmdqConfig == _i4GfxCmdqNewConfig)
    {
        // allocate cmdque buffer by means of default config
        if (_i4GfxCmdqueBufExist == (INT32) FALSE)
        {

            // calculate cmdque capacity
            GFX_CmdQueCalculateCapacity(_i4GfxCmdqConfig);

            u4Size = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
            u4Align = (UINT32)(E_GFX_CQ_ALIGN);

            // allocate a cmdque buffer for engine use
            _pu8GfxCmdqueBuf = (volatile UINT64 *) BSP_AllocAlignedDmaMemory(
                    u4Size, u4Align);
            VERIFY(_pu8GfxCmdqueBuf != NULL);
            
            Printf("GFX_CmdQueInit[0x%x,0x%x,0x%x]\n",(UINT32)_pu8GfxCmdqueBuf,u4Size,_i4GfxCmdqConfig);
            
#if defined(CC_ENABLE_GRAPHIC_DUMP)
            _u4DumpAddr = (UINT32)BSP_AllocAlignedDmaMemory(GFX_DUMP_WIDTH*GFX_DUMP_HEIGHT*2, 16);  
            VERIFY(_u4DumpAddr != 0);           
#endif
            _i4GfxCmdqueBufExist = (INT32) TRUE;
        }
    }
    else // re-allocate cmdque buffer
    {
        // calculate cmdque capacity by means of new config
        GFX_CmdQueCalculateCapacity(_i4GfxCmdqNewConfig);

        #ifdef __KERNEL__
        u4Size = (UINT32)GFX_CMDQ_BUF_SIZE;
        u4Align = (UINT32)(PAGE_SIZE);
        #else
        u4Size = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
        u4Align = (UINT32)(E_GFX_CQ_ALIGN);
        #endif
        
        // free cmdque buffer
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32) _pu8GfxCmdqueBuf) == TRUE);

        // allocate new cmdque buffer for engine use
        _pu8GfxCmdqueBuf = (volatile UINT64 *) BSP_AllocAlignedDmaMemory(
                u4Size, u4Align);
        VERIFY(_pu8GfxCmdqueBuf != NULL);
        
        Printf("GFX_CmdQueInit[0x%x,0x%x]\n",(UINT32)_pu8GfxCmdqueBuf,u4Size);
        
#if defined(CC_ENABLE_GRAPHIC_DUMP)
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32) _u4DumpAddr) == TRUE);
        _u4DumpAddr = (UINT32)BSP_AllocAlignedDmaMemory(GFX_DUMP_WIDTH*GFX_DUMP_HEIGHT*2, 16);
        VERIFY(_u4DumpAddr != 0);       
#endif
        _i4GfxCmdqConfig = _i4GfxCmdqNewConfig;
    }

#if defined(GFX_ENABLE_SW_MODE)
    //-----------------------------------------------------------
    // sw mode
    if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        UINT32 u4GfxRegBase;

        GFX_DifGetRegBase(&u4GfxRegBase);

        _prRegBase = (volatile MI_DIF_UNION_T *)u4GfxRegBase;
    }
    //-----------------------------------------------------------
#endif  //#if defined(GFX_ENABLE_SW_MODE)

    // setup cmdque info
    GFX_CmdQueSetup();

    GFX_CmdQueReset();

#ifdef __KERNEL__
    if (u4Size != 0)
    {
        _u4CmdQSize = u4Size;
    }
    else
    {
        _u4CmdQSize = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
    }
#endif

#if defined(CC_ENABLE_GRAPHIC_DUMP)
    strcpy(_gDumpPath, "/mnt/usb/sda1");
    _u4UserSetWidth = GFX_DUMP_WIDTH;
    _u4UserSetHeight = GFX_DUMP_HEIGHT;

#endif

}

//-------------------------------------------------------------------------
/** GFX_CmdQueReset
 *  reset cmdque hw
 */
//-------------------------------------------------------------------------
void GFX_CmdQueReset(void)
{
    // hw mode setting
    if ((UINT32) E_GFX_HW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        volatile UINT32 u4Value;

#if defined(GFX_RISC_MODE)

        u4Value = GFX_READ32(GFX_REG_G_CONFIG);
        // (1) disable EN_DRAMQ flag
        // _prRegBase->rField.fg_EN_DRAMQ = GFX_DISABLE;
        u4Value = (u4Value) & (~GREG_MSK_EN_DRAMQ);
        GFX_WRITE32(GFX_REG_G_CONFIG, u4Value);

#else
        u4Value = GFX_READ32(GFX_REG_G_CONFIG);
        // (1) enable EN_DRAMQ flag
        // _prRegBase->rField.fg_EN_DRAMQ = GFX_ENABLE;
        u4Value = (u4Value) | (GREG_MSK_EN_DRAMQ);

        // (2) set DRAMQ_MODE to cyclic buffer mode
        // _prRegBase->rField.fg_DRAMQ_MODE = GFX_CMD_BUF_CYLIC;

        u4Value = (u4Value) & (~GREG_MSK_DRAMQ_MODE);

        // (3) enable/disable SHT_CMDQ flag
        // _prRegBase->rField.fg_SHT_CMDQ = _rGfxCmdQue.i4ShortCmdque;
        u4Value = (u4Value) | ((UINT32) _prGfxCmdQue->i4ShortQue
                << GREG_SHT_SHT_CMDQ);

        if(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU)
        {
            u4Value = (u4Value | GREG_MSK_DRAMQ_MODE);
        }
        GFX_WRITE32(GFX_REG_G_CONFIG, u4Value);

        // (4) set CYC_SIZE and DRAMQ_BSAD
        // _prRegBase->rField.fg_CYC_SIZE   = (UINT32)_rGfxCmdQue.i4QueConfig;
        // _prRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_rGfxCmdQue.pu8QueTop;
        // Hw must use PHYSICAL address
        
        u4Value = ((UINT32) _prGfxCmdQue->i4QueConfig << GREG_SHT_CYC_SIZE)
                | PHYSICAL(((UINT32) _prGfxCmdQue->pu8QueTop));
        
        GFX_WRITE32(GFX_REG_DRAMQ_STAD, u4Value);

        // power saving mode (need to measure HW)
        //_prRegBase->rField.fg_ENG_LP = GFX_ENABLE;
#endif // #if defined(GFX_RISC_MODE)
    }

#if defined(GFX_ENABLE_SW_MODE)
    //-----------------------------------------------------------
    // sw mode setting
    if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
    {
#if defined(GFX_RISC_MODE)

        _prRegBase->rField.fg_EN_DRAMQ = GFX_DISABLE;

#else

        _prRegBase->rField.fg_EN_DRAMQ = GFX_ENABLE;
        _prRegBase->rField.fg_DRAMQ_MODE = GFX_CMD_BUF_CYLIC;
        _prRegBase->rField.fg_SHT_CMDQ = (UINT32)_prGfxCmdQue->i4ShortQue;

        _prRegBase->rField.fg_CYC_SIZE = (UINT32)_prGfxCmdQue->i4QueConfig;
        // Sw must use VIRTUAL address
        _prRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_prGfxCmdQue->pu8QueTop;

        // power saving mode (need to measure HW)
        //_prRegBase->rField.fg_ENG_LP = GFX_ENABLE;

#endif // #if defined(GFX_RISC_MODE)
    }
    //-----------------------------------------------------------
#endif  //#if defined(GFX_ENABLE_SW_MODE)
}

void GFX_Set_DbgLvl(UINT32 u4Value)
{
    _fgGfx_Dbg_Lvl=u4Value;
    return;
}

UINT32 GFX_Get_DbgLvl(void)
{
    return _fgGfx_Dbg_Lvl;
}


void GFX_CmdQueDump(void)
{
    INT32 i4ReadIndex = _prGfxCmdQue->i4ReadIndex;
    INT32 i4QueSize   = _prGfxCmdQue->i4QueSize;
    UINT32 u4Reg      = 0x0;
    UINT32 u4Val      = 0x0;
    UINT32 u4Temp     = 0x0;
    
    Printf("GFX_CmdQueDump[size = %d]:\n",i4QueSize);

    for(u4Temp =0;u4Temp<i4QueSize;u4Temp++)
    {
        if(u4Temp && (0 == (u4Temp%2)))
        {
            Printf("\n");
        }
        
        u4Reg = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]>>32);
        u4Val = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]&0xffffffff);

        Printf("[reg= 0x%-8x, val= 0x%-8x] ",u4Reg,u4Val);

        i4ReadIndex = ((i4ReadIndex+1)%(E_GFX_CQ_QCAPACITY));
    }
    
    Printf("\n");

    return;
}

void GFX_CmdPreQueDump(void)
{
    INT32 i4ReadIndex = _prGfxCmdQue->i4PrevIndex;
    INT32 i4QueSize   = _prGfxCmdQue->i4PreQueSize;
    UINT32 u4Reg      = 0x0;
    UINT32 u4Val      = 0x0;
    UINT32 u4Temp     = 0x0;
    
    printf("GFX_CmdPreQueDump[size = %d]:\n",i4QueSize);

    for(u4Temp =0;u4Temp<i4QueSize;u4Temp++)
    {
        if(u4Temp && (0 == (u4Temp%2)))
        {
            printf("\n");
        }
        
        u4Reg = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]>>32);
        u4Val = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]&0xffffffff);

        printf("[reg= 0x%-8x, val= 0x%-8x] ",u4Reg,u4Val);

        i4ReadIndex = ((i4ReadIndex+1)%(E_GFX_CQ_QCAPACITY));
    }
    
    printf("\n");
    
    return;
}

void GFX_CmdQ_Check(void)
{
    if (_prGfxCmdQue->i4QueSize < GFX_CMQ_MIN_SIZE)
    {
        INT32 i4Count;
        INT32 i4Times;

        i4Times = ((INT32) GFX_CMQ_MIN_SIZE - _prGfxCmdQue->i4QueSize);

        for (i4Count = 0; i4Count < i4Times; i4Count++)
        {
            GFX_CmdQuePushBack((INT32) GREG_DUMMY, 0);
        }
    }

    // # of command must be even
    if (_prGfxCmdQue->i4QueSize & 1)
    {
        GFX_CmdQuePushBack((INT32)(GREG_DUMMY), 0);
    }

    return;
}

void GFX_CmdQ_Action_Sync(void)
{
    volatile UINT32 u4CmdQueLen;

    GFX_CmdQ_Check();

    // hw mode
    if ((UINT32) E_GFX_HW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        // lock cmdque resource
        GFX_LockCmdque();

        u4CmdQueLen = (UINT32)(_prGfxCmdQue->i4QueSize * GFX_ONE_CMD_SIZE);

#ifdef __KERNEL__
        {
            if( (_prGfxCmdQue->i4ReadIndex + u4CmdQueLen) <= E_GFX_CQ_QCAPACITY)
            {
                HalFlushInvalidateDCacheMultipleLine(
                        VIRTUAL((UINT32)&_pu8GfxCmdqueBuf[_prGfxCmdQue->i4ReadIndex]),
                        (UINT32)u4CmdQueLen);
            }
            else
            {
                HalFlushInvalidateDCacheMultipleLine(
                        VIRTUAL((UINT32)_pu8GfxCmdqueBuf), _u4CmdQSize);
            }
        }
#else
        HalFlushInvalidateDCache();
#endif

#if defined(CC_ENABLE_GRAPHIC_DUMP)
        if(_u4DumpBufferFlags & 0x1)
        {
            GFX_DumpSrc();
        }       
#endif      

        if(GFX_DBG_LVL(GFX_DBG_CMDQ))
        {
            GFX_CmdQueDump();
        }

        // in hw mode, gfx engine will run if DRAMQ_LEN is written.
        // 1 CMD size = 8 bytes
        GFX_WRITE32(GFX_REG_DRAMQ_LEN, u4CmdQueLen);

        GFX_Wait();
#if defined(CC_ENABLE_GRAPHIC_DUMP)
        if(_u4DumpBufferFlags & 0x2)
        {
            GFX_DumpDst();
        }       
#endif
        _prGfxCmdQue->i4PrevIndex = _prGfxCmdQue->i4ReadIndex;
        _prGfxCmdQue->i4ReadIndex = _prGfxCmdQue->i4WriteIndex;
        _prGfxCmdQue->i4PreQueSize= _prGfxCmdQue->i4QueSize;
        
        _prGfxCmdQue->i4QueSize = 0;
    }
    else
    {
#if defined(GFX_ENABLE_SW_MODE)
        // sw mode
        if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
        {
            // lock cmdque resource
            GFX_LockCmdque();
            HalFlushInvalidateDCache();

            // in sw mode, just record cmdque len for _GfxSwQueAction() use
            // 1 CMD size = 8 bytes
            u4CmdQueLen = (UINT32)(_prGfxCmdQue->i4QueSize * GFX_ONE_CMD_SIZE);
            _prRegBase->rField.fg_DRAMQ_LEN = u4CmdQueLen;

            _GfxSwQueAction();

            _prGfxCmdQue->i4PrevIndex = _prGfxCmdQue->i4ReadIndex;
            _prGfxCmdQue->i4ReadIndex = _prGfxCmdQue->i4WriteIndex;
            _prGfxCmdQue->i4QueSize = 0;

            GFX_HwISR(); // notify MW
        }
#endif  //#if defined(GFX_ENABLE_SW_MODE)
    }
}

UINT64 GFX_CmdQ_Action_Async(UINT32 u4_type)
{
    UINT32 u4CmdQueLen;
    UINT64 u8_id = 0x0;
    
    GFX_CmdQ_Check();

    u4CmdQueLen = (UINT32)(_prGfxCmdQue->i4QueSize * GFX_ONE_CMD_SIZE);

    #ifdef __KERNEL__ 
    if( (_prGfxCmdQue->i4ReadIndex + u4CmdQueLen) <= E_GFX_CQ_QCAPACITY)
    {
        HalFlushDCacheMultipleLine(
            VIRTUAL((UINT32)&_pu8GfxCmdqueBuf[_prGfxCmdQue->i4ReadIndex]),u4CmdQueLen);
    }
    else
    {
        HalFlushDCacheMultipleLine(
            VIRTUAL((UINT32)_pu8GfxCmdqueBuf),_u4CmdQSize);
    }
    #endif
    
    GFX_Task_List_Dcache_Add(E_GFX_TASK_DCACHE_INVALID);
    
    u8_id = GFX_Task_Send((void *)VIRTUAL((UINT32)&_pu8GfxCmdqueBuf[_prGfxCmdQue->i4ReadIndex]),u4CmdQueLen,u4_type);
    
    _prGfxCmdQue->i4PrevIndex = _prGfxCmdQue->i4ReadIndex;
    _prGfxCmdQue->i4ReadIndex = _prGfxCmdQue->i4WriteIndex;
    _prGfxCmdQue->i4PreQueSize= _prGfxCmdQue->i4QueSize;
    _prGfxCmdQue->i4QueSize = 0;

    return u8_id;
}


//-------------------------------------------------------------------------
/** GFX_CmdQueAction
 *  start gfx hardware command queue
 */
//-------------------------------------------------------------------------
UINT64 GFX_CmdQueAction(void)
{
    UINT64 u8_id = 0x0;
    
    if(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU)
    {
        u8_id = GFX_CmdQ_Action_Async(E_GFX_TASK_CMDQ);
    }
    else
    {
        GFX_CmdQ_Action_Sync();
    }

    return u8_id;
}

//-------------------------------------------------------------------------
/** GFX_RiscPushBack (Risc Mode)
 *  push back one register value into gfx register
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void GFX_RiscPushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;

    u4RegOffset = ((u4Reg << 2) & 0xfff);

    if (u4RegOffset == (UINT32)GFX_REG_G_MODE)
    {
        // lock cmdque resource
        GFX_LockCmdque();
        HalFlushInvalidateDCache();
        // write a cmd to the hw register (flush)
        GFX_WRITE32(u4RegOffset, u4Val);
    }
    else
    {
        // lock cmdque resource
        GFX_LockCmdque();
        // write a cmd to the hw register
        GFX_WRITE32(u4RegOffset, u4Val);
        // unlock cmdque resource
        GFX_UnlockCmdque();
    }
}


//-------------------------------------------------------------------------
/** GFX_CmdQuePushBack (Cmdque Mode)
 *  push back one register value into cmdque
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void GFX_CmdQuePushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;

    u4RegOffset = ((u4Reg << 2) & 0xfff);

    if ((_prGfxCmdQue->i4QueSize + GFX_CMD_MARGIN)
            >= _prGfxCmdQue->i4QueCapacity)
    {
        GFX_CmdQueAction();
    }

    // lock cmdque resource
    if(!(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU))
    {
        GFX_LockCmdque();
    }

    // calculate GFX register address
    u4Reg = (GFX_ADDR) | (u4RegOffset);

    // write one cmd to cmdque buffer
    _prGfxCmdQue->i4QueSize++;
    _prGfxCmdQue->pu8QueTop[_prGfxCmdQue->i4WriteIndex++] = ((((UINT64) u4Reg)
            << 32) | u4Val);

    if (_prGfxCmdQue->i4WriteIndex >= E_GFX_CQ_QCAPACITY)
    {
        _prGfxCmdQue->i4WriteIndex = 0;
    }

    // unlock cmdque resource
    if(!(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU))
    {
        GFX_UnlockCmdque();
    }
}

void GFX_MMU_RiscPushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;

    u4RegOffset = ((u4Reg << 2) & 0xfff);

    // lock cmdque resource
    GFX_LockCmdque();

    // write a cmd to the hw register
    GFX_MMU_WRITE32(u4RegOffset, u4Val);
    //Printf( "(%x, %x)\n", u4RegOffset, u4Val);
    // unlock cmdque resource
    GFX_UnlockCmdque();
}

//-------------------------------------------------------------------------
/** GFX_CmdQuePushBack (Cmdque Mode)
 *  push back one register value into cmdque
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void GFX_MMU_CmdQuePushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;

    u4RegOffset = ((u4Reg << 2) & 0xfff);

    if ((_prGfxCmdQue->i4QueSize + GFX_CMD_MARGIN)
            >= _prGfxCmdQue->i4QueCapacity)
    {
        GFX_CmdQueAction();
    }
    
    if(!(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU))
    {
        GFX_LockCmdque();
    }

    // calculate GFX register address
    //u4Reg = (VGFX_ADDR) | (u4RegOffset);
    u4Reg = (GFX_MMU_CMDQUE_ADDR) | (u4RegOffset);

    // write one cmd to cmdque buffer
    _prGfxCmdQue->i4QueSize++;
    _prGfxCmdQue->pu8QueTop[_prGfxCmdQue->i4WriteIndex++] = ((((UINT64) u4Reg)
            << 32) | u4Val);

    if (_prGfxCmdQue->i4WriteIndex >= E_GFX_CQ_QCAPACITY)
    {

        _prGfxCmdQue->i4WriteIndex = 0;
    }

    // unlock cmdque resource
    if(!(GFX_TASK_IS_INITED&GFX_TASK_SUPPORT_MMU))
    {
        GFX_UnlockCmdque();
    }
}

//-------------------------------------------------------------------------
/** GFX_CmdQueCalculateCapacity
 *  calculate cmdque capacity
 *  and set queconfig
 */
//-------------------------------------------------------------------------
void GFX_CmdQueCalculateCapacity(INT32 i4CmdqConfig)
{
    INT32 i;
    INT32 i4QueCapacity = 0;

    if (i4CmdqConfig > (INT32) E_GFX_CMDQ_CFG_16KB)
    {
        // disable short cmdque
        _prGfxCmdQue->i4ShortQue = GFX_DISABLE;
        _prGfxCmdQue->i4QueConfig = i4CmdqConfig - (INT32) E_GFX_CMDQ_CFG_32KB;
    }
    else
    {
        // enable short cmdque
        _prGfxCmdQue->i4ShortQue = GFX_ENABLE;
        _prGfxCmdQue->i4QueConfig = i4CmdqConfig;
    }

    // calculate cmdque capacity
    i4QueCapacity = (INT32) E_GFX_CMDQ_CAP_2KB;
    for (i = 0; i < i4CmdqConfig; i++)
    {
        i4QueCapacity = (i4QueCapacity * 2);
    } // ~for

    _prGfxCmdQue->i4QueCapacity = i4QueCapacity;
}

//-------------------------------------------------------------------------
/** GFX_CmdQueSetup
 *  setup command queue info includes
 *  que occupied size, que top, read/write index
 *
 */
//-------------------------------------------------------------------------
void GFX_CmdQueSetup(void)
{
    _prGfxCmdQue->i4QueSize = 0;
    _prGfxCmdQue->i4PrevIndex = 0;
    _prGfxCmdQue->i4ReadIndex = 0;
    _prGfxCmdQue->i4WriteIndex = 0;
    _prGfxCmdQue->i4QueFlushCount = 0;
    _prGfxCmdQue->i4QueIntCount = 0;
    _prGfxCmdQue->pu8QueTop = (volatile UINT64 *) VIRTUAL(
            (UINT32) _pu8GfxCmdqueBuf);
    _prGfxCmdQue->prNext = (struct _GFX_CMDQUE_T *) NULL;
}

void GFX_CmdQueSetExtBuf(volatile GFX_CMDQUE_T *prGfxCmdQue,
        volatile UINT64 *pu8CmdQueBuf, UINT32 u4Config)
{
    if (_prGfxCmdQue->i4QueSize > 0)
    {
        GFX_CmdQueAction();
    }

    GFX_LockCmdque();

    _fgExtCmdQue = TRUE;

    UNUSED(_fgExtCmdQue);

    _GFX_Reset();

    _pu8GfxCmdqueBuf = pu8CmdQueBuf;

    _prGfxCmdQue = prGfxCmdQue;

    if (u4Config < E_GFX_CMDQ_CFG_MAX)
    {
        _i4GfxCmdqConfig = u4Config;
    }
    GFX_CmdQueCalculateCapacity(_i4GfxCmdqConfig);

    #ifdef __KERNEL__
    _u4CmdQSize = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
    #endif

    _i4GfxCmdqueBufExist = (INT32) TRUE;

    // setup cmdque info
    GFX_CmdQueSetup();

    GFX_CmdQueReset();

    GFX_UnlockCmdque();
}

void gfx_dfb_set_dbg_info(UINT32 u4_type,UINT32 u4_val)
{
    if(u4_type >= E_DFB_DBG_INFO_MAX)
    {
        return;
    }
    _au4_gfx_dbg_info[u4_type]=u4_val;
    return;
}
UINT32 gfx_dfb_get_dbg_info(UINT32 u4_type)
{
    if(u4_type >= E_DFB_DBG_INFO_MAX)
    {
        return 0x0;
    }
    return _au4_gfx_dbg_info[u4_type];
}
#if defined(CC_ENABLE_GRAPHIC_DUMP)

INT32 GFX_CmdQueParse(GFX_COMMANDQUE_INFO_T *ptCmdInfo)
{
    INT32 i4ReadIndex = _prGfxCmdQue->i4ReadIndex;
    INT32 i4QueSize   = _prGfxCmdQue->i4QueSize;
    UINT32 u4Reg      = 0x0;
    UINT32 u4Val      = 0x0;
    UINT32 u4Temp     = 0x0;
    UINT32 u4SrcBase  = 0x0;
    UINT32 u4DstBase  = 0x0;

   // Printf("GFX_CmdQueDump[size = %d]:\n",i4QueSize);

    for(u4Temp =0;u4Temp<i4QueSize;u4Temp++)
    {
       // if(u4Temp && (0 == (u4Temp%2)))
       // {
           // Printf("\n");
       // }
        
        u4Reg = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]>>32);
        u4Val = (UINT32)(_prGfxCmdQue->pu8QueTop[i4ReadIndex]&0xffffffff);

       // Printf("[reg= 0x%-8x, val= 0x%-8x] ",u4Reg,u4Val);

        switch(u4Reg)
        {
        case ((GFX_ADDR) | 0x4010):
            ptCmdInfo->u4OP    = (u4Val & 0x1f0) >> 4;
            ptCmdInfo->u4DstCM = (u4Val & 0xf);
        break;
        case ((GFX_ADDR) | 0x4018):
            u4SrcBase &= 0xf0000000;
            u4SrcBase |= (u4Val & 0x3fffffff);
        break;
        case ((GFX_ADDR) | 0x401C):
            u4DstBase &= 0xf0000000;
            u4DstBase |= (u4Val & 0x3fffffff);
        break;
        case ((GFX_ADDR) | 0x4028):
            ptCmdInfo->u4SrcCM = (u4Val & 0x78000000) >> 27;
            ptCmdInfo->u4SrcWidth = (u4Val & 0x7fff);
            ptCmdInfo->u4SrcHeight = ((u4Val & 0x7ff0000) >> 16)  + 1;
        break;  
        case ((GFX_ADDR) | 0x402C):
            ptCmdInfo->u4SrcPitch = (u4Val & 0xffff0000) >> 12;
            ptCmdInfo->u4DstPitch = (u4Val & 0xffff) << 4;
        break;
        case ((GFX_ADDR) | 0x408C):
            ptCmdInfo->u4DstWidth = (u4Val & 0x7fff);
            ptCmdInfo->u4DstHeight = ((u4Val & 0x7ff0000) >> 16)  + 1;
        break;
        case ((GFX_ADDR) | 0x4118):
            u4SrcBase &= 0x3fffffff;
            u4DstBase &= 0x3fffffff;
            u4SrcBase |= (u4Val & 0xC0) << 24;
            u4DstBase |= (u4Val & 0x300) << 22; 
        break;
        default:
        break;  
        }
        i4ReadIndex = ((i4ReadIndex+1)%(E_GFX_CQ_QCAPACITY));
    }

    ptCmdInfo->pvSrc = u4SrcBase;
    ptCmdInfo->pvDst = u4DstBase;
/*
    Printf("u4OP=0x%x, "
        "pvSrc=0x%p, u4SrcPitch=%d, u4SrcWidth=%d, u4SrcHeight=%d, u4SrcCM=0x%x, "
        "pvDst=0x%p, u4DstPitch=%d, u4DstWidth=%d, u4DstHeight=%d, u4DstCM=0x%x\n",
        ptCmdInfo->u4OP,
        ptCmdInfo->pvSrc,ptCmdInfo->u4SrcPitch,ptCmdInfo->u4SrcWidth,ptCmdInfo->u4SrcHeight,ptCmdInfo->u4SrcCM,
        ptCmdInfo->pvDst,ptCmdInfo->u4DstPitch,ptCmdInfo->u4DstWidth,ptCmdInfo->u4DstHeight,ptCmdInfo->u4DstCM);
*/
    if(OP_BITBLT == ptCmdInfo->u4OP
        || OP_ALPHA_BLEND == ptCmdInfo->u4OP
        || OP_ALPHA_COMPOSE == ptCmdInfo->u4OP
        || OP_YCBCR_TO_RGB == ptCmdInfo->u4OP
        || OP_ALPHA_COMPOSE_LOOP == ptCmdInfo->u4OP)
    {
         ptCmdInfo->u4DstWidth  = ptCmdInfo->u4SrcWidth;
         ptCmdInfo->u4DstHeight = ptCmdInfo->u4SrcHeight;
    }
    else if(OP_RECT_FILL == ptCmdInfo->u4OP)
    {
/*
    Printf("u4OP=0x%x, "
        "pvSrc=0x%p, u4SrcPitch=%d, u4SrcWidth=%d, u4SrcHeight=%d, u4SrcCM=0x%x, "
        "pvDst=0x%p, u4DstPitch=%d, u4DstWidth=%d, u4DstHeight=%d, u4DstCM=0x%x\n",
        ptCmdInfo->u4OP,
        ptCmdInfo->pvSrc,ptCmdInfo->u4SrcPitch,ptCmdInfo->u4SrcWidth,ptCmdInfo->u4SrcHeight,ptCmdInfo->u4SrcCM,
        ptCmdInfo->pvDst,ptCmdInfo->u4DstPitch,ptCmdInfo->u4DstWidth,ptCmdInfo->u4DstHeight,ptCmdInfo->u4DstCM);
*/  
         ptCmdInfo->u4DstWidth  = ptCmdInfo->u4SrcWidth;
         ptCmdInfo->u4DstHeight = ptCmdInfo->u4SrcHeight;
         ptCmdInfo->u4SrcPitch  = ptCmdInfo->u4DstPitch;
         ptCmdInfo->pvSrc       = ptCmdInfo->pvDst;
         ptCmdInfo->u4SrcCM     = ptCmdInfo->u4DstCM;
    }

    if(ptCmdInfo->pvSrc & 0xF || ptCmdInfo->pvDst & 0xF)
    {
         return -1;
    }

    if(_u4DumpBufferOPFlags & (1 << ptCmdInfo->u4OP))
    {
         return 0;
    }
    
   // Printf("\n");

    return -1;   
}


static UINT32
_GFX_ColorModeTraslateByte(GFX_COLOR_MODE_T eColorMode)
{
    if(eColorMode == CM_RGB565_DIRECT16) 
    {
         return 2;
    }
    else if(eColorMode == CM_ARGB4444_DIRECT16)
    {
         return 2;
    }
    else
    {
         return 4;
    }
}


static INT32 _GFX_AcessFile(const char *file)
{
     GFX_FILE_HANDLE     fd           = NULL;

     fd = GFX_FILE_OPEN(file, O_RDONLY, 0644);

     if(fd <= 0)
     {
          return -1;
     }

     GFX_FILE_CLOSE(fd);

     return 0;
}


static INT32 _GFX_GenerateFilePath(const char *prefix, UINT32 u4Addr, UINT32 u4Op, 
                                 UINT32 u4Width, UINT32 u4Height, UINT32 u4Pitch,
                                 UINT32 u4Cm, UINT32 u4Length, char *retFilePath)
{
     UINT32 u4Num = 0;
     static UINT32 u4Count = 0;

     snprintf( retFilePath, u4Length, "%s/%s_%p_%04d", _gDumpPath, prefix, u4Addr, u4Num);

     if (prefix) 
     {
          /* Find the lowest unused index. */
          while (++u4Num < 10000) 
          {
               snprintf( retFilePath, u4Length, "%s/%s_%p_op-%d_wxh-%dx%d_pitch-%d_cm-%d_%04d.bmp",
                               _gDumpPath, prefix, u4Addr, u4Op, u4Width, u4Height, u4Pitch, u4Cm, u4Num);

               if (GFX_FILE_ACCESS( retFilePath, F_OK ) != 0) 
               {
/*             
                   snprintf( retFilePath, u4Length, "%s/%s_%p_%d_%d_%d_%04d.ppm",
                               _gDumpPath, prefix, u4Addr, u4Width, u4Height, u4Pitch, u4Num);

                    if (GFX_FILE_ACCESS( retFilePath, F_OK ) != 0)
*/                    
                         break;
               }

          }

          if (u4Num == 10000) 
          {
               return -1;
          }
     }

     memset(retFilePath, 0, u4Length);
     if(strstr(prefix, "OSD"))
     {
          snprintf( retFilePath, u4Length, "%s/%s_%p_op-%d_wxh-%dx%d_pitch-%d_cm-%d_%04d",
                         _gDumpPath, prefix, u4Addr, u4Op, u4Width, u4Height, u4Pitch, u4Cm, u4Num);      
     }
     else
     {
          snprintf( retFilePath, u4Length, "%s/%d_%s_%p_op-%d_wxh-%dx%d_pitch-%d_cm-%d_%04d",
                         _gDumpPath, u4Count++, prefix, u4Addr, u4Op, u4Width, u4Height, u4Pitch, u4Cm, u4Num); 
     }
     return 0;
}


INT32 GFX_DumpBuffer(const char *path, UINT32 u4Buffer,
           UINT32 u4ColorMode, UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height)
{
     BOOL                bRGB           = FALSE;
     BOOL                bAlpha         = FALSE;
     CHAR                rgbFile[60];
     CHAR                alphaFile[60];
     CHAR                head[30];
     UINT8               buf_p[u4Width * 3];
     UINT8               buf_g[u4Width];
     UINT64              u4ROffset      = 0;
     UINT64              u4AOffset      = 0;
     UINT32              u4Src          = 0;
     UINT8               *pu1Src        = NULL;
     UINT16              *pu2Src        = NULL;
     UINT32              *pu4Src        = NULL;  
     GFX_COLOR_MODE_T    eColorMode     = u4ColorMode;
     GFX_FILE_HANDLE     fd_p           = NULL;
     GFX_FILE_HANDLE     fd_g           = NULL;

     INT32               i, n, n3;

     switch(eColorMode)
     {
          case CM_ARGB4444_DIRECT16:
          case CM_ARGB8888_DIRECT32:
               bRGB = bAlpha = TRUE;
          break;
          default:
          return -1;    
     }

     if(bRGB)
     {
          sprintf(rgbFile, "%s.ppm", path);
          fd_p = GFX_FILE_OPEN((const char *) rgbFile,  O_EXCL | O_CREAT | O_WRONLY, 0644);
          if(fd_p <= 0)
          {
               Printf("open %s failed \n", rgbFile);
               return -1;
          }
          /* Write the pixmap header. */
          snprintf( head, 30,
                    "P6\n%d %d\n255\n", u4Width, u4Height );

          GFX_FILE_WRITE(fd_p, head, strlen(head), &u4ROffset);
          
     }

     if(bAlpha)
     {
          sprintf(alphaFile, "%s.pgm", path); 
          fd_g = GFX_FILE_OPEN((const char *) alphaFile,  O_EXCL | O_CREAT | O_WRONLY, 0644);
          if(fd_g <= 0)
          {
               GFX_FILE_CLOSE(fd_p);
               return -1;
          }
          snprintf( head, 30,
                    "P5\n%d %d\n255\n", u4Width, u4Height );
          GFX_FILE_WRITE(fd_g, head, strlen(head), &u4AOffset);
     }
     /* Write the pixmap (and graymap) data. */
     for (i = 0; i < u4Height; i++)
     {
          u4Src   = GFX_BMP_OFFSET(u4Buffer, u4Pitch, 0, i, eColorMode);
          pu1Src  = (UINT8 *) u4Src;
          pu2Src  = (UINT16 *)u4Src;
          pu4Src  = (UINT32 *)u4Src;
          /* Write color buffer to pixmap file. */
          if(bRGB)
          {
               if (eColorMode == CM_ARGB4444_DIRECT16)
               {
                    for (n = 0, n3 = 0; n < u4Width; n++, n3 += 3)
                    {
                         buf_p[n3 + 0] = (pu2Src[n] & 0x0F00) >> 4;
                         buf_p[n3 + 1] = (pu2Src[n] & 0x00F0);
                         buf_p[n3 + 2] = (pu2Src[n] & 0x000F) << 4;
                    }
               }
               else if (eColorMode == CM_ARGB8888_DIRECT32)
               {
                    for (n = 0, n3 = 0; n < u4Width; n++, n3 += 3)
                    {
                         buf_p[n3 + 0] = (pu4Src[n] & 0x0000FF);
                         buf_p[n3 + 1] = (pu4Src[n] & 0x00FF00) >>  8;
                         buf_p[n3 + 2] = (pu4Src[n] & 0xFF0000) >> 16;                  
                    }              
               }

               GFX_FILE_WRITE( fd_p, buf_p, u4Width * 3, &u4ROffset);
          }

          if(bAlpha)
          {
               if (eColorMode == CM_ARGB4444_DIRECT16)
               {
                    for (n = 0; n < u4Width; n++)
                    {
                         buf_g[n]      = (pu2Src[n] >> 12);
                         buf_g[n]     |=  buf_g[n] << 4;
                    }
               }
               else if (eColorMode == CM_ARGB8888_DIRECT32)
               {
                    for (n = 0; n < u4Width; n++)
                    {
                         buf_g[n]      = pu4Src[n] >> 24;                 
                    }              
               }

               GFX_FILE_WRITE( fd_g, buf_g, u4Width, &u4AOffset);
          }
     }

     if(bRGB)
     {
          GFX_FILE_CLOSE(fd_p);
     }

     if(bAlpha)
     {
          GFX_FILE_CLOSE(fd_g);
     }
     
     return 0;
}


INT32 GFX_DumpBmp(const char *path, UINT32 u4Buffer,
           UINT32 u4ColorMode, UINT32 u4Pitch, UINT32 u4Width, UINT32 u4Height)
{
    UINT64              u4Offset       = 0;
    UINT32              u4Src          = 0;
    UINT8               *pu1Src        = NULL;
    UINT16              *pu2Src        = NULL;
    UINT32              *pu4Src        = NULL;
    UINT32              u4FileSize     = 0;
    UINT32              num            = 0;
    INT32               i              = 0;
    GFX_COLOR_MODE_T    eColorMode     = u4ColorMode;
    GFX_FILE_HANDLE     fd             = NULL;
    
    char filename[120]; 

    UINT8 header[54]   = {
          0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
          54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 32, 0,
          0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          0, 0, 0, 0
    };

    UINT8  buf_p[u4Width * 4];

     u4FileSize = u4Width * u4Height * 4 + 54;
     header[2]  = (UINT8)(u4FileSize & 0x000000ff);
     header[3]  = (UINT8)((u4FileSize >> 8) & 0x000000ff);
     header[4]  = (UINT8)((u4FileSize >> 16)& 0x000000ff);
     header[5]  = (UINT8)((u4FileSize >> 24)& 0x000000ff);

     header[18] = (UINT8)(u4Width & 0x000000ff);
     header[19] = (UINT8)((u4Width >> 8) & 0x000000ff);
     header[20] = (UINT8)((u4Width >> 16)& 0x000000ff);
     header[21] = (UINT8)((u4Width >> 24)& 0x000000ff);  

     header[22] = (UINT8)(u4Height & 0x000000ff);
     header[23] = (UINT8)((u4Height >> 8) & 0x000000ff);
     header[24] = (UINT8)((u4Height >> 16)& 0x000000ff);
     header[25] = (UINT8)((u4Height >> 24)& 0x000000ff);
     
     /* Find the lowest unused index. */
     while (++num < 10000) 
     {
         snprintf( filename, 120, "%s_%d.bmp", path, (int)num);
    
         if (GFX_FILE_ACCESS( filename, F_OK ) != 0) 
         {
              break;
         }
         if (num == 10000) 
         {
              return -1;
         }
     }   
     
     fd = GFX_FILE_OPEN( filename, O_EXCL | O_CREAT | O_WRONLY, 0644 ); 
     if (fd <= 0)
     {
          return -1;
     }
 
     GFX_FILE_WRITE( fd, header, 54, &u4Offset);
 
     for (i = (u4Height -1); i >= 0; i--)
     {
          int n, n4;
          /* Prepare one row. */
          u4Src   = GFX_BMP_OFFSET(u4Buffer, u4Pitch, 0, i, eColorMode);
          pu1Src  = (UINT8 *) u4Src;
          pu2Src  = (UINT16 *)u4Src;
          pu4Src  = (UINT32 *)u4Src;

          if (eColorMode == CM_ARGB4444_DIRECT16)
          {
               for (n = 0, n4 = 0; n < u4Width; n++, n4 += 4)
               {
                    buf_p[n4 + 3]  = (pu2Src[n] >> 12);
                    buf_p[n4 + 3] |=  buf_p[n4 + 3] << 4;               
                    buf_p[n4 + 2]  = (pu2Src[n] & 0x0F00) >> 4;
                    buf_p[n4 + 1]  = (pu2Src[n] & 0x00F0);
                    buf_p[n4 + 0]  = (pu2Src[n] & 0x000F) << 4;
               }          
          }
          else if (eColorMode == CM_ARGB8888_DIRECT32)
          {
               for (n = 0, n4 = 0; n < u4Width; n++, n4 += 4)
               {
                    buf_p[n4 + 3] = pu4Src[n] >> 24;
                    buf_p[n4 + 2] = (pu4Src[n] & 0xFF0000) >> 16;
                    buf_p[n4 + 1] = (pu4Src[n] & 0x00FF00) >> 8;
                    buf_p[n4 + 0] = (pu4Src[n] & 0x0000FF);                                     
               }          
          }
          
          GFX_FILE_WRITE( fd, buf_p, u4Width * 4,  &u4Offset);  
     }
     
     GFX_FILE_CLOSE(fd);
     
     return 0;
}


INT32 GFX_TransformDump(UINT32 u4Data, UINT32 u4Pitch, 
             UINT32 u4Wdith, UINT32 u4Height, UINT32 u4ColorMode)
{
    RZ_OSD_SCL_PARAM_SET_T rSclParam;
    E_RZ_INOUT_TYPE_T eRzType;

    GFX_COLOR_MODE_T    eColorMode     = u4ColorMode;
    UINT32 u4DstWidth, u4DstHeight, u4DstPitch;

    x_memset(&rSclParam, 0, sizeof(RZ_OSD_SCL_PARAM_SET_T));

    if(u4Wdith < _u4UserSetWidth)
    {
         u4DstWidth = u4Wdith;
         u4DstPitch = u4Pitch;
    }
    else
    {
         u4DstWidth = _u4UserSetWidth;
         u4DstPitch = _u4UserSetWidth * 2;
    }

    if(u4Height < _u4UserSetHeight)
    {
         u4DstHeight = u4Height;
    }
    else
    {
         u4DstHeight = _u4UserSetHeight;
    }   

    switch (eColorMode)
    {
    case CM_RGB565_DIRECT16:
        rSclParam.u4SrcCM = E_RZ_OSD_DIR_CM_RGB565;
        break;

    case CM_ARGB4444_DIRECT16:
        rSclParam.u4SrcCM = E_RZ_OSD_DIR_CM_ARGB4444;
        break;

    case CM_ARGB8888_DIRECT32:
        rSclParam.u4SrcCM = E_RZ_OSD_DIR_CM_ARGB8888;
        break;

    default:
        return -1;
    }

    rSclParam.u4TgCM = E_RZ_OSD_DIR_CM_ARGB4444;

    //src original, absolute coordinate
    rSclParam.u4SrcBase = u4Data;
    rSclParam.u4IsSrcDirCM = (UINT32) 1;
    rSclParam.u4SrcBufLen = u4Pitch;
    rSclParam.u4SrcHOffset = 0;
    rSclParam.u4SrcVOffset = 0;
    rSclParam.u4SrcW = u4Wdith;
    rSclParam.u4SrcH = u4Height;

    //target original   , absolute coordinate
    rSclParam.u4TgBase = _u4DumpAddr;
    rSclParam.u4TgHOffset = 0;
    rSclParam.u4TgVOffset = 0;
    rSclParam.u4TgW = u4DstWidth;
    rSclParam.u4TgH = u4DstHeight;
    rSclParam.u4TgBufLen = u4DstPitch;

    _u4DumpWidth  = u4DstWidth;
    _u4DumpHeight = u4DstHeight;
    _u4DumpPitch  = u4DstPitch;

#ifdef CC_GFX_MMU
    rSclParam.u4RzIOMMU_MMUTableAddr = HalCpuGetPhyPgd();
    IMGRZ_Cfg_MMU_IO((BOOL) 1, (BOOL) 0);
#endif

    GFX_Flush_Invalid_DCache();

    eRzType = E_RZ_INOUT_TYPE_OSDUI;
    IMGRZ_Lock();
    IMGRZ_ReInit();
    
    IMGRZ_SetScaleOpt(eRzType);
    IMGRZ_Scale(&rSclParam);
    IMGRZ_Flush();
    IMGRZ_Wait();
    IMGRZ_Unlock(); 
    
    return 0;
}


INT32 GFX_DumpDebug(VOID)
{
     char                  fileDir[100];
     GFX_COMMANDQUE_INFO_T tComQueInfo;
     
     GFX_DUMPCHECK(GFX_CmdQueParse(&tComQueInfo));

     if(tComQueInfo.u4OP != OP_BITBLT
        && tComQueInfo.u4OP !=OP_STRETCH_BITBLT)
     {
          return -1;
     }

     GFX_DUMPCHECK(GFX_TransformDump(tComQueInfo.pvSrc, 
                                     tComQueInfo.u4SrcPitch,
                                     tComQueInfo.u4SrcWidth,
                                     tComQueInfo.u4SrcHeight,
                                     tComQueInfo.u4SrcCM));

     GFX_DUMPCHECK(_GFX_GenerateFilePath("SRC", 
                                     tComQueInfo.pvSrc,
                                     tComQueInfo.u4OP,
                                     tComQueInfo.u4SrcWidth,
                                     tComQueInfo.u4SrcHeight,
                                     tComQueInfo.u4SrcPitch,
                                     tComQueInfo.u4SrcCM,
                                     sizeof(fileDir), 
                                     fileDir));

     GFX_DUMPCHECK(   GFX_DumpBmp(fileDir, 
                                  VIRTUAL(_u4DumpAddr),
                                  CM_ARGB4444_DIRECT16, 
                                  _u4DumpPitch, 
                                  _u4DumpWidth, 
                                  _u4DumpHeight));

     GFX_DUMPCHECK(GFX_TransformDump(tComQueInfo.pvDst, 
                                     tComQueInfo.u4DstPitch,
                                     tComQueInfo.u4DstWidth,
                                     tComQueInfo.u4DstHeight,
                                     tComQueInfo.u4DstCM));
     
     GFX_DUMPCHECK(_GFX_GenerateFilePath("DST", 
                                     tComQueInfo.pvDst,
                                     tComQueInfo.u4OP,
                                     tComQueInfo.u4DstWidth,
                                     tComQueInfo.u4DstHeight,
                                     tComQueInfo.u4DstPitch,
                                     tComQueInfo.u4DstCM,
                                     sizeof(fileDir), 
                                     fileDir));

     GFX_DUMPCHECK(   GFX_DumpBmp(fileDir, 
                                  VIRTUAL(_u4DumpAddr),
                                  CM_ARGB4444_DIRECT16, 
                                  _u4DumpPitch, 
                                  _u4DumpWidth, 
                                  _u4DumpHeight));

     return 0;
         
}


INT32 GFX_DumpSrc(VOID)
{
    char                  fileDir[100];
    GFX_COMMANDQUE_INFO_T tComQueInfo;
    
    GFX_DUMPCHECK(GFX_CmdQueParse(&tComQueInfo));
    
    if(tComQueInfo.u4OP != OP_BITBLT
      && tComQueInfo.u4OP !=OP_STRETCH_BITBLT
      && tComQueInfo.u4OP != OP_ALPHA_BLEND
      && tComQueInfo.u4OP != OP_ALPHA_COMPOSE
      && tComQueInfo.u4OP != OP_YCBCR_TO_RGB
      && tComQueInfo.u4OP != OP_ALPHA_COMPOSE_LOOP)
    {
         return -1;
    }
    
    GFX_DUMPCHECK(GFX_TransformDump(tComQueInfo.pvSrc, 
                                    tComQueInfo.u4SrcPitch,
                                    tComQueInfo.u4SrcWidth,
                                    tComQueInfo.u4SrcHeight,
                                    tComQueInfo.u4SrcCM));
    
    GFX_DUMPCHECK(_GFX_GenerateFilePath("SRC", 
                                    tComQueInfo.pvSrc,
                                    tComQueInfo.u4OP,
                                    tComQueInfo.u4SrcWidth,
                                    tComQueInfo.u4SrcHeight,
                                    tComQueInfo.u4SrcPitch,
                                    tComQueInfo.u4SrcCM,
                                    sizeof(fileDir), 
                                    fileDir));
    
    GFX_DUMPCHECK(   GFX_DumpBmp(fileDir, 
                                 VIRTUAL(_u4DumpAddr),
                                 CM_ARGB4444_DIRECT16, 
                                 _u4DumpPitch, 
                                 _u4DumpWidth, 
                                 _u4DumpHeight));

    return 0;

}


INT32 GFX_DumpDst(VOID)
{
     char                 fileDir[100];
     GFX_COMMANDQUE_INFO_T tComQueInfo;
    
     GFX_DUMPCHECK(GFX_CmdQueParse(&tComQueInfo));
    
     if(tComQueInfo.u4OP != OP_BITBLT
       && tComQueInfo.u4OP !=OP_STRETCH_BITBLT
       && tComQueInfo.u4OP != OP_RECT_FILL
       && tComQueInfo.u4OP != OP_ALPHA_BLEND
       && tComQueInfo.u4OP != OP_ALPHA_COMPOSE
       && tComQueInfo.u4OP != OP_YCBCR_TO_RGB
       && tComQueInfo.u4OP != OP_ALPHA_COMPOSE_LOOP)
     {
          return -1;
     }


     GFX_DUMPCHECK(GFX_TransformDump(tComQueInfo.pvDst, 
                                     tComQueInfo.u4DstPitch,
                                     tComQueInfo.u4DstWidth,
                                     tComQueInfo.u4DstHeight,
                                     tComQueInfo.u4DstCM));
     
     GFX_DUMPCHECK(_GFX_GenerateFilePath("DST", 
                                     tComQueInfo.pvDst,
                                     tComQueInfo.u4OP,
                                     tComQueInfo.u4DstWidth,
                                     tComQueInfo.u4DstHeight,
                                     tComQueInfo.u4DstPitch,
                                     tComQueInfo.u4DstCM,
                                     sizeof(fileDir), 
                                     fileDir));

     GFX_DUMPCHECK(   GFX_DumpBmp(fileDir, 
                                  VIRTUAL(_u4DumpAddr),
                                  CM_ARGB4444_DIRECT16, 
                                  _u4DumpPitch, 
                                  _u4DumpWidth, 
                                  _u4DumpHeight));

     return 0;
}


INT32 GFX_DumpRegion(INT32 i4Region)
{
    char fileDir[100];
    char prefix[10];
    UINT32 u4CEn, u4BmpAddress, u4Pitch, u4Width, u4Height, u4CM;
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_DEC_EN, &u4CEn));
    
    if(u4CEn)
    {
         Printf("OSD Compress mode ! \n");
         return 0;
    }
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_BMP_ADDR, &u4BmpAddress));
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_BMP_PITCH, &u4Pitch));
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_BMP_W, &u4Width));
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_BMP_H, &u4Height));
    
    GFX_DUMPCHECK(OSD_RGN_Get(i4Region, OSD_RGN_COLORMODE, &u4CM));
    
    GFX_DUMPCHECK(GFX_TransformDump(u4BmpAddress, 
                                     u4Pitch,
                                     u4Width,
                                     u4Height,
                                     u4CM));
     
    sprintf(prefix, "%d_OSD", _u4OsdCnt);
    GFX_DUMPCHECK(_GFX_GenerateFilePath(prefix, 
                                     u4BmpAddress,
                                     100,
                                     u4Width,
                                     u4Height,
                                     u4Pitch,
                                     u4CM,
                                     sizeof(fileDir), 
                                     fileDir));

    GFX_DUMPCHECK(   GFX_DumpBmp(fileDir, 
                                  VIRTUAL(_u4DumpAddr),
                                  CM_ARGB4444_DIRECT16, 
                                  _u4DumpPitch, 
                                  _u4DumpWidth, 
                                  _u4DumpHeight));  
    

    return 0;
}


INT32 GFX_DumpOSD(UINT32 u4Plane)
{
     INT32 i4Region;

     GFX_DUMPCHECK(i4Region = OSD_PLA_GetFirstRegion(u4Plane));
     
     GFX_DUMPCHECK(GFX_DumpRegion(i4Region));
     
     return 0;
}


VOID GFX_EnableDumpBuffer(BOOL bEnable, UINT32 u4Flags, const CHAR *path,
                                 UINT32 u4Width, UINT32 u4Height)
{
     UINT32 u4Pitch;
     
     if(bEnable)
     {
          _u4DumpBufferFlags |= u4Flags; 
     }
     else
     {
          _u4DumpBufferFlags &= (~u4Flags);
     }

     _u4UserSetWidth = GFX_DUMP_WIDTH;
     _u4UserSetHeight = GFX_DUMP_HEIGHT;

     if(u4Width > 0 || u4Width < GFX_DUMP_WIDTH)
     {
          u4Pitch = (u4Width * 2);

          if(u4Pitch & 0xF)
          {
               u4Pitch = (u4Pitch + 0xF) & (~0xF);
               u4Width = u4Pitch  / 2;
          }
             
         _u4UserSetWidth = u4Width;
     }

     if(u4Height > 0 || u4Height < GFX_DUMP_HEIGHT)
     {
         _u4UserSetHeight = u4Height;
     }   
     
     if(path)
     {
         strcpy(_gDumpPath, path);
     }
}


INT32 GFX_SetPath(const CHAR *path)
{
    if(path)
    {
        strcpy(_gDumpPath, path);
    }
    
    return 0;
}


INT32 GFX_SetDumpSize(UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4Pitch;
    
    _u4UserSetWidth = GFX_DUMP_WIDTH;
    _u4UserSetHeight = GFX_DUMP_HEIGHT;

    if(u4Width > 0 || u4Width < GFX_DUMP_WIDTH)
    {
        u4Pitch = (u4Width * 2);
        
        if(u4Pitch & 0xF)
        {
            u4Pitch = (u4Pitch + 0xF) & (~0xF);
            u4Width = u4Pitch   / 2;
        }
    
        _u4UserSetWidth = u4Width;
    }

    if(u4Height > 0 || u4Height < GFX_DUMP_HEIGHT)
    {
        _u4UserSetHeight = u4Height;
    }   

    return 0;
}


INT32 GFX_SetDumpEnable(BOOL bEnable, UINT32 u4Flags, UINT32 u4OpFlags)
{
    if(bEnable)
    {
         _u4DumpBufferFlags |= 1 << u4Flags; 
         _u4DumpBufferOPFlags |= 1 << u4OpFlags;
    }
    else
    {
         _u4DumpBufferFlags &= (~ (1 << u4Flags));
         _u4DumpBufferOPFlags &= (~ (1 << u4OpFlags));
    }

    return 0;
}

INT32 GFX_DUMP_FILE(const char* pf_path,void* pv_data,unsigned int u4_size)
{
    GFX_FILE_HANDLE     fd             = NULL;
    UINT64              u4Offset       = 0;
    static int          u4_cnt         = 0;
    char filename[120]; 
    snprintf( filename, 120, "%s_%d.drf", pf_path, u4_cnt++);
    fd = GFX_FILE_OPEN( filename, O_EXCL | O_CREAT | O_WRONLY, 0644 ); 
    if (fd <= 0)
    {
        return -1;
    }
     GFX_FILE_WRITE( fd, pv_data, u4_size, &u4Offset);  
     GFX_FILE_CLOSE(fd);
     return 0;
}


INT32 GFX_DumpReg(UINT32 *pu4BaseAddr, UINT32 *pu4Len, UINT32 u4Size, const char *filename)
{
     UINT32 i, j, m, n;
     UINT32 u4BaseAddr, u4Len;
     UINT64 u8Offset = 0;
     char line[120];
     GFX_FILE_HANDLE     fd;

     fd = GFX_FILE_OPEN( filename, O_EXCL | O_CREAT | O_WRONLY, 0644 ); 
     if (fd <= 0)
     {
          return -1;
     }

     for(i = 0; i < u4Size; i++)
     {
          u4BaseAddr = pu4BaseAddr[i];
          u4Len      = pu4Len[i];

          m = u4Len / 4;
          n = u4Len % 4;

          sprintf(line, "%s", "\r\n\r\n");

          GFX_FILE_WRITE( fd, line, strlen(line), &u8Offset);

          for(j = 0; j < m; j++)
          {
               sprintf(line, "0x%08X | %08X %08X %08X %08X \r\n", u4BaseAddr, 
                 IO_REG32(u4BaseAddr, 0),
                 IO_REG32(u4BaseAddr, 4),
                 IO_REG32(u4BaseAddr, 8),
                 IO_REG32(u4BaseAddr, 0xc));

               GFX_FILE_WRITE( fd, line, strlen(line), &u8Offset);
               
               u4BaseAddr += 0x10;  
          }

          if(n > 0)
          {
               sprintf(line, "0x%08X |", u4BaseAddr);
          }

          for(j = 0; j < n; j++)
          {
               sprintf(line, "%s %08X", line, IO_REG32(u4BaseAddr, 4 * j));
          }

          if(n > 0)
          {       
               GFX_FILE_WRITE( fd, line, strlen(line), &u8Offset);
          }
     }

     GFX_FILE_CLOSE(fd);
     
     return 0;   
}


VOID GFX_AnalyOSD(UINT32 u4Plane)
{
     UINT32 pu4Base[] = {0xF0021000, 0xF0021200, 0xF0021300, 0xF0021500, 0xF0021600, 0xF0021700};
     UINT32 pu4Length[] = {0x40, 0x40, 0x40, 0x40, 0x40, 0x40};
     char filename[60];
     
     if(_u4DumpBufferFlags & (1 << 30))
     {
          GFX_DumpOSD(u4Plane);

          sprintf(filename, "%s/%d_osd_reg.txt", _gDumpPath, _u4OsdCnt);

          GFX_DumpReg(pu4Base, pu4Length, 6, filename);
          
          _u4OsdCnt++;        
     }
}


#endif




