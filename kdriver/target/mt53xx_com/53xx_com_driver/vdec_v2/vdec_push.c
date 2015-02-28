#include "vdec_drvif.h"
#include "vdec_push.h"
#include "fbm_drvif.h"

#include "imgrz_if.h"
#include "imgrz_drvif.h"

#include "dmx_mm_if.h"
#ifdef ENABLE_MULTIMEDIA
#include "img_lib_if.h"
#endif
#ifdef CC_TRUSTZONE_SUPPORT
#include "tz_if.h"
#endif

LINT_EXT_HEADER_BEGIN
#include "vdec_debug.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_mid.h"
LINT_EXT_HEADER_END


//static BOOL _fgPushInit = FALSE;
VDEC_DECODER_T *_prVdecPush = NULL;
//static BOOL _fgFirstVideoChunk = TRUE;
#define VDEC_PUSH_VP8_INVALID_PTS (2)
// #VPUSH_RV_SLICE_BUF#

#if VDEC_PUSH_MAX_DECODER==4
UINT32 *_pdwVPSliceBuf[VDEC_PUSH_MAX_DECODER]={NULL,NULL,NULL,NULL};
#elif VDEC_PUSH_MAX_DECODER==3
UINT32 *_pdwVPSliceBuf[VDEC_PUSH_MAX_DECODER]={NULL,NULL,NULL};
#elif VDEC_PUSH_MAX_DECODER==2
UINT32 *_pdwVPSliceBuf[VDEC_PUSH_MAX_DECODER]={NULL,NULL};
#else
UINT32 *_pdwVPSliceBuf[VDEC_PUSH_MAX_DECODER]={NULL};
#endif

//RV related #VPUSH_RV_SLICE_BUF#  

enum
{
    MOVE_SPECIAL_DATA_CONTINUE = 1,
    MOVE_SPECIAL_DATA_DO_NOT_CONTINUE = 2,
    MOVE_SPECIAL_DATA_FAIL = 3,
}MOVE_SPECIAL_DATA_RET;

enum
{
    SEND_ES_CONTINUE = 1,
    SEND_ES_DO_NOT_CONTINUE = 2,
    SEND_ES_FAIL = 3,
}SEND_ES_RET;

typedef INT32 (*MOVE_SPECIAL_DATA_FUNC)(VDEC_T*);
typedef INT32 (*PROCESS_MOVE_DATA_DONE)(const DMX_PES_MSG_T*);

typedef struct __CODEC_ENTRY_T
{
    MOVE_SPECIAL_DATA_FUNC move_special_data;
    PROCESS_MOVE_DATA_DONE process_move_data_done;
    VOID* priv;
}CODEC_ENTRY_T;


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#if defined(CC_TRUSTZONE_SUPPORT) && defined(CC_SVP_SUPPORT)
#define SUPPORT_VPUSH_TRUST_ZONE //Get memory from trust zone
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define CHECK_FALSE_RET(x) \
          do { \
            if (x == FALSE) { \
                LOG(3, "%s(%d) ret FALSE\n", __FUNCTION__, __LINE__); \
                return FALSE; \
            } \
          } while (0)

#define CHECK_FALSE(x) \
          do { \
            if (x == FALSE) { \
                LOG(3, "%s(%d) ret FALSE\n", __FUNCTION__, __LINE__); \
            } \
          } while (0)

#define VPUSH_MSG_UNDERRUN_THRD (1)

#define VPUSH_DATA_TIMER_THRSD (500) // 0.5 sec

#define ADDRESS_DEC_ALIGN(addr,align) (addr&(~(align-1)))
#define ADDRESS_INC_ALIGN(addr,align) ((addr+align-1)&(~(align-1)))

#define VPUSH_IS_PIC(fmt) ((VDEC_FMT_JPG == (fmt)) || (VDEC_FMT_PNG == (fmt)))
#define IDR_PIC 5
#define NonIDR_PIC 1
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))
#define IS_FRAME_NALU(x) ((((x & 0x1f) == IDR_PIC))||(((x & 0x1f) == NonIDR_PIC))) 

#define DMX_MOVE_DATA_TIMEOUT (5000)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//static HANDLE_T _hBufSem = NULL_HANDLE;
#ifdef ENABLE_MULTIMEDIA
static VDEC_APP_TYPE_T _arVdecAppType[] =
{
    {"SKYPE", 5, SWDMX_SRC_TYPE_NETWORK_SKYPE},
};
#endif

#ifdef ENABLE_MULTIMEDIA
static VDEC_CONTAINER_TYPE_T _arVdecContainerType[] =
{
    {"avi", 3, SWDMX_FMT_AVI},
    {"AVI", 3, SWDMX_FMT_AVI},
    {"mkv", 3, SWDMX_FMT_MKV},
    {"MKV", 3, SWDMX_FMT_MKV},
    {"rmff", 4, SWDMX_FMT_RM},
    {"RMFF", 4, SWDMX_FMT_RM},
    {"ts", 2, SWDMX_FMT_MPEG2_TS},
    {"TS", 2, SWDMX_FMT_MPEG2_TS},
};
#endif

extern CODEC_ENTRY_T s_codec_map[VDEC_FMT_MAX];

//-----------------------------------------------------------------------------
// Function prototype
//-----------------------------------------------------------------------------

VOID _VPUSH_PushLoop(VOID* pvArg);
void _VPUSH_SendCmd(VOID* prdec, UINT32 u4Cmd);
#ifdef LINUX_TURNKEY_SOLUTION
extern UINT32 u4SecureBufferAlloc(size_t size);
extern BOOL bSecureBufferFree(UINT32 u4Handle);
#endif
extern VOID FBM_FlushLockToEmptyQ(UCHAR ucFbgId);

#if 0
//-----------------------------------------------------------------------------
/** Enter buffer critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _BufLock(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        VERIFY(x_sema_create(&_hBufSem, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    VERIFY(x_sema_lock(_hBufSem, X_SEMA_OPTION_WAIT) == OSR_OK);
}

//-----------------------------------------------------------------------------
/** Leave buffer API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _BufUnlock(void)
{
    VERIFY(x_sema_unlock(_hBufSem) == OSR_OK);
}
#endif

static void _VPUSH_VDEC_Nfy(
    void*               pvNfyTag,
    VDEC_DEC_COND_T      eNfyCond,
    UINT32              u4Data1,
    UINT32              u4Data2)
{
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;

    prVdec = (VDEC_T*)pvNfyTag;

    if(!prVdec)
    {
        LOG(3, "%s(%d): prVdec null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);

    switch (eNfyCond)
    {
    case VDEC_COND_CTRL_DONE:
        {
            LOG(6, "%s(%d): VDEC_COND_CTRL_DONE(%d)\n", __FUNCTION__, __LINE__, u4Data1);
            if(u4Data1 == 1)//(UINT32)VID_DEC_CTRL_STOP)
            {
                _VPUSH_SendCmd((VOID*)prVdec, (UINT32)VPUSH_CMD_STOP);
            }
            else
            {
                LOG(1, "%s(%d): u4Data1(%d) unknown\n", __FUNCTION__, __LINE__, u4Data1);
            }
        }
        break;
    case VDEC_COND_DECODE_STATUS_CHG:
        {
            switch ( u4Data1 )
            {
                case VDEC_DEC_DECODE_FORCE_STOP:
                case VDEC_DEC_DECODE_CODEC_NOT_SUPPORT:
                case VDEC_DEC_DECODE_HD_NOT_SUPPORT:
                case VDEC_DEC_DECODE_ERROR:
                case VDEC_DEC_DECODE_CODEC_TYPE_CHG:
                case VDEC_DEC_DECODE_RES_NOT_SUPPORT:
                    if(prVdec->rInpStrm.fnCb.pfnVdecErrHandler)
                    {
                        LOG(0, "[VPUSH] call Vdec error(%d) handler,ErrorPes %d\n", u4Data1,prVdecEsInfo->u4EsDataCnt);
                        prVdec->rInpStrm.fnCb.pfnVdecErrHandler(
                            prVdec->rInpStrm.fnCb.u4VdecErrTag,
                            &u4Data1);
                    }
                    else
                    {
                        LOG(0, "[VPUSH] unexpected error(%d)\n", u4Data1);
                    }
                    break;
                default:
                    LOG(1, "[VPUSH] ignore status(%d)\n", u4Data1);
                    break;

            }
        }
        break;
    case VDEC_COND_THUMBNAIL_DONE:
    case VDEC_COND_THUMBNAIL_FAIL:
        {
             int i4ThumbResult;
        
            i4ThumbResult = (VDEC_COND_THUMBNAIL_DONE == eNfyCond) ? 0 : -1; // return 0 if success
            if(prVdec->rInpStrm.fnCb.pfnVdecThumbCb)
            {
                LOG(1, "[VPUSH] call Vdec thumbnail callback %d\n", i4ThumbResult);
                prVdec->rInpStrm.fnCb.pfnVdecThumbCb(
                    prVdec->rInpStrm.fnCb.u4VdecThumbTag,
                    &i4ThumbResult);
            }
        }
        break;
    case VDEC_COND_VPUSH_IO_DONE:
        {
            if (prVdecEsInfo->fgParsingInfo)
            {
                VERIFY(x_sema_unlock(prVdec->hIoQSema) == OSR_OK);
            }
        }
        break;
    case VDEC_COND_ERROR:
        {
            if(prVdec->rInpStrm.fnCb.pfnVdecErrHandler)
            {
                  LOG(0, "[VPUSH] call Vdec error(%d) handler\n", u4Data1);
                  prVdec->rInpStrm.fnCb.pfnVdecErrHandler(
                          prVdec->rInpStrm.fnCb.u4VdecErrTag,&u4Data1);
            }
        }
        break;
    default:
        {
            LOG(6, "%s %d\n", __FUNCTION__, __LINE__);
        }
    }

}

static UINT8 _VPUSH_GetPushId(VDEC_T *prVdec)
{
    UINT8 uIndex;
    for(uIndex=0;uIndex<VDEC_PUSH_MAX_DECODER;uIndex++)
    {
        if(prVdec==&(_prVdecPush->arDec[uIndex]))
        {
           return uIndex;
        }
    }

	return 0;
}

static BOOL u4AllocFbmBuffer(VDEC_T *prVdec, FBM_TYPE_T eType, UINT32 *puAddress, UINT32 *pu4Size)
{
    UINT8 ucVpushId;
    UINT32 u4VFifoAddr = 0, u4VFifoSize = 0;

    if (!prVdec || !puAddress || !pu4Size)
    {
        LOG(0, "%s Null pointer!\n", __FUNCTION__);
        return FALSE;
    }

    ucVpushId = _VPUSH_GetPushId(prVdec);

    switch (eType)
    {
    case FBM_TYPE_FEEDER:
        if (!prVdec->prFeederPool)
        {
            prVdec->prFeederPool = FBM_Alloc(FBM_TYPE_FEEDER);
            if (!prVdec->prFeederPool)
            {
                LOG(0,"[VPUSH-%d] Error, no available feeder\n", prVdec->ucVPushId);
                return FALSE;
            }
            u4VFifoAddr = prVdec->prFeederPool->u4Addr;
            u4VFifoSize = prVdec->prFeederPool->u4Size;
        }
        else
        {
            LOG(0,"[VPUSH-%d] Error, Already have feeder allocated\n", prVdec->ucVPushId);
            return FALSE;
        }
        break;
    case FBM_TYPE_DMX:
#if defined(SUPPORT_VPUSH_TRUST_ZONE)
    case FBM_TYPE_TZ_DMX:
#endif
        if (!prVdec->prDmxPool)
        {
            prVdec->prDmxPool = FBM_Alloc(eType);
            if (!prVdec->prDmxPool)
            {
                LOG(0,"[VPUSH-%d] Error, no available dmx[%d]\n", prVdec->ucVPushId, eType);
                return FALSE;
            }
            u4VFifoAddr = prVdec->prDmxPool->u4Addr;
            u4VFifoSize = prVdec->prDmxPool->u4Size;
        }
        else
        {
            LOG(0,"[VPUSH-%d] Error, Already have dmx[%d] allocated\n", prVdec->ucVPushId, eType);
            return FALSE;
        }
        break;
    default:
        LOG(0,"%s can not handle this type!\n", __FUNCTION__);
        ASSERT(0);
        return FALSE;
    }
    
    *puAddress = u4VFifoAddr;
    *pu4Size = u4VFifoSize;
    
    LOG(0, "%s *puAddress 0x%08X *pu4Size 0x%X\n", __FUNCTION__, *puAddress, *pu4Size);
    
    return TRUE;
}



static UINT32 _VPUSH_GetMsgCountInQ(VOID* prdec)
{
    VDEC_T *prVdec;
    INT32   i4Ret;
    UINT16  u2MstCount = 0;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    i4Ret = x_msg_q_num_msgs(prVdec->hMsgQ, &u2MstCount);

    if (i4Ret == OSR_NO_MSG)
    {
        u2MstCount = 0;
    }

    prVdec->u2MstCount = u2MstCount;
    return (u2MstCount);
}

#ifdef ENABLE_MULTIMEDIA
static BOOL _VPUSH_AddRvPic(VDEC_T *prVdec, VDEC_PES_INFO_T *prPesInfo)
{
#define VPUSH_IS_RV_B_PIC(p)  (3 == (p))

    INT32 i4TrDelta;
    BOOL fgDoSend = TRUE;
    UINT32 u4CurrPicTr;

#if 0 // no more need to access VFIFO!
    pbSc = (UINT8 *)VIRTUAL(prPesInfo->u4VldReadPtr);
    pbSc += prPesInfo->u1SliceCount * 8 + 1;

    for (i = 0; i < 4; i++)
    {
        if ((UINT32)pbSc >= VIRTUAL(prPesInfo->u4FifoEnd))
        {
            pbSc -= prPesInfo->u4FifoEnd - prPesInfo->u4FifoStart;
        }

        u4CurrPicTr <<= 8;
        u4CurrPicTr |= ((*pbSc) & 0xFF);

        pbSc++ ;
    }
#else
    u4CurrPicTr = prVdec->dwFirst4CC ; 
#endif

    if (VDEC_PUSH_FMT_RV30 == prVdec->ePushFmt)
    {
        prPesInfo->ucPicType = ((u4CurrPicTr >> 24) >> 3) & 3;
        u4CurrPicTr = ((u4CurrPicTr & 0xFFF80) >> 7);
    }
    else if (VDEC_PUSH_FMT_RV40 == prVdec->ePushFmt)
    {
        prPesInfo->ucPicType = ((u4CurrPicTr >> 24) >> 5) & 3;
        u4CurrPicTr = ((u4CurrPicTr & 0x7FFC0) >> 6);
    }
    else
    {
        LOG(0, "%s(%d): unknown subtype %d\n", __FUNCTION__, __LINE__, prVdec->ePushFmt);
        ASSERT(0);
        return FALSE;
    }

    if (VPUSH_IS_RV_B_PIC(prPesInfo->ucPicType))
    {
        if (prVdec->u4FordwardRefPts >= prVdec->u4BackwardRefPts)  // tmp solution for out-of-order B frames
        {
            fgDoSend = FALSE;
        }

        if (prVdec->u8FordwardRefPts >= prVdec->u8BackwardRefPts)  // tmp solution for out-of-order B frames
        {
            fgDoSend = FALSE;
        }

        i4TrDelta = u4CurrPicTr - prVdec->u4FordwardRefTr;
        if (i4TrDelta < 0)
        {
            i4TrDelta += 0x2000;
        }
        prPesInfo->u4PTS = prVdec->u4FordwardRefPts + (i4TrDelta * 90);
        prPesInfo->u8PTS = prVdec->u8FordwardRefPts + (i4TrDelta * 1000);
    }
    else
    {
        prVdec->u4FordwardRefTr = prVdec->u4BackwardRefTr;
        prVdec->u4BackwardRefTr = u4CurrPicTr;
        prVdec->u4FordwardRefPts = prVdec->u4BackwardRefPts;
        prVdec->u4BackwardRefPts = prPesInfo->u4PTS;
        prVdec->u8FordwardRefPts = prVdec->u8BackwardRefPts;
        prVdec->u8BackwardRefPts = prPesInfo->u8PTS;
    }

    return (fgDoSend)? TRUE : FALSE;
}
#endif

static BOOL __generate_vpes_from_dmx_msg(VDEC_PES_INFO_T* rPesInfo, const DMX_PES_MSG_T *prPes)
{
    if (!rPesInfo || !prPes)
    {
        return FALSE;
    }
    
    // u4Dts is 33-64 bits of u8PTS.
    #ifdef VDEC_PUSH_PTS_64_BITS
    rPesInfo->u8PTS = (UINT64)prPes->u4Dts;
    rPesInfo->u8PTS <<= 32;
    rPesInfo->u8PTS += (UINT64)prPes->u4Pts;
    {
        UINT64 u8Remainder;
        rPesInfo->u4PTS = (UINT32)u8Div6432((rPesInfo->u8PTS*9), 100, &u8Remainder);
        rPesInfo->u4DTS = prPes->u4Pts; // VDEC uses u4DTS as 32 bits DTS value
    }
    #else
    rPesInfo->u4PTS = prPes->u4Pts;
    rPesInfo->u4DTS = prPes->u4Pts; // VDEC uses u4DTS as 32 bits DTS value
    #endif
    rPesInfo->u4FifoStart = prPes->u4BufStart;
    rPesInfo->u4FifoEnd = prPes->u4BufEnd;
    rPesInfo->u4VldReadPtr = prPes->u4FrameAddr;
	rPesInfo->u4VldReadPtrEnd = prPes->u4Wp;
    rPesInfo->fgDtsValid = prPes->fgPtsDts;
    rPesInfo->fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo->fgGop = prPes->fgGop;
    rPesInfo->fgEos = prPes->fgEOS;
    rPesInfo->u8Offset = 0;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo->u4DmxFrameType = prPes->u4FrameType;
    #endif
    rPesInfo->fgMoveComplete = prPes->fgMoveComplete;

    return TRUE;
}

static  INT32 __vp9_process_dmx_move_data_cb(const DMX_PES_MSG_T *prPes)
{
    UCHAR ucBuf = 0;
    UINT32 u4Addr = 0;
    VDEC_T *prVdec = NULL;
    VDEC_PES_INFO_T* pPesInfo = NULL;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    prVdec = (VDEC_T *)prPes->pvInstanceTag;
    if (!prVdec)
    {
        return SEND_ES_FAIL;
    }

    pPesInfo = &(prVdec->arPesInfo[0]);
    x_memset(pPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    if (!__generate_vpes_from_dmx_msg(pPesInfo, prPes))
    {
        return SEND_ES_FAIL;
    }
    
    
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    pPesInfo->ucEsId = (UCHAR)prVdec->ucVdecId;
    pPesInfo->ucMpvId = VLD0;

    ASSERT(prVdecEsInfoKeep->fgLowLatency); // vp9 Always use the Lowlatency mode

    if (pPesInfo->fgMoveComplete)
    {
        VERIFY(x_sema_unlock(prVdec->hMoveEsmQSema) == OSR_OK); 
        return SEND_ES_DO_NOT_CONTINUE;
    }
    else
    {
        if(!pPesInfo->fgEos)
        {
            if(prPes->u4FrameAddr == 0)
            {
                LOG(3, "MMCallback Addr 0!\n");
            }
            else
            {
                ASSERT((pPesInfo->u4VldReadPtr < pPesInfo->u4FifoEnd) && (pPesInfo->u4VldReadPtr >= pPesInfo->u4FifoStart));
                x_memcpy((void*)pPesInfo->au1PicInfo, (void*)prPes->au1PicInfo, DMX_PICINFO_SIZE);
             //   x_memcpy((void*)(&(prVdec->arPesInfo[0])), (void*)&rPesInfo, sizeof(VDEC_PES_INFO_T));        //VP9 
             //   prVdec->arPesInfo[i].u4VldReadPtrEnd = prVdecEsInfo->u4VFifoPrevWptr;     //Fixme This is not needed?
             //   prVdec->arPesInfo[i].fgMoveComplete = TRUE;   //Fixme This is not needed?
                prVdecEsInfo->fgKeepPes = TRUE;

                #ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
                ucBuf = *(UCHAR*)(u4Addr);
                #else
                u4Addr = prPes->u4FrameAddr;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                ucBuf = *(UCHAR*)VIRTUAL(u4Addr);
                #endif

                if ((ucBuf&0x04)==0)
                {
                    prVdecEsInfo->ucPicType = I_TYPE;
                    LOG(3, "%s(%d): Key Frame\n", __FUNCTION__, __LINE__);
                }
                else
                {
                    LOG(3, "%s(%d): Normal Frame\n", __FUNCTION__, __LINE__);
                }

                VDEC_SendEs(pPesInfo);

            }
            
        }
        else
        {
            pPesInfo->u4FifoStart = 0;
        	pPesInfo->u4FifoEnd = 0;
        	pPesInfo->u4VldReadPtr = 0;
        	pPesInfo->u4VldReadPtrEnd = 0;
			VDEC_SendEs(pPesInfo);
            LOG(0, "%s(%d): fgEos\n", __FUNCTION__, __LINE__);
        }

        prVdec->fgFirstVideoChunk = FALSE;
    }

    return SEND_ES_DO_NOT_CONTINUE;
}


static BOOL _VPUSH_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_ES_INFO_T *prVdecEsInfo;    
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    VDEC_T *prVdec;
	INT32 u4SendDataRet = SEND_ES_CONTINUE;

    prVdec = (VDEC_T*)prPes->pvInstanceTag;

    if (VPUSH_ST_STOP == prVdec->eCurState)
    {
        LOG(1, "%s: VPUSH is in stop state.\n", __FUNCTION__);
        return TRUE;
    }

    if (s_codec_map[prVdec->eFmt].process_move_data_done)
    {
        u4SendDataRet = s_codec_map[prVdec->eFmt].process_move_data_done(prPes);
        if (u4SendDataRet == SEND_ES_DO_NOT_CONTINUE || u4SendDataRet == SEND_ES_FAIL) 
        {
            return TRUE;
        }
    }

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    rPesInfo.ucEsId = (UCHAR)prVdec->ucVdecId;
    if (prVdec->eFmt == VDEC_FMT_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {
        rPesInfo.ucMpvId = VLD0;
    }

    // u4Dts is 33-64 bits of u8PTS.
    #ifdef VDEC_PUSH_PTS_64_BITS
    rPesInfo.u8PTS = (UINT64)prPes->u4Dts;
    rPesInfo.u8PTS <<= 32;
    rPesInfo.u8PTS += (UINT64)prPes->u4Pts;
    {
        UINT64 u8Remainder;
        rPesInfo.u4PTS = (UINT32)u8Div6432((rPesInfo.u8PTS*9), 100, &u8Remainder);
        rPesInfo.u4DTS = prPes->u4Pts; // VDEC uses u4DTS as 32 bits DTS value
    }
    #else
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Pts; // VDEC uses u4DTS as 32 bits DTS value
    #endif
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.u4VldReadPtrEnd = prPes->u4Wp;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = 0;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif
   // rPesInfo.fgMoveComplete = prPes->fgMoveComplete;
    //LOG(3, "%s: u4FrameAddr%x MoveComplete:%d\n", __FUNCTION__,prPes->u4FrameAddr, prPes->fgMoveComplete);
#if 0
    if (rPesInfo.fgMoveComplete)
    {
        if(prVdecEsInfoKeep->fgLowLatency)
        {
            UINT32 i;
            for(i = 0; i < prVdec->ucPesIndex; i++)
            {
                if(i == prVdec->ucPesIndex - 1)
                {
                    prVdec->arPesInfo[i].fgMoveComplete = TRUE;
                    if(!prVdec->arPesInfo[i].fgEos)
                    {
                        prVdec->arPesInfo[i].u4VldReadPtrEnd = prVdecEsInfo->u4VFifoPrevWptr;
                        if(prVdec->eFmt == VDEC_FMT_H264)
                        {
                            if((prVdec->arPesInfo[i].u4FifoEnd - prVdecEsInfo->u4VFifoPrevWptr >= 6) && ((prVdec->arPesInfo[i].ucPicType & 0x1f) != 0x09))
                            {
                                UINT64 u4Delim = 0xF00901000000;
#if  defined(SUPPORT_VPUSH_TRUST_ZONE)
                                TZ_AU_DELIM_APPEND(prVdec->arPesInfo[i].u4VldReadPtrEnd, 1, 6, (UINT32 *)(&u4Delim));
#else
                                x_memcpy((VOID*)(VIRTUAL(prVdec->arPesInfo[i].u4VldReadPtrEnd + 1)), (VOID*)(&u4Delim), 6);
                                HalFlushInvalidateDCacheMultipleLine((VIRTUAL(prVdec->arPesInfo[i].u4VldReadPtrEnd + 1)),6);
#endif
                                prVdec->arPesInfo[i].u4VldReadPtrEnd += 5;
                                
                                LOG(3, "%s(%d): u4VldReadPtrEnd %x\n", __FUNCTION__, __LINE__, prVdec->arPesInfo[i].u4VldReadPtrEnd);
                            }
                        }
                        else if(prVdec->eFmt == VDEC_FMT_MP4)
                        {
                            if((prVdec->arPesInfo[i].u4FifoEnd - prVdecEsInfo->u4VFifoPrevWptr >=4))
                            {
                                UINT32 u4Delim = 0xB6010000;
#if  defined(SUPPORT_VPUSH_TRUST_ZONE)
                                TZ_AU_DELIM_APPEND(prVdec->arPesInfo[i].u4VldReadPtrEnd, 0, 4, &u4Delim);
#else
                                x_memcpy((VOID*)(VIRTUAL(prVdec->arPesInfo[i].u4VldReadPtrEnd)), (VOID*)(&u4Delim),4);
                                HalFlushInvalidateDCacheMultipleLine((VIRTUAL(prVdec->arPesInfo[i].u4VldReadPtrEnd)),4);
#endif
                                LOG(3, "%s(%d): u4VldReadPtrEnd %x\n", __FUNCTION__, __LINE__, prVdec->arPesInfo[i].u4VldReadPtrEnd);
                                prVdec->arPesInfo[i].u4VldReadPtrEnd += 4;
                            }
                        }
                    }
                }
                prVdecEsInfo->fgKeepPes = TRUE;
                VDEC_SendEs((void*)(&(prVdec->arPesInfo[i])));
            }
            prVdec->ucPesIndex = 0;
        }
        else
        {
            if(prVdecEsInfo->fgParsingInfo)
            {
                // push the buffered data
                VDEC_PES_INFO_T rPesInfo1;
                x_memcpy((VOID*)&rPesInfo1, (VOID*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
                rPesInfo1.ucPicType = I_TYPE;
                rPesInfo1.fgSeqEnd = TRUE;
                VDEC_SendEs((void*)&rPesInfo1);
            }
        }
        
        VERIFY(x_sema_unlock(prVdec->hMoveEsmQSema) == OSR_OK);
        return TRUE;
    }
#endif

    if(!rPesInfo.fgEos)
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }
    else
    {
        LOG(6, "%s(%d): fgEos\n", __FUNCTION__, __LINE__);
    }

    switch(prVdec->eFmt)
    {
        case VDEC_FMT_MPV:
            {
                switch (prPes->u4FrameType)
                {
                case PIC_TYPE_I:
                    rPesInfo.ucPicType = MPV_I_TYPE;
                    break;
                case PIC_TYPE_P:
                    rPesInfo.ucPicType = MPV_P_TYPE;
                    break;
                case PIC_TYPE_B:
                    rPesInfo.ucPicType = MPV_B_TYPE;
                    break;
                case PIC_TYPE_SEQ_END:
                    rPesInfo.ucPicType = MPV_I_TYPE;
                    rPesInfo.fgSeqEnd = TRUE;
                    break;
                default:
                    rPesInfo.ucPicType = 0;
        #ifndef CC_VDEC_FMT_DETECT
                    ASSERT(0);
                    break;
        #else
                    return TRUE;
        #endif
                }
            }
            break;
            
        case VDEC_FMT_H264:
            {
#ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
                rPesInfo.ucPicType = *(UINT8*)(u4Addr);
#else
                u4Addr = prPes->u4FrameAddr + 3;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);
#endif
                
                u4Addr = prPes->u4FrameAddr + 4;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                rPesInfo.u4VldReadPtr = u4Addr;
        
                if (prVdec->fgDecoderCalPts)
                {
                    if(IS_IDR_NALU(rPesInfo.ucPicType))
                    {
                        rPesInfo.fgDtsValid = TRUE;
                    }
                    else
                    {
                        rPesInfo.fgDtsValid = FALSE;
                    }            
                }
            
                // decoder calculates the timestamp
                if(!prVdec->fgFirstVideoChunk)
                {
        //              if(rPesInfo.u8PTS == 0)
        //              {
        //  #ifdef ENABLE_MULTIMEDIA
        //   //               if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_SKYPE)
        //                  {                    
        //                      LOG(3, "pts==0 fgDtsValid = false\n");
        //                      rPesInfo.fgDtsValid = FALSE;
        //                  }
        //  #endif
        //              }
                }
            }
            break;
            
        case VDEC_FMT_H265:
            {
                rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_H265;
                rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
            
                LOG(6, "rPesInfo.ucPicType: 0x%02x\n", rPesInfo.ucPicType);
            
                u4Addr = prPes->u4FrameAddr + 3;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                rPesInfo.u4VldReadPtr = u4Addr;
            
                switch (prPes->u4FrameType)
                {
                    case H265_SLICE_BLA_W_LP:           
                    case H265_SLICE_BLA_W_RADL:             
                    case H265_SLICE_BLA_N_LP:               
                    case H265_SLICE_IDR_W_RADL:             
                    case H265_SLICE_IDR_N_LP:               
                    case H265_SLICE_TRAIL_CRA:              
                    LOG(7, "VPUSH Got Iframe pts = %x \n",prPes->u4Pts);
                    
                    break;
            
                    /* case H265_PIC_TYPE_P:
                    u4SwdmxPicType = SWDMX_PIC_TYPE_P;
                    break;*/
            
                    /*case PIC_TYPE_B:
                    break;*/
            
                    default:
                    break;
                }
            }
            break;
            
        case VDEC_FMT_MP4:
            {
                if (VDEC_PUSH_FMT_MP4 == prVdec->ePushFmt)
                {
                    UCHAR *pucBuf;
        
                    if (prVdec->fgDecoderCalPts)
                    {
                        if(rPesInfo.u8PTS == prVdec->u8PrePts)
                        {
                            rPesInfo.fgNoVdecCalPts = TRUE;
                        }
                        else
                        {
                            rPesInfo.fgNoVdecCalPts = prVdec->fgNotCalPTS;
                        }
                    }
                    
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    u4Addr = (UINT32)prPes->au1PicInfo;
                    pucBuf = (UCHAR*)(u4Addr);
        #else
                    u4Addr = prPes->u4FrameAddr + 3;
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        #endif
        
                    if (pucBuf[0] == 0xB6)
                    {
                        if (prVdec->fgDecoderCalPts)
                        {
                            prVdec->fgNotCalPTS = TRUE;
                        }
                        
            #ifdef SUPPORT_VPUSH_TRUST_ZONE
                        u4Addr = (UINT32)(prPes->au1PicInfo + 1);
                        pucBuf = (UCHAR*)(u4Addr);
            #else
                        u4Addr = prPes->u4FrameAddr + 4;
                        if(u4Addr >= prPes->u4BufEnd)
                        {
                            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                        }
                        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            #endif
                        
                        switch (pucBuf[0] >> 6)
                        {
                            case 0:
                                rPesInfo.ucPicType  = I_TYPE;
                                if (prVdec->fgDecoderCalPts)
                                {
                                    rPesInfo.fgDtsValid = TRUE;
                                }
                                break;
                            case 1: case 3:
                                rPesInfo.ucPicType  = P_TYPE;
                                if (prVdec->fgDecoderCalPts)
                                {
                                    rPesInfo.fgDtsValid = FALSE;
                                }
                                break;
                            case 2:
                                rPesInfo.ucPicType = B_TYPE;
                                if (prVdec->fgDecoderCalPts)
                                {
                                    rPesInfo.fgDtsValid = FALSE;
                                }
                                break;
                            default:
                                rPesInfo.ucPicType = UNKNOWN_TYPE;
                                break;
                        }
                    }
                    else if ((pucBuf[0] & 0x20) == 0x20)
                    {
                        rPesInfo.fgSeqHdr = TRUE;
                    }
                    prVdec->u8PrePts = rPesInfo.u8PTS;
                }
                else if (VDEC_PUSH_FMT_DIVX311 == prVdec->ePushFmt)
                {
                    UCHAR *pucBuf;
        
                    rPesInfo.fgDtsValid = TRUE;
        
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    u4Addr = (UINT32)prPes->au1PicInfo;
                    pucBuf = (UCHAR*)(u4Addr);
        #else
                    u4Addr = prPes->u4FrameAddr;
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        #endif
        
                    if ((pucBuf[0] >> 6) & 0x1)
                    {
                        rPesInfo.ucPicType  = P_TYPE;
                    }
                    else
                    {
                        rPesInfo.ucPicType = I_TYPE;
                    }
                }
                else if (VDEC_PUSH_FMT_H263 == prVdec->ePushFmt)
                {
                    UCHAR *pucBuf;
                    rPesInfo.fgDtsValid = TRUE;
        
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    u4Addr = (UINT32)(prPes->au1PicInfo + 1);
                    pucBuf = (UCHAR*)(u4Addr);
        #else
                    u4Addr = prPes->u4FrameAddr + 4;
        
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        #endif
        
                    if((pucBuf[0] >> 1) & 0x01)
                    {
                        rPesInfo.ucPicType  = P_TYPE;
                    }
                    else
                    {
                        rPesInfo.ucPicType = I_TYPE;
                    }
                }
                else if (VDEC_PUSH_FMT_SORENSEN == prVdec->ePushFmt)
                {
                    UCHAR *pucBuf;
                    UINT8 u1SourceFormat;
                    
                    rPesInfo.fgDtsValid = TRUE;
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    u4Addr = (UINT32)prPes->au1PicInfo;
                    pucBuf = (UCHAR*)(u4Addr);
        #else
                    u4Addr = prPes->u4FrameAddr + 3;
        
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        #endif
                    
                    u1SourceFormat = (pucBuf[0] & 0x03)<<1;
                    
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    u4Addr = (UINT32)(prPes->au1PicInfo + 1);
        #else
                    u4Addr = prPes->u4FrameAddr + 4;
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
        #endif
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
                    
                    u1SourceFormat |= (pucBuf[0]>>7);
        
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    if (0 == u1SourceFormat)
                    {
                        u4Addr = (UINT32)(prPes->au1PicInfo + 3);
                    }
                    else if(1 == u1SourceFormat)
                    {
                        u4Addr = (UINT32)(prPes->au1PicInfo + 5);
                    }
                    else
                    {
                        u4Addr = (UINT32)(prPes->au1PicInfo + 1);
                    }
                    pucBuf = (UCHAR*)(u4Addr);
        #else
                    if (0 == u1SourceFormat)
                    {
                        u4Addr = prPes->u4FrameAddr + 6;
                    }
                    else if(1 == u1SourceFormat)
                    {
                        u4Addr = prPes->u4FrameAddr + 8;
                    }
                    else
                    {
                        u4Addr = prPes->u4FrameAddr + 4;
                    }
        
                    if(u4Addr >= prPes->u4BufEnd)
                    {
                        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                    }
                    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        #endif
                    
                    if((pucBuf[0] >> 5) & 0x03)
                    {
                        rPesInfo.ucPicType  = P_TYPE;
                    }
                    else
                    {
                        rPesInfo.ucPicType = I_TYPE;
                    }
                }        
            }
            break;
            
        case VDEC_FMT_AVS:
            {
                rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_AVS;
#ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
                rPesInfo.ucPicType = *(UINT8*)(u4Addr);
#else
                u4Addr = prPes->u4FrameAddr + 3;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);
    #endif
                
                if (rPesInfo.ucPicType == 0xB3)
                {
                }
                else if (rPesInfo.ucPicType == 0xB0)
                {
                    rPesInfo.fgSeqHdr = TRUE;
                }
            }
            break;
            
        case VDEC_FMT_MJPEG:            
            rPesInfo.ucPicType = I_TYPE;
            break;
            
        case VDEC_FMT_RV:
            {
                if(!prVdec->fgPacketAppend)
                {
                   // update PES slice info (wp && count)
                    rPesInfo.u4RenderPts = prVdec->dwCrntFrmSliceSzWp;
                    rPesInfo.u1SliceCount = prVdec->u1CrntFrmSliceNum;
                    LOG(3,"[vcbk]Wp = %d, Cnt = 0x%x\n",rPesInfo.u4RenderPts,rPesInfo.u1SliceCount);
    	#ifdef ENABLE_MULTIMEDIA
                    if (SWDMX_FMT_RM == prVdecEsInfo->eContainerType)
                    {
                        rPesInfo.fgDtsValid = TRUE;         
                            if (!_VPUSH_AddRvPic(prVdec, &rPesInfo))
                            {
                                LOG(1, "_VPUSH_AddRvPic fail!!!\n");
                                VERIFY(x_sema_unlock(prVdec->hRvMoveSema) == OSR_OK);
                                return TRUE; // drop this data
                            }           
                    }
                    else  // for MKV container type! don't have to calculate B PTS.
        #endif
                    {           
                        UCHAR ucPicType = prVdec->dwFirst4CC>>24;
            
                        rPesInfo.fgDtsValid = TRUE;
                        if (VDEC_PUSH_FMT_RV30 == prVdec->ePushFmt)
                        {
                            rPesInfo.ucPicType = (ucPicType >> 3) & 3;
                        }
                        else if (VDEC_PUSH_FMT_RV40 == prVdec->ePushFmt)
                        {
                            rPesInfo.ucPicType = (ucPicType >> 5) & 3;
                        }
                        else
                        {
                            LOG(0, "%s(%d): unknown subtype %d\n", __FUNCTION__, __LINE__, prVdec->ePushFmt);
                            ASSERT(0);
                            rPesInfo.ucPicType = 0;
                        }
                    }
                }
            }
            break;
            
        case VDEC_FMT_VP8:
            {
                UCHAR ucBuf;
        
                rPesInfo.fgDtsValid = TRUE;
                if(rPesInfo.u4DTS==VDEC_PUSH_VP8_INVALID_PTS) //workaround for vfifo ring to start address
                {
                    return TRUE;
                }
        
#ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
                ucBuf = *(UCHAR*)(u4Addr);
#else
                u4Addr = prPes->u4FrameAddr;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                ucBuf = *(UCHAR*)VIRTUAL(u4Addr);
#endif
                
                if ((ucBuf&0x1)==0)
                {
                    rPesInfo.ucPicType = I_TYPE;
                }
                else
                {
                    rPesInfo.ucPicType  = P_TYPE;
                }
            }
            break;
            
        case VDEC_FMT_VP6:
            {
                UCHAR ucBuf;
        
                rPesInfo.fgDtsValid = TRUE;
    #ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
                ucBuf = *(UCHAR*)(u4Addr);
    #else
                u4Addr = prPes->u4FrameAddr;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                ucBuf = *(UCHAR*)VIRTUAL(u4Addr);
    #endif
                
                if ((ucBuf&0x80)==0)
                {
                    rPesInfo.ucPicType = I_TYPE;
                }
                else
                {
                    rPesInfo.ucPicType = P_TYPE;
                }
            }
            break;
            
        case VDEC_FMT_WMV:
            {
                if(prVdec->ePushFmt == VDEC_PUSH_FMT_VC1)
                {
                    rPesInfo.u4VideoType = DMX_VIDEO_VC1;
                }
                else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV7)
                {
                    rPesInfo.u4VideoType = DMX_VIDEO_WMV7;
                }
                else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV8)
                {
                    rPesInfo.u4VideoType = DMX_VIDEO_WMV8;
                }
                else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV9)
                {
                    rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
                }
        
                rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        
                if(prVdec->fgFirstVideoChunk && prVdec->ePushFmt != VDEC_PUSH_FMT_VC1)
                {
                    VDEC_SEQUENCE_DATA_T rVDecSeqData;
                    UINT32 u4SequenceHeader;
                    UINT16 u4Width = prVdec->u4Width;
                    UINT16 u4Height = prVdec->u4Height;
        
        #ifdef SUPPORT_VPUSH_TRUST_ZONE
                    x_memcpy((VOID*)&u4SequenceHeader, (VOID*)((UINT32)prPes->au1PicInfo),4);
        #else
                    x_memcpy((VOID*)&u4SequenceHeader, (VOID*)VIRTUAL(prPes->u4FrameAddr),4);
        #endif
                    u4SequenceHeader = ((u4SequenceHeader&0x000000FF)<<24) | ((u4SequenceHeader&0x0000FF00)<<8) | ((u4SequenceHeader&0x00FF0000)>>8) | ((u4SequenceHeader&0xFF000000)>>24);
        
                    rVDecSeqData.u2_width = (UINT16)u4Width;
                    rVDecSeqData.u2_height = (UINT16)u4Height;
                    rVDecSeqData.u4SequenceHeader = u4SequenceHeader;
                    rVDecSeqData.u2SequenceHeaderSize = 4;
                    rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
        
                    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
        
                    prVdec->fgFirstVideoChunk = FALSE;
        
#ifdef CC_SUPPORT_TV_STAGEFRIGHT
#if 0  // ToDo: consider the compatibility with old OMX!!!
                    // megaa 20110719: fix WMV7 divide by zero problem
                    // (For WMV7, the 1st data that DMX moves is not only sequence header, so we have to send ES here.)
                    if (prVdec->ePushFmt != VDEC_PUSH_FMT_WMV7)
                    {
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    
                    return TRUE;
#endif
#else //CC_SUPPORT_TV_STAGEFRIGHT
#if 1 // TODO: please wmv owner review this
                    // megaa 20110719: fix WMV7 divide by zero problem
                    // (For WMV7, the 1st data that DMX moves is not only sequence header, so we have to send ES here.)
                    if (prVdec->ePushFmt == VDEC_PUSH_FMT_WMV7)
                    {
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    
                    return TRUE;
#endif
#endif
                }
                else if (!prVdec->fgFirstVideoChunk && prVdec->ePushFmt != VDEC_PUSH_FMT_VC1)
                {
                    // Special error handle: If input data is 1 byte only, and it is NULL, set it as skip frame to decode driver
                    if ((prVdec->u4PacketSize == 1) && (!prVdec->fgPacketAppend))
                    {
                        UCHAR *pucBuf;
            #ifdef SUPPORT_VPUSH_TRUST_ZONE
                        u4Addr = (UINT32)prPes->au1PicInfo;
                        pucBuf = (UCHAR*)(u4Addr);
            #else
                        u4Addr = prPes->u4FrameAddr;
                        if(u4Addr >= prPes->u4BufEnd)
                        {
                            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                        }
                        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            #endif
        
                        if (pucBuf[0] == 0) // NULL data
                        {
                            LOG(1, "WMV receive 1-byte NULL input data !! \n");
                            rPesInfo.u2SkipVidCount = 1;
                        }
                    }
                }
            }
            break;
        case VDEC_FMT_VP9:
            {
                UCHAR ucBuf;
#ifdef SUPPORT_VPUSH_TRUST_ZONE
                u4Addr = (UINT32)prPes->au1PicInfo;
#else
                u4Addr = prPes->u4FrameAddr;
                if(u4Addr >= prPes->u4BufEnd)
                {
                    u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
                }
                u4Addr = VIRTUAL(u4Addr);
#endif
                ucBuf = *(UCHAR*)(u4Addr);
                if ((ucBuf&0x04)==0)
                {
                    rPesInfo.ucPicType = I_TYPE;
                }
                else
                {
                    rPesInfo.ucPicType  = P_TYPE;
                }                
                rPesInfo.u4VldReadPtrEnd = prPes->u4Wp;
            }
            break;
        default:
            break;
    }
    
    if((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(0, "MMCallback Addr 0!\n");
    }
      // add by liuqu ,20120531 #VPUSH_RV_SLICE_BUF#
    else if ((prVdec->eFmt == VDEC_FMT_RV) &&(prVdec->fgPacketAppend))
    {
        LOG(3,"RV send rest part(s) of a frame!\n");
    }
    else
    {
        x_memcpy((void*)rPesInfo.au1PicInfo, (void*)prPes->au1PicInfo, DMX_PICINFO_SIZE);
        if(prVdecEsInfoKeep->fgLowLatency)
        {
            if(prVdec->ucPesIndex > 0)
                prVdec->arPesInfo[prVdec->ucPesIndex -1].u4VldReadPtrEnd = rPesInfo.u4VldReadPtr;
            x_memcpy((void*)(&(prVdec->arPesInfo[prVdec->ucPesIndex])), (void*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
            prVdec->ucPesIndex++;
        }
        else
        {
            VDEC_SendEs((void*)&rPesInfo);
        }
#ifdef ENABLE_MULTIMEDIA
        #if 0
        if (prVdecEsInfo->eMMSrcType == SWDMX_SRC_TYPE_NETWORK_SKYPE)
        {
            // push 264 data
            VDEC_PES_INFO_T rPesInfo1;
            x_memcpy((VOID*)&rPesInfo1, (VOID*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
            rPesInfo1.ucPicType = I_TYPE;
            rPesInfo1.fgSeqEnd = TRUE;
            VDEC_SendEs((void*)&rPesInfo1);
        }
        #endif
#endif
    }

    if(prVdec->eFmt != VDEC_FMT_H264)
    {
        prVdec->fgFirstVideoChunk = FALSE;
    }
    else
    {
        if((IS_FRAME_NALU(rPesInfo.ucPicType))&&(prVdec->fgFirstVideoChunk == TRUE))
        {
            prVdec->fgFirstVideoChunk = FALSE;
        }
    }
	
    // #VPUSH_RV_SLICE_BUF#
    if (prVdec->eFmt == VDEC_FMT_RV)
    {
        VERIFY(x_sema_unlock(prVdec->hRvMoveSema) == OSR_OK);
    }

    return TRUE;
}
#ifdef ENABLE_MULTIMEDIA
BOOL _VPUSH_JpegCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT8 u1Marker1, u1Marker2;
    UINT32 u4Addr, u4DataSz1 = 0, u4DataSz2 = 0;
    VDEC_T* prVdec = (VDEC_T*)prPes->pvInstanceTag;
    static UINT32 _u4TotalDataSz = 0;
    
    if(prPes->u4FrameAddr < prPes->u4Wp)
    {
        u4DataSz1 = prPes->u4Wp - prPes->u4FrameAddr;
    }
    else
    {
        u4DataSz1 = prPes->u4BufEnd - prPes->u4FrameAddr;
        u4DataSz2 = prPes->u4Wp - prPes->u4BufStart;
    }

    //check image start
    u1Marker1 = *(UINT8*)VIRTUAL(prPes->u4FrameAddr);
    u4Addr = prPes->u4FrameAddr + 1;
    if(u4Addr == prPes->u4BufEnd)
    {
        u4Addr = prPes->u4BufStart;
    }
    u1Marker2 = *(UINT8*)VIRTUAL(u4Addr);
    if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))||((u1Marker1 == 0x89) && (u1Marker2 == 0x50)))
    {
        _u4TotalDataSz = 0; //reset
        prVdec->fgInputBufReady = FALSE;
    }
    _u4TotalDataSz = _u4TotalDataSz + (u4DataSz1 + u4DataSz2);

    //can change to dmx find start code
    if(prPes->fgEOS)
    {
        LOG(5, "--- Callback EOS\n");
    }
    //Fix me, keep picture on screen
    //if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))/* || prPes->fgEOS*/)
    if(((u1Marker1 == 0xFF) && (u1Marker2 == 0xD8))||((u1Marker1 == 0x89) && (u1Marker2 == 0x50)) /*|| prPes->fgEOS*/)        
    {
        x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

        rPesInfo.ucEsId = (UCHAR)prVdec->ucVdecId;
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg  ???
        // u4Dts is 33-64 bits of u8PTS.
        #ifdef VDEC_PUSH_PTS_64_BITS
        rPesInfo.u8PTS = (UINT64)prPes->u4Dts;
        rPesInfo.u8PTS <<= 32;
        rPesInfo.u8PTS += (UINT64)prPes->u4Pts;
        #endif
        rPesInfo.u4PTS = prPes->u4Pts;
        rPesInfo.u4DTS = prPes->u4Pts; // VDEC uses u4DTS as 32 bits DTS value
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        rPesInfo.fgEos = prPes->fgEOS;
        rPesInfo.u8Offset = 0;
        rPesInfo.ucPicType = I_TYPE;
        
        VDEC_SendEs((void*)&rPesInfo);
        //flush ES Q
        //if(!prPes->fgEOS)
        {
            rPesInfo.fgSeqEnd = TRUE;
            VDEC_SendEs((void*)&rPesInfo);
        }
    }
    
    //TODO: IMG_SetFillBsBufEnd(0, _u4TotalDataSz, 0, prPes->fgEOS);
    if(!prVdec->fgInputBufReady)
    {
        if((_u4TotalDataSz >= 0x80000) || prPes->fgEOS)
        {
            IMG_SetFillBsBufEnd(0,_u4TotalDataSz, 0, prPes->fgEOS);
            prVdec->fgInputBufReady = TRUE;
        }
    }
    else
    {
        IMG_SetFillBsBufEnd(0,u4DataSz1, u4DataSz2, prPes->fgEOS);
    }
    
    return TRUE;
}
#endif
VOID _VPUSH_SetVideoFormat(VOID* prdec)
{
    VDEC_T *prVdec;
	DMX_VIDEO_TYPE_T eDmxVideoType=DMX_VIDEO_MPEG;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    // to support multiple video format, demux need change format before
    // we call move data.
    if(prVdec->fgEnDmx)
    {
        switch (prVdec->eFmt)
        {
        case VDEC_FMT_H265:
            eDmxVideoType=DMX_VIDEO_H265;
            break;
        case VDEC_FMT_WMV:
            if(prVdec->ePushFmt == VDEC_PUSH_FMT_VC1)
            {
                eDmxVideoType=DMX_VIDEO_VC1;
            }
            else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV7)
            {
                eDmxVideoType=DMX_VIDEO_WMV7;
            }
            else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV8)
            {
                eDmxVideoType=DMX_VIDEO_WMV8;
            }
            else if(prVdec->ePushFmt == VDEC_PUSH_FMT_WMV9)
            {
                eDmxVideoType=DMX_VIDEO_WMV9;
            }
            else
            {
                eDmxVideoType=DMX_VIDEO_VC1;
            }
            break;
        case VDEC_FMT_MP4:
        case VDEC_FMT_MJPEG:
            eDmxVideoType=DMX_VIDEO_MPEG4;
            break;
        case VDEC_FMT_MPV:
            eDmxVideoType=DMX_VIDEO_MPEG;
            break;
        case VDEC_FMT_H264:
            eDmxVideoType=DMX_VIDEO_H264;
            break;
        case VDEC_FMT_AVS:
            eDmxVideoType=DMX_VIDEO_AVS;
            break;
        case VDEC_FMT_RV:
            eDmxVideoType=DMX_VIDEO_RV;
            break;
#ifdef VDEC_IS_POST_MT5368
        case VDEC_FMT_VP6:
            eDmxVideoType=DMX_VIDEO_VP6;
            break;
        case VDEC_FMT_VP8:
            eDmxVideoType=DMX_VIDEO_VP8;
            break;
#endif
        case VDEC_FMT_VP9:
            eDmxVideoType=DMX_VIDEO_VP9;
            break;;
        default:
            LOG(3, "%s(%d): format(%d) not support\n", __FUNCTION__, __LINE__, (UINT32)(prVdec->eFmt));
            eDmxVideoType=DMX_VIDEO_MPEG;
            break;
        }

        DMX_MUL_SetVideoType(prVdec->u1DmxId, eDmxVideoType);
        DMX_SetVideoTypeByPidx(prVdec->u1DmxPid,eDmxVideoType);
    }
}


