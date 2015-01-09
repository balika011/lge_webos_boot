#include "x_printf.h"
#include "x_os.h"

#include "x_assert.h"
#include "x_timer.h"
#include "x_bim.h"
#include "x_lint.h"
#include "x_stl_lib.h"

#include "x_printf.h"
#include "x_rs_232.h"
#include "x_drv_cli.h"

//#include "UDVT.h"
//#include "UDVT_IF.h"
#include <linux/completion.h>


#define UDVT_LOG(level, fmt...)		UDVT_LOG_##level(fmt)
#define UDVT_LOG_0(fmt...)			//Printf(fmt)         //info 
#define UDVT_LOG_1(fmt...)			//Printf(fmt)         //error
#define UDVT_LOG_2(fmt...)         //Printf(fmt)         //warning
#define UDVT_LOG_3(fmt...)       //Printf(fmt)         //debug

#define UDVT_PACKET_MAX_DATA_LENGTH   250
#define UDVT_CHECKSUM_OK    1
#define UDVT_CHECKSUM_ERR   0

#define UDVT_GETDATA_TIMEOUT    200
#define UDVT_SENDDATA_TIMEOUT   200

#define UDVT_MAX_WORK_TESTCASE    10
#define UDVT_MAX_TESTCASE_MSG_NUM 10

#define UDVT_MAX_UDVT_INPUT_MSG_NUM    100
#define UDVT_MAX_UDVT_OUTPUT_MSG_NUM    100

#define UDVT_WRITEFILE_WRITABLE       1
#define UDVT_WRITEFILE_UNWRITABLE     0

typedef enum
{
  UDVT_CMD_MODULE_INIT_REQUEST = 0,
  UDVT_CMD_MODULE_INIT_RESPONSE,
  UDVT_CMD_VERSION_NUM_REQUEST,
  UDVT_CMD_VERSION_NUM_RESPONSE,
  UDVT_CMD_TEST_INIT_REQUEST,
  UDVT_CMD_TEST_INIT_RESPONSE,
  UDVT_CMD_TEST_START_REQUEST,
  UDVT_CMD_TEST_STATUS,
  UDVT_CMD_DATA_REQUEST,
  UDVT_CMD_DATA_RESPONSE,
  UDVT_CMD_TEST_DEINIT_REQUEST,
  UDVT_CMD_TEST_DEINIT_RESPONSE,
  UDVT_CMD_MODULE_DEINIT_REQUEST,
  UDVT_CMD_MODULE_DEINIT_RESPONSE,
  UDVT_CMD_TEST_SKIP_REQUEST,
  UDVT_CMD_TEST_SKIP_RESPONSE,
  UDVT_CMD_TEST_STOP_REQUEST,
  UDVT_CMD_TEST_STOP_RESPONSE,
  UDVT_CMD_TEST_OPENFILE,
  UDVT_CMD_TEST_OPENFILE_RESPONSE,
  UDVT_CMD_TEST_GET_RESTFILELENGTH,
  UDVT_CMD_TEST_GET_RESTFILELENGTH_RESPONSE,
  UDVT_CMD_TEST_READFILE,
  UDVT_CMD_TEST_GET_FILELENGTH,
  UDVT_CMD_TEST_GET_FILELENGTH_RESPONSE,
  UDVT_CMD_TEST_CLOSEFILE,
  UDVT_CMD_TEST_CLOSEFILE_RESPONSE,
  UDVT_CMD_TEST_SEEKFILE,
  UDVT_CMD_TEST_SEEKFILE_RESPONSE,
  UDVT_CMD_TEST_WAIT,
  UDVT_CMD_TEST_WAIT_RESPONSE,
  UDVT_CMD_TEST_WRITEFILE,
  UDVT_CMD_TEST_WRITEFILE_RESPONSE,
  UDVT_CMD_TEST_SAVELOG,
  UDVT_CMD_NUM
}UDVT_CMD_TYPE_e;

