
#define MSDC0_BASE (0xF006D000)
#define MSDC1_BASE (0xF0012000)
#define MSDC2_BASE (0x11250000)

//#include "common.h"
//#include "mmc_core.h"
#include "x_printf.h"
#include "x_os.h"
#include "x_timer.h"
#include "types.h"
#include "msdc_ett.h"
#include "msdc.h"

//#include "i2c.h"


 unsigned int msdc_base = 0;
#define MMC_DEBUG 0

#define CLK_CFG_3   (0x10000070)

#define MSDC_USE_EMMC50_SUPPORT 0
#define MSDC_USE_EMMC50_SUPPORT 0
#define MTK_MT6595_USED_800M 0
#define MMC_DEBUG 0

struct mmc_host ett_sd_host;
struct mmc_card ett_sd_card;

struct msdc_tune_para g_tune_para;
struct msdc_tune_debug g_tune_dbg;

u32 g_cmd_tune_done = 0;
u32 g_msdc_ett_debug = 0;
u32 g_msdc_ett_debug1 = 0;
u32 g_msdc_ett_debug1_dat_latch_ck_sel = 0;
u32 g_msdc_ett_debug1_ckgen_dly_sel = 0;
unsigned char g_block_pattern[128] = {
    0xFF, 0xFF, 0x0,  0xFF, 0xFF, 0xFF, 0x0,  0x0,
    0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x3,  0xCC, 0xCC,
    0xCC, 0x3,  0x3,  0xCC, 0xCC, 0xCC, 0xFF, 0xFF,
    0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE, 0xFF,
    
    0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD, 0xDD,
    0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF, 0xBB,
    0xBB, 0xFF, 0xFF, 0xFF, 0x7,  0xFF, 0xFF, 0xFF,
    0x7,  0x7,  0xFF, 0x7,  0xBB, 0xDD, 0xEE, 0xFF,

    0xFF, 0xFF, 0xFF, 0x0,  0xFF, 0xFF, 0xFF, 0x0,
    0xFF, 0xFF, 0xFF, 0x0,  0xFF, 0xFF, 0xFF, 0x0,
    0xCC, 0xCC, 0x3,  0x3,  0xCC, 0xCC, 0xCC, 0xFF,
    0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE,

    0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD,
    0xDD, 0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF,
    0xBB, 0xFF, 0xFF, 0xFF, 0xFF, 0x7,  0xFF, 0xFF,
    0xFF, 0x7,  0x7,  0xFF, 0x7,  0xBB, 0xDD, 0xEE,
};

//static u8 g_read_data_edge[8] = {MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING};
static u8 g_write_data_edge[4] = {MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING, MSDC_SMPL_RISING};

static msdc_priv_t msdc_priv[MSDC_MAX_NUM];

#define MSDC_RETRY_COUNT   (1000000)  // wait for command response
#define ERROR_TO  (1 << 0)
#define ERROR_CRC (1 << 1)

#define NR_MMC             (MSDC_MAX_NUM)


static int msdc_rsp[] = {
    0,  /* RESP_NONE */
    1,  /* RESP_R1 */
    2,  /* RESP_R2 */
    3,  /* RESP_R3 */
    4,  /* RESP_R4 */
    1,  /* RESP_R5 */
    1,  /* RESP_R6 */
    1,  /* RESP_R7 */
    7,  /* RESP_R1b */
};

struct msdc_cust msdc_cap[MSDC_MAX_NUM] = 
{
    {
        .clk_src     = 0,                                /* host clock source             */
        .cmd_edge    = MSDC_SMPL_RISING,                 /* command latch edge            */
        .data_edge   = MSDC_SMPL_RISING,                 /* data latch edge               */
        .clk_drv     = MSDC_DRVN_GEAR2,                  /* clock pad driving             */
        .cmd_drv     = MSDC_DRVN_GEAR2,                  /* command pad driving           */
        .dat_drv     = MSDC_DRVN_GEAR2,                  /* data pad driving              */
        .rst_drv     = MSDC_DRVN_GEAR2,                  /* data pad driving              */
        .ds_drv      = MSDC_DRVN_GEAR2,                  /* data pad driving              */
        .clk_18v_drv = MSDC_DRVN_DONT_CARE,              /* clock pad driving on 1.8V     */
        .cmd_18v_drv = MSDC_DRVN_DONT_CARE,              /* command pad driving on 1.8V   */
        .dat_18v_drv = MSDC_DRVN_DONT_CARE,              /* data pad driving on 1.8V      */
        .data_pins   = 8,                                /* data pins                     */
        .data_offset = 0,                                /* data address offset           */

        .flags = MSDC_HIGHSPEED | MSDC_HS200
        //MSDC_HIGHSPEED | MSDC_HS200 | MSDC_RST_PIN_EN 
    },

    {
        .clk_src     =  0,                               /* host clock source             */
        .cmd_edge    = MSDC_SMPL_RISING,                 /* command latch edge            */
        .data_edge   = MSDC_SMPL_RISING,                 /* data latch edge               */
        .clk_drv     = MSDC_DRVN_GEAR1,                  /* clock pad driving             */
        .cmd_drv     = MSDC_DRVN_GEAR1,                  /* command pad driving           */
        .dat_drv     = MSDC_DRVN_GEAR1,                  /* data pad driving              */
        .rst_drv     = MSDC_DRVN_DONT_CARE,              /* rst pad driving              */
        .ds_drv      = MSDC_DRVN_DONT_CARE,              /* ds pad driving              */
        .clk_18v_drv = MSDC_DRVN_GEAR2,                  /* clock pad driving on 1.8V     */
        .cmd_18v_drv = MSDC_DRVN_GEAR2,                  /* command pad driving on 1.8V   */
        .dat_18v_drv = MSDC_DRVN_GEAR2,                  /* data pad driving on 1.8V      */
        .data_pins   = 4,                                /* data pins                     */
        .data_offset = 0,                                /* data address offset           */

        /* hardware capability flags     */
        .flags = MSDC_HIGHSPEED | MSDC_UHS1 | MSDC_DDR 
    },
};


static const unsigned int tran_exp[] = {
    10000,      100000,     1000000,    10000000,
    0,      0,      0,      0
};

static const unsigned char tran_mant[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static const unsigned char mmc_tran_mant[] = {
    0,  10, 12, 13, 15, 20, 26, 30,
    35, 40, 45, 52, 55, 60, 70, 80,
};

static const unsigned int tacc_exp[] = {
    1,  10, 100,    1000,   10000,  100000, 1000000, 10000000,
};

static const unsigned int tacc_mant[] = {
    0,  10, 12, 13, 15, 20, 25, 30,
    35, 40, 45, 50, 55, 60, 70, 80,
};

static unsigned int ett_ntohl(unsigned int n)
{
    unsigned int t;
    unsigned char *b = (unsigned char*)&t;
    *b++ = ((n >> 24) & 0xFF);
    *b++ = ((n >> 16) & 0xFF);
    *b++ = ((n >> 8) & 0xFF);
    *b   = ((n) & 0xFF);
    return t;
}

u32 unstuff_bits(u32 *resp, u32 start, u32 size)
{
    const u32 __mask = (1 << (size)) - 1;
    const u32 __off = 3 - ((start) / 32);
    const u32 __shft = (start) & 31;
    u32 __res;

    __res = resp[__off] >> __shft;
    if ((size) + __shft >= 32)
        __res |= resp[__off-1] << (32 - __shft);
    return __res & __mask;
}

#define UNSTUFF_BITS(r,s,sz)    unstuff_bits(r,s,sz)


/* memory func, memory set, free */

/* delay func */

/* basic func */



#define sdr_set_bits(reg,bs) \
	do{\
		volatile unsigned int tv = MSDC_READ32(reg);\
		tv |= (u32)(bs); \
		MSDC_WRITE32(reg,tv); \
	}while(0)
#define sdr_clr_bits(reg,bs) \
do{\
		volatile unsigned int tv = MSDC_READ32(reg);\
		tv &= ~((u32)(bs)); \
		MSDC_WRITE32(reg,tv); \
	}while(0)
	
#define msdc_irq_save(val) \
    do { \
        val = MSDC_READ32(MSDC_INTEN); \
        sdr_clr_bits(MSDC_INTEN, val); \
    } while(0)

#define msdc_irq_restore(val) \
    do { \
        sdr_set_bits(MSDC_INTEN, val); \
    } while(0)

void ett_memset(void* buf, int len, int val)
{
    int i = 0;
    unsigned char* l_buf = buf;

    if (l_buf == NULL) return;

    for (i = 0; i < len; i++){
        *l_buf = val;
        l_buf++;
    }
}



#if 0//#ifdef MSDC_MT6595_WORKAROUND_FOR_ERR_HANDLE
int top_reset_restore_msdc_cfg(int restore)
{
    if(restore) {
        /* do we need, timeout here? */
        while (!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB)){
            sys_print("[%s %d] wait clock stable\n", __func__, __LINE__);
        }
    }
    
    return 0;     
}


/* program guide suggest need backup & restore SD_ACMD19_TRG(0x84), DMA_CFG(0x9c), 0x204, EMMC50_CFG2(0x21c) too,
 * cause SD_ACMD19_TRG and 0x204 software not used, DMA_CFG(0x9c), EMMC50_CFG2(0x21c) will runtime set */
#define TOP_RESET_BACKUP_REG_NUM (22)
struct msdc_reg_control top_reset_backup_reg_list[TOP_RESET_BACKUP_REG_NUM] = { 
        //{addr,                                       mask,       value,      reset value, func},
        {(MSDC0_BASE + OFFSET_MSDC_CFG),               0xffffffff, 0x0,        0x0,        top_reset_restore_msdc_cfg},
        {(MSDC0_BASE + OFFSET_MSDC_IOCON),             0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_INTEN),             0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_SDC_CFG),                0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_SDC_CSTS_EN),            0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_PATCH_BIT0),        0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_PATCH_BIT1),        0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_PAD_TUNE),          0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY0),        0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY1),        0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_CTL0),        0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DS_CTL0),     0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DS_TUNE),     0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_CMD_TUNE),    0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  0xffffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_CFG0),            0x7fffffff, 0x0,        0x0,        NULL},
        {(MSDC0_BASE + OFFSET_EMMC50_CFG1),            0xffffffff, 0x0,        0x0,        NULL},    
        {(MSDC0_BASE + OFFSET_EMMC50_CFG2),            0xffffffff, 0x0,        0x0,        NULL},//for axi brust length test  
        {(MSDC0_BASE + OFFSET_EMMC50_CFG3),            0xffffffff, 0x0,        0x0,        NULL},//for axi outstading test
}; 

/*
 *  when desc/basic dma data error happend, or enhance dma cmd/acmd/dma data error happend, do top layer reset
 */
static void msdc_top_reset(struct mmc_host *host)
{
    int i = 0, err = 0; 
    
    if(host->id != 0)
        return; 
        
    //printf("[%s]: back up\n",__func__); 
    for(i = 0; i < TOP_RESET_BACKUP_REG_NUM; i++)
    {
        MSDC_GET_FIELD(top_reset_backup_reg_list[i].addr, top_reset_backup_reg_list[i].mask,  top_reset_backup_reg_list[i].value); 
        if(top_reset_backup_reg_list[i].restore_func){
            err = top_reset_backup_reg_list[i].restore_func(0); 
            if(err) {
                sys_print("failed to backup reg[0x%x][0x%x], expected value[0x%x], actual value[0x%x] err=0x%x", 
                        top_reset_backup_reg_list[i].addr, top_reset_backup_reg_list[i].mask, top_reset_backup_reg_list[i].value, MSDC_READ32(top_reset_backup_reg_list[i].addr), err); 
            }
        }
    }
    
    //sys_print("[%s %d]: do msdc top reset\n", __func__, __LINE__); 
    MSDC_SET_FIELD(0x10003000, (0x1 << 19), 0x1); 
    CTP_Wait_msec(1);
    MSDC_SET_FIELD(0x10003000, (0x1 << 19), 0x0);
    
    for(i = 0; i < TOP_RESET_BACKUP_REG_NUM; i++)
    {
        MSDC_SET_FIELD(top_reset_backup_reg_list[i].addr, top_reset_backup_reg_list[i].mask,  top_reset_backup_reg_list[i].value); 
        if(top_reset_backup_reg_list[i].restore_func){
            err = top_reset_backup_reg_list[i].restore_func(1); 
            if(err) {
                sys_print("failed to restore reg[0x%x][0x%x], expected value[0x%x], actual value[0x%x] err=0x%x", 
                        top_reset_backup_reg_list[i].addr, top_reset_backup_reg_list[i].mask, top_reset_backup_reg_list[i].value, MSDC_READ32(top_reset_backup_reg_list[i].addr), err); 
            }
        }
    }
    
    return;
}
#endif 


void ett_msdc_abort(struct mmc_host *host)
{

    /* reset controller */
    MSDC_RESET();

    /* clear fifo */
    MSDC_CLR_FIFO();

    /* make sure txfifo and rxfifo are empty */
    if (MSDC_TXFIFOCNT() != 0 || MSDC_RXFIFOCNT() != 0) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Abort: TXFIFO(%d), RXFIFO(%d) != 0\n", host->id, MSDC_TXFIFOCNT(), MSDC_RXFIFOCNT());
#endif 
    }

    /* clear all interrupts */
    MSDC_WRITE32(MSDC_INT, MSDC_READ32(MSDC_INT));

}

/* mmc card init func */
void msdc_set_sr(struct mmc_host *host,int clk,int cmd, int dat, int rst, int ds)
{
	switch(host->id){
		case 0:
            MSDC_SET_FIELD(MSDC0_GPIO_CLK_BASE, GPIO_MSDC_SR_MASK, clk);
            MSDC_SET_FIELD(MSDC0_GPIO_CMD_BASE, GPIO_MSDC_SR_MASK, cmd);
            MSDC_SET_FIELD(MSDC0_GPIO_DAT_BASE, GPIO_MSDC_SR_MASK, dat);
            MSDC_SET_FIELD(MSDC0_GPIO_RST_BASE, GPIO_MSDC_SR_MASK, rst);
            MSDC_SET_FIELD(MSDC0_GPIO_DS_BASE, GPIO_MSDC_SR_MASK, ds);
			break;
		case 1:
            MSDC_SET_FIELD(MSDC1_GPIO_CLK_BASE, GPIO_MSDC_SR_MASK, clk);
            MSDC_SET_FIELD(MSDC1_GPIO_CMD_BASE, GPIO_MSDC_SR_MASK, cmd);
            MSDC_SET_FIELD(MSDC1_GPIO_DAT_BASE, GPIO_MSDC_SR_MASK, dat);
			break;
		default:
			break;
	}
}

/* for mt6595, tdsel set to '0x0' for msdc0, set to '0xa' for msdc1, set to '0xa' for msdc2 
 * note: need set to '0xf' for msdc1/2 under sleep mode in kernel driver, but _DVT not need */
void ett_msdc_set_tdsel(struct mmc_host *host, bool sd_18)
{
    /* need ask the IO designer for value */
	switch(host->id){
		case 0:
            MSDC_SET_FIELD(MSDC0_GPIO_PAD_BASE, GPIO_PAD_TDSEL_MASK, 0x0);
			break;
		case 1:
            MSDC_SET_FIELD(MSDC1_GPIO_PAD_BASE, GPIO_PAD_TDSEL_MASK, 0xa);
			break;
		default:
			break;
	}
}

int ett_msdc_get_tdsel(struct mmc_host *host)
{
    int tdsel;

    /* need ask the IO designer for value */
	switch(host->id){
		case 0:
            MSDC_GET_FIELD(MSDC0_GPIO_PAD_BASE, GPIO_PAD_TDSEL_MASK, tdsel);
			break;
		case 1:
            MSDC_GET_FIELD(MSDC1_GPIO_PAD_BASE, GPIO_PAD_TDSEL_MASK, tdsel);
			break;
		default:
			break;
	}

    return tdsel;
}

/* for mt6595, rdsel set to '0x0' for msdc0, set ro '0xc' for msdc1, set ro '0xc' for msdc2 */
void ett_msdc_set_rdsel(struct mmc_host *host, bool sd_18)
{
    /* need ask the IO designer for value */
	switch(host->id){
		case 0:
            MSDC_SET_FIELD(MSDC0_GPIO_PAD_BASE, GPIO_PAD_RDSEL_MASK, 0x0);
			break;
		case 1:
            if(sd_18){
                MSDC_SET_FIELD(MSDC1_GPIO_PAD_BASE, GPIO_PAD_RDSEL_MASK, 0x0);
            } else {
                MSDC_SET_FIELD(MSDC1_GPIO_PAD_BASE, GPIO_PAD_RDSEL_MASK, 0xc);
            }
            break;
		default:
			break;
	}
}

int ett_msdc_get_rdsel(struct mmc_host *host)
{
    int rdsel;

    /* need ask the IO designer for value */
	switch(host->id){
		case 0:
            MSDC_GET_FIELD(MSDC0_GPIO_PAD_BASE, GPIO_PAD_RDSEL_MASK, rdsel);
			break;
		case 1:
            MSDC_GET_FIELD(MSDC1_GPIO_PAD_BASE, GPIO_PAD_RDSEL_MASK, rdsel);
            break;
		default:
			break;
	}
   
    return rdsel;
}

