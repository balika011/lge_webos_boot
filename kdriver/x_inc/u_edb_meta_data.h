#ifndef _METADATA_SEARCH_H
#define _METADATA_SEARCH_H


#include "x_common.h"
#include "u_edb.h"


#define MAX_FIELD_NAME        (20)
#define MAX_STRING_VALUE      (1024)


/* ------------------------------------------------------------------------ */

typedef enum {
    MD_QUERY_OPERATOR_EQ,
    MD_QUERY_OPERATOR_NEQ,
    MD_QUERY_OPERATOR_GT,
    MD_QUERY_OPERATOR_GTEQ,
    MD_QUERY_OPERATOR_LT,
    MD_QUERY_OPERATOR_LTEQ,
    MD_QUERY_OPERATOR_CONTAINS,
    
    MD_QUERY_OPERATOR_AND = 0x100,
    MD_QUERY_OPERATOR_OR,
    MD_QUERY_OPERATOR_NOT,

    MD_QUERY_OPERATOR_IN = 0x200,
    MD_QUERY_OPERATOR_SORT_ASCENDING = 0x300,
    MD_QUERY_OPERATOR_SORT_DESCENDING 
} METADATA_QUERY_OPERATOR_T;

typedef enum {
    MD_QUERY_VALUE_NONE,
    MD_QUERY_VALUE_FIELD_NAME,
    MD_QUERY_VALUE_INTEGER,
    MD_QUERY_VALUE_BOOLEAN,
    MD_QUERY_VALUE_STRING,
    MD_QUERY_VALUE_STRING_COLLECTION,
    MD_QUERY_VALUE_CHANNEL,
    MD_QUERY_VALUE_CHANNEL_LIST,
    MD_QUERY_VALUE_PARENTAL_RATING,
    MD_QUERY_VALUE_TIME_RANGE,
    MD_QUERY_VALUE_QUERY

} MD_QUERY_VALUE_TYPE_T;


typedef struct {
    CHAR* scheme;
    UINT32 rating;
} MD_QUERY_PARENT_RATING_T;


#if 1
typedef struct {
    UINT32 start;
    UINT32 end;
} MD_QUERY_TIME_RANGE_T;
#endif



//copy from  mda_driver_internal.h
#define MAX_LEN_SUPPORTED_URI    (32)
#define MAX_LEN_SUPPORTED_UID    (16+1)
#define MAX_LEN_SUPPORTED_CCID    (20+1)
//copy from  mda_driver_internal.h end

////////////////////copy from mda_guide_dvb

typedef enum {
    MD_CHANNEL_TYPE_TV,
    MD_CHANNEL_TYPE_RADIO,
    MD_CHANNEL_TYPE_DATA,
    MD_CHANNEL_TYPE_VOD,       /**< virtual VOD channel */
    MD_CHANNEL_TYPE_PPV,
    MD_CHANNEL_TYPE_PLAY_LIST,
    MD_CHANNEL_TYPE_VIRTUAL,   /**< generic virtual channel */
    MD_CHANNEL_TYPE_DVR,       /**< virtual DVR channel */
    MD_CHANNEL_TYPE_SVOD,      /**< subscriber VOD channel */
    MD_CHANNEL_TYPE_FORCE_TUNE,/**< EAS force tune channel */
    MD_CHANNEL_TYPE_URI,       /**< A uri that doesn't exist in the
                                * channel manager */
    MD_CHANNEL_TYPE_WEB        /**< Web portal/walled garden */
} MD_ChannelType;
////////////////////copy from mda_guide_dvb end

////////////////////copy from mda_guide_dvb

typedef enum {
  /* Analog */
  MD_CHANNEL_ID_ANALOG = 0,

  /* DVB */
  MD_CHANNEL_ID_DVB_C = 10,
  MD_CHANNEL_ID_DVB_S,
  MD_CHANNEL_ID_DVB_T,
  MD_CHANNEL_ID_DVB_SI_DIRECT,

  /* ISDB */
  MD_CHANNEL_ID_ISDB_C = 20,
  MD_CHANNEL_ID_ISDB_S,
  MD_CHANNEL_ID_ISDB_T,

  /* ATSC */
  MD_CHANNEL_ID_ATSC_T = 30,

  MD_CHANNEL_ID_IPTV_SDS = 40,
  MD_CHANNEL_ID_IPTV_URI,

} MD_IDType;

