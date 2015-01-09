//================================================================================================
//CMI+Rtk table 
#include "NTK_72314_Frc.h"
void csotNtk4k2kInit(void);													
void csotNtk4k2kSendMute(UINT8 bOn);

r4k2kHandler csotNtk4k2kHandler=
{
E_4K2K_CSOT_NTK,
#ifdef CC_MT5882
1, //I2C channel
#else
2, //I2C channel
#endif
0xff, //I2C addr
9,
true, //2D3D timing change
0, //cmd interal (not used)
500, //mute time
csotNtk4k2kInit,
NULL, //send cmd
com4k2kSendPanelType,
NULL, //send panelmode
csotNtk4k2kSendMute, //mute
NULL, //protection window
NULL, //fhd buf
0,
NULL, //4k2k buf
0,
NULL, //vdo mode buf
0,
NULL, //photo mode buf
0,
NULL, //Resume mode buf
0,
NULL,
0
};
                                         

void csotNtk4k2kInit(void)
{
    if (App_FRC_314_Init() != FRC_OK)
    {
		Printf("%s, App_FRC_314_Init Failed!\n", __FUNCTION__);
    }
}

void csotNtk4k2kSendMute(UINT8 bOn)   
{
		if (App_FRC_Set_314_VideoMute(bOn) != FRC_OK)
		{
			Printf("%s, App_FRC_Set_314_VideoMute Failed!\n", __FUNCTION__);
		}
}
//================================================================================================