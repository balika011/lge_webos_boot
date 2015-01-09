
#include "hw_scpos.h"
#include "hw_psc.h"
#include "drv_scaler_psc.h"


#if 0
/*********************************************************************
 * old Scaling factor & Phase
 ********************************************************************/
void vPscSetHInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	PSC_WRITE_FLD(PSCCTRL_00, u4InitSign, PSC_H_PHASE_INIT_SIGN);
	PSC_WRITE_FLD(PSCCTRL_00, u4InitIntg, PSC_H_PHASE_INIT_INTG);
	PSC_WRITE_FLD(PSCCTRL_00, u4InitFrac, PSC_H_PHASE_INIT_FRAC);
}
void vPscSetHDeltaPhase(UINT32 u4DeltaIntg, UINT32 u4DeltaFrac)
{
	PSC_WRITE_FLD(PSCCTRL_01, u4DeltaIntg, PSC_H_PHASE_DELTA_INTG);
	PSC_WRITE_FLD(PSCCTRL_01, u4DeltaFrac, PSC_H_PHASE_DELTA_FRAC);
}
#endif

/*********************************************************************
 * @brief Nonlinear scaling on off 
 * @param: u4OnOff 1: ON / 0: OFF
 * 		   u4AddMinusSel: 1: side delta phase < middle delta phase 
 *						  0: side delta phase > middle delta phase
 ********************************************************************/
#if 0
void vPscNonlinearOnOff(UINT8 bOnOff, UINT32 u4AddMinusSel)
{
	PSC_WRITE_FLD(PSCCTRL_05, bOnOff, PSC_NONL_H_ENABLE);
	PSC_WRITE_FLD(PSCCTRL_05, u4AddMinusSel, PSC_NONL_H_ADD_MINUS_SEL);
}
void vPscSetNonlinearHDeltaPhase(UINT32 u4DeltaIntg, UINT32 u4DeltaFrac)
{
	PSC_WRITE_FLD(PSCCTRL_06, u4DeltaIntg, PSC_NONL_H_PHASE_DELTA);
	PSC_WRITE_FLD(PSCCTRL_06, u4DeltaFrac, PSC_NONL_H_PHASE_DELTA_FLOAT);
}
void vPscSetNonlinearHPos(UINT32 u4StartPos, UINT32 u4StopPos, UINT32 u4MidPos)
{
	PSC_WRITE_FLD(PSCCTRL_05, u4StartPos, PSC_NONL_H_START_POS);
	PSC_WRITE_FLD(PSCCTRL_05, u4StopPos, PSC_NONL_H_STOP_POS);
	PSC_WRITE_FLD(PSCCTRL_07, u4MidPos, PSC_NONL_H_MIDDLE_POS);
}
#endif

void vPscSetInputWidthHeight(UINT32 u4Width, UINT32 u4Height)
{
	PSC_WRITE_FLD(PSCCTRL_09, u4Width, PSC_INPUT_WIDTH);
	PSC_WRITE_FLD(PSCCTRL_09, u4Height, PSC_INPUT_HEIGHT);
}
void vPscSetOutputWidthHeight(UINT32 u4Width, UINT32 u4Height)
{
	PSC_WRITE_FLD(PSCCTRL_11, u4Width, PSC_OUTPUT_WIDTH);
	PSC_WRITE_FLD(PSCCTRL_11, u4Height, PSC_OUTPUT_HEIGHT);
}
void vPscSetPanelWidth(UINT32 u4Width)
{
	PSC_WRITE_FLD(PSCSYS_02, u4Width, PSC_PNL_WIDTH);
	PSC_WRITE_FLD(PSCSYS_2C, u4Width, PSC_SET_DATA_HACT);		
}

void vPscSetPanelHeight(UINT32 u4Height)
{
	PSC_WRITE_FLD(PSCSYS_02, u4Height, PSC_PNL_HEIGHT);
}

