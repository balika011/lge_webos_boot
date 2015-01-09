#ifndef _LG_CMD_RESUME_H_
#define _LG_CMD_RESUME_H_


/* Options */
#define USE_COMPRESS
//#define CRC_CHECK
#define USE_DMA
//#define DEBUG_DMA
#define SECURE_BOOT

/* Configurable var*/
#define IMAGE_LOAD_BUFFER_ADDR 0x18000000 // temporary address,

#define SB_AES_KEY_OFFSET (0x4000000 - 0x1000)	// for secure boot
#define SB_AES_KEY_SIZE		16
#define SB_HASH_OFFSET		0x1fff010
#define SB_HASH_SIZE		32

//#define DMA_READ_BUFFER_SIZE (32 * 1024)
#define DMA_READ_BUFFER_SIZE (64 * 1024)

#define __NEW_UTS_LEN 64
#define BM_END_OF_MAP (~0UL)

#define CRCPOLY_LE 0xedb88320 									// from kernel crc32
#define lzo1x_worst_compress(x) ((x) + ((x) / 16) + 64 + 3)		// from kernel lzo

#define HIB_PART_NAME "hib"

#define LGSNAP_COLDBOOT 0
#define LGSNAP_MAKING_IMAGE 1
#define LGSNAP_SNAPSHOTBOOT 2

/* kernel dependent structure */
struct new_utsname {
	char sysname[__NEW_UTS_LEN + 1];
	char nodename[__NEW_UTS_LEN + 1];
	char release[__NEW_UTS_LEN + 1];
	char version[__NEW_UTS_LEN + 1];
	char machine[__NEW_UTS_LEN + 1];
	char domainname[__NEW_UTS_LEN + 1];
};

/* kernel dependent structure, It must be synchronized with struct swsusp_info at kernel/power/power.h */
struct swsusp_info {
	struct new_utsname	uts;
	u32			version_code;
	unsigned long		num_physpages;
	int			cpus;
	unsigned long		image_pages;
	unsigned long		pages;
	unsigned long		size;

	// LG snapshoe metadata
	unsigned long saved_context_paddr;
	unsigned long in_suspend_paddr;
	unsigned long resume_func_vaddr;
	unsigned long compressed_image_size;
};

// own structure to track resume progress
struct swsusp_runtime_val
{
	unsigned int total_image_pages;
	unsigned int total_data_pages;
	unsigned int total_meta_pages;
	unsigned int left_data_pages;
	unsigned int left_meta_pages;
	unsigned int cur_meta_floffset;
};

u32 _crc32_le(u32 crc, unsigned char const *p, size_t len);
void print_swsusp_resume_val(void);
int read_swsusp_header(struct partition_info *mpi);
int do_hib(void);
int check_snapshot_mode(void);

#ifdef CONFIG_HIBERNATION
void remake_hib(void);
#else
#define remake_hib()
#endif

#endif