void ett_msdc_pin_pud(struct mmc_host *host, u32 pin, u32 mode, u32 val)
{
	switch(host->id){
		case 0:
            switch(pin){
                case GPIO_CLK_CTRL:
                    MSDC_SET_FIELD(MSDC0_GPIO_CLK_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC0_GPIO_CLK_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                case GPIO_CMD_CTRL:
                    MSDC_SET_FIELD(MSDC0_GPIO_CMD_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC0_GPIO_CMD_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                case GPIO_DAT_CTRL:
                    MSDC_SET_FIELD(MSDC0_GPIO_DAT_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC0_GPIO_DAT_BASE, GPIO_MSDC_PUPD_MASK, mode);             
                    break;
                case GPIO_RST_CTRL:
                    MSDC_SET_FIELD(MSDC0_GPIO_RST_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC0_GPIO_RST_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                case GPIO_DS_CTRL:
                    MSDC_SET_FIELD(MSDC0_GPIO_DS_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC0_GPIO_DS_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                default: 
                    break;
            }
            break;
        case 1:
            switch(pin){
                case GPIO_CLK_CTRL:
                    MSDC_SET_FIELD(MSDC1_GPIO_CLK_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_CLK_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                case GPIO_CMD_CTRL:
                    MSDC_SET_FIELD(MSDC1_GPIO_CMD_BASE, GPIO_MSDC_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_CMD_BASE, GPIO_MSDC_PUPD_MASK, mode);
                    break;
                case GPIO_DAT_CTRL:
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT0_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT0_PUPD_MASK, mode);             
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT1_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT1_PUPD_MASK, mode);             
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT2_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT2_PUPD_MASK, mode);             
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT3_R1R0_MASK, val);
                    MSDC_SET_FIELD(MSDC1_GPIO_DATPUPD_BASE, GPIO_MSDC_DAT3_PUPD_MASK, mode);             
                    break;
                default: 
                    break;
            }
            break;
        default:
            break;
    }
}

void ett_msdc_set_smt(struct mmc_host *host,int set_smt)
{
;
}

/* host can modify from 0-7 */

void ett_msdc_set_driving(struct mmc_host *host,struct msdc_cust *msdc_cap, bool sd_18)
{	

    sdr_clr_bits(MSDC_PAD_CTL0, (0x7<<15) | (0x7<<0));
    sdr_clr_bits(MSDC_PAD_CTL1, (0x7<<15) | (0x7<<0));
    sdr_clr_bits(MSDC_PAD_CTL2, (0x7<<15) | (0x7<<0));	
    
    sdr_set_bits(MSDC_PAD_CTL0, ((0x4)<<15) | ((msdc_cap->clk_drv&0x7)<<0));
    sdr_set_bits(MSDC_PAD_CTL1, ((0x4)<<15) | ((msdc_cap->cmd_drv&0x7)<<0));
    sdr_set_bits(MSDC_PAD_CTL2, ((0x4)<<15) | ((msdc_cap->dat_drv&0x7)<<0));
   
}


void ett_msdc_config_pin(struct mmc_host *host, int mode)
{
#if ETT_STRIP_CODE
    sys_print("[SD%d] Pins mode(%d), none(0), down(1), up(2), keep(3)\n", host->id, mode);
#endif 
    
    switch (mode) {
        case MSDC_PIN_PULL_UP:
            ett_msdc_pin_pud(host, GPIO_CLK_CTRL, MSDC_GPIO_PULL_UP, MSDC_PULL_10K);
            ett_msdc_pin_pud(host, GPIO_CMD_CTRL, MSDC_GPIO_PULL_UP, MSDC_PULL_10K);
            ett_msdc_pin_pud(host, GPIO_DAT_CTRL, MSDC_GPIO_PULL_UP, MSDC_PULL_10K);
            ett_msdc_pin_pud(host, GPIO_DS_CTRL, MSDC_GPIO_PULL_UP, MSDC_PULL_10K);
            break;
        case MSDC_PIN_PULL_DOWN:
            ett_msdc_pin_pud(host, GPIO_CLK_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_50K);
            ett_msdc_pin_pud(host, GPIO_CMD_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_50K);
            ett_msdc_pin_pud(host, GPIO_DAT_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_50K);
            ett_msdc_pin_pud(host, GPIO_DS_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_50K); 
            break;
        case MSDC_PIN_PULL_NONE:
            ett_msdc_pin_pud(host, GPIO_CMD_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_0K);
            ett_msdc_pin_pud(host, GPIO_DAT_CTRL, MSDC_GPIO_PULL_DOWN, MSDC_PULL_0K);
        default:
            break;
    }
}

void ett_msdc_set_startbit(struct mmc_host *host, u8 start_bit)
{
#if (1 == MSDC_USE_EMMC50_SUPPORT)
    u32 l_start_bit;
    msdc_priv_t *priv = (msdc_priv_t*)host->priv;

    if (host->id != 0){
        return; 
    }

    /* set start bit */
    MSDC_SET_FIELD(MSDC_CFG, MSDC_CFG_START_BIT, start_bit);
    priv->start_bit = start_bit;

    MSDC_GET_FIELD(MSDC_CFG, MSDC_CFG_START_BIT, l_start_bit);
    switch (l_start_bit){
        case 0:
            sys_print("[info][%s %d] read data start bit at rising edge\n", __func__, __LINE__);
            break;
        case 1:
            sys_print("[info][%s %d] read data start bit at falling edge\n", __func__, __LINE__);
            break;
        case 2:
            sys_print("[info][%s %d] read data start bit at rising & falling edge\n", __func__, __LINE__);
            break;
        case 3:
            sys_print("[info][%s %d] read data start bit at rising | falling edge\n", __func__, __LINE__);
            break;
        default:
            break;
    }
#endif
}


void ett_msdc_set_smpl(struct mmc_host *host, u8 HS400, u8 mode, u8 type)
{	
	int i=0;
    msdc_priv_t *priv = (msdc_priv_t*)host->priv;

	switch(type)
	{
		case TYPE_CMD_RESP_EDGE:	
			if(HS400){
				// eMMC5.0 only output resp at CLK pin, so no need to select DS pin 
				MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_PADCMD_LATCHCK, 0); //latch cmd resp at CLK pin
	    		MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CMDRSP_SEL, 0);//latch cmd resp at CLK pin
			}
			
			if(mode == MSDC_SMPL_RISING|| mode == MSDC_SMPL_FALLING){
				if(HS400){
					MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CMDEDGE_SEL, mode);
				}else{
					MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, mode);
				}
				priv->rsmpl = mode;
			}else {
				sys_print("[%s]: SD%d invalid resp parameter: HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
			}
			break;
		case TYPE_WRITE_CRC_EDGE: 			
		    if(HS400){
		    	MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRCSTS_SEL, 1);//latch write crc status at DS pin				
			}else {
		    	MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRCSTS_SEL, 0);//latch write crc status at CLK pin		
			}
			
			if(mode == MSDC_SMPL_RISING|| mode == MSDC_SMPL_FALLING){
				if(HS400){
					MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRCSTS_EDGE, mode);
				}else{
					MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 0);
					MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL, mode);
				}
				priv->wdsmpl = mode;
			}else if(mode == MSDC_SMPL_SEPERATE&& !HS400) {
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D0SPL, g_write_data_edge[0]); //only dat0 is for write crc status.
				priv->wdsmpl = mode;
			}else {
				sys_print("[%s]: SD%d invalid crc parameter: HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
			}
			break;
		case TYPE_READ_DATA_EDGE:
			if(HS400){
				ett_msdc_set_startbit(host, START_AT_RISING_AND_FALLING); //for HS400, start bit is output both on rising and falling edge
				priv->start_bit = START_AT_RISING_AND_FALLING;
			}else {				
				ett_msdc_set_startbit(host, START_AT_RISING); //for the other mode, start bit is only output on rising edge. but DDR50 can try falling edge if error casued by pad delay
				priv->start_bit = START_AT_RISING;
			}
			if(mode == MSDC_SMPL_RISING|| mode == MSDC_SMPL_FALLING){
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL_SEL, 0);
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, mode);
				priv->rdsmpl = mode;
			}else if(mode == MSDC_SMPL_SEPERATE) {
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_R_D_SMPL_SEL, 1);	
				for(i=0; i<8; i++);
				{
					MSDC_SET_FIELD(MSDC_IOCON, (MSDC_IOCON_R_D0SPL << i), g_write_data_edge[i]);
				}	
				priv->rdsmpl = mode;		
			}else {
				sys_print("[%s]: SD%d invalid read parameter: HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
			}
			break;
		case TYPE_WRITE_DATA_EDGE:		
		    MSDC_SET_FIELD(EMMC50_CFG0, MSDC_EMMC50_CFG_CRCSTS_SEL, 0);//latch write crc status at CLK pin	
			
			if(mode == MSDC_SMPL_RISING|| mode == MSDC_SMPL_FALLING){
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 0);
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL, mode);
				priv->wdsmpl = mode;
			}else if(mode == MSDC_SMPL_SEPERATE) {
				MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL, 1);
				for(i=0; i<4; i++);
				{
					MSDC_SET_FIELD(MSDC_IOCON, (MSDC_IOCON_W_D0SPL << i), g_write_data_edge[i]);//dat0~4 is for SDIO card. 
				}
				priv->wdsmpl = mode;
			}else {
				sys_print("[%s]: SD%d invalid write parameter: HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
			}
			break;
		default:
			sys_print("[%s]: SD%d invalid parameter: HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
			break;
	}
			//printf("[%s]: SD%d HS400=%d, type=%d, mode=%d\n", __func__, host->id, HS400, type, mode);
}


void ett_msdc_clock(struct mmc_host *host, int on)
{
}

u32 ett_msdc_power(struct mmc_host *host, u32 on)
{
//    if (on){
//        ett_msdc_config_pin(host, MSDC_PIN_PULL_UP);
//        ett_msdc_set_rdsel(host,0);
//        ett_msdc_set_tdsel(host,0);
//    }
//    
//    /* no need to open power */
//
//    if (!on){
//        ett_msdc_config_pin(host, MSDC_PIN_PULL_DOWN);
//    }
//   
//    /* msdc clock alway on under DVT test environment */
//    ett_msdc_clock(host, on);
    return 0;
}

void ett_msdc_host_reset(struct mmc_host *host)
{
    
    ett_msdc_power(host, 0);
    ett_msdc_power(host, 1);
    MSDC_RESET();

}



void ett_msdc_config_bus(struct mmc_host *host, u32 width)
{
    
    u32 val  = MSDC_READ32(SDC_CFG);

    val &= ~SDC_CFG_BUSWIDTH;

    switch (width) {
    case HOST_BUS_WIDTH_1:
        val |= (MSDC_BUS_1BITS << 16);
        break;
    case HOST_BUS_WIDTH_4:
        val |= (MSDC_BUS_4BITS << 16);
        break;
    case HOST_BUS_WIDTH_8:
        val |= (MSDC_BUS_8BITS << 16);
        break;
    default:
        width = HOST_BUS_WIDTH_1;
        break;
    }
    MSDC_WRITE32(SDC_CFG, val);

#if ETT_STRIP_CODE
    sys_print("[SD%d] Bus Width: %d\n", host->id, width);
#endif 
}

void ett_msdc_config_clksrc(struct mmc_host *host, clk_source_t clksrc)
{

    // modify the clock 
    if (host->card && mmc_card_hs400(host->card)){

        sys_print("[info][%s %d] hs400 mode, change pll mux to 800Mhz\n", __func__, __LINE__);
        /* after the card init flow, if the card support hs400 mode
         * modify the mux channel of the msdc pll */

        // mux select

        //host->pll_mux_clk = MSDC_CLKSRC_800MHZ;
        //host->src_clk = MSDC_800M_SCLK;
        //host->src_clk = MSDC_400M_SCLK;
    } else {
        //before the card init flow 
        host->pll_mux_clk = MSDC_CLKSRC_200MHZ;
        host->src_clk = MSDC_OP_SCLK;
    }

    sys_print("[info][%s %d] input clock is %dkHz\n", __func__, __LINE__, host->src_clk/1000);
}


enum {
    IDX_CLK_TARGET = 0,
    IDX_CLK_SRC_VAL,
    IDX_CLK_MODE_VAL,
    IDX_CLK_DIV_VAL,
    IDX_CLK_DRV_VAL,
    IDX_CMD_DRV_VAL,
    IDX_DAT_DRV_VAL,
    IDX_CLK_SAMP_VAL,
    IDX_CLK_IDX_MAX
};

#define MSDC_CLK_S_REG1   0xF000D380
#define MSDC_CLK_S_REG0   0xF000D32C
#define MSDC_CLK_H_REG1   0xF000D384
#define MSDC_CLK_H_REG0   0xF000D3A0
#define MSDC_CLK_GATE_BIT (0x1<<7)
#define MSDC_CLK_SEL_MASK (0x0F<<0)

#define MSDC_CLK_TARGET   200000000, 162000000, 144000000, 120000000, 100000000, 80000000, 50000000, 50000000, 25000000, 12000000,  0
#define MSDC_CLK_SRC_VAL  11,         4,  5,   6,  11,  9,  8, 11, 11,  0,  0
#define MSDC_CLK_MODE_VAL 1,          1,  1,   1,   0,  1,  1,  2,  0,  0,  0
#define MSDC_CLK_DIV_VAL  0,          0,   0,   0,   0,  0,  0,  0,  2,  0, 16
#define MSDC_CLK_DRV_VAL  1,          1,   1,   1,   1,  1,  1,  1,  1,  1,  0
#define MSDC_CMD_DRV_VAL  1,          1,  1,   1,   1,  1,  1,  1,  1,  1,  0
#define MSDC_DAT_DRV_VAL  1,          1,   1,   1,   1,  1,  1,  1,  1,  1,  0


u32 ettmsdcClk[][11] = {{MSDC_CLK_TARGET},
                                      {MSDC_CLK_SRC_VAL},
                                      {MSDC_CLK_MODE_VAL},
                                      {MSDC_CLK_DIV_VAL},
                                      {MSDC_CLK_DRV_VAL},
                                      {MSDC_CMD_DRV_VAL},
                                      {MSDC_DAT_DRV_VAL}};


    
    
static void msdc_clksrc_onoff(struct mmc_host *host, u32 on)
{

    if (on) 
    {
    	if(host->id== 0)
             sdr_clr_bits(MSDC_CLK_S_REG1, MSDC_CLK_GATE_BIT);	
             
         if(host->id== 1)
             sdr_clr_bits(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);	
          	
    } 
    else 
    {                
         if(host->id== 0)
             sdr_set_bits(MSDC_CLK_S_REG1, MSDC_CLK_GATE_BIT);	
             
         if(host->id== 1)
             sdr_set_bits(MSDC_CLK_S_REG0, MSDC_CLK_GATE_BIT);	      	
    }      	
}

static void msdc_select_clksrc(struct mmc_host* host, int clksrc)
{   
    
    	sys_print("host%d select clk source %d",host->id,clksrc);
      
        // Clr msdc_src_clk selection
        if(host->id== 0) //emmc
        {
        	msdc_clksrc_onoff(host, 0);
            sdr_clr_bits(MSDC_CLK_S_REG1, MSDC_CLK_SEL_MASK);  
            #if defined(CC_EMMC_DDR50)
            sdr_set_bits(MSDC_CLK_S_REG1, 0x8<<0);  
            #endif
            sdr_set_bits(MSDC_CLK_S_REG1, clksrc<<0);  
            msdc_clksrc_onoff(host, 1);
        }
        else
        if(host->id == 1) //sd
        {
        	msdc_clksrc_onoff(host, 0);
        	sdr_clr_bits(MSDC_CLK_S_REG0, MSDC_CLK_SEL_MASK);  
            sdr_set_bits(MSDC_CLK_S_REG0, clksrc<<0); 
            msdc_clksrc_onoff(host, 1); 
        }
}


static int msdc_clk_stable(struct mmc_host *host,u32 mode, u32 div)
{
	msdc_clksrc_onoff(host, 0);
    MSDC_SET_FIELD(MSDC_CFG, MSDC_CFG_CKMOD | MSDC_CFG_CKDIV, (mode << 8) | div); 
    msdc_clksrc_onoff(host, 1);
    /* wait clock stable */
//    while (!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB))
//    {
//    	CTP_Wait_usec(1);
//    }
    MSDC_RETRY((!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB)), 5, 5000);
	return 0;
}


static void msdc_set_mclk(struct mmc_host *host, int ddr, u32 hz)
{
    u32 mode = 1; /* no divisor */
    u32 div  = 0;
    u32 flags;
    u32 sdClkSel = 0, idx = 0;

    if (!hz) { // set mmc system clock to 0
        sys_print("msdc%d -> set mclk to 0\n",host->id);  // fix me: need to set to 0
        host->cur_bus_clk = 0;
        //msdc_reset_hw(host->id);
        return;
    }
    else
    {
    	  sys_print("msdc%d -> set cur_bus_clk to %d \n",host->id,hz);
    	  host->cur_bus_clk = hz;
    }

    msdc_irq_save(flags);

    

    do
    {
	    if((hz < ettmsdcClk[IDX_CLK_TARGET][idx]) ||
	       (ddr && (ettmsdcClk[IDX_CLK_MODE_VAL][idx] != 2)))
		    continue;
	    else
		    break;
	
    }while(++idx < (sizeof(ettmsdcClk[0]) / sizeof(ettmsdcClk[0][0])));
    sys_print("msdc set idx %d \n",idx);
    
    idx = (idx >= (sizeof(ettmsdcClk[0]) / sizeof(ettmsdcClk[0][0])))?((sizeof(ettmsdcClk[0]) / sizeof(ettmsdcClk[0][0]))):idx;
    	
     
    	
    sdClkSel = ettmsdcClk[IDX_CLK_SRC_VAL][idx];
    div = ettmsdcClk[IDX_CLK_DIV_VAL][idx];
    mode = ettmsdcClk[IDX_CLK_MODE_VAL][idx];
    
    msdc_select_clksrc(host,sdClkSel);
        /* wait clock stable */
    //while (!(MSDC_READ32(MSDC_CFG) & MSDC_CFG_CKSTB));
    
    msdc_clk_stable(host,mode, div);
    
    msdc_cap[host->id].clk_drv = ettmsdcClk[IDX_CLK_DRV_VAL][idx];
    msdc_cap[host->id].cmd_drv = ettmsdcClk[IDX_CMD_DRV_VAL][idx];
    msdc_cap[host->id].dat_drv = ettmsdcClk[IDX_DAT_DRV_VAL][idx];
    ett_msdc_set_driving(host, &msdc_cap[host->id], 0);
    
    host->src_clk = ettmsdcClk[IDX_CLK_TARGET][idx];
    


    if(hz >= 25000000)
    	sys_print("msdc%d -> !!! Set<%dKHz> -> sclk<%dKHz> DDR<%d> mode<%d> div<%d> srcidx<%d>\n" ,
		                host->id, hz/1000,host->cur_bus_clk/1000, ddr, mode, div,idx);
	else
		sys_print("msdc%d -> !!! Set<%dKHz>  -> sclk<%dKHz> DDR<%d> mode<%d> div<%d> srcidx<%d>\n" ,
		                host->id, hz/1000, host->cur_bus_clk/1000, ddr, mode, div,idx);


    msdc_irq_restore(flags);
}




void ett_msdc_config_clock(struct mmc_host *host, u32 ddr, u32 hz, u32 hs_timing)
{
	msdc_priv_t *priv = host->priv;
     msdc_set_mclk(host, ddr, hz);
   
	if(hs_timing & EXT_CSD_HS_TIMEING_HS400){		
	    ett_msdc_set_smpl(host, 1, priv->rsmpl, TYPE_CMD_RESP_EDGE);
	    ett_msdc_set_smpl(host, 1, priv->rdsmpl, TYPE_READ_DATA_EDGE);
	    ett_msdc_set_smpl(host, 1, priv->wdsmpl, TYPE_WRITE_CRC_EDGE);
	}else {		
		ett_msdc_set_smpl(host, 0, priv->rsmpl, TYPE_CMD_RESP_EDGE);
		ett_msdc_set_smpl(host, 0, priv->rdsmpl, TYPE_READ_DATA_EDGE);
		ett_msdc_set_smpl(host, 0, priv->wdsmpl, TYPE_WRITE_CRC_EDGE);
	}

    sys_print("[SD%d] SET_CLK(%dkHz): bus_clk(%dkHz) SRC_CLK(%dkHz) busmode(%d) DS(%d) RS(%d)\n",
        host->id, hz/1000, host->cur_bus_clk/1000, host->src_clk/1000, hs_timing, priv->rdsmpl, priv->rsmpl);
}





void ett_msdc_set_dmode(struct mmc_host *host, int mode)
{
    /* no need set */
}

void ett_msdc_set_pio_bits(struct mmc_host *host, int bits)
{
    msdc_priv_t *priv = (msdc_priv_t*)host->priv;

    priv->pio_bits = bits;
}

void ett_msdc_set_timeout(struct mmc_host *host, u32 ns, u32 clks) //u64 ns
{
    u32 timeout, clk_ns;

    clk_ns  = 1000000000UL / host->cur_bus_clk;
    timeout = ns / (u32)clk_ns + clks;
    timeout = timeout >> 20;                  /* in 1048576 sclk cycle unit (MT6583 MSDC IP)*/
    timeout = timeout > 1 ? timeout - 1 : 0;
    timeout = timeout > 255 ? 255 : timeout;

    MSDC_SET_FIELD(SDC_CFG, SDC_CFG_DTOC, timeout);

#ifdef ETT_MMC_DBG_LOG
    sys_print("[SD%d] Set read data timeout: %dns %dclks -> %d x 65536 cycles\n", host->id, ns, clks, timeout + 1);
#endif
}

void ett_msdc_card_detect(struct mmc_host *host, int on)
{
    /* no need here */
}

void ett_msdc_intr_unmask(struct mmc_host *host, u32 bits)
{
    u32 val;

    val  = MSDC_READ32(MSDC_INTEN);
    val |= bits;
    MSDC_WRITE32(MSDC_INTEN, val);    
}

int ett_msdc_init(int id, struct mmc_host *host, int clksrc, int mode)
{
    msdc_priv_t *priv;

    clksrc = (clksrc == -1) ? msdc_cap[id].clk_src : clksrc;

    priv = &msdc_priv[id];

    host->id     = id;

    if (id == 0){
        host->base_addr  = MSDC0_BASE;
    } else if (id == 1){
        host->base_addr  = MSDC1_BASE;
    }

    msdc_base = host->base_addr;

    host->f_max  = MSDC_MAX_SCLK;
    host->f_min  = MSDC_MIN_SCLK;
    host->blkbits= MMC_BLOCK_BITS;
    host->blklen = 0;
    host->priv   = (void*)priv;
        
    host->caps   = MMC_CAP_MULTIWRITE;
    
    if (msdc_cap[id].flags & MSDC_HIGHSPEED)
        host->caps |= (MMC_CAP_MMC_HIGHSPEED | MMC_CAP_SD_HIGHSPEED);
    if (msdc_cap[id].flags & MSDC_UHS1)
        host->caps |= MMC_CAP_SD_UHS1;
    if (msdc_cap[id].flags & MSDC_DDR)
        host->caps |= MMC_CAP_DDR;
    if (msdc_cap[id].data_pins == 4)
        host->caps |= MMC_CAP_4_BIT_DATA;
    if (msdc_cap[id].data_pins == 8)
        host->caps |= MMC_CAP_8_BIT_DATA | MMC_CAP_4_BIT_DATA;
    if (msdc_cap[id].flags & MSDC_HS200)
        host->caps |= MMC_CAP_EMMC_HS200;
    if (msdc_cap[id].flags & MSDC_HS400)
        host->caps |= MMC_CAP_EMMC_HS400;
    
    /* msdc0 only support 1.8 IO */    
    if(host->caps & (MMC_CAP_EMMC_HS200 | MMC_CAP_EMMC_HS400))
        host->ocr_avail = 0xff8080;
    else
        host->ocr_avail = MMC_VDD_30_31 | MMC_VDD_33_34;  /* TODO: To be customized */
    
    host->max_blk_size  = 2048;
    host->max_blk_count = 65535;

    priv->wdsmpl       = msdc_cap[id].data_edge;
    priv->rsmpl       = msdc_cap[id].cmd_edge;

    ett_msdc_host_reset(host);
    
    ett_msdc_clock(host, 1);

    ett_msdc_power(host, MMC_POWER_OFF);
    ett_msdc_power(host, MMC_POWER_ON);
    /* set to SD/MMC mode */
    MSDC_SET_FIELD(MSDC_CFG, MSDC_CFG_MODE, MSDC_SDMMC);//SD 3.0 host not support Memory stick mode(YD comment).
                                                        //Before MSDC_RESET, SW should switch to SD mode first
                                                        //(FPGA will not be gate by MSDC_CFG[0] = 0,but ASIC will.)

    MSDC_RESET();
    MSDC_CLR_FIFO();
    MSDC_CLR_INT();

    /* reset tuning parameter */

    /* reset tuning parameter */
    MSDC_WRITE32(MSDC_PAD_TUNE, 0x0000000);
    MSDC_WRITE32(MSDC_DAT_RDDLY0, 0x00000000);
    MSDC_WRITE32(MSDC_DAT_RDDLY1, 0x00000000);
    //MSDC_WRITE32(MSDC_IOCON, 0x00000004);      

    /* PATCH_BIT0[9-7] set to "1" as default[designer asked]
     * 2013-1-6 rollback to default value, for e2 eco verify */
    //MSDC_WRITE32(MSDC_PATCH_BIT0, 0x003C00CF);   
    MSDC_WRITE32(MSDC_PATCH_BIT0, 0x003C004F);   

    /* High 16 bit = 0 mean Power KPI is on, open KPI exclude MSDC_CK_SD_CKGN[designer asked] 
     * bit6-7 ECO switch, enable it for SLT load test */ 
    //MSDC_WRITE32(MSDC_PATCH_BIT1, 0x100000C9);  
    MSDC_WRITE32(MSDC_PATCH_BIT1, 0xFFFF00C9); /* 2013-1-6 close KPI for e2 eco verify */
    //MSDC_PATCH_BIT1:WRDAT_CRCS_TA_CNTR need fix to 3'001 by default,(<50MHz) (>=50MHz set 3'001 as initial value is OK for tunning)
    //YD:CMD_RSP_TA_CNTR need fix to 3'001 by default(<50MHz)(>=50MHz set 3'001as initial value is OK for tunning)                    
    
    MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_PIO);

    /* enable SDIO mode. it's must otherwise sdio command failed */
    MSDC_SET_BIT32(SDC_CFG, SDC_CFG_SDIO);
    
    /* enable wake up events */
    MSDC_SET_BIT32(SDC_CFG, SDC_CFG_INSWKUP);

    ett_msdc_set_smt(host, 1);

    /* set clk, cmd, dat pad driving */
    //ett_msdc_set_driving(host, &msdc_cap[id], 0);

    /* set sampling edge */
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_RSPL, priv->rsmpl);
    MSDC_SET_FIELD(MSDC_IOCON, MSDC_IOCON_DSPL, priv->wdsmpl);

    /* write crc timeout detection */
    MSDC_SET_FIELD(MSDC_PATCH_BIT0, 1 << 30, 1);

    ett_msdc_config_clksrc(host, clksrc);
    ett_msdc_config_bus(host, HOST_BUS_WIDTH_1);
    ett_msdc_config_clock(host, 0, MSDC_MIN_SCLK, 0);
    /* disable sdio interrupt by default. sdio interrupt enable upon request */
    ett_msdc_intr_unmask(host, 0x0001FF7B);
    ett_msdc_set_dmode(host, mode);
    ett_msdc_set_pio_bits(host, 32);
    ett_msdc_set_timeout(host, 100000000, 0);
    ett_msdc_card_detect(host, 1);


    //=======================================================================================//
    /* disable SDIO func */
    MSDC_SET_FIELD(SDC_CFG, SDC_CFG_SDIO, 0);
    MSDC_SET_FIELD(SDC_CFG, SDC_CFG_SDIOIDE, 0);
    MSDC_SET_FIELD(SDC_CFG, SDC_CFG_INSWKUP, 0);

    /* clock free run for emmc debug, sd30 can not free clock during switch volt */
    if (id == 0){
        MSDC_SET_FIELD(MSDC_CFG, MSDC_CFG_CKPDN, 1);
    } 
    
    return 0;
}

