/*
 * Copyright 2008,2010 Freescale Semiconductor, Inc
 * Andy Fleming
 *
 * Based (loosely) on the Linux code
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef _MMC_H_
#define _MMC_H_

#include <linux/list.h>

#define SD_VERSION_SD	0x20000
#define SD_VERSION_2	(SD_VERSION_SD | 0x20)
#define SD_VERSION_1_0	(SD_VERSION_SD | 0x10)
#define SD_VERSION_1_10	(SD_VERSION_SD | 0x1a)
#define MMC_VERSION_MMC		0x10000
#define MMC_VERSION_UNKNOWN	(MMC_VERSION_MMC)
#define MMC_VERSION_1_2		(MMC_VERSION_MMC | 0x12)
#define MMC_VERSION_1_4		(MMC_VERSION_MMC | 0x14)
#define MMC_VERSION_2_2		(MMC_VERSION_MMC | 0x22)
#define MMC_VERSION_3		(MMC_VERSION_MMC | 0x30)
#define MMC_VERSION_4		(MMC_VERSION_MMC | 0x40)

#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399) || \
	defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
#define MMC_MODE_HS         (1<<0)
#define MMC_MODE_HS_52MHz   (1<<1)
#define MMC_MODE_DDR_18V    (1<<2)
#define MMC_MODE_DDR_12V    (1<<3)
#define MMC_MODE_DDR_MASK   (MMC_MODE_DDR_18V | MMC_MODE_DDR_12V)
#define MMC_MODE_HS200_18V  (1<<4)
#define MMC_MODE_HS200_12V  (1<<5)
#define MMC_MODE_HS200_MASK (MMC_MODE_HS200_18V | MMC_MODE_HS200_12V)
#define MMC_MODE_RESERVED1  (1<<6)
#define MMC_MODE_RESERVED2  (1<<7)
#define MMC_MODE_4BIT		(1<<8)
#define MMC_MODE_8BIT		(1<<9)

#define SPEED_MODE_DS               (0x0)
#define SPEED_MODE_HS               (0x1)
#define SPEED_MODE_HS200            (0x2)
#define SPEED_MODE_SDR50            (0x2)
#define SPEED_MODE_SDR104           (0x3)
#define SPEED_MODE_DDR50            (0x4) 
#else
#define MMC_MODE_HS		0x001
#define MMC_MODE_HS_52MHz	0x010
#define MMC_MODE_4BIT		0x100
#define MMC_MODE_8BIT		0x200
#endif

#define SD_DATA_4BIT	0x00040000

#define IS_SD(x) (x->version & SD_VERSION_SD)

#define MMC_DATA_READ		1
#define MMC_DATA_WRITE		2

#define NO_CARD_ERR		-16 /* No SD/MMC card inserted */
#define UNUSABLE_ERR		-17 /* Unusable Card */
#define COMM_ERR		-18 /* Communications Error */
#define TIMEOUT			-19

#define MMC_CMD_GO_IDLE_STATE		0
#define MMC_CMD_SEND_OP_COND		1
#define MMC_CMD_ALL_SEND_CID		2
#define MMC_CMD_SET_RELATIVE_ADDR	3
#define MMC_CMD_SET_DSR			4
#define MMC_CMD_SWITCH			6
#define MMC_CMD_SELECT_CARD		7
#define MMC_CMD_SEND_EXT_CSD		8
#define MMC_CMD_SEND_CSD		9
#define MMC_CMD_SEND_CID		10
#define MMC_CMD_STOP_TRANSMISSION	12
#define MMC_CMD_SEND_STATUS		13
#define MMC_CMD_SET_BLOCKLEN		16
#define MMC_CMD_READ_SINGLE_BLOCK	17
#define MMC_CMD_READ_MULTIPLE_BLOCK	18
#define MMC_CMD_WRITE_SINGLE_BLOCK	24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK	25
#define MMC_CMD_PROGRAM_CSD	    27
#define MMC_CMD_SET_WRITE_PROT		28
#define MMC_CMD_CLR_WRITE_PROT		29
#define MMC_CMD_ERASE_START     35
#define MMC_CMD_ERASE_END       36
#define MMC_CMD_ERASE           38
#define MMC_CMD_APP_CMD			55

