/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

#ifndef _U_EDB_H_
#define _U_EDB_H_

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_EDB Event DataBase
 *
 *  @ingroup groupMW
 *  @brief Event Context is used to get Event Inforamtion (EPG data) from stream.
 *
 *  %Event DataBase is designed for user to retrive event information (EPG data)
 *   that tranmitted via transport stream.
 *
 *   Event DataBase is consitant of a framework, and a set of difference sub 
 *   engine. Those engines are designed for retrieve different event information
 *   table for differenct DTV standard. E.g. there would be a DVB sub engine, 
 *   which is used to retrieve and parse EIT table that defined in DVB standard. 
 *   And there also be a ATSC sub engine, which is used to retrieve and parse 
 *   EIT table that defined in ATSC standard.
 *
 *  @see groupMW_EDB
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle.h"
#include "u_lnk_list.h"
#include "u_svl.h"



//#define EDB_INTERNAL_MONITOR_EVENT


#define EDB_DVB_BRDCST_GRP                          ( (MAKE_BIT_MASK_8(BRDCST_TYPE_DVB)) | (MAKE_BIT_MASK_8(BRDCST_TYPE_DTMB)) )
#define EDB_ISDB_BRDCST_GRP                         (MAKE_BIT_MASK_8(BRDCST_TYPE_ISDB))
#define EDB_NAME_MAX_SIZE                           ((SIZE_T)  16)
#define EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS    "\n"                /*between extennded text and items*/
#define EDB_SEPARATOR_OF_SHORT_TEXT_AND_ITEMS_IF_THERE_IS_NO_EXTENDED_TEXT    "\n"   
#define EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT    "-"                 /*between item descript and item text*/
#define EDB_SEPARATOR_OF_ITEM_AND_ITEM              "\n"                /*between item descript and item text*/
#define EDB_SEPARATOR_OF_MULT_LANG                  "\n"                /*between each language*/
#define EDB_TRANCATE_SPACE_BUFFER                   ((UINT16)(1024*8))
#define EDB_EVENT_DETAIL_BUFFER                     ((UINT16)(1024*8))
#define EDB_MIN_SED_COMPARE_LENGTH		            ((SIZE_T)(   0x7))


#define EDB_EXTENDED_STRATAGE2


typedef  UINT32 EDB_NFY_REASON_T;

/* Notification Reason used in (*x_edb_nfy_fct)() */
#define EDB_REASON_UNKNOWN                      ((UINT32) 0x00000000)

#define EDB_REASON_HANDLE_CLOSED                MAKE_BIT_MASK_32(0)
#define EDB_REASON_EVENT_NOT_FOUND              MAKE_BIT_MASK_32(1)
#define EDB_REASON_EVENT_EXPIRED                MAKE_BIT_MASK_32(2)
#define EDB_REASON_DT_TIMEOUT                   MAKE_BIT_MASK_32(3)
#define EDB_REASON_DT_DISCONT                   MAKE_BIT_MASK_32(4)
#define EDB_REASON_SVCTX_GET_FAILED             MAKE_BIT_MASK_32(5)
#define EDB_REASON_NOW_EVENT_UPDATED            MAKE_BIT_MASK_32(6)
#define EDB_REASON_NEX_EVENT_UPDATED            MAKE_BIT_MASK_32(7)
#define EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED  MAKE_BIT_MASK_32(8)
#define EDB_REASON_EIT_PF_UPDATED               MAKE_BIT_MASK_32(9)
#define EDB_REASON_EIT_S_UPDATED                MAKE_BIT_MASK_32(10)

/* Return values */
#define EDBR_OK                                 ((INT32)    0)
#define EDBR_ALREADY_INIT                       ((INT32)   -1)
#define EDBR_CMD_NOT_SUPPORT                    ((INT32)   -2)
#define EDBR_DUP_BRDCST_TYPE                    ((INT32)   -3)
#define EDBR_ENG_NOT_FOUND                      ((INT32)   -4)
#define EDBR_EVENT_UNAVAIL                      ((INT32)   -5)
#define EDBR_INFO_NOT_FOUND                     ((INT32)   -6)
#define EDBR_INTERNAL_ERROR                     ((INT32)   -7)
#define EDBR_INV_ARG                            ((INT32)   -8)
#define EDBR_INV_COMMAND                        ((INT32)   -9)
#define EDBR_INV_COND                           ((INT32)  -10)
#define EDBR_INV_HANDLE                         ((INT32)  -11)
#define EDBR_NOT_INIT                           ((INT32)  -12)
#define EDBR_OUT_OF_HANDLES                     ((INT32)  -13)
#define EDBR_OUT_OF_MEM                         ((INT32)  -14)
#define EDBR_SVC_UNAVAIL                        ((INT32)  -15)
#define EDBR_OUT_OF_RESOURCE                    ((INT32)  -16)
#define EDBR_HANDLE_NOT_SUPPORT                 ((INT32)  -17)
#define EDBR_ALREADY_EXIST                      ((INT32)  -18)
#define EDBR_VERSION_NOT_CHANGE                 ((INT32)  -19)
#define EDBR_PARSE_ERROR                        ((INT32)  -20)
#define EDBR_HASH_NOT_EXIST                     ((INT32)  -21)
#define EDBR_HASH_PARSE_BREAK                   ((INT32)  -22)
#define EDBR_EDB_NOT_CREATED                    ((INT32)  -23)
#define EDBR_EDB_SECTION_NOT_ENOUGH             ((INT32)  -24)


extern UINT16          g_edb_monitor_on_id;
extern UINT16          g_edb_monitor_ts_id;
extern UINT16          g_edb_monitor_svc_id;

#define MONITOR_ON_ID                           (g_edb_monitor_on_id)/*((UINT16)0x1013)*/
#define MONITOR_TS_ID                           (g_edb_monitor_ts_id)/*((UINT16)0x502 )*/
#define MONITOR_SVC_ID                          (g_edb_monitor_svc_id)/*((UINT16)0x520 )*/


/* Command codes used to load events */

typedef UINT32  EDB_COMMAND_CODE_T;

#define EDB_CMD_END                             ((EDB_COMMAND_CODE_T) 0)
#define EDB_CMD_OPEN_DATA                       ((EDB_COMMAND_CODE_T) 1)/*(EDB_ENG_OPEN_DATA_T*) to setup a source control engine 
                                                                          repository with (ui2_svl_id, e_brdcst_type, ps_tuner_name). 
                                                                          This command can be hooked several times for different 
                                                                          source control engine repositories.*/
#define EDB_CMD_CFG_PF_ONLY                     ((EDB_COMMAND_CODE_T) 2)/*(BOOL) to ask to load and cache present/following events only (default: false)*/
#define EDB_CMD_CFG_ACTUAL_ONLY                 ((EDB_COMMAND_CODE_T) 3)/*(BOOL) to ask to load and cache events from EIT actual only (default: false)*/
#define EDB_CMD_CFG_MAX_DAYS                    ((EDB_COMMAND_CODE_T) 4)/*(UINT16)to restrict cache days. For EU models, the days of EIT loading will
                                                                          be also restricted. (default: 8 days)*/
#define EDB_CMD_CFG_PREF_LANG                   ((EDB_COMMAND_CODE_T) 5)/*(EDB_PREF_LANG_T*) to provide preference language (default: (1, eng)
                                                                           EDB_CMD_COUNTRY_CODE: (CHAR*) to provide country code for 
                                                                           mapping correct character set (default: GBR)*/
#define EDB_CMD_CFG_COUNTRY_CODE                ((EDB_COMMAND_CODE_T) 6)
#define EDB_CMD_CFG_ACTIVE_WIN                  ((EDB_COMMAND_CODE_T) 7)
#define EDB_CMD_CFG_EVENT_MIN_SECS              ((EDB_COMMAND_CODE_T) 8)
#define EDB_CMD_CFG_FAKE_EVENT_INSERTION_ENABLE ((EDB_COMMAND_CODE_T) 9)
#define EDB_CMD_CFG_FAKE_EVENT_MIN_SECS         ((EDB_COMMAND_CODE_T) 10)
#define EDB_CMD_CFG_TIME_CONFLICT_ALLOW         ((EDB_COMMAND_CODE_T) 11)
#define EDB_CMD_CFG_TIME_PARTIAL_OVERLAP_ALLOW  ((EDB_COMMAND_CODE_T) 12)
#define EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR      ((EDB_COMMAND_CODE_T) 13)
#define EDB_CMD_CFG_DVBC_OPERATOR               ((EDB_COMMAND_CODE_T) 14)


#define EDB_CMD_DO_CURRENT_SERVICE              ((EDB_COMMAND_CODE_T) 15)
#define EDB_CMD_DO_RESTART                      ((EDB_COMMAND_CODE_T) 16)
#define EDB_CMD_DO_CLEAN                        ((EDB_COMMAND_CODE_T) 17)
#define EDB_CMD_DO_ENABLE                       ((EDB_COMMAND_CODE_T) 18)
#define EDB_CMD_DO_TUNER_CHANGE                 ((EDB_COMMAND_CODE_T) 19)
#define EDB_CMD_DO_ASU_MODE                     ((EDB_COMMAND_CODE_T) 20)
#define EDB_CMD_DO_FLASH_EVENTS                 ((EDB_COMMAND_CODE_T) 21)
#define EDB_CMD_DO_SYNC_EVENTS                  ((EDB_COMMAND_CODE_T) 22)
#define EDB_CMD_RISKTV                          ((EDB_COMMAND_CODE_T) 23)
#define EDB_CMD_CHANNEL_SCAN_ACTIVE             ((EDB_COMMAND_CODE_T) 24)


#define EDB_CMD_CFG_EIT_M_ENABLE				((EDB_COMMAND_CODE_T) 25)/*ISDB used*/
#define EDB_CMD_CFG_EIT_L_ENABLE				((EDB_COMMAND_CODE_T) 26)/*ISDB used*/

#define EDB_CMD_CFG_TUNER_TYPE  				((EDB_COMMAND_CODE_T) 27)/**/
#define EDB_CMD_CFG_DVBS_OPERATOR               ((EDB_COMMAND_CODE_T) 28)
#define EDB_CMD_CFG_NO_CHECK_PF_EVENT_TIME      ((EDB_COMMAND_CODE_T) 29)
#define EDB_CMD_MAX_NUM                         ((EDB_COMMAND_CODE_T) 30) 



#define EDB_MAX_NUM_PREF_LANG                   ((UINT8)    6)
#define EDB_MAX_NUM_CA_SYSTEM                   ((UINT8)    4)

#define EDB_MAX_LENGTH_EVENT_TITLE              ((UINT8)   60)
#define EDB_MAX_LENGTH_EVENT_GUIDANCE			((UINT8)   75)
#define EDB_MAX_NUM_RATING_INFO                 ((UINT8)    4)
#define EDB_MAX_NUM_CATEGORY                    ((UINT8)    8)
#define EDB_MAX_LEN_ITEM_DESC_CHAR              ((UINT8)  255)
#define EDB_MAX_LEN_ITEM_CHAR                   ((UINT8)  255)
#define EDB_MAX_NUM_ITEMS                       ((UINT8)  20)

#define EDB_MAX_PER_CH_TBL                      ((UINT8)   16)   /*16 for 64 days,max*/


#define MAX_COMPONENT_INFO                      ((UINT8)    8)
#define EDB_MAX_LENGTH_EVENT_COMPONENT_TEXT     ((UINT8)   32)


#define EDB_MIN_MEMORY_SIZE                     ((SIZE_T)1024*1024*2)/**/
#define EDB_MAX_MEMORY_SIZE                     (edb_get_memory_size())/**/
#define EDB_MAX_MEMORY_SIZE_FOR_FLASH_POOL      (edb_get_flash_size_for_pool())/**/



#define EDB_MEMORY_THRESHOLD                    ((SIZE_T)1024*150)/**/
#define EDB_MEMORY_THRESHOLD_FOR_SINGLE_EVNET   ((SIZE_T)1024*20)/**/

#define EDB_MAX_ENG_DATA                        ((UINT8)        10)/**/
#define EDB_MAX_INTER_CH_LOCATE                 ((UINT16) 1024 * 2)/*enough ??*/
#define EDB_POOL_HASH_CHANNEL_TABLE_SIZE        ((UINT16) 100     )
#define EDB_POOL_MAX_CHANNEL_EVENT              ((UINT16) 120 * (EDB_MAX_PER_CH_TBL * 4)  )  /*(On day 96) * (1 table id=4 days)*/
#define EDB_POOL_CHANNEL_HASH_EVENT_TABLE_SIZE  ((UINT16) 50 )  
#define EDB_POOL_MAX_SECION_NUM_SIZE            ((UINT16) 512 )  //used for collect sections to merge


#define EDB_EVENT_MAX_DES_SIZE                  ((UINT16)      0xFF)  

#define MAX_EMPTY_CELL_SUPPORT                  ((UINT8)  32      )
#define EVENT_EMPTY_CELL_START_EVENT_ID         ((UINT16) 65500   )
/*for CRID chage this value from 16 to 128*/
#define MAX_CRID_URL_INFO           ((UINT8)128)
#define MAX_TURKEY_ID_LENGTH        ((UINT8)8)



#if 0
typedef UINT8  EDB_EVENT_TYPE_T;
#define EDB_EVENT_TYPE_PRESENT           ((UINT8)  0)
#define EDB_EVENT_TYPE_FOLLOW            ((UINT8)  1)
#define EDB_EVENT_TYPE_SCHEDULE          ((UINT8)  2)

typedef enum
{
    EDB_EVENT_BRDCST_TYPE_DVB = 0,
    EDB_EVENT_BRDCST_TYPE_ATSC
}EDB_EVENT_BRDCST_TYPE_T; /*TODO*/
#endif




