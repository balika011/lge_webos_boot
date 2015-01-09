###############################################################################
# Copyright Statement:                                                        #
#                                                                             #
#   This software/firmware and related documentation ("MediaTek Software")    #
# are protected under international and related jurisdictions'copyright laws  #
# as unpublished works. The information contained herein is confidential and  #
# proprietary to MediaTek Inc. Without the prior written permission of        #
# MediaTek Inc., any reproduction, modification, use or disclosure of         #
# MediaTek Software, and information contained herein, in whole or in part,   #
# shall be strictly prohibited.                                               #
# MediaTek Inc. Copyright (C) 2010. All rights reserved.                      #
#                                                                             #
#   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND      #
# AGREES TO THE FOLLOWING:                                                    #
#                                                                             #
#   1)Any and all intellectual property rights (including without             #
# limitation, patent, copyright, and trade secrets) in and to this            #
# Software/firmware and related documentation ("MediaTek Software") shall     #
# remain the exclusive property of MediaTek Inc. Any and all intellectual     #
# property rights (including without limitation, patent, copyright, and       #
# trade secrets) in and to any modifications and derivatives to MediaTek      #
# Software, whoever made, shall also remain the exclusive property of         #
# MediaTek Inc.  Nothing herein shall be construed as any transfer of any     #
# title to any intellectual property right in MediaTek Software to Receiver.  #
#                                                                             #
#   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its  #
# representatives is provided to Receiver on an "AS IS" basis only.           #
# MediaTek Inc. expressly disclaims all warranties, expressed or implied,     #
# including but not limited to any implied warranties of merchantability,     #
# non-infringement and fitness for a particular purpose and any warranties    #
# arising out of course of performance, course of dealing or usage of trade.  #
# MediaTek Inc. does not provide any warranty whatsoever with respect to the  #
# software of any third party which may be used by, incorporated in, or       #
# supplied with the MediaTek Software, and Receiver agrees to look only to    #
# such third parties for any warranty claim relating thereto.  Receiver       #
# expressly acknowledges that it is Receiver's sole responsibility to obtain  #
# from any third party all proper licenses contained in or delivered with     #
# MediaTek Software.  MediaTek is not responsible for any MediaTek Software   #
# releases made to Receiver's specifications or to conform to a particular    #
# standard or open forum.                                                     #
#                                                                             #
#   3)Receiver further acknowledge that Receiver may, either presently        #
# and/or in the future, instruct MediaTek Inc. to assist it in the            #
# development and the implementation, in accordance with Receiver's designs,  #
# of certain softwares relating to Receiver's product(s) (the "Services").    #
# Except as may be otherwise agreed to in writing, no warranties of any       #
# kind, whether express or implied, are given by MediaTek Inc. with respect   #
# to the Services provided, and the Services are provided on an "AS IS"       #
# basis. Receiver further acknowledges that the Services may contain errors   #
# that testing is important and it is solely responsible for fully testing    #
# the Services and/or derivatives thereof before they are used, sublicensed   #
# or distributed. Should there be any third party action brought against      #
# MediaTek Inc. arising out of or relating to the Services, Receiver agree    #
# to fully indemnify and hold MediaTek Inc. harmless.  If the parties         #
# mutually agree to enter into or continue a business relationship or other   #
# arrangement, the terms and conditions set forth herein shall remain         #
# effective and, unless explicitly stated otherwise, shall prevail in the     #
# event of a conflict in the terms in any agreements entered into between     #
# the parties.                                                                #
#                                                                             #
#   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and     #
# cumulative liability with respect to MediaTek Software released hereunder   #
# will be, at MediaTek Inc.'s sole discretion, to replace or revise the       #
# MediaTek Software at issue.                                                 #
#                                                                             #
#   5)The transaction contemplated hereunder shall be construed in            #
# accordance with the laws of Singapore, excluding its conflict of laws       #
# principles.  Any disputes, controversies or claims arising thereof and      #
# related thereto shall be settled via arbitration in Singapore, under the    #
# then current rules of the International Chamber of Commerce (ICC).  The     #
# arbitration shall be conducted in English. The awards of the arbitration    #
# shall be final and binding upon both parties and shall be entered and       #
# enforceable in any court of competent jurisdiction.                         #
###############################################################################
##############################################################################
# $RCSfile: Makefile,v $
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#
#         BRDCST_IF naming rule:
#
#             [chip number][digital demod]_P_[analog demod](_[parameter])
#
#         e.g. 5387DTD_TDAU4XB02A_P_anana
#              5386DTD_SS203EH_P_atdna
#         
##############################################################################

THIS_COMPONENT = $(BRDCST_IF)

export APPEND_OBJ_TYPE := brdcst_if/$(BRDCST_IF)
export BRDCST_IF_PARAM := $(word 2, $(subst _P_, P_, $(BRDCST_IF)))

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../../..
endif

#export TUNER_SRC_ROOT = .

SRC =

OPTIONAL_SUB_COMPONENTS=

#add symbol TUNER_SUPPORT_SCART_OUT here to avoid build code error when build all library under NA sys build directory
#need refine this under mt539x