#define SD_CMD_SEND_RELATIVE_ADDR	3
#define SD_CMD_SWITCH_FUNC		6
#define SD_CMD_SEND_IF_COND		8

#define SD_CMD_APP_SET_BUS_WIDTH	6
#define SD_CMD_APP_SEND_OP_COND		41
#define SD_CMD_APP_SEND_SCR		51

/* SCR definitions in different words */
#define SD_HIGHSPEED_BUSY	0x00020000
#define SD_HIGHSPEED_SUPPORTED	0x00020000

#define MMC_HS_TIMING		  0x00000100
#define MMC_HS_26MHZ          (1<<0)
#define MMC_HS_52MHZ		  (1<<1)
#define MMC_DDR_52MHZ_18V     (1<<2)
#define MMC_DDR_52MHZ_12V     (1<<3)
#define MMC_HS200_18V         (1<<4)
#define MMC_HS200_12V         (1<<5)

#define OCR_BUSY	0x80000000
#define OCR_HCS		0x40000000

#define MMC_VDD_165_195		0x00000080	/* VDD voltage 1.65 - 1.95 */
#define MMC_VDD_20_21		0x00000100	/* VDD voltage 2.0 ~ 2.1 */
#define MMC_VDD_21_22		0x00000200	/* VDD voltage 2.1 ~ 2.2 */
#define MMC_VDD_22_23		0x00000400	/* VDD voltage 2.2 ~ 2.3 */
#define MMC_VDD_23_24		0x00000800	/* VDD voltage 2.3 ~ 2.4 */
#define MMC_VDD_24_25		0x00001000	/* VDD voltage 2.4 ~ 2.5 */
#define MMC_VDD_25_26		0x00002000	/* VDD voltage 2.5 ~ 2.6 */
#define MMC_VDD_26_27		0x00004000	/* VDD voltage 2.6 ~ 2.7 */
#define MMC_VDD_27_28		0x00008000	/* VDD voltage 2.7 ~ 2.8 */
#define MMC_VDD_28_29		0x00010000	/* VDD voltage 2.8 ~ 2.9 */
#define MMC_VDD_29_30		0x00020000	/* VDD voltage 2.9 ~ 3.0 */
#define MMC_VDD_30_31		0x00040000	/* VDD voltage 3.0 ~ 3.1 */
#define MMC_VDD_31_32		0x00080000	/* VDD voltage 3.1 ~ 3.2 */
#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */
#define MMC_VDD_34_35		0x00400000	/* VDD voltage 3.4 ~ 3.5 */
#define MMC_VDD_35_36		0x00800000	/* VDD voltage 3.5 ~ 3.6 */

#define MMC_SWITCH_MODE_CMD_SET		0x00 /* Change the command set */
#define MMC_SWITCH_MODE_SET_BITS	0x01 /* Set bits in EXT_CSD byte
						addressed by index which are
						1 in value field */
#define MMC_SWITCH_MODE_CLEAR_BITS	0x02 /* Clear bits in EXT_CSD byte
						addressed by index, which are
						1 in value field */
#define MMC_SWITCH_MODE_WRITE_BYTE	0x03 /* Set target byte to value */

#define SD_SWITCH_CHECK		0
#define SD_SWITCH_SWITCH	1

/*
 * EXT_CSD fields
 */

#define EXT_CSD_BUS_WIDTH	183	/* R/W */
#define EXT_CSD_HS_TIMING	185	/* R/W */
#define EXT_CSD_CARD_TYPE	196	/* RO */
#define EXT_CSD_REV		192	/* RO */
#define EXT_CSD_SEC_CNT		212	/* RO, 4 bytes */

/*
 * EXT_CSD field definitions
 */

#define EXT_CSD_CMD_SET_NORMAL		(1<<0)
#define EXT_CSD_CMD_SET_SECURE		(1<<1)
#define EXT_CSD_CMD_SET_CPSECURE	(1<<2)

