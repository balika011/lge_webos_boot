//CC_AUD_AQ_IN_DRVCUST
#define AUD_EXTERN_FILE


//=======================
//  [!!!!**CAUTION**!!!!]
// Please exclude Loader
//=======================
#if ((!defined(CC_MTK_PRELOADER)) && (!defined(CC_MTK_LOADER))) 


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
/* =======================================================
 [!!!!**CAUTION**!!!!]
 You can only include header 
    files in \target\mtxxxx\xxxx_driver\drv_inc\
 or files in \vm_linux\output\sysbuild\xxxx\rel(or debug)\inc\drv_inc\
 or files in "the same directory."
========================================================= */
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_util.h"
#include "aud_if.h"
#include "drvcust_if.h"
#include "nor_if.h"
#include "nand_if.h"
#include "storage_if.h"


//---------------------------------------------------------------------------
// Const defitions
//---------------------------------------------------------------------------
#if 0  // Develope your code here.
#define AQ_LEN          (200*1024)  // In Bytes.
#define AQ_PARTITION    16          // AQ Partition on Flash.
#define VOL_TAB_OFFSET  8       // The offset of vol table on dram.
#endif


//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

// =================================================
// [!!!!**CAUTION**!!!!]
// 1. Use printf, not LOG
// 2. You can only call public functions in xxx_if.c
//==================================================


INT32 DrvCust_AQ_SetGet(AUD_DrvCust_AQ_OP_T eType, void* arg1, void* arg2, UINT32 arg3, UINT32 arg4)
{
    UNUSED(DrvCust_AQ_SetGet);
    UNUSED(eType);
    UNUSED(arg1);
    UNUSED(arg3);
    UNUSED(arg4);    

#if 0 // Develope your code here.
    switch (eType)
    {
    case AUD_DrvCust_AQ_Set_Init:
        DrvCust_AQ_Init();
        break;
    case AUD_DrvCust_AQ_Get_VolTal:
        DrvCust_AQ_GetVolTab((UINT32*)arg1, arg3);
        break;        
    default:
        return;
    }
#endif

    return 0;    
}


//-----------------------------------------------------------------------------
// [Example 1] DrvCust_AQ_Init
//
//    Get AQ data from flash, and put data to DRAM.
// 
//  Return 0  : OK
//         -1 : NG
//-----------------------------------------------------------------------------

INT32 DrvCust_AQ_Init(void)
{
    UNUSED(DrvCust_AQ_Init);
#if 0 // Develope your code here.
    UINT32 i;
    UINT8* pu1DataAddr;
    UINT8 u1AQPartition;    

    u1AQPartition = AQ_PARTITION;
    printf("AQ! - u1AQPartition = %d\n", u1AQPartition);
    
    pu1DataAddr = (UINT8*)x_mem_alloc(AQ_LEN);
    if (pu1DataAddr == NULL)
    {
        printf("AQ! - memory allocate failed!! (Size=%d Bytes)\n", AQ_LEN);
        return -1; 
    }
    
    STORG_SyncRead(u1AQPartition, 0, (VOID*)pu1DataAddr, AQ_LEN);
    vAprocAqData_Write(0, pu1DataAddr, AQ_LEN);    
    x_mem_free(pu1DataAddr);
#endif    

    return 0;
}


//-----------------------------------------------------------------------------
// [Example 2]  DrvCust_AQ_GetVolTab
//
//    Vol table from audio driver, is set value from AQ.
// 
//  Return 0  : OK
//         -1 : NG
//-----------------------------------------------------------------------------
INT32 DrvCust_AQ_GetVolTab(UINT32* pu4VolTab, UINT32 u4Size)
{
    UNUSED(DrvCust_AQ_GetVolTab);
    UNUSED(pu4VolTab);
    UNUSED(u4Size);

#if 0 // Develope your code here.
    UINT32 u4AqVolTable[100];

    // Get Vol Table from AQ Dram
    vAprocAqData_Read(VOL_TAB_OFFSET, (UINT8*)u4AqVolTable, 100*4);    
    x_memcpy((VOID*)(VIRTUAL(pu4VolTab)), (const VOID *)(VIRTUAL(u4AqVolTable)), u4Size);
#endif    

   return 0;
}


#endif //#if ((!defined(CC_MTK_PRELOADER)) && (!defined(CC_MTK_LOADER))) 

