#include <serial.h>

#include <common.h>
//#include "types.h"
//#include "emmc_core.h"
//#include "emmc_uboot.h"
//#include "emmc_partition_info.h"

//#include "timer.h"
//#include "gic.h"
#include <malloc.h>

#include "mmc.h"
//#include <config.h>
//#include "configs/mt5396.h"

#if 0
unsigned int EMMC_INIT = 0;
unsigned int WriteTime;
unsigned int ReadTime;
unsigned int WriteAddress;
unsigned int ReadAddress;
unsigned int BLOCKSIZE = 0;
unsigned int BLOCKCOUNT = 0;
unsigned int TOTALSIZE = 0;

#define MMC_SECTOR_SIZE	512

struct lge_header emmc_partition_header;
struct lge_endmark emmc_partition_endmark;
struct lge_partition emmc_partition_info[EMMC_PARTITION_MAX];

extern unsigned int EMMC_INTERRUPT;

int uboot_emmc_read(unsigned int buffer, unsigned int size, unsigned int address)
{
#if 1
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	unsigned long n;
	struct mmc *mmc = find_mmc_device(devnum);

	mmc_init(mmc);

	n = mmc->block_dev.byte_read(devnum, address, size, (void *)buffer);

	/* FIXME, flush cache after read */
	flush_cache((ulong)buffer, size); /* FIXME */
	return (n == size) ? 0 : -1;

#else
	unsigned int BlockCount;
	unsigned int BlockSize = MMC_ADMA_BLOCK_MAX;
	unsigned int BlockModsize;
	unsigned int result;
	unsigned int i;

	if (size & (MMC_SECTOR_SIZE - 1))
	{
		EMMC_PRINT("data unit only is 512Byte!\n");
		return EMMC_ERROR_UNITSIZE_FALSE;
	}

	BlockCount = size / MMC_ADMA_BLOCK_MAX;
	BlockModsize = size % MMC_ADMA_BLOCK_MAX;
	if (BlockModsize)
		BlockCount++;

	for (i=0; i<BlockCount; i++)
	{
		if (i < (BlockCount - 1))
		{
			result = read_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), MMC_ADMA_BLOCK_MAX, address + (i*MMC_ADMA_BLOCK_MAX));
			if (result)
			{
				EMMC_PRINT("READ FAIL\n");
				return -1;
			}
		}
		else
		{
			if (BlockModsize)
				result = read_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), BlockModsize, address + (i*MMC_ADMA_BLOCK_MAX));
			else
				result = read_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), MMC_ADMA_BLOCK_MAX, address + (i*MMC_ADMA_BLOCK_MAX));
			if (result)
			{
				EMMC_PRINT("READ FAIL\n");
				return -1;
			}
		}
	}

	return 0;

#endif

}

int uboot_emmc_write(unsigned int buffer, unsigned int size, unsigned int address)
{
#if 1
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	unsigned long n;
	struct mmc *mmc = find_mmc_device(devnum);
	mmc_init(mmc);

	n = mmc->block_dev.byte_write(devnum, address, size, (void *)buffer);

	return (n == size) ? 0 : 1;

#else
	unsigned int BlockCount;
	unsigned int BlockSize = MMC_ADMA_BLOCK_MAX;
	unsigned int BlockModsize;
	unsigned int result;
	unsigned int i;

	if (size & (MMC_SECTOR_SIZE - 1))
	{
		EMMC_PRINT("data unit only is 512Byte!\n");
		return EMMC_ERROR_UNITSIZE_FALSE;
	}

	BlockCount = size / MMC_ADMA_BLOCK_MAX;
	BlockModsize = size % MMC_ADMA_BLOCK_MAX;
	if (BlockModsize)
		BlockCount++;

	for (i=0; i<BlockCount; i++)
	{
		if (i < (BlockCount - 1))
		{
			result = write_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), MMC_ADMA_BLOCK_MAX, address + (i*MMC_ADMA_BLOCK_MAX));
			if (result)
			{
				EMMC_PRINT("WRITE FAIL\n");
				return -1;
			}
		}
		else
		{
			if (BlockModsize)
				result = write_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), BlockModsize, address + (i*MMC_ADMA_BLOCK_MAX));
			else
				result = write_emmc(buffer + (i*MMC_ADMA_BLOCK_MAX), MMC_ADMA_BLOCK_MAX, address + (i*MMC_ADMA_BLOCK_MAX));
			if (result)
			{
				EMMC_PRINT("WRITE FAIL\n");
				return -1;
			}
		}
	}

	return 0;
#endif
}

