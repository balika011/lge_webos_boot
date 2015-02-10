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

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "x_os.h"	
#include "x_bim.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "nptv_debug.h"
#include "drv_pq_cust.h"

// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_ospe.h"
#include "hw_tcon.h"

// === DEFINE =============================================================================
#define SUPPORT_AMBILIGHTV2     1
#define AMBILIGHT_OFFSET1       0xC
#define AMBILIGHT_OFFSET2       0x8
#define USE_POST_GAMMA          SV_FALSE
#define UINT8_CLIP(val, min, max) ((UINT8)((val>=max) ? max : ((val<=min) ? min : val)))  
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================


// === GLOBAL VARIABLE ====================================================================
// Gamma
UINT16 u2GammaDecodedTable[3][GAMMA_INDEX];
UINT16 u2GammaZeroR, u2GammaZeroG, u2GammaZeroB;
UINT8 bGammaEnable;
UINT8 bMuteOnOff = SV_OFF;
UINT8 bxvYCCOnOff = SV_OFF;
UINT8 bxvYCCGammaOnOff = SV_OFF;
UINT8 bxvYCCAntiGammaOnOff = SV_OFF;
UINT16 u2xvYCCMatrix[9];

// Ambilight
#define AMBILIT_ROFFSET 0
#define AMBILIT_GOFFSET 640
#define AMBILIT_BOFFSET 1280

#if SUPPORT_AMBILIGHTV2
UINT16 u2AmbiLitArray[1920];
UINT8 u1AmbiLitBLKArray[640];
#endif

#ifndef NDEBUG    // PQ SRAM Stress Test Only Take Effect in Debug Build
UINT16 u2GammaReadBackTable[3][256];    
#endif

UINT8 bfgGammaKickPingPong = SV_FALSE;
UINT8 bfgIsGammaDecoding = SV_FALSE;

// move SRAM status flag to external, allows fast boot to check SRAM status
UINT8 bIsGammaSRAMInitialized = SV_FALSE;
UINT8 bIsGammaOn = SV_FALSE;    
    
// === EXTERN VARIABLE ====================================================================
EXTERN UINT8 bForceAllPQOff;

// === STATIC VARIABLE ====================================================================
// === BODY ===============================================================================

/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/

static void vDrvGammaSpline(UINT32 u4Count, INT32 *xx, INT32 *yy, INT32 *y)
{
    UINT32  i, j, jk;
    static INT32 h[64], v[64], d[64], l[64], z[64], u[64];
    static INT32 aa[64], bb[64], cc[64], dd[64];
    static INT32 a[64], b[64], c[64];
    INT32 x;
    
    for (i = 0; i <= u4Count; i++)
    {
        yy[i] = (yy[i]<<14);
    }
    
    for (i = 0; i < u4Count; i++)
    {
        h[i] = xx[i+1] - xx[i] ;
    }
    
    for (i = 1; i < u4Count; i++) 
    {
        if (h[i]*h[i-1] != 0)
        {
            v[i] = 6*(yy[i+1]-yy[i])/h[i]-6*(yy[i]-yy[i-1])/h[i-1];
        }

        a[i] = 2 * (h[i-1] + h[i]);
        b[i] = h[i-1];
        c[i] = h[i];
    }

    d[1] = a[1];    
    for (i = 2; i < u4Count; i++) 
    {
        if (d[i-1] != 0)
        {
            l[i] = (b[i]<<8)/d[i-1];
        }
        d[i] = a[i] - ((l[i] * c[i-1]+128)>>8);
    }

    z[1] = v[1];
    for (i = 2; i < u4Count; i++)
    {
        z[i] = v[i] - ((l[i] * z[i-1]+128)>>8);
    }
    
	u[0]  = 0;
	u[u4Count] = 0;
    if (d[u4Count-1] != 0)
    {
        u[u4Count-1] = z[u4Count-1]/d[u4Count-1] ;
    }
    
    for (i = (u4Count-2); i >= 1 ; i--) 
    { 
        if (d[i] != 0)
        {
            u[i] = (z[i] - c[i] * u[i+1]) / d[i] ;
        }
    }

    for (i = 0; i < u4Count; i++)
    {
        bb[i] = (u[i] / 2);
        dd[i] = yy[i] ;
        if ((xx[i + 1] - xx[i]) != 0) 
        {
            aa[i] = ((u[i+1]-u[i])/(xx[i+1]-xx[i]) / 6);
            cc[i] = ((yy[i+1]-yy[i])/(xx[i+1]-xx[i])-(xx[i+1]-xx[i])*(2*u[i]+u[i+1])/6);
        }
    }
    dd[u4Count] = yy[u4Count];
    
    for (i = 0; i < 256; i++)
    {
        x = i;
        jk = 0 ;

        for (j = 0; j <= (u4Count-1); j++) 
        {
            if ( (x >= xx[j]) && (x < xx[j+1]))
            {
                jk = j ;
            }
        }

        if (x >= xx[u4Count])
        {
            jk = u4Count;
        }

        y[i] = ((aa[jk] * ((x - xx[jk])*(x - xx[jk])*(x - xx[jk]))
               + bb[jk] * ((x - xx[jk])*(x - xx[jk]))
               + cc[jk] * (x - xx[jk]) + dd[jk] + 8192)>>14);
    }
}

