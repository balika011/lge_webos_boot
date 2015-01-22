/* do read parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "msdc_ett.h"
#include "msdc.h"

extern u32 g_msdc_ett_debug1;
extern u32 g_msdc_ett_debug1_dat_latch_ck_sel;
extern u32 g_msdc_ett_debug1_ckgen_dly_sel;

void ett_read_test(void)
{
    int l_int_data_latch_cksel = 0;   //clock timing relate
    int l_ckgen_msdc_dly_sel = 0;     
    int l_end = 0;     
#ifdef ETT_SPILT_TUNE
    u8  l_read_datx_sampl_edge[8] = {0}; //pad relate
    //int l_read_dat0_sampl_edge = 0;   //pad relate
    //int l_read_dat1_sampl_edge = 0;   
    //int l_read_dat2_sampl_edge = 0;   
    //int l_read_dat3_sampl_edge = 0;  
    //int l_read_dat4_sampl_edge = 0;   
    //int l_read_dat5_sampl_edge = 0; 
    //int l_read_dat6_sampl_edge = 0;  
    //int l_read_dat7_sampl_edge = 0;   
    u8  l_read_datx_delay[8] = {0}; 
    //int l_read_dat0_delay = 0;   
    //int l_read_dat1_delay = 0;   
    //int l_read_dat2_delay = 0;   
    //int l_read_dat3_delay = 0;   
    //int l_read_dat4_delay = 0;   
    //int l_read_dat5_delay = 0;   
    //int l_read_dat6_delay = 0;   
    //int l_read_dat7_delay = 0;
    //
    u8 i = 0;
    u8 l_read_pin = 0;
    u8 bus_width = 0;
#else
    int l_read_dat_sampl_edge = 0;   //pad relate
    int l_read_dat_delay = 0;   
#endif

    int l_result = 0, l_count = 0, l_max_count = 0;

    struct mmc_host *host;
    struct mmc_card *card;

    struct mmc_command cmd;

    host = &ett_sd_host;
    card = &ett_sd_card;

    ett_memset(&cmd, sizeof(struct mmc_command), 0x0);

    /* command tuning first, apply command parameter result base on the test */
    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, g_tune_para.cmd_rsp_ta_cntr);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, g_tune_para.pad_cmd_rsp_rxdel);
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, g_tune_para.iocon_rspl);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, g_tune_para.pad_tune_cmdrrdly);



    /* set to default value */
    if(!g_msdc_ett_debug1)
    	{
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0x1);
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0x0);
    }
    else
    {
    	MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, g_msdc_ett_debug1_dat_latch_ck_sel);
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, g_msdc_ett_debug1_ckgen_dly_sel);
    }
    g_tune_dbg.read_count = 0;
    g_tune_dbg.read_passed = 0;
    g_tune_dbg.read_failed = 0;
#ifdef ETT_SPILT_TUNE
    g_tune_para.read_max_window[0] = 0;
    g_tune_para.read_max_window[1] = 0;
    g_tune_para.read_max_window[2] = 0;
    g_tune_para.read_max_window[3] = 0;
    g_tune_para.read_max_window[4] = 0;
    g_tune_para.read_max_window[5] = 0;
    g_tune_para.read_max_window[6] = 0;
    g_tune_para.read_max_window[7] = 0;
#else 
    g_tune_para.read_max_window = 0;
#endif 
    /* get current releate paras */
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);

