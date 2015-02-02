/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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

/*
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <stdio_dev.h>
#include <version.h>
#include <net.h>
#include <serial.h>
#include <nand.h>
#include <onenand_uboot.h>
#include <mmc.h>
#include <libfdt.h>
#ifdef CFG_LG_CHG
#include <mt5882_lgcmnio.h>
#include <lg_modeldef.h>
#include <cmd_splash.h>
#include <partinfo.h>
#include <cmd_polltimer.h>
#include <cmd_resume.h>
#endif
#include <fdtdec.h>
#include <post.h>
#include <logbuff.h>

#ifdef CONFIG_BITBANGMII
#include <miiphy.h>
#endif

#ifdef CONFIG_DRIVER_SMC91111
#include "../drivers/net/smc91111.h"
#endif
#ifdef CONFIG_DRIVER_LAN91C96
#include "../drivers/net/lan91c96.h"
#endif

#if defined(CC_MT5882)
#include "drvcust_if.h"
#endif
#include "x_dram.h"

DECLARE_GLOBAL_DATA_PTR;
#define CONFIG_KERNEL_START_ADDRESS 0x20200000
    #define SMP_RIU_BASE          0x1f000000
    #define SMP_DUMMY_BANK             0xe9e
    #define SMP_DUMMY_MAGIC           0x8000
    #define SMP_DUMMY_PA_START        0x8004
    #define SMP_DUMMY_SP              0x8008
    #define SMP_DUMMY_TTB0            0x800c
    #define SMP_DBG_CACHE_S           0xf001
    #define SMP_DBG_CACHE_E           0xf002
    #define SMP_DBG_RAM_INLOOP        0xf003
    #define SMP_DBG_GET_MAGIC_NUM     0xf004
    #define SMP_DBG_IN_SECOND_INIT    0xf005
    #define SMP_DBG_IN_STACK_READY    0xf006
    #define SMP_DBG_IN_MMU_ON         0xf007
    #define SMP_DBG_IN_C_CODE         0xf008

    #define MAGIC_NUMBER    (0x0000babe)
    #define BOOT_CPU        (0)
ulong monitor_flash_len;

#ifdef CONFIG_HAS_DATAFLASH
extern int  AT91F_DataflashInit(void);
extern void dataflash_print_info(void);
#endif
volatile int LogEnable = 1;

#if defined(CONFIG_HARD_I2C) || \
    defined(CONFIG_SOFT_I2C)
#include <i2c.h>
#endif
#ifdef CFG_LG_CHG
void BootSplash(void);
static void verify_mac_address(void);
void del_timer_all(void);
#endif
/* Fix DECLARE_GLOBAL_DATA_PTR error for the V6 gcc toolchain */
gd_t *gd;
extern int dual_bank_init (void);

/************************************************************************
 * Coloured LED functionality
 ************************************************************************
 * May be supplied by boards if desired
 */
inline void __coloured_LED_init(void) {}
void coloured_LED_init(void)
	__attribute__((weak, alias("__coloured_LED_init")));
inline void __red_led_on(void) {}
void red_led_on(void) __attribute__((weak, alias("__red_led_on")));
inline void __red_led_off(void) {}
void red_led_off(void) __attribute__((weak, alias("__red_led_off")));
inline void __green_led_on(void) {}
void green_led_on(void) __attribute__((weak, alias("__green_led_on")));
inline void __green_led_off(void) {}
void green_led_off(void) __attribute__((weak, alias("__green_led_off")));
inline void __yellow_led_on(void) {}
void yellow_led_on(void) __attribute__((weak, alias("__yellow_led_on")));
inline void __yellow_led_off(void) {}
void yellow_led_off(void) __attribute__((weak, alias("__yellow_led_off")));
inline void __blue_led_on(void) {}
void blue_led_on(void) __attribute__((weak, alias("__blue_led_on")));
inline void __blue_led_off(void) {}
void blue_led_off(void) __attribute__((weak, alias("__blue_led_off")));

//#define mb()		do { dsb(); outer_sync(); } while (0)


/*
 ************************************************************************
 * Init Utilities							*
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

#if defined(CONFIG_ARM_DCC) && !defined(CONFIG_BAUDRATE)
#define CONFIG_BAUDRATE 115200
#endif

static int init_baudrate(void)
{
	gd->baudrate = getenv_ulong("baudrate", 10, CONFIG_BAUDRATE);
	return 0;
}
extern  void GIC_Init(void);
extern int cpu_init (void);
static int display_banner(void)
{
	printf("\n\n%s\n\n", version_string);
	debug("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _TEXT_BASE,
	       _bss_start_ofs + _TEXT_BASE, _bss_end_ofs + _TEXT_BASE);
#ifdef CONFIG_MODEM_SUPPORT
	debug("Modem Support enabled\n");
#endif
#ifdef CONFIG_USE_IRQ
    cpu_init();
	GIC_Init();
	printf("IRQ Stack: %08lx\n", IRQ_STACK_START);
	printf("FIQ Stack: %08lx\n", FIQ_STACK_START);
#endif

	return (0);
}

/*
 * WARNING: this code looks "cleaner" than the PowerPC version, but
 * has the disadvantage that you either get nothing, or everything.
 * On PowerPC, you might see "DRAM: " before the system hangs - which
 * gives a simple yet clear indication which part of the
 * initialization if failing.
 */
static int display_dram_config(void)
{
	int i;

#ifdef DEBUG
	puts("RAM Configuration:\n");

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
		printf("Bank #%d: %08lx ", i, gd->bd->bi_dram[i].start);
		print_size(gd->bd->bi_dram[i].size, "\n");
	}
#else
	ulong size = 0;

	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		size += gd->bd->bi_dram[i].size;

	puts("DRAM:  ");
	print_size(size, "\n");
#endif

	return (0);
}

#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
static int init_func_i2c(void)
{
	puts("I2C:   ");
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	puts("ready\n");
	return (0);
}
#endif

#if defined(CONFIG_CMD_PCI) || defined (CONFIG_PCI)
#include <pci.h>
static int arm_pci_init(void)
{
	pci_init();
	return 0;
}
#endif /* CONFIG_CMD_PCI || CONFIG_PCI */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

