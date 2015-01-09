#ifndef _MTD_INFO_H_
#define _MTD_INFO_H_

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

/* magic number */
#define CONFIG_MAGIC				0x20110620

/*-----------------------------------------------------------------------------
 * map info
 */
#define FLASH_BASE					0x00000000
#define FLASH_SIZE					(1024*1024*1024)
#define FLASH_BANKWIDTH 			8

/*-----------------------------------------------------------------------------
 * mtd partition info
 */
#define MTD_FLG_FIXED				1
#define MTD_FLG_MASTER				2
#define MTD_FLG_IDKEY				4
#define MTD_FLG_CACHE				8
#define MTD_FLG_DATA				16

#define MTD_STR_LEN					32
#define MTD_MAP_MAX					1
#define MTD_PARTITION_MAX			64

typedef enum PART_INFO {
	PART_INFO_IDX	= 0,
	PART_INFO_OFFSET,
	PART_INFO_SIZE,
	PART_INFO_FILESIZE
} PART_INFO_T;

struct mtd_map_info {
	char name[MTD_STR_LEN];
	unsigned int size;
	unsigned int phys;
	void *virt;
	void *cached;
	int bankwidth;
	unsigned int used;
};

struct mtd_partition_info {
	char name[MTD_STR_LEN];				/* identifier string */
	unsigned int offset;				/* offset within the master MTD space */
	unsigned int size;					/* partition size */
	char filename[MTD_STR_LEN];			/* file name */
	unsigned int filesize;				/* file size */
	unsigned int sw_ver;				/* software version */
	unsigned char used;					/* Is this partition is used? */
	unsigned char valid;				/* Is this partition is valid? */
	unsigned int mask_flags;			/* master MTD flags to mask out for this partition */
};

struct boot_mtd_info {
	unsigned int					magic;
	unsigned int					cur_epk_ver;
	unsigned int					old_epk_ver;
	unsigned char 					npartition;
	struct mtd_map_info				map[MTD_MAP_MAX];
	struct mtd_partition_info		partition[MTD_PARTITION_MAX];
};

extern struct boot_mtd_info			mtdinfo;

#define GET_MTD_INFO()				((struct boot_mtd_info *)&(mtdinfo))
#define GET_MAP_INFO(x)				((struct mtd_map_info *)&(mtdinfo.map[0]))
#define GET_PARTITION_INFO(x)		((struct mtd_partition_info *)&(mtdinfo.partition[x]))
#define GET_PARTITION_NUM()			(mtdinfo.npartition)


#define DEFAULT_BOOT_1__BASE		(FLASH_BASE)
#define DEFAULT_BOOT_1__SIZE		(0x100000)

#define DEFAULT_MTDINFO_BASE		(DEFAULT_BOOT_1__BASE + DEFAULT_BOOT_1__SIZE)
#define DEFAULT_MTDINFO_SIZE		(0x80000)

//#define DEFAULT_BOOT_2__BASE		(DEFAULT_MTDINFO_BASE + DEFAULT_MTDINFO_SIZE)
//#define DEFAULT_BOOT_2__SIZE		(DEFAULT_BOOT_1__SIZE)

#define DEFAULT_HIST____BASE		(DEFAULT_BBMINFO_BASE - 0x180000)
#define DEFAULT_HIST____SIZE		(0x80000)

#define DEFAULT_ENV_NVM_BASE		(DEFAULT_HIST____BASE + DEFAULT_HIST____SIZE)
#define DEFAULT_ENV_NVM_SIZE		(0x80000)

#define DEFAULT_MACADR__BASE		(DEFAULT_ENV_NVM_BASE + DEFAULT_ENV_NVM_SIZE)
#define DEFAULT_MACADR__SIZE		(0x80000)

#define DEFAULT_BBMINFO_BASE		(DEFAULT_RESERVD_BASE - DEFAULT_BBMINFO_SIZE)
#define DEFAULT_BBMINFO_SIZE		(0x400000)

#define DEFAULT_RESERVD_BASE		(FLASH_SIZE - DEFAULT_RESERVD_SIZE)
#define DEFAULT_RESERVD_SIZE		(FLASH_SIZE >> 5)


#if 0
#define DEFAULT_BOOT1___BASE		(FLASH_BASE)
#define DEFAULT_BOOT1___SIZE		(0x80000)