void vPscSetPanelHTotal(UINT32 u4HTotal)
{
	PSC_WRITE_FLD(PSCSYS_03, u4HTotal, PSC_PNL_HTOTAL);
	PSC_WRITE_FLD(PSCSYS_2C, (u4HTotal-1), PSC_SET_DATA_HTOT);	
}

void vPscSetPanelVTotal(UINT32 u4VTotal)
{
	PSC_WRITE_FLD(PSCSYS_03, u4VTotal, PSC_PNL_VTOTAL);
	PSC_WRITE_FLD(PSCSYS_2D, (u4VTotal-1), PSC_SET_DATA_VTOT);		
}

UINT32 u4PscGetPanelWidth(void)
{
	return PSC_READ_FLD(PSCSYS_02, PSC_PNL_WIDTH);
}

UINT32 u4PscGetPanelHeight(void)
{
	return PSC_READ_FLD(PSCSYS_02, PSC_PNL_HEIGHT);
}

UINT32 u4PscGetPanelHTotal(void)
{
	return PSC_READ_FLD(PSCSYS_03, PSC_PNL_HTOTAL);
}

UINT32 u4PscGetPanelVTotal(void)
{
	return PSC_READ_FLD(PSCSYS_03, PSC_PNL_VTOTAL);
}

UINT32 u4PscGetOutputHActiveStatus(void)
{
    return PSC_READ_FLD(PSCSYS_2B, PSC_STA_OUT_MEASURE_HACTIVE);
}

UINT32 u4PscGetOutputVActiveStatus(void)
{
    return PSC_READ_FLD(PSCSYS_2B, PSC_STA_OUT_MEASURE_VACTIVE);
}

UINT32 u4PscGetOutputHTotalStatus(void)
{
    return PSC_READ_FLD(PSCSYS_2A, PSC_STA_OUT_MEASURE_HTOTAL);
}

UINT32 u4PscGetOutputVTotalStatus(void)
{
    return PSC_READ_FLD(PSCSYS_2A, PSC_STA_OUT_MEASURE_VTOTAL);
}

UINT16 u2PscGetPanelVCNTStatus(void)
{
    return PSC_READ_FLD(PSCSYS_20, PSC_STA_V_CNT);
}

void vPscSetPanelHSyncFp(UINT32 u4FrontPorch)
{
	PSC_WRITE_FLD(PSCSYS_04, u4FrontPorch, PSC_PNL_FRNT_HPORCH);
}

void vPscSetPanelVSyncFp(UINT32 u4FrontPorch)
{
	PSC_WRITE_FLD(PSCSYS_05, u4FrontPorch, PSC_PNL_FRNT_VPORCH);
}

void vPscSetPanelHSyncBp(UINT32 u4BackPorch)
{
	PSC_WRITE_FLD(PSCSYS_04, u4BackPorch, PSC_PNL_BACK_HPORCH);
}

void vPscSetPanelVSyncBp(UINT32 u4BackPorch)
{
	PSC_WRITE_FLD(PSCSYS_05, u4BackPorch, PSC_PNL_BACK_VPORCH);
}

UINT32 u4PscGetPanelHSyncFp(void)
{
	return PSC_READ_FLD(PSCSYS_04, PSC_PNL_FRNT_HPORCH);
}

UINT32 u4PscGetPanelVSyncFp(void)
{
	return PSC_READ_FLD(PSCSYS_05, PSC_PNL_FRNT_VPORCH);
}

UINT32 u4PscGetPanelHSyncBp(void)
{
	return PSC_READ_FLD(PSCSYS_04, PSC_PNL_BACK_HPORCH);
}

UINT32 u4PscGetPanelVSyncBp(void)
{
	return PSC_READ_FLD(PSCSYS_05, PSC_PNL_BACK_VPORCH);
}

UINT32 vPscGetInputHactive(void)
{
	return PSC_READ_FLD(PSCSYS_28, PSC_STA_HACTIVE);
}

