#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

#ifdef CC_EMMC_BOOT
#include <mmc.h>
#else
#include <nand.h>
#endif
#include <partinfo.h>

#include <exports.h>
#include "bgtask_powseq.h"

#undef DEBUG
#ifdef DEBUG
#define dprintf(fmt, args...)	printf("%s: " fmt, __FUNCTION__, ## args)
#else
#define dprintf(fmt, args...)
#endif	/* DEBUG */

//#undef	PARALLEL_DECOMP
#define	PARALLEL_DECOMP
#define	FS_HDR_SIZE			512
#define CRMFS_HDR           "Compressed ROMFS"

typedef enum {
	LZX_TYPE_LZO = 0,
	LZX_TYPE_LZ4,
	LZX_TYPE_UNKNOWN
} LZX_TYPE_T;

int do_read_and_decomp(int type, unsigned int src, unsigned int addr, unsigned int decomp_dst, unsigned int len);
extern int lzo_compare_magic(unsigned char *buf);
extern int lzo_get_destsize(unsigned char * buf);

extern int unlzo_get_hdroffset(u8 *input, int in_len);
extern int unlz4_get_hdroffset(u8 *input, int in_len);

extern int lzo_do_decomp(unsigned char * pDecomp, unsigned char * pComp, unsigned long * pDecompSize);
extern int lz4_do_decomp(unsigned char * pDecomp, unsigned char * pComp, unsigned long * pDecompSize);

extern int unlzo_read(u8 *input, int in_len, int (*fill) (void *fill_data), void *fill_data, u8 *output, int *posp);
extern int unlz4_read(u8 *input, int in_len, int (*fill) (void *fill_data), void *fill_data, u8 *output, int *posp);

#define	FLASH_BASE			0

#ifdef MTDINFO_IN_FLASH
extern int write_flash_data(uchar *src, ulong addr, ulong cnt);
#else

#endif

ulong appxip_len = 0;	/* app xip length */
ulong fontxip_len = 0;	/* font xip length */

DECLARE_GLOBAL_DATA_PTR;

/* FIXME, 속도 문제로 emmc 는 사용 안됨., 왜일까... */
int read_blocks_with_bg_task (off_t ofs, size_t *len, u_char *buf, void (*bg_task(BGTASK_POWSEQ_FLAG)))
{
	int ret = 0;
	long	tot_size = 0, left = 0;
	ulong	rd_size = 0;
#ifdef CC_EMMC_BOOT
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	struct mmc *emmc = find_mmc_device(devnum);
#endif

	tot_size	= 0;
	left		= len;

#ifdef CC_EMMC_BOOT
	/* hadle unaligned start offset */
	if (ofs & emmc->read_bl_len) {
		rd_size = ofs & emmc->read_bl_len;
		ret = emmc->block_dev.byte_read(devnum, ofs, rd_size, buf);

		if (ret != rd_size) {
			printf("emmc failed..(tot=%ld, rd_size=%ld)\n", tot_size, rd_size);
			return 1;
		}
		buf 		+= rd_size;
		tot_size	+= rd_size;
		left		-= rd_size;
		ofs			+= rd_size;
	}
#endif
	while (tot_size < len)
	{
#ifdef CC_EMMC_BOOT
		if (left > emmc->read_bl_len) {
			rd_size = emmc->read_bl_len;
			ret = emmc->block_dev.block_read(devnum, ofs/rd_size, 1, buf);
			if (ret != 1)
				ret = 0;
		}
		else {
			rd_size = left;
			ret = emmc->block_dev.byte_read(devnum, ofs, rd_size, buf);
		}

		if (ret < 1) {
			printf("emmc failed..(tot=%ld, rd_size=%ld)\n", tot_size, rd_size);
			return 1;
		}
		ofs += rd_size;
#else
		rd_size	= (left < nand_info[0].erasesize)? left : nand_info[0].erasesize;
		ret		= nand_read(&nand_info[0], ofs+tot_size, &rd_size, (u_char *)buf);
		if (ret) {
			printf("nand_read failed..(tot=%ld, rd_size=%ld)\n", tot_size, rd_size);
			return 1;
		}
#endif
		buf 		+= rd_size;
		tot_size	+= rd_size;
		left		-= rd_size;

		if (bg_task)
			bg_task(BGTASK_CONT);
	}

	return 0;
}

