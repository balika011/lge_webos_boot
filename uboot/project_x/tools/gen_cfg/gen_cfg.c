/*
* Copyright (c) MediaTek Inc.
*
* This program is distributed under a dual license of GPL v2.0 and
* MediaTek proprietary license. You may at your option receive a license
* to this program under either the terms of the GNU General Public
* License (GPL) or MediaTek proprietary license, explained in the note
* below.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 as
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
*
/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * $RCSfile: gen_cfg.c,v $
 * $Revision: #2 $
 * $Date: 2015/04/01 $
 * $Author: p4admin $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: e97a84de05bddd9a7a208405b6f42137 $
 *
 * Description:
 *         This file generates a C source file with pre-defined sections.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/x_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Version 1.01. */
#define VER_STRING  "Rev: 1.00"

/* Return values. */
#define RET_OK                        ((INT32)   0)
#define RET_INV_ARGUMENT              ((INT32)  -1)
#define RET_MISSING_ARGUMENTS         ((INT32)  -2)
#define RET_INPUT_ALREADY_OPEN        ((INT32)  -3)
#define RET_CANNOT_OPEN_INPUT         ((INT32)  -4)
#define RET_CANNOT_ALLOC_OUTPUT_CTRL  ((INT32)  -5)
#define RET_CANNOT_OPEN_OUTPUT        ((INT32)  -6)
#define RET_ROOT_ALREADY_DEFINED      ((INT32)  -7)
#define RET_CANNOT_SET_ROOT           ((INT32)  -8)
#define RET_WRITE_FAILURE             ((INT32)  -9)
#define RET_MISSING_SQUARE_BRACKT     ((INT32) -10)
#define RET_MISSING_OUT_FILE_NAME     ((INT32) -11)

/* Argument control types. */
#define ARG_FLAG_ABORT   ((UINT16) 0x0001)
#define ARG_FLAG_NO_ARG  ((UINT16) 0x0002)

/* Input string size */
#define MAX_INP_STR  1024


/* Configuration path defines. */
#define CFG_PATH_START  '['
#define CFG_PATH_END    ']'

#define CFG_FILE_APPEND  '&'
#define CFG_FILE_NAME       "cfg.mak.tmp"
#define FINAL_CFG_FILE_NAME "cfg.mak"

/* Argument process control block and callback function. */
typedef INT32 (*cmd_prc_fct) (CHAR* ps_arg);

typedef struct _PRC_CMD_T
{
    CHAR*        ps_cmd;
    cmd_prc_fct  pf_cmd_prc;
    UINT16       ui2_flags;
}   PRC_CMD_T;

/* The following configuration structure contains a single */
/* output file description. Immediately after the end of   */
/* this structure, the absolute output configuration file  */
/* name is append.                                         */
typedef struct _CFG_OUT_T
{
    struct _CFG_OUT_T*  pt_next;

    FILE*  pt_out_file;
}   CFG_OUT_T;

/*The structure conatins the cfg file name*/
typedef struct _CFG_OUT_FILE_T
{
    struct _CFG_OUT_FILE_T*  pt_next;

    CHAR *  ps_file_name;
}   CFG_OUT_FILE_T;


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static FILE*  pt_inp_file = NULL;    /* References the configuration file. */

static CFG_OUT_T*  pt_head_cfg_out = NULL;  /* Refernces the configuration output files. */

static CHAR*  ps_root = NULL;        /* References the root path. */

static CHAR  s_inp_str  [MAX_INP_STR];
static CHAR  s_rel_path [MAX_INP_STR];

static BOOL  b_clean = FALSE;

static CFG_OUT_FILE_T* pt_head_out_file = NULL;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static INT32 cmd_help (CHAR*  ps_arg);
static INT32 cmd_input (CHAR*  ps_arg);
static INT32 cmd_clean (CHAR*  ps_arg);
static INT32 cmd_root (CHAR*  ps_arg);
static INT32 proc_args (INT32       i4_argc,
                        VOID**      ppv_argv,
                        PRC_CMD_T*  pt_cmd_arr);
