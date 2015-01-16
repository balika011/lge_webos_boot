typedef enum
{
	RGBA8888,
	RGB565,
	RGBA2101010,
	YUVA2101010,
	YUV422,
	FORMAT_NONE = 0xff
}SCPIP_GFX_FORMAT;

typedef enum {
    GFX_MAIN_SCL        =0,
    GFX_SUB_SCL         =1,
    GFX_MAIN_DI         =2,
    GFX_SUB_DI          =3,
    GFX_MAIN_PDS        =4,
    GFX_SUB_PDS         =5,
    GFX_MON_OUT     =6,
    GFX_SRC_SEL_MAX     =7
} SCPIP_GFX_SRC_SEL;

typedef struct R3DGfxInf
{
	UINT8 u1PathIn;
	UINT8 u1Sel444;
	UINT8 u1Bypass3x3;

	UINT32 u4Width;
	UINT32 u4Height;

	UINT8 u1FrameNumber;
	UINT8 u1WriteMode;
	UINT8 u1WriteEnable;
	UINT8 u13D22DEnable;
	
	UINT32 u4BassAddr0;
	UINT32 u4BassAddrOffset;
	
	UINT32 u4WAddrHigh;
	UINT32 u4WAddrLow;

	SCPIP_GFX_FORMAT Format;
	UINT8 u1AlphaPos;
	UINT8 u1AlphaValue;
	
	UINT8 u1HBoundEn;
	UINT16 u2HBoundValue;

	UINT8 u1FrameDropEn;
	UINT32 u1DropM;
	UINT32 u1DropN;

	UINT8 u1MuxB;
	UINT8 u1MuxG;
	UINT8 u1MuxR;

}R3DGfxInf;

EXTERN void vScpip_GFX_init(void);
EXTERN UINT8 u1Scpip_GFX_Source_Sel(UINT8 bPath);
EXTERN UINT8 u1Scpip_GFX_Update_Resolution(UINT32 u4Width, UINT32 u4Height);
EXTERN void u1Scpip_GFX_Init_Dram(void);


EXTERN UINT8 u1Scpip_GFX_Set_Frame_Num(UINT8 u1Number);
EXTERN UINT8 u1Scpip_GFX_Set_Dram_Address(UINT32 u4BaseAddr, UINT32 u4FrameSize, UINT8 u1FrameNum);
EXTERN UINT8 u1Scpip_GFX_Set_Write_Mode(UINT8 u1Select);
EXTERN UINT8 u1Scpip_GFX_Write_Trigger(void);
EXTERN UINT8 u1Scpip_GFX_Write_Enable(UINT8 u1Enable);
EXTERN UINT8 u1Scpip_GFX_Set_Alpha(UINT8 u1Alpha);

//EXTERN UINT8 u1Scpip_GFX_Set_3DTo2D_En(UINT8 u1Enable);
EXTERN UINT8 u1Scpip_GFX_Set_3DTo2D_En(UINT8 u1VdpId, UINT8 u1Enable);
//EXTERN UINT8 u1Scpip_GFX_Set_Frame_Drop(UINT8 u1Enable,UINT8 u1DropN, UINT8 u1DropM);
EXTERN UINT8 u1Scpip_GFX_Set_Frame_Drop(UINT8 u1VdpId, UINT8 u1Enable, UINT8 u1DropM, UINT8 u1DropN);
EXTERN UINT8 u1Scpip_GFX_Set_Dump_Format(SCPIP_GFX_FORMAT Format);

EXTERN UINT8 u1Scpip_GFX_GetSource(void);
EXTERN UINT32 u4Scpip_GFX_GetWidth(void);
EXTERN UINT32 u4Scpip_GFX_GetHeight(void);
EXTERN UINT8 u1Scpip_GFX_GetCurrentFrameIdx(void);
EXTERN UINT8 u1Scpip_GFX_GetWriteStatus(void);
EXTERN void vScpip_GFX_Query(void);
EXTERN void vScpipDumpInit(void);
EXTERN void vScposGfxDumpStart(UINT8 u1Path);
EXTERN void vScposGfxDumpEnd(UINT8 u1Path);
EXTERN void u1Scpip_GFX_Write_Freeze(UINT8 u1Freeze);
EXTERN UINT8 u1Scpip_GFX_Set_3x3(void);
EXTERN void u1Scpip_GFX_Copy_Buffer(void);



