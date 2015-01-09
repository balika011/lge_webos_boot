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
 * $RCSfile: dsp_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_reg.h
 *  Brief of file dsp_reg.h.
 *  Details of file dsp_reg.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

#ifndef DSP_REG_H
#define DSP_REG_H

#include "x_typedef.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "rc2d_int.h"
#include "d2rc_int.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "aud_hw.h"

//-----------------------------------------------------------------------------
// ADSP RISC Register Map
//-----------------------------------------------------------------------------
#define ADSP_REG_OFST                           (0x40)         //(0x100>>2)
#define AUD_REG_OFST                            (0x00)
#define ADSP_REG_OFST_DSP1                 (0x140)        //(0x500 >> 2) 

#if 0
// DSP Control Reg (5100h)
#define RW_ADSPCR                               (ADSP_REG_OFST)
    #define  PROM_WR                            ((UINT32)0x01 << 1)
    #define  PRAM_WR                            ((UINT32)0x01 << 2)
    #define  DROM_WR                            ((UINT32)0x01 << 3)
    #define  HOT_DNLD                           ((UINT32)0x01 << 6)
    #define  PMEM_AUTO_INC                      ((UINT32)0x01 << 10)
    #define  DR_RD_EN                           ((UINT32)0x01 << 12)
// PROM Download Reg (5104h)
#define RW_PROM_DNLD                            (ADSP_REG_OFST + (0x0004 >> 2))
// PRAM Download Reg (5108h)
#define RW_PRAM_DNLD                            (ADSP_REG_OFST + (0x0008 >> 2))
// Counter Control Reg (FPGA only)(510Ch)
#define WO_CNTR_CLR                             (ADSP_REG_OFST + (0x000C >> 2))
    #define  CNTR_CLR                           (0x01 << 1)
// PRAM Addr Reg (5110h)
#define RW_DNLD_CNTR                            (ADSP_REG_OFST + (0x0010 >> 2))
// DROM Download Reg (FPGA only) (5158h)
#define RW_DROM_DNLD                            (ADSP_REG_OFST + (0x0058 >> 2))
#else
// DSP0 Control Reg (5100h), DSP1 Control Reg (5500H)
#define RW_ADSPCR(dspid)         (dspid ?(ADSP_REG_OFST_DSP1):(ADSP_REG_OFST))
    #define  PROM_WR                    ((UINT32)((0x01 << 1) + (0x01 << 10)))
    #define  PRAM_WR                    ((UINT32)0x01 << 2)
    #define  DROM_WR                    ((UINT32)( (0x01 << 3) + (0x01 << 10)))
    #define  HOT_DNLD                   ((UINT32)0x01 << 6)    
    #define  PMEM_AUTO_INC                      ((UINT32)0x01 << 10)
    #define  DR_RD_EN                           ((UINT32)0x01 << 12)    
// DSP1 PROM Download Reg (5504h), DSP0 PROM Download Reg (5104H)
#define RW_PROM_DNLD(dspid)         (dspid ? (ADSP_REG_OFST_DSP1 + (0x0004 >> 2)):(ADSP_REG_OFST + (0x0004 >> 2)))
// DSP1 PRAM Download Reg (5508h), DSP0 PRAM Download Reg (5108H)
#define RW_PRAM_DNLD(dspid)         (dspid ?(ADSP_REG_OFST_DSP1 + (0x0008 >> 2)): (ADSP_REG_OFST + (0x0008 >> 2)))
// DSP1 Counter Control Reg (FPGA only)(550Ch), DSP0 Counter Control Reg (FPGA only)(510CH)
#define WO_CNTR_CLR(dspid)         (dspid ?(ADSP_REG_OFST_DSP1 + (0x000C >> 2)): (ADSP_REG_OFST + (0x000C >> 2)))
    #define  CNTR_CLR                   (0x01 << 1)
// DSP1 PRAM Addr Reg (5510h), DSP0 PRAM Addr Reg (5110H)
#define RW_DNLD_CNTR(dspid)         (dspid ?(ADSP_REG_OFST_DSP1 + (0x0010 >> 2)):  (ADSP_REG_OFST + (0x0010 >> 2)))
// DSP1 DROM Download Reg (FPGA only) (5558h), DSP0 DROM Download Reg (FPGA only) (5158H)
#define RW_DROM_DNLD(dspid)         (dspid ?  (ADSP_REG_OFST_DSP1 + (0x0058 >> 2)):(ADSP_REG_OFST + (0x0058 >> 2)))
#endif

#if defined(CC_AUD_ARM_SUPPORT)
//-----------------------------------------------------------------------------
// Audio ARM Register Map
//-----------------------------------------------------------------------------