#define EXT_CSD_CARD_TYPE_26	(1<<0)	/* Card can run at 26MHz */
#define EXT_CSD_CARD_TYPE_52	(1<<1)	/* Card can run at 52MHz */

#define EXT_CSD_BUS_WIDTH_1	     0	    /* Card is in 1 bit mode */
#define EXT_CSD_BUS_WIDTH_4	     1	    /* Card is in 4 bit mode */
#define EXT_CSD_BUS_WIDTH_8	     2	    /* Card is in 8 bit mode */
#define EXT_CSD_BUS_WIDTH_4_DDR  5       /* Card is in 4bit-ddr mode */
#define EXT_CSD_BUS_WIDTH_8_DDR  6       /* Card is in 8bit-ddr mode */

#define R1_ILLEGAL_COMMAND		(1 << 22)
#define R1_APP_CMD			(1 << 5)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136     (1 << 1)                /* 136 bit response */
#define MMC_RSP_CRC     (1 << 2)                /* expect valid crc */
#define MMC_RSP_BUSY    (1 << 3)                /* card may send busy */
#define MMC_RSP_OPCODE  (1 << 4)                /* response contains opcode */

#define MMC_RSP_NONE    (0)
#define MMC_RSP_R1      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
			MMC_RSP_BUSY)
#define MMC_RSP_R2      (MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3      (MMC_RSP_PRESENT)
#define MMC_RSP_R4      (MMC_RSP_PRESENT)
#define MMC_RSP_R5      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7      (MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)


struct mmc_cid {
	unsigned long psn;
	unsigned short oid;
	unsigned char mid;
	unsigned char prv;
	unsigned char mdt;
	char pnm[7];
};

/*
 * WARNING!
 *
 * This structure is used by atmel_mci.c only.
 * It works for the AVR32 architecture but NOT
 * for ARM/AT91 architectures.
 * Its use is highly depreciated.
 * After the atmel_mci.c driver for AVR32 has
 * been replaced this structure will be removed.
 */
struct mmc_csd
{
	u8	csd_structure:2,
		spec_vers:4,
		rsvd1:2;
	u8	taac;
	u8	nsac;
	u8	tran_speed;
	u16	ccc:12,
		read_bl_len:4;
	u64	read_bl_partial:1,
		write_blk_misalign:1,
		read_blk_misalign:1,
		dsr_imp:1,
		rsvd2:2,
		c_size:12,
		vdd_r_curr_min:3,
		vdd_r_curr_max:3,
		vdd_w_curr_min:3,
		vdd_w_curr_max:3,
		c_size_mult:3,
		sector_size:5,
		erase_grp_size:5,
		wp_grp_size:5,
		wp_grp_enable:1,
		default_ecc:2,
		r2w_factor:3,
		write_bl_len:4,
		write_bl_partial:1,
		rsvd3:5;
	u8	file_format_grp:1,
		copy:1,
		perm_write_protect:1,
		tmp_write_protect:1,
		file_format:2,
		ecc:2;
	u8	crc:7;
	u8	one:1;
};

struct mmc_cmd {
	ushort cmdidx;
	uint resp_type;
	uint cmdarg;
	uint response[4];
	uint flags;
};

struct mmc_data {
	union {
		char *dest;
		const char *src; /* src buffers don't get written to */
	};
	uint flags;
	uint blocks;
	uint blocksize;
};

struct mmc {
	struct list_head link;
	char name[32];
	void *priv;
	uint voltages;
	uint version;
	uint f_min;
	uint f_max;
	int high_capacity;
	uint bus_width;
	uint clock;
	uint card_caps;
	uint host_caps;
	uint ocr;
	uint scr[2];
	uint csd[4];
	uint cid[4];
	ushort rca;
	uint tran_speed;
	uint read_bl_len;
	uint write_bl_len;
	u64 capacity;
	block_dev_desc_t block_dev;
	int (*send_cmd)(struct mmc *mmc,
			struct mmc_cmd *cmd, struct mmc_data *data);
	void (*set_ios)(struct mmc *mmc);
	int (*init)(struct mmc *mmc);
#ifdef CONFIG_MMC_MBLOCK
	uint b_max;
#endif
};

