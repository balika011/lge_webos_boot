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
 * $RCSfile: hw_vbi.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/* Ported from 5392 (Revision 1.16) */
#include "general.h"
#include "hw_vbi.h"
#include "drv_vbi.h"

#if TVD_SERRCNT_WA
extern BOOL _sbSerrCntAdjEn;
#endif
/* Set which VBI services are activated in which VBI slicer(VBI0 or VBI2) */
void vHwVbiSetLineEnable(UINT8 bVBISuite, UINT32 u4LineEn)
{
    if(bVBISuite == VBI0)
    {
        if (u4LineEn & VPS_LINE_EN)
            vIO32WriteFldAlign(VBI_00, 1, VBI_VPS_EN);
        else
            vIO32WriteFldAlign(VBI_00, 0, VBI_VPS_EN);

        if (u4LineEn & CC_LINE_EN)
            vIO32WriteFldAlign(VBI_00, 1, VBI_CC_EN);
        else
            vIO32WriteFldAlign(VBI_00, 0, VBI_CC_EN);  

        if (u4LineEn & WSS_LINE_EN)
        {
			#if SUPPORT_TYPE_B_VBI
            vIO32WriteFldAlign(VBI_TYPE_B_00, 1, VBI_B_HD_WSS_EN);
			#endif
            vIO32WriteFldAlign(VBI_00, 1, VBI_WSS_EN);
        }
        else
        {
			#if SUPPORT_TYPE_B_VBI
            vIO32WriteFldAlign(VBI_TYPE_B_00, 0, VBI_B_HD_WSS_EN);
			#endif
            vIO32WriteFldAlign(VBI_00, 0, VBI_WSS_EN);        
        }
        if (u4LineEn & TT_LINE_EN)
            vIO32WriteFldAlign(VBI_00, 1, VBI_TT_EN);
        else
            vIO32WriteFldAlign(VBI_00, 0, VBI_TT_EN); 
    }
    else
    {
        if (u4LineEn & VPS_LINE_EN)
            vIO32WriteFldAlign(VBI2_00, 1, VBI2_VPS_EN);
        else
            vIO32WriteFldAlign(VBI2_00, 0, VBI2_VPS_EN);

        if (u4LineEn & CC_LINE_EN)
            vIO32WriteFldAlign(VBI2_00, 1, VBI2_CC_EN);
        else
            vIO32WriteFldAlign(VBI2_00, 0, VBI2_CC_EN);  

        if (u4LineEn & WSS_LINE_EN)
        {
			#if SUPPORT_TYPE_B_VBI
            vIO32WriteFldAlign(VBI2_TYPE_B_00, 1, VBI2_B_HD_WSS_EN);
			#endif
            vIO32WriteFldAlign(VBI2_00, 1, VBI2_WSS_EN);
        }
        else
        {
			#if SUPPORT_TYPE_B_VBI
            vIO32WriteFldAlign(VBI2_TYPE_B_00, 0, VBI2_B_HD_WSS_EN);
			#endif
            vIO32WriteFldAlign(VBI2_00, 0, VBI2_WSS_EN);        
        }
        if (u4LineEn & TT_LINE_EN)
            vIO32WriteFldAlign(VBI2_00, 1, VBI2_TT_EN);
        else
            vIO32WriteFldAlign(VBI2_00, 0, VBI2_TT_EN); 
    }
}

/* Set  VBI DRAM output mode */
void vHwVbiSetOutputMode(UINT8 bVBISuite, UINT32 u4Mode)
{
    if(bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_04, u4Mode, VBI_OMODE);          
    }
    else
    {
        vIO32WriteFldAlign(VBI2_04, u4Mode, VBI2_OMODE);        
    }
}

/* Set VBI source */
void vHwVbiSetSrc(UINT8 bVBISuite, UINT8 bSrc)
{
    if(bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_01, bSrc, VBI_VSRC);         
    }
    else
    {
        vIO32WriteFldAlign(VBI2_01, bSrc, VBI2_VSRC);          
    }
}

/* Get VBI source */
UINT8 bHwVbiGetSrc(UINT8 bVBISuite)
{    
    UINT8 u1Src;
    
    if (bVBISuite == VBI0)    
    {      
        u1Src = IO32ReadFldAlign(VBI_01, VBI_VSRC);          
    }    
    else    
    {       
        u1Src = IO32ReadFldAlign(VBI2_01, VBI2_VSRC);                    
    }

    return u1Src;
}

/* Set VBI DRAM starting address with 16 bytes alignment */
void vHwVbiSetMbase0(UINT8 bVBISuite, UINT32 u4BaseAddr)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_02, (u4BaseAddr>>4), VBI_MBASE0);        
		vIO32WriteFldAlign(VBI_0E, (u4BaseAddr >> 29), VBI_MBASE01);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_02, (u4BaseAddr>>4), VBI2_MBASE0); 
		vIO32WriteFldAlign(VBI_0E, (u4BaseAddr >> 29), VBI2_MBASE01);
    }
}

/* Set VBI DRAM FIFO length with 16 bytes alignment */
void vHwVbiSetMOffset(UINT8 bVBISuite, UINT32 u4offset)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_04, (u4offset>>4), VBI_MOFFSET); 
		vIO32WriteFldAlign(VBI_0E, (u4offset >> 29), VBI_MOFFSET1);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_04, (u4offset>>4), VBI2_MOFFSET);  
		vIO32WriteFldAlign(VBI_0E, (u4offset >> 29), VBI2_MOFFSET1);
    }
}

/* Set VBI DRAM FIFO or LINE mode */
void vHwVbiSetFifoMode(UINT8 bVBISuite, UINT32 u4FifoMode)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_04, u4FifoMode, VBI_MFIFO);          
    }
    else
    {
        vIO32WriteFldAlign(VBI2_04, u4FifoMode, VBI2_MFIFO);           
    }
}

/* Reset VBI DRAM FIFO pointer */
void vHwVbiResetFifo(UINT8 bVBISuite)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_04, 1, VBI_MFIFORST);
	    vIO32WriteFldAlign(VBI_0E, 1, VBI_MFIFORST1);
        //vUtDelay10ms(2); //Wait Rst ready.
    }
    else
    {
        vIO32WriteFldAlign(VBI2_04, 1, VBI2_MFIFORST);     
        //vUtDelay10ms(2); //Wait Rst ready.
    }
}

/* Clear Reset VBI DRAM FIFO pointer */
void vHwVbiClrResetFifo(UINT8 bVBISuite)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_04, 0, VBI_MFIFORST);    
		vIO32WriteFldAlign(VBI_0E, 0, VBI_MFIFORST1);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_04, 0, VBI2_MFIFORST);        
    }
}

/* Read VBI DRAM FIFO write pointer */
UINT32 u4HwVbiReadWritePtr(UINT8 bVBISuite)
{
    UINT32 u4WP;
    
    if (bVBISuite == VBI0)
    {
        u4WP = (u4GetVbiWP() + (IO32ReadFldAlign(VBI_0E, VBI_MBASE01) << 29));
    }
    else
    {
        u4WP = (u4GetVbi2WP() + (IO32ReadFldAlign(VBI_0E, VBI2_MBASE01) << 29));
    }

    return u4WP;
}

void vHwVbiSetLSC(UINT8 bVBISuite, UINT32 u4LSC)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_03, u4LSC, VBI_LSC);        
    }
    else
    {
        vIO32WriteFldAlign(VBI2_03, u4LSC, VBI2_LSC);        
    }
}