#-------------------------------------------------
####################################################################################
####### depend on BRDCST_IF to define some marco about compile option
####################################################################################
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135"
DEFINES += -DCC_MT5135
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135DVBT"
DEFINES += -DCC_MT5135DVBT
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135DVBC"
DEFINES += -DCC_MT5135DVBC
endif

ifeq "$(findstring MT5136, $(BRDCST_IF))" "MT5136"
#ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5136"
DEFINES += -DCC_MT5136
endif

#LL
ifeq "$(findstring 5881MT5136, $(BRDCST_IF))" "5881MT5136"
#ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5136" 
DEFINES += -DCC_MT5136
DEFINES += -DCC_MT5881
endif

ifeq "$(findstring 5398MT5136, $(BRDCST_IF))" "5398MT5136"
#ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5136" 
DEFINES += -DCC_MT5136
DEFINES += -DCC_MT5398
endif

# For MT539x
DEFINES += -DfcADD_DIAG_INFO

####################################################################################
####### depend on BRDCST_IF to select EWS information
####################################################################################
ifeq "$(findstring ISDBT, $(BRDCST_IF))" "ISDBT"
DEFINES += -DTUNER_ISDBT_EWS
endif

####################################################################################
####### depend on BRDCST_IF to select tuner module
####################################################################################
ifeq "$(findstring DTDMBDVBTC, $(BRDCST_IF))" "DTDMBDVBTC"
OPTIONAL_SUB_COMPONENTS  += TunerDTMBI
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
DEFINES += -DINTERNAL_DTMB
endif
ifeq "$(findstring DTDDVBTC, $(BRDCST_IF))" "DTDDVBTC"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
endif
ifeq "$(findstring DTD, $(BRDCST_IF))" "DTD"
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5112EE"
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
DEFINES += -DfcEXTERN_DEMOD
endif

ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "ISDBTTC90517"
DEFINES += -DSUPPORT_BZ
endif
ifeq "$(findstring DVBT, $(BRDCST_IF))" "DVBT"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135DVBC"
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
DEFINES += -DCC_CAB_DEMOD_TUNER_BYPASS
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5135MT5112EE"
OPTIONAL_SUB_COMPONENTS  += TunerDTD
DEFINES += -DDTD_INCLUDE
DEFINES += -DfcEXTERN_DEMOD
DEFINES += -DCC_MT5135
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif
ifeq "$(findstring DVBTC, $(BRDCST_IF))" "DVBTC"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif

ifeq "$(findstring DTMBC, $(BRDCST_IF))" "DTMBC"
OPTIONAL_SUB_COMPONENTS  += TunerDTMBI
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif

ifeq "$(findstring DTMBC, $(BRDCST_IF))" "DTMBC"
DEFINES += -DINTERNAL_DTMB
endif

ifeq "$(findstring MT5136, $(BRDCST_IF))" "MT5136"
#ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5136"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT2
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif
ifeq "$(findstring MT5136_VDRDVBS, $(BRDCST_IF))" "MT5136_VDRDVBS"
DEFINES += -DCC_TER_DEMOD_TUNER_BYPASS
endif
ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5175DTMB"
OPTIONAL_SUB_COMPONENTS  += TunerDTMB
DEFINES += -DCC_MT5175
endif

###############DUAL_DEBUG_USE##########
#ifeq "$(findstring DUAL, $(BRDCST_IF))" "DUAL"
#OPTIONAL_SUB_COMPONENTS  += TunerDVBT
#OPTIONAL_SUB_COMPONENTS  += TunerDVBC
#OPTIONAL_SUB_COMPONENTS  += TunerDummy
#DEFINES += -DCC_DUAL_TUNER_DUMMY
#endif
#######################################

ifeq "$(findstring 5880, $(BRDCST_IF))" "5880"
DEFINES += -DCC_MT5880_CODE_SIZE_CTRL
endif

ifeq "$(word 2, $(subst _, , $(BRDCST_IF)))" "MT5175"
OPTIONAL_SUB_COMPONENTS  += TunerDTMB
DEFINES += -DCC_MT5175
endif

ifeq "$(findstring 5881, $(BRDCST_IF))" "5881"
#OPTIONAL_SUB_COMPONENTS  += TunerDTMB LL for 5175+5881
DEFINES += -DCC_MT5881
endif

ifeq "$(findstring 5399, $(BRDCST_IF))" "5399"
#OPTIONAL_SUB_COMPONENTS  += TunerDTMB LL for 5175+5399
DEFINES += -DCC_MT5399
endif

ifeq "$(findstring dummy, $(BRDCST_IF))" "dummy"
OPTIONAL_SUB_COMPONENTS  += TunerDummy
endif
ifeq "$(findstring DUMMY, $(BRDCST_IF))" "DUMMY"
OPTIONAL_SUB_COMPONENTS  += TunerDummy
endif
###if not 3-party,then include TunerISDBT
ifneq "$(findstring VDRISDBT, $(BRDCST_IF))" "VDRISDBT"
ifeq "$(findstring ISDBT,$(BRDCST_IF))" "ISDBT"
OPTIONAL_SUB_COMPONENTS  += Tuner3rdParty
endif
endif

