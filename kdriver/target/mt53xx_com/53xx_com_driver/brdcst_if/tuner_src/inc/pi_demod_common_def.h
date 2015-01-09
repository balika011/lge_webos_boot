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
#ifndef PI_DEMOD_COMMON_DEF_H
#define PI_DEMOD_COMMON_DEF_H
//DVBT
enum
{
	SAW_BW_6M = 0,
	SAW_BW_7M,
	SAW_BW_8M,
    SAW_BW_5M
};
enum
{
    cIF_TARGET_LEVEL_0CCC = 0,
    cIF_TARGET_LEVEL_0A97,
    cIF_TARGET_LEVEL_09CC,
    cIF_TARGET_LEVEL_07CC,
    cIF_TARGET_LEVEL_0CCC_0D36
};
enum
{
	DVBT_QPSK =0,
	DVBT_Q16,
	DVBT_Q64,
	DVBT_Qam_Mode_Size 
};

#define DVBT_Qam_Mode_Unknow	DVBT_Qam_Mode_Size
enum
{
	DVBT_CR_12 = 0,
	DVBT_CR_23,
	DVBT_CR_34,
	DVBT_CR_56,
	DVBT_CR_78,
	DVBT_CR_Mode_Size
};

#define ccTUNER_LOCK_CONF_THRESHOLD      0
#define cALC_ADC_INPUT_SWING             2  //Volt
#define cALC_ADC_COMM_VOLT               1.2
#define cALC_ADC_BIAS                    (cALC_ADC_COMM_VOLT - cALC_ADC_INPUT_SWING/2)
#endif