int print_cpuinfo(void);
#include "x_bim.h"
void __dram_init_banksize(void)
{
	#if 0//defined(CC_DYNAMIC_FBMSRM_CONF)
    //UINT32 bSonyModelType;
	//if(0==DRVCUST_InitQuery(eGet2KModelSupport,&bSonyModelType))
	{
		if(BIM_READ32(0xe0)!=0)//if(bSonyModelType==0) 	
			{
			gd->bd->bi_dram[0].size =  (TOTAL_MEM_SIZE - FBM_MEM_CFG_MT5890_3DTV_4K- DIRECT_FB_MEM_SIZE - FB_MEM_SIZE);		
		    printf("gd->bd->bi_dram[0].size 0x%x \n",gd->bd->bi_dram[0].size);
			#if defined(PHYS_SDRAM_2) && defined(PHYS_SDRAM_2_SIZE)
			gd->bd->bi_dram[1].start = (DEFAULT_CHANNEL_A_SIZE );
			gd->bd->bi_dram[1].size	 = (DEFAULT_CHANNEL_B_SIZE - FBM_MEM_CFG_MT5890_3DTV_4K_CHB-TRUSTZONE_MEM_SIZE);
			printf("gd->bd->bi_dram[1].start 0x%x \n",gd->bd->bi_dram[1].start);
			printf("gd->bd->bi_dram[1].size 0x%x \n",gd->bd->bi_dram[1].size);
			#endif
			}
		else
			{
			gd->bd->bi_dram[0].size =  (TOTAL_MEM_SIZE - FBM_MEM_CFG_MT5890_3DTV_FHD - DIRECT_FB_MEM_SIZE - FB_MEM_SIZE);
			printf("gd->bd->bi_dram[0].size 0x%x \n",gd->bd->bi_dram[0].size);
			#if defined(PHYS_SDRAM_2) && defined(PHYS_SDRAM_2_SIZE)
			gd->bd->bi_dram[1].start = (DEFAULT_CHANNEL_A_SIZE );
			gd->bd->bi_dram[1].size	 = (DEFAULT_CHANNEL_B_SIZE - FBM_MEM_CFG_MT5890_3DTV_FHD_CHB -TRUSTZONE_MEM_SIZE);
			printf("gd->bd->bi_dram[1].start 0x%x \n",gd->bd->bi_dram[1].start);
			printf("gd->bd->bi_dram[1].size 0x%x \n",gd->bd->bi_dram[1].size);
		    #endif
			}
	}
	#else
	gd->bd->bi_dram[0].size =  gd->ram_size;

	#if defined(PHYS_SDRAM_2) && defined(PHYS_SDRAM_2_SIZE)
	gd->bd->bi_dram[1].start = PHYS_SDRAM_2;
	gd->bd->bi_dram[1].size	 = PHYS_SDRAM_2_SIZE;
	//printf("gd->bd->bi_dram[1].start 0x%x \n",PHYS_SDRAM_2);
	//printf("gd->bd->bi_dram[1].size 0x%x \n",PHYS_SDRAM_2_SIZE);
	#endif
	#endif
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
}
void dram_init_banksize(void)
	__attribute__((weak, alias("__dram_init_banksize")));

init_fnc_t *init_sequence[] = {
#if defined(CONFIG_ARCH_CPU_INIT)
	arch_cpu_init,		/* basic arch cpu dependent setup */
#endif
#if defined(CONFIG_BOARD_EARLY_INIT_F)
	board_early_init_f,
#endif
#ifdef CONFIG_OF_CONTROL
	fdtdec_check_fdt,
#endif
	//timer_init,		/* initialize timer */
#ifdef CONFIG_FSL_ESDHC
	get_clocks,
#endif
	env_init,		/* initialize environment */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
#ifndef CFG_LG_CHG
	console_init_f,		/* stage 1 init of console */
#endif
	display_banner,		/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
#if defined(CONFIG_DISPLAY_BOARDINFO)
	checkboard,		/* display board info */
#endif
#if defined(CONFIG_HARD_I2C) || defined(CONFIG_SOFT_I2C)
	init_func_i2c,
#endif
	dram_init,		/* configure available RAM banks */
	NULL,
};

#ifdef CFG_LG_CHG
extern unsigned int print_partinfo(void);

init_fnc_t *init_sequence_post[] = {
	console_init_f, 	/* stage 1 init of console */
	display_banner, 	/* say that we are here */
#if defined(CONFIG_DISPLAY_CPUINFO)
	print_cpuinfo,		/* display cpu info (and speed) */
#endif
//	print_partinfo,
	NULL,
};

extern int verify_apps(int boot_mode);
#define IS_SNAPSHOTBOOT		( (check_snapshot_mode() == LGSNAP_RESUME)? BOOT_SNAPSHOT : BOOT_COLD  )

#ifdef SIGN_USE_PARTIAL
//#define BOOTCOMMAND "cp2ramz kernel 0x7000000 0x7FC0;verification 0 kernel 0x7000000;xipz lgapp;bootm 0x7FC0"
#define BOOTCOMMAND "cp2ramz lz4 kernel 0x7000000 0x7FC0;verification 0 kernel 0x7000000;xiplz4 lgapp;bootm 0x7FC0"
#else
#define BOOTCOMMAND "cp2ram tzfw 0x200000;verification 0 tzfw 0x200000;cp2ram lginit 0x100000;verification 0 lginit 0x100000;cp2ramz lz4 kernel 0x8000000 0x7FC0;verification 0 kernel 0x8000000;xiplz4 lgapp;bootm 0x7FC0"
#endif


 int fast_boot(void)
{
	char *s = NULL, key = 0;
	int i;
	char *cmd1 = BOOTCOMMAND;
	char *cmd2[1024];

//#if defined(CONFIG_MULTICORES_PLATFORM)
		second_main();
//#endif

	printf("[%4d] fast boot check \n", readMsTicks());
	/* delay 10 * 1us */

#if 0 // temporary disable	
#ifndef UNSECURE
	if ((DDI_NVM_GetDebugStatus() != RELEASE_LEVEL))
#endif
#endif
	{
		for (i=0; i<20; ++i) {
			if (tstc()) {	/* we got a key press	*/
				key = getc();
				printf("key in %02x\n", key);
				//if (key == 0x0d || key == 0x60)
				if (key == 0x60)
					break;
			}
			udelay (10);
		}
	}
	//for test
	//
	//display_banner();
	//add_timer(450,1,printf,"test");

	if(key != 0x60) {
	//if(key != 0x0d && key != 0x60) {
		BootSplash();

#ifdef SIGN_USE_PARTIAL
		verify_apps(IS_SNAPSHOTBOOT);
//		BOOTLOG("verify_apps();\n");
#endif

#ifdef	CONFIG_HIBERNATION
		if(check_snapshot_mode() == LGSNAP_RESUME)
		{	
			disable_wp(0); // eMMC W/P disable before snapshot booting
			if(do_hib() == -1)
			{
				printk("boot state is snapshot, but snapshot image is not valid\n");
				printk("check the snapshot image\n");

				if(DDI_NVM_GetDebugStatus() == RELEASE_LEVEL)
					remake_hib();
			}

#ifdef SIGN_USE_PARTIAL
			verify_apps(BOOT_COLD);
//			BOOTLOG("verify_apps();\n");
#endif			
		}
#endif

		printf("bootcmd cmd = %s \n",cmd1);
		run_command(cmd1, 0);
	}
	
	disable_wp(0); // eMMC W/P disable for user debugging in boot prompt
	
	return 0;
}
#endif

