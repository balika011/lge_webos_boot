/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * Copyright (C) 2001  Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307	 USA
 *
 */

#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <fdt.h>
#include <libfdt.h>
#include <fdt_support.h>

#define CFG_LG_CHG
#include <lg_modeldef.h>
#include "drvcust_if.h"
#include "c_model.h"
#include "x_bim.h"
#include <cmd_resume.h>
#include <mmc.h>
#include "x_ldr_env.h"
#include <cmd_micom.h>
#include <cmnio_type.h>
#include "x_dram.h"
#include <swum.h>
#include <asm/io.h>
#include <x_hal_5381.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd);

# ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd);
# endif
static void setup_commandline_tag (bd_t *bd, char *commandline);

# ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start,
			      ulong initrd_end);
# endif
static void setup_end_tag (bd_t *bd);

static struct tag *params;
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void);
#if defined(CONFIG_OF_LIBFDT)
static int bootm_linux_fdt(int machid, bootm_headers_t *images);
#endif

void arch_lmb_reserve(struct lmb *lmb)
{
	ulong sp;

	/*
	 * Booting a (Linux) kernel image
	 *
	 * Allocate space for command line and board info - the
	 * address should be as high as possible within the reach of
	 * the kernel (see CONFIG_SYS_BOOTMAPSZ settings), but in unused
	 * memory, which means far enough below the current stack
	 * pointer.
	 */
	sp = get_sp();
	debug("## Current stack ends at 0x%08lx ", sp);

	/* adjust sp by 1K to be safe */
	sp -= 1024;
	lmb_reserve(lmb, sp,
		    gd->bd->bi_dram[0].start + gd->bd->bi_dram[0].size - sp);
}

static void announce_and_cleanup(void)
{
	printf("\nStarting kernel ...\n\n");

#ifdef CONFIG_USB_DEVICE
	{
		extern void udc_disconnect(void);
		udc_disconnect();
	}
#endif

    {
        extern void turn_off_USB_power_switch(void);
        turn_off_USB_power_switch();
    }

	cleanup_before_linux();
}

#ifdef CFG_LG_CHG
extern unsigned int kmemsize;
extern unsigned int chb_kmemstart;
extern unsigned int chb_kmemsize;
extern unsigned int tzsize;

#define arg_next(p)				(char *)((unsigned long)p+strlen(p))

extern uint32_t appxip_len;				/* app xip length */
extern uint32_t fontxip_len;			/* font xip length */

extern char strModelOpt[];
extern char strHWOption[];
extern char bPortProtect;
extern UINT16 gToolOpt[];
extern MODEL_INFO_DB_T gModelInfoDB;

extern void release_non_boot_core(void);

extern char * get_blkdev_name(void);
extern int get_blkdev_idx(const char *name);
//extern char buildVers[];
extern uint8_t gDispType;
extern int getFullVerifyOTP(void);
extern unsigned int tzcorestart;

