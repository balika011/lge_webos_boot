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
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: u_secm_api.h,v $
 * $Revision: #1 $
 * $Date: 2015/01/09 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

#ifndef _U_SECM_API_H_
#define _U_SECM_API_H_


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* the following defines are also used in */
/* secm_set_mon_filter() and secm_set_acq_filter() */
#define SECM_VBIT_NO_CRC_CHECK      MAKE_BIT_MASK_16(8)  /* no CRC check */
#define SECM_VBIT_NO_VER_CHECK      MAKE_BIT_MASK_16(9)  /* no ver check */

#define SECM_VBIT_NO_TS_ID_CHECK    MAKE_BIT_MASK_16(10)  /* no TS_ID check */
#define SECM_VBIT_NO_ON_ID_CHECK    MAKE_BIT_MASK_16(11)  /* no ON_ID check */

#define SECM_VBIT_SYN_IND_MASK      (((UINT16) 0x3) << 12)

/* 0x00, accept sections regardless section_syntax_indicator bit */
#define SECM_VBIT_SYN_IND_IGNORE    ((UINT16) 0x0000)

/* 0x01, accept sections with section_syntax_indicator bit set */
#define SECM_VBIT_SYN_IND_SET       (((UINT16) 0x1) << 12)

/* 0x10, accept sections with section_syntax_indicator bit clear */
#define SECM_VBIT_SYN_IND_CLEAR     (((UINT16) 0x2) << 12)

/* 0x11, invalid setting */
#define SECM_VBIT_SYN_IND_INVALID   (((UINT16) 0x3) << 12)

#define SECM_VBIT_PRIV_BIT_MASK     (((UINT16) 0x3) << 14)

/* 0x00, accept sections regardless section_private bit */
#define SECM_VBIT_PRIV_BIT_IGNORE   ((UINT16) 0x0000)

/* 0x01, accept sections with section_private bit set */
#define SECM_VBIT_PRIV_BIT_SET      (((UINT16) 0x1) << 14)

/* 0x10, accept sections with section_private bit clear */
#define SECM_VBIT_PRIV_BIT_CLEAR    (((UINT16) 0x2) << 14)

/* 0x11, invalid setting */
#define SECM_VBIT_PRIV_BIT_INVALID  (((UINT16) 0x3) << 14)

#define SECM_VBIT_PLOAD_BITS \
    (SECM_VBIT_PAYLOAD0 | SECM_VBIT_PAYLOAD1 | \
     SECM_VBIT_PAYLOAD2 | SECM_VBIT_PAYLOAD3)

#define SECM_VBIT_SEC_BITS \
    (SECM_VBIT_EXTID | SECM_VBIT_BEGIN_SECNO | SECM_VBIT_PAYLOAD0 | \
     SECM_VBIT_PAYLOAD1 | SECM_VBIT_PAYLOAD2 | SECM_VBIT_PAYLOAD3)

#define SECM_SEC_VBITS \
    (SECM_VBIT_SEC_BITS | SECM_VBIT_SYN_IND_MASK | SECM_VBIT_PRIV_BIT_MASK)



#define SECM_RAW_REQ_OFFSET_MIN     ((UINT8) 1)
#define SECM_RAW_REQ_OFFSET_MAX     ((UINT8) 31)
#define SECM_RAW_REQ_PATTERN_LEN_MAX \
    (UINT8) (SECM_RAW_REQ_OFFSET_MAX - SECM_RAW_REQ_OFFSET_MIN + 1)

#define SECM_RAW_SPEC_NULL              ((UINT16) 0x0000)
#define SECM_RAW_SPEC_NO_CRC_CHECK      SECM_VBIT_NO_CRC_CHECK
#define SECM_RAW_SPEC_NO_VER_CHECK      SECM_VBIT_NO_VER_CHECK
#define SECM_RAW_SPEC_NO_TS_ID_CHECK    SECM_VBIT_NO_TS_ID_CHECK
#define SECM_RAW_SPEC_NO_ON_ID_CHECK    SECM_VBIT_NO_ON_ID_CHECK
#define SECM_RAW_SPEC_MARK_SECTION      MAKE_BIT_MASK_16(12)

/* SECM_PID_FROM_LOGICAL_FILTER: indicates PID is specified (taken) from */
/* secm_alloc_mon_filter() and secm_alloc_acq_filter() */
/* otherwise, PID is specified (taken) from */
/* secm_set_mon_filter() and secm_set_acq_filter() */


#define SECM_PID_FROM_LOGICAL_FILTER    ((UINT8) 0x01)

/* SECM_RESERVE_HW_FILTER instructs section manager to reserved (allocated) */
/* a HW PID filter and a HW section filter if such resources are */
/* still available, otherwise, a SECMR_NO_RESOURCE is returned from the API */
/* the HW resources reserved (allocated) are held up until a close is issed */

#define SECM_RESERVE_HW_FILTERS         ((UINT8) 0x02)

