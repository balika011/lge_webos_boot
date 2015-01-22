/* do write parameter select */

//#include "common.h"
#include "types.h"
#include "x_printf.h"
#include "msdc_ett.h"
#include "msdc.h"

void vData_Tx_Set_Value(u8 u1Data_number,u32 u4Value)
{
	u32 u4Register_base;
	u32 u4Mask=0;
	
	switch(u1Data_number)
	{
		case 0:
		{
			u4Register_base=0x11230190;
			u4Mask=(0x1F<<6);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);
		}
		case 1:
		{
			u4Register_base=0x11230190;
			u4Mask=(0x1F<<22);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);

		}	
		case 2:
		{
			u4Register_base=0x11230194;
			u4Mask=(0x1F<<6);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);
		}
		case 3:
		{
			u4Register_base=0x11230194;
			u4Mask=(0x1F<<22);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);

		}	
		case 4:
		{
			u4Register_base=0x11230198;
			u4Mask=(0x1F<<6);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);

		}
		case 5:
		{
			u4Register_base=0x11230198;
			u4Mask=(0x1F<<22);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);

		}	
		case 6:
		{
			u4Register_base=0x1123019c;
			u4Mask=(0x1F<<6);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);

		}
		case 7:
		{
			u4Register_base=0x1123019c;
			u4Mask=(0x1F<<22);
			MSDC_SET_FIELD(u4Register_base,u4Mask,u4Value);
		}		

	}

}

#if 0
void vData_Tx_Tuning(struct mmc_host *host,u32 u4StartBlock,u32 u4BlockNum,uchar *src)
{
	u32 i=0;
	int ret=0;
	
	for(i=0;i<32;i++)
	{
		vData_Tx_Set_Value(0,i);
		vData_Tx_Set_Value(1,i);
		vData_Tx_Set_Value(2,i);
		vData_Tx_Set_Value(3,i);
		vData_Tx_Set_Value(4,i);
		vData_Tx_Set_Value(5,i);
		vData_Tx_Set_Value(6,i);
		vData_Tx_Set_Value(7,i);
		ret = host->blk_write(host, u4StartBlock, src, u4BlockNum);
        if(ret == MMC_ERR_TIMEOUT) 
		{
           printf("N ");
        }
		else if(ret == MMC_ERR_NONE)
		{
			printf("O ");
		}
		else 
		{
			printf("X ");
		}
	}
}
#endif 
static void msdc_get_tune_default_value(void)
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


