#ifndef _EPK_H_
#define _EPK_H_

#define RELEASE_MODE		0
#define DEBUG_MODE 			1
#define TEST_MODE 			2
#define UNKNOWN_MODE		3

#define PAK_TYPE_ID_LEN		4
#define FILE_PATH_LEN		1024

/**********************************************************/
#define RAWIMAGE_MAGIC			"LGRAWIMG"
#define SECTOR_SIZE_BOOT		(0x200)
#define BOOT_AREA_SIZE			(0x20000)
#define HEAD_FILE_NAME			"tmp_rawImg_head"
/**********************************************************/

typedef unsigned char	UINT8;
typedef unsigned int	UINT32;

/* FILE TYPE */
typedef enum
{
	FILE_TYPE_PAK	= 0,
	FILE_TYPE_EPK,
	FILE_TYPE_UNKNOWN
} FILE_TYPE_T;

typedef struct
{
	UINT32				imageOffset;
	UINT32				imageSize;
} PAK_LOCATION_T;

// EPK를 해제 후 PAK Header 128 Bytes
typedef struct
{
	char				imageType[PAK_TYPE_ID_LEN];
	UINT32				imageSize;
	char				modelName[64];
	UINT32				SWVersion;
	UINT32				SWDate;
	UINT32				DevMode; 		/* 0 : Release Mode , 1 : Debug Mode */
	UINT32              loadAddr;		/*< 0x24 : laod Address                      */
	UINT32				magic_number;	/* Magic Number */
	UINT8				Reserved[36];
} PAK_HEADER_T;

typedef struct
{
	char				fileType[PAK_TYPE_ID_LEN];
	UINT32				fileSize;
	UINT32				fileNum;
	UINT32				epkVersion;
	char				ota_id[32];
	PAK_LOCATION_T		imageLocation[];
} EPK_HEADER_T;

typedef enum
{
	CREATE_PAK,
	CREATE_EPK,
	UNCOMP_EPK,
} CREATE_OPT_T;

/**********************************************************/
typedef struct
{
	unsigned long long image_offset;	// 8Byte
	unsigned long long image_size;		// 8Byte
	unsigned long long flash_offset;	// 8Byte
	unsigned char dummy[8];				// 8Byte
}PART_LOCATION_T;

typedef struct
{
	char magic[8];							// 8Byte
	unsigned char partition_count;			// 1Byte
	unsigned char dummy1[3];				// 3Byte
	unsigned char check_boot_area;			// 1Byte
	unsigned char dummy2[3];				// 3Byte
	PART_LOCATION_T part_location[];		// 32Byte * partition_count
}RAWIMG_HEADER_T;
/**********************************************************/

#endif /* _EPK_H_ */
