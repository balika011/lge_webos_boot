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

#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_drv_cli.h"
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_timer.h"
#include "x_assert.h"
#include "fbm_drvif.h"
#include "drvcust_if.h"


#include "x_dram.h"
#include "srm_drvif.h"
#include "x_dram.h"
#include "x_ckgen.h"
#ifdef __KERNEL__
#include "linux/file_ops.h"
#endif
#ifdef CONFIG_TV_DRV_VFY 
#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"
#define CC_EMULATION
CLI_EXEC_T* GetJpgCmdTbl(void);
extern INT32 _Cmd_JdecVfyAutoTest(INT32 i4Argc, const CHAR** aszArgv);
extern INT32 _Cmd_JdecLog(INT32 i4Argc, const CHAR** aszArgv);
CLIMOD_DEBUG_FUNCTIONS(JPG)

static CLI_EXEC_T _arJpgCmdTbl[] =
{
	 {"verify",     "vfy", 	_Cmd_JdecVfyAutoTest,     NULL, "verify", CLI_GUEST},
	 {"sdl",     "sdl", 	_Cmd_JdecLog,     NULL, "sdl", CLI_GUEST},
   CLIMOD_DEBUG_CLIENTRY(JPG),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};
CLI_MAIN_COMMAND_ITEM(Jpg) 
{
    "jpg",
    NULL,
    NULL,
    _arJpgCmdTbl,
    "Jpeg command",
    CLI_SUPERVISOR
};
#else
#include "jpg_if.h"
#include "img_lib_if.h"
#ifdef JPEG_AUTO_TEST
#define CC_EMULATION
#endif

#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#include "drv_t32.h"

#ifdef JPEG_AUTO_TEST

#ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
typedef struct
{
    UINT32 u4Reserved                       : 8;
    UINT32 u4Bmgp1ag                        : 5;
    UINT32                                  : 2;
    UINT32 fgBmbp1                          : 1;
    UINT32 u4Bmgp2ag                        : 3;
    UINT32 fgBmbp2                          : 1;
    UINT32 u4Bmgp3ag                        : 3;
    UINT32 fgBmbp3                          : 1;
    UINT32 fgReadOnly                       : 1;
    UINT32 fgWriteOnly                      : 1;
    UINT32                                  : 2;
    UINT32 fgFreeze                         : 1;
    UINT32 fgDynPrioEn                      : 1;
    UINT32 fgReqAle                         : 1;
    UINT32 fgPreAct                         : 1;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed))  HAL_DRAMC_T;
#endif

#ifdef JPEG_AUTO_TEST_PERFORMANCE
extern UINT32 mh_width;
extern UINT32 mh_height;
#endif

//#include "auto_test.h"
#ifndef __MODEL_slt__
#define PICTURE_BUFFER_SIZE  (1024*1024*10)
UINT32 pAutoRunCheckSum[]=
{
0x335df06e, 0x638478e9, 0x335df06e, 0xc603ee98, 0xbdea6ea3, 0x7c7c73b0, 0x760b0d97, 0xb19e4b69, 0xee9bb6d2, 0xf53edcc4
, 0x5444ff01, 0xac19ef07, 0xbe86220c, 0x2e1bf833, 0x6f182265, 0xd252ab79, 0x9be0121f, 0x37ed2b04, 0xd3714f45, 0x0a56fc32
, 0xc04585f9, 0x1fc942e4, 0x6f5614a8, 0x85c97245, 0x226f4e88, 0xb3e6b977, 0xae06ca4b, 0x6a387cf6, 0x91bbc305, 0xf3b8cdfc
, 0xa3ffcda0, 0x6bef3cd1, 0x4b2f04ee, 0x2ddadc32, 0x3f9b7e36, 0x2e8a0560, 0xc9054539, 0xb878c48a, 0xc24fb950, 0x6002cc17
, 0x22e554d3, 0xc08030be, 0xecc7df46, 0x9e929938, 0x819dfe30, 0x1f0dd4cb, 0xfcc8662b, 0x4248f88d, 0x25b59886, 0x37cb8db8
, 0xe3b04af0, 0xd1615e46, 0x619171b6, 0xaadd9e14, 0x8433c73d, 0xb7329d68, 0xc54fc247, 0xef92c859, 0x9d65fd8c, 0x6a27b82b
, 0xf064214d, 0x650f9545, 0xdf250dd3, 0x18b263d8, 0xadb3eaf3, 0x2a2f66c0, 0xf9ad3db9, 0xf9e4ce1d, 0x3440de59, 0x824d7c09
, 0xafdf28a4, 0xb8e13f48, 0x190a1c1f, 0xff4b601c, 0x89a559bd, 0x4f27d0c6, 0x6477bc4a, 0x62cb9290, 0xe7f09de1, 0xccbe832c
, 0xa6d3ee06, 0xb7661b32, 0x836f62ad, 0x53789c3b, 0x482ab663, 0x60c19da3, 0x8d1545ee, 0x0b03d7ea, 0xbb19d569, 0x8e79e7c0
, 0xdb2238c3, 0x115ca62c, 0x31762c43, 0xe7e9664a, 0x3e3b77c5, 0x877d698a, 0x4b3fb473, 0x49dae77b, 0x130e9015, 0x6b2a3a6c
, 0x360b2905, 0x9d0731d4, 0xb9a6af6e, 0xe951e563, 0xdf6618a8, 0xa1b4e81b, 0xd9d196f6, 0x0988caa4, 0x537693d0, 0xe9a0cd74
, 0xe4b5964e, 0x53edf9e1, 0x570e23b9, 0xc0b8ef4c, 0xa1409b0e, 0x1d30424b, 0x40ab5e94, 0x18c756a7, 0x5665d406, 0x8901be75
, 0xd91743af, 0x9dec62b2, 0xa849a0aa, 0x6158aa2b, 0xcae5e5fa, 0x68e71abb, 0xf8b5eb1c, 0x6f364316, 0x8882f14c, 0xf0f9e3aa
, 0x9b34d861, 0xa1ec41e2, 0x2b803178, 0xc9904134, 0xf11c4b20, 0x751b401b, 0xfeba7304, 0xbb13c1dd, 0x67fc8b9d, 0x1df19358
, 0xaf82243f, 0x7f1bebbb, 0x8fc412c4, 0x4f586eb0, 0x76cd9256, 0x2e163c7b, 0xf21da33f, 0x7e8731f1, 0x53e2136f, 0x100f1d69
, 0xaa55d13d, 0xa4cadea4, 0x1f169734, 0x094c65cf, 0xeb4f2d14, 0x063e4750, 0xf900cf5a, 0xfa069cf9, 0xcd6f134e, 0xd6c3e157
, 0x7cca8689, 0xe50cbfe3, 0x3ce7bf56, 0xe31c3129, 0x7c8ab94f, 0x75ca0f04, 0xcc376b64, 0x0d29ed49, 0x9341be4c, 0x53ea14c1
, 0xc26e5c71, 0xe01a07b8, 0x198c1c2f, 0x56abe23f, 0x2c762c3d, 0x64448f67, 0x145e979d, 0x0f8120fa, 0xe0544ff4, 0x13a05901
, 0xeb03f9b8, 0x888b66ee, 0x43405777, 0xd3f92b54, 0x5fd5e12d, 0x9987abda, 0x97b5e03a, 0x76ce215c, 0x11d194d9, 0x75e8c5e9
, 0x2b1380b8, 0x40fa6c5a, 0x2f826a3a, 0x89b5df45, 0x044ba22f, 0x644f3e8d, 0xa4bf9dbb, 0x2cefc552, 0xcf809d68, 0x204b84cc
, 0x63727391, 0x39733fff, 0xd75f7571, 0x87c199e0, 0xb1d26e82, 0x67bf654a, 0x5e11ccb2, 0xd81d86f6, 0x776ee75e, 0x3e16b661
, 0xe65b9b5d, 0x953f8595, 0x751a42c3, 0x35b40381, 0xbafe9c0a, 0x35f9e245, 0x87519a56, 0x99eacda3, 0x5eedd3fd, 0x0e76c990
, 0x79d47132, 0xb6eba8b1, 0x11eb690a, 0xd711d986, 0xc4966fe9, 0x15223fe1, 0xf4e4777a, 0x0a91ee2c, 0x11580f30, 0x40d597d7
, 0x22fb034a, 0x45a45a25, 0x35a50c1f, 0x67af1fb7, 0xe123b07d, 0x7126afa5, 0xdb74bc95, 0x6d053d7f, 0x1ab89201, 0xf90f06de
, 0x91e4cf5c, 0x601db6e5, 0x626ed015, 0xf8cf945c, 0x97993b86, 0x22909b43, 0x738b9eaf, 0xca882788, 0x4e61ecd1, 0x854cda72
, 0x20df40c3, 0x3c1b780d, 0x38e659b5, 0x8a244d1d, 0x8b984073, 0x95437d8e, 0x345aeb31, 0x25149eb8, 0x8e707b47, 0x6bdfe7cd
, 0x47160853, 0xf0a8807a, 0x0a0686c4, 0x4fa3eea7, 0x6c234aaf, 0x7797dabf, 0xccc5e73b, 0xac40a652, 0x7c34e7e4, 0x5014bf98
, 0x915619cf, 0x8e661b87, 0x252257bb, 0x45a3717a, 0x6b70fb09, 0x2a412639, 0xd24db784, 0x78759a6c, 0x58d0ff7e, 0x1acdca08
, 0x858308a7, 0x93d9d5d1, 0x0cb2684b, 0x9e7362a1, 0x52cabb79, 0xae61b7ff, 0x54f166f3, 0xb722c535, 0x7038ceea, 0x94e9a0bb
, 0x22655eb7, 0xe593d775, 0xba01734a, 0x6c02b0ab, 0x56a6465d, 0x248b078b, 0x63ab77a1, 0x944f68f0, 0x1f76d7d3, 0x7fa3927e
, 0xecea1510, 0xc2b6c0ba, 0xee3e2213, 0x2b43c4c7, 0x883cf403, 0x037fc8d2, 0x679b97f2, 0x3194dd28, 0xe2361dda, 0xec2d4592
, 0x2139e6b5, 0xd77b7465, 0x6b9fe987, 0x8a5efecd, 0x897aa022, 0x5548e409, 0x584d9b84, 0x048c9ca3, 0xd3f9b38e, 0xf4da3a9e
, 0x773e1c72, 0xd0f74990, 0xe93ab2bb, 0xf2d498f5, 0xd53c88bc, 0x0ea53aa0, 0xa79b6fc2, 0xb1eb342a, 0x81dd2fe9, 0x6fd6c16b
, 0xe7997687, 0x7528e422, 0x38e28575, 0xe0f8e107, 0xce8f9396, 0x4109abbf, 0xb3cab477, 0xc366fcc3, 0xafbfcbd7, 0x8ae7e709
, 0x4abe581e, 0xf68539a6, 0xb01a4a83, 0xbf23dc7b, 0xd212956d, 0xc29e6702, 0x7c7a3b8f, 0x755a51e5, 0x71e931f9, 0xccb9c230
, 0xba18fa71, 0xae06dd0c, 0x14a532ac, 0x2e2d4ca6, 0xce8790a6, 0x8a5e2497, 0xc21eac95, 0x964880c3, 0xbf01a926, 0x9d333cb3
, 0xba297867, 0x92d5b679, 0xe777222c, 0xa7305b52, 0xf134e42d, 0x70b1f48b, 0x6fc81218, 0xbc4de501, 0xf6041bd6, 0xa316d0bf
, 0x8b7f0810, 0x639778f2, 0xe3fd9e7f, 0xe9c6ccdd, 0xd5913e3c, 0x25df4846, 0x6ccbc92b, 0x9330a553, 0xc13e2fd6, 0x74d11a46
, 0xaf9f7a4a, 0xcf11e740, 0x9d2f2414, 0xbff2137b, 0x5f8fbe15, 0x26855f2b, 0x2eeaeb1b, 0x13e3f213, 0x73be1282, 0xeeb31cd6
, 0x6d02ae9b, 0x2ed7a0a5, 0x6528594c, 0x687457c6, 0x47e34dcc, 0xdde117c2, 0x29949d2c, 0x0d7491a3, 0x2f95d5dd, 0xf286d94f
, 0x908a2451, 0x3e0b3cd0, 0x436fb5d8, 0xc358dc19, 0x90839adf, 0xb0a8a048, 0x4a176873, 0x17f0921a, 0x8fb0c37c, 0xd1bdca44
, 0x178407f1, 0xa0b41695, 0x0211dafb, 0x4aaf9489, 0x2b2e524e, 0xb5e00e7a, 0x7441dea1, 0x4aa64c8c, 0xf4b7f58b, 0x19749db2
, 0xb4a6c568, 0x5a4889c6, 0x584c9240, 0x2a1914d9, 0xb51a0f16, 0xefeaee9f, 0xfd964249, 0xcd0d1565, 0xcf7f5f48, 0x2cd05bcf
, 0x84c61c34, 0x4adfba27, 0x4796a5de, 0x3cc0024f, 0x2de7a138, 0x42a4a895, 0x27b5e71c, 0x58e1bd04, 0xe952f071, 0x61f8aedc
, 0x34b07a9d, 0xa0c488fa, 0xd0ea71e8, 0xf4a655f3, 0x49e20e09, 0x45a288ed, 0xf29dc862, 0xfe4edeae, 0x49f9ec76, 0x5a545cca
, 0x020628e5, 0xd7446d07, 0x7373287f, 0x91b11bd2, 0x3160a223, 0x8960ed3f, 0xef5d6894, 0xc5c8451f, 0x192d035f, 0x04012660
, 0x881bfd79, 0x52116455, 0x422b1548, 0xce3c115e, 0x5c837689, 0xb93ca508, 0x2bd5d4a0, 0x77410b94, 0x48ea7955, 0x63b8fc8f
, 0x76963deb, 0xad21f12b, 0x278f268b, 0x1edd8e75, 0x55d6c900, 0x6d199ab2, 0xad6bab53, 0x2634047f, 0x21de0dde, 0xc5b6200c
, 0xddcff3db, 0x83f03419, 0x11fe03b3, 0xea2fecf8, 0xbdbf8543, 0x091b8af6, 0xb92013b8, 0xb3ce6a9b, 0x4c674006, 0x8bcc2023
, 0x5982b886, 0x39d056fa, 0x3546794c, 0x01a0aaee, 0x7b74c8c5, 0x9a757581, 0x530164b2, 0x6bb81b4e, 0x0c393a6f, 0x69961bc2
, 0x26307920, 0x7aae0a18, 0xfd17b01c, 0x0a0016e9, 0xf2c67067, 0x42e8238f, 0x76910292, 0x0b199d2a, 0x4163c955, 0x89350d7f
, 0x0b1d0388, 0x54aa030e, 0x70573a28, 0x62b08c2f, 0xa520919b, 0x533a1fc2, 0x46f7ed77, 0x9e5d65c5, 0x1a6454cf, 0x681ca278
, 0xd47a0f1b, 0xee7cc32f, 0x283b419a, 0x6cbf2e7d, 0x240beecb, 0x0183ae35, 0xa3b51584, 0x6131d71c, 0x6a9546c9, 0xff04ace4
, 0x152871a8, 0xb47d6186, 0x9f6c5f56, 0x9990a1b3, 0xebab0933, 0xd58d1b16, 0x052159e4, 0xf0b0cf89, 0xd958536a, 0xd368882f
, 0xf7e97f42, 0xea3247c2, 0x4dc0e9c5, 0x9c308cdd, 0x8e7ac797, 0x60563a78, 0xfc66fd29, 0x5299b793, 0x96deeaaa, 0x23552c77
, 0xc17196d3, 0x6cd1e432, 0x7035d2da, 0x00cb1e06, 0x5c278613, 0x42911e36, 0x54c6ae31, 0x0ba25dcf, 0xb135cebc, 0xeb33d530
, 0x6812dadf, 0x9e72a96e, 0xa321e6d5, 0xf41351a8, 0xea30693b, 0x0a707fae, 0x5297ce8c, 0x30aad4b7, 0x843caf35, 0x81c9538f
, 0x7f9dde1f, 0x999fdd66, 0xdb05381f, 0x88bc57e0, 0x03590268, 0x13f13722, 0x86d90359, 0x065551ca, 0x39d0a234, 0xea295819
, 0xa8b46119, 0xa71dfd62, 0xb9655759, 0x21f3f7a5, 0xfdb8f1ac, 0x8d180f95, 0x087e9a02, 0x2a2c1431, 0x78d1d8eb, 0xc68b64d9
, 0x5b6f186f, 0x8299e7ae, 0x9380cee0, 0x03c67c06, 0x7422f0b7, 0xbf5aff3d, 0x8da3f337, 0xad546c70, 0xf6c6231c, 0x74e9c8bb
, 0x780d89fe, 0x09a412a4, 0x174eee42, 0x502d3304, 0x75cba0bc, 0xacc38d3e, 0x2b8ad456, 0x78d465d0, 0x387f95c6, 0xa081c37c
, 0x65bed8a9, 0x9c0d59d1, 0x64becab0, 0x1e70041c, 0xe7ac2ec4, 0x38b7b9cc, 0xbafdbdb2, 0x8a4e220a, 0xe4a6a1f1, 0x178f01c7
, 0x600eb060, 0xfa7a67a5, 0x0e866f89, 0xf9f8a862, 0x88291d88, 0x37181404, 0xd42be3df, 0xbe283be1, 0x650109ff, 0x5f01d207
, 0x81afa52c, 0x8f81c857, 0x1cd7ca4e, 0xd9867dcf, 0xfb9a8e5c, 0x06a150f9, 0x45d248b9, 0x5922e47d, 0x573983ef, 0xb0fbfadb
, 0xb858976b, 0x886e15ca, 0x5a64893f, 0x32cff9d2, 0x28a36e6d, 0x3bd7abd6, 0x55a92eca, 0x904b2e0d, 0xeb66a174, 0x084225eb
, 0x63c439aa, 0xba08d0f7, 0x2d76eff7, 0x902e4f99, 0x5c0af733, 0x7d976bcd, 0x5c73d524, 0x8c329980, 0x4883ea7f, 0x2149e848
, 0x89ff22e3, 0xb5013f2b, 0xe6b48843, 0x95220195, 0xa6862e6b, 0x09d0eecc, 0x35bbfe24, 0x52e3596c, 0xfc1c372a, 0xed650992
, 0x16c2c896, 0xcabd6c75, 0xa54ceafd, 0x60720388, 0x8d213478, 0xcbf71511, 0xa2cbcc56, 0x742b7bf6, 0x7f6bab6c, 0xf19b9aff
, 0x09b42a0f, 0x038f90c2, 0x4795b853, 0xefc129ad, 0x085568a7, 0xdaf16d77, 0xdb0b2417, 0x631ff22f, 0x572bdeba, 0xc3094250
, 0x7860dbda, 0x7a8f12ef, 0x71df6a32, 0xb292ac9d, 0xd7991033, 0x3b69186c, 0x4f3bc046, 0xb2678cea, 0xf9390d82, 0x353a6adc
, 0xf7bbc85a, 0x6b99dd0c, 0xdaa944e4, 0xd1983c66, 0xd1368c41, 0x0f63863a, 0x61e1158e, 0xfc7be1a5, 0xbe4e9baa, 0xd0b5dc88
, 0x0dbd1d19, 0xd72822c5, 0x8a9f38f5, 0x805f7b86, 0x29387e75, 0xe0f5e14f, 0x485be721, 0x2996bbcc, 0xa39f7d6a, 0x8ff95b8d
, 0xdf1d6292, 0xd45099a2, 0xcab422a3, 0x28b7c377, 0x161ee265, 0xffff4d96, 0xd86425e6, 0x4ffd84de, 0x0cf20fbe, 0xd1cadd88
, 0x58df315e, 0x22c4db43, 0xdcb70d62, 0xb494ae81, 0xff824af1, 0x6dff5d2b, 0xddc54eab, 0x643e264b, 0x1149b991, 0x6e893ae2
, 0x31ce79e6, 0x5575a7c2, 0x27735739, 0x8f04cb98, 0x1682156b, 0x8184fcfa, 0xac325215, 0xb41da096, 0x8d63dce0, 0xadc9cd87
, 0xaeaf1d5a, 0x5a3e665e, 0x37155910, 0x9af4b139, 0xa9ad3b7a, 0xdfdb1613, 0xab9fba81, 0xf21b6059, 0xe82156b1, 0xbf055977
, 0x7dedafbd, 0xf63911da, 0xd453abae, 0xf38e78c3, 0x3ef85837, 0x6a9258a5, 0x7e6e8cf7, 0xed26697d, 0xf5f0c9a3, 0x153834f0
, 0x5e522058, 0xdc5e8cb4, 0x2a9728dd, 0x8a727b77, 0xa24cea1b, 0x2373ffe3, 0x6cf3552f, 0xcbdb42ac, 0x16ca46fd, 0x979fcbb7
, 0xe684021e, 0x26102684, 0x440034a2, 0xe07af49d, 0xbbd637b4, 0x3c5fc983, 0x7854cb45, 0xb3cb04de, 0xbbc9138f, 0x804bc376
, 0x3ac6db29, 0x490087a9, 0xed21e7b7, 0x898f47ce, 0xf596cc2d, 0xf3ac04d6, 0x90196fdb, 0xcbc9e3a5, 0x06c0535e, 0xc1015b6a
, 0x60b55c69, 0x7390759c, 0x696d79c4, 0x030432ca, 0x75bf0c5e, 0x5fa86d41, 0xc2278d94, 0x0dd80556, 0xdb888ad4, 0x32dd9f6d
, 0x9ddbc42a, 0x90c0f1e4, 0x6390d2d9, 0x27a3d49d, 0xb1470b19, 0xd9a5edf6, 0xecc8192a, 0xce509083, 0x1b7dc02c, 0xffdf3300
, 0x63070159, 0x06eccba9, 0x55f24f2e, 0x94849538, 0x644c220e, 0xc8ed05b0, 0x913ed340, 0xdc6e43e5, 0x774ba63e, 0x12a2ed80
, 0x2f2a6935, 0x69ff6e4d, 0xcdcd117e, 0x4bd690b2, 0x6b55c8c2, 0x0c39b2e3, 0xd27cf36f, 0xa66993d4, 0x23450263, 0xf460835e
, 0x15bf3d10, 0x980d8f09, 0x37c3974d, 0xc74d538b, 0x271da9bf, 0x1f3d8d51, 0xed57358a, 0xf5c4d2de, 0x92687be4, 0x95c54c1b
, 0x58383b42, 0x5efc7b9c, 0xeed96b21, 0x2f0f64c4, 0x1b8bf419, 0xa2e5b05f, 0x260e07f1, 0x937ed214, 0xbe1c7a2b, 0xdb5407d2
, 0x56beee3e, 0xdcc22d43, 0x26905ed0, 0x74fa84ca, 0x17cdcdd0, 0xc954c6ce, 0x495becde, 0x6a53009e, 0x8256936e, 0xcc1c62f4
, 0x092c61a7, 0x2701f578, 0x8b4248ea, 0x2eb2cb5c, 0xdcfc4b7a, 0x51a66461, 0x56f7e812, 0xc7badeac, 0x71f48ef2, 0x12b44025
, 0xcee39a14, 0xc7f246ec, 0xd742465d, 0xf968e4b6, 0xd1b81f8d, 0x0e43c127, 0x5fe7a9c9, 0x3e00fb2a, 0x9040139b, 0xf1aa47c0
, 0x0904501c, 0x8a28c9d2, 0xfe8565e3, 0xa59fecc5, 0x1ecf854d, 0x93063220, 0x137dc13b, 0x1e401383, 0xb6a26b8c, 0x527ef2d6
, 0xf14482b1, 0xa27b6322, 0xec43d865, 0x4b82162e, 0x1607cf4b, 0x747478b3, 0x5d5906b0, 0x9ecdae87, 0x8c4bb2de, 0xd7c8b0bc
, 0x77891ea0, 0xb18f2f4c, 0xe989222b, 0xbbb92cfc, 0xea1a9a78, 0xf9a5e3bb, 0x54088d05, 0xfb2d2e4a, 0xa8ff8ec8, 0x4221ec38
, 0x37a9ca31, 0x7d1f423a, 0x31a0318f, 0x4437b23c, 0x597fa9a7, 0x3b8ec50e, 0x6a063294, 0xdea34d0c, 0x63a046bd, 0x58e636d7
, 0x03c6af0c, 0xcfbfb594, 0x896122a7, 0x5035f7cd, 0xaf73e8c5, 0xd2b0c21c, 0x889659e1, 0x1d052549, 0x568b03e8, 0xe5be81e4
, 0x51f98bbf, 0xea522459, 0x8bed601f, 0xf03b9c8c, 0xf666c40a, 0x44d3cde8, 0x97d5576d, 0xa3ba7109, 0xf9e791c3, 0x65b15cb1
, 0xf12132ad, 0x368cfab8, 0x3934c4bd, 0xa603825c, 0x96bf5cc1, 0x860c8ea6, 0x84426b41, 0xdd4905e9, 0xb858951d, 0x028d6247
, 0x45c7fd38, 0x8ef7670e, 0x6a00f073, 0x1e50d8d0, 0xf803c9d6, 0x471861cc, 0xe164e851, 0xc5684344, 0x329b53f8, 0x43f02615
, 0x43dd9d22, 0x221af714, 0x227332be, 0xb9404803, 0x940316c2, 0x9509e57e, 0xd3ece6a0, 0xdbdf7a33, 0x84e71e90, 0x70122170
, 0x9e4e4826, 0x3e31cc0b, 0xc24c6ce4, 0xfd98aad8, 0xcc29ed4f, 0x6b63c085, 0xc9922a20, 0x171a4568, 0x814eeb01, 0xd37c5d78
, 0xfe03a0c8, 0x2aa97c0a, 0xdd2817f6, 0xe4961fca, 0x5d9f0a6a, 0x74dd6962, 0xe8847bc8, 0x5c8526ef, 0xf8d5e8d4, 0xa090e4ad
, 0x50b47eb2, 0x8ffa6000, 0xed7cc963, 0x00815f9d, 0x0b39105e, 0x4023d61e, 0xbaff92d0, 0x820ac86a, 0x5e8e11b9, 0xef8ab140
, 0xb3c74c85, 0xc2bdfa78, 0xc6f30c16, 0xc4d056b0, 0xd5153ac9, 0x95dd1bd4, 0xc85ddf67, 0x080374b0, 0xc1bcf620, 0x3f3adde0
, 0xc3bf3a40, 0x3f7ab414, 0x54640b01, 0x6fefa6e6, 0xef8ab140, 0xb3c74c85, 0xc4d056b0, 0xd5153ac9, 0x814eeb01, 0xa19bc5b0
, 0x3e41bb42, 0xf23e3d0e, 0xc6454de3, 0x0481ee61, 0x5566fec7, 0x9479ca1f, 0x6b9c09cd, 0x802024d3, 0x5f986081, 0x5871f3a4
, 0x3fb9c832, 0x2f3307ee, 0x081b2941, 0xf4854e06, 0xcbc15a70, 0xe3582ff3, 0x6f7e53a7, 0x903f4ac7, 0x11ab9fc0, 0x790cc4ca
, 0x97efaad8, 0x758e6ec6, 0xa50b56a0, 0xb8b9f523, 0x940e0e14, 0x5e9e71d3, 0x47972100, 0xaff371d4, 0x0d30fe97, 0x6db2ac1e
, 0x389b8d0f, 0x3c353c01, 0x25b7896b, 0xb09def31, 0x6fd1ac8c, 0xde807485, 0x110c8f56, 0xa1157e92, 0x94c65282, 0x316cc932
, 0x971a0260, 0x1d1373c8, 0xab11325c, 0x3c6e22f2, 0xd9adbb2e, 0x545c425e, 0x8929c69e, 0x504b3701, 0xaa475153, 0x130ac380
, 0x1cb17c5a, 0xa595255b, 0x2f7348bb, 0x231ae6ec, 0x7a9b5ecd, 0x9a1de6e0, 0x403ded33, 0x05480f38, 0x4e34ec6f, 0xd3c357b1
, 0x9676b550, 0xe209999e, 0x779202be, 0x335df06e, 0xf1063eb6, 0xfaa4bf21, 0xa6fc568c, 0xc603ee98, 0xec38c443, 0xfd03ec80
, 0x85184738, 0xe3374781, 0xcc67e1f2, 0x451d4c5b, 0xef07a414, 0xc8033cc9, 0xb94bd313, 0xe4b6bc57, 0xf6e2c092, 0x2c982fed
, 0xbc9ee3dd, 0xfbf32462, 0xdc77b940, 0x7dbee82c, 0xad1a6115, 0xe75f2ae2, 0xa3eb3ff6, 0x8d5b8eb8, 0xde3d3cff, 0xb9cb0c78
, 0xfdd5b95a
};
#else
UINT32 pAutoRunCheckSum[5]={0};
#define PICTURE_BUFFER_SIZE  (10)
#endif