void vDrvGammaInterpolation(UINT32 u4Count, INT32* iCtlPos, INT32* iCtlValue, INT32* OutputLut)
{
    UINT32 i;

    if (u4Count > 64)
    {
        Printf("Too many Control Point : %d > (Max 64 points)\n", u4Count);  
        return;
    }

    if (OutputLut == NULL)
    {
        Printf("Null output array\n");  
        return;
    }

#if 0   
    Printf("\n Control Point %d\n", u4Count);  
    for (i=0; i<u4Count; i++)
    {
        Printf(" %d\t %d \n", iCtlPos[i], iCtlValue[i]);  
    }
#endif
  
    vDrvGammaSpline(u4Count-1, iCtlPos, iCtlValue, OutputLut);
          
    OutputLut[256] = (OutputLut[255]*2) - OutputLut[254];

    for (i=0; i<=256; i++)
    {      
        OutputLut[i] = (OutputLut[i] < 0) ? 0 : (OutputLut[i]>4095) ? 4095 : OutputLut[i]; 
        
        if (i>0 && (OutputLut[i] < OutputLut[i-1]))
        {
            Printf("Value reverse !! Index : %d => %d < %d \n", i, OutputLut[i], OutputLut[i-1]);  
        }
    }                   

#if 0    
    Printf("\n Gamma Table \n", OutputLut[i]);  
    for (i=0; i<=256; i++)
    {
        Printf(" %d \n", OutputLut[i]);  
    }
#endif    
}


void vDrvSetOstgOptPos(UINT8 bOpt)
{
    vIO32WriteFldAlign(OSTG_01, bOpt, REG_OP_SEL);
}

void vDrvGammaOnOff(UINT8 bOnOff)
{
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_GAMMA);      
	vIO32WriteFldAlign(GAMMA_00, bModuleOnOff, C_GM_EN);
}

UINT8 u1DrvGetGammaOnOff(void)
{
    return IO32ReadFldAlign(GAMMA_00, C_GM_EN);
}

void vDrvGammaDecodeTable(void)
{
	UINT8 bValOld_R, bValOld_G, bValOld_B;
	UINT8  bFetch_R, bFetch_G, bFetch_B;
	UINT16 wValCur_R, wValCur_G, wValCur_B;
	UINT16 wIndex;

    // semaphore protection in case that DRAM was overwritten during ping-pong kick
    bfgIsGammaDecoding = SV_TRUE;

	bFetch_R = 0;
	bFetch_G = 0;
	bFetch_B = 0;

	wValCur_R = 0;
	wValCur_G = 0;
	wValCur_B = 0;

	bValOld_R = 0;
	bValOld_G = 0;
	bValOld_B = 0;

	bHiByte(wValCur_R) = 0;
	bHiByte(wValCur_G) = 0;
	bHiByte(wValCur_B) = 0;

	for (wIndex = 0; wIndex < GAMMA_INDEX; wIndex++)   //12-bits index
	{
		bFetch_R = GAMMA_256[0][wIndex];
		bFetch_G = GAMMA_256[1][wIndex];
		bFetch_B = GAMMA_256[2][wIndex];

		// decompress R value
		if (bValOld_R > bFetch_R)
		{
			++bHiByte(wValCur_R);
		}

		bLoByte(wValCur_R) = bFetch_R;
		bValOld_R= bFetch_R;

		// decompress G value
		if (bValOld_G > bFetch_G)
		{
			++bHiByte(wValCur_G);
		}

		bLoByte(wValCur_G) = bFetch_G;
		bValOld_G= bFetch_G;

		// decompress B value
		if (bValOld_B > bFetch_B)
		{
			++bHiByte(wValCur_B);
		}

		bLoByte(wValCur_B) = bFetch_B;
		bValOld_B= bFetch_B;
		
		u2GammaDecodedTable[0][wIndex] = wValCur_R;
		u2GammaDecodedTable[1][wIndex] = wValCur_G;
		u2GammaDecodedTable[2][wIndex] = wValCur_B;		
	}

    // semaphore unlock, decode table was ready
    bfgIsGammaDecoding = SV_FALSE;	
}

void  vDrvGammaKickPingPong(void)
{
    vIO32WriteFldAlign(GAMMA_06, 1, R_FILL_TABLE_READY);         // set ping-pong tick
    vIO32WriteFldAlign(GAMMA_06, 0, R_FILL_TABLE_READY);         // set ping-pong tick
}

