
#include <common.h>
#include <command.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <partinfo.h>
#include <exports.h>
#include <mmc.h>

#undef DEBUG
//#define DEBUG
/* note: prints function name for you */
#ifdef DEBUG
#define dprintf(fmt, args...)	printf("%s: " fmt, __FUNCTION__, ## args)
#define DEBUGF(fmt, args...)	printf(fmt ,##args)
#else
#define dprintf(fmt, args...)
#define DEBUGF(fmt, args...)
#endif	/* DEBUG */

#define println()				printf("\n")
#define ENTER()	DEBUGF("%s() enter\n", __FUNCTION__);
#define LEAVE()	DEBUGF("%s() leave\n", __FUNCTION__);
#define DEFAULT_REMAIN_INDEX	(8)

/* storage func */
extern int storage_read(uint32_t offset, size_t len, void* buf);
extern int storage_write(uint32_t offset, size_t len, void* buf);

unsigned int print_partinfo(void);
unsigned int save_partinfo(void);
unsigned int load_partinfo(void);
unsigned int load_default_partinfo(void);

static char *delim_ = " .:\n";	//\f\n\r\t\v
struct partmap_info partinfo;

DECLARE_GLOBAL_DATA_PTR;

static struct partmap_info *pmi = NULL;

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

static unsigned int get_bchar(const char *msg, char *bc, char bigch)
{
	int     argc;
	char    rbuff[1024];
	char    *argv[1024];

	printf( msg, (*bc) ? (bigch) : (bigch+0x20) );
	printf(" : ");

	gets(rbuff);

	argc = cfg_parse_args(rbuff, argv);
	if (argc == 1) {
		if (argv[0][0] == bigch)
			*bc = (unsigned char)1;
		else
			*bc = (unsigned char)0;
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
	extern int gets(char *s);
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

static const char *menu_partition_str[] = {
	"\nPartition Information ---------------------------------------------------------------------------------\n\n",
	"index[%d] - ",
	"cur epk ver : 0x%06x",
	"old epk ver : 0x%06x",
	"\t name	= %s",
	"\t offset	= 0x%08x",
	"\t size	= 0x%08x",
	"\t sw_ver = %d",
	"\t filename = %s",
	"\t flags = %s\n",
	"\t used = %c",
	"\t valid = %c"
};

static const char *menu_partition_info[] =
{
	"[%2d] \"%-12s\" : 0x%01x%08x-0x%01x%08x (0x%01x%08x)",
	" %c%c%c%c%c",
	" : \"%-20s\"[%d] - 0x%06x : (%c/%c) [%3d\%]\n",
	"[%2d] Empty\n"
};
unsigned int print_partinfo(void)
{
	struct partition_info *pi = NULL;
	int i=0;

	printf(menu_partition_str[0]);
//	printf("magic : %08x", partinfo.magic);println();
//	println();
	printf(menu_partition_str[2], partinfo.cur_epk_ver); println();
	printf(menu_partition_str[3], partinfo.old_epk_ver); println();
	println();

	if (partinfo.npartition > PARTITION_MAX) {
		printf ("[ERROR] Number of partition is %d\n", partinfo.npartition);
		return (unsigned int)-1;
	}

	for(i=0; i<(partinfo.npartition); i++) {
		pi = GET_PART_INFO(i);

		printf(menu_partition_info[0], i, pi->name,
				U64_UPPER(pi->offset), U64_LOWER(pi->offset), U64_UPPER(pi->offset+pi->size), U64_LOWER(pi->offset+pi->size), 
				U64_UPPER(pi->size), U64_LOWER(pi->size));

		printf(menu_partition_info[1],
				(pi->mask_flags & PART_FLG_FIXED)	? 'F' : '-',
				(pi->mask_flags & PART_FLG_MASTER)	? 'M' : '-',
				(pi->mask_flags & PART_FLG_SECURED)	? 'S' : '-',
				(pi->mask_flags & PART_FLG_IDKEY)	? 'I' : '-',
				(pi->mask_flags & PART_FLG_CACHE)	? 'C' : 
				((pi->mask_flags & PART_FLG_DATA)	? 'D' : '-')
			  );

		if(pi->mask_flags & PART_FLG_ERASE) 	printf("*");

		if(strlen(pi->filename)) {
			printf(menu_partition_info[2],
					pi->filename, pi->filesize, pi->sw_ver,
					pi->used  ? 'U' : 'u',
					pi->valid ? 'V' : 'v', (unsigned long)((double) pi->filesize/pi->size * 100)  );
		}
		else
			println();
	}

	return 0;
}

struct partition_info * get_partition(int idx)
{
	if( (idx >= 0) && (idx < partinfo.npartition))
		return GET_PART_INFO(idx);

	return (struct partition_info*)(NULL);
}

/* returned partition_info that is used and valid */
struct partition_info * get_used_partition(const char *name)
{
	struct partition_info *pi = NULL;
	unsigned int idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if(pi->used && (strncmp(name, pi->name, 4) == 0))
			return pi;
	}while(++idx < GET_PART_NUM());

	return (struct partition_info*)(NULL);
}

/* returned partition_info that is used and valid */
struct partition_info * get_partition_by_name(const char *name)
{
	struct partition_info *pi = NULL;
	unsigned int idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if((strncmp(name, pi->name, 4) == 0))
			return pi;
	}while(++idx < GET_PART_NUM());

	return (struct partition_info*)(NULL);
}