struct sd_mmc_ext_csd
{
	unsigned char       buffer[512];
	unsigned char       s_cmd_set;             // 1bit 504
	unsigned char       power_off_long_time;        // 1byte 247
	unsigned char       ini_timeout_ap;        // 1byte 241
	unsigned char       pwr_cl_ddr_52_360;     // 1byte 239
	unsigned char       pwr_cl_ddr_52_195;     // 1byte 238
	unsigned char       min_perf_ddr_w8_52;    // 1byte 235
	unsigned char       min_perf_ddr_r8_52;    // 1byte 234
	unsigned char       trim_mult;    		   // 1byte 232
	unsigned char       sec_support;   		   // 1byte 231
	unsigned char       sec_erase_mult;    	   // 1byte 230
	unsigned char       sec_trim_mult;    	   // 1byte 229
	unsigned char       boot_info;    		   // 1byte 228
	unsigned char 		boot_size_mult;		   //1byte 226
	unsigned char 		access_size;		   //1byte 225
	unsigned char 		hc_erase_grp_size;	   //1byte 224
	unsigned char 		erase_timeout_multi;   //1byte 223
	unsigned char 		rel_wr_sec_cnt;   	   //1byte 222
	unsigned char 		hc_wp_grp_size;        //1byte 221
	unsigned char 		sleep_cur_vcc;   	   //1byte 220
	unsigned char 		sleep_cur_vccq;   	   //1byte 219
	unsigned char 		slp_awk_timeout;   	   //1byte 217
    unsigned int		sec_count;             // 4bit 215-212
	unsigned char       min_pref_w_8_52;       // 1bit 210
	unsigned char       min_pref_r_8_52;       // 1bit 209
	unsigned char       min_pref_w_8_26_4_52;  // 1bit 208
	unsigned char       min_pref_r_8_26_4_52;  // 1bit 207
	unsigned char       min_pref_w_4_26;       // 1bit 206
	unsigned char       min_pref_r_4_26;       // 1bit 205
	unsigned char       pwr_cl_26_360;         // 1bit 203
	unsigned char       pwr_cl_52_360;         // 1bit 202
	unsigned char       pwr_cl_26_195;         // 1bit 201
	unsigned char       pwr_cl_52_195;         // 1bit 200
	unsigned char       card_type;             // 1bit 196
	unsigned char       csd_structure;         // 1bit 194
	unsigned char       ext_csd_rev;           // 1bit 192

	//Modes segment

	unsigned char       cmd_Set;               //1bit 191  R/W
	unsigned char       cmd_set_rev;           //bit1 189
	unsigned char       power_class;           //bit1 187
	unsigned char       hs_timing;             //bit1 185
	unsigned char       bus_width;             //bit1 183
	unsigned char       erased_mem_cont;       //bit1 181
	unsigned char       boot_config;       	   //bit1 179
	unsigned char       boot_config_prot;  	   //1byte 178
	unsigned char       boot_bus_width;        //bit1 177
	unsigned char       erase_grp_defn;        //bit1 175
	unsigned char       boot_wp;        //1byte 173
	unsigned char       user_wp;        //1byte 171
	unsigned char       fw_config;      //1byte 169
	unsigned char       rpmb_size_mult; //1byte 168
	unsigned char       rst_n_func;     //1byte 162
	unsigned char       part_support;   //1byte 160
	unsigned int        max_enh_size_mult;  //3bytes 157
	unsigned char       part_attrb;  //1byte 156
	unsigned char       part_set_complete;  //1byte 155
	unsigned int       GP_size_mult0;  //3byte 143
	unsigned int       GP_size_mult1;  //3byte 146
	unsigned int       GP_size_mult2;  //3byte 149
	unsigned int       GP_size_mult3;  //3byte 152
	unsigned int       enh_size_mult;  //3byte 140
	unsigned int       enh_start_addr;  //4byte 136
	unsigned char      sec_bad_blk_mngt;  //1byte 134
	unsigned char       power_off_noti;        // 1byte 34
}__attribute__((packed));
static struct sd_mmc_ext_csd ext_csd;

