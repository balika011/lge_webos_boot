/**@@@+++@@@@******************************************************************
**
** Microsoft (r) PlayReady (r)
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

/******************************************************************************
**
** This file declares a minimal XMR license parser to enable parsing specific objects out of XMR inside the secure crypto core.
** OEMs should not need to change this file in any way (except to get it to compile properly).
** If the device supports a secure crypto core, this file should compile ONLY for that core.
**
*******************************************************************************
*/

#ifndef __OEM_SPARSE_XMR_PARSER_H__
#define __OEM_SPARSE_XMR_PARSER_H__

#include <drmxmr.h>

ENTER_PK_NAMESPACE;

DRM_API DRM_RESULT DRM_CALL OEM_XMR_SparseUnpackLicense(
    __in_bcount( f_cbLicense ) const DRM_BYTE                    *f_pbLicense,
    __in                             DRM_DWORD                    f_cbLicense,
    __out                            DRM_XMR_LICENSE             *f_pXmrLicense );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_GetContainer(
    __in    const DRM_BYTE                     *f_pbLicense,
    __in          DRM_DWORD                     f_cbLicense,
    __in          DRM_WORD                      f_wContainerType,
    __inout       DRM_VOID                     *f_pvContainer );

/******************************************************************************
**
** Function :   OEM_XMR_ValidateLicenseObjects
**
** Synopsis :   Determines if the minimum set of objects is available
**              in this license to perform the sparse license validation 
**              required for key extraction.
**
** Arguments :
**      [f_pXmrLicense]   : License to validate
**
** Returns :    DRM_SUCCESS             if the license is valid
**              DRM_E_INVALID_LICENSE   if the license is not valid
**              DRM_E_INVALIDARG        if f_pXmrLicense is NULL
**
******************************************************************************/
DRM_API DRM_RESULT DRM_CALL OEM_XMR_ValidateSparseLicenseObjects(
    __in const DRM_XMR_LICENSE *f_pXmrLicense );

/******************************************************************************
**
** Function :   OEM_XMR_PARSE
**
** Synopsis :   Function Prototype to parse a serialized object buffer
**
** Arguments :
**      [f_pbBuffer] : Object Buffer
**      [f_iObject]  : Index marking beginning of object in the buffer
**      [f_cbObject] : Size of serialized object
**      [f_pvObject] : Actual object returned as void
**
** Returns :
**
** Notes :      Callers must keep the stack around as long as they use the
**              contents of the structure returned. The structure might have
**              some intermediate nodes allocated out of this stack
**
******************************************************************************/
typedef DRM_RESULT  ( DRM_CALL *OEM_XMR_PARSE )(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_Invalid(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_DWORD_Versioned(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_ContentKey(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_RsaPubkey(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_EccDevicekey(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_UplinkKID(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_SecurityLevel(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_UplinkKID2(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_OuterContainer(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_GlobalContainer(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_KeyMaterialContainer(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_Signature(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_OptimizedContentKey(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_AuxKey(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_UplinkX(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );

DRM_API DRM_RESULT DRM_CALL OEM_XMR_FindObject(
    __in                             DRM_WORD   f_wObjectType,
    __in_bcount( f_cbLicense ) const DRM_BYTE  *f_pbLicense,
    __in                             DRM_DWORD  f_cbLicense,
    __out                            DRM_DWORD *f_piObject,
    __out                            DRM_DWORD *f_pcbObject );

PREFAST_PUSH_DISABLE_EXPLAINED(__WARNING_NONCONST_PARAM_25004, "Prefast Noise: f_pvObject cannot be const for function to match parsing fn prototype")
DRM_API DRM_RESULT DRM_CALL OEM_XMR_Parse_Ignore(
    __in    const DRM_BYTE                    *f_pbBuffer,
    __in          DRM_DWORD                    f_iObject,
    __in          DRM_DWORD                    f_cbObject,
    __inout       DRM_VOID                    *f_pvObject );
PREFAST_POP /* __WARNING_NONCONST_PARAM_25004 */


typedef struct
{
    OEM_XMR_PARSE           fnParse;
    DRM_BOOL                rgfIsVersionSupported[XMR_VERSION_MAX];
} XMR_SPARSE_PARSER_INTERNAL_DEFINES;

EXIT_PK_NAMESPACE;

#endif  /* __OEM_SPARSE_XMR_PARSER_H__ */