int ett_mmc_init_host(int id, struct mmc_host *host, int clksrc, u8 mode)
{
    ett_memset(host, 0, sizeof(struct mmc_host));

    return ett_msdc_init(id, host, clksrc, mode);
}

int ett_msdc_card_protected(struct mmc_host *host)
{
    int prot;

    if (msdc_cap[host->id].flags & MSDC_WP_PIN_EN) {
        MSDC_GET_FIELD(MSDC_PS, MSDC_PS_WP, prot);
    } else {
        prot = 0;
    }

    return prot;
}

int ett_msdc_send_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
    msdc_priv_t *priv = (msdc_priv_t*)host->priv;
    u32 opcode = cmd->opcode;
    u32 rsptyp = cmd->rsptyp;   
    u32 rawcmd;

    u32 error = MMC_ERR_NONE;

    /* rawcmd :
     * vol_swt << 30 | auto_cmd << 28 | blklen << 16 | go_irq << 15 | 
     * stop << 14 | rw << 13 | dtype << 11 | rsptyp << 7 | brk << 6 | opcode
     */
    rawcmd = (opcode & ~(SD_CMD_BIT | SD_CMD_APP_BIT)) | 
        msdc_rsp[rsptyp] << 7 | host->blklen << 16;

    if (opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK) {
        rawcmd |= ((2 << 11) | (1 << 13));
        if (priv->autocmd & MSDC_AUTOCMD12)
            rawcmd |= (1 << 28);
        else if (priv->autocmd & MSDC_AUTOCMD23)
            rawcmd |= (2 << 28);
    } else if (opcode == MMC_CMD_WRITE_BLOCK) {
        rawcmd |= ((1 << 11) | (1 << 13));
    } else if (opcode == MMC_CMD_READ_MULTIPLE_BLOCK) {
        rawcmd |= (2 << 11);
        if (priv->autocmd & MSDC_AUTOCMD12)
            rawcmd |= (1 << 28);
        else if (priv->autocmd & MSDC_AUTOCMD23)
            rawcmd |= (2 << 28);
    } else if (opcode == MMC_CMD_READ_SINGLE_BLOCK || 
               opcode == SD_ACMD_SEND_SCR ||
               opcode == SD_CMD_SWITCH ||
               opcode == MMC_CMD_SEND_EXT_CSD) {
        rawcmd |= (1 << 11);    
    } else if (opcode == SD_IO_RW_EXTENDED) {
        if (cmd->arg & 0x80000000)  /* R/W flag */
            rawcmd |= (1 << 13);
        if ((cmd->arg & 0x08000000) && ((cmd->arg & 0x1FF) > 1))
            rawcmd |= (2 << 11); /* multiple block mode */
        else
            rawcmd |= (1 << 11);
    } else if (opcode == MMC_CMD_STOP_TRANSMISSION) {
        rawcmd |= (1 << 14);
        rawcmd &= ~(0x0FFF << 16);
    } else if (opcode == SD_IO_RW_DIRECT) {
        if ((cmd->arg & 0x80000000) && ((cmd->arg >> 9) & 0x1FFFF))/* I/O abt */
            rawcmd |= (1 << 14);
    } else if (opcode == SD_CMD_VOL_SWITCH) {
        rawcmd |= (1 << 30);
    } else if (opcode == SD_CMD_SEND_TUNING_BLOCK) {
        rawcmd |= (1 << 11); /* CHECKME */
        if (priv->autocmd & MSDC_AUTOCMD19)
            rawcmd |= (3 << 28);
    } else if (opcode == MMC_CMD_GO_IRQ_STATE) {
        rawcmd |= (1 << 15);
    } else if (opcode == MMC_CMD_WRITE_DAT_UNTIL_STOP) {
        rawcmd |= ((1<< 13) | (3 << 11));
    } else if (opcode == MMC_CMD_READ_DAT_UNTIL_STOP) {
        rawcmd |= (3 << 11);
    }

#ifdef ETT_MSDC_DBG_LOG_CMD
    sys_print("[SD%d] CMD(%d): ARG(0x%x), RAW(0x%x), RSP(%d)\n", 
        host->id, (opcode & ~(SD_CMD_BIT | SD_CMD_APP_BIT)), cmd->arg, rawcmd, rsptyp);
#endif

    /* FIXME. Need to check if SDC is busy before data read/write transfer */
    if (opcode == MMC_CMD_SEND_STATUS) {
//        while ((MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)){
//            ;
//        }
MSDC_RETRY(((MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)), 5, 5000);
    if(MSDC_READ32(SDC_STS) & SDC_STS_CMDBUSY)
    	{
    //sys_print("[SD%d] CMD(%d): ARG(0x%x), RAW(0x%x), RSP(%d)\n", 
        //host->id, (opcode & ~(SD_CMD_BIT | SD_CMD_APP_BIT)), cmd->arg, rawcmd, rsptyp);
    		        /* reset controller */
        MSDC_RESET();

        /* clear fifo */
        MSDC_CLR_FIFO();
        MSDC_CLR_INT();
    	}

    } else {
//        while ((MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)){
//            ;
//        }

        MSDC_RETRY(((MSDC_READ32(SDC_STS) & SDC_STS_SDCBUSY)), 5, 5000);
            if(MSDC_READ32(SDC_STS) & (SDC_STS_SDCBUSY))
    	{
    		    //sys_print("[SD%d] CMD(%d): ARG(0x%x), RAW(0x%x), RSP(%d)\n", 
        //host->id, (opcode & ~(SD_CMD_BIT | SD_CMD_APP_BIT)), cmd->arg, rawcmd, rsptyp);

    		        /* reset controller */
        MSDC_RESET();

        /* clear fifo */
        MSDC_CLR_FIFO();
        MSDC_CLR_INT();
    	}
    }
    


    /* start command */
    MSDC_WRITE32(SDC_ARG, cmd->arg); 
    MSDC_WRITE32(SDC_CMD, rawcmd);     


    cmd->error = error;

    return error;
}

void msdc_dump_gpio_register(int id)
{
//    if (id == 0 || id == 0xff) {
//        /* emmc clk/cmd/data pad parameters */
//        sys_print("[GPIO] Reg[0x%x] MSDC0_CLK_PAD        = 0x%x\n", MSDC0_GPIO_CLK_BASE, *(u32*)(MSDC0_GPIO_CLK_BASE));
//        sys_print("                   MS0CK_R0R1[1:0]    = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS0CK_PUPD[2]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS0CK_DRV[10:8]    = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS0CK_SR[12]       = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 12) & 0x1);
//        sys_print("                   MS0CK_SMT[13]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 13) & 0x1);
//        sys_print("                   MS0CK_IES[14]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CLK_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC0_CMD_PAD        = 0x%x\n", MSDC0_GPIO_CMD_BASE, *(u32*)(MSDC0_GPIO_CMD_BASE));
//        sys_print("                   MS0CMD_R0R1[1:0]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS0CMD_PUPD[2]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS0CMD_DRV[10:8]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS0CMD_SR[12]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 12) & 0x1);
//        sys_print("                   MS0CMD_SMT[13]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 13) & 0x1);
//        sys_print("                   MS0CMD_IES[14]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_CMD_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC0_DAT_PAD        = 0x%x\n", MSDC0_GPIO_DAT_BASE, *(u32*)(MSDC0_GPIO_DAT_BASE));
//        sys_print("                   MS0DAT_R0R1[1:0]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS0DAT_PUPD[2]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS0DAT_DRV[10:8]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS0DAT_SR[12]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 12) & 0x1);
//        sys_print("                   MS0DAT_SMT[13]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 13) & 0x1);
//        sys_print("                   MS0DAT_IES[14]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DAT_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC0_DS_PAD         = 0x%x\n", MSDC0_GPIO_DS_BASE, *(u32*)(MSDC0_GPIO_DS_BASE));
//        sys_print("                   MS0DS_R0R1[1:0]    = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS0DS_PUPD[2]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS0DS_DRV[10:8]    = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS0DS_SR[12]       = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 12) & 0x1);
//        sys_print("                   MS0DS_SMT[13]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 13) & 0x1);
//        sys_print("                   MS0DS_IES[14]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_DS_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC0_RST_PAD        = 0x%x\n", MSDC0_GPIO_RST_BASE, *(u32*)(MSDC0_GPIO_RST_BASE));
//        sys_print("                   MS0RST_R0R1[1:0]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS0RST_PUPD[2]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS0RST_DRV[10:8]   = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS0RST_SR[12]      = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 12) & 0x1);
//        sys_print("                   MS0RST_SMT[13]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 13) & 0x1);
//        sys_print("                   MS0RST_IES[14]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_RST_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC0_PAD            = 0x%x\n", MSDC0_GPIO_PAD_BASE, *(u32*)(MSDC0_GPIO_PAD_BASE));
//        sys_print("                   MS0_TDSEL[3:0]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_PAD_BASE)) >> 0)  & 0xf);
//        sys_print("                   MS0_RDSEL[9:4]     = 0x%x\n", ((*(u32*)(MSDC0_GPIO_PAD_BASE)) >> 4)  & 0x3f);
//
//    } else if (id == 1 || id == 0xff) {
//        /* sd clk/cmd/data pad parameters */
//        sys_print("[GPIO] Reg[0x%x] MSDC1_CLK_PAD        = 0x%x\n", MSDC1_GPIO_CLK_BASE, *(u32*)(MSDC1_GPIO_CLK_BASE));
//        sys_print("                   MS1CK_R0R1[1:0]    = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS1CK_PUPD[2]      = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS1CK_DRV[10:8]    = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS1CK_SR[12]       = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 12) & 0x1);
//        sys_print("                   MS1CK_SMT[13]      = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 13) & 0x1);
//        sys_print("                   MS1CK_IES[14]      = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CLK_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC1_CMD_PAD        = 0x%x\n", MSDC1_GPIO_CMD_BASE, *(u32*)(MSDC1_GPIO_CMD_BASE));
//        sys_print("                   MS1CMD_R0R1[1:0]   = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS1CMD_PUPD[2]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS1CMD_DRV[10:8]   = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS1CMD_SR[12]      = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 12) & 0x1);
//        sys_print("                   MS1CMD_SMT[13]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 13) & 0x1);
//        sys_print("                   MS1CMD_IES[14]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_CMD_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC1_DAT_PAD        = 0x%x\n", MSDC1_GPIO_DAT_BASE, *(u32*)(MSDC1_GPIO_DAT_BASE));
//        sys_print("                   MS1DAT_DRV[10:8]   = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DAT_BASE)) >> 8)  & 0x7);
//        sys_print("                   MS1DAT_SR[12]      = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DAT_BASE)) >> 12) & 0x1);
//        sys_print("                   MS1DAT_SMT[13]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DAT_BASE)) >> 13) & 0x1);
//        sys_print("                   MS1DAT_IES[14]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DAT_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC1_DAT_PUPD       = 0x%x\n", MSDC1_GPIO_DATPUPD_BASE, *(u32*)(MSDC1_GPIO_DATPUPD_BASE));
//        sys_print("                   MS1DAT1_R0R1[1:0]  = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 0)  & 0x3);
//        sys_print("                   MS1DAT1_PUPD[2]    = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 2)  & 0x1);
//        sys_print("                   MS1DAT2_R0R1[5:4]  = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 4)  & 0x3);
//        sys_print("                   MS1DAT2_PUPD[6]    = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 6)  & 0x1);
//        sys_print("                   MS1DAT3_R0R1[9:8]  = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 8)  & 0x3);
//        sys_print("                   MS1DAT3_PUPD[10]   = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 10) & 0x1);
//        sys_print("                   MS1DAT4_R0R1[13:12]= 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 12) & 0x3);
//        sys_print("                   MS1DAT4_PUPD[14]   = 0x%x\n", ((*(u32*)(MSDC1_GPIO_DATPUPD_BASE)) >> 14) & 0x1);
//
//        sys_print("[GPIO] Reg[0x%x] MSDC1_PAD            = 0x%x\n", MSDC1_GPIO_PAD_BASE, *(u32*)(MSDC1_GPIO_PAD_BASE));
//        sys_print("                   MS1_TDSEL[3:0]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_PAD_BASE)) >> 0)  & 0xf);
//        sys_print("                   MS1_RDSEL[9:4]     = 0x%x\n", ((*(u32*)(MSDC1_GPIO_PAD_BASE)) >> 4)  & 0x3f);
//        sys_print("                   MS1_BIAS[12:15]    = 0x%x\n", ((*(u32*)(MSDC1_GPIO_PAD_BASE)) >> 12) & 0xf);
//    } 
}


