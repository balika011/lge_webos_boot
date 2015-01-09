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
/******************************************************************************
* [File]			msdc_test.c
* [Version]			v1.0
* [Revision Date]	2013-02-28
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2013-02-28
* [Description]
*	Test code about MSDC
* [Copyright]
*	Copyright (C) 2007 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/

#include "x_typedef.h"
#include "x_ldr_env.h"
#include "x_assert.h"
#include "x_util.h"
#include "x_rand.h"
#include "x_timer.h"

#include "msdc_debug.h"
#include "msdc_host_hw.h"
#include "msdc_slave_hw.h"
#include "msdc_drv.h"
#include "msdc_if.h"

// For SD gpio detection pin
#include "x_gpio.h"

#if 1//def CC_MSDC_SDMMC_TEST

extern sdcard_t sdCard[];
extern sdhost_t sdHost[];
extern msdc_env_t msdcenv;

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Definition
//---------------------------------------------------------------------------

extern UINT32 msdcClk[][MSDC_CLK_IDX_MAX];
extern UINT32 ch;

//static HANDLE_T _hMSDCCardDetThrd = NULL;
//static INT32 _i4CardRemove = 1;
    
#define Reg_Addr(rel)   (MSDC_BASE_ADDR + (ch * MSDC_CH_OFFSET) + rel)
#define MSDC_CARD_DET_STACK_SZ        (1024)
#define MSDC_CARD_DET_PRIORITY        (40)
#define MSDC_CARD_DET_PERIOD          (500)

#define MSDC_DATA_START_ADDR          (0x0)
#define MSDC_DATA_CHUNK_SIZE          (0x20000)
#define MSDC_DATA_TOTAL_SIZE          (0x100000)

#define MSDC_DATA_TEST_MODE1          (0x1)
#define MSDC_DATA_TEST_MODE2          (0x2)
#define MSDC_DATA_TEST_MODE3          (0x3)

#define SEED0					0x12345678
#define SEED1					0x87654321
#define SEED2					0x56781234
#define SEED3					0x43218765
#define SEED4					0xABCDEFAB
#define SEED5					0xFEDCBA12
#define SEED6					0x05162738
#define SEED7					0xF0E1D2C3
#define SEED8					0xB2A3C5D6
#define SEED9					0x2B3A5C6D
#define SEEDA					0xFADBCC66
#define SEEDB					0x66CCBDAF

#define for_loop_inc(__idx, __idx_min, __idx_max)    for(__idx=__idx_min;__idx<=__idx_max;__idx++)
#define for_loop_dec(__idx, __idx_min, __idx_max)    for(__idx=__idx_max;__idx<=__idx_min;__idx--)


INT32 MSDCTest_Register(VOID)
{
    UINT32 i = 0, j = 0, val_1, val_2;
    
    char *bit_attr[] = {"aaaaaarraaaaaaaaaaxxxxxxxxxxxxxx",  /*0x00*/
                        "aaaaaaxxaaaaaaxxaaaaaaaaaaxxxxxx",
                        "arxxxxxxxxxxaaaarrrrrrrrrxxxxxxr",
                        "ccxccccccccrccccccccxxxxxxxxxxxx",
                        "aaxaaaaaaaaaaaaaaaaaxxxxxxxxxxxx",  /*0x10*/
                        "rrrrrrrrxxxxxxxxrrrrrrrrxxxxxxxa",
                        "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "aaxxxxxxxxxxxxxxaaxaaaxxaaaaaaaa",  /*0x30*/
                        "aaaaaaaaaaxaaaaaaaaaaaaaaaaaaaax",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "rrxxxxxxxxxxxxxxxxxxxxxxxxxxxxxr",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",  /*0x40*/
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",  /*0x50*/
                        "cccccccccccccccccccccccccccccccc",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "rrrrrrrrrrrrrrrrxxxxxxxxxxxxxxxx",  /*0x60*/
                        "wwaaxxxxxxxxaaaaxxxxxxxxxxxxxxxx",  /*0x70*/
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "ccccrcrxxxxxxxxxxxxxxxxxxxxxxxxx",
                        "axxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",  /*0x80*/
                        "aaaaxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",  /*0x90*/
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "wawxxxxxaxaxaaaxxxxxxxxxxxxxxxxx",
                        "raxxxxxxaaxxaaxxaxxxxxxxxxxxxxxx",
                        "aaaaaaaaaaaaaaaaxxxxxxxxxxxxxxxx",  /*0xA0*/
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "aaaaaaaaaaaaaaaaaaaaaaxxxxaaaaaa",  /*0xB0*/
                        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                        "aaaxaaaxaaaaxxxaaaaaaaaaaaaaaaaa",  /*0xE0*/
                        "aaaxaaaxaaaaxxxaaaaaaaaaaaaaaaaa",
                        "aaaxaaaxaaaaxxxaaaaaaaaaaaaaaaaa",
                        "aaaaaxxxaaaaaxxxaaaaaxaaaaaaaaaa",
                        "aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx",  /*0xF0*/
                        "aaaaaxxxaaaaaxxxaaaaaxxxaaaaaxxx",
                        "aaaaaaaaaaaaaaxxaaaaaaaaaaaaaaax",
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr",  /*0x100*/
                        "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"};
    
    char *bit_val[] = {"100110010000000000xxxxxxxxxxxxxx",  /*0x00*/
                       "000000xx000000xx0000000000xxxxxx",
                       "00xxxxxxxxxx0000000000000xxxxxx0",
                       "00x00000000000000000xxxxxxxxxxxx",
                       "00x00000000000000000xxxxxxxxxxxx",  /*0x10*/
                       "00000000xxxxxxxx00000000xxxxxxx0",
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",
                       "00xxxxxxxxxxxxxx00x010xx00000000",  /*0x30*/
                       "0000000000x00000000000000000000x",
                       "00000000000000000000000000000000",
                       "00xxxxxxxxxxxxxxxxxxxxxxxxxxxxx0",
                       "00000000000000000000000000000000",  /*0x40*/
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",
                       "10000000000000000000000000000000",  /*0x50*/
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",
                       "0000000000000000xxxxxxxxxxxxxxxx",  /*0x60*/
                       "0000xxxxxxxx0000xxxxxxxxxxxxxxxx",  /*0x70*/
                       "11000000000000000000010000000000",
                       "0000000xxxxxxxxxxxxxxxxxxxxxxxxx",
                       "0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                       "00000000000000000000000000000000",  /*0x80*/
                       "0000xxxxxxxxxxxxxxxxxxxxxxxxxxxx",
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",  /*0x90*/
                       "00000000000000000000000000000000",
                       "000xxxxx0x0x011xxxxxxxxxxxxxxxxx",
                       "00xxxxxx00xx00xx0xxxxxxxxxxxxxxx",
                       "0000000000000000xxxxxxxxxxxxxxxx",  /*0xA0*/
                       "00000000000000000000000000000000",
                       "00000000000000000000000000000000",
                       "1111001000000000001111xxxx000010",  /*0xB0*/
                       "10010000000000001111111111111111",
                       "000x000x0000xxx11001000000000000",  /*0xE0*/
                       "000x000x0000xxx00101000000000000",
                       "000x000x0000xxx00101000000000000",
                       "00000xxx00000xxx00000x0000000000",
                       "00000xxx00000xxx00000xxx00000xxx",  /*0xF0*/
                       "00000xxx00000xxx00000xxx00000xxx",
                       "00000000000000xx000000000000000x",
                       "00000000000000000000000000000000",  /*0x100*/
                       "00000000000000000000000000000000"};

    UINT16 reg_addr[] = {0x00, 0x04, 0x08, 0x0C, 0x10, 0x14, 0x18, 0x1C, 0x30,
                        0x34, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x50, 0x58, 
                        0x5C, 0x60, 0x70, 0x74, 0x78, 0x7C, 0x80, 0x84, 0x88,
                        0x90, 0x94, 0x98, 0x9C, 0xA0, 0xA4, 0xA8, 0xB0, 0xB4,
                        0xE0, 0xE4, 0xE8, 0xEC, 0xF0, 0xF4, 0xF8, 0x100, 0x104};

#if 0
    /*verify the validation about string array between attribute and value*/
    if(sizeof(bit_attr) != sizeof(bit_val))
    {
        Printf("size dismatch(%d <-> %d) between array bit_attr and bit_val!\n", sizeof(bit_attr), 
                                                                                 sizeof(bit_val));
        return MSDC_FAILED;
    }
    for(;i<sizeof(bit_attr)/sizeof(bit_attr[0]);i++)
    {
        for(;j<sizeof(bit_attr[i]);j++)
        {
            if(sizeof(*bit_attr[i]) != sizeof(*bit_val[i]))
            {
                Printf("size dismatch(%d <-> %d) between array bit_attr[%d] and bit_val[%d]!\n", sizeof(*bit_attr[i]), 
                                                                                                 sizeof(*bit_val[i]), 
                                                                                                 i, i);
                return MSDC_FAILED;
            }

            if((*(*(attr_ptr + i) + j) == 'x') &&
               (*(*(val_ptr + i) + j) != 'x'))
            {
                Printf("value-attr dismatch(%c <-> %c) between array bit_attr[%d][%d] and bit_val[%d][%d]!\n", 
                                                                                                 *(*(attr_ptr + i) + j), 
                                                                                                 *(*(val_ptr + i) + j), 
                                                                                                 i, j, i, j);
                return MSDC_FAILED;
            }

            if((*(*(val_ptr + i) + j) == 'x') &&
               (*(*(attr_ptr + i) + j) != 'x'))
            {
                Printf("value-attr dismatch(%c <-> %c) between array bit_attr[%d][%d] and bit_val[%d][%d]!\n", 
                                                                                                 *(*(attr_ptr + i) + j), 
                                                                                                 *(*(val_ptr + i) + j), 
                                                                                                 i, j, i, j);
                return MSDC_FAILED;
            }
        }
    }
