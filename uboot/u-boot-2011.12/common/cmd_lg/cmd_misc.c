#include <common.h>
#include <command.h>
#include <net.h>

#include <xyzModem.h>
#include <environment.h>

#include <partinfo.h>

#include <cmd_resume.h>

extern block_dev_desc_t *get_dev (char* ifname, int dev);
extern int fat_register_device(block_dev_desc_t *dev_desc, int part_no);
extern void set_uart_baudrate(int baudrate);
extern long file_fat_read(const char *filename, void *buffer, unsigned long maxsize);

#ifdef MTDINFO_IN_FLASH
extern int write_flash_data(uchar *src, ulong addr, ulong cnt);
#else
#ifdef CC_EMMC_BOOT
#include <mmc.h>
extern struct partmap_info partinfo;
#undef CFG_ENV_OFFSET
#define CFG_ENV_OFFSET	(off_t)((partinfo.dev.size) - CFG_ENV_SIZE)
#else
#include <nand.h>
#ifdef CONFIG_MTD_NAND_BBM
extern int nand_bbm_update(struct mtd_info * mtd);
#endif
#endif
int storage_read(uint32_t offset,size_t len,void * buf);
int storage_write(uint32_t offset,size_t len,const void * buf);
int storage_erase(unsigned long long offset, unsigned long long len);
#endif

extern u8 SWU_MICOM_BinUpdate(char* pImg_start, u32 size);
extern u8 SWU_SPI_Update(unsigned char* pImg_start, u32 size);

//extern uchar default_environment[];
extern int default_environment_size;
extern env_t *env_ptr;

DECLARE_GLOBAL_DATA_PTR;

char boot1stVer[10];
ulong default_offset = CFG_LOAD_ADDR;

int storage_init(void);
int storage_read(uint32_t offset, size_t len, void* buf);
int storage_write(uint32_t offset, size_t len, const void* buf);


#ifndef CC_EMMC_BOOT
int write_flash_data(nand_info_t *nand, ulong ofs, ulong cnt, u_char *src)
{
	ulong length;
	ulong start=0, offset;
	ulong erasesize = nand->erasesize;
	int ret = 0;

	length  = cnt;
	length += (length % erasesize) ? (erasesize - (length % erasesize)) : 0;

	/* 1. erase */
	printf ("Erasing Nand (0x%08x+0x%08x) ---> ", (unsigned int)ofs, (unsigned int)length);
	for(start = 0; start < length; start += erasesize) {
		offset = ofs + start;
		// erase block
		if (nand_erase(nand, offset, erasesize)) {
			printf ("Failed\n");
			return 1;
		}
	}
	printf ("OK\n");

#ifdef CONFIG_MTD_NAND_BBM
	/* 2. bbm update */
	nand_bbm_update(nand);
#endif

	/* 3. write */
	printf ("Writing Nand (0x%08x+0x%08x) ---> ", (unsigned int)ofs, (unsigned int)length);
	ret = nand_write(nand, ofs, (size_t *)&length, src);
	if(ret) {
		printf ("Failed[%d]\n", ret);
		return 1;
	}
	printf ("OK\n");

	return 0;
}
#endif

#ifdef CONFIG_LOAD_TFTP
int tftp_get (char *filename)
{
	int  size;
	char fullpath[128];

	sprintf(fullpath, "%s", filename);

	load_addr = default_offset;
	copy_filename (BootFile, fullpath, strlen(fullpath)+1);

	if ((size = NetLoop(TFTPGET)) < 0)
		return 1;

	/* done if no file was loaded (no errors though) */
	if (size == 0)
		return 0;

	return size;
}
#endif

#ifdef CONFIG_LOAD_FAT32
int fat_fsload(char *filename)
{
	long size;
	block_dev_desc_t *dev_desc = NULL;
	int dev = 0, part = 1;

	dev_desc = get_dev("usb", dev);
	if (dev_desc == NULL) {
		puts ("Invalid boot device\n");
		return 1;
	}

	if (fat_register_device(dev_desc, part) != 0) {
		printf ("Unable to use usb %d:%d for fatload\n", dev, part);
		return 1;
	}

	size = file_fat_read(filename, (unsigned char *) default_offset, 0);
	if(size == -1) {
		printf("Unable to read \"%s\" from usb %d:%d\n", filename, dev, part);
		return 1;
	}

	return size;
}
#endif

