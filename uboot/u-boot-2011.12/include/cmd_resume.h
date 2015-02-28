#ifndef _LG_CMD_RESUME_H_
#define _LG_CMD_RESUME_H_


/* Options */
#define USE_COMPRESS
//#define CRC_CHECK
// LM14, #define USE_DMA
//#define DEBUG_DMA
#define SECURE_BOOT

#define CRCPOLY_LE 0xedb88320

#define HIB_PART_NAME "hib"
#define SNAP_PART_NAME            "hib"
#define SNAPSHOT_PARTIAL_VERIFY
//#define SNAPSHOT_VERIFY
//#define CONFIG_SECURITY_BOOT

#define NUMBER_OF_FRAGMENT        32
#define FRAGMENT_UNIT_SIZE        128*1024  //128KByte
#define SIGNATURE_SIZE			  32

#define LGSNAP_COLDBOOT 0
#define LGSNAP_MAKING_IMAGE 1
#define LGSNAP_RESUME 2
#define USB_PORT0 0
#define USB_PORT1 1
#define USB_PORT2 2
#define USB_PORT3 4

//#define SNAPSHOT_IMAGE_LOAD_ADDR 0x2E400000

/*
 * These structures and MACROs must be synchronized with kernel/power/power.h
 */
#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define HEADER_SIZE PAGE_SIZE

#define ALIGN(x, a)             __ALIGN_MASK((x), (typeof(x)) (a) - 1)
#define __ALIGN_MASK(x, mask)   (((x) + (mask)) & ~(mask))

#define GET_SNAP_HEADER(si) ((void *)((unsigned int)si + PAGE_SIZE - ALIGN(sizeof(struct snapshot_header), 32)))


/* for version code */
#define LG_SNAPSHOT_MAGIC_CODE 0x03160326

#define PART_TYPE_NAME_LEN 20
#define VALUE_NAME_LEN 20
#define MAX_DEP_PARTS 10
#define MAX_PART_VALUES 2
#define MAX_MERGE_COUNT 64 // max page merge count for compression, it should be synchronized kernel/power/rawdev_snapshot_io.c

enum compress_algorithm {
	UNCOMPRESSED,
	LZO,
	LZ4HC,
	LZ4,
};


#define __NEW_UTS_LEN 64
struct new_utsname {
	char sysname[__NEW_UTS_LEN +1 ];
	char nodename[__NEW_UTS_LEN + 1];
	char release[__NEW_UTS_LEN + 1];
	char version[__NEW_UTS_LEN + 1];
	char machine[__NEW_UTS_LEN + 1];
	char domainname[__NEW_UTS_LEN + 1];
};

struct swsusp_info {
	struct new_utsname	uts;
	u32			version_code;
	unsigned long		num_physpages;
	int			cpus;
	unsigned long		image_pages;
	unsigned long		pages;
	unsigned long		size;
} __attribute__((aligned(PAGE_SIZE)));


struct dep_parts {
	char part_type[PART_TYPE_NAME_LEN];
	char value_name[MAX_PART_VALUES][VALUE_NAME_LEN];
	int nr_dep_parts;

	struct dep_part_info {
		int partnum;
		unsigned int value[MAX_PART_VALUES];
	} dep_part_info[MAX_DEP_PARTS];
};

struct snapshot_header {
	unsigned long magic;
	enum compress_algorithm compress_algo;
	unsigned long image_size;
	unsigned int  pfn_mi_cnt;
	unsigned long metadata_start_offset_for_comp;
	unsigned int crc;
	unsigned long pa_resume_func;
	struct dep_parts deps;
};

/* For adjacent pfn merge */
struct merge_block_info {
	unsigned long merged_cnt;
	unsigned long compblock_len;
};

struct pfn_merge_info {
	unsigned int start_pfn;
	unsigned int compressed;
	struct merge_block_info info;
};


typedef struct storage_partition
{
//	char			name[PART_NAME_STR_LEN];	/* identifier string                               */
	u64				offset;						/* offset within the master MTD space              */
	u64				size;						/* partition size                                  */
	u64				filesize;					/* file size                                       */
	uint8_t			used;
	uint8_t			valid;
} storage_partition_t;



u32 _crc32_le(u32 crc, unsigned char const *p, size_t len);
void print_swsusp_resume_val(void);
int do_hib(void);
int check_snapshot_mode(void);


void remake_hib(void);

#endif

