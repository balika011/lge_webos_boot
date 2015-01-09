#ifndef _B2R_EMU_H_
#define _B2R_EMU_H_

#include "x_typedef.h"


//#define B2R_FBM_CFG   //This define must be marked in in drv build.


typedef INT32 EMU_FILE_HANDLE;
#define EMU_FILE_HANDLE_NULL (-1)

typedef enum
{
    EMU_SRC_ICE = 0,
    EMU_SRC_IDE,
    EMU_SRC_USB,
    EMU_SRC_MAX
} EMU_SRC_TYPE_T;


typedef struct 
{
    UINT32 ADDR_MODE;
    UINT32 ADDR_SWAP;
    UINT32 FIELD;
    UINT32 FLIP;
    UINT32 HDEW;
    UINT32 H_OFFSET;
    UINT32 LINE_OFFSET;
    UINT32 MIRROR;
    UINT32 M_422;
    UINT32 OUT_I;
    UINT32 R_MAP_C;
    UINT32 R_MAP_Y;
    UINT32 VDEW;
    UINT32 V_OFFSET;
    UINT32 DEPTH;
    UINT32 TAG_REMOVAL;
    UINT32 PACK_MODE;
	UINT32 u4SrcWidth;
	UINT32 u4SrcHeight;
	UINT32 u4TagWidth;
	UINT32 u4TagHeight;
    UINT32 u4_Tag_LINE_OFFSET;
} B2R_CFG_FILE;


typedef struct _EMU_ITEM_T
{
    struct _EMU_ITEM_T* next;
    UINT8  data[128];
    UINT32 flag;
    UINT32 total;
    UINT32 fail;
} EMU_ITEM_T;

//private cfg

typedef struct _EMU_ITEM_HEAD_T
{
    struct _EMU_ITEM_T* next;
} EMU_ITEM_HEAD_T;

typedef struct 
{
    UINT8  u1B2rId; // 0 : HD, 1, SD
    BOOL   fgOld;  // 1 : old b2r;  0, new b2r
    UINT32 u4Rate;  
    UINT32 u4Scale;  
    CHAR   acRootPath[256];
    BOOL   fgTwoAddr;  // 1 : old b2r;  0, new b2r
} EMU_LIST_CFG_T;

typedef struct _EMU_LIST_T
{
    struct _EMU_LIST_T* next;
    UINT8  data[256];
    UINT32 u4flag;
    EMU_ITEM_HEAD_T rItemHead;

    //commom cfg
    EMU_LIST_CFG_T tCfg;

    //private cfg

} EMU_LIST_T;

typedef struct _EMU_LIST_HEAD_T
{
    struct _EMU_LIST_T* next;
} EMU_LIST_HEAD_T;


typedef struct _EMU_LISTS_T
{
    UINT32 flag;
    EMU_LIST_HEAD_T rListHead;
} EMU_LISTS_T;


EMU_FILE_HANDLE emu_fopen(CHAR *pu1Path, EMU_SRC_TYPE_T eType);
VOID   emu_fclose(EMU_FILE_HANDLE hHandle);
UINT32 emu_fread(EMU_FILE_HANDLE hHandle, CHAR *pu1Addr, UINT32 u4Size);
UINT32 emu_fwrite(EMU_FILE_HANDLE hHandle, UINT8 *pu1Addr, UINT32 u4Size);
UINT32 emu_fsize(EMU_FILE_HANDLE hHandle);
UINT32 emu_fseek(EMU_FILE_HANDLE hHandle, UINT32 u4Offset);
char * emu_strsep(char **s, const char *ct);
char *emu_str_find(char* s, char* a);
void emu_item_add(EMU_ITEM_T *ptItems, UINT32 pos, CHAR *data);
UINT32 emu_str2int(char *str);

#endif /*_B2R_EMU_H_*/