/* EDB States */
/*------------------------------------------------------------------*/
/*! @enum EDB_COND_T
 *  @brief EDB engine condition
 *  @code
 *  typedef enum
 *  {
 *      EDB_COND_UNCONFIG = 0,
 *      EDB_COND_STOPPED,
 *      EDB_COND_STARTED
 *  } EDB_COND_T;
 *
 *  @endcode
 *  @li@c  
 *  @li@c  
 *  @li@c  
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EDB_COND_UNCONFIG = 0,
    EDB_COND_STOPPED,
    EDB_COND_STARTED
} EDB_COND_T;



/* EDB KEY TYPE */
/*------------------------------------------------------------------*/
/*! @enum EDB KEY TYPE
 *  @brief EDB key type
 *  @code
 *  typedef enum
 *  {
 *   EDB_KEY_TYPE_EVENT_ID,   
 *   EDB_KEY_TYPE_START_TIME,
 *   EDB_KEY_TYPE_DURATION,    
 *   EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
 *   EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
 *   EDB_KEY_TYPE_SHORT_EVENT_DETAIL_BY_ID,
 *   EDB_KEY_TYPE_EXTENDED_EVENT_DETAIL_BY_ID,
 *   EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID,
 *   EDB_KEY_TYPE_PRESENT_EVENT,
 *   EDB_KEY_TYPE_FOLLOWING_EVENT,
 *   EDB_KEY_TYPE_SINGLE_EVENT_BY_ID,
 *   EDB_KEY_TYPE_SINGLE_EVENT_BY_UTC,
 *   EDB_KEY_TYPE_EVENT_IDS_BY_RANGE,
 *   EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC,
 *   EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC,
 *   EDB_KEY_TYPE_EVENT_IDS_BY_FILTER,
 *   EDB_KEY_TYPE_EVENT_IDS_BY_QUERY,
 *   EDB_KEY_TYPE_EVENT_DESC,
 *   EDB_KEY_TYPE_EVENT_NUM_BY_RANGE_ASYNC,
 *   EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC_ASYNC,
 *   EDB_KEY_TYPE_RATING_LIST_BY_ID,
 *   EDB_KEY_TYPE_AUDIO_COMPONENT_BY_ID,
 *   EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC, the utc is given for checking the validity of current PF
 *   EDB_KEY_TYPE_LINKAGE,
 *   EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID,
 *   EDB_KEY_TYPE_SINGLE_POOL_SERIAL_EVENT_BY_ID,
 *   EDB_KEY_TYPE_EVENT_IDS_BY_DESIRED_STRING,
 *   EDB_KEY_TYPE_DETAIL_LANGUAGE,
 *   EDB_KEY_TYPE_PREVIOUS_EVENT_BY_UTC,
 *   EDB_KEY_TYPE_COMPONENT,   
 *    EDB_KEY_TYPE_CONTENT_IDENTIFIER
 *  } EDB_KEY_TYPE_T;
 *
 *  @endcode
 *  @li@c  EDB_KEY_TYPE_EVENT_ID                                        -get event id
 *  @li@c  EDB_KEY_TYPE_START_TIME                                    -get event start time(default event:  present event)
 *  @li@c  EDB_KEY_TYPE_EVENT_TITLE_BY_ID                        -get event title by id
 *  @li@c  EDB_KEY_TYPE_EVENT_DETAIL_BY_ID                      -get event detail by id
 *  @li@c EDB_KEY_TYPE_SHORT_EVENT_DETAIL_BY_ID           -get short event by id 
 *  @li@c EDB_KEY_TYPE_EXTENDED_EVENT_DETAIL_BY_ID     - get extend event by id
 *  @li@c EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID                - get event item descriptor by id
 *  @li@c EDB_KEY_TYPE_PRESENT_EVENT                               - get parent event 
 *  @li@c EDB_KEY_TYPE_FOLLOWING_EVENT                          - get following event
 *  @li@c EDB_KEY_TYPE_SINGLE_EVENT_BY_ID                      - get single event by id
 *  @li@c EDB_KEY_TYPE_EVENT_IDS_BY_RANGE                    - get event ids by given range
 *  @li@c EDB_KEY_TYPE_SINGLE_EVENT_BY_UTC                  - get single event by utc
 *  @li@c EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC      - get current next event by utc
 *  @li@c EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC   -get current and previous event by utc
 *  @li@c EDB_KEY_TYPE_EVENT_IDS_BY_FILTER                      -get event ids by filter
 *  @li@c EDB_KEY_TYPE_EVENT_IDS_BY_QUERY                      -get event ids by query
 *  @li@c EDB_KEY_TYPE_EVENT_DESC                                    -clean this channel's version mask,dvbsi  will resend this channel's EIT section
 *  @li@c EDB_KEY_TYPE_RATING_LIST_BY_ID                         - get rating list by id
 *  @li@c EDB_KEY_TYPE_AUDIO_COMPONENT_BY_ID               -get audio componet by id
 *  @li@c EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC       -get N&N event number by utc
 *  @li@c EDB_KEY_TYPE_LINKAGE                                           -get linkage info
 *  @li@c EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID                    -get guidance info
 *  @li@c EDB_KEY_TYPE_SINGLE_POOL_SERIAL_EVENT_BY_ID - serial events for flash  
 *  @li@c EDB_KEY_TYPE_EVENT_IDS_BY_DESIRED_STRING      - get event ids by desired string
 *  @li@c EDB_KEY_TYPE_DETAIL_LANGUAGE                           - get detail language
 *  @li@c EDB_KEY_TYPE_PREVIOUS_EVENT_BY_UTC                - get previous event by utc
 *  @li@c EDB_KEY_TYPE_COMPONENT                                     - get component info
 *  @li@c EDB_KEY_TYPE_CONTENT_IDENTIFIER                        - get content identififer decriptor info

 */
/*------------------------------------------------------------------*/


typedef enum
{
    EDB_KEY_TYPE_UNKNOWN = 0,
    EDB_KEY_TYPE_EVENT_ID,   
    EDB_KEY_TYPE_START_TIME,
    EDB_KEY_TYPE_DURATION,    
    EDB_KEY_TYPE_EVENT_TITLE_BY_ID,
    EDB_KEY_TYPE_EVENT_DETAIL_BY_ID,
    EDB_KEY_TYPE_SHORT_EVENT_DETAIL_BY_ID,
    EDB_KEY_TYPE_EXTENDED_EVENT_DETAIL_BY_ID,
    EDB_KEY_TYPE_EVENT_ITEM_DESC_BY_ID,
    EDB_KEY_TYPE_PRESENT_EVENT,
    EDB_KEY_TYPE_FOLLOWING_EVENT,
    EDB_KEY_TYPE_SINGLE_EVENT_BY_ID,
    EDB_KEY_TYPE_SINGLE_EVENT_BY_UTC,
    EDB_KEY_TYPE_EVENT_IDS_BY_RANGE,
    EDB_KEY_TYPE_CURRENT_NEXT_EVENT_BY_UTC, /*the utc is given for searching these two events*/
    EDB_KEY_TYPE_CURRENT_PREVIOUS_EVENT_BY_UTC,
    EDB_KEY_TYPE_EVENT_IDS_BY_FILTER,
    EDB_KEY_TYPE_EVENT_IDS_BY_QUERY,
    EDB_KEY_TYPE_EVENT_DESC,
    EDB_KEY_TYPE_EVENT_NUM_BY_RANGE_ASYNC,
    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC_ASYNC,
    EDB_KEY_TYPE_RATING_LIST_BY_ID,
    EDB_KEY_TYPE_AUDIO_COMPONENT_BY_ID,
    EDB_KEY_TYPE_NOW_NEXT_EVENT_NUM_BY_UTC, /*the utc is given for checking the validity of current PF*/
    EDB_KEY_TYPE_LINKAGE,
    EDB_KEY_TYPE_GUIDANCE_TEXT_BY_ID,
    EDB_KEY_TYPE_SINGLE_POOL_SERIAL_EVENT_BY_ID,
    EDB_KEY_TYPE_EVENT_IDS_BY_DESIRED_STRING,
    EDB_KEY_TYPE_DETAIL_LANGUAGE,
    EDB_KEY_TYPE_PREVIOUS_EVENT_BY_UTC,/*the utc is given for searching the previous event*/
    EDB_KEY_TYPE_COMPONENT,   
    EDB_KEY_TYPE_CONTENT_IDENTIFIER,
	EDB_KEY_TYPE_TURKEY_IDENTIFIER,
    EDB_KEY_TYPE_EVENTS_INFO_BY_CRID_TYPE,
	EDB_KEY_TYPE_EVENTS_INFO_BY_TUREKEY_ID_TYPE,
    EDB_KEY_TYPE_SERIES_BY_ID,
    EDB_KEY_TYPE_CATEGORY_BY_ID,
#if 0    
    EDB_KEY_TYPE_COMMON,
    EDB_KEY_TYPE_EVENT_DETAIL,
    EDB_KEY_TYPE_EVENT_EXTENDED,
    EDB_KEY_TYPE_TIME_SHIFTED_EVENT ,      
    EDB_KEY_TYPE_CA_IDENTIFIER,    
    EDB_KEY_TYPE_CONTENT,         
    EDB_KEY_TYPE_PARENTAL_RATING, 
#endif    
    EDB_KEY_TYPE_RATING_VALID,
	EDB_KEY_TYPE_RATING_VALIDATION_TABLE,
	EDB_KEY_TYPE_EVENT_IDS_FROM_CURRENT_BY_NUM,
    EDB_KEY_TYPE_TOTAL_NUMBER     /* total number of key types */
} EDB_KEY_TYPE_T;






/*----------------------------------------------------------------------------*/
/*! @brief  EDB notify callback function

 *  @param  [in] h_edb,       - The handle of the edb
 *  @param  [in] e_reason,    - The notify reason of edb
 *  @param  [in] pv_nfy_tag,  - The tag provided when creating edb
 *  @param  [in] ui4_data1    - When e_reason is 
 *                              EDB_REASON_NOW_EVENT_UPDATED or EDB_REASON_NEX_EVENT_UPDATED
 *                                  ui4_data1 is ui2_svl_id of current svl and 
 *                                  ui4_data2 is ui4_channel_id of current service.
 *                              EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED
 *                                  ui4_data1 is ui2_svl_id of current svl 
 *                                  ui4_data2 is ui4_channel_id of updated service.
 * 
 *  @param  [in] ui4_data2    -
 *
 *
 *  @note   User should implement such function pointer and register to EDB
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/
typedef VOID (*x_edb_nfy_fct) (
        HANDLE_T                h_edb,
        EDB_NFY_REASON_T        e_reason,
        VOID*                   pv_nfy_tag,
        UINT32                  ui4_data1,
        UINT32                  ui4_data2);







/*----------------------------------------------------------------------------*/
/*! @brief  EDB query notify function

 *  @param  [in] ui2_svl_id,              - Svl ID
 *  @param  [in] ui4_channel_id,          - The input channel id when invoking this query
 *  @param  [in] t_base_utc,              - The input start time when invoking this query
 *  @param  [in] ui2_event_num,           - The number of events in this time of query
 *  @param  [in] ui2_channel_check_sum,   - The checksum of events in this time of query
 *  @param  [in] pv_tag                   - The input tag address when invoking this query
 *
 *
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/

typedef VOID (*x_edb_query_nfy_fct)(
        UINT16   ui2_svl_id,
        UINT32   ui4_channel_id,
        TIME_T   t_base_utc,
        UINT16   ui2_event_num,
        UINT16   ui2_channel_check_sum,
        VOID*    pv_tag
        );


/*------------------------------------------------------------------*/
/*! @struct EDB_ENG_OPEN_DATA_T
 *
 *  @code
 * typedef struct  _EDB_ENG_OPEN_DATA_T
 * {
 *     BRDCST_TYPE_T   e_brdcst_type;
 *     UINT16          ui2_svl_id;
 *     CHAR*           ps_tuner_name;  
 * } EDB_ENG_OPEN_DATA_T;  
 * 
 *  @endcode
 * 
 *  @li@c    
 *  @li@c    
 */
/*------------------------------------------------------------------*/
typedef struct  _EDB_ENG_OPEN_DATA_T
{
    BRDCST_TYPE_T   e_brdcst_type;
    UINT16          ui2_svl_id;
    CHAR*           ps_tuner_name;  
} EDB_ENG_OPEN_DATA_T; 



/*------------------------------------------------------------------*/
/*! @struct EDB_PREF_LANG_T
 *
 *  @code
 * typedef struct  EDB_PREF_LANG_T
 * {
 *     UINT8          ui1_num;
 *     ISO_639_LANG_T t_lang[EDB_MAX_NUM_PREF_LANG]; 
 * } EDB_PREF_LANG_T; 
 * 
 *  @endcode
 * 
 *  @li@c    ui1_num                   -The number of preferred languages
 *  @li@c    t_time_duration           -The array contains preferred languages 
 */
/*------------------------------------------------------------------*/
typedef struct  EDB_PREF_LANG_T
{
    UINT8          ui1_num;
    ISO_639_LANG_T t_lang[EDB_MAX_NUM_PREF_LANG]; 
} EDB_PREF_LANG_T; 