void vDrvGammaWriteTable(void)
{
    UINT16 u2GammaIdx = 0;

    vIO32WriteFldAlign(GAMMA_00, SV_ON, R_FORCE_SRAM_CS_ON); 		// set write auto-incremental
    vIO32WriteFldAlign(GAMMA_00, 1, R_WR_AUTO_INC_ADDR); 		// set write auto-incremental
    vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0

    for (u2GammaIdx = 0; u2GammaIdx < 256; u2GammaIdx++)
    {
		vIO32WriteFldAlign(GAMMA_03, u2GammaDecodedTable[2][u2GammaIdx], R_WDATA_B); 		// B[11:8]
		vIO32WriteFldMulti(GAMMA_02,
						P_Fld(u2GammaDecodedTable[0][u2GammaIdx], R_WDATA_R) |
						P_Fld(u2GammaDecodedTable[1][u2GammaIdx], R_WDATA_G)); 		
    }											

    // write last point
    vIO32WriteFldAlign(GAMMA_09, MIN(0x1000, u2GammaDecodedTable[0][u2GammaIdx]), C_GM_256_R);
    vIO32WriteFldAlign(GAMMA_10, MIN(0x1000, u2GammaDecodedTable[1][u2GammaIdx]), C_GM_256_G);
    vIO32WriteFldAlign(GAMMA_10, MIN(0x1000, u2GammaDecodedTable[2][u2GammaIdx]), C_GM_256_B);

    vIO32WriteFldAlign(GAMMA_00, 0, R_WR_AUTO_INC_ADDR); 		// set write auto-incremental	
    vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0
}

void vDrvGammaReadTable(UINT16 u2GammaTable[3][257])
{
    UINT16 u2GammaIdx = 0;
    UINT32 u4TableRG;
    
    vDrvGammaKickPingPong();
	vIO32WriteFldAlign(GAMMA_00, 1, R_RD_AUTO_INC_ADDR); 		// set write auto-incremental
	vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0

    for (u2GammaIdx = 0; u2GammaIdx < 256; u2GammaIdx++)
    {
    	u4TableRG = u4IO32Read4B(GAMMA_04);
    	u2GammaTable[0][u2GammaIdx] = (UINT16)(u4TableRG & 0xFFF);
    	u2GammaTable[1][u2GammaIdx] = (UINT16)((u4TableRG >> 16) & 0xFFF);
		u2GammaTable[2][u2GammaIdx] = IO32ReadFldAlign(GAMMA_05, R_RDATA_B);        
    }

    // read last point
	u2GammaTable[0][u2GammaIdx] = IO32ReadFldAlign(GAMMA_09, C_GM_256_R);
	u2GammaTable[1][u2GammaIdx] = IO32ReadFldAlign(GAMMA_10, C_GM_256_G);
	u2GammaTable[2][u2GammaIdx] = IO32ReadFldAlign(GAMMA_10, C_GM_256_B);


	vIO32WriteFldAlign(GAMMA_00, 0, R_RD_AUTO_INC_ADDR); 		// set write auto-incremental	
	vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0
    vDrvGammaKickPingPong();
}

#ifndef NDEBUG    // PQ SRAM Stress Test Only Take Effect in Debug Build
UINT8 bDrvGammaVerifyTable(void)
{
    UINT16 u2GammaIdx = 0;
    UINT32 u4TableRG;
    UINT8 bRet = SV_TRUE;

    vIO32WriteFldAlign(GAMMA_00, 1, R_RD_AUTO_INC_ADDR); 		// set write auto-incremental
    vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0

    for (u2GammaIdx = 0; u2GammaIdx < 256; u2GammaIdx++)
    {
    	u4TableRG = u4IO32Read4B(GAMMA_04);
    	u2GammaReadBackTable[0][u2GammaIdx] = (UINT16)(u4TableRG & 0xFFF);
    	u2GammaReadBackTable[1][u2GammaIdx] = (UINT16)((u4TableRG >> 16) & 0xFFF);
	    u2GammaReadBackTable[2][u2GammaIdx] = IO32ReadFldAlign(GAMMA_05, R_RDATA_B);    	
    }

    for (u2GammaIdx = 0; u2GammaIdx < 256; u2GammaIdx++)
    {        
        if(u2GammaReadBackTable[0][u2GammaIdx] != u2GammaDecodedTable[0][u2GammaIdx])
        {
            Printf("[FAIL] GAMMA [R] [%d] : [W] = %d, [R] = %d\n", u2GammaIdx, u2GammaDecodedTable[0][u2GammaIdx], u2GammaReadBackTable[0][u2GammaIdx]);
            bRet = SV_FALSE;
        }
        if(u2GammaReadBackTable[1][u2GammaIdx] != u2GammaDecodedTable[1][u2GammaIdx])
        {
            Printf("[FAIL] GAMMA [G] [%d] : [W] = %d, [R] = %d\n", u2GammaIdx, u2GammaDecodedTable[1][u2GammaIdx], u2GammaReadBackTable[1][u2GammaIdx]);
            bRet = SV_FALSE;
        }
        if(u2GammaReadBackTable[2][u2GammaIdx] != u2GammaDecodedTable[2][u2GammaIdx])
        {
            Printf("[FAIL] GAMMA [B] [%d] : [W] = %d, [R] = %d\n", u2GammaIdx, u2GammaDecodedTable[2][u2GammaIdx], u2GammaReadBackTable[2][u2GammaIdx]);
            bRet = SV_FALSE;
        }        
    }
    
    vIO32WriteFldAlign(GAMMA_00, 0, R_RD_AUTO_INC_ADDR); 		// set write auto-incremental	
    vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set initial address to 0
    return bRet;
}
#endif // NDEBUG

