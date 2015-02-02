
#include <malloc.h>
#include <common.h>
#include <command.h>

#include <debug.h>
#include <string.h>
#include <util.h>
#include <partinfo.h>
#include <mmc.h>
#include <linux/ctype.h>


#define EMMC_TEST_LOG 		1

typedef int (*SUB_FUNC_T)(int argc, char *argv[]);

extern unsigned int random(void);

static u32  emmc_test_transfer(void *buff, u32 offset, u32 size, u32 write , u32 alert_speed, u32 *time)
{
	u32 tick, elapsed, speed;;
	int rc;

	tick = readUsTicks();

	if (write)
		rc = storage_write(offset, size, (void*)buff);
	else
		rc = storage_read(offset, size, (void*)buff);
	elapsed = readUsTicks() - tick;

//	if(elapsed < 1000) speed = (size/1024)*1000*1000 / elapsed;
//	else speed = (size/1024)*1000 / (elapsed/1000);

	speed = (size/1024)*1000 * 100 /(elapsed/10);
	*(u32*)time = elapsed;

	#if (EMMC_TEST_LOG > 0)
	printf("0x%08X(0x%08X) - %8ld bytes %s: %s. %8ldus elapsed, %8ldKbps/sec\n",
			offset, size + offset, size, write > 0 ? "write" : "read ", rc < 0 ? "ER" : "OK", elapsed, speed);
	#endif

	if (speed < alert_speed)
	{
		printf("0x%08X(0x%08X) - %7ld bytes %s: %s. %3ld.%3ldms elapsed, %7ldKbps/sec\n",
			offset, size + offset, size, write > 0 ? "write" : "read ", rc < 0 ? "ER" : "OK",
			elapsed/1000, elapsed%1000, speed);
	}

	return speed;
}

static int subcmd_emmc_erase(int argc, char *argv[])
{
	u64		size, offset;
	int		rc;

	struct mmc	*mmc = find_mmc_device(CONFIG_SYS_MMC_ENV_DEV);

	if(!mmc)
	{
		printf("No mmc device at slot %x\n", CONFIG_SYS_MMC_ENV_DEV);
		return -1;
	}

	if(argc == 2)
	{
		int in_char;
		printf("Really erase all the data in emmc ? <y/N> ");
		in_char = getc();
		printf("%c\n\n", isprint(in_char)? in_char : ' ');

		if(in_char != 'y')
			return 0;

		offset = 0;
		size = mmc->capacity;
	}
	else
	{
		if(get_offset_size2(argc - 2, argv + 2, &offset, &size) < 0)
			return -1;

		if(size == 0)
		{
			printf("invalid size\n");
			return -1;
		}
	}

	printf("emmc erase (0x%012llx ~ 0x%012llx) : ", offset, offset + size);
	rc = storage_erase(offset, size);

	printf("%s\n", (rc == 0)? "OK" : "Fail");

	return 0;
}

static int subcmd_emmc_dump(int argc, char *argv[])
{
	ulong	size = 0, offset = 0, dst = CFG_LOAD_ADDR;
	int		ret = 0;
	char	cmd[20] = {0, };

	if(argc != 3 && argc != 4)
		return -1;

	if(get_offset_size(argc - 2, argv + 2, &offset, &size) < 0)
		return -2;

	if(size == 0)
		size = 512;

	ret = emmc_read(offset, size, dst);
	if(ret) {
		printf("block read failed..\n");
		return -1;
	}

	printf("Offset 0x%x, size: 0x%x, dst: 0x%x, dump:\n", offset, size, CFG_LOAD_ADDR);

	sprintf(cmd, "md.b 0x%x 0x%x\n", dst, size);
	ret = run_command(cmd, 0);

	return ret;
}


