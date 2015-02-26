#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <exports.h>

#ifdef CONFIG_HIBERNATION
#include <cmd_resume.h>
#endif

#ifdef CC_EMMC_BOOT
#include <mmc.h>
#endif

#include <xyzModem.h>
#include <net.h>
#include <partinfo.h>
#include <swu.h>

#include <cmd_resume.h>

#define	FLASH_BASE			0
#ifdef MTDINFO_IN_FLASH
extern int write_flash_data(uchar *src, ulong addr, ulong cnt);
#else
extern int storage_write(uint32_t offset, size_t len, const void* buf);
#endif

#undef SWU_DEBUG
#ifdef SWU_DEBUG
#define dprintf(fmt, args...)		printf("%s: " fmt, __FUNCTION__, ## args)
#else
#define dprintf(fmt, args...)
#endif	/* DEBUG */

extern ulong default_offset;
#ifdef CONFIG_LOAD_SERIAL
extern ulong get_serial_data(ulong offset);
#endif
#ifdef CONFIG_LOAD_TFTP
extern int tftp_get (char *filename);
#endif
#ifdef CONFIG_LOAD_FAT32
extern int fat_fsload(char *filename);
#endif
extern u8 SWU_MICOM_BinUpdate(char* pImg_start, u32 size);
extern u8 SWU_SPI_Update(unsigned char* pImg_start, u32 size);

#define CRC32_POLY	0x04c11db7		/* AUTODIN II, Ethernet, & FDDI */

u32	crc32_table[256];

static void init_crc32(void)
{
	int	i, j;
	u32	c;

	for (i = 0; i < 256; ++i) {
		for (c = i << 24, j = 8; j > 0; --j)
			c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
		crc32_table[i] = c;
	}
}

u32 calc_crc32(unsigned char *buf, u32 len)
{
	unsigned char	*p;
	u32				crc;
	static	u32		fTableOk=0;

//	if (!crc32_table[1])    /* if not already done */
//j		init_crc32();   	/* build table */
	if (fTableOk == 0)
	{
		init_crc32();   	/* build table */
		fTableOk = 1;
	}

	crc = 0xffffffff;       /* preload shift register, per CRC-32 spec */

	for (p = buf; len > 0; ++p, --len)
		crc = (crc << 8) ^ crc32_table[(crc >> 24) ^ *p];

	return(crc);
	//return ~crc;			/* transmit complement, per CRC-32 spec */
}

static FILE_TYPE_T _get_file_type(char *type)
{
	if(strncmp(type, "epak", PAK_TYPE_ID_LEN) == 0)
		return FILE_TYPE_EPK;
	else
		return FILE_TYPE_PAK;
}

static int check_storage_capacity(struct partmap_info * pmi)
{
#if defined(CC_EMMC_BOOT)
	extern struct mmc emmc_info[];
	printf("dev: %x vs part.dev: %x\n", emmc_info[0].capacity, pmi->dev.size);
	return (int)((u32)emmc_info[0].capacity - pmi->dev.size);
#else
	return 0;
#endif
}

static int check_capacity(struct partmap_info * pmi)
{
	printf("dev: %x vs %x\n", GET_DEVICE_INFO()->size, pmi->dev.size);
	if(GET_DEVICE_INFO()->size != pmi->dev.size) {
		return 1;
	}
	return 0;
}

static int check_idkey_change(struct partition_info * pi, struct partition_info *npi)
{
	if( (npi->offset != pi->offset) || (npi->size != pi->size) )
		return 1;
	else
		return 0;
}

#if 1
static int backup_idkey(struct partmap_info * pmi)
{
	struct partition_info *pi, *npi;
	int i, j;
	
	for(i = (GET_PART_NUM()); i > 0; i--) {
		pi = GET_PART_INFO(i);
		
		if(pi->mask_flags & PART_FLG_IDKEY || !strncmp(pi->name, "nvram", 4)) {
			printf("%s ] part_name = %s\n", __func__, pi->name);
			for(j = (pmi->npartition); j > 0; j--) {
				npi = &(pmi->partition[j]);
				
				if( !strncmp(pi->name, npi->name, 4) && check_idkey_change(pi,npi))
				{
					printf("%s has been changed offset or size(0x%x:0x%x)\n", pi->name, (ulong)pi->offset, (ulong)npi->offset);
					backup_partition(pi, npi);
				}	
			}
		}
	}
	return 0;
}