void vDrvGammaMute(UINT16 wGammaMuteR, UINT16 wGammaMuteG, UINT16 wGammaMuteB)
{
    UINT32 u2GammaZeroRG;

    // 1. read gamma on/off status and back-up zero point    
	vIO32WriteFldAlign(GAMMA_00, 1, R_VA_READ_LATEST_WDATA); 	// set ink mode (video/cpu use the same ping-pong table)	
	vIO32WriteFldAlign(GAMMA_00, 0, R_RD_AUTO_INC_ADDR); 		// set read auto-incremental	
	vIO32WriteFldAlign(GAMMA_00, 0, R_WR_AUTO_INC_ADDR); 		// set write auto-incremental	
	vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set address to 0

    if(bMuteOnOff == SV_OFF)
    {
        u2GammaZeroRG = u4IO32Read4B(GAMMA_04);
        u2GammaZeroR = (UINT16)(u2GammaZeroRG & 0xFFF);
        u2GammaZeroG = (UINT16)((u2GammaZeroRG >> 16) & 0xFFF);
 	    u2GammaZeroB = IO32ReadFldAlign(GAMMA_05, R_RDATA_B);

        bGammaEnable = u1DrvGetGammaOnOff(); 	    
    }

	vDrvGammaOnOff(SV_ON);

    // 2. write mute color
	vIO32WriteFldAlign(GAMMA_00, 1, R_VA_READ_LATEST_WDATA); 	// set ink mode (video/cpu use the same ping-pong table)	
	vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set address to 0

	vIO32WriteFldAlign(GAMMA_03, wGammaMuteB, R_WDATA_B); 		// B[11:8]
	vIO32WriteFldMulti(GAMMA_02,
						P_Fld(wGammaMuteR, R_WDATA_R) |
						P_Fld(wGammaMuteG, R_WDATA_G)); 
	bMuteOnOff = SV_ON;
	
}

void vDrvGammaUnmute(void)
{
    if(bMuteOnOff == SV_ON)
    {
    	vIO32WriteFldAlign(GAMMA_01, 0, R_ADDR); 					// set address to 0

    	vIO32WriteFldAlign(GAMMA_03, u2GammaZeroB, R_WDATA_B); 		
    	vIO32WriteFldMulti(GAMMA_02,
    						P_Fld(u2GammaZeroR, R_WDATA_R) |
    						P_Fld(u2GammaZeroG, R_WDATA_G)); 		
    	
    	vIO32WriteFldAlign(GAMMA_00, 0, R_VA_READ_LATEST_WDATA); 	// set ink mode (video/cpu use the same ping-pong table)	
    	
    	vDrvGammaOnOff(bGammaEnable);

    	bMuteOnOff = SV_OFF;
    }
}

void vDrvSetGammaWritePingPongFlag(UINT8 bfgOn)
{
    bfgGammaKickPingPong = bfgOn;
}

UINT8 bDrvGetGammaPingPongFlag(void)
{
    // Gamma Ping Pong Could Only Be Set When (1) Vsync Flag Down and (2) Someone (AP / Driver) Set Gamma
    UINT8 bfgGammaTriggerEn = !IO32ReadFldAlign(GAMMA_06, TABLE_WAIT_VSYNC);
    
    return (bfgGammaKickPingPong && bfgGammaTriggerEn);
}

void vDrvGammaInitSRAM(void)
{
    bIsGammaSRAMInitialized = SV_FALSE;
    bIsGammaOn = SV_FALSE;    
}

void vDrvGammaProc(void)
{
    // ping-pong kick could only happen when decoded table was done.
    if(bfgIsGammaDecoding == SV_FALSE) 
    {
        if(bDrvGetGammaPingPongFlag() == SV_TRUE)
        {
            vDrvGammaWriteTable();       
            vDrvGammaKickPingPong();      

            bIsGammaSRAMInitialized = SV_TRUE;
        
            // Clear Write Ping Pong Flag
            vDrvSetGammaWritePingPongFlag(SV_FALSE);
        }
        else
        {
            // gamma could only be set to on after 
            // (1) SRAM ready
            // (2) ping-pong kick, and passed one vsync
            if((bIsGammaSRAMInitialized == SV_TRUE) && (bIsGammaOn == SV_FALSE))
            {
                vDrvGammaOnOff(SV_ON);         
                bIsGammaOn = SV_TRUE;
            }
        }
    }
}

/*****************************************************************************************/
/***************************************  xvYCC   ****************************************/
/*****************************************************************************************/
// Note : Only 5398 support xvYCC
void vDrvxvYCCOnOff(UINT8 bOnOff)
{
#ifdef CC_MT5399
    UINT8 bModuleOnOff = bOnOff && (!bForceAllPQOff) && wReadQualityTable(QUALITY_MODULE_ONOFF_XVYCC);  
    bxvYCCOnOff = bModuleOnOff;
    vApiRegisterVideoEvent(PE_EVENT_XVYCC, SV_VP_MAIN, bxvYCCOnOff);
#else
	LOG(0, "NOT support xvYCC\n");
#endif
}

