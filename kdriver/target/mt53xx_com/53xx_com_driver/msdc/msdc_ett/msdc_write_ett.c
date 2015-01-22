/* do write parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "x_timer.h"
#include "msdc_ett.h"
#include "msdc.h"

         u32 g_int_data_latch_cksel;
         u32 g_ckgen_msdc_dly_sel;
         u32 g_write_crcs_ta_cntr;
         u32 g_write_pad_tune_datwrdly;
         u32 g_write_w_d0_smpl;
         u32 g_write_dat0_rddly;
         
extern u32 g_msdc_ett_debug1;
extern u32 g_msdc_ett_debug1_dat_latch_ck_sel;
extern u32 g_msdc_ett_debug1_ckgen_dly_sel;

void ett_write_test(void)
{
    int l_int_data_latch_cksel = 0;   //clock timing relate
    int l_ckgen_msdc_dly_sel = 0;     
    
    int l_write_crcs_ta_cntr = 0;   //pad relate
    int l_write_pad_tune_datwrdly = 0;   
    int l_write_w_d0_smpl = 0;      // bit10, just for crc status on dat0 
    int l_write_dat0_rddly = 0;

    int l_result = 0, l_count = 0, l_max_count = 0, l_end = 0;

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

    g_tune_dbg.write_count = 0;
    g_tune_dbg.write_passed = 0;
    g_tune_dbg.write_failed = 0;
    g_tune_para.write_max_window = 0;

    if (g_msdc_ett_debug == 0){
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


        /* set to default value */
        MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, 0);
        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, 0);

        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 0x1); // each data line has its own delay slection value
        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, 0x0);    // rising

        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DDLSEL, 0x1);  // each data line has its own delay slection value
        MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, 0x0);  
    } else {
        /* set to the value, user want debug */
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, g_int_data_latch_cksel);
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, g_ckgen_msdc_dly_sel);

        /* set to default value */
        MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, g_write_crcs_ta_cntr);
        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, g_write_pad_tune_datwrdly);

        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 0x1); // each data line has its own delay slection value
        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, g_write_w_d0_smpl);    // rising

        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DDLSEL, 0x1);  // each data line has its own delay slection value
        MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, g_write_dat0_rddly);  
    }

    /* get current releate paras */
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);
    MSDC_GET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, l_write_crcs_ta_cntr);
    MSDC_GET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, l_write_pad_tune_datwrdly);
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, l_write_w_d0_smpl);    
    MSDC_GET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, l_write_dat0_rddly);  

#if ETT_STRIP_CODE
    sys_print("write    paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", l_int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", l_ckgen_msdc_dly_sel);
    sys_print("               MSDC_WRDAT_CRCS_TA_CNTR   = %d\n", l_write_crcs_ta_cntr);
    sys_print("               MSDC_PAD_TUNE_DATWRDLY    = %d\n", l_write_pad_tune_datwrdly);
    sys_print("               MSDC_IOCON_WD0_SMPL       = %d\n", l_write_w_d0_smpl);
    sys_print("               MSDC_DAT_RDDLY0_D0        = %d\n", l_write_dat0_rddly);
#endif 
    /* send cmd25, not to try tuning */ 
    cmd.opcode  = MMC_CMD_WRITE_MULTIPLE_BLOCK;
    cmd.arg = ETT_TEST_START_ADDR - 88;  // offset is 1GB 
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    if (g_msdc_ett_debug == 0){
    /* MSDC_WRDAT_CRCS_TA_CNTR default value is 1, tune from the 1 */
    l_write_crcs_ta_cntr = 1;
    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, 0);
    do {
        MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, l_write_crcs_ta_cntr);

        l_write_pad_tune_datwrdly = 0;
        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, 0);
        do {
            MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, l_write_pad_tune_datwrdly);

            l_write_w_d0_smpl = 0;
            MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, 0x0);    // rising
            do { 
                MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, l_write_w_d0_smpl);   

                l_count = 0;
                l_end = 0;
                l_max_count = 0;
                l_write_dat0_rddly = 0;
                MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, 0x0);  
                do {
                    MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, l_write_dat0_rddly);  

                    if (cmd.arg > ETT_TEST_MAZ_ADDR){
                        cmd.arg = ETT_TEST_START_ADDR - 88; 
                    }
                    
                    cmd.arg += 88;  //avoid write the fixed blocks
                    l_result = mmc_rw_test(host, card, &cmd, 0xFF);
                    g_tune_dbg.write_count++;
                    if (l_result == MMC_ERR_NONE){
                        g_tune_dbg.write_passed++;
                        if ((++l_count) > l_max_count){
                            l_max_count = l_count;
                            l_end = l_write_dat0_rddly;
                        }
                    } else {
                        g_tune_dbg.write_failed++;
                        l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]write test failed(%d)\n", __func__, __LINE__, l_result);