#else
static int backup_idkey(struct partmap_info * pmi)
{
	struct partition_info *pi, *npi;
	int i, j;
	
	for(i=(GET_PART_NUM()-2); i>0; i--) {
		pi = GET_PART_INFO(i);
		
		if(pi->mask_flags & PART_FLG_IDKEY || !strncmp(pi->name, "nvram", 4)) {
			printf("%s ] part_name = %s\n", __func__, pi->name);
			for(j=(pmi->npartition-2); j>0; j--) {
				npi = &(pmi->partition[j]);
				
				if(strncmp(pi->name, npi->name, 4) == 0)
					backup_partition(pi, npi);
			}
		}
	}
	return 0;
}
#endif
static int _do_single_pak(PAK_HEADER_T * pak)
{
extern int erase_datapart(struct partmap_info *pmi);
extern int erase_cachepart(void);

	struct partition_info *mpi = NULL, *unmpi = NULL, *mtdmpi = NULL;;
	struct partmap_info *pmi = NULL;
	char imgtype[PAK_TYPE_ID_LEN + 1];
	u32 offset = 0, size = 0, idx = 0;
	u32 crc = 0, calcrc = 0;
	u32 swver = 0, floffset = 0, flsize = 0;
	int ret = 0;

	dprintf("pak = 0x%x\n", pak);

	/* to do : check model name */
	printf("PAK Model : %s\n", pak->modelName);

	/* to do : check dev mode */
	printf("\t Dev mode : %s\n", pak->DevMode ? "Debug Mode" : "Release Mode");

	/* to do : check crc */
	offset = (ulong)pak + sizeof(PAK_HEADER_T);
	size = pak->imageSize;
	memcpy((void*)&crc, (void*)(offset+size), 4);

	//temp
	printf("test size:0x%x offset:0x%x offset+size:0x%p \n", size, offset, ((u32 *)(offset+size)));

	if((calcrc = calc_crc32((unsigned char *)offset, size)) != crc) {
		printf("\t CRC check failed [%x/%x]\n", calcrc, crc);
		return -1;
	}

	printf("\t CRC check [calcrc:%x/crc:%x]\n", calcrc, crc);
	strncpy(imgtype, pak->imageType, PAK_TYPE_ID_LEN);
	imgtype[PAK_TYPE_ID_LEN] = '\0';

/* FIXME */
#ifdef CC_EMMC_BOOT
	if(!strcmp(imgtype, "mtdinfo"))
		sprintf(imgtype, "partinfo");
#endif
	printf("\t PAK type : %s\n", imgtype);

#if 0
	if(!strncmp(imgtype, "spiboot", 4)) {
		SWU_SPI_Update((unsigned char *)offset, size);
		return 0;
	}

	if(!strncmp(imgtype, "micom", 4)) {
		SWU_MICOM_BinUpdate((char *)offset, size);
		return 0;
	}
#endif

	/* get partition info & handle */
	mpi		= get_used_partition(imgtype);
	unmpi	= get_unused_partition(imgtype);
	if(!mpi) {
		printf("mpi = NULL\n");
		return -1;
	}

	if(!strcmp(mpi->name, "partinfo")) {
		pmi = (struct partmap_info *)(offset);

		/* check storage capacity */
#if 0	//no  need because of run-time emmc size branch		
		if(check_storage_capacity(pmi) < 0) 
		{	/* pmi->dev.size > emmc->chip_size */
			char in_char = 'N';
			printf("warning!!, plz check storeage capacity(%x)\n", pmi->dev.size);
			printf("Do you want download ? <y/N> ");
			in_char = (char)getc();
			printf("%c\n\n", isprint(in_char) ? in_char : ' ');
			
			if(in_char != 'y' && in_char != 'Y' )
				return -1;
		}

		/* check partmap  capacity */
		if(check_capacity(pmi)) 
		{
			mpi->sw_ver = 0;
			backup_idkey(pmi);
		}

			
#endif		
		do {
			mtdmpi = &(pmi->partition[idx]);
			if(mtdmpi->used && mtdmpi->valid && (strncmp(mpi->name, mtdmpi->name, 4) == 0))
				break;
		}while(++idx < PARTITION_MAX);

		if (mpi->sw_ver == mtdmpi->sw_ver) {
			printf("partinfo version is same case!! 0x%x -> 0x%x\n", mpi->sw_ver, mtdmpi->sw_ver);
			return 0;
		}
		printf("partinfo version is different case!! 0x%x -> 0x%x\n", mpi->sw_ver, mtdmpi->sw_ver);
		backup_idkey(pmi);
		erase_datapart(pmi);
	}

	if(unmpi) {
		floffset	= (ulong)(unmpi->offset + CFG_FLASH_BASE);
		flsize		= (ulong)(unmpi->size);
	} else {
		floffset	= (ulong)(mpi->offset + CFG_FLASH_BASE);
		flsize		= (ulong)(mpi->size);
	}

	if( (size > 0) && (size <= flsize) ) {

		/* to do version check */
		printf("\t SW Version : %x\n", pak->SWVersion);
		printf("\t SW Date : %x\n", pak->SWDate);
		swver = pak->SWVersion;

#ifdef MTDINFO_IN_FLASH
		flash_protect(FLAG_PROTECT_CLEAR, floffset, floffset+size-1, &flash_info[0]);
		write_flash_data((uchar*)offset, floffset, size);
#else
		#ifdef CC_EMMC_BOOT
		if(!strcmp(mpi->name, "kernel") || !strcmp(mpi->name, "lgapp")) {
			ret= storage_write_image_fastboot((uint32_t)floffset, (size_t)size, (const void *)offset);
		}
		else
		#endif
		
		if (!strcmp(mpi->name, "partinfo"))
		{		
			ret = storage_write(DEFAULT_MAPBAK__BASE, size+4, (const void *)offset);
			if(ret) {
				printf("emmc write failed..--;;\n");
				return ret;
			}
			
			ret= storage_write((uint32_t)floffset, (size_t)size+4, (const void *)offset);
			if(ret) {
				printf("storage write error.. --;;\n");
				return ret;
			}
		}
		else
		{			
			ret= storage_write((uint32_t)floffset, (size_t)size, (const void *)offset);
			if(ret) {
				printf("storage write error.. --;;\n");
				return ret;
			}
		}
			
#endif
		/* adjust mtdinfo with new info */
		if(unmpi) {
			unmpi->filesize = size;
			unmpi->sw_ver 	= swver;
			unmpi->used 	= YES;
			unmpi->valid	= YES;
			mpi->used 		= NO;
			mpi->valid 		= YES;

			swap_partition(mpi, unmpi);

		} else {
			mpi->filesize	= size;
			mpi->sw_ver		= swver;
		}


		/* save mtdinfo */
		if (strcmp(mpi->name, "partinfo"))
			save_partinfo();
		else {	/* load mtdinfo */
			load_partinfo();
			erase_cachepart();
		}

	} else {
		printf("\t Wrong image size [%x/%x]\n", size, flsize);
		return -1;
	}

	// To make new hibernation image

	if(-1 != get_partition_idx(HIB_PART_NAME))
		remake_hib();

	return 0;
}