#endif
    
    for(i=0;i<sizeof(bit_attr)/sizeof(char*);i++)
    {
        val_1 = MSDC_READ32(Reg_Addr(reg_addr[i]));
        for(j=0;j<x_strlen(bit_attr[0]);j++)
        {
            if((bit_attr[i][j] != 'w') &&
               (bit_attr[i][j] != 'x'))
            {
                if(((val_1>>j)&0x1) != (bit_val[i][j] - '0'))
                {
                    Printf("default value(%08x = %08x) dismatch(bit %d in reg %04X)!\n", Reg_Addr(reg_addr[i]), val_1,
                                                                                         j, reg_addr[i]);
                    //return MSDC_FAILED;
                }
            }
        }
    }
    Printf("PASS: Register default value at reset is identical with CODA, except write only and don't care registers\n");

    for(i=0;i<sizeof(bit_attr)/sizeof(char *);i++)
    {
        val_1 = MSDC_READ32(Reg_Addr(reg_addr[i]));
        for(j=0;j<x_strlen(bit_attr[0]);j++)
        {
            if(bit_attr[i][j] == 'r')
            {
                if((val_1>>j)&0x1)
                {
                    MSDC_CLRBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }
                else
                {
                    MSDC_SETBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }

                val_2 = MSDC_READ32(Reg_Addr(reg_addr[i]));
                if(((val_1>>j)&0x1) != ((val_2>>j)&0x1))
                {
                    Printf("default value dismatch(bit %d in reg %04X)!\n", j, reg_addr[i]);
                    //return MSDC_FAILED;
                }
            }
        }
    }
    Printf("PASS: Read only register bits can not be modified\n");

    for(i=0;i<sizeof(bit_attr)/sizeof(char *);i++)
    {
        val_1 = MSDC_READ32(Reg_Addr(reg_addr[i]));
        for(j=0;j<x_strlen(bit_attr[0]);j++)
        {
            if(bit_attr[i][j] == 'a')
            {
                if((val_1>>j)&0x1)
                {
                    MSDC_CLRBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }
                else
                {
                    MSDC_SETBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }

                val_2 = MSDC_READ32(Reg_Addr(reg_addr[i]));
                if(((val_1>>j)&0x1) == ((val_2>>j)&0x1))
                {
                    Printf("write 1/0 to writable and readable register failed(bit %d in reg %04X)(val1 %08x val2 %08x)!\n", j, reg_addr[i],
                                                                                                                              val_1, val_2);
                    //return MSDC_FAILED;
                }

                if((val_1>>j)&0x1)
                {
                    MSDC_SETBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }
                else
                {
                    MSDC_CLRBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }

                val_2 = MSDC_READ32(Reg_Addr(reg_addr[i]));
                if(((val_1>>j)&0x1) != ((val_2>>j)&0x1))
                {
                    Printf("write 0/1 to writable and readable register failed(bit %d in reg %04X)(val1 %08x val2 %08x)!\n", j, reg_addr[i],
                                                                                                                              val_1, val_2);
                    //return MSDC_FAILED;
                }

                if(bit_val[i][j] == '1')
                {
                    MSDC_SETBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }
                else
                {
                    MSDC_CLRBIT(Reg_Addr(reg_addr[i]), (0x1<<j));
                }
                
            }
        }
    }
    Printf("PASS: Readable and writable register bits can be written and read back in the same value\n");
     
    return MSDC_SUCCESS;
}
#if 0
static VOID MSDCTest_DetThrd(VOID *arg)
{
    INT32 i4CardRemove = _i4CardRemove;
        
    while(1)
    {
        i4CardRemove = MSDCDetectSDGPIO();
        if(i4CardRemove != _i4CardRemove)
        {
            if(i4CardRemove == 0)
            {
                Printf("sd/mmc card is inserted(^_^)\n");
            }
            else
            {
                Printf("sd/mmc card is removed(T_T)\n");
            }
        }

        x_thread_delay(MSDC_CARD_DET_PERIOD);
    }

}
#endif
INT32 MSDCTest_Detection(VOID)
{
    INT32 i4Ret = MSDC_SUCCESS;
#if 0
    if(_hMSDCCardDetThrd == NULL)
    {
        i4Ret = x_thread_create(&_hMSDCCardDetThrd, "MSDC_DET",
                                MSDC_CARD_DET_STACK_SZ, MSDC_CARD_DET_PRIORITY,
                                MSDCTest_DetThrd, 0,
                                NULL);

    }
#endif
    return i4Ret;
}

INT32 MSDCTest_Protection(VOID)
{

    return MSDC_SUCCESS;
}

INT32 MSDCTest_Configuration(UINT32 clk, UINT32 datamode, UINT32 buswidth,
                                     UINT32 speedmode)
{
    INT32 i4Ret = MSDC_SUCCESS;

    i4Ret = MsdcSetBusWidth(buswidth);
    if(MSDC_SUCCESS != i4Ret)
    {
        goto end;    
    }

    i4Ret = MsdcSetClkFreq(clk);
    if(MSDC_SUCCESS != i4Ret)
    {
        goto end;    
    }

    i4Ret = MsdcSetDataMode(datamode);
    if(MSDC_SUCCESS != i4Ret)
    {
        goto end;    
    }

end:
    return i4Ret;
}

VOID MSDCTet_DataRandomGen(UINT32 *pRamAddr, UINT32 len, UINT32 seed)
{
    UINT32 i = 0;
    INT32 rand_val = 0;

    srand(seed);
    for(;i<len;i+=sizeof(INT32))
    {
        rand_val = rand();
        x_memcpy((void *)(pRamAddr+(i>>2)), (void *)(&rand_val), sizeof(INT32));
    }
    
#if 1
    MSDC_LOG(MSG_LVL_ERR, "*****random data generated!*****\n");
    for(i = 0;i<0x10;i+=0x10)
    {
        if(i%0x10 == 0x0)
        {
            MSDC_LOG(MSG_LVL_ERR, "%08x | %08x %08x %08x %08x\n", ((UINT32)pRamAddr + i), 
                                                                   *(pRamAddr+i),
                                                                   *(pRamAddr+i+1),
                                                                   *(pRamAddr+i+2),
                                                                   *(pRamAddr+i+3));
        }
    }
    MSDC_LOG(MSG_LVL_ERR, "********************************\n");
#endif
}

INT32 MSDCTest_DataCompare(UINT32 *pRamAddr1,  UINT32 *pRamAddr2, UINT32 len)
{
    UINT32 i = 0;

    for(;i<len;i+=4)
    {
        if(*(pRamAddr1+(i>>2)) != *(pRamAddr2+(i>>2)))
        {
            MSDC_LOG(MSG_LVL_ERR, "compare addr1(%08x) to addr2(%08x) failed:(%08x %08x)!\n",
                                     (UINT32)(pRamAddr1+(i>>2)), (UINT32)(pRamAddr2+(i>>2)),
                                     *(pRamAddr1+(i>>2)), *(pRamAddr2+(i>>2)));
            return MSDC_FAILED;
        }
    }

    return MSDC_SUCCESS;
}

INT32 MSDCTest_DataTransmission(UINT32 *pSrcWrite,  UINT32 *pDestRead, UINT32 testmode)
{
    UINT32 total_sz = MSDC_DATA_TOTAL_SIZE,
           chuck_sz = MSDC_DATA_CHUNK_SIZE,
           i = 0, step_sz = SDHC_BLK_SIZE;
	INT32 i4Ret = MSDC_SUCCESS;

    if(MSDC_DATA_TEST_MODE1 == testmode)
    {
        step_sz = SDHC_BLK_SIZE;
    }
    else if(MSDC_DATA_TEST_MODE2 == testmode)
    {
        step_sz = total_sz;
    }
    else if(MSDC_DATA_TEST_MODE3 == testmode)
    {
        step_sz = chuck_sz;
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "not support data test mode(%d)", testmode);
        i4Ret = MSDC_FAILED;
        goto end;
    }

    MSDC_LOG(MSG_LVL_ERR, "*****data transmission test*****\n");
    MSDC_LOG(MSG_LVL_ERR, "Test Mode: %d\n", testmode);
    MSDC_LOG(MSG_LVL_ERR, "Src Addr for Write: %08x\n", (UINT32)pSrcWrite);
    MSDC_LOG(MSG_LVL_ERR, "Dest Addr for Read: %08x\n", (UINT32)pDestRead);
    MSDC_LOG(MSG_LVL_ERR, "Block Size: %08x\n", SDHC_BLK_SIZE);
    MSDC_LOG(MSG_LVL_ERR, "Chuck Size: %08x\n", chuck_sz);
    MSDC_LOG(MSG_LVL_ERR, "Total Size: %08x\n", total_sz);
    MSDC_LOG(MSG_LVL_ERR, "Step Size: %08x\n", step_sz);
    MSDC_LOG(MSG_LVL_ERR, "********************************\n");
    

    for(i=0;i<total_sz;i+=step_sz)
    {
        i4Ret = MsdcWriteCard((UINT32 *)(pSrcWrite+(i>>2)), (UINT64)(MSDC_DATA_START_ADDR+i), step_sz); 
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "write data from dram(0x%08x) to emmc/sd(0x%08x%08x) in test mode(%d) faield!\n",
                                    (UINT32)(pSrcWrite+(i>>2)), (UINT32)(((UINT64)(MSDC_DATA_START_ADDR+i))>>32),
                                    (UINT32)((UINT64)(MSDC_DATA_START_ADDR+i)), testmode);
            goto end;
        }
    }

    for(i=0;i<total_sz;i+=step_sz)
    {
        i4Ret = MsdcReadCard((UINT64)(MSDC_DATA_START_ADDR+i), (UINT32 *)(pDestRead+(i>>2)), step_sz); 
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "write data from  emmc/sd(0x%08x%08x) to dram(0x%08x) in test mode(%d) faield!\n",
                                           (UINT32)(((UINT64)(MSDC_DATA_START_ADDR+i))>>32),
                                           (UINT32)((UINT64)(MSDC_DATA_START_ADDR+i)), (UINT32)(pSrcWrite+(i>>2)), 
                                            testmode);
            goto end;
        }
    }

    i4Ret = MSDCTest_DataCompare(pSrcWrite, pDestRead, total_sz); 
    if(MSDC_SUCCESS != i4Ret)
    {
        goto end;
    }