void vHwVbiSetInitZCThr(UINT8 bVBISuite, UINT8 u1ZCThr)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_00, u1ZCThr, VBI_ZC_THR);           
    }
    else
    {
        vIO32WriteFldAlign(VBI2_00, u1ZCThr, VBI2_ZC_THR);         
    }
}

UINT8 bHwVbiGetInitZCThr(UINT8 bVBISuite)
{
    UINT8 u1ZCThr;
    
    if (bVBISuite == VBI0)
    {
        u1ZCThr = IO32ReadFldAlign(VBI_00, VBI_ZC_THR);          
    }
    else
    {
        u1ZCThr = IO32ReadFldAlign(VBI2_00, VBI2_ZC_THR);           
    }

    return u1ZCThr;
}

/* Set,Clear Progressive Scan Mode */
void vHwVbiSetClrPScan(UINT8 bVBISuite, UINT8 bSetClr)
{
    if (bVBISuite == VBI0)
    {
        if (bSetClr == 1)
        {
            vIO32WriteFldAlign(VBI_01, 1, VBI_PSCAN);                
        }
        else
        {
            vIO32WriteFldAlign(VBI_01, 0, VBI_PSCAN);              
        }
    }
    else
    {
        if (bSetClr == 1)
        {
            vIO32WriteFldAlign(VBI2_01, 1, VBI2_PSCAN);             
        }
        else
        {
            vIO32WriteFldAlign(VBI2_01, 0, VBI2_PSCAN);             
        }
    }
}

/* Set WSS Frequency */
void vHwVbiSetWSSFreq(UINT8 bVBISuite, UINT32 u4Freq)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_0A, u4Freq, VBI_WSS_FREQ);         
    }
    else
    {
        vIO32WriteFldAlign(VBI2_0A, u4Freq, VBI2_WSS_FREQ);        
    }
}
#if SUPPORT_TYPE_B_VBI
/* Set TYPE B WSS Frequency */
void vHwVbiSetTypeBWSSFreq(UINT8 bVBISuite, UINT32 u4Freq)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_TYPE_B_00, u4Freq, VBI_B_HD_FREQ);         
    }
    else
    {
        vIO32WriteFldAlign(VBI2_TYPE_B_00, u4Freq, VBI2_B_HD_FREQ);        
    }
}
#endif
/* Set VBI Input Mode, only distinguish 525/625 mode */
void vHwVbiSetIMode(UINT8 bVBISuite, UINT8 bImode)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_01, bImode, VBI_IMODE);          
    }
    else
    {
        vIO32WriteFldAlign(VBI2_01, bImode, VBI2_IMODE);          
    }
}

/* Set VBI initial zero-crossing auto detect enable */
void vHwVbiSetInitZC(UINT8 bVBISuite, UINT8 bSet)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_01, bSet, VBI_INIT_ZC);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_01, bSet, VBI2_INIT_ZC);
    }
}