#define PROGRESSIVE_AUTO_TEST_NUM	  (29)
#define JPG_VERIFY_IMG_ID   0

#ifdef JDEC_HW_IO_MMU
UINT32 pTestBuffer = 0x3600000;
#else
UCHAR pTestBuffer[PICTURE_BUFFER_SIZE+64]={0};
#endif
#define SRC_BUFFER_ALIGNED      (UCHAR*)(((((INT32)pTestBuffer)+63)/64)*64)

#define VERTIFY_DATA_Y "Y.bin"
#define VERTIFY_DATA_C "C.bin"
#define VERTIFY_DATA_CB "Cb.bin"
#define VERTIFY_DATA_CR "Cr.bin"
#define VERTIFY_DATA_PIC "PIC.bin"

#define VERTIFY_DATA_YUV_Y "yuv1.y"
#define VERTIFY_DATA_YUV_U "yuv1.u"
#define VERTIFY_DATA_YUV_V "yuv1.v"

#define JPEG_VERTIFY_CHECKSUM_MAX 4
#define JPEG_VERTIFY_CHECKFILE_MAX 5
#define ALIGN2048(addr) (addr+0x800)&(0xfffff800)


typedef struct
{
  UCHAR *pu1InputFileName;
  UINT32 puInputFileSize;
  UCHAR *pu1Comd;
  UINT32 u4FlgSave;
  UINT32 aru4CheckSum[JPEG_VERTIFY_CHECKSUM_MAX];
}T_JPG_AUTO_TEST_CASE;

T_JPG_AUTO_TEST_CASE _rBaseTestCase[]=
{
    {"KOALA_BASE_443x600_37586.jpg",37586,NULL,1,{0,0,0}},
    {"CANON_G2_1024x768_133404.jpg",133404,NULL,1,{0,0,0}},
    {"CANON_S410_1704x2272_1181151.jpg",1181151,NULL,1,{0,0,0}},
    {"FUJI_FINEPIX500_1280x1024_609347.jpg",609347,NULL,1,{0,0,0}},
    {"FUJI_A201_1600X1200_777701.jpg",777701,NULL,1,{0,0,0}},
    {"OLYMPUS_C220Z_1600x1200_415536.jpg",415536,NULL,1,{0,0,0}},
    {"SONY_DSC_2592X1944_2173531.jpg",2173531,NULL,1,{0,0,0}},
    {"SONY_SORA_T30_2048x1536_1187521.jpg",1187521,NULL,1,{0,0,0}},
    
    {NULL,0,NULL,0,{0,0,0}}, 
};
T_JPG_AUTO_TEST_CASE _rAutoTestCase[]=
{

	//error bistream
  {"0020_MAUI_00116561.jpg",36666,NULL,0,{0,0,0}},	
  {"021.jpg",209789,NULL,0,{0,0,0}},	
  {"1004.jpg",478362,NULL,0,{0,0,0}},	
  {"1201miss22.jpg",84827,NULL,0,{0,0,0}},	
  {"concert.jpg",1726808,NULL,0,{0,0,0}},	
  {"DataLost_jpg_2.jpg",1225917,NULL,0,{0,0,0}},	
  {"DCAM00044.JPG",2424587,NULL,0,{0,0,0}},	
  {"DSC_0006.JPG",2383551,NULL,0,{0,0,0}},	
  {"FinepixF610_04.jpg",2433985,NULL,0,{0,0,0}},	
  {"Logitech_webcam_test.jpg",50567,NULL,0,{0,0,0}},	  
  {"P33853.jpg",6013860,NULL,0,{0,0,0}},	  
  {"W11.jpg",278321,NULL,0,{0,0,0}},	    
  {"19_4000X.jpg",2190838,NULL,0,{0,0,0}},	  

	//small pictures
  {"woodup_1x1.jpg",2220,NULL,0,{0,0,0}},	
  {"woodup_1x2.jpg",2115,NULL,0,{0,0,0}},	
  {"woodup_1x4.jpg",2018,NULL,0,{0,0,0}},	
  {"woodup_2x1.jpg",2131,NULL,0,{0,0,0}},	
  {"woodup_2x2.jpg",2072,NULL,0,{0,0,0}},	
  {"woodup_2x4.jpg",2009,NULL,0,{0,0,0}},	
  {"woodup_4x1.jpg",2051,NULL,0,{0,0,0}},	
  {"woodup_4x2.jpg",2015,NULL,0,{0,0,0}},	  

	//special baseline
  {"normal_CANON_G2_1024x768_133404.jpg",133404,NULL,0,{0,0,0}},
  {"normal_CANON_S410_1704x2272_1181151.jpg",1181151,NULL,0,{0,0,0}},	//MultiDQT
  {"normal_FUJI_A201_1600X1200_777701.jpg",777701,NULL,0,{0,0,0}},
  {"normal_FUJI_FINEPIX500_1280x1024_609347.jpg",609347,NULL,0,{0,0,0}},
  {"normal_KOALA_BASE_443x600_37586.jpg",37586,NULL,0,{0,0,0}},
  {"normal_OLYMPUS_C220Z_1200x1600_395007.jpg",395007,NULL,0,{0,0,0}},
  {"normal_OLYMPUS_C220Z_1600x1200_415536.jpg",415536,NULL,0,{0,0,0}},
  {"normal_SONY_DSC_2592X1944_2173531.jpg",2173531,NULL,0,{0,0,0}},
  {"normal_SONY_SORA_T30_2048x1536_1187521.jpg",1187521,NULL,0,{0,0,0}},
  {"spec_444_2950x2094_633228.jpg",633228,NULL,0,{0,0,0}},  
  {"spec_1x5000_776.jpg",776,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_4x4_705.jpg",705,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_4x500_1921.jpg",1921,"jset 8 1 *de **",1,{0,0,0}},
  {"spec_gray1_512x512_56170.jpg",56170,"jset 8 1 *jset 14 1 *de **",0,{0,0,0}},  
  {"spec_gray3_600x400_37078.jpg",37078,"jset 8 1 *jset 14 1 *de **",0,{0,0,0}},  
  {"spec_MulitDQT_1200x1600_415536.jpg",415536,"jset 8 1 *jset 14 0 *de **",0,{0,0,0}},  
  {"spec_small_1x1_306.jpg",306,"jset 8 1 *jset 14 0 *de **",1,{0,0,0}}, 
  {"spec_6000x5000_351870.jpg",351870,"jset 8 3 *jset 15 0 *jset 10 1 *jset 11 0 *de **",1,{0,0,0}}, 

  {NULL,0,NULL,0,{0,0,0}}, 
};