UINT32 vPscGetInputHtotal(void)
{
	return PSC_READ_FLD(PSCSYS_28, PSC_STA_HTOTAL);
}

UINT32 vPscGetInputVactive(void)
{
	return PSC_READ_FLD(PSCSYS_29, PSC_STA_VACTIVE);
}

UINT32 vPscGetInputVtotal(void)
{
	return PSC_READ_FLD(PSCSYS_29, PSC_STA_VTOTAL);
}

//0: vsync 1: vde
void vPscSelectOsdVsync(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_14, bOnOff, PSC_OSD_VSYNC_SEL);
}
//Vmsk crop for overscan
void vPscSetVmskCrop(UINT32 u4Start,UINT32 u4End)
{
	PSC_WRITE_FLD(PSCCTRL_19, u4Start, PSC_VMSK_STR_CROP_1);
	PSC_WRITE_FLD(PSCCTRL_19, u4End, PSC_VMSK_END_CROP_1);
}
//Hmsk crop for overscan
void vPscSetHmskCrop(UINT32 u4Start,UINT32 u4End)
{
	PSC_WRITE_FLD(PSCCTRL_19, u4Start, PSC_HMSK_STR_CROP_1);
	PSC_WRITE_FLD(PSCCTRL_19, u4End, PSC_HMSK_END_CROP_1);
}
void vPscSetHmskStartOfst(UINT32 u4HStart)
{
	PSC_WRITE_FLD(PSCCTRL_0D, u4HStart, PSC_HMSK_STR_OFST_1);
}
void vPscSetVmskStartOfst(UINT32 u4VStart)
{
	PSC_WRITE_FLD(PSCCTRL_0E, u4VStart, PSC_VMSK_STR_OFST_1);
}

void vPscSetMirrorEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCCTRL_17, bOnOff, PSC_MIRROR_EN);
}
void vPscSetFlipEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCCTRL_17, bOnOff, PSC_FLIP_EN);
}
#if 0
 //main path engine dispplay mode
 void vPscSetMainEngineDispModeEnable(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_ENG_DISP_MODE_1);
 }
 //pip path engine dispplay mode
 void vPscSetPipEngineDispModeEnable(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_ENG_DISP_MODE_2);
 }
 //linesync mode (main path only)
 void vPscSetLinesyncModeEnable(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_LINESYNC_MODE);
 }
 //scaler display mode enable
 void vPscSetScalerDispModeEnable(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_SCALER_DISP_MODE);
 }
#endif
 /*********************************************************************
 * Frame Track
 ********************************************************************/
 void vPscSetFrameTrackTargetDistance(UINT32 u4Distance)
 {
	 PSC_WRITE_FLD(PSCSYS_10, u4Distance, PSC_FRM_TRK_LINE);
 }
 //DDDS track signal inverse (fast/slow switch)
 void vPscSetDddsTrackInverse(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_11, bOnOff, PSC_DDDS_TRK_INV);
 }
 //DDDS track mechanism enable
 void vPscSetDddsFrameTrackEnable(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_11, bOnOff, PSC_FRM_TRK_DDDS_EN);
 }
 //apply frm trk result to next one frame only
 void vPscSetFrameTrackOnceOnly(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_11, bOnOff, PSC_FRM_TRK_ONCE_ONLY);
 }
 //00: vmsk, 01:vde  10:vsync
 void vPscSelectFrameTrackOut(UINT32 u4Select)
 {
	 PSC_WRITE_FLD(PSCSYS_11, u4Select, PSC_FRM_TRK_OUT_SEL);
 }
 //vsync in edge sel : 1:rise 0:fall
 void vPscSelectFrameTrackVsyncInEdge(UINT8 bOnOff)
 {
	 PSC_WRITE_FLD(PSCSYS_11, bOnOff, PSC_FRM_TRK_VIN_EDGE_SEL);
 }
 //use ddds to track frm with turbo speed
 void vPscSetDddsTrackFrameTurboSpeed(UINT32 u4Speed)
 {
	 PSC_WRITE_FLD(PSCSYS_14, u4Speed, PSC_DDDS_TURBO_RGN);
 }

 /**
 * @brief set frame track step (unit: line)
 * @when step is zero, frame track off
 */
 void vPscSetFrameTrackStep(UINT32 u4Step)
 {
	 PSC_WRITE_FLD(PSCSYS_14, u4Step, PSC_FRM_TRK_STEP);
 }
 
 UINT32 vPscGetFrameTrackLockOkStatus(void)
 {
	 return PSC_READ_FLD(PSCSYS_14, PSC_STA_FRM_TRK_LOCK_OK);
 }
 
 //when lock, tolerate the num of line change
 void vPscSetFrameTrackLockTolerateLineNum(UINT32 u4LineNum)
 {
	 PSC_WRITE_FLD(PSCSYS_15, u4LineNum, PSC_FRM_LOCK_TOR);
 }
 
 //frm track lock window (with target)
 void vPscSetFrameTrackLockWin(UINT32 u4LockWin)
 {
	 PSC_WRITE_FLD(PSCSYS_15, u4LockWin, PSC_FRM_LOCK_WIN);
 }
 
 //frm trk turgo region ref to target
 void vPscSetFrameTrackTurboRegion(UINT32 u4Region)
 {
	 PSC_WRITE_FLD(PSCSYS_27, u4Region, PSC_FRM_TRK_TURBO_RGN);
 }