/* brief: Set VBI paramter according to input source type or specific application
 * param: bVBISuite specifies VBI0 or VBI2
 * param: bVbiMode: VBI_NTSC_MODE / VBI_PAL_MODE / VBI_CNI_MODE
 *  VBI_NTSC_MODE: general setting for NTSC input (CC/WSS)
 *  VBI_PAL_MODE: general setting for PAL input (TT/WSS)
 *  VBI_CNI_MODE: general setting for CNI code retrieve.
 * retval    None
*/
void vHwVbiSetVbiMode(UINT8 bVBISuite, UINT8 bVbiMode)
{
    if (bVBISuite == VBI0) 
    {
        switch (bVbiMode) 
        {
            case VBI_NTSC_MODE:
                
                /* NTSC, CC525 + WSS525 in VBI0 */

                /* General setting */
                vIO32WriteFldAlign(VBI_00, 18, VBI_THR_DET_WIDTH);
                vIO32WriteFldAlign(VBI_01, 0, VBI_STC_ERR);              /* "NO" set in VBI2 */

                /* CC setting */
                vIO32WriteFldAlign(VBI_05, 0, VBI_CC_DRAM_EN);                             
                vIO32WriteFldAlign(VBI_05, 1, VBI_CC_DC_SEL);  
                vIO32WriteFldAlign(VBI_05, 114, VBI_CC_STC_END);
                vIO32WriteFldAlign(VBI_06, 0x8F00, VBI_CC_FREQ);
                vIO32WriteFldAlign(VBI_05, 1, VBI_CC_LPF_EN);
                vIO32WriteFldAlign(VBI_00, 1, VBI_CC_ECC_EN);

                #if SUPPORT_CLOSE_CAPTION_2X
                /* CC2X setting*/
                vIO32WriteFldAlign(VBI_03, 1, VBI_CC2X); 
                vIO32WriteFldAlign(VBI_0B, 0, VBI_TT_DC_SEL);
                vIO32WriteFldAlign(VBI_0B, 114, VBI_TT_STC_END);                 
                vIO32WriteFldAlign(VBI_0C, 0x8EFD, VBI_TT_FREQ); // Spec:0xD680, gEditor:0xD200
                vIO32WriteFldAlign(VBI_0B, 1, VBI_TT_LPF_EN);                
                vIO32WriteFldAlign(VBI_0C, 183, VBI_TT_STCODE); 
                vIO32WriteFldAlign(VBI_0C, 7, VBI_TT_LEN);                                                          	                                            	            
                vIO32WriteFldAlign(VBI_0B, 1, VBI_TT_INIT_SEL);
                vIO32WriteFldAlign(VBI_00, 1, VBI_TT_ECC_EN);
                vIO32WriteFldAlign(VBI_00, 0, VBI_TT_ECC_SEL);                 
                vIO32WriteFldAlign(VBI_00, 0, VBI_TT_ECC_EN); // Disable ECC enable

                //Disable GD 
                vIO32WriteFldAlign(VBI_0B, 0, VBI_GRPDLY_CMP_EN); // Disable GD CMP
                #endif

                /* Group Delay Setting */
                vIO32WriteFldAlign(VBI_01, 0, VBI_ERR_POW_SEL);
                vIO32WriteFldAlign(VBI_01, 0, VBI_ERR_NO_ZERO);

                /* VPS setting */
                vIO32WriteFldAlign(VBI_07, 0, VBI_VPS_DRAM_EN);

                /* WSS setting */
                vIO32WriteFldAlign(VBI_09, 0, VBI_WSS_DRAM_EN);
                vIO32WriteFldAlign(VBI_09, 59, VBI_WSS_STC_END);
                vIO32WriteFldAlign(VBI_0A, 64, VBI_WSS_STCODE);
                vIO32WriteFldAlign(VBI_0A, 4, VBI_WSS_LEN);
                vHwVbiSetWSSFreq(VBI0, 0x283B);
                
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_LPF_EN)); /* bit 30 */
                /* WSS 525 no effect for  VBI_WSS_DC_SEL */
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_DC_SEL)); /* bit 29*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_INIT_SEL)); /* bit 28*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_RUNIN_EN)); /* bit 27*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_CRISEL)); /* bit 26*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_STSEL)); /* bit 25*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_STCHK_SEL)); /* bit 23*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_BSTART_SEL)); /* bit 15*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_STRICT_RUNIN)); /* bit 14*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_BB_SEL)); /* bit 13*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_F1_SEL)); /* bit 12*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_LERR_EN)); /* bit 11*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_IGNORE_AMBI)); /* bit 10*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS_LPF_EN)); /* bit 30*/                
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS_LPF_SEL)); /* bit 24*/

                /* VBI Input Mode setting */
                vIO32WriteFldAlign(VBI_01, VBI_IMODE_NTSC, VBI_IMODE);  
                /* VBI CC clock run-in status setting */
                vIO32WriteFldAlign(VBI_01, 1, VBI_CNI0_STA_SEL);
                /* Adatpive ZC Setting */
                vIO32WriteFldAlign(VBI_0D, 0, VBI_ADAPTIVE_INIT_ZC);
				
				#if SUPPORT_TYPE_B_VBI
                vIO32WriteFldAlign(VBI_TYPE_B_00, 0, VBI_B_LSC_MSB); 
                vIO32WriteFldAlign(VBI_TYPE_B_00, 0, VBI_B_HD_DC_SEL);
                vIO32WriteFldAlign(VBI_TYPE_B_00, 0, VBI_B_HD_INIT_SEL);                 
                
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0, VBI_B_HD_RUNIN_EN);                
                vIO32WriteFldAlign(VBI_TYPE_B_01, 1, VBI_B_HD_LPF_EN); 
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0, VBI_B_HD_LPF_SEL);                                                          	                                            	            
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0, VBI_B_HD_DRAM_EN);
                vIO32WriteFldAlign(VBI_TYPE_B_01, 3, VBI_B_HD_SAMPLE_SEL);
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0x21, VBI_B_HD_LEN);                 
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0x3b, VBI_B_STC_END);
                vIO32WriteFldAlign(VBI_TYPE_B_01, 0x40, VBI_B_STC);
				
                vHwVbiSetTypeBWSSFreq(VBI0, 0x1400);
                #endif
                break;
                
            case VBI_PAL_MODE:
            case VBI_CNI_MODE:
                
                /* PAL, TT 625B and WSS 625 */
                /* General setting */
                vIO32WriteFldAlign(VBI_00, 12, VBI_THR_DET_WIDTH);
                vIO32WriteFldAlign(VBI_01, 0, VBI_STC_ERR);                    

                /* CC setting */
				vIO32WriteFldAlign(VBI_05, 0, VBI_CC_DRAM_EN);                             
                vIO32WriteFldAlign(VBI_05, 1, VBI_CC_DC_SEL);  
                vIO32WriteFldAlign(VBI_05, 114, VBI_CC_STC_END);
                vIO32WriteFldAlign(VBI_06, 0x9000, VBI_CC_FREQ);
                vIO32WriteFldAlign(VBI_05, 1, VBI_CC_LPF_EN);
                vIO32WriteFldAlign(VBI_00, 1, VBI_CC_ECC_EN);

                /* VPS setting */
                vIO32WriteFldAlign(VBI_07, 0, VBI_VPS_DRAM_EN);                

                /* WSS setting */
                vIO32WriteFldAlign(VBI_09, 0, VBI_WSS_DRAM_EN); 
                vIO32WriteFldAlign(VBI_09, 75, VBI_WSS_STC_END);
                vIO32WriteFldAlign(VBI_0A, 248, VBI_WSS_STCODE);
                vIO32WriteFldAlign(VBI_0A, 3, VBI_WSS_LEN);                 
                vIO32WriteFldAlign(VBI_0A, 0x1CCD, VBI_WSS_FREQ); 
                
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_LPF_EN)); /* bit 30 */
                /* Recommand: VBI_WSS_DC_SEL = 1 */
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS_DC_SEL)); /* bit 29*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_INIT_SEL)); /* bit 28*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS_RUNIN_EN)); /* bit 27*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_CRISEL)); /* bit 26*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_STSEL)); /* bit 25*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_STCHK_SEL)); /* bit 23*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_BSTART_SEL)); /* bit 15*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS625_STRICT_RUNIN)); /* bit 14*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_BB_SEL)); /* bit 13*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_F1_SEL)); /* bit 12*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_LERR_EN)); /* bit 11*/
                vIO32WriteFldAlign(VBI_09, 1, (VBI_WSS625_IGNORE_AMBI)); /* bit 10*/
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_LPF_EN)); /* bit 30*/                
                vIO32WriteFldAlign(VBI_09, 0, (VBI_WSS_LPF_SEL)); /* bit 24*/

                /* TTX setting */
                vIO32WriteFldAlign(VBI_0B, 57, VBI_TT_STC_END); 
                vIO32WriteFldAlign(VBI_0C, 39, VBI_TT_STCODE); 
                vIO32WriteFldAlign(VBI_0C, 83, VBI_TT_LEN);
                vIO32WriteFldAlign(VBI_0C, 0x14c2, VBI_TT_FREQ); 
                vIO32WriteFldAlign(VBI_0B, 1, VBI_TT_LPF_EN);

                /* Recommand: VBI_TT_ZC_SEL = 0, VBI_TT_DC_SEL = x*/ 
                vIO32WriteFldAlign(VBI_01, 0, VBI_TT_ZC_SEL);
                vIO32WriteFldAlign(VBI_0B, 1, VBI_TT_DC_SEL); 
                
                vIO32WriteFldAlign(VBI_0B, 1, VBI_TT_INIT_SEL);
                vIO32WriteFldAlign(VBI_00, 1, VBI_TT_ECC_EN);
                vIO32WriteFldAlign(VBI_00, 1, VBI_TT_ECC_SEL);        

                /* VBI Input Mode setting */
                vIO32WriteFldAlign(VBI_01, VBI_IMODE_PALN, VBI_IMODE);                 

                /* For CNI mode line settings are required, 
                   since VBI_ResetSrc() won't do these settings */
                   
                #if SUPPORT_CLOSE_CAPTION_2X
                /* Reconver CC2X setting*/
                vIO32WriteFldAlign(VBI_03, 0, VBI_CC2X); 
                // Enable GD 
                vIO32WriteFldAlign(VBI_0B, 1, VBI_GRPDLY_CMP_EN); // Disable GD CMP
                #endif
                /* Group Delay Setting */
                vIO32WriteFldAlign(VBI_01, 1, VBI_ERR_POW_SEL);
                vIO32WriteFldAlign(VBI_01, 0, VBI_ERR_NO_ZERO);
                vIO32WriteFldAlign(VBI_0D, 1, VBI_ADAPTIVE_INIT_ZC);
                break;

            default:
                break;
        }

        vIO32WriteFldAlign(VBI_01, 1, VBI_3D_SEL);
        //vIO32WriteFldAlign(VBI_00, 0, VBI_PE_GAIN);
        vIO32WriteFldAlign(VBI_00, 4, VBI_PROBLEM_TH);
        vIO32WriteFldAlign(VBI_01, 0, VBI_ECC_SEL2);
        vIO32WriteFldAlign(VBI_01, 0, VBI_ECC_SEL3);
        vIO32WriteFldAlign(VBI_03, 3, VBI_STC_PTHR);
        vIO32WriteFldAlign(VBI_01, 1, VBI_TT_LINE_INFO);
        vHwVbiSetInitZC(VBI0, 1);
        vIO32WriteFldAlign(VBI_0A, 0, VBI_WSS_PSCAN); /* 3x down-sample */
        vIO32WriteFldAlign(VBI_01, 0, VBI_TVD_HSEL);
        vIO32WriteFldMulti(VBI_0D,  P_Fld(0,VBI_TT_FAST_ZC)|
                                   P_Fld(1,VBI_YEDGE_EN)|
                                   P_Fld(0,VBI_WAND_YC)|
                                   P_Fld(1,VBI_GRPDLY_BW_SEL)|
                                   P_Fld(4,VBI_BLANK_ZC_THR));
    } 
    else 
    {
        switch (bVbiMode) 
        {
            case VBI_NTSC_MODE:
            
                /* NTSC, CC525 + WSS525 in VBI2 */
                /* General setting */

                /* CC setting */
                vIO32WriteFldAlign(VBI2_05, 0, VBI2_CC_DRAM_EN);
                vIO32WriteFldAlign(VBI2_05, 114, VBI2_CC_STC_END);
                vIO32WriteFldAlign(VBI2_06, 0x8F00, VBI2_CC_FREQ);
                vIO32WriteFldAlign(VBI2_05, 1, VBI2_CC_LPF_EN);

                /* VPS setting */
                vIO32WriteFldAlign(VBI2_07, 0, VBI2_VPS_DRAM_EN);

                /* WSS setting */
                vIO32WriteFldAlign(VBI2_09, 0, VBI2_WSS_DRAM_EN);
                vIO32WriteFldAlign(VBI2_09, 59, VBI2_WSS_STC_END);
                vIO32WriteFldAlign(VBI2_0A, 64, VBI2_WSS_STCODE);
                vIO32WriteFldAlign(VBI2_0A, 4, VBI2_WSS_LEN);
                vHwVbiSetWSSFreq(VBI2, 0x283B);
                
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_LPF_EN)); /* bit 30 */
                /* WSS 525 no effect for  VBI2_WSS_DC_SEL */
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_DC_SEL)); /* bit 29*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_INIT_SEL)); /* bit 28*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_RUNIN_EN)); /* bit 27*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_CRISEL)); /* bit 26*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_STSEL)); /* bit 25*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_STCHK_SEL)); /* bit 23*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_BSTART_SEL)); /* bit 15*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_STRICT_RUNIN)); /* bit 14*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_BB_SEL)); /* bit 13*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_F1_SEL)); /* bit 12*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_LERR_EN)); /* bit 11*/
                vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_IGNORE_AMBI)); /* bit 10*/
                vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_LPF_EN)); /* bit 30*/                
                vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_LPF_SEL)); /* bit 24*/
                /* VBI Input Mode setting */
                vIO32WriteFldAlign(VBI2_01, VBI_IMODE_NTSC, VBI2_IMODE); 
                /* VBI CC clock run-in status setting */
                vIO32WriteFldAlign(VBI2_01, 1, VBI2_CNI0_STA_SEL);
                /* Adatpive ZC Setting */
                vIO32WriteFldAlign(VBI2_0D, 0, VBI_ADAPTIVE_INIT_ZC);
				
                #if SUPPORT_TYPE_B_VBI
                vIO32WriteFldAlign(VBI2_TYPE_B_00, 0, VBI2_B_LSC_MSB); 
                vIO32WriteFldAlign(VBI2_TYPE_B_00, 0, VBI2_B_HD_DC_SEL);
                vIO32WriteFldAlign(VBI2_TYPE_B_00, 0, VBI2_B_HD_INIT_SEL);
                
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0, VBI2_B_HD_RUNIN_EN);                
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 1, VBI2_B_HD_LPF_EN); 
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0, VBI2_B_HD_LPF_SEL);                                                          	                                            	            
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0, VBI2_B_HD_DRAM_EN);
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 3, VBI2_B_HD_SAMPLE_SEL);
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0x21, VBI2_B_HD_LEN);                 
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0x3b, VBI2_B_STC_END);
                vIO32WriteFldAlign(VBI2_TYPE_B_01, 0x40, VBI2_B_STC);
				
                vHwVbiSetTypeBWSSFreq(VBI2, 0x1400);
                #endif
                break;
              
          case VBI_PAL_MODE:
              /* PAL, TT 625B and WSS 625 */
              /* CC setting */
              vIO32WriteFldAlign(VBI2_05, 0, VBI2_CC_DRAM_EN);
              vIO32WriteFldAlign(VBI2_05, 114, VBI2_CC_STC_END);
              vIO32WriteFldAlign(VBI2_06, 0x9000, VBI2_CC_FREQ);
              vIO32WriteFldAlign(VBI2_05, 1, VBI2_CC_LPF_EN);
              
              /* VPS setting */
              vIO32WriteFldAlign(VBI2_07, 0, VBI2_VPS_DRAM_EN);

              /* WSS setting */
              vIO32WriteFldAlign(VBI2_09, 0, VBI2_WSS_DRAM_EN);
              vIO32WriteFldAlign(VBI2_09, 75, VBI2_WSS_STC_END);
              vIO32WriteFldAlign(VBI2_0A, 248, VBI2_WSS_STCODE);
              vIO32WriteFldAlign(VBI2_0A, 3, VBI2_WSS_LEN);
              vIO32WriteFldAlign(VBI2_0A, 0x1CCD, VBI2_WSS_FREQ);
              
              vIO32WriteFldAlign(VBI2_09, 0, VBI2_WSS_LPF_EN); /* bit 30 */
              /* Recommand: VBI_WSS_DC_SEL = 1 */
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_DC_SEL)); /* bit 29*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_INIT_SEL)); /* bit 28*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_RUNIN_EN)); /* bit 27*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_CRISEL)); /* bit 26*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_STSEL)); /* bit 25*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_STCHK_SEL)); /* bit 23*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_BSTART_SEL)); /* bit 15*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_STRICT_RUNIN)); /* bit 14*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_BB_SEL)); /* bit 13*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_F1_SEL)); /* bit 12*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_LERR_EN)); /* bit 11*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_IGNORE_AMBI)); /* bit 10*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_LPF_EN)); /* bit 30*/                
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_LPF_SEL)); /* bit 24*/

              /* TTX setting */              
              vIO32WriteFldAlign(VBI2_0B, 57, VBI2_TT_STC_END);
              vIO32WriteFldAlign(VBI2_0C, 39, VBI2_TT_STCODE);
              vIO32WriteFldAlign(VBI2_0C, 83, VBI2_TT_LEN);
              vIO32WriteFldAlign(VBI2_0C, 0x14C2, VBI2_TT_FREQ);
              vIO32WriteFldAlign(VBI2_00, 1, VBI2_TT_ECC_EN);
              vIO32WriteFldAlign(VBI2_00, 1, VBI2_TT_ECC_SEL);                                  

              /* VBI Input Mode setting */
              vIO32WriteFldAlign(VBI2_01, VBI_IMODE_PALN, VBI2_IMODE);               
              /* Adatpive ZC Setting */
              vIO32WriteFldAlign(VBI2_0D, 1, VBI_ADAPTIVE_INIT_ZC);
              break;
              
          case VBI_CNI_MODE:
            
              /* PAL, TT 625B, WSS 625 and VPS */
              /* CC setting */
              vIO32WriteFldAlign(VBI2_05, 0, VBI2_CC_DRAM_EN);
              vIO32WriteFldAlign(VBI2_05, 114, VBI2_CC_STC_END);
              vIO32WriteFldAlign(VBI2_06, 0x9000, VBI2_CC_FREQ);
              vIO32WriteFldAlign(VBI2_05, 1, VBI2_CC_LPF_EN);
              
              /* VPS setting */
              vIO32WriteFldAlign(VBI2_07, 0, VBI2_VPS_DRAM_EN);
              vIO32WriteFldAlign(VBI2_07, 70, VBI2_VPS_STC_END);
              vIO32WriteFldAlign(VBI2_08, 0x1CCD, VBI2_VPS_FREQ);

              /* WSS setting */	            
              vIO32WriteFldAlign(VBI2_09, 0, VBI2_WSS_DRAM_EN);
              vIO32WriteFldAlign(VBI2_09, 75, VBI2_WSS_STC_END);
              vIO32WriteFldAlign(VBI2_0A, 248, VBI2_WSS_STCODE);
              vIO32WriteFldAlign(VBI2_0A, 3, VBI2_WSS_LEN);
              vIO32WriteFldAlign(VBI2_0A, 0x1CCD, VBI2_WSS_FREQ);
              
              vIO32WriteFldAlign(VBI2_09, 0, VBI2_WSS_LPF_EN); /* bit 30 */
              /* Recommand: VBI_WSS_DC_SEL = 1 */
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_DC_SEL)); /* bit 29*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_INIT_SEL)); /* bit 28*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS_RUNIN_EN)); /* bit 27*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_CRISEL)); /* bit 26*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_STSEL)); /* bit 25*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_STCHK_SEL)); /* bit 23*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_BSTART_SEL)); /* bit 15*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS625_STRICT_RUNIN)); /* bit 14*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_BB_SEL)); /* bit 13*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_F1_SEL)); /* bit 12*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_LERR_EN)); /* bit 11*/
              vIO32WriteFldAlign(VBI2_09, 1, (VBI2_WSS625_IGNORE_AMBI)); /* bit 10*/
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_LPF_EN)); /* bit 30*/                
              vIO32WriteFldAlign(VBI2_09, 0, (VBI2_WSS_LPF_SEL)); /* bit 24*/
              
              /* TTX setting */
              vIO32WriteFldAlign(VBI2_0B, 57, VBI2_TT_STC_END);
              vIO32WriteFldAlign(VBI2_0C, 39, VBI2_TT_STCODE);
              vIO32WriteFldAlign(VBI2_0C, 83, VBI2_TT_LEN);
              vIO32WriteFldAlign(VBI2_0C, 0x14C2, VBI2_TT_FREQ);
              vIO32WriteFldAlign(VBI2_00, 1, VBI2_TT_ECC_EN);
              vIO32WriteFldAlign(VBI2_00, 1, VBI2_TT_ECC_SEL);      

              /* Recover setting from P-Scan HDTV mode */
              vIO32WriteFldAlign(VBI2_03, AV_EU_LSC, VBI2_LSC);
              vIO32WriteFldAlign(VBI2_00, AV_ZC_TH, VBI2_ZC_THR);                
              vIO32WriteFldAlign(VBI2_01, 0, VBI2_PSCAN);               

              /* VBI Input Mode setting */
              vIO32WriteFldAlign(VBI2_01, VBI_IMODE_PALN, VBI2_IMODE);               
              /* Adatpive ZC Setting */
              vIO32WriteFldAlign(VBI2_0D, 1, VBI_ADAPTIVE_INIT_ZC);
              break;
              
          default:
              break;
        }

        //vIO32WriteFldAlign(VBI2_00, 2, VBI2_PE_GAIN);
        vIO32WriteFldAlign(VBI2_01, 1, VBI2_STC_ERR);
        vIO32WriteFldAlign(VBI2_00, 5, VBI2_PROBLEM_TH);
        vIO32WriteFldAlign(VBI2_01, 0, VBI2_ECC_SEL2);
        vIO32WriteFldAlign(VBI2_01, 0, VBI2_ECC_SEL3);
        vIO32WriteFldAlign(VBI2_01, 1, VBI2_TT_LINE_INFO);
        vHwVbiSetInitZC(VBI2, 1);
        vIO32WriteFldAlign(VBI2_0A, 0, VBI2_WSS_PSCAN); /* 3x down-sample */
		vIO32WriteFldMulti(VBI2_0D,  P_Fld(0,VBI2_TT_FAST_ZC)|
                                    P_Fld(1,VBI2_YEDGE_EN)|
                                    P_Fld(0,VBI2_WAND_YC)|
                                    P_Fld(4,VBI_BLANK_ZC_THR));
        
    }
}