#ifdef ETT_SPILT_TUNE
    /* set to default value */
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL_SEL, 0x1); // each data line has its own delay slection value
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D0SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D1SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D2SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D3SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D4SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D5SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D6SPL, 0x0);   
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D7SPL, 0x0);   
    
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DDLSEL, 0x1);  // each data line has its own delay slection value
    MSDC_WRITE32(MSDC_DAT_RDDLY0, 0x0); 
    MSDC_WRITE32(MSDC_DAT_RDDLY1, 0x0);  

    /* get current releate paras */
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D0SPL, l_read_datx_sampl_edge[0]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D1SPL, l_read_datx_sampl_edge[1]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D2SPL, l_read_datx_sampl_edge[2]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D3SPL, l_read_datx_sampl_edge[3]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D4SPL, l_read_datx_sampl_edge[4]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D5SPL, l_read_datx_sampl_edge[5]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D6SPL, l_read_datx_sampl_edge[6]);   
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D7SPL, l_read_datx_sampl_edge[7]);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D0SPL, l_read_dat0_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D1SPL, l_read_dat1_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D2SPL, l_read_dat2_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D3SPL, l_read_dat3_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D4SPL, l_read_dat4_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D5SPL, l_read_dat5_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D6SPL, l_read_dat6_sampl_edge);   
    //MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D7SPL, l_read_dat7_sampl_edge);   

    MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, l_read_datx_delay[0]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D1, l_read_datx_delay[1]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D2, l_read_datx_delay[2]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D3, l_read_datx_delay[3]);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, l_read_dat0_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D1, l_read_dat1_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D2, l_read_dat2_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D3, l_read_dat3_delay);   

    MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D4, l_read_datx_delay[4]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D5, l_read_datx_delay[5]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D6, l_read_datx_delay[6]);   
    MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D7, l_read_datx_delay[7]);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D4, l_read_dat4_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D5, l_read_dat5_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D6, l_read_dat6_delay);   
    //MSDC_GET_FIELD(MSDC_DAT_RDDLY1, MSDC_DAT_RDDLY1_D7, l_read_dat7_delay);   

