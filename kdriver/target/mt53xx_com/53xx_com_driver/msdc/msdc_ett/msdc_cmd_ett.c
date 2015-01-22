
/* do command parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "msdc_ett.h"
#include "msdc.h"

extern struct mmc_host ett_sd_host;
extern struct mmc_card ett_sd_card;
extern u32 g_msdc_ett_debug1;
extern u32 g_msdc_ett_debug1_dat_latch_ck_sel;
extern u32 g_msdc_ett_debug1_ckgen_dly_sel;
extern u32 maxclock;

void ett_cmd_test(void)
{
    int l_int_data_latch_cksel = 0;   //clock timing relate
    int l_ckgen_msdc_dly_sel = 0;     
    int l_cmd_rsp_ta_cntr = 0;        //pad relate
    int l_pad_cmd_rsp_rxdel = 0;
    int l_iocon_rspl = 0;
    int l_pad_tune_cmdrrdly = 0;
    int l_end = 0;

    int l_result = 0, l_count = 0, l_max_count = 0;

    struct mmc_host *host;
    struct mmc_card *card;

    struct mmc_command cmd;

    host = &ett_sd_host;
    card = &ett_sd_card;

    ett_memset(&cmd, sizeof(struct mmc_command), 0x0);

    /* set to default value */
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0x1);
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0x0);
    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, 0x0);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, 0x0);
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, 0x0);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, 0x0);
    g_tune_dbg.cmd_count = 0;
    g_tune_dbg.cmd_passed = 0;
    g_tune_dbg.cmd_failed = 0;
    g_tune_para.cmd_max_window = 0;
    
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
    

    
    /* get current releate paras */
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
    MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);
    MSDC_GET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, l_cmd_rsp_ta_cntr);
    MSDC_GET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, l_pad_cmd_rsp_rxdel);
    MSDC_GET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, l_iocon_rspl);
    MSDC_GET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, l_pad_tune_cmdrrdly);

#if ETT_STRIP_CODE
    sys_print("command paras: MSDC_INT_DAT_LATCH_CK_SEL = %d\n", l_int_data_latch_cksel);
    sys_print("               MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", l_ckgen_msdc_dly_sel);
    sys_print("               MSDC_CMD_RSP_TA_CNTR      = %d\n", l_cmd_rsp_ta_cntr);
    sys_print("               MSDC_PAD_TUNE_CMDRRDLY    = %d\n", l_pad_cmd_rsp_rxdel);
    sys_print("               MSDC_IOCON_RSPL           = %d\n", l_iocon_rspl);
    sys_print("               MSDC_PAD_TUNE_CMDRDLY     = %d\n", l_pad_tune_cmdrrdly);
#endif
    /* comand test flow */
    
    /* update paras */
    
    /* send cmd13, not to try tuning */ 
    cmd.opcode  = MMC_CMD_SEND_STATUS;
    cmd.arg     = card->rca << 16;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    /* MSDC_CMD_RSP_TA_CNTR default value is 1, tune from the 1 */
    l_cmd_rsp_ta_cntr = 1;
    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, 0x0);
    do {
        MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, l_cmd_rsp_ta_cntr);
        
        l_pad_cmd_rsp_rxdel = 0;
        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, 0x0);
        do {
            MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, l_pad_cmd_rsp_rxdel);
            
            l_iocon_rspl = 0;
            MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, 0x0);
            do {
                MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, l_iocon_rspl);

                l_count = 0;
                l_end = 0;
                l_max_count = 0;
                l_pad_tune_cmdrrdly = 0;
                MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, 0x0);
                do {
                    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, l_pad_tune_cmdrrdly);

                    l_result = mmc_cmd(host, &cmd);
                    g_tune_dbg.cmd_count++;
                    if (l_result == MMC_ERR_NONE){
                        g_tune_dbg.cmd_passed++;
                        if ((++l_count) > l_max_count){
                            l_max_count = l_count;
                            l_end = l_pad_tune_cmdrrdly;
                        }

                    } else {
                        g_tune_dbg.cmd_failed++;
                        l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]command test failed(%d)\n", __func__, __LINE__, l_result);
