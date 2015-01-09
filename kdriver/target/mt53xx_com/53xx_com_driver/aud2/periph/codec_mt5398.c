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
 * $RCSfile: codec_mt8291.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "sif_if.h"  // for i2c
#include "aud_if.h"
#include "drvcust_if.h"
#include "codec_mt5398.h"
#include "aud_debug.h"
#include "x_pinmux.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_END
#include "drvcust_if.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "aud_hw.h"

//#define LOG_REG_WRITE

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
BOOL _fgHpPlugFlag = FALSE;
#ifdef CC_SUPPORT_STR
BOOL _fgPwmClkEnable = TRUE;
#endif

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#ifdef LOG_REG_WRITE
#undef AUD_WRITE32
#undef AUD_CLR_BIT

void AUD_WRITE32(UINT32 offset, UINT32 value)
{
    IO_WRITE32(AUDIO_BASE, (offset), (value));
    //Printf("[!!!PWM!!!] I2C_EXT 58 00005%03x[31:00] %08x RW\n", offset, value);
}

#define AUD_CLR_BIT(Reg, Bit) AUD_WRITE32(Reg, AUD_READ32(Reg) & (~(Bit)))
#endif

#if 0   ///change macro to function
///Corbra have 3 pwm groups
///Viper have 4 pwm groups
///Each group only have 2 channels
#define vWritePWMG1(addr, data) \
    AUD_WRITE32(REG_INDIRECT_G1_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT_G1_DATA, data);\
     
#define vWritePWMG2(addr, data) \
    AUD_WRITE32(REG_INDIRECT_G2_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT_G2_DATA, data);\
     
#define vWritePWMG3(addr, data) \
    AUD_WRITE32(REG_INDIRECT_G3_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT_G3_DATA, data);\
     
/// for Viper Use
#define vWritePWMG4(addr, data) \
    AUD_WRITE32(REG_INDIRECT_G4_ADDR, addr);\
    AUD_WRITE32(REG_INDIRECT_G4_DATA, data);\
    ;
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
static void _MT5398_Init_all(UINT8 u1ClkFmt, BOOL fgInit);
static void _MT5398_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt);

//---------------------------------------------------------------------------
// Static variables a
//---------------------------------------------------------------------------

#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    #ifdef CC_SUPPORT_96K_AOUT
/// Fs*15=Fs*3*5, Set 5X Table
/// 96*15 = 1440KHz
/// Fsout =240.00 kHz, FsIn = 48.00 kHz, bw = 20.00 kHz, target att. = -100.00 dB
/// 3-23 NS coefs bw=30k Hinf=16 order=5 QLEV=4, 24 ch0 intp gain, 25-84 intp coefs,217 ch1 intp gain
static const UINT32 YRAM_5X[] =
{
    0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
    0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
    0x000000, 0x0dc200, 0xfffe00, 0xffff00, 0x000300, 0x000c00, 0x001800, 0x002200, 0x002400, 0x001700,     /// 0x115 CH0 Gain
    0xfffb00, 0xffd500, 0xffb200, 0xffa200, 0xffb500, 0xffee00, 0x004200, 0x009400, 0x00c000, 0x00a900,
    0x004500, 0xffa900, 0xff0800, 0xfea600, 0xfebb00, 0xff5a00, 0x006300, 0x017f00, 0x023c00, 0x023800,
    0x014b00, 0xffa600, 0xfdd100, 0xfc8100, 0xfc5b00, 0xfda900, 0x002900, 0x031200, 0x054800, 0x05c400,
    0x040100, 0x004b00, 0xfbc600, 0xf81e00, 0xf6f700, 0xf94500, 0xfec400, 0x05df00, 0x0c0c00, 0x0e9b00,
    0x0bb500, 0x034200, 0xf74a00, 0xebb200, 0xe54200, 0xe83f00, 0xf6f600, 0x10c400, 0x31dc00, 0x540800,
    0x702500, 0x7fff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x0dc200    /// 0x1D6 CH1 Gain
};
    #else
/// Fs*30=Fs*3*10, Set 10X Table
/// 48*30 = 1440KHz
/// Fsout = 192.00 kHz, FsIn = 48.00 kHz, bw = 20.00 kHz, target att. = -100.00 dB
/// 3-23 NS coefs bw=30k Hinf=16 order=5 QLEV=4, 24 intp gain, 25-72 intp coefs
static const UINT32 YRAM_10X[] =
{
    0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
    0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
    0x000000, 0x0dc200, 0xfffe00, 0xfffe00, 0xffff00, 0x000000, 0x000200, 0x000600, 0x000a00, 0x000f00,     /// 0x115 CH0 Gain
    0x001500, 0x001b00, 0x002000, 0x002300, 0x002400, 0x002100, 0x001b00, 0x001100, 0x000300, 0xfff100,
    0xffde00, 0xffcb00, 0xffb900, 0xffac00, 0xffa400, 0xffa500, 0xffaf00, 0xffc200, 0xffdf00, 0x000300,
    0x002d00, 0x005800, 0x008100, 0x00a300, 0x00b900, 0x00bf00, 0x00b300, 0x009300, 0x006100, 0x001f00,
    0xffd100, 0xff7e00, 0xff2e00, 0xfee900, 0xfeb900, 0xfea300, 0xfeae00, 0xfedc00, 0xff2c00, 0xff9a00,
    0x001e00, 0x00ae00, 0x013b00, 0x01b900, 0x021700, 0x024900, 0x024700, 0x020a00, 0x019300, 0x00e800,
    0x001600, 0xff2d00, 0xfe4200, 0xfd6e00, 0xfcc600, 0xfc6200, 0xfc4f00, 0xfc9900, 0xfd4000, 0xfe3d00,
    0xff7d00, 0x00e700, 0x025b00, 0x03b500, 0x04cf00, 0x058800, 0x05c600, 0x057600, 0x049500, 0x032b00,
    0x015000, 0xff2900, 0xfce700, 0xfabf00, 0xf8ec00, 0xf7a200, 0xf70f00, 0xf75200, 0xf87800, 0xfa7a00,
    0xfd3800, 0x008100, 0x040e00, 0x078d00, 0x0aa300, 0x0cfa00, 0x0e4300, 0x0e4000, 0x0cd000, 0x09ee00,
    0x05b700, 0x006e00, 0xfa7500, 0xf44900, 0xee7c00, 0xe9ab00, 0xe66e00, 0xe55000, 0xe6c300, 0xeb1200,
    0xf25b00, 0xfc8c00, 0x095c00, 0x185000, 0x28c200, 0x39e900, 0x4ae600, 0x5ad200, 0x68cd00, 0x740d00,
    0x7bf000, 0x7fff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x0dc200    /// 0x1D6 CH1 Gain
};

/// Fs*24=Fs*3*8, Set 8X Table
/// 48*24 = 1152KHz
static const UINT32 YRAM_8X[] =
{
    0x3eb600,0x2b9a00,0x249f00,0x0d9000,0xfde900,0x000000,0x000000,0x0d1a00,0xf2e600,0x100000,0x100000,
    0x100000,0x100000,0x000000,0x000000,0x100000,0x100000,0x100000,0xfee300,0xfce300,0x000000,0x0c0000,
    0xfffe00,0xfffe00,0xffff00,0x000200,0x000600,0x000c00,0x001300,0x001a00,0x002000,0x002400,0x002300,
    0x001c00,0x000f00,0xfffd00,0xffe600,0xffcd00,0xffb700,0xffa800,0xffa300,0xffad00,0xffc500,0xffeb00,
    0x001d00,0x005300,0x008600,0x00ad00,0x00bf00,0x00b600,0x008f00,0x004a00,0xffef00,0xff8800,0xff2400,
    0xfed400,0xfea700,0xfeaa00,0xfee300,0xff5100,0xffea00,0x009c00,0x014d00,0x01e100,0x023d00,0x024c00,
    0x020000,0x015a00,0x006900,0xff4b00,0xfe2600,0xfd2700,0xfc7d00,0xfc4a00,0xfca700,0xfd9400,0xfefd00,
    0x00b800,0x028900,0x042900,0x055300,0x05c900,0x056600,0x041f00,0x020f00,0xff7200,0xfc9f00,0xfa0100,
    0xf80600,0xf71100,0xf76500,0xf91b00,0xfc1a00,0x001100,0x048200,0x08ca00,0x0c3c00,0x0e3300,0x0e2f00,
    0x0bee00,0x077600,0x012500,0xf9ad00,0xf20200,0xeb4700,0xe6ac00,0xe54900,0xe7f900,0xef3c00,0xfb1b00,
    0x0b2400,0x1e6800,0x338f00,0x48fa00,0x5ce600,0x6d9f00,0x79ae00,0x7fff00,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,0x000000,
    0x000000,0x000000,0x000000,0x000000,0x000000,0x0c0000  /// 0x1D6 CH1 Gain
};

    #endif
#else
/// Fs*9=Fs*3*3, 3X
/// 48*9 = 432KHz
/// Fsout = 192.00 kHz, FsIn = 48.00 kHz, bw = 20.00 kHz, target att. = -100.00 dB
/// 3-23 NS coefs bw=30k Hinf=16 order=5 QLEV=4, 24 intp gain, 25-72 intp coefs
static const UINT32 YRAM_3X[] =
{
    0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
    0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
    0x000000, 0x0c0000, 0xfff700, 0xffed00, 0xfff600, 0x001a00, 0x003700, 0x001c00, 0xffc700, 0xff8700,     /// 0x115 CH0 Gain
    0xffc200, 0x006e00, 0x00e700, 0x007800, 0xff4100, 0xfe6d00, 0xff2d00, 0x013800, 0x029200, 0x015b00,
    0xfe1b00, 0xfbf800, 0xfdda00, 0x02de00, 0x062c00, 0x035700, 0xfbb000, 0xf68e00, 0xfac400, 0x06a200,
    0x0f0600, 0x08b100, 0xf4d900, 0xe4c700, 0xee7400, 0x199700, 0x552100, 0x7fff00, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x0c0000    /// 0x1D6 CH1 Gain
};
/// Fs*8=Fs*2*4, 4X
/// 48*8 = 384KHz
/// Fsout =192.00 kHz, FsIn = 48.00 kHz, bw = 20.00 kHz, target att. = -100.00 dB
/// 3-23 NS coefs bw=30k Hinf=16 order=5 QLEV=4, 24 ch0 intp gain, 25-72 intp coefs,217 ch1 intp gain
static const UINT32 YRAM_4X[] =
{
    0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
    0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
    0x000000, 0x0dc200, 0xfffe00, 0xffff00, 0x000800, 0x001600, 0x002300, 0x002200, 0x000800, 0xffda00,     /// 0x115 CH0 Gain
    0xffae00, 0xffa300, 0xffd300, 0x003600, 0x009d00, 0x00c300, 0x007600, 0xffc000, 0xfef600, 0xfe9a00,
    0xff0a00, 0x003c00, 0x019f00, 0x025b00, 0x01c500, 0xffe500, 0xfd9900, 0xfc4100, 0xfcf700, 0xffc800,
    0x036c00, 0x05c100, 0x04fd00, 0x00dd00, 0xfb3600, 0xf73f00, 0xf7e600, 0xfde600, 0x06c900, 0x0dac00,
    0x0d9f00, 0x04a400, 0xf5a900, 0xe83900, 0xe59200, 0xf45600, 0x14c400, 0x3f7a00, 0x67ac00, 0x7fff00,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x0dc200    /// 0x1D6 CH1 Gain
};
/// Fs*10=Fs*2*5, 5X
/// 48*10 = 480KHz
/// Fsout =240.00 kHz, FsIn = 48.00 kHz, bw = 20.00 kHz, target att. = -100.00 dB
/// 3-23 NS coefs bw=30k Hinf=16 order=5 QLEV=4, 24 ch0 intp gain, 25-84 intp coefs,217 ch1 intp gain
static const UINT32 YRAM_5X[] =
{
    0x3eb600, 0x2b9a00, 0x249f00, 0x0d9000, 0xfde900, 0x000000, 0x000000, 0x0d1a00, 0xf2e600, 0x100000,
    0x100000, 0x100000, 0x100000, 0x000000, 0x000000, 0x100000, 0x100000, 0x100000, 0xfee300, 0xfce300,
    0x000000, 0x0dc200, 0xfffe00, 0xffff00, 0x000300, 0x000c00, 0x001800, 0x002200, 0x002400, 0x001700,     /// 0x115 CH0 Gain
    0xfffb00, 0xffd500, 0xffb200, 0xffa200, 0xffb500, 0xffee00, 0x004200, 0x009400, 0x00c000, 0x00a900,
    0x004500, 0xffa900, 0xff0800, 0xfea600, 0xfebb00, 0xff5a00, 0x006300, 0x017f00, 0x023c00, 0x023800,
    0x014b00, 0xffa600, 0xfdd100, 0xfc8100, 0xfc5b00, 0xfda900, 0x002900, 0x031200, 0x054800, 0x05c400,
    0x040100, 0x004b00, 0xfbc600, 0xf81e00, 0xf6f700, 0xf94500, 0xfec400, 0x05df00, 0x0c0c00, 0x0e9b00,
    0x0bb500, 0x034200, 0xf74a00, 0xebb200, 0xe54200, 0xe83f00, 0xf6f600, 0x10c400, 0x31dc00, 0x540800,
    0x702500, 0x7fff00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x0dc200    /// 0x1D6 CH1 Gain
};
#endif

static CODEC_FUNCTBL_T _rCodecFunTbl =
{
    _MT5398_Init,                   //pfnInit
    _MT5398_Mute,                 //pfnMute
    _MT5398_MuteCLK,              //pfnMuteClk
    _MT5398_AdcFmtCfg,       //pfnSetAdcOutFmt;
    _MT5398_DacFmtCfg,      	//pfnSetDacInFmt;
    NULL,                                  //pfnSetDacGain;
    NULL,                                  //pfnGetDacGain
    NULL,                                  //pfnSetAdcGain
    NULL,                                  //pfnGetAdcGain
    NULL,                                  //pfnSetHpGain
    NULL,                                  //pfnGetHpGain
    _MT5398_ChlSel,               //pfnSetAdcMuxSel
    NULL,                           	//pfnRegWrite
    NULL,                                  //pfnRegRead
    _MT5398_DacChlSel,           //pfnSetDacChl
    _MT5398_DacChlSelQuery,   //pfnGetDacChl
    _MT5398_ADCDOWN            //pfnADCDown
};