#if ETT_STRIP_CODE
    sys_print("read    paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", l_int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", l_ckgen_msdc_dly_sel);
    sys_print("               MSDC_IOCON_D0SPL          = %d\n", l_read_datx_sampl_edge[0]);
    sys_print("               MSDC_IOCON_D1SPL          = %d\n", l_read_datx_sampl_edge[1]);
    sys_print("               MSDC_IOCON_D2SPL          = %d\n", l_read_datx_sampl_edge[2]);
    sys_print("               MSDC_IOCON_D3SPL          = %d\n", l_read_datx_sampl_edge[3]);
    sys_print("               MSDC_IOCON_D4SPL          = %d\n", l_read_datx_sampl_edge[4]);
    sys_print("               MSDC_IOCON_D5SPL          = %d\n", l_read_datx_sampl_edge[5]);
    sys_print("               MSDC_IOCON_D6SPL          = %d\n", l_read_datx_sampl_edge[6]);
    sys_print("               MSDC_IOCON_D7SPL          = %d\n", l_read_datx_sampl_edge[7]);
    sys_print("               MSDC_DAT_RDDLY0_D0        = %d\n", l_read_datx_delay[0]);
    sys_print("               MSDC_DAT_RDDLY0_D1        = %d\n", l_read_datx_delay[1]);
    sys_print("               MSDC_DAT_RDDLY0_D2        = %d\n", l_read_datx_delay[2]);
    sys_print("               MSDC_DAT_RDDLY0_D3        = %d\n", l_read_datx_delay[3]);
    sys_print("               MSDC_DAT_RDDLY1_D4        = %d\n", l_read_datx_delay[4]);
    sys_print("               MSDC_DAT_RDDLY2_D5        = %d\n", l_read_datx_delay[5]);
    sys_print("               MSDC_DAT_RDDLY3_D6        = %d\n", l_read_datx_delay[6]);
    sys_print("               MSDC_DAT_RDDLY4_D7        = %d\n", l_read_datx_delay[7]);
    //sys_print("               MSDC_IOCON_D0SPL          = %d\n", l_read_dat0_sampl_edge);
    //sys_print("               MSDC_IOCON_D1SPL          = %d\n", l_read_dat1_sampl_edge);
    //sys_print("               MSDC_IOCON_D2SPL          = %d\n", l_read_dat2_sampl_edge);
    //sys_print("               MSDC_IOCON_D3SPL          = %d\n", l_read_dat3_sampl_edge);
    //sys_print("               MSDC_IOCON_D4SPL          = %d\n", l_read_dat4_sampl_edge);
    //sys_print("               MSDC_IOCON_D5SPL          = %d\n", l_read_dat5_sampl_edge);
    //sys_print("               MSDC_IOCON_D6SPL          = %d\n", l_read_dat6_sampl_edge);
    //sys_print("               MSDC_IOCON_D7SPL          = %d\n", l_read_dat7_sampl_edge);
    //sys_print("               MSDC_DAT_RDDLY0_D0        = %d\n", l_read_dat0_delay);
    //sys_print("               MSDC_DAT_RDDLY0_D1        = %d\n", l_read_dat1_delay);
    //sys_print("               MSDC_DAT_RDDLY0_D2        = %d\n", l_read_dat2_delay);
    //sys_print("               MSDC_DAT_RDDLY0_D3        = %d\n", l_read_dat3_delay);
    //sys_print("               MSDC_DAT_RDDLY1_D4        = %d\n", l_read_dat4_delay);
    //sys_print("               MSDC_DAT_RDDLY2_D5        = %d\n", l_read_dat5_delay);
    //sys_print("               MSDC_DAT_RDDLY3_D6        = %d\n", l_read_dat6_delay);
    //sys_print("               MSDC_DAT_RDDLY4_D7        = %d\n", l_read_dat7_delay);
#endif

#else
    /* set to default value */
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL_SEL, 0x0); // share one read latch data para for all data line
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, 0x0);    // rising

    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DDLSEL, 0x0);          // share one delay slection value for all data line
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, 0x0); 


    /* get current releate paras */
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, l_read_dat_sampl_edge);   
    MSDC_GET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, l_read_dat_delay); 

    sys_print("read    paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", l_int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", l_ckgen_msdc_dly_sel);
    sys_print("               MSDC_IOCON_DSPL           = %d\n", l_read_dat_sampl_edge);
    sys_print("               MSDC_PAD_TUNE_DATRRDLY    = %d\n", l_read_dat_delay);
#endif
    
    /* send cmd18, not to try tuning */ 
    cmd.opcode  = MMC_CMD_READ_MULTIPLE_BLOCK;
    //cmd.opcode  = MMC_CMD_READ_SINGLE_BLOCK;
    cmd.arg = ETT_TEST_START_ADDR - 88;  // offset is 1GB 
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

#ifdef ETT_SPILT_TUNE
    /* sd always 4bit mode, emmc 8bit mode */
    bus_width = 8;
    if (mmc_card_sd(card)) {
        bus_width = 4;
    }

    for (i = 0; i < bus_width; i++){
        l_read_pin = 1 << i;
        l_read_datx_sampl_edge[i] = 0;
        MSDC_SET_FIELD(MSDC_IOCON, 0x1 << (16 + i), 0);   
        do { 
            MSDC_SET_FIELD(MSDC_IOCON, 0x1 << (16 + i), l_read_datx_sampl_edge[i]);   

            l_count = 0;
            l_end = 0;
            l_max_count = 0;
            l_read_datx_delay[i] = 0;
            MSDC_WRITE32(MSDC_DAT_RDDLY0, 0x0); 
            MSDC_WRITE32(MSDC_DAT_RDDLY1, 0x0);  
            do {
                if (i < 4) {
                    MSDC_SET_FIELD(MSDC_DAT_RDDLY0, 0x1f << ((3-i)*8), l_read_datx_delay[i]);   
                } else {
                    MSDC_SET_FIELD(MSDC_DAT_RDDLY1, 0x1f << ((7-i)*8), l_read_datx_delay[i]);   
                }

                if (cmd.arg > ETT_TEST_MAZ_ADDR){
                   cmd.arg = ETT_TEST_START_ADDR - 88; 
                }

                cmd.arg += 88;  //avoid read the fixed blocks
                l_result = mmc_rw_test(host, card, &cmd, l_read_pin);
                g_tune_dbg.read_count++;
                if (l_result == MMC_ERR_NONE){
                    g_tune_dbg.read_passed++;
                    if ((++l_count) > l_max_count){
                        l_max_count = l_count;
                        l_end = l_read_datx_delay[i];
                    }
                } else {
                    g_tune_dbg.read_failed++;
                    l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]read test failed(%d 0x%x)\n", __func__, __LINE__, l_result, g_tune_dbg.dcrc);
#endif
                }

#ifdef ETT_SHOW_PASS_WINDOW
                sys_print("%c(0x%x) ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'), g_tune_dbg.dcrc);
#else
                sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\n", g_tune_dbg.read_count, l_result, l_int_data_latch_cksel, 
                        l_ckgen_msdc_dly_sel,
                        l_read_datx_sampl_edge[i],
                        l_read_datx_delay[i]);
#endif
                g_tune_dbg.dcrc = 0;

            } while (l_read_datx_delay[i]++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
            sys_print("r:%d(%d\t%d)\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end, l_int_data_latch_cksel, 
                                                                                                                         l_ckgen_msdc_dly_sel,
                                                                                                                         l_read_datx_sampl_edge[i]);
#endif
            /* l_max_count will be 0, 1, ... 32 */
            if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                sys_print("[%s %d]read window find(%d)\n", __func__, __LINE__, g_tune_dbg.read_count);
#endif
                //update the max real window
                if (l_max_count > g_tune_para.read_max_window[i]){  // new window observe
                    g_tune_para.read_max_window[i] = l_max_count;

                    g_tune_para.read_datx_sampl_edge[i] = l_read_datx_sampl_edge[i]; //pad relate
                    g_tune_para.read_datx_delay[i] = l_end - g_tune_para.read_max_window[i] / 2;
                }
            } else {
#ifdef ETT_TEST_DBG_LOG
                sys_print("[%s %d]read window not find\n", __func__, __LINE__);
#endif
            }
        } while (l_read_datx_sampl_edge[i]++ < 1);

        /* window not observe, need tuning clock paramters */ 
        if (g_tune_para.read_max_window[i] == 0){
            l_int_data_latch_cksel = 0;
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0);
            do { 
                MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);

                l_ckgen_msdc_dly_sel = 0;
                MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0);
                do { 
                    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);

                    l_read_pin = 1 << i;
                    l_read_datx_sampl_edge[i] = 0;
                    MSDC_SET_FIELD(MSDC_IOCON, 0x1 << (16 + i), 0);   
                    do { 
                        MSDC_SET_FIELD(MSDC_IOCON, 0x1 << (16 + i), l_read_datx_sampl_edge[i]);   

                        l_count = 0;
                        l_end = 0;
                        l_max_count = 0;
                        l_read_datx_delay[i] = 0;
                        MSDC_WRITE32(MSDC_DAT_RDDLY0, 0x0); 
                        MSDC_WRITE32(MSDC_DAT_RDDLY1, 0x0);  
                        do {
                            if (i < 4) {
                                MSDC_SET_FIELD(MSDC_DAT_RDDLY0, 0x1f << ((3-i)*8), l_read_datx_delay[i]);   
                            } else {
                                MSDC_SET_FIELD(MSDC_DAT_RDDLY1, 0x1f << ((7-i)*8), l_read_datx_delay[i]);   
                            }

                            if (cmd.arg > ETT_TEST_MAZ_ADDR){
                                cmd.arg = ETT_TEST_START_ADDR - 88; 
                            }

                            cmd.arg += 88;  //avoid read the fixed blocks
                            l_result = mmc_rw_test(host, card, &cmd, l_read_pin);
                            g_tune_dbg.read_count++;
                            if (l_result == MMC_ERR_NONE){
                                g_tune_dbg.read_passed++;
                                if ((++l_count) > l_max_count){
                                    l_max_count = l_count;
                                    l_end = l_read_datx_delay[i];
                                }
                            } else {
                                g_tune_dbg.read_failed++;
                                l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                                sys_print("[%s %d]read test failed(%d 0x%x)\n", __func__, __LINE__, l_result, g_tune_dbg.dcrc);
#endif
                            }

#ifdef ETT_SHOW_PASS_WINDOW
                            sys_print("%c(0x%x) ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'), g_tune_dbg.dcrc);
#else
                            sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\n", g_tune_dbg.read_count, l_result, l_int_data_latch_cksel, 
                                    l_ckgen_msdc_dly_sel,
                                    l_read_datx_sampl_edge[i],
                                    l_read_datx_delay[i]);
#endif
                            g_tune_dbg.dcrc = 0;

                        } while (l_read_datx_delay[i]++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                        sys_print("r:%d(%d\t%d)\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end, l_int_data_latch_cksel, 
                                l_ckgen_msdc_dly_sel,
                                l_read_datx_sampl_edge[i]);
#endif
                        /* l_max_count will be 0, 1, ... 32 */
                        if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                            sys_print("[%s %d]read window find(%d)\n", __func__, __LINE__, g_tune_dbg.read_count);
#endif
                            //update the max real window
                            if (l_max_count > g_tune_para.read_max_window[i]){  // new window observe
                                g_tune_para.read_max_window[i] = l_max_count;

                                g_tune_para.int_data_latch_cksel = l_int_data_latch_cksel;  //clock relate
                                g_tune_para.ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;
                                g_tune_para.read_datx_sampl_edge[i] = l_read_datx_sampl_edge[i]; //pad relate
                                g_tune_para.read_datx_delay[i] = l_end - g_tune_para.read_max_window[i] / 2;
                            }
                        } else {
#ifdef ETT_TEST_DBG_LOG
                            sys_print("[%s %d]read window not find\n", __func__, __LINE__);
#endif
                        }
                    } while (l_read_datx_sampl_edge[i]++ < 1);
                } while (l_ckgen_msdc_dly_sel++ < 31);
            } while (l_int_data_latch_cksel++ < 7);
        }

        //print each test result
        sys_print("count  = %d failed = %d, passed = %d max_window = %d\n", g_tune_dbg.read_count, g_tune_dbg.read_failed, g_tune_dbg.read_passed, g_tune_para.read_max_window[i]);

        sys_print("read    paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", g_tune_para.int_data_latch_cksel);
        sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", g_tune_para.ckgen_msdc_dly_sel);
        sys_print("               MSDC_IOCON_D%dSPL         = %d\n", i, g_tune_para.read_datx_sampl_edge[i]);
        sys_print("               MSDC_DAT_RDDLY%d_D%d      = %d\n", (i < 4) ? 0 : 1, i, g_tune_para.read_datx_delay[i]);

        g_tune_dbg.read_count = 0;
        g_tune_dbg.read_failed = 0; 
        g_tune_dbg.read_passed = 0;
    }

    //print final result
    sys_print("data[0] max_window = %d\n", g_tune_para.read_max_window[0]);
    sys_print("data[1] max_window = %d\n", g_tune_para.read_max_window[1]);
    sys_print("data[2] max_window = %d\n", g_tune_para.read_max_window[2]);
    sys_print("data[3] max_window = %d\n", g_tune_para.read_max_window[3]);
    sys_print("data[4] max_window = %d\n", g_tune_para.read_max_window[4]);
    sys_print("data[5] max_window = %d\n", g_tune_para.read_max_window[5]);
    sys_print("data[6] max_window = %d\n", g_tune_para.read_max_window[6]);
    sys_print("data[7] max_window = %d\n", g_tune_para.read_max_window[7]);

    sys_print("               MSDC_IOCON_D0SPL          = %d\n", g_tune_para.read_datx_sampl_edge[0]);
    sys_print("               MSDC_IOCON_D1SPL          = %d\n", g_tune_para.read_datx_sampl_edge[1]);
    sys_print("               MSDC_IOCON_D2SPL          = %d\n", g_tune_para.read_datx_sampl_edge[2]);
    sys_print("               MSDC_IOCON_D3SPL          = %d\n", g_tune_para.read_datx_sampl_edge[3]);
    sys_print("               MSDC_IOCON_D4SPL          = %d\n", g_tune_para.read_datx_sampl_edge[4]);
    sys_print("               MSDC_IOCON_D5SPL          = %d\n", g_tune_para.read_datx_sampl_edge[5]);
    sys_print("               MSDC_IOCON_D6SPL          = %d\n", g_tune_para.read_datx_sampl_edge[6]);
    sys_print("               MSDC_IOCON_D7SPL          = %d\n", g_tune_para.read_datx_sampl_edge[7]);
    sys_print("               MSDC_DAT_RDDLY0_D0        = %d\n", g_tune_para.read_datx_delay[0]);
    sys_print("               MSDC_DAT_RDDLY0_D1        = %d\n", g_tune_para.read_datx_delay[1]);
    sys_print("               MSDC_DAT_RDDLY0_D2        = %d\n", g_tune_para.read_datx_delay[2]);
    sys_print("               MSDC_DAT_RDDLY0_D3        = %d\n", g_tune_para.read_datx_delay[3]);
    sys_print("               MSDC_DAT_RDDLY1_D4        = %d\n", g_tune_para.read_datx_delay[4]);
    sys_print("               MSDC_DAT_RDDLY2_D5        = %d\n", g_tune_para.read_datx_delay[5]);
    sys_print("               MSDC_DAT_RDDLY3_D6        = %d\n", g_tune_para.read_datx_delay[6]);
    sys_print("               MSDC_DAT_RDDLY4_D7        = %d\n", g_tune_para.read_datx_delay[7]);