/*
 * source state notify function
 * e_nfy_cond : either SECM_COND_SRC_DISC or SECM_COND_SRC_CONN
 */

#define SECM_COND_SRC_DISC          ((SECM_COND_T) 0xd0)
#define SECM_COND_SRC_CONN          ((SECM_COND_T) 0xc0)

typedef enum
{
    SRC_COMP_TYPE_TUNER = 0,
    SRC_COMP_TYPE_POD_STACK,
    SRC_COMP_TYPE_UT,
    SRC_COMP_TYPE_IP_STACK,
    SRC_COMP_TYPE_BUF_AGT
} SRC_COMP_TYPE_T;

typedef struct _SECM_SRC_COMP_TUNER
{
    DRV_TYPE_T  e_drv_type;     /* source component type */
    UINT16      ui2_id;         /* source component id */
} SECM_SRC_COMP_TUNER_T;


typedef struct _SECM_SRC_COMP_POD
{
    /* TODO: re-visit */
    DRV_TYPE_T  e_drv_type;     /* source component type */
    UINT16      ui2_id;         /* source component id */
    /* and more */
} SECM_SRC_COMP_POD_T;


typedef struct _SECM_SRC_COMP_IP
{
    /* TODO: re-visit */
    DRV_TYPE_T  e_drv_type;     /* source component type */
    UINT16      ui2_id;         /* source component id */
    /* and more */
} SECM_SRC_COMP_IP_T;


typedef struct
{
    DRV_TYPE_T  e_drv_type;
    UINT16      ui2_id;
    /* and more? */
}   SECM_SRC_COMP_BUF_AGT_T;


typedef struct _SECM_SRC_COMP
{
    DRV_TYPE_T  e_drv_type;     /* source component type */
    UINT16      ui2_id;         /* source component id */
} SECM_SRC_COMP_T;

typedef struct _SECM_RAW_REQ
{
    UINT8   *pui1_pos_pattern;      /* positive pattern */
    UINT8   *pui1_pos_pat_mask;     /* positive pattern mask */
    UINT8   *pui1_neg_pattern;      /* negative pattern */
    UINT8   *pui1_neg_pat_mask;     /* negative pattern mask */
    UINT8   ui1_tbl_id;             /* section: byte0, table id */
    UINT8   ui1_tbl_id_mask;        /* table id mask */
    UINT8   ui1_pattern_len;        /* pos/net patten length */
    UINT8   ui1_pattern_offset;     /* patten offset: 1..31 */
    UINT16  ui2_spec;
} SECM_RAW_REQ_T;

/*for mw to update the section mask*/
typedef enum
{
    SECM_EIT_SET_SECTION = 0,
    SECM_EIT_SET_TABLE,
    SECM_EIT_SET_CHANNEL,
    SECM_EIT_SET_ALL_CHANNEL
}SECM_EIT_SET_TYPE_T;

typedef struct _SECM_SECTION_KEY_SET
{
    SECM_EIT_SET_TYPE_T eSetType;

    UINT8  ui1_tableid; //talbe id 
    UINT8  ui1_version; //talbe version
    UINT8  ui1_secnum; //section number.

    /*CHANNEL*/
    UINT16 ui2_onid;
    UINT16 ui2_tsid;
    UINT16 ui2_svcid;
}SECM_SECTION_KEY_SET_T;


typedef UINT16 SECM_COND_T;

typedef VOID (* secm_src_nfy_fct)(VOID          *pv_nfy_tag,
                                  SECM_COND_T   e_nfy_cond);


/*
 * return code:
 *      the number of bytes specified in "z_copy_len" or
 *      0 for no data being copied
 */
typedef SIZE_T (* secm_copy_fct) (VOID     *pv_src_tag,
                                  UINT8    *pui1_dest,
                                  SIZE_T   z_skip_len,
                                  SIZE_T   z_copy_len);

/* 
 * notification function, pv_nfy in ecm_alloc_acq_filter() or
 * ecm_alloc_mon_filter().
 *
 * pv_nfy_tag: notification tag to pf_nfy in secm_alloc_acq_filter() or
 *             secm_alloc_mon_filter().
 * e_nfy_cond: contains notification condition.
 * pv_src_tag: contains a reference to the source (section tabel). this is
 *             used in SECM_NFY_INFO_T and is passed to secm_get_data().
 * z_data_len: contains length of the section table.
 *
 * return code:
 *      TRUE: notification function is done with the section data. 
 *      FALSE: notification function could not process the section completely
 *             and needs to re-acquire it again.
 */
typedef BOOL (* secm_nfy_fct)(VOID          *pv_nfy_tag, /* from API caller */
                              SECM_COND_T   e_nfy_cond,  /* from sec mngr */
                              VOID          *pv_src_tag, /* from sec mngr */
                              SIZE_T        z_data_len,  /* total section len */
                              secm_copy_fct pf_copy);


#endif /* _U_SECM_API_H_ */