#if AUTO_CC_DETECT
/* brief: Set VBI paramter according to input source type or specific application
 * param: bVbiMode: VBI_NTSC_MODE / VBI_PAL_MODE
 *  VBI_NTSC_MODE: general setting for NTSC input (CC/WSS)
 *  VBI_PAL_MODE: general setting for PAL input (TT/WSS)
 * retval    None
*/
void vHwVbiSetVbiModeForAutoCC(UINT8 bVbiMode)
{
    vHwVbiSetSrc(VBI2, VBI_VSRC_TVD3D);
    switch (bVbiMode) 
    {
        case VBI_NTSC_MODE:
            /* CC setting */
            vIO32WriteFldAlign(VBI2_05, 0, VBI2_CC_DRAM_EN);
            vIO32WriteFldAlign(VBI2_05, 114, VBI2_CC_STC_END);
            vIO32WriteFldAlign(VBI2_06, 0x8F00, VBI2_CC_FREQ);
            vIO32WriteFldAlign(VBI2_05, 1, VBI2_CC_LPF_EN);
            
            /* VBI Input Mode setting */
            vIO32WriteFldAlign(VBI2_01, VBI_IMODE_NTSC, VBI2_IMODE); 
            /* VBI CC clock run-in status setting */
            vIO32WriteFldAlign(VBI2_01, 1, VBI2_CNI0_STA_SEL);
            /* Adatpive ZC Setting */
            vIO32WriteFldAlign(VBI2_0D, 0, VBI_ADAPTIVE_INIT_ZC);
            break;
            
        case VBI_PAL_MODE:
            /* PAL, TT 625B and WSS 625 */
            /* CC setting */
            vIO32WriteFldAlign(VBI2_05, 0, VBI2_CC_DRAM_EN);
            vIO32WriteFldAlign(VBI2_05, 114, VBI2_CC_STC_END);
            vIO32WriteFldAlign(VBI2_06, 0x9000, VBI2_CC_FREQ);
            vIO32WriteFldAlign(VBI2_05, 1, VBI2_CC_LPF_EN);

            /* VBI Input Mode setting */
            vIO32WriteFldAlign(VBI2_01, VBI_IMODE_PALN, VBI2_IMODE);               
            /* Adatpive ZC Setting */
            vIO32WriteFldAlign(VBI2_0D, 1, VBI_ADAPTIVE_INIT_ZC);
            break;          
    }

    //vIO32WriteFldAlign(VBI2_00, 2, VBI2_PE_GAIN);
    vIO32WriteFldAlign(VBI2_01, 1, VBI2_STC_ERR);
    vIO32WriteFldAlign(VBI2_00, 5, VBI2_PROBLEM_TH);
    vIO32WriteFldAlign(VBI2_01, 0, VBI2_ECC_SEL2);
    vIO32WriteFldAlign(VBI2_01, 0, VBI2_ECC_SEL3);
    vIO32WriteFldAlign(VBI2_01, 1, VBI2_TT_LINE_INFO);
    vHwVbiSetInitZC(VBI2, 1);
    vIO32WriteFldAlign(VBI2_0A, 0, VBI2_WSS_PSCAN); /* 3x down-sample */
    vIO32WriteFldMulti(VBI2_0D,   P_Fld(0,VBI2_TT_FAST_ZC)|
                                                    P_Fld(1,VBI2_YEDGE_EN)|
                                                    P_Fld(0,VBI2_WAND_YC)|
                                                    P_Fld(4,VBI_BLANK_ZC_THR));
                                                    
}
void vHwVbiGetCCLine(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI_05, VBI_CC_START0); 
            *pu1End = IO32ReadFldAlign(VBI_05, VBI_CC_END0);             
        }
        else
        {
            /* Field 1 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI_05, VBI_CC_START1); 
            *pu1End = IO32ReadFldAlign(VBI_05, VBI_CC_END1);             
        }
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_05, VBI2_CC_START0); 
            *pu1End = IO32ReadFldAlign(VBI2_05, VBI2_CC_END0);
        }
        else
        {
            /* Field 1 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_05, VBI2_CC_START1); 
            *pu1End = IO32ReadFldAlign(VBI2_05, VBI2_CC_END1);              
        }
    }
    #if TVD_SERRCNT_WA
    if(_sbSerrCntAdjEn==TRUE && IO32ReadFldAlign(TG_07, SERR_TYPE)==1)
    {
        *pu1Start = (*pu1Start)+1;
        *pu1End = (*pu1End)+1;
    }
    #endif
}
#endif

void vHwVbiSetCCLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel)
{
    #if TVD_SERRCNT_WA
    if(_sbSerrCntAdjEn==TRUE && IO32ReadFldAlign(TG_07, SERR_TYPE)==1)
    {
        u1F0LineSel--;
        u1F1LineSel--;
    }
    #endif
    
    if (bVBISuite == VBI0)
    {
        /* Field 0 CC line selection */
        vIO32WriteFldAlign(VBI_05, u1F0LineSel, VBI_CC_START0); 
        vIO32WriteFldAlign(VBI_05, u1F0LineSel, VBI_CC_END0); 
        
        /* Field 1 CC line selection */
        vIO32WriteFldAlign(VBI_05, u1F1LineSel, VBI_CC_START1); 
        vIO32WriteFldAlign(VBI_05, u1F1LineSel, VBI_CC_END1); 
    }
    else
    {
        /* Field 0 CC line selection */
        vIO32WriteFldAlign(VBI2_05, u1F0LineSel, VBI2_CC_START0);
        vIO32WriteFldAlign(VBI2_05, u1F0LineSel, VBI2_CC_END0);       
        
        /* Field 1 CC line selection */
        vIO32WriteFldAlign(VBI2_05, u1F1LineSel, VBI2_CC_START1);
        vIO32WriteFldAlign(VBI2_05, u1F1LineSel, VBI2_CC_END1);        
    }
}

void vHwVbiSetWSSLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel)
{
    #if TVD_SERRCNT_WA
    if(_sbSerrCntAdjEn==TRUE && IO32ReadFldAlign(TG_07, SERR_TYPE)==1)
    {
        u1F0LineSel--;
        u1F1LineSel--;
    }
    #endif
    
    if (bVBISuite == VBI0)
    {
        /* Field 0 WSS line selection */
        vIO32WriteFldAlign(VBI_09, u1F0LineSel, VBI_WSS_SEL0);        
        
        /* Field 1 WSS line selection */
        vIO32WriteFldAlign(VBI_09, u1F1LineSel, VBI_WSS_SEL1);             
    }
    else
    {
        /* Field 0 WSS line selection */
        vIO32WriteFldAlign(VBI2_09, u1F0LineSel, VBI2_WSS_SEL0);            
        
        /* Field 1 WSS line selection */
        vIO32WriteFldAlign(VBI2_09, u1F1LineSel, VBI2_WSS_SEL1);           
    }
}

#if SUPPORT_TYPE_B_VBI
void vHwVbiSetTypeBWSSLine(UINT8 bVBISuite, UINT8 u1F0LineSel, UINT8 u1F1LineSel)
{
    if (bVBISuite == VBI0)
    {
        /* Field 0 WSS line selection */
        vIO32WriteFldAlign(VBI_TYPE_B_00, u1F0LineSel, VBI_B_HD_LINE_SEL0);        
        
        /* Field 1 WSS line selection */
        vIO32WriteFldAlign(VBI_TYPE_B_00, u1F1LineSel, VBI_B_HD_LINE_SEL1);             
    }
    else
    {
        /* Field 0 WSS line selection */
        vIO32WriteFldAlign(VBI2_TYPE_B_00, u1F0LineSel, VBI2_B_HD_LINE_SEL0);            
        
        /* Field 1 WSS line selection */
        vIO32WriteFldAlign(VBI2_TYPE_B_00, u1F1LineSel, VBI2_B_HD_LINE_SEL1);           
    }
}
#endif