extern void set_uart_baudrate(int baudrate);

extern ushort gu4PausePL2303;
static int _do_epk(int type, char *filename)
{
	EPK_HEADER_T	*epk = NULL;
	PAK_HEADER_T	*pak = NULL;
	PAK_LOCATION_T	*pakloc = NULL;
	u32 size = 0, filenum = 0, i = 0, count = 0;
	u32 filetype = 0;
	char dnfilename[128];
	char *userpath;
	char *prj_name;

	userpath = getenv("userpath");
	prj_name = getenv("prj_name");
	if(userpath && prj_name) {
		sprintf(dnfilename, "%s/pak-%s/%s", userpath, prj_name, filename);
	}
	else if (prj_name) {
		sprintf(dnfilename, "pak-%s/%s", prj_name, filename);
	}
	else {
		sprintf(dnfilename, "%s", filename);
	}

	switch(type) {
#ifdef CONFIG_LOAD_TFTP
		case LOAD_TYPE_TFTP:
			gu4PausePL2303 = 1;
			size = tftp_get(dnfilename);
			gu4PausePL2303 = 0;
			break;
#endif
#ifdef CONFIG_LOAD_FAT32
		case LOAD_TYPE_FAT32:
			size = fat_fsload(filename);
			break;
#endif
#ifdef CONFIG_LOAD_ZMODEM
		case LOAD_TYPE_ZMODEM:
			size = rz("test.txt", default_offset);
			break;
#endif
#ifdef CONFIG_LOAD_HZMODEM
		case LOAD_TYPE_HZMODEM:
			set_uart_baudrate(CONFIG_RZ_BAUDRATE);
			size = rz("test.txt", default_offset);
			break;
#endif
#ifdef CONFIG_LOAD_SERIAL
		case LOAD_TYPE_SERIAL:
			size = get_serial_data(default_offset);
			break;
#endif
		default:
			size = 0;
			break;
	}

	if(size == 0) {
		dprintf("file receive error...(type:%d, size:%d)\n", type, size);
		goto _epk_end;
	}

	filetype = _get_file_type((char*)default_offset);

	switch (filetype) {
		case FILE_TYPE_EPK:
			goto _epk_start;
			break;

		case FILE_TYPE_PAK:
			pak = (PAK_HEADER_T*)default_offset;
			_do_single_pak(pak);
			#if 0
			goto _save_mtdinfo;
			#else
			goto _epk_end;
			#endif
			break;

		default:
			printf("unknown file type (0x%x)\n", filetype);
			return -1;
	}

_epk_start:	
	epk = (EPK_HEADER_T*)(default_offset);

	/* check epk version */
	dprintf("check epk version : %lx\n", epk->epkVersion);
	GET_PARTMAP_INFO()->old_epk_ver = GET_PARTMAP_INFO()->cur_epk_ver;
	GET_PARTMAP_INFO()->cur_epk_ver = epk->epkVersion;

	/* handle epk package */
	filenum = epk->fileNum;

	for(i=0; i<filenum;i++) {

		dprintf("filenum = %d [%d]\n", filenum, i);

		pakloc = &(epk->imageLocation[i]);

		if(pakloc->imageOffset == 0)
			continue;

		pak = (PAK_HEADER_T *)(default_offset + pakloc->imageOffset);

		if(_do_single_pak(pak)) {
			printf("Huk --;;\n");
		}

		count++;
		if(count==filenum)
			break;
	}


#if 0
_save_mtdinfo:
	/* save mtdinfo */
	save_mtdinfo();
#endif

_epk_end:
#ifdef CONFIG_LOAD_HZMODEM
	if(type == LOAD_TYPE_HZMODEM) {
		printf("\n\n");
		set_uart_baudrate(CONFIG_BAUDRATE);
	}
#endif
	return 0;
}

