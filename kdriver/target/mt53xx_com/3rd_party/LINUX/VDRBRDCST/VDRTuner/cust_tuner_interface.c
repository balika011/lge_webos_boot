/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2008, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2015/01/09 $
 * $RCSfile: tuner_interface.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file tuner_interface.c
 *  Implement primitive tuner driver interface.
 */

#include "tuner_interface_if.h"
#include "x_typedef.h"
#include "cust_tunerType.h"
#include "tunerDebug.h"
#include "PI_Def_dbg.h"

typedef struct 
{
    UINT8                id;
    ITUNER_FCT_T*       fct;
} CUST_TUNER_FCT_MAP_T;

#ifdef CC_SONY_EU_TUNERALL
UINT8 u1TunerVersion = 0; //To indicate tuner version
enum
{
    SONY_RE227,
    SONY_RE227_EVB,
    SONY_RE231
};
#endif

#if CUST_TUNER_TYPE==CUST_TUNER_LG_TDVWH810F
EXTERN ITUNER_FCT_T piTunerFct_TDVWH810F ;
#endif

#if CUST_TUNER_TYPE==CUST_TUNER_SS_DTVS203
EXTERN ITUNER_FCT_T piTunerFct_DTVS203 ;//defined in pd_tuner_dtvs203.c
#endif


#if CUST_TUNER_TYPE==CUST_TUNER_SONY
EXTERN ITUNER_FCT_T piTunerFct_SONY ;//defined in pd_tuner_sony.c
#endif

#if CUST_TUNER_TYPE==CUST_TUNER_SONY_RE215
EXTERN ITUNER_FCT_T piTunerFct_RE215 ;//defined in pd_tuner_sony.c
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18273)||(CUST_TUNER_TYPE==CUST_TUNER_NXPLIST)
EXTERN ITUNER_FCT_T piTunerFct_TDA18273 ;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18257)
EXTERN ITUNER_FCT_T piTunerFct_TDA18257;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_SS_DTVS20C)||(CUST_TUNER_TYPE==CUST_TUNER_NXPLIST)
EXTERN ITUNER_FCT_T piTunerFct_DTVS20C ;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_SILICON_TUNERA)
EXTERN ITUNER_FCT_T piTunerFct_TUNERA ;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_SHARP_VA1G2CD)
EXTERN ITUNER_FCT_T piTunerFct_VA1G2CD;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_TCL_F20WT)
EXTERN ITUNER_FCT_T piTunerFct_F20WT ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2158)
EXTERN ITUNER_FCT_T piTunerFct_SI2158 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_LG_G230D)||(CUST_TUNER_TYPE==CUST_TUNER_SIXLST)
EXTERN ITUNER_FCT_T piTunerFct_TDSY_G230D ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_Mxl_MXL601)
EXTERN ITUNER_FCT_T piTunerFct_MXL601 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18274)
EXTERN ITUNER_FCT_T piTunerFct_TDA18274 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_tda182I5a)
EXTERN ITUNER_FCT_T piTunerFct_TDA182I5A ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RB221)
EXTERN ITUNER_FCT_T piTunerFct_RB221 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA217)
EXTERN ITUNER_FCT_T piTunerFct_RA217 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA227)
EXTERN ITUNER_FCT_T piTunerFct_RA227 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA231)
EXTERN ITUNER_FCT_T piTunerFct_RA231 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RE227) || (CUST_TUNER_TYPE==CUST_TUNER_SIXLST) || (CUST_TUNER_TYPE==CUST_TUNER_SONYEULST)
EXTERN ITUNER_FCT_T piTunerFct_RE227 ;
#endif

#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RE231) || (CUST_TUNER_TYPE==CUST_TUNER_SIXLST) || (CUST_TUNER_TYPE==CUST_TUNER_SONYEULST)
EXTERN ITUNER_FCT_T piTunerFct_RE231 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RB231)
EXTERN ITUNER_FCT_T piTunerFct_RB231 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2157) || (CUST_TUNER_TYPE==CUST_TUNER_SIXLST)
EXTERN ITUNER_FCT_T piTunerFct_SI2157 ;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18275)
EXTERN ITUNER_FCT_T piTunerFct_TDA18275;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_HELENE)
EXTERN ITUNER_FCT_T piTunerFct_ASCOT3HELENE;
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2151) 
EXTERN ITUNER_FCT_T piTunerFct_SI2151 ;
#endif


