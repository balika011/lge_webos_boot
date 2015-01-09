#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <mtdinfo.h>
#include <exports.h>
#undef DEBUG
/* note: prints function name for you */
#ifdef DEBUG
#define dprintf(fmt, args...)	printf("%s: " fmt, __FUNCTION__, ## args)
#else
#define dprintf(fmt, args...)
#endif	/* DEBUG */

#define println()				printf("\n")

#ifdef MTDINFO_IN_FLASH
extern int write_flash_data(uchar *src, ulong addr, ulong cnt);
#else
/* references to names in cmd_nand.c */
#include <nand.h>
extern nand_info_t nand_info[];
extern int write_flash_data(nand_info_t *nand, ulong ofs, ulong cnt, u_char *src);
#endif

unsigned int print_mtdinfo(void);
unsigned int save_mtdinfo(void);
unsigned int make_default_mtdinfo(struct boot_mtd_info *mtd_info);
unsigned int load_mtdinfo(void);
unsigned int load_default_mtdinfo(void);

char *delim_ = " .:\n";	//\f\n\r\t\v
struct boot_mtd_info mtdinfo;

DECLARE_GLOBAL_DATA_PTR;

#ifdef CFG_LG_CHG
extern int gets(char *s);
#endif
static int cfg_parse_args(char *cmdline, char **argv)
{
	char *tok;
	int argc = 0;

	argv[argc] = NULL;

	for (tok = strtok(cmdline, delim_); tok; tok = strtok(NULL, delim_)) {
		argv[argc++] = tok;
	}

	return argc;
}

#if 0
static unsigned int get_bool( const char *msg, unsigned int *tf)
{
	int     argc;
	char    rbuff[1024];
	char    *argv[1024];

	printf( msg, *tf ? "YES" : "NO" );
	printf( " : " );
	gets( rbuff );

	argc = cfg_parse_args( rbuff, argv );
	if (argc == 1) {
		if(!memcmp(argv[0], "YES", strlen(argv[0])))
			*tf = YES;
		else
			*tf = NO;
	}
	return 0;
}

static unsigned int get_uchr(const char *msg, unsigned char *uc)
{
	int     argc;
	char    rbuff[1024];
	char    *argv[1024];

	printf(msg, *uc);
	printf(" : ");

	gets(rbuff);

	argc = cfg_parse_args(rbuff, argv);
	if (argc == 1) {
		*uc = simple_strtoul(argv[0], NULL, 0);
	}
	return 0;
}
#endif

static unsigned int get_char(const char *msg, char *cc)
{
	int     argc;
	char    rbuff[1024];
	char    *argv[1024];

	printf( msg, *cc );
	printf(" : ");

	gets(rbuff);

	argc = cfg_parse_args(rbuff, argv);
	if (argc == 1) {
		*cc = toupper(argv[0][0]);
	}

	return 0;
}

static unsigned int get_uint(const char *msg, unsigned int *num)
{
	int     argc;
	char    rbuff[128];
	char    *argv[128];

	printf(msg, (int)*num);
	printf(" : ");

	gets(rbuff);

	argc = cfg_parse_args(rbuff, argv);
	if (argc == 1) {
		*num = simple_strtoul( argv[0], NULL, 0 );
	}
	return 0;
}

static unsigned int get_str(const char *msg, char *ptr)
{
	char rbuff[1024];

	printf(msg, ptr);
	printf("\n\t : ");

	gets(rbuff);

	if(strlen(rbuff) > 0)
		strcpy(ptr, rbuff);

	return 0;
}

static unsigned char get_select(void)
{
	unsigned char rbuf = '\0';

	get_char("select ", (char *)&rbuf);
	println();

	return rbuf;
}

const char *menu_partition_str[] = {
	"\nMTD Partition Information ---------------------------------------------------------------------------------\n\n",
	"index[%d] - ",
	"cur epk ver : 0x%06x",
	"old epk ver : 0x%06x",
	"\t name	= %s",
	"\t offset	= 0x%08x",
	"\t size	= 0x%08x",
	"\t sw_ver = %d",
	"\t filename = %s",
	"\t MTD flags = %s\n"
};

const char *menu_partition_info[] = {
	"[%2d] \"%-12s\" : 0x%08x-0x%08x (0x%08x)",
	" %c%c%c%c",
	" : \"%-20s\"[%d] - 0x%06x : (%c/%c)\n",
	"[%2d] Empty\n"
};