struct sd_mmc_csd_v4
{
    unsigned char       csd_struct         :2;         //127-126 bits=4
    unsigned int        specs_ver          :4;         //125-122 bits=4
    unsigned int        reserve3           :2;         //121-120 bits=2
    //121-120 reserved
    unsigned char       taac;                          //119-112 bits=8
    unsigned char       nsac;                          //111-104 bits=8
    unsigned char       tran_speed;                    //103-96  bits-8
    unsigned short      ccc;                           //95-84 bits=12
    unsigned char       read_bl_len;                   //83-80 bits=4
    unsigned int        read_partial       :1,         //79-79 bits=1
                        write_misalign     :1,         //78-78 bits=1
                        read_misalign      :1;         //77-77 bits=1
    unsigned int        dsr_imp            :1;         //75-75 bits=1
    unsigned int        reserve2           :2;         //75-74 bits=2
    //75-74 bits=3  reserved
    unsigned int        c_size             :12;         //73-62 bits=12

    unsigned int        vdd_r_curr_min     :3;         //61-59 bits=3
    unsigned int        vdd_r_curr_max     :3;         //58-56 bits=3
    unsigned int        vdd_w_curr_min     :3;         //55-53 bits=3
    unsigned int        vdd_w_curr_max     :3;         //52-50 bits=3
    //50-61 bits=12 reserved previously
    unsigned            c_size_mult        :3;         //49-47 bits=3

    unsigned int        erase_grp_size     :5;         //46-42 bits=5
    unsigned int        erase_grp_mult     :5;         //41-32 bits=5
    unsigned int        wp_grp_size        :5;         //36-32 bits=5
    unsigned int        wp_grp_enable      :1;         //31-31 bits=2
    unsigned int        default_ecc        :2;         //30-29 bits=2
    //29-46 bits=18 reserved prevoiously
    unsigned int        r2w_factor         :3;         //28-26 bits=3
    unsigned int        write_bl_len       :4;         //25-22 bits=4
    unsigned            write_partial      :1;         //21-21 bits=1

    unsigned int        reserve1           :4;         //20-17 reserved
    unsigned int        content_prot_app   :1,         //16-16 bits=1
                        file_format_grp    :1,         //15-15 bits=1
                        copy               :1,         //14-14 bits=1
                        perm_write_protect :1,         //13-13 bits=1
                        tmp_write_prot     :1;         //12-12 bits=1
    unsigned int        file_format        :2;         //11-10 bits=2
    unsigned int        ecc                :2;         //09-08 bits=2
};
static struct sd_mmc_csd_v4 mmc_csd_v4;




extern struct mmc emmc_info[];

int mmc_register(struct mmc *mmc);
int mmc_initialize(bd_t *bis);
int mmc_init(struct mmc *mmc);
int mmc_init_fast(struct mmc *mmc);
int mmc_read(struct mmc *mmc, u64 src, uchar *dst, int size);
int mmc_erase(struct mmc *mmc, u64 bytestart, u64 length, uint mode);
int mmc_erase_all(struct mmc *mmc);
int mmc_wp(struct mmc *mmc, ulong bytestart, uint level, uint type, uint fgEn);
void mmc_set_clock(struct mmc *mmc, uint clock);
struct mmc *find_mmc_device(int dev_num);
int mmc_set_dev(int dev_num);
void print_mmc_devices(char separator);
int board_mmc_getcd(u8 *cd, struct mmc *mmc);

// Async read API.
int emmc_async_read_setup(int dev_num, u64 bytestart, ulong bytecnt, void *dst);
int emmc_async_dma_start_trigger(uint length);
int emmc_async_dma_wait_finish(void);
int emmc_async_read_finish(int dev_num);
int emmc_async_read_stop(int dev_num, int i4fg);

#ifdef CC_EMMC_BOOT
int atmel_mci_init(void *regs);
#else
int mmc_legacy_init(int verbose);
#endif

#endif /* _MMC_H_ */