static CHAR* skip_ws (CHAR*  ps_str);
static CHAR* get_rel_path (CHAR*  ps_str);
static INT32 open_cfg_out (CHAR*  ps_rel_path,
                           BOOL   b_append);
static VOID close_cfg_out (VOID);
static INT32 wrt_cfg_out (CHAR*  ps_str);
static INT32 prc_cfg (VOID);
static INT32 cmp_cfg_file(VOID);


/* Argument control array. */
static PRC_CMD_T t_cmd_arr [] =
{
    {"-h",    cmd_help,  (ARG_FLAG_ABORT | ARG_FLAG_NO_ARG)},
    {"-help", cmd_help,  (ARG_FLAG_ABORT | ARG_FLAG_NO_ARG)},
    {"-i",    cmd_input, 0},
    {"-c",    cmd_clean,  0},
    {"-r",    cmd_root,  0},
    {NULL,    NULL,      0}
};


/*-----------------------------------------------------------------------------
 * Name:  cmd_help
 *
 * Description: This function shows the help menu. The argument ps_arg will
 *              not be used.
 *
 * Inputs:  ps_arg  Ignored.
 *
 * Outputs: -
 *
 * Returns: RET_OK  Success.
 ----------------------------------------------------------------------------*/
static INT32 cmd_help (CHAR* ps_arg)
{
    printf ("\n");
    printf ("gen_cfg    %s\n\n", VER_STRING);
    printf ("This program creates from a configuration file one or more make files which contain\n");
    printf ("settings used during the build process of a final image.\n");
    printf ("Usage:\n");
    printf ("   gen_cfg  -i <input file> -r <root path>\n\n");
    printf ("      -i  Specifies the configuraion input file name. This argument is required.\n");
    printf ("      -c  clean configuration files.\n");
    printf ("      -r  Specifies the root path. This argument is optional.\n");
    printf ("      -h  Displays this help menu.\n");
    printf ("\n");

    return (RET_OK);
}


/*-----------------------------------------------------------------------------
 * Name:  cmd_input
 *
 * Description: This function opens the configuration input file.
 *
 * Inputs:  ps_arg  References the input file name.
 *
 * Outputs: -
 *
 * Returns: RET_OK                 Success
 *          RET_CANNOT_OPEN_INPUT  Input file could not be opened.
 ----------------------------------------------------------------------------*/
static INT32 cmd_input (CHAR* ps_arg)
{
    INT32 i4_return;


    i4_return = RET_INPUT_ALREADY_OPEN;

    if (pt_inp_file == NULL)
    {
        i4_return = RET_CANNOT_OPEN_INPUT;

        if ((pt_inp_file = fopen (ps_arg, "r")) != NULL)
        {
            i4_return = RET_OK;
        }
        else
        {
            printf ("\n");
            printf ("ERROR: Could not open input file: %s\n\n", ps_arg);
        }
    }
    else
    {
        printf ("\n");
        printf ("ERROR: Only one input file can be opened.\n\n");
    }

    return (i4_return);
}
/*-----------------------------------------------------------------------------
 * Name:  cmd_clean
 *
 * Description: This function cleans the configuration output files.
 *
 * Inputs:  ps_arg  References the input file name.
 *
 * Outputs: -
 *
 * Returns: RET_OK                 Success
 *          RET_CANNOT_OPEN_INPUT  Input file could not be opened.
 ----------------------------------------------------------------------------*/
static INT32 cmd_clean (CHAR* ps_arg)
{
    INT32 i4_return;

    b_clean = TRUE;
    
    i4_return = RET_INPUT_ALREADY_OPEN;

    if (pt_inp_file == NULL)
    {
        i4_return = RET_CANNOT_OPEN_INPUT;

        if ((pt_inp_file = fopen (ps_arg, "r")) != NULL)
        {
            i4_return = RET_OK;
        }
        else
        {
            printf ("\n");
            printf ("ERROR: Could not open input file: %s\n\n", ps_arg);
        }
    }
    else
    {
        printf ("\n");
        printf ("ERROR: Only one input file can be opened.\n\n");
    }

    return (i4_return);
}