/*------------------------------------------------------------------*/
/*! @struct EDB_CH_ID_T
 * 
 *  @code
 * 
 * typedef struct _EDB_CH_ID_T                 
 * {
 *     BRDCST_TYPE_T       e_brdcst_type;
 *     UINT16                 ui2_svl_id;
 *     UINT32                 ui4_channel_id;
 * } EDB_CH_ID_T;                           @endcode
 *  @endcode
 * 
 *  @li@c    e_brdcst_type                  - Broadcast type.
 *  @li@c    ui2_svl_id                     - svl id
 *  @li@c    ui4_channel_id                 - channel id
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_ID_T
{
    BRDCST_TYPE_T           e_brdcst_type;
    UINT16                  ui2_svl_id;
    UINT32                  ui4_channel_id;
} EDB_CH_ID_T;


#define EDB_MAX_NUM_ACTIVE_CHANNEL  ((UINT8) 32)

/*------------------------------------------------------------------*/
/*! @struct  EDB_CACHE_ACTIVE_WIN_T
 * 
 *  @code
 * 
 *typedef struct _EDB_ACTIVE_WIN_T
 *{
 *    UINT8     ui1_num_channel;
 *    EDB_CH_ID_T at_channel_id[EDB_MAX_NUM_ACTIVE_CHANNEL];
 *    TIME_T    t_start_time;
 *    TIME_T    t_duration;
 *} EDB_CACHE_ACTIVE_WIN_T; 
 *   
 *  @endcode
 * 
 *  @li@c      ui1_num_channel            - The number of active channels 
 *  @li@c      at_channel_id              - The array contains the id of active channels
 *  @li@c      t_start_time               - The start time of active window
 *  @li@c      t_duration                 - the duration of active window
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CACHE_ACTIVE_WIN_T
{
    UINT8           ui1_num_channel;
    EDB_CH_ID_T     at_channel_id[EDB_MAX_NUM_ACTIVE_CHANNEL];
    TIME_T          t_start_time;
    TIME_T          t_duration;
} EDB_CACHE_ACTIVE_WIN_T; 



/*------------------------------------------------------------------*/
/*! @struct  EDB_CH_EVT_ID_T

 *  @code
 * 
 * typedef struct _EDB_CH_EVT_ID_T            
 * {
 *     BRDCST_TYPE_T       e_brdcst_type;
 *     UINT16                 ui2_svl_id;
 *     UINT32                 ui4_channel_id;
 *     UINT16                 ui2_event_id;
 * } EDB_CH_EVT_ID_T;
 *   
 *  @endcode
 * 
 *  @li@c     e_brdcst_type            - Broadcast type. 
 *  @li@c     ui2_svl_id               - Svl ID
 *  @li@c     ui4_channel_id           - Channel ID
 *  @li@c     ui2_event_id             - Event ID
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_EVT_ID_T
{
    BRDCST_TYPE_T          e_brdcst_type;
    UINT16                 ui2_svl_id;
    UINT32                 ui4_channel_id;
    UINT16                 ui2_event_id;
    BOOL                   b_auto_event_id;
    BOOL                   b_valid;
    TIME_T                 t_start_time;
} EDB_CH_EVT_ID_T;

typedef struct _EDB_CH_EVT_ID_CRID_T
{
    BRDCST_TYPE_T          e_brdcst_type;
    UINT16                 ui2_svl_id;
    UINT32                 ui4_channel_id;
    UINT16                 ui2_event_id;
    BOOL                   b_auto_event_id;
    TIME_T                 t_start_time;
    UINT8                  ui1_crid_type;
    CHAR                  s_crid_url[MAX_CRID_URL_INFO];
    UINT16                 ui2_crid_match_events_num;
} EDB_CH_EVT_ID_CRID_T;
typedef struct _EDB_CH_EVT_ID_TURKEY_ID_T
{
    BRDCST_TYPE_T          e_brdcst_type;
    UINT16                 ui2_svl_id;
    UINT32                 ui4_channel_id;
    UINT16                 ui2_event_id;
    BOOL                   b_auto_event_id;
    TIME_T                 t_start_time;
    UINT8                  ui1_turkey_id_type;
    CHAR                   s_turkey_id[MAX_TURKEY_ID_LENGTH];
    UINT16                 ui2_turkey_id_match_events_num;
} EDB_CH_EVT_ID_TURKEY_ID_T;
/*------------------------------------------------------------------*/
/*! @struct  EDB_CH_EVT_ID_T

 *  @code
 * 
 *typedef struct _EDB_CH_EVT_ID_COMPONENT_T
 *{
 *   BRDCST_TYPE_T          e_brdcst_type;
 *     UINT16                 ui2_svl_id;
 *     UINT32                 ui4_channel_id;
 *     UINT16                 ui2_event_id;
 *     BOOL                   b_auto_event_id;
 *     UINT8                  ui1_component_tag;
 * } EDB_CH_EVT_ID_COMPONENT_T;

 *   
 *  @endcode
 * 
 *  @li@c     e_brdcst_type            - Broadcast type. 
 *  @li@c     ui2_svl_id               - Svl ID
 *  @li@c     ui4_channel_id           - Channel ID
 *  @li@c     ui2_event_id             - Event ID
 *  @li@c     b_auto_event_id       - use default event id or not
 *  @li@c     ui1_component_tag    - component tag
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_EVT_ID_COMPONENT_T
{
    BRDCST_TYPE_T          e_brdcst_type;
    UINT16                 ui2_svl_id;
    UINT32                 ui4_channel_id;
    UINT16                 ui2_event_id;
    BOOL                   b_auto_event_id;
    UINT8                  ui1_component_tag;
} EDB_CH_EVT_ID_COMPONENT_T;

/* EDB DESIRED STRING MATCH TYPE*/
/*------------------------------------------------------------------*/
/*! @enum EDB_DESIRED_STRING_MATCH_TYPE_T
 *  @brief EDB engine condition
 *  @code
 * typedef enum 
 * {
 *    EDB_DESIRED_STRING_MATCH_TITLE,
 *     EDB_DESIRED_STRING_MATCH_TITLE_AND_DETAIL
 * }EDB_DESIRED_STRING_MATCH_TYPE_T;

 *
 *  @endcode
 *  @li@c  EDB_DESIRED_STRING_MATCH_TITLE                      -only match title
 *  @li@c  EDB_DESIRED_STRING_MATCH_TITLE_AND_DETAIL - match title and detail
 *  @li@c  
 */
/*------------------------------------------------------------------*/

typedef enum 
{
    EDB_DESIRED_STRING_MATCH_TITLE,
    EDB_DESIRED_STRING_MATCH_TITLE_AND_DETAIL
}EDB_DESIRED_STRING_MATCH_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct  EDB_DESIRED_STRING_T

 *  @code
 * 
 * typedef struct _EDB_DESIRED_STRING_T
 *{
 *   BRDCST_TYPE_T                 e_brdcst_type;
 *   UINT16                              ui2_svl_id;
 *   CHAR*                               ps_desired_string;
 *    SIZE_T                              z_string_length;
 *   EDB_DESIRED_STRING_MATCH_TYPE_T     e_match_type;
 *} EDB_DESIRED_STRING_T;
 *   
 *  @endcode
 * 
 *  @li@c     e_brdcst_type            - e_brdcst_type
 *  @li@c     ui2_svl_id                  - ui2_svl_id
 *  @li@c    ps_desired_string        - desired_string                        
 *  @li@c   z_string_length              -string_length
 *  @li@c     pui2_ch_event_id        - The start pointer of channel and event id.
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_DESIRED_STRING_T
{
    BRDCST_TYPE_T                       e_brdcst_type;
    UINT16                              ui2_svl_id;
    CHAR*                               ps_desired_string;
    SIZE_T                              z_string_length;
    EDB_DESIRED_STRING_MATCH_TYPE_T     e_match_type;
} EDB_DESIRED_STRING_T;


/*------------------------------------------------------------------*/
/*! @struct  EDB_BUCKET_INFO_T

 *  @code
 * 
 * typedef struct _EDB_BUCKET_INFO_T             
 * {
 *     UINT16                   ui2_checksum
 *     UINT16*                  pui2_num_events;
 *     EDB_CH_EVT_ID_T*      pui2_ch_event_id;
 * } EDB_BUCKET_INFO_T;
 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui2_checksum            - The checksum of the events in this bucket
 *  @li@c     pui2_num_events         - [In] the max num of event for this bucket
 *                                      [out] the actual num of events for this query
 *
 *  @li@c     pui2_ch_event_id        - The start pointer address of the array to put ID of events.
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_BUCKET_INFO_T
{
    UINT16                   ui2_checksum;
    UINT16*                  pui2_num_events;
    EDB_CH_EVT_ID_T*         pui2_ch_event_id;
} EDB_BUCKET_INFO_T;



/*------------------------------------------------------------------*/
/*! @struct  EDB_GET_EVENT_FROM_CURRENT_T

 *  @code
 * 
 * typedef struct _EDB_GET_EVENT_FROM_CURRENT_T             
 * {
 *     UINT16*                      pui2_num_events;
 *     UINT16*				  pui2_ch_event_id;
 * } EDB_GET_EVENT_FROM_CURRENT_T;
 * 
 *   
 *  @endcode
 *  
 *  @li@c     pui2_num_events         - [In] the max num of event for this query
 *                                      [out] the actual num of events for this query
 *
 *  @li@c     pui2_ch_event_id        - The start pointer address of the array to put ID of events.
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_GET_EVENT_FROM_CURRENT_T
{
    UINT16*                  pui2_num_events;
    UINT16*                  pui2_ch_event_id;
} EDB_GET_EVENT_FROM_CURRENT_T;


#define EDB_MAX_TUNER_NAME       SYS_NAME_LEN                     /*tuner name max length */
#define EDB_MAIN                 "EDB_MAIN"                      /*EDB name */
#define EDB_DVB_MAX_TBL          ((UINT8)(34))                  /*2 EIT PF + 32 EIT S(actual + other) */
#define EDB_ISDB_MAX_HEIT_TBL         ((UINT8)(18))             /*2 EIT PF + 32 EIT S(actual + other) */



#define     EDB_EVENT_GOT_EVENT_ID              MAKE_BIT_MASK_32(0)
#define     EDB_EVENT_GOT_START_TIME            MAKE_BIT_MASK_32(1)
#define     EDB_EVENT_GOT_DURATION              MAKE_BIT_MASK_32(2)
#define     EDB_EVENT_GOT_CAPTION               MAKE_BIT_MASK_32(3)
#define     EDB_EVENT_GOT_FREE_CA_MODE          MAKE_BIT_MASK_32(4)
#define     EDB_EVENT_GOT_LINKAGE               MAKE_BIT_MASK_32(5)
#define     EDB_EVENT_GOT_EVENT_TITLE           MAKE_BIT_MASK_32(6)
#define     EDB_EVENT_GOT_EVENT_DETAIL          MAKE_BIT_MASK_32(7)
#define     EDB_EVENT_GOT_EVENT_EXTENDED        MAKE_BIT_MASK_32(8)
#define     EDB_EVENT_GOT_TIME_SHIFT            MAKE_BIT_MASK_32(9)
#define     EDB_EVENT_GOT_COMPONENT             MAKE_BIT_MASK_32(10)
#define     EDB_EVENT_GOT_CA_IDENTIFIER         MAKE_BIT_MASK_32(11)
#define     EDB_EVENT_GOT_CONTENT               MAKE_BIT_MASK_32(12)
#define     EDB_EVENT_GOT_RATING                MAKE_BIT_MASK_32(13)
#define     EDB_EVENT_GOT_GUIDANCE              MAKE_BIT_MASK_32(14)

#define     EDB_EVENT_GOT_EVENT_GROUP           MAKE_BIT_MASK_32(15)
#define     EDB_EVENT_GOT_DIGITAL_CPY_CTRL      MAKE_BIT_MASK_32(16)
#define     EDB_EVENT_GOT_AUDIO_COMPONENT       MAKE_BIT_MASK_32(17)
#define     EDB_EVENT_GOT_CA_CONTRACT_INF0      MAKE_BIT_MASK_32(18)
#define     EDB_EVENT_GOT_SERIES                MAKE_BIT_MASK_32(19)
#define     EDB_EVENT_GOT_COMPONENT_GROUP       MAKE_BIT_MASK_32(20)
#define     EDB_EVENT_GOT_CONTENT_IDENTIFIER    MAKE_BIT_MASK_32(21)
#define     EDB_EVENT_GOT_IDENTIFIER            MAKE_BIT_MASK_32(22)


/*------------------------------------------------------------------*/
/*! @struct  EDB_RATING_T

 *  @code
 * 
 * typedef struct _EDB_RATING_T
 * {
 *   UINT16                  ui2_dimension;
 *    UINT16                  ui2_rating_value;
 * } EDB_RATING_T;

 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui2_dimension            - The number of rating value
 *  @li@c     ui2_rating_value         - rating value

 */
/*------------------------------------------------------------------*/

typedef struct _EDB_RATING_T
{
    UINT16                  ui2_dimension;
    UINT16                  ui2_rating_value;
} EDB_RATING_T;

/*------------------------------------------------------------------*/
/*! @struct  EDB_ITEM_T

 *  @code
 * 
 *typedef struct _EDB_ITEM_T
 *{
 *    UINT8       ui1_item_desc_len;
 *    CHAR        ac_item_desc_char[EDB_MAX_LEN_ITEM_DESC_CHAR];
 *    UINT8       ui1_item_len;
 *    CHAR        ac_item_char[EDB_MAX_LEN_ITEM_CHAR];
 *} EDB_ITEM_T;

 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui1_item_desc_len            - item descriptor length
 *  @li@c     ac_item_desc_char          -  item descriptor char
 *  @li@c    ui1_item_len                    -  item length 
 *  @li@c    ac_item_char                   -  itme char

 */
/*------------------------------------------------------------------*/

typedef struct _EDB_ITEM_T
{
    UINT8       ui1_item_desc_len;
    CHAR        ac_item_desc_char[EDB_MAX_LEN_ITEM_DESC_CHAR];
    UINT8       ui1_item_len;
    CHAR        ac_item_char[EDB_MAX_LEN_ITEM_CHAR];
} EDB_ITEM_T;


/*------------------------------------------------------------------*/
/*! @struct  EDB_EVENT_ITEM_INFO_T
 *
 *  @code
 * 
 *typedef struct _EDB_EVENT_ITEM_INFO_T
 *{
 *    UINT8       ui1_item_number;
 *    EDB_ITEM_T  at_item_list[EDB_MAX_NUM_ITEMS];
 *} EDB_EVENT_ITEM_INFO_T;
 *
 *  @endcode
 * 
 *  @li@c     ui1_item_number            - the number of total items
 *  @li@c     at_item_list               -  item list
 *
 *
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_ITEM_INFO_T
{
    UINT8              ui1_item_number;
    EDB_ITEM_T         at_item_list[EDB_MAX_NUM_ITEMS];
}EDB_EVENT_ITEM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  _EDB_COMPONENT_DESCRIPTOR_T

 *  @code
 * 
 *typedef struct _EDB_COMPONENT_DESCRIPTOR_T
 *{
 *    UINT8           ui1_stream_content;
 *    UINT8           ui1_component_type;
 *    UINT8           ui1_component_tag;
 *    ISO_639_LANG_T  t_lang;
 * } EDB_COMPONENT_INFO_T;
 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui1_stream_content            - stream content
 *  @li@c     ui1_component_type          - component type
 *  @li@c    ui1_component_tag             - component tag
 *  @li@c    t_lang                                - language

 */
/*------------------------------------------------------------------*/

typedef struct _EDB_COMPONENT_DESCRIPTOR_T
{
    UINT8           ui1_stream_content;
    UINT8           ui1_component_type;
    UINT8           ui1_component_tag;
    ISO_639_LANG_T  t_lang;
} EDB_COMPONENT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct  _EDB_EVENT_LINKAGE_INFO_T

 *  @code
 * 
 *typedef struct _EDB_EVENT_LINKAGE_INFO_T
 *{
 *    UINT16  ui2_on_id;
 *    UINT16  ui2_ts_id;
 *    UINT16  ui2_svc_id;
 *}EDB_EVENT_LINKAGE_INFO_T;

 * 
 *   
 *  @endcode
 * 
 *  @li@c     ui2_on_id           - on id
 *  @li@c     ui2_ts_id            - ts id
 *  @li@c    ui2_svc_id          - svc id
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_LINKAGE_INFO_T
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
}EDB_EVENT_LINKAGE_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct  _EDB_COMPONENT_INFO_TEXT_T

 *  @code
 * 
 *typedef struct _EDB_COMPONENT_INFO_TEXT_T
 *{
 *    UINT8           ui1_stream_content;
 *    UINT8           ui1_component_type;
 *    UINT8           ui1_component_tag;
 *    ISO_639_LANG_T  t_lang;
 *     CHAR            ac_component_text[EDB_MAX_LENGTH_EVENT_COMPONENT_TEXT];
 * } EDB_COMPONENT_INFO_TEXT_T;