void ett_msdc_dump_register(struct mmc_host *host)
{
#if ETT_STRIP_CODE
    sys_print("[SD%d] Reg[00] MSDC_CFG       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x00));
    sys_print("[SD%d] Reg[04] MSDC_IOCON     = 0x%x\n", host->id,*(u32*)(msdc_base + 0x04));
    sys_print("[SD%d] Reg[08] MSDC_PS        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x08));
    sys_print("[SD%d] Reg[0C] MSDC_INT       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x0C));
    sys_print("[SD%d] Reg[10] MSDC_INTEN     = 0x%x\n", host->id,*(u32*)(msdc_base + 0x10));    
    sys_print("[SD%d] Reg[14] MSDC_FIFOCS    = 0x%x\n", host->id,*(u32*)(msdc_base + 0x14));
    sys_print("[SD%d] Reg[18] MSDC_TXDATA    = not read\n");                        
    sys_print("[SD%d] Reg[1C] MSDC_RXDATA    = not read\n");
    sys_print("[SD%d] Reg[30] SDC_CFG        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x30));
    sys_print("[SD%d] Reg[34] SDC_CMD        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x34));
    sys_print("[SD%d] Reg[38] SDC_ARG        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x38));
    sys_print("[SD%d] Reg[3C] SDC_STS        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x3C));
    sys_print("[SD%d] Reg[40] SDC_RESP0      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x40));
    sys_print("[SD%d] Reg[44] SDC_RESP1      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x44));
    sys_print("[SD%d] Reg[48] SDC_RESP2      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x48));                                
    sys_print("[SD%d] Reg[4C] SDC_RESP3      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x4C));
    sys_print("[SD%d] Reg[50] SDC_BLK_NUM    = 0x%x\n", host->id,*(u32*)(msdc_base + 0x50));
    sys_print("[SD%d] Reg[58] SDC_CSTS       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x58));
    sys_print("[SD%d] Reg[5C] SDC_CSTS_EN    = 0x%x\n", host->id,*(u32*)(msdc_base + 0x5C));
    sys_print("[SD%d] Reg[60] SDC_DATCRC_STS = 0x%x\n", host->id,*(u32*)(msdc_base + 0x60));
    sys_print("[SD%d] Reg[70] EMMC_CFG0      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x70));                        
    sys_print("[SD%d] Reg[74] EMMC_CFG1      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x74));
    sys_print("[SD%d] Reg[78] EMMC_STS       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x78));
    sys_print("[SD%d] Reg[7C] EMMC_IOCON     = 0x%x\n", host->id,*(u32*)(msdc_base + 0x7C));            
    sys_print("[SD%d] Reg[80] SD_ACMD_RESP   = 0x%x\n", host->id,*(u32*)(msdc_base + 0x80));
    sys_print("[SD%d] Reg[84] SD_ACMD19_TRG  = 0x%x\n", host->id,*(u32*)(msdc_base + 0x84));      
    sys_print("[SD%d] Reg[88] SD_ACMD19_STS  = 0x%x\n", host->id,*(u32*)(msdc_base + 0x88));
    sys_print("[SD%d] Reg[90] DMA_SA         = 0x%x\n", host->id,*(u32*)(msdc_base + 0x90));
    sys_print("[SD%d] Reg[94] DMA_CA         = 0x%x\n", host->id,*(u32*)(msdc_base + 0x94));
    sys_print("[SD%d] Reg[98] DMA_CTRL       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x98));
    sys_print("[SD%d] Reg[9C] DMA_CFG        = 0x%x\n", host->id,*(u32*)(msdc_base + 0x9C));                        
    sys_print("[SD%d] Reg[A0] SW_DBG_SEL     = 0x%x\n", host->id,*(u32*)(msdc_base + 0xA0));
    sys_print("[SD%d] Reg[A4] SW_DBG_OUT     = 0x%x\n", host->id,*(u32*)(msdc_base + 0xA4));
    sys_print("[SD%d] Reg[B0] PATCH_BIT0     = 0x%x\n", host->id,*(u32*)(msdc_base + 0xB0));            
    sys_print("[SD%d] Reg[B4] PATCH_BIT1     = 0x%x\n", host->id,*(u32*)(msdc_base + 0xB4));
    sys_print("[SD%d] Reg[E0] PAD_CTL0       = 0x%x\n", host->id,*(u32*)(msdc_base + 0xE0));        
    sys_print("[SD%d] Reg[E4] PAD_CTL1       = 0x%x\n", host->id,*(u32*)(msdc_base + 0xE4));
    sys_print("[SD%d] Reg[E8] PAD_CTL2       = 0x%x\n", host->id,*(u32*)(msdc_base + 0xE8));
    sys_print("[SD%d] Reg[EC] PAD_TUNE       = 0x%x\n", host->id,*(u32*)(msdc_base + 0xEC));
    sys_print("[SD%d] Reg[F0] DAT_RD_DLY0    = 0x%x\n", host->id,*(u32*)(msdc_base + 0xF0));                        
    sys_print("[SD%d] Reg[F4] DAT_RD_DLY1    = 0x%x\n", host->id,*(u32*)(msdc_base + 0xF4));
    sys_print("[SD%d] Reg[F8] HW_DBG_SEL     = 0x%x\n", host->id,*(u32*)(msdc_base + 0xF8));
    sys_print("[SD%d] Reg[100] MAIN_VER      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x100));     
    sys_print("[SD%d] Reg[104] ECO_VER       = 0x%x\n", host->id,*(u32*)(msdc_base + 0x104));

    if (host->id == 0){
//        sys_print("[SD%d] Reg[180] EMMC50_PAD_CTL0         = 0x%x\n", host->id,*(u32*)(msdc_base + 0x180));
//        sys_print("[SD%d] Reg[184] EMMC50_PAD_DS_CTL0      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x184));
//        sys_print("[SD%d] Reg[188] EMMC50_PAD_DS_TUNE      = 0x%x\n", host->id,*(u32*)(msdc_base + 0x188));
//        sys_print("[SD%d] Reg[18C] EMMC50_PAD_CMD_TUNE     = 0x%x\n", host->id,*(u32*)(msdc_base + 0x18C));
//        sys_print("[SD%d] Reg[190] EMMC50_PAD_DAT01_TUNE   = 0x%x\n", host->id,*(u32*)(msdc_base + 0x190));
//        sys_print("[SD%d] Reg[194] EMMC50_PAD_DAT23_TUNE   = 0x%x\n", host->id,*(u32*)(msdc_base + 0x194));
//        sys_print("[SD%d] Reg[198] EMMC50_PAD_DAT45_TUNE   = 0x%x\n", host->id,*(u32*)(msdc_base + 0x198));
//        sys_print("[SD%d] Reg[19C] EMMC50_PAD_DAT67_TUNE   = 0x%x\n", host->id,*(u32*)(msdc_base + 0x19C));
//
//        sys_print("[SD%d] Reg[208] EMMC50_CFG0             = 0x%x\n", host->id,*(u32*)(msdc_base + 0x208));
//        sys_print("[SD%d] Reg[20C] EMMC50_CFG1             = 0x%x\n", host->id,*(u32*)(msdc_base + 0x20C));
//        sys_print("[SD%d] Reg[21C] EMMC50_CFG2             = 0x%x\n", host->id,*(u32*)(msdc_base + 0x21C));
//        sys_print("[SD%d] Reg[220] EMMC50_CFG3             = 0x%x\n", host->id,*(u32*)(msdc_base + 0x220));
//        sys_print("[SD%d] Reg[224] EMMC50_CFG4             = 0x%x\n", host->id,*(u32*)(msdc_base + 0x224));
    }

    msdc_dump_gpio_register(host->id);

#endif
}


u32 ett_msdc_intr_wait(struct mmc_host *host, u32 intrs, struct mmc_command *cmd)
{
    u32 sts;


//    while (((sts = MSDC_READ32(MSDC_INT)) & intrs) == 0){
//        //sys_print("wait cmd interupt(0x%x:0x%x)\n", sts, intrs);
//        ;
//    }
    MSDC_RETRY((((sts = MSDC_READ32(MSDC_INT)) & intrs) == 0), 5, 5000);

#ifdef ETT_MSDC_DBG_LOG_CMD
    sys_print("====[SD%d] INT(0x%x)\n", host->id, sts);
#endif

    MSDC_WRITE32(MSDC_INT, (sts & intrs));
    if (~intrs & sts) {
#if ETT_STRIP_CODE
        sys_print("[SD%d]<CHECKME> Unexpected INT(0x%x)\n", 
            host->id, ~intrs & sts);
#endif 

        ett_msdc_dump_register(host);
        
    }
    
    return sts;
}

int ett_msdc_wait_rsp(struct mmc_host *host, struct mmc_command *cmd)
{
    u32 rsptyp = cmd->rsptyp;
    u32 status;
    u32 opcode = (cmd->opcode & ~(SD_CMD_BIT | SD_CMD_APP_BIT));
    u32 error = MMC_ERR_NONE;
    u32 wints = MSDC_INT_CMDTMO | MSDC_INT_CMDRDY | MSDC_INT_RSPCRCERR |
        MSDC_INT_ACMDRDY | MSDC_INT_ACMDCRCERR | MSDC_INT_ACMDTMO |
        MSDC_INT_ACMD19_DONE;

    if (cmd->opcode == MMC_CMD_GO_IRQ_STATE)
        wints |= MSDC_INT_MMCIRQ;

    status = ett_msdc_intr_wait(host, wints, cmd);

    if (status == 0) {
        error = MMC_ERR_TIMEOUT;
        goto end;
    }
    
    if ((status & MSDC_INT_CMDRDY) || (status & MSDC_INT_ACMDRDY) ||
        (status & MSDC_INT_ACMD19_DONE)) {
        switch (rsptyp) {
        case RESP_NONE:
#ifdef ETT_MSDC_DBG_LOG_CMD
            sys_print("[SD%d] CMD(%d): RSP(%d)\n", host->id, opcode, rsptyp);
#endif
            break;
        case RESP_R2:
        {
            u32 *resp = &cmd->resp[0];
            *resp++ = MSDC_READ32(SDC_RESP3);
            *resp++ = MSDC_READ32(SDC_RESP2);
            *resp++ = MSDC_READ32(SDC_RESP1);
            *resp++ = MSDC_READ32(SDC_RESP0);
#ifdef ETT_MSDC_DBG_LOG_CMD
            sys_print("[SD%d] CMD(%d): RSP(%d) = 0x%x 0x%x 0x%x 0x%x\n", 
                host->id, opcode, cmd->rsptyp, cmd->resp[0], cmd->resp[1], cmd->resp[2], cmd->resp[3]);         
#endif
            break;
        }
        default: /* Response types 1, 3, 4, 5, 6, 7(1b) */
            if ((status & MSDC_INT_ACMDRDY) || (status & MSDC_INT_ACMD19_DONE))
                cmd->resp[0] = MSDC_READ32(SDC_ACMD_RESP);
            else
                cmd->resp[0] = MSDC_READ32(SDC_RESP0);
#ifdef ETT_MSDC_DBG_LOG_CMD
            sys_print("[SD%d] CMD(%d): RSP(%d) = 0x%x AUTO(%d)\n", host->id, opcode, 
                cmd->rsptyp, cmd->resp[0], 
                ((status & MSDC_INT_ACMDRDY) || (status & MSDC_INT_ACMD19_DONE)) ? 1 : 0);
#endif
            break;
        }
    } else if ((status & MSDC_INT_RSPCRCERR) || (status & MSDC_INT_ACMDCRCERR)) {
        error = MMC_ERR_BADCRC;
#ifdef ETT_MSDC_DBG_LOG_CMD
        sys_print("[SD%d] CMD(%d): RSP(%d) ERR(BADCRC)\n", 
            host->id, opcode, cmd->rsptyp);
#endif
    } else if ((status & MSDC_INT_CMDTMO) || (status & MSDC_INT_ACMDTMO)) {
        error = MMC_ERR_TIMEOUT;
#ifdef ETT_MSDC_DBG_LOG_CMD
        sys_print("[SD%d] CMD(%d): RSP(%d) ERR(CMDTO) AUTO(%d)\n", 
            host->id, opcode, cmd->rsptyp, status & MSDC_INT_ACMDTMO ? 1: 0);
#endif
    } else {
        error = MMC_ERR_INVALID;
#if ETT_STRIP_CODE
        sys_print("[SD%d] CMD(%d): RSP(%d) ERR(INVALID), Status:%x\n", 
            host->id, opcode, cmd->rsptyp, status);   
#endif
    }

end:

    if (rsptyp == RESP_R1B) {
        /* card will pull down DAT0 pin for busy signal, 
         * so host need poll DAT0 turn to high */
        //while ((MSDC_READ32(MSDC_PS) & 0x10000) != 0x10000);
        MSDC_RETRY(((MSDC_READ32(MSDC_PS) & 0x10000) != 0x10000), 5, 5000);
    }

    cmd->error = error;
 
    return error;
}

int ett_msdc_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
    int err;

    err = ett_msdc_send_cmd(host, cmd);
    if (err != MMC_ERR_NONE){
#if ETT_STRIP_CODE
        sys_print("======[%s:%d]====cmd send failed==\n", __func__, __LINE__);
#endif 
        return err;
    }

    err = ett_msdc_wait_rsp(host, cmd);

    if (err == MMC_ERR_BADCRC) {
#ifdef ETT_MSDC_DBG_LOG
        sys_print("======[%s:%d]====cmd crc error==\n", __func__, __LINE__);
#endif
    }
    
    if (err == MMC_ERR_TIMEOUT) {
#ifdef ETT_MSDC_DBG_LOG
        sys_print("======[%s:%d]====cmd to error==\n", __func__, __LINE__);
#endif
    }
    return err;
}

int mmc_cmd(struct mmc_host *host, struct mmc_command *cmd)
{
    int err;
    int retry = cmd->retries;
    unsigned char l_int_data_latch_cksel = 0; 
    unsigned char l_ckgen_msdc_dly_sel  = 0;     

    /* use tune result */
    if (g_cmd_tune_done == 1){
        /*  apply command parameter result of clock relate  */
        MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
        MSDC_GET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, g_tune_para.cmd_int_data_latch_cksel);
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, g_tune_para.cmd_ckgen_msdc_dly_sel);
    }


    do {
        err = ett_msdc_cmd(host, cmd);
        if (err == MMC_ERR_NONE)
            break;    
    } while(retry--);

    /* restore the value  */
    if (g_cmd_tune_done == 1){
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_INT_DAT_LATCH_CK_SEL, l_int_data_latch_cksel);
        MSDC_SET_FIELD(MSDC_PATCH_BIT0, MSDC_PB0_CKGEN_MSDC_DLY_SEL, l_ckgen_msdc_dly_sel);
    }
    return err;
}

int mmc_go_idle(struct mmc_host *host)
{
    struct mmc_command cmd;

    cmd.opcode  = MMC_CMD_GO_IDLE_STATE;
    cmd.rsptyp  = RESP_NONE;
    cmd.arg     = 0;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    return mmc_cmd(host, &cmd); 
}

static int mmc_send_if_cond(struct mmc_host *host, u32 ocr)
{
    struct mmc_command cmd;
    int err;
    static const u8 test_pattern = 0xAA;
    u8 result_pattern;

    /*
     * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
     * before SD_APP_OP_COND. This command will harmlessly fail for
     * SD 1.0 cards.
     */

    cmd.opcode  = SD_CMD_SEND_IF_COND;
    cmd.arg     = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err != MMC_ERR_NONE)
        return err;

    result_pattern = cmd.resp[0] & 0xFF;

    if (result_pattern != test_pattern)
        return MMC_ERR_INVALID;

    return MMC_ERR_NONE;
}

static int mmc_send_io_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
    struct mmc_command cmd;
    int i, err = 0;

    ASSERT(host);

    x_memset(&cmd, 0, sizeof(struct mmc_command));

    cmd.opcode = SD_IO_SEND_OP_COND;
    cmd.arg    = ocr;
    cmd.rsptyp = RESP_R4;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    for (i = 100; i; i--) {
        err = mmc_cmd(host, &cmd);
        if (err)
            break;

        /* if we're just probing, do a single pass */
        if (ocr == 0)
            break;

        if (cmd.resp[0] & MMC_CARD_BUSY)
            break;

        err = MMC_ERR_TIMEOUT;
        
        CTP_Wait_msec(10);
    }

    if (rocr)
        *rocr = cmd.resp[0];

    return err;
}


static int mmc_app_cmd(struct mmc_host *host, struct mmc_command *cmd,
                       u32 rca, int retries)
{
    int err = MMC_ERR_FAILED;
    struct mmc_command appcmd;

    appcmd.opcode  = MMC_CMD_APP_CMD;
    appcmd.arg     = rca << 16;
    appcmd.rsptyp  = RESP_R1;
    appcmd.retries = CMD_RETRIES;
    appcmd.timeout = CMD_TIMEOUT;

    // for cmd tuning
    host->app_cmd_arg = appcmd.arg;

    do {
        err = mmc_cmd(host, &appcmd);

        if (err == MMC_ERR_NONE){
            host->app_cmd = TRUE;
            err = mmc_cmd(host, cmd);
        }
        
        if (err == MMC_ERR_NONE){
            host->app_cmd = FALSE;
            break;
        }
    } while (retries--);

    return err;
}

static int mmc_send_app_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
    struct mmc_command cmd;
    int i, err = 0;

    cmd.opcode  = SD_ACMD_SEND_OP_COND;
    cmd.arg     = ocr;
    cmd.rsptyp  = RESP_R3;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    for (i = 100; i; i--) {
        err = mmc_app_cmd(host, &cmd, 0, CMD_RETRIES);
        if (err != MMC_ERR_NONE)
            break;

        if (cmd.resp[0] & MMC_CARD_BUSY || ocr == 0)
            break;

        err = MMC_ERR_TIMEOUT;

        CTP_Wait_msec(10);
    }

    if (rocr)
        *rocr = cmd.resp[0];

    return err;
}

static int mmc_send_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
    struct mmc_command cmd;
    int i, err = 0;

    cmd.opcode  = MMC_CMD_SEND_OP_COND;
    cmd.arg     = ocr;
    cmd.rsptyp  = RESP_R3;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    for (i = 100; i; i--) {
        err = mmc_cmd(host, &cmd);
        if (err)
            break;

        /* if we're just probing, do a single pass */
        if (ocr == 0)
            break;

        if (cmd.resp[0] & MMC_CARD_BUSY)
            break;

        err = MMC_ERR_TIMEOUT;

        CTP_Wait_msec(10);
    }

    if (!err && rocr)
        *rocr = cmd.resp[0];

    return err;
}

static u32 ett_mmc_select_voltage(struct mmc_host *host, u32 ocr)
{
    int bit;

    ocr &= host->ocr_avail;

    bit = uffs(ocr);
    if (bit) {
        bit -= 1;
        ocr &= 3 << bit;
    } else {
        ocr = 0;
    }
    return ocr;
}

static int ett_mmc_send_if_cond(struct mmc_host *host, u32 ocr)
{
    struct mmc_command cmd;
    int err;
    static const u8 test_pattern = 0xAA;
    u8 result_pattern;

    /*
     * To support SD 2.0 cards, we must always invoke SD_SEND_IF_COND
     * before SD_APP_OP_COND. This command will harmlessly fail for
     * SD 1.0 cards.
     */

    cmd.opcode  = SD_CMD_SEND_IF_COND;
    cmd.arg     = ((ocr & 0xFF8000) != 0) << 8 | test_pattern;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err != MMC_ERR_NONE)
        return err;

    result_pattern = cmd.resp[0] & 0xFF;

    if (result_pattern != test_pattern)
        return MMC_ERR_INVALID;

    return MMC_ERR_NONE;
}

static int ett_mmc_send_app_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
    struct mmc_command cmd;
    int i, err = 0;

    cmd.opcode  = SD_ACMD_SEND_OP_COND;
    cmd.arg     = ocr;
    cmd.rsptyp  = RESP_R3;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    for (i = 100; i; i--) {
        err = mmc_app_cmd(host, &cmd, 0, CMD_RETRIES);
        if (err != MMC_ERR_NONE)
            break;

        if (cmd.resp[0] & MMC_CARD_BUSY || ocr == 0)
            break;

        err = MMC_ERR_TIMEOUT;

        CTP_Wait_msec(10000);
    }

    if (rocr)
        *rocr = cmd.resp[0];

    return err;
}

static int ett_mmc_send_op_cond(struct mmc_host *host, u32 ocr, u32 *rocr)
{
    struct mmc_command cmd;
    int i, err = 0;

    cmd.opcode  = MMC_CMD_SEND_OP_COND;
    cmd.arg     = ocr;
    cmd.rsptyp  = RESP_R3;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    for (i = 100; i; i--) {
        err = mmc_cmd(host, &cmd);
        if (err)
            break;

        /* if we're just probing, do a single pass */
        if (ocr == 0)
            break;

        if (cmd.resp[0] & MMC_CARD_BUSY)
            break;

        err = MMC_ERR_TIMEOUT;

        //CTP_Wait_msec(10);
        CTP_Wait_msec(4000);
    }

    if (!err && rocr)
        *rocr = cmd.resp[0];

    return err;
}

static int ett_mmc_all_send_cid(struct mmc_host *host, u32 *cid)
{
    int err;
    struct mmc_command cmd;

    /* send cid */
    cmd.opcode  = MMC_CMD_ALL_SEND_CID;
    cmd.arg     = 0;
    cmd.rsptyp  = RESP_R2;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err != MMC_ERR_NONE)
        return err;

    x_memcpy(cid, cmd.resp, sizeof(u32) * 4);

    return MMC_ERR_NONE;
}

static int ett_mmc_decode_cid(struct mmc_card *card)
{
    u32 *resp = card->raw_cid;

    x_memset(&card->cid, 0, sizeof(struct mmc_cid));

    if (mmc_card_sd(card)) {
        /*
         * SD doesn't currently have a version field so we will
         * have to assume we can parse this.
         */
        card->cid.manfid        = UNSTUFF_BITS(resp, 120, 8);
        card->cid.oemid            = UNSTUFF_BITS(resp, 104, 16);
        card->cid.prod_name[0]    = UNSTUFF_BITS(resp, 96, 8);
        card->cid.prod_name[1]    = UNSTUFF_BITS(resp, 88, 8);
        card->cid.prod_name[2]    = UNSTUFF_BITS(resp, 80, 8);
        card->cid.prod_name[3]    = UNSTUFF_BITS(resp, 72, 8);
        card->cid.prod_name[4]    = UNSTUFF_BITS(resp, 64, 8);
        card->cid.hwrev            = UNSTUFF_BITS(resp, 60, 4);
        card->cid.fwrev            = UNSTUFF_BITS(resp, 56, 4);
        card->cid.serial        = UNSTUFF_BITS(resp, 24, 32);
        card->cid.year            = UNSTUFF_BITS(resp, 12, 8);
        card->cid.month            = UNSTUFF_BITS(resp, 8, 4);

        card->cid.year += 2000; /* SD cards year offset */
    } else {
        /*
         * The selection of the format here is based upon published
         * specs from sandisk and from what people have reported.
         */
        switch (card->csd.mmca_vsn) {
        case 0: /* MMC v1.0 - v1.2 */
        case 1: /* MMC v1.4 */
            card->cid.manfid        = UNSTUFF_BITS(resp, 104, 24);
            card->cid.prod_name[0]  = UNSTUFF_BITS(resp, 96, 8);
            card->cid.prod_name[1]  = UNSTUFF_BITS(resp, 88, 8);
            card->cid.prod_name[2]  = UNSTUFF_BITS(resp, 80, 8);
            card->cid.prod_name[3]  = UNSTUFF_BITS(resp, 72, 8);
            card->cid.prod_name[4]  = UNSTUFF_BITS(resp, 64, 8);
            card->cid.prod_name[5]  = UNSTUFF_BITS(resp, 56, 8);
            card->cid.prod_name[6]  = UNSTUFF_BITS(resp, 48, 8);
            card->cid.hwrev         = UNSTUFF_BITS(resp, 44, 4);
            card->cid.fwrev         = UNSTUFF_BITS(resp, 40, 4);
            card->cid.serial        = UNSTUFF_BITS(resp, 16, 24);
            card->cid.month         = UNSTUFF_BITS(resp, 12, 4);
            card->cid.year          = UNSTUFF_BITS(resp, 8, 4) + 1997;
            break;

        case 2: /* MMC v2.0 - v2.2 */
        case 3: /* MMC v3.1 - v3.3 */
        case 4: /* MMC v4 */
            card->cid.manfid        = UNSTUFF_BITS(resp, 120, 8);
            //card->cid.cbx           = UNSTUFF_BITS(resp, 112, 2);
            card->cid.oemid         = UNSTUFF_BITS(resp, 104, 16);
            card->cid.prod_name[0]  = UNSTUFF_BITS(resp, 96, 8);
            card->cid.prod_name[1]  = UNSTUFF_BITS(resp, 88, 8);
            card->cid.prod_name[2]  = UNSTUFF_BITS(resp, 80, 8);
            card->cid.prod_name[3]  = UNSTUFF_BITS(resp, 72, 8);
            card->cid.prod_name[4]  = UNSTUFF_BITS(resp, 64, 8);
            card->cid.prod_name[5]  = UNSTUFF_BITS(resp, 56, 8);
            card->cid.serial        = UNSTUFF_BITS(resp, 16, 32);
            card->cid.month         = UNSTUFF_BITS(resp, 12, 4);
            card->cid.year          = UNSTUFF_BITS(resp, 8, 4) + 1997;
            break;

        default:
#if ETT_STRIP_CODE
            sys_print("[SD%d] Unknown MMCA version %d\n",
                mmc_card_id(card), card->csd.mmca_vsn);
#endif
       return -1;

        }
    }
    #if ETT_STRIP_CODE
            sys_print("[SD%d] MMCA version %d\n",
                mmc_card_id(card), card->csd.mmca_vsn);
#endif
   return 0;
}

static int ett_mmc_decode_csd(struct mmc_card *card)
{
    struct mmc_csd *csd = &card->csd;
    unsigned int e, m, csd_struct;
    u32 *resp = card->raw_csd;

    if (mmc_card_sd(card)) {
        csd_struct = UNSTUFF_BITS(resp, 126, 2);
        csd->csd_struct = csd_struct;

        switch (csd_struct) {
        case 0:
            m = UNSTUFF_BITS(resp, 115, 4);
            e = UNSTUFF_BITS(resp, 112, 3);
            csd->tacc_ns     = (tacc_exp[e] * tacc_mant[m] + 9) / 10;
            csd->tacc_clks     = UNSTUFF_BITS(resp, 104, 8) * 100;

            m = UNSTUFF_BITS(resp, 99, 4);
            e = UNSTUFF_BITS(resp, 96, 3);
            csd->max_dtr      = tran_exp[e] * tran_mant[m];
            csd->cmdclass      = UNSTUFF_BITS(resp, 84, 12);

            e = UNSTUFF_BITS(resp, 47, 3);
            m = UNSTUFF_BITS(resp, 62, 12);
            csd->capacity      = (1 + m) << (e + 2);

            csd->read_blkbits = UNSTUFF_BITS(resp, 80, 4);
            csd->read_partial = UNSTUFF_BITS(resp, 79, 1);
            csd->write_misalign = UNSTUFF_BITS(resp, 78, 1);
            csd->read_misalign = UNSTUFF_BITS(resp, 77, 1);
            csd->r2w_factor = UNSTUFF_BITS(resp, 26, 3);
            csd->write_blkbits = UNSTUFF_BITS(resp, 22, 4);
            csd->write_partial = UNSTUFF_BITS(resp, 21, 1);

            csd->erase_blk_en = UNSTUFF_BITS(resp, 46, 1);
            csd->erase_sctsz = UNSTUFF_BITS(resp, 39, 7) + 1;
            csd->write_prot_grpsz = UNSTUFF_BITS(resp, 32, 7);
            csd->write_prot_grp = UNSTUFF_BITS(resp, 31, 1);
            csd->perm_wr_prot = UNSTUFF_BITS(resp, 13, 1);
            csd->tmp_wr_prot = UNSTUFF_BITS(resp, 12, 1);
            csd->copy = UNSTUFF_BITS(resp, 14, 1);
            csd->dsr = UNSTUFF_BITS(resp, 76, 1);

            break;
        case 1:
            /*
             * This is a block-addressed SDHC card. Most
             * interesting fields are unused and have fixed
             * values. To avoid getting tripped by buggy cards,
             * we assume those fixed values ourselves.
             */
            mmc_card_set_blockaddr(card);

            csd->tacc_ns     = 0; /* Unused */
            csd->tacc_clks     = 0; /* Unused */

            m = UNSTUFF_BITS(resp, 99, 4);
            e = UNSTUFF_BITS(resp, 96, 3);
            csd->max_dtr      = tran_exp[e] * tran_mant[m];
            csd->cmdclass      = UNSTUFF_BITS(resp, 84, 12);

            m = UNSTUFF_BITS(resp, 48, 22);
            csd->capacity     = (1 + m) << 10;

            csd->read_blkbits = 9;
            csd->read_partial = 0;
            csd->write_misalign = 0;
            csd->read_misalign = 0;
            csd->r2w_factor = 4; /* Unused */
            csd->write_blkbits = 9;
            csd->write_partial = 0;

            csd->erase_blk_en = UNSTUFF_BITS(resp, 46, 1);
            csd->erase_sctsz = UNSTUFF_BITS(resp, 39, 7) + 1;
            csd->write_prot_grpsz = UNSTUFF_BITS(resp, 32, 7);
            csd->write_prot_grp = UNSTUFF_BITS(resp, 31, 1);
            csd->perm_wr_prot = UNSTUFF_BITS(resp, 13, 1);
            csd->tmp_wr_prot = UNSTUFF_BITS(resp, 12, 1);
            csd->copy = UNSTUFF_BITS(resp, 14, 1);
            csd->dsr = UNSTUFF_BITS(resp, 76, 1);
            
            break;
        default:
#if ETT_STRIP_CODE
            sys_print("[SD%d] Unknown CSD ver %d\n", mmc_card_id(card), csd_struct);
#endif
            return MMC_ERR_INVALID;
        }
    } else {
        /*
         * We only understand CSD structure v1.1 and v1.2.
         * v1.2 has extra information in bits 15, 11 and 10.
         */
        csd_struct = UNSTUFF_BITS(resp, 126, 2);

        if (csd_struct != CSD_STRUCT_VER_1_0 && csd_struct != CSD_STRUCT_VER_1_1 
            && csd_struct != CSD_STRUCT_VER_1_2 && csd_struct != CSD_STRUCT_EXT_CSD) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Unknown CSD ver %d\n", mmc_card_id(card), csd_struct);
#endif 
            return MMC_ERR_INVALID;
        }

        csd->csd_struct = csd_struct;
        csd->mmca_vsn    = UNSTUFF_BITS(resp, 122, 4);
        m = UNSTUFF_BITS(resp, 115, 4);
        e = UNSTUFF_BITS(resp, 112, 3);
        csd->tacc_ns     = (tacc_exp[e] * tacc_mant[m] + 9) / 10;
        csd->tacc_clks   = UNSTUFF_BITS(resp, 104, 8) * 100;

        m = UNSTUFF_BITS(resp, 99, 4);
        e = UNSTUFF_BITS(resp, 96, 3);
        csd->max_dtr      = tran_exp[e] * mmc_tran_mant[m];
        csd->cmdclass     = UNSTUFF_BITS(resp, 84, 12);

        e = UNSTUFF_BITS(resp, 47, 3);
        m = UNSTUFF_BITS(resp, 62, 12);
        csd->capacity     = (1 + m) << (e + 2);

        csd->read_blkbits = UNSTUFF_BITS(resp, 80, 4);
        csd->read_partial = UNSTUFF_BITS(resp, 79, 1);
        csd->write_misalign = UNSTUFF_BITS(resp, 78, 1);
        csd->read_misalign = UNSTUFF_BITS(resp, 77, 1);
        csd->r2w_factor = UNSTUFF_BITS(resp, 26, 3);
        csd->write_blkbits = UNSTUFF_BITS(resp, 22, 4);
        csd->write_partial = UNSTUFF_BITS(resp, 21, 1);

        csd->erase_sctsz = (UNSTUFF_BITS(resp, 42, 5) + 1) * (UNSTUFF_BITS(resp, 37, 5) + 1);
        csd->write_prot_grpsz = UNSTUFF_BITS(resp, 32, 7);
        csd->write_prot_grp = UNSTUFF_BITS(resp, 31, 1);
        csd->perm_wr_prot = UNSTUFF_BITS(resp, 13, 1);
        csd->tmp_wr_prot = UNSTUFF_BITS(resp, 12, 1);
        csd->copy = UNSTUFF_BITS(resp, 14, 1);
        csd->dsr = UNSTUFF_BITS(resp, 76, 1);
    }

#if MMC_DEBUG
    mmc_dump_csd(card);
#endif

    return 0;
}

static int ett_mmc_send_relative_addr(struct mmc_host *host, struct mmc_card *card, unsigned int *rca)
{
    int err;
    struct mmc_command cmd;

    x_memset(&cmd, 0, sizeof(struct mmc_command));

    if (mmc_card_mmc(card)) { /* set rca */
        cmd.opcode  = MMC_CMD_SET_RELATIVE_ADDR;
        cmd.arg     = *rca << 16;
        cmd.rsptyp  = RESP_R1;
        cmd.retries = CMD_RETRIES;
        cmd.timeout = CMD_TIMEOUT;
    } else {  /* send rca */
        cmd.opcode  = SD_CMD_SEND_RELATIVE_ADDR;
        cmd.arg     = 0;
        cmd.rsptyp  = RESP_R6;
        cmd.retries = CMD_RETRIES;
        cmd.timeout = CMD_TIMEOUT;
    }
    err = mmc_cmd(host, &cmd);
    if ((err == MMC_ERR_NONE) && !mmc_card_mmc(card))
        *rca = cmd.resp[0] >> 16;

    return err;
}

static int ett_mmc_read_csds(struct mmc_host *host, struct mmc_card *card)
{
    int err;
    struct mmc_command cmd;

    cmd.opcode  = MMC_CMD_SEND_CSD;
    cmd.arg     = card->rca << 16;
    cmd.rsptyp  = RESP_R2;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT * 100;

    err = mmc_cmd(host, &cmd);
    if (err == MMC_ERR_NONE)
        x_memcpy(&card->raw_csd, &cmd.resp[0], sizeof(u32) * 4);
    return err;
}

int ett_mmc_select_card(struct mmc_host *host, struct mmc_card *card)
{
    int err;
    struct mmc_command cmd;

    cmd.opcode  = MMC_CMD_SELECT_CARD;
    cmd.arg     = card->rca << 16;
    cmd.rsptyp  = RESP_R1B;     /* CHECKME */
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    return err;
}

int ett_mmc_send_status(struct mmc_host *host, struct mmc_card *card, u32 *status)
{
    int err;
    struct mmc_command cmd;

    cmd.opcode  = MMC_CMD_SEND_STATUS;
    cmd.arg     = card->rca << 16;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err == MMC_ERR_NONE) {
        *status = cmd.resp[0];
        #if MMC_DEBUG
        mmc_dump_card_status(*status);
        #endif
    }
    return err;
}

int ett_mmc_switch(struct mmc_host *host, struct mmc_card *card, u8 set, u8 index, u8 value)
{
    int err;
    u32 status = 0;
    struct mmc_command cmd;

    cmd.opcode = MMC_CMD_SWITCH;
    cmd.arg = (MMC_SWITCH_MODE_WRITE_BYTE << 24) |
        (index << 16) | (value << 8) | set;
    cmd.rsptyp = RESP_R1B;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err != MMC_ERR_NONE)
        return err;

    do {
        err = ett_mmc_send_status(host, card, &status);
        if (err) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Fail to send status %d\n", host->id, err);
#endif
            break;
        }
        if (status & R1_SWITCH_ERROR) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] switch error. arg(0x%x), status(0x%x)\n", host->id, cmd.arg, status);
#endif 
            return MMC_ERR_FAILED;
        }
    } while (!(status & R1_READY_FOR_DATA) || (R1_CURRENT_STATE(status) == 7));

    return err;
}

