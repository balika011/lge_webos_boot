
/* do command parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "msdc_ett.h"
#include "msdc.h"

extern struct mmc_host ett_sd_host;
extern struct mmc_card ett_sd_card;


void ett_cmd_tx_test(void)
{
    int l_emmc50_pad_cmd_tune_txdly = 0;
    int l_end = 0;

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
    
    /* get current releate paras */
    MSDC_GET_FIELD(EMMC50_PAD_CMD_TUNE, MSDC_EMMC50_PAD_CMD_TUNE_TXDLY, l_emmc50_pad_cmd_tune_txdly);

#if ETT_STRIP_CODE
    sys_print("command paras: MSDC_CMDTXDLY           = %d\n", l_emmc50_pad_cmd_tune_txdly);
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
    l_emmc50_pad_cmd_tune_txdly = 0;
    MSDC_SET_FIELD(EMMC50_PAD_CMD_TUNE, MSDC_EMMC50_PAD_CMD_TUNE_TXDLY, 0);
    do {
        MSDC_SET_FIELD(EMMC50_PAD_CMD_TUNE, MSDC_EMMC50_PAD_CMD_TUNE_TXDLY, l_emmc50_pad_cmd_tune_txdly);

        l_result = mmc_cmd(host, &cmd);
        g_tune_dbg.cmd_tx_count++;
        if (l_result == MMC_ERR_NONE){
            g_tune_dbg.cmd_tx_passed++;
            if ((++l_count) > l_max_count){
                l_max_count = l_count;
                l_end = l_emmc50_pad_cmd_tune_txdly;
            }

        } else {
            g_tune_dbg.cmd_tx_failed++;
            l_count = 0;
#ifdef ETT_TEST_DBG_LOG
            sys_print("[%s %d]command test failed(%d)\n", __func__, __LINE__, l_result);
#endif
        }

#ifdef ETT_SHOW_PASS_WINDOW
        sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
#else
        sys_print("(%d\t%d)\t%d\n", g_tune_dbg.cmd_tx_count, l_result, l_emmc50_pad_cmd_tune_txdly);
#endif 

    } while (l_emmc50_pad_cmd_tune_txdly++ < 31);

#ifdef ETT_SHOW_PASS_WINDOW
                sys_print("c:%d(%d\t%d)\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end);
#endif

                /* l_max_count will be 0, 1, ... 32 */
                if (l_max_count >= ETT_TUNE_WINDOW){
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]cmd window find(%d)\n", __func__, __LINE__, g_tune_dbg.cmd_tx_count);
#endif
                    //update the max real window
                    if (l_max_count > g_tune_para.cmd_tx_max_window){  // new window observe
                        g_tune_para.cmd_tx_max_window = l_max_count;    

                        g_tune_para.emmc50_cmdtxdly = l_end - g_tune_para.cmd_tx_max_window / 2;
                    }
                } else {
#ifdef ETT_TEST_DBG_LOG
                    sys_print("[%s %d]cmd window not find\n", __func__, __LINE__);
#endif
                }


    //print final result
    sys_print("count  = %d failed = %d, passed = %d max_window = %d\n", g_tune_dbg.cmd_tx_count, g_tune_dbg.cmd_tx_failed, g_tune_dbg.cmd_tx_passed, g_tune_para.cmd_max_window);
    sys_print("command paras(%d): MSDC_CMDTXDLY = %d\n", l_max_count, g_tune_para.emmc50_cmdtxdly);
}