* 
 *   
 *  @endcode
 * 
 *  @li@c    ui1_stream_content           - strean content
 *  @li@c     ui1_component_type         - component type
 *  @li@c    ui1_component_tag          -  component tag
 *  @li@c    t_lang                              - language
 *  @li@c   ac_component_text           - component text
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_COMPONENT_INFO_TEXT_T
{
    UINT8           ui1_stream_content;
    UINT8           ui1_component_type;
    UINT8           ui1_component_tag;
    ISO_639_LANG_T  t_lang;
    CHAR            ac_component_text[EDB_MAX_LENGTH_EVENT_COMPONENT_TEXT];
} EDB_COMPONENT_INFO_TEXT_T;

/*------------------------------------------------------------------*/
/*! @struct  _EDB_EVENT_COMMON_T

 *  @code
 * 
 *typedef struct _EDB_EVENT_COMMON_T
 *{
 *    UINT16                  ui2_svc_id;
 *    UINT16                  ui2_event_id;
 *}EDB_EVENT_COMMON_T;
* 
 *   
 *  @endcode
 * 
 *  @li@c    ui2_svc_id;           - svc id
 *  @li@c     ui2_event_id         - event id
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_COMMON_T
{
    UINT16                  ui2_svc_id;
    UINT16                  ui2_event_id;
}EDB_EVENT_COMMON_T;

#define MAX_EVENT_COMMON_INFO       ((UINT8)32)   /* max event common info number */
#define MAX_EVENT_LINKAGE_INFO      ((UINT8)4)    /* max event linkage info number */
//#define MAX_CRID_URL_INFO           ((UINT8)16)   /* max CRID url number */

#define GUIDANCE_MODE_NOT_DEFINE 				((UINT8)0)
#define UNSUITABLE_UNTIL_AFTER_THE_WATERSHED 	((UINT8)1)

/*------------------------------------------------------------------*/
/*! @struct  _EDB_EVENT_INFO_T

 *  @code
 * 
 *typedef struct _EDB_EVENT_INFO_T
 *{
 *    UINT16                      ui2_svl_id;
 *    UINT32                      ui4_channel_id;
 *    UINT16                      ui2_event_id;
 *    TIME_T                      t_start_time;
 *    TIME_T                      t_duration;
 *    BOOL                        b_caption;
 *    UINT16                      ui2_event_mask;
 *    BOOL                        b_free_ca_mode;
 *    UINT8                       ui1_event_title_len;
 *    CHAR                        ac_event_title[EDB_MAX_LENGTH_EVENT_TITLE + 1];
 *    UINT8                       ui1_guidance_mode;
 *    UINT8                       ui1_event_guidance_len;
 *    CHAR                        ac_event_guidance[EDB_MAX_LENGTH_EVENT_GUIDANCE + 1];
 *    UINT8                       ui1_num_rating;
 *    UINT16                      aui2_ca_system_id[EDB_MAX_NUM_CA_SYSTEM];
 *    UINT8                       ui1_num_event_category;
 *    UINT8                       aui1_event_category[EDB_MAX_NUM_CATEGORY];
 *   UINT8                       ui1_num_comp_info;
 *    EDB_COMPONENT_INFO_T        at_comp_info[MAX_COMPONENT_INFO];
 *    UINT8                       ui1_num_linkage_info;
  *   EDB_EVENT_LINKAGE_INFO_T    at_linkage_info[MAX_EVENT_LINKAGE_INFO];
 *    UINT16                      ui2_checksum;    
 *    UINT16                      ui2_cur_ts_id;
 *    UINT8                       ui1_num_event_common;
 *    EDB_EVENT_COMMON_T          at_event_common[MAX_EVENT_COMMON_INFO];
 *} EDB_EVENT_INFO_T;
 

 *   
 *  @endcode
 * 
 *  @li@c    ui2_svl_id                      - svl id
 *  @li@c     ui4_channel_id                 - channel id
 *  @li@c    ui2_event_id                   -  event id
 *  @li@c    t_start_time                     - start time
 *  @li@c   t_duration                           - event duration
 *  @li@c   b_caption                           - caption or not
 *  @li@c   ui2_event_mask                 - event mask
 *  @li@c   b_free_ca_mode                 - ca mode or not
 *  @li@c   ui1_event_title_len             - event title length 
 *  @li@c   ac_event_title                     - event titile text
 *  @li@c   ui1_guidance_mode              - guidance mode
 *  @li@c   ui1_event_guidance_len          - guidance length
 *  @li@c   ac_event_guidance               -   guidance text
 *  @li@c   ui1_num_rating                     - rating number
 *  @li@c   aui2_ca_system_id               - ca system id
 *  @li@c   ui1_num_event_category         - event category numbers
 *  @li@c   aui1_event_category              -event category text
 *  @li@c   ui1_num_comp_info                  -componet number
 *  @li@c   at_comp_info                            - componet info
 *  @li@c   ui1_num_linkage_info                -linkage numbers
 *  @li@c   at_linkage_info                         -linkage inof
 *  @li@c   ui2_checksum                           - check sum
 *  @li@c  ui2_cur_ts_id                               - current transport stream id
 *  @li@c   ui1_num_event_common            - the number of event common
 *  @li@c   at_event_common                     - the content of event common
 
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_INFO_T
{
    UINT16                      ui2_svl_id;
    UINT8                       ui1_table_id;
	UINT8                       ui1_section_num;
	UINT8                       ui1_running_status;
    UINT32                      ui4_channel_id;
    UINT16                      ui2_event_id;
    TIME_T                      t_start_time;
    TIME_T                      t_duration;
    BOOL                        b_caption;
    UINT16                      ui2_event_mask;
    BOOL                        b_free_ca_mode;
    UINT8                       ui1_event_title_len;
    CHAR                        ac_event_title[EDB_MAX_LENGTH_EVENT_TITLE + 1];
	UINT8						ui1_guidance_mode;
	UINT8                       ui1_event_guidance_len;
	CHAR						ac_event_guidance[EDB_MAX_LENGTH_EVENT_GUIDANCE + 1];
    UINT8                       ui1_num_rating;
    UINT16                      aui2_ca_system_id[EDB_MAX_NUM_CA_SYSTEM];
    UINT8                       ui1_num_event_category;
    UINT8                       aui1_event_category[EDB_MAX_NUM_CATEGORY];
    UINT8                       ui1_num_comp_info;
    EDB_COMPONENT_INFO_T        at_comp_info[MAX_COMPONENT_INFO];
    UINT8                       ui1_num_linkage_info;
    EDB_EVENT_LINKAGE_INFO_T    at_linkage_info[MAX_EVENT_LINKAGE_INFO];
    UINT16                      ui2_checksum;    
    UINT16                      ui2_cur_ts_id;
    UINT8                       ui1_num_event_common;
    EDB_EVENT_COMMON_T          at_event_common[MAX_EVENT_COMMON_INFO];

#if 0
    EDB_RATING_T           at_rating_info[EDB_MAX_NUM_RATING_INFO];
#endif
} EDB_EVENT_INFO_T;


/* EDB_RATING_TYPE_T*/
/*------------------------------------------------------------------*/
/*! @enum EDB_RATING_TYPE_T
 *  @brief EDB engine condition
 *  @code
 * typedef enum 
 * {
 *     EDB_RATING_NORNAL,
 *    EDB_RATING_NO_EIT,
 *    EDB_RATING_NO_DESC,
 *    EDB_RATING_NO_SDT_PF_FLAG
 * }EDB_RATING_TYPE_T;

 *
 *  @endcode
 *  @li@c  EDB_RATING_NORNAL                       -EIT & FLAG existed
 *  @li@c  EDB_RATING_NO_EIT                        - NO EIT
 *  @li@c  EDB_RATING_NO_DESC                     -EIT exist but no rating descriptor
 *  @li@c  EDB_RATING_NO_SDT_PF_FLAG         - no pf flag 
 *  @li@c  EDB_RATING_NO_SDT_ACTUAL          -no sdt actual exist
 
 */
/*------------------------------------------------------------------*/

typedef enum 
{
    EDB_RATING_NORNAL,
    EDB_RATING_NO_EIT,
    EDB_RATING_NO_DESC,
    EDB_RATING_NO_SDT_PF_FLAG,
    EDB_RATING_NO_SDT_ACTUAL
}EDB_RATING_TYPE_T;




/*------------------------------------------------------------------*/
/*! @struct  _EDB_RATING_LIST_T

 *  @code
 * 
 typedef struct _EDB_RATING_LIST_T
 {
     UINT16                  ui2_rating_region;
     UINT16                  ui2_rated_dimensions;
     CHAR*                   ps_country_code;
     EDB_RATING_T*           pt_ratings;
 
     struct _EDB_RATING_LIST_T*  pt_next;
 
     EDB_RATING_T            t_rating;
     ISO_3166_COUNT_T        t_country_code;
     EDB_RATING_TYPE_T       t_rating_type;
 } EDB_RATING_LIST_T;

* 
 *   
 *  @endcode
 * 
 *  @li@c    ui2_rating_region           - rating region
 *  @li@c     ui2_rated_dimensions         - dimension of rating list
 *  @li@c    ps_country_code          -  country code
 *  @li@c    pt_ratings                    - the pointor to rating info
 *  @li@c   t_rating                         - rating info
 *  @li@c   t_country_code              - country code 
 *  @li@c   t_rating_type                - rating type
 *  @li@c   pt_next                         - point to next rating info
 
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_RATING_LIST_T
{
    UINT16                  ui2_rating_region;
    UINT16                  ui2_rated_dimensions;
    CHAR*                   ps_country_code;
    EDB_RATING_T*           pt_ratings;

    struct _EDB_RATING_LIST_T*  pt_next;

    EDB_RATING_T            t_rating;
    ISO_3166_COUNT_T        t_country_code;
    EDB_RATING_TYPE_T       t_rating_type;
} EDB_RATING_LIST_T;

/* EDB_TABLE_STATUS_T*/
/*------------------------------------------------------------------*/
/*! @enum EDB_TABLE_STATUS_T
 *  @brief EDB engine condition
 *  @code
 *  typedef enum
 *  {
 *	   EDB_TABLE_STATUS_Y = 0,
 *	   EDB_TABLE_STATUS_U,
 *	   EDB_TABLE_STATUS_N
 *    } EDB_STATUS_T;

 *
 *  @endcode
 *  @li@c  EDB_TABLE_STATUS_Y                             - table exist
 *  @li@c  EDB_TABLE_STATUS_U                             - table undecided
 *  @li@c  EDB_TABLE_STATUS_N                             - no table
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EDB_TABLE_STATUS_Y = 0,
    EDB_TABLE_STATUS_U,
    EDB_TABLE_STATUS_N
} EDB_TABLE_STATUS_T;
/*------------------------------------------------------------------*/
/*! @struct  EDB_RATTING_VALIDATION_TABLE_T

 *  @code
 * 
 typedef struct _EDB_RATTING_VALIDATION_TABLE_T
 {
	 EDB_TABLE_STATUS_T  t_sdt_act_table_status;
	 EDB_TABLE_STATUS_T  t_eit_p_table_status; 	 
 } EDB_RATTING_VALIDATION_TABLE_T;

 * 
 *   
 *  @endcode
 * 
 *  @li@c    t_sdt_act_table_status           - sdt actual table's status
 *  @li@c    t_eit_p_table_status              - eit p table's status
  */
/*------------------------------------------------------------------*/

typedef struct _EDB_RATTING_VALIDATION_TABLE_T
{
	EDB_TABLE_STATUS_T t_sdt_act_table_status;
    EDB_TABLE_STATUS_T t_eit_p_table_status;		
} EDB_RATTING_VALIDATION_TABLE_T;