static void _do_str_sort(int cnt, char *str[])
{
	int i=0, j=0, index=0;
	char *tmp, *min;

	for(i=0; i<cnt-1; i++)
	{
		index = i;
		min=str[i];
		for(j=i+1; j<cnt; j++)
		{
			if(strcmp(min, str[j]) > 0)
			{
				min = str[j];
				index = j;
			}
		}
		if( i != index)
		{
			tmp = str[i];
			str[i]=str[index];
			str[index]=tmp;
		}
	}
}

static int _do_epk_ex(int type, int argc, char *argv[])
{
	char dnfilename[128]={0,}, boot_cmd[128]={0, };
	int ret = 0, file_num = 0, i = 0, j = 0, index = 0, is_support_head = 0;
	u32 total_rsize = 0, current_wsize = 0, wsize = 0, rsize = 0, remained = 0;
	u32 emmc_write_offset = 0x0;
	PART_LOCATION_T	*partloc = NULL;
	RAWIMG_HEADER_T *check_type = (RAWIMG_HEADER_T*)malloc(SECTOR_SIZE_BOOT);
	if(check_type == NULL) return -1;

	_do_str_sort(argc, argv);
	for(i=0; i<argc; i++)
	{
		printf("dnfilename[%d]: %s\n", i, argv[i]);
	}

	for(i=0; i<argc; i++)
	{
		current_wsize = 0;
		sprintf(dnfilename, "%s", argv[i]);
		printf("dnfilename: %s\n", dnfilename);
	
		switch(type) {
#ifdef CONFIG_LOAD_TFTP
			case LOAD_TYPE_TFTP:
				gu4PausePL2303 = 1;
				rsize = tftp_get(dnfilename);
				gu4PausePL2303 = 0;
				break;
#endif
#ifdef CONFIG_LOAD_FAT32
			case LOAD_TYPE_FAT32:
				rsize = fat_fsload(dnfilename);
				break;
#endif
#ifdef CONFIG_LOAD_ZMODEM
			case LOAD_TYPE_ZMODEM:
				rsize = rz("test.txt", default_offset);
				break;
#endif
#ifdef CONFIG_LOAD_HZMODEM
			case LOAD_TYPE_HZMODEM:
				set_uart_baudrate(CONFIG_RZ_BAUDRATE);
				rsize = rz("test.txt", default_offset);
				break;
#endif
#ifdef CONFIG_LOAD_SERIAL
			case LOAD_TYPE_SERIAL:
				rsize = get_serial_data(default_offset);
				break;
#endif
			default:
				rsize = 0;
				break;
		}

		if(rsize == 0) {
			dprintf("file receive error...(type:%d, size:%d)\n", type, rsize);
			ret = -1;
			goto _bin_end;
		}

		total_rsize += rsize;
		printf("binary[%d] file size : %d\n", i, rsize);
		printf("total_rsize: 0x%x, \n", total_rsize);

		if(i == 0)
		{
			memcpy(check_type, default_offset, SECTOR_SIZE_BOOT);

			if(!strncmp(check_type->magic, RAWIMAGE_MAGIC, strlen(RAWIMAGE_MAGIC)))
			{
				is_support_head = 1;
				file_num = (int)check_type->partition_count;
				printf("Binary header exist!!!\n");
				printf("binary magic: %8s\n", check_type->magic);
				printf("boot area: %s\n", (check_type->check_boot_area)?"Used":"Unused");
				printf("partition number: %d\n", file_num);

				for (j=0; j<file_num; j++) {
					partloc = &(check_type->part_location[j]);
					printf("partition location [%d] info - image_offset: 0x%llx, image_size: 0x%llx, flash_offset: 0x%llx\n", j, partloc->image_offset, partloc->image_size, partloc->flash_offset);
				}
				current_wsize += SECTOR_SIZE_BOOT;
			}
			else
			{
				printf("Origial raw image!!!\n", i);
			}
		}

		if(is_support_head)
		{
			if(index == 0 && check_type->check_boot_area)
			{
				sprintf(boot_cmd, "mmc write.boot 1 0x%x 0 0x%x", default_offset+current_wsize, BOOT_AREA_SIZE);
				printf("%s\n", boot_cmd);
				run_command(boot_cmd,0);
				printf("Use boot area.. download complete!\n");
				current_wsize += BOOT_AREA_SIZE;
			}

			while(index<file_num)
			{
				if(remained == 0)
				{
					partloc = &(check_type->part_location[++index]);
					emmc_write_offset = (u32)(partloc->flash_offset);
					remained = (u32)(partloc->image_size);
					printf("remeained is zero, increase part index[%d -> %d], emmc_write_offset: 0x%x, remained = image_size: 0x%x\n", index-1, index, emmc_write_offset, remained);
				}

				printf("1. part_index: %d, emmc_write_offset: 0x%x, remained: 0x%x, current_wsize: 0x%x\n", index, emmc_write_offset, remained, current_wsize);

				if(total_rsize < partloc->image_offset+partloc->image_size)
				{
					wsize = rsize - current_wsize;
				}
				else
				{
					wsize = remained;
				}

				printf("2. emmc_write_offset: 0x%x, wsize: 0x%x, current_wsize: 0x%x\n", emmc_write_offset, wsize, current_wsize);

				ret = storage_write(emmc_write_offset, (size_t)(wsize), (const void *)(default_offset+current_wsize));
				if(ret) {
					printf("part[%d]: emmc write failed..--;;\n", i);
					ret = -1;
					goto _bin_end;
				}
				current_wsize += wsize;
				emmc_write_offset += wsize;
				remained -= wsize;
				printf("3. emmc_write_offset: 0x%x, remained: 0x%x, current_wsize: 0x%x\n", emmc_write_offset, remained, current_wsize);

				if(current_wsize == rsize)
				{
					printf("current_wsize: 0x%x.. Receive next file!!\n", current_wsize);
					break;
				}
			}
		}
		else
		{
			printf("binary[%d] file copy start. wait a minute plz.\n", i);

			wsize = rsize - current_wsize;
			ret = storage_write(emmc_write_offset, (size_t)(wsize), (const void *)(default_offset+current_wsize));
			printf("Download completed part[%d] image. file offset: 0x%x, file size: 0x%x\n", i, emmc_write_offset, wsize);
			emmc_write_offset += wsize;
		}
		if(ret) {
			printf("part[%d]: emmc write failed..--;;\n", i);
			ret = -1;
			goto _bin_end;
		}
	}

_bin_end:
#ifdef CONFIG_LOAD_HZMODEM
	if(type == LOAD_TYPE_HZMODEM) {
		printf("\n\n");
		set_uart_baudrate(CONFIG_BAUDRATE);
	}
#endif
	if(check_type) free(check_type);

	return ret;
}