unsigned int print_mtdinfo(void)
{
	struct mtd_partition_info *mpi = NULL;
	int i=0;

	printf(menu_partition_str[0]);
	printf(menu_partition_str[2], mtdinfo.cur_epk_ver); println();
	printf(menu_partition_str[3], mtdinfo.old_epk_ver); println();
	println();
	for(i=0; i<(mtdinfo.npartition); i++) {
		mpi = GET_PARTITION_INFO(i);

		printf(menu_partition_info[0], i, mpi->name,
				mpi->offset, (mpi->offset+mpi->size), mpi->size);

		printf(menu_partition_info[1],
				(mpi->mask_flags & MTD_FLG_FIXED)	? 'F' : '-',
				(mpi->mask_flags & MTD_FLG_MASTER)	? 'M' : '-',
				(mpi->mask_flags & MTD_FLG_IDKEY)	? 'I' : '-',
				(mpi->mask_flags & MTD_FLG_CACHE)	? 'C' : '-'
			  );

		if(strlen(mpi->filename)) {
			printf(menu_partition_info[2],
					mpi->filename, mpi->filesize, mpi->sw_ver,
					mpi->used  ? 'U' : 'u',
					mpi->valid ? 'V' : 'v');
		}
		else
			println();
	}

	return 0;
}

struct mtd_partition_info * get_mtdpartinfo(int idx)
{
	if( (idx > 0) && (idx < mtdinfo.npartition))
		return GET_PARTITION_INFO(idx);

	return (struct mtd_partition_info*)(NULL);
}

unsigned int get_partition_information(PART_INFO_T partinfo, char *name)
{
	struct mtd_partition_info *mpi = NULL;
	unsigned int idx = 0;

	do {
		mpi = GET_PARTITION_INFO(idx);
		if(mpi->used && mpi->valid && (strncmp(name, mpi->name, 4) == 0)) {
			switch (partinfo) {
				case PART_INFO_IDX 		: return idx;
				case PART_INFO_OFFSET 	: return mpi->offset;
				case PART_INFO_SIZE 	: return mpi->size;
				case PART_INFO_FILESIZE	: return mpi->filesize;
			}
		}
	}while(++idx < MTD_PARTITION_MAX);

	if (partinfo == PART_INFO_IDX && (strncmp(name, "rootfs", 4) == 0))
		return 3; //rootfs default parition
	if (partinfo == PART_INFO_IDX && (strncmp(name, "lginit", 4) == 0))
		return 4; //lginit default parition

	return -1;
}

struct mtd_partition_info * get_partition(char *name)
{
	struct mtd_partition_info *mpi = NULL;
	unsigned int idx = 0;

	do {
		mpi = GET_PARTITION_INFO(idx);
		if(mpi->used && mpi->valid && (strncmp(name, mpi->name, 4) == 0))
			return mpi;
	}while(++idx < MTD_PARTITION_MAX);

	return (struct mtd_partition_info*)(NULL);
}

struct mtd_partition_info * get_unused_partition(char *name)
{
	struct mtd_partition_info * mpi = NULL;
	unsigned int idx = 0;

	do {
		mpi = GET_PARTITION_INFO(idx);
		if(!mpi->used && mpi->valid && (strncmp(name, mpi->name, 4) == 0))
			return mpi;
	}while(++idx < MTD_PARTITION_MAX);

	return (struct mtd_partition_info*)(NULL);
}

unsigned int swap_partition(struct mtd_partition_info * mpi1, struct mtd_partition_info *mpi2)
{
	struct mtd_partition_info temp;

	temp = *mpi1;
	*mpi1 = *mpi2;
	*mpi2 = temp;

	return 0;
}

static unsigned int modify_partition(unsigned int index)
{
	struct mtd_partition_info *partition = GET_PARTITION_INFO(index);

	printf  (menu_partition_str[1], index);						println();
	get_str (menu_partition_str[4], &(partition->name[0]));		println();
	get_uint(menu_partition_str[5], &(partition->offset));		println();
	get_uint(menu_partition_str[6], &(partition->size));		println();
	get_uint(menu_partition_str[7], &(partition->sw_ver));		println();
	get_str (menu_partition_str[8], &(partition->filename[0]));	println();

#if 0
	switch (partition->mask_flags) {
		case MTD_WRITEABLE:
			printf(menu_partition_str[9], "MTD_WRITEABLE");
			println();
			break;
		default:
			break;
	}
#endif

	return 0;
}

static unsigned int add_partition(void)
{
	int i=0;
	struct mtd_partition_info *partition  = NULL;

	/* find empty index */
	do {
		partition = GET_PARTITION_INFO(i);
		if( (partition->used == NO) && (partition->valid == NO) ) {
			break;
		}
	} while(++i<MTD_PARTITION_MAX);

	/* check right index */
	if(i==MTD_PARTITION_MAX) {
		printf("no empty entry...\n");
		return 0;
	}

	/* add new entry */
	modify_partition(i);

	return 0;
}

