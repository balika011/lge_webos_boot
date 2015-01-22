/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2010
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

#include <common.h>
#include <config.h>
#include <intrCtrl.h>
#include <CTP_type.h>
#include <CTP_heap.h>
#include <CTP_mem.h>
#include <api.h>
#include <processor.h>

#include "msdc_ett.h"


        u32 g_int_data_latch_cksel;
        u32 g_ckgen_msdc_dly_sel;
        u32 g_write_crcs_ta_cntr;
        u32 g_write_pad_tune_datwrdly;
        u32 g_write_w_d0_smpl;
        u32 g_write_dat0_rddly;


extern void CTP_EngineMain(void);
extern void __stack_chk_guard_setup(void);
static CTP_STATUS_T InitTestSuites(void);

#define CTP_USER_COMMAND_SIZE       128
char                g_azCTP_UserCommand[CTP_USER_COMMAND_SIZE];
unsigned char command_index = 0;

/*******************************************************************************
* FUNCTION
*   CTP_GetUserCommand
*
* DESCRIPTION
*   None
*
* CALLS
*   None
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*   None
*******************************************************************************/

CTP_STATUS_T CTP_GetUserCommand(char *pzCommand)
{
#ifdef WIN32
    gets(pzCommand);
#else
    unsigned char   ucChar;
    UINT32          u4Length = 0;
    while ((ucChar = ReadDebugByte()) != '\r')
    {
        if(ucChar == 0xFF)
        {
            continue;
        }
        printf("%c", ucChar);
        if(ucChar == 0x08 && u4Length != 0)
        {
            pzCommand--;
            u4Length--;
        }
        else
        {
            *pzCommand = ucChar;
            pzCommand++;
            u4Length++;
       }
    }
    *pzCommand = '\0';
    printf("\r\n");
#endif
    return CTP_SUCCESS;
}


/*******************************************************************************
* FUNCTION
*   CTP_ToInt
*
* DESCRIPTION
*   None
*
* CALLS
*   None
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*   None
*******************************************************************************/

UINT32 CTP_ToInt(char *str)
{
#ifdef WIN32
    return atoi(str);
#else
    UINT32 result;

    for (result = 0; *str >= '0' && *str <= '9'; str++)
    {
        result = (result*10) + (*str-'0');
    }
    return result;
#endif /* #ifdef WIN32 */
}


/*******************************************************************************
* FUNCTION
*   CTP_GetUserNumCommand
*
* DESCRIPTION
*   None
*
* CALLS
*   None
*
* PARAMETERS
*   None
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*   None
*******************************************************************************/

CTP_STATUS_T CTP_GetUserNumCommand(UINT32 *pu4UserNum, UINT32 u4MinValidValue, UINT32 u4MaxValidValue)
{
    UINT32 u4UserNum;

    CTP_GetUserCommand(g_azCTP_UserCommand);
    u4UserNum = CTP_ToInt(g_azCTP_UserCommand);

    if ((u4UserNum > u4MaxValidValue) ||
        (u4UserNum < u4MinValidValue) ||
        ((u4UserNum == 0) && g_azCTP_UserCommand[0] != '0'))
    {
        printf("[WARNING] Invalide input = %s\r\n", g_azCTP_UserCommand);
        return CTP_FAIL;
    }
    *pu4UserNum = u4UserNum;
    return CTP_SUCCESS;
}