void vDrvxvYCCGammaOnOff(UINT8 bOnOff)
{
#ifdef CC_MT5399
    bxvYCCGammaOnOff = bOnOff;
    vApiRegisterVideoEvent(PE_EVENT_XVYCC_GAMMA, SV_VP_MAIN, SV_ON);
#else
	LOG(0, "NOT support xvYCC\n");
#endif
}

void vDrvxvYCCAntiCGammaOnOff(UINT8 bOnOff)
{
#ifdef CC_MT5399
    bxvYCCAntiGammaOnOff = bOnOff;
    vApiRegisterVideoEvent(PE_EVENT_XVYCC_ANTIGAMMA, SV_VP_MAIN, SV_ON);
#else
	LOG(0, "NOT support xvYCC\n");
#endif    
}

void vDrvxvYCC3x3OnOff(UINT8 bOnOff)
{
#ifdef CC_MT5399
	vIO32WriteFldAlign(XVYCC_00, !bOnOff, REG_3X3_BYPASS);
#else
	LOG(0, "NOT support xvYCC\n");
#endif    
}

void vDrvxvYCCSetPath(UINT8 bPath)
{
#ifdef CC_MT5399
	vIO32WriteFldAlign(XVYCC_00, (bPath == SV_VP_MAIN) ? 0 : 1, REG_XVYCC_M_P_SW);
#else
	LOG(0, "NOT support xvYCC\n");
#endif    
}

void vDrvxvYCCLoad3x3Matrix(const UINT16 * pwTable)
{
#ifdef CC_MT5399
    x_memcpy(u2xvYCCMatrix, (UINT16*) pwTable, 9*sizeof(UINT16));
    vApiRegisterVideoEvent(PE_EVENT_XVYCC_MATRIX, SV_VP_MAIN, SV_ON);
#else
	LOG(0, "NOT support xvYCC\n");
#endif    
}


UINT8 bDrvGetxvYCCOnOff(void)
{
#ifdef CC_MT5399
	return (!IO32ReadFldAlign(XVYCC_00, REG_BYPASS_XVYCC_MAIN));
#else
	LOG(0, "NOT support xvYCC\n");
    return 0;
#endif    
}

void vDrvxvYCCSetClip(UINT8 bOnOff)
{
#ifdef CC_MT5399
    vIO32WriteFldAlign(OSTG_00, bOnOff, REG_XVYCC_CLIP_EN);
#else
	LOG(0, "NOT support xvYCC\n");
#endif    
}

/*****************************************************************************************/
/*********************************** GAIN / OFFSET ***************************************/
/*****************************************************************************************/
void vDrvSetRGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_00, wValue, R_GAIN_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_01, wValue, R_GAIN_PIP);
    }
}
   
void vDrvSetGGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_00, wValue, G_GAIN_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_01, wValue, G_GAIN_PIP);
    }
}

void vDrvSetBGain1(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_00, wValue, B_GAIN_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_01, wValue, B_GAIN_PIP);
    }
}

void vDrvSetROffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_02, wValue, R_OFST1_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_04, wValue, R_OFST1_PIP);
    }
}

void vDrvSetGOffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_02, wValue, G_OFST1_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_04, wValue, G_OFST1_PIP);
    }
}

void vDrvSetBOffset11(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_02, wValue, B_OFST1_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(GAINOSET_04, wValue, B_OFST1_PIP);
    }
}

void vDrvSetROffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_03, wValue, R_OFST2_MAIN);
    }
    else 
    {
        vIO32WriteFldAlign(GAINOSET_05, wValue, R_OFST2_PIP);
    }
}

void vDrvSetGOffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_03, wValue, G_OFST2_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(GAINOSET_05, wValue, G_OFST2_PIP);
    }
}

void vDrvSetBOffset12(UINT8 bPath, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(GAINOSET_03, wValue, B_OFST2_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(GAINOSET_05, wValue, B_OFST2_PIP);
    }
}

UINT16 wDrvGetRGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_00, R_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_01, R_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetRGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT16 wDrvGetGGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_00, G_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_01, G_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetGGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT16 wDrvGetBGain1(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_00, B_GAIN_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_01, B_GAIN_PIP);
    }
    else
    {
        LOG(0, "wDrvGetBGain1 Path Error!");
        ASSERT(0);
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetROffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_02, R_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_04, R_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetROffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_02, G_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_04, G_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetROffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetBOffset11(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_02, B_OFST1_MAIN) ;
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_04, B_OFST1_PIP) ;
    }
    else
    {
        LOG(0, "dwDrvGetBOffset11 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetROffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_03, R_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_05, R_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetROffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetGOffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_03, G_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_05, G_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetGOffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

UINT32 dwDrvGetBOffset12(UINT8 bPath)
{
    UINT32 dwReadOutVal = 0;

    if (bPath == SV_VP_MAIN)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_03, B_OFST2_MAIN);
    }
    else if (bPath == SV_VP_PIP)
    {
        dwReadOutVal = IO32ReadFldAlign(GAINOSET_05, B_OFST2_PIP);
    }
    else
    {
        LOG(0, "dwDrvGetBOffset12 Path Error!");
        ASSERT(0);
    }

    if (dwReadOutVal >> 9 == 0x1)
    {
        dwReadOutVal |= 0xFFFFFC00;
    }

    return dwReadOutVal;
}