static int subcmd_emmc_test(int argc, char *argv[])
{
	static char		*emmc_test_buf;
	//static int		emmc_test_buf_inited = 0;
	char			*endptr;
	u32				i, loop;
	u32				rand_limit;

	u32				e_start;
	u32				e_size;
	u32				e_chunk;
	u32				e_write;

	u32				speed= 0;
	u32				r_speed = 0, r_speed_min = 0xFFFFFFFF, r_speed_max = 0;
	u32				w_speed = 0, w_speed_min = 0xFFFFFFFF, w_speed_max = 0;
	u32				r_count = 0;
	u32				w_count = 0;
	u32				alert_speed;
	u32				chunk_sz;
	struct mmc pemmc;
	u64				offset, size;

	u32				time = 0;
	u32				r_time_min = 0xFFFFFFFF;
	u32				r_time_max = 0;

	u32				w_time_min = 0xFFFFFFFF;
	u32				w_time_max = 0;
	#if (EMMC_TEST_LOG < 1)
	u32				percent;
	#endif

	printf("subcmd_emmc_test start \n");

	if (argc < 6)
		return -1;

	emmc_test_buf = (char *)0x200000;
	memset(emmc_test_buf, 0xA5, 1024*1024);

#if 0
	for (i=2; i<argc; i++)
	{
		//printf("argv[%u] = %s\n",i,argv[i]);
		if (i >= 3)
		{
			//strtoul(argv[i], &endptr, 0);
			 simple_strtoul(argv[i], &endptr, 16);
			if(*endptr != '\0')
				return -1;
		}
	}
#endif

/**
 *	case1 : sequential read
 *	case2 : sequential read
 *	case3 : random read
 *	case4 : random write
 *	case5 : same address write
 *	case6 : random read & write
 */
	if (argc == 3)
	{
		e_start = 1024 * 1024;
		e_size =  100 * 1024 * 1024;
		e_chunk = 64 * 1024;
	}
	else if (argc == 6)
	{
		e_start =  simple_strtoul(argv[3], &endptr, 16); //strtoul(argv[3], &endptr, 0);
		e_size =  simple_strtoul(argv[4], &endptr, 16);//strtoul(argv[4], &endptr, 0);
		e_chunk =  simple_strtoul(argv[5], &endptr, 16);//strtoul(argv[5], &endptr, 0);
	}
	else
	{
		return -1;
	}

	if((strcmp(argv[2], "case3") == 0) || (strcmp(argv[2], "case4") == 0) || (strcmp(argv[2], "case6") == 0))
	{
		loop = (50*1024*1024)/e_chunk;
		rand_limit = e_size/e_chunk;
	}
	else
	{
		loop = e_size / e_chunk;
		rand_limit = 1024;
	}

	if(strcmp(argv[2], "case1") == 0)	alert_speed = 30*1024;
	if(strcmp(argv[2], "case2") == 0)	alert_speed = 5*1024;
	if(strcmp(argv[2], "case3") == 0)	alert_speed = 1024;
	if(strcmp(argv[2], "case4") == 0)	alert_speed = 20;
	if(strcmp(argv[2], "case5") == 0)	alert_speed = 1024;
	if(strcmp(argv[2], "case6") == 0)	alert_speed = 1024;

	printf("EMMC START ADDR  = 0x%X\n",e_start);
	printf("EMMC TEST  CHUNK = %u Byte\n",e_chunk);
	printf("EMMC TEST  LOOP  = %u\n",loop);
	printf("EMMC TEST  SIZE  = %u Byte\n",e_chunk*loop);
	printf("===============================================================\n");

	if((strcmp(argv[2], "case1") == 0) || (strcmp(argv[2], "case3") == 0))
	{
		e_write = 0;
	}
	else
	{
		e_write = 1;
	}

	//pemmc = get_emmc_info();
	chunk_sz	= pemmc.block_dev.blksz;//pemmc->max_block_size;
	printf("chunk_sz = %d pemmc->block_dev->blksz = %d \n ",chunk_sz,pemmc.block_dev.blksz);
	r_speed		= 0;
	r_speed_min	= 0xFFFFFFFF;
	r_speed_max	= 0;
	w_speed		= 0;
	w_speed_min	= 0xFFFFFFFF;
	w_speed_max = 0;
	r_count		= 0;
	w_count		= 0;

	for (i=0; i < loop ; i++)
	{
		if((strcmp(argv[2], "case1") == 0) || (strcmp(argv[2], "case2") == 0))
		{
			offset = e_start + (i * e_chunk);
		}
		else if((strcmp(argv[2], "case3") == 0) || (strcmp(argv[2], "case4") == 0) || (strcmp(argv[2], "case6") == 0))
		{
			//offset = e_start + (rand_num(rand_limit) * e_chunk);
			offset = e_start + ( (random() % rand_limit) * e_chunk);
		}
		else if(strcmp(argv[2], "case5") == 0)
		{
			offset = e_start;
		}
		else
		{
			return -1;
		}

		if(strcmp(argv[2], "case6") == 0)
		{
			//e_write = rand_num(1024) & 0x1;
			e_write = (random() % 1024) & 0x1;
			
			
		}
		size = e_chunk;

		speed = emmc_test_transfer(emmc_test_buf,offset,size,e_write, alert_speed, &time);
		#if (EMMC_TEST_LOG < 1)
		percent = (i*100)/((e_size / e_chunk));
		printf("Progress = %u\r",percent);
		#endif
		if (e_write == 0)
		{
			r_speed = r_speed + speed;
			r_count++;

			if (speed < r_speed_min)
				r_speed_min = speed;
			if (speed > r_speed_max)
				r_speed_max = speed;

			if (time >= r_time_max)
				r_time_max = time;
			if (r_time_min >= time)
				r_time_min = time;
		}
		else
		{
			w_speed = w_speed + speed;
			w_count++;

			if (speed < w_speed_min)
				w_speed_min = speed;
			if (speed > w_speed_max)
				w_speed_max = speed;

			if (time >= w_time_max)
				w_time_max = time;
			if (w_time_min >= time)
				w_time_min = time;
		}
	}

	if (r_count)
	{
		printf("EMMC READ  Chunk %7d: %2u.%03u %2u.%03u %2u.%03u Mbyte/sec\n",
			chunk_sz, (r_speed/r_count)/1000, (r_speed/r_count)%1000,
			r_speed_min/1000, r_speed_min%1000, r_speed_max/1000, r_speed_max%1000);
	}

	if (w_count)
	{
		printf("EMMC WRITE Chunk %7d: %2u.%03u %2u.%03u %2u.%03u Mbyte/sec\n",
			chunk_sz, (w_speed/w_count)/1000, (w_speed/w_count)%1000,
			w_speed_min/1000, w_speed_min%1000, w_speed_max/1000, w_speed_max%1000);
	}

	return 0;
}