int ett_mmc_switch_uhs1(struct mmc_host *host, struct mmc_card *card, unsigned int mode)
{
//    int err;
//    u8  status[64];
//
//    const char *smode[] = { "SDR12", "SDR25", "SDR50", "SDR104", "DDR50" };
//    
//    err = ett_mmc_sd_switch(host, card, 1, 0, mode, (mmc_switch_t*)&status[0]);
//
//    if (err != MMC_ERR_NONE)
//           goto out;
//
//    if ((status[16] & 0xF) != mode) {
//#if ETT_STRIP_CODE
//        sys_print("[SD%d] UHS-1 %s mode not supported!\n", host->id, smode[mode]);
//#endif
//        err = MMC_ERR_FAILED;
//    } else {
//        card->uhs_mode = mode;
//        mmc_card_set_uhs1(card);
//#if ETT_STRIP_CODE
//        sys_print("[SD%d] Switch to UHS-1 %s mode!\n", host->id, smode[mode]);
//#endif
//        if (mode == MMC_SWITCH_MODE_DDR50) {
//            mmc_card_set_ddr(card);
//        }
//    }
//
//out:
//    return err;
    return 0;
}


#if 1
/*
 *  different register settings between eMMC 4.5 backward speed mode and HS400 speed mode
 */
#define HS400_BACKUP_REG_NUM (48)
static struct msdc_reg_control hs400_backup_reg_list[HS400_BACKUP_REG_NUM] = { 
  //{addr,                                         mask,                                value,     reset value, func},
    {(MSDC0_BASE + OFFSET_MSDC_PATCH_BIT1),        (MSDC_PB1_WRDAT_CRCS_TA_CNTR),       0x0,       0x1,         NULL},//0xB4[2:0]
    {(MSDC0_BASE + OFFSET_MSDC_PATCH_BIT0),        (MSDC_PB0_INT_DAT_LATCH_CK_SEL),     0x0,       0x0,         NULL},//0xB0[9:7]
    {(MSDC0_BASE + OFFSET_MSDC_IOCON),             (MSDC_IOCON_DSPL),                   0x0,       0x0,         NULL},//0x04[2:2]
    {(MSDC0_BASE + OFFSET_MSDC_PAD_TUNE),          (MSDC_PAD_TUNE_DATRRDLY),            0x0,       0x0,         NULL},//0xEC[12:8]
    {(MSDC0_BASE + OFFSET_MSDC_IOCON),             (MSDC_IOCON_DDLSEL),                 0x0,       0x0,         NULL},//0x04[3:3]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY0),        (MSDC_DAT_RDDLY0_D3),                0x0,       0x0,         NULL},//0xF0[4:0]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY0),        (MSDC_DAT_RDDLY0_D2),                0x0,       0x0,         NULL},//0xF0[12:8]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY0),        (MSDC_DAT_RDDLY0_D1),                0x0,       0x0,         NULL},//0xF0[20:16]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY0),        (MSDC_DAT_RDDLY0_D0),                0x0,       0x0,         NULL},//0xF0[28:24]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY1),        (MSDC_DAT_RDDLY1_D7),                0x0,       0x0,         NULL},//0xF4[4:0]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY1),        (MSDC_DAT_RDDLY1_D6),                0x0,       0x0,         NULL},//0xF4[12:8]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY1),        (MSDC_DAT_RDDLY1_D5),                0x0,       0x0,         NULL},//0xF4[20:16]
    {(MSDC0_BASE + OFFSET_MSDC_DAT_RDDLY1),        (MSDC_DAT_RDDLY1_D4),                0x0,       0x0,         NULL},//0xF4[28:24]
    {(MSDC0_BASE + OFFSET_MSDC_IOCON),             (MSDC_IOCON_R_D_SMPL_SEL),           0x0,       0x0,         NULL},//0x04[5:5]
    {(MSDC0_BASE + OFFSET_MSDC_IOCON),             (MSDC_IOCON_R_D0SPL),                0x0,       0x0,         NULL},//0x04[16:16]
    {(MSDC0_BASE + OFFSET_MSDC_IOCON),             (MSDC_IOCON_W_D_SMPL),               0x0,       0x0,         NULL},//0x04[8:8]
    {(MSDC0_BASE + OFFSET_MSDC_PAD_TUNE),          (MSDC_PAD_TUNE_DATWRDLY),            0x0,       0x0,         NULL},//0xEC[4:0]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT0_RXDLY3SEL),    0x0,       0x0,         NULL},//0x190[0:0]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT1_RXDLY3SEL),    0x0,       0x0,         NULL},//0x190[16:16]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT2_RXDLY3SEL),    0x0,       0x0,         NULL},//0x194[0:0]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT3_RXDLY3SEL),    0x0,       0x0,         NULL},//0x194[16:16]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT4_RXDLY3SEL),    0x0,       0x0,         NULL},//0x198[0:0]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT5_RXDLY3SEL),    0x0,       0x0,         NULL},//0x198[16:16]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT6_RXDLY3SEL),    0x0,       0x0,         NULL},//0x19C[0:0]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT7_RXDLY3SEL),    0x0,       0x0,         NULL},//0x19C[16:16]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT0_RXDLY3),       0x0,       0x0,         NULL},//0x190[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT1_RXDLY3),       0x0,       0x0,         NULL},//0x190[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT2_RXDLY3),       0x0,       0x0,         NULL},//0x194[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT3_RXDLY3),       0x0,       0x0,         NULL},//0x194[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT4_RXDLY3),       0x0,       0x0,         NULL},//0x198[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT5_RXDLY3),       0x0,       0x0,         NULL},//0x198[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT6_RXDLY3),       0x0,       0x0,         NULL},//0x19C[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT7_RXDLY3),       0x0,       0x0,         NULL},//0x19C[21:17]
    
    /* HQA asked cmd line delay 8 and dat line delay 4 under hs400 mode */
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_CMD_TUNE),    (MSDC_EMMC50_PAD_CMD_TUNE_TXDLY),    0x0,       0x8,         NULL},//0x190[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT0_TXDLY),        0x0,       0x0,         NULL},//0x190[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT01_TUNE),  (MSDC_EMMC50_PAD_DAT1_TXDLY),        0x0,       0x0,         NULL},//0x190[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT2_TXDLY),        0x0,       0x0,         NULL},//0x194[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT23_TUNE),  (MSDC_EMMC50_PAD_DAT3_TXDLY),        0x0,       0x0,         NULL},//0x194[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT4_TXDLY),        0x0,       0x0,         NULL},//0x198[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT45_TUNE),  (MSDC_EMMC50_PAD_DAT5_TXDLY),        0x0,       0x0,         NULL},//0x198[21:17]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT6_TXDLY),        0x0,       0x0,         NULL},//0x19C[5:1]
    {(MSDC0_BASE + OFFSET_EMMC50_PAD_DAT67_TUNE),  (MSDC_EMMC50_PAD_DAT7_TXDLY),        0x0,       0x0,         NULL},//0x19C[21:17]
};

/* need reset some register while switch to hs400 mode with emmc50 
 * do stress test need change mode from hs400 to others, so need backup if switched */
int msdc_register_partial_backup_and_reset(struct mmc_host* host)
{
    int i = 0, err = 0;
    
    for(i = 0; i < HS400_BACKUP_REG_NUM; i++) {
        MSDC_GET_FIELD(hs400_backup_reg_list[i].addr, hs400_backup_reg_list[i].mask,  hs400_backup_reg_list[i].value); 
        MSDC_SET_FIELD(hs400_backup_reg_list[i].addr, hs400_backup_reg_list[i].mask,  hs400_backup_reg_list[i].default_value); 
        if(hs400_backup_reg_list[i].restore_func){
            err = hs400_backup_reg_list[i].restore_func(0); 
            if(err) {
                sys_print("[%s]: failed to restore reg[0x%x][0x%x], expected value[0x%x], actual value[0x%x] err=0x%x", 
                        __func__, hs400_backup_reg_list[i].addr, hs400_backup_reg_list[i].mask, hs400_backup_reg_list[i].default_value, MSDC_READ32(hs400_backup_reg_list[i].addr), err); 
            }
        }
    }
   
    return 0;
}

int msdc_register_partial_restore(struct mmc_host* host)
{
    int i = 0, err = 0; 

    for(i = 0; i < HS400_BACKUP_REG_NUM; i++){
        MSDC_SET_FIELD(hs400_backup_reg_list[i].addr, hs400_backup_reg_list[i].mask,  hs400_backup_reg_list[i].value); 
        if(hs400_backup_reg_list[i].restore_func){
            err = hs400_backup_reg_list[i].restore_func(1); 
            if(err) {
                sys_print("[%s]:failed to restore reg[0x%x][0x%x], expected value[0x%x], actual value[0x%x] err=0x%x", 
                        __func__, hs400_backup_reg_list[i].addr, hs400_backup_reg_list[i].mask, hs400_backup_reg_list[i].value, MSDC_READ32(hs400_backup_reg_list[i].addr), err); 
            }
        }
    }

    return 0;
}


/* need reset some register while switch to hs400 mode with emmc50 */
int mmc_register_partial_reset(struct mmc_host* host)
{
    if (host->id != 0){
        return 1;
    }

    /* back up, then reset to default value */
    msdc_register_partial_backup_and_reset(host);

    return 0;
}

int mmc_register_partial_restore(struct mmc_host* host)
{
    if (host->id != 0){
        return 1;
    }

    /* back up, then reset to default value */
    msdc_register_partial_restore(host);

    return 0;
}

int ett_mmc_set_bus_width(struct mmc_host *host, struct mmc_card *card, int width)
{
    int err = MMC_ERR_NONE;
    u32 arg;
    struct mmc_command cmd;

    if (mmc_card_sd(card)) {
        if (width == HOST_BUS_WIDTH_8) {
            width = HOST_BUS_WIDTH_4;
        }        

        if ((width == HOST_BUS_WIDTH_4) && (host->caps & MMC_CAP_4_BIT_DATA)) {
            arg = SD_BUS_WIDTH_4;
        } else {
            arg = SD_BUS_WIDTH_1;
            width = HOST_BUS_WIDTH_1;
        }
        
        cmd.opcode  = SD_ACMD_SET_BUSWIDTH;
        cmd.arg     = arg;
        cmd.rsptyp  = RESP_R1;
        cmd.retries = CMD_RETRIES;
        cmd.timeout = CMD_TIMEOUT;

        err = mmc_app_cmd(host, &cmd, card->rca, 0);
        if (err != MMC_ERR_NONE)
            goto out;

        ett_msdc_config_bus(host, width);
    } else if (mmc_card_mmc(card)) {
        if (card->csd.mmca_vsn < CSD_SPEC_VER_4)
            goto out;

        if (width == HOST_BUS_WIDTH_8) {
            if (host->caps & MMC_CAP_8_BIT_DATA) {
                /* need make sure card current bus mode */ 
                if (mmc_card_hs400(card) || mmc_card_highspeed(card)){
                    arg = ((host->caps & MMC_CAP_DDR) && card->ext_csd.ddr_support) ? 
                        EXT_CSD_BUS_WIDTH_8_DDR : EXT_CSD_BUS_WIDTH_8;
                } else if (mmc_card_hs200(card) || mmc_card_backyard(card)) {
                    arg = EXT_CSD_BUS_WIDTH_8;
                } else {
                    width = HOST_BUS_WIDTH_4;
                }
                arg = ((host->caps & MMC_CAP_DDR) && card->ext_csd.ddr_support) ? 
                    EXT_CSD_BUS_WIDTH_8_DDR : EXT_CSD_BUS_WIDTH_8;
            } else {
                width = HOST_BUS_WIDTH_4;
            }
        } 

        if (width == HOST_BUS_WIDTH_4) {
            if (host->caps & MMC_CAP_4_BIT_DATA) {
                /* need make sure card current bus mode */ 
                if (mmc_card_hs400(card) || mmc_card_highspeed(card)){
                    arg = ((host->caps & MMC_CAP_DDR) && card->ext_csd.ddr_support) ? 
                        EXT_CSD_BUS_WIDTH_4_DDR : EXT_CSD_BUS_WIDTH_4;
                } else if (mmc_card_hs200(card) || mmc_card_backyard(card)) {
                    arg = EXT_CSD_BUS_WIDTH_4;
                } else {
                    width = HOST_BUS_WIDTH_1;
                }
            } else {
                width = HOST_BUS_WIDTH_1;
            }
        }

        if (width == HOST_BUS_WIDTH_1)
            arg = EXT_CSD_BUS_WIDTH_1;
        
        err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, arg);
        if (err != MMC_ERR_NONE) {
            sys_print("[SD%d] Switch to bus width(0x%x) failed\n", host->id, arg);
            goto out;
        }

        if (arg == EXT_CSD_BUS_WIDTH_8_DDR || arg == EXT_CSD_BUS_WIDTH_4_DDR) {
            mmc_card_set_ddr(card);
        } else {
            mmc_card_clr_ddr(card);
        }
      
        /* maybe the sdr/ddr mode will changed, so need re-config the clock settting 
         * but it's not suggest to configure clock setting at this point [feifei.wang] */
        //msdc_config_clock(host, mmc_card_ddr(card), host->cur_bus_clk);

        ett_msdc_config_bus(host, width);

    } else {
        ASSERT(0); /* card is not recognized */
    }
