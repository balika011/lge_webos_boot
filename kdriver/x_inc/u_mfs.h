
#ifndef _U_MFS_H
#define _U_MFS_H

#include "u_mdb.h"
#include "u_fm.h"

#define	MFS_MAX_FILE_LEN        (256 - 1) /* Unicode not including trailing NULL */
#define	MFS_MAX_PATH_LEN       (1152 - 1)

#define MFS_SEEK_BGN  0x1
#define MFS_SEEK_CUR  0x2
#define MFS_SEEK_END  0x3

#define MFS_MODE_TYPE_FILE       ((UINT32) 0x00010000)
#define MFS_MODE_TYPE_DIR        ((UINT32) 0x00020000)

#define MTP_FILE_DESC   (HT_GROUP_MTP + ((HANDLE_TYPE_T) 3))

#define	MFR_OK                  ((INT32)  0)
#define	MFR_ARG                 ((INT32) -1)
#define	MFR_HANDLE              ((INT32) -2)
#define	MFR_INVAL               ((INT32) -3)
#define MFR_CORE                ((INT32) -4)
#define	MFR_NO_ENTRY            ((INT32) -5)
#define	MFR_NOT_DIR             ((INT32) -6)
#define MFR_EOF                 ((INT32) -7)

typedef FM_DIR_INFO_T  MFS_DIR_INFO_T;
typedef FM_FILE_INFO_T MFS_FILE_INFO_T; 

typedef struct _MFS_DIR_ENTRY_T
{
   UINT8   ui1_len;                                                    /* number of characters */
   CHAR    s_name[MDB_ITEM_STR_LEN_MAX];//(MFS_MAX_FILE_LEN + 1) * 4];  /* for UTF-8, enough??? */

   UINT64  ui8_dirent_ofst;
   //UINT32  ui4_dirent_len;
   FM_FILE_INFO_T t_file_info;
} MFS_DIR_ENTRY_T;

#endif