#define   MAX_PROG_NAME_LEN     64

typedef struct {
    char name[MAX_PROG_NAME_LEN + 1]; /**< short channel name */
    CHAR uri[MAX_LEN_SUPPORTED_URI]; /**< URI for tuning with */
    UINT32 number; /**< logical channel number */
    CHAR long_name[MAX_PROG_NAME_LEN + 1]; /**< long channel name */
    CHAR description[MAX_PROG_NAME_LEN + 1]; /**< channel description */
    MD_ChannelType chan_type; /**< channel type */
    CHAR uid[MAX_LEN_SUPPORTED_UID]; /**< unique id for channel */
    MD_IDType id_type; /**< OIPF IDs */
    CHAR ccid[MAX_LEN_SUPPORTED_CCID];
    UINT32 onid;
    UINT32 tsid;
    UINT32 sid;
    UINT32 source_id;
} MD_CHANNEL_INFO;
////////////////////copy from mda_guide_dvb end


typedef struct _MD_CHANNEL//mda_channel_str
{
    UINT32  index;         /**< index of this channel in list from 0 */
    MD_CHANNEL_INFO *data;   /**< pointer to static test data */
}MD_CHANNEL;


struct _MD_QUERY_T;

/** The MDAMetadataQueryValue represents a constant value used in a query. */
typedef struct _MD_QUERY_VALUE_T {
    /** Defines which member of the union is in use */
    MD_QUERY_VALUE_TYPE_T e_type;

    union {
        /** For values of type MDA_METADATA_QUERY_VALUE_FIELD_NAME */
        CHAR* ps_field_name_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_INTEGER */
        //UINT64 ui8_integer_value; //Bin modify

        UINT32 ui4_integer_value; //Bin modify

        /** For values of type MDA_METADATA_QUERY_VALUE_BOOLEAN */
        BOOL b_boolean_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_STRING */
        CHAR* ps_string_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_STRING_COLLECTION */
        CHAR* ps_string_collection_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_CHANNEL */
#if 0
        MD_CHANNEL* pt_channel_value;
#else        
        CHAR*   ps_channel_name;
#endif        


        //MDAChannel channelValue;
        /** For values of type MDA_METADATA_QUERY_VALUE_CHANNEL_LIST */
        CHAR* ps_channel_list_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_PARENTAL_RATING */
        MD_QUERY_PARENT_RATING_T t_parent_rating_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_TIME_RANGE */
        MD_QUERY_TIME_RANGE_T t_time_range_value;

        /** For values of type MDA_METADATA_QUERY_VALUE_QUERY */
        struct _MD_QUERY_T* pt_query_value;
    } value;
} MD_QUERY_VALUE_T;


/** The MDAMetadataQuery structure represents a query expression that describes
 *  some or all of the search terms for a metadata query.
 */
typedef struct _MD_QUERY_T{
    /** For use by Galio only */
    UINT32 refcount;

    /** The left-hand side of the query expression. This shall be null for an
     * expression using a unary operator. */
    /*const*/ struct _MD_QUERY_VALUE_T lhs;

    /** The operator used in the query expression */
    METADATA_QUERY_OPERATOR_T e_operator;

    /** The right-hand side of the query expression. */
    /*const*/ struct _MD_QUERY_VALUE_T rhs;

} MD_QUERY_T;


extern INT32 md_travel_pool(HANDLE_T h_edb_client,EDB_MD_SEARCH_T* pt_md_search);

extern INT32 md_search_start(HANDLE_T h_edb_client, EDB_MD_SEARCH_T* pt_md_search);

extern INT32 md_search_stop(HANDLE_T h_edb_client,EDB_BUCKET_INFO_T* pt_bucket);

extern INT32 md_search_get_page(
        HANDLE_T h_edb_client,
        UINT32 ui4_page_number,
        UINT32 ui4_page_size,
        VOID*   pv_callback,
        VOID*   pv_tag
        );

extern EDB_BUCKET_INFO_T* md_search_get_bucket(VOID);

#endif /* included_mda_metadata_search_h */

/* eof */