end:
    return i4Ret;
}

/* testing objective:
* 1. kinds of different data transfer test
* 2. single, multiple read/write interleave test at normal temperature
*/
INT32 MSDCTest_DataAutoTest(UINT32 *pSrcWrite,  UINT32 *pDestRead)
{
    UINT32 bus_clk[] = {0, 24, 48, 80, 200},
           bus_width[] = {1, 4, 8},
           data_mode[] = {PIO_DATA_MODE, BASIC_DMA_DATA_MODE, 
                          DESC_DMA_DATA_MODE, ENHANCED_DMA_DATA_MODE},
           test_mode[] = {MSDC_DATA_TEST_MODE1,
                          MSDC_DATA_TEST_MODE2,
                          MSDC_DATA_TEST_MODE3},
           random_seed[] = {SEED2, SEED4, SEED1};
    UINT32 busclk_idx = 0, busclk_idx_min = 0, busclk_idx_max = 2,
           buswidth_idx = 0, buswidth_idx_min = 0, buswidth_idx_max = 2, 
           datamode_idx = 0, datamode_idx_min = 0, datamode_idx_max = 2,
           testmode_idx = 0, testmode_idx_min = 0, testmode_idx_max = 2;
    INT32 i4Ret = MSDC_SUCCESS; 

    testmode_idx_max = 0;
    buswidth_idx_min = 0;
    datamode_idx_min = 0;
    testmode_idx_min = 0;
    //busclk_idx_max = sizeof(bus_clk)/sizeof(bus_clk[0]);
    //busclk_idx_max = 2;
    //datamode_idx_max = sizeof(data_mode)/sizeof(data_mode[0]);
    datamode_idx_max = 2;
    buswidth_idx_max = sizeof(bus_width)/sizeof(bus_width[0])-1;
    testmode_idx_max = sizeof(test_mode)/sizeof(test_mode[0])-1;

    if(sdCard[ch].cardType == CARDTYPE_MEM_SD)
    {
        buswidth_idx_max = 1;
    }

    MSDC_LOG(MSG_LVL_ERR, "----------data auto test----------\n");
    for(datamode_idx=datamode_idx_min;datamode_idx<=datamode_idx_max;datamode_idx++)
    {
        i4Ret = MsdcSetDataMode(data_mode[datamode_idx]);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "data mode is switch to %d failed!\n", data_mode[datamode_idx]);
            goto end;    
        }
            
        for(buswidth_idx=buswidth_idx_min;buswidth_idx<=buswidth_idx_max;buswidth_idx++)
        {
            i4Ret = MsdcSetBusWidth(bus_width[buswidth_idx]);
            if(MSDC_SUCCESS != i4Ret)
            {
                MSDC_LOG(MSG_LVL_ERR, "bus width is switch to %d failed!\n", bus_width[buswidth_idx]);
                goto end;    
            }
                
            for(busclk_idx=busclk_idx_min;busclk_idx<=busclk_idx_max;busclk_idx++)
            { 
                i4Ret = MsdcSetBusClock(bus_clk[busclk_idx]);
                if(MSDC_SUCCESS != i4Ret)
                {
                    MSDC_LOG(MSG_LVL_ERR, "buw clock is switch to %d failed!\n", bus_clk[busclk_idx]);
                    goto end;    
                }

                /* For some clolck frequency, it need some special speed mode
                         * SD card:
                         *   DS - 0~25MHz with 3.3V in SDR mode
                         *   HS - 0~50MHz with 3.3V in SDR mode
                         *   SDR12 - 0~25MHz with 1.8V in SDR mode
                         *   SDR25 - 0~50MHz with 1.8V in SDR mode
                         *   SDR50 - 0~100MHz with 1.8V in SDR mode
                         *   SDR104 - 0~208MHz with 1.8V in SDR mode
                         *   DDR50 - 0~50MHz with 1.8V in DDR mode
                         * eMMC:
                         *   DS - 0~26MHz with 3.3V/1.8V in SDR mode
                         *   HS - 0~52MHz with 3.3V/1.8V in SDR mode
                         *   HS-DDR - 0~52MHz with 1.8V in DDR mode
                         *   HS200 - 0~200MHz with 1.8V in SDR mode
                         */
                if(bus_clk[busclk_idx] > 24)
                {
                    i4Ret = MsdcSetSpeedMode(1);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "switch to high speed mode failed!\n");
                        goto end;    
                    }     
                }
                else
                {
                    i4Ret = MsdcSetSpeedMode(0);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "switch to default speed mode failed!\n");
                        goto end;    
                    }  
                }

                for(testmode_idx=testmode_idx_min;testmode_idx<=testmode_idx_max;testmode_idx++)
                {
                    MSDC_LOG(MSG_LVL_ERR, "\n");
                    MSDC_LOG(MSG_LVL_ERR, "Data Mode:%d\n", data_mode[datamode_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "Bus Width:%d\n", bus_width[buswidth_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "Bus Clock:%d\n", bus_clk[busclk_idx]);
                    
                    MSDCTet_DataRandomGen(pSrcWrite, MSDC_DATA_TOTAL_SIZE, random_seed[testmode_idx]);
                    
                    i4Ret = MSDCTest_DataTransmission(pSrcWrite, pDestRead, test_mode[testmode_idx]);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "data auto test failed(test mode:%d, data mode:%d, bus width:%d, bus clk:%d)\n",
                                                                           test_mode[testmode_idx], data_mode[datamode_idx],
                                                                           bus_width[buswidth_idx], bus_clk[busclk_idx]);
                        goto end;
                    }
                }
            }
        }
    }

end:
    return i4Ret;
}

INT32 MSDCTest_DataEndAutoTest(UINT32 *pSrcWrite,  UINT32 *pDestRead)
{
    UINT32 data_end[] = {0x0, 0x1, 0x2, 0x3};
    UINT32 dataend_idx = 0, dataend_idx_min = 0, dataend_idx_max = 3;
    INT32 i4Ret = MSDC_SUCCESS; 

    MSDC_LOG(MSG_LVL_ERR, "----------data end auto test----------\n");
    for(dataend_idx=dataend_idx_min;dataend_idx<=dataend_idx_max;dataend_idx++)
    {
        if((sdCard[ch].cardType == CARDTYPE_MEM_SD) &&
           ((dataend_idx==0x1) || (dataend_idx==0x3)))
        {
            MSDC_LOG(MSG_LVL_ERR, "this card doesn't support this data end mode(%d)!\n", data_end[dataend_idx]);
            continue;
        }
        
        i4Ret = MsdcSetDataEndMode(data_end[dataend_idx]);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "data end mode is switch to %d failed!\n", data_end[dataend_idx]);
            goto end;    
        }  

        MSDC_LOG(MSG_LVL_ERR, "Data End Mode:%d\n", data_end[dataend_idx]);
        i4Ret = MSDCTest_DataAutoTest(pSrcWrite,  pDestRead);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "data auto test with data end mode change failed(%d)!\n", data_end[dataend_idx]);
            goto end;    
        } 
    }

end:
    return i4Ret;
}

#define BOOT_CONFIG_RESETMODE_SHIFT           (0)
#define BOOT_CONFIG_RESETMODE_MASK      (0x03<<0)
#define BOOT_CONFIG_RESET_POWER_ON      (0x01<<0)
#define BOOT_CONFIG_RESET_SOFTWARE      (0x02<<0)
#define BOOT_CONFIG_RESET_HARDWARE      (0x03<<0)

#define BOOT_CONFIG_BOOTMODE_SHIFT            (2)
#define BOOT_CONFIG_BOOTMODE_MASK       (0x01<<2)
#define BOOT_CONFIG_BOOTMODE_NORM       (0x00<<2)
#define BOOT_CONFIG_BOOTMODE_ALTE       (0x01<<2)