void emmc_partition_save(unsigned int address)
{
	unsigned int i;
	unsigned char *buffer;
	unsigned char num_c[16];
	unsigned int num;
	unsigned int count = 0;
	unsigned int part_num = 0;
	unsigned int part_count = 0;
	unsigned long total_size = 0;
	unsigned int copy_point = 0;

	buffer = (unsigned char*)address;

	for(i=0; ; i++)
	{
		// partition number
		if (buffer[i] == 'p')
		{
			if (buffer[i+1] != ',')
			{
				part_num = (buffer[i+1] - 0x30);
				i++;
			}
			else
			{
				part_num = ((buffer[i+1] - 0x30) * 10) + (buffer[i+2] - 0x30);
				i++;
				i++;
			}
			i++;
			part_count++;
		}
		else
		{
			break;
		}

		// partition used mark
		emmc_partition_info[part_num-1].used = 1;

		// partition name
		for(;buffer[i]!='"';i++);
		i++;
		copy_point = i;
		for(count=0;buffer[i]!='"';i++,count++);
		memcpy(emmc_partition_info[part_num-1].name,&buffer[copy_point],count);
		i++;

		// start address
		for(;;i++)
		{
			if (((buffer[i]=='0') && (buffer[i+1]=='x')) || ((buffer[i]=='0') && (buffer[i+1]=='X')))
			{
				break;
			}
		}
		copy_point = i;
		for(count=0;buffer[i]!=',';i++,count++);
		memset(num_c,0x0,16);
		memcpy(num_c,&buffer[copy_point],count);
		num = simple_strtoul(num_c, NULL, 16);
		emmc_partition_info[part_num-1].start = num;

		// size
		for(;;i++)
		{
			if (((buffer[i]=='0') && (buffer[i+1]=='x')) || ((buffer[i]=='0') && (buffer[i+1]=='X')))
			{
				break;
			}
		}
		copy_point = i;
		for(count=0;buffer[i]!=';';i++,count++);
		memset(num_c,0x0,16);
		memcpy(num_c,&buffer[copy_point],count);
		num = simple_strtoul(num_c, NULL, 16);
		total_size += num;
		emmc_partition_info[part_num-1].size = num;
		emmc_partition_info[part_num-1].end = emmc_partition_info[part_num-1].start + num - 1;

		for(;buffer[i]!='\n';i++);
	}

	emmc_partition_header.magic_0 = 'L';
	emmc_partition_header.magic_1 = 'G';
	emmc_partition_header.magic_2 = 'E';
	emmc_partition_header.magic_3 = 'P';
	emmc_partition_header.sector_size = 512;
	emmc_partition_header.version = 0x10000;
	emmc_partition_header.size = total_size;
	emmc_partition_header.count = part_count;

	emmc_partition_endmark.magic_0 = 'D';
	emmc_partition_endmark.magic_1 = 'T';
	emmc_partition_endmark.magic_2 = 'V';
	emmc_partition_endmark.magic_3 = '!';

	unsigned char temp[8192];

	memset(temp,0,8192);
	memcpy(temp,&emmc_partition_header,sizeof(emmc_partition_header));
	memcpy(temp+512,&emmc_partition_info,sizeof(emmc_partition_info));
	memcpy(temp+7680,&emmc_partition_endmark,sizeof(emmc_partition_endmark));

	uboot_emmc_write(temp,8192,EMMC_PARTITION_INFO_BASE_ADDRESS);

	printf("=========================================================================\n");
	printf("EMMC Partition Info\n");
	printf("-------------------\n");
	for(i=0; i<EMMC_PARTITION_MAX; i++)
	{
		if (emmc_partition_info[i].used)
		{
			printf("p%u 0x%08X -- 0x%08X : 0x%08X, %s\n",i,(unsigned int)emmc_partition_info[i].start,
				                                           (unsigned int)emmc_partition_info[i].start + (unsigned int)emmc_partition_info[i].size - 1,
				                                           (unsigned int)emmc_partition_info[i].size,
				                                           emmc_partition_info[i].name);
		}
	}
	printf("=========================================================================\n");

	return;
}

int do_emmc_write(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long a;
	unsigned long b;
	unsigned long c;

	ulong timer;
	ulong timer_start;
	ulong speed;

	if (argc == 4)
	{
		a = simple_strtoul(argv[1], NULL, 16);
		b = simple_strtoul(argv[2], NULL, 16);
		c = simple_strtoul(argv[3], NULL, 16);
		timer_start = get_timer(0);
		uboot_emmc_write((unsigned int)a, (int)b, (unsigned int)c);
		timer = get_timer(timer_start);
		speed = (b * 1000) / (unsigned int)timer;
		printf("emmc write time %u, %u KByte\n",timer,speed);
	}
	else
	{
		printf("format error\n");
	}

	return 0;
}