BOOL _VPUSH_AllocDmx(VOID* prdec)
{
    DMX_DECODER_CALLBACKS_T rDmxCallback;
    DMX_MM_T rDmxMM;
    UINT32 u4Flag = 0;
    VDEC_T *prVdec;
    BOOL fgRet;
    UINT32 u4VfifoAddr = 0, u4VfifoSize = 0;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(prVdec->fgEnDmx)
    {
        LOG(3, "%s(%d): (prVdec->fgEnDmx) already allocated\n", __FUNCTION__, __LINE__);
        return TRUE;
    }

    DMX_MUL_EnableMultipleInst(TRUE);
    
    prVdec->u1DmxId =
        DMX_MUL_GetAvailableInst(
            (DMX_CONN_TYPE_T)((UINT32)DMX_CONN_TYPE_BUFAGENT_2 +
                prVdec->ucVPushId));
    if(prVdec->u1DmxId == 0xFF)
    {
        LOG(3, "%s(%d): DMX_MUL_GetAvailableInst fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    fgRet = DMX_MUL_SetInstType(prVdec->u1DmxId, DMX_IN_PLAYBACK_MM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    DMX_SetToDecoder(TRUE);
    x_memset(&rDmxCallback, 0, sizeof(rDmxCallback));
    #ifdef ENABLE_MULTIMEDIA
    if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_JpegCallback;
    }
    else
    #endif
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_VideoCallback;
    }
    rDmxCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    fgRet = DMX_MUL_SetDecoderCallbacks(prVdec->u1DmxId, &rDmxCallback);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    prVdec->u1DmxPid =DMX_MUL_GetAvailablePidx(prVdec->u1DmxId);
    if(prVdec->u1DmxPid >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(%d): (prVdec->u1DmxPid == DMX_MULTIPLE_NULL_PIDX)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    _VPUSH_SetVideoFormat(prdec);
    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));

    u4Flag = (UINT32)(DMX_MM_FLAG_TYPE
        | DMX_MM_FLAG_INSTANCE_TAG
        | DMX_MM_FLAG_VALID
        | DMX_MM_FLAG_BUF
        | DMX_MM_FLAG_SEARCH_START_CODE
        | DMX_MM_FLAG_COMPLETE_MSG
        );

    if ((VDEC_FMT_MPV == prVdec->eFmt)
    || (VDEC_FMT_H264 == prVdec->eFmt)    
    || (VDEC_FMT_H265 == prVdec->eFmt)
    || ((VDEC_FMT_WMV == prVdec->eFmt) && (prVdec->ePushFmt == VDEC_PUSH_FMT_VC1))
    || (VDEC_FMT_AVS == prVdec->eFmt)
    )
    {
        rDmxMM.fgSearchStartCode = TRUE;
    }
    else
    {
        rDmxMM.fgSearchStartCode = FALSE;
    }

    rDmxMM.fgEnable = TRUE;
    rDmxMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rDmxMM.pvInstanceTag = (void*)prVdec;
    rDmxMM.fgAllocBuf = FALSE;
    // same with VDEC id
    
#if defined(SUPPORT_VPUSH_TRUST_ZONE)
	if(u4AllocFbmBuffer(prVdec,FBM_TYPE_TZ_DMX,&u4VfifoAddr,&u4VfifoSize)==FALSE)
#else
    if(u4AllocFbmBuffer(prVdec,FBM_TYPE_DMX,&u4VfifoAddr,&u4VfifoSize)==FALSE)
#endif
    {
        LOG(0,"u4AllocFbmBuffer for dmx error \n");
		return FALSE;
    }

    rDmxMM.u4BufAddr = u4VfifoAddr;
    rDmxMM.u4BufSize = u4VfifoSize;
    rDmxMM.u1ChannelId = prVdec->ucChannelId;
    rDmxMM.fgSendCompleteMsg = TRUE;
    fgRet = DMX_MM_Set(prVdec->u1DmxPid, u4Flag, &rDmxMM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MM_Set fail\n", __FUNCTION__, __LINE__);
        //when set fail, nothing to free
        //DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
        
        return FALSE;
    }

    prVdec->u4VFifoSize = u4VfifoSize;
    prVdec->u4VFifoAddr = u4VfifoAddr;
   
    if(!DMX_MM_SetDRMMode(prVdec->u1DmxPid,DMX_DRM_MODE_BYPASS))
    {
        LOG(3, "%s(%d): DMX_MM_SetDRMMode fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    LOG(2,"VPush:DMX_MM_SetDRMMode set OK..........\n");

    prVdec->fgEnDmx = TRUE;
    return fgRet;
}

BOOL _VPUSH_ReleaseDmx(VOID* prdec)
{
    DMX_MM_T rDmxMM;
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(!prVdec->fgEnDmx)
    {
        LOG(3, "%s(%d): (!prVdec->fgEnDmx) not allocated\n", __FUNCTION__, __LINE__);
        return TRUE;
    }

    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
    rDmxMM.fgEnable = FALSE;

    VERIFY(DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_VALID, &rDmxMM));
    DMX_MM_Free(prVdec->u1DmxPid);
    DMX_MUL_FreePidx(prVdec->u1DmxPid);

    DMX_MUL_FreeInst(prVdec->u1DmxId);
    prVdec->u1DmxId = 0xFF;
    
    prVdec->fgEnDmx = FALSE;
    
    if(prVdec->prDmxPool)
    {
        FBM_Free(prVdec->prDmxPool);
        prVdec->prDmxPool = NULL;
    }
    else
    {
        return FALSE;
    }

    prVdec->u4VFifoSize = 0;
    prVdec->u4VFifoAddr = 0;

    return TRUE;
}