unsigned int kmemsize = 0;
unsigned int chb_kmemstart = 0;
unsigned int chb_kmemsize = 0;
unsigned int tzsize = 0;
#define FLASH_BASE 0
#define FLASH_TZFW_TOTAL_LEN 0x200000
#define FLASH_KERMEM_TOTAL_LEN 0x20000
#define FLASH_TZBIN_TOTAL_LEN (FLASH_TZFW_TOTAL_LEN-FLASH_KERMEM_TOTAL_LEN)

ulong get_tzfw_base(void)
{
        ulong src;
        struct partition_info *mpi = NULL;

        /* find partition */
        mpi = get_used_partition("tzfw");
        if(!mpi) {
            printf("Error: Cannot find tzfw partition\n");
            //while (1);
	    return 0;
        }

        src = FLASH_BASE + mpi->offset;
        printf("(yjdbg) find tzfw: pos: %lx\n", src);

	return src;
}

void get_kernel_size(ulong src)
{
#if 1  // hongjun changed mem get method ,  not from tz.bin 

		kmemsize = TCMGET_CHANNELA_SIZE() * 0x100000 - PHYS_SDRAM_1_FBM_DFB_FB_TRUSTZONE_SIZE; 
		chb_kmemsize = DIRECT_FB_MEM_SIZE + FB_MEM_SIZE;
		
		tzsize  = TRUSTZONE_MEM_SIZE;
		
		 if (TCMGET_CHANNELA_SIZE() == 0x300) /* 768M case */
		 {
			  kmemsize += 47 * 0x100000;	 /* increase kmemsize by 47MB */
			  chb_kmemsize -= 47 * 0x100000; /* decrease FBDFB size by 47MB */
		 }
		 
		chb_kmemstart = TCMGET_CHANNELA_SIZE() * 0x100000 - FBM_MEM_CFG_SIZE - tzsize;

		printf("[uboot debug] kernel memory size=0x%x\n", kmemsize);
		printf("[uboot debug] FBM dram start addr=0x%x,  size=0x%x \n", chb_kmemstart, FBM_MEM_CFG_SIZE);
		printf("[uboot debug] FB/DFB dram size=0x%x\n", chb_kmemsize);
		printf("[uboot debug] TZ dram size=0x%x\n", tzsize);

#if 0
	printf(" 0x%x, 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x \n" ,FBM_SECURE_FEEDER_2K_SIZE , FBM_FEEDER_TVP_SIZE , FBM_DMX1_SIZE , FBM_SCPOS_MAIN_SIZE , FBM_MPEG_Y_SIZE  ,FBM_PQ_TOOL_POOL_SIZE , 
		FBM_HW_DEMOD_SIZE ,FBM_OD_SIZE , FBM_JPEVDP_POOL_SIZE , \
		FBM_DMX_PID_BUF_SIZE , FBM_VENC_SIZE  , FBM_TWO_FBP_SIZE , FBM_FD_SIZE , FBM_PVR_SIZE , FBM_SWDMX_SIZE , FBM_VSS_SIZE , FBM_DRM_BUF_SIZE , \
		FBM_MUXER_SIZE ,FBM_IMGRZ_3D_KR_BUF , FBM_SLT_EXTRA_SIZE , FBM_PNG_POOL_SIZE , FBM_TDCAV_POOL_SIZE  , FBM_DMX_ADD_SIZE , \
		FBM_EXTRA_FOR_4K2K_FEEDER_DMX ,\
		FBM_DSP_BIN_POOL_SIZE , FBM_TVE_POOL_FBM_SIZE  , FBM_JPEG_SIZE, FBM_DSP_POOL_SIZE , FBM_REDUCE_FBM_DMX_SIZE);
#endif

#else
	int i;

        uint8_t kermembuf[FLASH_KERMEM_TOTAL_LEN];
        int rc;
        unsigned int* penv;

        rc = storage_read(src, FLASH_KERMEM_TOTAL_LEN, kermembuf);
        #if 0
        for (i = 0; i < 0x200; i++)
        {
            printf("%02x ", kermembuf[i]);
            if ((i%16)==15)
            {
                printf("\n");
            }
        }
        #endif

        penv = (unsigned int*)kermembuf;


        //get ATAG kernel memory from env.o
        if (penv[0]==0x464c457f)
        {
            int i=1;
            for (; i<0x40; i++)
            {
                // find magic key, the next work is global_kernel_dram_size
                if (penv[i] == 0x53965368)
                {
#ifdef CC_SUPPORT_MULTI_DRAM_COMBINATION    
// LG proto-PCBA, only 1 DRAM config
#if 0
                    if(PCB_DRAM_SIZE_QUERY() == DEFAULT_LCD_1280MB_DRAM)
#endif
                    {
                        kmemsize = penv[i+1];//DEFAULT_LINUX_KERNEL_MEM_SIZE
                        chb_kmemstart = penv[i+2];//DEFAULT_CHANNEL_A_SIZE, 1G
                        chb_kmemsize = penv[i+3];//(DEFAULT_CHANNEL_B_SIZE-0x7800000), 256M - 0x7800000
						fbdfbsize = penv[i+4];
                    }
// LG proto-PCBA, only 1 DRAM config
#if 0
                    else if(PCB_DRAM_SIZE_QUERY() == PDP_1536MB_DRAM)
                    {
                        kmemsize = penv[i+1];//DEFAULT_LINUX_KERNEL_MEM_SIZE
                        chb_kmemstart = penv[i+2];//DEFAULT_CHANNEL_A_SIZE, 1G
                        chb_kmemsize = penv[i+3] + 256 * 1024 *1024;//compare to 1.2G PCB, PDP CHB should be added 256M
						fbdfbsize = penv[i+4];
                    }
			else if(PCB_DRAM_SIZE_QUERY() == LCD_768MB_DRAM)
                    {
                     	kmemsize = penv[i+1]- 512 * 1024 *1024;//DEFAULT_LINUX_KERNEL_MEM_SIZE
                     	chb_kmemstart = 512 * 1024 *1024;//DEFAULT_CHANNEL_A_SIZE, 1G
                     	chb_kmemsize = penv[i+3] + 0x5000000;//(DEFAULT_CHANNEL_B_SIZE - 0x2800000), 256M - 0x2800000
				fbdfbsize = penv[i+4];
                    }
                    else//756 MB
                    {
                        kmemsize = penv[i+1] - 512 * 1024 *1024;//compare to 1.2G PCB, kernel menory on CHA should be subtracted 512M
                        chb_kmemstart = 512 * 1024 *1024;//channel b start address (chnnel a size)
                        chb_kmemsize = penv[i+3];//the same as DEFAULT_LCD_1280MB_DRAM
                        fbdfbsize = penv[i+4];
                    }
#endif
#else
                     kmemsize = TCMGET_CHANNELA_SIZE() * 0x100000 - penv[i+1];
				     chb_kmemstart = penv[i+2];
				     chb_kmemsize = penv[i+3];
				     fbdfbsize = penv[i+4]; 

				     if (TCMGET_CHANNELA_SIZE() == 0x300) /* 768M case */
				     {
					      kmemsize += 47 * 0x100000;     /* increase kmemsize by 47MB */
					      chb_kmemsize -= 47 * 0x100000; /* decrease FBDFB size by 47MB */
				     }

#endif                    
					printf("uboot memory debug %d, 0x%x 0x%x 0x%x 0x%x\n", i, penv[i+1], penv[i+2], penv[i+3], penv[i+4]);
					printf("kernel dram1 size=0x%x\n", kmemsize);
					printf("FBM dram size =0x%x\n", chb_kmemstart);
					printf("FBDFB dram size=0x%x\n", chb_kmemsize);
					printf("TZ dram size=0x%x\n", fbdfbsize);
                    break;
                }
            }
        }

        //kmemsize = 0xc700000;
	if (kmemsize == 0)
	{
	    printf("Error: Please update tzfw partition\n");
	    //while (1);
	}
	#endif
}

