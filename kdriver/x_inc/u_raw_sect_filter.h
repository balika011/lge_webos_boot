
#ifndef  _U_RAW_SECT_FILTER_H_
#define  _U_RAW_SECT_FILTER_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_common.h"
#include "x_common.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/*the return values*/
#define RSFR_OK                                  ((INT32) 0) /*ok*/
#define RSFR_NOT_INIT                            ((INT32) -1) /*raw section filter not init*/
#define RSFR_INIT_FAIL                           ((INT32) -2) /*fail to init*/
#define RSFR_INV_ARG                             ((INT32) -3) /*arguments or filter id invalid */
#define RSFR_OPERATION_NOT_AVAILABLE             ((INT32) -4) /*system error or operation invalid*/
#define RSFR_NOT_START                           ((INT32) -5) /*filter has not already started*/
#define RSFR_ALREADY_STARTED                     ((INT32) -6) /*filter has already started*/
#define RSFR_ALREADY_STOPPED                     ((INT32) -7) /*filter has already stopped*/
#define RSFR_FILTER_ALLOC_FAIL                   ((INT32) -8) /*fail to allocate filter*/
#define RSFR_FILTER_START_FAIL                   ((INT32) -9) /*fail to allocate filter*/
#define RSFR_FILTER_STOP_FAIL                    ((INT32) -10) /*fail to allocate filter*/
#define RSFR_FILTER_FREE_FAIL                    ((INT32) -11) /*fail to free filter*/
#define RSFR_LOCK_ERROR                          ((INT32) -12) /*fail to get lock*/
#define RSFR_UNLOCK_ERROR                        ((INT32) -13) /*fail to release lock*/
#define RSFR_TUNER_MODE_SET_ERROR                ((INT32) -14) /*tuner connection error, can not load section*/
#define RSFR_SECTION_BUFFER_ERROR                ((INT32) -15) /*previous section buffer is not freed*/

/*ui1_pattern_len+ui1_pattern_offset should not over this value*/
#define RSF_FILTER_PATTERN_MAX_LEN       16

/*not set any flag, use default value*/
#define RSF_REQ_FLAG_NULL              ((UINT16)(0x00))  
/*if set the flag , hardware will not check section CRC, else do hardware CRC check in default, */
#define RSF_REQ_FLAG_NO_CRC_CHECK      ((UINT16)(0x01))  
/* notify all sections if set this flag, else only notify the updated section in default*/
#define RSF_REQ_FLAG_NO_VER_CHECK      ((UINT16)(0x02)) 


typedef struct _RSF_REQ
{
    UINT8   ui1_pos_pattern[RSF_FILTER_PATTERN_MAX_LEN];          /* positive pattern */
    UINT8   ui1_pos_pattern_mask[RSF_FILTER_PATTERN_MAX_LEN];     /* positive pattern mask */
    UINT8   ui1_neg_pattern[RSF_FILTER_PATTERN_MAX_LEN];          /* negative pattern */
    UINT8   ui1_neg_pattern_mask[RSF_FILTER_PATTERN_MAX_LEN];     /* negative pattern mask */
    UINT8   ui1_pattern_len;            /* pos/net patten length,zero means no patten, load table only by pid*/
    UINT8   ui1_pattern_offset;         /* patten offset: value[0,RSF_FILTER_PATTERN_OFFSET_MAX_LEN] */
    UINT16  ui2_flag;               /*the filter request flag*/
} RSF_REQ_T;

/* raw section filter source type, only support one source at a time*/
typedef enum
{
    RSF_SOURCE_UNKOWN = 0,
    RSF_SOURCE_CAB_DIG, /*cable---digital*/
    RSF_SOURCE_SAT_DIG, /*satellite---digital*/
    RSF_SOURCE_TER_DIG, /*terrestrial---digital*/
    RSF_SOURCE_AGENT    /*other source like MMP file*/
}RSF_TUNER_MODE_T;

typedef enum
{
    RSF_NFY_TYPE_DATA = 0, /* section data */
    RSF_NFY_TYPE_BUFF_OV  /* section buffer overflow */
} RSF_NFY_TYPE_T;

typedef enum
{
    RSF_NFY_RET_PROCESSED = 0,/* this notify be processed */
    RSF_NFY_RET_NOT_NEED     /* this notify not need */
} RSF_NFY_RET_T;

typedef enum
{
    RSF_FILRER_FLAG_SAME_PROCESS = 0,   /* CLENT and Raw section filter module in the same process */
    RSF_FILRER_FLAG_DIFF_PROCESS        /* CLENT and Raw section filter module in the different process */
} RSF_FILRER_FLAG_T;

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_nfy_fct
 *
 * Description: This is a callback function for loading section ,once the section arrived,  
                the serial number is valid only when callback function return RSF_NFY_RET_PROCESSED.
 *
 * Inputs:  pv_nfy_data         Contains a notify data from API caller.
 *          pui1_section        The the section data pointer.
 *          ui2_sect_data_size  The total size of section data.
 *          ui4_serial          the serial number of this section notify 
 *
 * Outputs: -
 *
 * Returns: TRUE        success to get section
 *          FALSE       fail to get section
 ----------------------------------------------------------------------------*/
typedef RSF_NFY_RET_T (* x_raw_sect_filter_nfy_fct)(VOID*                pv_nfy_tag, 
                                               RSF_NFY_TYPE_T       e_nfy_type,
                                               UINT8*               pui1_section,  
                                               UINT16               ui2_sect_data_size, 
                                               UINT32               ui4_serial); 


#endif /* _U_RAW_SECT_FILTER_H_ */