#endif
                    }

#ifdef ETT_SHOW_PASS_WINDOW
                    sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
#else
                    sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\t%d\t%d\n", g_tune_dbg.write_count, l_result, l_int_data_latch_cksel, 
                                                                                   l_ckgen_msdc_dly_sel,
                                                                                   l_write_crcs_ta_cntr,
                                                                                   l_write_pad_tune_datwrdly,
                                                                                   l_write_w_d0_smpl,
                                                                                   l_write_dat0_rddly);
#endif
                } while (l_write_dat0_rddly++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                sys_print("w:%d(%d\t%d)\t%d\t%d\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end,  l_int_data_latch_cksel, 
                                                                                                                                  l_ckgen_msdc_dly_sel,
                                                                                                                                  l_write_crcs_ta_cntr,
                                                                                                                                  l_write_pad_tune_datwrdly,
                                                                                                                                  l_write_w_d0_smpl);
#endif

                /* l_max_count will be 0, 1, ... 32 */
                if (l_max_count > ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]write window find(%d)\n", __func__, __LINE__, g_tune_dbg.write_count);
#endif
                    //update the max real window
                    if (l_max_count > g_tune_para.write_max_window){
                        g_tune_para.write_max_window = l_max_count;

                        g_tune_para.write_crcs_ta_cntr = l_write_crcs_ta_cntr; //pad relate
                        g_tune_para.write_pad_tune_datwrdly = l_write_pad_tune_datwrdly;
                        g_tune_para.write_w_d0_smpl = l_write_w_d0_smpl;
                        g_tune_para.write_dat0_rddly = l_end - g_tune_para.write_max_window / 2;
                        g_tune_para.int_data_latch_cksel = l_int_data_latch_cksel;  //clock relate
                        g_tune_para.ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;
                    }
                } else {
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]write window not find(%d)\n", __func__, __LINE__, g_tune_dbg.write_count);
#endif
                }
            } while (l_write_w_d0_smpl++ < 1);
        } while (l_write_pad_tune_datwrdly++ < 31);
    } while (l_write_crcs_ta_cntr++ < 7);

    /* window not observe, need tuning clock paramters */ 
    if (g_tune_para.write_max_window == 0){
    	    sys_print("[%s %d]write window not find(%d)\n", __func__, __LINE__, g_tune_dbg.write_count);
        l_int_data_latch_cksel = 0;
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0);
        do { 
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);

            l_ckgen_msdc_dly_sel = 0;
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0);
            do { 
                MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);

                l_write_crcs_ta_cntr = 1;
                MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, 0);
                do {
                    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, l_write_crcs_ta_cntr);

                    l_write_pad_tune_datwrdly = 0;
                    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, 0);
                    do {
                        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, l_write_pad_tune_datwrdly);

                        l_write_w_d0_smpl = 0;
                        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, 0x0);    // rising
                        do { 
                            MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, l_write_w_d0_smpl);   

                            l_count = 0;
                            l_end = 0;
                            l_max_count = 0;
                            l_write_dat0_rddly = 0;
                            MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, 0x0);  
                            do {
                                MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, l_write_dat0_rddly);  

                                if (cmd.arg > ETT_TEST_MAZ_ADDR){
                                    cmd.arg = ETT_TEST_START_ADDR - 8; 
                                }

                                cmd.arg += 88;  //avoid write the fixed blocks
                                l_result = mmc_rw_test(host, card, &cmd, 0xFF);
                                g_tune_dbg.write_count++;
                                if (l_result == MMC_ERR_NONE){
                                    g_tune_dbg.write_passed++;
                                    if ((++l_count) > l_max_count){
                                        l_max_count = l_count;
                                        l_end = l_write_dat0_rddly;
                                    }
                                } else {
                                    g_tune_dbg.write_failed++;
                                    l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                                    sys_print("[%s %d]step2: write test failed(%d)\n", __func__, __LINE__, l_result);
#endif
                                }