BOOL _VPUSH_AllocFeeder(VOID* prdec)
{
    VDEC_T *prVdec;
    UINT32 u4Size;
    UINT32 j;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(u4AllocFbmBuffer(prVdec,FBM_TYPE_FEEDER,&prVdec->u4FifoStart,&u4Size)==FALSE)
    {
        LOG(0,"Alloc Feeder buffer error \n");
	    return FALSE;
    }
	
    // DDI is 4 bytes start address alignment
    // DDI is 16 bytes buffer size alignment. (188 is better)
    prVdec->u4FifoStart=VIRTUAL(prVdec->u4FifoStart);
    prVdec->u4FifoEnd=prVdec->u4FifoStart+u4Size;
	/*
	if(u4Size > VDEC_PUSH_BIG_FEEDER_SIZE)
    {
        prVdec->u4BufCnt = VDEC_BUF_COUNT;
    }
    else
    {       
        prVdec->u4BufCnt = VDEC_BUF_COUNT/2;
    } 
	*/
//    u4Size = u4Size/ VDEC_BUF_COUNT;
	prVdec->u4BufCnt = VDEC_BUF_COUNT;

    u4Size = u4Size/ prVdec->u4BufCnt;
    u4Size -= (u4Size % 752);
    
    for(j=0;j<prVdec->u4BufCnt;j++)
    {
        prVdec->arBufInfo[j].fgUsed = FALSE;
        prVdec->arBufInfo[j].u4BufAddr = prVdec->u4FifoStart + (j * u4Size);
        prVdec->arBufInfo[j].u4BufSize = u4Size;

        LOG(2, "%s(%d): [Buf %d] addr 0x%08x, size 0x%08x \n", __FUNCTION__, __LINE__, 
            j, prVdec->arBufInfo[j].u4BufAddr, prVdec->arBufInfo[j].u4BufSize);
    }

    return TRUE;
}


BOOL _VPUSH_ReleaseFeeder(VOID* prdec)
{
    VDEC_T *prVdec;
    UINT32 j;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    prVdec->u4FifoStart = 0;
    prVdec->u4FifoEnd = 0;

    for(j=0;j<VDEC_BUF_COUNT;j++)
    {
        LOG(3, "_VPUSH_ReleaseFeeder\n");
        prVdec->arBufInfo[j].fgUsed = FALSE;
        prVdec->arBufInfo[j].u4BufAddr = 0;
        prVdec->arBufInfo[j].u4BufSize = 0;
    }

    if (prVdec->prFeederPool)
    {
        FBM_Free(prVdec->prFeederPool);
        prVdec->prFeederPool = NULL; 
    }

    return TRUE;
}


BOOL _VPUSH_SetDmx(VDEC_T *prVdec, BOOL fgSearchStartCode)
{
    DMX_DECODER_CALLBACKS_T rDmxCallback;
    DMX_MM_T rDmxMM;
    UINT32 u4Flag = 0;
    BOOL fgRet;

    if (!prVdec)
    {
        LOG(3, "%s(%d): prVdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if (prVdec->fgEnDmx)
    {
        x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
        rDmxMM.fgEnable = FALSE;

        VERIFY(DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_VALID, &rDmxMM));
        DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);
        //prVdec->fgEnDmx = FALSE;

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
    }

    DMX_MUL_EnableMultipleInst(TRUE);

    prVdec->u1DmxId =
        DMX_MUL_GetAvailableInst(
            (DMX_CONN_TYPE_T)((UINT32)DMX_CONN_TYPE_BUFAGENT_2 +
                prVdec->ucVPushId));
    if(prVdec->u1DmxId == 0xFF)
    {
        LOG(3, "%s(%d): DMX_MUL_GetAvailableInst fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    fgRet = DMX_MUL_SetInstType(prVdec->u1DmxId, DMX_IN_PLAYBACK_MM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    DMX_SetToDecoder(TRUE);
    x_memset(&rDmxCallback, 0, sizeof(rDmxCallback));
    #ifdef ENABLE_MULTIMEDIA
    if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_JpegCallback;
    }
    else
    #endif
    {
        rDmxCallback.pfnSendVideoPes = _VPUSH_VideoCallback;
    }
    rDmxCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    fgRet = DMX_MUL_SetDecoderCallbacks(prVdec->u1DmxId, &rDmxCallback);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MUL_SetDecoderCallbacks fail\n", __FUNCTION__, __LINE__);
        return fgRet;
    }

    prVdec->u1DmxPid =DMX_MUL_GetAvailablePidx(prVdec->u1DmxId);
    _VPUSH_SetVideoFormat((VOID *)prVdec);
    if(prVdec->u1DmxPid >= DMX_NUM_PID_INDEX)
    {
        LOG(3, "%s(%d): (prVdec->u1DmxPid == DMX_MULTIPLE_NULL_PIDX)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    u4Flag = (UINT32)(DMX_MM_FLAG_TYPE
        | DMX_MM_FLAG_INSTANCE_TAG
        | DMX_MM_FLAG_VALID
        | DMX_MM_FLAG_BUF
        | DMX_MM_FLAG_SEARCH_START_CODE
        | DMX_MM_FLAG_COMPLETE_MSG
        );

    x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
    rDmxMM.fgSearchStartCode = fgSearchStartCode;
    rDmxMM.fgEnable = TRUE;
    rDmxMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rDmxMM.pvInstanceTag = (void*)prVdec;
    rDmxMM.fgAllocBuf = FALSE;
    // same with VDEC id
	if(prVdec->u4VFifoSize==0 || prVdec->u4VFifoAddr==0)
	{
       UINT32 u4VfifoAddr=0,u4VfifoSize=0;

#if defined(SUPPORT_VPUSH_TRUST_ZONE)
	   if(u4AllocFbmBuffer(prVdec,FBM_TYPE_TZ_DMX,&u4VfifoAddr,&u4VfifoSize)==FALSE)
#else
	   if(u4AllocFbmBuffer(prVdec,FBM_TYPE_DMX,&u4VfifoAddr,&u4VfifoSize)==FALSE)
#endif
	   {
		   LOG(0,"u4AllocFbmBuffer for dmx error.. \n");
		   return FALSE;
	   }
              
	   prVdec->u4VFifoAddr=u4VfifoAddr;
	   prVdec->u4VFifoSize=u4VfifoSize;
	}
    rDmxMM.u4BufAddr = prVdec->u4VFifoAddr;
    rDmxMM.u4BufSize = prVdec->u4VFifoSize;
    rDmxMM.u1ChannelId = prVdec->ucChannelId;
    rDmxMM.fgSendCompleteMsg = TRUE;
    fgRet = DMX_MM_Set(prVdec->u1DmxPid, u4Flag, &rDmxMM);
    if(!fgRet)
    {
        LOG(3, "%s(%d): DMX_MM_Set fail\n", __FUNCTION__, __LINE__);
        //when set fail, nothing to free
        //DMX_MM_Free(prVdec->u1DmxPid);
        DMX_MUL_FreePidx(prVdec->u1DmxPid);

        DMX_MUL_FreeInst(prVdec->u1DmxId);
        prVdec->u1DmxId = 0xFF;
        return FALSE;
    }

    prVdec->fgEnDmx = TRUE;

    return TRUE;
}


#define H264_STARTCODE_LEN 3
#define MOVEPSSIZE (1024)


BOOL _VPUSH_DmxMoveData(VOID* prdec, DMX_MM_DATA_T *prDmxMMData)
{
    VDEC_T *prVdec;
    BOOL fgRet;

    prVdec = (VDEC_T*)prdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(prDmxMMData->u4FrameSize)
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prDmxMMData->u4StartAddr), (UINT32)(prDmxMMData->u4FrameSize));
    }

    if(prVdec->fgIsSecureInput)
    {
        prDmxMMData->u4BufStart=prVdec->u4SecureInputStartAddr;
        prDmxMMData->u4BufEnd= prVdec->u4SecureInputEndAddr;
        LOG(5,"VPush MoveData:BufAddr=0x%x,BufEnd=0x%x,Addr=0x%x,Size=0x%x,Pts=0x%x\n",prDmxMMData->u4BufStart, prDmxMMData->u4BufEnd, \
			 prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize, prDmxMMData->u4Pts);
    }

    // TODO: lock sema for multi-instance support
    _VPUSH_SetVideoFormat(prdec);
    fgRet = DMX_MUL_MoveData(prVdec->u1DmxId, prDmxMMData);

    if (fgRet && prDmxMMData->u4FrameSize)
    {
        prVdec->u8TotalPushSize += prDmxMMData->u4FrameSize;
    }
    else
    {
	    if(!fgRet)
		{
            LOG(0,"_VPUSH_DmxMoveData error ret=%d,size=%d,pts=%d\n",fgRet,prDmxMMData->u4FrameSize,prDmxMMData->u4Pts);
		}
    }

    return fgRet;
}

BOOL _VPUSH_MoveVC1StartCode(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    static UCHAR* pucHdrBufAlloc = 0;
    UINT32 u4Tmp = 0x0D010000;
 
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;


    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0x0;
        pucHdrBufAlloc[1] = 0x0;
        pucHdrBufAlloc[2] = 0x1;
        pucHdrBufAlloc[3] = 0xD;
    }

    x_memcpy((VOID*)(VIRTUAL((UINT32)&pucHdrBufAlloc[0])),(VOID*)&u4Tmp,4);
     
    //insert start code
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    rDmxMMData.u1Idx = prVdec->u1DmxPid;
    rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
    rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = prBytesInfo->fgEos;
    #ifdef VDEC_PUSH_PTS_64_BITS
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
    rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
    #else
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
    rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
    #endif
    rDmxMMData.fgFrameHead= TRUE;

    if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
    {
        LOG(1, "Demuxer fails to move sequence header. 1\n");
        fgRet = FALSE;
    }

    UNUSED(fgRet);
    return TRUE;
}

static BOOL _VPUSH_Move264StartCode(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    static UCHAR* pucHdrBufAlloc = 0;
    UINT32 u4Tmp = 0x01000000;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;


    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0x0;
        pucHdrBufAlloc[1] = 0x0;
        pucHdrBufAlloc[2] = 0x0;
        pucHdrBufAlloc[3] = 0x1;
    }

    x_memcpy((VOID*)(VIRTUAL((UINT32)&pucHdrBufAlloc[0])),(VOID*)&u4Tmp,4);

    //insert start code
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    rDmxMMData.u1Idx = prVdec->u1DmxPid;
    rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
    rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = prBytesInfo->fgEos;
    #ifdef VDEC_PUSH_PTS_64_BITS
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
    rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
    #else
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
    rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
    #endif

    if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
    {
        LOG(1, "Demuxer fails to move sequence header. 1\n");
        fgRet = FALSE;
    }

    UNUSED(fgRet);
    return TRUE;
}

BOOL _VPUSH_Move264FirstData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
#define SPS 0x07
#define PPS 0x08
#define IS_PS_NALU(x) ((((x) & 0x1f) == SPS) || (((x) & 0x1f) == PPS))
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet=TRUE;
    UCHAR* pucHdrBuf;
    UINT8 u1PSNum=0;
    UINT8 u1NumOfPS=0;
    UINT16 u2PSLength=0;
    UINT32 i=0, u4TotalMove=0;
    static UCHAR* pucHdrBufAlloc = 0;
    VDEC_ES_INFO_T* prVdecEsInfo;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    prVdec->u4NaluSizeRemain = 0;
    prVdec->u4NaluSizePending = 0;
    prVdec->u1StartCodeLenMinus1Remain = 0;

    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0;
        pucHdrBufAlloc[1] = 0;
        pucHdrBufAlloc[2] = 0;
        pucHdrBufAlloc[3] = 1;
    }
    pucHdrBuf = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
#ifdef ENABLE_MULTIMEDIA
	if((prVdecEsInfo->eContainerType == SWDMX_FMT_AVI) &&
            prBytesInfo->fgCodecConfigData && (pucHdrBuf[0]!=1 || ((pucHdrBuf[4]>>2)&0x3f)!=0x3f))
	{
        prVdec->fgInsertStartcode=FALSE;
	    prVdec->fgNonFirst=FALSE;
	    LOG(0, "%s(%d): CodecConfigData for sequence header error!!!\n", __FUNCTION__, __LINE__);
		return FALSE;
	}
	else
	{
		pucHdrBuf += 4; //skip unused info
		prVdec->u1StartCodeLenMinus1 = (*pucHdrBuf++) & 0x3;
		u4TotalMove = 5;
	}
#endif
	
    for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
    {
        if(u1PSNum == 0)
        {
            u1NumOfPS = (*pucHdrBuf++) & 0x1f;
        }
        else
        {
            u1NumOfPS = *pucHdrBuf++;
            if (u1NumOfPS > 128)
            {
                LOG(1, "# of PPS is wrong (%d)!!!\n", u1NumOfPS);
                _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                return FALSE;
            }
        }
        for (i = 0; i < u1NumOfPS; i++)
        {
            u2PSLength = (UINT16)(*pucHdrBuf++);
            u2PSLength <<= 8;
            u2PSLength += (UINT16)(*pucHdrBuf++);
            LOG(1, "## PSNum %d, Len %d, SCLen %d\n", 
                u1NumOfPS, u2PSLength, prVdec->u1StartCodeLenMinus1 + 1);
            if ((u4TotalMove + u2PSLength) > prBytesInfo->u4BytesSize)
            {
                LOG(1, "%s(%d): move size is wrong!!!\n", __FUNCTION__, __LINE__);
                _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                return FALSE;
            }
            //insert start code
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4FrameSize = 4;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            #ifdef VDEC_PUSH_PTS_64_BITS
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
            rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
            #else
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
            rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
            #endif

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 1\n");
                fgRet = FALSE;
            }

            if (!IS_PS_NALU(*pucHdrBuf))
            {
                LOG(1, "%s(%d): 1st byte(%02x) is not PS!!!\n", __FUNCTION__, __LINE__, *pucHdrBuf);
                _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                return FALSE;
            }

            //copy SPS or PPS
            //set move data parameter
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = prVdec->u4FifoStart;
            rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBuf;
            rDmxMMData.u4FrameSize = u2PSLength;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            #ifdef VDEC_PUSH_PTS_64_BITS
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
            rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
            #else
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
            rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
            #endif

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 2\n");
                fgRet = FALSE;
            }
            pucHdrBuf += u2PSLength;
            u4TotalMove += u2PSLength;
        }
    }
    UNUSED(fgRet);
    return TRUE;
}

BOOL _VPUSH_Move264Data(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    UCHAR *pucFdrBuf;
    UCHAR *pucBuf;
    UINT32 u4NaluSize;
    BOOL fgRet;
    UINT32 u4FrmSize;
    UINT32 i;
    UINT32   u4Loop = 0;
    UINT32   u4DataSize = 0;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    pucFdrBuf = (UCHAR*)prBytesInfo->u4BytesAddr;
    pucBuf = pucFdrBuf;
    u4FrmSize = 0;
    fgRet = TRUE;
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);

    if (prBytesInfo->u4BytesSize == 0 && prBytesInfo->fgEos)
    {
        x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
        rDmxMMData.u1Idx = prVdec->u1DmxPid;
        rDmxMMData.u4BufStart = prVdec->u4FifoStart;
        rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
        rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr;
        rDmxMMData.fgEOS = TRUE;
        #ifdef VDEC_PUSH_PTS_64_BITS
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
        rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
        #else
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
        rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
        #endif    
        rDmxMMData.u4FrameSize = 1;
        rDmxMMData.fgIgnoreData = TRUE;
    	
        fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
        return fgRet;
	}     
    while(u4FrmSize < prBytesInfo->u4BytesSize)
    {
        if(prVdec->u4NaluSizeRemain > 0)
        {
            if(prVdec->u4NaluSizeRemain <= prBytesInfo->u4BytesSize)
            {
                u4NaluSize = prVdec->u4NaluSizeRemain;
                prVdec->u4NaluSizeRemain = 0;
            }
            else
            {
                u4NaluSize = prBytesInfo->u4BytesSize;
                prVdec->u4NaluSizeRemain -= prBytesInfo->u4BytesSize;
            }
            u4FrmSize += u4NaluSize;
            pucFdrBuf += u4NaluSize;
        }
        else
        {
            u4NaluSize = prVdec->u4NaluSizePending;//0;
            //insert start code
            prVdec->u1StartCodeLenMinus1Remain = 
                (prVdec->u1StartCodeLenMinus1Remain>0)?prVdec->u1StartCodeLenMinus1Remain:prVdec->u1StartCodeLenMinus1;
            for(i = 0; i < prVdec->u1StartCodeLenMinus1Remain; i++)
            {
                if((u4FrmSize + i) == prBytesInfo->u4BytesSize)
                {
                    prVdec->u4NaluSizePending = u4NaluSize;
                    LOG(1, "##SC at boundary %d\n", i);
                    break;
                }
                u4NaluSize += (UINT32)*pucFdrBuf;
                u4NaluSize <<= 8;
                *pucFdrBuf++ = 0;
            }
            prVdec->u1StartCodeLenMinus1Remain -= i;

            if(prVdec->u1StartCodeLenMinus1Remain > 0) //start code is on push unit boundary
            {
                u4NaluSize = i;     
                u4FrmSize += i;
                LOG(1, "##SC at boundary %d\n", prVdec->u1StartCodeLenMinus1Remain);
            }
            else
            {
                u4NaluSize += (UINT32)*pucFdrBuf;
                prVdec->u4NaluSizePending = 0;

                if (u4NaluSize == 0)
                {
                    // move data and leave
					x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
                    rDmxMMData.u1Idx = prVdec->u1DmxPid;
                    rDmxMMData.u4BufStart = prVdec->u4FifoStart;
                    rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
                    rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr + 4;
                    rDmxMMData.u4FrameSize = prBytesInfo->u4BytesSize - 4;
                    rDmxMMData.fgEOS = prBytesInfo->fgEos;
                    #ifdef VDEC_PUSH_PTS_64_BITS
                    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
                    rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
                    #else
                    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
                    rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
                    #endif

                    fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
                    return fgRet;
                }
                else if(u4NaluSize >= (0x1000000))    // 16MB
                {
                    if(prVdecEsInfo)
                    {
                        prVdecEsInfo->u4NALUSizeErrThrsd++;
                        LOG(2, "Invalid NALU size(0x%x > 16MB), PTS(0x%x), Cnt(%d).\n", 
                                u4NaluSize, (UINT32)prBytesInfo->u8BytesPTS,prVdecEsInfo->u4NALUSizeErrThrsd);
                        if(prVdecEsInfo->u4NALUSizeErrThrsd >= 150)
                        {
                            LOG(0,"NALU size 150 times error,nfy not support\n");
                            _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                            return FALSE;
                        }
                    }
                    return TRUE;
                }
                
                *pucFdrBuf++ = 1;

                //if((prSwdmxMp4Info->u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                //    (prSwdmxMp4Info->u1StartCodeLenMinus1 > 0))
                //{
                    for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN - prVdec->u1StartCodeLenMinus1); u4Loop++)
                    {
                        pucBuf--;
                        *pucBuf = 0;
                    }
                u4FrmSize += ((prVdec->u1StartCodeLenMinus1 + 1) + u4NaluSize);
                pucFdrBuf += u4NaluSize;
                u4NaluSize += (H264_STARTCODE_LEN /*-
                                prVdec->u1StartCodeLenMinus1)+
                                (prVdec->u1StartCodeLenMinus1 */+ 1);
            }
            if(u4FrmSize <= prBytesInfo->u4BytesSize)
            {
                prVdec->u4NaluSizeRemain = 0;
                //LOG(2, "##Nalu %d/%d\n", u4NaluSize, prBytesInfo->u4BytesSize);
            }
            else
            {
                prVdec->u4NaluSizeRemain = u4FrmSize - prBytesInfo->u4BytesSize;
                LOG(1, "##Nalu %d/%d/Acc%d\n", u4NaluSize, prBytesInfo->u4BytesSize, prVdec->u4NaluSizeRemain);
                u4NaluSize -= prVdec->u4NaluSizeRemain;
            }
        }

        u4DataSize = u4NaluSize;/* + (H264_STARTCODE_LEN -
                            prVdec->u1StartCodeLenMinus1)+
                            (prVdec->u1StartCodeLenMinus1 + 1);*/
		x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
        rDmxMMData.u1Idx = prVdec->u1DmxPid;
        rDmxMMData.u4BufStart = prVdec->u4FifoStart;
        rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
        rDmxMMData.u4FrameSize = u4DataSize;
        rDmxMMData.fgEOS = prBytesInfo->fgEos;
        #ifdef VDEC_PUSH_PTS_64_BITS
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
        rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
        #else
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
        rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
        #endif

        fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
        //}

        // nalu size
        //u4FrmSize += ((prVdec->u1StartCodeLenMinus1 + 1) + u4NaluSize);
        //pucFdrBuf += u4NaluSize;
        pucBuf = pucFdrBuf;
    }
    return fgRet;
}

BOOL _VPUSH_Move265FirstData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
/*
#define HEVC_VPS 0x20
#define HEVC_SPS 0x21
#define HEVC_PPS 0x22
#define IS_HEVC_PS_NALU(x) ((((x) & 0x3f) == HEVC_SPS) || (((x) & 0x3f) == HEVC_PPS) || (((x) & 0x3f) == HEVC_VPS))
*/
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet=TRUE;
    UCHAR* pucHdrBuf;
    UINT8 u1PSNum=0;
    UINT8 u1PSNumArrays = 0;
    UINT8 u1NumOfPS=0;
    UINT16 u2PSLength=0;
    UINT32 i=0, u4TotalMove=0;
    static UCHAR* pucHdrBufAlloc = 0;
    VDEC_ES_INFO_T* prVdecEsInfo;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    prVdec->u4NaluSizeRemain = 0;
    prVdec->u4NaluSizePending = 0;
    prVdec->u1StartCodeLenMinus1Remain = 0;

    if(!pucHdrBufAlloc)
    {
        pucHdrBufAlloc = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(MOVEPSSIZE,16));
        pucHdrBufAlloc[0] = 0;
        pucHdrBufAlloc[1] = 0;
        pucHdrBufAlloc[2] = 0;
        pucHdrBufAlloc[3] = 1;
    }
    pucHdrBuf = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);
/*
    tempucHdrBuf = pucHdrBuf;
    printf("First 265 data:\n");
    for(J=0;J<prBytesInfo->u4BytesSize;J++)
    {
        printf("%02x ",(*tempucHdrBuf++));
    }
    printf("\n===End===\n");
    */
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);

    
#ifdef ENABLE_MULTIMEDIA
	if((prVdecEsInfo->eContainerType == SWDMX_FMT_AVI) &&
            prBytesInfo->fgCodecConfigData && (pucHdrBuf[0]!=1 || ((pucHdrBuf[4]>>2)&0x3f)!=0x3f))
	{
        prVdec->fgInsertStartcode=FALSE;
	    prVdec->fgNonFirst=FALSE;
	    LOG(0, "%s(%d): CodecConfigData for sequence header error!!!\n", __FUNCTION__, __LINE__);
		return FALSE;
	}
	else
	{
		pucHdrBuf += 21; //skip unused info
		prVdec->u1StartCodeLenMinus1 = 3;//(*pucHdrBuf++) & 0x3;
		pucHdrBuf ++;
		u1PSNumArrays = (*pucHdrBuf++);
		u4TotalMove = 25;
	}
#endif
	LOG(0,"PsNumArrays=%d\n",u1PSNumArrays);
    for(u1PSNum = 0; u1PSNum < u1PSNumArrays; u1PSNum++)    // 0 for SPS, 1 for PPS
    {
        LOG(0, "u1ArrayIdx: %d\n", u1PSNum);
        pucHdrBuf ++;//skip 1 byte for arraycompletence/reserve/nal_type

        u1NumOfPS = (UINT16)(*pucHdrBuf++);
        u1NumOfPS <<= 8;
        u1NumOfPS += (UINT16)(*pucHdrBuf++);

        LOG(0, "u1NumOfPS: %d\n", u1NumOfPS);
        for (i = 0; i < u1NumOfPS; i++)
        {
            u2PSLength = (UINT16)(*pucHdrBuf++);
            u2PSLength <<= 8;
            u2PSLength += (UINT16)(*pucHdrBuf++);
            LOG(0, "## PSNum %d, Len %d, SCLen %d\n", 
                u1NumOfPS, u2PSLength, prVdec->u1StartCodeLenMinus1 + 1);
            if ((u4TotalMove + u2PSLength) > prBytesInfo->u4BytesSize)
            {
                LOG(0, "%s(%d): move size is wrong!!!\n", __FUNCTION__, __LINE__);
                _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                return FALSE;
            }
            //insert start code
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4BufEnd = (UINT32)(pucHdrBufAlloc + MOVEPSSIZE);
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBufAlloc;
            rDmxMMData.u4FrameSize = 4;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            #ifdef VDEC_PUSH_PTS_64_BITS
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
            rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
            #else
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
            rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
            #endif

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 1\n");
                fgRet = FALSE;
            }
            /*
            if (!IS_HEVC_PS_NALU(*pucHdrBuf))
            {
                LOG(1, "%s(%d): 1st byte(%02x) is not PS!!!\n", __FUNCTION__, __LINE__, *pucHdrBuf);
              //  _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
              //  return FALSE;
            }
            */
            //copy SPS or PPS or VPS
            //set move data parameter
            x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
            rDmxMMData.u1Idx = prVdec->u1DmxPid;
            rDmxMMData.u4BufStart = prVdec->u4FifoStart;
            rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
            rDmxMMData.u4StartAddr = (UINT32)pucHdrBuf;
            rDmxMMData.u4FrameSize = u2PSLength;
            rDmxMMData.fgEOS = prBytesInfo->fgEos;
            #ifdef VDEC_PUSH_PTS_64_BITS
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
            rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
            #else
            rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
            rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
            #endif

            if (_VPUSH_DmxMoveData(prVdec, &rDmxMMData)  != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header. 2\n");
                fgRet = FALSE;
            }
            pucHdrBuf += u2PSLength;
            u4TotalMove += u2PSLength;
        }
    }
    UNUSED(fgRet);
    return TRUE;
}

BOOL _VPUSH_Move265Data(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    UCHAR *pucFdrBuf;
    UCHAR *pucBuf;
    UINT32 u4NaluSize;
    BOOL fgRet;
    UINT32 u4FrmSize;
    UINT32 i;
    UINT32   u4Loop = 0;
    UINT32   u4DataSize = 0;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    pucFdrBuf = (UCHAR*)prBytesInfo->u4BytesAddr;
    pucBuf = pucFdrBuf;
    u4FrmSize = 0;
    fgRet = TRUE;
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
	x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (prBytesInfo->u4BytesSize == 0 && prBytesInfo->fgEos)
    {
        rDmxMMData.u1Idx = prVdec->u1DmxPid;
        rDmxMMData.u4BufStart = prVdec->u4FifoStart;
        rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
        rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr;
        rDmxMMData.fgEOS = TRUE;
        #ifdef VDEC_PUSH_PTS_64_BITS
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
        rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
        #else
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
        rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
        #endif    
        rDmxMMData.u4FrameSize = 1;
        rDmxMMData.fgIgnoreData = TRUE;
    	
        fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
        return fgRet;
	}     
    while(u4FrmSize < prBytesInfo->u4BytesSize)
    {
        if(prVdec->u4NaluSizeRemain > 0)
        {
            if(prVdec->u4NaluSizeRemain <= prBytesInfo->u4BytesSize)
            {
                u4NaluSize = prVdec->u4NaluSizeRemain;
                prVdec->u4NaluSizeRemain = 0;
            }
            else
            {
                u4NaluSize = prBytesInfo->u4BytesSize;
                prVdec->u4NaluSizeRemain -= prBytesInfo->u4BytesSize;
            }
            u4FrmSize += u4NaluSize;
            pucFdrBuf += u4NaluSize;
        }
        else
        {
            u4NaluSize = prVdec->u4NaluSizePending;//0;
            //insert start code
            prVdec->u1StartCodeLenMinus1Remain = 
                (prVdec->u1StartCodeLenMinus1Remain>0)?prVdec->u1StartCodeLenMinus1Remain:prVdec->u1StartCodeLenMinus1;
            for(i = 0; i < prVdec->u1StartCodeLenMinus1Remain; i++)
            {
                if((u4FrmSize + i) == prBytesInfo->u4BytesSize)
                {
                    prVdec->u4NaluSizePending = u4NaluSize;
                    LOG(1, "##SC at boundary %d\n", i);
                    break;
                }
                u4NaluSize += (UINT32)*pucFdrBuf;
                u4NaluSize <<= 8;
                *pucFdrBuf++ = 0;
            }
            prVdec->u1StartCodeLenMinus1Remain -= i;

            if(prVdec->u1StartCodeLenMinus1Remain > 0) //start code is on push unit boundary
            {
                u4NaluSize = i;     
                u4FrmSize += i;
                LOG(1, "##SC at boundary %d\n", prVdec->u1StartCodeLenMinus1Remain);
            }
            else
            {
                u4NaluSize += (UINT32)*pucFdrBuf;
                prVdec->u4NaluSizePending = 0;

                if (u4NaluSize == 0)
                {
                    // move data and leave
                    rDmxMMData.u1Idx = prVdec->u1DmxPid;
                    rDmxMMData.u4BufStart = prVdec->u4FifoStart;
                    rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
                    rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr + 4;
                    rDmxMMData.u4FrameSize = prBytesInfo->u4BytesSize - 4;
                    rDmxMMData.fgEOS = prBytesInfo->fgEos;
                    #ifdef VDEC_PUSH_PTS_64_BITS
                    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
                    rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
                    #else
                    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
                    rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
                    #endif

                    fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
                    return fgRet;
                }
                else if(u4NaluSize >= (0x1000000))    // 16MB
                {
                    if(prVdecEsInfo)
                    {
                        prVdecEsInfo->u4NALUSizeErrThrsd++;
                        LOG(2, "Invalid NALU size(0x%x > 16MB), PTS(0x%x), Cnt(%d).\n", 
                                u4NaluSize, (UINT32)prBytesInfo->u8BytesPTS,prVdecEsInfo->u4NALUSizeErrThrsd);
                        if(prVdecEsInfo->u4NALUSizeErrThrsd >= 150)
                        {
                            LOG(0,"NALU size 150 times error,nfy not support\n");
                            _VPUSH_VDEC_Nfy(prdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
                            return FALSE;
                        }
                    }
                    return TRUE;
                }
                
                *pucFdrBuf++ = 1;

                //if((prSwdmxMp4Info->u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                //    (prSwdmxMp4Info->u1StartCodeLenMinus1 > 0))
                //{
                    for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN - prVdec->u1StartCodeLenMinus1); u4Loop++)
                    {
                        pucBuf--;
                        *pucBuf = 0;
                    }
                u4FrmSize += ((prVdec->u1StartCodeLenMinus1 + 1) + u4NaluSize);
                pucFdrBuf += u4NaluSize;
                u4NaluSize += (H264_STARTCODE_LEN /*-
                                prVdec->u1StartCodeLenMinus1)+
                                (prVdec->u1StartCodeLenMinus1 */+ 1);
            }
            if(u4FrmSize <= prBytesInfo->u4BytesSize)
            {
                prVdec->u4NaluSizeRemain = 0;
                //LOG(2, "##Nalu %d/%d\n", u4NaluSize, prBytesInfo->u4BytesSize);
            }
            else
            {
                prVdec->u4NaluSizeRemain = u4FrmSize - prBytesInfo->u4BytesSize;
                LOG(1, "##Nalu %d/%d/Acc%d\n", u4NaluSize, prBytesInfo->u4BytesSize, prVdec->u4NaluSizeRemain);
                u4NaluSize -= prVdec->u4NaluSizeRemain;
            }
        }

        u4DataSize = u4NaluSize;/* + (H264_STARTCODE_LEN -
                            prVdec->u1StartCodeLenMinus1)+
                            (prVdec->u1StartCodeLenMinus1 + 1);*/

        rDmxMMData.u1Idx = prVdec->u1DmxPid;
        rDmxMMData.u4BufStart = prVdec->u4FifoStart;
        rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
        rDmxMMData.u4FrameSize = u4DataSize;
        rDmxMMData.fgEOS = prBytesInfo->fgEos;
        #ifdef VDEC_PUSH_PTS_64_BITS
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
        rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
        #else
        rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
        rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
        #endif

        fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);
        //}

        // nalu size
        //u4FrmSize += ((prVdec->u1StartCodeLenMinus1 + 1) + u4NaluSize);
        //pucFdrBuf += u4NaluSize;
        pucBuf = pucFdrBuf;
    }
    return fgRet;
}


