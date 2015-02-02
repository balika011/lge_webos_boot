#ifndef _PART_INFO_H_
#define _PART_INFO_H_

#ifndef NO
#define NO							0x00
#define YES							0x01
#endif

#undef BIG_ENDIAN
#ifdef BIG_ENDIAN
#define SWAP32(x)					((x & 0x000000FF) << 24) | ((x & 0x0000FF00) << 8) | \
									((x & 0x00FF0000) >> 8) | ((x & 0xFF000000) >> 24)
#else
#define SWAP32(x)					x
#endif

#define DEFAULT_STORAGE_SIZE		(0x100000000)	//4GB
#define CONFIG_EMMC_BASE			(0x00000000)
#define CONFIG_EMMC_SIZE			(0xE9000000)	// in case of SAMSUNG 4GB
#define CONFIG_EMMC_RSVD_SIZE		(0x17000000)
/* magic number */
#define PARTMAP_MAGIC					(0x20120716)

/* partinfo version */
#define PARTINFO_VER					(0x0)

/*-----------------------------------------------------------------------------
 * partition info
 */
#define PART_FLG_FIXED				1
#define PART_FLG_MASTER				2
#define PART_FLG_IDKEY				4
#define PART_FLG_CACHE				8
#define PART_FLG_DATA					16
#define PART_FLG_SECURED				32
#define PART_FLG_ERASE				64

#define STR_LEN_MAX					32
#define PARTITION_MAX				128

typedef enum PART_INFO {
	PART_INFO_IDX	= 0,
	PART_INFO_OFFSET,
	PART_INFO_SIZE,
	PART_INFO_FILESIZE
} PART_INFO_TYPE;

typedef struct device_info
{
	char			name[STR_LEN_MAX];
	unsigned long long	size;
	unsigned long long 	phys;
	void			*virt;
	void			*cached;
	int				bankwidth;
	unsigned int	used;
};

typedef struct partition_info
{
	char			name[STR_LEN_MAX];			/* identifier string                               */
	unsigned long long	offset;						/* offset within the master MTD space              */
	unsigned long long	size;						/* partition size                                  */
	char			filename[STR_LEN_MAX];		/* file name                                       */
	unsigned int	filesize;					/* file size                                       */
	unsigned int	sw_ver;						/* software version                                */
	unsigned char	used;						/* Is this partition is used?                      */
	unsigned char	valid;						/* Is this partition is valid?                     */
	unsigned int	mask_flags;					/* master MTD flags to mask out for this partition */
};

typedef struct partmap_info
{
	unsigned int					magic;
	unsigned int					cur_epk_ver;
	unsigned int					old_epk_ver;
	unsigned char 					npartition;
	struct device_info				dev;
	struct partition_info			partition[PARTITION_MAX];
};

extern struct partmap_info			partinfo;



#define DEFAULT_SECUREBOOT_BASE     (CONFIG_EMMC_BASE)
#define DEFAULT_SECUREBOOT_SIZE     (0x80000)

#define DEFAULT_BOOT__1_BASE        (DEFAULT_SECUREBOOT_BASE + DEFAULT_SECUREBOOT_SIZE)
#define DEFAULT_BOOT__1_SIZE        (0xC0000)

#define DEFAULT_PARTINF_BASE        (DEFAULT_BOOT__1_BASE + DEFAULT_BOOT__1_SIZE)
#define DEFAULT_PARTINF_SIZE        (0x40000)

#define DEFAULT_MAPBAK__BASE        (DEFAULT_PARTINF_BASE + DEFAULT_PARTINF_SIZE)
#define DEFAULT_MAPBAK__SIZE        (0x40000)

#define DEFAULT_BOOT__2_BASE        (DEFAULT_MAPBAK__BASE + DEFAULT_MAPBAK__SIZE)
#define DEFAULT_BOOT__2_SIZE        (DEFAULT_BOOT__1_SIZE)