/*------------------------------------------------------------------*/
/*! @struct  _EDB_ISDB_AUDIO_COMPONENT_T

 *  @code
 * 
 * typedef struct _EDB_ISDB_AUDIO_COMPONENT_T
 * {
 *    UINT8 ui1_stream_content;
 *    UINT8 ui1_component_type;
 *    UINT8 ui1_component_tag;
 *    UINT8 ui1_stream_type;
 *    UINT8 ui1_simulcast_grp_tag;
 *    BOOL  b_es_multi_lingual_flag;
 *    BOOL  b_main_component_flag;
 *    UINT8 ui1_quality_indicator;
 *    UINT8 ui1_sample_rate;
 *    ISO_639_LANG_T t_audio_lang1;
 *    CHAR* ps_audio_name1;
 *    ISO_639_LANG_T t_audio_lang2;
 *    CHAR* ps_audio_name2; 
 *    struct _EDB_ISDB_AUDIO_COMPONENT_T* pt_next;
 *  } EDB_ISDB_AUDIO_COMPONENT_T;
 *
 * 
 *   
 *  @endcode
 * 
 *  @li@c    ui1_stream_content         - stream content
 *  @li@c    ui1_component_type         - component type
 *  @li@c    ui1_component_tag          - component tag
 *  @li@c    ui1_stream_type            - stream typE
 *  @li@c    ui1_simulcast_grp_tag      - simulcast grp tag
 *  @li@c    b_es_multi_lingual_flag    - es multi lingual flag 
 *  @li@c    b_main_component_flag      - main component flag
 *  @li@c    ui1_quality_indicator      - quality indicator
 *  @li@c    ui1_sample_rate            - sample rate
 *  @li@c    t_audio_lang1              - audio1 language
 *  @li@c    ps_audio_name1             - audio1 name
 *  @li@c    t_audio_lang2              - audio2 language
 *  @li@c    ps_audio_name1             - audio1 name
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_ISDB_AUDIO_COMPONENT_T
{
    UINT8 ui1_stream_content;
    UINT8 ui1_component_type;
    UINT8 ui1_component_tag;
    UINT8 ui1_stream_type;
    UINT8 ui1_simulcast_grp_tag;
    BOOL  b_es_multi_lingual_flag;
    BOOL  b_main_component_flag;
    UINT8 ui1_quality_indicator;
    UINT8 ui1_sample_rate;
    ISO_639_LANG_T t_audio_lang1;
    CHAR* ps_audio_name1;
    ISO_639_LANG_T t_audio_lang2;
    CHAR* ps_audio_name2; 
    struct _EDB_ISDB_AUDIO_COMPONENT_T* pt_next;
} EDB_ISDB_AUDIO_COMPONENT_T;

/*----------------------------------------------------------------------------*/
/*! @brief  EDB filter function

 *  @param  [in] pt_event_info      - The pointer of event info which is used as criterions to filter events.
 *  @param  [in] pv_tag             - The input tag address when invoking this query
 *
 *
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/

typedef BOOL (*x_edb_filter_fct)(
        EDB_EVENT_INFO_T*    pt_event_info,
        VOID*                       pv_tag
        );



/*------------------------------------------------------------------*/
/*! @struct EDB_CH_TIME_FILTER_T 

 *  @code
 * 
 * typedef struct _EDB_CH_TIME_FILTER_T            
 * {
 *     BRDCST_TYPE_T               e_brdcst_type;
 *     UINT16                      ui2_svl_id;
 *     UINT32                      ui4_channel_id;
 *     TIME_T                      t_start_time;
 *     TIME_T                      t_end_time;
 *     x_edb_filter_fct            pf_filter_fct;  
 *     VOID*                       pv_filter_tag;  
 *     x_edb_query_nfy_fct         pf_nfy_fct;     
 *     VOID*                       pv_nfy_tag;     
 * }EDB_CH_TIME_FILTER_T; 
 *   
 *  @endcode
 * 
 *  @li@c   e_brdcst_type       - Broadcast type  
 *  @li@c   ui2_svl_id          - Svl list id
 *  @li@c   ui4_channel_id      - Channel ID
 *  @li@c   t_start_time        - indicates the specific UTC instead start time.
 *  @li@c   t_end_time          - indicates the specific UTC instead end time.
 *  @li@c   pf_filter_fct       - The function to provide the additional filter criterions. If the function point is NULL, it means that additional filter is not required.
 *  @li@c   pv_filter_tag       - The tag to be input when invoking the filter function
 *  @li@c   pf_nfy_fct          - When the type of key is ASYNC, the result will be provide with this notify function.
 *  @li@c   pv_nfy_tag          - The tag to be input when invoking the notify function.
 *                                      
 *
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_TIME_FILTER_T
{
    BRDCST_TYPE_T               e_brdcst_type;
    UINT16                      ui2_svl_id;
    UINT32                      ui4_channel_id;
    TIME_T                      t_start_time;
    TIME_T                      t_end_time;
    x_edb_filter_fct            pf_filter_fct;
    VOID*                       pv_filter_tag;
    x_edb_query_nfy_fct         pf_nfy_fct;
    VOID*                       pv_nfy_tag;
} EDB_CH_TIME_FILTER_T;


/*------------------------------------------------------------------*/
/*! @struct EDB_CH_SEARCH_T 

 *  @code
 * 
 * typedef struct _EDB_CH_SEARCH_T            
 * {
 *     BRDCST_TYPE_T               e_brdcst_type;
 *     UINT16                      ui2_svl_id;
 *     UINT32                      ui4_channel_id;
 *     UINT16                      ui2_event_num;     
 * }EDB_CH_SEARCH_T; 
 *   
 *  @endcode
 * 
 *  @li@c   e_brdcst_type       - Broadcast type  
 *  @li@c   ui2_svl_id          - Svl list id
 *  @li@c   ui4_channel_id      - Channel ID
 *  @li@c   ui2_event_num      - indicates how many events will be got.
 *                                      
 *
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CH_SEARCH_T
{
    BRDCST_TYPE_T               e_brdcst_type;
    UINT16                      ui2_svl_id;
    UINT32                      ui4_channel_id;
    UINT16                      ui2_event_num;
} EDB_CH_SEARCH_T;


/*------------------------------------------------------------------*/
/*! @struct _EDB_MD_SEARCH_T 

 *  @code
 * 
 
 * typedef struct _EDB_MD_SEARCH_T
 * {
 *     VOID*   pv_query;
 *     UINT16  ui2_channel_count;
 *     UINT32* pui4_channel_ids;
 * } EDB_MD_SEARCH_T;
 *   
 *  @endcode
 * 
 *  @li@c   pv_query            - binary tree query
 *  @li@c   ui2_channel_count   - Channel count 
 *  @li@c   pui4_channel_ids    - channel ids list
 *
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_MD_SEARCH_T
{
    BRDCST_TYPE_T       e_brdcst_type;
    UINT16              ui2_svl_id;
    VOID*   pv_query;
    UINT16  ui2_channel_count;
    UINT32* pui4_channel_ids;
    UINT8*  pui1_query_buffer;
    UINT32  ui4_query_buffer_len;
} EDB_MD_SEARCH_T;



/*------------------------------------------------------------------*/
/*! @struct _EDB_COMMAND_T 

 *  @code
 * 
 
 * typedef struct _EDB_COMMAND_T
 *{
 *    EDB_COMMAND_CODE_T    e_code;
 *    union
 *    {
 *        EDB_ENG_OPEN_DATA_T*            pt_eng_open_data;   
 *         BOOL                            b_bool;            
 *
 *        UINT16                          ui2_number;  
 *         EDB_PREF_LANG_T*                pt_pref_lang;      
 *        ISO_3166_COUNT_T                t_country_code;     
 *        EDB_CACHE_ACTIVE_WIN_T*         pt_active_win;      
 *        EDB_CH_ID_T*                    pt_ch_id;         
 *        CHAR                            s_separator[4];     
 *        UINT32                 t_operator;
 *    } u;
 *} EDB_COMMAND_T;

 *   
 *  @endcode
 * 
 *  @li@c   pt_eng_open_data         - EDB_CMD_OPEN_DATA
 *  @li@c   b_bool                   - EDB_CMD_CFG_PF_ONLY EDB_CMD_CFG_ACTUAL_ONLY
 *  @li@c   ui2_number               - EDB_CMD_CFG_MAX_DAYS  EDB_CMD_CFG_EVENT_MIN_SECS  EDB_CMD_DO_CURRENT_SERVICE EDB_CMD_DO_ENABLE
 *  @li@c   pt_pref_lang             - EDB_CMD_CFG_PREF_LANG
 *  @li@c   t_country_code           - EDB_CMD_CFG_COUNTRY_CODE
 *  @li@c   pt_active_win            - EDB_CMD_CFG_ACTIVE_WIN
 *  @li@c   pt_ch_id                 - channel id 
 *  @li@c   s_separator              - EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR
 *  @li@c   t_operator              - operator set by scan
 
 *
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_COMMAND_T
{
    EDB_COMMAND_CODE_T    e_code;
    union
    {
        EDB_ENG_OPEN_DATA_T*            pt_eng_open_data;   /*EDB_CMD_OPEN_DATA */
        BOOL                            b_bool;             /*EDB_CMD_CFG_PF_ONLY EDB_CMD_CFG_ACTUAL_ONLY */
        UINT16                          ui2_number;         /*EDB_CMD_CFG_MAX_DAYS  EDB_CMD_CFG_EVENT_MIN_SECS  EDB_CMD_DO_CURRENT_SERVICE EDB_CMD_DO_ENABLE*/
        EDB_PREF_LANG_T*                pt_pref_lang;       /*EDB_CMD_CFG_PREF_LANG*/
        ISO_3166_COUNT_T                t_country_code;     /*EDB_CMD_CFG_COUNTRY_CODE*/
        EDB_CACHE_ACTIVE_WIN_T*         pt_active_win;      /*EDB_CMD_CFG_ACTIVE_WIN*/
        EDB_CH_ID_T*                    pt_ch_id;           /**/
        CHAR                            s_separator[4];     /*EDB_CMD_CFG_EVENT_DETAIL_SEPARATOR*/
        UINT32                          t_operator;
        BRDCST_MEDIUM_T                 t_brdcst_medium;
    } u;
} EDB_COMMAND_T;




/*----------------------------------------------------------------------------*/
/*! @brief  EDB free single event 

 *  @param  [in] pv_event_obj,              - the event object to be freed
 *
 *  @see
 *  @return     VOID
 */
/*----------------------------------------------------------------------------*/

typedef VOID (*x_edb_eng_free_single_event) (
        VOID*   pv_event_obj);


/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_LINKAGE_T
 *  @brief  Event linkage 

 *  @code
 *typedef struct _EDB_EVENT_LINKAGE_T
 *{
 *    UINT16      ui2_on_id;
 *    UINT16      ui2_ts_id;
 *    UINT16      ui2_svc_id;
 *      UINT16      ui2_target_event_id;
 *    UINT8       ui1_target_listed;
 *    UINT8       ui1_event_simulcast;
 *    UINT8       ui1_reserved;
 *    UINT8       ui1_linkage_type;
 *} EDB_EVENT_LINKAGE_T;

 *  @endcode

 *  @li@c    ui2_on_id                     - On id
 *  @li@c    ui2_ts_id                    - Ts id
 *  @li@c   ui2_svc_id                      -Svc id
 *  @li@c   ui2_target_event_id     -target event id
 *  @li@c    ui1_target_listed      - target listed
 *  @li@c   ui1_event_simulcast     -event simulcast
 *  @li@c   ui1_linkage_type        - linkage type
 *  @li@c   ui1_reserved            - reserved for future use
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_LINKAGE_T
{
    UINT16      ui2_on_id;
    UINT16      ui2_ts_id;
    UINT16      ui2_svc_id;

    UINT16      ui2_target_event_id;
    UINT8       ui1_target_listed;
    UINT8       ui1_event_simulcast;
    UINT8       ui1_reserved;
    UINT8       ui1_linkage_type;
} EDB_EVENT_LINKAGE_T;



/*------------------------------------------------------------------*/
/*! @struct EDB_EVENT_LINKAGE_LIST_T
 *  @brief  Event linkage LIST

 *  @code
 * typedef struct _EDB_EVENT_LINKAGE_LIST_T
 * {
 *     UINT32                  ui4_num_event_linkage; 
 *     EDB_EVENT_LINKAGE_T*  pt_event_linkage;
 * } EDB_EVENT_LINKAGE_LIST_T;
 *  @endcode

 *  @li@c    ui4_num_event_linkage      - Number event linkage in EIT
 *  @li@c    pt_event_linkage           - Pointer to the array of event linkage
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_LINKAGE_LIST_T
{
    UINT8                   ui1_num_event_linkage;    
    EDB_EVENT_LINKAGE_T*    pt_event_linkage;
} EDB_EVENT_LINKAGE_LIST_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_EXTENDED_ITEM
 *  @brief  Event extend item

 *  @code
 *typedef struct _EDB_EVENT_EXTENDED_ITEM
 *{
 *    CHAR*      ps_extended_item_descript;
 *    CHAR*      ps_extended_item_text;
 *}EDB_EVENT_EXTENDED_ITEM;

 *  @endcode

 *  @li@c    ps_extended_item_descript      - item descriptor text
 *  @li@c    ps_extended_item_text           -  item test
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_EXTENDED_ITEM
{
    CHAR*      ps_extended_item_descript;
    CHAR*      ps_extended_item_text;
}EDB_EVENT_EXTENDED_ITEM;


/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_EXTENDED_ITEM_LIST_T
 *  @brief  Event item LIST

 *  @code
 *typedef struct _EDB_EVENT_EXTENDED_ITEM_LIST_T
 *{
 *    UINT8                               ui1_extended_items_num;
 *    EDB_EVENT_EXTENDED_ITEM*            pt_extended_items;
 *}EDB_EVENT_EXTENDED_ITEM_LIST_T;

 *  @endcode

 *  @li@c    ui1_extended_items_num      - Number event extended item
 *  @li@c    pt_extended_items           - Pointer to the array of event item
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_EXTENDED_ITEM_LIST_T
{
    UINT8                               ui1_extended_items_num;
    EDB_EVENT_EXTENDED_ITEM*            pt_extended_items;
}EDB_EVENT_EXTENDED_ITEM_LIST_T;


/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_EXTENDED
 *  @brief  Event extended 

 *  @code
 * typedef struct _EDB_EVENT_EXTENDED
 *{
 *    CHAR*                               ps_extended_text;
 *    EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_extended_item_list;
 *}EDB_EVENT_EXTENDED;

 *  @endcode

 *  @li@c    ps_extended_text      - extend descriptor text
 *  @li@c    pt_extended_item_list           - Pointer to the extended item list
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_EXTENDED
{
    CHAR*                               ps_extended_text;
    EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_extended_item_list;
}EDB_EVENT_EXTENDED;


/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_EXTENDED_LIST_T
 *  @brief  Event extendedLIST

 *  @code
 *typedef struct _EDB_EVENT_EXTENDED_LIST_T
 *{
 *    UINT8                               ui1_event_extended_num;
 *    EDB_EVENT_EXTENDED*                 pt_event_extended;
 *}EDB_EVENT_EXTENDED_LIST_T;

 *  @endcode

 *  @li@c    ui1_event_extended_num      - Number event extended
 *  @li@c    pt_event_extended           - Pointer to the event extended struct
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_EXTENDED_LIST_T
{
    UINT8                               ui1_event_extended_num;
    EDB_EVENT_EXTENDED*                 pt_event_extended;
}EDB_EVENT_EXTENDED_LIST_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_TIME_SHIFT
 *  @brief  Event time shift

 *  @code
 
 *typedef struct _EDB_EVENT_TIME_SHIFT
 *{
 *    UINT16                              ui2_reference_service_id; 
 *    UINT16                              ui1_reference_event_id;
 *}EDB_EVENT_TIME_SHIFT;

 *  @endcode

 *  @li@c    ui2_reference_service_id      - reference_service_id
 *  @li@c    ui1_reference_event_id           - reference_event_id 
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_TIME_SHIFT
{
    UINT16                              ui2_reference_service_id; /*16 uimsbf*/
    UINT16                              ui1_reference_event_id;/* 16 uimsbf*/
}EDB_EVENT_TIME_SHIFT;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_COMPONENT
 *  @brief  Event componet

 *  @code
 *typedef struct _EDB_EVENT_COMPONENT
 *{
 *    UINT8                               ui1_stream_content;
 *    UINT8                               ui1_component_type;
 *    UINT8                               ui1_component_tag; 
 *    ISO_639_LANG_T                      t_lang;
 *    CHAR*                               ps_text;
 *}EDB_EVENT_COMPONENT;

 *  @endcode

 *  @li@c    ui1_stream_content      - stream content
 *  @li@c    ui1_component_type     -component_type
 *  @li@c   ui1_component_tag       -component tag
 *  @li@c   t_lang                          - language
 *  @li@c   ps_text                         -text
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_COMPONENT
{
    UINT8                               ui1_stream_content;/*  4 uimsbf*/
    UINT8                               ui1_component_type;/*  8 uimsbf*/
    UINT8                               ui1_component_tag; /*  8 uimsbf*/
    ISO_639_LANG_T                      t_lang;
    CHAR*                               ps_text;
}EDB_EVENT_COMPONENT;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_COMPONENT_LIST
 *  @brief  Event component list

 *  @code
 * typedef struct _EDB_EVENT_COMPONENT_LIST
 *{
 *    UINT8                               ui1_num;
 *    EDB_EVENT_COMPONENT*                pt_event_component;
 *}EDB_EVENT_COMPONENT_LIST;

 *  @endcode

 *  @li@c    ui1_num      - Number event component
 *  @li@c    pt_event_component           - event component
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_COMPONENT_LIST
{
    UINT8                               ui1_num;
    EDB_EVENT_COMPONENT*                pt_event_component;
}EDB_EVENT_COMPONENT_LIST;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_CA_SYSTEM_ID_LIST_T
 *  @brief  Event componentLIST

 *  @code
 *typedef struct _EDB_EVENT_CA_SYSTEM_ID_LIST_T
 *{
 *    UINT16              ui2_num_ca_ids;
 *    UINT16*             pui2_ca_ids;
 *} EDB_EVENT_CA_SYSTEM_ID_LIST_T;

 *  @endcode

 *  @li@c    ui2_num_ca_ids      - Number ca ids
 *  @li@c    pui2_ca_ids           - Pointer to the array of ca id
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_CA_SYSTEM_ID_LIST_T
{
    UINT16              ui2_num_ca_ids;
    UINT16*             pui2_ca_ids;
} EDB_EVENT_CA_SYSTEM_ID_LIST_T;