int get_partition_idx(const char *name)
{
	struct partition_info *pi = NULL;
	unsigned int idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if(pi->used && pi->valid && (strncmp(name, pi->name, 4) == 0))
			return idx;
	}while(++idx < GET_PART_NUM());

	return -1;
}

struct partition_info * get_unused_partition(const char *name)
{
	struct partition_info * pi = NULL;
	unsigned int idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if(!pi->used && pi->valid && (strncmp(name, pi->name, 4) == 0))
			return pi;
	}while(++idx < GET_PART_NUM());

	return (struct partition_info*)(NULL);
}


unsigned int swap_partition(struct partition_info * pi1, struct partition_info *pi2)
{
	struct partition_info temp;

	temp = *pi1;
	*pi1 = *pi2;
	*pi2 = temp;

	return 0;
}

static unsigned int modify_partition(unsigned int index)
{
	struct partition_info *partition = GET_PART_INFO(index);

	printf   (menu_partition_str[ 1], index);                     println();
	get_str  (menu_partition_str[ 4], &(partition->name[0]));     println();
	get_uint (menu_partition_str[ 5], &(partition->offset));      println();
	get_uint (menu_partition_str[ 6], &(partition->size));        println();
	get_uint (menu_partition_str[ 7], &(partition->sw_ver));      println();
	get_str  (menu_partition_str[ 8], &(partition->filename[0])); println();
	get_bchar(menu_partition_str[10], &(partition->used), 'U');   println();
	get_bchar(menu_partition_str[11], &(partition->valid),'V');   println();
	return 0;
}