#ifdef CC_SUPPORT_TV_STAGEFRIGHT // TODO: please rv owner review this
static VOID _VPUSH_RvConfig(VDEC_T *prVdec, BOOL fgIsRv9)
{
    if (fgIsRv9)
    {
        //VDEC_ES_INFO_T *prVdecEsInfo;
        LOG(0, "FMT_RV40\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_RV40);
    }
    else
    {
        //VDEC_ES_INFO_T *prVdecEsInfo;
        LOG(0, "FMT_RV30\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_RV30);
    }

    VERIFY(_VPUSH_SetDmx(prVdec, FALSE));
}
#endif


// parse rv codec  opaque data
VOID _VPUSH_RvOpaqueParse(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
#define HX_GET_MAJOR_VERSION(prodVer) (((prodVer) >> 28) & 0xF)
#define HX_GET_MINOR_VERSION(prodVer) (((prodVer) >> 20) & 0xFF)

#define RV30_MAJOR_BITSTREAM_VERSION  3
#define RV30_BITSTREAM_VERSION        2

#define RV40_MAJOR_BITSTREAM_VERSION  4
#define RV40_BITSTREAM_VERSION        0

    UINT8 *pu1Data;
    UINT32 ui4_spo_extra_flags = 0;
    UINT32 ui4_version = 0;
    UINT32 ui4_major_ver;
    UINT32 ui4_minor_ver;
    UINT32 ui4_num_rpr_size = 0;
    BOOL fgIsRv9 = TRUE;

    if (!prVdec)
    {
        LOG(0, "%s(%d): prVdec is null\n", __FUNCTION__, __LINE__);
        ASSERT(prVdec);
        return;
    }

    if (!prBytesInfo)
    {
        LOG(0, "%s(%d): prBytesInfo is null\n", __FUNCTION__, __LINE__);
        ASSERT(prBytesInfo);
        return;
    }

// TODO: please rv owner review this
#ifdef CC_SUPPORT_TV_STAGEFRIGHT  // new OMX
    UNUSED(ui4_major_ver);
    UNUSED(ui4_minor_ver);
    pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    VDEC_LOADL_DWRD(pu1Data + 4, ui4_spo_extra_flags);
    ASSERT(ui4_spo_extra_flags == 0x5649444F);  // 4cc: "VIDO"
    VDEC_LOADL_DWRD(pu1Data + 8, ui4_version);

    if (ui4_version == 0x52563430)  // 4cc: "RV40"
    {
        UINT16 u2Data;

        fgIsRv9 = TRUE;
        //ui4_num_rpr_size = 0;  already 0

        // reload width and height from the opaque data since it is more precise
        VDEC_LOADL_WORD(pu1Data + 12, u2Data);
        prVdec->u4Width = u2Data;
        VDEC_LOADL_WORD(pu1Data + 14, u2Data);
        prVdec->u4Height = u2Data;
    }
    else
    {
        fgIsRv9 = FALSE;
        ASSERT(0);  // not yet, helix player doesn't support rv8
        goto PRS_OPAQUE_RTN_FAIL;
    }
#else  // old OMX
    pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    VDEC_LOADB_DWRD(pu1Data, ui4_spo_extra_flags);
    VDEC_LOADB_DWRD(pu1Data + 4, ui4_version);

    ui4_major_ver = HX_GET_MAJOR_VERSION(ui4_version);
    ui4_minor_ver = HX_GET_MINOR_VERSION(ui4_version);

    prVdec->ePushFmt = VDEC_PUSH_FMT_RV40;
    if (!(ui4_minor_ver & 0x80))
    {
        if (ui4_major_ver == RV30_MAJOR_BITSTREAM_VERSION
            && ui4_minor_ver == RV30_BITSTREAM_VERSION)
        {
            UINT8* pbBasePtr = pu1Data + 8;
            UINT32 i;

            prVdec->ePushFmt = VDEC_PUSH_FMT_RV30;
            fgIsRv9 = FALSE;
            prVdec->au4RprSizeArray[0] = prVdec->u4Width;
            prVdec->au4RprSizeArray[1] = prVdec->u4Height;
            ui4_num_rpr_size = (ui4_spo_extra_flags >> 16) & 7;
            if (ui4_num_rpr_size < 8)
            {
                if (prBytesInfo->u4BytesSize >= (ui4_num_rpr_size * 2 + 8))
                {
                    for (i = 0; i < ui4_num_rpr_size; i++)
                    {
                        prVdec->au4RprSizeArray[2 * i + 2] = (UINT32)(*pbBasePtr << 2);
                        pbBasePtr++;
                        prVdec->au4RprSizeArray[2 * i + 3] = (UINT32)(*pbBasePtr << 2);
                        pbBasePtr++;
                    }
                }
                else
                {
                    LOG(0, "%s(%d): opaque data(%d) is too small!! rpr size(%d)\n", 
                        __FUNCTION__, __LINE__, prBytesInfo->u4BytesSize, ui4_num_rpr_size);
                    goto PRS_OPAQUE_RTN_FAIL;
                }
            }
        }
        else if (ui4_major_ver == RV40_MAJOR_BITSTREAM_VERSION
            && ui4_minor_ver == RV40_BITSTREAM_VERSION)
        {
            fgIsRv9 = TRUE;
        }
        else
        {
            LOG(0, "%s(%d): ui4_minor_ver(0x%08x) and ui4_minor_ver(0x%08x) are incorrect\n",
                __FUNCTION__, __LINE__, ui4_major_ver, ui4_minor_ver);
            goto PRS_OPAQUE_RTN_FAIL;
        }
    }
    else
    {
        LOG(0, "%s(%d): ui4_minor_ver(0x%08x) is incorrect\n", __FUNCTION__, __LINE__, ui4_minor_ver);
        goto PRS_OPAQUE_RTN_FAIL;
    }

    if (!fgIsRv9 && ui4_num_rpr_size > 0)  // It will be 0 for RV9.  For RV8, add the count of native size.
    {
        ui4_num_rpr_size++;
    }
#endif  // end else #if 1 (new/old OMX)

    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_RV_RPR_INFO, ui4_num_rpr_size, (UINT32)prVdec->au4RprSizeArray, fgIsRv9);
#ifdef CC_SUPPORT_TV_STAGEFRIGHT // TODO: please rv owner review this
    _VPUSH_RvConfig(prVdec, fgIsRv9);
#endif
    return;
PRS_OPAQUE_RTN_FAIL:
    _VPUSH_VDEC_Nfy((void *)prVdec, VDEC_COND_DECODE_STATUS_CHG, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT, 0);
}

#if 0
#ifdef CC_SUPPORT_TV_STAGEFRIGHT
BOOL _VPUSH_RvPreparse(VDEC_BYTES_INTO_T *prBytesInfo)
{
    UINT8 *pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    UINT32 *pu4Data = (UINT32 *)pu1Data;
    UINT32 u4BytesSize = prBytesInfo->u4BytesSize;
    UINT32 u4PicSz;
    UINT32 u4SliceNum;
    UINT32 u4HeaderSz;
    UINT32 i, j, u4SliceOfst, u4NextSliceOfst;

    ASSERT(((UINT32)pu1Data) % 4 == 0);  // make sure the alignment for UINT32 pointer access later

    VDEC_LOADB_DWRD(pu1Data, u4PicSz);
    VDEC_LOADB_DWRD(pu1Data + 16, u4SliceNum);

    u4HeaderSz = u4SliceNum * 8 + 20;
    if (u4BytesSize <= u4HeaderSz)
    {
        LOG(0, "%s(%d): %d slice, push data size(%d) is too small!!\n",
            __FUNCTION__, __LINE__, u4SliceNum, u4BytesSize);
        ASSERT(0);
        return FALSE;
    }

    pu1Data[0] = (UINT8)u4SliceNum;
    for (i = 24, j = 1; j < u4SliceNum; i += 8, j++)
    {
        VDEC_LOADB_DWRD(pu1Data + i, u4SliceOfst);
        VDEC_LOADB_DWRD(pu1Data + i + 8, u4NextSliceOfst);
        pu4Data[j] = u4NextSliceOfst - u4SliceOfst;
    }

    pu4Data[u4SliceNum] = u4PicSz - u4NextSliceOfst;

    return TRUE;
}
#else  // old OMX
/*
    ffmpeg RM data header format
    1byte: slice number - 1 (should be 0~126)
    slice number * 8byte(1, 0, 0, 0, 4b_slice_ofst_le)

    TODO: clarify what does 1, 0, 0, 0 mean
*/
BOOL _VPUSH_RvPreparse(VDEC_BYTES_INTO_T *prBytesInfo)
{
    UINT8 *pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
    UINT32 *pu4Data = (UINT32 *)pu1Data;
    UINT32 u4SliceNum = (UINT32)*pu1Data + 1;
    UINT32 u4BytesSize = prBytesInfo->u4BytesSize;
    UINT32 i, j, u4SliceOfst = 0, u4NextSliceOfst = 0, u4HeaderSz;

    u4HeaderSz = u4SliceNum * 8 + 1;
    if (u4BytesSize <= u4HeaderSz)
    {
        LOG(0, "%s(%d): %d slice, push data size(%d) is too small!!\n",
            __FUNCTION__, __LINE__, u4SliceNum, u4BytesSize);
        ASSERT(0);
        return FALSE;
    }

    for (i = 5, j = 1; j < u4SliceNum; i += 8, j++)
    {
        VDEC_LOADL_DWRD(&pu1Data[i], u4SliceOfst);
        VDEC_LOADL_DWRD(&pu1Data[i + 8], u4NextSliceOfst);
        pu4Data[j] = u4NextSliceOfst - u4SliceOfst;
    }
    pu1Data[0] = (UINT8)u4SliceNum;

    if (u4BytesSize <= (u4NextSliceOfst + u4HeaderSz))
    {
        LOG(0, "%s(%d): push data size(%d) is too small!!\n",
            __FUNCTION__, __LINE__, u4BytesSize);
        ASSERT(0);
        return FALSE;
    }
    pu4Data[u4SliceNum] = u4BytesSize - u4NextSliceOfst - u4HeaderSz;

    return TRUE;
}
#endif  // end else #if 1 (new/old OMX)

#endif

// #VPUSH_RV_SLICE_BUF#
/*
    ffmpeg RM data header format
    1byte: slice number - 1 (should be 0~126)
    slice number * 8byte(1, 0, 0, 0, 4b_slice_ofst_le)

    TODO: clarify what does 1, 0, 0, 0 mean
*/
BOOL _VPUSH_RvPreparse1( VDEC_T *prVdec , VDEC_BYTES_INTO_T *prBytesInfo)
{
    UINT32 u4BytesSize = prBytesInfo->u4BytesSize;
    UCHAR ucEsId = prVdec->ucVdecId ;

    if (!(prBytesInfo->fgAppend))
    {
        UINT8 * pu1Data = (UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr);
        UINT32 u4SliceNum = (UINT32)*pu1Data + 1;
        UINT32 i, j, u4SliceOfst = 0, u4NextSliceOfst = 0, u4HeaderSz;

        u4HeaderSz = u4SliceNum * 8 + 1;

        if (u4BytesSize <= u4HeaderSz)
        {
            LOG(0, "%s(%d): %d slice, push data size(%d) is too small!!\n",
                __FUNCTION__, __LINE__, u4SliceNum, u4BytesSize);
            ASSERT(0);
            return FALSE;
        }

        prVdec->u1CrntFrmSliceNum = u4SliceNum ;
        prVdec->dwCrntFrmSliceSzWp = prVdec->dwVPSliceSzBufWp ;

#define VDEC_MULTI_USE(v)    v[ucEsId]

        if (VDEC_MULTI_USE(_pdwVPSliceBuf) == NULL)
        {
            VDEC_MULTI_USE(_pdwVPSliceBuf) = (UINT32 *)x_mem_alloc(4*RM_V_PUSH_SLICE_SZ_BUF_NS);
        }

        prVdec->pu4VPSliceSzBuf = VDEC_MULTI_USE(_pdwVPSliceBuf) ;

        if (prVdec->pu4VPSliceSzBuf  == NULL)
        {
            LOG(0,"VPUSH allocate slice buf (20k) failed!");
            ASSERT(0);
        }

        for (i = 5, j = 1; j < u4SliceNum; i += 8, j++)
        {
            VDEC_LOADL_DWRD(&pu1Data[i], u4SliceOfst);
            VDEC_LOADL_DWRD(&pu1Data[i + 8], u4NextSliceOfst);

            if (j == 1)
            {
                LOG(2,"Slice Buf Addr = 0x%x,Wp = %d, ",&(prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWp]),prVdec->dwVPSliceSzBufWp);
            }

            prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWp] =  u4NextSliceOfst - u4SliceOfst;

            if (j == 1)
                LOG(2,"Slice[0] = %d\n",prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWp]);

            if (++prVdec->dwVPSliceSzBufWp == RM_V_PUSH_SLICE_SZ_BUF_NS)
            {
                prVdec->dwVPSliceSzBufWp = 0;
            }
        }

//fill the first 4 byte for rvaddpic useage.
        VDEC_LOADB_DWRD(&pu1Data[u4SliceNum*8+1], prVdec->dwFirst4CC);
        LOG(1,"B:dwFirst4CC = 0x%x\n", prVdec->dwFirst4CC);

// since a frame can be sent by 2 or more times...
        if (u4BytesSize <= (u4NextSliceOfst + u4HeaderSz))
        {
            //liuqu add for debug.20120531
            LOG(1,"u4NextSliceOfst = %d,u4SliceNum = %d\n",u4NextSliceOfst,u4SliceNum);
        }

        // This value might be less than ZERO
        prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWp] = u4BytesSize - u4NextSliceOfst - u4HeaderSz;
        prVdec->dwVPSliceSzBufWpBak = prVdec->dwVPSliceSzBufWp;

        if (++prVdec->dwVPSliceSzBufWp == RM_V_PUSH_SLICE_SZ_BUF_NS)
        {
            prVdec->dwVPSliceSzBufWp = 0;
        }

    }
// this is not the 1st part of a frame!
    else
    {
        prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWpBak] += u4BytesSize;

        LOG(1,"fgAppend = TRUE!u4BytesSize = %d,Now Last Slice Sz= %d\n",u4BytesSize,
            prVdec->pu4VPSliceSzBuf[prVdec->dwVPSliceSzBufWpBak]);
    }

    return TRUE;
}

void _VPUSH_H264CfgScaleAndRate(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
	VDEC_PTS_INFO_T rPTSInfo;
	
	if(prVdec->u4Rate >= 10000)
	{
		prVdec->fgVariableFrameRate = TRUE;
		prVdec->fgDecoderCalPts = FALSE;
	}
	else
	{
		switch(prVdec->u4Rate)
		{
			case 2397 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 100000;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*100000*1000)/(23976);
			#endif
				rPTSInfo.u4Rate = 100000;
				rPTSInfo.u8Scale = 375374700;
				break;
			case 2400:
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 1;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*1*1000)/(24000);
			#endif
				rPTSInfo.u4Rate = 1;
				rPTSInfo.u8Scale = 3750;
				break;
			case 2500 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 1;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*1*1000)/(25000);
			#endif
				rPTSInfo.u4Rate = 1;
				rPTSInfo.u8Scale = 3600;
				break;
			case 2997 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 100000;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*100000*1000)/(29970);
			#endif
				rPTSInfo.u4Rate = 100000;
				rPTSInfo.u8Scale = 300300300;
				break;
			case 3000:
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 1;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*1*1000)/(30000);
			#endif
				rPTSInfo.u4Rate = 1;
				rPTSInfo.u8Scale = 3000;
				break;
			case 5000 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 1;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*1*1000)/(50000);
			#endif
				rPTSInfo.u4Rate = 1;
				rPTSInfo.u8Scale = 1800;
				break;
			case 5994 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 100000;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*100000*1000)/(59940);
			#endif
				rPTSInfo.u4Rate = 100000;
				rPTSInfo.u8Scale = 150075037;
				break;
							
			case 6000 :
			#ifdef VDEC_PUSH_PTS_64_BITS
				rPTSInfo.u4Rate64 = 1;
				rPTSInfo.u8Scale64 = (VDEC_U8_SECOND_CLOCK*1*1000)/(60000);
			#endif
				rPTSInfo.u4Rate = 1;
				rPTSInfo.u8Scale = 1500;
				break;
			default :
				return;		  
		}
		LOG(1,"VPUSH Cfg Rate = %lx pts(%lx)\n",prVdec->u4Rate,(UINT32)prBytesInfo->u8BytesPTS);
		VDEC_StartPTS(prVdec->ucVdecId, TRUE, (UINT32)prBytesInfo->u8BytesPTS, &rPTSInfo);
		prVdec->fgVariableFrameRate = FALSE;
	}

	return;
}


VOID _VPUSH_Mpeg4Config(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
#define M4V_START_CODE			0x000001	
#define H263_VIDEO_START_MASK            	0xfffffc00
#define H263_VIDEO_START_CODE          	0x00008000
#define FLV_VIDEO_START_CODE   0x00008400

    UINT32 u4Temp;
    VDEC_LOADB_DWRD((UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr), u4Temp);

    if ((u4Temp >> 8) == M4V_START_CODE)
    {
        LOG(0, "FMT_MP4\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_MP4);
        _VPUSH_SetDmx(prVdec, TRUE);
        if (prVdec->fgDecoderCalPts)
        {
            if(prVdec->u4Rate >= 10000)
            {
                prVdec->fgVariableFrameRate = TRUE;
                prVdec->fgDecoderCalPts = FALSE;
            }
            else
            {
                VDEC_PTS_INFO_T rPTSInfo;
                #ifdef VDEC_PUSH_PTS_64_BITS
                rPTSInfo.u8Scale = 100 * VDEC_U8_SECOND_CLOCK;
                rPTSInfo.u8Scale64 = 100 * VDEC_U8_SECOND_CLOCK;
                #else
                rPTSInfo.u8Scale = 100 * 90000;
                rPTSInfo.u8Scale64 = 100 * 90000;
                #endif
                rPTSInfo.u4Rate = prVdec->u4Rate;
                rPTSInfo.u4Rate64 = prVdec->u4Rate;
                VDEC_StartPTS(prVdec->ucVdecId, TRUE, 0, &rPTSInfo);
                prVdec->fgVariableFrameRate = FALSE;
            }
        }
    }
    else if ((u4Temp  & H263_VIDEO_START_MASK) == H263_VIDEO_START_CODE)
    {
        LOG(0, "FMT_H263\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_H263);
        _VPUSH_SetDmx(prVdec, FALSE);
    }
    else if ((u4Temp  & H263_VIDEO_START_MASK) == FLV_VIDEO_START_CODE)
    {
        LOG(0, "FMT_SORENSEN\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_SORENSEN);
        _VPUSH_SetDmx(prVdec, FALSE);
    }
    // Because the biCompression is not fixed for DivX311
    // we can't use any start code to specify the video type for DivX311.
    else //if (u4Temp == DX311_VIDEO_START_CODE)
    {
        LOG(0, "FMT_DIVX311\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_DIVX311);
        _VPUSH_SetDmx(prVdec, FALSE);
    }
}


#ifdef CC_SUPPORT_TV_STAGEFRIGHT
static VOID _VPUSH_WmvConfig(VDEC_T *prVdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    asf_bitmapinfoheader_s *pASF_BitHeader = NULL;
    pASF_BitHeader = (asf_bitmapinfoheader_s *)prBytesInfo->u4BytesAddr;
    if (!strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "WMV9", 4) 
        || !strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "wmv9", 4) 
        || !strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "WMV3", 4)
        || !strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "wmv3", 4))
    {
        //VDEC_ES_INFO_T *prVdecEsInfo;
        LOG(0, "FMT_WMV9\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_WMV9);
        _VPUSH_SetDmx(prVdec, FALSE);
    }
    else if (!strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "WVC1", 4) || !strncmp((UCHAR *)(&pASF_BitHeader->biCompression), "wvc1", 4))
    {
        //VDEC_ES_INFO_T *prVdecEsInfo;
        LOG(0, "FMT_VC1\n");
        _VPUSH_SetPushType(prVdec, VDEC_PUSH_FMT_VC1);
        _VPUSH_SetDmx(prVdec, TRUE);
    }
    else
    {
        ASSERT(0);  // invalid
    }
}
#endif


/*
 * mkv demuxer will insert sequence header before each block
 * but a MPEG picture might be divided into multiple block
 * This behavior will cause vfifo has improper data
 */ 
//#define MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
#ifdef MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
#define MPEG_SEQ_LEN (144) // max mpeg1/2 seq len is 140
UINT8 _u1MpegSeq[MPEG_SEQ_LEN];
UINT32 _u4MpegSeqLen;
#error "move above variable to VDEC_T for multi-instance support"
#endif

static void _VPUSH_FlushEsmQ(UCHAR ucEsId)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_PES_INFO_T _rPesInfo; 

    if(ucEsId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "ucEsId >= VDEC_PUSH_MAX_DECODER\n");
        ASSERT(ucEsId < VDEC_PUSH_MAX_DECODER);
        ucEsId = (VDEC_PUSH_MAX_DECODER-1);
    }

    prVdecEsInfo = _VDEC_GetEsInfo(ucEsId);
    if(!prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return;
    }

    LOG(6, "Vpush Flush Es for changing channel %d\n",ucEsId);
    _VDEC_LockFlushMutex(ucEsId);
    prVdecEsInfo->fgFlushEsmQ = TRUE;

     _VDEC_UnlockFlushMutex(ucEsId);

    _VDEC_SendCmd(ucEsId, ((UINT32)VDEC_CMD_FLUSH_ESMQ));

    x_memset(&_rPesInfo, 0, sizeof(_rPesInfo));
    // Virtual Picture Header for Event Trigger
    _rPesInfo.ucEsId = ucEsId;
    //_arMpvPesInfo[ucEsId].ucMpvId = VDEC_MAX_VLD;
    // should VDEC_MAX_VLD
    _rPesInfo.ucMpvId = VDEC_MAX_VLD;

    VERIFY(VDEC_SendEs(&_rPesInfo) == 1);

    // lock until flush done in main loop
    _VDEC_LockFlushSema(ucEsId);

    LOG(6, "Vpush Flush Es finish\n");
}

static BOOL  _VPUSH_DmxMoveSecureData(VDEC_T *prVdec,const DMX_MM_DRM_AES_T *prAesSetting,DMX_MM_DATA_T* pDmxMMData,BOOL fgMove)
{
    //DMX_MM_SetAES(prVdec->u1DmxId,DMX_MM_DRM_FLAG_ALL,&rAesSetting);

	if(!DMX_MM_SetDRMMode(prVdec->u1DmxPid,DMX_DRM_MODE_ND))
	{
	   return FALSE;
	}
    if(!DMX_MM_SetSecureAES(prVdec->u1DmxPid,(UINT32)DMX_MM_DRM_FLAG_ALL,prAesSetting,TRUE))
    {
       LOG(2,"VPush:DMX_MM_SetSecureAES set error..........\n");
       return FALSE;
    }

    if(fgMove)
    {
        if(!_VPUSH_DmxMoveData(prVdec, pDmxMMData))
        {
            LOG(2,"VPush:_VPUSH_DmxMoveData set error..........\n");
            return FALSE;
        }
    }
    return TRUE;
}