#else  // ETT_SPILT_TUNE end

    l_read_dat_sampl_edge = 0;
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, 0);   
    do { 
        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, l_read_dat_sampl_edge);   

        l_count = 0;
        l_end = 0;
        l_max_count = 0;
        l_read_dat_delay = 0;
        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, 0x0); 
        do {
            MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, l_read_dat_delay); 

            if (cmd.arg > ETT_TEST_MAZ_ADDR){
                cmd.arg = ETT_TEST_START_ADDR - 88; 
            }

            cmd.arg += 88;  //avoid read the fixed blocks
            l_result = mmc_rw_test(host, card, &cmd, 0xFF);
            g_tune_dbg.read_count++;
            if (l_result == MMC_ERR_NONE){
                g_tune_dbg.read_passed++;
                if ((++l_count) > l_max_count){
                    l_max_count = l_count;
                    l_end = l_read_dat_delay;
                }
            } else {
                g_tune_dbg.read_failed++;
                l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                sys_print("[%s %d]read test failed(%d)\n", __func__, __LINE__, l_result);
#endif
            }

#ifdef ETT_SHOW_PASS_WINDOW
            sys_print("%c(0x%x) ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'), g_tune_dbg.dcrc);
#else
            sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\n", g_tune_dbg.read_count, l_result, l_int_data_latch_cksel, 
                    l_ckgen_msdc_dly_sel,
                    l_read_dat_sampl_edge,
                    l_read_dat_delay);
