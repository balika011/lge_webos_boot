
#ifndef  _X_RAW_SECT_FILTER_H_
#define  _X_RAW_SECT_FILTER_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_raw_sect_filter.h"

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_init
 *
 * Description: This function for section filter initialization 
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: RSFR_OK                      Success to initialize section filter
 *          RSFR_INIT_FAIL               Fail to initialize section filter
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_init( VOID );

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_set_tuner_mode
 *
 * Description: This function for setting the tuner mode
 *
 * Inputs:  e_tuner_mode         Contains a tuner mode
 *
 * Outputs: -
 *
 * Returns: RSFR_OK                         Success to set the tuner type
 *          RSFR_NOT_INIT                   Raw section filter not inited
 *          RSFR_INV_ARG                    Invalid arguments
 *          RSFR_LOCK_ERROR                 Fail to get or release raw section filter lock
 *          RSFR_UNLOCK_ERROR               Fail to release raw section filter lock
 *          RSFR_OPERATION_NOT_AVAILABLE    system error or operation invalid
 *          RSFR_SET_TUNER_TYPE_FAIL        Fail to set tuner type
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_set_tuner_mode(RSF_TUNER_MODE_T     e_tuner_mode);

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_alloc_filter
 *
 * Description: This function for allocating a filter to load section, tuner connection must already be ready
 *
 * Inputs:  ui2_pid         the pid field of a section
 *          e_flag          the flag of the filter request
 *          ui4_buf_size    the buffer size for caching sections
 *
 * Outputs: pui4_filter_id  the id of filter
 *
 * Returns: RSFR_OK                         Success to reserve a filter
 *          RSFR_NOT_INIT                   Raw section filter not inited
 *          RSFR_INV_ARG                    Invalid arguments
 *          RSFR_LOCK_ERROR                 Fail to get raw section filter lock
 *          RSFR_UNLOCK_ERROR               Fail to release raw section filter lock
 *          RSFR_OPERATION_NOT_AVAILABLE    system error or operation invalid
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_alloc_filter( UINT32*           pui4_filter_id, 
                                        UINT16              ui2_pid,
                                        RSF_FILRER_FLAG_T   e_flag,
                                        UINT32              ui4_buf_size);

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_free_filter
 *
 * Description: This function for free a filter
 *
 * Inputs:  ui4_filter_id        the id of filter
 *
 * Outputs: -
 *
 * Returns: RSFR_OK              Success to release a filter
 *          RSFR_INV_ARG         Invalid arguments
 *          RSFR_LOCK_ERROR      Fail to get or release raw section filter lock
 *          RSFR_UNLOCK_ERROR    Fail to release raw section filter lock
 *          RSFR_SRC_CONN_ERROR  Fail to free filter 
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_free_filter(UINT32 ui4_filter_id);

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_start_filter
 *
 * Description: This function for starting to load section by the reserved  filter 
 *
 * Inputs:  ui4_filter_id       the id of filter
 *          pt_req              the filter request arguments
 *          pv_nfy_tag          Contains a user data from API caller. 
 *          pv_nfy              The callback function when section is avaiable
 *
 * Outputs: -
 *
 * Returns: RSFR_OK                         Success to start to load section
 *          RSFR_INV_ARG                    Invalid arguments
 *          RSFR_LOCK_ERROR                 Fail to get or release raw section filter lock
 *          RSFR_UNLOCK_ERROR               Fail to release raw section filter lock
 *          RSFR_ALREADY_STARTED            The filter filter has already started
 *          RSFR_SRC_CONN_ERROR             Tuner connection status error, can not load section
 *          RSFR_OPERATION_NOT_AVAILABLE    system error or operation invalid
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_start_filter( UINT32                        ui4_filter_id,
                                        RSF_REQ_T*                      pt_req,
                                        VOID*                           pv_nfy_tag,
                                        x_raw_sect_filter_nfy_fct       pf_nfy);

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_stop_filter
 *
 * Description: This function for stopping loading secttion 
 *
 * Inputs:  ui4_filter_id        the id of filter
 *
 * Outputs: -
 *
 * Returns: RSFR_OK                         Success to stop loading section
 *          RSFR_INV_ARG                    Invalid arguments
 *          RSFR_LOCK_ERROR                 Fail to get or release raw section filter lock
 *          RSFR_UNLOCK_ERROR               Fail to release raw section filter lock
 *          RSFR_ALREADY_STOPPED            The filter has already stopped
 *          RSFR_NOT_START                  The filter has not started
 *          RSFR_SECTION_BUFFER_NOT_FREE    The section buffer is not freed completely
 *          RSFR_OPERATION_NOT_AVAILABLE    system error or operation invalid
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_stop_filter(UINT32 ui4_filter_id);

/*-----------------------------------------------------------------------------
 * Name: x_raw_sect_filter_free_sect_buf
 *
 * Description: This function for freeing section buffer 
 *
 * Inputs:  ui4_filter_id        the id of filter
 *          ui4_serial           the serial number of this section notify 
 *
 * Outputs: -
 *
 * Returns: RSFR_OK                         Success to stop loading section
 *          RSFR_INV_ARG                    Invalid arguments
 *          RSFR_LOCK_ERROR                 Fail to get or release raw section filter lock
 *          RSFR_UNLOCK_ERROR               Fail to release raw section filter lock
 *          RSFR_ALREADY_STOPPED            The filter has already stopped
 *          RSFR_NOT_START                  The filter has not started
 *          RSFR_OPERATION_NOT_AVAILABLE    system error or operation invalid
 ----------------------------------------------------------------------------*/
INT32 x_raw_sect_filter_free_sect_buf(UINT32 ui4_filter_id, UINT32 ui4_serial);

#endif /* _X_RAW_SECT_FILTER_H_ */