void C_Main(void)
{
#if defined(MINI)
    UINT32  l_value;
    UINT32  l_stress_test = 0;
    UINT32  l_ret;
    UINT32  l_host_id;
    UINT32  base;

    
    system_init();

#if 1
    sys_print("\r\n****** MediaTek Device Test for %s Built %s********\r\n", CHIPSET,__DATE__);
    sys_print("\r\n======================================================\r\n");
    sys_print("\r\nThis is mini DVT \r\n");
    sys_print("\r\n======================================================\r\n");
    
    ett_msdc_prepara();
    do {
        sys_print("\r\n");
        sys_print("[0]  normal test \r\n");
        sys_print("[1]  high voltage core1/2 test\r\n");                
        sys_print("[2]  low voltage core1/2 test\r\n");                
        sys_print("[3]  high voltage core1/2 & low voltage VIO test\r\n");                
        sys_print("[4]  low voltage core1/2 & high voltage VIO test\r\n");                
        sys_print("Please select test card : ");                
    } while (CTP_GetUserNumCommand(&l_value, 0, 5) != CTP_SUCCESS);

    //VCORE1  (VCORE_PDN) 
    //pmic_config_interface(0x24C,0x44,0x7F,0); // [6:0]: VDVFS11_VOSEL;
    //pmic_config_interface(0x24E,0x44,0x7F,0); // [6:0]: VDVFS11_VOSEL_ON;

    //VCORE2  (VCORE_AO) 
    //pmic_config_interface(0x36A,0x44,0x7F,0); // [6:0]: VCORE2_VOSEL;
    //pmic_config_interface(0x36C,0x44,0x7F,0); // [6:0]: VCORE2_VOSEL_ON;

    //all voltage = 700mv + 6.25mv * register value (2nd parameter of task)

    //VIO18  
    //ret = pmic_config_interface(0x396,0x20,0x7F,0); // [6:0]: VIO18_VOSEL;
    //ret = pmic_config_interface(0x398,0x20,0x7F,0); // [6:0]: V IO18_VOSEL_ON;

    //all voltage = 1400mv + 12.5mv * register value (2nd parameter of task)

    if (l_value == 0){
        //vcore1: 1.125v, vcore2: 1.125v, VIO: 1.8v

#ifdef ETT_PMIC_WRAP_READY
        /* vcore1 to 1.125 */
        pwrap_read(0x24C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x44;
        pwrap_write(0x24C, l_ret);

        pwrap_read(0x24E, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x44;
        pwrap_write(0x24E, l_ret);

        /* vcore2 to 1.125 */
        pwrap_read(0x36A, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x44;
        pwrap_write(0x36A, l_ret);

        pwrap_read(0x36C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x44;
        pwrap_write(0x36C, l_ret);

        /* vio18 to 1.8v */
        pwrap_read(0x396, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x20;
        pwrap_write(0x396, l_ret);

        pwrap_read(0x398, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x20;
        pwrap_write(0x398, l_ret);
#endif

    } else if (l_value == 1){
        //vcore1: 1.15v, vcore2: 1.15v, VIO: 1.9v

#ifdef ETT_PMIC_WRAP_READY
        /* vcore1 to 1.15 */
        pwrap_read(0x24C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x24C, l_ret);

        pwrap_read(0x24E, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x24E, l_ret);

        /* vcore2 to 1.15 */
        pwrap_read(0x36A, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x36A, l_ret);

        pwrap_read(0x36C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x36C, l_ret);

        /* vio18 to 1.9v */
        pwrap_read(0x396, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x396, l_ret);

        pwrap_read(0x398, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x398, l_ret);
#endif
    } else if (l_value == 2){
        //vcore1: 0.95v, vcore2: 0.95v, VIO: 1.7v
        
#ifdef ETT_PMIC_WRAP_READY
        /* vcore1 to 0.95 */
        pwrap_read(0x24C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x24C, l_ret);

        pwrap_read(0x24E, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x24E, l_ret);

        /* vcore2 to 0.95 */
        pwrap_read(0x36A, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x36A, l_ret);

        pwrap_read(0x36C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x36C, l_ret);

        /* vio18 to 1.7v */
        pwrap_read(0x396, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x18;
        pwrap_write(0x396, l_ret);

        pwrap_read(0x398, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x18;
        pwrap_write(0x398, l_ret);
#endif
    } else if (l_value == 3) {
        //vcore1: 1.15v, vcore2: 1.15v, VIO: 1.7v
        
#ifdef ETT_PMIC_WRAP_READY
        /* vcore1 to 1.15 */
        pwrap_read(0x24C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x24C, l_ret);

        pwrap_read(0x24E, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x24E, l_ret);

        /* vcore2 to 1.15 */
        pwrap_read(0x36A, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x36A, l_ret);

        pwrap_read(0x36C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x48;
        pwrap_write(0x36C, l_ret);

        /* vio18 to 1.7v */
        pwrap_read(0x396, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x18;
        pwrap_write(0x396, l_ret);

        pwrap_read(0x398, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x18;
        pwrap_write(0x398, l_ret);
#endif 
    } else if (l_value == 4) {
        //vcore1: 0.95v, vcore2: 0.95v, VIO: 1.9v
        
#ifdef ETT_PMIC_WRAP_READY
        /* vcore1 to 0.95 */
        pwrap_read(0x24C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x24C, l_ret);

        pwrap_read(0x24E, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x24E, l_ret);

        /* vcore2 to 0.95 */
        pwrap_read(0x36A, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x36A, l_ret);

        pwrap_read(0x36C, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x36C, l_ret);

        /* vio18 to 1.9v */
        pwrap_read(0x396, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x396, l_ret);

        pwrap_read(0x398, &l_ret);
        l_ret &= ~0x7F;
        l_ret |= 0x28;
        pwrap_write(0x398, l_ret);
#endif 
    }

#ifdef ETT_PMIC_WRAP_READY
    pwrap_read(0x24C, &l_ret);
    sys_print("[info][%s %d]0x24C = 0x%x, vcore1 = %d mv\n", __func__, __LINE__, l_ret, 700 + (625*l_ret)/100);                
    pwrap_read(0x24E, &l_ret);
    sys_print("[info][%s %d]0x24E = 0x%x\n", __func__, __LINE__, l_ret);                
    pwrap_read(0x36A, &l_ret);
    sys_print("[info][%s %d]0x36A = 0x%x, vcore2 = %d mv\n", __func__, __LINE__, l_ret, 700 + (625*l_ret)/100);                
    pwrap_read(0x36C, &l_ret);
    sys_print("[info][%s %d]0x36C = 0x%x\n", __func__, __LINE__, l_ret);                
    pwrap_read(0x396, &l_ret);
    sys_print("[info][%s %d]0x396 = 0x%x vio = %d mv\n", __func__, __LINE__, l_ret, 1400 + (125*l_ret)/10);                
    pwrap_read(0x398, &l_ret);
    sys_print("[info][%s %d]0x398 = 0x%x\n", __func__, __LINE__, l_ret);                
#endif
    do {
        sys_print("\r\n");
        sys_print("[0]  MSDC ETT - emmc \r\n");
        sys_print("[1]  MSDC ETT - sd\r\n");                
        sys_print("[2]  MSDC ETT - debug\r\n");                
        sys_print("Please select test card : ");                
    } while (CTP_GetUserNumCommand(&l_value, 0, 3) != CTP_SUCCESS);

    if (l_value == 0){
        l_host_id  = 0;
        base = MSDC0_BASE;

        do {
            sys_print("\r\n");
            sys_print("[0]  emmc - default mode\r\n");
            sys_print("[1]  emmc - ddr@highspeed\r\n");                
            sys_print("[2]  emmc - sdr@hs200\r\n");                
            sys_print("[3]  emmc - sdr@hs400\r\n");                
            sys_print("[4]  emmc - ddr@highspeed stress test\r\n");                
            sys_print("[5]  emmc - sdr@hs200 stress test\r\n");                
            sys_print("[6]  emmc - sdr@hs400 stress test\r\n");                
            sys_print("Please select test card : ");                
        } while (CTP_GetUserNumCommand(&l_value, 0, 7) != CTP_SUCCESS);

        switch (l_value){
            case 0:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[0].flags = 0;    
                break;
            case 4:
                l_stress_test = 1;
                l_value = 1;
            case 1:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[0].flags =  MSDC_HIGHSPEED | MSDC_DDR;    
                break;
            case 5:
                l_stress_test = 1;
                l_value = 2;
            case 2:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[0].flags =  MSDC_HIGHSPEED | MSDC_DDR | MSDC_HS200;    
                break;
            case 6:
                l_stress_test = 1;
                l_value = 3;
            case 3:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[0].flags =  MSDC_HIGHSPEED | MSDC_DDR | MSDC_HS200 | MSDC_HS400;    
                break;
            default:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[0].flags = 0;    
                break;
        }
    } else if (l_value == 1){
        l_host_id  = 1;
        base = MSDC1_BASE;

        do {
            sys_print("\r\n");
            sys_print("[0]  sd - default mode\r\n");
            sys_print("[1]  sd - sdr@highspeed\r\n");                
            sys_print("[2]  sd - sdr@sdr50\r\n");                
            sys_print("[3]  sd - sdr@sdr104\r\n");                
            sys_print("[4]  sd - ddr@ddr50\r\n");                
            sys_print("[5]  sd - sdr@highspeed stress test\r\n");                
            sys_print("[6]  sd - sdr@sdr50 stress test\r\n");                
            sys_print("[7]  sd - sdr@sdr104 stress test\r\n");                
            sys_print("[8]  sd - ddr@ddr50 stress test\r\n");                
            sys_print("Please select test card : ");                
        } while (CTP_GetUserNumCommand(&l_value, 0, 9) != CTP_SUCCESS);

        switch (l_value){
            case 0:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags = 0; 
                break;
            case 5:
                l_stress_test = 1;
                l_value = 1;
            case 1:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags =  MSDC_HIGHSPEED;    
                break;
            case 6:
                l_stress_test = 1;
                l_value = 2;
            case 2:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags =  MSDC_HIGHSPEED | MSDC_UHS1;    
                break;
            case 7:
                l_stress_test = 1;
                l_value = 3;
            case 3:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags =  MSDC_HIGHSPEED | MSDC_DDR | MSDC_UHS1;    
                break;
            case 8:
                l_stress_test = 1;
                l_value = 4;
            case 4:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags =  MSDC_HIGHSPEED | MSDC_DDR | MSDC_UHS1;    
                break;
            default:
                sys_print("[%s %d]\n", __func__, __LINE__);
                msdc_cap[1].flags = 0;    
                break;
        }
    } else if (l_value == 2){
        /* debug mode */
        g_msdc_ett_debug = 1;

        sys_print("emmc, hs200 mode, write crc debug, asked by designer\n");
      
        l_host_id  = 0;
        base = MSDC0_BASE;
        msdc_cap[0].flags =  MSDC_HIGHSPEED | MSDC_DDR | MSDC_HS200 | MSDC_HS400;    


#if 0
        do {
            sys_print("\r\n");
            sys_print("MSDC_INT_DAT_LATCH_CK_SE \r\n");
            sys_print("Please select the value[0-7] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.int_data_latch_cksel), 0, 8) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_CKGEN_MSDC_DLY_SEL \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.ckgen_msdc_dly_sel), 0, 32) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_WRDAT_CRCS_TA_CNTR \r\n");
            sys_print("Please select the value[0-7] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.write_crcs_ta_cntr), 0, 8) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_PAD_TUNE_DATWRDLY \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.write_pad_tune_datwrdly), 0, 32) != CTP_SUCCESS);
        
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_IOCON_WD0_SMPL \r\n");
            sys_print("Please select the value[0-1] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.write_w_d0_smpl), 0, 2) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_DAT_RDDLY0_D0 \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&(g_tune_para.write_dat0_rddly), 0, 32) != CTP_SUCCESS);
#else

        do {
            sys_print("\r\n");
            sys_print("MSDC_INT_DAT_LATCH_CK_SE \r\n");
            sys_print("Please select the value[0-7] : ");                
        } while (CTP_GetUserNumCommand(&g_int_data_latch_cksel, 0, 8) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_CKGEN_MSDC_DLY_SEL \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&g_ckgen_msdc_dly_sel, 0, 32) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_WRDAT_CRCS_TA_CNTR \r\n");
            sys_print("Please select the value[0-7] : ");                
        } while (CTP_GetUserNumCommand(&g_write_crcs_ta_cntr, 0, 8) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_PAD_TUNE_DATWRDLY \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&g_write_pad_tune_datwrdly, 0, 32) != CTP_SUCCESS);
        
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_IOCON_WD0_SMPL \r\n");
            sys_print("Please select the value[0-1] : ");                
        } while (CTP_GetUserNumCommand(&g_write_w_d0_smpl, 0, 2) != CTP_SUCCESS);
        
        do {
            sys_print("\r\n");
            sys_print("MSDC_DAT_RDDLY0_D0 \r\n");
            sys_print("Please select the value[0-31] : ");                
        } while (CTP_GetUserNumCommand(&g_write_dat0_rddly, 0, 32) != CTP_SUCCESS);
#endif
    }

    l_ret = msdc_test_init(l_host_id, l_value);
    if (l_ret != 0){
        sys_print("[%s %d][SD%d] card init failed(%d)\n", __func__, __LINE__, l_host_id, l_ret);
        while(1);
    }

    ett_cmd_test();

    if (msdc_cap[0].flags == (MSDC_HIGHSPEED | MSDC_DDR | MSDC_HS200 | MSDC_HS400)){
        /* for emmc50, used other tuning flow */
        ett_write_tx_test();
        ett_emmc50_rw_test();
        ett_cmd_tx_test();
    } else {
        ett_write_test();
        ett_read_test();
    }

    /* stress test */
    if (l_stress_test == 1){
        /* after apply the best tune parameter, then do stress test */
        while (1){
            mmc_basic_rw_test();
        }
    }

#endif

#else
    int i = 0;
   
    if (processor_id() == 0)
        system_init();
    else
        slaveCPU_system_init();

    sys_print("\r\n****** MediaTek Device Test for %s Built %s********\r\n", CHIPSET,__DATE__);
    sys_print("\r\nCPU%d Start initialization ..................................\r\n",processor_id());

    // Initial Heap Memory Manager
    if (processor_id() == 0)
	g_Heap_NC = (unsigned int *)NC_HEAP_START0;
    else
	g_Heap_NC = (unsigned int *)NC_HEAP_START1;
    dbg_print("\r\nCPU%d heap start:%X, with size:%X\r\n", processor_id(), g_Heap_NC, NC_HEAP_SIZE);
    MEM_Initialize(g_Heap, HEAP_SIZE, g_Heap_NC, NC_HEAP_SIZE);
    dbg_print("start:%x,end:%x,size:%x,NR_CPUS:%d\n",NC_HEAP_START, NC_HEAP_END, NC_HEAP_END - NC_HEAP_START, __NR_CPUS__);

    if (InitTestSuites() == CTP_FAIL) {
        sys_print("Spin forever !!!\r\n");
        while(1) ;
    }
    
    CTP_EngineMain();
#endif
}

#if !defined(MINI)
CTP_STATUS_T InitTestSuites(void)
{
    char *pString;
    CTP_STATUS_T eStatus = CTP_SUCCESS;
    UINT32	i;

    eStatus = CTP_testsuite((void **)&pString);
    if (eStatus == CTP_FAIL)
    {
        //sys_print("[WARNING] Fail to initialize(%d): %s\r\n", i, pString);
        return CTP_FAIL;
    }

    return eStatus;
}
#endif

#ifdef __MTK_SLT_
extern void max_power_test();
extern void memcpy_l1_test();
extern void memcpy_l2_test();
extern void vfp_func_test();
extern void adv_simd_test();
extern void scu_test();
extern void fdct_test();
extern void imdct_test();
extern volatile unsigned int sync_flag_start[8] __attribute__ ((__section__ ("MULTI_CTP_SHARE")));

#define PRE_BUILD_TMP_PASS
void SlaveCPU_C_Main(void) 
{
    unsigned int nCores = get_cpu_cores();
    
    int i ;
    static int j = 0;
    slaveCPU_system_init();

    while(1) {

        //while(j)
        //    __asm__ __volatile__ ("wfi" : : : "memory");

#ifdef SCU_TEST
        if (nCores == 2 || nCores == 8 || nCores == 4)
        {

            /*Sync CPU0*/
            if(__is_dcache_enable()) {
                __disable_dcache();
                __inner_flush_dcache_L1();
            }
            for (i = 1; i < nCores; i++){
                sync_flag_start[i]=S_SCU_TEST;
            }
            while(S_SCU_TEST!=sync_flag_start[0]);
            scu_test();
        }
#endif
#ifdef MAX_POWER_TEST
        /*Sync CPU0*/
        while(S_MAX_POWER_TEST!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_MAX_POWER_TEST;
        }
        max_power_test();
#endif
#ifdef MEMCPY_L1_TEST
        /*Sync CPU0*/
        while(S_MEMORY_CPY_L1!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_MEMORY_CPY_L1;
        }
        memcpy_l1_test();
#endif
#ifdef MEMCPY_L2_TEST
        /*Sync CPU0*/
        while(S_MEMORY_CPY_L2!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_MEMORY_CPY_L2;
        }
        memcpy_l2_test();
#endif
#ifdef VFP_TEST
        /*Sync CPU0*/
        while(S_VFP_FUNC!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_VFP_FUNC;
        }
        vfp_func_test();
#endif
#ifdef SIMD_TEST
        /*Sync CPU0*/
        while(S_ADV_SIMD!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_ADV_SIMD;
        }
        adv_simd_test();
#endif
#ifdef FDCT_TEST
        /*Sync CPU0*/
        while(S_FDCT_TEST!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_FDCT_TEST;
        }
        fdct_test();
#endif
#ifdef IMDCT_TEST
        /*Sync CPU0*/
        while(S_IMDCT_TEST!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_IMDCT_TEST;
        }
        imdct_test();
#endif
#ifdef DHRYSTONE_TEST
        /*Sync CPU0*/
        while(S_DHRY_TEST!=sync_flag_start[0]);
        for (i = 1; i < nCores; i++){
            sync_flag_start[i]=S_DHRY_TEST;
        }
        dhrystone_test();
#endif
        __asm__ __volatile__ ("wfi" : : : "memory");
        ++j;
        sys_print("%d\n", ++j); 
    }
}
#endif /* !_MTK_SLT_ */