#define UDVT_GET_HANDLE_INPUT_QUEUE   1
#define UDVT_GET_HANDLE_OUTPUT_QUEUE  2

#define UDVT_GET_TESTCASE_PACKET_INPUT  1
#define UDVT_GET_TESTCASE_PACKET_OUTPUT 2

#define UDVT_CLEAR_STATUS_BIT         0xFEFF
#define UDVT_SET_STATUS_BIT           0x0100
#define UDVT_STATUS_BIT_MASK          0x0100
#define UDVT_CLEAR_RETRANSMIT_BIT     0xFDFF
#define UDVT_SET_RETRANSMIT_BIT       0x0200
#define UDVT_RETRANSMIT_BIT_MASK      0x0200
#define UDVT_MESSAGE_TYPE_MASK        0x00FF


#define UDVT_DATA_PACKET             0x00
#define UDVT_LARGE_DATA_PACKET       0x01
#define UDVT_STATUS_PACKET           0x02
#define UDVT_REQUEST_PACKET          0x03


#define UDVT_RESEND_REQUEST         0x04


typedef enum
{
  UDVT_ACK,
  UDVT_NACK
}UDVT_ACK_e;

typedef enum
{
  UDVT_FUN_TIMEOUT = 2,
  UDVT_FUN_OK = 1,
  UDVT_FUN_ERR = 0
}UDVT_FUN_RET_E;


#define UDVT_PACKAGE_SWITCH_RESEND    3

#define UDVT_FUN_TIMEOUT    2
#define UDVT_FUN_OK         1
#define UDVT_FUN_ERR        0

typedef struct _UDVT_TAG_PacketHeader
{
  UINT16 PacketHeader;
  UINT16 PacketLength;
  UINT32 lPacketChecksum;
}UDVT_PacketHeader_s;

#define UDVT_PACKET_HEADER_SIZE   8

typedef struct _UDVT_TAG_CompletePacket
{
  UDVT_PacketHeader_s sPacketHeader;
  UINT32 lSourceId;
  UINT32 lDestinationId;
  UINT32 lCommand;
  UINT32 lData[UDVT_PACKET_MAX_DATA_LENGTH];
}UDVT_CompletePacket_s;

#define UDVT_PACKET_SIZE  (UDVT_PACKET_HEADER_SIZE + 12 + UDVT_PACKET_MAX_DATA_LENGTH)

#define UDVT_PORT_SIZE  0x4000

typedef struct _UDVT_Configuration
{
  UINT32 PacketHeaderSize;
  void (*InitPortFun)(void);
  UINT32 (*GetDataFun)(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut);
  UINT32 (*SendDataFun)(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut);
  UINT32 (*GetDataDirect)(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut);
  UINT32 (*SendDataDirect)(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut);
  HANDLE_T  hInMsgQueue;
  HANDLE_T  hOutMsgQueue;
}UDVT_Configuration_s;

typedef struct _UDVT_TestCase_Env
{
  UINT32 case_id;
  UINT32 host_id;
  HANDLE_T msg_queue;
  HANDLE_T work_thread;
  HANDLE_T exit_sema;
  HANDLE_T wait_sema;

  struct completion exit_complete;
  UDVT_CompletePacket_s InPacket;
  UDVT_CompletePacket_s OutPacket;
  UINT32 u4TestProgress;
  struct task_struct *pTask;
  int pThread_id;
  UINT32 pcFileHandle;
  UINT32 pcFileLength;
  UINT32 pcFileResponse;
  UINT32 waitResponse;
  UINT32 fileWritable;
}UDVT_TestCase_Env_s;

#define UDVT_OPENFILE_MODE_DATASIZE   12
#define UDVT_OPENFILE_FILENAME_LEN    500

typedef struct _UDVT_OpenFile
{
  char fmode[UDVT_OPENFILE_MODE_DATASIZE];
  char fname[UDVT_OPENFILE_FILENAME_LEN];
}UDVT_OpenFile_s;

void UDVT_SendACK(void);