#define BOOT_CONFIG_BUSWIDTH_SHIFT            (3)
#define BOOT_CONFIG_BUSWIDTH_MASK       (0x03<<3)
#define BOOT_CONFIG_BUSWIDTH_X1         (0x00<<3)
#define BOOT_CONFIG_BUSWIDTH_X4         (0x01<<3)
#define BOOT_CONFIG_BUSWIDTH_X8         (0x02<<3)

#define BOOT_CONFIG_SPEEDMODE_SHIFT           (5)
#define BOOT_CONFIG_SPEEDMODE_MASK      (0x03<<5)
#define BOOT_CONFIG_SPEEDMODE_DS        (0x00<<5)
#define BOOT_CONFIG_SPEEDMODE_HS        (0x01<<5)
#define BOOT_CONFIG_SPEEDMODE_DDR       (0x02<<5)

#define BOOT_CONFIG_ACK_SHIFT                 (7)
#define BOOT_CONFIG_ACK_MASK            (0x01<<7)
#define BOOT_CONFIG_ACK_WITHOUT         (0x00<<7)
#define BOOT_CONFIG_ACK_WITH            (0x01<<7)

#define BOOT_CONFIG_DATAMODE_SHIFT            (8)
#define BOOT_CONFIG_DATAMODE_MASK       (0x03<<8)
#define BOOT_CONFIG_DATAMODE_PIO        (0x01<<8)
#define BOOT_CONFIG_DATAMODE_BASIC      (0x02<<8)
#define BOOT_CONFIG_DATAMODE_DESC       (0x03<<8)

#define BOOT_CONFIG_DMA_BURSTSZ_SHIFT         (10)
#define BOOT_CONFIG_DMA_BURSTSZ_MASK    (0x07<<10)
#define BOOT_CONFIG_DMA_BURSTSZ_8B      (0x03<<10)
#define BOOT_CONFIG_DMA_BURSTSZ_16B     (0x04<<10)
#define BOOT_CONFIG_DMA_BURSTSZ_32B     (0x05<<10)
#define BOOT_CONFIG_DMA_BURSTSZ_64B     (0x06<<10)

#define BOOT_CONFIG_BOOTSZ_AWARE_SHIFT        (13)
#define BOOT_CONFIG_BOOTSZ_AWARE_MASK   (0x01<<13)
#define BOOT_CONFIG_BOOTSZ_AWARE_EN     (0x00<<13)
#define BOOT_CONFIG_BOOTSZ_AWARE_DIS    (0x01<<13)

#define BOOT_CONFIG_BOOTAREA_SHIFT            (14)
#define BOOT_CONFIG_BOOTAREA_MASK       (0x07<<14) 
#define BOOT_CONFIG_BOOTAREA_NO         (0x00<<14) 
#define BOOT_CONFIG_BOOTAREA_BOOT1      (0x01<<14) 
#define BOOT_CONFIG_BOOTAREA_BOOT2      (0x02<<14) 
#define BOOT_CONFIG_BOOTAREA_USER       (0x07<<14) 

INT32 MSDCTest_BootSetting(UINT32 config, UINT32 test_mode)
{
    UINT8 partition_config = 0, boot_bus_width = 0;  
    INT32 i4Ret = MSDC_SUCCESS;

    /* step 1 - boot area setting
      */
    //1.1 - partition configuration setting
    partition_config = (((config&BOOT_CONFIG_BOOTAREA_MASK)>>BOOT_CONFIG_BOOTAREA_SHIFT)<<3) |
                       (((config&BOOT_CONFIG_ACK_MASK)>>BOOT_CONFIG_ACK_SHIFT)<<6);
    if(((config&BOOT_CONFIG_BOOTAREA_MASK) == BOOT_CONFIG_BOOTAREA_BOOT1) ||
       ((config&BOOT_CONFIG_BOOTAREA_MASK) == BOOT_CONFIG_BOOTAREA_BOOT2))
    {
        partition_config |= (((config&BOOT_CONFIG_BOOTAREA_MASK)>>BOOT_CONFIG_BOOTAREA_SHIFT)<<0);
    }
    else if((config&BOOT_CONFIG_BOOTAREA_MASK) == BOOT_CONFIG_BOOTAREA_USER)
    {
        partition_config |= 0x0;
    }
    else if((config&BOOT_CONFIG_BOOTAREA_MASK) == BOOT_CONFIG_BOOTAREA_NO)
    {
        Printf("Disable boot mode!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }
    else
    {
        Printf("Invalid boot area setting(%02X)!\n", (((config&BOOT_CONFIG_BOOTAREA_MASK)>>BOOT_CONFIG_BOOTAREA_SHIFT)<<0));
        i4Ret = MSDC_FAILED;
        goto end;
    }

    //1.2 - boot bus width setting
    boot_bus_width = (((config&BOOT_CONFIG_BUSWIDTH_MASK)>>BOOT_CONFIG_BUSWIDTH_SHIFT)<<0) |
                     (((config&BOOT_CONFIG_SPEEDMODE_MASK)>>BOOT_CONFIG_SPEEDMODE_SHIFT)<<3);

    // 1.2.x - special mask for special test case
    if(test_mode == 0x01)
    {
        partition_config &= (~((0x1<<6) | (0x7<<3)));  
        Printf("card's partition configuration:%02x\n", partition_config);
    }
    else if(test_mode == 0x2)
    {
        partition_config &= (~(0x1<<6));  
        Printf("card's partition configuration:%02x\n", partition_config);
    }
    else if(test_mode == 0x3)
    {
        partition_config &= (~(0x7<<3)); 
        Printf("card's partition configuration:%02x\n", partition_config);
    }
    else if(test_mode == 0x4)
    {
        partition_config |= (0x1<<6); 
        Printf("card's partition configuration:%02x\n", partition_config);
    }
    else
    {
        // nothing to do
    }

    //1.3 - programming PARTITION_CONFIG in emmc device
    if(MSDC_SUCCESS != MsdcSetEXTCSD(179, partition_config))
    {
        Printf("programming ext_csd[%d] by %02x failed!\n", 179, partition_config);
        i4Ret = MSDC_FAILED;
        goto end;
    }

    //1.4 - programming BOOT_BUS_WIDTH in emmc device
    if(MSDC_SUCCESS != MsdcSetEXTCSD(177, boot_bus_width))
    {
        Printf("programming ext_csd[%d] by %02x failed!\n", 177, boot_bus_width);
        i4Ret = MSDC_FAILED;
        goto end;
    }


end:
    return i4Ret;    
}


INT32 MSDCTest_BootGoPreIdleState(UINT32 reset_mode)
{
    INT32 i4Ret = MSDC_SUCCESS;

    // power off-on
    if(reset_mode == BOOT_CONFIG_RESET_POWER_ON)
    {
        //TO-DO
#if 0
        if((MSDC_READ32(MSDC_PS)>>16)&0x1FFF == 0x1FFF)
        {
            MSDC_LOG(MSG_LVL_ERR, "please power off emmc device!\n");
            while(1)
            {
                if((MSDC_READ32(MSDC_PS)>>16)&0x1FFF == 0x0)
                {
                    break;
                }
            }
        }

        MSDC_LOG(MSG_LVL_ERR, "please power on emmc device!\n");
        while(1)
        {
            if((MSDC_READ32(MSDC_PS)>>16)&0x1FFF == 0x1FFF)
            {
                break;
            }
        }
#endif
    }
    else if(reset_mode == BOOT_CONFIG_RESET_SOFTWARE)
    {
        i4Ret = cmd0_go_idle_state(0xF0F0F0F0);
        if(MSDC_SUCCESS != i4Ret)
        {
            MSDC_LOG(MSG_LVL_ERR, "send cmd0+0xF0F0F0F0 to let card go into preidle state failed!\n");   
            i4Ret = MSDC_FAILED;
            goto end;
        }        
    }
    else if(reset_mode == BOOT_CONFIG_RESET_HARDWARE)
    {
        // TO-DO
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "unknown reset mode!\n");  
        i4Ret = MSDC_FAILED;
        goto end;
    }

end:
    return i4Ret;  
}

INT32 MSDCTest_BootClr(VOID)
{
    INT32 i4Ret = MSDC_SUCCESS;
        
    MSDC_CLRBIT(EMMC_CFG0, BTSUP);
    
    return i4Ret;  
}

INT32 MSDCTest_BootStop(VOID)
{
    INT32 i4Ret = MSDC_SUCCESS;

    MSDC_WRITE32(SDC_ARG, 0x0);
    MSDC_WRITE32(SDC_CMD, 0x1000);
    MSDC_SETBIT(EMMC_CFG0, BTWDLY_7x32);
    MSDC_SETBIT(EMMC_CFG0, BTSTOP);
    MSDC_LOG(MSG_LVL_ERR, "polling emmc_sts.boot_up_state = 0!\n");
    while(MSDC_READ32(EMMC_STS)&BTSTS);
  
    return i4Ret; 
}

INT32 MSDCTest_BootReadData(UINT32 *pDestRead, UINT32 len)
{
    UINT32 *pcurAddr = pDestRead,
           pEndAddr = (UINT32)pDestRead + len, 
           rxCnt = 0,
           rxFifoCnt = 0,
           i = 0, emmc_sts = 0;   
    INT32 i4Ret = MSDC_SUCCESS;

    // boot data max wait-time as 1s
    for(i=0;i<1000;i++)
    {
        emmc_sts = MSDC_READ32(EMMC_STS);
        Printf("emmc_sts: %08x---\n", emmc_sts);
        if(emmc_sts&BTDRCV)
        {
            if(emmc_sts&BTDERR)
            {
                MSDC_LOG(MSG_LVL_ERR, "boot data crc error(%08x)!\n", emmc_sts);
                i4Ret = MSDC_FAILED;
                goto end;
            }
        }
        else if(emmc_sts&BTDTO)
        {
            MSDC_LOG(MSG_LVL_ERR, "boot data timeout(%08x)!\n", emmc_sts);
            i4Ret = MSDC_FAILED;
            goto end;        
        }
        else
        {
            continue;
        }
            
        HAL_Delay_us(1000);          
    }

    MSDC_LOG(MSG_LVL_ERR, "boot data received success(%08x)!\n", emmc_sts);

    MSDC_LOG(MSG_LVL_ERR, "start to receive boot data!\n");
    while((UINT32)pcurAddr < pEndAddr)     
    {        
      // wait until fifo has enough data        
      rxFifoCnt = (MSDC_READ32(MSDC_FIFOCS) & MSDC_FIFOCS_RXFIFOCNT_MASK);        
      while ((pEndAddr > (UINT32)pcurAddr) && (sizeof(int) <= rxFifoCnt))        
      {            
          // Read Data            
          *pcurAddr = MSDC_READ32(MSDC_RXDATA);   
          pcurAddr++;            
          rxCnt += sizeof(int);            
          rxFifoCnt -= sizeof(int); 
          
          if (rxCnt == 128)   // Check CRC error happens or not every 512 Byte            
          {                
              if (MSDC_READ32(EMMC_STS) & (BTDTO | BTDERR))                
              {                    
                  MSDC_LOG(MSG_LVL_ERR, "crc error when boot code transfer!\n");              
                  i4Ret =  MSDC_FAILED; 
                  goto end;
              }                

              // Clear the counter                
             rxCnt = 0;            
          }                  
      }     
  }
  

end:
    return i4Ret;  
}

INT32 MSDCTest_BootAckHandle(UINT32 boot_ack)
{
    UINT32 emmc_sts = 0, i = 0;
    INT32 i4Ret = MSDC_SUCCESS;

    while(!(MSDC_READ32(EMMC_STS)&BTSTS));

    if(boot_ack == BOOT_CONFIG_ACK_WITHOUT)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot ack is disabled, no need to handle!\n");
        goto end;
    }
    else if(boot_ack == BOOT_CONFIG_ACK_WITH)
    {
        // boot ack max wait-time as 50ms
        for(i=0;i<60;i++)
        {
            emmc_sts = MSDC_READ32(EMMC_STS);
            Printf("emmc_sts: %08x+++\n", emmc_sts);
            if(emmc_sts&BTARCV)
            {
                if(emmc_sts&BTAERR)
                {
                    MSDC_LOG(MSG_LVL_ERR, "boot ack crc error(%08x)!\n", emmc_sts);
                    i4Ret = MSDC_FAILED;
                    goto end; 
                }   
            }
            else if(emmc_sts&BTATO)
            {
                MSDC_LOG(MSG_LVL_ERR, "boot ack timeout(%08x)!\n", emmc_sts);
                i4Ret = MSDC_FAILED;
                goto end;        
            }
            else
            {
                continue;
            }
            
            HAL_Delay_us(1000);  
        }

        MSDC_LOG(MSG_LVL_ERR, "boot ack received success(%08x)!\n", emmc_sts);
        i4Ret = MSDC_SUCCESS;
        goto end; 
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "unknown boot ack setting!\n");
        i4Ret = MSDC_FAILED;
        goto end;    
    }
    