extern int gSecureFlag;
int do_xip (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char	*s,	*fshdr;
	char	pname[STR_LEN_MAX] = {0,};
	int		idx = 0, ret = 0;
	ulong	*xiplen = NULL;
	ulong	s_msec, e_msec, src, dst, cp_pos;
	ulong	imgfilesize = 0;
#ifndef CC_EMMC_BOOT
	ulong	tmpfilesize = 0;
#endif
	ulong	decomp_size1 = 0, decomp_size2 = 0;
	struct	partition_info *mpi = NULL;
	static	ulong xip_size = 0;

	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	/* find partition */
	mpi = get_used_partition(argv[1]);
	if(!mpi) {
		printf("Unknown partition name(%s)..\n", pname);
		return 1;
	}

	imgfilesize = mpi->filesize;
	if (imgfilesize == 0) {
		printf("Partition Image Size Zero Length Case!! ==> Skipped!!\n");
		return 1;
	}

	if (!strcmp(mpi->name, "lgapp")) {
		s = getenv("appxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &appxip_len;

	} else if (!strcmp(mpi->name, "lgfont")) {
		s = getenv("fontxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &fontxip_len;
	}

	/* flash source address */
	src			= FLASH_BASE + mpi->offset;
	cp_pos		= CFG_LOAD_ADDR;
	printf("cp_pos = 0x%x \n",cp_pos);
#if 0
#ifdef CC_EMMC_BOOT
	ret = emmc_read(src, FS_HDR_SIZE, (u_char *)cp_pos);
	if (ret) {
		printf("emmc read failed..\n");
		return 1;
	}
#else
	tmpfilesize = FS_HDR_SIZE;
	ret		= nand_read(&nand_info[0], src, &tmpfilesize, (u_char *)cp_pos);
	if (ret) {
		printf("nand read failed..\n");
		return 1;
	}
#endif
#else
	ret = storage_read(src, FS_HDR_SIZE, (u_char *)cp_pos);
	if (ret) {
		printf("storage read failed..\n");
		return 1;
	}
#endif

	if (!lzo_compare_magic((unsigned char *)cp_pos))
		goto do_lzo;
	else
		goto do_cramfs;

do_lzo :

	s_msec	= readMsTicks();


#ifdef CC_EMMC_BOOT
	emmc_read(src,imgfilesize, cp_pos);
#else
//	ret = read_blocks_with_bg_task (src, imgfilesize, cp_pos, BG_PowerSeqTask);
	ret = read_blocks_with_bg_task (src, imgfilesize, cp_pos, NULL);
#endif
	if (ret) {
		printf("block read failed..\n");
		return 1;
	}
#ifdef CC_A1_SECURE_BOOT
	if (!strcmp(mpi->name, "lgapp"))
	{
		printf("[%d]:lgapp verification start \n", readMsTicks());
		verifyPartition(mpi->name,0,cp_pos);
		polling_timer();
		if( !strcmp("lgapp",mpi->name) )
			gSecureFlag |= SECURE_FLG_LGAPP;
		printf("[%d]:lgapp verification end \n", readMsTicks());
	}
#endif

	//EPI On
	//BootSplash();

	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, imgfilesize / (e_msec-s_msec));

	decomp_size1 = lzo_get_destsize((unsigned char *)cp_pos);
	if (decomp_size1 == 0) {
		printf("decompressed size is zero at LZO header\n");
		return 1;
	}

	*xiplen		 = (decomp_size1 & 0xFFF00000) + 0x100000;
	xip_size	+= *xiplen;

	polling_timer();

	dst = APPXIP_LOADADDR;
	s_msec	= readMsTicks();
	printf("[%4d] [Lzo Img] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, imgfilesize, (unsigned int)src, (unsigned int)dst);
	lzo_do_decomp((u_char *)dst, (u_char *)cp_pos, &decomp_size2);
	if (decomp_size2 != decomp_size1) {
		printf("decompressed size=%d is different with %d\n", (int)decomp_size2, (int)decomp_size1);
		return 1;
	}
	e_msec	= readMsTicks();
	printf("[%4d] [Lzo Img]...done (%ld msec)\n", (int)e_msec, e_msec-s_msec);
	
//	BootSplash();
	polling_timer();

	return 0;

do_cramfs :

	fshdr = (char *)cp_pos;

	//EPI On
	//BootSplash();
	polling_timer();
	if (!strncmp(&fshdr[16], CRMFS_HDR, 15)) {
		*xiplen = (imgfilesize & 0xFFF00000) + 0x100000;

		/* set xipfs_len in MB */
		xip_size += *xiplen;

		dst = APPXIP_LOADADDR;
		s_msec  = 0; //get_cur_time();
		printf("[%4d] [Cramfs Img] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", (int)s_msec, mpi->name, imgfilesize, (unsigned int)src, (unsigned int)dst);

#ifdef CC_EMMC_BOOT
		emmc_read(src,imgfilesize, dst);
#else
	//	ret = read_blocks_with_bg_task (src, imgfilesize, dst, BG_PowerSeqTask);
		ret = read_blocks_with_bg_task (src, imgfilesize, cp_pos, NULL);
#endif
		if (ret) {
			printf("block read failed..\n");
			return 1;
		}

		e_msec  = 0; //get_cur_time();
//		printf("[%4d] ...done (%ld KB/s)\n", (int)e_msec, imgfilesize / (e_msec-s_msec));
		printf("[%4d] ...done\n", (int)e_msec);
		//Inverter On
		//BootSplash();
	}
	else
	{
		//Inverter On
		//BootSplash();
		polling_timer();
		printf("[%s] is not xip image!\n", mpi->name);
		return 0;
	}

	return 0;
}

U_BOOT_CMD(
	xip,	2,	0,	do_xip,
	"xip\t- copy to ram for xip\n",
	"xip [lgapp | lgfont]\n"
);

int do_xipz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char	*s,	*fshdr;
	char	pname[STR_LEN_MAX] = {0,};
	int		idx = 0, ret = 0, ret1;
	ulong	*xiplen = NULL;
	ulong	s_msec, e_msec, src, dst, cp_pos, len;
#ifndef CC_EMMC_BOOT
	ulong	tmpfilesize = 0;
#endif
	ulong	decomp_size1 = 0, decomp_size2 = 0;
	struct	partition_info *mpi = NULL;
	static	ulong xip_size = 0;

	if (argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	polling_timer();
	/* find partition */
	mpi = get_used_partition(argv[1]);
	if(!mpi) {
		printf("Unknown partition name(%s)..\n", pname);
		return 1;
	}

	len = mpi->filesize;
	if (len == 0) {
		printf("Partition Image Size Zero Length Case!! ==> Skipped!!\n");
		return 1;
	}

	if (!strcmp(mpi->name, "lgapp")) {
		s = getenv("appxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &appxip_len;

	} else if (!strcmp(mpi->name, "lgfont")) {
		s = getenv("fontxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &fontxip_len;
	}
	polling_timer();
	/* flash source address */
	src			= FLASH_BASE + mpi->offset;
	cp_pos		= CFG_LOAD_ADDR;
	printf("cp_pos = 0x%x \n",cp_pos);

	ret = storage_read(src, FS_HDR_SIZE, (u_char *)cp_pos);
	if (ret) {
		printf("storage read failed..\n");
		return 1;
	}

	//BootSplash();

	decomp_size1 = unlzo_get_decompsize((unsigned char *)cp_pos);
	if (decomp_size1 == 0) {
		printf("decompressed size is zero at LZO header\n");
		return 1;
	}
	printf("decomp_size1 = 0x%x \n",decomp_size1);
	*xiplen		 = (decomp_size1 & 0xFFF00000) + 0x100000;
	xip_size	+= *xiplen;
	dst = APPXIP_LOADADDR;

	polling_timer();
#ifdef	PARALLEL_DECOMP
	s_msec = readMsTicks();
	printf("[%4d] Do XIP \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)cp_pos);

#ifndef WEBOS_DEVELOPMENT
	if (!strcmp(mpi->name, "lgapp"))
	{
		printf("hongjun return it when lgapp!!!!!\n");
		return 0;
	}
#endif
	ret = do_read_and_decomp(LZX_TYPE_LZO, src, cp_pos, dst, len);
	if (ret != 0)
	{
        do
        {
            if(ret == 2)
            {
                ret1 = emmc_async_read_stop(0, 0);
            }
			else
			{
                ret1 = emmc_async_read_stop(0, 1);
			}
            if(ret1 != -1)
			    ret = do_read_and_decomp(LZX_TYPE_LZO, src, cp_pos, dst, len);
        }while((ret1 != -1) && (ret != 0));

		if(ret != 0)
		{
		    printf("simultaneous read and decomp fail\n");
		    return 1;
		}
	}

	polling_timer();
	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));
#else
	s_msec = readMsTicks();
	printf("[%4d] Load \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)cp_pos);

	ret = emmc_read(src, len, cp_pos);
	if (ret)	{ printf("emmc read failed..\n");	return 1; }

	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));

	//EPI On
	//BootSplash();

	polling_timer();
	s_msec	= readMsTicks();
	printf("[%4d] LZO decomp (%ld bytes) from 0x%08x to 0x%08x\n", s_msec, len, (unsigned int)cp_pos, (unsigned int)dst);

	lzo_do_decomp((u_char *)dst, (u_char *)cp_pos, &decomp_size2);
	if (decomp_size2 != decomp_size1) {
		printf("decompressed size=%d is different with %d\n", (int)decomp_size2, (int)decomp_size1);
		return 1;
	}
	e_msec	= readMsTicks();
	printf("[%4d] LZO decomp ar 0x%08x (%ld bytes)...done (%ld msec)\n", (int)e_msec, (unsigned int)dst, decomp_size2, e_msec-s_msec);
#endif

	#ifdef CC_A1_SECURE_BOOT
	if (!strcmp(mpi->name, "lgapp"))
	{
		printf("[%d]:lgapp verification start \n", readMsTicks());
#ifdef SIGN_USE_PARTIAL
		if( getFullVerifyOTP() )
			verifyPartition(mpi->name, cp_pos,1);		
		else
			verifyPartialPartition(mpi->name, cp_pos, 1);	
#else
		verifyPartition(mpi->name,cp_pos,1);
#endif
		if( !strcmp("lgapp",mpi->name) )
			gSecureFlag |= SECURE_FLG_LGAPP;
		printf("[%d]:lgapp verification end \n", readMsTicks());
	}
	#endif

	//BootSplash();
	polling_timer();
	return 0;

}

U_BOOT_CMD(
	xipz,	2,	0,	do_xipz,
	"xip\t- copy to ram for xip\n",
	"xip [lgapp | lgfont]\n"
);

int do_cp2ram (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char pname[STR_LEN_MAX]={0,};
	int	idx = 0, ret = 0;
	ulong src, dst, len = 0, addr = 0,offset = 0;
	ulong rd_size = 0;
	long  tot_size = 0, left = 0;
	unsigned int s_msec, e_msec;
	struct partition_info *mpi = NULL;

	if (argc == 3 || argc == 5) {
		strcpy(pname, argv[1]);
		addr = simple_strtoul(argv[2], NULL, 16);
	}
	else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	polling_timer();

	/* find partition */
	do {
		mpi = GET_PART_INFO(idx);
		if(mpi->used && mpi->valid && (strcmp(pname, mpi->name) == 0))
		{
			break;
		}
	} while (++idx < PARTITION_MAX);

	if (idx == PARTITION_MAX) {
		printf("Unknown partition name(%s)..\n", pname);
		return 1;
	}

	if (argc == 3) {
        offset = mpi->offset;
	    len = mpi->filesize;
	}
	else {
	    offset = mpi->offset + simple_strtoul(argv[3], NULL, 16);
	    len = simple_strtoul(argv[4], NULL, 16);
	}

	if (len == 0) {
		printf("Partition Image Size Zero Length Case!! ==> Skipped!!\n");
		return 1;
	}

	if ((offset + len) > (mpi->offset + mpi->size)) {
		printf("Exceed partition size!! ==> Skipped!!\n");
		return 1;
	}

	src = (FLASH_BASE + offset);
	dst = addr;

	polling_timer();
	s_msec = readMsTicks();
	printf("[%4d] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)dst);

#ifdef MTDINFO_IN_FLASH
	memcpy((void *)dst, (void *)src, len);
#else
	#ifdef CC_EMMC_BOOT
//	printf("emmc read start \n");
	ret = emmc_read(offset, len, dst);
//	printf("emmc read end \n");
	#else
	ret = read_blocks_with_bg_task (offset, len, dst, NULL);
	#endif
	if (ret) {
		printf("block read failed..\n");
		return 1;
	}
#endif
	polling_timer();
	e_msec = readMsTicks();
	printf("[%4d] ...done (%ld KB/s)\n", (int)e_msec, len / (e_msec-s_msec));
	
	if (strcmp(pname, "lginit") == 0)
		//BootSplash();
		polling_timer();
	return 0;
}

/* ====================================================================== */
U_BOOT_CMD(
	cp2ram,	3,	0,	do_cp2ram,
	"cp2ram\t- copy to ram for partition name\n",
	"cp2ram [partition name] [dest address]\n"
);

#define EMMC_ASYNC_SIZE (64*1024)

struct emmc_fill_data
{
        int cmd_data_left;
        int recv_data_left;
};
extern int emmc_fill(struct emmc_fill_data *ef);
#if 0
//defined in cmd_mmc.c
int emmc_fill(struct emmc_fill_data *ef)
{
	int size, recv_size, ret;

	if (!ef->recv_data_left)
		return -1;

	// Wait for previous block complete.
	recv_size = size = (ef->recv_data_left < EMMC_ASYNC_SIZE) ? ef->recv_data_left : EMMC_ASYNC_SIZE;
	ret = emmc_async_dma_wait_finish();

	if (ret)
	{
		printf("emmc_async_dma_wait_finish %d ret %d\n", size, ret);
		return -1;
	}

	ef->recv_data_left -= size;

	// Now send command for next.
	if (ef->cmd_data_left)
	{
		size = (ef->cmd_data_left < EMMC_ASYNC_SIZE) ? ef->cmd_data_left : EMMC_ASYNC_SIZE;
		ret = emmc_async_dma_start_trigger(size);
		if (ret)
			printf("emmc_async_dma_start_trigger %d ret %d\n", size, ret);
		ef->cmd_data_left -= size;
	}

	return recv_size;
}
#endif
int do_read_and_decomp(int type, unsigned int src, unsigned int addr, unsigned int decomp_dst, unsigned int len)
{
	int	ret = 0, dout;
	ulong dst, dma_size0, dma_size1;
	ulong rd_size = 0;
	long  tot_size = 0, left = 0;
	struct emmc_fill_data fdata;
	int hdroffset;

	int (*unlzx_get_hdroffset)(u8*, int);
	int (*unlzx_read)(u8*, int, int (*)(void *), void *, u8*, int*);

	dprintf(" type(%d), src(%x), addr(%x), decomp_dst(%x), len(%x)\n", type, src, addr, decomp_dst, len);
	
	switch (type) {
	case LZX_TYPE_LZO:
		{
			unlzx_get_hdroffset = unlzo_get_hdroffset;
			unlzx_read = unlzo_read;
		} break;
	case LZX_TYPE_LZ4:
		{
			unlzx_get_hdroffset = unlz4_get_hdroffset;
			unlzx_read = unlz4_read;
		} break;
	default:
		printf("unknown lzx type %d\n", type);
		return (-1);
	}
	
	dst = addr;

	// align to 512 bytes
	len = (len + 511) & (~511);

	dma_size0 = (len < EMMC_ASYNC_SIZE) ? len : EMMC_ASYNC_SIZE;
	fdata.recv_data_left = fdata.cmd_data_left = len - dma_size0;

	ret = emmc_async_read_setup(0, src, len, addr);
	if (ret)
	{
		printf("emmc_async_read_setup done %d\n", ret);
		return 2;
	}

	// Read first block, must wait for first block
	ret = emmc_async_dma_start_trigger(dma_size0);
	ret = emmc_async_dma_wait_finish();
	if(ret)
	{
        printf("emmc_async_dma_wait_finish done %d\n", ret);
		return 3;
	}

//	if(type == LZX_TYPE_LZ4)
//		hexdump((u8*)addr,dma_size0);
		
	// Now, read next, don't wait.
	if (fdata.cmd_data_left)
	{
		dma_size1 = (fdata.cmd_data_left < EMMC_ASYNC_SIZE) ? fdata.cmd_data_left : EMMC_ASYNC_SIZE;
		ret = emmc_async_dma_start_trigger(dma_size1);
		fdata.cmd_data_left -= dma_size1;
	}

	hdroffset = unlzx_get_hdroffset(addr, dma_size0);
	if (hdroffset < 0)
	{
		printf("can't find lzop hdr\n");
		return 1;
	}

	ret = unlzx_read(addr+hdroffset, dma_size0-hdroffset, emmc_fill, &fdata, decomp_dst, &dout);
	if (ret) {
		printf("block unlzo failed.. %d\n", ret);
		return 1;
	}

	HalFlushInvalidateDCache();

	// Send finish command.
	ret = emmc_async_read_finish(0);
	if (ret)
		printf("emmc_async_read_finish done %d\n", ret);

	return 0;
}

int do_cp2ramz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char pname[STR_LEN_MAX]={0,};
	int	idx = 0, ret = 0, dout, ret1;
	ulong src, dst, len = 0, addr = 0, offset = 0, decomp_dst = 0, dma_size0, dma_size1;
	ulong rd_size = 0, decomp_size2 = 0;
	long  tot_size = 0, left = 0;
	unsigned int s_msec, e_msec;
	struct partition_info *mpi = NULL;
	struct emmc_fill_data fdata;
	int lzx_type, hdroffset;

	int (*lzx_do_decomp)(unsigned char*, unsigned char*, unsigned long*);

	if(strncmp(argv[1], "lzo", 3) == 0)
	{
		lzx_type = LZX_TYPE_LZO;
		lzx_do_decomp = lzo_do_decomp;
	}
	else if(strncmp(argv[1], "lz4", 3) == 0)
	{
		lzx_type = LZX_TYPE_LZ4;
		lzx_do_decomp = lz4_do_decomp;
	}
	else
	{
		lzx_type = LZX_TYPE_UNKNOWN;
		lzx_do_decomp = NULL;
	}
	/* cp2ramz lzo kernel 0x7000000 0x7FC0;verification 0 kernel 0x7000000;xiplz4 lgapp;bootm 0x7FC0 */
	/* 0          1    2       3             4 */
	if (argc == 5) {
		strcpy(pname, argv[2]);
		addr = simple_strtoul(argv[3], NULL, 16);
		decomp_dst = simple_strtoul(argv[4], NULL, 16);
	}
	else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	polling_timer();

	/* find partition */
	do {
		mpi = GET_PART_INFO(idx);
		if(mpi->used && mpi->valid && (strcmp(pname, mpi->name) == 0))
		{
			break;
		}
	} while (++idx < PARTITION_MAX);

	if (idx == PARTITION_MAX) {
		printf("Unknown partition name(%s)..\n", pname);
		return 1;
	}

	if (argc == 5) {
        offset = mpi->offset;
	    len = mpi->filesize;
	}
	else {
	    offset = mpi->offset + simple_strtoul(argv[3], NULL, 16);
	    len = simple_strtoul(argv[4], NULL, 16);
	}

	if (len == 0) {
		printf("Partition Image Size Zero Length Case!! ==> Skipped!!\n");
		return 1;
	}

	if ((offset + len) > (mpi->offset + mpi->size)) {
		printf("Exceed partition size!! ==> Skipped!!\n");
		return 1;
	}

	//BootSplash();
	polling_timer();
	src = (FLASH_BASE + offset);
	dst = addr;

#ifdef PARALLEL_DECOMP
	s_msec = readMsTicks();
	printf("[%4d] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)dst);

	ret = do_read_and_decomp(lzx_type, src, dst, decomp_dst, len);
	if (ret != 0)
	{
        do
        {
            if(ret == 2)
            {
                ret1 = emmc_async_read_stop(0, 0);
            }
			else
			{
                ret1 = emmc_async_read_stop(0, 1);
			}
            if(ret1 != -1)
			    ret = do_read_and_decomp(lzx_type, src, dst, decomp_dst, len);
        }while((ret1 != -1) && (ret != 0));

		if(ret != 0)
		{
		    printf("simultaneous read and decomp fail\n");
		    return 1;
		}
	}
	polling_timer();
	e_msec = readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));
#else
	s_msec = readMsTicks();
	printf("[%4d] Copy \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)dst);

	ret = emmc_read(src, len, dst);
	if (ret)	{ printf("block read failed..\n");	return 1; }

	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));

	printf("[%4d] LZX decomp (%ld bytes) from 0x%08x to 0x%08x\n", e_msec, len, (unsigned int)dst, (unsigned int)decomp_dst);
	if(lzx_do_decomp)
		lzx_do_decomp((u_char *)decomp_dst, (u_char *)dst, &decomp_size2);

	s_msec	= readMsTicks();
	printf("[%4d] LZX decomp (%ld bytes)...done (%ld msec)\n", (int)s_msec, decomp_size2, s_msec-e_msec);
#endif
	polling_timer();
	return 0;
}