#if  defined(CC_MT5387) && !defined(CC_MT5363)   
typedef enum
{
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_150MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_180MHZ,
  VDEC_VLC_CLK_200MHZ,
  VDEC_VLC_CLK_300MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_JPGDEC_CKCFG)
typedef enum
{
  VDEC_VLC_CLK_240MHZ,  
  VDEC_VLC_CLK_216MHZ, 
  VDEC_VLC_CLK_192MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_80MHZ,
  VDEC_VLC_CLK_60MHZ,  
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_GRAPHIC_5395_PLL)
typedef enum
{
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_120MHZ,
  VDEC_VLC_CLK_144MHZ,
  VDEC_VLC_CLK_168MHZ,
  VDEC_VLC_CLK_216MHZ,  
  VDEC_VLC_CLK_240MHZ,
  VDEC_VLC_CLK_MED_MHZ,  
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#elif defined(JPG_CLK_FROM_GRAPHIC)
typedef enum
{
  VDEC_VLC_CLK_171MHZ,
  VDEC_VLC_CLK_172MHZ,
  VDEC_VLC_CLK_216MHZ,
  VDEC_VLC_CLK_229MHZ,
  VDEC_VLC_CLK_240MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#else
typedef enum
{
  VDEC_VLC_CLK_100MHZ,
  VDEC_VLC_CLK_108MHZ,
  VDEC_VLC_CLK_133MHZ,
  VDEC_VLC_CLK_162MHZ,
  VDEC_VLC_CLK_180MHZ,
  VDEC_VLC_CLK_200MHZ,
  VDEC_VLC_CLK_MAX
}e_vdec_vld_clk;
#endif

static char* decode_mode_str[] = \
    {"MCU(0)", "MCU Row(1)", "MCU Row+IMGRZ(2)", "Picture + IMGRZ(3)", "Video_Skype(4)", \
     "MCU_MULTI_COLLECT(5)", "MCU_ENHANCE(6)", "MCU_ROW_MULTI_COLLECT(7)", "MCU_ROW_ENHANCE(8)", "SCAN_MULTI_COLLECT(9)", "SCAN_ENHANCE(10)"};

typedef enum
{
  PIC_DEC_MODE_BASELINE_MCU=0,
  PIC_DEC_MODE_MCU_ROW,
  PIC_DEC_MODE_MCU_ROW_IMGRZ,
  PIC_DEC_MODE_PICTURE_IMGRZ,
  PIC_DEC_MODE_PICTURE_VIDEO_MODE,  // 4
  
  PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_MCU_ENHANCE,
  PIC_DEC_MODE_PROG_ROW_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_ROW_ENHANCE,
  PIC_DEC_MODE_PROG_SCAN_MULTI_COLLECT,
  PIC_DEC_MODE_PROG_SCAN_ENHANCE,
}e_decode_mode;
#define IS_PROGRESSIVE_DECODER (_JpegVerParam.i4JpegDecMode >= PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT &&\
                             _JpegVerParam.i4JpegDecMode <= PIC_DEC_MODE_PROG_SCAN_ENHANCE)
typedef enum
{
   JPEG_VERIFY_PARAM_MULTI_SET=0,                     
  JPEG_VERIFY_PARAM_CHECKSUM,
  JPEG_VERIFY_PARAM_CHECKFILE,
  JPEG_VERIFY_PARAM_SRC_BUFFER,
  JPEG_VERIFY_PARAM_SRC_BUFSIZE,
  JPEG_VERIFY_PARAM_PIC_SIZE,   //5
  JPEG_VERIFY_PARAM_TAY_BUFFER,
  JPEG_VERIFY_PARAM_TAC_BUFFER, //7
  JPEG_VERIFY_PARAM_DEC_MODE,
  JPEG_VERIFY_PARAM_VLD_CLK,
  JPEG_VERIFY_PARAM_TO_OSD,
  JPEG_VERIFY_PARAM_OSD_CM,
  JPEG_VERIFY_PARAM_RST_OUT,   //12
  JPEG_VERIFY_PARAM_OUT_MODE,
  JPEG_VERIFY_PARAM_GRAY_PIC,
  JPEG_VERIFY_PARAM_DUMP_COLBUF,
  JPEG_VERIFY_PARAM_BIG_PIC,
  JPEG_VERIFY_PARAM_OUTSWAP,   //17
  JPEG_VERIFY_PARAM_MID,
  JPEG_VERIFY_PARAM_PIC_STOP, 
  JPEG_VERIFY_PARAM_DEC_TIME,//20
  JPEG_VERIFY_PARAM_RZ_TIME,
  JPEG_VERIFY_PARAM_PIC_FORMAT, //22
  JPEG_VERIFY_PARAM_PARTIAL_OUTPUT, // 23
  JPEG_VERIFY_PARAM_MULTI_INIT=100,
  JPEG_VERIFY_PARAM_SHOW_HELP=200
}E_JPG_VERIFY_PARAM;


typedef struct _T_JPG_VERIFY_PARAM
{
  INT32 i4CheckSumNum;
  INT32 i4CheckFileNum;
  INT32 arCheckSum[JPEG_VERTIFY_CHECKSUM_MAX]; 
  INT32 arCheckFile[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 arCompAddr[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 arCompSize[JPEG_VERTIFY_CHECKFILE_MAX];
  INT32 i4JpegSrcBufAddr;
  INT32 i4JpegSrcBufSize;
  INT32 i4JpegPicSize;
  INT32 i4JpegTaYBufAddr;
  INT32 i4JpegTaCBufAddr;
  INT32 i4JpegDecMode;
  INT32 i4JpgVldClock;
  INT32 i4IsVdo2OSD;
  INT32 i4JpgOsdModeCM;
  INT32 i4IsRastOut;
  INT32 i4JpegModeOutPutMode;
  INT32 i4IsGrayPicture;
  INT32 i4DumpColorBuffer;
  INT32 i4IsBigPicture;
  INT32 i4OutSwapMode;
  INT32 i4MidTest;
  INT32 i4PicModeStopTest;
  INT32 i4GetDecTimeProfile;
  INT32 i4GetRZTimeProfile;
  INT32 i4PicFormat;
  INT32 i4PartialParam[4];
}T_JPG_VERIFY_PARAM,*P_JPG_VERIFY_PARAM;

#endif

#endif

#ifdef __MODEL_slt__
#include "x_fm.h"
static UINT32 _u4JpgSlt = 0;
static CHAR _acJpgFileName[50];
#endif

#define DEFINE_IS_LOG CLI_IsLog
#include "x_debug.h"

//#include "drv_t32.h"

extern void MPV_PllInit(void);

/******************************************************************************
* Macro            : helper macros to reduce code
******************************************************************************/
#define NONCACHE(addr)                                (((addr) & 0x0fffffff) | 0x30000000)
#define TRY(X) \
    /* following do {} while (0) should be safe */ \
    LINT_SUPPRESS_NEXT_STATEMENT(717) \
    do \
    { \
        i4Ret = (X); \
        if (i4Ret) \
        { \
            goto _catch_err; \
        } \
    } while (0)

typedef enum _ENUM_CLIJPG_ERR_CODE_T
{
    E_CLIJPG_OK                = 0,
    E_CLIJPG_ERR_GENERAL    = 1000,
    E_CLIJPG_ERR_PARAM        = 1001,
    E_CLIJPG_ERR_MEM		=1002,
    // add here
    E_CLIJPG_UNKNOWN        = 1999
} ENUM_CLIJPG_ERR_CODE_T;

typedef struct _JPGCLI_TEST_SET_T
{
    CHAR *szShortName;
    UINT32 u4ByteSize;
    CHAR *szFileName;
} JPGCLI_TEST_SET_T;

/******************************************************************************
* Test suit        : cli "jpg.t32test" data
******************************************************************************/
#ifndef JPG_SEMI_HOST
#define JPG_SEMI_HOST
#endif

#define NEW_VERIFY_FLOW 1


static INT32 _JpgCmdDebugStep(INT32 i4Argc, const CHAR ** szArgv);

#if defined (CC_EMULATION) || defined (__MODEL_slt__)

#ifdef JPG_SEMI_HOST
static CHAR _szOutputDir[] =
    "d:/jpg_test/output";
#endif

JPGCLI_TEST_SET_T *_arJpgTestSet = NULL;
static CHAR *_szInputDir = NULL;
static UINT32 _u4fileSize = 0, _u4filled_size = 0;
#ifndef JPEG_AUTO_TEST
static INT32 _i4TestSetIdx = 0;
#endif
static BOOL _fgJpegInstance = FALSE;
static BOOL _fgJpegWriteEnable = FALSE;
static DRV_OSD_T *_prOsd = NULL;
static ENUM_IMG_DISP_COLOR_MODE _eDispMode = AYCBCR8888_DIRECT32;
static UINT32 _au4BytePerPixel[] = {2, 0, 0, 2};		///< bit depth (left shift value), index=ENUM_IMG_DISP_COLOR_MODE
static UINT32 _u4NtyValue = JPG_HW_NFY_FINISHED;
	
static HANDLE_T _hJPGDecSema;
extern void JPGCLI_GetTestSet(INT32 i4Index, JPGCLI_TEST_SET_T **pprSet, CHAR **pszInputDir);
extern INT32 JPGCLI_GetMaxSet(void);
static UINT8* _pu1Bs = NULL;


// 5391
extern INT32 vJpegVerify(INT32 i4Argc, const CHAR ** szArgv);
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdSetDecMode(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _JpgCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv);


/******************************************************************************
* Prototype        : cli "jpg" commands
******************************************************************************/

CLI_EXEC_T* GetJpgCmdTbl(void);
static INT32 _JpgCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoSetDecModPath(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoSetVLDClock(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoTestImgRzParam(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv);
//static INT32 _JpgCmdAutoProgDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdInit(INT32 i4Argc, const CHAR ** szArgv);
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdDecode(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdChangeSet(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdT32Test(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdWriteEnable(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdWriteDisable(INT32 i4Argc, const CHAR ** szArgv);
#endif
static INT32 _JPGDecodePicture(INT32 i4TestMode, INT32 i4Index, UINT8* pu1Addr, UINT32 u4Size);
static UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen);
static INT32 _JpgCmdAutoGetFmt(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv);
static INT32 _JpgCmdJSet(INT32 i4Argc, const CHAR ** szArgv);

#ifdef __MODEL_slt__
static INT32 _JpgCmdSlt(INT32 i4Argc, const CHAR ** szArgv);
#endif
extern void *IMG_OSD_create(UINT32 dispColorMode);

void _JPGFillBufCB(
UINT32 u4ImgID,
void *pv_tag,
void *pv_data,
UINT32 u4State);

#endif//ifdef cc emulation
/******************************************************************************
* Variable        : cli "jpg" command table
******************************************************************************/
CLIMOD_DEBUG_FUNCTIONS(JPG)

static CLI_EXEC_T _arJpgCmdTbl[] =
{
#if defined (CC_EMULATION) || defined (__MODEL_slt__)   
    {"init",        "i",    _JpgCmdInit, NULL,            "Jpeg init,(0:OSD AYCbCr8888, 1:OSD YCbYCr, 2:VP)", CLI_SUPERVISOR},
#ifdef JPEG_AUTO_TEST 
    {"decode",       "de",    _JpgCmdAutoDecode, NULL,        "Decode a jpeg file,if decode mode is 4-progressive,please add a index", CLI_SUPERVISOR},
    {"format",       "fmt",    _JpgCmdAutoGetFmt, NULL,        "Get decoded picture format", CLI_SUPERVISOR},
#else
    {"decode",        "de",    _JpgCmdDecode, NULL,        "Decode a jpeg file", CLI_SUPERVISOR},
    {"changeset",     "cs",    _JpgCmdChangeSet, NULL,        "Change test set", CLI_SUPERVISOR},
    {"enable",        "en",    _JpgCmdWriteEnable, NULL,    "Test result write via t32host", CLI_SUPERVISOR},
    {"disable",       "di",    _JpgCmdWriteDisable, NULL,    "Test result ignore", CLI_SUPERVISOR},
    {"t32test",       "tt",    _JpgCmdT32Test, NULL,        "T32 Host Test Suit", CLI_SUPERVISOR},
#endif
   {"8520",        NULL,    vJpegVerify, NULL,        "8520 verification", CLI_SUPERVISOR}, 
#ifdef JPEG_AUTO_TEST
    {"AutoSave",        "save",    _JpgCmdAutoSaveData, NULL,        "Auto save data output data to D:\\jpg_test\\dump\\upload", CLI_SUPERVISOR},
    {"AutoLoad",        "load",    _JpgCmdAutoLoadData, NULL,        "Load file data from D:\\jpg_test\\default", CLI_SUPERVISOR},
    {"AutoTest",        "run",      _JpgCmdPcAutoTest, NULL,        "Auto Burning Test,should set decode mode as mcu-row(jset.8 1) first ", CLI_SUPERVISOR},
    {"SetParam",        "jset",    _JpgCmdJSet, NULL,        "Set Jpeg and imgrz Verify Param", CLI_SUPERVISOR},
    {"cal",		NULL,	 _JpgCmdAutoGetAlign, NULL, 	   "Calculate Align", CLI_SUPERVISOR}, 
#else
    {"decmod",        NULL,    _JpgCmdSetDecMode, NULL,        "5391 set dec mode", CLI_SUPERVISOR},
#endif
    {"wh",        NULL,    _JpgCmdSetTgWH, NULL,        "set target width & height for imgrz output", CLI_SUPERVISOR},
#ifdef __MODEL_slt__    
    {"slt",        NULL,    _JpgCmdSlt, NULL,        "slt", CLI_SUPERVISOR},
#endif 

#endif
    {"debugimg",        "dimg",    _JpgCmdDebugStep, NULL,            "Jpeg _JpgCmdDebugStep(0:JPG, 1:PNG )", CLI_SUPERVISOR},
    CLIMOD_DEBUG_CLIENTRY(JPG),
    {NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Jpg) 
{
    "jpg",
    NULL,
    NULL,
    _arJpgCmdTbl,
    "Jpeg command",
    CLI_SUPERVISOR
};

/******************************************************************************
* Variable        : cli "jpg" enums
******************************************************************************/


/******************************************************************************
* Flag(s)        :
******************************************************************************/

/******************************************************************************
* Implementation        : helper functions for commands parsing
******************************************************************************/
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#ifdef JPEG_AUTO_TEST

extern void JPG_SetDecMode5391(INT32 i4DecMode);
extern void i4JpegSetProgressiveDecodeMode(INT32 newMode);
extern UINT32 _JPG_GetYTgW(void);
extern UINT32 _JPG_GetYTgH(void);
extern VOID vJpegVertifyAutoCreateBuffer(UCHAR *pBufferAddr);
extern INT32 vJpegVerifyAuto(INT32 index,INT32 i4JpgSize);
extern UINT32 StrToDec(const CHAR* pszStr, UINT32 u4Len);
static INT32 _CLI_CMD(const CHAR* szCmd, ...);

static T_JPG_VERIFY_PARAM _JpegVerParam;

static UCHAR *_arOutFileName[]={VERTIFY_DATA_Y,VERTIFY_DATA_C,VERTIFY_DATA_CB,VERTIFY_DATA_CR,VERTIFY_DATA_PIC,VERTIFY_DATA_YUV_Y,VERTIFY_DATA_YUV_U,VERTIFY_DATA_YUV_V};
static UINT32 _CheckSumCompmareInner=0,_InnerComparePicIndex=0;

#if  defined(CC_MT5387) && !defined(CC_MT5363)   
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{108,3},{150,6},{162,1},{180,2},{200,5},{300,4},{200,5}};
#elif defined(JPG_CLK_FROM_JPGDEC_CKCFG)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{240,1},{216,2},{192,3},{162,4},{108,5},{80,6}, {60,7}, {216,2}};
#elif defined(JPG_CLK_FROM_GRAPHIC_5395_PLL)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{108,6},{120,5},{144,4},{168,3},{216,2},{240,1},{999,9},{240,1}};
#elif defined(JPG_CLK_FROM_GRAPHIC)
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{171,4},{172,3},{216,2},{229,5},{240,1},{240,1}};
#else
static INT32 _u4VLDCLKTable[VDEC_VLC_CLK_MAX+1][2]={{100,6},{108,3},{133,5},{162,1},{180,2},{200,4},{200,4}};
#endif

INT32 StrToSInt(const CHAR* pszStr)
{
   BOOL fgNa=FALSE;
   const CHAR* readPos=pszStr;
   UINT32 u4Len;
   UINT32 u4DecVaue=0;
   if (pszStr == NULL)
   {
		return 0;
   }

   if(readPos[0]=='-')
   {
     fgNa=TRUE;
	 readPos++;
   }
   u4Len = x_strlen(readPos);
   u4DecVaue=StrToDec(readPos, u4Len);
   return fgNa ? (-u4DecVaue) : u4DecVaue;
}

INT32 _JpgVerifyCmdSetParam(INT32 i4ParamType,INT32 Param)
{
   _JpgCmdJSet(0xffff0000|i4ParamType,(const CHAR**)Param);
   return 0;
}

static INT32 _JpgCmdJSet(INT32 i4Argc, const CHAR ** szArgv)
{
  E_JPG_VERIFY_PARAM e_JpegParam=JPEG_VERIFY_PARAM_MULTI_INIT;
  BOOL bIsCLI=TRUE,bDecModeChange=FALSE;
  
  
  if((i4Argc&0Xffff0000))
  {
	  bIsCLI=FALSE;
	  e_JpegParam=(E_JPG_VERIFY_PARAM)(i4Argc&0xffff);
  }
  else
  {
    if(i4Argc>1)
	  e_JpegParam=(E_JPG_VERIFY_PARAM)StrToInt(szArgv[1]);
	else
	  e_JpegParam=JPEG_VERIFY_PARAM_SHOW_HELP;
  }
  
  switch(e_JpegParam)
  {
    case JPEG_VERIFY_PARAM_SRC_BUFFER:
	case JPEG_VERIFY_PARAM_TAY_BUFFER:
	case JPEG_VERIFY_PARAM_TAC_BUFFER:
	case JPEG_VERIFY_PARAM_TO_OSD:
	case JPEG_VERIFY_PARAM_RST_OUT:
	case JPEG_VERIFY_PARAM_OUT_MODE:
	case JPEG_VERIFY_PARAM_GRAY_PIC:
	case JPEG_VERIFY_PARAM_DUMP_COLBUF:
	case JPEG_VERIFY_PARAM_BIG_PIC:
	case JPEG_VERIFY_PARAM_VLD_CLK:
	case JPEG_VERIFY_PARAM_OUTSWAP:
	case JPEG_VERIFY_PARAM_OSD_CM:
	case JPEG_VERIFY_PARAM_PIC_FORMAT:
	case JPEG_VERIFY_PARAM_DEC_MODE:
	case JPEG_VERIFY_PARAM_PIC_SIZE:
    case JPEG_VERIFY_PARAM_SRC_BUFSIZE:
	case JPEG_VERIFY_PARAM_MID:
	case JPEG_VERIFY_PARAM_PIC_STOP:
	case JPEG_VERIFY_PARAM_DEC_TIME:
	case JPEG_VERIFY_PARAM_RZ_TIME:

	{
	   INT32 *pShiftBase=NULL,i4ParamShift=0,i4ParamValue=0;
       pShiftBase= &_JpegVerParam.i4JpegSrcBufAddr;
	   i4ParamShift=(e_JpegParam-JPEG_VERIFY_PARAM_SRC_BUFFER);
	   i4ParamValue=bIsCLI ? (INT32)StrToSInt(szArgv[2]) : (INT32)szArgv;
	   if(e_JpegParam==JPEG_VERIFY_PARAM_OSD_CM)
	   	  i4ParamValue+=3;
	   else if(e_JpegParam==JPEG_VERIFY_PARAM_DEC_MODE)
	   	  bDecModeChange=TRUE;
	   else if(e_JpegParam==JPEG_VERIFY_PARAM_VLD_CLK)
       {
          if(i4ParamValue > VDEC_VLC_CLK_MAX)
          {
            i4ParamValue = 0;
          }
          Printf("New VLD Clock: %dMHz\n", _u4VLDCLKTable[i4ParamValue][0]);
       }
	   pShiftBase[i4ParamShift]=i4ParamValue;
       break;
	}
	case JPEG_VERIFY_PARAM_CHECKSUM:
	{
		 INT32 *Param=(INT32 *)szArgv;
		 INT32 CheckSumNum=0,i=0;
         if(bIsCLI)
		    break;
		 
		 CheckSumNum=Param[0];
		 if(CheckSumNum==0 ||CheckSumNum>JPEG_VERTIFY_CHECKSUM_MAX)
		 	break;
		 _JpegVerParam.i4CheckSumNum=CheckSumNum;
		 
         for(i=0;i<CheckSumNum;i++)
         {
           _JpegVerParam.arCheckSum[i]=Param[i+1];
         }
		 break;
	}

	case JPEG_VERIFY_PARAM_CHECKFILE:
	{
		INT32 *Param=(INT32 *)szArgv;
		INT32 CheckFileNum=0,i=0;

        CheckFileNum= bIsCLI ? (INT32)StrToInt(szArgv[2]) : Param[0];
		if(CheckFileNum==0 ||CheckFileNum>JPEG_VERTIFY_CHECKFILE_MAX)
		  break;

		_JpegVerParam.i4CheckFileNum=CheckFileNum;

		for(i=0;i<CheckFileNum;i++)
		{   
		     //Fileindex, Address, Size
			_JpegVerParam.arCheckFile[i]=bIsCLI ? (INT32)StrToInt(szArgv[2+i*3]) : Param[1+i*3];
			_JpegVerParam.arCompAddr[i]= bIsCLI ? (INT32)StrToInt(szArgv[2+i*3+1]) : Param[1+i*3+1];
			_JpegVerParam.arCompSize[i]= bIsCLI ? (INT32)StrToInt(szArgv[2+i*3+2]) : Param[1+i*3+2];
		}
		break;
	}
	case JPEG_VERIFY_PARAM_PARTIAL_OUTPUT:
	{
		INT32 *Param=(INT32 *)szArgv;
		INT32 i = 0; 
		for(i = 0; i < 4; i++)
		{
			_JpegVerParam.i4PartialParam[i] = bIsCLI ? (INT32)StrToSInt(szArgv[2+i]) : Param[1+i];
		}
		break;
	}
	case JPEG_VERIFY_PARAM_MULTI_INIT:
	{
	  _JpegVerParam.i4JpegDecMode=1;
	  _JpegVerParam.i4IsVdo2OSD=0;
	  _JpegVerParam.i4IsRastOut=1;
	  _JpegVerParam.i4JpgOsdModeCM=5;
	  _JpegVerParam.i4JpegModeOutPutMode=-1;
	  _JpegVerParam.i4OutSwapMode=0;
	  _JpegVerParam.i4IsGrayPicture=0;
	  _JpegVerParam.i4DumpColorBuffer=0;
	  _JpegVerParam.i4IsBigPicture=0;
	  _JpegVerParam.i4GetDecTimeProfile=0;
	  _JpegVerParam.i4GetRZTimeProfile=0;
	  _JpegVerParam.i4JpgVldClock=VDEC_VLC_CLK_MAX;
	  bDecModeChange=TRUE;
	
	  break;
	}
	
	case JPEG_VERIFY_PARAM_MULTI_SET:
	{
	   INT32 i4ParamNum=0,i=0,i4ParamValue=0;
	   INT32 *Param=(INT32 *)szArgv;
	   i4ParamNum=bIsCLI ? i4Argc-2 : Param[0];
	   
       if(bIsCLI && !i4ParamNum)
       {
	      Printf("arg: decmode(0:mcu, 1:row, 2:row_rz, 3:pic_rz,4:Prog)\n");
		  Printf("     Vdo2Osd(0,1)  \nOsdCM(0:ARGB8888,1:YCbYCr,2:CbYCrY)||VdoIsRastOut(0,1)\n");  
		  Printf("     OutPutMode(-1,0,1,2) \n OutSwapMode(0,6)\n");
		  Printf("	   GrayPic(0,1)  \nDumpColorBuffer(0:no,1:yes) \n TestBigPicture(0:n,1:yes)\n");
		  Printf("	   ShowDecTimeProfile(0,1)  ShowRZTimeProfile(0,1)\n");
	   }

       for(i=0;i<i4ParamNum;i++)
       {
         i4ParamValue= bIsCLI ? (INT32)StrToSInt(szArgv[2+i]) : Param[1+i];

		 if(i==0)
		 {
		     _JpegVerParam.i4JpegDecMode=i4ParamValue;
			 bDecModeChange=TRUE;
		 }
		 else if(i==1)
		   _JpegVerParam.i4IsVdo2OSD=i4ParamValue;
		 else if(i==2)
		 {
		   if(_JpegVerParam.i4IsVdo2OSD)
		   	{
			   _JpegVerParam.i4JpgOsdModeCM=i4ParamValue+3;
			   _JpegVerParam.i4IsRastOut=1;
		   	}
		   else
		   	{
		   	   _JpegVerParam.i4IsRastOut=i4ParamValue;
			   _JpegVerParam.i4JpgOsdModeCM=5;
		   	}
		 }
		 else if(i==3)
		 {
			 _JpegVerParam.i4JpegModeOutPutMode=i4ParamValue;
		 }
		 else if(i==4)
            _JpegVerParam.i4OutSwapMode=i4ParamValue;
		 else if(i==5)
            _JpegVerParam.i4IsGrayPicture=i4ParamValue;
		 else if(i==6)
            _JpegVerParam.i4DumpColorBuffer=i4ParamValue;
		 else if(i==7)
            _JpegVerParam.i4IsBigPicture=i4ParamValue;
		 else if(i==8)
           _JpegVerParam.i4GetDecTimeProfile=i4ParamValue;
		 else if(i==9)
           _JpegVerParam.i4GetRZTimeProfile=i4ParamValue;

       }
	   if(bIsCLI)
	   	{
	       Printf("--------------------------------------------------------------------\n");
	       Printf("DecMode:%s\n",_JpegVerParam.i4JpegDecMode==0 ? "MCU" : (_JpegVerParam.i4JpegDecMode==1 ? "MCU_ROW" : (_JpegVerParam.i4JpegDecMode==2 ? "MCU_ROW+IMGRZ" : (_JpegVerParam.i4JpegDecMode==3 ? "Pic+IMGRZ" : (_JpegVerParam.i4JpegDecMode==4 ? "Progressive" : "Pic+Video mode output")) ) ));
	       Printf("IsVdo2Osd:%s\n",_JpegVerParam.i4IsVdo2OSD ? "Yes" : "No");
	       if(_JpegVerParam.i4IsVdo2OSD)
		      Printf("OSDOut:%s\n",_JpegVerParam.i4JpgOsdModeCM==3 ? "ARGB8888" : (_JpegVerParam.i4JpgOsdModeCM==4 ? "YCbYCr" : "CbYCrY"));
	       else
		      Printf("VDO RastOut:%s\n",_JpegVerParam.i4IsRastOut ? "Yes" : "No");

		   Printf("OutPutMode:%s\n",_JpegVerParam.i4JpegModeOutPutMode<0 ? "Auto" : (_JpegVerParam.i4JpegModeOutPutMode==0 ? "420" : (_JpegVerParam.i4JpegModeOutPutMode==1 ? "422" : "444") ) );
		   Printf("OutSwapMode:%s\n",_JpegVerParam.i4OutSwapMode==0 ? "16x32" : "64x32");
		   Printf("GrayPictrue:%s\n",_JpegVerParam.i4IsGrayPicture ? "Yes" : "No");
		   Printf("DumpColorBuffer:%s\n",_JpegVerParam.i4DumpColorBuffer ? "Yes" : "No");
		   Printf("BigPictrue:%s\n",_JpegVerParam.i4IsBigPicture ? "Yes" : "No");
		   Printf("ShowRZTimeProfile:%s\n",_JpegVerParam.i4GetDecTimeProfile ? "Yes" : "No");
		   Printf("ShowDecTimeProfile:%s\n",_JpegVerParam.i4GetRZTimeProfile ? "Yes" : "No");
		   Printf("--------------------------------------------------------------------\n");
	   	}
	   break;
	}

	case JPEG_VERIFY_PARAM_SHOW_HELP:
	{
		INT32 i=0,i4Pos=0;
		UCHAR pu1LogStr[256]={0};
		Printf("-------------------------------------------------------\n");
        Printf("Cmd Type:jpg.jset type [value...]\n");
		Printf(" 0-->MultiSet(decmode isVdo2Osd [OsdCm,RstOut] OutPutMode OutSwapMode GrayPic DumpColorBuf BigPicture DecTime RZTime)\n");
	    Printf(" 1-->CheckSum(%d",_JpegVerParam.i4CheckSumNum);
		for(i=0;i<_JpegVerParam.i4CheckSumNum;i++)
		{
           Printf(" 0x%x ",_JpegVerParam.arCheckSum[i]);
		}
		Printf(")\n 2-->CheckFile(%d",_JpegVerParam.i4CheckFileNum);
		for(i=0;i<_JpegVerParam.i4CheckFileNum;i++)
		{
			Printf("[%s Addr:0x%x Size:0x%x]",_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompSize[i]);
		}
		Printf(")\n 3-->SetSrcBufferAddr(0x%x)\n 4-->SetSrcBuffSize(0x%x)\n",_JpegVerParam.i4JpegSrcBufAddr,_JpegVerParam.i4JpegSrcBufSize);
        Printf(" 5-->SetSrcPicSize(%d Bytes)\n",_JpegVerParam.i4JpegPicSize);
		Printf(" 6-->SetYTargetBuffer(0x%x)\n 7-->SetCTargetBuffer(0x%x)\n",_JpegVerParam.i4JpegTaYBufAddr,_JpegVerParam.i4JpegTaCBufAddr);
        i4Pos=0;
		for(i=0;i<VDEC_VLC_CLK_MAX;i++)
        {
          UCHAR *puStr=pu1LogStr+i4Pos;
		  x_sprintf((CHAR *)puStr,"%d:%dMHz ",i,_u4VLDCLKTable[i][0]);
	      i4Pos+=9;
        }
		pu1LogStr[i4Pos]=0;
		
		Printf(" 8-->SetDecMode(%d)(0:MCU 1:MCU-Row 2:MCU-Row+Imgrz 3:Pic+imgrz 4: Pic+video mode output 5-10:progressive mode )\n 9-->SetVLDClock(%d %dMHz %d) (%s)\n",_JpegVerParam.i4JpegDecMode,_JpegVerParam.i4JpgVldClock,_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][0],_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][1],pu1LogStr);
		Printf("10-->IsVdo2Osd(%d)(IMGRZ:if set, imgrz output is one component,else imgrz output is Y/C two component)\n11-->OsdCM(%d)(IMGRZ:0-AYCbCr8888,1-YCbYCr4444,2-CbYCrY4444.used when IsVdo2Osd is seted.)\n12-->RstOut(%d)(IMGRZ:output is rast or block for Y/C two component,used when IsVdo2Osd is not be seted)\n",_JpegVerParam.i4IsVdo2OSD,_JpegVerParam.i4JpgOsdModeCM-3,_JpegVerParam.i4IsRastOut);
		Printf("13-->OutPutMode(%d)(-1:Default,0:420 1:422)(IMGRZ:if IsVdo2Osd is not set, this define for imgrz output format)\n14-->GrayPic(%d)(If input is a gray picture,please set this flag)\n15-->DumpColorBuffer(%d)(if want force dump color buffer,please set this flag)\n",_JpegVerParam.i4JpegModeOutPutMode,_JpegVerParam.i4IsGrayPicture,_JpegVerParam.i4DumpColorBuffer);
		Printf("16-->IsBigPicture(%d)(not use)\n17-->OutSwapMode(%d)(IMGRZ:0--16x32,6:64x32)\n18-->TestMID(%d)(not use)\n",_JpegVerParam.i4IsBigPicture,_JpegVerParam.i4OutSwapMode,_JpegVerParam.i4MidTest);
		Printf("19-->TestPicStop(%d)(force stop decode)\n20-->DecTime(%d)\n21-->ReSizeTime(%d)\n",_JpegVerParam.i4PicModeStopTest,_JpegVerParam.i4GetDecTimeProfile,_JpegVerParam.i4GetRZTimeProfile);
		Printf("22-->PicFormat(%d)(read only)\n",_JpegVerParam.i4PicFormat);
		Printf("23-->Partial Output Param (%d %d %d %d)(x,y,w,h)\n",_JpegVerParam.i4PartialParam[0],_JpegVerParam.i4PartialParam[1],_JpegVerParam.i4PartialParam[2],_JpegVerParam.i4PartialParam[3]);
		Printf("100-->Init\n200->Help\n");
		Printf("-------------------------------------------------------\n");
		break;
	}
	
	default:
	{
      Printf("No This Param:%d\n",e_JpegParam);
	  break;
    }
	
  }

  if(bDecModeChange)
  {
	  if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_BASELINE_MCU)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_MCU_ROW)
	  {
        JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_MCU_ROW_IMGRZ)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
	  }
	  else if(_JpegVerParam.i4JpegDecMode == PIC_DEC_MODE_PICTURE_IMGRZ)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
	  }
	  else if(IS_PROGRESSIVE_DECODER)
	  {
        i4JpegSetProgressiveDecodeMode(_JpegVerParam.i4JpegDecMode-PIC_DEC_MODE_PROG_MCU_MULTI_COLLECT);
	  }
	  else if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_VIDEO_MODE)
	  {
		JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_VIDEO_OUTPUT);
	  }
      else
      {
		Printf("invalid decode mode:%d\n", _JpegVerParam.i4JpegDecMode);
        return 0;
      }
      Printf("New decode mode: %s\n", decode_mode_str[_JpegVerParam.i4JpegDecMode]);
  }
  
  return 0;
}