static VOID  _VPUSH_DmxMoveDataDecryptProcess(VDEC_T *prVdec,
	DMX_MM_DATA_T *pDmxMMData,VDEC_BYTES_INTO_T *prBytesInfo)
{
    VDEC_DECRYPT_INFO_T *pDecryptInfo=&prVdec->rDecryptInfo;
	DMX_MM_DRM_AES_T rAesSetting;
	UINT32 u4CopySize,u4EmptySize,u4EcryptAddr,u4MoveSize;
    x_memset(pDmxMMData, 0, sizeof(DMX_MM_DATA_T));
	x_memset(&rAesSetting,0,sizeof(DMX_MM_DRM_AES_T));
	pDmxMMData->u1Idx = prVdec->u1DmxPid;
	pDmxMMData->u4BufStart = (UINT32)pDecryptInfo->arTempUnitBuf;
	pDmxMMData->u4BufEnd = (UINT32)pDecryptInfo->arTempUnitBuf+VDEC_DECRYPT_BUFLEN;
	pDmxMMData->fgEOS = FALSE;
#ifdef VDEC_PUSH_PTS_64_BITS
	pDmxMMData->u4Pts = (UINT32)(prBytesInfo->u8BytesPTS&(UINT64)0xFFFFFFFF);
	pDmxMMData->u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
#else
	pDmxMMData->u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
	pDmxMMData->u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
#endif
	rAesSetting.fgCBC=FALSE;
	rAesSetting.u4KeyBitLen=128;
	rAesSetting.fgEncrypt=FALSE;
	
	if(prBytesInfo->u4EncryptLen>0)
	{
	   if(pDecryptInfo->arTempUnitBuf==NULL)
       {
           pDecryptInfo->arTempUnitBuf = (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(VDEC_DECRYPT_BUFLEN,1024));
           if(pDecryptInfo->arTempUnitBuf==NULL)
           {
              LOG(0,"_VPUSH_DmxMoveDataDecryptProcess encrypt buffer malloc faile\n");
              return;
           }
           else
           {
               pDmxMMData->u4BufStart = (UINT32)pDecryptInfo->arTempUnitBuf;
               pDmxMMData->u4BufEnd = (UINT32)pDecryptInfo->arTempUnitBuf+VDEC_DECRYPT_BUFLEN;
           }
       }
	   u4EcryptAddr=prBytesInfo->u4BytesAddr+prBytesInfo->u4EncryptOffset;
	   if(prBytesInfo->u4EncryptOffset!=0)
	   {
          pDecryptInfo->fgReseted=TRUE;
	   }
	   else
	   {
		  pDecryptInfo->fgReseted=FALSE;
	   }
	   
       if(pDecryptInfo->u4TempBufWp!=0)
       {
           if(pDecryptInfo->fgReseted)
           {
			   rAesSetting.u4Offset=0;
			   rAesSetting.u4EncryLen=pDecryptInfo->u4TempBufWp;
			   pDmxMMData->u4StartAddr = (UINT32)pDecryptInfo->arTempUnitBuf;
			   pDmxMMData->u4FrameSize = pDecryptInfo->u4TempBufWp;
			   pDmxMMData->fgTrigger=FALSE;
               _VPUSH_DmxMoveSecureData(prVdec,&rAesSetting,pDmxMMData,TRUE);
			   pDecryptInfo->u4TempBufWp=0;
           }
		   else
		   {
			   u4EmptySize=VDEC_DECRYPT_UNIT_LEN-pDecryptInfo->u4TempBufWp;
			   u4CopySize=u4EmptySize>prBytesInfo->u4EncryptLen ? prBytesInfo->u4EncryptLen : u4EmptySize;
			   x_memcpy(pDecryptInfo->arTempUnitBuf+pDecryptInfo->u4TempBufWp,(void*)u4EcryptAddr,u4CopySize);
			   pDecryptInfo->u4TempBufWp+=u4CopySize;
			   //prBytesInfo->u4EncryptOffset+=u4CopySize;
			   prBytesInfo->u4EncryptLen-=u4CopySize;
               prBytesInfo->u4BytesSize-=u4CopySize;
			   prBytesInfo->u4BytesAddr+=u4CopySize;
			   if(pDecryptInfo->u4TempBufWp==VDEC_DECRYPT_UNIT_LEN 
			   	|| (prBytesInfo->u4EncryptLen==0&&prBytesInfo->u4BytesSize!=0))
			   {
			   
			   	  rAesSetting.u4Offset=0;
			      rAesSetting.u4EncryLen=pDecryptInfo->u4TempBufWp;
				  pDmxMMData->u4StartAddr = (UINT32)pDecryptInfo->arTempUnitBuf;
				  pDmxMMData->u4FrameSize = pDecryptInfo->u4TempBufWp;
				  pDmxMMData->fgTrigger=FALSE;
                  _VPUSH_DmxMoveSecureData(prVdec,&rAesSetting,pDmxMMData,TRUE);
				  pDecryptInfo->u4TempBufWp=0;
			   }
			   
			   if(prBytesInfo->u4BytesSize==0)
			   {
                   return ;
			   }
		   }
       	}

        if(prBytesInfo->fgEncryptReset==FALSE 
			&&prBytesInfo->u4EncryptLen!=0
			&&(prBytesInfo->u4EncryptLen+prBytesInfo->u4EncryptOffset==prBytesInfo->u4BytesSize))
        {
            u4CopySize=prBytesInfo->u4EncryptLen%VDEC_DECRYPT_UNIT_LEN;
			if(u4CopySize)
			{
			    prBytesInfo->u4EncryptLen-=u4CopySize;
				prBytesInfo->u4BytesSize-=u4CopySize;
				u4EcryptAddr=prBytesInfo->u4BytesAddr+prBytesInfo->u4EncryptOffset+prBytesInfo->u4EncryptLen;
				x_memcpy(pDecryptInfo->arTempUnitBuf+pDecryptInfo->u4TempBufWp,(void*)u4EcryptAddr,u4CopySize);
				pDecryptInfo->u4TempBufWp+=u4CopySize;
			}
        }
        else if(prBytesInfo->u4EncryptLen!=0&&(prBytesInfo->u4EncryptLen+prBytesInfo->u4EncryptOffset<prBytesInfo->u4BytesSize))
        {
			rAesSetting.u4Offset=prBytesInfo->u4EncryptOffset;
			rAesSetting.u4EncryLen=prBytesInfo->u4EncryptLen;
			pDmxMMData->u4BufStart = prVdec->u4FifoStart;
			pDmxMMData->u4BufEnd = prVdec->u4FifoEnd;
			pDmxMMData->u4StartAddr = prBytesInfo->u4BytesAddr;
			pDmxMMData->u4FrameSize = prBytesInfo->u4EncryptLen+prBytesInfo->u4EncryptOffset;
			// pDmxMMData->fgTrigger=TRUE;
			u4MoveSize=pDmxMMData->u4FrameSize;
			_VPUSH_DmxMoveSecureData(prVdec,&rAesSetting,pDmxMMData,TRUE);

			prBytesInfo->u4EncryptLen=0;
			prBytesInfo->u4EncryptOffset=0;
			prBytesInfo->u4BytesAddr+=u4MoveSize;
			prBytesInfo->u4BytesSize-=u4MoveSize;
			pDecryptInfo->fgReseted=TRUE;
        }
		
        if(prBytesInfo->u4BytesSize==0)
        {
            return;
        }

        rAesSetting.u4Offset=prBytesInfo->u4EncryptOffset;
        rAesSetting.u4EncryLen=prBytesInfo->u4EncryptLen;
	}
	else
	{
		pDecryptInfo->fgReseted=TRUE;
		if(pDecryptInfo->u4TempBufWp!=0)
		{
			rAesSetting.u4Offset=0;
			rAesSetting.u4EncryLen=pDecryptInfo->u4TempBufWp;
			pDmxMMData->u4StartAddr = (UINT32)pDecryptInfo->arTempUnitBuf;
			pDmxMMData->u4FrameSize = pDecryptInfo->u4TempBufWp;
	        pDmxMMData->fgTrigger=FALSE;
            _VPUSH_DmxMoveSecureData(prVdec,&rAesSetting,pDmxMMData,TRUE);
			pDecryptInfo->u4TempBufWp=0;
		}
        rAesSetting.u4Offset=0;
        rAesSetting.u4EncryLen=0;
	}

	_VPUSH_DmxMoveSecureData(prVdec,&rAesSetting,NULL,FALSE);
	pDmxMMData->fgTrigger=FALSE;
	if(prBytesInfo->u4EncryptLen==0)
	{
		if(!DMX_MM_SetDRMMode(prVdec->u1DmxPid,DMX_DRM_MODE_BYPASS))
		{
		   return;
		}
		
	}
	else
	{
        if(pDecryptInfo->fgReseted)
        {
			pDmxMMData->fgTrigger=TRUE;
        }
	}
	return;
}

static BOOL _VPUSH_MoveData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    DMX_MM_DATA_T rDmxMMData;
    VDEC_T *prVdec;
    BOOL fgRet;
    UCHAR *ptr;
    INT32 MoveSpecialDataRet = MOVE_SPECIAL_DATA_CONTINUE;
	
#ifdef ENABLE_MULTIMEDIA
	VDEC_ES_INFO_T *prVdecEsInfo; 
#endif
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(!prBytesInfo)
    {
        LOG(3, "%s(%d): prBytesInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(prBytesInfo->fgEos)
    {
        //DMX_MM_T rDmxMM;
        //x_memset(&rDmxMM, 0, sizeof(DMX_MM_T));
        //rDmxMM.fgSearchStartCode = FALSE;
        //fgRet = DMX_MM_Set(prVdec->u1DmxPid, DMX_MM_FLAG_SEARCH_START_CODE, &rDmxMM);
        LOG(5, "vpush got fgEos\n");
        prVdec->fgGotEos = TRUE;
    }
	
#ifdef ENABLE_MULTIMEDIA
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
    }
#endif
    // last PTS also need to be re-calculate
    //else
    // remove 1000000 to 90000 clock translate
    #if !defined(VDEC_PUSH_PTS_64_BITS)
    {
        UINT64 u8Tmp;
        UINT64 u8Remainder;

        u8Tmp = (UINT64)prBytesInfo->u8BytesPTS;
        prBytesInfo->u8BytesPTS = (UINT32)u8Div6432((u8Tmp*9), 100, &u8Remainder);

        //tmp solution for magic pts in PS/TS format
        if(prBytesInfo->u8BytesPTS == 114417928)
        {
            prBytesInfo->u8BytesPTS = 0;
        }

    }
    #endif
    LOG(9, "vpush: size=%d, addr=0x%08x, pts=0x%llx\n", prBytesInfo->u4BytesSize, prBytesInfo->u4BytesAddr, prBytesInfo->u8BytesPTS);

    if(prVdec->fgNonFirst==FALSE && prVdec->fgIsSecureInput)
    {
        LOG(0,"First secure data, skip.");
        prVdec->fgNonFirst = TRUE;
        return TRUE;
    }
    
    if(prVdec->fgIsSecureInput)
	{
        if(prVdec->fgNonFirst==FALSE && prVdec->fgGstPlay)
        {
            LOG(0,"First secure data,skip.");
            prVdec->fgNonFirst = TRUE;
            return TRUE;
        }
        else 
        {
            LOG(5,"_VPUSH_MoveData SecureData:(Addr=0x%x, 0x%x) Size=0x%x\n",\
                prBytesInfo->u4BytesAddr,VIRTUAL(prBytesInfo->u4BytesAddr),prBytesInfo->u4BytesSize);
            
            prBytesInfo->u4BytesAddr=VIRTUAL(prBytesInfo->u4BytesAddr);
        }
	}

    prVdec->fgPacketAppend = prBytesInfo->fgAppend;
    prVdec->u4PacketSize = prBytesInfo->u4BytesSize;
    
    if(prVdec->rDecryptInfo.fgDecryptPlayback)
    {
        _VPUSH_DmxMoveDataDecryptProcess(prVdec,&rDmxMMData,prBytesInfo);
        if(prBytesInfo->u4BytesSize==0&&prBytesInfo->fgEos==FALSE)
        {
            return TRUE;
        }
    }
    else
    {
        x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    }

#ifdef CC_SUPPORT_TV_STAGEFRIGHT
/*
Fixme Use the prVdec->fgNonFirst at the condition. 
Move all the process of prVdec->fgNonFirst to one Funciton. 
The best way is provide a function pointer to Decoder, 
The Decode should register the funciton pointer, and implement the fucntion to Process the first data. 
*/
    if (VDEC_FMT_WMV == prVdec->eFmt && VDEC_PUSH_FMT_UNUSED == prVdec->ePushFmt)
    {
        ASSERT(!prVdec->fgNonFirst);
        _VPUSH_WmvConfig(prVdec, prBytesInfo);
    }
#endif

    if (VDEC_FMT_H264 == prVdec->eFmt)
    {
        ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
        if (prVdec->fgIsSecureInput==FALSE && (!prVdec->fgNonFirst) && (!prVdec->fgInsertStartcode))
        {
            if(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
               ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01)))
            {
            }
            else
			{
#ifdef ENABLE_MULTIMEDIA
			   if((prVdecEsInfo) && (prVdecEsInfo->eContainerType == SWDMX_FMT_MPEG2_TS))
			   {
				   LOG(1,"Ts file has no StartCode\n");
				   prVdec->fgInsertStartcode = FALSE;
			   }
			   else
#endif
			   {
				   prVdec->fgInsertStartcode = TRUE;
			   }
		    }
        }

        if(prVdec->fgDecoderCalPts && (!prVdec->fgNonFirst))
        {
            _VPUSH_H264CfgScaleAndRate(prVdec, prBytesInfo);
        }

        if(prVdec->fgInsertStartcode)
        {
            if (!prVdec->fgNonFirst)
            {
                prVdec->fgNonFirst = TRUE;
                return _VPUSH_Move264FirstData(prVdec, prBytesInfo);
            }
            else
            {
                return _VPUSH_Move264Data(prVdec, prBytesInfo);
            }
        }
		else if(prVdec->fgIsSecureInput==FALSE)
		{
			if(!(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
				   ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01))))
			{
				_VPUSH_Move264StartCode(prVdec, prBytesInfo);
			}
		}
    }
    else if(prVdec->eFmt == VDEC_FMT_H265)
    {
        ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
        if (prVdec->fgIsSecureInput==FALSE && (!prVdec->fgNonFirst) && (!prVdec->fgInsertStartcode))
        {
            if(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
               ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01)))
            {
                //found start code
                LOG(3, "hevc start code\n");
            }
            else
            {
                prVdec->fgInsertStartcode = TRUE;
            }
        }

        if(prVdec->fgInsertStartcode)
        {
            if (!prVdec->fgNonFirst)
            {
                prVdec->fgNonFirst = TRUE;
                return _VPUSH_Move265FirstData(prVdec, prBytesInfo);
            }
            else
            {
                return _VPUSH_Move265Data(prVdec, prBytesInfo);
            }
        }
        else if(prVdec->fgIsSecureInput==FALSE)
        {
            if(!(((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
                   ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01))))
            {
                _VPUSH_Move264StartCode(prVdec, prBytesInfo);
            }
        }

    }
    else if(prVdec->eFmt == VDEC_FMT_WMV && prVdec->ePushFmt == VDEC_PUSH_FMT_VC1)
    {
        if (!prVdec->fgNonFirst)
        {
            // Identify ASF-WVC1 format by ASF_Binding byte.
#ifdef CC_SUPPORT_TV_STAGEFRIGHT  // TODO: please wmv owner review this
            ptr = (UCHAR*)(prBytesInfo->u4BytesAddr + sizeof(asf_bitmapinfoheader_s));

            if (((ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01) && (ptr[4] == 0x0F)) ||
                ((ptr[2] == 0x00) && (ptr[3] == 0x00) && (ptr[4] == 0x01) && (ptr[5] == 0x0F)))
            {
                LOG(3, "%s(%d): ASF + WVC1, need insert start code\n", __FUNCTION__, __LINE__);
                /*
                   megaa: Start code insertion is done in ASF extractor now.
                          If we want to do the insertion here, the bug that redundant sequence header
                          and entry point header entrance with fgNonFirst flag cleared should be fixed.
                */
#if 0
                prVdec->fgInsertStartcode = TRUE;
#endif
#if 1  // megaa: BYTE 0 is not ASF_Binding byte now!!!
                if (ptr[5] & 0x01) // ASF_Binding byte, bit0 must be 1'b1
#endif
                {
                    LOG(3, "%s(%d): Setup binding byte to WMV decoder : %02x\n", __FUNCTION__, __LINE__, ptr[5]);
                    VDEC_SetParam(prVdec->ucVdecId, 18, (UINT32)(&(ptr[5])), 0, 0);
                }
            }
#else
            ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
            if (((ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01) && (ptr[4] == 0x0F)) ||
                ((ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x00) && (ptr[4] == 0x01) && (ptr[5] == 0x0F)))
            {
                if (ptr[0] & 0x01) // ASF_Binding byte, bit0 must be 1'b1
                {
                    LOG(3, "%s(%d): ASF + WVC1, need insert start code\n", __FUNCTION__, __LINE__);
                    prVdec->fgInsertStartcode = TRUE;

                    LOG(3, "%s(%d): Setup binding byte to WMV decoder : %02x\n", __FUNCTION__, __LINE__, ptr[0]);
                    VDEC_SetParam(prVdec->ucVdecId, 18, (UINT32)(&(ptr[0])), 0, 0);
                }
            }
#endif
        }

        if (prVdec->fgInsertStartcode && prVdec->fgNonFirst)
        {
            ptr = (UCHAR*)prBytesInfo->u4BytesAddr;
            if (((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01)) ||
                ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x00) && (ptr[3] == 0x01)))
            {
                // already have start code in BDU, no need to insert start code
            }
			else if (prBytesInfo->fgAppend)
			{
				// append data to previous packet, no need to insert start code
			}
			else if (prBytesInfo->u4BytesSize == 0)
			{
			    LOG(3, "%s(L:%d) u4BytesSize == 0\n", __FUNCTION__, __LINE__);
				// size = 0, we don't need to insert start code
			}
            else
            {
                _VPUSH_MoveVC1StartCode(prVdec, prBytesInfo);
            }
        }
        if (!prVdec->fgNonFirst)
        {
            prVdec->fgNonFirst = TRUE;
        }
    }
    else if (VDEC_FMT_MP4 == prVdec->eFmt)
    {
        prVdec->fgNotCalPTS = FALSE;
        
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            UINT32 u4Temp = 0;
            
            VDEC_LOADB_DWRD((UINT8 *)VIRTUAL(prBytesInfo->u4BytesAddr), u4Temp);
            if (u4Temp == 0)
            {
                LOG(0, "Skip zero startcode data to avoid incorrect decoder config.\n");
                return TRUE;
            }
            
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            _VPUSH_Mpeg4Config(prVdec, prBytesInfo);
            prVdec->fgNonFirst = TRUE;
        }
    }
    else if (VDEC_FMT_RV == prVdec->eFmt)
    {
#ifdef CC_SUPPORT_TV_STAGEFRIGHT  // new OMX
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            _VPUSH_RvOpaqueParse(prVdec, prBytesInfo);
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            //prVdec->fgNonFirst = TRUE;
            //return TRUE;
        }
        if (prVdec->fgNonFirst && !prBytesInfo->fgEos)
        {
            #if 0
            // TODO: please rv owner add new omx flow for _VPUSH_RvPreparse1
            if (!_VPUSH_RvPreparse(prBytesInfo))
            {
                return TRUE;
            }
            #endif
        }
#else  // old OMX
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            _VPUSH_RvOpaqueParse(prVdec, prBytesInfo);
            prVdec->fgNonFirst = TRUE;
            return TRUE;
        }

        if (!prBytesInfo->fgEos)
        {
           // if (!_VPUSH_RvPreparse(prBytesInfo))
            if (!_VPUSH_RvPreparse1(prVdec, prBytesInfo)) // #VPUSH_RV_SLICE_BUF#
            {
                return TRUE;
            }
        }
#endif  // end else #if 1 (new/old OMX)
    }
    else if(prVdec->eFmt==VDEC_FMT_VP8)
    {
        if(prBytesInfo->fgAppend==FALSE)
        {
    		DMX_MM_T rDMXInfo;
    		UINT32 u4EmptyData;
    		x_memset(&rDMXInfo, 0, sizeof(DMX_MM_T));
    		DMX_MM_Get(prVdec->u1DmxPid,DMX_MM_FLAG_BUF,&rDMXInfo);
    		if (rDMXInfo.u4Wp+prBytesInfo->u4BytesSize>rDMXInfo.u4BufAddr+rDMXInfo.u4BufSize)
    		{
    			x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    			rDmxMMData.u1Idx = prVdec->u1DmxPid;
    			rDmxMMData.u4BufStart = prVdec->u4FifoStart;
    			rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
    			rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr;
    			rDmxMMData.u4FrameSize = rDMXInfo.u4BufAddr+rDMXInfo.u4BufSize-rDMXInfo.u4Wp;
    			rDmxMMData.fgEOS = FALSE;
    			rDmxMMData.u4Pts = VDEC_PUSH_VP8_INVALID_PTS;
    			_VPUSH_DmxMoveData(prVdec, &rDmxMMData);
    		}
    		
    	    u4EmptyData = DMX_MUL_GetEmptySize(prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid);
    		if(u4EmptyData<prBytesInfo->u4BytesSize)
    		{
    		   x_thread_delay(500);
    		}
        }
    }
    else if(VDEC_FMT_MJPEG == prVdec->eFmt)
    {
#ifdef CC_SUPPORT_TV_STAGEFRIGHT
        if (!prVdec->fgNonFirst)
        {
            VDEC_SEQUENCE_DATA_T rVDecSeqData;
            x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
            rVDecSeqData.u2_width = (UINT16)prVdec->u4Width;
            rVDecSeqData.u2_height = (UINT16)prVdec->u4Height;
            rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
            VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SEQ_INFO, (UINT32)&rVDecSeqData, 0, 0);
            prVdec->fgNonFirst = TRUE;
        }
#endif
       if(prBytesInfo->fgCodecConfigData)
       {
    	  LOG(0, "@@@@Drop MJPEG config data.\n");       
          return FALSE;
       }
    }
#ifdef MKV_INSERT_MPEG_SEQ_HDR_SW_WORKAROUND
    else if (VDEC_FMT_MPV == prVdec->eFmt)
    {
        if (!prVdec->fgNonFirst)
        {
            prVdec->fgNonFirst = TRUE;
            _u4MpegSeqLen = 0;

            ptr = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);
            if ((ptr[0] == 0x00) && (ptr[1] == 0x00) && (ptr[2] == 0x01) && (ptr[3] == 0xB3))
            {
                UINT32 i;

                for (i = 4; i < MPEG_SEQ_LEN; i++)
                {
                    if ((ptr[i] == 0x00) && (ptr[i + 1] == 0x00) && (ptr[i + 2] == 0x01))
                    {
                        // find next start code, seq hdr got
                        break;
                    }
                }

                if (i < MPEG_SEQ_LEN)
                {
                    _u4MpegSeqLen = i;
                    x_memcpy(_u1MpegSeq, ptr, i);
                    LOG(0, "1st seq hdr got, len is %d\n", _u4MpegSeqLen);
                }
            }
        }
        else if (_u4MpegSeqLen && (prBytesInfo->u4BytesSize > _u4MpegSeqLen))
        {
            ptr = (UCHAR*)VIRTUAL(prBytesInfo->u4BytesAddr);

            if (0 == x_memcmp(_u1MpegSeq, ptr, _u4MpegSeqLen))
            {
                // seq hdr is duplicate
                prBytesInfo->u4BytesSize -= _u4MpegSeqLen;
                prBytesInfo->u4BytesAddr += _u4MpegSeqLen;
            }
        }
    }
#endif

// TODO
#ifdef CC_SUPPORT_TV_STAGEFRIGHT  // new OMX
    if (prVdec->eFmt == VDEC_FMT_WMV)
    {
        if (!prVdec->fgNonFirst)
        {
            asf_bitmapinfoheader_s *pASF_BitHeader = NULL;
            pASF_BitHeader = (asf_bitmapinfoheader_s *)prBytesInfo->u4BytesAddr;
            prVdec->fgNonFirst = TRUE;
            prVdec->u4Width  = pASF_BitHeader->biWidth;
            prVdec->u4Height = pASF_BitHeader->biHeight;
            LOG(3, "Pre-set WMV W x H = %d x %d !!!\n", prVdec->u4Width, prVdec->u4Height);
            prBytesInfo->u4BytesAddr += sizeof(asf_bitmapinfoheader_s);
            prBytesInfo->u4BytesSize -= sizeof(asf_bitmapinfoheader_s);
        }
    }
#endif

    prVdec->pCurrentInputFrame = prBytesInfo;
    if (prBytesInfo->u4BytesSize && s_codec_map[prVdec->eFmt].move_special_data)
    {
        MoveSpecialDataRet = s_codec_map[prVdec->eFmt].move_special_data(prVdec);
    }
    
    if (MOVE_SPECIAL_DATA_DO_NOT_CONTINUE == MoveSpecialDataRet)
    {
        return TRUE;   
    }
    else if (MOVE_SPECIAL_DATA_FAIL == MoveSpecialDataRet)
    {
        return FALSE;
    }

    rDmxMMData.u1Idx = prVdec->u1DmxPid;
    rDmxMMData.u4BufStart = prVdec->u4FifoStart;
    rDmxMMData.u4BufEnd = prVdec->u4FifoEnd;
    rDmxMMData.u4StartAddr = prBytesInfo->u4BytesAddr;
    rDmxMMData.u4FrameSize = prBytesInfo->u4BytesSize;
    rDmxMMData.fgEOS = prBytesInfo->fgEos;
    //rDmxMMData.fgMoveComplete = TRUE;
    #ifdef VDEC_PUSH_PTS_64_BITS
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS & (UINT64)0xFFFFFFFF);
    rDmxMMData.u4Dts = (UINT32)((prBytesInfo->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
    #else
    rDmxMMData.u4Pts = (UINT32)(prBytesInfo->u8BytesPTS);
    rDmxMMData.u4Dts = (UINT32)(prBytesInfo->u8BytesPTS);
    #endif

    // in case of eos message with zero data length.
    if (prBytesInfo->u4BytesSize == 0)
    {
        ASSERT(prBytesInfo->fgEos);
        rDmxMMData.u4FrameSize = 1;
        rDmxMMData.fgIgnoreData = TRUE;
    }
    
    if (prVdec->eFmt == VDEC_FMT_WMV && prVdec->ePushFmt != VDEC_PUSH_FMT_VC1)
    {
        // If 'Append' is set, it means this data should be concatenate with previous one, it is not frame head.
        if (prBytesInfo->fgAppend == FALSE)
        {
            rDmxMMData.fgFrameHead = TRUE;
        }
    }

    fgRet = _VPUSH_DmxMoveData(prVdec, &rDmxMMData);

     // #VPUSH_RV_SLICE_BUF#
    if (prVdec->eFmt == VDEC_FMT_RV)
        VERIFY(x_sema_lock(prVdec->hRvMoveSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(!prVdec->fgNonFirst)
    {
        LOG(6, "1st move, Fifo(0x%x-0x%x) Addr(0x%x) Size(0x%x)\n",
            rDmxMMData.u4BufStart,
            rDmxMMData.u4BufEnd,
            rDmxMMData.u4StartAddr,
            rDmxMMData.u4FrameSize);
        prVdec->fgNonFirst = TRUE;
    }

    return fgRet;
}


void _VPUSH_SendCmd(VOID* prdec, UINT32 u4Cmd)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    VDEC_T *prVdec;
    VDEC_MSG_INTO_T rMsg;
    VDEC_MSG_INTO_T rMsgDump;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    LOG(3, "[VPUSH-%d] Send Cmd 0x%x\n", (UINT32)prVdec->ucVPushId, (UINT32)u4Cmd);

    x_memset(&rMsg, 0, sizeof(VDEC_MSG_INTO_T));
    rMsg.eMsgType = VPUSH_MSG_CMD;
    rMsg.u.eCmd = (VPUSH_CMD_T)u4Cmd;

    i4Ret = x_msg_q_send(prVdec->hMsgQ,(void*)&rMsg, sizeof(VDEC_MSG_INTO_T), 254);
    if (i4Ret == OSR_TOO_MANY)
    {
        LOG(1, "Send Cmd(%d) 0x%x -> drop previous Msg\n", (UINT32)prVdec->ucVPushId, (UINT32)u4Cmd);
        zMsgSize = sizeof(VDEC_MSG_INTO_T);
        VERIFY(x_msg_q_receive(&u2MsgQIdx, &rMsgDump, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK);
        i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&rMsg, sizeof(VDEC_MSG_INTO_T), 254);
    }
    else
    {
        VERIFY(i4Ret == OSR_OK);
    }
    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
}

ENUM_VPUSH_MSG_T _VPUSH_ReceiveMsg(VOID* prdec, BOOL bIsBlock)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    VDEC_MSG_INTO_T rMsgTmp;
    ENUM_VPUSH_STATE_T ePreStatus;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return VPUSH_MSG_NONE;
    }
    prVdec = (VDEC_T*)prdec;

    zMsgSize = sizeof(VDEC_MSG_INTO_T);

    if(bIsBlock)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &prVdec->rMsg, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_WAIT);
        ASSERT(i4Ret == OSR_OK);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &prVdec->rMsg, &zMsgSize,
                &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
    if (i4Ret == OSR_NO_MSG)
    {
        return VPUSH_MSG_NONE;
    }
    else if (i4Ret == OSR_OK)
    {
        ASSERT(zMsgSize == sizeof(VDEC_MSG_INTO_T));
    }

    if(prVdec->rMsg.eMsgType == VPUSH_MSG_CMD)
    {
        LOG(3, "[VPUSH-%d] Rsv u4Cmd %x\n", prVdec->ucVPushId, prVdec->rMsg.u.eCmd);

        ePreStatus = prVdec->eCurState;

        prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
        prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);

        //filter illegal command
        switch (prVdec->rMsg.u.eCmd)
        {
            case VPUSH_CMD_STOP:
                prVdecEsInfoKeep->fgVPush = FALSE;                
                prVdecEsInfoKeep->fgLowLatency = FALSE;
                prVdecEsInfo->fgMMPlayback = FALSE;
                prVdec->ePreviousState = prVdec->eCurState;
                prVdec->eCurState = VPUSH_ST_STOP;
                prVdec->u4PreviousStopId = prVdec->u4CurStopId;
                prVdec->u4CurStopId ++;

                i4Ret = OSR_OK;
                while(i4Ret == OSR_OK)
                {
                    i4Ret = x_msg_q_receive(&u2MsgQIdx, &rMsgTmp, &zMsgSize,
                            &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT);
                }
                UNUSED(_VPUSH_GetMsgCountInQ(prdec));
                LOG(6, "%s(%d): VPUSH_CMD_STOP\n", __FUNCTION__, __LINE__);
                if(prVdec->rInpStrm.fnCb.pfnVdecStopDone)
                {
                    prVdec->rInpStrm.fnCb.pfnVdecStopDone(
                             prVdec->rInpStrm.fnCb.u4StopTag);
                }
                if(prVdec->hStopSema)
                {
                    VERIFY(x_sema_unlock(prVdec->hStopSema) == OSR_OK);
                }
                prVdec->u8TotalPushSize = 0;
                prVdec->fgNonFirst = FALSE;
                prVdec->fgInputBufReady = FALSE;
                prVdec->fgFirstVideoChunk = TRUE;
                prVdec->fgInsertStartcode = FALSE;
                prVdec->fgGotEos = FALSE;
                break;
            case VPUSH_CMD_FLUSH:
                
                while (OSR_OK == x_msg_q_receive(&u2MsgQIdx, &rMsgTmp, &zMsgSize,
                                                 &(prVdec->hMsgQ), 1, X_MSGQ_OPTION_NOWAIT))
                {
                    ASSERT(zMsgSize == sizeof(VDEC_MSG_INTO_T));
                    if (rMsgTmp.eMsgType == VPUSH_MSG_DATA)
                    {
                        if (!_VPUSH_PutDataDone(prVdec, rMsgTmp.u.rBytesInfo.u4BytesAddr))
                        {
                            LOG(3, "%s(%d): _VPUSH_PutDataDone fail\n", __FUNCTION__, __LINE__);
                        }
                    }
                }
               
                LOG(1, "%s(%d): Get VPUSH_CMD_FLUSH\n",__FUNCTION__, __LINE__);
                UNUSED(_VPUSH_GetMsgCountInQ(prdec));
                DMX_MM_FlushBuffer(prVdec->u1DmxPid);
                //flush first make sure vdec can unlock from get frame.
                if(prVdecEsInfoKeep->eVPushPlayMode != VDEC_PUSH_MODE_TUNNEL)
                {
                    FBM_FlushLockToEmptyQ(prVdecEsInfo->ucFbgId);
                }
                VDEC_ReleaseDispQ(prVdec->ucVdecId);
                _VPUSH_FlushEsmQ(prVdec->ucVdecId);
                //flush again to free frame flushed.
                if(prVdecEsInfoKeep->eVPushPlayMode != VDEC_PUSH_MODE_TUNNEL)
                {
                    FBM_FlushLockToEmptyQ(prVdecEsInfo->ucFbgId);
                }
                LOG(1, "%s(%d): VPUSH_CMD_FLUSH u4DmxAvailSize(%d)\n",\
                    __FUNCTION__, __LINE__,\
                    DMX_MUL_GetEmptySize(prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid));
				
                prVdec->rDecryptInfo.u4TempBufWp=0;
                prVdec->rDecryptInfo.fgReseted=TRUE;
                if(prVdec->rInpStrm.fnCb.pfnVdecFlushDone)
                {
                    prVdec->rInpStrm.fnCb.pfnVdecFlushDone(
                             prVdec->rInpStrm.fnCb.u4FlushTag);
                }

                if(prVdec->hFlushSema)
                {
                    VERIFY(x_sema_unlock(prVdec->hFlushSema) == OSR_OK);
                }

                prVdec->u8TotalPushSize = 0;
                //prVdec->fgNonFirst = FALSE;
                break;
            case VPUSH_CMD_PLAY:
                if((prVdec->eCurState == VPUSH_ST_STOP) ||
                   (prVdec->eCurState == VPUSH_ST_IDLE))
                {
                    prVdec->ePreviousState = prVdec->eCurState;
                    prVdec->eCurState = VPUSH_ST_PLAY;
                }
                break;
            case VPUSH_CMD_PLAY_SYNC:
                if((prVdec->eCurState == VPUSH_ST_STOP) ||
                   (prVdec->eCurState == VPUSH_ST_IDLE))
                {
                    prVdec->ePreviousState = prVdec->eCurState;
                    prVdec->eCurState = VPUSH_ST_PLAY;
                    VDEC_Play(prVdec->ucVdecId, prVdec->eFmt);
                }
                break;
            case VPUSH_CMD_PAUSE:
                if (prVdec->eCurState == VPUSH_ST_PLAY)
                {
                    prVdec->ePreviousState = prVdec->eCurState;
                    prVdec->eCurState = VPUSH_ST_IDLE;
                }
                break;
            default:
                break;
        }

        if (prVdec->eCurState != ePreStatus)
        {
            LOG(3, "[VPUSH-%d] Status Change %x to %x\n",
                prVdec->ucVPushId, (UINT32)ePreStatus, (UINT32)prVdec->eCurState);
        }
    }

    return prVdec->rMsg.eMsgType;
}