UINT32 UDVT_PacketSwitch(UDVT_CompletePacket_s *pPacket);
UDVT_FUN_RET_E UDVT_PostMessageToTestCase(UINT32 TestCaseIndex,UDVT_CompletePacket_s *pPacket);
UINT32 UDVT_FindWorkTestCaseByDestID(UINT32 caseID);
UINT32 UDVT_AddWorkTestCase(UINT32 caseID,UINT32 hostID);
UDVT_FUN_RET_E UDVT_IsCheckSumOk(UDVT_CompletePacket_s *pPacket);
UINT32 UDVT_FindWorkTestCaseByThreadHandle(HANDLE_T hThread);
void UDVT_TestCaseThread(void *pv_arg);
void UDVT_TaskFun(void *pv_arg);
void UDVT_ResetTestCaseEnv(UINT32 TestCaseIndex);

UDVT_FUN_RET_E UDVT_SendResponse(UINT32 TestCaseIndex,UINT32 lCmd);

extern int UDVT_Init(void);
extern UDVT_CompletePacket_s *UDVT_GetTestCasePacketByThreadHandle(HANDLE_T hThread,UINT32 packetType);
extern void UDVT_Config(UDVT_Configuration_s *pcfg);
extern void * UDVT_GetUDVTHandle(UINT32 type);
extern UDVT_FUN_RET_E UDVT_PostMessageToHost(UDVT_CompletePacket_s *pPacket);
extern UINT32 UDVT_CalculateCheckSum(UDVT_CompletePacket_s *pPacket);
extern UDVT_FUN_RET_E UDVT_ReportTestCaseProgress(HANDLE_T hThread,UINT32 u4Percent);
extern UINT16 UDVT_FrameTheHeader(UINT8 messageType, UDVT_ACK_e statusBit,BOOL retransmitBit);
extern UDVT_FUN_RET_E UDVT_WaitPCResponse(UINT32 TestCaseIndex,UINT32 timeout);
extern UINT32 UDVT_GetTestCaseOpenFileHandle(UINT32 caseID);
extern UINT32 UDVT_GetFileLength(UINT32 caseID);
extern UINT32 UDVT_ReadFileFromPort(UINT8 *pData,UINT32 DataSize);
extern UINT32 UDVT_WriteFiletoPort(UINT8 *pData,UINT32 DataSize);
extern UINT32 UDVT_GetFileResponse(UINT32 caseID);
extern UINT32 UDVT_GetWaitResponse(UINT32 caseID);
extern UINT32 UDVT_IsFileWriteable(UINT32 caseID);

#define FATS_WAITRESPONSE_ERR        0x00
#define FATS_WAITRESPONSE_YES        0x01
#define FATS_WAITRESPONSE_NO         0x02
#define FATS_WAITRESPONSE_TIMEOUT    0x03


typedef enum
{
	UDVT_TEST_PASS = 0,
	UDVT_TEST_FAIL,
}UDVT_TEST_RESULT_e;

extern void UDVT_IF_TestProgress(UINT32 u4Percent);
extern void UDVT_IF_SendResult(UDVT_TEST_RESULT_e TestResult);
extern UINT32 UDVT_IF_OpenFile(const char *filename,const char *mode);
extern UINT32 UDVT_IF_ReadFile(void *ptr,UINT32 size,UINT32 count,UINT32 stream);
extern UINT32 UDVT_IF_GetFileLength(UINT32 stream);
extern UINT32 UDVT_IF_CloseFile(UINT32 stream);
extern UINT32 UDVT_IF_SeekFile(UINT32 stream,INT32 offset,UINT32 origin);
extern UINT32 UDVT_IF_DebugCheckSum(void *ptr,UINT32 count);
extern UINT32 FATS_IF_WaitResponse(const char *pMsgStr,UINT32 timeout);
extern UINT32 UDVT_IF_WriteFile(void *ptr,UINT32 size,UINT32 count,UINT32 stream);
extern UINT32 FATS_IF_SaveLog(UINT32 logLvl,const char *pformat,...);