/* ====================================================================== */
U_BOOT_CMD(
	cp2ramz,	5,	0,	do_cp2ramz,
	"cp2ramz\t- copy to ram for partition name with lzo decompress\n",
	"cp2ramz [lzx type] [partition name] [read dest address] [decompress dest address]\n"
);

int do_tick (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	printf("tick = %d\n", readMsTicks());
	return 0;
}

U_BOOT_CMD(
	tick,	1,	0,	do_tick,
	"tick\t- get ticks\n",
	"tick\n"
);


int do_emmc_dump (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int	ret = 0;
	ulong src, dst, len = 0;
	char cmd[20];

	printf("argc %d", argc);
	if (argc == 3) {
		src = simple_strtoul(argv[1], NULL, 16);
		len = simple_strtoul(argv[2], NULL, 16);
	}
	else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	dst = 0x2000000;
	printf("src address : 0x%x size : 0x%x\n", src, len);

	ret = emmc_read(src, len, dst);
	if (ret) {
		printf("block read failed..\n");
		return 1;
	}
	
	sprintf(cmd,"md.w 0x%x %d\n", dst, len);
	run_command(cmd, 0);
	return 0;
}

/* ====================================================================== */
U_BOOT_CMD(
	emmcdump,	3,	0,	do_emmc_dump,
	"emmcdump\t- copy to ram form emmc and display\n",
	"emmcdump [emmc address] [size]\n"
);