#define DEFAULT_SWUE__1_BASE        (DEFAULT_BOOT__2_BASE + DEFAULT_BOOT__2_SIZE)
#define DEFAULT_SWUE__1_SIZE        (0x1400000)
#define DEFAULT_SWUE__2_BASE        (DEFAULT_SWUE__1_BASE + DEFAULT_SWUE__1_SIZE)
#define DEFAULT_SWUE__2_SIZE        (DEFAULT_SWUE__1_SIZE)
#define DEFAULT_CRC32___BASE        (DEFAULT_SWUE__2_BASE + DEFAULT_SWUE__2_SIZE)
#define DEFAULT_EMERG___BASE        (DEFAULT_SWUE__2_BASE + DEFAULT_SWUE__2_SIZE)
#define DEFAULT_EMERG___SIZE        (0x40000)
#define DEFAULT_CRC32___BASE        (DEFAULT_EMERG___BASE + DEFAULT_EMERG___SIZE)
#define DEFAULT_CRC32___SIZE        (0x40000)
#define DEFAULT_MACADR__BASE        (DEFAULT_CRC32___BASE + DEFAULT_CRC32___SIZE)
#define DEFAULT_MACADR__SIZE        (0x40000)
#define DEFAULT_ENV_NVM_BASE        (DEFAULT_MACADR__BASE + DEFAULT_MACADR__SIZE)
#define DEFAULT_ENV_NVM_SIZE        (0x40000)
#define DEFAULT_REMAIN__BASE        (DEFAULT_ENV_NVM_BASE + DEFAULT_ENV_NVM_SIZE)
#define DEFAULT_REMAIN__SIZE        (DEFAULT_RESERVD_BASE - DEFAULT_REMAIN__BASE)
#define DEFAULT_RESERVD_BASE        (DEFAULT_STORAGE_SIZE - CONFIG_EMMC_RSVD_SIZE)
#define DEFAULT_RESERVD_SIZE        (CONFIG_EMMC_RSVD_SIZE)


#define DEFAULT_DEVICE            {                                                            \
    .name        = "mt53xx-emmc",                                                               \
    .size        = SWAP32(DEFAULT_STORAGE_SIZE),    .phys        = 0,                            \
    .virt        = 0x0,                             .cached      = 0x0,                          \
    .bankwidth   = 0x0,                             .used        = SWAP32(YES)                   \
}

#define DEFAULT_PTT_SECUREBOOT    {                                                            \
    .name        = "secureboot",                                                                \
    .offset      = SWAP32(DEFAULT_SECUREBOOT_BASE), .size        = SWAP32(DEFAULT_SECUREBOOT_SIZE), \
    .filename    = "secureboot.bin",                .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER | PART_FLG_FIXED)                                           \
}

#define DEFAULT_PTT_BOOT_1        {                                                            \
    .name        = "boot",                                                                      \
    .offset      = SWAP32(DEFAULT_BOOT__1_BASE),    .size        = SWAP32(DEFAULT_BOOT__1_SIZE), \
    .filename    = "boot.bin",                      .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER | PART_FLG_FIXED)                                           \
}

#define DEFAULT_PTT_PARTINF       {                                                            \
    .name        = "partinfo",                                                                  \
    .offset      = SWAP32(DEFAULT_PARTINF_BASE),    .size        = SWAP32(DEFAULT_PARTINF_SIZE), \
    .filename    = "PART.INFO",                     .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER | PART_FLG_FIXED)                                           \
}

#define DEFAULT_PTT_MAPBAK        {                                                            \
    .name        = "mapbak",                                                                    \
    .offset      = SWAP32(DEFAULT_MAPBAK__BASE),    .size        = SWAP32(DEFAULT_MAPBAK__SIZE), \
    .filename    = "PART.INFO",                     .filesize    = 0x0,                          \
    .sw_ver      = SWAP32(PARTINFO_VER),                                                        \
    .used        = NO,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER | PART_FLG_FIXED)                                           \
}
#define DEFAULT_PTT_BOOT_2        {                                                            \
    .name        = "boot",                                                                      \
    .offset      = SWAP32(DEFAULT_BOOT__2_BASE),    .size        = SWAP32(DEFAULT_BOOT__2_SIZE), \
    .filename    = "boot.bin",                      .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER | PART_FLG_FIXED)                                           \
}
#define DEFAULT_PTT_SWUE_1        {                                                            \
    .name        = "swue",                                                                      \
    .offset      = SWAP32(DEFAULT_SWUE__1_BASE),    .size        = SWAP32(DEFAULT_SWUE__1_SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER)                                                            \
}
#define DEFAULT_PTT_SWUE_2        {                                                            \
    .name        = "swue",                                                                      \
    .offset      = SWAP32(DEFAULT_SWUE__2_BASE),    .size        = SWAP32(DEFAULT_SWUE__2_SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = YES,                          \
    .mask_flags  = 0                                                                            \
}
#define DEFAULT_PTT_CRC32         {                                                            \
    .name        = "crc32info",                                                                 \
    .offset      = SWAP32(DEFAULT_CRC32___BASE),    .size        = SWAP32(DEFAULT_CRC32___SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = YES,                          \
    .mask_flags  = (PART_FLG_MASTER)                                                            \
}
#define DEFAULT_PTT_MACADR        {                                                            \
    .name        = "macadr",                                                                    \
    .offset      = SWAP32(DEFAULT_MACADR__BASE),    .size        = SWAP32(DEFAULT_MACADR__SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = NO,                           \
    .mask_flags  = (PART_FLG_FIXED)                                                             \
}
#define DEFAULT_PTT_ENV_NVM       {                                                            \
    .name        = "env_nvm",                                                                   \
    .offset      = SWAP32(DEFAULT_ENV_NVM_BASE),    .size        = SWAP32(DEFAULT_ENV_NVM_SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_FIXED)                                                             \
}