#ifdef ETT_SHOW_PASS_WINDOW
                                sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));

#else
                                sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\t%d\t%d\n", g_tune_dbg.write_count, l_result, l_int_data_latch_cksel, 
                                                                                               l_ckgen_msdc_dly_sel,
                                                                                               l_write_crcs_ta_cntr,
                                                                                               l_write_pad_tune_datwrdly,
                                                                                               l_write_w_d0_smpl,
                                                                                               l_write_dat0_rddly);
#endif

                            } while (l_write_dat0_rddly++ < 31);
#ifdef ETT_SHOW_PASS_WINDOW
                            sys_print("w:%d(%d\t%d)\t%d\t%d\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end,  l_int_data_latch_cksel, 
                                                                                                                                  l_ckgen_msdc_dly_sel,
                                                                                                                                  l_write_crcs_ta_cntr,
                                                                                                                                  l_write_pad_tune_datwrdly,
                                                                                                                                  l_write_w_d0_smpl);
#endif
                            /* l_max_count will be 0, 1, ... 32 */
                            if (l_max_count > ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                                sys_print("[%s %d]step2: write window find(%d)\n", __func__, __LINE__, g_tune_dbg.write_count);
#endif

                                //update the max real window
                                if (l_max_count > g_tune_para.write_max_window){
                                    g_tune_para.write_max_window = l_max_count;

                                    g_tune_para.int_data_latch_cksel = l_int_data_latch_cksel;  //clock relate
                                    g_tune_para.ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;
                                    g_tune_para.write_crcs_ta_cntr = l_write_crcs_ta_cntr; //pad relate
                                    g_tune_para.write_pad_tune_datwrdly = l_write_pad_tune_datwrdly;
                                    g_tune_para.write_w_d0_smpl = l_write_w_d0_smpl;
                                    g_tune_para.write_dat0_rddly = l_end - g_tune_para.write_max_window / 2;
                                }
                            } else {
#ifdef ETT_TEST_DBG_LOG
                                sys_print("[%s %d]step2: write window not find\n", __func__, __LINE__);
#endif
                            }
                        } while (l_write_w_d0_smpl++ < 1);
                    } while (l_write_pad_tune_datwrdly++ < 31);
                } while (l_write_crcs_ta_cntr++ < 7);
            } while (l_ckgen_msdc_dly_sel++ < 31);
        } while (l_int_data_latch_cksel++ < 7);
    }
    } else {
       /* debug mode */ 
        do {
            if (cmd.arg > ETT_TEST_MAZ_ADDR){
                cmd.arg = ETT_TEST_START_ADDR - 88; 
            }

            cmd.arg += 88;  //avoid write the fixed blocks
            l_result = mmc_rw_test(host, card, &cmd, 0xFF);
            g_tune_dbg.write_count++;
            if (l_result == MMC_ERR_NONE){
                g_tune_dbg.write_passed++;
            } else {
                g_tune_dbg.write_failed++;
            }
            sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));

            if (g_tune_dbg.write_count % 32){
                sys_print("\n");
            }

            /* delay sometime for next write test */
            CTP_Wait_msec(100000);
        } while (1);
    }

    //print final result
    sys_print("count = %d failed = %d, passed = %d, max_window = %d\n", g_tune_dbg.write_count, g_tune_dbg.write_failed, g_tune_dbg.write_passed, g_tune_para.write_max_window);

    sys_print("write   paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", g_tune_para.int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", g_tune_para.ckgen_msdc_dly_sel);
    sys_print("               MSDC_WRDAT_CRCS_TA_CNTR   = %d\n", g_tune_para.write_crcs_ta_cntr);
    sys_print("               MSDC_PAD_TUNE_DATWRDLY    = %d\n", g_tune_para.write_pad_tune_datwrdly);
    sys_print("               MSDC_IOCON_WD0_SMPL       = %d\n", g_tune_para.write_w_d0_smpl);
    sys_print("               MSDC_DAT_RDDLY0_D0        = %d\n", g_tune_para.write_dat0_rddly);
    
}