end:
    return i4Ret;  
}


INT32 MSDCTest_BootGoBootState(UINT32 boot_ack, UINT32 boot_mode, UINT32 clk, UINT32 len)
{
    UINT32 emmc_cfg_tmp = 0;
    INT32 i4Ret = MSDC_SUCCESS;

    while(MSDC_READ32(SDC_STS)&SDC_STS_SDCBUSY);

    MSDC_SETBIT(EMMC_CFG0, BTSUP);
    if(boot_mode == BOOT_CONFIG_BOOTMODE_NORM)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot mode 0!\n"); 
        MSDC_CLRBIT(EMMC_CFG0, 0x1<<2);    
    }
    else if(boot_mode == BOOT_CONFIG_BOOTMODE_ALTE)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot mode 1!\n"); 
        MSDC_SETBIT(EMMC_CFG0, 0x1<<2); 
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "unknown boot mode!\n");    
        i4Ret = MSDC_FAILED;
        goto end;
    }
    MSDC_WRITE32(SDC_BLK_NUM, len>>9);

    if(boot_ack == BOOT_CONFIG_ACK_WITHOUT)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot ack disable!\n"); 
        MSDC_SETBIT(EMMC_CFG0, 0x1<<3);  
    }
    else if(boot_ack == BOOT_CONFIG_ACK_WITH)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot ack enable!\n"); 
        MSDC_CLRBIT(EMMC_CFG0, 0x1<<3); 
    }
    else
    {
        MSDC_LOG(MSG_LVL_ERR, "unknown boot ack mode!\n");    
        i4Ret = MSDC_FAILED;
        goto end;
    }

    if(clk == 0)
        clk = 352000;
    else if(clk == 13)
        clk = 13500000;
    else
        clk *= 1000000;    
    emmc_cfg_tmp = (((clk>>16)/20+1)<<20) |
                   (((clk>>16)+1)<<0);
    MSDC_WRITE32(EMMC_CFG1, emmc_cfg_tmp);
    MSDC_LOG(MSG_LVL_ERR, "emmc cfg1: %08x\n", emmc_cfg_tmp);

    if(boot_mode == BOOT_CONFIG_BOOTMODE_ALTE)
    {
        MSDC_WRITE32(SDC_ARG, 0xFFFFFFFA); 
    }
    MSDC_WRITE32(SDC_CMD, 0x02001000);
    
    MSDC_SETBIT(EMMC_CFG0, BTSTART);
    

end:
    return i4Ret;  
}


INT32 MSDCTest_BootStart(UINT32 config, UINT32 clk, UINT32 len)
{
    INT32 i4Ret = MSDC_SUCCESS;

    // step 1 - force card enters "Pre-idle" state
    i4Ret = MSDCTest_BootGoPreIdleState(config&BOOT_CONFIG_RESETMODE_MASK);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "go into pre-idle state failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step 2 - force card enters "boot-mode" state
    i4Ret = MSDCTest_BootGoBootState(config&BOOT_CONFIG_ACK_MASK, config&BOOT_CONFIG_BOOTMODE_MASK, clk, len);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "go into boot-idle state failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }
    
end:
    return i4Ret;      
}

INT32 MSDCTest_BootMode(UINT32 config, UINT32 clk, UINT32 *pDestRead, UINT32 len, UINT32 test_mode)
{
    INT32 i4Ret = MSDC_SUCCESS;

    // Step 0 - boot configuration
    i4Ret = MSDCTest_BootSetting(config, test_mode);
    if(MSDC_SUCCESS != i4Ret)
    {
        Printf("boot setting failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step 1/2 - boot start
    i4Ret = MSDCTest_BootStart(config, clk, len);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot ack handle failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step 3 - boot ack handle
    i4Ret = MSDCTest_BootAckHandle(config&BOOT_CONFIG_ACK_MASK);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot ack handle failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step4 - boot data transfer
    i4Ret = MSDCTest_BootReadData(pDestRead, len);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot data transfer failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step5 - boot stop
    i4Ret = MSDCTest_BootStop();
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot stop operation failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // step6 - boot clear
    i4Ret = MSDCTest_BootClr();
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot clear operation failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

end:
    return i4Ret;    
}

/* test_mdoe:
* 0x0 - normal case
* 0x1 - boot ack timeout
* 0x2 - boot ack crc error
* 0x3 - boot data timeout
* 0x4 - boot data crc error
*/
INT32 MSDCTest_BootInt(UINT32 *pSrcWrite, UINT32 *pDestRead, UINT32 test_mode)
{
    INT32 i4Ret = MSDC_SUCCESS;
    UINT32 config = BOOT_CONFIG_RESET_SOFTWARE | 
                    BOOT_CONFIG_BOOTMODE_ALTE |
                    BOOT_CONFIG_BUSWIDTH_X1 |
                    BOOT_CONFIG_SPEEDMODE_DS |
                    BOOT_CONFIG_ACK_WITH |
                    BOOT_CONFIG_DATAMODE_PIO |
                    BOOT_CONFIG_DMA_BURSTSZ_64B |
                    BOOT_CONFIG_BOOTSZ_AWARE_EN |
                    BOOT_CONFIG_BOOTAREA_USER; 
    UINT32 clk = 13,
           len = 0x8000,
           seed = SEED2,
           width = 0x1;

    if(test_mode == 0x3)
    {
        config &= (~BOOT_CONFIG_ACK_MASK);
        config |= BOOT_CONFIG_ACK_WITHOUT;
    }
    else if(test_mode == 0x4)
    {
        config &= (~BOOT_CONFIG_ACK_MASK);
        config |= BOOT_CONFIG_ACK_WITHOUT;
    }
    else
    {
        // nothing to do
    }
    Printf("host's partition configuration:%02x\n", config);

    
    // write new test data       
    if(test_mode != 0x2)
    {
        MSDCTet_DataRandomGen(pSrcWrite, len, seed);
    }
    else
    {
        x_memset(pSrcWrite, 0x00, len);
    }
    i4Ret = MsdcWriteCard((UINT32 *)pSrcWrite, (UINT64)0x0, (UINT32)len);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "write data into emmc failed!\n");
        i4Ret = MSDC_FAILED;
        goto end;
    }

    // set bus width
    i4Ret = MsdcSetBusWidth(width);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "bus width is switch to %d failed!\n", width);
        goto end;    
    }

    // set bus clock
    i4Ret = MsdcSetBusClock(clk);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "bus clock is switch to %d failed!\n", clk);
        goto end;    
    }

    // boot mode enable
    i4Ret = MSDCTest_BootMode(config, clk, pDestRead, len, test_mode);
    if(MSDC_SUCCESS != i4Ret)
    {
        MSDC_LOG(MSG_LVL_ERR, "boot mode run failed!\n");
        goto end;    
    }

    // compare the boot data
    i4Ret = MSDCTest_DataCompare(pSrcWrite, pDestRead, len); 
    if(MSDC_SUCCESS != i4Ret)
    {    
        MSDC_LOG(MSG_LVL_ERR, "compare data failed!\n");
        goto end;
    }   
    