#define DEFAULT_BBMINFO_BASE		(DEFAULT_BOOT1___BASE + DEFAULT_BOOT1___SIZE)
#define DEFAULT_BBMINFO_SIZE		(0x40000)

#define DEFAULT_MTDINFO_BASE		(DEFAULT_BBMINFO_BASE + DEFAULT_BBMINFO_SIZE + 0x40000) //0xa0000 ~ 0xc0000 : env area
#define DEFAULT_MTDINFO_SIZE		(0x40000)

#define DEFAULT_ROOTFS__BASE		(DEFAULT_MTDINFO_BASE + DEFAULT_MTDINFO_SIZE)
#define DEFAULT_ROOTFS__SIZE		(0x700000)

#define DEFAULT_LGINIT__BASE		(DEFAULT_ROOTFS__BASE + DEFAULT_ROOTFS__SIZE)
#define DEFAULT_LGINIT__SIZE		(0x80000)

#define DEFAULT_KERNEL__BASE		(DEFAULT_LGINIT__BASE + DEFAULT_LGINIT__SIZE)
#define DEFAULT_KERNEL__SIZE		(0x600000)


//#define DEFAULT_BOOT2___BASE		(DEFAULT_LGINIT__BASE + DEFAULT_LGINIT__SIZE)
//#define DEFAULT_BOOT2___SIZE		(0x80000)
#endif


#define DEFAULT_MAP_0				{														\
	name		: "mstar_map0",																\
	size		: SWAP32(FLASH_SIZE),			phys		: SWAP32(FLASH_BASE),			\
	virt		: 0x0, 							cached		: 0x0,							\
	bankwidth	: SWAP32(FLASH_BANKWIDTH),		used		: SWAP32(YES)					\
}

#define DEFAULT_EMPTY_MAP			{														\
	name		: " ",																		\
	size		: 0x0,							phys		: 0x0,							\
	virt		: 0x0, 							cached		: 0x0,							\
	bankwidth	: 0x0,							used		: SWAP32(NO)					\
}

#define DEFAULT_MAP_INFO			{														\
	DEFAULT_MAP_0,																			\
	DEFAULT_EMPTY_MAP																		\
}

#if 0
#define DEFAULT_PTT_BBMINFO			{														\
	name		: "bbminfo",																\
	offset		: SWAP32(DEFAULT_BBMINFO_BASE),	size		: SWAP32(DEFAULT_BBMINFO_SIZE), \
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: NO,							\
	mask_flags	: 0 																		\
}

#define DEFAULT_PTT_BOOT1			{														\
	name		: "boot",																	\
	offset		: SWAP32(DEFAULT_BOOT1___BASE),	size		: SWAP32(DEFAULT_BOOT1___SIZE),	\
	filename	: "uboot.bin",			filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: 0 																		\
}

#define DEFAULT_PTT_MTDINFO 		{														\
	name		: "mtdinfo",																\
	offset		: SWAP32(DEFAULT_MTDINFO_BASE),	size		: SWAP32(DEFAULT_MTDINFO_SIZE),	\
	filename	: "MTD.INFO",					filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: 0 																		\
}

#define DEFAULT_PTT_ROOTFS			{														\
	name		: "rootfs", 																\
	offset		: SWAP32(DEFAULT_ROOTFS__BASE),	size		: SWAP32(DEFAULT_ROOTFS__SIZE),	\
	filename	: "rootfs.bin",			filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: 0 															\
}

#define DEFAULT_PTT_LGINIT			{														\
	name		: "lginit", 																\
	offset		: SWAP32(DEFAULT_LGINIT__BASE),	size		: SWAP32(DEFAULT_LGINIT__SIZE),	\
	filename	: "lginit.squashfs",			filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: 0															\
}

#define DEFAULT_PTT_KERNEL			{														\
	name		: "kernel", 																\
	offset		: SWAP32(DEFAULT_KERNEL__BASE),	size		: SWAP32(DEFAULT_KERNEL__SIZE),	\
	filename	: "uImage",						filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: 0															\
}


#define DEFAULT_PTT_NULL			{														\
	name		: "",																		\
	offset		: 0x0000000,					size		: 0x0,							\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: NO,							\
	mask_flags	: 0 																		\
}
#endif