out: 
    if (mmc_card_sd(card)) {
        sys_print("[info][%s %d] switch to %dbit bus width, err = %d\n", __func__, __LINE__, (arg == SD_BUS_WIDTH_4) ? 4 : 1, err);
    } else {
        switch (arg){
            case EXT_CSD_BUS_WIDTH_1:
                sys_print("[info][%s %d] switch to 1bit bus width, err = %d\n", __func__, __LINE__, err);
                break;
            case EXT_CSD_BUS_WIDTH_4:
                sys_print("[info][%s %d] switch to 4bit bus width, err = %d\n", __func__, __LINE__, err);
                break;
            case EXT_CSD_BUS_WIDTH_8:
                sys_print("[info][%s %d] switch to 8bit bus width, err = %d\n", __func__, __LINE__, err);
                break;
            case EXT_CSD_BUS_WIDTH_4_DDR:
                sys_print("[info][%s %d] switch to 4bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
                break;
            case EXT_CSD_BUS_WIDTH_8_DDR:
                sys_print("[info][%s %d] switch to 8bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
                break;
            default:
                sys_print("[info][%s %d] switch to ?bit bus width(DDR), err = %d\n", __func__, __LINE__, err);
                break;
        }
    }
    return err;
}

void ett_mmc_switch_card_timing(struct mmc_host *host, unsigned int clkhz)
{

	struct mmc_card *card = host->card;
	int result = 0;
	int uhsmode = 0;
	
	if (card && mmc_card_mmc(card)){

		if ((clkhz > MSDC_52M_SCLK) && (host->caps & MMC_CAP_DDR) && (host->caps & MMC_CAP_EMMC_HS400) && !mmc_card_hs400(card)){	
 			if ((ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1) == MMC_ERR_NONE) &&
				(ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DDR) == MMC_ERR_NONE) && 
			    (ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 3) == MMC_ERR_NONE)){
				sys_print("[SD%d] Switch to HS400 mode!\n", host->id);

                mmc_register_partial_reset(host);

                mmc_card_clr_speed_mode(card);
				mmc_card_set_hs400(card);


                mmc_card_set_ddr(card);
			} else {
				result = -__LINE__;
				goto failure;
			}
		} else if ((clkhz > MSDC_52M_SCLK) && (host->caps & MMC_CAP_EMMC_HS200) && !mmc_card_hs200(card)){	
			
			sys_print("[SD%d] prepare Switch to HS200 mode!\n", host->id);

            if (mmc_card_hs400(card)){
                mmc_register_partial_restore(host);
            }
 			
            if ((ett_mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8) == MMC_ERR_NONE) && 	
			    (ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2) == MMC_ERR_NONE)){
				sys_print("[SD%d] Switch to HS200 mode!\n", host->id);
                mmc_card_clr_speed_mode(card);
				mmc_card_set_hs200(card);

                /*if ddr enable, disable it */
                if (host->caps & MMC_CAP_DDR){
                    host->caps &= ~MMC_CAP_DDR;
                    mmc_card_clr_ddr(card);
                } 
            } else {
                result = -__LINE__;
                goto failure;
            }
        } else if ((clkhz > MSDC_26M_SCLK) && (host->caps & MMC_CAP_MMC_HIGHSPEED) && !mmc_card_highspeed(card)){
            
            if (mmc_card_hs400(card)){
                mmc_register_partial_restore(host);
            }
            
            
            
            if ((ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1) == MMC_ERR_NONE) && (ett_mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8) == MMC_ERR_NONE)){
                sys_print("[SD%d] Switch to High-Speed mode!\n", host->id);
                mmc_card_clr_speed_mode(card);
                mmc_card_set_highspeed(card);
            } else {
                result = -__LINE__;
                goto failure;
            }
        } else if (clkhz > 0){

            if (mmc_card_hs400(card)){
                mmc_register_partial_restore(host);
            }

            if (ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 0) == MMC_ERR_NONE){
                sys_print("[SD%d] Switch to Default mode!\n", host->id);
                mmc_card_clr_speed_mode(card);
                mmc_card_set_backyard(card);
            } else {
                result = -__LINE__;
                goto failure;
            }
        }
    } else if (card && mmc_card_sd(card) && (card->version > SD_VER_10)){	
        if ((clkhz > MSDC_100M_SCLK) && (host->caps & MMC_CAP_SD_UHS1)){
            uhsmode = MMC_SWITCH_MODE_SDR104;
        } else if (clkhz > MSDC_52M_SCLK){
            uhsmode = MMC_SWITCH_MODE_SDR50;
        } else if ((clkhz > MSDC_26M_SCLK) && (host->caps & MMC_CAP_SD_HIGHSPEED)){
			if (card->sw_caps.ddr && (host->caps & MMC_CAP_DDR)){
				uhsmode = MMC_SWITCH_MODE_DDR50;
			} else {
				uhsmode = MMC_SWITCH_MODE_SDR25;

                /*if ddr enable, disable it */
                if (host->caps & MMC_CAP_DDR){
                    host->caps &= ~MMC_CAP_DDR;
                    mmc_card_clr_ddr(card);
                }
			}
		} else if (clkhz > 0){
			uhsmode = MMC_SWITCH_MODE_SDR12;
		}

		if (ett_mmc_switch_uhs1(host, card, uhsmode) != 0){
			result = -__LINE__;
			goto failure;
		}
	}
	
failure:
	if(result)
		sys_print("[%s]: result=%d\n", __func__, result);
	return;

}



#else
void ett_mmc_switch_card_timing (struct mmc_host *host, unsigned int clkhz)
{
    int id = host->id;
    struct mmc_card *card = host->card;
    int result = 0;
    int uhsmode = 0;
    
    if(card && mmc_card_mmc(card)){
        if ((clkhz > MSDC_52M_SCLK) && (msdc_cap[id].flags & MSDC_HS200)){
            if(ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2) == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to HS200 mode!\n", host->id);
#endif
                mmc_card_set_hs200(card);
            }else {
                result = -__LINE__;
                goto failure;
            }
        }
        else if ((clkhz > MSDC_26M_SCLK) && (msdc_cap[id].flags & MSDC_HIGHSPEED)) {
            if(ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1) == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to High-Speed mode!\n", host->id);
#endif
                mmc_card_set_highspeed(card);
            }else {
                result = -__LINE__;
                goto failure;
            }
        }else if(clkhz > 0) {
            if(ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 0) == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to Default mode!\n", host->id);
#endif
            }else {
                result = -__LINE__;
                goto failure;
            }
        }
    }else if(card && mmc_card_sd(card)){    
        if ((clkhz > MSDC_100M_SCLK) && (msdc_cap[id].flags & MSDC_UHS1)) {
            uhsmode = MMC_SWITCH_MODE_SDR104;
        } else if (clkhz > MSDC_52M_SCLK) {
            uhsmode = MMC_SWITCH_MODE_SDR50;
        } else if ((clkhz > MSDC_26M_SCLK) && (msdc_cap[id].flags & MSDC_HIGHSPEED)) {//&& freq > 50000000
            if (card->sw_caps.ddr && msdc_cap[id].flags & MSDC_DDR) {
                uhsmode = MMC_SWITCH_MODE_DDR50;
            } else {
                uhsmode = MMC_SWITCH_MODE_SDR25;
            }
        } else if (clkhz > 0){
            uhsmode = MMC_SWITCH_MODE_SDR12;
        }
#if 1
        if (ett_mmc_switch_uhs1(host, card, uhsmode) != 0) {
            result = -__LINE__;
            goto failure;
        }
#endif
    }
failure:
    return;

}
#endif


void ett_mmc_set_clock(struct mmc_host *host, int ddr, unsigned int hz)
{
    unsigned int hs_timing = 0;
    
    if (hz >= host->f_max) {
        hz = host->f_max;
    } else if (hz < host->f_min) {
        hz = host->f_min;
    }
    
    ett_mmc_switch_card_timing(host, hz);
   
    if (host->card && mmc_card_hs400(host->card)){
        hs_timing |= EXT_CSD_HS_TIMEING_HS400;
    }

    if (host->card) {
        ett_msdc_config_clock(host, (mmc_card_ddr(host->card) > 0) ? 1 : 0, hz, hs_timing);
    } else {
        ett_msdc_config_clock(host, ddr ? 1 : 0, hz, hs_timing);
    }
}








int ett_mmc_send_stop(struct mmc_host *host, struct mmc_card *card, u32 *status)
{
    int err;
    struct mmc_command cmd;

    cmd.opcode  = MMC_CMD_STOP_TRANSMISSION;
    cmd.arg     = 0;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_cmd(host, &cmd);

    if (err == MMC_ERR_NONE) {
        *status = cmd.resp[0];
    }
    return err;
}



void ett_msdc_set_blklen(struct mmc_host *host, u32 blklen)
{


    host->blklen     = blklen;
    MSDC_CLR_FIFO();
}

void ett_msdc_set_blknum(struct mmc_host *host, u32 blknum)
{
    
    MSDC_WRITE32(SDC_BLK_NUM, blknum);
}

void msdc_set_timeout(struct mmc_host *host, u32 ns, u32 clks) //u64 ns
{
    u32 timeout, clk_ns;

    clk_ns  = 1000000000UL / host->cur_bus_clk;
    timeout = ns / (u32)clk_ns + clks;
    
#if ETT_STRIP_CODE
    sys_print("host->sclk(%d),timeout(%d)\n",host->cur_bus_clk, timeout);
#endif    
    timeout = timeout >> 20; /* in 1048576 sclk cycle unit (MT6583 MSDC IP)*/
    timeout = timeout > 1 ? timeout - 1 : 0;
    timeout = timeout > 255 ? 255 : timeout;

    MSDC_SET_FIELD(SDC_CFG, SDC_CFG_DTOC, timeout);

#if ETT_STRIP_CODE
    sys_print("[SD%d] Set read data timeout: %dns %dclks -> %d x 65536 cycles\n",
        host->id, ns, clks, timeout + 1);
#endif
}

int ett_msdc_write_fifo(u32 *ptr, u32 size, int loop)
{
    u32  left = size;    
    u32  error = 0;
    u32  intsts = 0; 
    u32* l_ptr = ptr; 
    u32  retry = 0;
    u32  xfer_done = 0;     

    while (left || (xfer_done == 0)) {
        if ((left >= MSDC_FIFO_SZ) && (MSDC_TXFIFOCNT() == 0)) {
            int count = MSDC_FIFO_SZ >> 2;
            do {
                if (loop == 0){
                    MSDC_FIFO_WRITE32(*ptr); ptr++;
                } else if (loop == 0xff){
                    /* for one word loop */
                    MSDC_FIFO_WRITE32(*ptr);
                } else {
                    /* apply 128byte noise bytes */
                    MSDC_FIFO_WRITE32(*ptr); 

                    if ((unsigned int)ptr - (unsigned int)l_ptr == (loop * 2 - 4)){
                        ptr = l_ptr;
                    } else {
                        ptr++;
                    }
                }
            } while (--count);
            left -= MSDC_FIFO_SZ;          
        } else if (left < MSDC_FIFO_SZ && MSDC_TXFIFOCNT() == 0) {
            while (left) {
                if (loop == 0){
                    MSDC_FIFO_WRITE32(*ptr); ptr++;
                } else if (loop == 0xff){
                    MSDC_FIFO_WRITE32(*ptr);
                } else {
                    MSDC_FIFO_WRITE32(*ptr);

                    if ((unsigned int)ptr - (unsigned int)l_ptr == (loop * 2 - 4)){
                        ptr = l_ptr;
                    } else {
                        ptr++;
                    }

                }
                
                left -= 4;   // 512 * n
            }             
        }          
        
        /* need to check if need to abort */
        intsts = MSDC_READ32(MSDC_INT); 
        if(intsts){
            MSDC_WRITE32(MSDC_INT, intsts);  /* clear interrupt */
#ifdef ETT_MSDC_DBG_LOG
            sys_print("[%s %d]pio_write clear intr<0x%x> left<%d>\n", __func__, __LINE__, intsts, left);    
#endif
            if (intsts & MSDC_INT_XFER_COMPL) {
#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]write data done\n", __func__, __LINE__);    
#endif 
                xfer_done = 1;    
            }
            
            if (MSDC_INT_DATCRCERR & intsts) {
                error |= ERROR_CRC; 
#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]write data CRC error: intsts<0x%x>\n", __func__, __LINE__, intsts);    
#endif                
                break;
            }
            if (MSDC_INT_DATTMO & intsts) {
                error |= ERROR_TO;
#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]write data timeout: intsts<0x%x>\n", __func__, __LINE__, intsts);    
#endif                
                break;    
            }
        }        
        
        retry ++; 
        if (!(retry % MSDC_RETRY_COUNT)) {
#if ETT_STRIP_CODE
            sys_print("[%s %d]pio write <%d> times, FIFO<%d>, left<%d>\n", __func__, __LINE__, retry, MSDC_TXFIFOCNT(), left);    
#endif 
        }         
    }
        
    if (error) {
#ifdef ETT_MSDC_DBG_LOG
        sys_print("[%s %d]===error=== write fifo error = 0x%x\n", __func__, __LINE__, error);
#endif
        return error;     
    }
    
    /* check FIFO count is 0 */
    retry = 0;
    while (MSDC_TXFIFOCNT() != 0) {
        retry ++; 
        if (!(retry % MSDC_RETRY_COUNT)) {
#if ETT_STRIP_CODE
            sys_print("[%s %d]pio write wait FIFO 0 <%d> times, FIFO<%d>, left<%d>\n", __func__, __LINE__, retry, MSDC_TXFIFOCNT(), left);    
#endif 
        }          
    }

    return error;     
}

static int ett_msdc_read_fifo(u32 *ptr, u32 size, int loop, int read_pin, int ddr)
{
    u32  left = size;    
    u32  error = 0; //ret
    u32  intsts = 0; 
    u32  retry = 0;
    u32  xfer_done = 0; 
    u32  fifo_have = 0; 
    u32  l_dcrc = 0; 

    while (left || (xfer_done == 0)) {

        /* check fifo have data? */
        fifo_have = MSDC_RXFIFOCNT();
        //sys_print("left= %d =fifi have %d\n", left, fifo_have);
        if ((left >=  MSDC_FIFO_THD) && (fifo_have >= MSDC_FIFO_THD)) {    
            int count = MSDC_FIFO_THD >> 2; 
            do {
                if (loop == 0){
                    *ptr++ = MSDC_FIFO_READ32();
                } else {
                    *ptr = MSDC_FIFO_READ32();
                }
            } while (--count);
            left -= MSDC_FIFO_THD;                
        } else if ((left < MSDC_FIFO_THD) && (fifo_have >= left)) {
            while (left) {
                if (loop == 0){
                    *ptr++ = MSDC_FIFO_READ32();
                } else {
                    *ptr = MSDC_FIFO_READ32();
                }
                left -= 4;  // we are 512 * n
            }            
        }
        
        /* need to check if need to abort */
        intsts = MSDC_READ32(MSDC_INT); 
        if(intsts){
            MSDC_WRITE32(MSDC_INT, intsts);  /* clear interrupt */
#ifdef ETT_MSDC_DBG_LOG
            sys_print("[%s %d]pio_read clear intr<0x%x> left<%d>\n", __func__, __LINE__, intsts, left);    
#endif
            if (intsts & MSDC_INT_XFER_COMPL) {
#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]read data done\n", __func__, __LINE__);   
#endif
                xfer_done = 1;    // we need it. 
            }
            
            if (MSDC_INT_DATCRCERR & intsts) {
#ifndef ETT_SPILT_TUNE
                error |= ERROR_CRC; 
                MSDC_GET_FIELD(SDC_DCRC_STS, SDC_DCRC_STS_POS | SDC_DCRC_STS_NEG, l_dcrc);   
                g_tune_dbg.dcrc = l_dcrc;
#else
                /* find crc error with the test pin */
                MSDC_GET_FIELD(SDC_DCRC_STS, SDC_DCRC_STS_POS | SDC_DCRC_STS_NEG, l_dcrc);   
                g_tune_dbg.dcrc = l_dcrc;
                if (ddr){
                    if (read_pin & (l_dcrc >> 8))
                        error |= ERROR_CRC; 
                } else {
                    if (read_pin & l_dcrc)
                        error |= ERROR_CRC; 
                }
#endif

#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]===error=== read CRC error: intsts<0x%x>\n", __func__, __LINE__, intsts);    
#endif
                break;
            }

            if (MSDC_INT_DATTMO & intsts) {
                error |= ERROR_TO;
#ifdef ETT_MSDC_DBG_LOG
                sys_print("[%s %d]read data timeout: intsts<0x%x> left<%d>\n", __func__, __LINE__, intsts, left);    
#endif                
                break;    
            }
        }
        
        retry ++; 
        if (!(retry % MSDC_RETRY_COUNT)) {
#if ETT_STRIP_CODE
            sys_print("[%s %d]pio read <%d> times, FIFO<%d>, left<%d>\n", __func__, __LINE__, retry, MSDC_RXFIFOCNT(), left);    
#endif
        }        
    } 

    if (error) {
#ifdef ETT_MSDC_DBG_LOG
        sys_print("[%s %d]===error=== read fifo error = 0x%x\n", __func__, __LINE__, error);
#endif
        return error;     
    }

    /* check xfer_done interrupt */ // interrupt when last byte loaded to FIFO
    
#ifndef ETT_SPILT_TUNE
    if (xfer_done == 0) {
        sys_print("[%s %d]Error: no xfer_done interrupt! left<%d> FIFO<%d>\n", __func__, __LINE__, left, MSDC_RXFIFOCNT());
        return 3;
        //while(1);
    }
#endif


    return error;                 
}

static void mmc_decode_ext_csd(struct mmc_card *card, unsigned char* raw_ext_csd)
{
    u8 *ext_csd = raw_ext_csd;    

    card->ext_csd.sectors =
           ext_csd[EXT_CSD_SEC_CNT + 0] << 0 |
        ext_csd[EXT_CSD_SEC_CNT + 1] << 8 |
        ext_csd[EXT_CSD_SEC_CNT + 2] << 16 |
        ext_csd[EXT_CSD_SEC_CNT + 3] << 24;

    card->ext_csd.rev = ext_csd[EXT_CSD_REV];
    card->ext_csd.hc_erase_grp_sz = ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512 * 1024;
    card->ext_csd.hc_wp_grp_sz = ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE] * 512 * 1024;
    card->ext_csd.trim_tmo_ms = ext_csd[EXT_CSD_TRIM_MULT] * 300;
    card->ext_csd.boot_info   = ext_csd[EXT_CSD_BOOT_INFO];
    card->ext_csd.boot_part_sz = ext_csd[EXT_CSD_BOOT_SIZE_MULT] * 128 * 1024;
    card->ext_csd.access_sz = (ext_csd[EXT_CSD_ACC_SIZE] & 0xf) * 512;
    card->ext_csd.rpmb_sz = ext_csd[EXT_CSD_RPMB_SIZE_MULT] * 128 * 1024;
    card->ext_csd.erased_mem_cont = ext_csd[EXT_CSD_ERASED_MEM_CONT];
    card->ext_csd.part_en = ext_csd[EXT_CSD_PART_SUPPORT] & EXT_CSD_PART_SUPPORT_PART_EN ? 1 : 0;
    card->ext_csd.enh_attr_en = ext_csd[EXT_CSD_PART_SUPPORT] & EXT_CSD_PART_SUPPORT_ENH_ATTR_EN ? 1 : 0;
    card->ext_csd.enh_start_addr = 
        (ext_csd[EXT_CSD_ENH_START_ADDR + 0] |
         ext_csd[EXT_CSD_ENH_START_ADDR + 1] << 8 |
         ext_csd[EXT_CSD_ENH_START_ADDR + 2] << 16 |
         ext_csd[EXT_CSD_ENH_START_ADDR + 3] << 24);
    card->ext_csd.enh_sz = 
        (ext_csd[EXT_CSD_ENH_SIZE_MULT + 0] | 
         ext_csd[EXT_CSD_ENH_SIZE_MULT + 1] << 8 | 
         ext_csd[EXT_CSD_ENH_SIZE_MULT + 2] << 16) * 512 * 1024 *
         ext_csd[EXT_CSD_HC_WP_GPR_SIZE] * ext_csd[EXT_CSD_HC_ERASE_GRP_SIZE];

    if (card->ext_csd.sectors)
           mmc_card_set_blockaddr(card);
   
    card->ext_csd.hs_max_dtr = 0; 
    if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS400_1_2V) ||
        (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS400_1_8V)){
		card->ext_csd.hs_max_dtr = 200000000;
		card->ext_csd.ddr_support = 1;
		card->version = EMMC_VER_50; 
    }else if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS200_1_2V) ||
        (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_HS200_1_8V)) {
        card->ext_csd.hs_max_dtr = 200000000;
        if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52_1_2V) ||
               (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52)){ 
            card->ext_csd.ddr_support = 1;
        }
		card->version = EMMC_VER_45; 
    } else if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52_1_2V) ||
               (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_DDR_52)) {
        card->ext_csd.ddr_support = 1;
        card->ext_csd.hs_max_dtr = 52000000;
		card->version = EMMC_VER_44; 
    } else if (ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_52) {
        card->ext_csd.hs_max_dtr = 52000000;
		card->version = EMMC_VER_43; 
    } else if ((ext_csd[EXT_CSD_CARD_TYPE] & EXT_CSD_CARD_TYPE_26)) {
        card->ext_csd.hs_max_dtr = 26000000;
		card->version = EMMC_VER_42; 
    } else {
        /* MMC v4 spec says this cannot happen */
#if ETT_STRIP_CODE
        sys_print("[SD%d] MMCv4 but HS unsupported\n", card->host->id);
#endif
    }

#if MMC_DEBUG
    //mmc_dump_ext_csd(card);
#endif
    return;
}

/* Read and decode extended CSD. */
int ett_mmc_read_ext_csd(struct mmc_host *host, struct mmc_card *card)
{
    int err;
    u32 *ptr;
    struct mmc_command cmd;
    unsigned char raw_ext_csd[512];


    if (card->csd.mmca_vsn < CSD_SPEC_VER_4) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] MMCA_VSN: %d. Skip EXT_CSD\n", host->id, card->csd.mmca_vsn);