#ifdef CC_TRUSTZONE_SUPPORT
#include <c_model.h>
#include "x_bim.h"
#include "x_dram.h"
void move_tz_binary(ulong src)
{
        uint8_t *TzDram;
        int rc;

        //printf("(yjdbg) tz memory base: 0x%lx\n", kmemsize+DRVCUST_InitGet(eTzCodeBase));
//        TzDram = (uint8_t *)(kmemsize+DRVCUST_InitGet(eTzCodeBase));
#if defined(CC_TRUSTZONE_IN_CHB)
        TzDram = (uint8_t *)(TCMGET_CHANNELA_SIZE()*0x100000+TCMGET_CHANNELB_SIZE()*0x100000-TRUSTZONE_MEM_SIZE+DRVCUST_InitGet(eTzCodeBase));
#else
        TzDram = (uint8_t *)(TOTAL_MEM_SIZE - TRUSTZONE_MEM_SIZE /*kmemsize*/+DRVCUST_InitGet(eTzCodeBase));
#endif
        printf("(yjdbg) tz memory base: 0x%x\n", (unsigned int)TzDram);
        rc = storage_read(src+FLASH_KERMEM_TOTAL_LEN, FLASH_TZBIN_TOTAL_LEN, TzDram);

        #if 0
        for (i = 0; i < 0x200; i++)
        {
            printf("%02x ", TzDram[i]);
            if ((i%16)==15)
            {
                printf("\n");
            }
        }
        #endif
}
#endif

#ifdef CC_TRUSTZONE_SUPPORT
void __attribute__((__noinline__)) wrapper_theKernel(unsigned int addr, int zero, int arch, uint params, uint kernel_entry)
{
	void	(*theKernel)(int zero, int arch, uint params, uint kernel_entry);
	kernel_entry = __builtin_return_address(0);
	theKernel = addr;
	printf("[%4d] Jump to kernel...ep=0x%08lx, machid=0x%x, bi=0x%lx\n", readMsTicks(), kernel_entry, arch, params);
	theKernel (zero, arch, params, kernel_entry);
}

extern asmlinkage int trustzone_arch_suspend(void);
extern asmlinkage int trustzone_arch_resume(void);
unsigned int tz_init_once=0;
void init_tz(void)
{
	void	(*theKernel)(int zero, int arch, uint params, uint kernel_entry);

	cleanup_before_linux();

//	theKernel = (void (*)(int, int, uint, uint))(kmemsize + DRVCUST_InitGet(eTzCodeBase));
#if defined(CC_TRUSTZONE_IN_CHB)
        theKernel = (void (*)(int, int, uint, uint))(TCMGET_CHANNELA_SIZE()*0x100000+TCMGET_CHANNELB_SIZE()*0x100000-TRUSTZONE_MEM_SIZE+DRVCUST_InitGet(eTzCodeBase));
#else
        theKernel = (void (*)(int, int, uint, uint))(TOTAL_MEM_SIZE - TRUSTZONE_MEM_SIZE /*kmemsize*/+DRVCUST_InitGet(eTzCodeBase));
#endif
	printf("before trustzone_arch_suspend\n");
	trustzone_arch_suspend();
	printf("after trustzone_arch_suspend\n");
	if (tz_init_once == 0)
	{
	    tz_init_once = 1;
	    wrapper_theKernel(theKernel, 0, 0, 0, 0); // ignore all parameters
	    trustzone_arch_resume();
	}
#if (CONFIG_ENABLE_MMU)
	//HalInitMMU_TZ(0x1F00000);
#endif
	printf("Return back to uboot\n");
}

#endif
void board_init_f(ulong bootflag)
{
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	gd_t *id;
	ulong addr, addr_sp;
#ifdef CONFIG_PRAM
	ulong reg;
#endif

	/* Pointer is writable since we allocated a register for it */
	gd = (gd_t *) ((CONFIG_SYS_INIT_SP_ADDR) & ~0x07);
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");

	memset((void *)gd, 0, sizeof(gd_t));

	gd->mon_len = _bss_end_ofs;
#ifdef CONFIG_OF_EMBED
	/* Get a pointer to the FDT */
	gd->fdt_blob = _binary_dt_dtb_start;
#elif defined CONFIG_OF_SEPARATE
	/* FDT is at end of image */
	gd->fdt_blob = (void *)(_end_ofs + _TEXT_BASE);
#endif
	/* Allow the early environment to override the fdt address */
	gd->fdt_blob = (void *)getenv_ulong("fdtcontroladdr", 16,
						(uintptr_t)gd->fdt_blob);

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr) {
		if ((*init_fnc_ptr)() != 0) {
			hang ();
		}
	}

	debug("monitor len: %08lX\n", gd->mon_len);
	/*
	 * Ram is setup, size stored in gd !!
	 */
	debug("ramsize: %08lX\n", gd->ram_size);