/*****************************************************************************************/
/************************************ BLUE STRETCH ***************************************/
/*****************************************************************************************/
void vDrvSetBlueStretchOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(BLUESTCH_00, bOnOff, C_BS_EN_MAIN);
    }
    else
    {
        vIO32WriteFldAlign(BLUESTCH_00, bOnOff, C_BS_EN_PIP);
    }
}

void vDrvSetBlueStretchParam(void)
{
	return;
#if 0
    vIO32WriteFldMulti(BLUESTCH_00, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_LUMA_THD), C_BS_Y_THD) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_SAT_THD), C_BS_SAT_THD) |
                                    P_Fld(!wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN2_ENABLE), C_BS_ONE_GAIN_MODE) |
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_BASE_1), C_BS_BASE_1));
    vIO32WriteFldMulti(BLUESTCH_01, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R), C_BS_GAIN_R) | 
                                   P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G), C_BS_GAIN_G) | 
                                   P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B), C_BS_GAIN_B));
    vIO32WriteFldMulti(BLUESTCH_02, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R_1), C_BS_GAIN_R_1) | 
                                   P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G_1), C_BS_GAIN_G_1) | 
                                   P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B_1), C_BS_GAIN_B_1));    
#ifndef CC_MT5882
    vIO32WriteFldMulti(BLUESTCH_03, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R_2), C_BS_GAIN_R_2) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G_2), C_BS_GAIN_G_2) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B_2), C_BS_GAIN_B_2));                    
    vIO32WriteFldMulti(BLUESTCH_04, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R_3), C_BS_GAIN_R_3) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G_3), C_BS_GAIN_G_3) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B_3), C_BS_GAIN_B_3));                    
    vIO32WriteFldMulti(BLUESTCH_05, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R_4), C_BS_GAIN_R_4) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G_4), C_BS_GAIN_G_4) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B_4), C_BS_GAIN_B_4));                       
#endif                                    
    vDrvSetBlueStretchBase();
#endif
}

void vDrvSetBlueStretchBase(void)
{
    INT32 i4Val, i4Ofst = IO32ReadFldAlign(RGBOFFSET_00, R_OFFSET_MAIN);
    UINT32 u4PrevStageBase = 0;

    i4Ofst = ((i4Ofst & 0x400) ? (i4Ofst - 0x800) : (i4Ofst)) / 4;    // express in 8 bit
    i4Val = ((IO32ReadFldAlign(YCBCR2RGB_00, Y2R_MATRIX00) * IO32ReadFldAlign(BLUESTCH_00, C_BS_Y_THD) + 0x200) >> 10) + i4Ofst + 1;
    i4Val = UINT8_CLIP(i4Val, 0, 255);
    vIO32WriteFldAlign(BLUESTCH_01, i4Val, C_BS_BASE);
    vIO32WriteFldAlign(BLUESTCH_00, ((UINT32)IO32ReadFldAlign(BLUESTCH_00, C_BS_BASE_1)>i4Val) ? 
                        IO32ReadFldAlign(BLUESTCH_00, C_BS_BASE_1) : i4Val, C_BS_BASE_1);

    u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_00, C_BS_BASE_1);
    vIO32WriteFldAlign(BLUESTCH_03, ((UINT32)IO32ReadFldAlign(BLUESTCH_03, C_BS_BASE_2)>u4PrevStageBase) ? 
                    IO32ReadFldAlign(BLUESTCH_03, C_BS_BASE_2) : u4PrevStageBase, C_BS_BASE_2);
    u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_03, C_BS_BASE_2);
    vIO32WriteFldAlign(BLUESTCH_04, ((UINT32)IO32ReadFldAlign(BLUESTCH_04, C_BS_BASE_3)>u4PrevStageBase) ? 
                    IO32ReadFldAlign(BLUESTCH_04, C_BS_BASE_3) : u4PrevStageBase, C_BS_BASE_3);
    u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_04, C_BS_BASE_3);
    vIO32WriteFldAlign(BLUESTCH_05, ((UINT32)IO32ReadFldAlign(BLUESTCH_05, C_BS_BASE_4)>u4PrevStageBase) ? 
                    IO32ReadFldAlign(BLUESTCH_05, C_BS_BASE_4) : u4PrevStageBase, C_BS_BASE_4);
}