/*-----------------------------------------------------------------------------
 * Name:  cmd_root
 *
 * Description: This function processes the root command.
 *
 * Inputs:  ps_arg  References the root path.
 *
 * Outputs: -
 *
 * Returns: RET_OK                   Success
 *          RET_OUTPUT_ALREADY_OPEN  Output file is alreday open.
 *          RET_CANNOT_OPEN_OUTPUT   Output file could not be opened.
 ----------------------------------------------------------------------------*/
static INT32 cmd_root (CHAR* ps_arg)
{
    INT32 i4_return;


    i4_return = RET_ROOT_ALREADY_DEFINED;

    if (ps_root == NULL)
    {
        i4_return = RET_CANNOT_SET_ROOT;

        if (ps_arg == NULL)
        {
            if ((ps_root = (CHAR*) malloc (1)) != NULL)
            {
                i4_return = RET_OK;

                (*ps_root) = '\0';
            }

            else
            {
                printf ("\n");
                printf ("ERROR: Could not set root: \".\"\n\n");
            }
        }
        else
        {
            if ((ps_root = (CHAR*) malloc (strlen (ps_arg) + 1)) != NULL)
            {
                i4_return = RET_OK;

                strcpy (ps_root, ps_arg);
            }
            else
            {
                printf ("\n");
                printf ("ERROR: Could not set root: \"%s\"\n\n", ps_arg);
            }
        }
    }
    else
    {
        printf ("\n");
        printf ("ERROR: Only one root can be set.\n\n");
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  proc_args
 *
 * Description: This rountine processes the arguments passed to the main
 *              function. In case of an error the argument processing will be
 *              aborted. It is the responsibility of the caller to perform any
 *              cleanup of allocated resources.
 *
 * Inputs:  i4_argc     Contains the number of arguments.
 *          ppv_argv    References the array of arguments.
 *          pt_cmd_arr  References the command process control structure.
 *
 * Outputs: -
 *
 * Returns: RET_OK if all is ok else some negative error value.
 ----------------------------------------------------------------------------*/
static INT32 proc_args (INT32       i4_argc,
                        VOID**      ppv_argv,
                        PRC_CMD_T*  pt_cmd_arr)
{
    CHAR* ps_arg;
    INT32 i4_arg_idx;
    INT32 i4_return;
    INT32 i4_cmd_arr_idx;

    i4_return  = RET_OK;
    i4_arg_idx = 1;

    while ((i4_return == RET_OK)   &&
           (i4_arg_idx < i4_argc))
    {
        ps_arg         = NULL;
        i4_cmd_arr_idx = 0;

        /* Search for command. */
        while ((pt_cmd_arr [i4_cmd_arr_idx].ps_cmd != NULL)                                                                                    &&
               ((ps_arg = strstr ((CHAR*) (ppv_argv [i4_arg_idx]), pt_cmd_arr [i4_cmd_arr_idx].ps_cmd)) != ((CHAR*) (ppv_argv [i4_arg_idx]))))
        {
            i4_cmd_arr_idx++;
        }

        if (pt_cmd_arr [i4_cmd_arr_idx].ps_cmd != NULL)
        {
            /* Process command. */
            ps_arg += strlen (pt_cmd_arr [i4_cmd_arr_idx].ps_cmd);
            if (((pt_cmd_arr [i4_cmd_arr_idx].ui2_flags & ARG_FLAG_NO_ARG) == 0) &&
                (strlen (ps_arg) == 0))
            {
                i4_arg_idx++;

                if (i4_arg_idx < i4_argc)
                {
                    ps_arg = (CHAR*) (ppv_argv [i4_arg_idx]);
                }
                else
                {
                    i4_return = RET_INV_ARGUMENT;

                    printf ("\n");
                    printf ("ERROR: Missing argument: %s\n\n", (CHAR*) (ppv_argv [i4_arg_idx - 1]));
                }
            }

            if (i4_return == RET_OK)
            {
                i4_return = pt_cmd_arr [i4_cmd_arr_idx].pf_cmd_prc (ps_arg);
            }

            /* Abort further processing if b_abort == TRUE. */
            if ((pt_cmd_arr [i4_cmd_arr_idx].ui2_flags & ARG_FLAG_ABORT) != 0)
            {
                i4_arg_idx = i4_argc;
            }
        }
        else
        {
            i4_return = RET_INV_ARGUMENT;

            printf ("\n");
            printf ("ERROR: Unknown argument: %s\n\n", (CHAR*) (ppv_argv [i4_arg_idx]));
        }

        i4_arg_idx++;
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  skip_ws
 *
 * Description: This rountine skips any white space.
 *
 * Inputs:  ps_str  References the string were white spaces shall be skipped.
 *
 * Outputs: -
 *
 * Returns: A reference to the next non-white space character.
 ----------------------------------------------------------------------------*/
static CHAR* skip_ws (CHAR*  ps_str)
{
    while (((*ps_str) != '\0') &&
           ((*ps_str) <= ' '))
    {
        ps_str++;
    }

    return (ps_str);
}


/*-----------------------------------------------------------------------------
 * Name:  get_rel_path
 *
 * Description: This rountine copies a relative path from the input string and
 *              returns a reference to the first character after the copied
 *              relative path.
 *
 * Inputs:  ps_str  References the input string
 *
 * Outputs: -
 *
 * Returns: A reference to the next character after the copied relative path.
 ----------------------------------------------------------------------------*/
static CHAR* get_rel_path (CHAR*  ps_str)
{
    CHAR* ps_rel_path;


    ps_rel_path = &(s_rel_path [0]);

    while (((*ps_str) > ' ')            &&
           ((*ps_str) != CFG_PATH_END))
    {
        (*(ps_rel_path++)) = (*(ps_str++));
    }

    (*ps_rel_path) = '\0';

    return (ps_str);
}


/*-----------------------------------------------------------------------------
 * Name:  open_cfg_out
 *
 * Description: This rountine opens a configuration output file.
 *
 * Inputs:  ps_rel_path  References the relative path.
 *          b_append     Indicates if the output file shall be oppened in
 *                       append or truncate mode.
 *
 * Outputs: -
 *
 * Returns: RET_OK if all is ok else some negative error value.
 ----------------------------------------------------------------------------*/
static INT32 open_cfg_out (CHAR*  ps_rel_path,
                           BOOL   b_append)
{
    INT32      i4_return;
    SIZE_T     z_len;
    BOOL       b_exist = FALSE;
    FILE*      pt_file_handle = NULL;
    CFG_OUT_T* pt_cfg_out;

    CHAR*            ps_file_name;
    CFG_OUT_FILE_T*  pt_cfg_out_file;

    i4_return = RET_CANNOT_ALLOC_OUTPUT_CTRL;

    /* Calculate the space for the absolute path of the cnfiguration file. */
    z_len = 0;
    if (ps_root != NULL)
    {
        z_len     += strlen ((const char*) ps_root) + 1;
    }

    z_len += strlen ((const char*) ps_rel_path) + 1 + strlen ((const char*) CFG_FILE_NAME) + 1;

    if ((pt_cfg_out = (CFG_OUT_T*) malloc (sizeof (CFG_OUT_T) + z_len)) != NULL &&
        (pt_cfg_out_file = (CFG_OUT_FILE_T*) malloc (sizeof (CFG_OUT_FILE_T))) != NULL &&
        (ps_file_name = (CHAR*) malloc (z_len)) != NULL)
    {
        i4_return = RET_CANNOT_OPEN_OUTPUT;

        pt_cfg_out->pt_out_file = NULL;
        pt_cfg_out->pt_next     = pt_head_cfg_out;
        pt_head_cfg_out         = pt_cfg_out;

        if (ps_root != NULL)
        {

            sprintf (((CHAR*) &(pt_cfg_out [1])), "%s/%s/%s", ps_root, ps_rel_path, CFG_FILE_NAME);
     
            /* save cfg.mak.tmp file name*/    
            sprintf((CHAR*)ps_file_name , "%s/%s/%s", ps_root, ps_rel_path, CFG_FILE_NAME);   
        }
        else
        {
            sprintf (((CHAR*) &(pt_cfg_out [1])), "%s/%s", ps_rel_path, CFG_FILE_NAME);
            /* save cfg.mak.tmp file name*/ 
            sprintf((CHAR*) ps_file_name , "%s/%s", ps_rel_path, CFG_FILE_NAME);
        }

        if((pt_file_handle = (fopen(((const char*) &(pt_cfg_out [1])), "r" ))) != NULL)
        {
            b_exist = TRUE;
            fclose (pt_file_handle);  
        }
                
        if ((pt_cfg_out->pt_out_file = fopen (((const char*) &(pt_cfg_out [1])), (b_append ? "a" : "w"))) != NULL)
        {
            if(!b_exist)
            {
                pt_cfg_out_file->ps_file_name = ps_file_name;
                pt_cfg_out_file->pt_next      = pt_head_out_file;
                pt_head_out_file          = pt_cfg_out_file;                
            }
            i4_return = RET_OK;
            
        }
        else
        {
            printf ("\n");
            printf ("ERROR: Cannot open file \"%s\"\n\n", ((CHAR*) &(pt_cfg_out [1])));
        }
    }
    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  cleab_cfg_out
 *
 * Description: This rountine cleans a configuration output file.
 *
 * Inputs:  ps_rel_path  References the relative path.
 *
 * Outputs: -
 *
 * Returns: RET_OK if all is ok else some negative error value.
 ----------------------------------------------------------------------------*/
static INT32 clean_cfg_out (CHAR*  ps_rel_path)
{
    INT32      i4_return;
    SIZE_T     z_len;
    CFG_OUT_T* pt_cfg_out;


    i4_return = RET_CANNOT_ALLOC_OUTPUT_CTRL;

        /* Calculate the space for the absolute path of the cnfiguration file. */
    z_len = 0;
    if (ps_root != NULL)
    {
        z_len += strlen ((const char*) ps_root) + 1;
    }

    z_len += strlen ((const char*) ps_rel_path) + 1 + strlen ((const char*) FINAL_CFG_FILE_NAME) + 1;
    
    if ((pt_cfg_out = (CFG_OUT_T*) malloc (sizeof (CFG_OUT_T) + z_len)) != NULL)
    {
        pt_cfg_out->pt_out_file = NULL;
        pt_cfg_out->pt_next     = pt_head_cfg_out;
        pt_head_cfg_out         = pt_cfg_out;

        if (ps_root != NULL)
        {
            sprintf (((CHAR*) &(pt_cfg_out [1])), "%s/%s/%s", ps_root, ps_rel_path, FINAL_CFG_FILE_NAME);
        }
        else
        {
            sprintf (((CHAR*) &(pt_cfg_out [1])), "%s/%s", ps_rel_path, FINAL_CFG_FILE_NAME);
        }

        printf("remove %s\n", (CHAR*) &(pt_cfg_out [1]));
        remove((CHAR*) &(pt_cfg_out [1]));
    }

    return RET_OK;
}



/*-----------------------------------------------------------------------------
 * Name:  close_cfg_out
 *
 * Description: This rountine closes all configuration output files currently
 *              open.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID close_cfg_out (VOID)
{
    CFG_OUT_T* pt_cfg_out;

    while ((pt_cfg_out = pt_head_cfg_out) != NULL)
    {
        pt_head_cfg_out = pt_cfg_out->pt_next;

        if (pt_cfg_out->pt_out_file != NULL)
        {
            fclose (pt_cfg_out->pt_out_file);
        }

        free ((void*) pt_cfg_out);
    }
}


/*-----------------------------------------------------------------------------
 * Name:  wrt_cfg_out
 *
 * Description: This rountine writes a single line to all configuration output
 *              files. In case the single line is empty or no output file is
 *              opened, thi API will take no action.
 *
 * Inputs:  ps_str  References the single line.
 *
 * Outputs: -
 *
 * Returns: RET_OK if all is ok else some negative error value.
 ----------------------------------------------------------------------------*/
static INT32 wrt_cfg_out (CHAR*  ps_str)
{
    INT32      i4_return;
    CFG_OUT_T* pt_cfg_out;


    i4_return = RET_OK;

    if ((ps_str                         != NULL)  &&
        ((*ps_str)                      != '\0')  &&
        ((pt_cfg_out = pt_head_cfg_out) != NULL))
    {
        while ((i4_return == RET_OK) &&
               (pt_cfg_out != NULL))
        {
            if (!b_clean)
            {
                if (fputs (((const char*) ps_str), pt_cfg_out->pt_out_file) == EOF)
                {
                    i4_return = RET_WRITE_FAILURE;
    
                    printf ("\n");
                    printf ("ERROR: Write failure in file \"%s\"\n\n", ((CHAR*) &(pt_cfg_out [1])));
                }
            }

            pt_cfg_out = pt_cfg_out->pt_next;
        }
    }

    return (i4_return);
}

static INT32 cmp_cfg_file(VOID)
{
    INT32 i4_return = RET_OK;
    CFG_OUT_FILE_T*   pt_cfg_out_file;  

    while ((pt_cfg_out_file = pt_head_out_file) != NULL)
    {
        FILE* pt_cfg_file_tmp = NULL;
        FILE* pt_cfg_file = NULL; 
        BOOL  b_is_same = TRUE;
        CHAR* ps_file_name = NULL;
      
        if ((pt_cfg_out_file->ps_file_name != NULL)&&
            (pt_cfg_file_tmp = fopen ((CHAR*)pt_cfg_out_file->ps_file_name, "r" )) != NULL)
        {
            SIZE_T t_len = strlen((CHAR*)pt_cfg_out_file->ps_file_name);

            if((ps_file_name = (CHAR*) malloc (t_len + 1)) != NULL)
            {
                strcpy(ps_file_name, pt_cfg_out_file->ps_file_name);
                ps_file_name[t_len - 4] = '\0';   
            }
            if ((ps_file_name != NULL)&&
                (pt_cfg_file = fopen ((CHAR*)ps_file_name, "r" )) != NULL)  
            {

                /*compare if the cfg.mak and cfg.mak.tmp is same*/
                CHAR s_str      = fgetc(pt_cfg_file);
                CHAR s_str_tmp  = fgetc(pt_cfg_file_tmp);
                while(!(feof (pt_cfg_file_tmp)) && !(feof (pt_cfg_file)))
                {
                    if(s_str != s_str_tmp)
                    {
                        b_is_same = FALSE;
                        break;
                    }
                    s_str = fgetc(pt_cfg_file);    
                    s_str_tmp = fgetc(pt_cfg_file_tmp);
                }
                if (!(s_str == EOF && s_str_tmp == EOF))
                {
                    b_is_same = FALSE;
                }
                fclose (pt_cfg_file);    
            }
            else
            {
                b_is_same = FALSE;
            }   
            fclose (pt_cfg_file_tmp);  
            
            if(b_is_same)
            {
                remove((CHAR*) pt_cfg_out_file->ps_file_name);
            }
            else
            {
                remove((CHAR*) ps_file_name);
                rename((CHAR*) pt_cfg_out_file->ps_file_name, (CHAR*) ps_file_name);
                
            }   
            
        }
        else
        {
            i4_return = RET_CANNOT_OPEN_OUTPUT;
        }
        if(ps_file_name != NULL)
        {
            free((void*)ps_file_name);
        }
        if(pt_cfg_out_file->ps_file_name != NULL)
        {
            free((void*) pt_cfg_out_file->ps_file_name);
        }
        pt_head_out_file = pt_cfg_out_file->pt_next;        
        if(pt_cfg_out_file != NULL)
        {
            free((void*)pt_cfg_out_file);
        }
      
    }

    return i4_return;
}


/*-----------------------------------------------------------------------------
 * Name:  proc_cfg
 *
 * Description: This rountine processes the configuration file.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: RET_OK if all is ok else some negative error value.
 ----------------------------------------------------------------------------*/
static INT32 prc_cfg (VOID)
{
    BOOL  b_append;
    CHAR* ps_str;
    INT32 i4_return;


    i4_return = RET_OK;
    while ((i4_return == RET_OK)                                                    &&
           (! (feof (pt_inp_file)))                                                 &&
           ((ps_str = fgets (&(s_inp_str [0]), MAX_INP_STR, pt_inp_file)) != NULL))
    {
        ps_str = skip_ws (&(s_inp_str [0]));

        /* Process the new configuration path. */
        if ((*ps_str) == CFG_PATH_START)
        {
            close_cfg_out ();

            ps_str = skip_ws (ps_str + 1);

            while ((i4_return == RET_OK)       &&
                   ((*ps_str) != CFG_PATH_END) &&
                   ((*ps_str) != '\0'))
            {
                /* Detect if the output file shall be append or truncate. */
                b_append = ((*ps_str) == CFG_FILE_APPEND);

                if (b_append)
                {
                    ps_str = skip_ws (ps_str + 1);
                }

                if (((*ps_str) != CFG_PATH_END) &&
                    ((*ps_str) != '\0'))
                {
                    ps_str = get_rel_path (ps_str);

                    if (!b_clean)
                    {
                        i4_return = open_cfg_out (&(s_rel_path [0]), b_append);
                    }
                    else
                    {
                        i4_return = clean_cfg_out (&(s_rel_path [0]));
                    }
                    ps_str = skip_ws (ps_str);
                }
                else
                {
                    i4_return = RET_MISSING_OUT_FILE_NAME;

                    printf ("\n");
                    printf ("ERROR: Missing output file name in line: \"%s\"\n\n", &(s_inp_str [0]));
                }
            }
            
            if (i4_return == RET_OK)
            {
                if ((*ps_str) == CFG_PATH_END)
                {
                    wrt_cfg_out (ps_str + 1);
                }
                else
                {
                    i4_return = RET_MISSING_SQUARE_BRACKT;

                    printf ("\n");
                    printf ("ERROR: Missing ']' in line: \"%s\"\n\n", &(s_inp_str [0]));
                }
            }
        }
        else
        {
            wrt_cfg_out (&(s_inp_str [0]));
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name:  main
 *
 * Description: This is the main entry point of the 'gen_cfg' processor.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          ppv_argv  References the arguments.
 *
 * Outputs: -
 *
 * Returns: 0               Success
 *          Negative value  Failure
 ----------------------------------------------------------------------------*/
int main (int     i4_argc,
          char**  ppv_argv)
{
    INT32 i4_return;
    /* Parse "gen_cfg" command arguments. */
    if ((i4_return = proc_args (((INT32) i4_argc), ((VOID**) ppv_argv), &(t_cmd_arr [0]))) == RET_OK)
    {
        i4_return = RET_MISSING_ARGUMENTS;

        if (pt_inp_file != NULL)
        {
            i4_return = prc_cfg ();
        }
        else
        {
            printf ("\n");
            printf ("ERROR: Missing arguments.\n\n");
        }
    }   
    /* Close and free allocated resources. */
    close_cfg_out ();
    
    /*compare if the cfg.mak.tmp is same with cfg.mak*/
    i4_return = cmp_cfg_file();

    if (pt_inp_file != NULL)
    {
        fclose (pt_inp_file);
    }

    if (ps_root != NULL)
    {
        free ((VOID*) ps_root);
    }

    return ((int) i4_return);
}