enum data_direction {
	BIDIRECTIONAL = 0,
	TO_DEVICE = 1,
	FROM_DEVICE = 2
};
#define x_alloc_aligned_dma_mem(size,align) VIRTUAL(BSP_AllocAlignedDmaMemory(size,align))
#define x_free_aligned_dma_mem(addr)  BSP_FreeAlignedDmaMemory(PHYSICAL(addr))

static HANDLE_T *phSendQueue;
static HANDLE_T *phGetQueue;

UDVT_Configuration_s udvt_if_cfg;


extern int UDVT_open(int portn);
extern INT32 DrvFSCloseFile(INT32 iFd);
extern int UDVT_Buffed_Write(int portn, const unsigned char *buf, int count);
extern int UDVT_Buffed_Read(int portn, unsigned char *buf, int count);
extern int UDVT_Direct_Read(int portn, unsigned char *buf, int count,int *completesem);
extern UINT32 UDVT_Direct_Write(int portn, unsigned char *buf, int count,int *completesem);
extern unsigned long UDVT_IsDirectReadComplete(int portn);
extern unsigned long UDVT_IsDirectWriteComplete(int portn);

//extern int k_uart_open(void *inode, void *flip);
//extern int k_uart_ioctl(void *inode, void *file, unsigned int cmd,unsigned long arg);
//extern UINT32 k_uart_write(void *file, const char *Buf, size_t size, void * loff);
//extern UINT32 k_uart_read(void *file, const char *Buf, size_t size, void * loff);


UINT32 GetDataFun(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut)
{
//  return k_uart_read(0,(char *)pDataBuff,DataLength,0);
  return UDVT_Buffed_Read(0,(char *)pDataBuff,DataLength);
  
}

UINT32 SendDataFun(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut)
{
//  return k_uart_write(0,(char *)pDataBuff,DataLength,0);
	UINT32 writeN;
	do
	{
  	writeN = UDVT_Buffed_Write(0,(char *)pDataBuff,DataLength);
  	if(writeN < DataLength)
  	{
  		x_thread_delay(10);
  		DataLength = DataLength - writeN;
  		writeN = 0;
  	}
  }while(writeN != DataLength);
	return 0;
}

UINT32 GetDataDirect(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut)
{
  UINT32 n;
  int complete;

  complete = 0;
  UDVT_Direct_Read(0,pDataBuff,DataLength,&complete);
  for(n=0;n<TimeOut;n++)
  {
    //if(complete == 1)
    if(UDVT_IsDirectReadComplete(0) == 1)
    {
      
      break;
    }
    else
    {
      x_thread_delay(10);
    }
  }

  return 0;
}


UINT32 SendDataDirect(UINT8 *pDataBuff,UINT32 DataLength,UINT32 TimeOut)
{
  UINT32 n;
  int complete;

  complete = 0;
  UDVT_Direct_Write(0,pDataBuff,DataLength,&complete);
  for(n=0;n<TimeOut;n++)
  {
    //if(complete == 1)
    if(UDVT_IsDirectWriteComplete(0) == 1)
    {
      
      break;
    }
    else
    {
      x_thread_delay(100);
    }
  }

  return 0;
}


void _vPortInit(void)
{
//  k_uart_open(0,0);
//  k_uart_ioctl(0,0,UART_CMD_SET_PORT, UART_PORT_2);
//  k_uart_ioctl(0,0,UART_CMD_SET_MODE, 1);
//  k_uart_ioctl(0,0,UART_CMD_SET_BAUD, RS_232_SPEED_115200);
    UDVT_open(0);
  


}
extern void vTestMapUnUnit(void);
INT32 _i4UDVTInit(INT32 i4Argc, const CHAR ** szArgv)
{
  udvt_if_cfg.GetDataFun = GetDataFun;
  udvt_if_cfg.SendDataFun = SendDataFun;
  udvt_if_cfg.InitPortFun = _vPortInit;
  udvt_if_cfg.GetDataDirect = GetDataDirect;
  udvt_if_cfg.SendDataDirect = SendDataDirect;
  UDVT_Config(&udvt_if_cfg);
  UDVT_Init();

  phGetQueue = (HANDLE_T *)(UDVT_GetUDVTHandle(UDVT_GET_HANDLE_INPUT_QUEUE));
  phSendQueue =(HANDLE_T *)(UDVT_GetUDVTHandle(UDVT_GET_HANDLE_OUTPUT_QUEUE));
  vTestMapUnUnit();
  return 0;
}