void linux_param_set(char *kargs)
{
	char *lock_time;
	char *bootmode = getenv("bootmode");
	char *init_file = getenv("initfile");
	uint32_t xip_size_mb = 0, appxip_addr = 0, fontxip_addr = 0;
	char *mac, *print, *lpj,*emmc_log;
	//char *malimem, *umpmem;
	int swumode = get_swumode();
	unsigned int soc_rev = 0;
	struct mmc	*mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);
	
	ulong keymem = 0;	/* key memory addr */
	ulong keymem_len = 0;	/* key  memory  length */
	
    UINT8 au4CustKey[256] = {
		0x99, 0xc4, 0xcd, 0x15, 0xaf, 0x54, 0xe0, 0xf6, 0x0f, 0x70, 0x25, 0x12, 
	   0xfd, 0x30, 0x83, 0xb6, 0x0e, 0x00, 0x09, 0x91, 0xa2, 0x78, 0xd9, 0x54, 
	   0x01, 0xae, 0xb1, 0xbd, 0xb1, 0x4d, 0x8e, 0x06, 0x57, 0x64, 0x64, 0x38, 
	   0x85, 0x04, 0xe9, 0x65, 0x8e, 0x1a, 0x64, 0xca, 0xf5, 0xa1, 0x14, 0x85, 
	   0x51, 0x6c, 0x5f, 0xbc, 0xe2, 0x23, 0xbb, 0x99, 0xe1, 0xd9, 0x30, 0x3d, 
	   0x05, 0x71, 0x06, 0x7a, 0xe4, 0xbc, 0x51, 0x25, 0x23, 0xf8, 0x52, 0x9f, 
	   0x31, 0xae, 0x24, 0x0e, 0x68, 0x4c, 0x4c, 0x5e, 0x22, 0x7f, 0x31, 0x28, 
	   0x73, 0x2b, 0x93, 0x8b, 0x0a, 0x40, 0x15, 0xe5, 0x9f, 0xa5, 0xbd, 0xe7, 
	   0xfb, 0xb0, 0x53, 0xf5, 0xcd, 0x8b, 0x8c, 0x9e, 0x21, 0x6a, 0xdb, 0xd3, 
	   0x4d, 0xca, 0xb2, 0xb6, 0x60, 0xab, 0xaf, 0xb9, 0x3f, 0xec, 0x5a, 0x85, 
	   0xd2, 0x69, 0x40, 0xda, 0x04, 0xa2, 0xd2, 0x56, 0x91, 0x62, 0x72, 0x1d, 
	   0xd6, 0x4c, 0xe2, 0x1d, 0x8d, 0x26, 0xc2, 0xaf, 0x77, 0x78, 0xad, 0x6c, 
	   0x14, 0xe1, 0xde, 0xa0, 0x6d, 0xdf, 0xc4, 0x18, 0x8d, 0x19, 0xb5, 0xe3, 
	   0x1a, 0x5e, 0xda, 0x37, 0xfd, 0x85, 0x10, 0x7a, 0xca, 0xc2, 0x07, 0x99, 
	   0x3b, 0x13, 0x89, 0x02, 0xd5, 0x3a, 0xfd, 0x18, 0xf9, 0x9e, 0x0b, 0xfd, 
	   0xf9, 0x2d, 0xa3, 0x83, 0xdc, 0x63, 0x71, 0x27, 0x19, 0x66, 0xd7, 0xf1, 
	   0xf3, 0x60, 0x4f, 0x5d, 0xa9, 0xe9, 0x63, 0x79, 0x72, 0x3d, 0x03, 0x5e, 
	   0x6a, 0x2e, 0x09, 0xe6, 0x22, 0xd8, 0x42, 0x09, 0xa8, 0x66, 0x04, 0x5c, 
	   0xea, 0x0e, 0xfa, 0x56, 0x5a, 0xd7, 0xfc, 0x28, 0x91, 0x6c, 0x55, 0xc1, 
	   0x73, 0x3a, 0x10, 0x31, 0x57, 0x0b, 0x6e, 0x71, 0xd8, 0xbb, 0xc4, 0x04, 
	   0x7e, 0xc7, 0x68, 0xb2, 0xa4, 0xc6, 0x54, 0x71, 0x83, 0xd7, 0x73, 0xfc, 
	   0x6a, 0x91, 0xbc, 0xc2
    	};
#ifdef SIGN_USE_PARTIAL	
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)0xfb005000;
#endif
#ifdef SIGN_USE_PARTIAL	
#if 0
		if ( getFullVerifyOTP() )
		{
			sprintf(arg_next(kargs),"fullverify ");
		}	
		else
#endif
		{
			//copy pub key to mem		
			memcpy((void *)0x4000000, (void *)au4CustKey, 256);
		}