static unsigned int edit_partition(void)
{
	unsigned int index = 0;

	/* get index */
	get_uint("select index", &index);
	println();

	/* edit indexed entry */
	if(index < MTD_PARTITION_MAX)
		modify_partition(index);
	else
		printf("%d is wrong index...!!\n", index);

	return 0;
}

static unsigned int blank_partition(void)
{
	unsigned int index = 0;
	struct mtd_partition_info *partition  = NULL;

	/* get index */
	get_uint("select index", &index);
	println();

	partition = GET_PARTITION_INFO(index);
	partition->filesize = 0;
	partition->sw_ver = 0;

	nand_erase(&nand_info[0], partition->offset, partition->size);

	return 0;
}

static unsigned int rm_partition(void)
{
	struct mtd_partition_info *partition = NULL;
	struct mtd_partition_info null_partition = DEFAULT_PTT_NULL;
	unsigned int index = 0;

	/* get index */
	get_uint("select index", &index);
	println();

	/* remove indexed entry */
	if(index < MTD_PARTITION_MAX) {
		partition = GET_PARTITION_INFO(index);
		*partition = null_partition;
	} else
		printf("%d is wrong index...!!\n", index);

	return 0;
}

const char cfg_menu_default_str[] = "D.\tDefault\n";
const char cfg_menu_load_str[] = "L.\tLoad\n";
const char cfg_menu_save_str[] = "S.\tSave\n";
const char cfg_menu_quit_str[] = "Q.\tExit\n";

const char *menu_partition_cmd[] = {
	"A.\tAdd    flash partition\n",
	"E.\tEdit   flash partition\n",
	"B.\tBlank  flash partition\n",
	"R.\tRemove flash partition\n",
} ;
unsigned int config_mtdinfo(void)
{
	unsigned char rchr;

	do {
		print_mtdinfo();
		println();
		printf(menu_partition_cmd[0]);
		printf(menu_partition_cmd[1]);
		printf(menu_partition_cmd[2]);
		printf(menu_partition_cmd[3]);
		println();
		printf(cfg_menu_default_str);
		printf(cfg_menu_load_str);
		printf(cfg_menu_save_str);
		printf(cfg_menu_quit_str);
		println();

		rchr = get_select();
		switch(rchr) {
			case 'a' : case 'A' :	add_partition();	break;
			case 'e' : case 'E' :	edit_partition();	break;
			case 'b' : case 'B' :	blank_partition();	break;
			case 'r' : case 'R' :	rm_partition();		break;
			case 'd' : case 'D' : 	load_default_mtdinfo();	break;
			case 'l' : case 'L' : 	load_mtdinfo();		break;
			case 's' : case 'S'	: 	save_mtdinfo();		break;
			case 'q' : case 'Q'	:	return 0;
			default : 				break;
		}

	} while(1);

	return 0;
}

unsigned int save_mtdinfo(void)
{
	struct mtd_map_info	*mmi = NULL;
	struct mtd_partition_info *mpi = NULL;
	unsigned int nmap=0, npartition=0;
	int i=0, ret = 0;
	ulong size;

#if 0
	/* 1. map info */
	for(i=0; i<MTD_MAP_MAX; i++) {
		mmi = &(mtdinfo.map[i]);
		if(mmi->used == YES)
			nmap++;
	}
#endif

	/* 2. partition info */
	for(i=0; i<MTD_PARTITION_MAX; i++) {
		mpi = &(mtdinfo.partition[i]);
		if(mpi->size)
			npartition++;
	}

	mtdinfo.magic		= CONFIG_MAGIC ;
	//mtdinfo.nmap		= nmap;
	mtdinfo.npartition	= npartition;

#ifdef DEBUG
	dprintf("\n%s: magic [%08x] nmap=[%d] npartition=[%d]\n", __func__,
			mtdinfo.magic, 1/*mtdinfo.nmap*/, mtdinfo.npartition);
#endif

#ifdef MTDINFO_IN_FLASH
	// store in flash
	write_flash_data((u_char *)(&mtdinfo), (ulong)FLASH_MTD_INFO, sizeof(struct boot_mtd_info));
	return 0;
#else
	size = sizeof(struct boot_mtd_info);
	ret = write_flash_data(&nand_info[0], (ulong)DEFAULT_MTDINFO_BASE, (ulong)size, (u_char *)&mtdinfo);
	if(ret) {
		printf("nand write failed..--;;\n");
		return ret;
	}

	return ret;
#endif
}