#ifdef CONFIG_LOAD_KERMIT
ulong kermit_get(ulong offset)
{
	int size, i;

	set_kerm_bin_mode ((ulong *) offset);
	size = k_recv ();

	/*
	 * Gather any trailing characters (for instance, the ^D which
	 * is sent by 'cu' after sending a file), and give the
	 * box some time (100 * 1 ms)
	 */
	for (i=0; i<100; ++i) {
		if (tstc()) {
			(void) getc();
		}
		udelay(1000);
	}

	flush_cache (offset, size);

	printf("## Total Size      = 0x%08x = %d Bytes\n", size, size);

	return size;
}

int do_rkermit (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong size=0;
	ulong address=0;

	address = simple_strtoul(argv[1], NULL, 16);
	size = kermit_get(address);

	if(!size) {
		printf("rkermit failed...(size:%d)\n", size);
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	rk,	2,	0,	do_rkermit,
	"rk\t- downlaod image file,though zmodem\n",
	"[address]\n"
);
#endif

#ifdef CONFIG_LOAD_SERIAL
ulong get_serial_data(ulong offset)
{
	extern int serial_tstc (void);
	extern int serial_getc (void);

	unsigned char  *buf = NULL;
	ulong  ch;
	ulong len=0;
	ulong sTimer, cTimer = 0, dsTimer = 0, tmdiff = 0;
	ulong timeout=60, dataflush;
	printf("Download from serial... send image file\n");

	len = 0;
	buf = (unsigned char*)(default_offset);

	do {
		sTimer = get_timer(0);
_getc:
		if ( serial_tstc() ) {
			ch = serial_getc();
			buf[len] = (unsigned char)ch;
			if(len==0)
				dsTimer = get_timer(0);
			len++;
			timeout = 1;
		} else {
			cTimer = get_timer(0);
			tmdiff = (cTimer - sTimer) / CFG_HZ;
			if(tmdiff > timeout)	{
				goto _end_of_down;
			}
			goto _getc;
		}
	} while(1);

_end_of_down:
	printf("Download done size : %d bytes in %d.%03dms\n", len, (cTimer-dsTimer)/1000, (cTimer-dsTimer)%1000);

	while (serial_tstc())
		dataflush = serial_getc();

	return len;
}

int do_rserial(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong size=0;
	ulong address=0;

	address = simple_strtoul(argv[1], NULL, 16);
	size = get_serial_data(address);

	if(!size) {
		printf("rserial failed...(size:%d)\n", size);
		return -1;
	}

	return 0;
}

U_BOOT_CMD(
	rs,	2,	0,	do_rserial,
	"rs\t- downlaod image file,though zmodem\n",
	"[address]\n"
);
#endif

extern ushort gu4PausePL2303;

static int _load(int type, char* partname)
{
	ulong size = 0, floffset = 0, flsize = 0, idx = 0;
#ifdef CC_EMMC_BOOT
	struct partition_info *mpi = NULL, *mtdmpi = NULL;
	struct partmap_info *pmi = NULL;
#else
	struct partition_info *mpi = NULL, *mtdmpi = NULL;
	struct partmap_info *pmi = NULL;
#endif

	char dnfilename[64];

#ifndef CC_EMMC_BOOT
	if(!strncmp(partname, "spiboot", 4))
	{
		sprintf(dnfilename, "u-boot-spi.bin");
		goto loads_start;
	}
#endif

	if(!strncmp(partname, "lcdmicom", 4))
	{
		sprintf(dnfilename, "micom_lcd_nec.dat");
		goto loads_start;
	}

	if(!strncmp(partname, "pdpmicom", 4))
	{
		sprintf(dnfilename, "micom_pdp.hex");
		goto loads_start;
	}

	mpi = get_used_partition(partname);
	if(!mpi) {
		printf("unknown partition : %s\n", partname);
		return 0;
	}

	floffset	= (ulong)(mpi->offset + CFG_FLASH_BASE);
	flsize		= (ulong)mpi->size;
	sprintf(dnfilename, mpi->filename);

loads_start:
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
			size = fat_fsload(dnfilename);
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
#ifdef CONFIG_LOAD_KERMIT
		case LOAD_TYPE_KERMIT:
			size = kermit_get(default_offset);
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
		printf("size is zero..!!\n");
		goto loads_end;
	}

	if(flsize != 0 && (size > flsize)) {
		printf("out of partition size : %x / %x\n", (unsigned int)size, (unsigned int)flsize);
		goto loads_end;
	}

	if(!strncmp(partname, "spiboot", 4)) {
//		SWU_SPI_Update((unsigned char *) default_offset, size);
		goto loads_end;
	}

	if(!strncmp(partname, "lcdmicom", 4) || !strncmp(partname, "pdpmicom", 4)) {
		SWU_MICOM_BinUpdate((char *)default_offset, size);
		goto loads_end;
	}

	if (!strcmp(mpi->name, "partinfo")) {
		pmi = (struct partmap_info *)(default_offset);
		do {
			mtdmpi = &(pmi->partition[idx]);
			if(mtdmpi->used && mtdmpi->valid && (strncmp(mpi->name, mtdmpi->name, 4) == 0))
				break;
		}while(++idx < PARTITION_MAX);

		if (mpi->sw_ver == mtdmpi->sw_ver) {
			printf("partinfo version is same case!! 0x%x -> 0x%x\n", mpi->sw_ver, mtdmpi->sw_ver);
			goto loads_end;
		}
		printf("mtdinfo version is different case!! 0x%x -> 0x%x\n", mpi->sw_ver, mtdmpi->sw_ver);
	}

	storage_write(floffset,size, (u_char*)default_offset);
#ifndef CC_EMMC_BOOT
	#ifdef MTDINFO_IN_FLASH
	flash_protect(FLAG_PROTECT_CLEAR, floffset, floffset+size-1, &flash_info[0]);
	write_flash_data((uchar*)default_offset, floffset, size);
	#else
	write_flash_data(&nand_info[0], (ulong)floffset, (ulong)size, (u_char *)default_offset);
	#endif
#endif
	mpi->filesize = size;


	/* save partinfo */
	if (strcmp(mpi->name, "partinfo"))
		save_partinfo();

	/* load partinfo */
	if (!strcmp(mpi->name, "partinfo"))
		load_partinfo();

loads_end:
#ifdef CONFIG_LOAD_HZMODEM
	if(type == LOAD_TYPE_HZMODEM) {
		printf("\n\n");
		set_uart_baudrate(CONFIG_BAUDRATE);
	}
#endif

	return 0;
}