end:
    return i4Ret;  
}


INT32 MSDCTest_BootModeAutoTest(UINT32 *pSrcWrite, UINT32 *pDestRead)
{
    INT32 i4Ret = MSDC_SUCCESS;
    UINT32 bus_clk[] = {0, 13, 24},
           bus_width[] = {0x0, 0x1, 0x2},
           data_len[] = {0x8000, 0xA000, 0x10000},
           boot_mode[] = {0x0, 0x1},
           random_seed[] = {SEED2, SEED4, SEED1};
    UINT32 busclk_idx = 0, busclk_idx_min = 0, busclk_idx_max = 2,
           buswidth_idx = 0, buswidth_idx_min = 0, buswidth_idx_max = 2,
           datalen_idx = 0, datalen_idx_min = 0, datalen_idx_max = 2,
           bootmode_idx = 0, bootmode_idx_min = 0, bootmode_idx_max = 1,
           config = BOOT_CONFIG_RESET_SOFTWARE | 
                    BOOT_CONFIG_BOOTMODE_NORM |
                    BOOT_CONFIG_BUSWIDTH_X1 |
                    BOOT_CONFIG_SPEEDMODE_DS |
                    BOOT_CONFIG_ACK_WITH |
                    BOOT_CONFIG_DATAMODE_PIO |
                    BOOT_CONFIG_DMA_BURSTSZ_64B |
                    BOOT_CONFIG_BOOTSZ_AWARE_EN |
                    BOOT_CONFIG_BOOTAREA_USER; 

    for_loop_inc(datalen_idx, datalen_idx_min, datalen_idx_max)
    {
        for_loop_inc(busclk_idx, busclk_idx_min, busclk_idx_max)
        {
            for_loop_inc(buswidth_idx, buswidth_idx_min, buswidth_idx_max)
            {               
                for_loop_inc(bootmode_idx, bootmode_idx_min, bootmode_idx_max)
                {
                    config &= (~BOOT_CONFIG_BOOTMODE_MASK);
                    config |= (boot_mode[bootmode_idx]<<BOOT_CONFIG_BOOTMODE_SHIFT);

                    MSDC_LOG(MSG_LVL_ERR, "\ndata len:%08x!\n", data_len[datalen_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "bus clk:%d!\n", bus_clk[busclk_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "bus width:%d!\n", bus_width[buswidth_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "boot mode:%d!\n", boot_mode[bootmode_idx]);
                    MSDC_LOG(MSG_LVL_ERR, "boot config:%08x!\n", config);

                    // re-initilization for write new test data
                    MSDC_PinMux(1);
                    i4Ret = MsdcSysInit();
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "sys init failed!\n", bus_clk[busclk_idx]);
                        goto end;    
                    }
                    i4Ret = MsdcIdentyCard(TRUE);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "identy card failed!\n", bus_clk[busclk_idx]);
                        goto end;    
                    }
                    i4Ret = MsdcSetBusClock(bus_clk[busclk_idx]);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "bus clock is switch to %d failed!\n", bus_clk[busclk_idx]);
                        goto end;    
                    }

                    // bus width re-set
                    config &= (~BOOT_CONFIG_BUSWIDTH_MASK);
                    config |= (bus_width[buswidth_idx]<<BOOT_CONFIG_BUSWIDTH_SHIFT);
                    if(bus_width[buswidth_idx]==0)
                    {
                        i4Ret = MsdcSetBusWidth(0x1);
                    }
                    else
                    {
                        i4Ret = MsdcSetBusWidth(0x1<<(bus_width[buswidth_idx]+1));
                    }
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "bus width is switch to %d failed!\n", bus_width[buswidth_idx]);
                        goto end;    
                    }

                    // write new test data       
                    MSDCTet_DataRandomGen( pSrcWrite, data_len[datalen_idx], random_seed[bootmode_idx]);
                    i4Ret = MsdcWriteCard((UINT32 *)pSrcWrite, (UINT64)0x0, (UINT32)data_len[datalen_idx]);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "write data into emmc failed!\n");
                        i4Ret = MSDC_FAILED;
                        goto end;
                    }
                    
                    i4Ret = MSDCTest_BootMode(config, bus_clk[busclk_idx], pDestRead, data_len[datalen_idx], 0x0);
                    if(MSDC_SUCCESS != i4Ret)
                    {
                        MSDC_LOG(MSG_LVL_ERR, "boot mode run failed!\n");
                        goto end;    
                    }

                    i4Ret = MSDCTest_DataCompare(pSrcWrite, pDestRead, data_len[datalen_idx]); 
                    if(MSDC_SUCCESS != i4Ret)
                    {    
                        MSDC_LOG(MSG_LVL_ERR, "compare data failed!\n");
                        goto end;
                    }
                }
            }
        }
    }


end:
    return i4Ret; 
}

INT32 MSDCTest_AutoTuning(UINT32 target)
{
    INT32 i4Ret = MSDC_SUCCESS;

	// data mode check
    if(sdHost[ch].dataMode != PIO_DATA_MODE)
    {
        MSDC_LOG(MSG_LVL_ERR, "auto tuning need to be in PIO mode!\n");
		i4Ret = MSDC_SUCCESS;
		goto end;
    }

    // bus width check
    if(((sdCard[ch].cardType == CARDTYPE_MEM_SD) &&(sdHost[ch].busWidth != 4)) ||
	   ((sdCard[ch].cardType == CARDTYPE_MEM_MMC) &&(sdHost[ch].busWidth != 8)))
	{
        MSDC_LOG(MSG_LVL_ERR, "auto tuning need to be in full bus width!\n");
		i4Ret = MSDC_SUCCESS;
		goto end;
    }

    // speed mode check
	if(((sdCard[ch].cardType == CARDTYPE_MEM_SD) && (!(sdHost[ch].speedMode & SPEED_MODE_SDR104))) ||
	   ((sdCard[ch].cardType == CARDTYPE_MEM_MMC) && (!(sdHost[ch].speedMode & SPEED_MODE_HS200))))
	{
        MSDC_LOG(MSG_LVL_ERR, "auto tuning need to be in SDR104 or HS200!\n");
		i4Ret = MSDC_SUCCESS;
		goto end;
    }

    // select tuning target
	MSDC_WRITE32(ACMD19_TRG, target);

	// send cmd19
	i4Ret = cmd19_sd_send_tuning_pattern();
	if(MSDC_SUCCESS != i4Ret)
	{
        MSDC_LOG(MSG_LVL_ERR, "auto-tuning failed!\n");
		goto end;
	}

	MSDC_LOG(MSG_LVL_ERR, "auto-tuning success!\n");
	
end:
    return i4Ret;  
}

#if 0
static const UINT8 tuning_data_pattern[128] = 
{
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC, 0xCC,
	0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF, 0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE, 0xFF,
	0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD, 0xDD, 0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF, 0xBB,
	0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF, 0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE, 0xFF,
	0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xCC, 0xCC, 0xCC, 0x33, 0xCC,
	0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0xCC, 0xFF, 0xFF, 0xFF, 0xEE, 0xFF, 0xFF, 0xFF, 0xEE, 0xEE,
	0xFF, 0xFF, 0xFF, 0xDD, 0xFF, 0xFF, 0xFF, 0xDD, 0xDD, 0xFF, 0xFF, 0xFF, 0xBB, 0xFF, 0xFF, 0xFF, 
	0xBB, 0xBB, 0xFF, 0xFF, 0xFF, 0x77, 0xFF, 0xFF, 0xFF, 0x77, 0x77, 0xFF, 0x77, 0xBB, 0xDD, 0xEE
};
#endif