#if 0
 /************************************************************
 * @u4Enable: vertical down scaler integral method enable 
 * @u4Ratio: vertical down scaler ratio for integral method.
 *           output_height * 2048 / input_height			 
 ************************************************************/
void vPscVdsIntegralEnbale(UINT8 bOnOff,UINT32 u4Ratio)
{
	PSC_WRITE_FLD(PSCCTRL_17, bOnOff, PSC_V_DS_INTEGRAL_EN);
	PSC_WRITE_FLD(PSCCTRL_17, u4Ratio, PSC_V_DS_INTEGRAL_RATIO);
}
 /************************************************************
 * @u4Enable: horizontal down scaler integral method enable 
 * @u4Ratio: horizontal down scaler ratio for integral method.
 *           output_width * 2048 / input_width		 
 ************************************************************/
void vPscHdsIntegralEnbale(UINT8 bOnOff,UINT32 u4Ratio)
{
	 PSC_WRITE_FLD(PSCCTRL_17, bOnOff, PSC_H_DS_INTEGRAL_EN);
	 PSC_WRITE_FLD(PSCCTRL_17, u4Ratio, PSC_H_DS_INTEGRAL_RATIO);
}
#endif 
 //0:2d, 2:frame packing, 3:frame seq, 4:t&b, 8:s by s, c:line interleave
 void vPscSet3DMode(UINT32 u4Mode)
{
	PSC_WRITE_FLD(PSCCTRL_05, u4Mode, PSC_3D_MODE);
}
 //set res import enable for r2 ??
void vPscSetResImportEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCCTRL_0A, bOnOff, PSC_SET_RES_IMPORT_ENABLE);
}
//toggle set res from scaler ??
void vPscSetResToggle(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCCTRL_0A, bOnOff, PSC_SET_RES_TOGGLE);
}
//mjc interface htotal
void vPscSetMjcDispInterfaceHtotal(UINT32 u4Htotal)
{
	PSC_WRITE_FLD(PSCCTRL_18, u4Htotal, PSC_MJC_DISP_INF_HTOTAL);
}
UINT32 vPscGetMjcDispInterfaceHtotal(void)
{
	return PSC_READ_FLD(PSCCTRL_18, PSC_MJC_DISP_INF_HTOTAL);
}
void vPscSetMjcInitDataRatio(UINT32 u4DataRatio)
{
	PSC_WRITE_FLD(PSCCTRL_0C, u4DataRatio, PSC_MJC_INIT_DAT_RATIO);
}

