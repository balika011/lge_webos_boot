
#include "x_printf.h"
#include "x_typedef.h"
#include "Lpartinfo.h"
#include "msdc_if.h"

#define U64_UPPER(x)	(unsigned long)( x >> 32 )
#define U64_LOWER(x)	(unsigned long)( x & 0xffffffff)

struct partmap_info partinfo;

//crc
#define CRC32_POLY	0x04c11db7		/* AUTODIN II, Ethernet, & FDDI */
static UINT32 crc32_table[256];

static void init_crc32(void)
{
	INT32	i, j;
	UINT32	c;

	for (i = 0; i < 256; ++i) {
		for (c = i << 24, j = 8; j > 0; --j)
			c = c & 0x80000000 ? (c << 1) ^ CRC32_POLY : (c << 1);
		crc32_table[i] = c;
	}
}

UINT32 calc_crc32(UINT8 *buf, UINT32 len)
{
	UINT8               *p;
	UINT32              crc;
	static  UINT32      fTableOk=0;
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
static const CHAR *menu_partition_str[] = {
	"\nPartition Information ---------------------------------------------------------------------------------\n\n",
	"index[%d] - ",
	"cur epk ver : 0x%06x\n",
	"old epk ver : 0x%06x\n",
	"\t name	= %s",
	"\t offset	= 0x%08x",
	"\t size	= 0x%08x",
	"\t sw_ver = %d",
	"\t filename = %s",
	"\t flags = %s\n",
	"\t used = %c",
	"\t valid = %c"
};

static const CHAR *menu_partition_info[] =
{
	"[%2d] \"%-12s\" : 0x%01x%08x-0x%01x%08x (0x%01x%08x)",
	" %c%c%c%c%c",
	" : \"%-20s\"[%d] - 0x%06x : (%c/%c) [%3d\%]\n",
	"[%2d] Empty\n"
};
UINT32 print_partinfo(void)
{
	struct partition_info *pi = NULL;
	int i=0;

	Printf(menu_partition_str[0]);
//	Printf("magic : %08x", partinfo.magic);
//	println();
	Printf(menu_partition_str[2], partinfo.cur_epk_ver); 
	Printf(menu_partition_str[3], partinfo.old_epk_ver); 

	if (partinfo.npartition > PARTITION_MAX) {
		Printf ("[ERROR] Number of partition is %d\n", partinfo.npartition);
		return (unsigned int)-1;
	}

	for(i=0; i<(partinfo.npartition); i++) {
		pi = GET_PART_INFO(i);

		Printf(menu_partition_info[0], i, pi->name,
				U64_UPPER(pi->offset), U64_LOWER(pi->offset), U64_UPPER(pi->offset+pi->size), U64_LOWER(pi->offset+pi->size), 
				U64_UPPER(pi->size), U64_LOWER(pi->size));

		Printf(menu_partition_info[1],
				(pi->mask_flags & PART_FLG_FIXED)	? 'F' : '-',
				(pi->mask_flags & PART_FLG_MASTER)	? 'M' : '-',
				(pi->mask_flags & PART_FLG_SECURED)	? 'S' : '-',
				(pi->mask_flags & PART_FLG_IDKEY)	? 'I' : '-',
				(pi->mask_flags & PART_FLG_CACHE)	? 'C' : 
				((pi->mask_flags & PART_FLG_DATA)	? 'D' : '-')
			  );

		if(pi->mask_flags & PART_FLG_ERASE) 	Printf("*");

		if(strlen(pi->filename)) {
			Printf(menu_partition_info[2],
					pi->filename, pi->filesize, pi->sw_ver,
					pi->used  ? 'U' : 'u',
					pi->valid ? 'V' : 'v', (unsigned long)((double) pi->filesize/pi->size * 100)  );
		}
		else {
			Printf ("\n");
		}
	}

	return 0;
}

struct partition_info * get_partition(INT32 idx)
{
	if( (idx >= 0) && (idx < partinfo.npartition))
		return GET_PART_INFO(idx);

	return (struct partition_info*)(NULL);
}

/* returned partition_info that is used and valid */
struct partition_info * get_used_partition(const CHAR *name)
{
	struct partition_info *pi = NULL;
	UINT32 idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if(pi->used && (strncmp(name, pi->name, 4) == 0))
			return pi;
	}while(++idx < GET_PART_NUM());

	return (struct partition_info*)(NULL);
}