#if 0
typedef struct _EDB_EVENT_CATEGORY_T
{
    UINT8           ui1_content_nibble_level_1;
    UINT8           ui1_content_nibble_level_2;
    UINT8           ui1_user_byte; 
} EDB_EVENT_CATEGORY_T;

typedef struct _EDB_EVENT_CATEGORY_LIST_T
{
    UINT8                    ui1_num_types;
    EDB_EVENT_CATEGORY_T*    pt_category;
} EDB_EVENT_CATEGORY_LIST_T;
#endif

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_RATING_T
 *  @brief  Event rating LIST

 *  @code
 *typedef struct _EDB_EVENT_RATING_T
 *{
 *    ISO_3166_COUNT_T         t_country;
 *    UINT8                    ui1_rating;
 *} EDB_EVENT_RATING_T;

 *  @endcode

 *  @li@c    t_country      - country
 *  @li@c    ui1_rating           - rating value
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_RATING_T
{
    ISO_3166_COUNT_T         t_country;
    UINT8                    ui1_rating;
} EDB_EVENT_RATING_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_RATING_LIST_T
 *  @brief  Event rating LIST

 *  @code
 *typedef struct _EDB_EVENT_RATING_LIST_T
 *{
 *    UINT8                    ui1_num;
 *    EDB_EVENT_RATING_T*      pt_rating;  
 *} EDB_EVENT_RATING_LIST_T;

 *  @endcode

 *  @li@c    ui1_num      - Number event rating in EIT
 *  @li@c    pt_rating           - Pointer to the array of event rating
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_RATING_LIST_T
{
    UINT8                    ui1_num;
    EDB_EVENT_RATING_T*      pt_rating;  
} EDB_EVENT_RATING_LIST_T;

/******************************************************
* Digital Copy Control Descriptor
*
*****************************************************/

/*------------------------------------------------------------------*/
/*! @struct _EDB_COMPONENT_CONTROL_LIST_T
 *  @brief  COMPONENT CONTROLLIST

 *  @code
 * typedef struct _EDB_COMPONENT_CONTROL_LIST_T
 * {
 *    UINT8        ui1_component_tag;
 *    UINT8        ui1_digital_recording_ctrl_data; 
 *    BOOL         b_max_bitrate_flag;
 *    UINT8        ui1_max_bitrate;
 *    UINT8        ui1_copy_ctrl_type;
 *    UINT8        ui1_APS_ctrl_data;
 * } EDB_COMPONENT_CONTROL_LIST_T;

 *  @endcode

 *  @li@c    ui1_component_tag      - component_tag
 *  @li@c    ui1_digital_recording_ctrl_data           - digital_recording_ctrl_data
 *  @li@c    b_max_bitrate_flag      - max_bitrate flag
 *  @li@c    ui1_component_tag       - component tag
 *  @li@c    ui1_max_bitrate      - max bit rate ,Only effect when  is b_max_bitrate_flag true
 *  @li@c    ui1_APS_ctrl_data      - component_tag
 */
/*------------------------------------------------------------------*/  
typedef struct _EDB_COMPONENT_CONTROL_LIST_T
{
    UINT8 ui1_component_tag;
    UINT8 ui1_digital_recording_ctrl_data;
    BOOL  b_max_bitrate_flag;
    UINT8 ui1_max_bitrate;     /* Only effect when  is b_max_bitrate_flag true*/
    UINT8 ui1_copy_ctrl_type;
    UINT8 ui1_APS_ctrl_data; /* Only effect when  ui1_copy_ctrl_type is true*/
} EDB_COMPONENT_CONTROL_LIST_T;


/*! @struct _EDB_DIGITAL_COPY_CONTROL_T
 *  @brief  digital copy control

 *  @code
 * typedef struct _EDB_DIGITAL_COPY_CONTROL_T
 * {
 *    UINT8      ui1_digital_recording_ctrl_data;
 *    BOOL       b_max_bitrate_flag; 
 *    UINT8      ui1_max_bitrate;
 *    UINT8      ui1_copy_ctrl_type;
 *    UINT8      ui1_APS_ctrl_data;
 *    BOOL       b_component_control_flag;
 *    EDB_COMPONENT_CONTROL_LIST_T* pt_component_ctrl_list;
 * } EDB_COMPONENT_CONTROL_LIST_T;

 *  @endcode

 *  @li@c    ui1_digital_recording_ctrl_data      - component_tag
 *  @li@c    b_max_bitrate_flag                   - max bitrate flag
 *  @li@c    ui1_max_bitrate                      - max bitrate
 *  @li@c    ui1_copy_ctrl_type                   - copy ctrl type
 *  @li@c    ui1_APS_ctrl_data                    - APS ctrl data
 *  @li@c    b_component_control_flag             - component control flag
 *  @li@c    pt_component_ctrl_list               - component ctrl list
 */
/*------------------------------------------------------------------*/ 
typedef struct _EDB_DIGITAL_COPY_CONTROL_T
{
    UINT8 ui1_digital_recording_ctrl_data;
    BOOL  b_max_bitrate_flag;    
    UINT8 ui1_max_bitrate;     /* Only effect when  b_max_bitrate_flag is true*/
    UINT8 ui1_copy_ctrl_type;
    UINT8 ui1_APS_ctrl_data;    /* Only effect when  ui1_copy_ctrl_type is true*/
    BOOL  b_component_control_flag;
    EDB_COMPONENT_CONTROL_LIST_T* pt_component_ctrl_list;
} EDB_DIGITAL_COPY_CONTROL_T;


/*------------------------------------------------------------------*/
/*! @struct _EDB_DIGITAL_COPY_LIST_T
 *  @brief  digital copy list

 *  @code
 *  typedef struct _EDB_DIGITAL_COPY_LIST_T
 *  {
 *    UINT8                         ui1_num;
 *    EDB_DIGITAL_COPY_CONTROL_T*   pt_digital_cpy_ctrl;
 *  } EDB_DIGITAL_COPY_LIST_T;
 *  @endcode

 *  @li@c    ui1_num              - digital cpy ctrl number
 *  @li@c    pt_digital_cpy_ctrl  - digital copy control
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_DIGITAL_COPY_LIST_T
{
    UINT8                    ui1_num;
    EDB_DIGITAL_COPY_CONTROL_T* pt_digital_cpy_ctrl;
} EDB_DIGITAL_COPY_LIST_T;

/******************************************************
* Audio Component Descriptor
******************************************************/
/*------------------------------------------------------------------*/
/*! @struct _EDB_AUDIO_COMPONENT_T
 *  @brief  audio component

 *  @code
 *  typedef struct _EDB_AUDIO_COMPONENT_T
 *  {
 *    UINT8    ui1_stream_content;
 *    UINT8    ui1_component_type;
 *    UINT8    ui1_component_tag;
 *    UINT8    ui1_stream_type;
 *    UINT8    ui1_simulcast_grp_tag;
 *    BOOL     b_es_multi_lingual_flag;
 *    BOOL     b_main_component_flag;
 *    UINT8    ui1_quality_indicator;
 *    UINT8    ui1_sample_rate;
 *    ISO_639_LANG_T t_audio_lang1;
 *    CHAR*    ps_audio_name1;
 *    ISO_639_LANG_T t_audio_lang2;
 *    CHAR*    ps_audio_name2;
 *  } EDB_AUDIO_COMPONENT_T;
 *  @endcode

 *  @li@c    ui1_stream_content  - digital cpy ctrl number
 *  @li@c    ui1_component_type  - digital copy control
 *  @li@c    ui1_component_tag   - component tag
 *  @li@c    ui1_stream_type     - stream type
 *  @li@c    ui1_simulcast_grp_tag     - simulcast grp tag
 *  @li@c    b_es_multi_lingual_flag   - es multi lingual flag
 *  @li@c    b_main_component_flag     - main component flag
 *  @li@c    ui1_quality_indicator     - quality indicator
 *  @li@c    ui1_sample_rate           - sample rate
 *  @li@c    t_audio_lang1             - audio1 lang
 *  @li@c    ps_audio_name1            - audio1 name
 *  @li@c    t_audio_lang1             - audio2 lang
 *  @li@c    ps_audio_name1            - audio2 name
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_AUDIO_COMPONENT_T
{
    UINT8 ui1_stream_content;
    UINT8 ui1_component_type;
    UINT8 ui1_component_tag;
    UINT8 ui1_stream_type;
    UINT8 ui1_simulcast_grp_tag;
    BOOL  b_es_multi_lingual_flag;
    BOOL  b_main_component_flag;
    UINT8 ui1_quality_indicator;
    UINT8 ui1_sample_rate;
    ISO_639_LANG_T t_audio_lang1;
    CHAR* ps_audio_name1;
    ISO_639_LANG_T t_audio_lang2;
    CHAR* ps_audio_name2;    
} EDB_AUDIO_COMPONENT_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_AUDIO_COMPONENT_LIST_T
 *  @brief  audio component list

 *  @code
 *  typedef struct _EDB_AUDIO_COMPONENT_LIST_T
 *  {
 *    UINT8                         ui1_num;
 *    EDB_AUDIO_COMPONENT_T*        pt_audio_component;
 *  } EDB_AUDIO_COMPONENT_LIST_T;
 *  @endcode

 *  @li@c    ui1_num              - audio component number
 *  @li@c    pt_audio_component   - audio component
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_AUDIO_COMPONENT_LIST_T
{
    UINT8                    ui1_num;
    EDB_AUDIO_COMPONENT_T*   pt_audio_component;
} EDB_AUDIO_COMPONENT_LIST_T;

/******************************************************
* CA Contract Info Descriptor
******************************************************/
/*------------------------------------------------------------------*/
/*! @struct _EDB_CA_CONTRACT_INFO_T
 *  @brief  ca contract info

 *  @code
 *  typedef struct _EDB_CA_CONTRACT_INFO_T
 *  {
 *    UINT16    ui2_CA_system_id;
 *    UINT8     ui1_CA_uint_id;
 *    UINT8     ui1_component_num;
 *    UINT8*    pui1_component_tags;
 *    UINT8     ui1_contract_ver_info_len;
 *    UINT8*    pui1_contract_ver_info;
 *    CHAR*     ps_fee_name;
 *  } EDB_CA_CONTRACT_INFO_T;
 *  @endcode

 *  @li@c    ui2_CA_system_id              - CA system id
 *  @li@c    ui1_CA_uint_id                - CA uint id
 *  @li@c    ui1_component_num             - component num
 *  @li@c    pui1_component_tags           - component tags
 *  @li@c    ui1_contract_ver_info_len     - contract ver info length
 *  @li@c    pui1_contract_ver_info        - contract ver info
 *  @li@c    ps_fee_name                   - fee name
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_CA_CONTRACT_INFO_T
{
    UINT16 ui2_CA_system_id;
    UINT8   ui1_CA_uint_id;
    UINT8   ui1_component_num;
    UINT8* pui1_component_tags;
    UINT8   ui1_contract_ver_info_len;
    UINT8* pui1_contract_ver_info;
    CHAR*  ps_fee_name;
} EDB_CA_CONTRACT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_CA_CONTRACT_LIST_T
 *  @brief  ca contract list

 *  @code
 *  typedef struct _EDB_CA_CONTRACT_LIST_T
 *  {
 *    UINT8                    ui1_num;
 *    EDB_CA_CONTRACT_INFO_T*  pt_ca_contract_info;
 *  } EDB_CA_CONTRACT_LIST_T;
 *  @endcode

 *  @li@c    ui1_num              - ca contract info number
 *  @li@c    pt_ca_contract_info  - CA contract info
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_CA_CONTRACT_LIST_T
{
    UINT8                    ui1_num;
    EDB_CA_CONTRACT_INFO_T*   pt_ca_contract_info;
} EDB_CA_CONTRACT_LIST_T;

/******************************************************
* Series Decriptor
******************************************************/
/*------------------------------------------------------------------*/
/*! @struct _EDB_SERIES_T
 *  @brief  series info

 *  @code
 *  typedef struct _EDB_SERIES_T
 *  {
 *    UINT16   ui2_series_id;
 *    UINT8    ui1_repeat_label;
 *    UINT8    ui1_program_pattern;
 *    BOOL     b_expire_date_valid_flag;
 *    UINT16   ui2_expire_date;
 *    UINT16   ui2_episode_num;
 *    UINT16   ui2_last_episode_num;
 *    CHAR*    ps_series_name;
 *  } EDB_CA_CONTRACT_INFO_T;
 *  @endcode

 *  @li@c    ui2_series_id                 - series id
 *  @li@c    ui1_repeat_label              - repeat label
 *  @li@c    ui1_program_pattern           - program pattern
 *  @li@c    b_expire_date_valid_flag      - expire date valid flag
 *  @li@c    ui2_expire_date               - expire date
 *  @li@c    ui2_episode_num               - episode number
 *  @li@c    ps_series_name                - series name
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_SERIES_T
{
    UINT16 ui2_series_id;
    UINT8   ui1_repeat_label;
    UINT8   ui1_program_pattern;
    BOOL    b_expire_date_valid_flag;
    UINT16 ui2_expire_date;
    UINT16 ui2_episode_num;
    UINT16 ui2_last_episode_num;
    CHAR*  ps_series_name;
} EDB_SERIES_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_SERIES_LIST_T
 *  @brief  series list

 *  @code
 *  typedef struct _EDB_SERIES_LIST_T
 *  {
 *    UINT8                    ui1_num;
 *    EDB_SERIES_T*            pt_series;
 *  } EDB_SERIES_LIST_T;
 *  @endcode

 *  @li@c    ui1_num              - series info number
 *  @li@c    pt_series            - series info
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_SERIES_LIST_T
{
    UINT8                    ui1_num;
    EDB_SERIES_T*            pt_series;
} EDB_SERIES_LIST_T;

/******************************************************
* Component Group Descriptor
******************************************************/
/*------------------------------------------------------------------*/
/*! @struct _EDB_CA_UNIT_INFO_T
 *  @brief  ca unit info

 *  @code
 *  typedef struct _EDB_CA_UNIT_INFO_T
 *  {
 *    UINT8   ui1_ca_unit_id;
 *    UINT8   ui1_component_num;
 *    UINT8*  pui1_component_tags;
 *  } EDB_CA_CONTRACT_INFO_T;
 *  @endcode

 *  @li@c    ui1_ca_unit_id                - ca unit id
 *  @li@c    ui1_component_num             - component number
 *  @li@c    pui1_component_tags           - component tags
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_CA_UNIT_INFO_T
{
    UINT8   ui1_ca_unit_id;
    UINT8   ui1_component_num;
    UINT8* pui1_component_tags;
} EDB_CA_UNIT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_COMPONENT_GROUP_INFO_T
 *  @brief  component group info

 *  @code
 *  typedef struct _EDB_COMPONENT_GROUP_INFO_T
 *  {
 *    UINT8   ui1_component_group_id;
 *    UINT8   ui1_num_of_ca_unit;
 *    INT16   i2_total_bit_rate;
 *    CHAR*   ps_description;
 *    EDB_CA_UNIT_INFO_T* pt_ca_unit_info_list;
 *  } EDB_COMPONENT_GROUP_INFO_T;
 *  @endcode

 *  @li@c    ui1_component_group_id      - component group id
 *  @li@c    ui1_num_of_ca_unit          - ca unit info number
 *  @li@c    i2_total_bit_rate           - total bit rate ,Only effect when i2_total_bit_rate is greater than or equal to 0
 *  @li@c    ps_description              - description
 *  @li@c    pt_ca_unit_info_list        - ca unit info list
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_COMPONENT_GROUP_INFO_T
{
    UINT8 ui1_component_group_id;
    UINT8   ui1_num_of_ca_unit;
    INT16 i2_total_bit_rate;     /* Only effect when i2_total_bit_rate is greater than or equal to 0*/
    CHAR* ps_description;
    EDB_CA_UNIT_INFO_T* pt_ca_unit_info_list;
} EDB_COMPONENT_GROUP_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_COMPONENT_GROUP_T
 *  @brief  component group 

 *  @code
 *  typedef struct _EDB_COMPONENT_GROUP_T
 *  {
 *    UINT8     ui1_component_group_type;
 *    BOOL      b_total_bit_rate_flag;
 *    UINT8     ui1_num_of_group;
 *    EDB_COMPONENT_GROUP_INFO_T* pt_component_group_list;
 *  } EDB_COMPONENT_GROUP_T;
 *  @endcode

 *  @li@c    ui1_component_group_type    - component group type
 *  @li@c    b_total_bit_rate_flag       - total bit rate flag
 *  @li@c    ui1_num_of_group            - group number
 *  @li@c    pt_component_group_list     - component group list
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_COMPONENT_GROUP_T
{
    UINT8 ui1_component_group_type;
    BOOL  b_total_bit_rate_flag;
    UINT8   ui1_num_of_group;
    EDB_COMPONENT_GROUP_INFO_T* pt_component_group_list;
} EDB_COMPONENT_GROUP_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_COMPONENT_GROUP_LIST_T
 *  @brief  component group list 

 *  @code
 *  typedef struct _EDB_COMPONENT_GROUP_LIST_T
 *  {
 *    UINT8                    ui1_num;
 *    EDB_COMPONENT_GROUP_T*            pt_component_group;
 *  } EDB_COMPONENT_GROUP_LIST_T;
 *  @endcode

 *  @li@c    ui1_num                     - component group number
 *  @li@c    pt_component_group          - component_group
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_COMPONENT_GROUP_LIST_T
{
    UINT8                    ui1_num;
    EDB_COMPONENT_GROUP_T*            pt_component_group;
} EDB_COMPONENT_GROUP_LIST_T;

    /*------------------------------------------------------------------*/
    /*! @struct _EDB_EVENT_SVC_EVENT_T
     *  @brief  Event svc
    
     *  @code
     
     typedef struct _EDB_EVENT_SVC_EVENT_T
     {
         UINT16                  ui2_svc_id;
         UINT16                  ui2_event_id;
     } EDB_EVENT_SVC_EVENT_T;


    
     *  @endcode
    
     *  @li@c    ui2_svc_id      - SVC id
     *  @li@c    ui2_event_id           - Event id
     */
    /*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_SVC_EVENT_T
{
    UINT16                  ui2_svc_id;
    UINT16                  ui2_event_id;
} EDB_EVENT_SVC_EVENT_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_ON_TS_SVC_EVT_ID_T
 *  @brief  Event svc event id

 *  @code
 *typedef struct _EDB_ON_TS_SVC_EVT_ID_T
 *{
 *    UINT16 ui2_on_id;
 *    UINT16 ui2_ts_id;
 *    UINT16 ui2_service_id;    
 *    UINT16 ui2_event_id;    
 *} EDB_ON_TS_SVC_EVT_ID_T;


 *  @endcode

 *  @li@c    ui2_on_id      - On id 
 *  @li@c    ui2_ts_id           - ts id 
 *  @li@c   ui2_service_id   -Svc id
 *  @li@c   ui2_event_id       -event id

  */