void vPscSetDispFifoWaterLevel(UINT32 u4WaterLevel)
{
	PSC_WRITE_FLD(PSCCTRL_0B, u4WaterLevel, PSC_DISP_FF_WTLV);
}

//set data to post proce
void vPscSetData(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_SET_DATA);
}
//time gen import r2 enable
void vPscTimegenImportEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_TG_IMPORT_ENABLE);
}
//prog import vcnt
void vPscTimegenImportVcnt(UINT32 u4Vcnt)
{
	PSC_WRITE_FLD(PSCSYS_01, u4Vcnt, PSC_TG_IMPORT_VCNT);
}
//internal clk en for oclk
void vPscSetOclkEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_00, bOnOff, PSC_OCLK_CLKEN);
}
/********************************** 
*  clock gated select
*  1: use internal clock gated
*  0: use ckgen clock gated
**********************************/
void vPscSetUseIntlclkEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_00, bOnOff, PSC_USE_INTL_CKEN);
}
//tve setting import toggle
#if 0
void vPscToggleTveSetting(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_TVE_SET_TOGGLE);
}
#endif
//output disp_r freerun enable
void vPscSetDispRFreeRunEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_01, bOnOff, PSC_TG_DISP_R_FREERUN_EN);
}
void vPscSetOutputDispRInverse(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_09, bOnOff, PSC_INV_OUTPUT_DISP_R);
}
//data enable raio of blank region when linesync mode
void vPscSetBlankRegionDataRatio(UINT32 u4DataRatio)
{
	PSC_WRITE_FLD(PSCSYS_11, u4DataRatio, PSC_TG_DAT_RATIO);
}
//main input/output vsync distance under engine disp mode
void vPscSetMainInOutVsyncDist(UINT32 u4Distance)
{
	PSC_WRITE_FLD(PSCSYS_12, u4Distance, PSC_INOUT_VSYNC_DIST_1);
}
//pip input/output vsync distance under engine disp mode
void vPscSetPipInOutVsyncDist(UINT32 u4Distance)
{
	PSC_WRITE_FLD(PSCSYS_12, u4Distance, PSC_INOUT_VSYNC_DIST_2);
}

//input frm mask number sel
void vPscSelectInputFrameMaskNum(UINT32 u4MaskNum)
{
	PSC_WRITE_FLD(PSCSYS_09, u4MaskNum, PSC_FRM_MASK_I_SEL);
}
//output frm mask number sel
void vPscSelectOutputFrameMaskNum(UINT32 u4MaskNum)
{
	PSC_WRITE_FLD(PSCSYS_09, u4MaskNum, PSC_FRM_MASK_O_SEL);
}
void vPscMaskInputFrame(UINT32 u4Frame)
{
	PSC_WRITE_FLD(PSCSYS_10, u4Frame, PSC_FRM_MASK_I);
}
void vPscMaskOutputFrame(UINT32 u4Frame)
{
	PSC_WRITE_FLD(PSCSYS_10, u4Frame, PSC_FRM_MASK_O);
}
//input frm mask function enable
void vPscSetInputFrameLRMaskEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_10, bOnOff, PSC_FRM_MASK_IN_LR_EN);
}
//output frm mask function enable
void vPscSetOutputFrameLRMaskEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_10, bOnOff, PSC_FRM_MASK_OUT_LR_EN);
}
//input frm mask function for 0:R 1:L
void vPscSelectInputFrameMaskLR(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_10, bOnOff, PSC_FRM_MASK_IN_LR_SEL);
}
//output frm mask function for 0:R 1:L
void vPscSelectOutputFrameMaskLR(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_10, bOnOff, PSC_FRM_MASK_OUT_LR_SEL);
}
//force input disp_r
void vPscSetForceInputDispR(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_09, bOnOff, PSC_FORCE_IN_DISP_R);
}
//force input disp_r val
void vPscSetForceInputDispRVal(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_09, bOnOff, PSC_FORCE_IN_DISP_R_VAL);
}


/************************************************
* main input PTG
*************************************************/