static CUST_TUNER_FCT_MAP_T cust_tuner_map[]=
{
/*{tunertype defined in cust_tunerType.h,function table defined in pd_tuner_xx.h */
{0,NULL},//Not used
#if CUST_TUNER_TYPE==CUST_TUNER_LG_TDVWH810F
{CUST_TUNER_LG_TDVWH810F,&piTunerFct_TDVWH810F},
#endif
#if CUST_TUNER_TYPE==CUST_TUNER_SS_DTVS203
{CUST_TUNER_SS_DTVS203,&piTunerFct_DTVS203},
#endif
#if CUST_TUNER_TYPE==CUST_TUNER_SONY
{CUST_TUNER_SONY,&piTunerFct_SONY},
#endif
#if CUST_TUNER_TYPE==CUST_TUNER_SONY_RE215
{CUST_TUNER_SONY_RE215,&piTunerFct_RE215},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18273)||(CUST_TUNER_TYPE==CUST_TUNER_NXPLIST)
{CUST_TUNER_NXP_TDA18273,&piTunerFct_TDA18273},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18257)
{CUST_TUNER_NXP_TDA18257,&piTunerFct_TDA18257},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SS_DTVS20C)||(CUST_TUNER_TYPE==CUST_TUNER_NXPLIST)
{CUST_TUNER_SS_DTVS20C,&piTunerFct_DTVS20C},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILICON_TUNERA)
{CUST_TUNER_SILICON_TUNERA,&piTunerFct_TUNERA},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SHARP_VA1G2CD)
{CUST_TUNER_SHARP_VA1G2CD,&piTunerFct_VA1G2CD},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_TCL_F20WT)
{CUST_TUNER_TCL_F20WT,&piTunerFct_F20WT},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2158)
{CUST_TUNER_SILABS_SI2158,&piTunerFct_SI2158},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_LG_G230D)|| (CUST_TUNER_TYPE==CUST_TUNER_SIXLST)
{CUST_TUNER_LG_G230D,&piTunerFct_TDSY_G230D},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_Mxl_MXL601)
{CUST_TUNER_Mxl_MXL601,&piTunerFct_MXL601},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18274)
{CUST_TUNER_NXP_TDA18274,&piTunerFct_TDA18274},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_tda182I5a)
{CUST_TUNER_NXP_tda182I5a,&piTunerFct_TDA182I5A},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RB221)
{CUST_TUNER_SONY_RB221,&piTunerFct_RB221},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA217)
{CUST_TUNER_SONY_RA217,&piTunerFct_RA217},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA227)
{CUST_TUNER_SONY_RA227,&piTunerFct_RA227},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RA231)
{CUST_TUNER_SONY_RA231,&piTunerFct_RA231},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RE227)|| (CUST_TUNER_TYPE==CUST_TUNER_SIXLST)|| (CUST_TUNER_TYPE==CUST_TUNER_SONYEULST)
{CUST_TUNER_SONY_RE227,&piTunerFct_RE227},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RE231)|| (CUST_TUNER_TYPE==CUST_TUNER_SIXLST)|| (CUST_TUNER_TYPE==CUST_TUNER_SONYEULST)
{CUST_TUNER_SONY_RE231,&piTunerFct_RE231},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_RB231)
{CUST_TUNER_SONY_RB231,&piTunerFct_RB231},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2157)|| (CUST_TUNER_TYPE==CUST_TUNER_SIXLST)
{CUST_TUNER_SILABS_SI2157,&piTunerFct_SI2157},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_NXP_TDA18275)
{CUST_TUNER_NXP_TDA18275,&piTunerFct_TDA18275},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SONY_HELENE)
{CUST_TUNER_SONY_HELENE,&piTunerFct_ASCOT3HELENE},
#endif
#if (CUST_TUNER_TYPE==CUST_TUNER_SILABS_SI2151)
{CUST_TUNER_SILABS_SI2151,&piTunerFct_SI2151},
#endif
};
//#endif

#ifdef CC_SONY_EU_TUNERALL
static UINT8 GetSonyTunerType(VOID)
{
    UINT8 u1Val = SONY_RE227;
    UINT8 u1TunerType = CUST_TUNER_SONY_RE227;
    UINT32 u4Ofst;
    u4Ofst = DRVCUST_OptGet(eTunerTypeOfst0);
    VERIFY(0 == EEPROM_Read((UINT64)u4Ofst, (UINT32)(void *)&u1Val, 1));
    mcDBG_MSG_INFO(("Get tuner type from EEPROM address:%d, value:%d!\n",u4Ofst,u1Val));
    switch(u1Val)
    {
        case SONY_RE227:
            u1TunerType = CUST_TUNER_SONY_RE227;
            u1TunerVersion = 0;
            break;
        case SONY_RE227_EVB:
            u1TunerType = CUST_TUNER_SONY_RE227;
            u1TunerVersion = 1;
            break;
        case SONY_RE231:
            u1TunerType= CUST_TUNER_SONY_RE231;
            break;
        default:
            u1TunerType = CUST_TUNER_SONY_RE227;
            u1TunerVersion = 0;
            break;
    }
    return u1TunerType;
}
#endif

ITUNER_FCT_T* GetCustTunerFuncTable(UINT16 id){
    
	UINT8 u1FctTblNum=sizeof(cust_tuner_map)/sizeof(cust_tuner_map[0]);
	ITUNER_FCT_T* pTunerFunc=NULL;
	UINT8 ii=0;
	mcDBG_MSG_INFO(("Total %d func table exist\n",u1FctTblNum-1)); 
	
	if(u1FctTblNum==1)
	{
	    mcDBG_MSG_ERR(("Func table is NULL!Please check your SW setting\n"));
	}
	else if(u1FctTblNum==2)
	 {
	   pTunerFunc=cust_tuner_map[1].fct;
	   mcDBG_MSG_INFO(("Tuner Type=0x%02X\n",cust_tuner_map[1].id));
	 }
	else //u1FctTblNum>2
	 {
	  for(ii=0;ii<u1FctTblNum;ii++)
	    {
#ifdef CC_SONY_EU_TUNERALL
          id = GetSonyTunerType();
#endif
	     if(cust_tuner_map[ii].id==id)
		 {
		  pTunerFunc=cust_tuner_map[ii].fct;
		  mcDBG_MSG_INFO(("TunerAll tunertype\n"));
		  break;
		 }
	    }
	 }
	 return pTunerFunc;
}