static UINT8 tuning_result[8][64];
INT32 MSDCTest_HS200Tuning(UINT32 target)
{
    INT32 i4Ret = MSDC_SUCCESS;
	UINT32 data_len = 128;
	//UINT32 ddr = 0;
	UINT32 dcrc = 0;
    UINT32 rxdly, cur_rxdly, orig_rxdly;
    UINT32 dsmpl, cur_dsmpl,  orig_dsmpl;
	UINT32 i, j, k, result[2]={0}, retry = 100;

	if(sdHost[ch].speedMode != SPEED_MODE_HS200)
    {
        MSDC_LOG(MSG_LVL_ERR, "emmc doesn't stay at HS200 state!\n");
		goto end;
	}

	if(sdHost[ch].busWidth == 4)
	{
        data_len = 64;
	}
	else if(sdHost[ch].busWidth == 8)
    {
        data_len = 128;
    }
	else
	{
        MSDC_LOG(MSG_LVL_ERR, "cmd21 tuning process is only used at 4/8bit width!\n");
		goto end;
	}

    /* re-initilize teh tuning result */
	for(i=0;i<8;i++)
		for(j=0;j<64;j++)
			tuning_result[i][j] = 0;

    /* make sure the unified sample and delay value */
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_R_D_SMPL_SEL);
	MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_DLYLINE_SEL);

	/* get the initialized sample and delay value */
	orig_dsmpl = ((MSDC_READ32(MSDC_IOCON) & MSDC_IOCON_D_SMPL) >> MSDC_IOCON_D_SMPL_SHIFT);
	orig_rxdly = ((MSDC_READ32(PAD_TUNE) & PAD_DAT_RD_RXDLY) >> PAD_DAT_RD_RXDLY_SHIFT);
	rxdly = 0; 
	
    do 
    {		
        /* re-assign rxdlay value */
		cur_rxdly = (orig_rxdly + rxdly) % 32;
		MSDC_CLRBIT(MSDC_IOCON, PAD_DAT_RD_RXDLY);
        MSDC_SETBIT(PAD_TUNE, cur_rxdly << PAD_DAT_RD_RXDLY_SHIFT);

        for (dsmpl = 0; dsmpl < 2; dsmpl++) 
        {
            cur_dsmpl = (orig_dsmpl + dsmpl) % 2;

            MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
            MSDC_SETBIT(MSDC_IOCON, (cur_dsmpl << MSDC_IOCON_D_SMPL_SHIFT));

			for(k=0;k<retry;k++)
			{
			    i4Ret = cmd21_mmc_send_tuning_block((VOID *)0x1000000, data_len);	
				
				dcrc = (MSDC_READ32(SDC_DATCRC_STS) & (~SDC_DATCRC_STS_NEG));

				MSDC_LOG(MSG_LVL_ERR, "dsmpl<%d>, rxdly<%02x>==========>dcrc:%02x\n", cur_dsmpl, cur_rxdly, dcrc);

	            /* fill the result into array */
	            for(i=0;i<8;i++)
	            {  
				    if(((dcrc>>i) & 0x1) ||
					   (i4Ret != MSDC_SUCCESS))
					    tuning_result[i][32*cur_dsmpl+cur_rxdly] = 1;
	            }

				if(dcrc == 0xFF)
					break;
			}			
        }
    }while(++rxdly < 32);

    /* print the result from array */
	for(i=0;i<8;i++)
	{
	    result[0] = result[1] = 0;
		
	    for(j=0;j<64;j++)
	    {
	        if(tuning_result[i][j] == 1)
	        {
                result[j/32] |= (0x1<<(j%32));
	        }
	    }

		MSDC_LOG(MSG_LVL_ERR, "data%d: %08x %08x\n", i, result[0], result[1]);
	}

end:
    return MSDC_SUCCESS;  
}


#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
static UINT8 ca_tuning_read_result[2][32][32];
static UINT8 ca_tuning_write_result[2][8][32];

#define MEM_READ_ADDR    0x6400000
#define MEM_WRITE_ADDR   0x12C00000
#define EMMC_START_ADDR  0x40000000
#define DATA_LENGTH      0x1000000
#endif

INT32 MSDCTest_HS200Calibration(UINT32 u4Offset, UINT32 u4Size)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    INT32 i4Ret = MSDC_SUCCESS;
	UINT32 data_len = 128;
    
	UINT32 i, j, k;
    
	if((sdHost[ch].speedMode != SPEED_MODE_HS200)&&(sdHost[ch].speedMode != SPEED_MODE_SDR104))
    {
        Printf("device doesn't stay at HS200/SDR104 state!\n");
		goto end;
	}
	if(sdHost[ch].busWidth == 4)
	{
        data_len = 64;
	}
	else if(sdHost[ch].busWidth == 8)
    {
        data_len = 128;
    }
	else
	{
        Printf("calibration tuning process is only used at 4/8bit width!\n");
		goto end;
	}

    for(i=0;i<2;i++)
    {
	    for(j=0;j<32;j++)
        {   
		    for(k=0;k<32;k++)
            {      
	    		ca_tuning_read_result[i][j][k] = 0;
            }
        }
    }
    for(i=0;i<2;i++)
    {
        for(j=0;j<8;j++)
        {
            for(k=0;k<32;k++)
            {
                ca_tuning_write_result[i][j][k] = 0; 
            }
        }
    }
    memset((unsigned int *)MEM_WRITE_ADDR, 0x55aaaa55, u4Size);

    Printf("++++++++++ Start HS200 Calibration! ++++++++++\n");

    Printf("+++++++++++++++++ write part  ++++++++++++++++\n");