static unsigned int add_partition(void)
{
	int i=0;
	struct partition_info *partition  = NULL;

	/* find empty index */
	do {
		partition = GET_PART_INFO(i);
		if( (partition->used == NO) && (partition->valid == NO) ) {
			break;
		}
	} while(++i<PARTITION_MAX);

	/* check right index */
	if(i==PARTITION_MAX) {
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
	if(index < PARTITION_MAX)
		modify_partition(index);
	else
		printf("%d is wrong index...!!\n", index);

	return 0;
}

static unsigned int blank_partition(void)
{
	unsigned int index = 0;
	struct partition_info *partition  = NULL;

	/* get index */
	get_uint("select index", &index);
	println();

	partition = GET_PART_INFO(index);
	partition->filesize = 0;
	partition->sw_ver = 0;

	return 0;
}

static unsigned int rm_partition(void)
{
	struct partition_info *partition = NULL;
	struct partition_info null_partition = DEFAULT_PTT_NULL;
	unsigned int index = 0;

	/* get index */
	get_uint("select index", &index);
	println();

	/* remove indexed entry */
	if(index < PARTITION_MAX) {
		partition = GET_PART_INFO(index);
		*partition = null_partition;
	} else
		printf("%d is wrong index...!!\n", index);

	return 0;
}

int backup_partition(struct partition_info *pi1, struct partition_info *pi2)
{
	void * buf;
	int ret = 0;

	if(pi1->size > pi2->size)
	{
		printf("Can not do backup_partition(ori %d > new %d)", pi1->size, pi2->size);
		return -1;
	}

	printf("Start %s ....\n", __func__);
	buf = (void *)malloc(pi1->size);

	ret = storage_read((ulong)pi1->offset, (uint)pi1->size, buf);
	if(ret < 0) {
		printf("storage read failed..--;;\n");
		goto exit;
	}

	ret = storage_write((ulong)pi2->offset, (uint)pi1->size, buf);
	if(ret < 0) {
		printf("storage write failed..--;;\n");
		goto exit;
	}

exit:
	free(buf);
	printf("Done %s\n", __func__);
	return ret;
}

static const char cfg_menu_default_str[] = "D.\tDefault\n";
static const char cfg_menu_load_str[] = "L.\tLoad\n";
static const char cfg_menu_save_str[] = "S.\tSave\n";
static const char cfg_menu_quit_str[] = "Q.\tExit\n";

static const char *menu_partition_cmd[] = {
	"A.\tAdd    partition\n",
	"E.\tEdit   partition\n",
	"B.\tBlank  partition\n",
	"R.\tRemove partition\n",
} ;
unsigned int config_partinfo(void)
{
	unsigned char rchr;

	do {
		print_partinfo();
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
			case 'a' : case 'A'	: add_partition();	break;
			case 'e' : case 'E'	: edit_partition();	break;
			case 'b' : case 'B'	: blank_partition();	break;
			case 'r' : case 'R'	: rm_partition();		break;
			case 'd' : case 'D'	: load_default_partinfo(); break;
			case 'l' : case 'L'	: load_partinfo();		break;
			case 's' : case 'S'	: save_partinfo();		break;
			case 'q' : case 'Q'	: return 0;
			default : 				break;
		}
	} while(1);

	return 0;
}

extern u32 calc_crc32(unsigned char *buf, u32 len);

unsigned int save_partinfo(void)
{
	struct partition_info *mpi = NULL;
	int i=0, ret = 0;
	int crc_offset = 4;
	ulong size;
	unsigned char *buf = NULL;
	u32 crc=0, calcrc;

	buf = &partinfo;//
	partinfo.magic			= PARTMAP_MAGIC ;
	size = sizeof(struct partmap_info);
	calcrc = calc_crc32(buf, size);	

	buf = (unsigned char *)malloc(size + crc_offset);
	if(buf == NULL){
		printf("malloc failed");
		return -1;
	}
	//copy partinfo to buf
	memcpy((void *)buf, &partinfo, size);
	//add crc to buf
	memcpy((void *)(buf + size), &calcrc, crc_offset);
	
	printf("\t CRC check [calcrc:%x]\n", calcrc);	
	printf("save partinfo  (%s :%d)\n", __FUNCTION__, __LINE__);

	printf("write to 0x%p (len=%d,ptr = 0x%p):mapbak \n",DEFAULT_MAPBAK__BASE, size + crc_offset, buf);
	ret = storage_write(DEFAULT_MAPBAK__BASE, size + crc_offset, (struct partition_info *) buf);
	if(ret) {
		printf("emmc write failed..--;;\n");
		return ret;
	}
	
	printf("write to 0x%p (len=%d,ptr = 0x%p):partinfo \n",DEFAULT_PARTINF_BASE, size + crc_offset, buf);
	ret = storage_write(DEFAULT_PARTINF_BASE, size + crc_offset, (struct partition_info *) buf);
	if(ret) {
		printf("emmc write failed..--;;\n");
		return ret;
	}
	free(buf);
	return ret;
}

extern u32 calc_crc32(unsigned char *buf, u32 len);