#if defined(CONFIG_SYS_MEM_TOP_HIDE)
	/*
	 * Subtract specified amount of memory to hide so that it won't
	 * get "touched" at all by U-Boot. By fixing up gd->ram_size
	 * the Linux kernel should now get passed the now "corrected"
	 * memory size and won't touch it either. This should work
	 * for arch/ppc and arch/powerpc. Only Linux board ports in
	 * arch/powerpc with bootwrapper support, that recalculate the
	 * memory size from the SDRAM controller setup will have to
	 * get fixed.
	 */
	gd->ram_size -= CONFIG_SYS_MEM_TOP_HIDE;
#endif

	addr = CONFIG_SYS_SDRAM_BASE + gd->ram_size;

#ifdef CONFIG_LOGBUFFER
#ifndef CONFIG_ALT_LB_ADDR
	/* reserve kernel log buffer */
	addr -= (LOGBUFF_RESERVE);
	debug("Reserving %dk for kernel logbuffer at %08lx\n", LOGBUFF_LEN,
		addr);
#endif
#endif

#ifdef CONFIG_PRAM
	/*
	 * reserve protected RAM
	 */
	reg = getenv_ulong("pram", 10, CONFIG_PRAM);
	addr -= (reg << 10);		/* size is in kB */
	debug("Reserving %ldk for protected RAM at %08lx\n", reg, addr);
#endif /* CONFIG_PRAM */

#if !(defined(CONFIG_SYS_ICACHE_OFF) && defined(CONFIG_SYS_DCACHE_OFF))
	/* reserve TLB table */
	addr -= (4096 * 4);

	/* round down to next 64 kB limit */
	addr &= ~(0x10000 - 1);

	gd->tlb_addr = addr;
	debug("TLB table at: %08lx\n", addr);
#endif

	/* round down to next 4 kB limit */
	addr &= ~(4096 - 1);
	debug("Top of RAM usable for U-Boot at: %08lx\n", addr);

#ifdef CONFIG_LCD
#ifdef CONFIG_FB_ADDR
	gd->fb_base = CONFIG_FB_ADDR;
#else
	/* reserve memory for LCD display (always full pages) */
	addr = lcd_setmem(addr);
	gd->fb_base = addr;
#endif /* CONFIG_FB_ADDR */
#endif /* CONFIG_LCD */

	/*
	 * reserve memory for U-Boot code, data & bss
	 * round down to next 4 kB limit
	 */
	addr -= gd->mon_len;
	addr &= ~(4096 - 1);

	debug("Reserving %ldk for U-Boot at: %08lx\n", gd->mon_len >> 10, addr);

#ifndef CONFIG_SPL_BUILD
	/*
	 * reserve memory for malloc() arena
	 */
	addr_sp = addr - TOTAL_MALLOC_LEN;
	debug("Reserving %dk for malloc() at: %08lx\n",
			TOTAL_MALLOC_LEN >> 10, addr_sp);
	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */
	addr_sp -= sizeof (bd_t);
	bd = (bd_t *) addr_sp;
	gd->bd = bd;
	debug("Reserving %zu Bytes for Board Info at: %08lx\n",
			sizeof (bd_t), addr_sp);

#ifdef CONFIG_MACH_TYPE
	gd->bd->bi_arch_number = CONFIG_MACH_TYPE; /* board id for Linux */
#endif

	addr_sp -= sizeof (gd_t);
	id = (gd_t *) addr_sp;
	debug("Reserving %zu Bytes for Global Data at: %08lx\n",
			sizeof (gd_t), addr_sp);

	/* setup stackpointer for exeptions */
	gd->irq_sp = addr_sp;
#ifdef CONFIG_USE_IRQ
	addr_sp -= (CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ);
	debug("Reserving %zu Bytes for IRQ stack at: %08lx\n",
		CONFIG_STACKSIZE_IRQ+CONFIG_STACKSIZE_FIQ, addr_sp);
#endif
	/* leave 3 words for abort-stack    */
	addr_sp -= 12;

	/* 8-byte alignment for ABI compliance */
	addr_sp &= ~0x07;
#else
	addr_sp += 128;	/* leave 32 words for abort-stack   */
	gd->irq_sp = addr_sp;
#endif

	debug("New Stack Pointer is: %08lx\n", addr_sp);

#ifdef CONFIG_POST
	post_bootmode_init();
	post_run(NULL, POST_ROM | post_bootmode_get(0));
#endif

	gd->bd->bi_baudrate = gd->baudrate;
	/* Ram ist board specific, so move it to board code ... */
	dram_init_banksize();
	display_dram_config();	/* and display it */

	addr =_TEXT_BASE;

	gd->relocaddr = addr;
	gd->start_addr_sp = addr_sp;
	gd->reloc_off = addr - _TEXT_BASE;
	debug("relocation Offset is: %08lx\n", gd->reloc_off);
	memcpy(id, (void *)gd, sizeof(gd_t));

	relocate_code(addr_sp, id, addr);

	/* NOTREACHED - relocate_code() does not return */
}

#if !defined(CONFIG_SYS_NO_FLASH)
static char *failed = "*** failed ***\n";
#endif

/*
 ************************************************************************
 *
 * This is the next part if the initialization sequence: we are now
 * running from RAM and have a "normal" C environment, i. e. global
 * data can be written, BSS has been cleared, the stack size in not
 * that critical any more, etc.
 *
 ************************************************************************
 */