/* returned partition_info that is used and valid */
struct partition_info * get_partition_by_name(const CHAR *name)
{
	struct partition_info *pi = NULL;
	UINT32 idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if((strncmp(name, pi->name, 4) == 0))
			return pi;
	}while(++idx < GET_PART_NUM());

	return (struct partition_info*)(NULL);
}

INT32 get_partition_idx(const CHAR *name)
{
	struct partition_info *pi = NULL;
	UINT32 idx = 0;

	do {
		pi = GET_PART_INFO(idx);
		if(pi->used && pi->valid && (strncmp(name, pi->name, 4) == 0))
			return idx;
	}while(++idx < GET_PART_NUM());

	return -1;
}
//if CRC fail, return directly
//will NOT use mapbak, just leave these complex proc to uboot
UINT32 load_partinfo(void)
{
	INT32  ret = 0;
	UINT32 size = 0;
	UINT32 crc = 0, calcrc = 0;	
	INT32 crc_offset = 4;
	UINT8 *offset = NULL;
	//UINT8 *buf = NULL;

	size = sizeof(struct partmap_info);
	Printf ("size of partmap_info %d\n", size);
// 0. check crc of partinfo
#if 1
	if(!offset){
		offset = x_mem_alloc(size + crc_offset);
		if(offset == NULL){
			Printf("x_mem_alloc failed");
			return -1;
		}
	}
	ret = MSDCPART_Read((UINT64)DEFAULT_PARTINF_BASE, offset, size + crc_offset);

	if(ret) {
		Printf("emmc read failed..--;;\n");
		x_mem_free(offset);
		return ret;
	}
	x_memcpy((void*)&crc, (void*)(offset + size), crc_offset);

	if((calcrc = calc_crc32(offset, size)) != crc)
	{
		//just read in mtk loader, not to read mapbak and copy to partinfo.
		#if 1
		Printf("\t partinfo CRC check failed [%x/%x], and NOT to read mapbak and copy to partinfo in mtk-loader\n", calcrc, crc);
		x_mem_free(offset);
		offset = NULL;
		return -1;
		#else 
		Printf("\t CRC check failed [%x/%x], so read mapbak and copy to partinfo \n", calcrc, crc);
		if(!buf){
			buf = x_mem_alloc(size + crc_offset);
			if(buf == NULL){ 
				Printf("x_mem_alloc failed");
				return -1;
			}
		}
		ret = storage_read((ulong)DEFAULT_MAPBAK__BASE, size + crc_offset, buf);
		if(ret) {
			Printf("emmc read failed..--;;\n");
			return ret;
		}

		ret = storage_write(DEFAULT_PARTINF_BASE, size + crc_offset, buf);
		if(ret) {
			Printf("emmc write failed..--;;\n");
			return ret;
		}
		x_mem_free(buf);
		#endif
	}

	Printf("\t partinfo CRC check OK [calcrc:%x/crc:%x]\n", calcrc, crc);
#endif

// 1. read partinfo
	//no need to load again, copy from DRAM to DRAM
	x_memcpy((void *)&partinfo, offset, size);
	//no need//ret = MSDCPART_Read((UINT64)DEFAULT_PARTINF_BASE, (void *)&partinfo, size);

	x_mem_free(offset);	
	return 0;
}
//just load default map, not to modify due to emmc size
UINT32 load_default_partinfo(void)
{
	struct partmap_info default_partinfo = DEFAULT_PARTMAP_INFO;
	struct device_info default_deviceinfo = DEFAULT_DEVICE;
	struct partition_info default_partitioninfo[PARTITION_MAX] = DEFAULT_PARTITION;
	x_memcpy((void*)(&(default_partinfo.dev)), (void*)(&default_deviceinfo), sizeof(default_deviceinfo));
	x_memcpy((void*)(default_partinfo.partition), (void*)(default_partitioninfo), sizeof(default_partitioninfo));

	x_memcpy((void *)&partinfo, (void*)&default_partinfo, sizeof(default_partinfo));
	return 0;
}

INT32 init_partinfo(void)
{
	static INT32 _binit = 0;

	if (_binit)
		return 0;

	if(load_partinfo()) {
		load_default_partinfo();
	}
//	else {
//		print_partinfo();
//		erase_flagged_part(&partinfo); //just read in mtk-loader
//	}
	
	_binit = 1;
//	print_partinfo();

	return 0;
}