#ifdef CONFIG_LOAD_TFTP
int do_load (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_TFTP, argv[1]);
}

U_BOOT_CMD(
	load,	2,	0,	do_load,
	"load\t- downlaod image file using tftp, and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

#ifdef CONFIG_LOAD_FAT32
int do_loadm (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_FAT32, argv[1]);
}

U_BOOT_CMD(
	loadm,	2,	0,	do_loadm,
	"loadm\t- downlaod image file using memory stick, and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

#ifdef CONFIG_LOAD_ZMODEM
int do_loadz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_ZMODEM, argv[1]);
}

U_BOOT_CMD(
	loadz,	2,	0,	do_loadz,
	"loadz\t- downlaod image file using zmodem, and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

#ifdef CONFIG_LOAD_HZMODEM
int do_loadhz (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_HZMODEM, argv[1]);
}

U_BOOT_CMD(
	loadhz,	2,	0,	do_loadhz,
	"loadhz\t- downlaod image file using zmodem(460800), and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

#ifdef CONFIG_LOAD_KERMIT
int do_loadk (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_KERMIT, argv[1]);
}

U_BOOT_CMD(
	loadk,	2,	0,	do_loadk,
	"loadk\t- downlaod image file using zmodem, and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

#ifdef CONFIG_LOAD_SERIAL
int do_loads (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if (argc > 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
	}
	return _load(LOAD_TYPE_SERIAL, argv[1]);
}

U_BOOT_CMD(
	loads,	2,	0,	do_loads,
	"loads\t- downlaod image file using serial, and write on flash \n",
	" [flashoffset] \n [partition name] - \n"
);
#endif

int do_defaultenv (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int ret = 0;
	struct partition_info *env_partinfo;
	if (default_environment_size > CFG_ENV_SIZE) {
		puts ("*** Error - default environment is too large\n\n");
		return 1;
	}

	memset (env_ptr, 0, sizeof(env_t));
	memcpy (env_ptr->data, default_environment, default_environment_size);
	env_ptr->crc = crc32(0, env_ptr->data, ENV_SIZE);
	gd->env_valid = 1;


	if((env_partinfo = get_partition_by_name("env_nvm")) != NULL)
	{
		printf("Saving Default Environment....\n");
		printf("[%s] env_part: %s\n", __func__, env_partinfo->name);
		printf("write to %x (len=%d,ptr = 0x%p) \n", (ulong)env_partinfo->offset, (ulong)env_partinfo->size, env_ptr);
#ifdef CC_EMMC_BOOT
		ret = storage_write((ulong)env_partinfo->offset, (ulong)env_partinfo->size, (u_char *)env_ptr);
#else
		ret = write_flash_data(&nand_info[0], (ulong)CFG_ENV_OFFSET, (ulong)CFG_ENV_SIZE, (u_char *)env_ptr);
#endif	
		if (ret) {
			printf("env save failed\n");
			return 1;
		}
	}
	else
	{
		printf("env_nvm partition not found.\n");
		return 1;	
	}

	return ret;
}

U_BOOT_CMD(
	defaultenv, 1,  0,  do_defaultenv,
	"defaultenv\t- set default env\n",
	"\n"
);

extern cmd_tbl_t __u_boot_cmd_setboot;
extern int do_setboot (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
int saveenv(void)
{
	char *rootfs_str, *argv[2];
	int	ret = 0;

	rootfs_str = getenv("rootfs");

	argv[0] = "setboot";
	argv[1] = rootfs_str;

	ret = do_setboot (&__u_boot_cmd_setboot, 0, 2, argv);
	if (ret) {
		printf("env save failed\n");
		return 1;
	}
	if(-1 != get_partition_idx(HIB_PART_NAME))
		remake_hib();
	
	return ret;
}

int do_revert(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong size = 0, floffset = 0, flsize = 0;
	struct partition_info *mpi = NULL;

	mpi = get_used_partition("lginit");
	if(!mpi) {
		printf("unknown partition : %s\n", "lginit");
		return -1;
	}

	mpi->filesize = 0;
	mpi->sw_ver = 0;

	//nand_erase(&nand_info[0], mpi->offset, mpi->size);
	storage_erase(mpi->offset, mpi->size);

	mpi = get_used_partition("lgapp");
	if(!mpi) {
		printf("unknown partition : %s\n", "lgapp");
		return -1;
	}

	mpi->filesize = 0;
	mpi->sw_ver = 0;

	//nand_erase(&nand_info[0], mpi->offset, mpi->size);
	storage_erase(mpi->offset, mpi->size);

	mpi = get_used_partition("lgfont");
	if(!mpi) {
		printf("unknown partition : %s\n", "lgfont");
		return -1;
	}

	mpi->filesize = 0;
	mpi->sw_ver = 0;

	//nand_erase(&nand_info[0], mpi->offset, mpi->size);
	storage_erase(mpi->offset, mpi->size);

	mpi = get_used_partition("kernel");
	if(!mpi) {
		printf("unknown partition : %s\n", "kernel");
		return 0;
	}

	floffset	= (ulong)(mpi->offset + CFG_FLASH_BASE);
	flsize		= (ulong)mpi->size;
	sprintf(mpi->filename, "uImage"); //to revert to normal

	size = tftp_get(mpi->filename);
	if(size == 0) {
		printf("size is zero..!!\n");
		return -1;
	}

	if(size > flsize) {
		printf("out of partition size : %x / %x\n", (unsigned int)size, (unsigned int)flsize);
		return -1;
	}

//	write_flash_data(&nand_info[0], (ulong)floffset, (ulong)size, (u_char *)default_offset);
	storage_write((uint32_t)floffset, (size_t)size, (const void *)default_offset);
	mpi->filesize = size;

	save_partinfo();

	return 0;
}

U_BOOT_CMD(
	revert,	1,	0,	do_revert,
	"revert\t- revert to normall mode\n",
	"revert \n"
);
#if 1
#define PARAM_SZ 0x46
#define NUMADDRS   8
#define BUFLENGTH  (7 + 4 * NUMADDRS * 2)	/* matching non-os */
#define	BOARD_TYPE_MASK	0x1F
#define	SER_NBR_MASK	0xFF
#define	ADDR_NBR_MASK	0x07
#define BOARD_SER_NBR_OFFSET	1000

#define SWAP16( x ) ((((x) & 0xFF00) >> 8) | (((x) & 0x00FF) << 8))

// for mac address
#define MAC_MAGIC_NUMBER 0x20110208
#define MAX_MAC_STR		17

#define LG_CHG_BOOT

int do_macprog(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    int i;
    int hdl;
    int retval=0;
    int mac_idx = 0;
    uint16_t macbuf[BUFLENGTH];
	uint16_t macbuftmp[BUFLENGTH];
    uint32_t boardtype = 0;
    uint16_t serialnum = 0;
    uint16_t revnum    = 0;
    uint8_t  checksum  = 0;
    char * str;
    char strtempa[3];
    char strtempb[3];

#ifdef LG_CHG_BOOT
	/* modify mac address format from xx-xx-xx-xx-xx-xx to xx:xx:xx:xx:xx:xx */
	char modified_mac[MAX_MAC_STR+1] = {0};
	int magic_number= MAC_MAGIC_NUMBER;
#define DASH 	0
#define COLON 	1
	int dash_colon ; //dash 0 , colon : 1;
	char * pMac;
#endif

	ulong size = 0, floffset = 0, flsize = 0;
	int idx=0;
	struct partition_info *mpi = NULL;

	int ret;

    for (i = 0; i < BUFLENGTH; i++)
        macbuf[i] = 0xffff;

    str = argv[1];//getenv("enetaddr");
    printf("str = %s \n",str);
#ifdef LG_CHG_BOOT
    if( !str || (strlen(str) != 17))
 	{
        printf( "Invalid MAC address format\n" );
        return -1;
    }

    if  ((str[2] == '-') && (str[5] == '-') &&
        (str[8] == '-') && (str[11] == '-') &&
        (str[14] == '-'))
	{
		dash_colon = DASH;
	}
    else if ((str[2] == ':') && (str[5] == ':') &&
        (str[8] == ':') && (str[11] == ':') &&
        (str[14] == ':')) // accept colon input, also.
	{
		dash_colon = COLON;
	}
	else
	{
        printf( "Invalid MAC address format\n" );
        return -1;
	}
#else
    if( !str || (strlen(str) != 17) ||
        (str[2] != '-') || (str[5] != '-') ||
        (str[8] != '-') || (str[11] != '-') ||
        (str[14] != '-')) {
        xprintf( "Invalid MAC address format\n" );
        return -1;
    }
#endif

    /* [xm] Is is for something else and will be corrected later */
    macbuf[0] = SWAP16( ((boardtype >> 16) & 0x0FFF) );
    macbuf[1] = SWAP16( (boardtype & 0xFFFF) );
    /* Inject the number of bytes, starting at the next word. */
    macbuf[2] = 0;
    macbuf[3] = SWAP16( NUMADDRS * 2 * 4 * 2 + 4 ); /* Same as non-os */
    macbuf[4] = SWAP16( serialnum );
    macbuf[5] = SWAP16( revnum );

    for( i = 0; i < NUMADDRS; i++ ) {
        mac_idx = 6+i*4;
        strncpy(strtempa,&str[0],2);
        strtempa[2] = '\0';
        strncpy(strtempb,&str[3],2);
        strtempb[2] = '\0';
        macbuf[mac_idx]   = SWAP16(((simple_strtoul(strtempa,NULL,16) << 8) & 0xFF00) | (simple_strtoul(strtempb,NULL,16) & 0xFF));
        strncpy(strtempa,&str[6],2);
        strtempa[2] = '\0';
        strncpy(strtempb,&str[9],2);
        strtempb[2] = '\0';
        macbuf[mac_idx+1] = SWAP16(((simple_strtoul(strtempa,NULL,16) << 8) & 0xFF00) | (simple_strtoul(strtempb,NULL,16) & 0xFF));
        strncpy(strtempa,&str[12],2);
        strtempa[2] = '\0';
        strncpy(strtempb,&str[15],2);
        strtempb[2] = '\0';
        macbuf[mac_idx+2] = SWAP16(((simple_strtoul(strtempa,NULL,16) << 8) & 0xFF00) | (simple_strtoul(strtempb,NULL,16) & 0xFF));
        macbuf[mac_idx+3] = SWAP16(0);
    }

    /* calculate checksum. */
    checksum = 0;
    for( i = 0; i < PARAM_SZ; i++ ) {
        checksum += ((macbuf[i] & 0xff00) >> 8);
        checksum +=  (macbuf[i] & 0x00ff);
    }
    macbuf[PARAM_SZ] = ((checksum & 0xFF) << 8) | (checksum & 0xFF);

    printf( "Programming flash...\n" );
#if 1
	do {
		mpi = GET_PART_INFO(idx);
		if( strncmp("macadr", mpi->name, 4) == 0 )
			break;
	}while(++idx < PARTITION_MAX);

	floffset	= (ulong)(mpi->offset + CFG_FLASH_BASE);
	size		= (ulong)mpi->size;
	printf("offset = 0x%x , psize = 0x%x \n",floffset,size);
//	for(i=0;i<BUFLENGTH;i++)
//		printf("macbuf[%d] = 0x%x \n",i,macbuf[i]);

	storage_write(floffset, size, (u_char*)macbuf);

	setenv("ethaddr",str);
	saveenv();
//	ret	= nand_read(&nand_info[0], floffset, (size_t*)&size, (u_char *)macbuftmp);
//	if (ret) {
//		printf("nand read failed..\n");
//		return 1;
//	}
//	for(i=0;i<BUFLENGTH;i++)
//		printf("macbuftmp[%d] = 0x%x \n",i,macbuftmp[i]);
#else
    retval = cfe_writeblk( hdl, (cfe_offset_t) 0x0,
                           (unsigned char *)macbuf, BUFLENGTH * 2 );
#endif
    printf( "done\n" );

    if( !retval ) {
#ifdef LG_CHG_BOOT
		printf( "Setting mac address...\n");
		if ( dash_colon == DASH)
		{
			strncpy(modified_mac, str, MAX_MAC_STR);
			modified_mac[2] = ':';
			modified_mac[5] = ':';
			modified_mac[8] = ':';
			modified_mac[11] = ':';
			modified_mac[14] = ':';
			modified_mac[MAX_MAC_STR] = '\0';
			pMac = modified_mac;
		}
		else
		{
			pMac = str;
		}
		printf("mac : %s\n", pMac);
		//env_setenv("ETH0_HWADDR", pMac, ENV_FLG_NORMAL);
		//env_save();

		// backup flash mac address value to nvram by zaewon.lee
		//NVM_MAC_ADDRESS_STORE_ITEM(macAddr,pMac);
		//NVM_MAC_ADDRESS_STORE_ITEM(validMark,&magic_number);
#endif
        retval =  0;
    } else {
        printf( "Failure while writing to flash.\n" );
        printf( "(%d != %d)\n", retval, (BUFLENGTH * 2) );
        retval = -1;
    }

    return retval;
}

U_BOOT_CMD(
	macprog,	2,	0,	do_macprog,
	"macprog\t- setting mac address \n",
	"macprog \n"
);

int do_print(cmd_tbl_t *cmdtp, int flag, int argc, char* argv[])
{
	int		changed = 0;
	char*	print = getenv("print");

	if(print == NULL)
	{
		setenv("print", "on");
		print = getenv("print");
		changed = 1;
	}
	printf("current print status : %s\n", print);

	if(argc == 2)
	{
		char* input = argv[1];

		if(!strcmp("on", input) || !strcmp("off", input))
		{
			if(strcmp(input, print))
			{
				printf("change the print status to %s\n", input);
				setenv("print", input);
				changed = 1;
			}
		}
	}

	if(changed)
		saveenv();

	return 0;
}

U_BOOT_CMD(
	print,	2,	0,	do_print,
	"print\t- turn on/off print message.\n",
	"print \n"
);

#if 1
typedef enum
{
	SPLASH_INIT,
	SPLASH_START,
	SPLASH_TRY2END0,
	SPLASH_TRY2END,
	SPLASH_END
}	SPLASH_MODE_T;

//extern u32 LogoDemo(void);
extern void BootSplash(void);
extern void Splash_MICOM_PanelOn( void );
extern void Splash_MICOM_InvOn( void );


int do_testlogo(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	printf("test logo \n");
#if 0
	Splash_MICOM_PanelOn();
	LogoDemo();
	Splash_MICOM_InvOn();
#else
	BootSplash();
#endif

	return 0;
}

U_BOOT_CMD(
	testlogo,	1,	0,	do_testlogo,
	"testlogo\t- boot logo test\n",
	"testlogo \n"
);
#endif

#endif

#ifndef CFG_LG_CHG
int get_tick(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	printf("current time tick = %ld\n", get_cur_time());
	return 0;
}

U_BOOT_CMD(
	tick,	1,	0,	get_tick,
	"tick\t- show time tick\n",
	"tick \n"
);
#endif /* ! CFG_LG_CHG */

#define	CACHE_LINE_WORDS			(8)
int test_cachemissmemcpy(volatile unsigned int * pSrc, volatile unsigned int * pDest, unsigned int count)
{
	volatile unsigned int *p1 = NULL;
	volatile unsigned int *p2 = NULL;
	unsigned int	i, k;
	unsigned int	nTest;
	unsigned int	loop;

	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (volatile unsigned int *) pSrc;
		p2 = (volatile unsigned int *) pDest;
		loop = (count/sizeof(unsigned int));

		for (k = 0; k < CACHE_LINE_WORDS ; k++)
			for (i = 0; i < loop; i+=CACHE_LINE_WORDS)
				p2[k+i] = p1[k+i];
	}

	return nTest;
}

int test_membandwidth(volatile unsigned int * pSrc, volatile unsigned int * pDest, unsigned int count)
{
	volatile unsigned int *p1 = NULL;
	volatile unsigned int *p2 = NULL;
	unsigned int	i;
	unsigned int	nTest;
	unsigned int	loop;

	for (nTest = 0; nTest < 10; nTest++)
	{
		p1 = (volatile unsigned int *) pSrc;
		p2 = (volatile unsigned int *) pDest;
		loop = (count/sizeof(unsigned int));

		for (i = 0; i < loop; i++)
			p2[i] = p1[i];
	}

	return nTest;
}

#ifndef CFG_LG_CHG
int do_chkmem(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	volatile unsigned int	*src, *dst;
	unsigned int count;
	int		loop;
	int		test_count;
	unsigned long	delay;
	unsigned long	tss, tse;

	if (argc != 2) {
		printf("argc=%d, Usage:\n%s\n", argc, cmdtp->usage);
		return -1;
	}

	count = simple_strtoul(argv[1], NULL, 16);
	if (count == 0 || count & 0x3) {
		printf("Zero length or invalid count\n");
		return -1;
	}

	src = (volatile unsigned int *) 0x82000000;
	dst = src + count;

	loop  = 0;
	printf("<mem read/write test>\n");
	while (loop < 3)
	{
		loop++;
		tss = get_cur_time();
		test_count = test_membandwidth(src, dst, count);
		tse = get_cur_time();

		delay = (tse > tss)? (tse-tss) : (0xFFFFFFFF-tss+tse);
		printf("Read/Write %3d: %d times, %8d bytes, %4ul msec => %6d KB/sec\n",
					loop,
					test_count,
					count,
					delay,
					(((count * test_count) / 1024) * 1000) / delay);
	}
	printf("\n<Cache Miss mem read/write test>\n");
	loop = 0;
	while (loop < 3)
	{
		loop++;

		tss = get_cur_time();
		test_count = test_cachemissmemcpy(src, dst, count);
		tse = get_cur_time();

		delay = (tse > tss)? (tse-tss) : (0xFFFFFFFF-tss+tse);
		printf("Read/Write %3d: %d times, %8d bytes, %4ul msec => %6d KB/sec\n",
					loop,
					test_count,
					count,
					delay,
					(((count * test_count) / 1024) * 1000) / delay);
	}
	return 0;
}

U_BOOT_CMD(
	chkmem,	2,	0,	do_chkmem,
	"chkmem\t- memory copy test\n",
	"chkmem count(bytes) \n"
);


extern void drvNAND_SetSpeed(int index);

int do_nandset(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{

	unsigned int index;

	if (argc != 2) {
		printf("argc=%d, Usage:\n%s\n", argc, cmdtp->usage);
		return -1;
	}

	index = simple_strtoul(argv[1], NULL, 16);
	drvNAND_SetSpeed(index);

	return 0;
}

U_BOOT_CMD(
	nandset, 2,	0,	do_nandset,
	"nandset\t- nand clock set\n",
	"nandset 1 ~ 4\n"
);
#endif /* ! CFG_LG_CHG */

char *get_1stbootver(void)
{
	memcpy(boot1stVer, (char *)0xbfc00014, 7);
	boot1stVer[7] = '\0';

	return (char *)&boot1stVer[0];
}

/*
 * storage funcs
 */
typedef enum
{
	STORAGE_TYPE_NONE = 0,
	STORAGE_TYPE_NAND,
	STORAGE_TYPE_EMMC,
} storage_type_t;

//static storage_type_t default_storage = STORAGE_TYPE_NAND;

static const char *storage_blkdev_str[] =
{
	"mtdblock",
	"mmcblk0p",
};

int storage_init(void)
{
#ifdef CC_EMMC_BOOT
	return emmc_init();
#else
	nand_init();
	return 0;
#endif
}

int storage_read(uint32_t offset, size_t len, void* buf)
{
#ifdef CC_EMMC_BOOT
	return emmc_read((off_t)offset, (size_t)len, (u_char *)buf);
#else
	return nand_read(&nand_info[0], (off_t)offset, (size_t *)&len, (u_char *)buf);
#endif
}

int storage_write(uint32_t offset, size_t len, const void* buf)
{
#ifdef CC_EMMC_BOOT
	return emmc_write((off_t)offset, (size_t)len, (u_char *)buf);
#else
	return nand_write(&nand_info[0], (off_t)offset, (size_t *)&len, (u_char *)buf);
#endif
}

#if CC_EMMC_BOOT
int storage_write_image_fastboot(uint32_t offset, size_t len, const void* buf)
{
#define EMMC_BLOCKALIGN_SIZE 		(0x400000)
	int tlen = 0;
	#if 0
	//erase
	for(tlen=0; tlen<len; tlen+=EMMC_BLOCKALIGN_SIZE) {
		emmc_erase(offset + tlen, EMMC_BLOCKALIGN_SIZE);
	}
	#endif
	//write
	for(tlen=0; tlen<len; tlen+=EMMC_BLOCKALIGN_SIZE) {
		emmc_write(offset + tlen, EMMC_BLOCKALIGN_SIZE, (buf + tlen));
	}

	return 0;
}
#endif

int storage_erase(unsigned long long offset, unsigned long long len)
{
#ifdef CC_EMMC_BOOT

	uint unit_size;
	uint count;
	int i = 0;
	uint erase_size;
	uint erase_mod_size;
	uint erase_unit_size;
	u64 erase_add_align;
	uint erase_add_size;
	char * erase_buf;
	erase_unit_size = emmc_get_erase_size();
	erase_add_align	= offset & ((u64)erase_unit_size -1);
	if(erase_add_align)
	{
		erase_add_size = erase_unit_size - erase_add_align;
		erase_buf = (char *)malloc(erase_unit_size);
		memset(erase_buf, 0x0, erase_unit_size);
		if(emmc_write((ulong)offset, (ulong)erase_add_size, erase_buf) != 0)
		{
			free(erase_buf);
			return -1;
		}
		offset = offset + (u64)erase_add_size;
		len = len - erase_add_size;
		free(erase_buf);
	}
	unit_size = erase_unit_size * 64;
	if(unit_size > 0x4000000)
	{
		unit_size = 0x4000000;
	}

	/* get erase loop count */
	count = len / unit_size;
	if (len & (unit_size - 1))
	{
		count++;
	}
	/* erase per unit size */
	if (count > 1)
	{
		for (i = 0; i < (count - 1); i++)
		{
			if(emmc_erase((ulong)(offset + (i * unit_size)), (ulong)unit_size) != 0)
				return -1;
		}
	}
	erase_size = len - (i * unit_size);

	if (emmc_get_trim_enable() == 1)
	{
		/* erase last */
		if(emmc_erase_trim((ulong)(offset + (i * unit_size)), (ulong)erase_size) != 0)
			return -1;
	}
	else
	{
		erase_mod_size = erase_size & (erase_unit_size - 1);

		if (erase_size >= erase_unit_size)
		{
			if(emmc_erase((ulong)(offset + (i * unit_size)), (ulong)(erase_size - erase_mod_size)) != 0)
				return -1;
		}

		if (erase_mod_size)
		{
			erase_buf = (char *)malloc(erase_unit_size);
			memset(erase_buf, 0x0, erase_unit_size);

			if(emmc_write((ulong)(offset + (i * unit_size) + erase_size - erase_mod_size), (ulong)erase_mod_size, erase_buf) != 0)
			{
				free(erase_buf);
				return -1;
			}

			free(erase_buf);
		}
	}
	return 0;
	//return emmc_erase((off_t)(offset), (size_t)len);
#else
	return nand_erase(&nand_info[0], offset, len);
#endif
}

char * get_blkdev_name(void)
{
#ifdef CC_EMMC_BOOT
	return (char *)storage_blkdev_str[STORAGE_TYPE_EMMC-1];
#else
	return (char *)storage_blkdev_str[STORAGE_TYPE_NAND-1];
#endif
}

int get_blkdev_idx(const char *name)
{
	int idx = get_partition_idx(name);
	#ifdef CC_EMMC_BOOT
	idx++;
	#endif
	return idx;
}

void do_erase_hib(void)
{
	struct partition_info	*mpi;
	mpi	= get_used_partition("hib");
	if(!mpi) {
		printf("mpi = NULL, no hibernation partition\n");
		return;
	}
	storage_erase(mpi->offset, 0x200);
	printf("hib partition erase complete\n");
	return;
}

U_BOOT_CMD(
		erase_hib,	1,	0,	do_erase_hib,
		"erase_hib\t- delete hibernation image from hib partition\n",
		"erase_hib\n"
		);