// Reset
#define APROC_RST                               (0x908)
    #define APROC_RESET                         (1U << 0)
    #define APROC_CLK_EN                        (1U << 1)

// Base address
#define APROC_BASE                              (0x904)
    #define APROC_CLK_SEL                       (1U << 0) //[0:0] ARM11 clock: 1: ARMPLL, 0: XTAL
    #define APROC_BASE_MASK                     (0x3FFFFFU << 10) //[31:10]: base address, must 1KB alignment


// Audio to Main shared regsiters (0 ~ 31)
#define APROC_A2MSR0                            (0x800)
#define APROC_A2MSR1                            (0x804)
#define APROC_A2MSR2                            (0x808)
#define APROC_A2MSR3                            (0x80C)
#define APROC_A2MSR4                            (0x810)
#define APROC_A2MSR5                            (0x814)
#define APROC_A2MSR6                            (0x818)
#define APROC_A2MSR22                           (0x858)
#define APROC_A2MSR23                           (0x85C)
#define APROC_A2MSR24                           (0x860)
#define APROC_A2MSR25                           (0x864)
#define APROC_A2MSR26                           (0x868)
#define APROC_A2MSR27                           (0x86C)
#define APROC_A2MSR28                           (0x870)
#define APROC_A2MSR29                           (0x874)
#define APROC_A2MSR30                           (0x878)
#define APROC_A2MSR31                           (0x87C)

// Main to Audio shared regsiters (0 ~ 31)
#define APROC_M2ASR0                            (0x880)
#define APROC_M2ASR1                            (0x884)
#define APROC_M2ASR2                            (0x888)

// PC
#define APROC_PC                                (0x914)

// Main to Audio Interrupt control
#define APROC_M2AINT                            (0x920)

#endif

//-----------------------------------------------------------------------------
// Register Access Macro
//-----------------------------------------------------------------------------
#define WriteREG(addr,value)        IO_WRITE32(AUDIO_BASE,((addr)<<2),(value))
#define ReadREG(addr)          	    IO_READ32(AUDIO_BASE,((addr)<<2))

//-----------------------------------------------------------------------------
// Memory Access Macro
//-----------------------------------------------------------------------------
//lint -emacro({717}, WriteDspCommDram)  to disable "do {...} while (0)" lint warning
#define WriteDspCommDram(dspid, addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))) = ((UINT32)(val)<<8);   \
                                      DSP_FlushInvalidateDCacheSmall(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)
#define WriteDspCommDram32bit(dspid, addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))) = ((UINT32)(val));   \
                                      DSP_FlushInvalidateDCacheSmall(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)

#define WriteDspCommDram2(dspid, addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))) = ((UINT32)(val)<<8);   \
                                      DSP_FlushInvalidateDCacheSmall2(dspid, (addr));  \
                                      }while(0)

#define WriteDspCommDram32(dspid, addr, val)   do{                                                       \
                                      *((UINT32 *)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))) = (val);   \
                                      DSP_FlushInvalidateDCacheSmall(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX, ((addr)<<2), 1);  \
                                      }while(0)

#define dReadDspCommDram(dspid, addr)       ((*((UINT32*)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))))>>8)
#define dReadDspCommDram32(dspid, addr)       (*((UINT32*)(VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))))


#define dReadDspCommDramAddr(dspid, addr)  (VIRTUAL(_u4DspBuf[dspid]+((addr)<<2)))

#define dReadDspNormDram(dspid,page,addr)     (*((UINT32*)(VIRTUAL(_u4DspDramBuf[dspid][(page)]+((addr)<<2)))))
#define dReadDspCmptDram(dspid,page,addr)     (*((UINT32*)(VIRTUAL(_u4DspCmptBuf[dspid][(page)]+((addr)<<2)))))

#define dReadDspNormDramAddr(dspid,page,addr)  (VIRTUAL(_u4DspDramBuf[dspid][(page)]+((addr)<<2)))
#define dReadDspCmptDramAddr(dspid,page,addr)  (VIRTUAL(_u4DspCmptBuf[dspid][(page)]+((addr)<<2)))


#define WriteDspDecDram(dspid, addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[dspid][0]+ ((addr)<<2))) = ((UINT32)(val)<<8);   \
                                    DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                                 \
                                     }while(0)
#define WriteDspDecDram32(dspid, addr, val)   do{                                                            \
                                     *((UINT32 *)(VIRTUAL(_u4DspDramBuf[dspid][0]+ ((addr)<<2))) = (val);   \
                                    DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                                 \
                                     }while(0)

#define wReadDspWORD(dspid,arg1)           (UINT16)(dReadDspCommDram(dspid,arg1) >> 8)
#define vWriteDspWORD(dspid,arg1,arg2)     WriteDspCommDram(dspid,arg1, (UINT32)((arg2) << 8))