void board_init_r(gd_t *id, ulong dest_addr)
{
	ulong malloc_start;
#if !defined(CONFIG_SYS_NO_FLASH)
	ulong flash_size;
#endif
#ifdef CFG_LG_CHG
	ulong src;
    init_fnc_t **init_fnc_ptr;
#endif
#if (CONFIG_ENABLE_MMU)
		HalInitMMU(0x1F00000);
#endif
	LogEnable = 2;

	gd = id;

	gd->flags |= GD_FLG_RELOC;	/* tell others: relocation done */

	monitor_flash_len = _end_ofs;

	/* Enable caches */
	enable_caches();

	debug("monitor flash len: %08lX\n", monitor_flash_len);
	board_init();	/* Setup chipselects */

#ifdef CONFIG_SERIAL_MULTI
	serial_initialize();
#endif

	debug("Now running in RAM - U-Boot at: %08lx\n", dest_addr);

#ifdef CONFIG_LOGBUFFER
	logbuff_init_ptrs();
#endif
#ifdef CONFIG_POST
	post_output_backlog();
#endif


	/* The Malloc area is immediately below the monitor copy in DRAM */
	malloc_start = dest_addr - TOTAL_MALLOC_LEN;
#if defined(CONFIG_MULTICORES_PLATFORM)
	mem_malloc_init (malloc_start, TOTAL_MALLOC_LEN-0x4000*6);
#else
	mem_malloc_init (malloc_start, TOTAL_MALLOC_LEN);
#endif
#if defined(CONFIG_MULTICORES_PLATFORM)
//    printf("Reserve all cores stack by digging malloc: TOTAL_MALLOC_LEN after = %x\n", TOTAL_MALLOC_LEN-0x4000*6);
//    printf("The end of the malloc addr = %x\n", malloc_start+TOTAL_MALLOC_LEN-0x4000*6);

    ulong malloc_end;
    unsigned int core_num=0;
    malloc_end = malloc_start+TOTAL_MALLOC_LEN;
    
    /*Keep the stack address for other cores*/
    writel(malloc_end, CONFIG_KERNEL_START_ADDRESS+SMP_DUMMY_SP);

   /*
    Here we put a specific number to the end of the stack.
    badbeef1: at the end of sp_svc for core1
    badbeef2: at the end of sp_svc for core2
    badbeef3: at the end of sp_svc for core3
    badbeef4: at the end of sp_irq for core1
    badbeef5: at the end of sp_irq for core2
    badbeef6: at the end of sp_irq for core3
   */

    for (core_num=1;core_num<=6;core_num++)
    {
        malloc_end-=(0x4000);
        *((unsigned int *)(malloc_end)) = 0xbadbeef0|core_num;
//        printf("Put boundary badbeef to = %x\n", malloc_end);
    }
#if 0
    unsigned int ttb_check;

    asm volatile ("mrc p15, 0, %0, c2, c0, 0"
                   : "=r" (ttb_check)
                   :
                   : "memory");
    printf("check TLB %x gd TlB %x\n", ttb_check, gd->tlb_addr);
#endif
#endif

#if defined(CONFIG_MULTICORES_PLATFORM)
    //scu_enable((void *)(0x16004000));  // Core 0 : For A12 Only    
    secondary_start_uboot();
#endif

#if !defined(CONFIG_SYS_NO_FLASH) && (!defined(CONFIG_NAND_BOOT) && !defined(CONFIG_EMMC_BOOT) && !defined(CONFIG_FAST_BOOT))
	puts("Flash: ");

	flash_size = flash_init();
	if (flash_size > 0) {
# ifdef CONFIG_SYS_FLASH_CHECKSUM
		char *s = getenv("flashchecksum");

		print_size(flash_size, "");
		/*
		 * Compute and print flash CRC if flashchecksum is set to 'y'
		 *
		 * NOTE: Maybe we should add some WATCHDOG_RESET()? XXX
		 */
		if (s && (*s == 'y')) {
			printf("  CRC: %08X", crc32(0,
				(const unsigned char *) CONFIG_SYS_FLASH_BASE,
				flash_size));
		}
		putc('\n');
# else	/* !CONFIG_SYS_FLASH_CHECKSUM */
		print_size(flash_size, "\n");
# endif /* CONFIG_SYS_FLASH_CHECKSUM */
	} else {
		puts(failed);
		hang();
	}
#endif

#if defined(CONFIG_CMD_NAND)
	puts("NAND:  ");
	nand_init();		/* go init the NAND */
#endif

#if defined(CONFIG_CMD_ONENAND)
	onenand_init();
#endif

#ifdef CONFIG_GENERIC_MMC
       puts("MMC:   ");
       mmc_initialize(gd->bd);
#endif

#ifdef CONFIG_HAS_DATAFLASH
	AT91F_DataflashInit();
	dataflash_print_info();
#endif

	/* initialize environment */
	env_relocate();

#ifdef CFG_LG_CHG
    for(init_fnc_ptr = init_sequence_post; *init_fnc_ptr; ++init_fnc_ptr)
    {
        if((*init_fnc_ptr)() != 0)
        {
            hang();
        }
    }
#endif

#if defined(CONFIG_CMD_PCI) || defined(CONFIG_PCI)
	arm_pci_init();
#endif
#ifdef CFG_LG_CHG
	verify_mac_address();
	stdio_preinit();

	/*
	 * call devices_init in DEBUG_LEVEL for using usb2serial.
	 * if not , call it later.
	 * */
	if (DDI_NVM_GetDebugStatus() == DEBUG_LEVEL)
	{
#ifdef CONFIG_USB_POWER_CONTROL
		extern void turn_on_USB_power_switch(void);
		turn_on_USB_power_switch();
#endif
		stdio_init();	/* get the devices list going. */
	}
	src = get_tzfw_base();
	if (src != 0)
	{
	    get_kernel_size(src);
#ifdef CC_TRUSTZONE_SUPPORT
	    move_tz_binary(src);
#endif
#if 0 //def	CONFIG_HIBERNATION
	    if(check_snapshot_mode() == LGSNAP_SNAPSHOTBOOT)
	    {
		init_tz(); // which will cause fail in verification
	    }
#endif
	}
	#if defined(CONFIG_MULTICORES_PLATFORM)
	//IO_WRITE32(0xf0008000, 0x2c, 0x42000000);
	extern void thread_start(void);
	thread_start();
	#else
	fast_boot();
	for(;;)
		{
			main_loop();
		}
	#endif
#endif

#if !defined(CONFIG_FAST_BOOT) || defined(CONFIG_SUPPORT_ETHERNET)
		/* IP Address */
		gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");
		puts("\n");
		print_IPaddr(gd->bd->bi_ip_addr);
		puts("\n");
	
		/* MAC Address */
		{
			int i;
			ulong reg;
			char *s, *e;
			char tmp[64];
	
			i = getenv_r ("ethaddr", tmp, sizeof (tmp));
			s = (i > 0) ? tmp : NULL;
	
			for (reg = 0; reg < 6; ++reg) {
				gd->bd->bi_enetaddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
				if (s)
					s = (*e) ? e + 1 : e;
			}
	
#ifdef CONFIG_HAS_ETH1
			i = getenv_r ("eth1addr", tmp, sizeof (tmp));
			s = (i > 0) ? tmp : NULL;
	
			for (reg = 0; reg < 6; ++reg) {
				gd->bd->bi_enet1addr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
				if (s)
					s = (*e) ? e + 1 : e;
			}
#endif
		}
#endif /* !defined(CONFIG_FAST_BOOT) */


	stdio_init();	/* get the devices list going. */

	jumptable_init();

#if defined(CONFIG_API)
	/* Initialize API */
	api_init();
#endif

	console_init_r();	/* fully init console as a device */

#if defined(CONFIG_ARCH_MISC_INIT)
	/* miscellaneous arch dependent initialisations */
	arch_misc_init();
#endif
#if defined(CONFIG_MISC_INIT_R)
	/* miscellaneous platform dependent initialisations */
	misc_init_r();
#endif
#ifndef CFG_LG_CHG
#if (CONFIG_DUAL_BANK_ROOTFS) || (CONFIG_DUAL_BANK_KERNEL)
    dual_bank_init();
#else
    dtvcfg_init();
#endif
#endif
	 /* set up exceptions */
	interrupt_init();
	/* enable exceptions */
	//enable_interrupts();

	/* Perform network card initialisation if necessary */
#if defined(CONFIG_DRIVER_SMC91111) || defined (CONFIG_DRIVER_LAN91C96)
	/* XXX: this needs to be moved to board init */
	if (getenv("ethaddr")) {
		uchar enetaddr[6];
		eth_getenv_enetaddr("ethaddr", enetaddr);
		smc_set_mac_addr(enetaddr);
	}
#endif /* CONFIG_DRIVER_SMC91111 || CONFIG_DRIVER_LAN91C96 */

	/* Initialize from environment */
	load_addr = getenv_ulong("loadaddr", 16, load_addr);
#if defined(CONFIG_CMD_NET)
	{
		char *s = getenv("bootfile");

		if (s != NULL)
			copy_filename(BootFile, s, sizeof(BootFile));
	}
#endif

#ifdef CONFIG_BOARD_LATE_INIT
	board_late_init();
#endif

#ifdef CONFIG_BITBANGMII
	bb_miiphy_init();
#endif
#if defined(CONFIG_CMD_NET)
	puts("Net:   ");
	eth_initialize(gd->bd);
#if defined(CONFIG_RESET_PHY_R)
	debug("Reset Ethernet PHY\n");
	reset_phy();
#endif
#endif

#ifdef CONFIG_POST
	post_run(NULL, POST_RAM | post_bootmode_get(0));
#endif

#if defined(CONFIG_PRAM) || defined(CONFIG_LOGBUFFER)
	/*
	 * Export available size of memory for Linux,
	 * taking into account the protected RAM at top of memory
	 */
	{
		ulong pram = 0;
		uchar memsz[32];

#ifdef CONFIG_PRAM
		pram = getenv_ulong("pram", 10, CONFIG_PRAM);
#endif
#ifdef CONFIG_LOGBUFFER
#ifndef CONFIG_ALT_LB_ADDR
		/* Also take the logbuffer into account (pram is in kB) */
		pram += (LOGBUFF_LEN + LOGBUFF_OVERHEAD) / 1024;
#endif
#endif
		sprintf((char *)memsz, "%ldk", (gd->ram_size / 1024) - pram);
		setenv("mem", (char *)memsz);
	}
#endif

	/* main_loop() can return to retry autoboot, if so just run it again. */
	for (;;) {
		main_loop();
	}

	/* NOTREACHED - no way out of command loop except booting */
}
void second_main(void)
{
	
#if !defined(CONFIG_FAST_BOOT) || defined(CONFIG_SUPPORT_ETHERNET)
			/* IP Address */
			gd->bd->bi_ip_addr = getenv_IPaddr ("ipaddr");
			puts("\n");
			print_IPaddr(gd->bd->bi_ip_addr);
			puts("\n");
		
			/* MAC Address */
			{
				int i;
				ulong reg;
				char *s, *e;
				char tmp[64];
		
				i = getenv_r ("ethaddr", tmp, sizeof (tmp));
				s = (i > 0) ? tmp : NULL;
		
				for (reg = 0; reg < 6; ++reg) {
					gd->bd->bi_enetaddr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
					if (s)
						s = (*e) ? e + 1 : e;
				}
		
#ifdef CONFIG_HAS_ETH1
				i = getenv_r ("eth1addr", tmp, sizeof (tmp));
				s = (i > 0) ? tmp : NULL;
		
				for (reg = 0; reg < 6; ++reg) {
					gd->bd->bi_enet1addr[reg] = s ? simple_strtoul (s, &e, 16) : 0;
					if (s)
						s = (*e) ? e + 1 : e;
				}
#endif
			}
#endif /* !defined(CONFIG_FAST_BOOT) */
	
	
		stdio_init();	/* get the devices list going. */
	
		jumptable_init();
	
#if defined(CONFIG_API)
		/* Initialize API */
		api_init();
#endif
	
		console_init_r();	/* fully init console as a device */
	
#if defined(CONFIG_ARCH_MISC_INIT)
		/* miscellaneous arch dependent initialisations */
		arch_misc_init();
#endif
#if defined(CONFIG_MISC_INIT_R)
		/* miscellaneous platform dependent initialisations */
		misc_init_r();
#endif
#ifndef CFG_LG_CHG
#if (CONFIG_DUAL_BANK_ROOTFS) || (CONFIG_DUAL_BANK_KERNEL)
		dual_bank_init();
#else
		dtvcfg_init();
#endif
#endif
		 /* set up exceptions */
		interrupt_init();
		/* enable exceptions */
		//enable_interrupts();
	
		/* Perform network card initialisation if necessary */
#if defined(CONFIG_DRIVER_SMC91111) || defined (CONFIG_DRIVER_LAN91C96)
		/* XXX: this needs to be moved to board init */
		if (getenv("ethaddr")) {
			uchar enetaddr[6];
			eth_getenv_enetaddr("ethaddr", enetaddr);
			smc_set_mac_addr(enetaddr);
		}
#endif /* CONFIG_DRIVER_SMC91111 || CONFIG_DRIVER_LAN91C96 */
	
		/* Initialize from environment */
		load_addr = getenv_ulong("loadaddr", 16, load_addr);
#if defined(CONFIG_CMD_NET)
		{
			char *s = getenv("bootfile");
	
			if (s != NULL)
				copy_filename(BootFile, s, sizeof(BootFile));
		}
#endif
	
#ifdef CONFIG_BOARD_LATE_INIT
		board_late_init();
#endif
	
#ifdef CONFIG_BITBANGMII
		bb_miiphy_init();
#endif
#if defined(CONFIG_CMD_NET)
		puts("Net:	 ");
		eth_initialize(gd->bd);
#if defined(CONFIG_RESET_PHY_R)
		debug("Reset Ethernet PHY\n");
		reset_phy();
#endif
#endif
	
#ifdef CONFIG_POST
		post_run(NULL, POST_RAM | post_bootmode_get(0));
#endif
	
#if defined(CONFIG_PRAM) || defined(CONFIG_LOGBUFFER)
		/*
		 * Export available size of memory for Linux,
		 * taking into account the protected RAM at top of memory
		 */
		{
			ulong pram = 0;
			uchar memsz[32];
	
#ifdef CONFIG_PRAM
			pram = getenv_ulong("pram", 10, CONFIG_PRAM);
#endif
#ifdef CONFIG_LOGBUFFER
#ifndef CONFIG_ALT_LB_ADDR
			/* Also take the logbuffer into account (pram is in kB) */
			pram += (LOGBUFF_LEN + LOGBUFF_OVERHEAD) / 1024;
#endif
#endif
			sprintf((char *)memsz, "%ldk", (gd->ram_size / 1024) - pram);
			setenv("mem", (char *)memsz);
		}
#endif
	
		
		printf("\n[Snapshot debug] normal uboot flow\n");
		/* main_loop() can return to retry autoboot, if so just run it again. */
		//for (;;) {
		//	main_loop();
		//}
}
void hang(void)
{
	puts("### ERROR ### Please RESET the board ###\n");
	for (;;);
}
#ifdef CFG_LG_CHG

