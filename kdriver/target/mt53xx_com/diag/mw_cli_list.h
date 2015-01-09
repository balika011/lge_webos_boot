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



#ifndef CONFIG_TV_DRV_VFY
extern CLI_EXEC_T *CLI_Os_function(void);
extern CLI_EXEC_T *CLI_IMGRZ_function(void);
extern CLI_EXEC_T *CLI_linux_function(void);
//extern CLI_EXEC_T *CLI_Playmgr_function(void);
extern CLI_EXEC_T *CLI_Gfx_function(void);

//extern CLI_EXEC_T *CLI_Gfx3d_function(void);

extern CLI_EXEC_T *CLI_GfxEmu_function(void);
extern CLI_EXEC_T *CLI_GfxSc_function(void);
extern CLI_EXEC_T *CLI_GfxScEmu_function(void);
extern CLI_EXEC_T *CLI_Osd_function(void);
extern CLI_EXEC_T *CLI_OsdDiag_function(void);
extern CLI_EXEC_T *CLI_Pmx_function(void);
extern CLI_EXEC_T *CLI_rMUSB_function(void);
extern CLI_EXEC_T *CLI_Fci_function(void);
extern CLI_EXEC_T *CLI_Uart_function(void);
extern CLI_EXEC_T *CLI_Psipsr_function(void);
extern CLI_EXEC_T *CLI_Nav_function(void);
extern CLI_EXEC_T *CLI_Sif_function(void);
extern CLI_EXEC_T *CLI_Pwm_function(void);
extern CLI_EXEC_T *CLI_Eeprom_function(void);
//extern CLI_EXEC_T *CLI_Stg_function(void);
extern CLI_EXEC_T *CLI_Nim_function(void);
extern CLI_EXEC_T *CLI_Ir_function(void);
extern CLI_EXEC_T *CLI_Rtc_function(void);
extern CLI_EXEC_T *CLI_Aud_function(void);
extern CLI_EXEC_T *CLI_Nptv_function(void);
extern CLI_EXEC_T *CLI_Av_function(void);
extern CLI_EXEC_T *CLI_Vdp_function(void);
extern CLI_EXEC_T *CLI_Fbm_function(void);
extern CLI_EXEC_T *CLI_Dbs_function(void);
extern CLI_EXEC_T *CLI_Mpv_function(void);
extern CLI_EXEC_T *CLI_Vdec_function(void);
#if defined(CC_MT5396)|| defined(CC_MT5398) || defined(CC_ENABLE_VOMX)
extern CLI_EXEC_T *CLI_Vomx_function(void);
#endif
#ifdef CC_SUPPORT_VENC
extern CLI_EXEC_T *CLI_VEnc_function(void);
#endif
#ifdef CC_SUPPORT_MUXER
extern CLI_EXEC_T *CLI_Muxer_function(void);
#endif
//#if defined(CC_MT5365) || defined(CC_MT5395)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)|| defined(CC_MT5880) || defined(CC_MT5860)||defined(CC_MT5881)
extern CLI_EXEC_T *CLI_Tcon_function(void);
//#endif
#if defined(CC_MT5890)
extern CLI_EXEC_T *CLI_TS_function(void);
#endif
extern CLI_EXEC_T *CLI_LZHS_function(void);
#ifdef CC_NAND_ENABLE
extern CLI_EXEC_T *CLI_Nand_function(void);
#endif
extern CLI_EXEC_T *CLI_Dmx_function(void);
extern CLI_EXEC_T *CLI_MemTest_function(void);
extern CLI_EXEC_T *CLI_Cec_function(void);
#if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
extern CLI_EXEC_T *CLI_Cbus_function(void);
#endif
extern CLI_EXEC_T *CLI_Pdwnc_function(void);
extern CLI_EXEC_T *CLI_Bwt_function(void);
#ifdef ENABLE_MULTIMEDIA
extern CLI_EXEC_T *CLI_Jpg_function(void);
#endif
extern CLI_EXEC_T *CLI_Gpio_function(void);
#ifndef CC_NOR_DISABLE
extern CLI_EXEC_T *CLI_Nor_function(void);
#endif
#if defined(CC_MT5365) || defined(CC_MT5395)
extern CLI_EXEC_T *CLI_Ldm_function(void);
#endif
extern CLI_EXEC_T *CLI_Mid_function(void);
#ifdef ENABLE_MULTIMEDIA
extern CLI_EXEC_T *CLI_Swdmx_function(void);
extern CLI_EXEC_T *CLI_Feeder_function(void);
#endif
#ifdef FVR_SUPPORT_CAPTURE
extern CLI_EXEC_T *CLI_Fvr_function(void);
#endif
extern CLI_EXEC_T *CLI_tve_function(void);
extern CLI_EXEC_T *CLI_TimeProfile_function(void);
extern CLI_EXEC_T *CLI_BIM_function(void);
extern CLI_EXEC_T *CLI_dhrystone_function(void);
#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5880)|| defined(CC_MT5881)|| defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
extern CLI_EXEC_T *CLI_NET_function(void);
#endif
extern CLI_EXEC_T *CLI_Pcmcia_function(void);
#ifdef DRV_SUPPORT_EXTMJC
extern CLI_EXEC_T *CLI_Extmjc_function(void);
#endif
extern CLI_EXEC_T *CLI_Cust_function(void);
#ifdef CC_SUPPORT_SMC_CLI
extern CLI_EXEC_T *CLI_Smc_function(void);
#endif
#ifdef CC_LINUX_SLT_SUPPORT
extern CLI_EXEC_T *CLI_SLT_function(void);
extern CLI_EXEC_T *CLI_Playmgr_function(void);
extern CLI_EXEC_T *CLI_Webp_function(void);
extern CLI_EXEC_T *CLI_Png_function(void);
#endif
static const CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] = {
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
// CLI_Gfx_function,
// CLI_GfxEmu_function,
// CLI_GfxSc_function,
// CLI_GfxScEmu_function,
CLI_Osd_function,
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
CLI_Pmx_function,
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
/*
CLI_OsdDiag_function,
CLI_Fci_function,
CLI_Uart_function,
CLI_Psipsr_function,
CLI_Nav_function,
*/
#if defined(LINUX_TURNKEY_SOLUTION) || (defined(LINUX_DRV_ROOT) && defined(__MODEL_slt__))
#if defined(CC_USB_COMPLIANCE_TEST) || defined(CC_CDC_SUPPORT) || defined(__MODEL_slt__)
CLI_rMUSB_function,
#endif
#else
CLI_rMUSB_function,
#endif //LINUX_TURNKEY_SOLUTION
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
CLI_Sif_function,
CLI_Pwm_function,
#ifdef CC_NO_KRL_UART_DRV
CLI_Uart_function,
#endif
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
CLI_Eeprom_function,
//CLI_Stg_function,
#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
CLI_Nim_function,
#ifdef CC_SUPPORT_SMC_CLI
CLI_Smc_function,
#endif
#if !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE)
CLI_Ir_function,
CLI_Rtc_function,
CLI_Aud_function,
CLI_Nptv_function,
CLI_Av_function,
CLI_Vdp_function,
CLI_Fbm_function,
CLI_Dbs_function,
CLI_Mpv_function,
CLI_Vdec_function,
#if defined(CC_MT5396)|| defined(CC_MT5398) || defined(CC_ENABLE_VOMX)
CLI_Vomx_function,
#endif
//#if defined(CC_MT5365) || defined(CC_MT5395)|| defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398)|| defined(CC_MT5880) || defined(CC_MT5860)||defined(CC_MT5881)
CLI_Tcon_function,
//#endif
#if defined(CC_MT5890)
  CLI_TS_function,
#endif
#ifdef CC_SUPPORT_VENC
CLI_VEnc_function,
#endif
#ifdef CC_SUPPORT_MUXER
CLI_Muxer_function,
#endif
#ifndef LINUX_TURNKEY_SOLUTION
CLI_LZHS_function,
#endif //LINUX_TURNKEY_SOLUTION
#if !defined(CC_MT8223)
#ifndef LINUX_TURNKEY_SOLUTION
#ifdef CC_NAND_ENABLE
CLI_Nand_function,
#endif
#endif //LINUX_TURNKEY_SOLUTION
CLI_Dmx_function,
#endif
CLI_MemTest_function,
CLI_Cec_function,
#if defined(CC_MT5881) || defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
CLI_Cbus_function,
#endif
CLI_Pdwnc_function,
// CLI_Bwt_function,
#ifdef ENABLE_MULTIMEDIA
CLI_Jpg_function,
#endif
CLI_Gpio_function,
#ifndef LINUX_TURNKEY_SOLUTION
#ifndef CC_NOR_DISABLE
CLI_Nor_function,
#endif
#endif //LINUX_TURNKEY_SOLUTION
#if defined(CC_MT5365) || defined(CC_MT5395)
CLI_Ldm_function,
#endif
CLI_Mid_function,
#ifdef ENABLE_MULTIMEDIA
CLI_Swdmx_function,
#if !defined(CC_MT8223)
CLI_Feeder_function,
#endif
#endif
#ifdef FVR_SUPPORT_CAPTURE
CLI_Fvr_function,
#endif
CLI_tve_function,
CLI_TimeProfile_function,
CLI_BIM_function,
CLI_Pcmcia_function,
#ifdef DRV_SUPPORT_EXTMJC
CLI_Extmjc_function,
#endif

CLI_Os_function,
#ifdef __KERNEL__
CLI_linux_function,
#endif // __KERNEL__
CLI_Gfx_function,

//CLI_Gfx3d_function,

CLI_IMGRZ_function,
//CLI_Playmgr_function,
#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5880)|| defined(CC_MT5881)|| defined(CC_MT5399) || defined(CC_MT5890) || defined(CC_MT5882)
CLI_NET_function,
#endif
#ifdef CC_LINUX_SLT_SUPPORT
CLI_Nav_function,
CLI_SLT_function,
CLI_Playmgr_function,
CLI_Webp_function,
CLI_Png_function,
#endif
#ifdef CC_CUSTOMER_CLI
CLI_Cust_function,
#endif

#endif /* !defined(CLI_STRESS_MODE) && !defined(DRV_CLI_DISABLE) */
};
#else	// CONFIG_TV_DRV_VFY
extern CLI_EXEC_T *CLI_udvt_function(void);
#ifdef LINUX_EMU_SIFSW_SUPPORT
extern CLI_EXEC_T *CLI_Gpio_function(void);
extern CLI_EXEC_T *CLI_Sif_function(void);
#endif
extern CLI_EXEC_T *CLI_Vdec_function(void);
extern CLI_EXEC_T *CLI_Gfx3d_function(void);
//extern CLI_EXEC_T *CLI_NETDC_function(void);
#ifdef ENABLE_MULTIMEDIA   
extern CLI_EXEC_T *CLI_Jpg_function(void);
extern CLI_EXEC_T *CLI_IMGRZ_VFY_function(void);
extern CLI_EXEC_T *CLI_PNG_VFY_function(void);
extern CLI_EXEC_T *CLI_IRT_function(void);

#endif

extern CLI_EXEC_T *CLI_udvt_function(void);
static const CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] = {
	CLI_udvt_function,		
	#ifdef LINUX_EMU_SIFSW_SUPPORT
    CLI_Gpio_function,
    CLI_Sif_function,
	#endif
	CLI_Vdec_function,
	CLI_Gfx3d_function,
    //CLI_NETDC_function,
#ifdef ENABLE_MULTIMEDIA   
CLI_IMGRZ_VFY_function,
CLI_PNG_VFY_function,
CLI_Jpg_function,
  CLI_IRT_function,

#endif
  CLI_udvt_function,

   
};
#endif