#endif
                    }

#ifdef ETT_SHOW_PASS_WINDOW
                    sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
#else
                    sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\t%d\t%d\n", g_tune_dbg.cmd_count, l_result, l_int_data_latch_cksel, 
                                                                                       l_ckgen_msdc_dly_sel,
                                                                                       l_cmd_rsp_ta_cntr,
                                                                                       l_pad_cmd_rsp_rxdel,
                                                                                       l_iocon_rspl,
                                                                                       l_pad_tune_cmdrrdly);
#endif 
                } while (l_pad_tune_cmdrrdly++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                sys_print("c:%d(%d\t%d)\t%d\t%d\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end,  l_int_data_latch_cksel, 
                                                                                         l_ckgen_msdc_dly_sel,
                                                                                         l_cmd_rsp_ta_cntr,
                                                                                         l_pad_cmd_rsp_rxdel,
                                                                                         l_iocon_rspl);
#endif

                /* l_max_count will be 0, 1, ... 32 */
                if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]cmd window find(%d)\n", __func__, __LINE__, g_tune_dbg.cmd_count);
#endif
                    //update the max real window
                    if (l_max_count > g_tune_para.cmd_max_window){  // new window observe
                        g_tune_para.cmd_max_window = l_max_count;    

                        g_tune_para.cmd_rsp_ta_cntr = l_cmd_rsp_ta_cntr;        //pad relate
                        g_tune_para.pad_cmd_rsp_rxdel = l_pad_cmd_rsp_rxdel;
                        g_tune_para.iocon_rspl = l_iocon_rspl;
                        g_tune_para.cmd_int_data_latch_cksel = l_int_data_latch_cksel;        // clock relate
                        g_tune_para.cmd_ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;    
                        g_tune_para.pad_tune_cmdrrdly = l_end - g_tune_para.cmd_max_window / 2;
                    }
                } else {
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]cmd window not find\n", __func__, __LINE__);
#endif
                }
            } while (l_iocon_rspl++ < 1);
        } while (l_pad_cmd_rsp_rxdel++ < 31);
    } while (l_cmd_rsp_ta_cntr++ < 7);

#if 1
    /* window not observe, need tuning clock paramters */ 
    if (g_tune_para.cmd_max_window == 0){
    	    sys_print("window not observe, need tuning clock paramters\n");
        l_int_data_latch_cksel = 0;
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, 0); do { 
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
            

            l_ckgen_msdc_dly_sel = 0;
            MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, 0);
            do { 
                MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);

                l_cmd_rsp_ta_cntr = 1;
                MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, 0x0);
                do {
                    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, l_cmd_rsp_ta_cntr);

                    l_pad_cmd_rsp_rxdel = 0;
                    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, 0x0);
                    do {
                        MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, l_pad_cmd_rsp_rxdel);

                        l_iocon_rspl = 0;
                        MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, 0x0);
                        do {
                            MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, l_iocon_rspl);

                            l_count = 0;
                            l_end = 0;
                            l_max_count = 0;
                            l_pad_tune_cmdrrdly = 0;
                            MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, 0x0);
                            do {
                                MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, l_pad_tune_cmdrrdly);

                                l_result = mmc_cmd(host, &cmd);
                                g_tune_dbg.cmd_count++;
                                if (l_result == MMC_ERR_NONE){
                                    g_tune_dbg.cmd_passed++;
                                    if ((++l_count) > l_max_count){
                                        l_max_count = l_count;
                                        l_end = l_pad_tune_cmdrrdly;
                                    }

                                } else {
                                    g_tune_dbg.cmd_failed++;
                                    l_count = 0;
#ifdef ETT_TEST_DBG_LOG
                                    sys_print("[%s %d]command test failed(%d)\n", __func__, __LINE__, l_result);
#endif
                                }