static CODEC_T _rCodecMT5398 =
{
    "MT5398",				// szName[32];
    0,							// i2MaxHpGain
    -72,							// i2MinHpGain
    0,							// i2MaxDacGain
    0,							// i2MinDacGain
    7,							// u1AdcMuxNum
    2,							// u1DacNum
    1,							// u1HpNum
    &_rCodecFunTbl			// rCodecFuncTbl
};

static BOOL fgPWMInit = FALSE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
///Corbra have 3 pwm groups
///Viper have 4 pwm groups
///Each group only have 2 channels
static void vWritePWMG1(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G1_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G1_DATA, data);
}

static void vWritePWMG2(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G2_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G2_DATA, data);
}

static void vWritePWMG3(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G3_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G3_DATA, data);
}

#ifdef CC_MT5880
static void vWritePWMG4(UINT32 addr, UINT32 data)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G4_ADDR, addr);
    AUD_WRITE32(REG_INDIRECT_G4_DATA, data);
}
#endif

static UINT32 u4ReadPWMG1(UINT32 addr)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return 0;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G1_ADDR, addr);
    return (AUD_READ32(REG_INDIRECT_G1_DATA));
}

static UINT32 u4ReadPWMG2(UINT32 addr)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return 0;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G2_ADDR, addr);
    return (AUD_READ32(REG_INDIRECT_G2_DATA));
}

static UINT32 u4ReadPWMG3(UINT32 addr)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return 0;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G3_ADDR, addr);
    return (AUD_READ32(REG_INDIRECT_G3_DATA));
}

#ifdef CC_MT5880
static UINT32 u4ReadPWMG4(UINT32 addr)
{
#ifdef CC_SUPPORT_STR
    if (!_fgPwmClkEnable)
    {
        return 0;
    }
#endif
    AUD_WRITE32(REG_INDIRECT_G4_ADDR, addr);
    return (AUD_READ32(REG_INDIRECT_G4_DATA));
}
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

void _MT5398_Init(void)
{
    BOOL    fAdcNegGain = 0;
    AUD_INTER_DAC_CFG_T* prInterDacUsage;
    BOOL fPowerStage = 0;
    BOOL fInternalLDO = 0;

    UINT32  u4TempReg1 = 0;
    UINT32  u4TempReg2 = 0;
    UNUSED( fPowerStage );
    UNUSED( fInternalLDO );
    UNUSED( u4TempReg1 );
    UNUSED( u4TempReg2 );
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_Init\n");
#endif

    //if (!DRVCUST_OptGet(eAudioAI2SuseforSpeaker))
    //{
    //    ADAC_GpioAmpEnable1(FALSE);
    //}

    if (!fgPWMInit)
    {
#if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882)
        ///Remove "ADC_VMID_REF_BG" by Kevin Lo designer request.
        ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M));
#elif defined(CC_MT5890) 
        ///by TC Wang request:bit30 alway==1,select Bandgap reference for VMID generation;bit28 always==0,select AVSS_CLN for VMID groud reference
        ///by TC Wang request:Add "Bit31:ADC_EN_5M_VMID" in Power up and auto tune status,after 1ms,this bit will set to 0.
        ANA_WRITE32(REG_AADC_CFG0, (ADC_BGVMID_PWD_ON | 0x013880c4 | VCMGEN_LP_1M));
        ///Auto -tune,Enable tuning for VMID refering to bandgap
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) | ADC_BGVMID_TUNE_EN));
        ANA_WRITE32(REG_AADC_CFG2, 0x41ae0000);
        x_thread_delay(1);
        ///Enter power-Saving,Power Off VMID generation circuit refering to bandgap 
        ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) | ADC_BGVMID_PWD_OFF));
        //x_thread_delay(100);///wait for VMID charge,then enter normal operation status
        //ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) | ADC_BGVMID_TUNE_DIS));
#elif defined(CC_MT5880)
        if (IS_IC_5860_ES2() || IS_IC_5880_ES2())
        {
            ANA_WRITE32(REG_AADC_CFG0, (0x001845c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
        }
        else
        {
            ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
        }
#else
        ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
#endif

#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882)
        // Remy ADAC Initial Setting.
        ANA_WRITE32(REG_ADAC_CFG0, PWMPAD_CFG_INIT);  /// for digital differential-positive.
        ANA_WRITE32(REG_ADAC_CFG1, PWMPAD_CFG_INIT);  /// for digital differential-negative.
    #ifdef CC_AUD_HP_DEPOP
        ///REG_ADAC_CFG2~REG_ADAC_CFG6 setting by De-pop flowchart.
    #else
        ///w 0xf0060108 0x00000555, without mute control, set 60108=0x555 directly
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_WRITE32(REG_ADAC_CFG3, (DAC_DFC_CH2 | DAC_DFC_CH1 | DAC_DFC_CH0) | (DAC_FS_CH2_P42 | DAC_FS_CH1_P42 | DAC_FS_CH0_P42));
        ANA_WRITE32(REG_ADAC_CFG4, (IB_PDET_10UA | IB_OPA_10UA | IB_PWMBUF_10UA | IB_STEP_10UA) | AC_OFF_PWRDET | (RAMP_STEP_398MV | RAMP_STEP_ST_53MV | CKF_DIV_8192));
        ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
        ANA_WRITE32(REG_ADAC_CFG6, (DAC_DESENSE_CH0));
    #endif

#elif defined(CC_MT5890)
        // Oryx ADAC Initial Setting.
        if (IS_IC_5890_ES1())
        {
            ANA_WRITE32(REG_ADAC_CFG0, PWMPAD_CFG_INIT1);  /// for digital differential-positive.
            ANA_WRITE32(REG_ADAC_CFG1, PWMPAD_CFG_INIT1);  /// for digital differential-negative.
        }
        else
        {
            ANA_WRITE32(REG_ADAC_CFG0, PWMPAD_CFG_INIT);  /// for digital differential-positive.
            ANA_WRITE32(REG_ADAC_CFG1, PWMPAD_CFG_INIT);  /// for digital differential-negative.
        }
    #ifdef CC_AUD_HP_DEPOP
        ///REG_ADAC_CFG2~REG_ADAC_CFG6 setting by De-pop flowchart.
    #else
        ///w 0xf0060108 0x00000555, without mute control, set 60108=0x555 directly
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_CLR_BIT(REG_ADAC_CFG3, DAC_HIZ_EN);
        ANA_WRITE32(REG_ADAC_CFG4, (DAC_OC_EN | DAC_OC_HIGH_SEL00 | DAC_OC_LOW_SEL11 | DAC_DFC_CH0));
        ANA_WRITE32(REG_ADAC_CFG5, (DAC_FS_CH3_P42 | DAC_FS_CH2_P42 | DAC_FS_CH1_P42 | DAC_FS_CH0_P42 | DAC_IB_IBGEN_10UA | IB_PDET_10UA | IB_HPA_10UA | IB_LOA_10UA | IB_PWMBUF_10UA | IB_STEP_10UA));
        ANA_WRITE32(REG_ADAC_CFG6, (RAMP_STEP_398MV | RAMP_STEP_ST_53MV | CKF_DIV_1024) | CKF_ST_DIV_8192);
        ANA_WRITE32(REG_ADAC_CFG7, DAC_DESENSE_CH3 | DAC_DESENSE_CH2 | DAC_DESENSE_CH1 | DAC_DESENSE_CH0);
        if (IS_IC_5890_ES1())
        {
            ANA_CLR_BIT(REG_ADAC_CFG9, DAC_432M_EN);
        }
        else
        {
            ANA_CLR_BIT(REG_ADAC_CFG9, DAC_HADDS1_EN);
        }
        ANA_WRITE32(REG_ADAC_CFG10, DAC_RAMP_R2R_EN|DAC_RAMP_DELAY_EN);
    #endif

#else
        fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
        //if( fInternalLDO )
        {
            ANA_WRITE32(REG_PWMDAC_CFG4, CUR_MORR_EN | ALR0_CAP_P36PF | ALR1_CAP_P36PF | ALR2_CAP_P36PF | ALR0_1K_MODE | ALR1_1K_MODE | ALR2_1K_MODE); ///0x40578000);   ///Kevin Suggest.

            u4TempReg1 = (LDO_CAB_CUR_2250U | AL0_CAB_01 | AL0_ILDO_EN | AR0_CAB_01 | AR0_ILDO_EN);
            u4TempReg2 = (AL1_CAB_01 | AL1_ILDO_EN | AR1_CAB_01 | AR1_ILDO_EN | AL2_CAB_01 | AL2_ILDO_EN | AR2_CAB_01 | AR2_ILDO_EN);
#if defined (CC_MT5398)
            if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL0AR0) != 0)
            {
                u4TempReg1 = (u4TempReg1 & (~AL0_ILDO_EN) & (~AR0_ILDO_EN));
            }
            if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL1AR1) != 0)
            {
                u4TempReg2 = (u4TempReg2 & (~AL1_ILDO_EN) & (~AR1_ILDO_EN));
            }
            if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL2AR2) != 0)
            {
                u4TempReg2 = (u4TempReg2 & (~AL2_ILDO_EN) & (~AR2_ILDO_EN));
            }
            ANA_WRITE32(REG_PWMDAC_CFG2, u4TempReg1);
            ANA_WRITE32(REG_PWMDAC_CFG3, u4TempReg2);
            LOG(0, "Initial REG_PWMDAC_CFG2=0x%x, REG_PWMDAC_CFG3=0x%x. \n", u4TempReg1, u4TempReg2);
#else
            ANA_WRITE32(REG_PWMDAC_CFG2, u4TempReg1);    ///0x00003830);   ///Kevin Suggest.
            ANA_WRITE32(REG_PWMDAC_CFG3, u4TempReg2);  ///0x30303030); ///Kevin Suggest.
#endif
            /// Clean to Zero
            ANA_WRITE32(REG_PWMDAC_CFG1, 0x00000000);   ///Kevin Suggest.
#ifdef CC_MT5398
            ANA_WRITE32(REG_PWMDAC_CFG5, 0x00000000);   ///Kevin Suggest.
#elif defined (CC_MT5880)
            ANA_WRITE32(REG_PWMDAC_CFG5, 0x1A820000);   /// Kevin Suggest.
            ANA_WRITE32(REG_PWMDAC_CFG6, 0xF0000000 | RG_HB_DFC_EN | RG_HB_FS_1_042V | RG_HB_FS_0_042V); //0xf0000194 ,for DC on depop , increasing analog gain will lead to a bigger AC off pop, so can not set as 0xf000019e
            ANA_WRITE32(REG_PWMDAC_CFG7, RG_HB_HIZ_DIS | RG_HB_IB_PDET_10U | RG_HB_IB_OPA_10U | RG_HB_IB_PWMBUF_10U | RG_HB_IB_STEP_10U | RG_HB_ENPWRDET_EN | RG_HB_STEP_112M | RG_HB_STEP_ST_112M); //0x0aa10024
            ANA_WRITE32(REG_PWMDAC_CFG8, RG_HB_CKF_2048 | RG_HB_CKF_ST_2048);
            ANA_WRITE32(REG_PWMDAC_CFG9, 0x00000000);
#ifdef LOG_REG_WRITE
            LOG(0, "=====> [depop] : after (a)\n");
            LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
            LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
            LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
            LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
#endif

            //when open Internal LDO, default use 1kohm mode. For powerstage should use 0k ohm mode
            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL0AR0);
            if (fPowerStage) //ALAR0
            {
                ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR0_1K_MODE));
            }

            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL1AR1);
            if (fPowerStage) //ALAR1
            {
                ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR1_1K_MODE));
            }

            fPowerStage = (BOOL)DRVCUST_OptGet(eAudioPowerstageUseAL2AR2);
            if (fPowerStage) //ALAR2
            {
                ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR2_1K_MODE));
            }
        }

        if (!fInternalLDO)
        {
            ANA_WRITE32(REG_PWMDAC_CFG2, (ANA_READ32(REG_PWMDAC_CFG2) & (~AL0_ILDO_EN) & (~AR0_ILDO_EN))); ///Kevin Suggest.
            ANA_WRITE32(REG_PWMDAC_CFG3, (ANA_READ32(REG_PWMDAC_CFG3) & (~AL1_ILDO_EN) & (~AR1_ILDO_EN) & (~AL2_ILDO_EN) & (~AR2_ILDO_EN)));
        }
#endif

#if defined(CC_MT5881) || defined(CC_MT5399) ||  defined(CC_MT5882)
        ANA_WRITE32(REG_AADC_CFG1, DE_SENSE_DWA | PGA_CLAMP_ON | DE_SENSE_EN_LVL15);
#elif defined(CC_MT5890) 
        ANA_WRITE32(REG_AADC_CFG1, (0xe0ab040a | PGA_CLAMP_ON));
#elif defined(CC_MT5880)
        if (IS_IC_5860_ES2() || IS_IC_5880_ES2())
        {
            ANA_WRITE32(REG_AADC_CFG1, DEMOD_GROUND_ON | PGA_CLAMP_ON | DE_SENSE_EN_LVL7);
        }
        else
        {
            ANA_WRITE32(REG_AADC_CFG1, DEMOD_GROUND_ON | PGA_CLAMP_ON);
        }
#else
        ANA_WRITE32(REG_AADC_CFG1, DEMOD_GROUND_ON | PGA_CLAMP_ON);
#endif
    }

    /// PWM related.
    _MT5398_Init_all(FS_48K, TRUE);

    /// ADC related: bit6=1 ADC_CLK1 phase inverse.
    AUD_WRITE32(REG_INTADC_SDADC0, 0x01020040);//0x5240
#if defined(CC_MT5880) /// Viper Only, Remy No Need.
    AUD_WRITE32(0x0248, (AUD_READ32(0x0248) & 0xFFFFFCFF));        // 0x5248[9:8]=00
#endif

    fAdcNegGain = (BOOL)DRVCUST_OptGet(eAudioADCNegDigitalGain);    ///[Joel]20101026
#if defined(CC_MT5880)
    if (IS_IC_5860_ES2() || IS_IC_5880_ES2())
    {
        if (fAdcNegGain)
        {
            AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N_5860_5880_ECO);    ///Bit25 is MSB of adc digital gain.
        }
        else
        {
            AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_5860_5880_ECO);
        }
    }
    else
#elif defined(CC_MT5399)
    if (IS_IC_5399_ES2())
    {
        if (fAdcNegGain)
        {
            AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N_5399_ECO);    ///Bit25 is MSB of adc digital gain.
        }
        else
        {
            AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_5399_ECO);
        }
    }
    else