/*------------------------------------------------------------------*/

typedef struct _EDB_ON_TS_SVC_EVT_ID_T
{
    UINT16 ui2_on_id;
    UINT16 ui2_ts_id;
    UINT16 ui2_service_id;    
    UINT16 ui2_event_id;    
} EDB_ON_TS_SVC_EVT_ID_T;

 /*------------------------------------------------------------------*/
/*! @struct _EDB_CONTENT_IDENTIFIER_DATA_T
 *  @brief  Event content data

 *  @code
 * typedef struct _EDB_CONTENT_IDENTIFIER_DATA_T
 *{
 *    UINT8       ui1_crid_type;
 *    UINT8       ui1_location;
 *    CHAR        s_crid_url[MAX_CRID_URL_INFO];
 *    UINT16      ui2_crid_ref;
 *   // struct _EDB_CONTENT_IDENTIFIER_DATA_T* pt_next;
     
 }EDB_CONTENT_IDENTIFIER_DATA_T;


 *  @endcode

 *  @li@c    ui1_crid_type      - Crid type 
 *  @li@c    ui1_location           -location 
 *  @li@c   s_crid_url              -crid url
 *  @li@c   ui2_crid_ref       -crid reference

 */
/*------------------------------------------------------------------*/
typedef struct _EDB_CONTENT_IDENTIFIER_DATA_T
{
    UINT8       ui1_crid_type;
    UINT8       ui1_location;
    CHAR        s_crid_url[MAX_CRID_URL_INFO];
    UINT16      ui2_crid_ref;
   // struct _EDB_CONTENT_IDENTIFIER_DATA_T* pt_next;
    
}EDB_CONTENT_IDENTIFIER_DATA_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_CONTENT_IDENTIFIER_INFO_T
 *  @brief  Event content identifier

 *  @code
 * typedef struct _EDB_CONTENT_IDENTIFIER_INFO_T
 * {
 *      UINT8 ui1_num;
 *      EDB_CONTENT_IDENTIFIER_DATA_T* pt_content_list;
    
 * }EDB_CONTENT_IDENTIFIER_INFO_T;

 *  @endcode

 *  @li@c    ui1_num      - Number of content identifier descriptor
 *  @li@c    pt_content_list           - Pointer to the array of content identifierlist
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CONTENT_IDENTIFIER_INFO_T
{
     UINT8 ui1_num;
     EDB_CONTENT_IDENTIFIER_DATA_T* pt_content_list;
    
}EDB_CONTENT_IDENTIFIER_INFO_T;

typedef struct _EDB_TURKEY_IDENTIFIER_DATA_T
{
    UINT8	    ui1_turkey_id_type;
    CHAR        s_turkey_id_byte[MAX_TURKEY_ID_LENGTH];
    UINT16      ui2_expiration_date;
}EDB_TURKEY_IDENTIFIER_DATA_T;
typedef struct _EDB_TURKEY_IDENTIFIER_INFO_T
{
     UINT8 ui1_num;
     EDB_TURKEY_IDENTIFIER_DATA_T* pt_turkey_id_list;
}EDB_TURKEY_IDENTIFIER_INFO_T;
/******************************************************
* Event group descriptor
******************************************************/
/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_GROUP_T
 *  @brief  event group info

 *  @code
 * typedef struct _EDB_EVENT_GROUP_T
 * {
 *      UINT8                   ui1_group_type;
 *      UINT8                   ui1_event_count;
 *      EDB_EVENT_SVC_EVENT_T*  pt_svc_event_ids; 
 *      UINT8                   ui1_other_network_event_count;
 *      EDB_ON_TS_SVC_EVT_ID_T* pt_on_ts_svc_event_ids;
 * }EDB_EVENT_GROUP_T;
 *
 *  @endcode

 *  @li@c    ui1_group_type      - group type
 *  @li@c    ui1_event_count     - event count
 *  @li@c    pt_svc_event_ids    - svc event ids
 *  @li@c    ui1_other_network_event_count    - other network event count
 *  @li@c    pt_on_ts_svc_event_ids    - on ts svc event ids
 */
/*------------------------------------------------------------------*/
typedef struct _EDB_EVENT_GROUP_T
{
    UINT8                   ui1_group_type;
    UINT8                   ui1_event_count;
    EDB_EVENT_SVC_EVENT_T*  pt_svc_event_ids;
    UINT8                   ui1_other_network_event_count;
    EDB_ON_TS_SVC_EVT_ID_T* pt_on_ts_svc_event_ids;
} EDB_EVENT_GROUP_T;

typedef struct _N_EDB_POOL_EVENT_INFO_T
{
    UINT16                          ui2_event_id;
    UINT8                           ui1_table_id;
    UINT8                           ui1_sect_num;
    TIME_T                          t_start_time;
    TIME_T                          t_duration;
	UINT16                          ui2_event_length;
	UINT8                           ui1_running_status;
	UINT8                           ui1_pading;
    ISO_639_LANG_T                  t_detail_lang;
    UINT8                           b_caption;
    UINT8                           b_free_ca_mode;
    UINT8                           b_has_private; 
	UINT8                           b_compress_flag;
    UINT32                          ui4_got_mask;
	UINT16							ui2_checksum;
    UINT16                          ui2_system_id_offset;
	UINT16                          ui2_event_category_offset;
    UINT16                          ui2_event_title_offset;
	UINT16                          ui2_event_detail_offset;
    UINT16                          ui2_event_guidance_offset;
	UINT16                          ui2_event_linkage_list_offset;
	UINT16                          ui2_time_shift_offset;
	UINT16                          ui2_ca_system_offset;
	UINT16                          ui2_rating_list_offset;
	UINT16                          ui2_component_list_offset;
	UINT16                          ui2_content_identifier_offset;
	UINT16                          ui2_turkey_identifier_offset;
}N_EDB_POOL_EVENT_INFO_T;
/*------------------------------------------------------------------*/
/*! @struct _EDB_POOL_EVENT_INFO_T
 *  @brief  Event event info

 *  @code
 
* typedef struct _EDB_POOL_EVENT_INFO_T
* {
*
*     UINT16                          ui2_event_id;
*     UINT8                           ui1_table_id;
*     UINT8                           ui1_sect_num;
*     
*     TIME_T                          t_start_time;
*     TIME_T                          t_duration;
*     
*     BOOL                            b_caption;
*     BOOL                            b_free_ca_mode;
*     BOOL                            b_has_private;
*     UINT8                           ui1_num_rating;
*     UINT8                           ui1_inter_mask;   //internal used mask
*     
*     UINT16                          aui2_ca_system_id       [EDB_MAX_NUM_CA_SYSTEM];
*     UINT8                           ui1_num_event_category;
*     UINT8                           aui1_event_category     [EDB_MAX_NUM_CATEGORY];
*    UINT8                           ui1_guidance_mode;
*   CHAR*                           ps_event_title;
*  ISO_639_LANG_T                  t_detail_lang;
*     CHAR*                           ps_event_detail;
*     UINT16                          ui2_short_len;
*     CHAR*                           ps_event_guidance;
* 
*     EDB_EVENT_LINKAGE_LIST_T*       pt_event_linkage_list;
*     EDB_EVENT_EXTENDED_LIST_T*      pt_event_extended_list;
*     EDB_EVENT_TIME_SHIFT*           pt_time_shift;
*     EDB_EVENT_COMPONENT_LIST*       pt_component_list;
*     EDB_EVENT_CA_SYSTEM_ID_LIST_T*  pt_ca_system;
*     EDB_EVENT_RATING_LIST_T*        pt_rating_list;
*     EDB_CONTENT_IDENTIFIER_INFO_T*  pt_content_identifier;   
*     
*
*     EDB_EVENT_GROUP_T*              pt_event_group;
*     EDB_DIGITAL_COPY_LIST_T*        pt_digital_copy_list;
*     EDB_AUDIO_COMPONENT_LIST_T*     pt_audio_comp_list;
*     EDB_CA_CONTRACT_LIST_T*         pt_ca_contract_list;
*     EDB_SERIES_LIST_T*              pt_series_list;
*     EDB_COMPONENT_GROUP_LIST_T*     pt_comp_grp_list;
* 
* 
*     UINT16                          ui2_checksum;
* 
*     UINT32                          ui4_got_mask;
*     UINT32                          ui4_compress_mask;
* 
* }EDB_POOL_EVENT_INFO_T;


 *  @endcode

 *  @li@c    ui2_event_id      -event id
 *  @li@c    ui1_table_id           - table id
 *  @li@c   ui1_sect_num        - section number
 *  @li@c   t_start_time           - event start time 
 *  @li@c   t_duration              - event duration
 *  @li@c   b_caption               - is caption or not 
 *  @li@c   b_free_ca_mode     - is ca mode or not 
 *  @li@c   b_has_private        - has private or not
 *  @li@c   ui1_num_rating      - number of rating 
 *  @li@c   ui1_inter_mask      - internal use mask
 *  @li@c   aui2_ca_system_id   - ca system id
 *  @li@c   ui1_num_event_category  - number of event category
 *  @li@c   aui1_event_category       - event category array
 *  @li@c   ui1_guidance_mode         - guidance mode
 *  @li@c   ps_event_title                  -event title
 *  @li@c   t_detail_lang                   - language
 *  @li@c   ps_event_detail              - event detail
 *  @li@c   ui2_short_len                - short  event text length 
 *  @li@c   ps_event_guidance         -guidance decriptor
 *  @li@c   pt_event_linkage_list       - point to linkage list 
 *  @li@c   pt_event_extended_list      - point to extended list
 *  @li@c   pt_time_shift                   - point to time shift
 *  @li@c   pt_component_list           - point to component list
 *  @li@c   pt_ca_system                   - point to ca system
 *  @li@c   pt_rating_list                      - point to rating list
 *  @li@c   pt_content_identifier           - point to content identifier
 *  @li@c   ui2_checksum                    - check sum
 *  @li@c   ui4_got_mask                   - got mask
 *  @li@c   ui4_compress_mask           -compress mask (for private epg)
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_POOL_EVENT_INFO_T
{
#if 0 /*reserve*/
    DLIST_ENTRY_T(_EDB_POOL_EVENT_INFO_T)        t_link;
    UINT16                          ui2_svl_id;