#endif

	//default bootargs : default no args. So you can add.
	if(!strcmp(bootmode, "user"))
	{
		char *cmdline = getenv ("bootargs");
		if(cmdline != NULL) sprintf(arg_next(kargs), "%s ", cmdline);
		return;
	}

	if(!strcmp(bootmode, "webos")) // nfs booting
	{
		char *nfsroot	= getenv("nfsroot");
		char *nfsserver	= getenv("nfsserver");
		char *serverip	= getenv("serverip");
		char *ipaddr	= getenv("ipaddr");
		char *gatewayip	= getenv("gatewayip");
		char *netmask	= getenv("netmask");

		sprintf(arg_next(kargs), "root=/dev/nfs rw nfsroot=%s:%s,%s ",
				(nfsserver)?nfsserver:"nfsserver",
				(nfsroot)?nfsroot:"root",
				NFS_PARAMETER);

		sprintf(arg_next(kargs), "ip=%s:%s:%s:%s::eth0:off ", ipaddr, serverip, gatewayip, netmask);
	}
	else	// bootmode == auto
	{
		char root_dev[32] = {0, };
		struct partition_info *pi = NULL;

		if(swumode)
		{
			pi = get_used_partition("swue");
			sprintf(root_dev, "/dev/%s%d ", get_blkdev_name(), get_blkdev_idx("swue"));
		}
		else
		{
			pi = get_used_partition("rootfs");
			sprintf(root_dev, "/dev/%s%d ", get_blkdev_name(), get_blkdev_idx("rootfs"));
		}

		if(!pi) {
			printf("Invalid rootfs partition\n");
			return;
		}

		sprintf(arg_next(kargs), "root=%s ro%s rootfstype=%s ",
				root_dev,
				(init_file)? init_file:"",
				"squashfs");
	}

//add mac address for debugging in mtk office
	mac = getenv("ethaddr");
	sprintf(arg_next(kargs),"macadr=%s ", mac);
//add mtk kernel bootargs 
	emmc_log   = getenv("emmclog");
	sprintf(arg_next(kargs),"emmclog=%s ", emmc_log);
	sprintf(arg_next(kargs), "%s ", "console=ttyMT0,115200n1 ");
	sprintf(arg_next(kargs), "%s ", "rootwait ");
	sprintf(arg_next(kargs), "%s ", "usbportusing=1,1,0,1 ");
	sprintf(arg_next(kargs), "%s ", "usbpwrgpio=406:1,407:1,-1:-1,-1:-1 ");
	sprintf(arg_next(kargs), "%s ", "usbocgpio=405:0,404:0,-1:-1,-1:-1 ");
	sprintf(arg_next(kargs), "%s ", "tzsz=18m ");
    sprintf(arg_next(kargs), "tzcorestart=0x%x ", tzcorestart);
	
	/* change vmalloc memory for increase low memory size ,  700M-->650M (1G) ,  700M--> 600M(768M) marked by   hongjun.chu*/	
	if (TCMGET_CHANNELA_SIZE() == 0x300) /* 768M case */
	{
		sprintf(arg_next(kargs), "%s ", "vmalloc=600mb");
	}
	else
	{
		sprintf(arg_next(kargs), "%s ", "vmalloc=650mb");
	}
	
//LG's 
	if(swumode)
		sprintf(arg_next(kargs), "swumode ");

	if ((!(strncmp(getenv("print"),"off",3))) || (DDI_NVM_GetDebugStatus() != DEBUG_LEVEL))
	{
		printf("SILENT MODE!\n");
		sprintf(arg_next(kargs),"%s ", "quiet loglevel=0"); // silent mode
	}
	sprintf(arg_next(kargs), "%s ", "devtmpfs.mount=1");
	sprintf(arg_next(kargs),"modelopt=%s ", strModelOpt);
	sprintf(arg_next(kargs), "hwopt=%s ", strHWOption);

	DDI_NVM_Read(MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.validMark) - (UINT32)&gModelInfoDB, \
			sizeof(gModelInfoDB.validMark), (UINT8 *)&(gModelInfoDB.validMark));
	if(gModelInfoDB.validMark != 0xffffffff)
	{
		DDI_NVM_Read(MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.aModelName) - (UINT32)&gModelInfoDB, \
				sizeof(gModelInfoDB.aModelName), (UINT8 *)&(gModelInfoDB.aModelName));

		DDI_NVM_Read(MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.aSerialNum) - (UINT32)&gModelInfoDB,   \
				sizeof(gModelInfoDB.aSerialNum), (UINT8 *)&(gModelInfoDB.aSerialNum));

		DDI_NVM_Read(MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.group_code) - (UINT32)&gModelInfoDB,   \
				sizeof(gModelInfoDB.group_code), (UINT8 *)&(gModelInfoDB.group_code));
	}

	sprintf(arg_next(kargs), "ToolOpt=%d:%d:%d:%d:%d:%d:%d ", gToolOpt[0], gToolOpt[1], gToolOpt[2], gToolOpt[3], gToolOpt[4], gToolOpt[5], gToolOpt[6]);
	sprintf(arg_next(kargs), "debugMode=%d ", DDI_NVM_GetDebugStatus());
	sprintf(arg_next(kargs), "countryGrp=%d ", gModelInfoDB.group_code);
	sprintf(arg_next(kargs), "modelName=%s ", gModelInfoDB.aModelName);
	sprintf(arg_next(kargs), "serialNum=%s ", gModelInfoDB.aSerialNum);
	sprintf(arg_next(kargs), "%s ", "sver=3.00.00 bver=3.00.00");

	if(IS_IC_5882_ES1() || IS_IC_5882_ES2())
		sprintf(arg_next(kargs), "%s ", "chip=A5LRA0");
	else
		sprintf(arg_next(kargs), "%s ", "chip=A5LRB0");

	if(MICOM_IsPowerOnly() || !DDI_NVM_GetInstopStatus())
		sprintf(arg_next(kargs), "%s ", "factory");

	if(!mmc)
		printf("No mmc device at slot %x\n", CONFIG_SYS_MMC_ENV_DEV);
	else
		sprintf(arg_next(kargs), "emmc_size=0x%01lx%08lx ", U64_UPPER(mmc->capacity), U64_LOWER(mmc->capacity));

