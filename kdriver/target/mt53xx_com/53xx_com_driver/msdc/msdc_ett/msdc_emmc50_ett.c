/* do write parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "x_timer.h"
#include "msdc_ett.h"
#include "msdc.h"

static int g_hs400_write_error;

void msdc_get_tune_default_value(void)
{
	MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_CMD_RSP_TA_CNTR,0x01);
	MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_WRDAT_CRCS_TA_CNTR,0x01);
			
	//MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, cur_rsmpl);
	*((volatile uint32*)(MSDC_IOCON))=0x02000000;
	*((volatile uint32*)(MSDC_DAT_RDDLY0))=0x00000000;
	*((volatile uint32*)(MSDC_DAT_RDDLY1))=0x00000000;
	*((volatile uint32*)(MSDC_PAD_TUNE))=0x00000000;
	*((volatile uint32*)(EMMC50_PAD_DAT01_TUNE))=0x00000000;
	*((volatile uint32*)(EMMC50_PAD_DAT23_TUNE))=0x00000000;
	*((volatile uint32*)(EMMC50_PAD_DAT45_TUNE))=0x00000000;
	*((volatile uint32*)(EMMC50_PAD_DAT67_TUNE))=0x00000000;

}


void ett_emmc50_rw_test(void)
{
    int l_ds_dly1 = 0;   //pad relate
    int l_ds_dly3 = 0;     
    
    int l_result = 0, l_count = 0, l_max_count = 0, l_end = 0, l_loop;

    struct mmc_host *host;
    struct mmc_card *card;

    struct mmc_command write_cmd;
    struct mmc_command read_cmd;

    host = &ett_sd_host;
    card = &ett_sd_card;

    ett_memset(&write_cmd, sizeof(struct mmc_command), 0x0);
    ett_memset(&read_cmd, sizeof(struct mmc_command), 0x0);

    /* command tuning first, apply command parameter result base on the test */
    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_CMD_RSP_TA_CNTR, g_tune_para.cmd_rsp_ta_cntr);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRRDLY, g_tune_para.pad_cmd_rsp_rxdel);
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, g_tune_para.iocon_rspl);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_CMDRDLY, g_tune_para.pad_tune_cmdrrdly);

    /* set to default value */
    MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY1, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY3, 0);

    /* get current releate paras */
    MSDC_GET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY1, l_ds_dly1);
    MSDC_GET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY3, l_ds_dly3);

	#if ETT_STRIP_CODE
    sys_print("hs400   paras: MSDC_EMMC50_PAD_DS_TUNE_DLY1 = %d\n", l_ds_dly1);
    sys_print("               MSDC_EMMC50_PAD_DS_TUNE_DLY3 = %d\n", l_ds_dly3);
	#endif 

    /* send cmd25, not to try tuning */ 
    write_cmd.opcode  = MMC_CMD_WRITE_MULTIPLE_BLOCK;
    write_cmd.arg = ETT_TEST_START_ADDR - 88;  // offset is 1GB 
    write_cmd.rsptyp  = RESP_R1;
    write_cmd.retries = 0;
    write_cmd.timeout = CMD_TIMEOUT;

    /* send cmd18, not to try tuning */ 
    read_cmd.opcode  = MMC_CMD_READ_MULTIPLE_BLOCK;
    read_cmd.arg = ETT_TEST_START_ADDR - 88;  // offset is 1GB 
    read_cmd.rsptyp  = RESP_R1;
    read_cmd.retries = 0;
    read_cmd.timeout = CMD_TIMEOUT;


	msdc_get_tune_default_value();

	
    if (g_msdc_ett_debug == 0)
	{
        l_ds_dly1 = 0;
        MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY1, 0);
        do 
		{
            MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY1, l_ds_dly1);

            l_count = 0;
            l_end = 0;
            l_max_count = 0;
            l_ds_dly3 = 0;
            MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY3, 0);
            do 
			{
                MSDC_SET_FIELD(EMMC50_PAD_DS_TUNE, MSDC_EMMC50_PAD_DS_TUNE_DLY3, l_ds_dly3);

                for (l_loop = 0; l_loop < 15; l_loop++)
				{
                    if (read_cmd.arg > ETT_TEST_MAZ_ADDR)
					{
                        read_cmd.arg = ETT_TEST_START_ADDR - 88; 
                    }

                    g_hs400_write_error = 0;

                    //read frist
                    read_cmd.arg += 88;  //avoid write the fixed blocks
                    l_result = mmc_rw_test(host, card, &read_cmd, 0xFF);
                    g_tune_dbg.hs400_count++;
                    if (l_result != MMC_ERR_NONE)
					{
                        g_tune_dbg.hs400_failed++;
                        l_count = 0;
						#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]hs400 read test failed(%d)\n", __func__, __LINE__, l_result);
						#endif
                        goto by_pass_write_test;
                    }                

                    if (write_cmd.arg > ETT_TEST_MAZ_ADDR)
					{
                        write_cmd.arg = ETT_TEST_START_ADDR - 88; 
                    }

                    //write
                    write_cmd.arg += 88;  //avoid write the fixed blocks
                    l_result = mmc_rw_test(host, card, &write_cmd, 0xFF);
                    if (l_result != MMC_ERR_NONE)
					{
                        g_tune_dbg.hs400_failed++;
                        g_hs400_write_error = 1;
                        l_count = 0;
						#ifdef ETT_TEST_DBG_LOG
                        sys_print("[%s %d]hs400 write test failed(%d)\n", __func__, __LINE__, l_result);
						#endif
                        goto by_pass_write_test;
                    }
                }

by_pass_write_test:
                g_tune_dbg.hs400_count++;
                if (l_result == MMC_ERR_NONE)
				{
                    g_tune_dbg.hs400_passed++;
                    if ((++l_count) > l_max_count)
					{
                        l_max_count = l_count;
                        l_end = l_ds_dly3;
                    }
                } 
				else 
                {
                    l_count = 0;
                }


				#ifdef ETT_SHOW_PASS_WINDOW
                /* find out the error occur under read or write? */  
                if (g_hs400_write_error == 1)
				{
                    sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'x' : 't'));
                } 
				else 
                {
                    sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
                }
				#else
                sys_print("(%d\t%d)\t%d\t%d\n", g_tune_dbg.hs400_count, l_result, l_ds_dly1, l_ds_dly3);
				#endif
            } while (l_ds_dly3++ < 32);

			#ifdef ETT_SHOW_PASS_WINDOW
            sys_print("hs400:%d(%d\t%d)\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end,  l_ds_dly1);
			#endif

            /* l_max_count will be 0, 1, ... 32 */
            if (l_max_count > ETT_TUNE_WINDOW)
			{
				#ifdef ETT_TEST_DBG_LOG
                sys_print("[%s %d]write window find(%d)\n", __func__, __LINE__, g_tune_dbg.hs400_count);
				#endif
                //update the max real window
                if (l_max_count > g_tune_para.hs400_max_window)
				{
                    g_tune_para.hs400_max_window = l_max_count;

                    g_tune_para.hs400_ds_dly1 = l_ds_dly1; //pad relate
                    g_tune_para.hs400_ds_dly3 = l_end - g_tune_para.hs400_max_window / 2;
                }
            } 
			else
			{
				#ifdef ETT_TEST_DBG_LOG
                sys_print("[%s %d]write window not find(%d)\n", __func__, __LINE__, g_tune_dbg.hs400_count);
				#endif
            }
        } while (l_ds_dly1++ < 32);

        /* window not observe, need tuning clock paramters */ 
    } 
	else
    {
        /* debug mode */ 
        do 
		{
            if (read_cmd.arg > ETT_TEST_MAZ_ADDR)
			{
                read_cmd.arg = ETT_TEST_START_ADDR - 88; 
            }

            read_cmd.arg += 88;  //avoid read the fixed blocks
            l_result = mmc_rw_test(host, card, &read_cmd, 0xFF);
            g_tune_dbg.hs400_count++;
            if (l_result == MMC_ERR_NONE)
			{
                g_tune_dbg.hs400_passed++;
            } 
			else
            {
                g_tune_dbg.hs400_failed++;
            }
            sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));

            if (g_tune_dbg.hs400_count % 32){
                sys_print("\n");
            }

            /* delay sometime for next write test */
            CTP_Wait_msec(100000);
        } while (1);
    }

    //print final result
    sys_print("count = %d failed = %d, passed = %d, max_window = %d\n", g_tune_dbg.hs400_count, g_tune_dbg.hs400_failed, g_tune_dbg.hs400_passed, g_tune_para.hs400_max_window);

    sys_print("hs400   paras: MSDC_EMMC50_PAD_DS_TUNE_DLY1 = %d\n", g_tune_para.hs400_ds_dly1);
    sys_print("               MSDC_EMMC50_PAD_DS_TUNE_DLY3 = %d\n", g_tune_para.hs400_ds_dly3);
}