// Base0 is controlled by the relation between Y_THD and Matrix Coef
void vDrvSetBlueStretchBaseExt(UINT8 u1Idx, UINT8 u1BSBase)
{
    INT32 i4Val, i4Ofst = IO32ReadFldAlign(RGBOFFSET_00, R_OFFSET_MAIN);
    UINT32 u4PrevStageBase = 0;

    i4Ofst = ((i4Ofst & 0x400) ? (i4Ofst - 0x800) : (i4Ofst)) / 4;    // express in 8 bit
    i4Val = ((IO32ReadFldAlign(YCBCR2RGB_00, Y2R_MATRIX00) * IO32ReadFldAlign(BLUESTCH_00, C_BS_Y_THD) + 0x200) >> 10) + i4Ofst + 1;
    i4Val = UINT8_CLIP(i4Val, 0, 255);
    vIO32WriteFldAlign(BLUESTCH_01, i4Val, C_BS_BASE);

    switch (u1Idx)
    {
    case 1:
        u4PrevStageBase = i4Val;
        vIO32WriteFldAlign(BLUESTCH_00, ((UINT32)u1BSBase>u4PrevStageBase) ? 
                        u1BSBase : i4Val, C_BS_BASE_1);
        break;
    case 2:
        u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_00, C_BS_BASE_1);
        vIO32WriteFldAlign(BLUESTCH_03, ((UINT32)u1BSBase>u4PrevStageBase) ? 
                        u1BSBase : i4Val, C_BS_BASE_2);
        break;    
    case 3:
        u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_03, C_BS_BASE_2);
        vIO32WriteFldAlign(BLUESTCH_04, ((UINT32)u1BSBase>u4PrevStageBase) ? 
                        u1BSBase : i4Val, C_BS_BASE_3);
        break;    
    case 4:
        u4PrevStageBase = IO32ReadFldAlign(BLUESTCH_04, C_BS_BASE_3);
        vIO32WriteFldAlign(BLUESTCH_05, ((UINT32)u1BSBase>u4PrevStageBase) ? 
                        u1BSBase : i4Val, C_BS_BASE_4);
        break;            
    default:        
        break;
    }        
}


void vDrvSetBlueStretchParamExt(UINT8 u1Idx, UINT8 u1BSGainR, UINT8 u1BSGainG, UINT8 u1BSGainB)
{
    vIO32WriteFldMulti(BLUESTCH_00, P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_LUMA_THD), C_BS_Y_THD) | 
                                    P_Fld(wReadQualityTable(QUALITY_BLUE_STRETCH_SAT_THD), C_BS_SAT_THD) |
                                    P_Fld(!wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN2_ENABLE), C_BS_ONE_GAIN_MODE));
    switch (u1Idx)
    {
    case 0:
        vIO32WriteFldMulti(BLUESTCH_01, P_Fld(u1BSGainR, C_BS_GAIN_R) | 
                                        P_Fld(u1BSGainG, C_BS_GAIN_G) | 
                                        P_Fld(u1BSGainB, C_BS_GAIN_B));            
        break;
    case 1:
        vIO32WriteFldMulti(BLUESTCH_02, P_Fld(u1BSGainR, C_BS_GAIN_R_1) | 
                                        P_Fld(u1BSGainG, C_BS_GAIN_G_1) | 
                                        P_Fld(u1BSGainB, C_BS_GAIN_B_1));            
        break;
    case 2:
        vIO32WriteFldMulti(BLUESTCH_03, P_Fld(u1BSGainR, C_BS_GAIN_R_2) | 
                                        P_Fld(u1BSGainG, C_BS_GAIN_G_2) | 
                                        P_Fld(u1BSGainB, C_BS_GAIN_B_2));                    
        break;    
    case 3:
        vIO32WriteFldMulti(BLUESTCH_04, P_Fld(u1BSGainR, C_BS_GAIN_R_3) | 
                                        P_Fld(u1BSGainG, C_BS_GAIN_G_3) | 
                                        P_Fld(u1BSGainB, C_BS_GAIN_B_3));                    
        break;    
    case 4:
        vIO32WriteFldMulti(BLUESTCH_05, P_Fld(u1BSGainR, C_BS_GAIN_R_4) | 
                                        P_Fld(u1BSGainG, C_BS_GAIN_G_4) | 
                                        P_Fld(u1BSGainB, C_BS_GAIN_B_4));                    
        break;            
    default:        
        break;
    }
        
}

UINT8 bDrvGetBlueStretchOnOff(void)
{
	return (IO32ReadFldAlign(BLUESTCH_00, C_BS_EN_MAIN));
}

/**************************************************************************************/
/*********************************   AMBILIGHT V2   ***********************************/
/**************************************************************************************/
#if SUPPORT_AMBILIGHTV2

void vDrvAmbiLitSysInit(void)
{
    // Set System Info
    vIO32WriteFldAlign(AMBL_00_ES2, SV_ON, AMBI_SW_EN);
    vIO32WriteFldAlign(AMBL_00_ES2, 0, R_AMBI_START_BLKNUM); 
    vDrvAmbiLitSetMaskMode(0);

    // Set Panel Info, for FHD (1920x1080) Panel Case
    vIO32WriteFldAlign(AMBL_00_ES2, 4, R_AMBI_SHIFT_N);            
    vIO32WriteFldAlign(AMBL_01_ES2, 60, R_AMBI_BLK_W);    
    vIO32WriteFldAlign(AMBL_01_ES2, 54, R_AMBI_BLK_H);    
    vIO32WriteFldAlign(AMBL_01_ES2, 60, R_AMBI_BLK_W_LAST);    
    vIO32WriteFldAlign(AMBL_01_ES2, 54, R_AMBI_BLK_H_LAST);    
}

void vDrvAmbiLitSetMaskMode(UINT8 u1MaskMode)
{
    vIO32WriteFldAlign(AMBL_00_ES2, u1MaskMode, R_AMBI_INTR_MASK_MODE);
}