#if defined(LG_CHG)
    if ( DDI_NVM_GetFullVerifyFlag() )
    {
        printf("verify_done = 0x%x \n",verify_done);
        sprintf(arg_next(kargs), "fullverify ");
        if ( verify_done == (VERIFY_APPS_DONE | VERIFY_TZFW_DONE) )
        {
            printf("although current fullverify was ON, all apps and kernel is verified in full, we set full verify flag off \n");
            DDI_NVM_SetFullVerifyFlag(0);
        }
    }
#else
    sprintf(arg_next(kargs), "fullverify ");
#endif

	if((check_snapshot_mode() == LGSNAP_MAKING_IMAGE))
	{
		sprintf(arg_next(kargs), "snapshot ");
	}
	sprintf(arg_next(kargs), "resume=/dev/%s%d ", get_blkdev_name(), get_blkdev_idx(SNAP_PART_NAME));

	{
		unsigned char art = DDI_NVM_GetSnapShotART();
		if( art == 'A' )
			sprintf(arg_next(kargs), "art=auto art_period=20 ");
		else if( art == 'R' )
			sprintf(arg_next(kargs), "art=resume art_period=20 ");
		else if( art == 'M' )
			sprintf(arg_next(kargs), "art=making art_period=20 ");
	}
#if defined(LG_CHG)   // for kernel  reserve memory for customer public key 
	keymem = 0x4000000;	
	keymem_len = 256;
	sprintf( arg_next(kargs), "keymem=0x%x,0x%x ", keymem,  keymem_len);
#endif

	sprintf(arg_next(kargs), "%s ", "cmdEnd");

	
}
#endif