#define WriteDspDecCacheDram(dspid, addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2)))) =((UINT32)(val)<<8);  \
                                        DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                               \
                                        }while(0)
#define WriteDspDecCacheDram32(dspid, addr, val) do{                                                             \
                                            (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2)))) = (val);  \
                                            DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                               \
                                            }while(0)
///CC_VORBIS_SUPPORT
#define WriteDspDec2CacheDram(dspid, addr, val) do{                                                             \
                                        (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2)))) =((UINT32)(val)<<8);  \
                                        DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                               \
                                        }while(0)
#define WriteDspDec2CacheDram32(dspid, addr, val) do{                                                             \
                                            (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2)))) = (val);  \
                                            DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                               \
                                            }while(0)
#define WriteDspDec3CacheDram(dspid, addr, val) do{                                                             \
                                            (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2)))) =((UINT32)(val)<<8);  \
                                            DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC3_DRAM_IDX);                                               \
                                            }while(0)
#define WriteDspDec3CacheDram32(dspid, addr, val) do{                                                             \
                                                (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2)))) = (val);  \
                                                DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC3_DRAM_IDX);                                               \
                                                }while(0)


#define ReadDspDecCacheDram(dspid, addr, data) do{                                                             \
                                        DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                   \
                                        data = ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2))))>>8); \
                                        }while(0)

#define ReadDspDec2CacheDram(dspid, addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                \
                                        data = ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2))))>>8); \
                                        }while(0)
#define ReadDspDec3CacheDram(dspid, addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC3_DRAM_IDX);                                \
                                        data = ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2))))>>8); \
                                        }while(0)                                        

#define ReadDspDecCacheDram32(dspid, addr, data) do{                                                             \
                                        DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC1_DRAM_IDX);                                   \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2)))); \
                                        }while(0)

#define ReadDspDec2CacheDram32(dspid, addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC2_DRAM_IDX);                                \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2)))); \
                                        }while(0)

#define ReadDspDec3CacheDram32(dspid, addr, data) do{                                                             \
                                       DSP_FlushInvalidateDCache(dspid, CACHE_FLUSH_TYPE_DATA, TYPE_DEC3_DRAM_IDX);                                \
                                        data = (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2)))); \
                                        }while(0)


#define DspDecCacheDramData(dspid,addr) ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2))))>>8)
#define DspDecCacheDramData32(dspid,addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2))))
///CC_VORBIS_SUPPORT
#define DspDec2CacheDramData(dspid,addr) ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2))))>>8)
#define DspDec2CacheDramData32(dspid,addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2))))
#define DspDec3CacheDramData(dspid,addr) ((*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2))))>>8)
#define DspDec3CacheDramData32(dspid,addr) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2))))
///FEW_DRAM_FLUSH
#define DspDecCacheDramWriteData(dspid, addr, val)   (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2)))) =((UINT32)(val)<<8)
#define DspDecCacheDramWriteData32(dspid, addr, val) (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][0] + ((addr)<<2)))) =((UINT32)(val))
#define DspDec2CacheDramWriteData(dspid, addr, val)  (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2)))) =((UINT32)(val)<<8)
#define DspDec2CacheDramWriteData32(dspid, addr, val)(*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][1] + ((addr)<<2)))) =((UINT32)(val))
#define DspDec3CacheDramWriteData(dspid, addr, val)  (*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2)))) =((UINT32)(val)<<8)
#define DspDec3CacheDramWriteData32(dspid, addr, val)(*((UINT32 *)VIRTUAL(_u4DspDramBuf[dspid][2] + ((addr)<<2)))) =((UINT32)(val))


#define uReadDspShmBYTE(dspid, addr) (*((UINT8*)(_u4ShareMem[dspid]+(addr))))
#define u2ReadDspShmWORD(dspid, addr) (*((UINT16*)(_u4ShareMem[dspid]+(addr))))
#define u4ReadDspShmDWRD(dspid, addr) (*((UINT32*)(_u4ShareMem[dspid]+(addr))))

#define vWriteDspShmBYTE(dspid, addr,value) (*((UINT8*)(_u4ShareMem[dspid] + (addr))))=(UINT8)(value);
#define vWriteDspShmWORD(dspid, addr,value) (*((UINT16*)(_u4ShareMem[dspid] + (addr)))) = (UINT16)(value);
#define vWriteDspShmDWRD(dspid, addr,value) (*((UINT32*)(_u4ShareMem[dspid] + (addr)))) = (UINT32)(value);

#ifdef CC_AUD_ARM_SUPPORT
// Audio ARM
#define u4AprocBIM_read(a) BIM_READ32(a)
#define vAprocBIM_write(a,b) BIM_WRITE32(a,b)
#endif

#endif  // DSP_REG_H