#endif
        return MMC_ERR_NONE;
    }

    /*
     * As the ext_csd is so large and mostly unused, we don't store the
     * raw block in mmc_card.
     */
    //memset(&card->raw_ext_csd[0], 0, 512);
    //ptr = (u32*)&card->raw_ext_csd[0];
    ptr = (u32*)raw_ext_csd;

    cmd.opcode  = MMC_CMD_SEND_EXT_CSD;
    cmd.arg     = 0;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;
    
    ett_msdc_set_blknum(host, 1);
    ett_msdc_set_blklen(host, 512);
    ett_msdc_set_timeout(host, 100000000, 0);
    err = mmc_cmd(host, &cmd);
    if (err != MMC_ERR_NONE)
        goto out;

    err = ett_msdc_read_fifo(ptr, 512, 0, 0, 0);
    if (err != MMC_ERR_NONE)
        goto out;
    
    mmc_decode_ext_csd(card, raw_ext_csd);

out:
    return err;
}

int ett_mmc_set_blk_length(struct mmc_host *host, u32 blklen)
{
    int err;
    struct mmc_command cmd;

    /* set block len */
    cmd.opcode  = MMC_CMD_SET_BLOCKLEN;
    cmd.rsptyp  = RESP_R1;
    cmd.arg     = blklen;
    cmd.retries = 3;
    cmd.timeout = CMD_TIMEOUT;
    err = mmc_cmd(host, &cmd);

    if (err == MMC_ERR_NONE)
        ett_msdc_set_blklen(host, blklen);

    return err;
}

int ett_mmc_set_card_detect(struct mmc_host *host, struct mmc_card *card, int connect)
{
    int err;
    struct mmc_command cmd;

    cmd.opcode  = SD_ACMD_SET_CLR_CD;
    cmd.arg     = connect;
    cmd.rsptyp  = RESP_R1; /* CHECKME */
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    err = mmc_app_cmd(host, &cmd, card->rca, CMD_RETRIES);
    return err;
}

static int ett_mmc_read_scrs(struct mmc_host *host, struct mmc_card *card)
{
    int err = MMC_ERR_NONE; 
    int retries;
    struct mmc_command cmd;
    struct sd_scr *scr = &card->scr;
    u32 resp[4];
    u32 tmp;

    ett_msdc_set_blknum(host, 1);
    ett_msdc_set_blklen(host, 8);
    ett_msdc_set_timeout(host, 100000000, 0);

    cmd.opcode  = SD_ACMD_SEND_SCR;
    cmd.arg     = 0;
    cmd.rsptyp  = RESP_R1;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = CMD_TIMEOUT;

    mmc_app_cmd(host, &cmd, card->rca, CMD_RETRIES);
    if ((err != MMC_ERR_NONE) || !(cmd.resp[0] & R1_APP_CMD))
        return MMC_ERR_FAILED;

    retries = 50000;

    /* 8 bytes = 2 words */
    err = ett_msdc_read_fifo(card->raw_scr, 8, 0, 0, 0);
    if (err != MMC_ERR_NONE)
        return err;

#if ETT_STRIP_CODE
    sys_print("[SD%d] SCR: %x %x (raw)\n", host->id, card->raw_scr[0], card->raw_scr[1]);
#endif
    tmp = ett_ntohl(card->raw_scr[0]);
    card->raw_scr[0] = ett_ntohl(card->raw_scr[1]);
    card->raw_scr[1] = tmp;   

#if ETT_STRIP_CODE
    sys_print("[SD%d] SCR: %x %x (ett_ntohl)\n", host->id, card->raw_scr[0], card->raw_scr[1]);
#endif
    resp[2] = card->raw_scr[1];
    resp[3] = card->raw_scr[0];

    if (UNSTUFF_BITS(resp, 60, 4) != 0) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Unknown SCR ver %d\n",
            mmc_card_id(card), UNSTUFF_BITS(resp, 60, 4));
#endif 
        return MMC_ERR_INVALID;
    }

    scr->scr_struct = UNSTUFF_BITS(resp, 60, 4);
    scr->sda_vsn = UNSTUFF_BITS(resp, 56, 4);
    scr->data_bit_after_erase = UNSTUFF_BITS(resp, 55, 1);
    scr->security = UNSTUFF_BITS(resp, 52, 3);
    scr->bus_widths = UNSTUFF_BITS(resp, 48, 4);
    scr->sda_vsn3 = UNSTUFF_BITS(resp, 47, 1);
    scr->ex_security = UNSTUFF_BITS(resp, 43, 4);
    scr->cmd_support = UNSTUFF_BITS(resp, 32, 2);
#if ETT_STRIP_CODE
    sys_print("[SD%d] SD_SPEC(%d) SD_SPEC3(%d) SD_BUS_WIDTH=%d\n", 
        mmc_card_id(card), scr->sda_vsn, scr->sda_vsn3, scr->bus_widths);        
    sys_print("[SD%d] SD_SECU(%d) EX_SECU(%d), CMD_SUPP(%d): CMD23(%d), CMD20(%d)\n", 
        mmc_card_id(card), scr->security, scr->ex_security, scr->cmd_support,
        (scr->cmd_support >> 1) & 0x1, scr->cmd_support & 0x1);
#endif    
    return err;
}




static int ett_mmc_sd_switch(struct mmc_host *host, 
                         struct mmc_card *card, 
                         int mode, int group, u8 value, mmc_switch_t *resp)
{
    int err = MMC_ERR_FAILED;
    struct mmc_command cmd;
    u32 *sts = (u32 *)resp;
    int retries;

    mode = !!mode;
    value &= 0xF;

    /* argument: mode[31]= 0 (for check func.) and 1 (for switch func) */
    cmd.opcode = SD_CMD_SWITCH;
    cmd.arg = mode << 31 | 0x00FFFFFF;
    cmd.arg &= ~(0xF << (group * 4));
    cmd.arg |= value << (group * 4);
    cmd.rsptyp = RESP_R1;
    cmd.retries = CMD_RETRIES;
    cmd.timeout = 100;  /* 100ms */

    ett_msdc_set_blknum(host, 1);
    ett_msdc_set_blklen(host, 64);
    ett_msdc_set_timeout(host, 100000000, 0);
    err = mmc_cmd(host, &cmd);

    if (err != MMC_ERR_NONE)
        goto out;

    retries = 50000;

    /* 512 bits = 64 bytes = 16 words */
    err = ett_msdc_read_fifo(sts, 64, 0, 0, 0);
    if (err != MMC_ERR_NONE)
        goto out;

#if MMC_DEBUG
    {
        int i;
        u8 *byte = (u8*)&sts[0];

        /* Status:   B0      B1    ...
         * Bits  : 511-504 503-495 ...
         */

        for (i = 0; i < 4; i++) {
            MSG(RSP, "  [%d-%d] %xh %xh %xh %xh\n", 
                ((3 - i + 1) << 7) - 1, (3 - i) << 7, 
                sts[(i << 2) + 0], sts[(i << 2) + 1],
                sts[(i << 2) + 2], sts[(i << 2) + 3]);
        }
        for (i = 0; i < 8; i++) {
            MSG(RSP, "  [%d-%d] %xh %xh %xh %xh %xh %xh %xh %xh\n",
                ((8 - i) << 6) - 1, (8 - i - 1) << 6, 
                byte[(i << 3) + 0], byte[(i << 3) + 1], 
                byte[(i << 3) + 2], byte[(i << 3) + 3],
                byte[(i << 3) + 4], byte[(i << 3) + 5], 
                byte[(i << 3) + 6], byte[(i << 3) + 7]);
        }
    }
#endif

out:
    return err;
}


int ett_mmc_switch_hs(struct mmc_host *host, struct mmc_card *card)
{
    int err;
    u8  status[64];
    int val = MMC_SWITCH_MODE_SDR25;

    err = ett_mmc_sd_switch(host, card, 1, 0, val, (mmc_switch_t*)&status[0]);

    if (err != MMC_ERR_NONE)
        goto out;

    if ((status[16] & 0xF) != 1) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] HS mode not supported!\n", host->id);
#endif
        err = MMC_ERR_FAILED;
    } else {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Switch to HS mode!\n", host->id);
#endif
        mmc_card_set_highspeed(card);
    }

out:
    return err;
}





/* Fetches and decodes switch information */
static int ett_mmc_read_switch(struct mmc_host *host, struct mmc_card *card)
{
    int err;
    u8  status[64];

    err = ett_mmc_sd_switch(host, card, 0, 0, 1, (mmc_switch_t*)&status[0]);
    if (err != MMC_ERR_NONE) {
        /* Card not supporting high-speed will ignore the command. */
        err = MMC_ERR_NONE;
        goto out;
    }

    /* bit 511:480 in status[0]. bit 415:400 in status[13] */
    if (status[13] & 0x01) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: Default/SDR12\n", host->id);
#endif
        card->sw_caps.hs_max_dtr = 25000000;  /* default or sdr12 */
    }
    if (status[13] & 0x02) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: HS/SDR25\n", host->id);
#endif
        card->sw_caps.hs_max_dtr = 50000000;  /* high-speed or sdr25 */
    } 
    if (status[13] & 0x10) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: DDR50\n", host->id);
#endif
        card->sw_caps.hs_max_dtr = 50000000;  /* ddr50 */
        card->sw_caps.ddr = 1;
    }
    if (status[13] & 0x04) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: SDR50\n", host->id);
#endif
        card->sw_caps.hs_max_dtr = 100000000; /* sdr50 */    
    }    
    if (status[13] & 0x08) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: SDR104\n", host->id);
#endif
        card->sw_caps.hs_max_dtr = 208000000; /* sdr104 */
    }    
    if (status[9] & 0x01) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: Type-B Drv\n", host->id);
#endif
    }    
    if (status[9] & 0x02) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: Type-A Drv\n", host->id);
#endif
    } 
    if (status[9] & 0x04) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: Type-C Drv\n", host->id);
#endif
    }    
    if (status[9] & 0x08) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: Type-D Drv\n", host->id);
#endif
    }
    if (status[7] & 0x01) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: 200mA current limit\n", host->id);
#endif
    }    
    if (status[7] & 0x02) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: 400mA current limit\n", host->id);
#endif
    } 
    if (status[7] & 0x04) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: 600mA current limit\n", host->id);
#endif
    }    
    if (status[7] & 0x08) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Support: 800mA current limit\n", host->id);
#endif
    }

out:
    return err;
}

int ett_mmc_switch_drv_type(struct mmc_host *host, struct mmc_card *card, int val)
{
    int err;
    u8  status[64];
    const char *type[] = { "TYPE-B", "TYPE-A", "TYPE-C", "TYPE-D" };
    
    err = ett_mmc_sd_switch(host, card, 1, 2, val, (mmc_switch_t*)&status[0]);

    if (err != MMC_ERR_NONE)
           goto out;

    if ((status[15] & 0xF) != val) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] UHS-1 %s drv not supported!\n", host->id, type[val]);
#endif
        err = MMC_ERR_FAILED;
    } else {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Switch to UHS-1 %s drv!\n", host->id, type[val]);
#endif
    }

out:
    return err;
}

int ett_mmc_switch_max_cur(struct mmc_host *host, struct mmc_card *card, int val)
{
    int err;
    u8  status[64];
    const char *curr[] = { "200mA", "400mA", "600mA", "800mA" };
    
    err = ett_mmc_sd_switch(host, card, 1, 3, val, (mmc_switch_t*)&status[0]);

    if (err != MMC_ERR_NONE)
           goto out;

    if (((status[15] >> 4) & 0xF) != val) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] UHS-1 %s max. current not supported!\n", host->id, curr[val]);
#endif
        err = MMC_ERR_FAILED;
    } else {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Switch to UHS-1 %s max. current!\n", host->id, curr[val]);
#endif
    }

out:
    return err;
}

int msdc_switch_volt(struct mmc_host *host, int volt)
{
    int err = MMC_ERR_FAILED;

    u32 status;

    u32 sclk = host->cur_bus_clk;

    /* make sure SDC is not busy (TBC) */
//    while (!SDC_IS_BUSY()){
//#if ETT_STRIP_CODE
//        sys_print("[%s %d] wait sdc busy signal clear\n", __func__, __LINE__);
//#endif
//    }
    MSDC_RETRY((!SDC_IS_BUSY()), 5, 5000);

    /* pull up disabled in CMD and DAT[3:0] to allow card drives them to low */

    /* check if CMD/DATA lines both 0 */
    if ((MSDC_READ32(MSDC_PS) & ((1 << 24) | (0xF << 16))) == 0) {

        /* pull up disabled in CMD and DAT[3:0] */
        //ett_msdc_config_pin(host, MSDC_PIN_PULL_NONE);

        /* change signal from 3.3v to 1.8v */
        //msdc_sd_power_switch(host, 1);
        //upmu_set_rg_vmc_vosel(1);
        //upmu_set_rg_vmc_en(1);

        /* wait at least 5ms for 1.8v signal switching in card */
        CTP_Wait_msec(10);

        /* config clock to 10~12MHz mode for volt switch detection by host. */
        ett_msdc_config_clock(host, 0, 12000000, 0);/*For FPGA 13MHz clock,this not work*/

        /* pull up enabled in CMD and DAT[3:0] */
//        ett_msdc_config_pin(host, MSDC_PIN_PULL_UP);
        CTP_Wait_msec(100005);

        /* start to detect volt change by providing 1.8v signal to card */
        MSDC_SET_BIT32(MSDC_CFG, MSDC_CFG_BV18SDT);

        /* wait at max. 1ms */
        CTP_Wait_msec(1);

        //while ((status = MSDC_READ32(MSDC_CFG)) & MSDC_CFG_BV18SDT);
        MSDC_RETRY(((status = MSDC_READ32(MSDC_CFG)) & MSDC_CFG_BV18SDT), 5, 5000);

        if (status & MSDC_CFG_BV18PSS)
            err = MMC_ERR_NONE;

        /* config clock back to init clk freq. */
        ett_msdc_config_clock(host, 0, sclk, 0);
    }   


        
    return err;
}


static int ett_mmc_switch_volt(struct mmc_host *host, struct mmc_card *card)
{
    int err = 0;;
//    struct mmc_command cmd;
//    
//    cmd.opcode  = SD_CMD_VOL_SWITCH;
//    cmd.arg     = 0;
//    cmd.rsptyp  = RESP_R1;
//    cmd.retries = CMD_RETRIES;
//    cmd.timeout = CMD_TIMEOUT;
//
//    err = mmc_cmd(host, &cmd);
//
//    if (err == MMC_ERR_NONE)
//        err = msdc_switch_volt(host, MMC_VDD_18_19);

    return err;
}
 
#define MSDC_USE_MODE_SWITCH_WHILE_INIT 0 //tbd


u32 maxclock = 0;

int ett_mmc_init_mem_card(struct mmc_host *host, struct mmc_card *card, u32 ocr)
{
    int err, id = host->id;
    int s18a = 0;

    /*
     * Sanity check the voltages that the card claims to
     * support.
     */
    if (ocr & 0x7F) {
#if ETT_STRIP_CODE
        sys_print("card claims to support voltages "
            "below the defined range. These will be ignored.\n");
#endif
        ocr &= ~0x7F;
    }

    ocr = host->ocr = ett_mmc_select_voltage(host, ocr);

    /*
     * Can we support the voltage(s) of the card(s)?
     */
    if (!host->ocr) {
        err = MMC_ERR_FAILED;
        goto out;
    }

    mmc_go_idle(host);

    /* send interface condition */
    if (mmc_card_sd(card))
        err = ett_mmc_send_if_cond(host, ocr);

    /* host support HCS[30] */
    ocr |= (1 << 30);

    if (!err) {
        /* host support S18A[24] and XPC[28]=1 to support speed class */
        if (host->caps & MMC_CAP_SD_UHS1)
            ocr |= ((1 << 28) | (1 << 24));
    }

    /* send operation condition */
    if (mmc_card_sd(card)) {
        err = ett_mmc_send_app_op_cond(host, ocr, &card->ocr);
    } else {
        /* The extra bit indicates that we support high capacity */
        err = ett_mmc_send_op_cond(host, ocr, &card->ocr);
    }

    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in SEND_OP_COND cmd(0x%x)\n", id, err);
#endif
        goto out;
    }

    /* set hcs bit if a high-capacity card */
    card->state |= ((card->ocr >> 30) & 0x1) ? MMC_STATE_HIGHCAPS : 0;
    s18a = (card->ocr >> 24) & 0x1;

    /* S18A support by card. switch to 1.8V signal */
    if (s18a) {
        err = ett_mmc_switch_volt(host, card);        
        if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
            sys_print("[%s %d:SD%d] Fail in SWITCH_VOLT cmd\n", __func__, __LINE__, id);
#endif
            goto out;
        }
    }
    /* send cid */
    err = ett_mmc_all_send_cid(host, card->raw_cid);

    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in SEND_CID cmd\n", id);
#endif
        goto out;
    }
    

    if (mmc_card_mmc(card))
        card->rca = 0x1; /* assign a rca */

    /* set/send rca */
    err = ett_mmc_send_relative_addr(host, card, &card->rca);
    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in SEND_RCA cmd\n", id);
#endif
        goto out;
    }

    /* send csd */
    err = ett_mmc_read_csds(host, card);
    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in SEND_CSD cmd\n", id);
#endif
        goto out;        
    }

    /* decode csd */
    err = ett_mmc_decode_csd(card);
    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in decode csd\n", id);
#endif
        goto out;        
    }
    
    err = ett_mmc_decode_cid(card);
    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in decode csd\n", id);
#endif
        goto out;        
    }

    /* select this card */
    err = ett_mmc_select_card(host, card);
    if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
        sys_print("[SD%d] Fail in select card cmd\n", id);
#endif
        goto out;
    }

    if (mmc_card_sd(card)) {
        /* send scr */    
        err = ett_mmc_read_scrs(host, card);
        if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Fail in SEND_SCR cmd\n", id);
#endif
            goto out;
        }

        if ((card->csd.cmdclass & CCC_SWITCH) && 
            (ett_mmc_read_switch(host, card) == MMC_ERR_NONE)) {
            do {
                if (s18a && (host->caps & MMC_CAP_SD_UHS1)) {
                    /* TODO: Switch driver strength first then current limit
                     *       and access mode */
                    unsigned int freq, uhs_mode, drv_type, max_curr;
                    freq = min(host->f_max, card->sw_caps.hs_max_dtr);

                    if (freq > 100000000) {
                        uhs_mode = MMC_SWITCH_MODE_SDR104;
                    } else if (freq <= 100000000 ) {//&& freq > 50000000
                        if (card->sw_caps.ddr && (host->caps & MMC_CAP_DDR)) {
                            uhs_mode = MMC_SWITCH_MODE_DDR50;
                        } else {
                            uhs_mode = MMC_SWITCH_MODE_SDR50;
                        }
                    } else if (freq <= 50000000 && freq > 25000000) {
                        uhs_mode = MMC_SWITCH_MODE_SDR25;
                    } else {
                        uhs_mode = MMC_SWITCH_MODE_SDR12;
                    }
                    drv_type = MMC_SWITCH_MODE_DRV_TYPE_B;
                    max_curr = MMC_SWITCH_MODE_CL_200MA;

                    if (ett_mmc_switch_drv_type(host, card, drv_type) == MMC_ERR_NONE &&
                        ett_mmc_switch_max_cur(host, card, max_curr) == MMC_ERR_NONE && 
                        ett_mmc_switch_uhs1(host, card, uhs_mode) == MMC_ERR_NONE) {
                        break;
                    } else {
                        ett_mmc_switch_drv_type(host, card, MMC_SWITCH_MODE_DRV_TYPE_B);
                        ett_mmc_switch_max_cur(host, card, MMC_SWITCH_MODE_CL_200MA);                        
                    }
                }

                if ((!(host->caps & MMC_CAP_SD_UHS1)) && (host->caps & MMC_CAP_SD_HIGHSPEED)) {
                    ett_mmc_switch_hs(host, card);
                    break;
                }
            } while(0);
        }

        /* set bus width */
        ett_mmc_set_bus_width(host, card, HOST_BUS_WIDTH_4); 

        /* compute bus speed. */
        card->maxhz = (unsigned int)-1;

        if (mmc_card_highspeed(card) || mmc_card_uhs1(card)) {
            if (card->maxhz > card->sw_caps.hs_max_dtr)
                card->maxhz = card->sw_caps.hs_max_dtr;
        } else if (card->maxhz > card->csd.max_dtr) {
            card->maxhz = card->csd.max_dtr;
        }
    } else {
        /* at the begin, the emmc card is under backward mode, this mode can support 1/4/8 buswidth */
        mmc_card_set_backyard(card);

        /* set bus width, if support HS200, needs to set 4 or 8 width first. */
        //ett_mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8);
    
        /*HS200 make sure host voltage support HS200*/
        if ((host->caps & MMC_CAP_EMMC_HS200) && !(host->ocr_avail & 0x80)){
            host->caps  = host->caps &(~MMC_CAP_EMMC_HS200);
#if ETT_STRIP_CODE
            sys_print("[SD%d] can not switch to HS200:Host voltage not support(0x%x)!\n",id,host->ocr_avail);
#endif
        }
        
        /* send ext csd */
        err = ett_mmc_read_ext_csd(host, card);
        if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Fail in SEND_EXT_CSD cmd\n", id);
#endif
            goto out;        
        }
     