unsigned int load_partinfo(void)
{
	int ret = 0;
	size_t size = 0;
	u32 crc = 0, calcrc = 0;	
	int crc_offset = 4;
	unsigned char *offset = NULL;
	unsigned char *buf = NULL;

	size = sizeof(struct partmap_info);

// 0. check crc of partinfo
#if 1
	if(!offset){
		offset = malloc(size + crc_offset);
		if(offset == NULL){
			printf("malloc failed");
			return -1;
		}
	}
	ret = storage_read((ulong)DEFAULT_PARTINF_BASE, size + crc_offset, offset);
	if(ret) {
		printf("emmc read failed..--;;\n");
		return ret;
	}
	memcpy((void*)&crc, (void*)(offset + size), crc_offset);

	if((calcrc = calc_crc32(offset, size)) != crc)
	{
		printf("\t CRC check failed [%x/%x], so read mapbak and copy to partinfo \n", calcrc, crc);
		if(!buf){
			buf = malloc(size + crc_offset);
			if(buf == NULL){ 
				printf("malloc failed");
				return -1;
			}
		}
		ret = storage_read((ulong)DEFAULT_MAPBAK__BASE, size + crc_offset, buf);
		if(ret) {
			printf("emmc read failed..--;;\n");
			return ret;
		}

		ret = storage_write(DEFAULT_PARTINF_BASE, size + crc_offset, buf);
		if(ret) {
			printf("emmc write failed..--;;\n");
			return ret;
		}
		free(buf);
	}
	free(offset);	

	printf("\t CRC check OK [calcrc:%x/crc:%x]\n", calcrc, crc);
#endif

// 1. read partinfo
	if(!pmi){
		pmi = (struct partmap_info *)malloc(sizeof(struct partmap_info));
		if(pmi == NULL){
			printf("malloc failed");
			return -1;
		}
	}
	
	ret = storage_read((ulong)DEFAULT_PARTINF_BASE, size, (u_char *)pmi);
	if(ret) {
		printf("emmc read failed..--;;\n");
		return ret;
	}
	
	memcpy(&partinfo, pmi, sizeof(struct partmap_info));	

	return 0;
}

unsigned int load_default_partinfo(void)
{
	int i, cnt;
	unsigned long long sto_size, floffset;	
#if defined(CC_EMMC_BOOT)
	extern struct mmc emmc_info[];
#endif
	//initialize a default partmap_info
	struct partmap_info default_partinfo = DEFAULT_PARTMAP_INFO;

	#ifdef CC_EMMC_BOOT
	if(emmc_info[0].capacity > CONFIG_EMMC_HYNIX_SIZE)
		sto_size = CONFIG_EMMC_REAL_SIZE;
	else
		sto_size = CONFIG_EMMC_HYNIX_SIZE;
	#else
	sto_size = CFG_FLASH_SIZE;
	#endif
	floffset = sto_size - DEFAULT_STORAGE_SIZE;
	
	printf("CONFIG_EMMC_REAL_SIZE = 0x%01x%08x\n", U64_UPPER(sto_size),U64_LOWER(sto_size));
	printf("DEFAULT_STORAGE_SIZE  = 0x%09llx\n", DEFAULT_STORAGE_SIZE);

	partinfo = default_partinfo;
	partinfo.dev.size = sto_size;

	cnt = partinfo.npartition;
	partinfo.partition[DEFAULT_REMAIN_INDEX-1].size += floffset;
	for (i = DEFAULT_REMAIN_INDEX; i < cnt; i++) {
		partinfo.partition[i].offset += floffset;
	}
	save_partinfo();
	return 0;
}

int init_partinfo(void)
{
	static int _binit = 0;

	ENTER();
	if (_binit)
		return 0;

	if(load_partinfo()) {
		load_default_partinfo();
	}
	else {
//		print_partinfo();
		erase_flagged_part(&partinfo);
	}
	
	_binit = 1;
//	print_partinfo();

	LEAVE();
	return 0;
}

int do_partinfo ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[] )
{
	if(load_partinfo()) {
		load_default_partinfo();
	}

	config_partinfo();

	return 0;
}

/* ====================================================================== */
U_BOOT_CMD(
		partinfo,      2,      0,      do_partinfo,
		"partinfo\t- edit or add or remove partinfo\n",
		" - \n"
		);

int erase_flagged_part(struct partmap_info *pmi)
{
	struct partition_info *mpi = NULL;
	int i,updated = 0;

	for(i=0; i<pmi->npartition; i++) {
		mpi = &(pmi->partition[i]);
		if(mpi->mask_flags & PART_FLG_ERASE) {
			printf("PART [%02d] %s part has DELETE flag\n", i, mpi->name);
			
			if ( emmc_info[0].capacity > 0x100000000 )
			{
				if (!strncmp("apps",mpi->name,4))
				{
					mpi->size = emmc_info[0].capacity - mpi->offset;
				}
			}
			
			emmc_erase(mpi->offset,mpi->size);
			mpi->mask_flags &= ~PART_FLG_ERASE;
			updated = 1;
		}
	}
	if (updated)
		save_partinfo();

	return 0;
}