//input pat gen 3d inverse
void vPscInverseInputPatternGen3D(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_21, bOnOff, PSC_INPTG_3D_INV_1);
}
//input pat gen 3d mode, <00:close, 01:LR, 10:LLRR(no use), 11:LR(R is green)>
void vPscSetInputPatternGen3DMode(UINT32 u4Mode3D)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Mode3D, PSC_INPTG_3D_MODE_1);
}
void vPscSetInputPatternGenAngle(UINT32 u4Angle)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Angle, PSC_INPTG_ANGLE_SEL_1);
}
//input pat gen boundary enable
void vPscSetInputPatternGenBoundaryEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_21, bOnOff, PSC_INPTG_BND_EN_1);
}
void vPscSetInputPatternGenBoundary(UINT32 u4Hboundary,UINT32 u4Vboundary)
{
	PSC_WRITE_FLD(PSCSYS_22, u4Hboundary, PSC_INPTG_BND_W_1);//width
	PSC_WRITE_FLD(PSCSYS_22, u4Vboundary, PSC_INPTG_BND_H_1);//height
}

void vPscSetInputPatternGenFreeRunEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_21, bOnOff, PSC_INPTG_FREERUN_EN_1);
}
void vPscSetInputPatternGenFreeRunSpeed(UINT32 u4Speed)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Speed, PSC_INPTG_FREERUN_SPEED_1);
}
void vPscSetInputPatternGenColorMode(UINT32 u4ModeColor)
{
	PSC_WRITE_FLD(PSCSYS_21, u4ModeColor, PSC_INPTG_COLOR_MODE_1);
}
void vPscSetInputPatternGenFreq(UINT32 u4Freq)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Freq, PSC_INPTG_FREQ_SEL_1);
}
void vPscSetInputPatternHorLineEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_21, bOnOff, PSC_INPTG_HOR_LINE_EN_1);
}
//input pat gen pat mode <00:video, 01:3:2, 10:2:2>
void vPscSetInputPatternMode(UINT32 u4PatMode)
{
	PSC_WRITE_FLD(PSCSYS_21, u4PatMode, PSC_INPTG_PAT_MODE_1);
}
void vPscSetInputPatternGenEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_23, bOnOff, PSC_INPTG_ENABLE_1);
}
//input pat gen left side
void vPscSetInputPatternLeftSide(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_21, bOnOff, PSC_INPTG_LEFT_SIDE_1);
}
//input pat gen pat duty cycle
void vPscSetInputPatternDutyCycle(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Data, PSC_INPTG_PAT_DUTY_CYCLE_1);
}
//input pat gen ypat1
void vPscSetInputPatternYpat1(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_21, u4Data, PSC_INPTG_YPAT1_1);
}
//input pat gen ypat2
void vPscSetInputPatternYpat2(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_23, u4Data, PSC_INPTG_YPAT2_1);
}

/************************************************
* main output PTG
*************************************************/