#ifdef CONFIG_LOAD_TFTP
int do_swu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("         :\n%s\n", cmdtp->help);
		return -1;
	}
	return _do_epk(LOAD_TYPE_TFTP, argv[1]);
}

U_BOOT_CMD(
		swu,	2,	0,	do_swu,
		"swu\t- downlaod epk image file from tftp, and write on flash \n",
		"[filename]\n"
		);

int do_swub (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int i = 0;
	if(argc > 1)
	{
		for(i=1; i<argc; i++)
		{
			// check file name format
			if(strstr(argv[i], "test")==NULL || strstr(argv[i], ".part")==NULL)
			{
				printf("File format is wrong: %s\n", argv[i]);
				printf("File format: test_filename.part1 .. test_filename.partN\n");
				return -1;
			}
		}

		return _do_epk_ex(LOAD_TYPE_TFTP, argc-1, argv+1);
	}
	else {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("         :\n%s\n", cmdtp->help);
		return -1;
	}
}

U_BOOT_CMD(
		swub,	5,	0,	do_swub,
		"swub\t- downlaod raw image file from tftp, and write on flash \n",
		"swub [filename] || swub [filename1] [filename2]\n"
		);

#endif

#ifdef CONFIG_LOAD_FAT32
int do_swum (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc != 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("         :\n%s\n", cmdtp->help);
		return -1;
	}
	return _do_epk(LOAD_TYPE_FAT32, argv[1]);
}

