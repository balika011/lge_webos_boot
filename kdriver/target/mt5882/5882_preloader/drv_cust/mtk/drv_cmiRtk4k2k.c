//================================================================================================
//CMI+Rtk table 

void cmiRtk4k2kInit(void);													
void cmiRtk4k2kSendMute(UINT8 bOn);
                                          
static UINT8 _au1cmiRtk4K2K30Buf[] = { 0x26, 0x38, 0x50, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x02 };

static UINT8 _au1cmiRtkFHD120Buf[] = { 0x26, 0x38, 0x50, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00 };
                                                 
static UINT8 _au1cmiRtkVdoModeBuf[] = { 0x26, 0x38, 0x50, 0x64, 0x00, 0x00, 0x00, 0x00, 0x09 };
static UINT8 _au1cmiRtkPhotoModeBuf[] = { 0x26, 0x38, 0x50, 0x64, 0x00, 0x00, 0x00, 0x00, 0x0a };
static UINT8 _au1cmiRtkModeResumeBuf[] = { 0x26, 0x38, 0x50, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00 };

static UINT8 _au1cmiRtkInit[] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x04 };  // 400ms mute time

#if 0
static UINT8 _au1MuteDefaultBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00 };  // default duration
static UINT8 _au1Mute100msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x01 };  // default duration + 100ms
static UINT8 _au1Mute200msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x02 };  // default duration + 200ms
static UINT8 _au1Mute300msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x03 };  // default duration + 300ms
static UINT8 _au1Mute500msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x05 };  // default duration + 500ms
static UINT8 _au1Mute1500msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x0f};  // default duration +1600ms
static UINT8 _au1Mute3200msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x20};  // default duration + 500ms
static UINT8 _au1Mute2000msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x14};  // default duration + 500ms
static UINT8 _au1Mute10000msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x64};  // default duration + 500ms
static UINT8 _au1Mute1000msBuf[9] = { 0x26, 0x38, 0x64, 0x75, 0x00, 0x00, 0x00, 0x00, 0x0a};  // default duration + 500ms
#endif

r4k2kHandler cmiRtk4k2kHandler=
{
E_4K2K_CMI_RTK,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
0xe0, //I2C addr
9,
FALSE, //2D3D timing change
700, //cmd interval ms
500, //mute time
cmiRtk4k2kInit,
com4k2kSendCmd,
com4k2kSendPanelType,
com4k2kSendPanelMode,
NULL, //mute
NULL, //protection window
_au1cmiRtkFHD120Buf, //fhd buf
sizeof(_au1cmiRtkFHD120Buf),
_au1cmiRtk4K2K30Buf, //4k2k buf
sizeof(_au1cmiRtk4K2K30Buf),
_au1cmiRtkVdoModeBuf, //vdo mode buf
sizeof(_au1cmiRtkVdoModeBuf),
_au1cmiRtkPhotoModeBuf, //photo mode buf
sizeof(_au1cmiRtkPhotoModeBuf),
_au1cmiRtkModeResumeBuf, //Resume mode buf
sizeof(_au1cmiRtkModeResumeBuf),
NULL,
0,
};
void cmiRtk4k2kInit(void)
{
    //Send init command.
	cmiRtk4k2kHandler.frc_send_cmd(_au1cmiRtkInit, sizeof(_au1cmiRtkInit));
}


//================================================================================================