#ifdef ETT_SHOW_PASS_WINDOW
                                sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
#else
                                sys_print("(%d\t%d)\t%d\t%d\t%d\t%d\t%d\t%d\n", g_tune_dbg.cmd_count, l_result, l_int_data_latch_cksel, 
                                                                                         l_ckgen_msdc_dly_sel,
                                                                                         l_cmd_rsp_ta_cntr,
                                                                                         l_pad_cmd_rsp_rxdel,
                                                                                         l_iocon_rspl,
                                                                                         l_pad_tune_cmdrrdly);
#endif
                            } while (l_pad_tune_cmdrrdly++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                            sys_print("c:%d(%d\t%d)\t%d\t%d\t%d\t%d\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end,  l_int_data_latch_cksel, 
                                                                                         l_ckgen_msdc_dly_sel,
                                                                                         l_cmd_rsp_ta_cntr,
                                                                                         l_pad_cmd_rsp_rxdel,
                                                                                         l_iocon_rspl);
#endif

                            /* l_max_count will be 0, 1, ... 32 */
                            if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                                sys_print("[%s %d]step2: cmd window find(%d)\n", __func__, __LINE__, g_tune_dbg.cmd_count);
#endif

                                //update the max real window
                                if (l_max_count > g_tune_para.cmd_max_window){  // new window observe
                                    g_tune_para.cmd_max_window = l_max_count;

                                    g_tune_para.cmd_int_data_latch_cksel = l_int_data_latch_cksel;        // clock relate
                                    g_tune_para.cmd_ckgen_msdc_dly_sel = l_ckgen_msdc_dly_sel;        
                                    
                                    g_tune_para.cmd_rsp_ta_cntr = l_cmd_rsp_ta_cntr;        //pad relate
                                    g_tune_para.pad_cmd_rsp_rxdel = l_pad_cmd_rsp_rxdel;
                                    g_tune_para.iocon_rspl = l_iocon_rspl;
                                    g_tune_para.pad_tune_cmdrrdly = l_end - g_tune_para.cmd_max_window / 2;
                                }
                            } else {
#ifdef ETT_TEST_DBG_LOG
                                sys_print("[%d %s]step2: cmd window not find\n", __func__, __LINE__);
#endif
                            }
                        } while (l_iocon_rspl++ < 1);
                    } while (l_pad_cmd_rsp_rxdel++ < 31);
                } while (l_cmd_rsp_ta_cntr++ < 7);

            } while (l_ckgen_msdc_dly_sel++ < 31);
        } while (l_int_data_latch_cksel++ < 7);
    }
#endif 

    //print final result
    sys_print("count  = %d failed = %d, passed = %d max_window = %d\n", g_tune_dbg.cmd_count, g_tune_dbg.cmd_failed, g_tune_dbg.cmd_passed, g_tune_para.cmd_max_window);
    sys_print("command paras(%d): MSDC_INT_DAT_LATCH_CK_SEL = %d\n", l_max_count, g_tune_para.cmd_int_data_latch_cksel);
    sys_print("                   MSDC_CKGEN_MSDC_DLY_SEL   = %d\n", g_tune_para.ckgen_msdc_dly_sel);
    sys_print("                   MSDC_CMD_RSP_TA_CNTR      = %d\n", g_tune_para.cmd_rsp_ta_cntr);
    sys_print("                   MSDC_PAD_TUNE_CMDRRDLY    = %d\n", g_tune_para.pad_cmd_rsp_rxdel);
	udelay(10);
    sys_print("                   MSDC_IOCON_RSPL           = %d\n", g_tune_para.iocon_rspl);
    sys_print("                   MSDC_PAD_TUNE_CMDRDLY     = %d\n", g_tune_para.pad_tune_cmdrrdly);

    g_cmd_tune_done  = 1;
}