void vDrvAmbiLitUpdateLine(UINT16 * u2AmbiLitArray, UINT8 * u1AmbilBlk, UINT8 u1Line)
{
    UINT8 u1Block, u1CurBlock;
    UINT16 u2BlockResultR;
    UINT32 u4BlockResultGB;
    
    for (u1Block = 0; u1Block < 32; u1Block++)
    {
        u2BlockResultR = IO32ReadFldAlign(AMBL_02_ES2, AMBI_RESULT_R); // get R result
        u1CurBlock = IO32ReadFldAlign(AMBL_02_ES2, AMBI_BLKNUM);        
        u4BlockResultGB = u4IO32Read4B(AMBL_03_ES2);                   // get G/B result, and blk_num + 1 automatically after reading the register

        u2AmbiLitArray[AMBILIT_ROFFSET + u1Line*32 + u1Block] = u2BlockResultR;
        u2AmbiLitArray[AMBILIT_GOFFSET + u1Line*32 + u1Block] = (UINT16)(u4BlockResultGB & 0xFFFF);
        u2AmbiLitArray[AMBILIT_BOFFSET + u1Line*32 + u1Block] = (UINT16)((u4BlockResultGB>>16) & 0xFFFF);        

        // for debug
        u1AmbilBlk[u1Line*32 + u1Block] = u1CurBlock;

    }
}

UINT8 u1DrvAmbiLitGetStrapID(void)
{
    return IO32ReadFldAlign(AMBL_02_ES2, AMBI_STRAP_ID);
}

void vDrvAmbiLPrintContent(UINT8 u1Color)
{
    UINT8 u1Block, u1Line;

    if(u1Color < 3)
    {
        for (u1Line = 0; u1Line < 20; u1Line++)
        {
            Printf("{%d}\n", u1Line);
            for (u1Block = 0; u1Block < 32; u1Block++)
            {
                Printf("%d ", u2AmbiLitArray[u1Color*640 + u1Line*32 + u1Block]);
            }
            Printf("\n");
        }
    }
    else
    {
        Printf("R[0] / G[1] / B[2]\n");
    }
}

void _vDrvAmbiLISR(UINT16 u2Vector)
{
    UINT8 u1IntOn[2];   // 0 : Ambilight  1 : OSTG Line Interrupt
    UINT8 u1AmbiLitLineID=0;

    ASSERT(u2Vector == VECTOR_POST_PROC);  

    // Input Vsync Rising
    u1IntOn[0] = IO32ReadFldAlign(INTR_00, STATUS_INTR_AMBILIT);
    u1IntOn[1] = IO32ReadFldAlign(INTR_00, STATUS_INTR_OSTGLINE);

    if (u1IntOn[0]) // Ambilight ISR
    { 
        u1AmbiLitLineID = u1DrvAmbiLitGetStrapID();
        vDrvAmbiLitUpdateLine(u2AmbiLitArray, u1AmbiLitBLKArray, u1AmbiLitLineID);        
     
        vIO32WriteFldAlign(INTR_02, 0x1, C_INTR_CLEAR_AMBILIT); // Routine Done, Clear ISR
        vIO32WriteFldAlign(INTR_02, 0x0, C_INTR_CLEAR_AMBILIT); // Routine Done, Clear ISR        
    }
    
    if(u1IntOn[1]) // OSTG VSYNC ISR
    {
        vIO32WriteFldAlign(INTR_02, 0x1, C_INTR_CLEAR_OSTGLINE); // Routine Done, Clear ISR
        vIO32WriteFldAlign(INTR_02, 0x0, C_INTR_CLEAR_OSTGLINE); // Routine Done, Clear ISR        
    }

    VERIFY(BIM_ClearIrq(VECTOR_POST_PROC));
}

void vDrvAmbiLEnableISR(UINT8 bFlagEn)
{    
    vIO32WriteFldAlign(ARM_INTR2, bFlagEn, ARM_INTR_EN_1);     // Enable SPI ISR
//    vIO32WriteFldAlign(ARM_INTR2, 0, ARM_INTR_EN_2);     // Enable SPI ISR    
}

void vDrvInitAmbiLISR(void)
{
    static UINT8 u1AmbiLIsrInitialized = SV_FALSE;
    
    x_os_isr_fct pfnOldIsr;

    if (!u1AmbiLIsrInitialized)
    {
        vIO32WriteFldAlign(INTR_02, 0x3F, C_INTR_MASK); // Set Mask For Ambilight ISR
    
        if (x_reg_isr(VECTOR_POST_PROC, _vDrvAmbiLISR, &pfnOldIsr) != OSR_OK)     // Register ISR
        {
            Printf("= AmbiL Register ISR Error! =\n");
        }
        else
        {
            Printf("= AmbiL Register ISR OK! =\n");
        }

        u1AmbiLIsrInitialized = SV_TRUE;

        vDrvAmbiLEnableISR(SV_ON);        
    }
}

void vDrvAmbilightISRInit(void)
{
    vDrvAmbiLitSysInit();
    vDrvInitAmbiLISR();
}
#endif //#if SUPPORT_AMBILIGHTV2