//output pattern gen enable
void vPscSetOutputPatternGenEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_23, bOnOff, PSC_OUTPTG_ENABLE_1);
}
//output pat gen 3d inverse
void vPscInverseOutputPatternGen3D(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_24, bOnOff, PSC_OUTPTG_3D_INV_1);
}
//output pat gen 3d mode, <00:close, 01:LR, 10:LLRR(no use), 11:LR(R is green)>
void vPscSetOutputPatternGen3DMode(UINT32 u4Mode3D)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Mode3D, PSC_OUTPTG_3D_MODE_1);
}
void vPscSetOutputPatternGenAngle(UINT32 u4Angle)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Angle, PSC_OUTPTG_ANGLE_SEL_1);
}
//output pat gen boundary enable
void vPscSetOutputPatternGenBoundaryEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_24, bOnOff, PSC_OUTPTG_BND_EN_1);
}
void vPscSetOutputPatternGenBoundary(UINT32 u4Hboundary,UINT32 u4Vboundary)
{
	PSC_WRITE_FLD(PSCSYS_25, u4Hboundary, PSC_OUTPTG_BND_W_1);//width
	PSC_WRITE_FLD(PSCSYS_25, u4Vboundary, PSC_OUTPTG_BND_H_1);//height
}
void vPscSetOutputPatternGenFreeRunEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_24, bOnOff, PSC_OUTPTG_FREERUN_EN_1);
}
void vPscSetOutputPatternGenFreeRunSpeed(UINT32 u4Speed)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Speed, PSC_OUTPTG_FREERUN_SPEED_1);
}
void vPscSetOutputPatternGenColorMode(UINT32 u4ModeColor)
{
	PSC_WRITE_FLD(PSCSYS_24, u4ModeColor, PSC_OUTPTG_COLOR_MODE_1);
}
void vPscSetOutputPatternGenFreq(UINT32 u4Freq)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Freq, PSC_OUTPTG_FREQ_SEL_1);
}
void vPscSetOutputPatternHorLineEnable(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_24, bOnOff, PSC_OUTPTG_HOR_LINE_EN_1);
}
//output pat gen pat mode <00:video, 01:3:2, 10:2:2>
void vPscSetOutputPatternMode(UINT32 u4PatMode)
{
	PSC_WRITE_FLD(PSCSYS_24, u4PatMode, PSC_OUTPTG_PAT_MODE_1);
}
//output pat gen left side
void vPscSetOutputPatternLeftSide(UINT8 bOnOff)
{
	PSC_WRITE_FLD(PSCSYS_24, bOnOff, PSC_OUTPTG_LEFT_SIDE_1);
}
//output pat gen pat duty cycle
void vPscSetOutputPatternDutyCycle(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Data, PSC_OUTPTG_PAT_DUTY_CYCLE_1);
}
//output pat gen ypat1
void vPscSetOutputPatternYpat1(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_24, u4Data, PSC_OUTPTG_YPAT1_1);
}
//input pat gen ypat2
void vPscSetOutputPatternYpat2(UINT32 u4Data)
{
	PSC_WRITE_FLD(PSCSYS_23, u4Data, PSC_OUTPTG_YPAT2_1);
}
void vPscSetInPtgStyle( void)
{
	vPscSetInputPatternGenAngle(7);
	vPscSetInputPatternGenFreeRunEnable(1);
	vPscSetInputPatternGenFreeRunSpeed(1);
	vPscSetInputPatternLeftSide(1);
	vPscSetInputPatternDutyCycle(0x1F);
	vPscSetInputPatternGenColorMode(1);
	vPscSetInputPatternYpat1(1);
	vPscSetInputPatternYpat2(5);
}

void vPscSetOutPtgStyle( void)
{
	vPscSetOutputPatternGenAngle(7);		
	vPscSetOutputPatternGenFreeRunEnable(1);
	vPscSetOutputPatternGenFreeRunSpeed(0xE);
	vPscSetOutputPatternLeftSide(1);
	vPscSetOutputPatternDutyCycle(0x1F);
	vPscSetOutputPatternGenColorMode(1);
	vPscSetOutputPatternYpat1(1);
	vPscSetOutputPatternYpat2(5);
}

/**
  *  @brief get status of input disp_r
  */
UINT8 u1PscGetInDispR(void)
{
    return PSC_READ_FLD(PSCSYS_09, PSC_STA_IN_DISP_R);
}

/**
  *  @brief get status of output disp_r
  */
UINT8 u1PscGetOutDispR(void)
{
    return PSC_READ_FLD(PSCSYS_09, PSC_STA_OUT_DISP_R);
}

void vPscSetInvOsdHVSync(UINT8 bInvHSync, UINT8 bInvVSync)
{
    PSC_WRITE_FLD(PSCSYS_11, bInvHSync, PSC_INV_OSD_HSYNC);
    PSC_WRITE_FLD(PSCSYS_11, bInvVSync, PSC_INV_OSD_VSYNC);
}