void ett_write_tx_test(void)
{
    int l_emmc50_dat0_txdly = 0;   
    int l_emmc50_dat1_txdly = 0;   
    int l_emmc50_dat2_txdly = 0;   
    int l_emmc50_dat3_txdly = 0;   
    int l_emmc50_dat4_txdly = 0;   
    int l_emmc50_dat5_txdly = 0;   
    int l_emmc50_dat6_txdly = 0;   
    int l_emmc50_dat7_txdly = 0;   

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

    /* write tuning first, apply write parameter result base on the test */
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, g_tune_para.int_data_latch_cksel);
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, g_tune_para.ckgen_msdc_dly_sel);

    MSDC_SET_FIELD(MSDC_PATCH_BIT1, MSDC_PB1_WRDAT_CRCS_TA_CNTR, g_tune_para.write_crcs_ta_cntr);
    MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_TUNE_DATWRDLY, g_tune_para.write_pad_tune_datwrdly);

    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 0x1); // each data line has its own delay slection value
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, g_tune_para.write_w_d0_smpl);    // rising

    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DDLSEL, 0x1);  // each data line has its own delay slection value
    MSDC_SET_FIELD(MSDC_DAT_RDDLY0, MSDC_DAT_RDDLY0_D0, g_tune_para.write_dat0_rddly);   


	

    g_tune_dbg.write_count = 0;
    g_tune_dbg.write_passed = 0;
    g_tune_dbg.write_failed = 0;
    g_tune_para.write_max_window = 0;

    /* get current releate paras */
    MSDC_GET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT0_TXDLY, l_emmc50_dat0_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT1_TXDLY, l_emmc50_dat1_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT2_TXDLY, l_emmc50_dat2_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT3_TXDLY, l_emmc50_dat3_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT4_TXDLY, l_emmc50_dat4_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT5_TXDLY, l_emmc50_dat5_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT6_TXDLY, l_emmc50_dat6_txdly);
    MSDC_GET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT7_TXDLY, l_emmc50_dat7_txdly);

	#if ETT_STRIP_CODE
    sys_print("write tx paras: MSDC_EMMC50_PAD_DAT0_TXDLY = %d\n", l_emmc50_dat0_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT1_TXDLY = %d\n", l_emmc50_dat1_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT2_TXDLY = %d\n", l_emmc50_dat2_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT3_TXDLY = %d\n", l_emmc50_dat3_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT4_TXDLY = %d\n", l_emmc50_dat4_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT5_TXDLY = %d\n", l_emmc50_dat5_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT6_TXDLY = %d\n", l_emmc50_dat6_txdly);
    sys_print("                MSDC_EMMC50_PAD_DAT7_TXDLY = %d\n", l_emmc50_dat7_txdly);
	#endif 
    /* send cmd25, not to try tuning */ 
    cmd.opcode  = MMC_CMD_WRITE_MULTIPLE_BLOCK;
    cmd.arg = ETT_TEST_START_ADDR - 88;  // offset is 1GB 
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    l_emmc50_dat0_txdly = 0;
    MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT0_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT1_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT2_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT3_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT4_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT5_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT6_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT7_TXDLY, 0);

	msdc_get_tune_default_value();
	
    do 
	{
        MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT0_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT1_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT2_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT3_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT4_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT5_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT6_TXDLY, l_emmc50_dat0_txdly);
        MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT7_TXDLY, l_emmc50_dat0_txdly);

        if (cmd.arg > ETT_TEST_MAZ_ADDR)
		{
            cmd.arg = ETT_TEST_START_ADDR - 88; 
        }

        cmd.arg += 88;  //avoid write the fixed blocks
        l_result = mmc_rw_test(host, card, &cmd, 0xFF);
        g_tune_dbg.write_tx_count++;
        if (l_result == MMC_ERR_NONE)
		{
            g_tune_dbg.write_tx_passed++;
            if ((++l_count) > l_max_count)
			{
                l_max_count = l_count;
                l_end = l_emmc50_dat0_txdly;
            }
        } 
		else 
        {
            g_tune_dbg.write_tx_failed++;
            l_count = 0;
			#ifdef ETT_TEST_DBG_LOG
            sys_print("[%s %d]write test failed(%d)\n", __func__, __LINE__, l_result);
			#endif
        }

		#ifdef ETT_SHOW_PASS_WINDOW
        sys_print("%c ", (l_result == MMC_ERR_NONE) ? 'O' : ((l_result == MMC_ERR_BADCRC) ? 'X' : 'T'));
		#else
        sys_print("(%d\t%d)\t%d\n", g_tune_dbg.write_count, l_result, l_emmc50_dat0_txdly);
		#endif
    } while (l_emmc50_dat0_txdly++ < 31);

	#ifdef ETT_SHOW_PASS_WINDOW
    sys_print("w:%d(%d\t%d)\t%d\n", l_max_count, l_end - (l_max_count ? (l_max_count - 1) : 0), l_end, l_emmc50_dat0_txdly);
	#endif

    /* l_max_count will be 0, 1, ... 32 */
    if (l_max_count > ETT_TUNE_WINDOW){
		#ifdef ETT_TEST_DBG_LOG
        sys_print("[%s %d]write window find(%d)\n", __func__, __LINE__, g_tune_dbg.write_tx_count);
		#endif
        //update the max real window
        if (l_max_count > g_tune_para.write_tx_max_window){
            g_tune_para.write_tx_max_window = l_max_count;

            g_tune_para.emmc50_dattxdly = l_end - g_tune_para.write_tx_max_window / 2;
        }
    } else {
		#ifdef ETT_TEST_DBG_LOG
        sys_print("[%s %d]write window not find(%d)\n", __func__, __LINE__, g_tune_dbg.write_tx_count);
		#endif
    }

    //print final result
    sys_print("count = %d failed = %d, passed = %d, max_window = %d\n", g_tune_dbg.write_tx_count, g_tune_dbg.write_tx_failed, g_tune_dbg.write_tx_passed, g_tune_para.write_tx_max_window);

    sys_print("write tx paras: MSDC_EMMC50_PAD_DAT0_TXDLY = %d\n", g_tune_para.emmc50_dattxdly);

    MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT0_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT01_TUNE, MSDC_EMMC50_PAD_DAT1_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT2_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT23_TUNE, MSDC_EMMC50_PAD_DAT3_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT4_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT45_TUNE, MSDC_EMMC50_PAD_DAT5_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT6_TXDLY, 0);
    MSDC_SET_FIELD(EMMC50_PAD_DAT67_TUNE, MSDC_EMMC50_PAD_DAT7_TXDLY, 0);


}


