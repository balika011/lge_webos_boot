/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: x_cl_LZMA_eng.h,v $
 * $Revision: #1 $ 1.0
 * $Date: 2015/01/09 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/2 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 4a9fb4bc9ca23ba07447000acd3bdc4d $ Asa
 *
 * Description: 
 *         This file exports public API of the LZMA engine
 *---------------------------------------------------------------------------*/
#ifndef _X_CL_LZMA_ENG__H_
#define _X_CL_LZMA_ENG__H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This API registers LZMA engine to generic compression library layer. */
extern BOOL x_cl_lzma_init (void);
         
#endif /* _X_CL_LZMA_ENG__H_ */