#endif



    UINT16                          ui2_event_id;
    UINT8                           ui1_table_id;
    UINT8                           ui1_sect_num;
    
    TIME_T                          t_start_time;
    TIME_T                          t_duration;
    
    BOOL                            b_caption;
    BOOL                            b_free_ca_mode;
    BOOL                            b_has_private;
    UINT8                           ui1_num_rating;
    UINT8                           ui1_inter_mask;   //internal used mask
    UINT8                           ui1_running_status;   //internal used mask
    
    UINT16                          aui2_ca_system_id       [EDB_MAX_NUM_CA_SYSTEM];
    UINT8                           ui1_num_event_category;
    UINT8                           aui1_event_category     [EDB_MAX_NUM_CATEGORY];
    UINT8                           ui1_guidance_mode;
    CHAR*                           ps_event_title;
    ISO_639_LANG_T                  t_detail_lang;
    CHAR*                           ps_event_detail;
    UINT16                          ui2_short_len;
    CHAR*                           ps_event_guidance;

    EDB_EVENT_LINKAGE_LIST_T*       pt_event_linkage_list;
    EDB_EVENT_EXTENDED_LIST_T*      pt_event_extended_list;
    EDB_EVENT_TIME_SHIFT*           pt_time_shift;
    EDB_EVENT_COMPONENT_LIST*       pt_component_list;
    EDB_EVENT_CA_SYSTEM_ID_LIST_T*  pt_ca_system;
    EDB_EVENT_RATING_LIST_T*        pt_rating_list;
    EDB_CONTENT_IDENTIFIER_INFO_T*  pt_content_identifier;   
    EDB_TURKEY_IDENTIFIER_INFO_T*   pt_turkey_identifier;   

    EDB_EVENT_GROUP_T*              pt_event_group;
    EDB_DIGITAL_COPY_LIST_T*        pt_digital_copy_list;
    EDB_AUDIO_COMPONENT_LIST_T*     pt_audio_comp_list;
    EDB_CA_CONTRACT_LIST_T*         pt_ca_contract_list;
    EDB_SERIES_LIST_T*              pt_series_list;
    EDB_COMPONENT_GROUP_LIST_T*     pt_comp_grp_list;


    UINT16                          ui2_checksum;

    UINT32                          ui4_got_mask;
    UINT32                          ui4_compress_mask;

}EDB_POOL_EVENT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_POOL_CH_KEY
 *  @brief  Event event pool ch key

 *  @code
 
* typedef struct _EDB_EVENT_POOL_CH_KEY
* {
*     UINT16                          ui2_svl_id;
*     UINT16                          ui2_on_id;
*     UINT16                          ui2_ts_id;
*     UINT16                          ui2_svc_id;
* }EDB_EVENT_POOL_CH_KEY;


 *  @endcode

 *  @li@c    ui2_svl_id     - Svl id 
 *  @li@c    ui2_on_id          - On id
  *  @li@c   ui2_ts_id      - Ts id 
 *  @li@c    ui2_svc_id          - Svc id
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_POOL_CH_KEY
{
    UINT16                          ui2_svl_id;
    UINT16                          ui2_on_id;
    UINT16                          ui2_ts_id;
    UINT16                          ui2_svc_id;
}EDB_EVENT_POOL_CH_KEY;

#if 0 /*reserve*/
typedef DLIST_T(_EDB_EVENT_INFO_T)  EDB_EVENT_POOL_EVENT_LIST_T;
#endif


/*------------------------------------------------------------------*/
/*! @struct _EDB_CHANNEL_GUIDANCE_INFO
 *  @brief  EDB channel guidance info

 *  @code
 * typedef struct _EDB_CHANNEL_GUIDANCE_INFO{
 *      UINT8               ui1_guidance_mode;
 *     CHAR*               ps_event_guidance;
 * }EDB_CHANNEL_GUIDANCE_INFO;


 *  @endcode

 *  @li@c    ui1_guidance_mode      - guidance mode
 *  @li@c    ps_event_guidance           - guidande text
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_CHANNEL_GUIDANCE_INFO{
    UINT8               ui1_guidance_mode;
    CHAR*               ps_event_guidance;
}EDB_CHANNEL_GUIDANCE_INFO;

typedef struct _N_EDB_EVENT_POOL_CH_T
{
    EDB_EVENT_POOL_CH_KEY           t_key;
    TS_DESCR_T                      t_ts_descr;
    UINT16                          ui2_svl_id;
    UINT32                          ui4_channel_id;
    BRDCST_MEDIUM_T                 e_brdcst_medium;
    UINT32                          ui4_time_stamp;
    VOID*                           pv_data;        /*EDB_EVENT_POOL_CH_VER*  */
    HANDLE_T                        h_event_hash;   /* 
                                                       Include pt_present_event pt_following_event
                                                       */
    N_EDB_POOL_EVENT_INFO_T*          pt_present_event;
    N_EDB_POOL_EVENT_INFO_T*          pt_following_event;
    BOOL                            fg_present_event_from_pf;
    BOOL                            fg_follow_event_from_pf;

    x_edb_eng_free_single_event     pf_free_event;
    CHAR                            ac_name[MAX_PROG_NAME_LEN];
    VOID*                           pv_special_data;

    EDB_EVENT_RATING_LIST_T*        pt_pmt_rating_list;
    UINT16                          ui2_raw_ts_id;
	EDB_CHANNEL_GUIDANCE_INFO		t_channel_guidance_info;
    BOOL                            b_eit_pf_flag;
}N_EDB_EVENT_POOL_CH_T;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_POOL_CH_T
 *  @brief  edb event pool channel info

 *  @code
*typedef struct _EDB_EVENT_POOL_CH_T
*{
*   EDB_EVENT_POOL_CH_KEY           t_key;
*   TS_DESCR_T                      t_ts_descr;
*   UINT16                          ui2_svl_id;
*   UINT32                          ui4_channel_id;
*   BRDCST_MEDIUM_T                 e_brdcst_medium;
*    UINT32                          ui4_time_stamp;
*    VOID*                           pv_data;       
*    HANDLE_T                        h_event_hash;   
*    EDB_POOL_EVENT_INFO_T*          pt_present_event;
*    EDB_POOL_EVENT_INFO_T*          pt_following_event;
*    BOOL                            fg_present_event_from_pf;
*
*    x_edb_eng_free_single_event     pf_free_event;
*    CHAR                            ac_name[MAX_PROG_NAME_LEN];
*    VOID*                           pv_special_data;
*
*    EDB_EVENT_RATING_LIST_T*        pt_pmt_rating_list;
*    UINT16                          ui2_raw_ts_id;
*   EDB_CHANNEL_GUIDANCE_INFO   t_channel_guidance_info;
*    BOOL                            b_eit_pf_flag;
*}EDB_EVENT_POOL_CH_T;
* *  @endcode

 *  @li@c    t_key      - EDB_EVENT_POOL_CH_KEY
 *  @li@c    t_ts_descr           - TS descriptor
 *  @li@c   ui2_svl_id              -svl id 
 *  @li@c   ui4_channel_id          -channel id 
 *  @li@c   e_brdcst_medium         - broadcast medium
 *  @li@c   ui4_time_stamp          -time stamp
 *  @li@c   pv_data                 -EDB_EVENT_POOL_CH_VE
 *  @li@c   h_event_hash        - event hash
 *  @li@c   pt_present_event    -point to present event 
 *  @li@c   pt_following_event  - pont to following event 
 *  @li@c   fg_present_event_from_pf  -event from p/f or not
 *  @li@c   pf_free_event                   -point to event free function
 *  @li@c   ac_name                         -channle name
 *  @li@c   pv_special_data                 - sdt p/f flag
 *  @li@c   pt_pmt_rating_list              - point to pmt rating list
 *  @li@c   ui2_raw_ts_id                   - raw ts id
 *  @li@c   t_channel_guidance_info     - channel guidance info
 *  @li@c   b_eit_pf_flag                     - eit pf flag
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_POOL_CH_T
{
    EDB_EVENT_POOL_CH_KEY           t_key;
    TS_DESCR_T                      t_ts_descr;
    UINT16                          ui2_svl_id;
    UINT32                          ui4_channel_id;
    BRDCST_MEDIUM_T                 e_brdcst_medium;
    /*UINT16                        ui2_total_events;*/
    UINT32                          ui4_time_stamp;
    VOID*                           pv_data;        /*EDB_EVENT_POOL_CH_VER*  */
    HANDLE_T                        h_event_hash;   /* 
                                                       ui2_event_id _EDB_EVENT_INFO_T* 
                                                       Include pt_present_event pt_following_event
                                                       */
    EDB_POOL_EVENT_INFO_T*          pt_present_event;
    EDB_POOL_EVENT_INFO_T*          pt_following_event;
    BOOL                            fg_present_event_from_pf;
    BOOL                            fg_follow_event_from_pf;

    x_edb_eng_free_single_event     pf_free_event;
    CHAR                            ac_name[MAX_PROG_NAME_LEN];
    VOID*                           pv_special_data;

    EDB_EVENT_RATING_LIST_T*        pt_pmt_rating_list;
    UINT16                          ui2_raw_ts_id;
	EDB_CHANNEL_GUIDANCE_INFO		t_channel_guidance_info;
    BOOL                            b_eit_pf_flag;
}EDB_EVENT_POOL_CH_T;


/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_POOL_T
 *  @brief  Event pool

 *  @code
 * typedef struct _EDB_EVENT_POOL_T
 * {
 *     UINT16                       ui2_total_channels;
 *     UINT32                       ui4_last_time_stamp;
 *     HANDLE_T                     h_ch_hash;
 * }EDB_EVENT_POOL_T;

 *  @endcode

 *  @li@c    ui2_total_channels      - total channels 
 *  @li@c    ui4_last_time_stamp   - last time stamp
 *  @li@c    h_ch_hash                  - channel hash
 */ 
/*------------------------------------------------------------------*/


typedef struct _EDB_EVENT_POOL_T
{
    UINT16                       ui2_total_channels;
    UINT32                       ui4_last_time_stamp;
    HANDLE_T                     h_ch_hash;/*hash of EDB_EVENT_POOL_CH_T */
}EDB_EVENT_POOL_T;


typedef enum
{
    MD_SEARCH_PROGRESS,  /**< more data is available */
    MD_SEARCH_COMPLETE,  /**< search has completed (no more data available)  */
    MD_SEARCH_ABORTED,   /**< search has been aborted */
    MD_SEARCH_PF_UPDATE,
    MD_SEARCH_INVALIDATED /**< all data in this list is invalid, a new fetch must be created */
} MD_SEARCH_EVENT;//AGuideSearchEvent;

/*------------------------------------------------------------------*/
/*! @struct _EDB_EVENT_DESC_T
 *  @brief  Event descr 

 *  @code
 *   typedef struct _EDB_EVENT_DESC_T
 * {
 *     BRDCST_TYPE_T               e_brdcst_type;
 *     UINT16                      ui2_svl_id;
 *     UINT32                      ui4_channel_id;
 *     UINT16                      ui2_event_id;
 *     UINT8                       ui1_tag;
 *     UINT8                       ui1_tag_ext;
 *   EDB_EVENT_POOL_CH_KEY       t_key; 
 *     UINT8                       ui1_tbl_id;
 *   UINT8                       ui1_sec_num;
 *     CHAR*                       pc_buffer;//for save descript raw data used
 *     BOOL                        b_valid;
 *     SIZE_T                      z_len;
    BOOL                        b_finish;
} EDB_EVENT_DESC_T;


 *  @endcode

 *  @li@c    e_brdcst_type          - broadcast type
 *  @li@c   ui2_svl_id                  - svl id 
 *  @li@c   ui4_channel_id           - channel id 
 *  @li@c   ui2_event_id             - event id 
 *  @li@c   ui1_tag                     - tag 
 *  @li@c   ui1_tag_ext                - tag ext
 *  @li@c   t_key                       - EDB_EVENT_POOL_CH_KEY
 *  @li@c   ui1_tbl_id;                 - table id 
 *  @li@c   ui1_sec_num;            - section number
 *  @li@c   pc_buffer                   - for save descript raw data used
 *  @li@c   b_valid;                    - valide or not 
 *  @li@c   z_len;                      - length 
 *  @li@c   b_finish;                   - is finishe or not
 */
/*------------------------------------------------------------------*/

typedef struct _EDB_EVENT_DESC_T
{
    BRDCST_TYPE_T               e_brdcst_type;
    UINT16                      ui2_svl_id;
    UINT32                      ui4_channel_id;
    UINT16                      ui2_event_id;
    UINT8                       ui1_tag;
    UINT8                       ui1_tag_ext;
    EDB_EVENT_POOL_CH_KEY       t_key; 
    UINT8                       ui1_tbl_id;
    UINT8                       ui1_sec_num;
    CHAR*                       pc_buffer;//for save descript raw data used
    BOOL                        b_valid;
    SIZE_T                      z_len;
    BOOL                        b_finish;
} EDB_EVENT_DESC_T;


typedef VOID (*MD_SEARCH_NOTIFY) (MD_SEARCH_EVENT e_event, VOID *pv_tag);
/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_EDB               */
/*----------------------------------------------------------------------------*/

#endif /* _U_EDB_H_ */