INT32 _JpgVerifyCmdGetParam(INT32 i4ParamType, INT32 Param)
{
  switch(i4ParamType)
  {
    case JPEG_VERIFY_PARAM_SRC_BUFFER:
	case JPEG_VERIFY_PARAM_TAY_BUFFER:
	case JPEG_VERIFY_PARAM_TAC_BUFFER:
	case JPEG_VERIFY_PARAM_TO_OSD:
	case JPEG_VERIFY_PARAM_RST_OUT:
	case JPEG_VERIFY_PARAM_OUT_MODE:
	case JPEG_VERIFY_PARAM_GRAY_PIC:
	case JPEG_VERIFY_PARAM_DUMP_COLBUF:
	case JPEG_VERIFY_PARAM_BIG_PIC:
	case JPEG_VERIFY_PARAM_CHECKSUM:
	case JPEG_VERIFY_PARAM_VLD_CLK:
	case JPEG_VERIFY_PARAM_OUTSWAP:
	case JPEG_VERIFY_PARAM_OSD_CM:
	case JPEG_VERIFY_PARAM_PIC_FORMAT:
	case JPEG_VERIFY_PARAM_DEC_MODE:
	case JPEG_VERIFY_PARAM_PIC_SIZE:
    case JPEG_VERIFY_PARAM_SRC_BUFSIZE:
    case JPEG_VERIFY_PARAM_MID:
	case JPEG_VERIFY_PARAM_PIC_STOP:
	case JPEG_VERIFY_PARAM_DEC_TIME:
	case JPEG_VERIFY_PARAM_RZ_TIME:
   	{
		 
		INT32 *pShiftBase=NULL,i4ParamShift=0,i4Ret=0;
		pShiftBase= &_JpegVerParam.i4JpegSrcBufAddr;
		i4ParamShift=(i4ParamType-JPEG_VERIFY_PARAM_SRC_BUFFER);
		
		if(Param)
		{
          *((INT32 *)Param)=pShiftBase[i4ParamShift];
		}

		if(i4ParamType==JPEG_VERIFY_PARAM_VLD_CLK)
		   i4Ret=_u4VLDCLKTable[pShiftBase[i4ParamShift]][1];
		else
		   i4Ret=pShiftBase[i4ParamShift];
		return i4Ret ;
   	}
	case JPEG_VERIFY_PARAM_PARTIAL_OUTPUT:
	{
		if(Param)
		{
		   *((INT32 *)Param)=(INT32)_JpegVerParam.i4PartialParam;
		}
		break;
	}
	
    default:
	{
       Printf("Not Support %d,Please Eidt This Functin\n",i4ParamType);
	   break;
    }
  }
  return 0;
}