void vHwVbiSetVPSInterval(UINT8 bVBISuite, UINT8 u1F0Start, UINT8 u1F0End)
{
    if (bVBISuite == VBI0)
    {
        /* Field 0 VPS interval start */
        vIO32WriteFldAlign(VBI_07, u1F0Start, VBI2_VPS_START0);
        /* Field 0 VPS interval end */
        vIO32WriteFldAlign(VBI_07, u1F0End, VBI2_VPS_END0);
        /* Field 1 VPS interval start */
        vIO32WriteFldAlign(VBI_07, u1F0Start, VBI2_VPS_START1);
        /* Field 1 VPS interval end */
        vIO32WriteFldAlign(VBI_07, u1F0End, VBI2_VPS_END1);            
    }
    else
    {
        /* Field 0 VPS interval start */
        vIO32WriteFldAlign(VBI2_07, u1F0Start, VBI2_VPS_START0);
        /* Field 0 VPS interval end */
        vIO32WriteFldAlign(VBI2_07, u1F0End, VBI2_VPS_END0);
        /* Field 1 VPS interval start */
        vIO32WriteFldAlign(VBI2_07, u1F0Start, VBI2_VPS_START1);
        /* Field 1 VPS interval end */
        vIO32WriteFldAlign(VBI2_07, u1F0End, VBI2_VPS_END1);         
    }
}



void vHwVbiSetTTInterval0(UINT8 bVBISuite, UINT8 u1F0Start, UINT8 u1F0End)
{
    if (bVBISuite == VBI0)
    {
        /* Field 0 TTX interval start */
        vIO32WriteFldAlign(VBI_0B, u1F0Start, VBI_TT_START0);             
        
        /* Field 0 TTX interval end */
        vIO32WriteFldAlign(VBI_0B, u1F0End, VBI_TT_END0);          
    }
    else
    {
        /* Field 0 TTX interval start */
        vIO32WriteFldAlign(VBI2_0B, u1F0Start, VBI2_TT_START0);         
        
        /* Field 0 TTX interval end */
        vIO32WriteFldAlign(VBI2_0B, u1F0End, VBI2_TT_END0);           
    }
}

void vHwVbiSetTTInterval1(UINT8 bVBISuite, UINT8 u1F1Start, UINT8 u1F1End)
{
    if (bVBISuite == VBI0)
    {
        /* Field 1 TTX interval start */
        vIO32WriteFldAlign(VBI_0B, u1F1Start, VBI_TT_START1);          
        
        /* Field 1 TTX interval end */
        vIO32WriteFldAlign(VBI_0B, u1F1End, VBI_TT_END1);          
    }
    else
    {
        /* Field 1 TTX interval start */
        vIO32WriteFldAlign(VBI2_0B, u1F1Start, VBI2_TT_START1);           
        
        /* Field 1 TTX interval end */
        vIO32WriteFldAlign(VBI2_0B, u1F1End, VBI2_TT_END1);        
    }
}