#elif defined(CC_MT5890)
    if (IS_IC_5861())
    {
       
	   if (fAdcNegGain)
	   {   
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N_5861);	 ///Bit25 is MSB of adc digital gain.				   
	   }
	   else
	   {
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_5861);    ///Bit25 is MSB of adc digital gain. 
	   }
       
    }
	else
    if (IS_IC_5890_MPS1())
    {
       
	   if (fAdcNegGain)
	   {   
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N_5890_MPS1);	 ///Bit25 is MSB of adc digital gain.				   
	   }
	   else
	   {
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_5890_MPS1);    ///Bit25 is MSB of adc digital gain. 
	   }
       
    }
	else
    if (IS_IC_5890_ES3())
    {
       
	   if (fAdcNegGain)
	   {   
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N_5890_ES3);	 ///Bit25 is MSB of adc digital gain.				   
	   }
	   else
	   {
		    AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_5890_ES3);    ///Bit25 is MSB of adc digital gain. 
	   }
       
    }
	else
#endif
	{	   
        if (fAdcNegGain)
		{   
			AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN_N);  ///Bit25 is MSB of adc digital gain.				   
		}
		else
		{
			AUD_WRITE32(REG_INTADC_SDADC1, ADC_GAIN);	 ///Bit25 is MSB of adc digital gain. 
		}
		   
	}

#if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5882)
    ///Remove "ADC_VMID_REF_BG" by Kevin Lo designer request.
    ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M));
#elif defined(CC_MT5890) 
    ANA_WRITE32(REG_AADC_CFG0, 0x013880e4);
    ANA_WRITE32(REG_AADC_CFG2, 0x41ae0000);
#elif defined(CC_MT5880)
    if (IS_IC_5860_ES2() || IS_IC_5880_ES2())
    {
        ANA_WRITE32(REG_AADC_CFG0, (0x001845c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
    }
    else
    {
        ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
    }
#else
    ANA_WRITE32(REG_AADC_CFG0, (0x001844c0 | VCMGEN_LP_1M)); //Pair 1 + ADC_R_PD,ADC_L_PD=0 + Gain 4 + others default + Enable ADC VCM for internal LDO enable use.
#endif

    UNUSED(DRVCUST_OptQuery(eAudioInterDacChlConfig, (UINT32*)(void*)&prInterDacUsage));

    if (prInterDacUsage != NULL)
    {
        _MT5398_DacChlSel(INTERNAL_DAC_ID_MAIN, prInterDacUsage->eDac0ChlSel);
        _MT5398_DacChlSel(INTERNAL_DAC_ID_AUX, prInterDacUsage->eDac1ChlSel);
        _MT5398_DacChlSel(INTERNAL_DAC_ID_THIRD, prInterDacUsage->eDac2ChlSel);
        _MT5398_DacChlSel(INTERNAL_DAC_ID_4TH, prInterDacUsage->eDac3ChlSel);
    }

    //if (!DRVCUST_OptGet(eAudioAI2SuseforSpeaker))
    //{
    //    x_thread_delay(10 + 40);    ///40 for pwm fade-in time.
    //    // For AC On Pop - power stage need 50% duty than un-mute it.
    //    ADAC_GpioAmpEnable1(TRUE);
    //}

    fgPWMInit = TRUE;

}

#ifdef XY_RAM_TEST
void _MT5398_XYRamTest(BOOL xy)
{
	UINT32 i, data, failcnt;
    Printf("_MT5398_XYRamTest\n");
    if (xy == 0)
    {
        vWritePWMG1(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG2(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG3(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#endif

        Printf("******************************PG1 X Ram********************************\n");
        failcnt = 0;
        for (i = 0; i < 76; i++)          //benzhai : 256 shrink to 76 after Remy
        {
            vWritePWMG1(A_PCADDR, i);
            vWritePWMG1(A_PCDATA, i + 1);
            data = u4ReadPWMG1(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            if (data-i != 1)
                failcnt++;
        }
        Printf("Fail count %d (0: Pass)\n ", failcnt);
        Printf("******************************PG2 X Ram********************************\n");
        failcnt = 0;
        for (i = 0; i < 76; i++)         //benzhai : 256 shrink to 76 after Remy
        {
            vWritePWMG2(A_PCADDR, i);
			vWritePWMG2(A_PCDATA, i+1);
            data = u4ReadPWMG2(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            if (data-i != 1)
                failcnt++;
        }
        Printf("Fail count %d (0: Pass)\n ", failcnt);
        Printf("******************************PG3 X Ram********************************\n");
        failcnt=0;
        for (i = 0; i < 76; i++)        //benzhai : 256 shrink to 76 after Remy
        {
            vWritePWMG3(A_PCADDR, i);
			vWritePWMG3(A_PCDATA, i+1);
            data = u4ReadPWMG3(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            if (data-i != 1)
                failcnt++;
        }
        Printf("Fail count %d (0: Pass)\n ", failcnt);

#ifdef CC_MT5880
        Printf("******************************PG4 X Ram********************************\n");
        failcnt = 0;
        for (i = 0; i < 256; i++)
        {
            vWritePWMG4(A_PCADDR, i);
			vWritePWMG4(A_PCDATA, i+1);
            data = u4ReadPWMG4(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            if (data-i != 1)
                failcnt++;
        }
        Printf("Fail count %d (0: Pass)\n ", failcnt);
#endif

        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
#endif
    }
    else if (xy == 1)
    {
        vWritePWMG1(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG2(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG3(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#endif

        Printf("******************************PG1 Y Ram********************************\n");
        failcnt = 0;
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG1(A_PCADDR, 0x100 + i);
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
            vWritePWMG1(A_PCDATA, (i+1)<<8);
#else
            vWritePWMG1(A_PCADDR, YRAM_3X[i]);
#endif
            vWritePWMG1(A_PCADDR, 0x100 + i);
            data = u4ReadPWMG1(A_PCDATA);
            Printf("yram %d=", i);
			Printf("%d,  ",data>>8);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            data>>=8;
            if (data-i != 1)
                failcnt++;
        }
        Printf("\nFail count %d (0: Pass)\n ", failcnt);
        Printf("******************************PG2 Y Ram********************************\n");
        failcnt = 0;
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG2(A_PCADDR, 0x100 + i);
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
            vWritePWMG2(A_PCDATA, (i+1)<<8);
#else
            vWritePWMG2(A_PCADDR, YRAM_4X[i]);
#endif
            vWritePWMG2(A_PCADDR, 0x100 + i);
            data = u4ReadPWMG2(A_PCDATA);
            Printf("yram %d=", i);
			Printf("%d,  ",data>>8);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            data>>=8;
            if (data-i != 1)
                failcnt++;
        }
        Printf("\nFail count %d (0: Pass)\n ", failcnt);
        Printf("******************************PG3 Y Ram********************************\n");
        failcnt = 0;
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG3(A_PCADDR, 0x100 + i);
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
            vWritePWMG3(A_PCDATA, (i+1)<<8);
#else
            vWritePWMG3(A_PCADDR, YRAM_5X[i]);
#endif
            vWritePWMG3(A_PCADDR, 0x100 + i);
            data = u4ReadPWMG3(A_PCDATA);
            Printf("yram %d=", i);
			Printf("%d,  ",data>>8);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            data>>=8;
            if (data-i != 1)
                failcnt++;
        }
        Printf("\nFail count %d (0: Pass)\n ", failcnt);
#ifdef CC_MT5880
        Printf("******************************PG4 Y Ram********************************\n");
        failcnt = 0;
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG4(A_PCADDR, 0x100 + i);
            vWritePWMG4(A_PCDATA, (i+1)<<8);
            vWritePWMG4(A_PCADDR, 0x100 + i);
            data = u4ReadPWMG4(A_PCDATA);
            Printf("yram %d=", i);
			Printf("%d,  ",data>>8);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
            data>>=8;
            if (data-i != 1)
                failcnt++;
        }
        Printf("\nFail count %d (0: Pass)\n ", failcnt);
#endif

        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
#endif
    }
}


void _MT5398_XYRamRead(BOOL xy)
{
    UINT32 i, data;
    Printf("_MT5398_XYRamRead\n");
    if (xy == 0)
    {
        vWritePWMG1(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG2(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG3(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
#endif

        //vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
        //vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) & 0xfffffe));
        //vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) & 0xfffffe));

        Printf("******************************PG1 X Ram********************************\n");
        for (i = 0; i < 256; i++)
        {
            vWritePWMG1(A_PCADDR, i);
            //vWritePWMG1(A_PCDATA, i+1);
            data = u4ReadPWMG1(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG2 X Ram********************************\n");
        for (i = 0; i < 256; i++)
        {
            vWritePWMG2(A_PCADDR, i);
            //vWritePWMG2(A_PCADDR, i+1);
            data = u4ReadPWMG2(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG3 X Ram********************************\n");
        for (i = 0; i < 256; i++)
        {
            vWritePWMG3(A_PCADDR, i);
            //vWritePWMG3(A_PCADDR, i+1);
            data = u4ReadPWMG3(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }

#ifdef CC_MT5880
        Printf("******************************PG4 X Ram********************************\n");
        for (i = 0; i < 256; i++)
        {
            vWritePWMG4(A_PCADDR, i);
            //vWritePWMG3(A_PCADDR, i+1);
            data = u4ReadPWMG4(A_PCDATA);
            Printf("xram %d=", i);
            Printf("%d,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
#endif

        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
#endif
    }
    else if (xy == 1)
    {
        //vWritePWMG1(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        //vWritePWMG2(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        //vWritePWMG3(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) & 0xfffffe));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
#endif
        Printf("******************************PG1 Y Ram********************************\n");
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG1(A_PCADDR, 0x100 + i);
            //vWritePWMG1(A_PCADDR, YRAM_3X[i]);
            //data= u4ReadPWMG1(A_PCADDR);
            data = u4ReadPWMG1(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG2 Y Ram********************************\n");
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG2(A_PCADDR, 0x100 + i);
            //vWritePWMG2(A_PCADDR, YRAM_4X[i]);
            //data= u4ReadPWMG2(A_PCADDR);
            data = u4ReadPWMG2(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG3 Y Ram********************************\n");
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG3(A_PCADDR, 0x100 + i);
            //vWritePWMG3(A_PCADDR, YRAM_5X[i]);
            //data= u4ReadPWMG3(A_PCADDR);
            data = u4ReadPWMG3(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }

#ifdef CC_MT5880
        Printf("******************************PG4 Y Ram********************************\n");
        for (i = 0; i <= 0xD6; i++)
        {
            vWritePWMG4(A_PCADDR, 0x100 + i);
            //vWritePWMG3(A_PCADDR, YRAM_5X[i]);
            //data= u4ReadPWMG3(A_PCADDR);
            data = u4ReadPWMG4(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
#endif
        //vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        //vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        //vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) | PEN));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) | PEN));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) | PEN));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) | PEN));
#endif
    }
}
#endif  ///XY_RAM_TEST




void _MT5398_YRamRead(UINT8 staddr,UINT8 length)
{
    UINT32 i, data;
	UINT8 SUM=(staddr+length-1);
    Printf("_MT5398_XYRamRead\n");
  
        //vWritePWMG1(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        //vWritePWMG2(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        //vWritePWMG3(A_PGCTRL0, NAI_EN);    ///Enable PCADDR Auto-Increment
        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) & 0xfffffe));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
#endif
        Printf("******************************PG1 Y Ram********************************\n");
        for (i = staddr; i <= SUM; i++)
        {
            vWritePWMG1(A_PCADDR, 0x100 + i);
            //vWritePWMG1(A_PCADDR, YRAM_3X[i]);
            //data= u4ReadPWMG1(A_PCADDR);
            data = u4ReadPWMG1(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG2 Y Ram********************************\n");
        for (i = staddr; i <= SUM; i++)
        {
            vWritePWMG2(A_PCADDR, 0x100 + i);
            //vWritePWMG2(A_PCADDR, YRAM_4X[i]);
            //data= u4ReadPWMG2(A_PCADDR);
            data = u4ReadPWMG2(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
        Printf("******************************PG3 Y Ram********************************\n");
        for (i = staddr; i <= SUM; i++)
        {
            vWritePWMG3(A_PCADDR, 0x100 + i);
            //vWritePWMG3(A_PCADDR, YRAM_5X[i]);
            //data= u4ReadPWMG3(A_PCADDR);
            data = u4ReadPWMG3(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }

#ifdef CC_MT5880
        Printf("******************************PG4 Y Ram********************************\n");
        for (i = staddr; i <= SUM; i++)
        {
            vWritePWMG4(A_PCADDR, 0x100 + i);
            //vWritePWMG3(A_PCADDR, YRAM_5X[i]);
            //data= u4ReadPWMG3(A_PCADDR);
            data = u4ReadPWMG4(A_PCDATA);
            Printf("yram %d=", i);
            Printf("0x%x,  ", data);
            if ((i + 1) % 4 == 0)
            {
                Printf("\n");
            }
        }
#endif
        //vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        //vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        //vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) | PEN));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) | PEN));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) | PEN));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) | PEN));
#endif
}





void _MT5398_ADCDOWN(BOOL fgEnalbe)
{
    BOOL fInternalLDO = 0;
    UNUSED( fInternalLDO );
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_ADCDOWN: %d\n", fgEnalbe);
#endif
    if (fgEnalbe)
    {
        ANA_SET_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD));
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        ///VMID have low-frequency noise and DAC also have noise issue. (DAC refer VMID)
        ANA_SET_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);   ///Enable VCMGEN 1M Resister, before power on.
#else
        fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
        if ( fInternalLDO )
        {
            ///Internal LDO will refer VMID voltage.
            ANA_SET_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);     ///Enable VCMGEN 1M Resister, before power on.
        }
        else
        {
            ANA_CLR_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);   ///Disable VCMGEN 1M Resister, after power down.
        }
#endif
#ifdef CC_SUPPORT_STR
        ANA_CLR_BIT(REG_AADC_CFG1, PGA_EN);
#endif
    }
    else
    {
#ifdef CC_SUPPORT_STR
        ANA_SET_BIT(REG_AADC_CFG1, PGA_EN);
#endif
        ANA_SET_BIT(REG_AADC_CFG1, ADC_VCMGEN_1M_R_ON);   ///Enable VCMGEN 1M Resister, before power on.
        ANA_CLR_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD)); // power on
    }
}

#ifdef CC_SUPPORT_STR
void _MT5398_DACDOWN(BOOL fgEnable)
{
    if (fgEnable)
    {
        //ANALOG
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        /// No Need.
#elif defined (CC_MT5398)
        ANA_WRITE32(REG_PWMDAC_CFG5, 0x00150000);	///Kevin Suggest.
#elif defined (CC_MT5880)
        ANA_WRITE32(REG_PWMDAC_CFG5, 0x00150000);	/// Kevin Suggest.
#endif
        //DIGITAL
        vIO32WriteFldAlign(CKGEN_PWM_CKCFG, 1, FLD_PWM_CK_PD);
        vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, 1, FLD_PWM2_CK_PD);
        vIO32WriteFldAlign(CKGEN_PWM3_CKCFG, 1, FLD_PWM3_CK_PD);
        _fgPwmClkEnable = FALSE;
        fgPWMInit = FALSE;
    }
    else
    {
        //ANALOG
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        /// No Need.
#elif defined (CC_MT5398)
        ANA_WRITE32(REG_PWMDAC_CFG5, 0x00000000);	///Kevin Suggest.
#elif defined (CC_MT5880)
        ANA_WRITE32(REG_PWMDAC_CFG5, 0x1A820000);	/// Kevin Suggest.
#endif
        //DIGITAL
        vIO32WriteFldAlign(CKGEN_PWM_CKCFG, 0, FLD_PWM_CK_PD);
        vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, 0, FLD_PWM2_CK_PD);
        vIO32WriteFldAlign(CKGEN_PWM3_CKCFG, 0, FLD_PWM3_CK_PD);
        _fgPwmClkEnable = TRUE;
    }
}
#endif

void _MT5398_AADCDOWNCH(UINT8 u1CH, BOOL fgDown)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_ADCDOWNCH: ch(%d), fgDown(%d)\n", u1CH, fgDown);
#endif
    if (fgDown)
    {
        switch (u1CH)
        {
            case 0:  // PD L
                ANA_SET_BIT(REG_AADC_CFG0, ADC_L_PD );
                break;
            case 1: //PD R
                ANA_SET_BIT(REG_AADC_CFG0, ADC_R_PD );
                break;
            case 2: //PD L&R
                ANA_SET_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD));
                break;
            default:
                break;
        }
    }
    else
    {
        switch (u1CH)
        {
            case 0:  // Power on  L
                ANA_CLR_BIT(REG_AADC_CFG0, ADC_L_PD );
                break;
            case 1: //PO R
                ANA_CLR_BIT(REG_AADC_CFG0, ADC_R_PD );
                break;
            case 2: //PO L&R
                ANA_CLR_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD));
                break;
            default:
                break;
        }
    }
}

void _MT5398_AADCMute(UINT8 u1CH, BOOL fgMute)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_AADCMute:ch(%d), fgMute(%d)\n", u1CH, fgMute);
#endif

    if ( fgMute )
    {
        switch (u1CH)
        {
            case 0:   // mute L
                ANA_SET_BIT(REG_AADC_CFG0, ADC_L_MUTE);
                break;
            case 1:   //mute R
                ANA_SET_BIT(REG_AADC_CFG0, ADC_R_MUTE);
                break;
            case 2:    //mute L & R
                ANA_SET_BIT(REG_AADC_CFG0, ADC_L_MUTE | ADC_R_MUTE);
                break;
            default:
                break;
        }
    }
    else  //unmute
    {
        switch (u1CH)
        {
            case 0:   // unmute L
                ANA_CLR_BIT(REG_AADC_CFG0, ADC_L_MUTE);
                break;
            case 1:   //unmute R
                ANA_CLR_BIT(REG_AADC_CFG0, ADC_R_MUTE);
                break;
            case 2:   //unmute L & R
                ANA_CLR_BIT(REG_AADC_CFG0, ADC_L_MUTE | ADC_R_MUTE);
                break;
            default:
                break;
        }
    }
}

void _MT5398_ADCGainQuery(void)
{
    Printf("ADC gain : 0x%x\n", AUD_READ32(REG_INTADC_SDADC1));
}

void _MT5398_SetADCGain(UINT32 u4Value)
{
    AUD_WRITE32(REG_INTADC_SDADC1, u4Value);
}


///PWMDAC digital mute.
void _MT5398_MuteCLK(UINT8 u1DacId, BOOL fgMute)
{
    UNUSED(u1DacId);

    LOG(0, "[!!!PWM_FUNC!!!] _MT5398_MuteCLK: AUD(%d) fgMute = %d\n", u1DacId, fgMute);

    //DAC mute(can mute any clock pop noise, DC level from 3.3V down to 0V)
    if (fgMute) // Mute, fadeout
    {
#if defined(CC_MT5880)      //de-pop , after Turn on power stage, defined(CC_DAC_DE_POP) 
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);                   //00000
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : DC off set:\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
#endif

        /// A_POE = 0x333333 can't do fade-in / out.
        /// Need change back to 0x030303.
        vWritePWMG1(A_POE, P1EN | P0EN);
        vWritePWMG2(A_POE, P1EN | P0EN);
        vWritePWMG3(A_POE, P1EN | P0EN);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, P1EN | P0EN);
#endif

        //change fade-out to gpi Hi-Z Mode, to avoid pop noise when mute OPA
#if 1
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        ///Printf("[!!!PWM_FUNC!!!] MT5881 without 1KOhm mode.\n");
#else
        ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR0_1K_MODE)); // 1. change 1Kohm mode to 0Kohm mode
        ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR1_1K_MODE));
        ANA_WRITE32(REG_PWMDAC_CFG4, ANA_READ32(REG_PWMDAC_CFG4) & (~ALR2_1K_MODE));
#endif

#if defined (CC_MT5398)
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL2AR2);
#elif defined (CC_MT5881)
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL1);
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR1);
        /// To Do at Mastang
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL2);
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR2);
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));
#elif defined (CC_MT5399) || defined(CC_MT5882)
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2);
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));
#elif defined (CC_MT5890) 
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1);
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2);
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        //ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));        
#elif defined (CC_MT5880)
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR0);    //AR0~AR2, AL0~AR2 are bonded together
#endif
#else //fade-out
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_ON);
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x1F << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x1F << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x1F << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
#endif

    }
    else // UnMute, fadein (Already fade-out)
    {
        #if defined (CC_MT5398)
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL2AR2);
#elif defined (CC_MT5881)
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL1);
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR1);
        /// To Do at Mastang
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL2);
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR2);
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
#elif defined (CC_MT5399) || defined(CC_MT5882)
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL2AR2);
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
#elif defined(CC_MT5890)
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL1AR1);
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL2AR2);
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        //ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));        
#elif defined (CC_MT5880)
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR0);    //AR0~AR2, AL0~AR2 are bonded together
#endif

        ///For pwm async fifo enable and write/read pointer margin
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);

        // 1. A_POE pwm channel disable.
        vWritePWMG1(A_POE, 0x000000);
        vWritePWMG2(A_POE, 0x000000);
        vWritePWMG3(A_POE, 0x000000);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, 0x000000);