BOOL _VPUSH_Play(VOID* prdec)
{
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_T *prVdec;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if((!prVdecEsInfoKeep) || (!prVdecEsInfo))
    {
        LOG(1, "%s(%d): instance null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    prVdecEsInfoKeep->fgVPush = TRUE;
    prVdecEsInfo->fgMMPlayback = TRUE;    
    prVdecEsInfoKeep->fgLowLatency = FALSE;
    
    if(prVdec->eFmt == VDEC_FMT_H264 || prVdec->eFmt == VDEC_FMT_H265)
    {
       prVdecEsInfo->eMMSrcType = SWDMX_SRC_TYPE_NETWORK_NETFLIX; // enable seamless
       prVdecEsInfo->u4SeamlessWidth = 1920;
       prVdecEsInfo->u4SeamlessHeight = 1080;
       VDEC_ChkSeamlessModeChg(prVdec->ucVdecId,prVdecEsInfo->u4SeamlessWidth,prVdecEsInfo->u4SeamlessHeight);
    }
    
    if(prVdec->fgLowLatencyMode)
    {
        prVdecEsInfoKeep->fgLowLatency = TRUE;
    }

    if(prVdecEsInfoKeep->eVPushPlayMode == VDEC_PUSH_MODE_TUNNEL)
    {
        LOG(0, "set AV_SYNC_MODE_AUDIO_MASTER to STC0");
        STC_SetSTCSyncMode(0, AV_SYNC_MODE_AUDIO_MASTER);
        LOG(0, "set VID_SYNC_MODE_SLAVE_MM to VDEC%d", prVdec->ucVdecId);
        VDEC_SyncStc(prVdec->ucVdecId, VID_SYNC_MODE_SLAVE_MM, 0/*u2PcrId*/);
        // reset STC
        STC_StopStc(0);
        STC_SetStcValue(0, 0);
        STC_StartStc(0);
    }
    else
    {
        // free run
        VDEC_SyncStc(prVdec->ucVdecId, VID_SYNC_MODE_NONE, 0/*u2PcrId*/);
    }

    // clean the esmq info.
    // We need to clean it when VDEC_Stop with DMX free video pid
    MPV_FlushEsmQ(prVdec->ucVdecId, FALSE);
    LOG(0,"[VPUSH-%d] Play(%d) fmt=%d\n", prVdec->ucVPushId, prVdec->ucVdecId, prVdec->eFmt);
    prVdec->eCurStateSync = VPUSH_ST_PLAY;
    prVdec->rDecryptInfo.fgReseted=TRUE;
    prVdec->rDecryptInfo.u4TempBufWp=0;
    _VPUSH_SendCmd(prdec, (UINT32)VPUSH_CMD_PLAY);
    
    //register notification
    //LOG(6, "[VPUSH] register notification before vdec play.\n");
    x_memset(&rNfyInfo, 0, sizeof(VDEC_DEC_NFY_INFO_T));
    rNfyInfo.pvTag = (void*)prdec;
    rNfyInfo.pfDecNfy = _VPUSH_VDEC_Nfy;
    VDEC_SetDecNfy(prVdec->ucVdecId, &rNfyInfo);
    return VDEC_Play(prVdec->ucVdecId, prVdec->eFmt);
}

BOOL _VPUSH_PauseVdec(VOID* prdec)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    VDEC_Pause(prVdec->ucVdecId);
	return TRUE;
}

BOOL _VPUSH_PlayVdec(VOID* prdec)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    return VDEC_Play(prVdec->ucVdecId, prVdec->eFmt);
}

BOOL _VPUSH_Stop(VOID* prdec)
{
    VDEC_T *prVdec;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    LOG(6, "%s(%d)\n", __FUNCTION__, __LINE__);

    prVdec = (VDEC_T*)prdec;

    x_memset(&rNfyInfo, 0, sizeof(VDEC_DEC_NFY_INFO_T));
    rNfyInfo.pvTag = (void*)prdec;
    rNfyInfo.pfDecNfy = _VPUSH_VDEC_Nfy;
    VDEC_SetDecNfy(prVdec->ucVdecId, &rNfyInfo);

    prVdec->eCurStateSync = VPUSH_ST_STOP;
    VDEC_Stop(prVdec->ucVdecId);
    return TRUE;
}


BOOL _VPUSH_Flush(VOID* prdec)
{
    //VDEC_T *prVdec;
    //VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    LOG(6, "%s(%d)\n", __FUNCTION__, __LINE__);

    //prVdec = (VDEC_T*)prdec;

    _VPUSH_SendCmd(prdec, (UINT32)VPUSH_CMD_FLUSH);
    return TRUE;
}


BOOL _VPUSH_StopSync(VOID* prdec)
{
    INT32 i4Ret;
    VDEC_T *prVdec;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;

    x_memset(&rNfyInfo, 0, sizeof(VDEC_DEC_NFY_INFO_T));
    rNfyInfo.pvTag = (void*)prdec;
    rNfyInfo.pfDecNfy = _VPUSH_VDEC_Nfy;
    VDEC_SetDecNfy(prVdec->ucVdecId, &rNfyInfo);

    prVdec->eCurStateSync = VPUSH_ST_STOP;
    prVdec->ePushFmt = VDEC_PUSH_FMT_UNUSED;
    VDEC_Stop(prVdec->ucVdecId);

//  may timeout under multi omx case(havn't find out why 5 secs is not enough), by zhipeng
//        i4Ret = x_sema_lock_timeout(prVdec->hStopSema, VDEC_PUSH_STOP_SYNC_WAIT_TIME);
    i4Ret = x_sema_lock(prVdec->hStopSema, X_SEMA_OPTION_WAIT);
    if(i4Ret != OSR_OK)
    {
        ASSERT(i4Ret == OSR_TIMEOUT);
        LOG(3, "%s(%d): stop timeout\n", __FUNCTION__, __LINE__);

        // make the lock in lock status
        x_thread_delay(1);
        x_sema_lock(prVdec->hStopSema, X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        return FALSE;
    }
    
    LOG(0,"[VPUSH-%d] Stop Done(%d) fmt=%d\n", prVdec->ucVPushId, prVdec->ucVdecId, prVdec->eFmt);
    return TRUE;
}


BOOL _VPUSH_FlushSync(VOID* prdec)
{
    INT32 i4Ret;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    LOG(1, "%s(%d)\n", __FUNCTION__, __LINE__);

    prVdec = (VDEC_T*)prdec;
    _VPUSH_SendCmd(prVdec, (UINT32)VPUSH_CMD_FLUSH);

    i4Ret = x_sema_lock(prVdec->hFlushSema, X_SEMA_OPTION_WAIT);
    if(i4Ret != OSR_OK)
    {
        ASSERT(i4Ret == OSR_TIMEOUT);
        LOG(3, "%s(%d): flush timeout\n", __FUNCTION__, __LINE__);

        // make the lock in lock status
        x_thread_delay(1);
        x_sema_lock(prVdec->hFlushSema, X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

        return FALSE;
    }

    return TRUE;
}

BOOL _VPUSH_PrsSeqHdr(VOID* prdec,
    VDEC_BYTES_INTO_T *prBytesInfo, FBM_SEQ_HDR_T *prSeqInfo)
{
    BOOL fgRet;
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if((!prdec) || (!prBytesInfo) || (!prSeqInfo))
    {
        LOG(3, "%s(%d): pointer null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);    
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    if(!prVdecEsInfo || !prVdecEsInfoKeep)
    {
        LOG(3, "%s(%d): prVdecEsInfo or prVdecEsInfoKeep null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    //reset flags
    VERIFY(x_sema_delete(prVdec->hIoQSema) == OSR_OK);
    VERIFY(x_sema_create(&prVdec->hIoQSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    prVdec->u8TotalPushSize = 0;
    prVdec->fgNonFirst = FALSE;
    prVdec->fgInputBufReady = FALSE;
    prVdec->fgFirstVideoChunk = TRUE;
    prVdec->fgInsertStartcode = FALSE;
    prVdec->fgGotEos = FALSE;
    prVdec->fgDecoderCalPts = FALSE;
    prVdec->ucPesIndex = 0 ;    
    prVdecEsInfoKeep->rPesInfo.ucMpvId = VDEC_MAX_VLD;
	
    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_PARSING_INFO, (UINT32)TRUE, 0, 0);
    fgRet = _VPUSH_PutData(prdec, prBytesInfo);
    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_PARSING_INFO, (UINT32)FALSE, 0, 0);
	
    if(VDEC_DEC_ERROR_NONE != fgRet)
    {
        LOG(3, "%s(%d): _VPUSH_PutData fail\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    //sequence info is not needed by omx component now
    #if 0
    if(prVdec->eFmt != VDEC_FMT_H265)
    {        
        FBM_SEQ_HDR_T *prFbmSeqHdr;
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
        if (!prFbmSeqHdr)
        {
            LOG(3, "%s(%d): FBM_GetFrameBufferSeqHdr fail\n", __FUNCTION__, __LINE__);
            return FALSE;
        }

        // TODO, get more seq/pic information
        x_memset(prSeqInfo, 0, sizeof(FBM_SEQ_HDR_T));
        prSeqInfo->u2HSize = prFbmSeqHdr->u2HSize;
        prSeqInfo->u2VSize = prFbmSeqHdr->u2VSize;
        prSeqInfo->fgCropping = prFbmSeqHdr->fgCropping;
        prSeqInfo->u4CropX = prFbmSeqHdr->u4CropX;
        prSeqInfo->u4CropY = prFbmSeqHdr->u4CropY;
        prSeqInfo->u4CropWidth = prFbmSeqHdr->u4CropWidth;
        prSeqInfo->u4CropHeight = prFbmSeqHdr->u4CropHeight;

        LOG(6, "%s(%d) ret (%dx%d)\n", __FUNCTION__, __LINE__,
            prSeqInfo->u2HSize, prSeqInfo->u2VSize);
        if(prSeqInfo->fgCropping)
            LOG(6, "%s(%d) crop (%dx%dx%dx%d)\n", __FUNCTION__, __LINE__,
                prSeqInfo->u4CropX, prSeqInfo->u4CropY, prSeqInfo->u4CropWidth, prSeqInfo->u4CropHeight);
    }
    #endif
    return TRUE;
}

ENUM_VDEC_STATE_T _VPUSH_GetDecoderStatus(UINT8 uDecId)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep=NULL;

    if(uDecId>=VDEC_PUSH_MAX_DECODER)
    {
       return VDEC_ST_MAX;
    }
	prVdecEsInfoKeep= _VDEC_GetEsInfoKeep((UINT8)uDecId);
    return prVdecEsInfoKeep->eCurState;
}


UCHAR _VPUSH_GetAvailableVdec(BOOL fgZeroFirst)
{
    INT8 iVdecId=0;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep=NULL;
	if(fgZeroFirst)
	{
		for(iVdecId=0;iVdecId<VDEC_PUSH_MAX_DECODER;iVdecId++)
		{
		   prVdecEsInfoKeep= _VDEC_GetEsInfoKeep((UINT8)iVdecId);
		   if(prVdecEsInfoKeep->eCurState == VDEC_ST_STOP)
		   {
			  break;
		   }
		}
	}
	else
	{
		for(iVdecId=VDEC_PUSH_MAX_DECODER-1;iVdecId>=0;iVdecId--)
		{
		   prVdecEsInfoKeep= _VDEC_GetEsInfoKeep((UINT8)iVdecId);
		   if(prVdecEsInfoKeep->eCurState == VDEC_ST_STOP)
		   {
			  break;
		   }
		}
	}
	
    if(iVdecId==VDEC_PUSH_MAX_DECODER ||iVdecId<0)
    {
        LOG(0,"_VPUSH_GetAvailableVdec fail\n"); 
		return VDEC_PUSH_MAX_DECODER;
    }
	else
	{
		return (UCHAR)iVdecId;
	}
	
}


BOOL _VPUSH_SetInfo(VOID* prdec, VDEC_SET_INTO_T *prVdecSetInfo)
{
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(!prVdec)
    {
        LOG(3, "%s(%d): prVdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    
    if(!prVdecSetInfo)
    {
        LOG(3, "%s(%d): prVdecSetInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_VDEC_ID)
    {
        // channelid is same with vdec id currently.
        prVdec->ucVdecId = prVdecSetInfo->ucVdecId;
        prVdec->ucChannelId = prVdecSetInfo->ucVdecId;
        LOG(3, "VPush(%d), ucVdecId(%d)\n", prVdec->ucVPushId, prVdec->ucVdecId);
        if(prVdec->ucVdecId>=VDEC_PUSH_MAX_DECODER)
        {
           return FALSE;
        }
    }

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(3, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    if(!prVdecEsInfoKeep)
    {
        LOG(3, "%s(%d): _VDEC_GetEsInfoKeep null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    LOG(7, "[VPUSH-%d] _VPUSH_SetInfo: mask(0x%08x)\n", prVdec->ucVPushId, prVdecSetInfo->u4InfoMask);
    if(prVdecSetInfo->u4InfoMask&VDEC_PUSH_SET_INFO_DECODE_ONLY)
    {
        prVdec->fgInsertStartcode = FALSE;
        prVdecEsInfoKeep->fgVPushDecodeOnly = prVdecSetInfo->fgVPushDecodeOnly;
        LOG(6, "VPush(%d), fgVPushDecodeOnly(%d)\n",
            prVdec->ucVPushId, prVdecSetInfo->fgVPushDecodeOnly);
    }
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_DECODE_MODE)
    {
        prVdecEsInfo->eDecodeMode = prVdecSetInfo->eDecodeMode;
        LOG(6, "VPush(%d), eDecodeMode(%d)\n",
            prVdec->ucVPushId, prVdecSetInfo->eDecodeMode);
    }
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_FBG_FROM_INST)
    {
        prVdecEsInfoKeep->fgVPushFBGFromInst = prVdecSetInfo->fgVPushFBGFromInst;
        LOG(6, "VPush(%d), fgVPushFBGFromInst(%d)\n",
            prVdec->ucVPushId, prVdecSetInfo->fgVPushFBGFromInst);
    }
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_DECRYIPT)
    {
        prVdec->rDecryptInfo.fgDecryptPlayback=prVdecSetInfo->fgVPushDecrypt;
        LOG(0, "VPush Marsk=%d,Decrypt=%d\n", prVdecSetInfo->u4InfoMask,prVdec->rDecryptInfo.fgDecryptPlayback);
    }
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_PLAY_MODE)
    {
        prVdecEsInfoKeep->eVPushPlayMode = prVdecSetInfo->eVpushPlayMode;
        LOG(3, "VPush Marsk=%d,VPushPlayMode=%d\n", prVdecSetInfo->u4InfoMask,prVdecEsInfoKeep->eVPushPlayMode);
    }

    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_SECURE_INPUT)
    {
        FBM_POOL_T* prSecurePool;
        prVdec->fgIsSecureInput=TRUE;
        prSecurePool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_SECURE_FEEDER);
        if(prSecurePool)
        {
            prVdec->u4SecureInputStartAddr = prSecurePool->u4Addr + (prVdec->fgGstPlay ? 0x200000 : 0);
            prVdec->u4SecureInputEndAddr =  prSecurePool->u4Addr + prSecurePool->u4Size;
            prVdec->u4SecureInputStartAddr=VIRTUAL(prVdec->u4SecureInputStartAddr);
            prVdec->u4SecureInputEndAddr=VIRTUAL(prVdec->u4SecureInputEndAddr);
        }
        
        LOG(2, "VPush(%d) set secure input buffer(0x%x--0x%x)\n",prVdec->ucVPushId,\
            prVdec->u4SecureInputStartAddr,prVdec->u4SecureInputEndAddr);
    }
    
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_VDP_ID)
    {
        VDEC_RmSetRenderVdp(prVdec->ucVdecId, prVdecSetInfo->ucVdpId);
    }
    
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_LOW_LATENCY)
    {
        prVdec->fgLowLatencyMode = TRUE;
    }
	
    if(prVdecSetInfo->u4InfoMask & VDEC_PUSH_SET_INFO_LGE_GST)
    {
        prVdec->fgGstPlay=TRUE;
        prVdecEsInfo->fgLGSeamless = TRUE;
        LOG(3, "VPush Marsk VDEC_PUSH_SET_INFO_LGE_GST\n");
    }
    return TRUE;
}
BOOL _VPUSH_GetInfo(VOID* prdec, VDEC_GET_INTO_T *prVdecGetInfo)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    if(!prVdecGetInfo)
    {
        LOG(3, "%s(%d): prVdecSetInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdecGetInfo->ucVdecId = prVdec->ucVdecId;
    prVdecGetInfo->ucVPushId = prVdec->ucVPushId;
    return TRUE;
}

BOOL _VPUSH_RegCb(VOID* prdec, VDEC_PUSH_CB_T *prVdecPushCb)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    if(!prVdecPushCb)
    {
        LOG(3, "%s(%d): prVdecInputCb null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_PUT_DATA_DONE)
    {
        prVdec->rInpStrm.fnCb.u4PutTag = prVdecPushCb->u4PutTag;
        prVdec->rInpStrm.fnCb.pfnVdecPutDataDone = prVdecPushCb->pfnVdecPutDataDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_STOP_DONE)
    {
        prVdec->rInpStrm.fnCb.u4StopTag = prVdecPushCb->u4StopTag;
        prVdec->rInpStrm.fnCb.pfnVdecStopDone = prVdecPushCb->pfnVdecStopDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_FLUSH_DONE)
    {
        prVdec->rInpStrm.fnCb.u4FlushTag = prVdecPushCb->u4FlushTag;
        prVdec->rInpStrm.fnCb.pfnVdecFlushDone = prVdecPushCb->pfnVdecFlushDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_DECODE_DONE)
    {
        prVdec->rInpStrm.fnCb.u4DecodeTag = prVdecPushCb->u4DecodeTag;
        prVdec->rInpStrm.fnCb.pfnVdecDecodeDone = prVdecPushCb->pfnVdecDecodeDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_IMG_FRAME_DONE)
    {
        prVdec->rInpStrm.fnCb.u4ImageTag = prVdecPushCb->u4ImageTag;
        prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone= prVdecPushCb->pfnVdecImgFrmDone;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_VDEC_ERR)
    {
        prVdec->rInpStrm.fnCb.u4VdecErrTag = prVdecPushCb->u4VdecErrTag;
        prVdec->rInpStrm.fnCb.pfnVdecErrHandler= prVdecPushCb->pfnVdecErrHandler;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_UNDERRUN)
    {
        prVdec->rInpStrm.fnCb.u4VdecUnderrunTag = prVdecPushCb->u4VdecUnderrunTag;
        prVdec->rInpStrm.fnCb.pfnVdecUnderrunCb = prVdecPushCb->pfnVdecUnderrunCb;
    }
    if(prVdecPushCb->u4Mask & VDEC_PUSH_CB_THUMBNAIL)
    {
        prVdec->rInpStrm.fnCb.u4VdecThumbTag = prVdecPushCb->u4VdecThumbTag;
        prVdec->rInpStrm.fnCb.pfnVdecThumbCb = prVdecPushCb->pfnVdecThumbCb;
    }

    return TRUE;
}

BOOL _VPUSH_PutData(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    INT32 i4Ret;
    VDEC_MSG_INTO_T rMsg;

    VDEC_T *prVdec;
    if((!prdec) || (!prBytesInfo))
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return VDEC_DEC_ERROR_INVALID_ARG;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->eCurStateSync == VPUSH_ST_STOP)
    {
        LOG(0, "%s(%d): under stop\n", __FUNCTION__, __LINE__);
        return VDEC_DEC_ERROR_WORNG_STATE;
    }

    // check put data semaphore
    // check put data semaphore
    if(prVdec->fgGstPlay==FALSE)
    {
    	i4Ret = x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_NOWAIT);
        VERIFY(i4Ret == OSR_WOULD_BLOCK);
        i4Ret = x_sema_lock(prVdec->hMoveEsmQSema, X_SEMA_OPTION_NOWAIT);
        VERIFY(i4Ret == OSR_WOULD_BLOCK);
    }

    x_memset(&rMsg, 0, sizeof(VDEC_MSG_INTO_T));
    rMsg.eMsgType = VPUSH_MSG_DATA;
    x_memcpy(&rMsg.u.rBytesInfo, prBytesInfo, sizeof(VDEC_BYTES_INTO_T));
    i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&rMsg,
        sizeof(VDEC_MSG_INTO_T), 255);
    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
    if(i4Ret != OSR_OK)
    {
        Printf("[VOMX]Empty data error7\n");
        LOG(0, "Error:%s(%d): i4Ret(%d)),pts=%lld\n", __FUNCTION__, __LINE__, i4Ret,prBytesInfo->u8BytesPTS);
        return VDEC_DEC_ERROR_INTERNAL_ERROR;
    }

    // wait put data semaphore
    if(prVdec->fgGstPlay==FALSE)
	{
	    i4Ret = x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_WAIT);
	    if(i4Ret != OSR_OK)
	    {
	        ASSERT(i4Ret == OSR_TIMEOUT);
	        LOG(3, "%s(%d): putdata timeout\n", __FUNCTION__, __LINE__);
	    
	        // make the lock in lock status
	        x_thread_delay(1);
	        x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_NOWAIT);
	        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
	    
	        return FALSE;
	    }

	    if (prVdec->fgFifoFull)
	    {
	        prVdec->fgFifoFull = FALSE;
	        return FALSE;
	    }

        i4Ret = x_sema_lock_timeout(prVdec->hMoveEsmQSema, DMX_MOVE_DATA_TIMEOUT);
        if (i4Ret != OSR_OK)
        {
            LOG(0, "[VPUSH] move esmq fail.\n");
            return VDEC_DEC_ERROR_INTERNAL_ERROR;
        }

	}
	



    return  VDEC_DEC_ERROR_NONE;
}

BOOL _VPUSH_PutDataDone(VOID* prdec, UINT32 u4Tag)
{
    VDEC_T *prVdec;
    BOOL i4Ret = TRUE;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    UNUSED(u4Tag);//not used currently
    prVdec = (VDEC_T*)prdec;

	if(prVdec->fgGstPlay==FALSE &&prVdec->hPutDataSema)
    {
        VERIFY(x_sema_unlock(prVdec->hPutDataSema) == OSR_OK);
    }

	if(prVdec->rInpStrm.fnCb.pfnVdecPutDataDone)
    {
        i4Ret = prVdec->rInpStrm.fnCb.pfnVdecPutDataDone(
            prVdec->rInpStrm.fnCb.u4PutTag);
    }


    return i4Ret;
}

BOOL _VPUSH_SetSeamlessMode(VOID* prdec, UINT32 u4MaxWidth, UINT32 u4MaxHeight, UINT32 u4ResizerId)
{
    VDEC_T *prVdec;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
#ifndef ENABLE_MULTIMEDIA
    UNUSED(prVdec);
#else
    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SRC_TYPE, (UINT32)SWDMX_SRC_TYPE_NETWORK_LIVE_STREAMING, 0, 0);
    VDEC_SetTPTFlag(prVdec->ucVdecId, TRUE);
    //VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_SET_SEAMLESS_TARGET, u4MaxWidth, u4MaxHeight, 0);
    #ifdef CC_VDEC_RM_SUPPORT
    VDEC_RmSetImgrzIdMsk(prVdec->ucVdecId, u4ResizerId);
    #endif
#endif

    LOG(1, "%s(%d): StageFright set seamless.\n", __FUNCTION__, __LINE__);
    return TRUE;
}

BOOL _VPUSH_PutDataSync(VOID* prdec, VDEC_BYTES_INTO_T *prBytesInfo)
{
    INT32 i4Ret;
    VDEC_MSG_INTO_T rMsg;
    VDEC_ES_INFO_T *prVdecEsInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    VDEC_T *prVdec;
    if((!prdec) || (!prBytesInfo))
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->eCurStateSync == VPUSH_ST_STOP)
    {
        LOG(3, "%s(%d): under stop\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

#if 0
    if (prBytesInfo->fgEos)
    {
        LOG(3, "%s(%d): got eos, set to VPUSH_ST_STOP\n", __FUNCTION__, __LINE__);
        prVdec->eCurStateSync = VPUSH_ST_STOP;
    }
#endif

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(3, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    if(!prVdecEsInfoKeep)
    {
        LOG(3, "%s(%d): prVdecEsInfoKeep null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    UNUSED(_VPUSH_GetMsgCountInQ(prdec));
    if(prVdec->u2MstCount != 0)
    {
        // should be 0
        LOG(3, "%s(%d): u2MstCount(%d) != 0\n", __FUNCTION__, __LINE__, prVdec->u2MstCount);
    }
    #if 0 // it includes virtual ES (VDEC_MAX_VLD), u2Count is not the video ES count.
    if(prVdecEsInfo->u2Count != 0)
    {
        // should be 0
        LOG(3, "%s(%d): u2Count(%d) != 0\n", __FUNCTION__, __LINE__, prVdecEsInfo->u2Count);
    }
    #endif

    if (!prBytesInfo->fgEos && prVdecEsInfo->fgSendedEos)
    {
        LOG(3, "Found valid data after flush/eos. Clear fgSendedEos!\n");
        prVdecEsInfo->fgSendedEos = FALSE;
    }

    i4Ret = x_sema_lock(prVdec->hIoQSema, X_SEMA_OPTION_NOWAIT);
    VERIFY(i4Ret == OSR_WOULD_BLOCK);

    if((prBytesInfo->u4BytesSize) || (prBytesInfo->fgEos))
    {
        {
            i4Ret = x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_NOWAIT);
            VERIFY(i4Ret == OSR_WOULD_BLOCK);
        }
        {
            i4Ret = x_sema_lock(prVdec->hMoveEsmQSema, X_SEMA_OPTION_NOWAIT);
            VERIFY(i4Ret == OSR_WOULD_BLOCK);
        }

        x_memset(&rMsg, 0, sizeof(VDEC_MSG_INTO_T));
        rMsg.eMsgType = VPUSH_MSG_DATA;
        x_memcpy(&rMsg.u.rBytesInfo, prBytesInfo, sizeof(VDEC_BYTES_INTO_T));
        i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&rMsg,
            sizeof(VDEC_MSG_INTO_T), 255);
        if(i4Ret != OSR_OK)
        {
            ASSERT(0);
            LOG(3, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
            return FALSE;
        }
        
        // wait put data finish (VPUSH thread)
        i4Ret = x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_WAIT);

        if(i4Ret != OSR_OK)
        {
            ASSERT(i4Ret == OSR_TIMEOUT);
            LOG(3, "%s(%d): putdata timeout\n", __FUNCTION__, __LINE__);

            // make the lock in lock status
            x_thread_delay(1);
            x_sema_lock(prVdec->hPutDataSema, X_SEMA_OPTION_NOWAIT);
            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));

            return FALSE;
        }

        LOG(3, "%s(L:%d): 2.2 %d %d hMoveEsmQSema\n", __FUNCTION__, __LINE__,
            prBytesInfo->fgEos, prVdecEsInfo->fgSendedEos);

        // wait DMX send all the PES info to VDEC
        //  may timeout under multi omx case
//        i4Ret = x_sema_lock_timeout(prVdec->hMoveEsmQSema, VDEC_PUSH_MOVEESMQ_SYNC_WAIT_TIME);
        i4Ret = x_sema_lock(prVdec->hMoveEsmQSema, X_SEMA_OPTION_WAIT);

        if(i4Ret != OSR_OK)
        {
            ASSERT(i4Ret == OSR_TIMEOUT);
            LOG(3, "%s(%d): moveesmq timeout\n", __FUNCTION__, __LINE__);

            // make the lock in lock status
            x_thread_delay(1);
            i4Ret = x_sema_lock(prVdec->hMoveEsmQSema, X_SEMA_OPTION_NOWAIT);
            VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
            ASSERT(0);
            return FALSE;
        }
    }
    else if((prBytesInfo->u4BytesSize != 0) && (!prBytesInfo->fgEos))
    {
        LOG(3, "%s(L:%d) unexpected?\n", __FUNCTION__, __LINE__);
    }

    i4Ret = x_sema_lock(prVdec->hIoQSema, X_SEMA_OPTION_WAIT);

    if(i4Ret != OSR_OK)
    {
        ASSERT(i4Ret == OSR_TIMEOUT);

        // make the lock in lock status
        x_thread_delay(1);
        i4Ret = x_sema_lock(prVdec->hIoQSema, X_SEMA_OPTION_NOWAIT);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_WOULD_BLOCK));
        ASSERT(0);
        return FALSE;
    }

    LOG(3, "%s(L:%d): 7 hIoQSema\n", __FUNCTION__, __LINE__);
    return TRUE;
}