void vHwVbiHDTVSubSample(UINT8 u1Set)
{
    if (u1Set == 1)
    {
        vIO32WriteFldAlign(VBI_01, 1, VBI_HDTV_SYNC_DIV);  
        vIO32WriteFldAlign(VBI_01, 1, VBI_VGA_CKO_DIV2);   
    }
    else
    {
        vIO32WriteFldAlign(VBI_01, 0, VBI_HDTV_SYNC_DIV);  
        vIO32WriteFldAlign(VBI_01, 0, VBI_VGA_CKO_DIV2);           
    }
}

void vHwVbiHVPosAdjust(UINT8 u1Set)
{
    if (u1Set == 1)
    {
        vIO32WriteFldAlign(VBI_03, 1, VBI_HDTV_H_SEL);  
        vIO32WriteFldAlign(VBI_03, 1, VBI_HDTV_V_SEL);         
    }
    else
    {
        vIO32WriteFldAlign(VBI_03, 0, VBI_HDTV_H_SEL);  
        vIO32WriteFldAlign(VBI_03, 0, VBI_HDTV_V_SEL);  
    }
}

void vHwVbiCoreReset(UINT8 bVBISuite)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(SYS_00, 1, VBI_RST);
        vIO32WriteFldAlign(SYS_00, 0, VBI_RST);
    }
    else
    {
        vIO32WriteFldAlign(SYS_00, 1, VBI2_RST);
        vIO32WriteFldAlign(SYS_00, 0, VBI2_RST);
    }
}


/* Set VBI Gain, UNIT: 0.25, ex: 4 = 0.25*4 = 1*/
void vHwVbiSetVBIGain(UINT8 bVBISuite, UINT8 bVBIGain)
{
    if (bVBIGain > 0xF)
    {
        return;
    }

    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_0D, bVBIGain, VBI_TVD_GAIN);
    }
}


/* Set CC INIT SEL: 0: high bandwidth, 1:low bandwidth */
void vHwVbiSetCCInitSel(UINT8 bVBISuite, UINT8 u1Value)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_05, u1Value, VBI_CC_INIT_SEL);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_05, u1Value, VBI2_CC_INIT_SEL);
    }
}


void vHwVBISetCCECC(UINT8 u1OnOff)
{
    UINT32 u4ECCReg;
    u4ECCReg = IO32ReadFldAlign(VBI_00, VBI_CC_ECC_EN);

    if (u1OnOff)
    {
        if (u4ECCReg == 0)
        {
            vIO32WriteFldAlign(VBI_00, 1, VBI_CC_ECC_EN);
        }
    }
    else
    {
        if (u4ECCReg) 
        {
            vIO32WriteFldAlign(VBI_00, 0, VBI_CC_ECC_EN);
        }
    }
}

/* Set VBI PE_Gain */
void vHwVbiSetPEGain(UINT8 bVBISuite, UINT8 bSet)
{
    if (bVBISuite == VBI0)
    {
        vIO32WriteFldAlign(VBI_00, bSet, VBI_PE_GAIN);
    }
    else
    {
        vIO32WriteFldAlign(VBI2_00, bSet, VBI2_PE_GAIN);
    }
}


#if VBI_CLI_SUPPORT
/* Get VBI DRAM output mode */
UINT32 u4HwVbiGetOutputMode(UINT8 bVBISuite)
{
    UINT32 u4Mode;
    
    if(bVBISuite == VBI0)
    {
        u4Mode = IO32ReadFldAlign(VBI_04, VBI_OMODE);          
    }
    else
    {
        u4Mode = IO32ReadFldAlign(VBI2_04, VBI2_OMODE);        
    }

    return u4Mode; 
}

/* Get VBI DRAM starting address with 16 bytes alignment */
UINT32 u4HwVbiGetMbase0(UINT8 bVBISuite)
{
    UINT32 u4BaseAddr;
    
    if (bVBISuite == VBI0)
    {
		u4BaseAddr = IO32ReadFldAlign(VBI_02, VBI_MBASE0) + (IO32ReadFldAlign(VBI_0E, VBI_MBASE01) << 25);
    }
    else
    {
		u4BaseAddr = IO32ReadFldAlign(VBI2_02, VBI2_MBASE0) + (IO32ReadFldAlign(VBI_0E, VBI2_MBASE01) << 25);
    }

    u4BaseAddr <<= 4; 

    return u4BaseAddr;
}

/* Get VBI DRAM FIFO length with 16 bytes alignment */
UINT32 u4HwVbiGetMOffset(UINT8 bVBISuite)
{
    UINT32 u4Offset;
        
    if (bVBISuite == VBI0)
    {
		u4Offset = IO32ReadFldAlign(VBI_04, VBI_MOFFSET) + (IO32ReadFldAlign(VBI_0E, VBI_MOFFSET1) << 25);
    }
    else
    {
		u4Offset = IO32ReadFldAlign(VBI2_04, VBI2_MOFFSET) + (IO32ReadFldAlign(VBI_0E, VBI2_MOFFSET1) << 25);
    }

    u4Offset <<= 4; 

    return u4Offset;    
}

UINT32 u4HwVbiGetLineEnable(UINT8 bVBISuite)
{
    UINT32 EnLine = 0;
    
    if (bVBISuite == VBI0)
    {
        if (IO32ReadFldAlign(VBI_00, VBI_TT_EN)) EnLine |= TT_LINE_EN; 
        if (IO32ReadFldAlign(VBI_00, VBI_WSS_EN)) EnLine |= WSS_LINE_EN; 
        if (IO32ReadFldAlign(VBI_00, VBI_CC_EN)) EnLine |= CC_LINE_EN; 
        if (IO32ReadFldAlign(VBI_00, VBI_VPS_EN)) EnLine |= VPS_LINE_EN; 
    }
    else
    {
        if (IO32ReadFldAlign(VBI2_00, VBI2_TT_EN)) EnLine |= TT_LINE_EN; 
        if (IO32ReadFldAlign(VBI2_00, VBI2_WSS_EN)) EnLine |= WSS_LINE_EN; 
        if (IO32ReadFldAlign(VBI2_00, VBI2_CC_EN)) EnLine |= CC_LINE_EN; 
        if (IO32ReadFldAlign(VBI2_00, VBI2_VPS_EN)) EnLine |= VPS_LINE_EN; 
    }

    return EnLine;
}

UINT32 vHwVbiGetLSC(UINT8 bVBISuite)
{   
    UINT32 u4LSC;
    
    if (bVBISuite == VBI0)
    {
        u4LSC = IO32ReadFldAlign(VBI_03, VBI_LSC);        
    }
    else
    {
        u4LSC = IO32ReadFldAlign(VBI2_03, VBI2_LSC);          
    }

    return u4LSC;
}

void vHwVbiSetCCLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 CC line selection */
            vIO32WriteFldAlign(VBI_05, u1Start, VBI_CC_START0); 
            vIO32WriteFldAlign(VBI_05, u1End, VBI_CC_END0);             
        }
        else
        {
            /* Field 1 CC line selection */
            vIO32WriteFldAlign(VBI_05, u1Start, VBI_CC_START1); 
            vIO32WriteFldAlign(VBI_05, u1End, VBI_CC_END1);             
        }
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 CC line selection */
            vIO32WriteFldAlign(VBI2_05, u1Start, VBI2_CC_START0);
            vIO32WriteFldAlign(VBI2_05, u1End, VBI2_CC_END0);
        }
        else
        {
            /* Field 1 CC line selection */
            vIO32WriteFldAlign(VBI2_05, u1Start, VBI2_CC_START1);
            vIO32WriteFldAlign(VBI2_05, u1End, VBI2_CC_END1);  
        }
    }
}

void vHwVbiGetCCLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI_05, VBI_CC_START0); 
            *pu1End = IO32ReadFldAlign(VBI_05, VBI_CC_END0);             
        }
        else
        {
            /* Field 1 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI_05, VBI_CC_START1); 
            *pu1End = IO32ReadFldAlign(VBI_05, VBI_CC_END1);             
        }
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_05, VBI2_CC_START0); 
            *pu1End = IO32ReadFldAlign(VBI2_05, VBI2_CC_END0);
        }
        else
        {
            /* Field 1 CC line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_05, VBI2_CC_START1); 
            *pu1End = IO32ReadFldAlign(VBI2_05, VBI2_CC_END1);              
        }
    }
}

void vHwVbiSetWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 WSS line selection */
            vIO32WriteFldAlign(VBI_09, u1Line, VBI_WSS_SEL0);                   
        }
        else
        {
            /* Field 1 WSS line selection */
            vIO32WriteFldAlign(VBI_09, u1Line, VBI_WSS_SEL1);
        }
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 WSS line selection */
            vIO32WriteFldAlign(VBI2_09, u1Line, VBI2_WSS_SEL0);            
        }
        else
        {
            /* Field 1 WSS line selection */
            vIO32WriteFldAlign(VBI2_09, u1Line, VBI2_WSS_SEL1);            
        }
    }
}

void vHwVbiGetWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI_09, VBI_WSS_SEL0);                       
        }
        else
        {
            /* Field 1 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI_09, VBI_WSS_SEL1);                
        }
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI2_09, VBI2_WSS_SEL0);              
        }
        else
        {
            /* Field 1 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI2_09, VBI2_WSS_SEL1);               
        }
    }
}

#if SUPPORT_TYPE_B_VBI
void vHwVbiSetTypeBWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Line)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 WSS line selection */
            vIO32WriteFldAlign(VBI_TYPE_B_00, u1Line, VBI_B_HD_LINE_SEL0);                   
        }
        else
        {
            /* Field 1 WSS line selection */
            vIO32WriteFldAlign(VBI_TYPE_B_00, u1Line, VBI_B_HD_LINE_SEL1);
        }         
    }
    else
    {
        if (u1Field == 0)
        {    
            /* Field 0 WSS line selection */
            vIO32WriteFldAlign(VBI2_TYPE_B_00, u1Line, VBI2_B_HD_LINE_SEL0);            
        }
        else
        {
            /* Field 1 WSS line selection */
            vIO32WriteFldAlign(VBI2_TYPE_B_00, u1Line, VBI2_B_HD_LINE_SEL1);            
        }        
    }
}

void vHwVbiGetTypeBWSSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Line)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 0)
        {
            /* Field 0 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI_TYPE_B_00, VBI_B_HD_LINE_SEL0);                       
        }
        else
        {
            /* Field 1 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI_TYPE_B_00, VBI_B_HD_LINE_SEL1);                
        }
    }
    else
    {
        if (u1Field == 0)
        {
            /* Field 0 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI2_TYPE_B_00, VBI2_B_HD_LINE_SEL0);                       
        }
        else
        {
            /* Field 1 WSS line selection */
            *pu1Line = IO32ReadFldAlign(VBI2_TYPE_B_00, VBI2_B_HD_LINE_SEL1);                
        }       
    }
}
#endif

void vHwVbiGetTTXLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 1)
        {
            /* Field 1 TTX line selection */
            *pu1Start = IO32ReadFldAlign(VBI_0B, VBI_TT_START1); 
            *pu1End = IO32ReadFldAlign(VBI_0B, VBI_TT_END1); 
        }
        else
        {
            /* Field 0 TTX line selection */
            *pu1Start = IO32ReadFldAlign(VBI_0B, VBI_TT_START0); 
            *pu1End = IO32ReadFldAlign(VBI_0B, VBI_TT_END0);             
        }
    }
    else
    {
        if (u1Field == 1)
        {
            /* Field 1 TTX line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_0B, VBI2_TT_START1); 
            *pu1End = IO32ReadFldAlign(VBI2_0B, VBI2_TT_END1);             
        }
        else
        {
            /* Field 0 TTX line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_0B, VBI2_TT_START0); 
            *pu1End = IO32ReadFldAlign(VBI2_0B, VBI2_TT_END0);              
        }
    }
}

void vHwVbiGetVPSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8* pu1Start, UINT8* pu1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 1)
        {
            /* Field 1 VPS line selection */
            *pu1Start = IO32ReadFldAlign(VBI_07, VBI_VPS_START1);
            *pu1End = IO32ReadFldAlign(VBI_07, VBI_VPS_END1);            
        }
        else
        {
            /* Field 0 VPS line selection */
            *pu1Start = IO32ReadFldAlign(VBI_07, VBI_VPS_START0);
            *pu1End = IO32ReadFldAlign(VBI_07, VBI_VPS_END0);             
        }
    }
    else
    {
        if (u1Field == 1)
        {
            /* Field 1 VPS line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_07, VBI2_VPS_START1);
            *pu1End = IO32ReadFldAlign(VBI2_07, VBI2_VPS_END1);              
        }
        else
        {
            /* Field 0 VPS line selection */
            *pu1Start = IO32ReadFldAlign(VBI2_07, VBI2_VPS_START0);
            *pu1End = IO32ReadFldAlign(VBI2_07, VBI2_VPS_END0);             
        }
    }
}

void vHwVbiSetVPSLineRange(UINT8 bVBISuite, UINT8 u1Field, UINT8 u1Start, UINT8 u1End)
{
    if (bVBISuite == VBI0)
    {
        if (u1Field == 1)
        {
            /* Field 0 CC line selection */
            vIO32WriteFldAlign(VBI_07, u1Start, VBI_VPS_START1); 
            vIO32WriteFldAlign(VBI_07, u1End, VBI_VPS_END1);
        }
        else
        {
            /* Field 1 CC line selection */
            vIO32WriteFldAlign(VBI_07, u1Start, VBI_VPS_START0); 
            vIO32WriteFldAlign(VBI_07, u1End, VBI_VPS_END0);            
        }
    }
    else
    {
        if (u1Field == 1)
        {
            /* Field 0 CC line selection */
            vIO32WriteFldAlign(VBI2_07, u1Start, VBI2_VPS_START1); 
            vIO32WriteFldAlign(VBI2_07, u1End, VBI2_VPS_END1);            
        }
        else
        {
            /* Field 1 CC line selection */
            vIO32WriteFldAlign(VBI2_07, u1Start, VBI2_VPS_START0); 
            vIO32WriteFldAlign(VBI2_07, u1End, VBI2_VPS_END0);             
        }
    }
}

void vHwVbiSetDbgPort(UINT32 u4Mode)
{
    UINT32 u4Reg;

    /* 0x20022410[7:4] = 9*/    
    u4Reg = *((UINT32 *)0x20022410);
    u4Reg &= 0xFFFFFF0F;
    u4Reg |= 0x00000090;
    *((UINT32 *)0x20022410) = u4Reg;
    //Need To Do ??
}

UINT32 vHwVBIReadReg(UINT32 u4VbiReg)
{
    return u4IO32Read4B(u4VbiReg);
}

void vHwVBIWriteReg(UINT32 u4VbiReg, UINT32 u4Value)
{
    vIO32WriteFldAlign(u4VbiReg, u4Value, Fld(32, 0, AC_FULLDW));
}

#endif //end of CLI_SUPPORT