#endif

        // 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM
        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG1(A_PDATA, 0);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PDATA, 0);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PDATA, 0);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG4(A_PDATA, 0);
#endif

        // 3. Fade-out first - fast speed
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8)));
#if defined (CC_MT5881)
        if(IS_IC_5881_ES()) ///Remy digital pwmdac have bug(Hailong), need fix!!!
        {
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2); ///[11:0]=0 start from 0
        }
        else    ///if(IS_IC_5881_ES2())
        {
    #ifdef CC_ALR0_ALR1_PS90
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2); ///[11:0]=0 start from 0
    #else
            AUD_WRITE32(REG_PWMIP_MISC1, PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2); ///[11:0]=0 start from 0
    #endif
        }
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        AUD_WRITE32(REG_PWMIP_MISC1, PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2 | PWM_DIFF_PAIR_GP3); ///[11:0]=0 start from 0
#else
        AUD_WRITE32(REG_PWMIP_MISC1, 0);    ///[11:0]=0 start from 0
#endif
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x00 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));

        // 4. Delay time for fade-out using.
        x_thread_delay(1);

        // 5. A_POE pwm channel enable.
        // Set 0x030303
        vWritePWMG1(A_POE, P1EN | P0EN);
        vWritePWMG2(A_POE, P1EN | P0EN);
        vWritePWMG3(A_POE, P1EN | P0EN);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, P1EN | P0EN);
#endif

        // 6. Fade-in
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x05 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x05 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x05 << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
#if defined (CC_MT5881)
        if(IS_IC_5881_ES()) ///Remy digital pwmdac have bug(Hailong), need fix!!!
        {
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
        }
        else    ///if(IS_IC_5881_ES2())
        {
    #ifdef CC_ALR0_ALR1_PS90
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #else
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #endif
        }
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2 | PWM_DIFF_PAIR_GP3);
#else
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF);
#endif
    }
}


BOOL _fgDACStatus = FALSE; //after AC/DC on, the status is normal unmute
void _MT5398_Mute(UINT8 u1DacId, BOOL fgMute)
{
    //static BOOL fgInit = FALSE;
    UNUSED(u1DacId);

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_Mute: dec(%d) fgmute(%d)  dacstatus(%d):1-mute,0-unmute\n", u1DacId, fgMute, _fgDACStatus);
#endif

    if (!fgPWMInit)
    {
        return;
    }

    //if(fgInit&&(fgMute == _fgDACStatus))
    if (fgMute == _fgDACStatus)  //Befor DAC initial, _fgDAC Status shoud not change status
    {
        return;
    }
    //fgInit = TRUE;
    _fgDACStatus = fgMute;

    //PGA mute
    if (fgMute) // Mute
    {
#if defined(CC_MT5880)      /// mute: normal unmute --> soft mute --> normal mute, defined(CC_DAC_DE_POP) 
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);         //11011   normal unmute --> soft mute (k)
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (k)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
        x_thread_delay(100);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);         //internal slewing  01010 (f)
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (f)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
        x_thread_delay(100);                            // soft switch wait 100ms  to normal mute
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);         //normal mute  00010  (h)
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (h)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
#endif

        //turn on 0xf04
        ///Set 0x333333
        vWritePWMG1(A_POE, POZ_1 | POZ_0 | P1EN | P0EN);
        vWritePWMG2(A_POE, POZ_1 | POZ_0 | P1EN | P0EN);
        vWritePWMG3(A_POE, POZ_1 | POZ_0 | P1EN | P0EN);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, POZ_1 | POZ_0 | P1EN | P0EN);
#endif

    }
    else // UnMute
    {
#if defined(CC_MT5880)      ///unmute: normal mute --> soft unmute --> normal unmute, defined(CC_DAC_DE_POP) 
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);          //normal mute to internal slewing 11010 (j)
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (j)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
        x_thread_delay(25);                            // soft switch wait 100ms  to soft unmute
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);         //soft unmute 01011  (g)
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (g)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
        x_thread_delay(25);                             // soft switching wait 100ms to normal unmute
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);         //normal unmute 00011  (i)
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (i)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
#endif

        //turn off 0xf04
        ///Set 0x030303
        vWritePWMG1(A_POE, P1EN | P0EN);
        vWritePWMG2(A_POE, P1EN | P0EN);
        vWritePWMG3(A_POE, P1EN | P0EN);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, P1EN | P0EN);
#endif
    }
}

void _MT5398_MuteALRx(UINT8 u1ALRx, BOOL fgMute)
{
    if (!fgPWMInit)
    {
        return;
    }

    if (fgMute)
    {
        switch (u1ALRx)
        {
            case 0: //ALR0
                vWritePWMG1(A_POE, ((u4ReadPWMG1(A_POE)) | POZ_0 | POZ_1));
#ifdef CC_MT5880
                vWritePWMG4(A_POE, ((u4ReadPWMG4(A_POE)) | POZ_0 | POZ_1)); //Viper analog output
#endif
                break;
            case 1: //ALR1
                vWritePWMG2(A_POE, ((u4ReadPWMG2(A_POE)) | POZ_0 | POZ_1));
                break;
            case 2: //ALR2
                vWritePWMG3(A_POE, ((u4ReadPWMG3(A_POE)) | POZ_0 | POZ_1));
                break;
            case 3:
                break;
            case 4: //All
                vWritePWMG1(A_POE, ((u4ReadPWMG1(A_POE)) | POZ_0 | POZ_1));
#ifdef CC_MT5880
                vWritePWMG4(A_POE, ((u4ReadPWMG4(A_POE)) | POZ_0 | POZ_1)); //Viper analog output
#endif
                vWritePWMG2(A_POE, ((u4ReadPWMG2(A_POE)) | POZ_0 | POZ_1));
                vWritePWMG3(A_POE, ((u4ReadPWMG3(A_POE)) | POZ_0 | POZ_1));
                break;
            default:
                break;
        }
    }
    else
    {
        switch (u1ALRx)
        {
            case 0:
                vWritePWMG1(A_POE, ((u4ReadPWMG1(A_POE)) & 0xffffcf)); //[5:4] = 00'b
#ifdef CC_MT5880
                vWritePWMG4(A_POE, ((u4ReadPWMG4(A_POE)) & 0xffffcf)); //Viper analog output
#endif
                break;
            case 1: //ALR1
                vWritePWMG2(A_POE, ((u4ReadPWMG2(A_POE)) & 0xffffcf));
                break;
            case 2: //ALR2
                vWritePWMG3(A_POE, ((u4ReadPWMG3(A_POE)) & 0xffffcf));
                break;
            case 3:
                break;
            case 4: //All
                vWritePWMG1(A_POE, ((u4ReadPWMG1(A_POE)) & 0xffffcf)); //[5:4] = 00'b
#ifdef CC_MT5880
                vWritePWMG4(A_POE, ((u4ReadPWMG4(A_POE)) & 0xffffcf)); //Viper analog output
#endif
                vWritePWMG2(A_POE, ((u4ReadPWMG2(A_POE)) & 0xffffcf));
                vWritePWMG3(A_POE, ((u4ReadPWMG3(A_POE)) & 0xffffcf));
                break;
            default:
                break;
        }
    }
}


void _MT5398_InterPWMEnable(BOOL fgEnable)
{
    if (!fgPWMInit)
    {
        return;
    }
    if (fgEnable)
    {
        ///For pwm async fifo enable and write/read pointer margin
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);

        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) | PEN));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) | PEN));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) | PEN));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG4(A_PGCTRL0)) | PEN));