extern INT32 DrvFSOpenFile(char* pcDirFileName, UINT32 dwFlags, INT32* piFd);
extern INT32 DrvFSGetFileSize(INT32 iFd, UINT32 *pu4FileSize);
extern INT32 DrvFSReadFile(INT32 iFd, void* pbBuf, UINT32 u4Count);
extern INT32 DrvFSUSBOpenFile(char* pcDirFileName, UINT32 dwFlags, INT32* piFd);
extern INT32 DrvFSUSBGetFileSize(INT32 iFd, UINT32 *pu4FileSize);
extern INT32 DrvFSUSBReadFile(INT32 iFd, void* pbBuf, UINT32 u4Count);
#if 1
extern unsigned long get_drvmem_mem(void);
#else
unsigned long get_drvmem_mem()
{
	return 0xC2000000;
}
#endif


INT32 _i4UDVTFileIOTest(INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 piFd = 0;
    UINT32 pu4FileSize = 0;
    UINT32 u4Addr = 0;
    UINT32 i = 0;

    INT32 piFdUSB = 0;
    UINT32 pu4USBFileSize = 0;
    UINT32 u4USBAddr = 0;

    while(1)
    {
        if (0 == DrvFSOpenFile("C:\\1.txt", "r+b", &piFd))
        {
            Printf("Open File Success!\n");
        }
        else
        {
            Printf("Open File Fail Return!\n");
            break;
        }
    
        if (0 == DrvFSGetFileSize(piFd, &pu4FileSize))
        {
            Printf("File Length = 0x%x\n", pu4FileSize);
        }
        else
        {
            DrvFSCloseFile(piFd);
            Printf("Get File Length fail return ;\n");
            break;
        }
        
        if (!u4Addr)
            u4Addr = x_alloc_aligned_dma_mem(pu4FileSize, 0x0);
    
        if ( pu4FileSize == DrvFSReadFile(piFd, u4Addr, pu4FileSize))
        {
            DrvFSCloseFile(piFd);
            Printf("UDVT Read File Success !\n");
        }
        else
        {
            DrvFSCloseFile(piFd);
            Printf("UDVT Read File Fail!\n");
            break;
        }
    }
    
    return 1;
}

INT32 _i4UDVTMemTest(void)
{
    UINT32 u4Addr = 0;
    UINT32 u4Size = 0x100000;
    UINT32 i = 0;

    for(i = 0; i < 300; i++)
    {
        u4Addr = x_alloc_aligned_dma_mem(u4Size, 0x0);

        Printf("Alloc Memory Addr = 0x%x@\n",u4Addr);

        if (0xC0000000 == u4Addr)
        {
            Printf("Alloc Memory fail @0x%x,[Cnt] = 0x%x\n", u4Size,i);
            break;
        }
        else
        {
            Printf("Alloc Memory success @0x%x\n", u4Size);
            x_free_aligned_dma_mem(u4Addr);
            u4Size += 0x100000;
        }
    }
    return 1;
}

INT32 _i4UDVTDrvMemTest(INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 i = 0;
    UINT32 u4Addr  = 0;
    UINT32 u4Addr1 = 0;

    u4Addr = (UINT32)get_drvmem_mem();

    Printf("Drv Mem Addr = 0x%x,!\n",u4Addr);

    x_memset(VIRTUAL(u4Addr), 0x55, 0x10000000);

    Printf("Memory Set Success!\n");

    u4Addr1 = x_alloc_aligned_dma_mem(0x100000, 0x0);

    x_memcpy(u4Addr1, VIRTUAL(u4Addr), 0x100000);

    Printf("Memory Copy Success!\n");

    _i4UDVTMemTest();

    return 1;

}