unsigned int load_mtdinfo(void)
{
#ifdef MTDINFO_IN_FLASH
	struct boot_mtd_info * pmi;

	pmi = (struct boot_mtd_info *)(FLASH_MTD_INFO);

	memcpy(&mtdinfo, pmi, sizeof(struct boot_mtd_info));

	return 0;
#else
	int ret = 0;
	ulong size = 0;
	struct boot_mtd_info *pmi = NULL;

	pmi = (struct boot_mtd_info *)malloc(sizeof(struct boot_mtd_info));

	size = sizeof(struct boot_mtd_info);
	ret = nand_read(&nand_info[0], (ulong)DEFAULT_MTDINFO_BASE, (size_t *)&size, (u_char *)pmi);
	if(ret) {
		printf("nand read failed..--;;\n");
		return ret;
	}

	memcpy(&mtdinfo, pmi, sizeof(struct boot_mtd_info));

	return 0;
#endif
}

//by dhjung
#ifdef CONFIG_TFTP_PATH_IN_MTDINFO
unsigned int make_default_mtdinfo(struct boot_mtd_info *mtd_info)
{
	int i;
	char *prefix, fullpath[128];
	struct mtd_partition_info *mpi = NULL;

	prefix = getenv("userpath");

	for (i=0; i<MTD_PARTITION_MAX; i++) {
		mpi = &(mtd_info->partition[i]);
		if (strcmp(mpi->filename, ""))
		{
			if (prefix != NULL)
			{
				sprintf(fullpath, "%s%s", prefix, mpi->filename);
				strcpy(mpi->filename, fullpath);
			}
		}
	}

	return 0;
}
#endif

unsigned int load_default_mtdinfo(void)
{
	struct boot_mtd_info default_mtdinfo = DEFAULT_MTD_INFO;

#ifdef CONFIG_TFTP_PATH_IN_MTDINFO
	make_default_mtdinfo(&default_mtdinfo);
#endif

	mtdinfo = default_mtdinfo;
	save_mtdinfo();

	return 0;
}

unsigned int init_mtdinfo(void)
#ifdef MTDINFO_IN_FLASH
{
	struct boot_mtd_info * pmi = (struct boot_mtd_info *)(FLASH_MTD_INFO);

	dprintf("pmi(%x) - FLASH_MTD_INFO(%x)\n", (unsigned int*)pmi, FLASH_MTD_INFO);
	dprintf("pmi->magic(%x) - CONFIG_MAGIC(%x)\n", pmi->magic, CONFIG_MAGIC);

	if(pmi->magic != CONFIG_MAGIC) {
		load_default_mtdinfo();
		save_mtdinfo();
	} else
		load_mtdinfo();

	return 0;
}
#else
{
	int ret = 0;
	ulong size = 0;
	struct boot_mtd_info *pmi = NULL;

	pmi = (struct boot_mtd_info *)malloc(sizeof(struct boot_mtd_info));

	size = sizeof(struct boot_mtd_info);
	ret = nand_read(&nand_info[0], (ulong)DEFAULT_MTDINFO_BASE, (size_t *)&size, (u_char *)pmi);

	if(ret) {
		printf("nand read failed..--;;\n");
		return ret;
	}

	if(ret==0) {
		if(pmi->magic != CONFIG_MAGIC)
			load_default_mtdinfo();
		else
			load_mtdinfo();
	} else {
		printf("nand read failed..--;;\n");
		load_default_mtdinfo();
	}

	return 0;
}
#endif

int do_mtdinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[] )
#ifdef MTDINFO_IN_FLASH
{
	struct boot_mtd_info * pmi;

	pmi = (struct boot_mtd_info *)(FLASH_MTD_INFO);

	if(pmi->magic != CONFIG_MAGIC)
		load_default_mtdinfo();
	else
		load_mtdinfo();

	config_mtdinfo();

	return 0;
}
#else
{
	int ret = 0;
	ulong size = 0;
	struct boot_mtd_info *pmi = NULL;

	pmi = (struct boot_mtd_info *)malloc(sizeof(struct boot_mtd_info));

	size = sizeof(struct boot_mtd_info);
	ret = nand_read(&nand_info[0], (ulong)DEFAULT_MTDINFO_BASE, (size_t *)&size, (u_char *)pmi);

	if(ret==0) {
		if(pmi->magic != CONFIG_MAGIC)
			load_default_mtdinfo();
		else
			load_mtdinfo();
	} else {
		printf("nand read failed..--;;\n");
		load_default_mtdinfo();
	}

	config_mtdinfo();

	return ret;
}
#endif

/* ====================================================================== */
U_BOOT_CMD(
		mtdinfo,      2,      0,      do_mtdinfo,
		"mtdinfo\t- edit or add or remove mtdinfo\n",
		" - \n"
		);