static SUB_FUNC_T cmp_subcmd(char *str)
{
	typedef struct
	{
		char		*cmd;
		SUB_FUNC_T	func;
	} SUBCMD_T;

	SUBCMD_T cmd_tbl[] = {
		{ "erase", subcmd_emmc_erase   },
		{ "dump", subcmd_emmc_dump   },
		{ "test", subcmd_emmc_test   },
		{  NULL }
	};

	int		index;

	if (str == NULL)
		return NULL;

	for (index = 0; ; index++)
	{
		if (cmd_tbl[index].cmd == NULL)
			return NULL;

		if (!strcmp(cmd_tbl[index].cmd, str))
			break;
	}

	return (SUB_FUNC_T)cmd_tbl[index].func;
}


int cmd_emmc(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]) // should be used integer type function
{
	SUB_FUNC_T	func;

	if (argc < 2)
		goto usage;

	func = cmp_subcmd(argv[1]);

	if (func == NULL)
		goto usage;

	if(func(argc, argv) == 0)
		return 0;

usage:
	printf ("Usage in cmd_emmc :\n%s\n", cmdtp->usage);
	return -1;
	
}

U_BOOT_CMD(
     emmc,   6,  0,  cmd_emmc,
     "emmc dump, erase, test\n",
	 "emmc erase offset|partition size\n"
	 "emmc dump offset|partition size\n"
	 "emmc test case1-6 start size chunk \n"
);