INT32 _i4UDVTSendVirtualPacket(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT32 icmd;
  UINT32 isrc;
  UINT32 idst;
  UINT32 tmpN;
  UINT8 *tmpD;

  UDVT_CompletePacket_s pck;

  if(i4Argc < 5)
  {
    Printf("[UDVT]Send Virtual Packet [command] [ISource] [IDestination] [Data] \n");
    return -1;
  }

  icmd = StrToInt(szArgv[1]);
  isrc = StrToInt(szArgv[2]);
  idst = StrToInt(szArgv[3]);

  pck.lCommand = icmd;
  pck.lDestinationId = idst;
  pck.lSourceId = isrc;

  tmpN = 0;
  tmpD = (UINT8 *)(pck.lData);
  while(szArgv[4][tmpN])
  {
    tmpD[tmpN] = szArgv[4][tmpN];
    tmpN++;
  }
  tmpD[tmpN] = 0;

  pck.sPacketHeader.PacketHeader = 0;
  pck.sPacketHeader.PacketLength = tmpN+12;
  pck.sPacketHeader.lPacketChecksum = 0;

  x_msg_q_send(*phGetQueue,&pck,tmpN+20,100);
  return 0;
}


INT32 _i4UDVTTargetSimulator(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT32 step;
  char tmpStr[] = "[UDVT]Simulator is running....\n";
  step = 0;
  while(step < 100)
  {
    x_thread_delay(1000);
    step += 10;
    UDVT_IF_TestProgress(step);
    Printf(tmpStr);
    
  }
  Printf("[UDVT]Simulator run end!\n");
  UDVT_IF_SendResult(UDVT_TEST_PASS);
  return 0;
}

INT32 _i4UDVTWriteFileTest(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT8 *pData;
  UINT8 *wstr;
  UINT32 fHandle;
  UINT32 WriteLength;

  
  UINT32 n;
  UINT32 checksum = 0;

  pData = x_alloc_aligned_dma_mem(0x100000,4);
  if(i4Argc < 3)
  {
    Printf("[UDVT]fa [filename] [mode] [wstring]\n");

    return -1;
  }

  fHandle = UDVT_IF_OpenFile(szArgv[1],szArgv[2]);
  if(fHandle == 0)
  {
    Printf("[UDVT] Open pc file %s fail!\n",szArgv[1]);
  }
  else
  {
    Printf("[UDVT] Open pc file %s success!\n",szArgv[1]);
  }

  wstr = (UINT8 *)(szArgv[3]);
  n=0;
  while(*wstr)
  {
    pData[n] = *wstr;
    n++;
    wstr++;
  }

  pData[n] = 0;
  WriteLength = n;
  UDVT_IF_WriteFile(pData,1,WriteLength,fHandle);
  UDVT_IF_CloseFile(fHandle);

  for(n=0;n<WriteLength;n++)
  {
      
    checksum += pData[n];
  }
  FATS_IF_SaveLog(0,"\n[UDVT] Write %d btyes,checksum is %d\n",WriteLength,checksum);
  Printf("\n[UDVT] Write %d btyes,checksum is %d\n",WriteLength,checksum);
  x_free_aligned_dma_mem(pData);
  UDVT_IF_SendResult(UDVT_TEST_PASS);
  return 0;
}

INT32 _i4UDVTSaveLogTest(INT32 i4Argc, const CHAR ** szArgv)
{

  UINT32 lognum;
	UINT32 logn;


  if(i4Argc < 2)
  {
    Printf("[UDVT]sl [log num]\n");

    return -1;
  }
  lognum = StrToInt(szArgv[1]);
  
  for(logn=0;logn<lognum;logn++)
  {
  	FATS_IF_SaveLog(0,"[UDVT] log save test %d",logn);
  }
  return 0;
}