#ifdef JPG_SEMI_HOST_BY_USB
#include "x_fm.h"
BOOL _JPGLoadUsbFile(CHAR *pcFileName, UINT32 u4Addr)
{
    UINT64 u8FileSize, u8Tmp;
    UINT32 u4Request;
#ifdef __KERNEL__
	UINT32 u4Read;
    HANDLE_T  h_file;
#else
	UINT64 u8Read = 0;
	struct file *filep = NULL;
#endif
#ifndef __KERNEL__
    VERIFY(FMR_OK == x_fm_open(FM_ROOT_HANDLE, pcFileName, FM_READ_ONLY, 0777, FALSE, &h_file));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_END, &u8FileSize));
    VERIFY(FMR_OK == x_fm_lseek(h_file, 0, FM_SEEK_BGN, &u8Tmp));
#else
	VERIFY((filep = FILE_Open(_acJpgFileName, O_RDONLY, 0))!=NULL);
	u8FileSize = FILE_llSeek(filep, 0, 2);
	u8Tmp = FILE_llSeek(filep, 0, 0);
#endif		
    while (u8FileSize > 0)
    {
        if (u8FileSize >> 32)
        {
            u4Request = 0xffffffff;
        }
        else
        {
            u4Request = (UINT32)u8FileSize;
        }
#ifndef __KERNEL__
        x_fm_read(h_file, (VOID *)u4Addr, u4Request, &u4Read);
        ASSERT(u4Request == u4Read);
        u4Addr += u4Read;
        u8FileSize -= u4Read;
#else
		FILE_Read(filep, (VOID *)u4Addr, u4Request, &u8Read);
		ASSERT(u4Request == (UINT32)u8Read);
        u4Addr += (UINT32)u8Read;
        u8FileSize -= (UINT32)u8Read;
#endif
    }
#ifndef __KERNEL__
    VERIFY(FMR_OK == x_fm_close(h_file));
#else
	FILE_Close(filep);
#endif
    return TRUE;
}
#endif

static INT32 _JpgCmdAutoLoadData(INT32 i4Argc, const CHAR ** szArgv)
{
       UCHAR *szFileName=NULL;

#ifdef JPG_SEMI_HOST_BY_USB
   	CHAR pcIjpgFileName3[256];
#endif

	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc < 3)
	{
		Printf("arg: filename | filesize\n");
		return 0;
	}

	 szFileName=(UCHAR *)(szArgv[1]);
     _JpegVerParam.i4JpegPicSize=(INT32)StrToInt(szArgv[2]);
#ifdef JPG_SEMI_HOST
	  LOG(0,"Load File:%s",szFileName);
	  LOG(0,"\n");
      if(IS_PROGRESSIVE_DECODER)
      {
		  vJpegVertifyAutoCreateBuffer((UCHAR *)_JpegVerParam.i4JpegSrcBufAddr);
	  }

#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.i4JpegSrcBufAddr), (UINT32)(_JpegVerParam.i4JpegSrcBufSize));
#else
    HalFlushInvalidateDCache();
#endif
	#ifdef JPG_SEMI_HOST_BY_USB
		x_sprintf(pcIjpgFileName3 , "/mnt/usb_0/Jpg_test/default/");			     
		x_strcat(pcIjpgFileName3, (CHAR *)szFileName);
		//Printf("116 imgrz load %s \n", pcIjpgFileName3);              
		//Printf("117 load addr %x \n", _JpegVerParam.i4JpegSrcBufAddr);
		_JPGLoadUsbFile(pcIjpgFileName3, _JpegVerParam.i4JpegSrcBufAddr);
	#else
		UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny", "D:\\jpg_test\\default",szFileName,_JpegVerParam.i4JpegSrcBufAddr));
	#endif
#endif

        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.i4JpegSrcBufAddr), (UINT32)(_JpegVerParam.i4JpegSrcBufSize));
        #else
        HalFlushInvalidateDCache();
        #endif	
    
	  return 0;
}

static INT32 _JpgCmdAutoGetFmt(INT32 i4Argc, const CHAR ** szArgv)
{
   if(_JpegVerParam.i4PicFormat==4)
   	{
   	   Printf("Format:Unknow\n");
	   return 0;
   	}
   
   Printf("Format:%s\n",_JpegVerParam.i4PicFormat==0 ? "444" : (_JpegVerParam.i4PicFormat==1 ? "422V" : (_JpegVerParam.i4PicFormat==2 ? "422":"420")));
   return 0;
}

static INT32 _JpgCmdAutoDecode(INT32 i4Argc, const CHAR ** szArgv)
{
  
	if(!IS_PROGRESSIVE_DECODER)
	{
	    if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_MCU_ROW_IMGRZ || _JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_IMGRZ|| _JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_PICTURE_VIDEO_MODE)
	    {
	       UINT32 u4TagH,u4TagW;
		   u4TagW=_JPG_GetYTgW();
	       u4TagH=_JPG_GetYTgH();
		   if(_JpegVerParam.i4IsVdo2OSD)
		   {
		       if(_JpegVerParam.i4JpgOsdModeCM==3)
		       {
				   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*4*u4TagH);
		       }
			   else
			   {
				   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*2*u4TagH);
			   }
		   }
		   else
		   {
			   x_memset((void *)(_JpegVerParam.i4JpegTaYBufAddr),0,(u4TagW+16)*u4TagW);
			   x_memset((void *)(_JpegVerParam.i4JpegTaCBufAddr),0,(u4TagW+16)*u4TagW);
		   }
	    }
        _JPGDecodePicture(0, -1, (UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr), _JpegVerParam.i4JpegPicSize);
	}
	else
	{
	    INT32 i4Index=0;
	    if(i4Argc<2)
	    {
           Printf("de index(index in progressive file table,-1 to decodloaded progressive file)\n   if is not decode progressive file,pls non't set decode mode is 4 (jpg.jset 8 value)\n");
		   return 0;
	    }

		if(szArgv[1][0]=='-')
		{
          i4Index=-1;
		}
		else
		{
          i4Index=(INT32)StrToInt(szArgv[1]);
		}
		vJpegVertifyAutoCreateBuffer((UCHAR *)(_JpegVerParam.i4JpegSrcBufAddr));
		vJpegVerifyAuto(i4Index,_JpegVerParam.i4JpegPicSize);
	}
	_JpgCmdAutoGetFmt(0,NULL);
    return E_CLIJPG_OK;
}
static INT32 _JpgCmdAutoSaveData(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i=0;
   //UCHAR **pUploadFileName=NULL;
   UCHAR *prefix=NULL;
   const CHAR ** szArgParam=szArgv+1;
   
   if(i4Argc>1 && szArgv[1][0]=='-' && szArgv[1][1]=='p')
   {
     prefix=(UCHAR *)(szArgv[2]);
	 szArgParam=szArgv+3;
   }
   
   if(_JpegVerParam.i4CheckFileNum>=JPEG_VERTIFY_CHECKFILE_MAX)
   {
	   return 0;
   }       
    
   for(i=0;i<_JpegVerParam.i4CheckFileNum;i++)
   {
        #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
        HalFlushInvalidateDCacheMultipleLine((UINT32)(_JpegVerParam.arCompAddr[i]), (UINT32)(_JpegVerParam.arCompSize[i]));
        #else
        HalFlushInvalidateDCache();
        #endif
    
#ifdef JPG_SEMI_HOST
     if(prefix)
     {         
		 Printf("Save File:%s/%s_%s\n","D:/jpg_test/dump/upload",prefix,_arOutFileName[_JpegVerParam.arCheckFile[i]]);
		 UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s_%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",prefix,_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));
     }
	 else
	 {
		 if(i4Argc>_JpegVerParam.i4CheckFileNum)
		 {
			Printf("Save File:%s/%s\n","D:/jpg_test/dump/upload",szArgParam[i]);
			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",szArgParam[i],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));
		 }
		 else
		 {
      		Printf("Save File:%s/%s\n","D:/jpg_test/dump/upload",_arOutFileName[_JpegVerParam.arCheckFile[i]]);
   			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s\" 0x%08x--0x%08x","D:/jpg_test/dump/upload",_arOutFileName[_JpegVerParam.arCheckFile[i]],_JpegVerParam.arCompAddr[i],_JpegVerParam.arCompAddr[i]+_JpegVerParam.arCompSize[i]-1));		   
    	 }
	 }
     
#endif
   }
        //HalFlushInvalidateDCache();

	Printf("------------\n");
        
   return 0;
}

/*
static BOOL _JpgRunFlow(INT32 i4Argc, const CHAR ** szArgv)
{
    IMG_Lib_Init();   
    x_memset((VOID *)&rDecOut, 0x0, sizeof(IMG_REGION_T));
    rInfo.pu1InBuffer = (UINT8 *)_rPesInfo.u4FifoStart;
    rInfo.u8BufSize = (UINT32)(_rPesInfo.u4FifoEnd - _rPesInfo.u4FifoStart);
    rInfo.rDstRegion.u4StartX = (UINT32)0;
    rInfo.rDstRegion.u4StartY = (UINT32)0;
    
    rInfo.rDstRegion.u4Width = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.rDstRegion.u4Height = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_height;
    rInfo.rDstRegion.u4Pitch = (UINT32)prVdecEsInfo->rMMSeqInfo.u2_width;
    rInfo.eDispColorMode = Y_CBCR420_SEP16;
    rInfo.eRotateDegree = CLOCKWISE_0;
    rInfo.bKeepRatio = TRUE;
    rInfo.prDecOut = &rDecOut;
    
    rInfo.rDstRegion.pu1Addr = (UINT8 *)u4YAddr;
    rInfo.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
    //TO DO
    rInfo.rMjpg.u4RingOffset = _rPesInfo.u4VldReadPtr - _rPesInfo.u4FifoStart;
    rInfo.rMjpg.ucFbgId = _ucMJPEGFbgId;

    IMG_SetFillBsBufEnd((UINT32)rInfo.u8BufSize, 0, TRUE);
    IMG_Decode(rInfo); 

}

*/
static BOOL _JpgChecksumAutoCheck(UINT32 *pru4Checsum)
{
   INT32 i4CheckSumIndex;
   BOOL bRet=TRUE;//,bNeedCheck=TRUE;

   for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
   {
     if(pru4Checsum[i4CheckSumIndex]!=0)
     {
       break;
     }
   }

   if(i4CheckSumIndex==_JpegVerParam.i4CheckSumNum)
   {
   	 for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
	 {
	   Printf("ResultChecSum[%d]=0x%x\n",i4CheckSumIndex,_JpegVerParam.arCheckSum[i4CheckSumIndex]);
	 }
     Printf("No golden checksum to compare\n");
	 return TRUE;
   }
    
   for(i4CheckSumIndex=0;i4CheckSumIndex<_JpegVerParam.i4CheckSumNum;i4CheckSumIndex++)
   {
	 if(_JpegVerParam.arCheckSum[i4CheckSumIndex]!=pru4Checsum[i4CheckSumIndex])
	 {
		Printf("ChecSum[%d]=(Golden:0x%x,Result:0x%x)\n",i4CheckSumIndex,pru4Checsum[i4CheckSumIndex],_JpegVerParam.arCheckSum[i4CheckSumIndex]);
        bRet=FALSE;
	 }
   }
   return bRet;
}
static INT32 _JpgCmdAutoGetAlign(INT32 i4Argc, const CHAR ** szArgv)
{
  INT32 i4Number=0,i4Align=0;
  if(i4Argc<3)
  {
    Printf("arg:Num Align\n");
    return 0;
  }
  i4Number=(INT32)StrToInt(szArgv[1]);
  i4Align=(INT32)StrToInt(szArgv[2]);

  if(i4Align==0)
  	return 0;
  
  Printf("Align=%d\n",i4Align*((i4Number+(i4Align-1))/i4Align));
  return 0;
}
extern UINT32 _Jpg_GetDecChkSum(void);
static INT32 _JpgCmdMcuRowBurningTest(UINT32 u4StartPicture)
{
   UINT32 u4PictureIndex=0,u4PicSetIndex=0,u4MaxSetIndex=0,u4ChkSum = 0;
   INT32 i4Ret=0;
   u4MaxSetIndex=JPGCLI_GetMaxSet();

   if(u4StartPicture!=0)
   {
	   for(u4PicSetIndex=0;u4PicSetIndex<u4MaxSetIndex;u4PicSetIndex++)
	   {
	     INT32 i4PicInSet=0;
	      _InnerComparePicIndex=u4PictureIndex;
		  JPGCLI_GetTestSet(u4PicSetIndex, &_arJpgTestSet, &_szInputDir);
		  while( _arJpgTestSet[i4PicInSet].szShortName != NULL)
		  {
              u4PictureIndex++;
			  if(u4PictureIndex>=u4StartPicture)
			  {
                 break;
			  }
			  i4PicInSet++;
		  }

		  if(_arJpgTestSet[i4PicInSet].szShortName!=NULL)
		  {
              break;
		  }
	   }
   }
   else
   {
	   _InnerComparePicIndex=0;
   }

   Printf("StartSet=%d,VLDClock=%dMHz,RegisterVal=%d\n",u4PicSetIndex,_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][0],_u4VLDCLKTable[_JpegVerParam.i4JpgVldClock][1]);
   for(;u4PicSetIndex<u4MaxSetIndex;u4PicSetIndex++)
   {
	   JPGCLI_GetTestSet(u4PicSetIndex, &_arJpgTestSet, &_szInputDir);
	   u4PictureIndex=0;
	   while( _arJpgTestSet[u4PictureIndex].szShortName != NULL)
	   {
		   i4Ret = _JPGDecodePicture(0, u4PictureIndex, NULL, 0);

		   if(i4Ret!=0)
		   {
             LOG(0,"Picture Decode Error: Set=%d,Pic=%d,Ret=%d\n",u4PicSetIndex,u4PicSetIndex,i4Ret);
		   }
				u4ChkSum = _Jpg_GetDecChkSum();
				Printf("u4ChkSum 0x%08x\n",u4ChkSum);
				if(_CheckSumCompmareInner)
				{
				   if(u4ChkSum ==pAutoRunCheckSum[_InnerComparePicIndex])
					{
						Printf("%d:CheckSum Pass\n",_InnerComparePicIndex);
					}
				   else
					{
						Printf("%d,CheckSum Fail\n",_InnerComparePicIndex);
					}
				}
				
           u4PictureIndex++;
		   _InnerComparePicIndex++;
	   }
   }
   _InnerComparePicIndex=0;
   return 0;
}

static INT32 _JpgCmdBaseAutoTest(T_JPG_AUTO_TEST_CASE *prTestCase)
{
    UCHAR decode_mode_idx = 0;
    static UINT32 decode_mode[] = {PIC_DEC_MODE_MCU_ROW, PIC_DEC_MODE_MCU_ROW_IMGRZ, PIC_DEC_MODE_PICTURE_IMGRZ};
    static UCHAR *decode_mode_name[] = {"MCU_ROW", "ROW_RZ", "PIC_RZ"};
    
    while(prTestCase->pu1InputFileName)
    {
        UCHAR pszName[64]={0},*uNamePos;
        UINT32 u4StrIndex=0;
        uNamePos=prTestCase->pu1InputFileName;
        while(*uNamePos!='.')
        {
            pszName[u4StrIndex++]=*uNamePos++;
        }

         _CLI_CMD("load %s %d",prTestCase->pu1InputFileName,prTestCase->puInputFileSize);
        for(decode_mode_idx=0; decode_mode_idx<sizeof(decode_mode)/sizeof(decode_mode[0]); decode_mode_idx++)
        {
            if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_BASELINE_MCU)
            {
                _CLI_CMD("jset 15 0");
            }
            else if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_MCU_ROW_IMGRZ)/*use video mode*/
            {
                _CLI_CMD("jset 10 0");
                _CLI_CMD("jset 11 1");
            }
            else if(decode_mode[decode_mode_idx] == PIC_DEC_MODE_PICTURE_IMGRZ)/*use osd mode*/
            {
                _CLI_CMD("jset 10 1");
                _CLI_CMD("jset 11 0");
            }
            _CLI_CMD("jset 8 %d", decode_mode[decode_mode_idx]);
            _CLI_CMD("de");

            _CLI_CMD("save -p %s(%s)", pszName, decode_mode_name[decode_mode_idx]);
        }
        prTestCase++;
    }
  return 0;
}