VOID _VPUSH_DecodeInit(VOID)
{
    CHAR szBuf[16];
    VDEC_Init();
    if(!_prVdecPush)
    {
        LOG(3,"_prVdecPush NULL\n");
        _prVdecPush = (VDEC_DECODER_T*)x_mem_alloc(sizeof(VDEC_DECODER_T));
        if(!_prVdecPush)
        {
            LOG(0,"_prVdecPush Malloc Fail\n");
            return;
        }
        x_memset(_prVdecPush,0x00,sizeof(VDEC_DECODER_T));
    }
    if(!_prVdecPush->fgInited)
    {
        UINT32 i;
      //  x_memset(&_prVdecPush, 0, sizeof(VDEC_DECODER_T));        
        for(i=0;i<VDEC_PUSH_MAX_DECODER;i++)
        {
            _prVdecPush->arDec[i].hRvMoveSema=NULL_HANDLE;
            _prVdecPush->arDec[i].hDataTimer=NULL_HANDLE;
            _prVdecPush->arDec[i].ucVPushId = i;
            _prVdecPush->arDec[i].ucVdecId = i;
            _prVdecPush->arDec[i].ucChannelId = i;
            _prVdecPush->arDec[i].u4AllocBufCnt = 0;
			_prVdecPush->arDec[i].u4BufCnt = 0;
            _pdwVPSliceBuf[i]=NULL;
            _prVdecPush->arDec[i].rDecryptInfo.arTempUnitBuf=NULL;

            VERIFY(x_sema_create(&_prVdecPush->arDec[i].hStopSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) == OSR_OK);
            VERIFY(x_sema_create(&_prVdecPush->arDec[i].hPutDataSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) == OSR_OK);
            VERIFY(x_sema_create(&_prVdecPush->arDec[i].hMoveEsmQSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) == OSR_OK);
            VERIFY(x_sema_create(&_prVdecPush->arDec[i].hFlushSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) == OSR_OK);
            VERIFY(x_sema_create(&_prVdecPush->arDec[i].hIoQSema, X_SEMA_TYPE_BINARY,
                X_SEMA_STATE_LOCK) == OSR_OK);

            x_snprintf(szBuf, sizeof(szBuf), "PVBYTQ%d", i);
            VERIFY(x_msg_q_create(&_prVdecPush->arDec[i].hMsgQ, szBuf,
                sizeof(VDEC_MSG_INTO_T), VDEC_PUSH_MSG_Q_SIZE) == OSR_OK);

            x_snprintf(szBuf, sizeof(szBuf), "PVDEC%d", i);
            VERIFY(x_thread_create(&_prVdecPush->arDec[i].hThread, szBuf,
                VDEC_PUSH_STACK_SIZE, VDEC_PUSH_THREAD_PRIORITY,
                _VPUSH_PushLoop, 4, (VOID*)&_prVdecPush->arDec[i].ucVPushId) == OSR_OK);
        }

        _prVdecPush->fgInited = TRUE;
    }
}


BOOL _VPUSH_GetVFifoInfo(VOID* prdec,VDEC_VFIFO_INTO_T *prVfifoInfo)
{
    VDEC_T *prVdec=(VDEC_T *)prdec;
	
    if(prdec==NULL || prVfifoInfo==NULL)
    {
        LOG(3, "%s(%d,%d): prVfifoInfo is null\n", __FUNCTION__,(UINT32)prdec,(UINT32)prVfifoInfo);
        return FALSE;
    }
	
    prVfifoInfo->u4BufferCount = VDEC_BUF_COUNT;
    prVfifoInfo->u4BufferSize = prVdec->arBufInfo[0].u4BufSize;
    LOG(1,"_VPUSH_GetVFifoInfo BufCnt=%d,BufSize=0x%x\n",prVfifoInfo->u4BufferCount, prVfifoInfo->u4BufferSize);
    return TRUE;
}

#define ALIGNED(x, align) (((UINT32)(x) & (align - 1)) == 0)

#define SECURE_FEEDER_ALIGN (0x8000)

BOOL _VPUSH_GetFeederInfo(VOID* prdec, VDEC_FEEDER_INTO_T *prFeederInfo)
{
    VDEC_T *prVdec = NULL;
    if(!prFeederInfo || !prdec)
    {
        LOG(3, "%s(%d): prFeederInfo or prdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    if (prVdec->fgIsSecureInput && !prVdec->fgGstPlay)
    {
	#if defined(CC_FBM_SECURE_FEEDER)
        UINT32 u4size = 0;
        FBM_POOL_T* pool_info = FBM_GetPoolInfo(FBM_POOL_TYPE_SECURE_FEEDER);
        if (!pool_info)
        {
            return FALSE;
        }
        
        if (!ALIGNED(pool_info->u4Addr, SECURE_FEEDER_ALIGN))
        {
            LOG(0,"Secure buffer is not 32K align");
        }

        prFeederInfo->u4TotalSize = pool_info->u4Size; 
        prFeederInfo->u4BufferCount = prVdec->u4BufCnt;
        u4size = prFeederInfo->u4TotalSize/prVdec->u4BufCnt;
        //32K align
        prFeederInfo->u4BufferSize = ADDRESS_DEC_ALIGN(u4size, SECURE_FEEDER_ALIGN);
	#endif

    }
    else
    {
        prFeederInfo->u4TotalSize = prVdec->u4FifoEnd-prVdec->u4FifoStart;
        prFeederInfo->u4BufferCount = prVdec->u4BufCnt;
        prFeederInfo->u4BufferSize = prVdec->arBufInfo[0].u4BufSize;
    }
    return TRUE;

}

BOOL _VPUSH_SetRptr(UCHAR ucVdecId, UINT32 u4Rp,
    UINT32 u4FrameAddr)
{
    UINT32 i;
    VDEC_T *prVdec = NULL;

    for(i=0;i<VDEC_PUSH_MAX_DECODER;i++)
    {
        if(_prVdecPush->arDec[i].ucVdecId == ucVdecId)
        {
            prVdec = &_prVdecPush->arDec[i];
            break;
        }
        //ASSERT(i == ucVdecId);
    }
    if(!prVdec)
    {
        ASSERT(0);
        return FALSE;
    }

    // DMX: (UINT8 u1Channel, UINT32 u4Rp, UINT32 u4FrameAddr)
    return DMX_UpdateVideoReadPointer(prVdec->ucChannelId, u4Rp, u4FrameAddr);
}
VOID VOID_VPUSH_SetSeamlessInfo(VOID* prdec,BOOL fgSeamlessPlay, UINT32 u4SeamlessWidth, UINT32 u4SeamlessHeight)
{
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo = NULL;
    
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return;
    }
    
    prVdec = (VDEC_T*)prdec;
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);

    if(prVdecEsInfo != NULL)
    {
        LOG(3,"VOID_VPUSH_SetSeamlessInfo , fgSeamlessPlay = %d, u4SeamlessWidth = %d, u4SeamlessHeight = %d\n",fgSeamlessPlay,u4SeamlessWidth,u4SeamlessHeight);
        
        //hard code
#if 1
        prVdecEsInfo->fgSeamlessPlay= fgSeamlessPlay;
        prVdecEsInfo->u4SeamlessWidth = u4SeamlessWidth;
        prVdecEsInfo->u4SeamlessHeight = u4SeamlessHeight;
#else
        prVdecEsInfo->fgSeamlessPlay= TRUE;
        prVdecEsInfo->u4SeamlessWidth = 1920;
        prVdecEsInfo->u4SeamlessHeight = 1080;
#endif
    }
}

/* customization
typedef enum MEDIA_3D_TYPES {
    MEDIA_3D_NONE                       =0x0;
    MEDIA_3D_SIDE_BY_SIDE_HALF =0x1;
    MEDIA_3D_SIDE_BY_SIDE_HALF_LR   = MEDIA_3D_SIDE_BY_SIDE_HALF;
    MEDIA_3D_SIDE_BY_SIDE_HALF_RL   =0x2;
    MEDIA_3D_TOP_AND_BOTTOM_HALF    =0x3;
    MEDIA_3D_BOTTOM_AND_TOP_HALF    =0x4;
    MEDIA_3D_CHECK_BOARD                   =0x5;
    MEDIA_3D_FRAME_SEQUENTIAL            =0x6;
    MEDIA_3D_COLUMN_INTERLEAVE          =0x7;
    MEDIA_3D_SIDE_BY_SIDE_LR               =0x8;
    MEDIA_3D_SIDE_BY_SIDE_RL               =0x9;
    MEDIA_3D_FRAME_PACKING                  =0xA;
    MEDIA_3D_FIELD_ALTERNATIVE             =0xB;
    MEDIA_3D_LINE_ALTERNATIVE               =0xC;
    MEDIA_3D_DUAL_STREAM                      =0xD;
    MEDIA_3D_2DTO3D                               =0xE;
}
*/
VOID VOID_VPUSH_Set3DType(VOID* prdec,UINT32 u43DType)
{
    VDEC_T *prVdec;
    UINT32 u43DResType = 0; //VDEC_3D_RESOLUTION_HALF
    
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;
    printf("\nVOID_VPUSH_Set3DType=%d\n",u43DType);
    switch (u43DType)
    {
      case 8:
          u43DResType = VDEC_3D_RESOLUTION_FULL;
      case 1:
          u43DType = VDEC_3D_SBS_LF;
          break;
          
      case 9:
          u43DResType = VDEC_3D_RESOLUTION_FULL;
      case 2:
          u43DType = VDEC_3D_SBS_RF;
          break;
          
      case 3:
          u43DType = VDEC_3D_TB_LF;
          break;
      case 4:
          u43DType = VDEC_3D_TB_RF;
          break;
      case 5:
          u43DType = VDEC_3D_CHECKER_BOARD;
          break;
	  case 13:
		  u43DType = VDEC_3D_DUAL_STREAM;
		  break;
      default:
          u43DType = 0;
          break;
    }
    
    if(u43DType > 0)    //avoid overwrite the value driver auto-detected
    {
        VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_3D_INFO, u43DType, u43DResType, 0);
    }
}
VOID _VPUSH_SetPicSz(VOID* prdec, UINT32 u4Width, UINT32 u4Height,UINT32 u4FrameRate)
{
    VDEC_T *prVdec;
    VDEC_SEQUENCE_DATA_T rVDecSeqData;
    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return;
    }
    prVdec = (VDEC_T*)prdec;

    x_memset(&rVDecSeqData, 0, sizeof(rVDecSeqData));
                
    prVdec->u4Width = u4Width;
    prVdec->u4Height = u4Height;

    LOG(1,"video codec frame rate = %d\n",u4FrameRate);
    
    switch(u4FrameRate)
    {
        case 15:
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_15;
            prVdec->u4Rate = 1500;
            break;
        case 23 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_23_976;
            prVdec->u4Rate = 2397;
            break;
        case 24 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_24;
            prVdec->u4Rate = 2400;            
            break;
        case 25 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_25;
            prVdec->u4Rate = 2500;            
            break;
        case 29 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_29_97;
            prVdec->u4Rate = 2997;            
            break;
        case 30 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_30;
            prVdec->u4Rate = 3000;            
            break;
        case 50 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_50;
            prVdec->u4Rate = 5000;            
            break;
        case 59 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_59_94;
            prVdec->u4Rate = 5994;            
            break;
        case 60 :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_60;
            prVdec->u4Rate = 6000;            
            break;
        default :
            prVdec->eFrameRate = VDEC_SRC_FRAME_RATE_VARIABLE;
            prVdec->u4Rate = u4FrameRate * 100;
    }

    rVDecSeqData.e_frame_rate = prVdec->eFrameRate;
    
    VDEC_SetMMParam(prVdec->ucVdecId, VDEC_MM_FRAME_RATE, (UINT32)rVDecSeqData.e_frame_rate, 0, 0);
}


BOOL _VPUSH_SetPushType(VOID* prdec, ENUM_VDEC_PUSH_FMT_T ePushFmt)
{
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec is null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prVdec->ePushFmt = ePushFmt;

    return TRUE;
}