#endif
    }
    else
    {
        vWritePWMG1(A_PGCTRL0, ((u4ReadPWMG1(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG2(A_PGCTRL0, ((u4ReadPWMG2(A_PGCTRL0)) & 0xfffffe));
        vWritePWMG3(A_PGCTRL0, ((u4ReadPWMG3(A_PGCTRL0)) & 0xfffffe));
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, ((u4ReadPWMG4(A_PGCTRL0)) & 0xfffffe));
#endif
    }
}

//enable or disable internal LDO
//for Cobra/Viper/Python
void _MT5398_InterLDOEnable(BOOL fgEnable)
{
#if defined (CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    ///Printf("Remy without Internal LDO!!!\n");
    return;
#else
    if (!fgPWMInit)
    {
        return;
    }
    if (fgEnable) //enable internal LDO
    {
        ANA_SET_BIT(REG_PWMDAC_CFG2, AL0_ILDO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG2, AR0_ILDO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG3, AL1_ILDO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG3, AR1_ILDO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG3, AL2_ILDO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG3, AR2_ILDO_EN);
    }
    else  //disable internal LDO
    {
        ANA_CLR_BIT(REG_PWMDAC_CFG2, AL0_ILDO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG2, AR0_ILDO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG3, AL1_ILDO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG3, AR1_ILDO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG3, AL2_ILDO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG3, AR2_ILDO_EN);
    }
#endif    
}

UINT32 _MT5398_InterPWMGainGet(INT8 pgroup)
{
    UINT32 pgain = 0;
    if (!fgPWMInit)
    {
        return FALSE;
    }
    _MT5398_InterPWMEnable(FALSE);

    switch (pgroup)
    {
        case 1:
            /// PWMG1
            vWritePWMG1(A_PCADDR, CH0_GAIN);             ///AL0
            pgain = u4ReadPWMG1(A_PCDATA);
            LOG(5, "PWMDAC G1 Digital Gain = 0x%x. \n", pgain);
            break;
        case 2:
            /// PWMG2
            vWritePWMG2(A_PCADDR, CH0_GAIN);             ///AL1
            pgain = u4ReadPWMG2(A_PCDATA);
            LOG(5, "PWMDAC G2 Digital Gain = 0x%x. \n", pgain);
            break;
        case 3:
            /// PWMG3
            vWritePWMG3(A_PCADDR, CH0_GAIN);             ///AL2
            pgain = u4ReadPWMG3(A_PCDATA);
            LOG(5, "PWMDAC G3 Digital Gain = 0x%x. \n", pgain);
            break;
#ifdef CC_MT5880
        case 4:
            /// PWMG3
            vWritePWMG4(A_PCADDR, CH0_GAIN);             ///AL2
            pgain = u4ReadPWMG4(A_PCDATA);
            LOG(5, "PWMDAC G4 Digital Gain = 0x%x. \n", pgain);
            break;
#endif
        default:
            break;
    }
    _MT5398_InterPWMEnable(TRUE);
    return pgain;
}

void _MT5398_InterPWMGainSet(INT8 pgroup, INT32 pgain)
{
    if (!fgPWMInit)
    {
        return;
    }

    _MT5398_InterPWMEnable(FALSE);
    switch (pgroup)
    {
        case 1:
            ///PWMGroup1
            vWritePWMG1(A_PCADDR, CH0_GAIN);       ///AL0
            vWritePWMG1(A_PCDATA, pgain);
            vWritePWMG1(A_PCADDR, CH1_GAIN);       ///AR0
            vWritePWMG1(A_PCDATA, pgain);
#ifdef CC_MT5880  //for HP analog output : Viper & Python
            ///PWM Group4
            vWritePWMG4(A_PCADDR, CH0_GAIN);
            vWritePWMG4(A_PCDATA, ((~pgain) + 0x1));
            vWritePWMG4(A_PCADDR, CH1_GAIN);
            vWritePWMG4(A_PCDATA, ((~pgain) + 0x1));
#endif
            break;
        case 2:
            ///PWM Group2
            vWritePWMG2(A_PCADDR, CH0_GAIN);       ///AL1
            vWritePWMG2(A_PCDATA, pgain);
            vWritePWMG2(A_PCADDR, CH1_GAIN);       ///AR1
            vWritePWMG2(A_PCDATA, pgain);
            break;
        case 3:
            ///PWM Group3
            vWritePWMG3(A_PCADDR, CH0_GAIN);       ///AL2
            vWritePWMG3(A_PCDATA, pgain);
            vWritePWMG3(A_PCADDR, CH1_GAIN);       ///AR2
            vWritePWMG3(A_PCDATA, pgain);
            break;

        case 5:                                    //all group
            //all group
            vWritePWMG1(A_PCADDR, CH0_GAIN);       ///AL0
            vWritePWMG1(A_PCDATA, pgain);
            vWritePWMG1(A_PCADDR, CH1_GAIN);       ///AR0
            vWritePWMG1(A_PCDATA, pgain);
            vWritePWMG2(A_PCADDR, CH0_GAIN);       ///AL1
            vWritePWMG2(A_PCDATA, pgain);
            vWritePWMG2(A_PCADDR, CH1_GAIN);       ///AR1
            vWritePWMG2(A_PCDATA, pgain);
            vWritePWMG3(A_PCADDR, CH0_GAIN);       ///AL2
            vWritePWMG3(A_PCDATA, pgain);
            vWritePWMG3(A_PCADDR, CH1_GAIN);       ///AR2
            vWritePWMG3(A_PCDATA, pgain);
#ifdef CC_MT5880
            vWritePWMG4(A_PCADDR, CH0_GAIN);
            vWritePWMG4(A_PCDATA, ((~pgain) + 0x1));
            vWritePWMG4(A_PCADDR, CH1_GAIN);
            vWritePWMG4(A_PCDATA, ((~pgain) + 0x1));
#endif
            break;
        default:
            break;
    }

    _MT5398_InterPWMEnable(TRUE);
}


  
  void _MT5398_InterMonitorSet(INT8 pgroup, INT8 Monitor_ID)
  {
	  if (!fgPWMInit)
	  {
		  return;
	  }
	  _MT5398_InterPWMEnable(FALSE);
	  if(Monitor_ID==1)
		  {switch (pgroup)
		  {
			  case 1:
				  ///PWMGroup1
	       		IO_WRITE32(0x00000000, 0xf0008094, 0x00000000);
		   		IO_WRITE32(0x00000000, 0xf000d580, 0x0000000F);
				  break;
			  case 2:
				  ///PWM Group2
			  	IO_WRITE32(0x00000000, 0xf0008094, 0x00000008);
			  	IO_WRITE32(0x00000000, 0xf000d580, 0x0000000F);
				  break;
			  case 3:
				  ///PWM Group3
				IO_WRITE32(0x00000000, 0xf0008094, 0x0000000A);
				IO_WRITE32(0x00000000, 0xf000d580, 0x0000000F);
				  break;
			  default:
				  break;
	  		}
	  	}
  	 else
		{switch (pgroup)
		  {
			  case 1:
				  ///PWMGroup1
	       		IO_WRITE32(0x00000000, 0xf0008094, 0x00000000);
		   		IO_WRITE32(0x00000000, 0xf000d584, 0x0000000F);
				  break;
			  case 2:
				  ///PWM Group2
			  	IO_WRITE32(0x00000000, 0xf0008094, 0x00000008);
			  	IO_WRITE32(0x00000000, 0xf000d584, 0x0000000F);
				  break;
			  case 3:
				  ///PWM Group3
				IO_WRITE32(0x00000000, 0xf0008094, 0x0000000A);
				IO_WRITE32(0x00000000, 0xf000d584, 0x0000000F);
				  break;
			  default:
				  break;
	  		}			
  	 	}
	  _MT5398_InterPWMEnable(TRUE);
  }
 
 
 
 void _MT5398_InterPWMDelay(INT8 pgroup, INT32 pwmdelay)
 {
	 if (!fgPWMInit)
	 {
		 return;
	 }
 
	 _MT5398_InterPWMEnable(FALSE);
	 switch (pgroup)
	 {
		 case 1:
			 ///PWMGroup1
			 vWritePWMG1(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG1(A_P1PIN, pwmdelay);
			 vWritePWMG1(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG1(A_P3PIN, 0x600); 
#ifdef CC_MT5880  //for HP analog output : Viper & Python
			 ///PWM Group4
			 vWritePWMG4(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG4(A_P1PIN, pwmdelay);
			 vWritePWMG4(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG4(A_P3PIN, 0x600);

#endif
			 break;
		 case 2:
			 ///PWM Group2
			 vWritePWMG2(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG2(A_P1PIN, pwmdelay);
			 vWritePWMG2(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG2(A_P3PIN, 0x600);
			 break;
		 case 3:
			 ///PWM Group3
			 vWritePWMG3(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG3(A_P1PIN, pwmdelay);
			 vWritePWMG3(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG3(A_P3PIN, 0x600);
			 break;
 
		 case 5:									//all group
			 //all group
			 vWritePWMG1(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG1(A_P1PIN, pwmdelay);
			 vWritePWMG1(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG1(A_P3PIN, 0x600);
			 vWritePWMG2(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG2(A_P1PIN, pwmdelay);
			 vWritePWMG2(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG2(A_P3PIN, 0x600);
			 vWritePWMG3(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG3(A_P1PIN, pwmdelay);
			 vWritePWMG3(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG3(A_P3PIN, 0x600);

#ifdef CC_MT5880
			 vWritePWMG4(A_P0PIN, pwmdelay);		///AL0
			 vWritePWMG4(A_P1PIN, pwmdelay);
			 vWritePWMG4(A_P2PIN, 0x600);		    ///AR0
			 vWritePWMG4(A_P3PIN, 0x600);
#endif
			 break;
		 default:
			 break;
	 }
 
	 _MT5398_InterPWMEnable(TRUE);
 }
 
 void _MT5398_InterPWMDACRead(INT8 pgroup,UINT32 pwmdac_addr)
 {
	 UINT32 pgain = 0;
 	 switch (pgroup)
	 {
		 case 1:
			 /// PWMG1
			 pgain = u4ReadPWMG1(pwmdac_addr);
			 Printf("Group1 PWMDAC Reg:0x%x=0x%x\n", pwmdac_addr,pgain);
			 break;
		 case 2:
			 /// PWMG2
			 pgain = u4ReadPWMG2(pwmdac_addr);
			 Printf("Group2 PWMDAC Reg:0x%x=0x%x\n", pwmdac_addr,pgain);
			 break;
		 case 3:
			 /// PWMG3
			 pgain = u4ReadPWMG3(pwmdac_addr);
			 Printf("Group3 PWMDAC Reg:0x%x=0x%x\n", pwmdac_addr,pgain);
			 break;
#ifdef CC_MT5880
		 case 4:
			 /// PWMG3
			 pgain = u4ReadPWMG4(pwmdac_addr);
			 Printf("Group4 PWMDAC Reg:0x%x=0x%x\n", pwmdac_addr,pgain);
			 break;
#endif
		 default:
			 break;
	 }
 }

 
 void _MT5398_InterPWMDACWrite(INT8 pgroup, UINT32 pwmdac_addr,UINT32 pwmdac_data)
 {
	 if (!fgPWMInit)
	 {
		 return;
	 }
 
	 _MT5398_InterPWMEnable(FALSE);
	 switch (pgroup)
	 {
		 case 1:
			 vWritePWMG1(pwmdac_addr, pwmdac_data);		

#ifdef CC_MT5880  //for HP analog output : Viper & Python
			 ///PWM Group4
			 vWritePWMG4(pwmdac_addr, pwmdac_data);
#endif
			 break;
		 case 2:
			 ///PWM Group2
			 vWritePWMG2(pwmdac_addr, pwmdac_data);	
			 break;
		 case 3:
			 ///PWM Group3
			 vWritePWMG3(pwmdac_addr, pwmdac_data);	
			 break;
 
		 case 5:									//all group
			 //all group
			 vWritePWMG1(pwmdac_addr, pwmdac_data);  
			 vWritePWMG2(pwmdac_addr, pwmdac_data);  
			 vWritePWMG3(pwmdac_addr, pwmdac_data);  


#ifdef CC_MT5880
			 vWritePWMG4(pwmdac_addr, pwmdac_data);

#endif
			 break;
		 default:
			 break;
	 }
 
	 _MT5398_InterPWMEnable(TRUE);
 }



 void _MT5398_YRAMWrite(INT8 pgroup,UINT32 YRAM_Addr, UINT32 YRAM_Data)
 {
	 if (!fgPWMInit)
	 {
		 return;
	 }
 
	 _MT5398_InterPWMEnable(FALSE);
	 switch (pgroup)
	 {
		 case 1:
			 ///PWMGroup1
			 vWritePWMG1(A_PCADDR, YRAM_Addr);		
			 vWritePWMG1(A_PCDATA, YRAM_Data);
 
#ifdef CC_MT5880  //for HP analog output : Viper & Python
			 ///PWM Group4
			 vWritePWMG4(A_PCADDR, CH0_GAIN);
			 vWritePWMG4(A_PCDATA, ((~YRAM_Data) + 0x1));
 
#endif
			 break;
		 case 2:
			 ///PWM Group2
			 vWritePWMG2(A_PCADDR, YRAM_Addr);		
			 vWritePWMG2(A_PCDATA, YRAM_Data);
			 break;
		 case 3:
			 ///PWM Group3
			 vWritePWMG3(A_PCADDR, YRAM_Addr);		
			 vWritePWMG3(A_PCDATA, YRAM_Data);
			 break;
 
		 case 5:									//all group
			 //all group
			 vWritePWMG1(A_PCADDR, YRAM_Addr);		
			 vWritePWMG1(A_PCDATA, YRAM_Data);			 
			 vWritePWMG2(A_PCADDR, YRAM_Addr);		
			 vWritePWMG2(A_PCDATA, YRAM_Data);			
			 vWritePWMG3(A_PCADDR, YRAM_Addr);		
			 vWritePWMG3(A_PCDATA, YRAM_Data);

#ifdef CC_MT5880
			 vWritePWMG4(A_PCADDR, YRAM_Addr);
			 vWritePWMG4(A_PCDATA, ((~YRAM_Data) + 0x1));
#endif
			 break;
		 default:
			 break;
	 }
 
	 _MT5398_InterPWMEnable(TRUE);
 }
 

void _MT5398_PWMSigGen(INT8 SigType)
{       UINT32 SigType_Reg = 0;
		switch(SigType)
		 { case 1:
				SigType_Reg = 0x03111111;
				Printf("Signal Type ID %d: 1K Tone\n",SigType);
				break;
		   case 2:
				SigType_Reg = 0x03222222;
				Printf("Signal Type ID %d: 2K Tone\n",SigType);
				break;
		   case 3:
				SigType_Reg = 0x03333333;
				Printf("Signal Type ID %d: 3K Tone\n",SigType);
				break;
		   case 4:
				SigType_Reg = 0x03444444;
				Printf("Signal Type ID %d: 4K Tone\n",SigType);
				break;
		   case 5:
				SigType_Reg = 0x03555555;
				Printf("Signal Type ID %d: 6K Tone\n",SigType);
				break;
		   case 6:
				SigType_Reg = 0x03666666;
				Printf("Signal Type ID %d: 8K Tone\n",SigType);
				break;
		   case 7:
				SigType_Reg = 0x03777777;
				Printf("Signal Type ID %d: 12K Tone\n",SigType);
				break;
		   case 8:
				SigType_Reg = 0x03888888;
				Printf("Signal Type ID %d: 16K Tone\n",SigType);
				break;
		   case 9:
				SigType_Reg = 0x999999;
				Printf("Signal Type ID %d: Duty=100%\n",SigType);
				break;
		   case 10:
				SigType_Reg = 0xaaaaaa;
				Printf("Signal Type ID %d: Duty=75%\n",SigType);
				break;
		   case 11:
				SigType_Reg = 0xbbbbbb;
				Printf("Signal Type ID %d: Duty=0%\n",SigType);
				break;
		   case 12:
				SigType_Reg = 0xcccccc;
				Printf("Signal Type ID %d: Duty=25%\n",SigType);
				break;
		   default:
				break;
		  }
		
		    _MT5398_InterPWMEnable(0);
			AUD_WRITE32(REG_PWMCFG0_PG1, 0xfa0292f0);
			AUD_WRITE32(REG_PWMCFG0_PG2, 0xfa0292f0);
			AUD_WRITE32(REG_PWMCFG0_PG3, 0xfa0292f0);
			AUD_WRITE32(RREG_PWMIP_ABIST_CH,SigType_Reg);
			_MT5398_InterPWMEnable(1);
}

void _MT5398_InterDacEnable(BOOL fgEnable)
{

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_InterDacEnable: %d\n", fgEnable);
#endif

    if (!fgPWMInit)
    {
        return;
    }
#if defined (CC_MT5398)
    //PGA mute
    if (!fgEnable) // Mute
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL0AR0);
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL2AR2);
    }
    else // UnMute
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL0AR0);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX4, 0, FLD_PAD_AL2AR2);
    }
#elif defined (CC_MT5881)
    if (!fgEnable) // Mute
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL1);
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR1);
        /// To Do at Mastang
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL2);
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR2);
        ///w 0xf0060108 0x00000000, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));
    }
    else // UnMute
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR0);
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL1);
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR1);
        /// To Do at Mastang
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL2);
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AR2);
        ///w 0xf0060108 0x00000555, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
    }