ifeq "$(findstring VDRDemodLST, $(BRDCST_IF))" "VDRDemodLST"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
ifeq "$(CC_Sony_Maple_Drv)" "true"
OPTIONAL_SUB_COMPONENTS  += TunerDTD
endif
DEFINES += -DCUSTOMIZE_DVBDEMOD_TYPE
endif

ifeq "$(findstring InternalTC, $(BRDCST_IF))" "InternalTC"
OPTIONAL_SUB_COMPONENTS  += TunerDVBT
OPTIONAL_SUB_COMPONENTS  += TunerDVBC
endif

ifeq "$(findstring anaeu, $(BRDCST_IF_PARAM))" "anaeu"
OPTIONAL_SUB_COMPONENTS += TunerAnalogEU
ifdef SUPPORT_SIF_BYPASS
OPTIONAL_SUB_COMPONENTS += TunerATD
endif
endif
ifeq "$(findstring atd, $(BRDCST_IF_PARAM))" "atd"
OPTIONAL_SUB_COMPONENTS += TunerATD
endif

######################################################################################
#######  All BRDCST_IF will need TunerPool & CtrlBus module(ITuner) & TunerHAL module
######################################################################################
ifeq "$(BRDCST_IF)" "dummy"
OPTIONAL_SUB_COMPONENTS += TunerDummy
else
OPTIONAL_SUB_COMPONENTS += TunerPool
endif
OPTIONAL_SUB_COMPONENTS += CtrlBus
OPTIONAL_SUB_COMPONENTS += TunerHAL

####################################################################################
#######  When adding a new BRDCST_IF, add fcTUNER_TYPE here.
#######  Also need add a fcTUNER_TYPE number in Tuner/pd_tuner_type.h
ifeq "$(findstring F20WT,$(BRDCST_IF))" "F20WT"
DEFINES += -DfcTUNER_TYPE=cTCL_F20WT
DEFINES += -DCC_Tuner30V
endif
ifeq "$(findstring HFT28,$(BRDCST_IF))" "HFT28"
DEFINES += -DfcTUNER_TYPE=cXUGUANG_HFT28
endif

ifeq "$(findstring CTMRTUNER,$(BRDCST_IF))" "CTMRTUNER"
DEFINES += -DfcTUNER_TYPE=cCTMR_TUNER
endif

ifeq "$(findstring TDTKH701F,$(BRDCST_IF))" "TDTKH701F"
DEFINES += -DfcTUNER_TYPE=cLG_TDTKH701F
endif

ifeq "$(findstring TH2603,$(BRDCST_IF))" "TH2603"
DEFINES += -DfcTUNER_TYPE=cNuTune_TH2603
DEFINES += -DSTROafterWEAK_ACI
endif

ifeq "$(findstring TDTKG931D,$(BRDCST_IF))" "TDTKG931D"
DEFINES += -DfcTUNER_TYPE=cLG_TDTKG931D
endif
ifeq "$(findstring SI2156,$(BRDCST_IF))" "SI2156"
DEFINES += -DfcTUNER_TYPE=cSILABS_SI2156
endif

ifeq "$(findstring VA1E1BF2401,$(BRDCST_IF))" "VA1E1BF2401"
DEFINES += -DfcTUNER_TYPE=cSHARP_VA1E1BF2401
endif

ifeq "$(findstring VADTLST,$(BRDCST_IF))" "VADTLST"
DEFINES += -DfcTUNER_TYPE=cSHARPSAMSUNG_TUNERLST
DEFINES += -DI2C_TUNER_ALL
DEFINES += -DTUNERCNT=2
DEFINES += -DTUNER_INFO1=\(UINT16\)\(cSHARP_VA1E1BF2401\<\<8\|0xC2\)
DEFINES += -DTUNER_INFO2=\(UINT16\)\(cSAMSUNG_DTVA50CVH\<\<8\|0xC0\)
endif

###############Cust tuner type 
ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
ifeq "$(findstring VDRTunerLST,$(BRDCST_IF))" "VDRTunerLST"
DEFINES += -DfcTUNER_TYPE=cCUST_TUNER_LST
else
DEFINES += -DfcTUNER_TYPE=cCUST_TUNER
endif
endif
ifeq "$(findstring VDRDTMB, $(BRDCST_IF))" "VDRDTMB"
DEFINES += -DCC_EXT_DTMB
endif

ifeq "$(findstring F05WT,$(BRDCST_IF))" "F05WT"
DEFINES += -DfcTUNER_TYPE=cTCL_F05WT
endif
####################################################################################
#######  when adding a new BRDCST_IF, if don't need TunerAnalog module
#######  define your new OPTIONAL_SUB_COMPONENTS behind this line
####################################################################################

SUB_COMPONENTS =

DEFINES +=


##############################################################################
#
# Include the makefile common to all components
#
##############################################################################

include $(DRV_ROOT)/driver.mak
include $(DRV_ROOT)/brdcst_if.mak