static INT32 _JpgCmdProgAutoTest()
{
  INT32 i4ProgIndex=0;
  
  if(!IS_PROGRESSIVE_DECODER)
  {
    Printf("Current decode mode is not progressive: %s\n", decode_mode_str[_JpegVerParam.i4JpegDecMode]);
    return 0;
  }
  
  for(i4ProgIndex=0;i4ProgIndex<PROGRESSIVE_AUTO_TEST_NUM;i4ProgIndex++)
  {
    _CLI_CMD("de %d",i4ProgIndex);
  }

  return 0;
}

static INT32 _JpgCmdNormalAutoTest(T_JPG_AUTO_TEST_CASE *prTestCase)
{
  UCHAR puDoCmd[256],*puCmd;
  UINT32 u4CmdIndex=0;
  //BOOL bfgDoCmd=TRUE;
  _CLI_CMD("jset 8 1");
  while(prTestCase->pu1InputFileName)
  {
     _CLI_CMD("load %s %d",prTestCase->pu1InputFileName,prTestCase->puInputFileSize);
	 puCmd=prTestCase->pu1Comd;
	 if(puCmd)
	 {
	    do
	    {
			u4CmdIndex=0;
			while(*puCmd!='*')
			{
			  puDoCmd[u4CmdIndex++]=*puCmd++;
			}
			puDoCmd[u4CmdIndex]=0;
			_CLI_CMD((const CHAR*)(puDoCmd));
			puCmd++;
	    }while(*puCmd!='*');

        if(_JpgChecksumAutoCheck(prTestCase->aru4CheckSum))
        {
           Printf("Normal Test Pass:%s\n",prTestCase->pu1InputFileName);
        }
		else
	    {
			Printf("Normal Test Fail:%s\n",prTestCase->pu1InputFileName);
		}

		
	 }
	 else
	 {   
		 //CMU ROW
		 _CLI_CMD("jset 8 1");
		 _CLI_CMD("de");
	     if(_JpgChecksumAutoCheck(prTestCase->aru4CheckSum))
         {
            Printf("MCU-ROW Normal Test Pass:%s\n",prTestCase->pu1InputFileName);
         }
		 else
	     {
			Printf("MCU-ROW Normal Test Fail:%s\n",prTestCase->pu1InputFileName);
		 }
		 
		 //PIC-RZ
		 
		 _CLI_CMD("jset 8 3");
		 _CLI_CMD("jset 10 1");
		 _CLI_CMD("jset 11 0");
		 //_CLI_CMD("wh 768 512");
		 _CLI_CMD("de");
         prTestCase->u4FlgSave=TRUE;
		  
	 }

	 if(prTestCase->u4FlgSave)
	 {   
	     UCHAR pszName[64]={0},*uNamePos;
		 UINT32 u4StrIndex=0;
		 uNamePos=prTestCase->pu1InputFileName;
		 while(*uNamePos!='.')
		 {
            pszName[u4StrIndex++]=*uNamePos++;
		 }
		 _CLI_CMD("save -p %s",pszName);
	 }
	 prTestCase++;
  }

  return 0;
}
static INT32 _JpgCmdPcAutoTest(INT32 i4Argc, const CHAR ** szArgv)
{
   INT32 i4InnerBurningTestType=0,i4Loop=0,i4StartClk=0,i4EndClk=0,i=0,j=0;
   UINT32 u4StartPicIndex=0;
   INT32 i4RandomClk[VDEC_VLC_CLK_MAX]={0};
   
   if(i4Argc<2)
   {
       Printf("run 0 common baseline jpeg file verify\n");
   	   Printf("run 1 ClockIndex LoopTime\n");
   	   Printf("run 2 StartClkIndex EndClkIndex LoopTime\n");
   	   Printf("run 3 ClockIndex1,ClockIndex2,ClockIndex... LoopTime\n");
   	   Printf("run 4 Auto test normal and special picture\n");
   	   Printf("run 5 Auto test progressive picture\n");
	   return 0;
   }
    
    i4InnerBurningTestType=(INT32)StrToInt(szArgv[1]);

    if(i4InnerBurningTestType==0)
	{
        _JpgCmdBaseAutoTest(_rBaseTestCase);
	   return 0;
	}
    else if(i4InnerBurningTestType==1)// loop one clk for some times;
    {
       if(i4Argc<4)
       {
		 Printf("run 1 ClockIndex LoopTime\n run at one clk for some loop\n");
		 return 0;
       }
  
	   _JpegVerParam.i4JpgVldClock=(INT32)StrToInt(szArgv[2]);
	   i4Loop=(INT32)StrToInt(szArgv[3]);
	   if(i4Argc>=5)
	   {
          u4StartPicIndex=(INT32)StrToInt(szArgv[4]);
	   }
    }
    else if(i4InnerBurningTestType==2) // form start to end clk for some times.
    {
      if(i4Argc<5)
      {
		  Printf("run 2 StartClkIndex EndClkIndex LoopTime\n run form start to end clk for some loop\n");
		  return 0;
      }
	  
      i4StartClk=(INT32)StrToInt(szArgv[2]);
	  i4EndClk=(INT32)StrToInt(szArgv[3]);
      i4Loop=(INT32)StrToInt(szArgv[4]);
	  if(i4Argc>=6)
	  {
          u4StartPicIndex=(INT32)StrToInt(szArgv[5]);
	  }
    }
	else if(i4InnerBurningTestType==3) // do some clk for some times.
	{
	   if(i4Argc<4)
	   {
		 Printf("run 3 ClockIndex1,ClockIndex2,ClockIndex... loopTime\n run at some clk for some loop\n");
		 return 0;
	   }
	   
       for(i=2;i<i4Argc-1;i++)
       {
           i4RandomClk[i-2]=(INT32)StrToInt(szArgv[i]);
       }
	   i4Loop=(INT32)StrToInt(szArgv[i]);
	}
	else if(i4InnerBurningTestType==4)
	{
	   _JpgCmdNormalAutoTest(_rAutoTestCase);
       return 0;
	}
	else if(i4InnerBurningTestType==5)
	{
       _JpgCmdProgAutoTest();
	   return 0;
	}
	
	
	_CheckSumCompmareInner=1;

    for(i=0;i<i4Loop;i++)
    {
       Printf("MCU_ROW Stress Test:Loop=%d\n",i);
       if(i4InnerBurningTestType==1)
	      _JpgCmdMcuRowBurningTest(u4StartPicIndex);
	   else if(i4InnerBurningTestType==2)
	   {
          for(_JpegVerParam.i4JpgVldClock=i4StartClk;_JpegVerParam.i4JpgVldClock<=i4EndClk;_JpegVerParam.i4JpgVldClock++)
          {
             _JpgCmdMcuRowBurningTest(u4StartPicIndex);
			 u4StartPicIndex=0;
          }
	   }
	   else if(i4InnerBurningTestType==3)
	   {
          for(j=0;j<i4Argc-3;j++)
          {
             _JpegVerParam.i4JpgVldClock=i4RandomClk[j];
             _JpgCmdMcuRowBurningTest(u4StartPicIndex);
          }
	   }
	   u4StartPicIndex=0;
    }

	_CheckSumCompmareInner=0;
	return 0;
}


#endif
extern void _JPG_SetYTgW(UINT32 u4Width);
extern void _JPG_SetYTgH(UINT32 u4Height);
static INT32 _CLI_CMD(const CHAR* szCmd, ...)
{
    CHAR szBuf[256];
    VA_LIST t_ap;

    VA_START(t_ap, szCmd);
    x_vsprintf(szBuf, szCmd, t_ap);
    VA_END(t_ap);

    return CLI_Parser(szBuf);
}
static INT32 _JpgCmdSetTgWH(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4Width, u4Height;
	if(szArgv == NULL)
	{
		return 0;
	}
	
	if (i4Argc < 3)
	{
	    Printf("arg: width height");
	}
	
    u4Width  = StrToInt(szArgv[1]);
    u4Height = StrToInt(szArgv[2]);
    
    _JPG_SetYTgW(u4Width);
    _JPG_SetYTgH(u4Height);

    Printf("target width = %u, height = %u\n", u4Width, u4Height);
    return 0;
}
/*
static INT32 _JpgCmdSetTgH(INT32 i4Argc, const CHAR ** szArgv)
{
    return 0;
}
*/
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdWriteEnable(INT32 i4Argc, const CHAR ** szArgv)
{
	UNUSED(i4Argc);
	UNUSED(szArgv);
	_fgJpegWriteEnable = TRUE;
	return 0;
}

static INT32 _JpgCmdWriteDisable(INT32 i4Argc, const CHAR ** szArgv)
{
	UNUSED(i4Argc);
	UNUSED(szArgv);
	_fgJpegWriteEnable = FALSE;
	return 0;
}
#endif

static INT32 _JpgCmdInit(INT32 i4Argc, const CHAR ** szArgv)
{
	UINT32 u4DispMode;
	FBM_POOL_T*prFbmPool=NULL;
	
        #ifdef JPEG_AUTO_TEST_PERFORMANCE
        UINT32 u4FbmId=FBM_POOL_TYPE_SCPOS_DYN_MAIN,u4FbmId2=FBM_POOL_TYPE_SCPOS_DYN_SUB;
        #else
        UINT32 u4FbmId=FBM_POOL_TYPE_B2R,u4FbmId2=FBM_POOL_TYPE_SCPOS_MAIN;
        #endif
#if 1
	if(szArgv == NULL)
	{
		return 0;
	}
	if (i4Argc > 1)
	{
		u4DispMode = StrToInt(szArgv[1]);   
		if(u4DispMode >= (UINT32)COLOR_MODE_MAX)
		{
			return (INT32)E_CLIJPG_ERR_GENERAL;
		}

		if(i4Argc > 3)
		{
          u4FbmId=StrToInt(szArgv[2]);
		      u4FbmId2=StrToInt(szArgv[3]);
		}
    else
    {
            UINT32 u4FbmAddr, u4FbmSize;
            u4FbmAddr = DRVCUST_OptGet(eFbmMemAddr);
            u4FbmSize = DRVCUST_OptGet(eFbmMemSize);
            u4FbmAddr = u4FbmAddr + u4FbmSize - 10 * 1024 * 1024;
            u4FbmId = u4FbmAddr;
            u4FbmId2 = u4FbmId + 5 * 1024 * 1024;
    }


		/*
		if(u1FbmId>=FBM_POOL_TYPE_TOTAL || u1FbmId2>=FBM_POOL_TYPE_TOTAL)
			
		{
           return (INT32)E_CLIJPG_ERR_GENERAL;
		}
		*/
	}
	else
	{
		LOG(0, "usage:\n\t jpg.i {color mode,TagFbmId}\n");
		LOG(0, "\t {color mode} 0:OSD AYCbCr8888, 1:Video Plane\n");
		return (INT32)E_CLIJPG_OK;
	}
	
	if (!_fgJpegInstance)
	{
		_fgJpegInstance = TRUE;
		if(u4DispMode == 0)		//OSD ayuv8888
		{
			_eDispMode = AYCBCR8888_DIRECT32;
			_prOsd = (DRV_OSD_T *)IMG_OSD_create(6); //OSD_CM_AYCBCR8888_DIRECT32
			if(_prOsd == NULL)
			{
				LOG(0, "OSD create failed\n");
				return (INT32)E_CLIJPG_ERR_GENERAL;
			}
		}
		else
		{
			if(IMG_VDP_Connect(0) != E_IMG_OK) //VDP_1 main
			{
				return (INT32)E_CLIJPG_ERR_GENERAL;
			}		
		}

		if(IMG_Lib_Init()!= E_IMG_OK)
		{
			return (INT32)E_CLIJPG_ERR_GENERAL;
		}
		VERIFY (x_sema_create(&_hJPGDecSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
	}
	UNUSED(szArgv);
#endif
	// init mpv
	//MPV_PllInit();
	//*(volatile UINT32*)0x2000d048 = 3;
#ifdef JPEG_AUTO_TEST
    x_memset(&_JpegVerParam,0,sizeof(T_JPG_VERIFY_PARAM));
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_MULTI_INIT,NULL);
	
	#if 0//def JDEC_HW_IO_MMU
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,(INT32)(0x3600000));
	#else
	//_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,((INT32)pTestBuffer));	
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFFER, (INT32)SRC_BUFFER_ALIGNED);
	#endif
	
	_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_SRC_BUFSIZE,PICTURE_BUFFER_SIZE);
    if(u4FbmId>FBM_POOL_TYPE_TOTAL)
    {   
        u4FbmId=ALIGN2048(u4FbmId);
	    _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,u4FbmId);
    }
	else
	{
		prFbmPool = FBM_GetPoolInfo((UINT8)u4FbmId);
	    if(prFbmPool==NULL || prFbmPool->u4Addr==NULL || prFbmPool->u4Size==0)
	    {
	       LOG(0,"Get Y Target Buffer fail\n");
		   return 0;
	    }
		
	    _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,ALIGN2048(prFbmPool->u4Addr));
	}

    if(u4FbmId2>FBM_POOL_TYPE_TOTAL)
    {
        u4FbmId2=ALIGN2048(u4FbmId2);
		_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,u4FbmId2);
    }
	else
	{
		prFbmPool=NULL;
		prFbmPool=FBM_GetPoolInfo((UINT8)u4FbmId2);
		
		if(prFbmPool==NULL || prFbmPool->u4Addr==NULL || prFbmPool->u4Size==0)
		{
			LOG(0,"Get C Target Buffer fail\n");
			return 0;
		}
		_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,ALIGN2048(prFbmPool->u4Addr));
	}

    _JPG_SetYTgW(1024);
    _JPG_SetYTgH(768);

	#ifdef JDEC_HW_IO_MMU
	VLDJPG_MMU_init();
	#endif
	_CLI_CMD("jpg.jset 23 0 64 0 0");
    _CLI_CMD("jpg.d_l 0");
	Printf("SrcAddr=0x%x,TaYaddr=0x%x,TaCaddr=0x%x\n",_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_SRC_BUFFER,0),_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_TAY_BUFFER,0),_JpgVerifyCmdGetParam(JPEG_VERIFY_PARAM_TAC_BUFFER,0));
#endif 
	return (INT32)E_CLIJPG_OK;
}

#ifndef JPEG_AUTO_TEST
#if defined (CC_EMULATION) || defined (__MODEL_slt__)
static INT32 _JpgCmdDecode(INT32 i4Argc, const CHAR ** szArgv)
{
	// jpg.dump
	INT32 i4Ret = (INT32)E_CLIJPG_OK;
	INT32 i4TestMode;
	UINT8 *pu1JpegAddr;
	UINT32 u4JpegSize;

	if (i4Argc > 2)
	{ 
		if((szArgv == NULL) || (szArgv[1]==NULL) || (szArgv[2]==NULL) || (szArgv[3]==NULL))
		{
			return 0;
		}
		i4TestMode = (INT32)StrToInt(szArgv[1]);
		pu1JpegAddr    = (UINT8*)StrToInt(szArgv[2]);
		u4JpegSize    = StrToInt(szArgv[3]);

		if((i4TestMode < 0) || (i4TestMode > 4))
		{
			LOG(0, "\t test mode unsupport\n");
			LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
			LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
			return (INT32)E_CLIJPG_OK;
		}
		if((i4TestMode > 0) && (i4TestMode < 4) && (_prOsd == NULL))
		{
			LOG(0, "Not init yet\n");
			LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
			LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
			return (INT32)E_CLIJPG_OK;	
		}

		if (!((pu1JpegAddr == NULL) || (u4JpegSize == 0)))
		{
			i4Ret = _JPGDecodePicture(i4TestMode, -1, pu1JpegAddr, u4JpegSize);
			return i4Ret;
		}
	}
	UNUSED(szArgv);
	LOG(0, "usage:\n\t jpg.decode {test mode} {address} {size}\n");
	LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:VP\n");	
	
	return (INT32)E_CLIJPG_OK;
}
#endif

static INT32 _JpgCmdChangeSet        (INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4SetIndex = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}
	if (i4Argc > 1)
	{
		i4SetIndex = (INT32)StrToInt(szArgv[1]);
	}
	_i4TestSetIdx = i4SetIndex;
	UNUSED(szArgv);
	return 0;
}
#endif