#define DEFAULT_PTT_REMAIN        {                                                            \
    .name        = "remain",                                                                    \
    .offset      = SWAP32(DEFAULT_REMAIN__BASE),    .size        = SWAP32(DEFAULT_REMAIN__SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = YES,                          \
    .mask_flags  = (PART_FLG_FIXED)                                                             \
}

#define DEFAULT_PTT_HIST          {                                                            \
    .name        = "hist",                                                                      \
    .offset      = SWAP32(DEFAULT_HIST____BASE),    .size        = SWAP32(DEFAULT_HIST____SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = YES,                             .valid       = NO,                           \
    .mask_flags  = (PART_FLG_FIXED)                                                             \
}



#define DEFAULT_PTT_RESERVED      {                                                            \
    .name        = "reserved",                                                                  \
    .offset      = SWAP32(DEFAULT_RESERVD_BASE),    .size        = SWAP32(DEFAULT_RESERVD_SIZE), \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = NO,                           \
    .mask_flags  = (PART_FLG_FIXED)                                                             \
}

#define DEFAULT_PTT_NULL          {                                                            \
    .name        = "",                                                                          \
    .offset      = 0x0000000,                       .size        = 0x0,                          \
    .filename    = "",                              .filesize    = 0x0,                          \
    .sw_ver      = 0x0,                                                                         \
    .used        = NO,                              .valid       = NO,                           \
    .mask_flags  = 0                                                                            \
}


#define DEFAULT_PARTITION         {              \
    DEFAULT_PTT_SECUREBOOT,                      \
    DEFAULT_PTT_BOOT_1,                          \
    DEFAULT_PTT_PARTINF,                         \
    DEFAULT_PTT_MAPBAK,                          \
    DEFAULT_PTT_BOOT_2,                          \
    DEFAULT_PTT_MACADR,                          \
    DEFAULT_PTT_ENV_NVM,                         \
    DEFAULT_PTT_REMAIN,                          \
    DEFAULT_PTT_RESERVED,                        \
    DEFAULT_PTT_NULL                             \
}
#define DEFAULT_NUM_OF_PARTITION                 9


#define DEFAULT_PARTMAP_INFO			{		\
	.magic			= SWAP32(PARTMAP_MAGIC),	\
	.cur_epk_ver		= 0, 				\
	.old_epk_ver		= 0, 				\
	.npartition		= DEFAULT_NUM_OF_PARTITION,	\
	.dev			= DEFAULT_DEVICE, \
	.partition		= DEFAULT_PARTITION, \
}

#define GET_PARTMAP_INFO()		((struct partmap_info *)&(partinfo))
#define GET_DEVICE_INFO()			((struct device_info *)&(partinfo.dev))
#define GET_PART_INFO(x)			((struct partition_info *)&(partinfo.partition[x]))
#define GET_PART_NUM()				(partinfo.npartition)

extern struct partmap_info			partinfo;

extern struct partition_info *		get_partition(int idx);
extern struct partition_info *		get_used_partition(const char *name);
extern struct partition_info *		get_partition_by_name(const char *name);
extern int							get_partition_idx(const char *name);
extern struct partition_info *		get_unused_partition(const char *name);
extern unsigned int					swap_partition(struct partition_info *mpi1, struct partition_info *mpi2);
extern unsigned int					save_partinfo(void);
extern unsigned int					load_partinfo(void);
extern int							init_partinfo(void);
extern int 							backup_partition(struct partition_info *pi1, struct partition_info *pi2);

extern int 							erase_flagged_part(struct partmap_info *pmi);
extern int 							erase_datapart(struct partmap_info *pmi);
extern int 							erase_cachepart(void);
extern int 							get_offset_size(int argc, char* argv[], unsigned long* offset, unsigned long* size);
extern int 							get_offset_size2(int argc, char* argv[], unsigned long long* offset, unsigned long long* size);

#endif /* _PART_INFO_H_ */