#elif defined (CC_MT5399) || defined(CC_MT5882)
    if (!fgEnable) // Mute
    {
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2);
        ///w 0xf0060108 0x00000000, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));
    }
    else // UnMute
    {
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL1AR1);
        vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL2AR2);
        ///w 0xf0060108 0x00000555, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
    }    
#elif defined(CC_MT5890) 
    if (!fgEnable) // Mute
    {
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1);
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2);
        ///w 0xf0060108 0x00000000, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, 0);
        //ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN | AL1_GPI_EN | AR1_GPI_EN | AL0_GPI_EN | AR0_GPI_EN)));
    }
    else // UnMute
    {
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL0AR0); // 2. set gpi hi-z
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL1AR1);
        //vIO32WriteFldAlign(CKGEN_PMUX6, 0, FLD_PAD_AL2AR2);
        ///w 0xf0060108 0x00000555, for CLI use no need De-pop flowchart.
        ANA_WRITE32(REG_ADAC_CFG2, (ACT_ENVO_CH2 | ACT_PLAY_CH2) | (ACT_ENVO_CH1 | ACT_PLAY_CH1) | (ACT_ENVO_CH0 | ACT_PLAY_CH0));
        //ANA_WRITE32(REG_ADAC_CFG5, (CKF_ST_DIV_16384 | RAMP_INV_DIS | DAC_DESENSE_CH1));
    }        
#elif defined (CC_MT5880)
    //PGA mute
    if (!fgEnable) // Mute
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL0);
    }
    else // UnMute
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 0, FLD_PAD_AL0);
    }

#endif
}

void _MT5398_AdcFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_AdcFmtCfg: %d %d\n", eFormat, eMclk);
#endif

    switch (eFormat)
    {
        case FORMAT_LJ:
            break;
        case FORMAT_I2S:
            break;
        case FORMAT_RJ:
            break;
        default:
            break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
            break;
        case MCLK_256FS:
            break;
        case MCLK_384FS:
            break;
        case MCLK_512FS:
            break;
        default:
            break;
    }

}

void _MT5398_DacFmtCfg(DATA_FORMAT_T eFormat, MCLK_FREQUENCY_T eMclk)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_DacFmtCfg: %d %d\n", eFormat, eMclk);
#endif

    switch (eFormat)
    {
        case FORMAT_LJ:
            break;
        case FORMAT_I2S:
            break;
        case FORMAT_RJ:
            break;
        default:
            break;
    }

    switch (eMclk)
    {
        case MCLK_128FS:
            break;
        case MCLK_256FS:
            break;
        case MCLK_384FS:
            break;
        case MCLK_512FS:
            break;
        default:
            break;
    }

}

void _MT5398_ChlSel(UINT8 u1Indx)
{
    UINT32 u4Reg;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_ChlSel: %d\n", u1Indx);
#endif
    #if defined(CC_MT5890) 
        switch (u1Indx)
        {
            case 0:
                u4Reg = ADC_IN0L0R;
                break;
            case 1:
                u4Reg = ADC_IN1L1R;
                break;
            case 2:
                u4Reg = ADC_IN2L2R;
                break;
            case 3:
                u4Reg = ADC_IN3L3R;
                break;
            default:
                u4Reg = ADC_IN0L0R;
                break;
        }
    #else
        switch (u1Indx)
            {
                case 0:
                    u4Reg = ADC_IN0L0R;
                    break;
                case 1:
                    u4Reg = ADC_IN1L1R;
                    break;
                case 2:
                    u4Reg = ADC_IN2L2R;
                    break;
                case 3:
                    u4Reg = ADC_IN3L3R;
                    break;
                case 4:
                    u4Reg = ADC_IN4L4R;
                    break;       
                case 5:
                    u4Reg = ADC_IN5L5R;
                    break;
                case 6:
                    u4Reg = ADC_IN6L6R;
                    break;
                default:
                    u4Reg = ADC_IN0L0R;
                    break;
            }
    #endif    
    ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~(ADC_INPUT_MUX_MASK)) | u4Reg);
}

void _MT5398_DacChlSel(UINT8 u1DacId, AUD_CHL_DEF_T eChl)
{
    UINT32 u4DacPos, u4DacSel;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_DacChlSel: %d %d\n", u1DacId, eChl);
#endif

    if (u1DacId == INTERNAL_DAC_ID_MAIN)
    {
        u4DacPos = DAC0_SEL_POS;
    }
    else if (u1DacId == INTERNAL_DAC_ID_AUX)
    {
        u4DacPos = DAC1_SEL_POS;
    }
    else if (u1DacId == INTERNAL_DAC_ID_THIRD)
    {
        u4DacPos = DAC2_SEL_POS;
    }
    else
    {
        u4DacPos = DAC3_SEL_POS;
    }

    if (eChl == AUD_CHL_L_R)
    {
        u4DacSel = DAC_SEL_0_LR;
    }
    else if (eChl == AUD_CHL_LS_RS)
    {
        u4DacSel = DAC_SEL_0_SLR;
    }
    else if (eChl == AUD_CHL_C_SW)
    {
        u4DacSel = DAC_SEL_0_CLFE;
    }
    else if (eChl == AUD_CHL_IEC)
    {
        u4DacSel = DAC_SEL_0_CH78;
    }
    else if (eChl == AUD_CHL_DOWNMIX)
    {
        u4DacSel = DAC_SEL_0_CH90;
    }
    else if (eChl == AUD_CHL_BYPASS)
    {
        u4DacSel = DAC_SEL_0_CH1112;
    }
    else if (eChl == AUD_CHL_EXTCH)
    {
        u4DacSel = DAC_SEL_0_CH1314;
    }
    else if (eChl == AUD_CHL_AUX)
    {
        u4DacSel = DAC_SEL_1_LR;
    }
    else
    {
        u4DacSel = DAC_SEL_1_LR;
    }

    u4DacSel = u4DacSel << u4DacPos;

    AUD_WRITE32(REG_IDACHCG,
                ((AUD_READ32(REG_IDACHCG) & (~(DAC_SEL_MASK << u4DacPos))) | u4DacSel));

#if defined(CC_MT5881)
    if(IS_IC_5881_ES())
    {
        /// PWMGP1 and PWMGP2 input must use the same dsp channel
        Printf("[Remy_Ver_A]For Remy IC Bug, PWMGP1 and PWMGP2 input must use the same dsp channel!!!\n");
        u4DacSel = (AUD_READ32(REG_IDACHCG) & (DAC_SEL_MASK << DAC0_SEL_POS));
        AUD_WRITE32(REG_IDACHCG,
                    ((u4DacSel << DAC1_SEL_POS) | u4DacSel));
    }
#endif
}


static CHAR* _aszDacCh[] = {"LR", "SLR", "CLFE", "IEC", "HP", "Monitor", "Ext1314", "AUX"};
void _MT5398_DacChlSelQuery(void)
{
    UINT8 u1Index;
    UINT32 u4RegVal = AUD_READ32(REG_IDACHCG);

    u1Index = (u4RegVal >> DAC0_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 8)
    {
        Printf("DAC0: %s\n", _aszDacCh[u1Index]);
    }
    u1Index = (u4RegVal >> DAC1_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 8)
    {
        Printf("DAC1: %s\n", _aszDacCh[u1Index]);
    }
    u1Index = (u4RegVal >> DAC2_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 8)
    {
        Printf("DAC2: %s\n", _aszDacCh[u1Index]);
    }
    u1Index = (u4RegVal >> DAC3_SEL_POS)&DAC_SEL_MASK;
    if (u1Index < 8)
    {
        Printf("DAC3: %s\n", _aszDacCh[u1Index]);
    }
}

CODEC_T* _CODEC_GetMT5398Tbl(void)
{
    return &(_rCodecMT5398);
}

#if 0   // Unused
void _MT5398_PWM_SetSamplingRate(UINT8 u1ClkFmt)
{
#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_PWM_SetSamplingRate: %d\n", u1ClkFmt);
#endif
    _MT5398_Init_all(u1ClkFmt, FALSE);
}
#endif

static void _MT5398_Init_all(UINT8 u1ClkFmt, BOOL fgInit)
{
    BOOL fPhaseInverse = 0;
    BOOL fInternalLDO = 0;
    //UINT32 PWMGain = 0;
    //UINT32 PWMGain_N = 0;
    UINT32 PWMGain_3X = 0;
    UINT32 PWMGain_3X_N = 0;
    UINT32 PWMGain_N3X = 0;
    UINT32 PWMGain_N3X_N = 0;
    UINT32 u4TempReg1 = 0;

    AOUT_CFG_T* prAudOutFmt;
    UNUSED(fInternalLDO);
    UNUSED(u4TempReg1);

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_Init_all: %d %d\n", u1ClkFmt, fgInit);
#endif

    if (fgInit)
    {
#if defined(CC_MT5880)      /// for de-pop : start output slewing (d), defined(CC_DAC_DE_POP) 
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (d)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
        //x_thread_delay(500);                             //de-pop for output slewing wait  //500ms
#endif
    if(IS_IC_5890_ES1())
    {
        vIO32WriteFldAlign(CKGEN_PWM_CKCFG, PWM_CK_SEL_APLL, FLD_PWM_CK_SEL);  //CLK Gen to DAC APLL
        vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, PWM_CK_SEL_APLL, FLD_PWM2_CK_SEL);  //CLK Gen to DAC APLL
        vIO32WriteFldAlign(CKGEN_PWM3_CKCFG, PWM_CK_SEL_APLL, FLD_PWM3_CK_SEL);  //CLK Gen to DAC APLL
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_PWM_CKCFG, PWM_CK_SEL, FLD_PWM_CK_SEL);  //CLK Gen to DAC APLL
        vIO32WriteFldAlign(CKGEN_PWM2_CKCFG, PWM_CK_SEL, FLD_PWM2_CK_SEL);  //CLK Gen to DAC APLL
        vIO32WriteFldAlign(CKGEN_PWM3_CKCFG, PWM_CK_SEL, FLD_PWM3_CK_SEL);  //CLK Gen to DAC APLL
    }
#ifdef LOG_REG_WRITE
        Printf("[!!!PWM!!!] I2C_EXT 58 0000d244[3:0] %08x RW\n", PWM_CK_SEL);
        Printf("[!!!PWM!!!] I2C_EXT 58 0000d248[3:0] %08x RW\n", PWM_CK_SEL);
#endif
        ///Set PWMDAC Pad Config
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        /// 0xf00051EC = 0x00543210
        /// 0xf0005248 = 0x00BA9876
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG2, PWM_PAD_INIT2);
#elif defined(CC_MT5881)
        if(IS_IC_5881_ES())
        {
            AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT_ES);
        }
        else    ///if(IS_IC_5881_ES2())
        {
            AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
        }
#else
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
#endif

        ///For pwm async fifo enable and write/read pointer margin
        ///before A_PGCTRL0 0xF01[0]=1
        AUD_WRITE32(REG_PWMIP_MON_PG1, 0x00);
        AUD_WRITE32(REG_PWMIP_MON_PG1, PWMIP_MON_PG1_INIT);

#if defined (CC_MT5398)
        u4TempReg1 = (PWMPAD_CFG_INIT);
        if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL0AR0) != 0)
        {
            u4TempReg1 = (u4TempReg1 & 0xFFFFFFF3);
        }
        if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL1AR1) != 0)
        {
            u4TempReg1 = (u4TempReg1 & 0xFFFFFFCF);
        }
        if (IO32ReadFldAlign(CKGEN_PMUX4, FLD_PAD_AL2AR2) != 0)
        {
            u4TempReg1 = (u4TempReg1 & 0xFFFFFF3F);
        }
        ANA_WRITE32(REG_PWMDAC_CFG0, u4TempReg1);
        LOG(0, "Initial REG_PWMDAC_CFG0=0x%x. \n", u4TempReg1);
#elif defined (CC_MT5881) || defined (CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        ///Remy only have GPI, without GPO.
        ///Already set REG_ADAC_CFG0 before.
#else
        ///Re-latch setting
        ANA_WRITE32(REG_PWMDAC_CFG0, PWMPAD_CFG_INIT);      /// Define by CC_AUD_PWM_SAWLESS_PLL_EN