#if defined (CC_EMULATION) || defined (__MODEL_slt__)
#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdT32Test(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4Index;
	INT32 i4Ret;
	INT32 i4SetIndex = 0, i4MaxSet = 0;
	INT32 i4PicIndex = 0;
	INT32 i4TestMode = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc > 2)		// decode distinct pattern
	{
		// szArgv[2] is pattern index       
		if ((*szArgv[2] <= '9') && (*szArgv[2] >= '0'))
		{
			i4TestMode = (INT32)StrToInt(szArgv[1]);                
			i4PicIndex = (INT32)StrToInt(szArgv[2]);
			i4SetIndex = _i4TestSetIdx;
			i4MaxSet = _i4TestSetIdx + 1;
		}
	}
	else if(i4Argc == 2)	// decode all cases
	{
		i4TestMode = (INT32)StrToInt(szArgv[1]);        
		i4PicIndex = 0;
		i4SetIndex = _i4TestSetIdx;      
		i4MaxSet = JPGCLI_GetMaxSet();
	}
	else
	{
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");
		return (INT32)E_CLIJPG_OK;
	}

	if((i4TestMode < 0) || (i4TestMode > 4))
	{
		LOG(0, "Test mode unsupport\n");
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");		
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");	
		return (INT32)E_CLIJPG_OK;
	}

	if((i4TestMode > 0) && (i4TestMode < 4) && (_prOsd == NULL))
	{
		LOG(0, "Not init yet\n");
		LOG(0, "usage:\n\t jpg.tt {test mode} {test pattern number}\n");		
		LOG(0, "\t {test mode}0:verification, 1:imglib(LINEAR_IN_LINEAR_OUT), 2:LINEAR_IN_SINGLE_OUT, 3:RING_IN_SINGLE_OUT, 4:Video Plane\n");	
		return (INT32)E_CLIJPG_OK;	
	}
	
	for( ;i4SetIndex < i4MaxSet; i4SetIndex++)
	{
		i4Index = i4PicIndex;
		JPGCLI_GetTestSet(i4SetIndex, &_arJpgTestSet, &_szInputDir);
    
		while( _arJpgTestSet[i4Index].szShortName != NULL)
		{
			printf("filename : %s\n", _arJpgTestSet[i4Index].szFileName);			
			printf("filesize : %d\n", _arJpgTestSet[i4Index].u4ByteSize);
			i4Ret = _JPGDecodePicture(i4TestMode, i4Index, NULL, 0);
			if (i4Ret != 0)
			{
				if((i4Ret == -27) || (i4Ret == 3))
				{
					LOG(0, "Format Unsupport\n");
					i4Index++;
					continue;
				}
				if(i4Ret == -(INT32)E_CLIJPG_ERR_MEM) // memory alloc error if file too large, should not happen
				{
					i4Index++;
					continue;
				}
				// verification FPGA needs retry sometimes
				LOG(0, "Error while decode picture %s : ret(%d)\n", _arJpgTestSet[i4Index].szFileName, i4Ret);
				LOG(0, "Retry ...\n");
				i4Ret = _JPGDecodePicture(i4TestMode, i4Index, NULL, 0);
				if (i4Ret != 0)
				{
					LOG(0, "Error while decode picture %s : ret(%d)\n", _arJpgTestSet[i4Index].szFileName, i4Ret);
					LOG(0, "Fatal error !!!\n");
					//ASSERT(i4Ret == 0);
				}
			}
			if(i4Argc > 2)  // test distinct pattern
			{
				break;
			}
			i4Index++;
		}
		i4Index = 0;
	}

	UNUSED(szArgv);
	return (INT32)E_CLIJPG_OK;
}
#endif

#ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
//this function is for 5395
void _JPGDramAccessBtye_start(void)
{

#ifdef CC_MT5395
    UINT32 u4Channel = 0;
    UINT32 u4AgendId;
    UINT32 u4GroupId;
    UINT32 u4Total;
    UINT32 u4Base;
    volatile HAL_DRAMC_T* _prDramcReg; 
    HAL_DRAMC_T rDramcReg;

    if (u4Channel == 0)
    {
        u4Base =  DRAM_BASE;   
    }
    else
    {
        u4Base = DRAM_CHB_BASE;
    }
    _prDramcReg = (HAL_DRAMC_T*) (u4Base + 0x180);

    u4Total = 0;

    u4AgendId = 11;
    u4GroupId = 1;

    IO_REG32(u4Base, 0x18C) =(BSP_GetDomainClock(CAL_SRC_DMPLL) >> 1) ;;

    rDramcReg = *_prDramcReg;
    rDramcReg.fgBmbp1 = 0;
    rDramcReg.fgBmbp2 = 0;
    rDramcReg.fgBmbp3 = 0;
    if (u4Total != 0)
    {
        // All agent, including group 1, 2, and 3
        rDramcReg.u4Bmgp1ag = 0x1F;
    }
    else
    if (u4GroupId == 1)
    {
        rDramcReg.u4Bmgp1ag = u4AgendId;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.u4Bmgp2ag = u4AgendId;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.u4Bmgp3ag = u4AgendId;
    }
    *_prDramcReg = rDramcReg;
    if (u4GroupId == 1)
    {
        rDramcReg.fgBmbp1 = 1;
    }
    else
    if (u4GroupId == 2)
    {
        rDramcReg.fgBmbp2 = 1;
    }
    else
    if (u4GroupId == 3)
    {
        rDramcReg.fgBmbp3 = 1;
    }

    rDramcReg.fgReqAle = 0;
    *_prDramcReg = rDramcReg;
#endif
}

UINT32 _JPGDramAccessBtye_end(UINT32 u4GroupId)
{
#ifdef CC_MT5395
    UINT32 u4Result;
    UINT32 u4Channel = 0;
    UINT32 u4Base;

    if (u4Channel == 0)
    {
        u4Base =  DRAM_BASE;   
    }
    else
    {
        u4Base = DRAM_CHB_BASE;
    }
    
    if (u4GroupId == 1)
    {
        u4Result = IO_REG32(u4Base, 0x190);
    }
    else if(u4GroupId == 2)
    {
        u4Result = IO_REG32(u4Base, 0x194);
    }
    else if (u4GroupId == 3)
    {
        u4Result = IO_REG32(u4Base, 0x198);
    }
    else
    {
        u4Result = 0;
    }

    return u4Result*16;
    //Printf("Total BW bytes %d\n", u4Result*16);  
#endif
}
#endif

static INT32 _JPGDecodePicture(INT32 i4TestMode, INT32 i4Index, UINT8 *pu1JpegAddr, UINT32 u4JpegSize)
{
	//_arJpgTestSet
	INT32 i4Ret = 0;
	JMEM_YUV_LINEAR_T    *prYuvLinear = NULL;
	HAL_RAW_TIME_T        rRawTime1;
	HAL_RAW_TIME_T        rRawTime2;
	HAL_TIME_T            rHalTime1;
	HAL_TIME_T            rHalTime2;
	HAL_TIME_T            rHalTimeDuration;
	UINT8                *pu1JpegStream = NULL;
	IMG_PARAM_T rInfo;
	IMG_REGION_T rDecOut;
	IMG_BUF_PARAM_T rCmd;	
	UINT32 u4ImageSize;
	IMG_VP_INFO_T rVpInfo;
	static ENUM_IMG_ROTATE_PHASE eRotatePhase = CLOCKWISE_0;
	static BOOL bKeepRatio = FALSE;
	JPEGHANDLE hDec = (JPEGHANDLE)0;

#ifdef JPG_SEMI_HOST_BY_USB
   UINT32 i=0;	
   CHAR pcIjpgFileName[256];
   CHAR pcIjpgFileName_2[256];
#endif
      
	//---------- prepare memory buffer for input image and data structure
	if(i4Index >= 0)		//use test set pattern
	{
		pu1JpegStream =(UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr);//x_mem_alloc(_arJpgTestSet[i4Index].u4ByteSize + 128);
		u4ImageSize = _arJpgTestSet[i4Index].u4ByteSize;
		//pu1JpegStream = NONCACHE((INT32)pu1JpegStream);
		if (pu1JpegStream == NULL ||u4ImageSize>10*1024*1024 )
		{
			LOG(0, "Error while load test data : %s\n", _arJpgTestSet[i4Index].szFileName);
			LOG(0, "Can not allocate memory of size %d bytes\n", _arJpgTestSet[i4Index].u4ByteSize);
			return -(INT32)E_CLIJPG_ERR_MEM;
		}
		
		// sync ram and cache
                //HalFlushInvalidateDCache();

		// load test case
		#ifdef JPG_SEMI_HOST
			#ifdef JPG_SEMI_HOST_BY_USB
				x_strcpy(pcIjpgFileName, _szInputDir);

		                for (i = 3; pcIjpgFileName[i] != '\0'; i++)
		                {
		                    if (pcIjpgFileName[i] == '\\')
		                    {
		                        pcIjpgFileName[i] = '/';
		                    }
		                }			
				x_sprintf(pcIjpgFileName_2, "/mnt/usb_0/%s/", &pcIjpgFileName[3]);		
				x_strcat(pcIjpgFileName_2, (CHAR *)_arJpgTestSet[i4Index].szFileName);
				_JPGLoadUsbFile(pcIjpgFileName_2, (UINT32)pu1JpegStream);
			#else
				UNUSED(T32_HostExec(E_T32_EXEC, "d.load.binary \"%s\\%s\" 0x%08x /ny", _szInputDir, _arJpgTestSet[i4Index].szFileName, pu1JpegStream));
			            #ifndef JPEG_AUTO_TEST_PERFORMANCE
			            printf("d.load.binary \"%s\\%s\" 0x%08x /ny\n", _szInputDir, _arJpgTestSet[i4Index].szFileName, (UINT32)pu1JpegStream);
			            #endif			
			#endif
		#endif



		// sync ram and cache
            #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5890)
            HalFlushInvalidateDCacheMultipleLine((UINT32)(pu1JpegStream), (UINT32)(_arJpgTestSet[i4Index].u4ByteSize));
            #else
            HalFlushInvalidateDCache();
            #endif        
	}
	else				
	{
		pu1JpegStream = (UINT8 *)(_JpegVerParam.i4JpegSrcBufAddr);//pu1JpegAddr;
		u4ImageSize = u4JpegSize;                
#ifdef __MODEL_slt__
#ifndef __KERNEL__
		HANDLE_T hImgFile;
		UINT32 u4Read;
#else
		struct file *filep = NULL;
		UINT64 u8Read = 0;
        
#endif
		pu1JpegStream = VIRTUAL(BSP_AllocAlignedDmaMemory(u4ImageSize + 128, 64));
                
		if (pu1JpegStream == NULL)
		{
			LOG(0, "Error while load test data : %s\n", _arJpgTestSet[i4Index].szFileName);
			LOG(0, "Can not allocate memory of size %d bytes\n", u4ImageSize);
			return -(INT32)E_CLIJPG_ERR_MEM;
		}
#ifndef __KERNEL__
		if (FMR_OK != x_fm_open(FM_ROOT_HANDLE, _acJpgFileName, FM_READ_ONLY, 0777, FALSE, &hImgFile))
        {
            LOG(0, "Open file fail!\n");
            return -1;    
        }
	    if(FMR_OK != x_fm_read(hImgFile, pu1JpegStream, u4ImageSize, &u4Read))
        {
            LOG(0, "Read file fail!\n");
            return -1;        
        }
		x_fm_close(hImgFile);
#else
		filep = FILE_Open(_acJpgFileName, O_RDONLY , 0);
		if(!filep)
		{
			LOG(0, "Open file %s fail!\n", _acJpgFileName);
	        return -1;
		}
		if (u4ImageSize != FILE_Read(filep, pu1JpegStream, u4ImageSize, &u8Read))
                {
                    LOG(0, "Read file fail!\n");
                    return -1;        
                }
		FILE_Close(filep);
#endif

#if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)||defined(CC_MT5890)
                HalFlushDCacheMultipleLine((UINT32)(pu1JpegStream), (u4ImageSize + 128));
#else
                HalFlushInvalidateDCache();
#endif        
#endif
	}

	HAL_GetRawTime(&rRawTime1);
	
	//----------start decoding

        #ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
        _JPGDramAccessBtye_start();
        #endif
	
	if(i4TestMode == 0)	//verification
	{
		hDec = JDEC_AllocInstance();
		if (hDec == (JPEGHANDLE)NULL)
		{
			LOG(0, "Error while allocate jpeg decoder handle.\n");
			if(i4Index > 0 && pu1JpegStream != SRC_BUFFER_ALIGNED)
			{
				x_mem_free(pu1JpegStream);
			}
			return -(INT32)E_CLIJPG_ERR_GENERAL;
		}

		if (JDEC_New(hDec))
		{
			if(i4Index > 0 && pu1JpegStream!= SRC_BUFFER_ALIGNED)
			{
				x_mem_free(pu1JpegStream);
			}
			JDEC_FreeInstance(hDec);
			LOG(0, "Error while new jpeg decoder instance.\n");
			return -(INT32)E_CLIJPG_ERR_GENERAL;
		}
		
		TRY(JDEC_DecodeYUV(hDec, pu1JpegStream, u4ImageSize, &prYuvLinear));	
#ifdef JPEG_AUTO_TEST
        _JpegVerParam.i4PicFormat=prYuvLinear->u4ColorFMT;
        if(_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_BASELINE_MCU||_JpegVerParam.i4JpegDecMode==PIC_DEC_MODE_MCU_ROW || _JpegVerParam.i4DumpColorBuffer)
        {
		   INT32 arCheckFileParam[10];
		   arCheckFileParam[0]=3;
		   
		   arCheckFileParam[1]=5;
		   arCheckFileParam[2]=(INT32)(prYuvLinear->pvComp[0]);
		   arCheckFileParam[3]=prYuvLinear->au4AllocSize[0];

		   arCheckFileParam[4]=6;
		   arCheckFileParam[5]=(INT32)(prYuvLinear->pvComp[1]);
		   arCheckFileParam[6]=prYuvLinear->au4AllocSize[1];

		   arCheckFileParam[7]=7;
		   arCheckFileParam[8]=(INT32)(prYuvLinear->pvComp[2]);
		   arCheckFileParam[9]=prYuvLinear->au4AllocSize[2];
		   _JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_CHECKFILE,(INT32)arCheckFileParam);
         }
#endif
	}
	else if(i4TestMode == 4)	//video plane test, use RING_IN_DOUBLE_OUT mode
	{
		TRY((INT32)IMG_GetVideoPlaneInfo(&rVpInfo));
		IMG_REGION_T rRegion;
		rInfo.u8BufSize = u4ImageSize;
		rInfo.rDstRegion.u4Width = rVpInfo.u4Width;
		rInfo.rDstRegion.u4Height = rVpInfo.u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;		
		_eDispMode = rVpInfo.eDispColorMode;
		rInfo.rDstRegion.u4Pitch = (rVpInfo.u4Width<<_au4BytePerPixel[_eDispMode]) & (~(rVpInfo.u4PitchAlign - 1));
		rInfo.eDispColorMode = _eDispMode;
		rInfo.eRotateDegree = eRotatePhase; //CLOCKWISE_0
		rInfo.bKeepRatio = bKeepRatio;
		rInfo.prDecOut = &rDecOut;

		_pu1Bs = pu1JpegStream;
		_u4fileSize = u4ImageSize;
		_u4filled_size = 0;		
		rInfo.pu1InBuffer = NULL; //use internal ring buffer
		rInfo.rDstRegion.pu1Addr = rVpInfo.pu1BackFrameAddr[0];
		rInfo.rDstRegion.u4CbCrOffset = (UINT32)rVpInfo.pu1BackFrameAddr[1] - (UINT32)rVpInfo.pu1BackFrameAddr[0];
			
		TRY((INT32)IMG_Init(RING_IN_DOUBLE_OUT, NULL, JPG_VERIFY_IMG_ID, _JPGFillBufCB));

		TRY((INT32)IMG_Decode(JPG_VERIFY_IMG_ID, rInfo));
		VERIFY (x_sema_lock(_hJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);
		if(_u4NtyValue == JPG_HW_NFY_ERROR)
		{
			_u4NtyValue = JPG_HW_NFY_FINISHED;
			goto _catch_err;	
		}
		TRY((INT32)IMG_Flip());

		//set operation
		rCmd.eDispColorMode = _eDispMode;
		rCmd.eRotateDegree = eRotatePhase;
		rCmd.rSrcRegion = rDecOut;
		rInfo.rDstRegion.pu1Addr = rVpInfo.pu1CurFrameAddr[0];
		rInfo.rDstRegion.u4Depth = 1 << _au4BytePerPixel[_eDispMode];
		rInfo.rDstRegion.u4TotalLine = rVpInfo.u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;
		rCmd.rDstRegion = rInfo.rDstRegion;
		rCmd.bKeepRatio = bKeepRatio;
		IMG_Set(rCmd,&rRegion);	
		TRY((INT32)IMG_Flip());
		LOG(0, "phase %d\n", (UINT32)eRotatePhase);
		eRotatePhase  = (ENUM_IMG_ROTATE_PHASE)(((UINT32)eRotatePhase + 1) % (UINT32)PHASE_MAX);
		//bKeepRatio = ~bKeepRatio;
	}
	else						//image library API
	{
		rInfo.u8BufSize = u4ImageSize;
		rInfo.rDstRegion.u4Width = _prOsd->u4Width;
		rInfo.rDstRegion.u4Height = _prOsd->u4Height;
		rInfo.rDstRegion.u4StartX = 0;
		rInfo.rDstRegion.u4StartY = 0;		
		rInfo.rDstRegion.u4Pitch = _prOsd->u4Width<<_au4BytePerPixel[_eDispMode];
		rInfo.rDstRegion.u4CbCrOffset = rInfo.rDstRegion.u4Pitch * _prOsd->u4Height;
		rInfo.eDispColorMode = _eDispMode;
		rInfo.eRotateDegree = eRotatePhase;
		if(_eDispMode == AYCBCR8888_DIRECT32)
		{
			eRotatePhase  = (ENUM_IMG_ROTATE_PHASE)(((UINT32)eRotatePhase + 1) % (UINT32)PHASE_MAX);
			rInfo.rDstRegion.u4CbCrOffset = 0;
		}
		rInfo.bKeepRatio = bKeepRatio;

		rInfo.prDecOut = &rDecOut;
		
		if(i4TestMode == 1)
		{
			rInfo.pu1InBuffer = pu1JpegStream;
			rInfo.rDstRegion.pu1Addr = NULL;
			TRY((INT32)IMG_Init(LINEAR_IN_LINEAR_OUT, NULL, JPG_VERIFY_IMG_ID, NULL));
			TRY(IMG_GetOutputBuffer(JPG_VERIFY_IMG_ID, (void **)&prYuvLinear));
		}
		else if(i4TestMode == 2)
		{
			rInfo.pu1InBuffer = pu1JpegStream;
			rInfo.rDstRegion.pu1Addr = (UINT8 *)0x3000000;//_prOsd->pu1FrameBaseAddr;
			TRY((INT32)IMG_Init(LINEAR_IN_SINGLE_OUT, NULL, JPG_VERIFY_IMG_ID, NULL));
		}
		else
		{
			_pu1Bs = pu1JpegStream;
			_u4fileSize = u4ImageSize;
			_u4filled_size = 0;		
			rInfo.pu1InBuffer = NULL; //use internal ring buffer
			rInfo.rDstRegion.pu1Addr = (UINT8 *)0x3000000;//_prOsd->pu1FrameBaseAddr;
			TRY((INT32)IMG_Init(RING_IN_SINGLE_OUT, NULL, JPG_VERIFY_IMG_ID, _JPGFillBufCB));
		}

		TRY((INT32)IMG_Decode(JPG_VERIFY_IMG_ID, rInfo));
		VERIFY (x_sema_lock(_hJPGDecSema, X_SEMA_OPTION_WAIT) == OSR_OK);		
		bKeepRatio = ~bKeepRatio;
    }
      
      //-----------finish decoding

      #ifdef JPEG_DRAM_ACCESS_BYTE_MEASUREMENT
      Printf("JPG Total BW bytes %d\n", _JPGDramAccessBtye_end(1));  
      #endif
      
	HAL_GetRawTime(&rRawTime2);
	HAL_RawToTime(&rRawTime1, &rHalTime1);
	HAL_RawToTime(&rRawTime2, &rHalTime2);
	HAL_GetDeltaTime(&rHalTimeDuration, &rHalTime1, &rHalTime2);	

        #ifdef JPEG_AUTO_TEST_PERFORMANCE
        printf("%d \t %u.%06u \t %8d \t %6d \t %6d \t %s\n",_InnerComparePicIndex, rHalTimeDuration.u4Seconds, rHalTimeDuration.u4Micros, u4ImageSize, mh_width, mh_height , _JpegVerParam.i4PicFormat==0 ? "444" : (_JpegVerParam.i4PicFormat==1 ? "422V" : (_JpegVerParam.i4PicFormat==2 ? "422":"420")));
        #else
        printf("Decoded successully.\n");
        printf("Decode duration : %u.%06u seconds\n",
        	rHalTimeDuration.u4Seconds,
        	rHalTimeDuration.u4Micros);	
        #endif 
        
	if((i4TestMode < 2) && (prYuvLinear)) //verification or image library LINEAR_IN_LINEAR_OUT
	{
                // sync ram and cache
                #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalInvalidateDCacheMultipleLine((UINT32)(prYuvLinear->pvComp[0]), (UINT32)(prYuvLinear->au4AllocSize[0]+prYuvLinear->au4AllocSize[1]+prYuvLinear->au4AllocSize[2]));
                #else
                HalFlushInvalidateDCache();
                #endif 
		// save YUV outputs or checksum
		if (_fgJpegWriteEnable)
		{
			LOG(3, "Output data to output folder...\n");
			#ifdef JPG_SEMI_HOST
			UNUSED(T32_HostExec(E_T32_EXEC, "d.save.binary \"%s/%s.yuv\" 0x%08x--0x%08x",
				_szOutputDir,
				_arJpgTestSet[i4Index].szFileName,
				(void*)((UINT32)prYuvLinear->pvComp[0]),
				(void*)(((UINT32)prYuvLinear->pvComp[0] +
					prYuvLinear->au4AllocSize[0] +
					prYuvLinear->au4AllocSize[1] +
					prYuvLinear->au4AllocSize[2]) - 1)));
			#endif
		}
		else
		{
			UINT32 u4ChkSum32=0;
#ifndef JPEG_AUTO_TEST
            u4ChkSum32 = _CalcCheckSum(prYuvLinear->pvComp[0],
			prYuvLinear->au4AllocSize[0] +
			prYuvLinear->au4AllocSize[1] +
			prYuvLinear->au4AllocSize[2]);
            printf("Checksum : 0x%08x\n", u4ChkSum32);
#else
            #ifdef JPEG_AUTO_TEST_PERFORMANCE
            if(0)
            #else
            if(_JpegVerParam.i4JpegDecMode!=PIC_DEC_MODE_PICTURE_IMGRZ)
            #endif
            {
				u4ChkSum32 = _CalcCheckSum(prYuvLinear->pvComp[0],
				prYuvLinear->au4AllocSize[0] +
				prYuvLinear->au4AllocSize[1] +
				prYuvLinear->au4AllocSize[2]);
				Printf("Checksum : 0x%08x !!!\n", u4ChkSum32);


				if(_CheckSumCompmareInner)
				{
				   if(u4ChkSum32==pAutoRunCheckSum[_InnerComparePicIndex])
					{
						Printf("%d:CheckSum Pass\n",_InnerComparePicIndex);
					}
				   else
					{
						Printf("%d,CheckSum Fail\n",_InnerComparePicIndex);
					}
				}
				else
				{
					INT32 arCheckSum[2]={0};
					arCheckSum[0]=1;
					arCheckSum[1]=u4ChkSum32;
					_JpgVerifyCmdSetParam(JPEG_VERIFY_PARAM_CHECKSUM,(INT32)arCheckSum);
				}
            }
#endif
UNUSED(u4ChkSum32);

#ifdef __MODEL_slt__
            _u4JpgSlt = u4ChkSum32;
#endif
		}
	}

        #ifndef JPEG_AUTO_TEST_PERFORMANCE
        printf("done.\n");
        #endif
       
	_catch_err:
	IMG_Release(JPG_VERIFY_IMG_ID);	
	if(i4Index >= 0 && pu1JpegStream && pu1JpegStream !=SRC_BUFFER_ALIGNED)
	{
		x_mem_free(pu1JpegStream);
	}
	if(i4TestMode == 0)
	{
		JDEC_FreeInstance(hDec);
	}
    return i4Ret;
}
#endif

