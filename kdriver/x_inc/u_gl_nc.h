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
/*----------------------------------------------------------------------------*/
/*! @file u_gl_nc.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2015/01/09 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description: This file defines the named color definition.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_GL_NC_H_

#define GL_COLOR_DEF(a, r, g, b)    {(a), {(r)}, {(g)}, {(b)}}          /**< Color definition macro */

/* Named Color Definitions */

#define GL_NC_ALICEBLUE             GL_COLOR_DEF(255, 240, 248, 255)    /**< ALICEBLUE            */
#define GL_NC_ANTIQUEWHITE          GL_COLOR_DEF(255, 250, 235, 215)    /**< ANTIQUEWHITE         */
#define GL_NC_AQUA                  GL_COLOR_DEF(255,   0, 255, 255)    /**< AQUA                 */
#define GL_NC_AQUAMARINE            GL_COLOR_DEF(255, 127, 255, 212)    /**< AQUAMARINE           */
#define GL_NC_AZURE                 GL_COLOR_DEF(255, 240, 255, 255)    /**< AZURE                */
#define GL_NC_BEIGE                 GL_COLOR_DEF(255, 245, 245, 220)    /**< BEIGE                */
#define GL_NC_BISQUE                GL_COLOR_DEF(255, 255, 228, 196)    /**< BISQUE               */
#define GL_NC_BLACK                 GL_COLOR_DEF(255,   0,   0,   0)    /**< BLACK                */
#define GL_NC_BLANCHEDALMOND        GL_COLOR_DEF(255, 255, 235, 205)    /**< BLANCHEDALMOND       */
#define GL_NC_BLUE                  GL_COLOR_DEF(255,   0,   0, 255)    /**< BLUE                 */
#define GL_NC_BLUEVIOLET            GL_COLOR_DEF(255, 138,  43, 226)    /**< BLUEVIOLET           */
#define GL_NC_BROWN                 GL_COLOR_DEF(255, 165,  42,  42)    /**< BROWN                */
#define GL_NC_BURLYWOOD             GL_COLOR_DEF(255, 222, 184, 135)    /**< BURLYWOOD            */
#define GL_NC_CADETBLUE             GL_COLOR_DEF(255,  95, 158, 160)    /**< CADETBLUE            */
#define GL_NC_CHARTREUSE            GL_COLOR_DEF(255, 127, 255,   0)    /**< CHARTREUSE           */
#define GL_NC_CHOCOLATE             GL_COLOR_DEF(255, 210, 105,  30)    /**< CHOCOLATE            */
#define GL_NC_CORAL                 GL_COLOR_DEF(255, 255, 127,  80)    /**< CORAL                */
#define GL_NC_CORNFLOWERBLUE        GL_COLOR_DEF(255, 100, 149, 237)    /**< CORNFLOWERBLUE       */
#define GL_NC_CORNSILK              GL_COLOR_DEF(255, 255, 248, 220)    /**< CORNSILK             */
#define GL_NC_CRIMSON               GL_COLOR_DEF(255, 220,  20,  60)    /**< CRIMSON              */
#define GL_NC_CYAN                  GL_COLOR_DEF(255,   0, 255, 255)    /**< CYAN                 */
#define GL_NC_DARKBLUE              GL_COLOR_DEF(255,   0,   0, 139)    /**< DARKBLUE             */
#define GL_NC_DARKCYAN              GL_COLOR_DEF(255,   0, 139, 139)    /**< DARKCYAN             */
#define GL_NC_DARKGOLDENROD         GL_COLOR_DEF(255, 184, 134,  11)    /**< DARKGOLDENROD        */
#define GL_NC_DARKGRAY              GL_COLOR_DEF(255, 169, 169, 169)    /**< DARKGRAY             */
#define GL_NC_DARKGREEN             GL_COLOR_DEF(255,   0, 100,   0)    /**< DARKGREEN            */
#define GL_NC_DARKGREY              GL_COLOR_DEF(255, 169, 169, 169)    /**< DARKGREY             */
#define GL_NC_DARKKHAKI             GL_COLOR_DEF(255, 189, 183, 107)    /**< DARKKHAKI            */
#define GL_NC_DARKMAGENTA           GL_COLOR_DEF(255, 139,   0, 139)    /**< DARKMAGENTA          */
#define GL_NC_DARKOLIVEGREEN        GL_COLOR_DEF(255,  85, 107,  47)    /**< DARKOLIVEGREEN       */
#define GL_NC_DARKORANGE            GL_COLOR_DEF(255, 255, 140,   0)    /**< DARKORANGE           */
#define GL_NC_DARKORCHID            GL_COLOR_DEF(255, 153,  50, 204)    /**< DARKORCHID           */
#define GL_NC_DARKRED               GL_COLOR_DEF(255, 139,   0,   0)    /**< DARKRED              */
#define GL_NC_DARKSALMON            GL_COLOR_DEF(255, 233, 150, 122)    /**< DARKSALMON           */
#define GL_NC_DARKSEAGREEN          GL_COLOR_DEF(255, 143, 188, 143)    /**< DARKSEAGREEN         */
#define GL_NC_DARKSLATEBLUE         GL_COLOR_DEF(255,  72,  61, 139)    /**< DARKSLATEBLUE        */
#define GL_NC_DARKSLATEGRAY         GL_COLOR_DEF(255,  47,  79,  79)    /**< DARKSLATEGRAY        */
#define GL_NC_DARKSLATEGREY         GL_COLOR_DEF(255,  47,  79,  79)    /**< DARKSLATEGREY        */
#define GL_NC_DARKTURQUOISE         GL_COLOR_DEF(255,   0, 206, 209)    /**< DARKTURQUOISE        */
#define GL_NC_DARKVIOLET            GL_COLOR_DEF(255, 148,   0, 211)    /**< DARKVIOLET           */
#define GL_NC_DEEPPINK              GL_COLOR_DEF(255, 255,  20, 147)    /**< DEEPPINK             */
#define GL_NC_DEEPSKYBLUE           GL_COLOR_DEF(255,   0, 191, 255)    /**< DEEPSKYBLUE          */
#define GL_NC_DIMGRAY               GL_COLOR_DEF(255, 105, 105, 105)    /**< DIMGRAY              */
#define GL_NC_DIMGREY               GL_COLOR_DEF(255, 105, 105, 105)    /**< DIMGREY              */
#define GL_NC_DODGERBLUE            GL_COLOR_DEF(255,  30, 144, 255)    /**< DODGERBLUE           */
#define GL_NC_FIREBRICK             GL_COLOR_DEF(255, 178,  34,  34)    /**< FIREBRICK            */
#define GL_NC_FLORALWHITE           GL_COLOR_DEF(255, 255, 250, 240)    /**< FLORALWHITE          */
#define GL_NC_FORESTGREEN           GL_COLOR_DEF(255,  34, 139,  34)    /**< FORESTGREEN          */
#define GL_NC_FUCHSIA               GL_COLOR_DEF(255, 255,   0, 255)    /**< FUCHSIA              */
#define GL_NC_GAINSBORO             GL_COLOR_DEF(255, 220, 220, 220)    /**< GAINSBORO            */
#define GL_NC_GHOSTWHITE            GL_COLOR_DEF(255, 248, 248, 255)    /**< GHOSTWHITE           */
#define GL_NC_GOLD                  GL_COLOR_DEF(255, 255, 215,   0)    /**< GOLD                 */
#define GL_NC_GOLDENROD             GL_COLOR_DEF(255, 218, 165,  32)    /**< GOLDENROD            */
#define GL_NC_GRAY                  GL_COLOR_DEF(255, 128, 128, 128)    /**< GRAY                 */
#define GL_NC_GREY                  GL_COLOR_DEF(255, 128, 128, 128)    /**< GREY                 */
#define GL_NC_GREEN                 GL_COLOR_DEF(255,   0, 128,   0)    /**< GREEN                */
#define GL_NC_GREENYELLOW           GL_COLOR_DEF(255, 173, 255,  47)    /**< GREENYELLOW          */
#define GL_NC_HONEYDEW              GL_COLOR_DEF(255, 240, 255, 240)    /**< HONEYDEW             */
#define GL_NC_HOTPINK               GL_COLOR_DEF(255, 255, 105, 180)    /**< HOTPINK              */
#define GL_NC_INDIANRED             GL_COLOR_DEF(255, 205,  92,  92)    /**< INDIANRED            */
#define GL_NC_INDIGO                GL_COLOR_DEF(255,  75,   0, 130)    /**< INDIGO               */
#define GL_NC_IVORY                 GL_COLOR_DEF(255, 255, 255, 240)    /**< IVORY                */
#define GL_NC_KHAKI                 GL_COLOR_DEF(255, 240, 230, 140)    /**< KHAKI                */
#define GL_NC_LAVENDER              GL_COLOR_DEF(255, 230, 230, 250)    /**< LAVENDER             */
#define GL_NC_LAVENDERBLUSH         GL_COLOR_DEF(255, 255, 240, 245)    /**< LAVENDERBLUSH        */
#define GL_NC_LAWNGREEN             GL_COLOR_DEF(255, 124, 252,   0)    /**< LAWNGREEN            */
#define GL_NC_LEMONCHIFFON          GL_COLOR_DEF(255, 255, 250, 205)    /**< LEMONCHIFFON         */
#define GL_NC_LIGHTBLUE             GL_COLOR_DEF(255, 173, 216, 230)    /**< LIGHTBLUE            */
#define GL_NC_LIGHTCORAL            GL_COLOR_DEF(255, 240, 128, 128)    /**< LIGHTCORAL           */
#define GL_NC_LIGHTCYAN             GL_COLOR_DEF(255, 224, 255, 255)    /**< LIGHTCYAN            */
#define GL_NC_LIGHTGOLDENRODYELLOW  GL_COLOR_DEF(255, 250, 250, 210)    /**< LIGHTGOLDENRODYELLOW */
#define GL_NC_LIGHTGRAY             GL_COLOR_DEF(255, 211, 211, 211)    /**< LIGHTGRAY            */
#define GL_NC_LIGHTGREEN            GL_COLOR_DEF(255, 144, 238, 144)    /**< LIGHTGREEN           */
#define GL_NC_LIGHTGREY             GL_COLOR_DEF(255, 211, 211, 211)    /**< LIGHTGREY            */
#define GL_NC_LIGHTPINK             GL_COLOR_DEF(255, 255, 182, 193)    /**< LIGHTPINK            */
#define GL_NC_LIGHTSALMON           GL_COLOR_DEF(255, 255, 160, 122)    /**< LIGHTSALMON          */
#define GL_NC_LIGHTSEAGREEN         GL_COLOR_DEF(255,  32, 178, 170)    /**< LIGHTSEAGREEN        */
#define GL_NC_LIGHTSKYBLUE          GL_COLOR_DEF(255, 135, 206, 250)    /**< LIGHTSKYBLUE         */
#define GL_NC_LIGHTSLATEGRAY        GL_COLOR_DEF(255, 119, 136, 153)    /**< LIGHTSLATEGRAY       */
#define GL_NC_LIGHTSLATEGREY        GL_COLOR_DEF(255, 119, 136, 153)    /**< LIGHTSLATEGREY       */
#define GL_NC_LIGHTSTEELBLUE        GL_COLOR_DEF(255, 176, 196, 222)    /**< LIGHTSTEELBLUE       */
#define GL_NC_LIGHTYELLOW           GL_COLOR_DEF(255, 255, 255, 224)    /**< LIGHTYELLOW          */
#define GL_NC_LIME                  GL_COLOR_DEF(255,   0, 255,   0)    /**< LIME                 */
#define GL_NC_LIMEGREEN             GL_COLOR_DEF(255,  50, 205,  50)    /**< LIMEGREEN            */
#define GL_NC_LINEN                 GL_COLOR_DEF(255, 250, 240, 230)    /**< LINEN                */
#define GL_NC_MAGENTA               GL_COLOR_DEF(255, 255,   0, 255)    /**< MAGENTA              */
#define GL_NC_MAROON                GL_COLOR_DEF(255, 128,   0,   0)    /**< MAROON               */
#define GL_NC_MEDIUMAQUAMARINE      GL_COLOR_DEF(255, 102, 205, 170)    /**< MEDIUMAQUAMARINE     */
#define GL_NC_MEDIUMBLUE            GL_COLOR_DEF(255,   0,   0, 205)    /**< MEDIUMBLUE           */
#define GL_NC_MEDIUMORCHID          GL_COLOR_DEF(255, 186,  85, 211)    /**< MEDIUMORCHID         */
#define GL_NC_MEDIUMPURPLE          GL_COLOR_DEF(255, 147, 112, 219)    /**< MEDIUMPURPLE         */
#define GL_NC_MEDIUMSEAGREEN        GL_COLOR_DEF(255,  60, 179, 113)    /**< MEDIUMSEAGREEN       */
#define GL_NC_MEDIUMSLATEBLUE       GL_COLOR_DEF(255, 123, 104, 238)    /**< MEDIUMSLATEBLUE      */
#define GL_NC_MEDIUMSPRINGGREEN     GL_COLOR_DEF(255,   0, 250, 154)    /**< MEDIUMSPRINGGREEN    */
#define GL_NC_MEDIUMTURQUOISE       GL_COLOR_DEF(255,  72, 209, 204)    /**< MEDIUMTURQUOISE      */
#define GL_NC_MEDIUMVIOLETRED       GL_COLOR_DEF(255, 199,  21, 133)    /**< MEDIUMVIOLETRED      */
#define GL_NC_MIDNIGHTBLUE          GL_COLOR_DEF(255,  25,  25, 112)    /**< MIDNIGHTBLUE         */
#define GL_NC_MINTCREAM             GL_COLOR_DEF(255, 245, 255, 250)    /**< MINTCREAM            */
#define GL_NC_MISTYROSE             GL_COLOR_DEF(255, 255, 228, 225)    /**< MISTYROSE            */
#define GL_NC_MOCCASIN              GL_COLOR_DEF(255, 255, 228, 181)    /**< MOCCASIN             */
#define GL_NC_NAVAJOWHITE           GL_COLOR_DEF(255, 255, 222, 173)    /**< NAVAJOWHITE          */
#define GL_NC_NAVY                  GL_COLOR_DEF(255,   0,   0, 128)    /**< NAVY                 */
#define GL_NC_OLDLACE               GL_COLOR_DEF(255, 253, 245, 230)    /**< OLDLACE              */
#define GL_NC_OLIVE                 GL_COLOR_DEF(255, 128, 128,   0)    /**< OLIVE                */
#define GL_NC_OLIVEDRAB             GL_COLOR_DEF(255, 107, 142,  35)    /**< OLIVEDRAB            */
#define GL_NC_ORANGE                GL_COLOR_DEF(255, 255, 165,   0)    /**< ORANGE               */
#define GL_NC_ORANGERED             GL_COLOR_DEF(255, 255,  69,   0)    /**< ORANGERED            */
#define GL_NC_ORCHID                GL_COLOR_DEF(255, 218, 112, 214)    /**< ORCHID               */
#define GL_NC_PALEGOLDENROD         GL_COLOR_DEF(255, 238, 232, 170)    /**< PALEGOLDENROD        */
#define GL_NC_PALEGREEN             GL_COLOR_DEF(255, 152, 251, 152)    /**< PALEGREEN            */
#define GL_NC_PALETURQUOISE         GL_COLOR_DEF(255, 175, 238, 238)    /**< PALETURQUOISE        */
#define GL_NC_PALEVIOLETRED         GL_COLOR_DEF(255, 219, 112, 147)    /**< PALEVIOLETRED        */
#define GL_NC_PAPAYAWHIP            GL_COLOR_DEF(255, 255, 239, 213)    /**< PAPAYAWHIP           */
#define GL_NC_PEACHPUFF             GL_COLOR_DEF(255, 255, 218, 185)    /**< PEACHPUFF            */
#define GL_NC_PERU                  GL_COLOR_DEF(255, 205, 133,  63)    /**< PERU                 */
#define GL_NC_PINK                  GL_COLOR_DEF(255, 255, 192, 203)    /**< PINK                 */
#define GL_NC_PLUM                  GL_COLOR_DEF(255, 221, 160, 221)    /**< PLUM                 */
#define GL_NC_POWDERBLUE            GL_COLOR_DEF(255, 176, 224, 230)    /**< POWDERBLUE           */
#define GL_NC_PURPLE                GL_COLOR_DEF(255, 128,   0, 128)    /**< PURPLE               */
#define GL_NC_RED                   GL_COLOR_DEF(255, 255,   0,   0)    /**< RED                  */
#define GL_NC_ROSYBROWN             GL_COLOR_DEF(255, 188, 143, 143)    /**< ROSYBROWN            */
#define GL_NC_ROYALBLUE             GL_COLOR_DEF(255,  65, 105, 225)    /**< ROYALBLUE            */
#define GL_NC_SADDLEBROWN           GL_COLOR_DEF(255, 139,  69,  19)    /**< SADDLEBROWN          */
#define GL_NC_SALMON                GL_COLOR_DEF(255, 250, 128, 114)    /**< SALMON               */
#define GL_NC_SANDYBROWN            GL_COLOR_DEF(255, 244, 164,  96)    /**< SANDYBROWN           */
#define GL_NC_SEAGREEN              GL_COLOR_DEF(255,  46, 139,  87)    /**< SEAGREEN             */
#define GL_NC_SEASHELL              GL_COLOR_DEF(255, 255, 245, 238)    /**< SEASHELL             */
#define GL_NC_SIENNA                GL_COLOR_DEF(255, 160,  82,  45)    /**< SIENNA               */
#define GL_NC_SILVER                GL_COLOR_DEF(255, 192, 192, 192)    /**< SILVER               */
#define GL_NC_SKYBLUE               GL_COLOR_DEF(255, 135, 206, 235)    /**< SKYBLUE              */
#define GL_NC_SLATEBLUE             GL_COLOR_DEF(255, 106,  90, 205)    /**< SLATEBLUE            */
#define GL_NC_SLATEGRAY             GL_COLOR_DEF(255, 112, 128, 144)    /**< SLATEGRAY            */
#define GL_NC_SLATEGREY             GL_COLOR_DEF(255, 112, 128, 144)    /**< SLATEGREY            */
#define GL_NC_SNOW                  GL_COLOR_DEF(255, 255, 250, 250)    /**< SNOW                 */
#define GL_NC_SPRINGGREEN           GL_COLOR_DEF(255,   0, 255, 127)    /**< SPRINGGREEN          */
#define GL_NC_STEELBLUE             GL_COLOR_DEF(255,  70, 130, 180)    /**< STEELBLUE            */
#define GL_NC_TAN                   GL_COLOR_DEF(255, 210, 180, 140)    /**< TAN                  */
#define GL_NC_TEAL                  GL_COLOR_DEF(255,   0, 128, 128)    /**< TEAL                 */
#define GL_NC_THISTLE               GL_COLOR_DEF(255, 216, 191, 216)    /**< THISTLE              */
#define GL_NC_TOMATO                GL_COLOR_DEF(255, 255,  99,  71)    /**< TOMATO               */
#define GL_NC_TURQUOISE             GL_COLOR_DEF(255,  64, 224, 208)    /**< TURQUOISE            */
#define GL_NC_VIOLET                GL_COLOR_DEF(255, 238, 130, 238)    /**< VIOLET               */
#define GL_NC_WHEAT                 GL_COLOR_DEF(255, 245, 222, 179)    /**< WHEAT                */
#define GL_NC_WHITE                 GL_COLOR_DEF(255, 255, 255, 255)    /**< WHITE                */
#define GL_NC_WHITESMOKE            GL_COLOR_DEF(255, 245, 245, 245)    /**< WHITESMOKE           */
#define GL_NC_YELLOW                GL_COLOR_DEF(255, 255, 255,   0)    /**< YELLOW               */
#define GL_NC_YELLOWGREEN           GL_COLOR_DEF(255, 154, 205,  50)    /**< YELLOWGREEN          */

#endif