#endif
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, 0);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, 0);
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, 0);
#if defined(CC_MT5881)
        if(IS_IC_5881_ES()) ///Remy digital pwmdac have bug(Hailong), need fix!!!
        {
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
        }
        else    ///if(IS_IC_5881_ES2())
        {
    #ifdef CC_ALR0_ALR1_PS90
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #else
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #endif
        }
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2 | PWM_DIFF_PAIR_GP3);
#elif defined(CC_MT5880)
        AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON);
#else
        AUD_WRITE32(REG_PWMIP_MISC1, 0);
#endif

        UNUSED(DRVCUST_OptQuery(eAudioOutFmtTable, (UINT32*)(void*)&prAudOutFmt));
        if (prAudOutFmt->eFormat == FORMAT_I2S)
        {
            //I2S Mode for 0 degree phase error.
            //51C8/51CC/51E0: bit23,bit9 = 1
            //5008:181E: bit2 = 1 for I2S Mode
            AUD_WRITE32(REG_PWMCFG0_PG1, 0xfa029200);
            AUD_WRITE32(REG_PWMCFG0_PG2, 0xfa029200);
            AUD_WRITE32(REG_PWMCFG0_PG3, 0xfa029200);
        }
        else
        {
            //LJ/RJ Mode for 0 degree phase error.
            //51C8/51CC/51E0: bit23,bit9 = 0
            //5008:181A / 1812 : bit2 = 0 for LJ/RJ Mode
            AUD_WRITE32(REG_PWMCFG0_PG1, 0xfa029000);
            AUD_WRITE32(REG_PWMCFG0_PG2, 0xfa029000);
            AUD_WRITE32(REG_PWMCFG0_PG3, 0xfa029000);
        }

#if defined(CC_MT5881)
    if(IS_IC_5881_ES())
    {
        vWritePWMG1(A_P0PIN, C_P0PIN0_ES1);
        vWritePWMG1(A_P1PIN, C_P1PIN0_ES1);
        vWritePWMG1(A_P2PIN, C_P2PIN0_ES1);
        vWritePWMG1(A_P3PIN, C_P3PIN0_ES1);
        vWritePWMG1(A_POE, P1EN | P0EN);

        vWritePWMG2(A_P0PIN, C_P0PIN0_ES1);
        vWritePWMG2(A_P1PIN, C_P1PIN0_ES1);
        vWritePWMG2(A_P2PIN, C_P2PIN0_ES1);
        vWritePWMG2(A_P3PIN, C_P3PIN0_ES1);
        vWritePWMG2(A_POE, P1EN | P0EN);

        vWritePWMG3(A_P0PIN, C_P0PIN0_ES1);
        vWritePWMG3(A_P1PIN, C_P1PIN0_ES1);
        vWritePWMG3(A_P2PIN, C_P2PIN0_ES1);
        vWritePWMG3(A_P3PIN, C_P3PIN0_ES1);
        vWritePWMG3(A_POE, P1EN | P0EN);
    }
    else    ///if(IS_IC_5881_ES2())
#endif
    {
#if defined(CC_MT5881) && defined(CC_ALR0_ALR1_PS90)
        vWritePWMG1(A_P0PIN, C_P0PIN0_PS90);
        vWritePWMG1(A_P1PIN, C_P1PIN0_PS90);
        vWritePWMG1(A_P2PIN, C_P2PIN0_PS90);
        vWritePWMG1(A_P3PIN, C_P3PIN0_PS90);
#else
        vWritePWMG1(A_P0PIN, C_P0PIN0);
        vWritePWMG1(A_P1PIN, C_P1PIN0);
        vWritePWMG1(A_P2PIN, C_P2PIN0);
        vWritePWMG1(A_P3PIN, C_P3PIN0);
#endif
        vWritePWMG1(A_POE, P1EN | P0EN);

        vWritePWMG2(A_P0PIN, C_P0PIN0);
        vWritePWMG2(A_P1PIN, C_P1PIN0);
        vWritePWMG2(A_P2PIN, C_P2PIN0);
        vWritePWMG2(A_P3PIN, C_P3PIN0);
        vWritePWMG2(A_POE, P1EN | P0EN);

        vWritePWMG3(A_P0PIN, C_P0PIN0);
        vWritePWMG3(A_P1PIN, C_P1PIN0);
        vWritePWMG3(A_P2PIN, C_P2PIN0);
        vWritePWMG3(A_P3PIN, C_P3PIN0);
        vWritePWMG3(A_POE, P1EN | P0EN);
    }

#ifdef CC_MT5880
        vWritePWMG4(A_P0PIN, C_P4PIN0);
        vWritePWMG4(A_P1PIN, C_P5PIN0);
        vWritePWMG4(A_P2PIN, C_P6PIN0);
        vWritePWMG4(A_P3PIN, C_P7PIN0);
        vWritePWMG4(A_POE, P1EN | P0EN);
#endif
    }

    ////////////////////////////////////////////////////////////////
    fInternalLDO = (BOOL)DRVCUST_OptGet(eAudioInternalLDOEnable);
    //mute before turn of F01
#if defined(CC_MT5880)  ///for 5880, defined(CC_DAC_DE_POP) 
    if (fgInit)
    {
        vWritePWMG1(A_PGCTRL1, PGCTRL1_INIT_432);
        vWritePWMG2(A_PGCTRL1, PGCTRL1_INIT_576); ///PGCTRL1_INIT_384);
        vWritePWMG3(A_PGCTRL1, PGCTRL1_INIT_480);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL1, PGCTRL1_INIT_432);
#endif

        //for Viper HP depop: at present, pwmdac channel is enable (0xf04 = 0x03), but PWMDAC is disable(0xf01[0]=0). so, there is not output.

        // 1. A_POE pwm channel disable.

        ///[Joel 20100610] For PWM ECO part - sync ADC & PWMDAC square rise edge.
        //vWritePWMG1(0xDFF, 0x01);
        //vWritePWMG2(0xDFF, 0x01);

        // 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM

        // 3. Fade-out first
    }
    else
    {
        _MT5398_Mute(0, TRUE);
    }
#else  //for 5398,5881 ...
    if (fgInit)
    {
#if defined (CC_MT5881) || defined (CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    #ifdef CC_SUPPORT_96K_AOUT
        vWritePWMG1(A_PGCTRL1, PGCTRL1_INIT_1440_96K);
        vWritePWMG2(A_PGCTRL1, PGCTRL1_INIT_1440_96K);
        vWritePWMG3(A_PGCTRL1, PGCTRL1_INIT_1440_96K);
    #else 
        if (IS_IC_5890_ES1())
        {
            vWritePWMG1(A_PGCTRL1, PGCTRL1_INIT_1152);
            vWritePWMG2(A_PGCTRL1, PGCTRL1_INIT_1152);
            vWritePWMG3(A_PGCTRL1, PGCTRL1_INIT_1152);
        }
        else
        {
            vWritePWMG1(A_PGCTRL1, PGCTRL1_INIT_1440);
            vWritePWMG2(A_PGCTRL1, PGCTRL1_INIT_1440);
            vWritePWMG3(A_PGCTRL1, PGCTRL1_INIT_1440);
        }
    #endif
#else
        vWritePWMG1(A_PGCTRL1, PGCTRL1_INIT_432);
        vWritePWMG2(A_PGCTRL1, PGCTRL1_INIT_576); ///PGCTRL1_INIT_384);
        vWritePWMG3(A_PGCTRL1, PGCTRL1_INIT_480);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL1, PGCTRL1_INIT_432);
#endif
#endif

        // 1. A_POE pwm channel disable.
        vWritePWMG1(A_POE, 0x000000);
        vWritePWMG2(A_POE, 0x000000);
        vWritePWMG3(A_POE, 0x000000);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, 0x000000);
#endif

        ///[Joel 20100610] For PWM ECO part - sync ADC & PWMDAC square rise edge.
        //vWritePWMG1(0xDFF, 0x01);
        //vWritePWMG2(0xDFF, 0x01);

        // 2. Set A_PGTRL0 0xf01 bit0 = 1 enable PWM
        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG1(A_PDATA, 0);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PDATA, 0);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PDATA, 0);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG4(A_PDATA, 0);
#endif

        // 3. Fade-out first
        ///(0x00<<26) : fast speed
        ///(0xFF<<8) : Maximum step for short fade-out time
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00 << 26) | (0xFF << 8))); ///0x15000100);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00 << 26) | (0xFF << 8))); ///0x15000100);
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x00 << 26) | (0xFF << 8))); ///0x15000100);

#if defined(CC_MT5881)
        if(IS_IC_5881_ES()) ///Remy digital pwmdac have bug(Hailong), need fix!!!
        {
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
        }
        else    ///if(IS_IC_5881_ES2())
        {
    #ifdef CC_ALR0_ALR1_PS90
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #else
            AUD_WRITE32(REG_PWMIP_MISC1, PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #endif
        }
#elif defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        AUD_WRITE32(REG_PWMIP_MISC1, PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2 | PWM_DIFF_PAIR_GP3);
#elif defined(CC_MT5880)
        AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON);
#else
        AUD_WRITE32(REG_PWMIP_MISC1, 0x00); ///Initial gain = 0
#endif
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x00 << 26) | (0xFF << 8) | (0x3F << 0))); ///0x1500013f);
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x00 << 26) | (0xFF << 8) | (0x3F << 0))); ///0x1500013f);
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x00 << 26) | (0xFF << 8) | (0x3F << 0))); ///0x1500013f);
    }
    else
    {
        _MT5398_Mute(0, TRUE);
    }
#endif
    //////////////////////////////////////////////////////////////////


    vWritePWMG1(A_PIC, DACCIE);  // Enable Interrupt for DACC
    vWritePWMG2(A_PIC, DACCIE);  // Enable Interrupt for DACC
    vWritePWMG3(A_PIC, DACCIE);  // Enable Interrupt for DACC
#ifdef CC_MT5880
    vWritePWMG4(A_PIC, DACCIE);  // Enable Interrupt for DACC
#endif

    vWritePWMG1(A_PSTAT, 0xff00);  // Enable Interrupt for DACC
    vWritePWMG2(A_PSTAT, 0xff00);  // Enable Interrupt for DACC
    vWritePWMG3(A_PSTAT, 0xff00);  // Enable Interrupt for DACC
#ifdef CC_MT5880
    vWritePWMG4(A_PSTAT, 0xff00);  // Enable Interrupt for DACC
#endif

    _MT5398_PWM_SamplingRate_Fsi(u1ClkFmt);

    // default 48k
    {
#if defined (CC_MT5881) || defined (CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)  ///Remy without internal ldo.
        PWMGain_3X = (UINT32)PWM_DIGITAL_10X_GAIN;
        PWMGain_3X_N = (UINT32)PWM_DIGITAL_10X_GAIN_N;
        PWMGain_N3X = (UINT32)PWM_DIGITAL_10X_GAIN;
        PWMGain_N3X_N = (UINT32)PWM_DIGITAL_10X_GAIN_N;
#else
        PWMGain_3X = (UINT32)PWM_DIGITAL_3X_GAIN;   //for g1_432k & g2_576k
        PWMGain_3X_N = (UINT32)PWM_DIGITAL_3X_GAIN_N;
        PWMGain_N3X = (UINT32)PWM_DIGITAL_N3X_GAIN; //for g3_480k
        PWMGain_N3X_N = (UINT32)PWM_DIGITAL_N3X_GAIN_N;
        if ( fInternalLDO )
        {
            PWMGain_3X = (UINT32)PWM_DIGITAL_INTERNALLDO_3X_GAIN;      //for g1_432k & g2_576k
            PWMGain_3X_N = (UINT32)PWM_DIGITAL_INTERNALLDO_3X_GAIN_N;
            PWMGain_N3X = (UINT32)PWM_DIGITAL_INTERNALLDO_N3X_GAIN;    //for g3_480k
            PWMGain_N3X_N = (UINT32)PWM_DIGITAL_INTERNALLDO_N3X_GAIN_N;
        }
#endif
        ///PWMGroup1
        vWritePWMG1(A_PCADDR, CH0_GAIN);    /// AR0, To DO.
        vWritePWMG1(A_PCDATA, PWMGain_3X);
        vWritePWMG1(A_PCADDR, CH1_GAIN);    /// AL0
        vWritePWMG1(A_PCDATA, PWMGain_3X);
    #if defined(CC_MT5881)
        if(IS_IC_5881_ES())
        {
            ///PWMGP2 need set inversed gain with PWMGP1.
            ///PWM Group2
            vWritePWMG2(A_PCADDR, CH0_GAIN);    /// AL1
            vWritePWMG2(A_PCDATA, PWMGain_3X_N);
            vWritePWMG2(A_PCADDR, CH1_GAIN);    /// AR1
            vWritePWMG2(A_PCDATA, PWMGain_3X_N);
        }
        else ///if (IS_IC_5881_ES2())
    #endif
        {
            ///PWM Group2
            vWritePWMG2(A_PCADDR, CH0_GAIN);    /// AL1
            vWritePWMG2(A_PCDATA, PWMGain_3X);
            vWritePWMG2(A_PCADDR, CH1_GAIN);    /// AR1
            vWritePWMG2(A_PCDATA, PWMGain_3X);
        }
        ///PWM Group3
        vWritePWMG3(A_PCADDR, CH0_GAIN);    /// AL2
        vWritePWMG3(A_PCDATA, PWMGain_N3X);
        vWritePWMG3(A_PCADDR, CH1_GAIN);    /// AR2
        vWritePWMG3(A_PCDATA, PWMGain_N3X);
#ifdef CC_MT5880
        ///PWMGroup1                                                                  //for Viper/Python analog HP output
        vWritePWMG1(A_PCADDR, CH0_GAIN);    /// AR0
        vWritePWMG1(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN);
        vWritePWMG1(A_PCADDR, CH1_GAIN);    /// AL0
        vWritePWMG1(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN);
        ///PWMGroup4
        vWritePWMG4(A_PCADDR, CH0_GAIN);    /// AR0
        vWritePWMG4(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN_N);
        vWritePWMG4(A_PCADDR, CH1_GAIN);    /// AL0
        vWritePWMG4(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN_N);
#endif

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL1AR1PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWMG1(A_PCADDR, CH0_GAIN);    ///AR0, for g1_432k & g2_576k
            vWritePWMG1(A_PCDATA, PWMGain_3X_N);
            vWritePWMG1(A_PCADDR, CH1_GAIN);    ///AL0
            vWritePWMG1(A_PCDATA, PWMGain_3X_N);
#ifdef CC_MT5880
            vWritePWMG1(A_PCADDR, CH0_GAIN);    ///AR0
            vWritePWMG1(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN_N);
            vWritePWMG1(A_PCADDR, CH1_GAIN);    ///AL0
            vWritePWMG1(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN_N);
            vWritePWMG4(A_PCADDR, CH0_GAIN);    ///AR0
            vWritePWMG4(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN);
            vWritePWMG4(A_PCADDR, CH1_GAIN);    ///AL0
            vWritePWMG4(A_PCDATA, PWM_DIGITAL_INTERNALLDO_HP_GAIN);
#endif
        }

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL2AR2PhaseInverse);
        if (fPhaseInverse)
        {
    #if defined(CC_MT5881)
            if(IS_IC_5881_ES())
            {
                ///PWMGP2 need set inversed gain with PWMGP1.
                ///PWM Group2
                vWritePWMG2(A_PCADDR, CH0_GAIN);    /// AL1
                vWritePWMG2(A_PCDATA, PWMGain_3X);
                vWritePWMG2(A_PCADDR, CH1_GAIN);    /// AR1
                vWritePWMG2(A_PCDATA, PWMGain_3X);
            }
            else ///if (IS_IC_5881_ES2())
    #endif
            {
                vWritePWMG2(A_PCADDR, CH0_GAIN);    ///AR1, for g1_432k & g2_576k
                vWritePWMG2(A_PCDATA, PWMGain_3X_N);
                vWritePWMG2(A_PCADDR, CH1_GAIN);    ///AL1
                vWritePWMG2(A_PCDATA, PWMGain_3X_N);
            }
        }

        fPhaseInverse = (BOOL)DRVCUST_OptGet(eAudioAL3AR3PhaseInverse);
        if (fPhaseInverse)
        {
            vWritePWMG3(A_PCADDR, CH0_GAIN);    ///AR2, for g3_480k
            vWritePWMG3(A_PCDATA, PWMGain_N3X_N);
            vWritePWMG3(A_PCADDR, CH1_GAIN);    ///AL2
            vWritePWMG3(A_PCDATA, PWMGain_N3X_N);
        }
    }

    //////////////////////////////////////////////////////////////////