#endif
            g_tune_dbg.dcrc = 0;

        } while (l_read_dat_delay++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
        sys_print("r:%d(%d\t%d)\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end, l_int_data_latch_cksel, 
                l_ckgen_msdc_dly_sel,
                l_read_dat_sampl_edge);
#endif

        /* l_max_count will be 0, 1, ... 32 */
        if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
            sys_print("[%s %d]read window find(%d)\n", __func__, __LINE__, g_tune_dbg.read_count);
#endif
            //update the max real window
            if (l_max_count > g_tune_para.read_max_window){  // new window observe
                g_tune_para.read_max_window = l_max_count;

                g_tune_para.read_dat_sampl_edge = l_read_dat_sampl_edge; //pad relate
                g_tune_para.read_dat_delay = l_end - g_tune_para.read_max_window / 2;
            }
        } else {
#ifdef ETT_TEST_DBG_LOG
            sys_print("[%s %d]read window not find\n", __func__, __LINE__);
#endif
        }
    } while (l_read_dat_sampl_edge++ < 1);

    /* window not observe, need tuning clock paramters */ 
    if (g_tune_para.read_max_window == 0 ){
    	sys_print("yf test max read window == 0\n");
    	if(g_msdc_ett_debug1)
    	{
    		return;
    	}
        l_int_data_latch_cksel = 0;
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0);
        do { 
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);

            l_ckgen_msdc_dly_sel = 0;
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0);
            do { 
                MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);

                l_read_dat_sampl_edge = 0;
                MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, 0);   
                do { 
                    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, l_read_dat_sampl_edge);   

                    l_count = 0;
                    l_end = 0;
                    l_max_count = 0;
                    l_read_dat_delay = 0;
                    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, 0x0); 
                    do {
                        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATRRDLY, l_read_dat_delay); 

                        if (cmd.arg > ETT_TEST_MAZ_ADDR){
                            cmd.arg = ETT_TEST_START_ADDR - 88; 
                        }

                        cmd.arg += 88;  //avoid read the fixed blocks
                        l_result = mmc_rw_test(host, card, &cmd, 0xFF);
                        g_tune_dbg.read_count++;
                        if (l_result == MMC_ERR_NONE){
                            g_tune_dbg.read_passed++;
                            if ((++l_count) > l_max_count){
                                l_max_count = l_count;
                                l_end = l_read_dat_delay;
                            }
                        } else {
                            g_tune_dbg.read_failed++;
                            l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                            sys_print("[%s %d]read test failed(%d)\n", __func__, __LINE__, l_result);
#endif
                        }