BOOL _VPUSH_AllocVFifo(VOID* prdec,
    UINT32 *pu4BufSA, UINT32 u4Size, ENUM_VDEC_FMT_T eFmt)
{
    UINT32 i;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;
    prVdec->eFmt = eFmt;
    
    if(prVdec->u4AllocBufCnt == 0)
    {
        if(!_VPUSH_AllocDmx(prVdec))
        {
            LOG(3, "%s(%d): _VPUSH_AllocDmx fail\n", __FUNCTION__, __LINE__);
            ASSERT(0);
        }
    }

    if(prVdec->fgIsSecureInput && !prVdec->fgGstPlay)
    {
#ifdef LINUX_TURNKEY_SOLUTION
        prVdec->u4AllocBufCnt ++;
        *pu4BufSA=(UINT32)u4SecureBufferAlloc((size_t)u4Size);
#endif
        return TRUE;        
    }

    for(i=0;i<prVdec->u4BufCnt;i++)
    {
        if(!prVdec->arBufInfo[i].fgUsed)
        {
            prVdec->arBufInfo[i].fgUsed = TRUE;
            prVdec->u4AllocBufCnt++;
            break;
        }
    }
    if(i >= prVdec->u4BufCnt)
    {
        LOG(3, "%s(%d): (i >= prVdec->u4BufCnt)\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    *pu4BufSA = prVdec->arBufInfo[i].u4BufAddr;
    LOG(1, "_VPUSH_AllocVFifo, u4Size: %d, u4BufSize: %d\n", u4Size, prVdec->arBufInfo[i].u4BufSize);
    if(u4Size > prVdec->arBufInfo[i].u4BufSize)
    {
        LOG(0, "[warning] input port buf size %d > feeder buf size: %d\n", u4Size, prVdec->arBufInfo[i].u4BufSize);
    }
    //ASSERT(u4Size <= prVdec->arBufInfo[i].u4BufSize);
    //prVdec->eFmt = eFmt;
    return TRUE;
}

BOOL _VPUSH_FreeVFifo(VOID* prdec, UINT32 u4BufSA)
{
    UINT32 i;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    prVdec = (VDEC_T*)prdec;

    if(prVdec->fgIsSecureInput && !prVdec->fgGstPlay)
    {
#ifdef LINUX_TURNKEY_SOLUTION
        if(bSecureBufferFree((UINT32)u4BufSA))
        {
            LOG(0, "bSecureBufferFree, ret fail\n");
        }
        else
        {
            prVdec->u4AllocBufCnt --;
        }
#endif
    }
    else
    {        
        LOG(2, "_VPUSH_FreeVFifo, addr:0x%08x\n", u4BufSA);
        for(i=0;i<prVdec->u4BufCnt;i++)
        {
            if((prVdec->arBufInfo[i].fgUsed) &&
               (prVdec->arBufInfo[i].u4BufAddr == u4BufSA))
            {
                prVdec->arBufInfo[i].fgUsed = FALSE;
                prVdec->u4AllocBufCnt --;
                break;
            }
        }
    }
    
    if(prVdec->u4AllocBufCnt == 0)
    {
        // release dmx
        if(!_VPUSH_ReleaseDmx(prdec))
        {
            LOG(3, "%s(%d): _VPUSH_ReleaseDmx fail\n", __FUNCTION__, __LINE__);
            ASSERT(0);
        }
    }
    return TRUE;
}


VOID _VPUSH_PushLoop(VOID* pvArg)
{
    ENUM_VPUSH_MSG_T eMsg;
    //VDEC_MSG_INTO_T rBytesInfo;
    //UINT16 u2MsgQIdx;
    UCHAR ucVPushId;
    VDEC_T *prVdec;
    UINT32 u4DmxAvailSize;
    #ifdef ENABLE_MULTIMEDIA
    UINT32 u4CurRptr, u4VFifoStart = 0, u4VFifoEnd = 0;
    IMG_LIB_FILL_RING_BUF_T rStreamBufInfo;
    INT32 i4Ret;
    #endif
    BOOL fgEsmQFull;
    BOOL fgEsmQEmpty;

    if(pvArg == NULL)
    {
        ASSERT(!pvArg);
        ucVPushId = ES0;
        LOG(1, "_VPUSH_PushLoop(): (pvArg == NULL)\n");
        return;
    }
    else
    {
        ucVPushId = *(UCHAR*)pvArg;
        if(ucVPushId >= VDEC_PUSH_MAX_DECODER)
        {
            LOG(1, "_VPUSH_PushLoop(): (ucVPushId >= VDEC_PUSH_MAX_DECODER)\n");
            return;
        }
    }
    prVdec = (VDEC_T*)&_prVdecPush->arDec[ucVPushId];

    while (1)//!prVdec->fgGoDead)
    {
        /*
        if(prVdec->eCurState == VPUSH_ST_STOP)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }
        else if(prVdec->eCurState == VPUSH_ST_IDLE)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }
        else
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, FALSE);
        }
        */

        eMsg = _VPUSH_ReceiveMsg(prVdec, FALSE);
        if(eMsg == VPUSH_MSG_NONE)
        {
            eMsg = _VPUSH_ReceiveMsg(prVdec, TRUE);
        }

        if((eMsg == VPUSH_MSG_DATA) && (prVdec->eCurState != VPUSH_ST_STOP))
        {
            fgEsmQFull = FALSE;
            fgEsmQEmpty = FALSE;
            #ifdef ENABLE_MULTIMEDIA
            if ((prVdec->eFmt == VDEC_FMT_JPG)||(prVdec->eFmt == VDEC_FMT_PNG))
            {
                if(prVdec->rMsg.u.rBytesInfo.fgEos)
                {
                    LOG(6, "--- Rcv EOS\n");
                }
                x_memset((void*)&rStreamBufInfo, 0, sizeof(rStreamBufInfo));

                if(!prVdec->fgNonFirst || !prVdec->fgInputBufReady)
                {
                    prVdec->fgNonFirst = TRUE;
                    u4DmxAvailSize = prVdec->u4FifoEnd - prVdec->u4FifoStart;
                }
                //TODO: IMG_GetDecState(0)
                else if((IMG_GetDecState(0) == IMG_RELEASE) && 
                    (!prVdec->rMsg.u.rBytesInfo.fgEos))   //img driver not ready
                {
                    i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&prVdec->rMsg,
                        sizeof(VDEC_MSG_INTO_T), 254);
                    UNUSED(_VPUSH_GetMsgCountInQ((VOID*)prVdec));
                    if(i4Ret != OSR_OK)
                    {
                        ASSERT(0);
                        LOG(3, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
                        continue;
                    }
                    x_thread_delay(10);
                    continue;
                }
                else
                {
                    //TODO: IMG_GetBSBufInfo(0,&rStreamBufInfo)
                    u4DmxAvailSize = IMG_GetBSBufInfo(0,&rStreamBufInfo);
                }
                if(rStreamBufInfo.rEntry2.bValid)
                {
                    u4CurRptr = (UINT32)rStreamBufInfo.rEntry2.pvStartAddr + (UINT32)rStreamBufInfo.rEntry2.u4RequiredLen + 64;
                }
                else if(rStreamBufInfo.rEntry1.bValid)
                {
                    u4CurRptr = (UINT32)rStreamBufInfo.rEntry1.pvStartAddr + (UINT32)rStreamBufInfo.rEntry1.u4RequiredLen + 64;
                }
                else
                {
                    u4CurRptr = 0;
                }
                if(u4CurRptr > 0)
                {
                    if(prVdec->u4VFifoAddr && prVdec->u4VFifoSize)
                    {
                        //for JPEG
                        u4VFifoStart=VIRTUAL(prVdec->u4VFifoAddr);
				  	    u4VFifoEnd=u4VFifoStart+prVdec->u4VFifoSize;
                        //---for JPEG
                        u4CurRptr = (u4CurRptr >= u4VFifoEnd)?(u4CurRptr - (u4VFifoEnd - u4VFifoStart)):u4CurRptr;
                        if (!VDEC_SetRptr(prVdec->ucVdecId, u4CurRptr, u4CurRptr))
                        {
                            //LOG(0, "-");
                        }
                    }
                }
            }
            else
            #endif
            {
                UINT16 u2QueueSize, u2MaxQueueSize;                
                VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep = NULL;
                u4DmxAvailSize = DMX_MUL_GetEmptySize(
                    prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid);                
                prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
                if(!prVdecEsInfoKeep)
                {
                    LOG(3, "%s(%d): prVdecEsInfoKeep null\n", __FUNCTION__, __LINE__);
                    ASSERT(0);
                }
                VDEC_GetQueueInfo(prVdec->ucVdecId, &u2QueueSize, &u2MaxQueueSize);                
                if(prVdecEsInfoKeep->eVPushPlayMode != VDEC_PUSH_MODE_TUNNEL)
                {
                    if(u2QueueSize > (u2MaxQueueSize - 100))
                    {
                        fgEsmQFull = TRUE;
                    }
                }
                
                if (u2QueueSize == 0)
                {
                    fgEsmQEmpty = TRUE;
                }
            }
            
            if((u4DmxAvailSize < prVdec->rMsg.u.rBytesInfo.u4BytesSize + 32) || fgEsmQFull)
            {
                if(fgEsmQEmpty)
                {
                    LOG(2, "Auto flush dmx vfifo %d < %d\n",u4DmxAvailSize,prVdec->rMsg.u.rBytesInfo.u4BytesSize);
                    UNUSED(_VPUSH_GetMsgCountInQ(prVdec));
                    DMX_MM_FlushBuffer(prVdec->u1DmxPid);
                    _VPUSH_FlushEsmQ(prVdec->ucVdecId);
                    //VDEC_ReleaseDispQ(prVdec->ucVdecId);
                    //x_thread_delay(10);
                    //continue;
                }
                
                LOG(5, "Vfifo is full!, size: %d, pts: 0x%08x, fgEsmQFull:%d\n", prVdec->rMsg.u.rBytesInfo.u4BytesSize, (UINT32)(prVdec->rMsg.u.rBytesInfo.u8BytesPTS), fgEsmQFull);

                if(prVdec->fgGstPlay)
                {
                    i4Ret = x_msg_q_send(prVdec->hMsgQ, (void *)&prVdec->rMsg,sizeof(VDEC_MSG_INTO_T), 254);
                    UNUSED(_VPUSH_GetMsgCountInQ((VOID*)prVdec));
                    if(i4Ret != OSR_OK)
                    {
                        ASSERT(0);
                        LOG(0, "%s(%d): i4Ret(%d))\n", __FUNCTION__, __LINE__, i4Ret);
                    }
                    x_thread_delay(100);
                }
                else
                {
                	prVdec->fgFifoFull = TRUE;

	                if(!_VPUSH_PutDataDone(prVdec, prVdec->rMsg.u.rBytesInfo.u4BytesAddr))
	                {
	                    LOG(3, "%s(%d): _VPUSH_PutDataDone fail\n", __FUNCTION__, __LINE__);
	                }
                }

                continue;
            }

            if(!_VPUSH_MoveData(prVdec, &prVdec->rMsg.u.rBytesInfo))
            {
                LOG(3, "%s(%d): _VPUSH_MoveData fail\n", __FUNCTION__, __LINE__);
                if(prVdec->fgGstPlay==FALSE)
                {
                    VERIFY(x_sema_unlock(prVdec->hMoveEsmQSema) == OSR_OK);
                }
            }

            if(prVdec->rMsg.u.rBytesInfo.fgMoveBufDone)
            {
                if(!_VPUSH_PutDataDone(prVdec, prVdec->rMsg.u.rBytesInfo.u4BytesTag))
                {
                    LOG(3, "%s(%d): _VPUSH_PutDataDone fail\n", __FUNCTION__, __LINE__);
                }
            }
        }
    }
}

#if defined(CC_DTV_SUPPORT_LG)
static VOID _VPUSH_CheckData(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;

    if (NULL == pv_tag)
    {
        LOG(1, "%s: pv_tag is NULL\n", __FUNCTION__);
        ASSERT(0);
        return;
    }

    prVdec = (VDEC_T *)pv_tag;

	if(prVdec->ucVdecId>=VDEC_PUSH_MAX_DECODER)
	{
        LOG(1,"_VPUSH_CheckData vdec id not seted \n");
		return;
	}
	
    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if (NULL == prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

    if (SWDMX_SRC_TYPE_NETWORK_SKYPE == prVdecEsInfo->eMMSrcType)
    {
        // From LG's request, skype do not check underrun
    }
    else if (prVdec->fgGotEos)
    {
        // From LG's request, do not check underrun after EOS is got
    }
    else if (VPUSH_ST_PLAY == prVdec->eCurState
        && prVdec->u8TotalPushSizeBak == prVdec->u8TotalPushSize)
    {
        UINT16 u2QueueSize, u2MaxQueueSize;
		UINT16 u2Thrd;

		VDEC_GetQueueInfo(prVdec->ucVdecId, &u2QueueSize, &u2MaxQueueSize);

		u2Thrd = (VDEC_3D_MVC == prVdecEsInfo->e3DType)? 16 : VPUSH_MSG_UNDERRUN_THRD;

        if (u2QueueSize < u2Thrd)
        {
            if (prVdec->rInpStrm.fnCb.pfnVdecUnderrunCb)
            {
                prVdec->rInpStrm.fnCb.pfnVdecUnderrunCb(prVdec->rInpStrm.fnCb.u4VdecUnderrunTag);
            }
            LOG(1, "[OMX%d]Underrun  thrd = %d ap = %d!!!\n", prVdec->ucVPushId, u2Thrd, prVdecEsInfo->eMMSrcType);

        }
    }

    prVdec->u8TotalPushSizeBak = prVdec->u8TotalPushSize;

    UNUSED(pt_tm_handle);
}
#endif

UINT32 _VPUSH_GetDecoderCnt(void)
{
    return VDEC_PUSH_MAX_DECODER;
}

VOID* _VPUSH_AllocVideoDecoder(ENUM_VDEC_FMT_T eFmt, UCHAR ucVdecId)
{
    INT32 i = VDEC_PUSH_MAX_DECODER;
    //UCHAR ucVPushId = VDEC_PUSH_ES_START;
    VDEC_T *prVdec=NULL;
        
    if(ucVdecId < VDEC_PUSH_MAX_DECODER)
    {
        if((_prVdecPush->fgOccupied[ucVdecId] == FALSE) && (_VPUSH_GetDecoderStatus(ucVdecId) == VDEC_ST_STOP))
        {
            i = ucVdecId;
            _prVdecPush->arDec[i].ucVdecId = ucVdecId;
        }
    }
    else
    {
        for(i=0;i<VDEC_PUSH_MAX_DECODER;i++)
        {
            if((_prVdecPush->fgOccupied[i] == FALSE))
            {
                break;
            }
        }
    }

    if(i < VDEC_PUSH_MAX_DECODER)
    {
        _prVdecPush->fgOccupied[i] = TRUE;
        prVdec = &(_prVdecPush->arDec[i]);
    }
    else
    {
        LOG(1,"[VPUSH]_VPUSH_AllocVideoDecoder fail\n");
        return NULL;
    }
    
    LOG(1,"[VPUSH]_VPUSH_AllocVideoDecoder %d\n",i);

    FBM_SetFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_DECODE_ONLY);
    FBM_SetFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_NO_TIMEOUT);
#ifdef CC_VDEC_RM_SUPPORT
    LOG(0,"Define Enable CC_VDEC_RM_SUPPORT");
#endif

#ifdef CC_SUPPORT_VDEC_PREPARSE
    LOG(0,"Define Enable CC_SUPPORT_VDEC_PREPARSE");
#endif

    prVdec->fgNonFirst = FALSE;
    prVdec->fgInputBufReady = FALSE;
    prVdec->eFmt = eFmt;
    prVdec->fgFirstVideoChunk = TRUE;
    prVdec->u8TotalPushSize = 0;
    prVdec->fgInsertStartcode = FALSE;
    prVdec->fgGotEos = FALSE;
    prVdec->fgDecoderCalPts = FALSE;

      //  #VPUSH_RV_SLICE_BUF#
    prVdec->pu4VPSliceSzBuf = NULL;
    prVdec->dwCrntFrmSliceSzWp = 0;
    prVdec->u1CrntFrmSliceNum = 0;
    prVdec->dwVPSliceSzBufWp =0 ;
    prVdec->dwVPSliceSzBufWpBak= 0;
    prVdec->dwFirst4CC = 0 ;
    prVdec->u4VFifoAddr=0;
    prVdec->u4VFifoSize=0;
    prVdec->u4AllocBufCnt=0;
    prVdec->fgIsSecureInput = FALSE;    
    prVdec->fgLowLatencyMode = FALSE;
    prVdec->fgGstPlay=TRUE;

	x_memset(&(prVdec->rInpStrm.fnCb), 0, sizeof(VDEC_PUSH_CB_T));
#if defined(CC_DTV_SUPPORT_LG)
    if (!VPUSH_IS_PIC(eFmt))
    {
        if (NULL_HANDLE != prVdec->hDataTimer)
        {
            ASSERT(0);
            if (prVdec->fgDataTimerStart)
            {
                VERIFY(x_timer_stop(prVdec->hDataTimer) == OSR_OK);
                prVdec->fgDataTimerStart = FALSE;
            }

            VERIFY(x_timer_delete(prVdec->hDataTimer) == OSR_OK);
            prVdec->hDataTimer = NULL_HANDLE;
        }

        VERIFY(x_timer_create(&prVdec->hDataTimer) == OSR_OK);
        VERIFY(x_timer_start(prVdec->hDataTimer,
            VPUSH_DATA_TIMER_THRSD,
            X_TIMER_FLAG_REPEAT,
            _VPUSH_CheckData,
            (void*)prVdec) == OSR_OK);
        prVdec->fgDataTimerStart = TRUE;
    }
#endif

    if(!_VPUSH_AllocFeeder(prVdec))
    {
        LOG(0, "%s(L:%d) _VPUSH_AllocFeeder fail\n",
            __FUNCTION__, __LINE__);
        _prVdecPush->fgOccupied[i] = FALSE;
        return NULL;
    }

    // for RV move data and video callback sync!// #VPUSH_RV_SLICE_BUF#
    if (eFmt == VDEC_FMT_RV )
    {
        if(prVdec->hRvMoveSema==NULL_HANDLE)
        {
            VERIFY(x_sema_create(&prVdec->hRvMoveSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        }
    }
    else  // Other format,check whether the sema is deleted!
    {
        if (NULL_HANDLE != prVdec->hRvMoveSema)
        {
            VERIFY(x_sema_delete(prVdec->hRvMoveSema) == OSR_OK);
            prVdec->hRvMoveSema=NULL_HANDLE;
        }
    }

    return (VOID*)prVdec;
}

VOID _VPUSH_ReleaseVideoDecoder(VOID* prdec)
{
    VDEC_T *prVdec;
    VDEC_DEC_NFY_INFO_T rNfyInfo;
    VDEC_ES_INFO_KEEP_T *prVdecEsInfoKeep;

    if(!prdec)
    {
        LOG(3, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucVPushId >= VDEC_PUSH_MAX_DECODER)
    {
        // already released at stop?
        LOG(0, "%s(%d): ucVPushId 0xFF null\n", __FUNCTION__, __LINE__);
        return;
    }

#if defined(CC_DTV_SUPPORT_LG)
    if (prVdec->hDataTimer)
    {
        if (prVdec->fgDataTimerStart)
        {
            VERIFY(x_timer_stop(prVdec->hDataTimer) == OSR_OK);
            prVdec->fgDataTimerStart = FALSE;
        }

        VERIFY(x_timer_delete(prVdec->hDataTimer) == OSR_OK);
        prVdec->hDataTimer = NULL_HANDLE;
    }
#endif

    if(!_VPUSH_ReleaseFeeder(prVdec))
    {
        LOG(1, "%s(L:%d) _VPUSH_ReleaseFeeder fail\n",
            __FUNCTION__, __LINE__);
    }

      // #VPUSH_RV_SLICE_BUF#
    if (prVdec->hRvMoveSema)
    {
        VERIFY(x_sema_delete(prVdec->hRvMoveSema) == OSR_OK);
		prVdec->hRvMoveSema=NULL_HANDLE;
    }
     //re-init info
    prVdec->pu4VPSliceSzBuf = NULL;
    prVdec->dwCrntFrmSliceSzWp = 0;
    prVdec->u1CrntFrmSliceNum = 0;
    prVdec->dwVPSliceSzBufWp =0 ;
    prVdec->dwVPSliceSzBufWpBak= 0;
    prVdec->dwFirst4CC = 0 ;
    
    FBM_ClrFrameBufferGlobalFlag(0xFF, FBM_FLAG_FB_DECODE_ONLY);

	if(prVdec->ucVdecId<VDEC_PUSH_MAX_DECODER)
	{
		x_memset(&rNfyInfo, 0, sizeof(VDEC_DEC_NFY_INFO_T));
		rNfyInfo.pvTag = (void*)prdec;
		rNfyInfo.pfDecNfy = NULL;
		VDEC_SetDecNfy(prVdec->ucVdecId, &rNfyInfo);
	}
    _prVdecPush->fgOccupied[prVdec->ucVPushId] = FALSE;
    prVdec->fgFirstVideoChunk = TRUE;
    prVdec->ucVdecId = prVdec->ucVPushId;
    prVdec->ucChannelId = prVdec->ucVPushId;

#if 1 // inform VDEC_NotifyStop release done, keep this in the last!!!
    prVdecEsInfoKeep = _VDEC_GetEsInfoKeep(prVdec->ucVdecId);
    if (prVdecEsInfoKeep->hWaitStopSema)
    {
        INT32 i4Ret = x_sema_unlock(prVdecEsInfoKeep->hWaitStopSema);
        LOG(1, "vdec stop ready %d\n", i4Ret);
    }
    prVdecEsInfoKeep->eVPushPlayMode = VDEC_PUSH_MODE_UNKNOWN;

    if(prVdec->prDmxPool)
    {
        LOG(0,"[warning!]dmx not released, force do it\n");
        _VPUSH_ReleaseDmx(prdec);
    }
#endif

    prVdec->fgIsSecureInput = FALSE;
    return;
}

VOID _VPUSH_CreateFbgDone(UCHAR ucVdecId, VOID *pVoid)
{
    LOG(5,"_VPUSH_CreateFbgDone \n");
    return;
}

VOID _VPUSH_DecodeDone(UCHAR ucVdecId, VOID *pPicNfyInfo)
{
    UINT32 i;
    FBM_PIC_NTF_INFO_T *prPicNfyInfo;
    VDEC_T *prVdec = NULL;

    if(!pPicNfyInfo)
    {
        LOG(0, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    prPicNfyInfo = (FBM_PIC_NTF_INFO_T*)pPicNfyInfo;

    for(i=0;i<VDEC_PUSH_MAX_DECODER;i++)
    {
        if((_prVdecPush->arDec[i].ucVdecId == ucVdecId) &&
           (_prVdecPush->fgOccupied[i]))
        {
            prVdec = &_prVdecPush->arDec[i];
            break;
        }
        //ASSERT(i == ucVdecId);
    }
    if(!prVdec)
    {
        LOG(0, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }
    if(prVdec->eCurStateSync == VPUSH_ST_STOP)
    {
        LOG(0, "%s(%d): under stop\n", __FUNCTION__, __LINE__);
        return;
    }

    if(prVdec->rInpStrm.fnCb.pfnVdecDecodeDone)
    {
        prVdec->rInpStrm.fnCb.pfnVdecDecodeDone(
            prVdec->rInpStrm.fnCb.u4DecodeTag,
            prPicNfyInfo->ucFbgId,
            prPicNfyInfo->ucFbId,
            prPicNfyInfo->fgEos);
    }
    return;
}

VOID _VPUSH_ImgFrmDone(UCHAR ucVdecId, VOID *pNfyInfo)
{
    UINT32 i;
    VDEC_T *prVdec = NULL;

#if 0
    if(!pNfyInfo)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }
#endif

    for(i=0;i<VDEC_PUSH_MAX_DECODER;i++)
    {
        if(_prVdecPush->arDec[i].ucVdecId == ucVdecId)
        {
            prVdec = &_prVdecPush->arDec[i];
        }
        ASSERT(i == ucVdecId);
    }
    if(!prVdec)
    {
        LOG(1, "%s(%d): fail\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return;
    }

    if(prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone)
    {
        prVdec->rInpStrm.fnCb.pfnVdecImgFrmDone(
            prVdec->rInpStrm.fnCb.u4ImageTag,
            pNfyInfo);
    }
    return;
}


BOOL _VPUSH_GetDecSize(VOID* prdec, UINT64 *pu8DecSize, UINT64 *pu8UndecSize)
{
    UINT32 u4FifoSz, u4DmxAvailSize;
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucVPushId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVPushId);
        return FALSE;
    }
    if(prVdec->ucVdecId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVdecId);
        return FALSE;
    }
    if (0 == prVdec->u4VFifoSize)
    {
        LOG(1, "%s(%d): u4VFifoSize=0\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    u4FifoSz = prVdec->u4VFifoSize;
    u4DmxAvailSize = DMX_MUL_GetEmptySize(
        prVdec->u1DmxId, DMX_PID_TYPE_ES_VIDEO, prVdec->u1DmxPid);

    if (u4DmxAvailSize > u4FifoSz)
    {
        LOG(1, "empty size(%d) > fifo size(%d)\n", u4DmxAvailSize, u4FifoSz);
        ASSERT(0);
        *pu8UndecSize = 0;
    }
    else
    {
        *pu8UndecSize = (u4DmxAvailSize)? u4FifoSz - u4DmxAvailSize : 0;
    }

    if ((VPUSH_ST_PLAY == prVdec->eCurState)
    && (*pu8UndecSize)
    && prVdec->u8LastUndecodeSize
    && (prVdec->u8LastUndecodeSize == *pu8UndecSize))
    {
        prVdec->u4UnmoveCnt++;
        if (prVdec->u4UnmoveCnt > 5)
        {
            UINT16 u2QueueSize, u2MaxQueueSize;
            VDEC_GetQueueInfo(prVdec->ucVdecId, &u2QueueSize, &u2MaxQueueSize);
            if (0 == u2QueueSize)
            {
                LOG(2, "video underflow!!\n");
                *pu8UndecSize = 0;
            }
        }
    }
    else
    {
        prVdec->u4UnmoveCnt = 0;
        prVdec->u8LastUndecodeSize = *pu8UndecSize;
    }

    *pu8DecSize = (prVdec->u8TotalPushSize > *pu8UndecSize)
        ? prVdec->u8TotalPushSize - *pu8UndecSize
        : 0;

    LOG(6, "dec_sz: %lld,%lld\n", *pu8DecSize, *pu8UndecSize);

    return TRUE;
}


BOOL _VPUSH_ResetDecSize(VOID* prdec)
{
    VDEC_T *prVdec;

    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    prVdec->u8TotalPushSize = 0;

    return TRUE;
}


BOOL _VPUSH_SetAppType(VOID* prdec, char *pcAppType, UINT32 u4AppTypeLen)
{
#ifndef ENABLE_MULTIMEDIA
    UNUSED(prdec);
    UNUSED(pcAppType);
    UNUSED(u4AppTypeLen);
    return TRUE;
#else
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT32 i;

    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucVPushId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVPushId);
        return FALSE;
    }
    if(prVdec->ucVdecId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVdecId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdecEsInfo->eMMSrcType = SWDMX_SRC_TYPE_UNKNOWN;
    for (i = 0; i < sizeof(_arVdecAppType) / sizeof(_arVdecAppType[0]); i++)
    {
        if (0 == x_strncmp(pcAppType, _arVdecAppType[i].pcAppType, _arVdecAppType[i].u4AppTypeSize))
        {
            prVdecEsInfo->eMMSrcType = _arVdecAppType[i].eMMSrcType;
            if (SWDMX_SRC_TYPE_NETWORK_SKYPE == prVdecEsInfo->eMMSrcType)
            {
                LOG(6, "VPush(%d) Vdec(%d) skype\n", prVdec->ucVPushId, prVdec->ucVdecId);
            }
        }
    }

    return TRUE;
#endif
}


BOOL _VPUSH_SetContainerType(VOID* prdec, char *pcContainerType, UINT32 u4ContainerTypeLen, UINT32 u4Metadata)
{
#ifndef ENABLE_MULTIMEDIA
    UNUSED(prdec);
    UNUSED(pcContainerType);
    UNUSED(u4ContainerTypeLen);
    return TRUE;
#else
    VDEC_T *prVdec;
    VDEC_ES_INFO_T *prVdecEsInfo;
    UINT32 i;

    if(!prdec)
    {
        LOG(1, "%s(%d): prdec null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucVPushId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVPushId);
        return FALSE;
    }
    if(prVdec->ucVdecId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVdecId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec->u4ContainerMetadata = u4Metadata;
    prVdecEsInfo->eContainerType = SWDMX_FMT_UNKNOWN;
    prVdec->fgDecoderCalPts = FALSE;
    for (i = 0; i < sizeof(_arVdecContainerType) / sizeof(_arVdecContainerType[0]); i++)
    {
        if (0 == x_strncmp(pcContainerType, _arVdecContainerType[i].pcContainerType, _arVdecContainerType[i].u4ContainerTypeSize))
        {
            prVdecEsInfo->eContainerType = _arVdecContainerType[i].eContainerType;
            if (SWDMX_FMT_AVI == prVdecEsInfo->eContainerType)
            {
                prVdec->fgDecoderCalPts = TRUE;
                LOG(6, "VPush(%d) Vdec(%d) AVI Container\n", prVdec->ucVPushId, prVdec->ucVdecId);
            }
            else if (SWDMX_FMT_MKV == prVdecEsInfo->eContainerType)
            {
                if (!(prVdec->u4ContainerMetadata & VIDEO_MKVCodecData))
                {
                    LOG(6, "Found MKV without codec data. Enable decoder calculating PTS.");
                    prVdec->fgDecoderCalPts = TRUE;

                }
                LOG(6, "VPush(%d) Vdec(%d) MKV Container\n", prVdec->ucVPushId, prVdec->ucVdecId);
            }
            else if (SWDMX_FMT_RM == prVdecEsInfo->eContainerType)
            {
                LOG(6, "VPush(%d) Vdec(%d) RM Container %d\n", prVdec->ucVPushId, prVdec->ucVdecId, i);
            }
			else if (SWDMX_FMT_MPEG2_TS == prVdecEsInfo->eContainerType)
			{
                LOG(6, "VPush(%d) Vdec(%d) TS Container %d\n", prVdec->ucVPushId, prVdec->ucVdecId, i);
			}
            break;
        }
    }

    return TRUE;
#endif
}

BOOL _VPUSH_GetDisplayInfor(VOID* prdec, VDEC_DISP_FRAME_T *prDispFrame,UINT8 u1FbgId,UINT8 u1FbId)
{
    VDEC_T *prVdec;
    UINT32 u4Addr, u4AddrC;
    UINT32 u4FbWidth = 0;
    UINT32 u4FbHeight = 0;
    VDEC_ES_INFO_T *prVdecEsInfo;
    FBM_SEQ_HDR_T* prFbmSeqHdr = NULL;
    FBM_PIC_HDR_T* prFbmPicHdr = NULL;

    if((!prdec) || (!prDispFrame))
    {
        LOG(1, "%s(%d): prdec/prDispFrame null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    prVdec = (VDEC_T*)prdec;
    if(prVdec->ucVPushId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVPushId);
        return FALSE;
    }
    
    if(prVdec->ucVdecId >= VDEC_PUSH_MAX_DECODER)
    {
        LOG(1, "%s(%d): %d is invalid\n", __FUNCTION__, __LINE__, prVdec->ucVdecId);
        return FALSE;
    }

    prVdecEsInfo = _VDEC_GetEsInfo(prVdec->ucVdecId);
    if(!prVdecEsInfo)
    {
        LOG(1, "%s(%d): prVdecEsInfo null\n", __FUNCTION__, __LINE__);
        return FALSE;
    }

    if(prVdecEsInfo->ucFbgId == FBM_FBG_ID_UNKNOWN || u1FbgId==FBM_FBG_ID_UNKNOWN)
    {
        // before create FBG
        LOG(2, "%s(%d): ucFbgId 0xFF\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else if(u1FbgId!=prVdecEsInfo->ucFbgId)
    {
        // before create FBG
        LOG(2, "%s(%d): ucFbgId changed\n", __FUNCTION__, __LINE__);
        return FALSE;
    }
    else
    {
        if(FBM_FB_STATUS_LOCK != FBM_GetFrameBufferStatus(u1FbgId, u1FbId))
        {
            LOG(0, "%s(%d): [%d %d] status illegal\n", __FUNCTION__, __LINE__, u1FbgId, u1FbId);
            return FALSE;
        }
        // after create FBG
        FBM_GetFrameBufferAddr(prVdecEsInfo->ucFbgId,
            u1FbId, &u4Addr, &u4AddrC);
        prFbmPicHdr = FBM_GetFrameBufferPicHdr(prVdecEsInfo->ucFbgId, u1FbId);
        prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prVdecEsInfo->ucFbgId);
        FBM_GetFrameBufferSize(prVdecEsInfo->ucFbgId, &u4FbWidth, &u4FbHeight);

        if(prFbmPicHdr && prFbmSeqHdr && u4Addr && u4FbWidth && u4FbHeight)
        {            
            prDispFrame->u4Pitch = prFbmSeqHdr->u2LineSize;
            prDispFrame->u4Width = prFbmSeqHdr->u2HSize;
            prDispFrame->u4Height = prFbmSeqHdr->u2VSize;
            prDispFrame->u4FrameBufferStatus = 0;
            if (prFbmPicHdr->fgNotDisplay || FBM_ChkFrameBufferPicFlag(prVdecEsInfo->ucFbgId, u1FbId, FBM_MM_NOT_DISPLAY_FLAG))
            {
                prDispFrame->u4FrameBufferStatus = VDEC_FBSTSTUS_NOT_DISPLAY;
            }
            if (FBM_ChkFrameBufferPicFlag(prVdecEsInfo->ucFbgId, u1FbId, (FBM_MM_EOS_FLAG | FBM_MM_PSEUDO_EOS_FLAG)))
            {
                prDispFrame->fgEos = TRUE;
                LOG(3, "%s(%d): ucFbgId(0x%x) ucFbId(0x%x) fgEos\n",
                    __FUNCTION__, __LINE__, prVdecEsInfo->ucFbgId, u1FbId);
            }
            else
            {
                prDispFrame->fgEos = FALSE;
            }
            prDispFrame->fgCropping = prFbmPicHdr->fgCropping;
            prDispFrame->u4CropX = prFbmPicHdr->u4CropX;
            prDispFrame->u4CropY = prFbmPicHdr->u4CropY;
            prDispFrame->u4CropWidth = prFbmPicHdr->u4CropWidth;
            prDispFrame->u4CropHeight = prFbmPicHdr->u4CropHeight;

            prDispFrame->u4BufAddr = u4Addr;
            prDispFrame->u4BufAddrC=u4AddrC;
            prDispFrame->u4BufSize = (u4FbWidth * u4FbHeight);
            prDispFrame->u4BufWidth = u4FbWidth;
            prDispFrame->u4BufHeight = u4FbHeight;

            if(prFbmPicHdr->u4PicWidth && prFbmPicHdr->u4PicHeight && prFbmPicHdr->u4PicWidthPitch)
            {                
                prDispFrame->u4Pitch = prFbmPicHdr->u4PicWidthPitch;
                prDispFrame->u4Width = prFbmPicHdr->u4PicWidth;
                prDispFrame->u4Height = prFbmPicHdr->u4PicHeight;
            }
            if(prFbmSeqHdr->fgResizeSmallPic && prFbmPicHdr->u4PicWidthPitch > 0 &&
                prFbmPicHdr->u4PicWidth >0 && prFbmPicHdr->u4PicHeight > 0)
            {
                prDispFrame->u4Pitch = prFbmPicHdr->u4PicWidthPitch;
                prDispFrame->u4Width = prFbmPicHdr->u4PicWidth;
                prDispFrame->u4Height = prFbmPicHdr->u4PicHeight;
            }
            
            prDispFrame->u8Timestamp = prFbmPicHdr->u8PTS;

            if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_4_3)
            {
                prDispFrame->u4ParWidth = 1;
                prDispFrame->u4ParHeight = 1;
                prDispFrame->u4DarWidth = 4;
                prDispFrame->u4DarHeight = 3;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_16_9)
            {
                prDispFrame->u4ParWidth = 1;
                prDispFrame->u4ParHeight = 1;
                prDispFrame->u4DarWidth = 16;
                prDispFrame->u4DarHeight = 9;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_221_1)
            {
                prDispFrame->u4ParWidth = 1;
                prDispFrame->u4ParHeight = 1;
                prDispFrame->u4DarWidth = 221;
                prDispFrame->u4DarHeight = 100;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_10_11)
            {
                prDispFrame->u4ParWidth = 10;
                prDispFrame->u4ParHeight = 11;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_40_33)
            {
                prDispFrame->u4ParWidth = 40;
                prDispFrame->u4ParHeight = 33;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_16_11)
            {
                prDispFrame->u4ParWidth = 16;
                prDispFrame->u4ParHeight = 11;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_12_11)
            {
                prDispFrame->u4ParWidth = 12;
                prDispFrame->u4ParHeight = 11;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_3_2)
            {
                prDispFrame->u4ParWidth = 3;
                prDispFrame->u4ParHeight = 2;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_1_1)
            {
                prDispFrame->u4ParWidth = 1;
                prDispFrame->u4ParHeight = 1;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight * prFbmPicHdr->u4PicHeight;
            }
            else if(prFbmSeqHdr->ucAspRatInf == MPEG_ASPECT_RATIO_TRANSMIT)
            {
                prDispFrame->u4ParWidth = prFbmSeqHdr->u4AspectRatioWidth;
                prDispFrame->u4ParHeight = prFbmSeqHdr->u4AspectRatioHeight;
                prDispFrame->u4DarWidth = prFbmSeqHdr->u4AspectRatioWidth * prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prFbmSeqHdr->u4AspectRatioHeight * prFbmPicHdr->u4PicHeight;
            }
            else
            {
                prDispFrame->u4ParWidth = 1;
                prDispFrame->u4ParHeight = 1;
                prDispFrame->u4DarWidth = prDispFrame->u4ParWidth*prFbmPicHdr->u4PicWidth;
                prDispFrame->u4DarHeight = prDispFrame->u4ParHeight*prFbmPicHdr->u4PicHeight;
            }
        }
        else
        {
            LOG(3, "%s(%d): ucFbgId(0x%x) ucFbId(0x%x) addr unknown\n",
                __FUNCTION__, __LINE__, prVdecEsInfo->ucFbgId, u1FbId);
            return FALSE;
        }
    }

    LOG(9, "%s(%d): ucFbgId(0x%x) ucFbId(0x%x), Y 0x%x, C 0x%x\n",
        __FUNCTION__, __LINE__, prVdecEsInfo->ucFbgId, u1FbId, u4Addr, u4AddrC);

    return TRUE;
}

/*----------------------------------------------------------------------------------------
     //check super frame, and move super frame first
------------------------------------------------------------------------------------------*/
static  INT32 __vp9_move_special_frame(VDEC_T *vdec)
{
     UINT8 *p1Marker1;
     UINT8* pBufferStart = NULL;
     UINT32 BufferLength = 0;
     DMX_MM_DATA_T rDmxMMData;

     if (!vdec || !vdec->pCurrentInputFrame)
     {
         LOG(0, "Internal Error %s %d\n", __FUNCTION__, __LINE__);
         return MOVE_SPECIAL_DATA_FAIL;
     }

     pBufferStart = (UINT8 *)vdec->pCurrentInputFrame->u4BytesAddr;
     BufferLength = vdec->pCurrentInputFrame->u4BytesSize;

     p1Marker1 = (UINT8 *)pBufferStart + BufferLength  -1;

     if ((*p1Marker1 & 0xe0) == 0xc0)
     {
         UINT32 u4Frames, u4Mag, u4IndexSZ;
         u4Frames = (*p1Marker1 & 0x7) + 1;
         u4Mag = (((*p1Marker1) >> 3) & 0x3) + 1;
         u4IndexSZ = 2 + u4Mag * u4Frames;
         LOG(2, "%s %d 0x%x mag = %d frame = %d\n", __FUNCTION__, __LINE__, *p1Marker1, u4Mag, u4Frames);
         if (BufferLength >= u4IndexSZ)
         {
             UINT8 *p1Marker2;
             p1Marker2 = (UINT8 *)(pBufferStart + BufferLength - u4IndexSZ);
             LOG(2, "%s %d 0x%x\n", __FUNCTION__, __LINE__, *p1Marker2);
             if (*p1Marker2 == *p1Marker1)
             {

                 UINT32 i, j;
                 UINT8 *p1Info;
                 UINT8 *puFrame = pBufferStart;
                 p1Info = pBufferStart + BufferLength - u4IndexSZ + 1;

                 x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
                 rDmxMMData.u1Idx = vdec->u1DmxPid; 
                 rDmxMMData.u4BufStart = vdec->u4FifoStart;
                 rDmxMMData.u4BufEnd = vdec->u4FifoEnd;
                 rDmxMMData.fgEOS = vdec->pCurrentInputFrame->fgEos;  // Fixme Super frame do not set the EOS flag?
                 rDmxMMData.fgMoveComplete = FALSE;

                 for (i = 0; i < u4Frames; ++i)
                 {
                     UINT32 u4ThisSz = 0;
                     for (j = 0; j < u4Mag; ++j)
                     {
                         u4ThisSz |= (*p1Info++) << (j * 8);
                     }

                     LOG(2, "%s %d 0x%x SUPPER FRAME\n", __FUNCTION__, __LINE__, u4ThisSz);                     
                     rDmxMMData.u4StartAddr = (UINT32)(puFrame);
                     rDmxMMData.u4FrameSize = u4ThisSz;

                     #ifdef VDEC_PUSH_PTS_64_BITS
                     rDmxMMData.u4Pts = (UINT32)(vdec->pCurrentInputFrame->u8BytesPTS & (UINT64)0xFFFFFFFF);
                     rDmxMMData.u4Dts = (UINT32)((vdec->pCurrentInputFrame->u8BytesPTS >> 32) & (UINT64)0xFFFFFFFF);
                     #else
                     rDmxMMData.u4Pts = (UINT32)(vdec->pCurrentInputFrame->u8BytesPTS);
                     rDmxMMData.u4Dts = (UINT32)(vdec->pCurrentInputFrame->u8BytesPTS);
                     #endif

                     if (i == u4Frames - 1)
                     {
                         rDmxMMData.fgMoveComplete = TRUE;
                     }
                     _VPUSH_DmxMoveData(vdec, &rDmxMMData); 
                     puFrame += u4ThisSz;

                  }

                  return MOVE_SPECIAL_DATA_DO_NOT_CONTINUE;
                 }
            }
        }

     return MOVE_SPECIAL_DATA_CONTINUE;
}

CODEC_ENTRY_T s_codec_map[VDEC_FMT_MAX] = 
{
    {NULL, NULL, NULL},   // 0 VDEC_FMT_MPV
    {NULL, NULL, NULL},   // 1 VDEC_FMT_MP4
    {NULL, NULL, NULL},   // 2 VDEC_FMT_H264
    {NULL, NULL, NULL},   // 3 VDEC_FMT_WMV
    {NULL, NULL, NULL},   // 4 VDEC_FMT_H264VER
    {NULL, NULL, NULL},   // 5 VDEC_FMT_MJPEG
    {NULL, NULL, NULL},   // 6 VDEC_FMT_RV
    {NULL, NULL, NULL},   // 7 VDEC_FMT_AVS
    {NULL, NULL, NULL},   // 8 VDEC_FMT_VP6
    {NULL, NULL, NULL},   // 9 VDEC_FMT_VP8
    {NULL, NULL, NULL},   // 10 VDEC_FMT_RAW
    {NULL, NULL, NULL},   // 11 VDEC_FMT_OGG
    {NULL, NULL, NULL},   // 12 VDEC_FMT_JPG
    {NULL, NULL, NULL},   // 13 VDEC_FMT_PNG
    {NULL, NULL, NULL},   // 14 VDEC_FMT_H265
    {__vp9_move_special_frame, __vp9_process_dmx_move_data_cb, NULL},  // 15 VDEC_FMT_VP9
};