#if (1 ==  MSDC_USE_MODE_SWITCH_WHILE_INIT)
        /* activate high speed (if supported) */
        if ((card->ext_csd.hs_max_dtr > MSDC_52M_SCLK) && (host->caps & MMC_CAP_EMMC_HS400)){
			err = ett_mmc_set_blk_length(host, MMC_BLOCK_SIZE);

			if(err == MMC_ERR_NONE)
                err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2);
			else
				sys_print("[SD%d] Fail in set blklen cmd, card state=0x%x\n", id, card->state);
			
			if(err == MMC_ERR_NONE)			
        		err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);
			else 
				sys_print("[SD%d] Switch to HS200 mode failed!\n", host->id); 
			
			if(err == MMC_ERR_NONE)			
        		err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, EXT_CSD_BUS_WIDTH_8_DDR);
			else 
				sys_print("[SD%d] Switch to High-Speed mode failed!\n", host->id); 	        
				
			if(err == MMC_ERR_NONE)
				err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 3);
			else 
				sys_print("[SD%d] Switch to DDR mode failed!\n", id);
			
            if (err == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to HS400 mode!\n", host->id);
#endif
                mmc_card_set_hs400(card);
            }
            else {
				sys_print("[SD%d] Switch to HS400 mode failed!\n", host->id); 
            }
		} else if ((card->ext_csd.hs_max_dtr > 52000000) && (host->caps & MMC_CAP_EMMC_HS200)){
            err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 2);
            if (err == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to HS200 mode!\n", host->id);
#endif
                mmc_card_set_hs200(card);
            }
        } else if ((card->ext_csd.hs_max_dtr != 0) && (host->caps & MMC_CAP_MMC_HIGHSPEED)) {
            err = ett_mmc_switch(host, card, EXT_CSD_CMD_SET_NORMAL, EXT_CSD_HS_TIMING, 1);

            if (err == MMC_ERR_NONE) {
#if ETT_STRIP_CODE
                sys_print("[SD%d] Switch to High-Speed mode!\n", host->id);
#endif
                mmc_card_set_highspeed(card);
            }
        }
#endif

        /* set bus width */
        //ett_mmc_set_bus_width(host, card, HOST_BUS_WIDTH_8);

        /* compute bus speed. */
        card->maxhz = (unsigned int)-1;

        if (card->ext_csd.hs_max_dtr != 0) {
            if (card->maxhz > card->ext_csd.hs_max_dtr)
                card->maxhz = card->ext_csd.hs_max_dtr;
        } else if (card->maxhz > card->csd.max_dtr) {
            card->maxhz = card->csd.max_dtr;
        }

    }
    
    /* set block len. note that cmd16 is illegal while mmc card is in ddr mode */
    if (!(mmc_card_mmc(card) && (mmc_card_ddr(card) || mmc_card_hs400(card)))) {
        err = ett_mmc_set_blk_length(host, MMC_BLOCK_SIZE);
        if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Fail in set blklen cmd\n", id);
#endif
            goto out;
        }
    }

    /* set clear card detect */
    if (mmc_card_sd(card))
        ett_mmc_set_card_detect(host, card, 0);

    if (!mmc_card_sd(card) && mmc_card_blockaddr(card)) {
        /* The EXT_CSD sector count is in number or 512 byte sectors. */
        card->blklen = MMC_BLOCK_SIZE;
        card->nblks  = card->ext_csd.sectors;
    } else {
        /* The CSD capacity field is in units of read_blkbits.
         * set_capacity takes units of 512 bytes.
         */
        card->blklen = MMC_BLOCK_SIZE;
        card->nblks  = card->csd.capacity << (card->csd.read_blkbits - 9);
    }

#if ETT_STRIP_CODE
    sys_print("[SD%d] Size: %d MB, Max.Speed: %d kHz, blklen(%d), nblks(%d), ro(%d)\n", 
        id, ((card->nblks / 1024) * card->blklen) / 1024 , card->maxhz / 1000,
        card->blklen, card->nblks, mmc_card_readonly(card));
#endif

    card->ready = 1;
    
    if(maxclock == 50) card->maxhz = 52000000;
    if(maxclock == 200) card->maxhz = 200000000;
    if(maxclock == 162) card->maxhz = 162000000;
    		
     if(maxclock == 162)
    	{
    	   MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_CLK_TXDLY,0x8);
    	}
    	if(maxclock == 200)
    	{
    	   MSDC_SET_FIELD(MSDC_PAD_TUNE, MSDC_PAD_CLK_TXDLY,0xc);
    	}
    	

#if ETT_STRIP_CODE
    sys_print("[SD%d] Initialized, max hz %d\n", id,card->maxhz);
#endif

out:
    return err;
}


int ett_mmc_init_card(struct mmc_host *host, struct mmc_card *card)
{
    int err, id = host->id;
    u32 ocr;
    
    //memset(card, 0, sizeof(struct mmc_card));

    if (ett_msdc_card_protected(host))
        mmc_card_set_readonly(card);

    mmc_card_set_present(card);
    mmc_card_set_host(card, host);
    mmc_card_set_unknown(card);

    mmc_go_idle(host);

    /* send interface condition */
    err = mmc_send_if_cond(host, host->ocr_avail);
    if(err == MMC_ERR_TIMEOUT)
        sys_print("[SD%d] Fail in SEND_IF_COND cmd\n",id);
    if (mmc_send_io_op_cond(host, 0, &ocr) == MMC_ERR_NONE) {
        goto out;
    }
    /* query operation condition */ 
    err = mmc_send_app_op_cond(host, 0, &ocr);
    if (err != MMC_ERR_NONE) {
        err = mmc_send_op_cond(host, 0, &ocr);
        if (err != MMC_ERR_NONE) {
#if ETT_STRIP_CODE
            sys_print("[SD%d] Fail in MMC_CMD_SEND_OP_COND/SD_ACMD_SEND_OP_COND cmd\n", id);
#endif
            goto out;
        }
        mmc_card_set_mmc(card);
    } else {
#if ETT_STRIP_CODE
        sys_print("[%s %d:SD%d] begin sd card init flow\n", __func__, __LINE__, id);
#endif
        mmc_card_set_sd(card);
    }

    err = ett_mmc_init_mem_card(host, card, ocr);

    if (err)
        goto out;

out:
    if (err) {
        ett_msdc_power(host, MMC_POWER_OFF);
        return err;
    }
    host->card = card;    
    
    ett_mmc_set_clock(host, mmc_card_ddr(card), 0);

    ett_mmc_set_clock(host, mmc_card_ddr(card), card->maxhz);
    return 0;
}

int ett_mmc_init(int id, u32 trans_mode)
{   
    int err = MMC_ERR_NONE;
    struct mmc_host *host;
    struct mmc_card *card;

    host = &ett_sd_host;
    card = &ett_sd_card;
    x_memset(host, 0, sizeof(ett_sd_host));
    x_memset(card, 0, sizeof(ett_sd_card));    
    x_memset(&g_tune_para, 0, sizeof(g_tune_para)); 
    x_memset(&g_tune_dbg, 0, sizeof(g_tune_dbg)); 
    err = ett_mmc_init_host(id, host, -1, trans_mode);

   // change host->f_max here, for sd30 mode slection
   if (host->id == 1){
       switch (trans_mode){
           case 1:
               host->f_max = 50000000;        /* sdr@highspeed */
               break;
           case 2:
               host->f_max = 100000000;       /* sdr@sdr50 */
               break;
           case 3:
               host->f_max  = MSDC_MAX_SCLK;  /* sdr@sdr104*/
               break;
           case 4:
               host->f_max = 100000000;      /* ddr@ddr50 */
               break;
           default:
               break;
       }
   }

    if (err == MMC_ERR_NONE)
        err = ett_mmc_init_card(host, card);


    /* dump the register before the test */
    ett_msdc_dump_register(host);


    return err;
}

int mmc_rw_test(struct mmc_host *host, struct mmc_card *card, struct mmc_command *cmd, int dat_line)
{
    unsigned int l_buf_r = 0,test_i = 0;

    unsigned int l_ret = 0, l_status = 0, l_err = 0;
    
//    msdc_priv_t *priv = (msdc_priv_t*)host->priv;
    
//    priv->autocmd |= MSDC_AUTOCMD12;
    
    //sys_print("yf mmc_rw_test op == %d\n",cmd->opcode);

    /* send cmd first */
    if (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK || cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK){
        /* for multi r/w, fixed with 44kB */ 
        ett_msdc_set_blknum(host, 88);
    } else {
        /* single bloc r/w */ 
        ett_msdc_set_blknum(host, 1);
    }
    ett_msdc_set_blklen(host, 512);
    ett_msdc_set_timeout(host, 100000000, 0);


    /* no need do error handle with r/w cmd failed */
    /* command will always pass after command tuning test */
    l_ret = mmc_cmd(host, cmd);
    if (l_ret != MMC_ERR_NONE){
		#if ETT_STRIP_CODE
//        sys_print("[%s:%d] send cmd failed(%d)\n", __func__, __LINE__, l_ret);
		#endif
        //return l_ret;
        goto err;
    }

    if (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK){
        /* pio read */
        l_ret = ett_msdc_read_fifo(&l_buf_r, 88 * 512, 1, dat_line, mmc_card_ddr(host->card));

        if (l_ret != MMC_ERR_NONE)
            goto err;
    } else if (cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK){
        /* pio write */
        //l_ret = ett_msdc_write_fifo(&l_buf_w, 8 * 512, 0xff);
        l_ret = ett_msdc_write_fifo((unsigned int*)g_block_pattern, 88 * 512, 64);
        if (l_ret != MMC_ERR_NONE)
            goto err;
    } else if (cmd->opcode == MMC_CMD_READ_SINGLE_BLOCK){
        /* pio read */
        l_ret = ett_msdc_read_fifo(&l_buf_r, 512, 1, dat_line, mmc_card_ddr(host->card));

        if (l_ret != MMC_ERR_NONE)
            goto err;
    }
    
        if ((cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK) || (cmd->opcode == MMC_CMD_READ_MULTIPLE_BLOCK)){
        /* send stop cmd */
        l_err = ett_mmc_send_stop(host, card, &l_status);
        if (l_err != MMC_ERR_NONE){
              goto err;
        }
    }

    /* wait card program done for write operation */
    if (cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK){
        l_status = 0; 
        test_i = 0;
        while ((((l_status >> 9) & 0xF) != 4) && (test_i ++ < 300000)){
        	HAL_Delay_us(10);
            l_ret = ett_mmc_send_status(host, card, &l_status);
            if (l_ret != MMC_ERR_NONE){
            	return l_ret;
            }
        }
    }
    
    if(test_i >= 300000)
    	{
    		sys_print("[%s] failed to wait card program done for write operation state %d\n", __func__, ((l_status >> 9) & 0xF));
    	}

    return l_ret;

err:
    /* abort host first */
    //sys_print("[%s:%d] r/w failed\n", __func__, __LINE__);
    ett_msdc_abort(host);

resend:
    /* bring card to transfer state */
    l_err = ett_mmc_send_status(host, card, &l_status);
    if (l_err != MMC_ERR_NONE){
       return l_err;
    }
    
    /* if card not in trans state,do polling; 
     * if card in [data] or [rec] state, need send cmd12 */
    if ((((l_status >> 9) & 0xF) == 5) || (((l_status >> 9) & 0xF) == 6)){ 
		#ifdef ETT_MMC_DBG_LOG
        sys_print("[%s:%d]Send CMD12 for state back\n", __func__, __LINE__);
		#endif 
        l_err = ett_mmc_send_stop(host, card, &l_status);
        /* we must makesure cmd12 is send without crc error */
        if (l_err != MMC_ERR_NONE) {
			#if ETT_STRIP_CODE
            //sys_print("[%s:%d]Send CMD12 error\n", __func__, __LINE__);
			#endif 
            goto resend;
        }

        /* crc error, after send cmd12, wait card program done for write operation */
        if (cmd->opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK){
            l_status = 0; 
            test_i = 0;
            while ((((l_status >> 9) & 0xF) != 4) && (test_i ++ < 200000)){
            		HAL_Delay_us(10);
                l_err = ett_mmc_send_status(host, card, &l_status);
                if (l_err != MMC_ERR_NONE){
                   return l_err;
                }
            }
            if(test_i >= 200000)
            {
            	sys_print("[%s] failed1 to wait card program done for write operation\n", __func__, __LINE__, l_ret);
            }
        }
    } else if (((l_status >> 9) & 0xF) != 4){
        goto resend;
    }

    return l_ret;
}

/* make sure the basic r/w func is ok */
/* rule is write -> read -> compara */
unsigned char g_buf_r[512*8] = {0};
unsigned char g_buf_w[512*8] = {0};
int mmc_basic_rw_test(void)
{
    unsigned int l_ret = 0, l_status = 0, i;
    struct mmc_host *host;
    struct mmc_card *card;

    struct mmc_command cmd;

    host = &ett_sd_host;
    card = &ett_sd_card;

    ett_memset(&cmd, sizeof(struct mmc_command), 0x0);
    ett_memset(g_buf_w, 8*512, 0x5a);

    /* send cmd25, not to try tuning */ 
    cmd.opcode  = MMC_CMD_WRITE_MULTIPLE_BLOCK;
    cmd.arg     = 0x200000;     // offset is 1GB
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    /* send cmd first */
    ett_msdc_set_blknum(host, 8);
    ett_msdc_set_blklen(host, 512);
    ett_msdc_set_timeout(host, 100000000, 0);

    /* no need do error handle with r/w cmd failed */
    /* command will always pass after command tuning test */
    l_ret = mmc_cmd(host, &cmd);
    if (l_ret != MMC_ERR_NONE){
        sys_print("[%s:%d] send cmd failed", __func__, __LINE__);
        return l_ret;
    }

    if (cmd.opcode == MMC_CMD_WRITE_MULTIPLE_BLOCK){
        /* pio write */
        l_ret = ett_msdc_write_fifo((u32*)g_buf_w, 8 * 512, 0);
        if (l_ret != MMC_ERR_NONE)
            goto err;
    }

    /* send stop cmd */
    l_ret = ett_mmc_send_stop(host, card, &l_status);
    if (l_ret != MMC_ERR_NONE){
        sys_print("[%s:%d] send cmd12 failed", __func__, __LINE__);
    }


    l_ret = ett_mmc_send_status(host, card, &l_status);
    sys_print("l_ret = %d, l_status = 0x%x\n", l_ret, l_status);

    /* before read */
    sys_print("before read:\n");
    for (i = 0; i < 16; i++){
        sys_print("0x%x ", g_buf_r[i]);
    }
    sys_print("\n");


    /* send cmd18, not to try tuning */ 
    cmd.opcode  = MMC_CMD_READ_MULTIPLE_BLOCK;
    cmd.arg     = 0x200000;     // offset is 1GB
    cmd.rsptyp  = RESP_R1;
    cmd.retries = 0;
    cmd.timeout = CMD_TIMEOUT;

    /* send cmd first */
    ett_msdc_set_blknum(host, 8);
    ett_msdc_set_blklen(host, 512);
    ett_msdc_set_timeout(host, 100000000, 0);

    /* no need do error handle with r/w cmd failed */
    /* command will always pass after command tuning test */
    l_ret = mmc_cmd(host, &cmd);
    if (l_ret != MMC_ERR_NONE){
        sys_print("[%s:%d] send cmd failed", __func__, __LINE__);
        return l_ret;
    }

    if (cmd.opcode == MMC_CMD_READ_MULTIPLE_BLOCK){
        /* pio read */
        l_ret = ett_msdc_read_fifo((u32*)g_buf_r, 8 * 512, 0, 0, 0);

        if (l_ret != MMC_ERR_NONE)
            goto err;
    } 
   
    /* send stop cmd */
    l_ret = ett_mmc_send_stop(host, card, &l_status);
    if (l_ret != MMC_ERR_NONE){
        sys_print("[%s:%d] send cmd12 failed", __func__, __LINE__);
    }

    /* after read */
    sys_print("after read:\n");
    for (i = 0; i < 16; i++){
        sys_print("0x%x ", g_buf_r[i]);
    }
    sys_print("\n");

    for (i = 0; i < 8*512; i++){
        if (g_buf_r[i] != g_buf_w[i]){
            sys_print("[%d %s] r/w compara failed\n", __func__, __LINE__);
            break;
        }
    }
err:
    return 0;
}

#define PASS_COUNT 15
int check_array_result(u32 result, u32 show_log)
{
    u32 err = 0;       
    u32 start = 0; 
    u32 end = 0;  // we need ten 0.
    u32 bit = 0;

    // maybe result is 0 
    if (result == 0) {
        start = 0; end = 31;  
        err = 0;
        goto end;    
    }

find:      
    //printf("find \n");    
    start = end = 0;              
    while (bit < 32) { 
        if (result & (1 << bit)) { // failed
            bit++; continue;                         
        }
        start = end = bit; bit++;              
        //printf("<0x%x>start 0 is bit<%d>\n", result, start);        
        break;
    }

    while (bit < 32) {
        if (result & (1 << bit)){ // failed
            //printf("error bit<%d>\n", bit);            
            bit++; 
            if (end >= (start + PASS_COUNT)) {
                //MSDC_SET_FIELD(reg, field, (end + start)/2);      
                //printf("set <%d>\n", (end + start)/2);
                err = 0; 
                goto end;    
            } else {
                goto find;     
            }                               
        } else {
            end = bit; bit++;    
            //printf("end<%d>\n", end);
        }                
    }

end: 
    // let see the result                         
    //printf("start<%d>\t end<%d>\t value<0x%x>\n", start, end, result);  
    if (end >= (start + PASS_COUNT)) {
        if (show_log){
            sys_print("==PASS_COUNT<%d>== choose bit<%d> from<0x%x>\n",PASS_COUNT, (end + start)/2, result);
        }
        err = 0; 
    } else {
        if (show_log){
            sys_print("==PASS_COUNT<%d>== failed to find\n", PASS_COUNT);        
        } 
        err = 1; 
    } 
    return err;        
}

/* read test flow */

/* write test flow */

int msdc_test_init(unsigned int arg, unsigned int mode)
{
    unsigned int l_ret;

    l_ret = ett_mmc_init(arg, mode);
    if (l_ret != MMC_ERR_NONE){
         return l_ret;
    }

    return 0;
}

void ett_msdc_prepara(void)
{
    /* disable WDT */
    //MSDC_WRITE32(0x10007000, 0x22000000);

#if 0 //not work now 2014/1/19
    /* step1: open pll */
    MSDC_WRITE32(0x1020925C, 0x1);
    CTP_Wait_msec(1000);
    MSDC_WRITE32(0x1020925C, 0x2);
    
    MSDC_WRITE32(0x10209250, 0x1);
    CTP_Wait_msec(1000);


    /* step2: enable mux */
    MSDC_WRITE32(0x10000070, 0x02060301); 
    
    /* step3: enable clock */
    MSDC_WRITE32(0x10003010, 0x6000);      
#endif

    CTP_Wait_msec(2000);
}


#if 0 

/* apply off-line tune para */
/* *
 * @mode  0-16  emmc mode  0:ddr@highspeed 1:sdr@hs200 2:sdr@hs400
 *       17-32  sd mode   17:ddr@ddr50 18:sdr@sdr104 
 * */

typedef struct {
    /* MID + PNM in CID register */    
    u8   m_id;         // Manufacturer ID
    char pro_name[8];  // Product name

    u8 mode;    

    //clock relate
    u8 int_data_latch_ck_sel;
    u8 ckgen_msdc_dly_sel;

    // cmd pad relate
    u8 cmd_resp_ta_cntr;    //just for uhs104 mode
    u8 pad_cmd_resp_rxdly;
    u8 iocon_rspl;
    u8 pad_tune_cmdrrdly;

    // write pad relate
    u8 wrdat_crcs_ta_cntr;   //just for uhs104 mode
    u8 pad_tune_datwrdly;
    u8 iocon_d0spl;
    u8 dat_rddly0_d0;  

#ifndef ETT_SPILT_TUNE
    // read pad relate - case1
    u8 dspl;
    u8 pad_dat_rd_rxdly;
#else
    // read pad relate - case2
    u8 dspl_d0;
    u8 dspl_d1;
    u8 dspl_d2;
    u8 dspl_d3;
    u8 dspl_d4;
    u8 dspl_d5;
    u8 dspl_d6;
    u8 dspl_d7;
    //u8 dat_rddly0_d0;    //the same as write paras tune
    u8 dat_rddly0_d1;
    u8 dat_rddly0_d2;
    u8 dat_rddly0_d3;
    u8  dat_rddly1_d4;
    u8  dat_rddly1_d5;
    u8  dat_rddly1_d6;
    u8  dat_rddly1_d7;
#endif
}mmcdev_info,*pmmcdev_info;

#ifndef ETT_SPILT_TUNE
static const mmcdev_info g_mmcTable[] = {
    // hynix
    {0x90,    "HYNIX ",    0,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},

    {0x90,    "HYNIX ",    1,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},
    {0x90,    "HYNIX ",    2,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},

    // default 
    {0x00,    "xxxxxx",    0,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},

    {0x00,    "xxxxxx",    1,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},

    {0x00,    "xxxxxx",    2,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,
                               0,  0,  0,  0},
};
#else
static const mmcdev_info g_mmcTable[] = {
    // hynix
    {0x90,    "HYNIX ",    0,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},

    {0x90,    "HYNIX ",    1,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},

    {0x90,    "HYNIX ",    2,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},

    // default 
    {0x00,    "xxxxxx",    0,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},


    {0x00,    "xxxxxx",    1,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},

    {0x00,    "xxxxxx",    2,  0,  0,    
                               0,  0,  0,  0,
                               0,  0,  0,  0,  0,  0,  0,  0,
                               0,  0,  0,  0},
};
#endif

#endif