/*+++++++++++++++++++++++++++++write part++++++++++++++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
	MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
	MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);

    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_W_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_W_D_SMPL_SEL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_WR_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("WRDAT_CRCS_TA_CNTR |  |\n");

		for(j=0;j<8;j++)
		{
            MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);
            MSDC_SETBIT(PATCH_BIT1, (j<< WRDAT_CRCS_TA_CNTR_SHIFT));
            
            Printf("                   |%02x|",j);

	        for(k=0;k<32;k++)
	        {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_WR_RXDLY_SHIFT));
                i4Ret = MsdcWriteCard((UINT32 *)MEM_WRITE_ADDR, (UINT64)u4Offset, u4Size);  

                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_write_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_write_result[i][j][k]);

            }
            Printf("\n");

		}			
    }

    Printf("+++++++++++++++++ read part ++++++++++++++++\n");
/*++++++++++++++++++++++++++++++++read part+++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
    MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
    MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
    
    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_D_SMPL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_RD_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("CKGEN_MSDC_DLY_SEL |  |\n");
    
        for(j=0;j<32;j++)
        {
            MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
            MSDC_SETBIT(PATCH_BIT0, (j<< CKGEN_MSDC_DLY_SEL_SHIFT));
            
            Printf("                   |%02x|",j);
    
            for(k=0;k<32;k++)
            {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_RD_RXDLY_SHIFT));
                i4Ret = MsdcReadCard((UINT64)u4Offset,(UINT32 *)MEM_READ_ADDR,(UINT32)u4Size);  
    
                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_read_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_read_result[i][j][k]);
    
            }
            Printf("\n");
    
        }           
    }

end:
    return MSDC_SUCCESS;
#else
    Printf("This chip doesn't support HS200 mode!\n");
    return MSDC_SUCCESS;
#endif
}

INT32 MSDCTest_DDR50Calibration(UINT32 u4Offset, UINT32 u4Size)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    INT32 i4Ret = MSDC_SUCCESS;
	UINT32 data_len = 128;
    
	UINT32 i, j, k;
    
	if(sdHost[ch].speedMode != SPEED_MODE_DDR50)
    {
        Printf("emmc doesn't stay at DDR50 state!\n");
		goto end;
	}
	if(sdHost[ch].busWidth == 4)
	{
        data_len = 64;
	}
	else if(sdHost[ch].busWidth == 8)
    {
        data_len = 128;
    }
	else
	{
        Printf("calibration tuning process is only used at 4/8bit width!\n");
		goto end;
	}

    for(i=0;i<2;i++)
    {
	    for(j=0;j<32;j++)
        {   
		    for(k=0;k<32;k++)
            {      
	    		ca_tuning_read_result[i][j][k] = 0;
            }
        }
    }
    for(i=0;i<2;i++)
    {
        for(j=0;j<8;j++)
        {
            for(k=0;k<32;k++)
            {
                ca_tuning_write_result[i][j][k] = 0; 
            }
        }
    }
    memset((unsigned int *)MEM_WRITE_ADDR, 0x55aaaa55, u4Size);

    Printf("++++++++++ Start DDR50 Calibration! ++++++++++\n");

    Printf("+++++++++++++++++ write part  ++++++++++++++++\n");
/*+++++++++++++++++++++++++++++write part++++++++++++++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
	MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
	MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);

    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_W_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_W_D_SMPL_SEL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_WR_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("WRDAT_CRCS_TA_CNTR |  |\n");

		for(j=0;j<8;j++)
		{
            MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);
            MSDC_SETBIT(PATCH_BIT1, (j<< WRDAT_CRCS_TA_CNTR_SHIFT));
            
            Printf("                   |%02x|",j);

	        for(k=0;k<32;k++)
	        {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_WR_RXDLY_SHIFT));
                i4Ret = MsdcWriteCard((UINT32 *)MEM_WRITE_ADDR, (UINT64)u4Offset, u4Size);  

                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_write_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_write_result[i][j][k]);

            }
            Printf("\n");

		}			
    }

    Printf("+++++++++++++++++ read part ++++++++++++++++\n");
/*++++++++++++++++++++++++++++++++read part+++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
    MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
    MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
    
    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_D_SMPL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_RD_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("CKGEN_MSDC_DLY_SEL |  |\n");
    
        for(j=0;j<32;j++)
        {
            MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
            MSDC_SETBIT(PATCH_BIT0, (j<< CKGEN_MSDC_DLY_SEL_SHIFT));
            
            Printf("                   |%02x|",j);
    
            for(k=0;k<32;k++)
            {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_RD_RXDLY_SHIFT));
                i4Ret = MsdcReadCard((UINT64)u4Offset,(UINT32 *)MEM_READ_ADDR,(UINT32)u4Size);  
    
                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_read_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_read_result[i][j][k]);
    
            }
            Printf("\n");
    
        }           
    }

end:
    return MSDC_SUCCESS;
#else
    Printf("This chip doesn't support DDR50 mode!\n");
    return MSDC_SUCCESS;
#endif
}

INT32 MSDCTest_SDR50Calibration(UINT32 u4Offset, UINT32 u4Size)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    INT32 i4Ret = MSDC_SUCCESS;
	UINT32 data_len = 128;
    
	UINT32 i, j, k;
    
	if(sdHost[ch].speedMode != SPEED_MODE_SDR50)
    {
        Printf("emmc doesn't stay at SDR50 state!\n");
		goto end;
	}
	if(sdHost[ch].busWidth == 4)
	{
        data_len = 64;
	}
	else if(sdHost[ch].busWidth == 8)
    {
        data_len = 128;
    }
	else
	{
        Printf("calibration tuning process is only used at 4/8bit width!\n");
		goto end;
	}

    for(i=0;i<2;i++)
    {
	    for(j=0;j<32;j++)
        {   
		    for(k=0;k<32;k++)
            {      
	    		ca_tuning_read_result[i][j][k] = 0;
            }
        }
    }
    for(i=0;i<2;i++)
    {
        for(j=0;j<8;j++)
        {
            for(k=0;k<32;k++)
            {
                ca_tuning_write_result[i][j][k] = 0; 
            }
        }
    }
    memset((unsigned int *)MEM_WRITE_ADDR, 0x55aaaa55, u4Size);

    Printf("++++++++++ Start SDR50 Calibration! ++++++++++\n");

    Printf("+++++++++++++++++ write part  ++++++++++++++++\n");
/*+++++++++++++++++++++++++++++write part++++++++++++++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
	MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
	MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);

    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_W_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_W_D_SMPL_SEL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_WR_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("WRDAT_CRCS_TA_CNTR |  |\n");

		for(j=0;j<8;j++)
		{
            MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);
            MSDC_SETBIT(PATCH_BIT1, (j<< WRDAT_CRCS_TA_CNTR_SHIFT));
            
            Printf("                   |%02x|",j);

	        for(k=0;k<32;k++)
	        {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_WR_RXDLY_SHIFT));
                i4Ret = MsdcWriteCard((UINT32 *)MEM_WRITE_ADDR, (UINT64)u4Offset, u4Size);  

                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_write_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_write_result[i][j][k]);

            }
            Printf("\n");

		}			
    }

    Printf("+++++++++++++++++ read part ++++++++++++++++\n");
/*++++++++++++++++++++++++++++++++read part+++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
    MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
    MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
    
    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_D_SMPL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_RD_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("CKGEN_MSDC_DLY_SEL |  |\n");
    
        for(j=0;j<32;j++)
        {
            MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
            MSDC_SETBIT(PATCH_BIT0, (j<< CKGEN_MSDC_DLY_SEL_SHIFT));
            
            Printf("                   |%02x|",j);
    
            for(k=0;k<32;k++)
            {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_RD_RXDLY_SHIFT));
                i4Ret = MsdcReadCard((UINT64)u4Offset,(UINT32 *)MEM_READ_ADDR,(UINT32)u4Size);  
    
                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_read_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_read_result[i][j][k]);
    
            }
            Printf("\n");
    
        }           
    }

end:
    return MSDC_SUCCESS;
#else
    Printf("This chip doesn't support SDR50 mode!\n");
    return MSDC_SUCCESS;
#endif
}

static UINT8 ca_tuning_write_result_new[2][32][32][32];

INT32 MSDCTest_SDR50Calibration_new(UINT32 u4Offset, UINT32 u4Size)
{
#if defined(CC_MT5399) || defined(CONFIG_ARCH_MT5399)|| \
    defined(CC_MT5890) || defined(CONFIG_ARCH_MT5890)|| \
    defined(CC_MT5882) || defined(CONFIG_ARCH_MT5882)
    INT32 i4Ret = MSDC_SUCCESS;
	UINT32 data_len = 128;
    
	UINT32 h, i, j, k;
    
	if(sdHost[ch].speedMode != SPEED_MODE_SDR50)
    {
        Printf("emmc doesn't stay at SDR50 state!\n");
		goto end;
	}
	if(sdHost[ch].busWidth == 4)
	{
        data_len = 64;
	}
	else if(sdHost[ch].busWidth == 8)
    {
        data_len = 128;
    }
	else
	{
        Printf("calibration tuning process is only used at 4/8bit width!\n");
		goto end;
	}

    for(i=0;i<2;i++)
    {
	    for(j=0;j<32;j++)
        {   
		    for(k=0;k<32;k++)
            {      
	    		ca_tuning_read_result[i][j][k] = 0;
            }
        }
    }
    for(i=0;i<2;i++)
    {
     for(h=0;h<32;h++)
     {
        for(j=0;j<32;j++)
        {
            for(k=0;k<32;k++)
            {
                ca_tuning_write_result_new[i][h][j][k] = 0; 
            }
        }
     }
    }
    memset((unsigned int *)MEM_WRITE_ADDR, 0x55aaaa55, u4Size);

    Printf("++++++++++ Start SDR50 Calibration! ++++++++++\n");

    Printf("+++++++++++++++++ write part  ++++++++++++++++\n");
/*+++++++++++++++++++++++++++++write part++++++++++++++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
    MSDC_SETBIT(MSDC_IOCON, MSDC_IOCON_D_DLYLINE_SEL);
	MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
	MSDC_CLRBIT(PATCH_BIT1, WRDAT_CRCS_TA_CNTR);
	MSDC_CLRBIT(DAT_RD_DLY0, MSDC_DAT_RD_DLY0);
	
    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_W_D_SMPL_SEL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_W_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_W_D_SMPL_SEL is : %d ++++++++++++++\n", i);
     for(h=0;h<32;h++)
     {
     	MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
        MSDC_SETBIT(PATCH_BIT0, (h<< CKGEN_MSDC_DLY_SEL_SHIFT));
     	Printf("CKGEN_MSDC_DLY_SEL is : %d \n", h);
        Printf("DAT_RD_DLY0----------->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("PAD_DAT_WR_RXDLY   |  |\n");

		for(j=0;j<32;j++)
		{
            MSDC_CLRBIT(PAD_TUNE, PAD_DAT_WR_RXDLY);
            MSDC_SETBIT(PAD_TUNE, (j<< PAD_DAT_WR_RXDLY_SHIFT));
            Printf("                   |%02x|",j);

	        for(k=0;k<32;k++)
	        {  
                MSDC_CLRBIT(DAT_RD_DLY0, MSDC_DAT_RD_DLY0);
                MSDC_SETBIT(DAT_RD_DLY0, (k<< MSDC_DAT_RD_DLY0_SHIFT));
                i4Ret = MsdcWriteCard((UINT32 *)MEM_WRITE_ADDR, (UINT64)u4Offset, u4Size);  

                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_write_result_new[i][h][j][k] = 1;
                
                Printf("  %d ", ca_tuning_write_result_new[i][h][j][k]);

            }
            Printf("\n");

		}	
	}		
    }

    Printf("+++++++++++++++++ read part ++++++++++++++++\n");
/*++++++++++++++++++++++++++++++++read part+++++++++++++++++++++++++++++++++++*/     
    MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
    MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
    MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
    
    for (i = 0; i < 2; i++) 
    {
        MSDC_CLRBIT(MSDC_IOCON, MSDC_IOCON_D_SMPL);
        MSDC_SETBIT(MSDC_IOCON, (i << MSDC_IOCON_D_SMPL_SHIFT));
        Printf("++++++++++++++ MSDC_IOCON_D_SMPL is : %d ++++++++++++++\n", i);
        Printf("PAD_DAT_RD_RXDLY----->:"); 
        for (j = 0; j < 32; j++) 
            Printf(" %02x ", j);
        Printf("\n"); 
        Printf("CKGEN_MSDC_DLY_SEL |  |\n");
    
        for(j=0;j<32;j++)
        {
            MSDC_CLRBIT(PATCH_BIT0, CKGEN_MSDC_DLY_SEL);
            MSDC_SETBIT(PATCH_BIT0, (j<< CKGEN_MSDC_DLY_SEL_SHIFT));
            
            Printf("                   |%02x|",j);
    
            for(k=0;k<32;k++)
            {  
                MSDC_CLRBIT(PAD_TUNE, PAD_DAT_RD_RXDLY);
                MSDC_SETBIT(PAD_TUNE, (k<< PAD_DAT_RD_RXDLY_SHIFT));
                i4Ret = MsdcReadCard((UINT64)u4Offset,(UINT32 *)MEM_READ_ADDR,(UINT32)u4Size);  
    
                if (i4Ret != MSDC_SUCCESS)
                    ca_tuning_read_result[i][j][k] = 1;
                
                Printf("  %d ", ca_tuning_read_result[i][j][k]);
    
            }
            Printf("\n");
    
        }           
    }

end:
    return MSDC_SUCCESS;
#else
    Printf("This chip doesn't support SDR50 mode!\n");
    return MSDC_SUCCESS;
#endif
}
INT32 MSDCTest_Template(UINT32 config)
{
    INT32 i4Ret = MSDC_SUCCESS;

    goto end;	

end:
    return i4Ret;  
}

#endif