#ifdef ETT_SHOW_PASS_WINDOW
                        sys_print("%c(0x%x) ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'), g_tune_dbg.dcrc);
#else
                        sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\n", g_tune_dbg.read_count, l_result, l_int_data_latch_cksel, 
                                l_ckgen_msdc_dly_sel,
                                l_read_dat_sampl_edge,
                                l_read_dat_delay);
#endif
                        g_tune_dbg.dcrc = 0;
                    } while (l_read_dat_delay++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                    sys_print("r:%d(%d\t%d)\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end, l_int_data_latch_cksel, 
                            l_ckgen_msdc_dly_sel,
                            l_read_dat_sampl_edge);
#endif

                    /* l_max_count will be 0, 1, ... 32 */
                    if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]step2: read window find(%d)\n", __func__, __LINE__, g_tune_dbg.read_count);
#endif
                        //update the max real window
                        if (l_max_count > g_tune_para.read_max_window){  // new window observe
                            g_tune_para.read_max_window = l_max_count; 

                            g_tune_para.int_data_latch_cksel = l_int_data_latch_cksel;  //clock relate
                            g_tune_para.ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;
                            g_tune_para.read_dat_sampl_edge = l_read_dat_sampl_edge; //pad relate
                            g_tune_para.read_dat_delay = l_end - g_tune_para.read_max_window / 2;
                        }
                    } else {
#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]step2: read window not find\n", __func__, __LINE__);
#endif
                    }
                } while (l_read_dat_sampl_edge++ < 1);
            } while (l_ckgen_msdc_dly_sel++ < 31);
        } while (l_int_data_latch_cksel++ < 7);
    }

    //print final result
    sys_print("count  = %d failed = %d, passed = %d max_window = %d\n", g_tune_dbg.read_count, g_tune_dbg.read_failed, g_tune_dbg.read_passed, g_tune_para.read_max_window);

    sys_print("read    paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", g_tune_para.int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", g_tune_para.ckgen_msdc_dly_sel);
    sys_print("               MSDC_IOCON_DSPL           = %d\n", g_tune_para.read_dat_sampl_edge);
    sys_print("               MSDC_PAD_TUNE_DATRRDLY    = %d\n", g_tune_para.read_dat_delay);

#endif


}





