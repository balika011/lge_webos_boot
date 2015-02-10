//================================================================================================
//CMI+MST table 
#define CMI_MS_4K2K 1

void cmiMst4k2kInit(void);													
void cmiMst4k2kSendMute(UINT8 bOn);
void  cmiMst4k2ksendProtectWindows(UINT8 bWId, UINT16 u2Width, UINT16 u2Height,  UINT16 u2StartX,  UINT16 u2StartY);



static UINT8 _au1cmiMst4K2K30Buf[] = {
                                            0x40, 0x02, 0x0F, 0x31, 0x42, 0x00, 0x00,
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00};
static UINT8 _au1cmiMst4K2K30VB1Buf[] = {
                                            0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
                                            0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00 }; //color engine bypass

#if 0
static UINT8 _au1cmiMst4K2K60VB1Buf[] = {
                                            0x20, 0x01, 0x02, 0x00, 0x40, 0xFF, 0x00,
                                            0x80, 0x02, 0x00, 0x40, 0x62, 0x51, 0x73,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00}; //color engine bypass                                            
#endif
                                              
static UINT8 _au1cmiMstFHD120Buf[] ={       //120Hz
                                            0x40, 0x02, 0x0f, 0x31, 0x42, 0x00, 0x00,
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00};
static UINT8 _au1cmiMstFHD120VB1Buf[] = {
                                            0x80, 0x01, 0x02, 0x00, 0x00, 0x55, 0x00,
                                            0x80, 0x02, 0x00, 0x00, 0x02, 0x01, 0x03,                                            
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,  // video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00 }; //color engine bypass                                        
                                            
                                            
static UINT8 _au1cmiMstFHD60Buf[] = {	    //60Hz
                                            0x60, 0x02, 0x03, 0x21, 0x00, 0x00, 0x00,
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00};


static UINT8 _au1cmiMstFHD60VB1Buf[] = {	    //60Hz
                                            0x40, 0x01, 0x02, 0x00, 0x00, 0x11, 0x00,
                                            0x30, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
                                            0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                            0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,                                            
                                            0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,// video mode
                                            0x3D, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00}; //color engine bypass  

static UINT8 _au1cmiMstVdoModeBuf[] = {  
											 0x40, 0x02, 0x0f, 0x31, 0x42, 0x00, 0x00,
											 0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
											 0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
											 0x13, 0x01, 0xd0, 0x00, 0x00, 0x00, 0x00};

static UINT8 _au1cmiMstPhotoModeBuf[] ={ 
											 0x40, 0x02, 0x0f, 0x31, 0x42, 0x00, 0x00,
											 0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
											 0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00,
											 0x13, 0x01, 0xe0, 0x00, 0x00, 0x00, 0x00};


static UINT8 _au1cmiMstMuteBuf[]= { 0x43, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00};

/*
FHD/4k2k:
Main Menu:
43 05 01 00 F4 0E 0C
B3 06 01 00 60 08 0C

*/
 
static UINT8 _au1cmiMst4k2kBypWin[] = {
									 0x03, 0x05, 0x01, 0x00, 0xB8, 0x0D, 0xD0,
									 0x03, 0x06, 0x01, 0x00, 0x60, 0x08, 0x0C,

									}; 
//static UINT8 _au1cmiMstSatBuf[] = { 0x08, 0x05, 0x68, 0x00, 0x00, 0x00, 0x00};

static UINT8 _au1cmiMstGraphicMode[] = { 0x13, 0x01, 0x0f, 0x00, 0x00, 0x00, 0x00};

static UINT8 _au1cmiMstInit[] = {	
								0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, //Video mode
                              	0x33, 0x12, 0x01, 0x00, 0x00, 0x00, 0x00, // SR on
                               };

//60HZ LVDS
r4k2kHandler cmiMst4k2kHandler=
{
E_4K2K_CMI_MST,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
#if CMI_MS_4K2K
0x38, //I2C addr
#else
0x40
#endif
7,
TRUE, //2D3D timing change
20, //cmd interval ms >= 50ms
700, //mute time
cmiMst4k2kInit,
com4k2kSendCmd,
com4k2kSendPanelType,
com4k2kSendPanelMode,
cmiMst4k2kSendMute,
cmiMst4k2ksendProtectWindows,
_au1cmiMstFHD60Buf, //fhd buf
sizeof(_au1cmiMstFHD60Buf),
_au1cmiMst4K2K30Buf, //4k2k buf
sizeof(_au1cmiMst4K2K30Buf),
_au1cmiMstVdoModeBuf, //vdo mode buf
sizeof(_au1cmiMstVdoModeBuf),
_au1cmiMstPhotoModeBuf, //photo mode buf
sizeof(_au1cmiMstPhotoModeBuf),
_au1cmiMstFHD60Buf, //Resume mode buf
sizeof(_au1cmiMstFHD60Buf),
_au1cmiMstFHD120Buf,
sizeof(_au1cmiMstFHD120Buf),

};

//60HZ VB1
r4k2kHandler cmiMst60VB14k2kHandler=
{
E_4K2K_CMI_MST,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
#if CMI_MS_4K2K
0x38, //I2C addr
#else
0x40
#endif
7,
TRUE, //2D3D timing change
20, //cmd interval ms >= 50ms
700, //mute time
cmiMst4k2kInit,
com4k2kSendCmd,
com4k2kSendPanelType,
com4k2kSendPanelMode,
cmiMst4k2kSendMute,
cmiMst4k2ksendProtectWindows,
_au1cmiMstFHD60VB1Buf, //fhd buf
sizeof(_au1cmiMstFHD60VB1Buf),
_au1cmiMst4K2K30VB1Buf, //4k2k buf
sizeof(_au1cmiMst4K2K30VB1Buf),
_au1cmiMstVdoModeBuf, //vdo mode buf
sizeof(_au1cmiMstVdoModeBuf),
_au1cmiMstPhotoModeBuf, //photo mode buf
sizeof(_au1cmiMstPhotoModeBuf),
_au1cmiMstFHD60VB1Buf, //Resume mode buf
sizeof(_au1cmiMstFHD60VB1Buf),
_au1cmiMstFHD120VB1Buf,
sizeof(_au1cmiMstFHD120VB1Buf),
};

//120HZ LVDS
r4k2kHandler cmiMst1204k2kHandler=
{
E_4K2K_CMI_MST,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
0x38, //I2C addr
7,   
FALSE, //2D3D timing change
20, //cmd interval ms >= 50ms
700, //mute time
cmiMst4k2kInit,
com4k2kSendCmd,
com4k2kSendPanelType,
com4k2kSendPanelMode,
cmiMst4k2kSendMute,
cmiMst4k2ksendProtectWindows,
_au1cmiMstFHD120Buf,
sizeof(_au1cmiMstFHD120Buf),
_au1cmiMst4K2K30Buf, //4k2k buf
sizeof(_au1cmiMst4K2K30Buf),
_au1cmiMstVdoModeBuf, //vdo mode buf
sizeof(_au1cmiMstVdoModeBuf),
_au1cmiMstPhotoModeBuf, //photo mode buf
sizeof(_au1cmiMstPhotoModeBuf),
_au1cmiMstFHD120Buf,
sizeof(_au1cmiMstFHD120Buf),
_au1cmiMstFHD120Buf,
sizeof(_au1cmiMstFHD120Buf),

};

//120HZ VB1
r4k2kHandler cmiMst120VB14k2kHandler=
{
E_4K2K_CMI_MST,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
0x38, //I2C addr
7,   
FALSE, //2D3D timing change
20, //cmd interval ms >= 50ms
700, //mute time
cmiMst4k2kInit,
com4k2kSendCmd,
com4k2kSendPanelType,
com4k2kSendPanelMode,
cmiMst4k2kSendMute,
cmiMst4k2ksendProtectWindows,
_au1cmiMstFHD120VB1Buf,
sizeof(_au1cmiMstFHD120VB1Buf),
_au1cmiMst4K2K30VB1Buf, //4k2k buf
sizeof(_au1cmiMst4K2K30VB1Buf),
_au1cmiMstVdoModeBuf, //vdo mode buf
sizeof(_au1cmiMstVdoModeBuf),
_au1cmiMstPhotoModeBuf, //photo mode buf
sizeof(_au1cmiMstPhotoModeBuf),
_au1cmiMstFHD120VB1Buf,
sizeof(_au1cmiMstFHD120VB1Buf),
_au1cmiMstFHD120VB1Buf,
sizeof(_au1cmiMstFHD120VB1Buf),

};


void cmiMst4k2kInit(void)
{

	cmiMst4k2kSendMute(SV_ON);
	if(psCur4k2kHandler->u13D2DTimingChg || IS_VBYONE)
	{
		cmiMst4k2kHandler.frc_send_cmd(psCur4k2kHandler->pu1FhdBuf, psCur4k2kHandler->u1FhdBufSize);	
	}
    //Send init command.
	cmiMst4k2kHandler.frc_send_cmd(_au1cmiMstInit, sizeof(_au1cmiMstInit));
	cmiMst4k2kSendMute(SV_OFF);
}

void cmiMs4k2GraphicMode(UINT8 bOnOff)
{

	_au1cmiMstGraphicMode[2] = (bOnOff == 1)? 0xF0 : 0 ;
	cmiMst4k2kHandler.frc_send_cmd(_au1cmiMstGraphicMode, sizeof(_au1cmiMstGraphicMode));
}



void cmiMst4k2kSendMute(UINT8 bOn)   
{
	_au1cmiMstMuteBuf[2]= bOn ;
	 cmiMst4k2kHandler.frc_send_cmd(_au1cmiMstMuteBuf, sizeof(_au1cmiMstMuteBuf));

}

void  cmiMst4k2ksendProtectWindows(UINT8 bWId, UINT16 u2Width, UINT16 u2Height,  UINT16 u2StartX,  UINT16 u2StartY)
{
	UINT8  u1CmdLen ;	
	UINT16 u2HStart, u2HEnd, u2VStart, u2VEnd ;
	UINT8 u1Enable;
	
	//HStart/HEnd
	if((u2Width == 0) || (u2Height == 0))
	{
		u1Enable = u2HStart = u2HEnd = u2VStart = u2VEnd = 0 ;
		
	}
	else
	{
		u2HStart = u2StartX ;
		u2HEnd = (u2StartX + u2Width)  ;	
		u2VStart = u2StartY;
		u2VEnd = (u2StartY + u2Height)  ;	
		u1Enable = 1 ;
	}
	u1CmdLen = cmiMst4k2kHandler.u1CmdLen ;
	_au1cmiMst4k2kBypWin[2] =	(bWId << 1) | u1Enable ;
	_au1cmiMst4k2kBypWin[3] = (u2HStart >> 8) & 0xff ; //Hi-Byte
	_au1cmiMst4k2kBypWin[4] = u2HStart & 0xff ;	//Low-Byte
	_au1cmiMst4k2kBypWin[5] = (u2HEnd >> 8) & 0xff ; //Hi-Byte
	_au1cmiMst4k2kBypWin[6] = u2HEnd & 0xff ;	//Low-Byte	
	
	_au1cmiMst4k2kBypWin[u1CmdLen+2] =	(bWId << 1) | u1Enable ;
	_au1cmiMst4k2kBypWin[u1CmdLen+3] = (u2VStart >> 8) & 0xff ; //Hi-Byte
	_au1cmiMst4k2kBypWin[u1CmdLen+4] = u2VStart & 0xff ;	//Low-Byte
	_au1cmiMst4k2kBypWin[u1CmdLen+5] = (u2VEnd >> 8) & 0xff ; //Hi-Byte
	_au1cmiMst4k2kBypWin[u1CmdLen+6] = u2VEnd & 0xff ;	//Low-Byte		
	
	cmiMst4k2kHandler.frc_send_cmd(_au1cmiMst4k2kBypWin, sizeof(_au1cmiMst4k2kBypWin));


}

//End of CMO+ MST

//================================================================================================