int erase_datapart(struct partmap_info *pmi)
{
	struct partition_info *mpi = NULL;
	struct partition_info *oldmpi = NULL;
	int i, j;

	for(i=0; i<pmi->npartition; i++) {
		mpi = &(pmi->partition[i]);
		if(mpi->mask_flags & PART_FLG_DATA) {
			// find old mpi
			printf("[%02d] %s part is data part\n", i, mpi->name);
			j = get_partition_idx(mpi->name);
			if(j != 0 && (oldmpi = GET_PART_INFO(j))) {
				if((oldmpi->offset == mpi->offset) && (oldmpi->size == mpi->size)) {
					printf("[%s] no need of partition erasing\n", mpi->name);
					continue;
				}
			}

			if ( emmc_info[0].capacity > 0x100000000 )
			{
				if (!strncmp("apps",mpi->name,4))
				{
					mpi->size = emmc_info[0].capacity - mpi->offset;
				}
			}
			if (strncmp("apps", mpi->name, 4) == 0)
			{
				//bacaouse of offset overflow
				//when emmc driver support 64bit, it have to be changec
				printf("skip apps remove");
				//storage_erase(mpi->offset, 0x80000);
			}
			else
			{		
				storage_erase(mpi->offset, mpi->size);
			}
			printf("storage erased : ofs=0x%01x%08x ~ 0x%01x%08x\n",
				U64_UPPER(mpi->offset),U64_LOWER(mpi->offset),
				U64_UPPER(mpi->offset+mpi->size),U64_LOWER(mpi->offset+mpi->size));
		}
	}

	return 0;
}

int erase_cachepart(void)
{
#if 0 //def CC_EMMC_BOOT
	printf("%s is not implemented for emmc\n", __FUNCTION__);
#else
	struct partition_info *mpi = NULL;
	int i=0;

	for(i=0; i<GET_PART_NUM(); i++) {
		mpi = GET_PART_INFO(i);

		if(mpi->mask_flags & PART_FLG_CACHE) {
			printf("PART [%02d] %s\t part is cache part\n", i, mpi->name);
			storage_erase(mpi->offset, mpi->size);
			printf("storage erased : ofs=0x%01x%08x ~ 0x%01x%08x\n",
					U64_UPPER(mpi->offset),U64_LOWER(mpi->offset), 
					U64_UPPER(mpi->offset+mpi->size),U64_LOWER(mpi->offset+mpi->size));
		}
	}
#endif

	return 0;
}

int get_offset_size(int argc, char* argv[], unsigned long* offset, unsigned long* size)
{
	char *endptr;

	if(argc == 0) return -1;

	*offset = simple_strtoul(argv[0], &endptr, 0);
	if(*endptr == '\0' && argc == 1)	// if not partition name
		*size = 0;

	if(*endptr != '\0')		// partition name
	{
		struct partition_info *pi;
		if((pi = get_used_partition(argv[0])) == NULL)
		{
			if((pi = get_unused_partition(argv[0])) == NULL)
			{
				printf("'%s' partition is not exist\n", argv[0]);
				return -1;
			}
		}
		*offset = pi->offset;
		*size = pi->size;
	}

	if(argc > 1)
		*size = simple_strtoul(argv[1], NULL, 0);

	return 0;
}

int get_offset_size2(int argc, char* argv[], unsigned long long* offset, unsigned long long* size)
{
	char *endptr;

	if(argc == 0) return -1;

	*offset = simple_strtoull(argv[0], &endptr, 0);
	if(*endptr == '\0' && argc == 1)	// if not partition name
		*size = 0;

	if(*endptr != '\0')		// partition name
	{
		struct partition_info *pi;
		if((pi = get_used_partition(argv[0])) == NULL)
		{
			if((pi = get_unused_partition(argv[0])) == NULL)
			{
				printf("'%s' partition is not exist\n", argv[0]);
				return -1;
			}
		}
		*offset = pi->offset;
		*size = pi->size;
	}

	if(argc > 1)
		*size = simple_strtoull(argv[1], NULL, 0);

	return 0;
}