extern unsigned int unlz4_get_decompsize(unsigned char * buf);
extern int lz4_do_decomp(unsigned char * pDecomp, unsigned char * pComp, unsigned long * pDecompSize);

int do_xiplz4 (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	char	*s,	*fshdr;
	char	pname[STR_LEN_MAX] = {0,};
	int		idx = 0, ret = 0, ret1;
	ulong	*xiplen = NULL;
	ulong	s_msec, e_msec, src, dst, cp_pos, len;
#ifndef CC_EMMC_BOOT
	ulong	tmpfilesize = 0;
#endif
	ulong	decomp_size1 = 0, decomp_size2 = 0;
	struct	partition_info *mpi = NULL;
	static	ulong xip_size = 0;
	ulong	runVerify;
#ifndef WEBOS_DEVELOPMENT
	printf("hongjun do_xiplz4 return 0 directly\n");
	return 0;
#endif
	polling_timer();
	
	if (argc == 2) {
		mpi = get_used_partition(argv[1]);
		runVerify = 1;
	} else if (argc == 3) {
		mpi = get_used_partition(argv[1]);
		runVerify = simple_strtoul(argv[2], NULL, 10);
		printf("runVerify = %ld\n", runVerify);
	} else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
	
	/* find partition */
	mpi = get_used_partition(argv[1]);
	if(!mpi) {
		printf("Unknown partition name(%s)..\n", pname);
		return 1;
	}

	len = mpi->filesize;
	if (len == 0) {
		printf("Partition Image Size Zero Length Case!! ==> Skipped!!\n");
		return 1;
	}

	if (!strcmp(mpi->name, "lgapp")) {
		s = getenv("appxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &appxip_len;

	} else if (!strcmp(mpi->name, "lgfont")) {
		s = getenv("fontxip");
		if (*s == 'n' || s == NULL) {
			return 0;
		}
		xiplen = &fontxip_len;
	}
	polling_timer();
	/* flash source address */
	src			= FLASH_BASE + mpi->offset;
	cp_pos		= CFG_LOAD_ADDR;
	printf("cp_pos = 0x%x \n",cp_pos);

	ret = storage_read(src, FS_HDR_SIZE, (u_char *)cp_pos);
	if (ret) {
		printf("storage read failed..\n");
		return 1;
	}

	//BootSplash();

	decomp_size1 = unlz4_get_decompsize((unsigned char *)cp_pos);
	if (decomp_size1 == 0) {
		printf("decompressed size is zero at LZO header\n");
		return 1;
	}
	printf("decomp_size1 = 0x%x \n",decomp_size1);
	*xiplen		 = (decomp_size1 & 0xFFF00000) + 0x100000;
	xip_size	+= *xiplen;
	dst = APPXIP_LOADADDR;

	polling_timer();
#ifdef	PARALLEL_DECOMP
	s_msec = readMsTicks();
	printf("[%4d] Do XIP \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)cp_pos);
#ifndef WEBOS_DEVELOPMENT
	if (!strcmp(mpi->name, "lgapp"))
	{
		printf("hongjun return it when lgapp!!!!!\n");
		return 0;
	}
#endif

	ret = do_read_and_decomp(LZX_TYPE_LZ4, src, cp_pos, dst, len);
	if (ret != 0)
	{
		do
		{
			if(ret == 2)
			{
				ret1 = emmc_async_read_stop(0, 0);
			}
			else
			{
				ret1 = emmc_async_read_stop(0, 1);
			}
			if(ret1 != -1)
				ret = do_read_and_decomp(LZX_TYPE_LZ4, src, cp_pos, dst, len);
		}while((ret1 != -1) && (ret != 0));

		if(ret != 0)
		{
			printf("simultaneous read and decomp fail\n");
			return 1;
		}
	}

	polling_timer();
	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));
#else

	s_msec = readMsTicks();
	printf("[%4d] Load \"%s (%ld bytes)\" from 0x%08x to 0x%08x\n", s_msec, mpi->name, len, (unsigned int)src, (unsigned int)cp_pos);

	ret = emmc_read(src, len, cp_pos);
	if (ret)	{ printf("emmc read failed..\n");	return 1; }

	e_msec	= readMsTicks();
	printf("[%4d] ...done (%ld msec, %ld KB/s)\n", (int)e_msec, e_msec-s_msec, len / (e_msec-s_msec));

	//EPI On
	//BootSplash();

	polling_timer();
	s_msec	= readMsTicks();
	printf("[%4d] LZ4 decomp (%ld bytes) from 0x%08x to 0x%08x\n", s_msec, len, (unsigned int)cp_pos, (unsigned int)dst);

	lz4_do_decomp((u_char *)dst, (u_char *)cp_pos, &decomp_size2);
	if (decomp_size2 != decomp_size1) {
		printf("decompressed size=%d is different with %d\n", (int)decomp_size2, (int)decomp_size1);
		return 1;
	}
	e_msec	= readMsTicks();
	printf("[%4d] LZ4 decomp ar 0x%08x (%ld bytes)...done (%ld msec)\n", (int)e_msec, (unsigned int)dst, decomp_size2, e_msec-s_msec);
#endif

	#ifdef CC_A1_SECURE_BOOT
	if (runVerify && !strcmp(mpi->name, "lgapp"))
	{
		printf("[%d]:lgapp verification start \n", readMsTicks());
#ifdef SIGN_USE_PARTIAL
		if( getFullVerifyOTP() )
			verifyPartition(mpi->name, cp_pos,1);		
		else
			verifyPartialPartition(mpi->name, cp_pos, 1);	
#else
		verifyPartition(mpi->name,cp_pos,1);
#endif

			gSecureFlag |= SECURE_FLG_LGAPP;

		printf("[%d]:lgapp verification end \n", readMsTicks());
	}
	#endif

	//BootSplash();
	polling_timer();
	return 0;

}

U_BOOT_CMD(
	xiplz4,	2,	0,	do_xiplz4,
	"xiplz4\t- copy to ram for xip\n",
	"xiplz4 [lgapp | lgfont]\n"
);

int do_verify_xip(char * name)
{
	ulong verify_pos	= CFG_LOAD_ADDR;
	
	#ifdef CC_A1_SECURE_BOOT
	if (!strcmp(name, "lgapp"))
	{
		printf("[%d]:xip verification start at 0x%x\n", readMsTicks(), verify_pos);
#ifdef SIGN_USE_PARTIAL
		if( getFullVerifyOTP() )
			verifyPartition(name, verify_pos, 1);		
		else
			verifyPartialPartition(name, verify_pos, 1);	
#else
		verifyPartition(name, verify_pos, 1);
#endif
		gSecureFlag |= SECURE_FLG_LGAPP;

		printf("[%d]:xip verification end \n", readMsTicks());
	}
	#endif

	polling_timer();
	
	return 0;

}