int do_bootm_linux(int flag, int argc, char *argv[], bootm_headers_t *images)
{
	ulong	ep = 0;
	bd_t	*bd = gd->bd;
	char	*s;
	int	machid = bd->bi_arch_number;
    
        void	(*kernel_entry)(int zero, int arch, uint params, uint kernel_entry);

#ifdef CONFIG_CMDLINE_TAG

#ifdef CFG_LG_CHG
	char *kargs[1024] = {0};
#else
	char *commandline = getenv ("bootargs");
#endif


#endif

#ifdef CFG_LG_CHG
	linux_param_set(kargs);
	printf("cmdline = %s \n",kargs);
#endif

	printf("%s: images->legacy_hdr_valid = %ld\n", __func__, images->legacy_hdr_valid);
	if ((flag != 0) && (flag != BOOTM_STATE_OS_GO))
		return 1;
	/* find kernel entry point */
#if 1
        ep = 0x8000;
#else
	if (images->legacy_hdr_valid) {
		ep = image_get_ep (&images->legacy_hdr_os_copy);
		printf("legacy hdr %p\n", images->legacy_hdr_os);
		printf("legacy hdr->comp type = %d\n", images->legacy_hdr_os->ih_comp);
		if (images->legacy_hdr_os->ih_comp == 0)
			ep += sizeof(image_header_t);
#if defined(CONFIG_FIT)
	} else if (images->fit_uname_os) {
		ret = fit_image_get_entry (images->fit_hdr_os,
					images->fit_noffset_os, &ep);
		if (ret) {
			puts ("Can't get entry point property!\n");
			goto error;
		}
		printf("%s: CONFIG_FIT ???\n");
#endif
	} else {
		puts ("Could not find kernel entry point!\n");
		//goto error;
	}
#endif
	printf("%s: ep = 0x%lx\n", __func__, ep);


	s = getenv ("machid");
	if (s) {
		machid = simple_strtoul (s, NULL, 16);
		printf ("Using machid 0x%x from environment\n", machid);
	}

	show_boot_progress (15);

#ifdef CONFIG_OF_LIBFDT
	if (images->ft_len)
		return bootm_linux_fdt(machid, images);
#endif


#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
	setup_start_tag (bd);
#ifdef CONFIG_SERIAL_TAG
	setup_serial_tag (&params);
#endif
#ifdef CONFIG_REVISION_TAG
	setup_revision_tag (&params);
#endif
#ifdef CONFIG_SETUP_MEMORY_TAGS
	setup_memory_tags (bd);
#endif
#ifdef CONFIG_CMDLINE_TAG
#ifdef CFG_LG_CHG
	setup_commandline_tag (bd, kargs);
#else
	setup_commandline_tag (bd, commandline);
#endif
#endif
#ifdef CONFIG_INITRD_TAG
	if (images->rd_start && images->rd_end)
		setup_initrd_tag (bd, images->rd_start, images->rd_end);
#endif
	setup_end_tag(bd);
#endif

	BIM_SetTimeLog(3);
    release_non_boot_core();

	announce_and_cleanup();

	disable_wp(0); // eMMC W/P disable before jump to kernel
	//gd->flags &= ~GD_FLG_SILENT;
	printf("[%4d] Cold: Jump to kernel...ep=0x%08lx, machid=0x%x, bi=0x%lx\n", readMsTicks(), ep, machid, bd->bi_boot_params);
	//PrintLateBuffer();

#ifdef CC_TRUSTZONE_SUPPORT
	init_tz();
#endif
	printf ("\nStarting kernel ...\n\n");
	kernel_entry = (void (*)(int, int, uint, uint))ep;
	kernel_entry (0, machid, bd->bi_boot_params, ep); // jump to kernel

	/* does not return */

	return 1;
}

#if defined(CONFIG_OF_LIBFDT)
static int fixup_memory_node(void *blob)
{
	bd_t	*bd = gd->bd;
	int bank;
	u64 start[CONFIG_NR_DRAM_BANKS];
	u64 size[CONFIG_NR_DRAM_BANKS];

	for (bank = 0; bank < CONFIG_NR_DRAM_BANKS; bank++) {
		start[bank] = bd->bi_dram[bank].start;
		size[bank] = bd->bi_dram[bank].size;
	}

	return fdt_fixup_memory_banks(blob, start, size, CONFIG_NR_DRAM_BANKS);
}

static int bootm_linux_fdt(int machid, bootm_headers_t *images)
{
	ulong rd_len;
	void (*kernel_entry)(int zero, int dt_machid, void *dtblob);
	ulong of_size = images->ft_len;
	char **of_flat_tree = &images->ft_addr;
	ulong *initrd_start = &images->initrd_start;
	ulong *initrd_end = &images->initrd_end;
	struct lmb *lmb = &images->lmb;
	int ret;

	kernel_entry = (void (*)(int, int, void *))images->ep;

	boot_fdt_add_mem_rsv_regions(lmb, *of_flat_tree);

	rd_len = images->rd_end - images->rd_start;
	ret = boot_ramdisk_high(lmb, images->rd_start, rd_len,
				initrd_start, initrd_end);
	if (ret)
		return ret;

	ret = boot_relocate_fdt(lmb, of_flat_tree, &of_size);
	if (ret)
		return ret;

	debug("## Transferring control to Linux (at address %08lx) ...\n",
	       (ulong) kernel_entry);

	fdt_chosen(*of_flat_tree, 1);

	fixup_memory_node(*of_flat_tree);

	fdt_fixup_ethernet(*of_flat_tree);

	fdt_initrd(*of_flat_tree, *initrd_start, *initrd_end, 1);

	announce_and_cleanup();

	kernel_entry(0, machid, *of_flat_tree);
	/* does not return */

	return 1;
}
#endif