#ifndef JPEG_AUTO_TEST
static INT32 _JpgCmdSetDecMode(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4DecMode = 0;
    
	if(szArgv == NULL)
	{
		return 0;
	}

	if (i4Argc < 2)
	{
		Printf("arg: decmode (0:mcu, 1:row, 2:row_rz, 3:pic_rz) \n");
	}
	
	i4DecMode = (INT32)StrToInt(szArgv[1]);

	if (i4DecMode == 0)
	{
	    Printf("Set JPG decode mode = MCU\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU);
	}
	else if (i4DecMode == 1)
	{
	    Printf("Set JPG decode mode = MCU_ROW\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW);
	}
	else if (i4DecMode == 2)
	{
	    Printf("Set JPG decode mode = MCU_ROW + Resizer\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_MCU_ROW_RZ);
	}
	else if (i4DecMode == 3)
	{
	    Printf("Set JPG decode mode = PIC + Resizer\n");
	    JPG_SetDecMode5391((INT32)E_JPG_DEC_MODE_BASELINE_PIC_RZ);
	}
	
	UNUSED(szArgv);
	return 0;
}

#endif
void _JPGFillBufCB(
UINT32 u4ImgID,
void *pv_tag,
void *pv_data,
UINT32 u4State)
{
	IMG_LIB_FILL_RING_BUF_T *prBufInfo = (IMG_LIB_FILL_RING_BUF_T *)pv_data;
	IMG_LIB_ENTRY_BUF_T *prEntry1;
	IMG_LIB_ENTRY_BUF_T *prEntry2;
	BOOL bEntry1_valid, bEntry2_valid, isEOF=FALSE; 
	UINT32 u4Addr1, u4Addr2;
	UINT32 u4Required_len1, u4Required_len2;
	UINT32 u4curFilledSize1=0, u4curFilledSize2=0;

	if(u4State == JPG_HW_NFY_PROGRESS)
	{
		return;	//ignore the progress notify
	}
	else if((u4State == JPG_HW_NFY_FINISHED) ||(u4State == JPG_HW_NFY_ERROR))
	{
		_u4NtyValue = u4State;
		VERIFY (x_sema_unlock(_hJPGDecSema) == OSR_OK);
		return;
	}
	if(prBufInfo==NULL)
	{
		return;
	}
	prEntry1 = (IMG_LIB_ENTRY_BUF_T *)&prBufInfo->rEntry1;
	prEntry2 = (IMG_LIB_ENTRY_BUF_T *)&prBufInfo->rEntry2;
	
	bEntry1_valid = prBufInfo->rEntry1.bValid;
	u4Addr1 = (UINT32)prBufInfo->rEntry1.pvStartAddr;
	u4Required_len1 = prBufInfo->rEntry1.u4RequiredLen;
	bEntry2_valid = prBufInfo->rEntry2.bValid;
	u4Addr2 = (UINT32)prBufInfo->rEntry2.pvStartAddr;
	u4Required_len2 = prBufInfo->rEntry2.u4RequiredLen;
	
	if(bEntry1_valid>0)
	{
		if((_u4filled_size + u4Required_len1) > _u4fileSize)
		{
			u4curFilledSize1 = _u4fileSize - _u4filled_size;
			isEOF = TRUE;
		}
		else
		{
			u4curFilledSize1 = u4Required_len1;
		}
		
		x_memcpy((void *)u4Addr1, (void *)_pu1Bs, u4curFilledSize1);

				  #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(u4Addr1), (UINT32)(u4curFilledSize1));
                #else
                HalFlushInvalidateDCache();
                #endif 
                
		_u4filled_size += u4curFilledSize1;
		_pu1Bs += u4curFilledSize1;
	}
	if(bEntry2_valid>0)
	{
		if((_u4filled_size + u4Required_len2) > _u4fileSize)
		{
			u4curFilledSize2 = _u4fileSize - _u4filled_size;
			isEOF = TRUE;
		}
		else
		{
			u4curFilledSize2 = u4Required_len2;
		}
		
		x_memcpy((void *)u4Addr2, (void *)_pu1Bs, u4curFilledSize2);
		
                #if defined(CC_MT5365) || defined(CC_MT5395) ||defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
                HalFlushInvalidateDCacheMultipleLine((UINT32)(u4Addr2), (UINT32)(u4curFilledSize2));
                #else
                HalFlushInvalidateDCache();
                #endif
                
		_u4filled_size += u4curFilledSize2;
		_pu1Bs += u4curFilledSize2;
	}
	prEntry1->u4FillLen = u4curFilledSize1;
	prEntry2->u4FillLen = u4curFilledSize2;
	prBufInfo->bEOF = isEOF;

	// clean lint warning
	UNUSED(u4ImgID);
	if(pv_tag == NULL)
	{
		return;
	}
	else
	{
		*((UINT8 *)pv_tag) = 0;
	}
	//IMG_SetFillBsBufEnd(u4curFilledSize1, u4curFilledSize2, isEOF);
}

static UINT32 _CalcCheckSum(void* pvData, UINT32 u4ByteLen)
{
	UINT32* pu4Data;
	UINT32 u4Sum = 0;

	ASSERT(pvData!=NULL);
	pu4Data = (UINT32*)pvData;
	while (u4ByteLen > 3)
	{
		u4Sum += *pu4Data;
		if (u4ByteLen >= 4)
		{
			u4ByteLen -= 4;
		}
		else
		{
			u4ByteLen = 0;
		}
		pu4Data++;
	}

	return u4Sum;
}

#ifdef __MODEL_slt__
INT32 _JpgCmdSlt(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4SltAns;
    UINT32 u4JpegSize;

    if (i4Argc < 4)
    {
        Printf("jpg.slt <filename> <filesize> <checksum>\n");
#if defined(__KERNEL__)
		x_sprintf(_acJpgFileName, "/basic/slt_test_pattern/Baseline.jpg");
#else
        x_sprintf(_acJpgFileName, "/mnt/usb_0/Baseline.jpg");
#endif
        u4JpegSize = (UINT32)37586;
        //u4SltAns = (UINT32)0x9aea43a7;
        u4SltAns = (UINT32)0xc603ee98;        
    }
    else
    {
#if defined(__KERNEL__)
		x_sprintf(_acJpgFileName, "/basic/slt_test_pattern/%s", szArgv[1]);
#else
		x_sprintf(_acJpgFileName, "/mnt/usb_0/%s", szArgv[1]);
#endif
        u4JpegSize = (UINT32)StrToInt(szArgv[2]);
        u4SltAns = (UINT32)StrToInt(szArgv[3]);
    }
        
    _u4JpgSlt = 0;
    _CLI_CMD("fbm.i");
    _CLI_CMD("jpg.i 0");
    //_CLI_CMD("jpg.decmod 1");
    if (_JPGDecodePicture(0, -1, NULL, u4JpegSize))
    {
        Printf("jpeg decode error\n");
		Printf("JPGFAIL\n");
        return -1;
    }

    if (_u4JpgSlt != u4SltAns)
    {
        Printf("[JPG] Fail Count = %d\n", (INT32)_u4JpgSlt);
        return -1;
    }
    Printf("[JPG] %x , SLT JPGOK!\n", (INT32)u4SltAns);
    return 0;
}
#endif

#endif
#ifdef ENABLE_MULTIMEDIA
extern UINT32 u4hPNGSemaphoreDBGCnt;
extern UINT32 u4hPNGSemaphoreDBG[256][2];
#endif

extern UINT32 u4hInitSema;
extern UINT32 u4hReleaseSema;
extern UINT32 u4hFillBufSema;
extern UINT32 u4hSemaphoreDBG[256][2];
extern UINT32 u4hSemaphoreDBGCnt;

#ifdef LINUX_TURNKEY_SOLUTION
extern UINT32 _mtimageDebugStep;
extern UINT32 u4MtimageDBGCnt;
extern UINT32 u4MtimageDBG[256][2];
#endif

static INT32 _JpgCmdDebugStep(INT32 i4Argc, const CHAR ** szArgv)
{
	INT32 i4TestMode;	
	IMG_INFO_KEEP_T *prImgKeepInfo_0;
	IMG_INFO_KEEP_T *prImgKeepInfo_1;

	#ifdef LINUX_TURNKEY_SOLUTION
	INT32 i=0;
	#endif
	
	prImgKeepInfo_0 = _IMG_GetInfoKeep(0);
	prImgKeepInfo_1 = _IMG_GetInfoKeep(1);
	
	i4TestMode = (INT32)StrToInt(szArgv[1]);
	
	if(i4TestMode==0)
    {        
        Printf("\n_JpgCmdDebugStep testMode=[%d], state=[%d]\n", i4TestMode, prImgKeepInfo_0->u4CurState);
    }
    else if(i4TestMode==1)	    
    {
        Printf("\n_JpgCmdDebugStep testMode=[%d], state=[%d]\n", i4TestMode, prImgKeepInfo_1->u4CurState);
    }        
    #ifdef LINUX_TURNKEY_SOLUTION
    else if(i4TestMode==2)
    {
        Printf("\n_JpgCmdDebugStep _mtimageDebugStep=[%d]\n", _mtimageDebugStep);
    }    
    else if(i4TestMode==3)
    {

        Printf("\n_JpgCmdDebugStep u4hInitSema=[%d]\n", u4hInitSema);
        Printf("_JpgCmdDebugStep u4hReleaseSema=[%d]\n", u4hReleaseSema);
        Printf("_JpgCmdDebugStep u4hFillBufSema=[%d]\n", u4hFillBufSema);
        Printf("_JpgCmdDebugStep u4hSemaphoreDBGCnt=[%d]\n", u4hSemaphoreDBGCnt);
        
        for(i=0;i<256;i++)
        {
						if(i%8==0)
        			Printf("\n[@@%d]", i);
        			
        		Printf("[%d][%d]", u4hSemaphoreDBG[i][0],u4hSemaphoreDBG[i][1]);
       	}
    }  	
    else if(i4TestMode==4)
    {
        Printf("\n_mtimageDebugStep u4MtimageDBGCnt=[%d]\n", u4MtimageDBGCnt);
        
        for(i=0;i<256;i++)
        {
						if(i%8==0)
        			Printf("\n[@@%d]", i);
        			
        		Printf("[%d][%d]", u4MtimageDBG[i][0],u4MtimageDBG[i][1]);
       	}
    }  
#ifdef ENABLE_MULTIMEDIA	
    else if(i4TestMode==5)
    {
        Printf("\n u4hPNGSemaphoreDBG u4hPNGSemaphoreDBGCnt=[%d]\n", u4hPNGSemaphoreDBGCnt);
        
        for(i=0;i<256;i++)
        {
						if(i%8==0)
        			Printf("\n[@@%d]", i);
        			
        		Printf("[%d][%d]", u4hPNGSemaphoreDBG[i][0],u4hPNGSemaphoreDBG[i][1]);
       	}
    }     
#endif	

#endif
	return 0;
}
#endif