#define DEFAULT_PTT_BOOT_1			{														\
	name		: "boot",																	\
	offset		: SWAP32(DEFAULT_BOOT_1__BASE),size		: SWAP32(DEFAULT_BOOT_1__SIZE),\
	filename	: "boot.bin",					filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: (MTD_FLG_MASTER | MTD_FLG_FIXED)											\
}

#define DEFAULT_PTT_MTDINFO 		{														\
	name		: "mtdinfo",																\
	offset		: SWAP32(DEFAULT_MTDINFO_BASE),	size		: SWAP32(DEFAULT_MTDINFO_SIZE),	\
	filename	: "MTD.INFO",					filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: YES,							\
	mask_flags	: (MTD_FLG_MASTER | MTD_FLG_FIXED)											\
}

#define DEFAULT_PTT_HIST			{														\
	name		: "hist",																	\
	offset		: SWAP32(DEFAULT_HIST____BASE),	size		: SWAP32(DEFAULT_HIST____SIZE),	\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: YES,							valid		: NO,							\
	mask_flags	: (MTD_FLG_FIXED)															\
}

#define DEFAULT_PTT_ENV_NVM			{														\
	name		: "env_nvm",																\
	offset		: SWAP32(DEFAULT_ENV_NVM_BASE),	size		: SWAP32(DEFAULT_ENV_NVM_SIZE),	\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: NO,							\
	mask_flags	: (MTD_FLG_FIXED)															\
}

#define DEFAULT_PTT_MACADR			{														\
	name		: "macadr",																	\
	offset		: SWAP32(DEFAULT_MACADR__BASE),	size		: SWAP32(DEFAULT_MACADR__SIZE),	\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: NO,							\
	mask_flags	: (MTD_FLG_FIXED)															\
}

#define DEFAULT_PTT_BBMINFO			{														\
	name		: "bbminfo",																\
	offset		: SWAP32(DEFAULT_BBMINFO_BASE),	size		: SWAP32(DEFAULT_BBMINFO_SIZE),	\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: YES,							\
	mask_flags	: (MTD_FLG_FIXED)															\
}

#define DEFAULT_PTT_RESERVED		{														\
	name		: "reserved",																\
	offset		: SWAP32(DEFAULT_RESERVD_BASE),	size		: SWAP32(DEFAULT_RESERVD_SIZE),	\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: YES,							\
	mask_flags	: (MTD_FLG_FIXED)															\
}

#define DEFAULT_PTT_NULL			{														\
	name		: "",																		\
	offset		: 0x0000000,					size		: 0x0,							\
	filename	: "",							filesize	: 0x0,							\
	sw_ver		: 0x0,																		\
	used		: NO,							valid		: NO,							\
	mask_flags	: 0 																		\
}

#define DEFAULT_PARTITION			{			\
	DEFAULT_PTT_BOOT_1,							\
	DEFAULT_PTT_MTDINFO,						\
	DEFAULT_PTT_HIST,							\
	DEFAULT_PTT_ENV_NVM,						\
	DEFAULT_PTT_MACADR,							\
	DEFAULT_PTT_BBMINFO,						\
	DEFAULT_PTT_RESERVED,						\
	DEFAULT_PTT_NULL							\
}

#if 0
#define DEFAULT_PARTITION			{			\
	DEFAULT_PTT_BOOT1,							\
	DEFAULT_PTT_BBMINFO,						\
	DEFAULT_PTT_MTDINFO,						\
	DEFAULT_PTT_ROOTFS,							\
	DEFAULT_PTT_LGINIT,							\
	DEFAULT_PTT_KERNEL,							\
	DEFAULT_PTT_NULL							\
}
#endif

#define DEFAULT_NUM_OF_PARTITION				7

#define DEFAULT_MTD_INFO			{			\
	magic			: SWAP32(CONFIG_MAGIC),		\
	cur_epk_ver		: 0, 						\
	old_epk_ver		: 0, 						\
	npartition		: DEFAULT_NUM_OF_PARTITION,	\
	partition 		: DEFAULT_PARTITION			\
}

extern struct mtd_partition_info * get_mtdpartinfo(int idx);
extern struct mtd_partition_info * get_partition(char *name);
extern struct mtd_partition_info * get_unused_partition(char *name);
extern unsigned int swap_partition(struct mtd_partition_info *mpi1, struct mtd_partition_info *mpi2);
extern unsigned int save_mtdinfo(void);
extern unsigned int load_mtdinfo(void);

extern int erase_cachepart(void);

#endif /* MTD_INFO_H_ */