/*
 * 2011.08.23..zaewon.lee
 * These code is copied from l9 boot/platform/gp4/platform_board.c
 * please compare with mtk boot/uboot/u-boot-1.3.4/common/lg_cmd/cmd_misc.c
 */

/* flash에 저장된 mac address format..
 * broadcom에서 사용하는 포맷을 그대로 사용하는듯 하다
 * HEADER(12) + MAC(8) * 8 + DUMMY(64) + CHECKSUM(2)
 * 8개의 동일한 mac address가 저장되는데 이 중에 2번째에 위치하는 것을 사용함
 */
#define FLASH_MAC_DATA_SIZE		140
#define FLASH_MAC_HEADER_SIZE	12
#define FLASH_MAC_CHECKSUM_SIZE	2
#define FLASH_MAC_TOTAL_LEN		(FLASH_MAC_DATA_SIZE + FLASH_MAC_CHECKSUM_SIZE)
#define FLASH_MAC_START_INDEX	(12 + 8)	/* 2nd mac adddress offset */

#define EEPROM_MAC_MAGIC		0x20110208
#define MAC_STR_LEN				17
/* storage func */
extern int storage_read(uint32_t offset, size_t len, void* buf);
extern int storage_write(uint32_t offset, size_t len, void* buf);

static int verify_mac_checksum(uint8_t* data)
{
	uint8_t checksum = 0;
	uint32_t idx;

	if(data[FLASH_MAC_DATA_SIZE] != data[FLASH_MAC_DATA_SIZE+1])
		goto invalid;

	for(idx=0; idx<FLASH_MAC_DATA_SIZE; idx++)
	{
		checksum += data[idx];
	}

	if(checksum == data[FLASH_MAC_DATA_SIZE])
		return 0;

invalid:
#if 0
	printf("invalid checksum. calculated=%02x, data[n]=%02x, data[n+1]=%02x\n",
		checksum, data[FLASH_MAC_DATA_SIZE], data[FLASH_MAC_DATA_SIZE+1]);
#endif

	return -1;
}