void do_emmc_erase(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int *eBufferP;
	unsigned int eBuffer;
	unsigned int i;
	unsigned int result;
	unsigned char a;

	if (argc == 2)
	{
		a = simple_strtoul(argv[1], NULL, 16);
	}
	else if (argc == 1)
	{
		a = 0x00;
	}


	eBufferP = malloc(32*1024);
	eBuffer = (unsigned int)eBufferP;

	memset(eBufferP,a,32*1204);

	for(i=0; i<32; i++)
	{
		uboot_emmc_write(eBuffer, 32*1024, 32*1024*i);
	}

	EMMC_PRINT("0~1MB Erase to 0x%X\n",a);

	free(eBufferP);
	return 0;
}

int do_emmc_read(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long a;
	unsigned long b;
	unsigned long c;

	ulong timer;
	ulong timer_start;
	ulong speed;

	if (argc == 4)
	{
		a = simple_strtoul(argv[1], NULL, 16);
		b = simple_strtoul(argv[2], NULL, 16);
		c = simple_strtoul(argv[3], NULL, 16);
		timer_start = get_timer(0);
		uboot_emmc_read((unsigned int)a, (int)b, (unsigned int)c);
		timer = get_timer(timer_start);
		speed = (b * 1000) / (unsigned int)timer;
		printf("emmc read time %u us, %u KByte\n",(unsigned int)timer,(unsigned int)speed);
	}
	else
	{
		printf("format error\n");
	}

	return 0;
}

unsigned int do_emmc_init(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	struct mmc *mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc || mmc_init(mmc))
    {
		EMMC_PRINT("EMMC INIT ERROR\n");
		return -1;
	}
	
	return 0;
}

unsigned int do_emmc_partition_save(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int result;
	unsigned long a;

	if (argc != 2)
	{
		printf("Worng Arg\n");
		return;
	}

	a = simple_strtoul(argv[1], NULL, 16);
	emmc_partition_save(a);

	return;
}


U_BOOT_CMD(
	epsave, 2, 0, do_emmc_partition_save,
	"epsave - emmc partition info save\n",
	"\n\t - EMMC 0ffset 1MByte"
);


U_BOOT_CMD(
	eerase, 1, 0, do_emmc_erase,
	"eerase - erase emmc\n",
	"\n\t - EMMC 0~1MByte fill '0'"
);


U_BOOT_CMD(
	einit, 1, 0, do_emmc_init,
	"einit - init_emmc\n",
	"\n\t - EMMC Initialize"
);


U_BOOT_CMD(
	er, 4, 0, do_emmc_read,
	"er - emmc_read_image\n",
	"<mem> <start> <size>"
	"\n\t - EMMC read image"
);


U_BOOT_CMD(
	ew, 4, 0, do_emmc_write,
	"ew - emmc_write_image\n",
	"<mem> <start> <size>"
	"\n\t - EMMC write image"
);
#endif

static uint8_t *emmc_buf = (uint8_t *)0x2000000;

int emmc_init(void)
{
	struct mmc *mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if (!mmc) 
    {
		puts("no mmc devices available\n");
		return -1;
	}
	
	return mmc_init(mmc);
}

int emmc_write(off_t ofs, size_t len, u_char *buf)
{
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	unsigned long n;
	struct mmc *mmc = find_mmc_device(devnum);
	mmc_init(mmc);

	n = mmc->block_dev.byte_write(devnum, (unsigned long)ofs, (unsigned long)len, (void *)buf);

	return (n == len) ? 0 : 1;
}

int emmc_read(off_t ofs, size_t len, u_char *buf)
{
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	unsigned long n;
	struct mmc *mmc = find_mmc_device(devnum);

	mmc_init(mmc);

	n = mmc->block_dev.byte_read(devnum, ofs, len, (void *)buf);

	/* FIXME, flush cache after read */
	flush_cache((ulong)buf, len); /* FIXME */
	return (n == len) ? 0 : -1;

}

int emmc_erase(off_t ofs, size_t len)
{
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	struct mmc *mmc = find_mmc_device(devnum);
	mmc_init(mmc);
	return mmc_erase(mmc, (unsigned long)ofs, (unsigned long)len, 0);
}

int emmc_erase_trim(off_t ofs, size_t len)
{
	int	devnum = CONFIG_SYS_MMC_ENV_DEV;
	struct mmc *mmc = find_mmc_device(devnum);
	mmc_init(mmc);
	return mmc_erase(mmc, (unsigned long)ofs, (unsigned long)len, 1);
}