U_BOOT_CMD(
		swuu,	2,	0,	do_swum,
		"swum\t- downlaod epk image file from fat32, and write on flash \n",
		"[filename]\n"
		);
#endif

#ifdef CONFIG_LOAD_ZMODEM
int do_swuz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc != 1) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("		  :\n%s\n", cmdtp->help);
		return -1;
	}
	return _do_epk(LOAD_TYPE_ZMODEM, (void*)0x0);
}

U_BOOT_CMD(
		swuz,	1,	0,	do_swuz,
		"swuz\t- downlaod epk image file from zmodem, and write on flash \n",
		" \n"
		);
#endif

#ifdef CONFIG_LOAD_HZMODEM
int do_swuhz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc != 1) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("		  :\n%s\n", cmdtp->help);
		return -1;
	}
	return _do_epk(LOAD_TYPE_HZMODEM, (void*)0x0);
}

U_BOOT_CMD(
		swuhz,	1,	0,	do_swuhz,
		"swuhz\t- downlaod epk image file from high speed zmodem, and write on flash \n",
		" \n"
		);
#endif

#ifdef CONFIG_LOAD_SERIAL
int do_swus (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc != 1) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		printf ("		  :\n%s\n", cmdtp->help);
		return -1;
	}
	return _do_epk(LOAD_TYPE_SERIAL, (void*)0x0);
}

U_BOOT_CMD(
		swus,	1,	0,	do_swus,
		"swus\t- downlaod epk image file from serial, and write on flash \n",
		" \n"
		);
#endif

#if 1
/*
 * for swu mode test by junorion
 */
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*
 * for snapshot boot by junorion
 */
extern UINT8 DDI_NVM_GetMakeHib(void);
extern void DDI_NVM_SetMakeHib( UINT8 mode );

static int make_hib_cmd(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	UINT8 make_hib = DDI_NVM_GetMakeHib();

	printf("[make hib status]\n");
	printf("current : %s", make_hib ? "on" : "off");

	if(argc == 1) {
		printf("\n");
		return 0;
	}

	if (argc == 2) {
		UINT8 new_make_hib = make_hib;

		new_make_hib = (strcmp(argv[1], "on")) ? (UINT8)FALSE : (UINT8)TRUE;

		if(make_hib != new_make_hib) {
			printf(" => %s\n", argv[1]);
			DDI_NVM_SetMakeHib((UINT8)(new_make_hib));
		}
	}
	else
		return -1;

	return 0;
}

U_BOOT_CMD(
		make_hib,	2,	0,	make_hib_cmd,
		"make_hib\t- set make hib\n",
		"make_hib [on/off]\n"
		);
#endif