#if defined(CC_MT5880)      ///for 5880, defined(CC_DAC_DE_POP) 
    if (fgInit)
    {
        // Set A_PGTRL0 0xf01 bit0 = 1 enable PWM
        vWritePWMG1(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG1(A_PDATA, 0);
        vWritePWMG2(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG2(A_PDATA, 0);
        vWritePWMG3(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG3(A_PDATA, 0);
#ifdef CC_MT5880
        vWritePWMG4(A_PGCTRL0, C_PGCTRL0_PEN);
        vWritePWMG4(A_PDATA, 0);
#endif

        //de-pop (e), after Turn on power stage
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (e)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif

        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_UP_EN);              // normal un-mute 00011   (i)
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_SLEW_EN);
        ANA_CLR_BIT(REG_PWMDAC_CFG6, RG_HB_ENDP_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_ENVO_EN);
        ANA_SET_BIT(REG_PWMDAC_CFG6, RG_HB_PLAY_EN);
#ifdef LOG_REG_WRITE
        LOG(0, "=====> [depop] : after (i)\n");
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG6(60118) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG6));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG7(6011c) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG7));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG8(60120) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG8));
        LOG(0, "=====> [depop] : REG_PWMDAC_CFG9(60124) = 0x%x\n", ANA_READ32(REG_PWMDAC_CFG9));
#endif
    }
    else
    {
        _MT5398_Mute(0, FALSE);
    }
#else  //for 5398,5881...
    if (fgInit)
    {
        // PWM Initial Stage Fade-in Setting.
        // If no fade-out time, fade-in from duty 50%.
        // 4. Delay time for fade-out using.
        x_thread_delay(1);

        // 5. A_POE pwm channel enable.
        vWritePWMG1(A_POE, 0x030303);
        vWritePWMG2(A_POE, 0x030303);
        vWritePWMG3(A_POE, 0x030303);
#ifdef CC_MT5880
        vWritePWMG4(A_POE, 0x030303);
#endif

        // 6. Fade-in
        // (0x0D<<26): 99.5msec
        // (0x05<<26): 42.6msec
#if defined(CC_MT5881)
        if(IS_IC_5881_ES()) ///Remy digital pwmdac have bug(Hailong), need fix!!!
        {
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | ONEHEART_ON | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
        }
        else    ///if(IS_IC_5881_ES2())
        {
    #ifdef CC_ALR0_ALR1_PS90
            AUD_WRITE32(REG_PWMIP_MISC1, ONEHEART_ON | PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #else
            AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2);
    #endif
        }
#elif defined (CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | PWM_DIFF_PAIR_GP1 | PWM_DIFF_PAIR_GP2 | PWM_DIFF_PAIR_GP3);
#elif defined(CC_MT5880)
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF | ONEHEART_ON);
#else
        AUD_WRITE32(REG_PWMIP_MISC1, PWMIP_RAMP_OFF);
#endif
        AUD_WRITE32(REG_PWMIP_MISC0_PG1, ((0x0D << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG2, ((0x0D << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
        AUD_WRITE32(REG_PWMIP_MISC0_PG3, ((0x0D << 26) | (0x100 << 16) | (0x01 << 8) | (0x3F)));
    }
    else
    {
        _MT5398_Mute(0, FALSE);
    }
#endif
    //////////////////////////////////////////////////////////////////

    /// make DAC I2S mode and internal DAC LR output normal
#if defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
        /// 0xf00051EC = 0x00543210
        /// 0xf0005248 = 0x00BA9876
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG2, PWM_PAD_INIT2);
#elif defined(CC_MT5881)
        if(IS_IC_5881_ES())
        {
            AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT_ES);
        }
        else    ///if(IS_IC_5881_ES2())
        {
            AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
        }
#else
        AUD_WRITE32(REG_PWMIP_PWMPAD_CFG, PWM_PAD_INIT);
#endif

#if defined (CC_MT5398)
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL0AR0); //GPIO function
    }
    if (!DRVCUST_OptGet(eAudioDacAL1AR1Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL1AR1); //GPIO function
    }
    if (!DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX4, 1, FLD_PAD_AL2AR2); //GPIO function
    }
#elif defined (CC_MT5881)
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL0); //GPIO function
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR0); //GPIO function
        ///Clean 0xf0060108[3:0] for AL0/AR0 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH0 | ACT_ENVO_CH0 | ACT_SLEW_CH0 | ACT_PLAY_CH0))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL0_GPI_EN | AR0_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL1AR1Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL1); //GPIO function
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR1); //GPIO function
        ///Clean 0xf0060108[7:4] for AL1/AR1 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH1 | ACT_ENVO_CH1 | ACT_SLEW_CH1 | ACT_PLAY_CH1))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL1_GPI_EN | AR1_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        /// To Do at Mastang
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL2); //GPIO function
        ///vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AR2); //GPIO function
        ///Clean 0xf0060108[11:8] for AL2/AR2 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH2 | ACT_ENVO_CH2 | ACT_SLEW_CH2 | ACT_PLAY_CH2))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN)));
    }
#elif defined (CC_MT5399) || defined(CC_MT5882)
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); //GPIO function
        ///Clean 0xf0060108[3:0] for AL0/AR0 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH0 | ACT_ENVO_CH0 | ACT_SLEW_CH0 | ACT_PLAY_CH0))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL0_GPI_EN | AR0_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL1AR1Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1); //GPIO function
        ///Clean 0xf0060108[7:4] for AL1/AR1 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH1 | ACT_ENVO_CH1 | ACT_SLEW_CH1 | ACT_PLAY_CH1))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL1_GPI_EN | AR1_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2); //GPIO function
        ///Clean 0xf0060108[11:8] for AL2/AR2 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH2 | ACT_ENVO_CH2 | ACT_SLEW_CH2 | ACT_PLAY_CH2))));
        ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN)));
    }    
#elif defined(CC_MT5890) 
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable))
    {
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL0AR0); //GPIO function
        ///Clean 0xf0060108[3:0] for AL0/AR0 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH0 | ACT_ENVO_CH0 | ACT_SLEW_CH0 | ACT_PLAY_CH0))));
        //ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL0_GPI_EN | AR0_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL1AR1Enable))
    {
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL1AR1); //GPIO function
        ///Clean 0xf0060108[7:4] for AL1/AR1 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH1 | ACT_ENVO_CH1 | ACT_SLEW_CH1 | ACT_PLAY_CH1))));
        //ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL1_GPI_EN | AR1_GPI_EN)));
    }
    if (!DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        //vIO32WriteFldAlign(CKGEN_PMUX6, 1, FLD_PAD_AL2AR2); //GPIO function
        ///Clean 0xf0060108[11:8] for AL2/AR2 no output.
        ANA_WRITE32(REG_ADAC_CFG2, (ANA_READ32(REG_ADAC_CFG2) & (~(ACT_ENDP_CH2 | ACT_ENVO_CH2 | ACT_SLEW_CH2 | ACT_PLAY_CH2))));
        //ANA_WRITE32(REG_ADAC_CFG5, (ANA_READ32(REG_ADAC_CFG5) | (AL2_GPI_EN | AR2_GPI_EN)));
    }        
#elif defined (CC_MT5880)
    /// Judge AR/AL0~AR/AL3 enable or not.
    /// Set input high impedance when AR/ALx disable.
    if (!DRVCUST_OptGet(eAudioDacAL0AR0Enable) || !DRVCUST_OptGet(eAudioDacAL1AR1Enable) || !DRVCUST_OptGet(eAudioDacAL2AR2Enable))
    {
        vIO32WriteFldAlign(CKGEN_PMUX3, 1, FLD_PAD_AL0); //GPIO function
    }

#endif
}

static void _MT5398_PWM_SamplingRate_Fsi(UINT8 u1ClkFmt)
{
    int i, YRAMAdr;

#ifdef LOG_REG_WRITE
    Printf("[!!!PWM_FUNC!!!] _MT5398_PWM_SamplingRate_Fsi: %d\n", u1ClkFmt);
#endif

    ////////////////////////
    /// Only FS_48K case ///
    ////////////////////////

    // Enable Interrupt for DACC
    vWritePWMG1(A_PIC, DACCIE);
    vWritePWMG2(A_PIC, DACCIE);
    vWritePWMG3(A_PIC, DACCIE);
#ifdef CC_MT5880
    vWritePWMG4(A_PIC, DACCIE);
#endif

    // -+-+- Clear X-Ram -+-+-
    vWritePWMG1(A_PCADDR, 0x000); // X RAM
    vWritePWMG2(A_PCADDR, 0x000); // X RAM
    vWritePWMG3(A_PCADDR, 0x000); // X RAM
#ifdef CC_MT5880
    vWritePWMG4(A_PCADDR, 0x000); // X RAM
#endif

    for (i = 0; i < 0xDF; i++)
    {
        /// Set GP1
        vWritePWMG1(A_PCADDR, i);
        vWritePWMG1(A_PCDATA, 0x0);  // Clear X RAM as 0
        /// Set GP2
        vWritePWMG2(A_PCADDR, i);
        vWritePWMG2(A_PCDATA, 0x0);  // Clear X RAM as 0
        /// Set GP3
        vWritePWMG3(A_PCADDR, i);
        vWritePWMG3(A_PCDATA, 0x0);  // Clear X RAM as 0
#ifdef CC_MT5880
        /// Set GP4
        vWritePWMG4(A_PCADDR, i);
        vWritePWMG4(A_PCDATA, 0x0);  // Clear X RAM as 0
#endif
    }

    // -+-+- Load Y-Ram -+-+-
    vWritePWMG1(A_PCADDR, 0x100);
    vWritePWMG2(A_PCADDR, 0x100);
    vWritePWMG3(A_PCADDR, 0x100);
#ifdef CC_MT5880
    vWritePWMG4(A_PCADDR, 0x100);
#endif

    for (i = 0; i <= 0xD6; i++) ///0x115 CH0, 0x1D6 CH1 Gain.
    {
        YRAMAdr = 0x100 + i;
#if defined (CC_MT5881) || defined (CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
    #ifdef CC_SUPPORT_96K_AOUT
        /// for GP1/2/3, Fs*3*5 = 1440KHz
        vWritePWMG1(A_PCADDR, YRAMAdr);
        vWritePWMG1(A_PCDATA, YRAM_5X[i]);
        vWritePWMG2(A_PCADDR, YRAMAdr);
        vWritePWMG2(A_PCDATA, YRAM_5X[i]);
        vWritePWMG3(A_PCADDR, YRAMAdr);
        vWritePWMG3(A_PCDATA, YRAM_5X[i]);
    #else
        if (IS_IC_5890_ES1())
        {   /// for GP1/2/3, Fs*3*10 = 1440KHz
            vWritePWMG1(A_PCADDR, YRAMAdr);
            vWritePWMG1(A_PCDATA, YRAM_8X[i]);
            vWritePWMG2(A_PCADDR, YRAMAdr);
            vWritePWMG2(A_PCDATA, YRAM_8X[i]);
            vWritePWMG3(A_PCADDR, YRAMAdr);
            vWritePWMG3(A_PCDATA, YRAM_8X[i]);
        }
        else
        {
            /// for GP1/2/3, Fs*3*10 = 1440KHz
            vWritePWMG1(A_PCADDR, YRAMAdr);
            vWritePWMG1(A_PCDATA, YRAM_10X[i]);
            vWritePWMG2(A_PCADDR, YRAMAdr);
            vWritePWMG2(A_PCDATA, YRAM_10X[i]);
            vWritePWMG3(A_PCADDR, YRAMAdr);
            vWritePWMG3(A_PCDATA, YRAM_10X[i]);
        }
    #endif
#else
        /// for GP1, Fs*3*3 = 432KHz
        vWritePWMG1(A_PCADDR, YRAMAdr);
        vWritePWMG1(A_PCDATA, YRAM_3X[i]);
        /// for GP2, Fs*2*4 = 384KHz
        vWritePWMG2(A_PCADDR, YRAMAdr);
        vWritePWMG2(A_PCDATA, YRAM_4X[i]);
        /// for GP3, Fs*2*5 = 480KHz
        vWritePWMG3(A_PCADDR, YRAMAdr);
        vWritePWMG3(A_PCDATA, YRAM_5X[i]);
#ifdef CC_MT5880
        /// for GP4, Fs*3*3 = 432KHz
        vWritePWMG4(A_PCADDR, YRAMAdr);
        vWritePWMG4(A_PCDATA, YRAM_3X[i]);
#endif
#endif
    }
}