INT32 _i4UDVTAccessFileTest(INT32 i4Argc, const CHAR ** szArgv)
{
  UINT8 *pData;
  UINT32 fHandle;
  UINT32 ReadLength;
  UINT32 fileLength;
  
  UINT32 n;
  UINT32 checksum = 0;

  pData = x_alloc_aligned_dma_mem(0x100000,4);
  if(i4Argc < 3)
  {
    Printf("[UDVT]fa [filename] [mode]\n");

    return -1;
  }

  fHandle = UDVT_IF_OpenFile(szArgv[1],szArgv[2]);
  if(fHandle == 0)
  {
    Printf("[UDVT] Open pc file %s fail!\n",szArgv[1]);
  }
  else
  {
    Printf("[UDVT] Open pc file %s success!\n",szArgv[1]);
  }

  fileLength = UDVT_IF_GetFileLength(fHandle);
  Printf("file length is %d\n",fileLength);
  while(fileLength > 0)
  {
    if(fileLength > 0x100000)
    {
      
      ReadLength = UDVT_IF_ReadFile(pData,1,0x100000,fHandle);
      if(ReadLength != 0x100000)
      {
        Printf("error occur at %d\n",fileLength);
        break;
      }
      else
      {
        fileLength -= ReadLength;
      }
    }
    else
    {
      ReadLength = UDVT_IF_ReadFile(pData,1,fileLength,fHandle);
      if(ReadLength != fileLength)
      {
        Printf("error occur at %d\n",fileLength);
        break;
      }
      else
      {
        fileLength -= ReadLength;
      }
    }

    for(n=0;n<ReadLength;n++)
    {
      
      checksum += pData[n];
    }
  }

  UDVT_IF_CloseFile(fHandle);
  Printf("\n[UDVT] Read %d btyes,checksum is %d\n",fileLength,checksum);
  x_free_aligned_dma_mem(pData);
  UDVT_IF_SendResult(UDVT_TEST_PASS);
  return 0;
}


static CLI_EXEC_T arUDVTCmdTbl[] = {
//    CLIMOD_DEBUG_CLIENTRY(NOR),

    {"init","i",_i4UDVTInit,NULL,"UDVT Initialization",CLI_GUEST},
    {"SVP","s",_i4UDVTSendVirtualPacket,NULL,"Send Virtual Packet",CLI_GUEST}, 
    {"Simulator","sim",_i4UDVTTargetSimulator,NULL,"UDVT Target Simulator",CLI_GUEST},
    {"AccessFile","fa",_i4UDVTAccessFileTest,NULL,"UDVT Open File Test",CLI_GUEST},
    {"WriteFile","wf",_i4UDVTWriteFileTest,NULL,"UDVT Write File Test",CLI_GUEST},
    {"Savelog","sl",_i4UDVTSaveLogTest,NULL,"UDVT Save Log Test",CLI_GUEST},
    {"MemTest","mt",_i4UDVTFileIOTest,NULL,"UDVT Mem Test",CLI_GUEST},
    {NULL, NULL, NULL, NULL, NULL, CLI_GUEST}
};

/*
 * Do not set static and Add "CLI_MAIN_COMMAND" to add command dir into cli.
 */
/*
CLI_MAIN_COMMAND_ITEM(Nor)
{
    "nor", NULL, NULL, arNorCmdTbl, "Nor command", CLI_SUPERVISOR
};
*/

CLI_MAIN_COMMAND_ITEM(udvt)
{
    "udvt", NULL, NULL, arUDVTCmdTbl,
    "udvt command", CLI_GUEST
};



/*
static CLI_EXEC_T _rUDVTModCmdTbl =
{
    "UDVT",
    NULL,
    NULL,
    arUDVTCmdTbl,
    "UDVT command",
    CLI_GUEST
};


CLI_EXEC_T* GetUDVTCmdTbl(void)
{
    return &_rUDVTModCmdTbl;
}
*/