#if defined (CONFIG_SETUP_MEMORY_TAGS) || \
    defined (CONFIG_CMDLINE_TAG) || \
    defined (CONFIG_INITRD_TAG) || \
    defined (CONFIG_SERIAL_TAG) || \
    defined (CONFIG_REVISION_TAG)
static void setup_start_tag (bd_t *bd)
{
	params = (struct tag *) bd->bi_boot_params;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}


#ifdef CONFIG_SETUP_MEMORY_TAGS
static void setup_memory_tags (bd_t *bd)
{
	
	int i;


    for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++) {
        params->hdr.tag = ATAG_MEM;
        params->hdr.size = tag_size (tag_mem32);

        if ((i == 1) && (chb_kmemstart != 0))
{
            params->u.mem.start = chb_kmemstart;
}
        else
	{
            params->u.mem.start = bd->bi_dram[i].start;
	}
		
        if ((i == 0) && (kmemsize != 0))
	{
            params->u.mem.size = kmemsize;
	}
        else if ((i == 1) && (chb_kmemsize != 0))
	{
            params->u.mem.size = chb_kmemsize;
	}
	else
	{
            params->u.mem.size = bd->bi_dram[i].size;
            printf("(yjdbg) bank: %d, size: 0x%x\n", i, params->u.mem.size);
	}
        printf("bank %d: start = 0x%x size = 0x%x \n",i, params->u.mem.start,params->u.mem.size);
		params = tag_next (params);
	}
}
#endif /* CONFIG_SETUP_MEMORY_TAGS */


static void setup_commandline_tag (bd_t *bd, char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}


#ifdef CONFIG_INITRD_TAG
static void setup_initrd_tag (bd_t *bd, ulong initrd_start, ulong initrd_end)
{
	/* an ATAG_INITRD node tells the kernel where the compressed
	 * ramdisk can be found. ATAG_RDIMG is a better name, actually.
	 */
	params->hdr.tag = ATAG_INITRD2;
	params->hdr.size = tag_size (tag_initrd);

	params->u.initrd.start = initrd_start;
	params->u.initrd.size = initrd_end - initrd_start;

	params = tag_next (params);
}
#endif /* CONFIG_INITRD_TAG */

#ifdef CONFIG_SERIAL_TAG
void setup_serial_tag (struct tag **tmp)
{
	struct tag *params = *tmp;
	struct tag_serialnr serialnr;
	void get_board_serial(struct tag_serialnr *serialnr);

	get_board_serial(&serialnr);
	params->hdr.tag = ATAG_SERIAL;
	params->hdr.size = tag_size (tag_serialnr);
	params->u.serialnr.low = serialnr.low;
	params->u.serialnr.high= serialnr.high;
	params = tag_next (params);
	*tmp = params;
}
#endif

#ifdef CONFIG_REVISION_TAG
void setup_revision_tag(struct tag **in_params)
{
	u32 rev = 0;
	u32 get_board_rev(void);

	rev = get_board_rev();
	params->hdr.tag = ATAG_REVISION;
	params->hdr.size = tag_size (tag_revision);
	params->u.revision.rev = rev;
	params = tag_next (params);
}
#endif  /* CONFIG_REVISION_TAG */

static void setup_end_tag (bd_t *bd)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}
#endif /* CONFIG_SETUP_MEMORY_TAGS || CONFIG_CMDLINE_TAG || CONFIG_INITRD_TAG */

static ulong get_sp(void)
{
	ulong ret;

	asm("mov %0, sp" : "=r"(ret) : );
	return ret;
}

void display_boottime_log(void)
{
	char*	timelog = getenv("timelog");
	

	if(!strcmp("on", timelog))
	{
		enable_console();
	    {
			UINT32 TimeSpendOnLoader = (0xFFFFFFFF - BIM_GetTimeLog(2));
			UINT32 regval = (0xFFFFFFFF -__raw_readl(0xf0008000+ REG_RW_TIMER2_LOW));
			UINT32 regc = regval - TimeSpendOnLoader;
			printf("\033[0;31m[time] loader takes %dms, uboot takes %d.%03dms \033[0m\n", TimeSpendOnLoader/24000,(regc)/24000,(regc/24)%1000);
	    }
		disable_console();
	}
}