static void verify_mac_address(void)
{
	int rc, idx;
	struct partition_info * partinfo;
	uint8_t macbuf[FLASH_MAC_TOTAL_LEN];
	char mac_str[MAC_STR_LEN + 1];
	uint8_t valid_flash = 0;
	uint8_t update_env 	= 0;
	uint32_t loaded_valid_mark, valid_mark = EEPROM_MAC_MAGIC;
	char *eth_addr		=getenv("ethaddr");
	uint8_t update_eeprom = 0;

	/* read mac address in partition 'macadr' */
	if((partinfo = get_partition_by_name("macadr")) != NULL)
	{
		rc = storage_read(partinfo->offset, FLASH_MAC_TOTAL_LEN, macbuf);
		if(rc >= 0)
		{
			if(verify_mac_checksum(macbuf) == 0)
			{
				idx = FLASH_MAC_START_INDEX;
				sprintf(mac_str, "%02X:%02X:%02X:%02X:%02X:%02X",
					macbuf[idx+0], macbuf[idx+1], macbuf[idx+2],
					macbuf[idx+3], macbuf[idx+4], macbuf[idx+5]);
				mac_str[MAC_STR_LEN] = '\0';
				valid_flash = 1;
				if (strncmp(eth_addr,mac_str,MAC_STR_LEN))
				{
					/* update env variable */
					update_env = 1;
				}
			}
		}
	}
	else
	{
		printf("macadr partition not found.\n");
	}

#if 0 // backup to nvram is not implemented
	eth_addr = getenv("ethaddr");

	/* read mac addr from eeprom */
	NVM_MAC_ADDRESS_LOAD_ITEM(validMark, &loaded_valid_mark);
	if(valid_flash == 0)
	{
		if(loaded_valid_mark == valid_mark)
		{
			//printf("mac address in flash is broken, use it in eeprom!\n");
			NVM_MAC_ADDRESS_LOAD_ITEM(macAddr, mac_str);
			mac_str[MAC_STR_LEN] = '\0';
		}
		else
		{
			printf("mac address in both flash & eeprom are invalid\n");
			if(eth_addr != NULL)
			{
				strcpy(mac_str, eth_addr);
			}
			else
			{
				sprintf(mac_str, DEFAULT_ENV_ETHADDR);
			}
			update_eeprom = 1;
		}
	}
	else
	{
		if(loaded_valid_mark != valid_mark)
		{
			update_eeprom = 1;
		}
	}
	if(eth_addr == NULL || strcmp(mac_str, eth_addr) != 0)
	{
		printf("mac address in env and flash are different !\n");

		update_eeprom = 1;
		setenv("ethaddr", mac_str);
		saveenv();
	}

	if(update_eeprom)
	{
		NVM_MAC_ADDRESS_STORE_ITEM(macAddr, mac_str);
		NVM_MAC_ADDRESS_STORE_ITEM(validMark, &valid_mark);

		printf("writing mac address to eeprom...\n");
	}
#else
	if (valid_flash == 1)
	{
		setenv("ethaddr", mac_str);
		if (update_env == 1)
			saveenv();
	}
	else
		printf("not valid mac in macadr partition. use default ethaddr.\n");
#endif
}

#endif  /* CFG_LG_CHG */
