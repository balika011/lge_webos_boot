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
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #16 $
# $Date: 2015/03/11 $
# $Author: p4admin $
# $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/51 $
# $SWAuthor: Felisa Hsiao $
# $MD5HEX: 1476db419289e71bcc068156dea7ae10 $
#
# Description:
#         Makefile to build a "MT-5372" demo board build. The following
#         targets are supported:
#
#             all:           Compiles middleware, custom, target and this
#                            directory and creates a final image.
#             mw:            Compiles the middleware.
#             custom:        Compiles the custom.
#             target:        Compiles the MT-5372 target.
#             image:         Compiles this directory and creates a final
#                            image.
#             clean:         Cleans the middleware, custom, target and this
#                            directory and removes the run image.
#             clean_mw:      Cleans the middleware.
#             clean_custom:  Cleans the custom.
#             clean_target:  Cleans the MT-5372 target.
#             clean_image:   Cleans this directory and removes the run image.
#             clean_force:   Forcefully removes all libray and object
#                            directories as well as the run image. A fast way
#                            to perform cleanup.
#
#         The following commands are supported:
#
#             BUILD_CFG=debug
#                 Builds the specified target with symbolic debug info and the
#                 define file "mt5372_demo.def" is read. The run image is named
#                 is named "mt5372_demo_dbg".If BUILD_CFG is not specified then
#                 a non-debug build is created, which does not contain any
#                 symbolic debug information.
#
#             BRDCST_IF=<name>
#                 Allows the specifying of the front-end. The name must must
#                 be the directory name of the front-end, which follows the
#                 mt5372 targets brdcst_if sub-directory. For example,
#                 name may be set to "gen2004" or "5111" etc. In case
#                 BRDCST_IF is not set, "5111philips" is assumed.
#############################################################################

# EU_RUNNING_SPEC must be in (dvbtc, dvbt2, dvbt2s, oceania, taiwan, dvbt2s_dual), default is "dvbtc".
ifndef EU_RUNNING_SPEC
    export EU_RUNNING_SPEC := dvbtc
endif
#$(warning "---RUNING_SPEC=$(EU_RUNNING_SPEC)")

#
# Set initial values.
#
# drv_opt used under chiling
export COMPANY        := LGE

# For 3rd/OSS customization
export ODM            := LGE
export BRAND          := LGE


ifndef MW_ITG
	export MW_ITG := true
endif

ifndef LG_WEBOS_BUILD
	export LG_WEBOS_BUILD :=true
endif
ifeq "$(LG_WEBOS_BUILD)" "true"
export L_DUAL_BOOT := true
export 	PACK_TZBIN_INDIVIDUAL_PARTITION :=true
export LG_TOOLCHAIN = true
export LGE_PROTO_PCBA := true
endif 

ifndef CLI_LOG
export CLI_LOG := false
endif

ifndef LGE_PROTO_PCBA
export LGE_PROTO_PCBA := false
endif

ifndef LGE_INTERNAL_DEMODE_TYPE
export LGE_INTERNAL_DEMODE_TYPE := true
endif

# support one pkg cover different dram size
ifndef ONE_PKG_MULTI_DRAM_SIZE_SUPPORT
export ONE_PKG_MULTI_DRAM_SIZE_SUPPORT := true
endif

#
#Valgrind enable
#
ifndef ENABLE_VALGRIND
    export ENABLE_VALGRIND := false
endif

ifeq "$(ENABLE_VALGRIND)" "true"
    export BUILD_CFG := debug
    export SYM_SUPPORT := true
endif
#
# Weekly build type config.
#
# weekly build DVB-TC release version for pre-test.
ifeq "$(WEEKLY_BUILD_TYPE)" "PRETEST"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export EU_RUNNING_SPEC    := dvbt2s
endif
# weekly build DVB-TC release version for pre-test.
ifeq "$(WEEKLY_BUILD_TYPE)" "DVBT2S"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export EU_RUNNING_SPEC    := dvbt2s
endif
# weekly build oceania release version for pre-test.
ifeq "$(WEEKLY_BUILD_TYPE)" "OCEANIA"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export EU_RUNNING_SPEC    := oceania
endif
# weekly build debug version for stress test.
ifeq "$(WEEKLY_BUILD_TYPE)" "STRESS"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export BUILD_CFG          := debug
endif
# weekly build memory check version.
ifeq "$(WEEKLY_BUILD_TYPE)" "MEMCHK"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export SYS_MEM_CHK        := true
endif
# weekly build TMS version for performance test.
ifeq "$(WEEKLY_BUILD_TYPE)" "TMS"
    export RELEASE_3RD_POLICY := NET_AP_EU
    export TMS_SUPPORT        := true
endif

# check PCB configuration
# valid format: m/p*v*
ifndef PCB
    export PCB := m1v1
    $(warning, NO PCB config, default is $PCB)
endif


ifndef ENABLE_PART_VER_CHK
    export ENABLE_PART_VER_CHK = false
endif

ifeq "$(ENABLE_PART_VER_CHK)" "true"
    ifeq "$(BUILD_CFG)" "debug"
        export FORCE_PART_VERSION_UPG = true
    else 
        export FORCE_PART_VERSION_UPG = false
    endif
endif

ifndef MCM
    export MCM := false
endif

ifeq "$(MCM)" "false"
    ifndef MODEL
        export MODEL := mt5882_$(PCB)_eu
    endif
else
    export MODEL := mt5882_eu_mcm
endif

##for auto_test
ifndef LINUX_AUTO_TEST
    export LINUX_AUTO_TEST := false
endif

ifeq "$(LINUX_AUTO_TEST)" "true"
#export RELEASE_3RD_POLICY := NET_AP_EU
export SUPPORT_PIP := true
export OPENBOX := false
endif

#
# LITE version only include basic TV and full MMP, use 256M DRAM.
#
ifeq "$(LITE)" "true"
    export SYS_OAD_SUPPORT		    := false
    export SYS_MHEG5_SUPPORT	    := false
    export SYS_MHP_SUPPORT	        := false
    export HBBTV_SUPPORT	        := false
	export INET_SUPPORT 		    := false
	export SUPPORT_PIP			    := false
	export MMP_SUPPORT			    := full
	export ENABLE_DTV_TIME_SHIFT	:= false
	export ENABLE_DTV_PVR			:= false
	export ENABLE_AV_TIME_SHIFT 	:= false
	export ENABLE_AV_PVR 	        := false
	export 3D_SUPPORT			    := false
	export SYS_FLIP_SUPPORT		    := false
	export SYS_MIRROR_SUPPORT	    := false
	export TRUSTZONE 			    := false
	export DRAM_256M                := true
endif

# middleware model under project_x/sys_build
export MW_MODEL       := mt5882_eu_linux
export LINUX_SOLUTION := true
ifeq "$(LG_WEBOS_BUILD)" "true"
export KERNEL_VER     := linux-3.10
else
#remove this folder for reducing release time
#export KERNEL_VER     := linux-3.10_turnkey
endif

export VERSION        := IDTV1401_002306_12_001_37_001_8
export ROOTFS_NAME    := 5882_free
export ROOTFS_VER     := $(VERSION)
export TARGET_IC      := mt5882
export REF_TARGET_IC  := mt5882
export KEY_FROM_DFB   := true
export UBOOT_VERSION  := 2011.12
export APP_IF         := mtal
export APP_IF_2ND     := hal
export CUST_KERNEL_VER := $(KERNEL_VER)
export CUST_LOADER_VER := $(UBOOT_VERSION)
export NET_LAZY_INIT := true
export FAST_BOOT     := true
export DUAL_TUNER    := false

ifndef OPENBOX
export OPENBOX := false
endif

ifeq "$(RAMDISK)" "true"
export RAMDISK_ROOTFS := true
endif

export COMBINED_BUILD_CFG := true

export MMP_SUPPORT    := full

ifndef SECURE_BOOT
export SECURE_BOOT    := y
endif

ifeq "$(SECURE_BOOT)" "y"
export SECURE_BOOT_V2 := true
export BOOT_NO_LZHS  := false
export SECURE_BOOT_SCRAMBLE := false
endif

# secure boot, verify pkg key
ifndef USIG
export USIG := true
endif

export UBIFS          := true
export BUILD_PHASE    := PHASE1.75
ifndef MSDC_COMMON
export MSDC_COMMON      := true
endif

ifndef BOOT_TYPE
export BOOT_TYPE      := ROM2EMMC
endif

ifeq "$(BOOT_TYPE)" "ROM2EMMC"
    export SYS_IMG_FS     := ext4
    ifeq "$(BOOT_FROM_SDIO)" "true"
        export KERNEL_CONFIG_DIFF += sdio
    else
        export KERNEL_CONFIG_DIFF += emmc
        ifeq "$(MSDC_COMMON)" "true"
        export KERNEL_CONFIG_DIFF += msdc_common
        endif
    endif
    export SDMMC_SUPPORT  := false
    export EXT_FS_SUPPORT := true
else
    export SYS_IMG_FS     := ubifs
    export UBIFS          := true
    export SDMMC_SUPPORT  := false
endif

ifeq "$(SDMMC_SUPPORT)" "true"
export KERNEL_CONFIG_DIFF += sdmmc
endif

ifndef SDM_SUPPORT
export SDM_SUPPORT := true
endif

# MEMORY CHECK
ifndef SYS_MEM_CHK
export SYS_MEM_CHK := false
endif

#
## trust zone, security
#
ifndef TRUSTZONE
export TRUSTZONE := true
endif
ifeq "$(TRUSTZONE)" "true"
	ifndef SVP_SUPPORT
		export SVP_SUPPORT := true
	endif
	ifndef TVP_SUPPORT
		export TVP_SUPPORT := true
	endif
else
	export TVP_SUPPORT := false
	export SVP_SUPPORT := false
endif


ifeq "$(TRUSTZONE)" "true"
ifndef WIDEVINE_L1_SUPPORT
export WIDEVINE_L1_SUPPORT := false
endif
endif

# DM verity for security enhancement
ifndef DM_VERITY
    export DM_VERITY := false
endif
ifeq "$(DM_VERITY)" "true"
    export KERNEL_CONFIG_DIFF += dmverity
endif

#
# FTRACE(MET/SYSTrace) Support
#
ifeq "$(ENABLE_MET_TOOL)" "true"
export FTRACE_SUPPORT := true
endif
ifeq "$(FTRACE_SUPPORT)" "true"
export KERNEL_CONFIG_DIFF += ftrace
endif

ifneq "$(KERNEL_VER)" "linux-3.0"
export USE_MKNFSBIMG_V3 := true
endif

#
# Put basic into rootfs
#
ifndef BASIC_IN_ROOTFS
ifeq "$(LG_WEBOS_BUILD)" "true"
    export BASIC_IN_ROOTFS := false
else 
    export BASIC_IN_ROOTFS := true
endif 
endif

ifndef 3RD_RO
    export 3RD_RO := true
    ifndef 3RD_RO_DBG
        export 3RD_RO_DBG := false
    endif
endif

ifndef 3D_SUPPORT
    export 3D_SUPPORT := true
endif

ifeq "$(3D_SUPPORT)" "true"
    ifndef 3D_SUPPORT_SG_PANEL
        export 3D_SUPPORT_SG_PANEL := false
    endif
    ifndef 3DSUPPORT_2D_PANEL
        export 3DSUPPORT_2D_PANEL := false
    endif
    ifndef 3D_MM_DS_SUPPORT
        export 3D_MM_DS_SUPPORT := true
    endif
    ifndef 3D_PHOTO_ON_VDP
        export 3D_PHOTO_ON_VDP := true
    endif
endif

ifndef REBUILD_3RD_PARTY
    export REBUILD_3RD_PARTY := true
endif

ifndef LG_BOOT_TIME_REDUCE
export LG_BOOT_TIME_REDUCE := false
endif

# path customization for project_x/target/$(OS_TARGET)
export OS_TARGET      := linux-2.6.18

TARGET         := linux_mak
BUILD_NAME     := mt5882_eu_linux DTV_X_IDTV1401_002306_12_001_37_001
MODEL_NAME     := mt5882_eu_linux
CUSTOMER       := LGE_linux
CUSTOM         := mtk/dvb/demo2
CUST_MODEL     := $(MODEL_NAME)
SERIAL_NUMBER  := IDTV
THIS_ROOT      := $(shell pwd)

ifeq "$(SYS_IMG_FS)" "ext4"
    MAIN_PART_SIZE      := 75M
    3RD_PART_SIZE       := 192M
    PERM_PART_SIZE      := 8M
    3RD_RW_PART_SIZE    := 192M
else
ifeq "$(UBIFS)" "true"
    export UBI_MAX_LOG_BLK_EARSE_CNT_BASIC := 271
    export UBI_MAX_LOG_BLK_EARSE_CNT_PERM := 63
    ifeq "$(3RD_RO)" "true"
        export UBI_MAX_LOG_BLK_EARSE_CNT_3RD_RW := 1599
        ifeq "$(3RD_RO_DBG)" "true"
            export UBI_MAX_LOG_BLK_EARSE_CNT_3RD_RO := 1535  #((192MB/128KB)-1)
        endif
    else
        export UBI_MAX_LOG_BLK_EARSE_CNT_3RD := 1423
    endif
endif
endif


ifeq "$(APP_IF)" "mtal"
export MTAL_SUPPORT   := true
endif

ifndef WARNING_TO_ERROR
export WARNING_TO_ERROR = true
endif 

ifndef OPTIMIZE_LVL
export OPTIMIZE_LVL = -O2
endif

#
# Disable MET TOOL(just could run on CA9 IC) for default
#
ifndef ENABLE_MET_TOOL
    export ENABLE_MET_TOOL := false
endif

# THUMB mode will be default configuration
#linux thumb mode configuration, default is false
export THUMB ?= n
export CPU_TYPE = ca7

ifndef TOOL_CHAIN
export TOOL_CHAIN = 4.8.2
endif

ifndef LG_TOOLCHAIN
export LG_TOOLCHAIN = false
endif

ifndef ENABLE_CA9_NEON
export ENABLE_CA9_NEON = true
endif

ifndef ENABLE_CA9
export ENABLE_CA9=true
endif

ifeq "$(ENABLE_CA9_NEON)" "true"
export ENABLE_CA9 = false
endif

export DIRECTFB_VERSION :=1.5.3

export TMP_TO_BUILD_HAL = true

ifndef ENABLE_SMP_IC
export ENABLE_SMP_IC := true
endif

ifeq "$(ENABLE_CA9_NEON)_$(ENABLE_CA9)" "true_true"
$(error ENABLE_CA9_NEON and ENABLE_CA9 can not be used at the same time)
endif

# NAND size setting
ifndef NAND128M
export NAND_SIZE=false
endif

# Add local tuner driver setting.
#
ifeq "$(EU_RUNNING_SPEC)" "dvbt2"
    export SYS_DVBT2_SUPPORT := true
    export BRDCST_IF := 5882DTDDVBTC_VDRTuner_P_atdeu

    ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
        export 3RD_PARTIES := LINUX
        export 3RD_PARTIES_CUST :=VDRBRDCST
        export CUST_DEMOD := CXD2837
        export CUST_TUNER := sonyre231
    endif	
endif

ifeq "$(EU_RUNNING_SPEC)" "dvbtc"
    #export BRDCST_IF := 5882DVBTC_DTD_VDRTuner_P_atd
    export BRDCST_IF := 5882DTDDVBTC_CTMRTUNER_P_atdeu
        
    ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
        export 3RD_PARTIES := LINUX
        export 3RD_PARTIES_CUST :=VDRBRDCST
        export CUST_TUNER := Si2157
    endif
endif

ifeq "$(EU_RUNNING_SPEC)" "dvbt2s"
    export SYS_DVBT2_SUPPORT := true
    export SYS_DVBS_SUPPORT := true
    export DEFAULT_DB_SUPPORT := true

    ifeq "$(DUAL_TUNER)" "true"
        export BRDCST_IF := 5882DUAL_VDRDVBS_VDRTuner_P_atdeu
    endif

    # Add local tuner driver setting.
    ifndef BRDCST_IF
        export BRDCST_IF := 5882_VDRDOMEDT2_VDRDVBS_VDRTuner_P_atdeu
    endif

    #3rd_party demod
    ifeq "$(findstring VDRDOMEDT2,$(BRDCST_IF))" "VDRDOMEDT2"
        export 3RD_PARTIES += LINUX
        export 3RD_PARTIES_CUST +=VDRBRDCST
        export CUST_DEMOD += CXD2837
    endif
    ifeq "$(findstring VDRDVBS,$(BRDCST_IF))" "VDRDVBS"
        export 3RD_PARTIES += LINUX
        export 3RD_PARTIES_CUST +=VDRBRDCST
        export CUST_DEMOD += CXD2839
        ifeq "$(DUAL_TUNER)" "true"
            CUST_DEMOD := CXD2837_Dual CXD2839_Dual
        endif
    endif
    #3rd_party tuner
    ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
        export 3RD_PARTIES += LINUX
        export 3RD_PARTIES_CUST +=VDRBRDCST
        export CUST_TUNER += sonyre231
        ifeq "$(DUAL_TUNER)" "true"
            export CUST_TUNER := sonyre231_dual
        endif
    endif

    ifdef 3RD_PARTIES
        export 3RD_PARTIES:=$(sort $(3RD_PARTIES))
        export 3RD_PARTIES_CUST:=$(sort $(3RD_PARTIES_CUST))
    endif
endif

ifeq "$(EU_RUNNING_SPEC)" "oceania"
    export SYS_DVBT_OCEANIA_SUPPORT := true
endif

ifeq "$(EU_RUNNING_SPEC)" "taiwan"
    export SYS_TWN_SUPPORT := true
    
    ifndef BRDCST_IF
        export BRDCST_IF := 5882DVBTC_TDTKG931D_P_atdna
    endif
endif

ifeq "$(SYS_DVBS_SUPPORT)" "true"
    export SYS_DSMCC_SUPPORT := true
    export TWO_SAT_CHLIST_SUPPORT := true
endif

ifndef BRDCST_IF
    # bring up export BRDCST_IF := 5882DVBTC_VDRTuner_P_atdeu
    # eu export BRDCST_IF := 5882DVBTC_CTMRTUNER_P_atdeu
    export BRDCST_IF := 5882DTD_CTMRTUNER_P_atdeu
    export 3RD_PARTIES := 
    export 3RD_PARTIES_CUST := 
    export CUST_TUNER := 
ifeq "$(findstring VDRTuner,$(BRDCST_IF))" "VDRTuner"
        export 3RD_PARTIES :=
        export 3RD_PARTIES_CUST :=VDRBRDCST
        export CUST_TUNER := Si2157
    endif
endif

ifeq "$(DUMMY_TUNER)" "true"
    export BRDCST_IF := 5882DUMMY_F20WT_atdeu 
endif

#$(warning "---BRDCST_IF=$(BRDCST_IF)")
#$(warning "3RD_PARTIES---$(3RD_PARTIES)---$(3RD_PARTIES_CUST)")
#
# MTD Encryption
#
ifndef MTD_ENCRYPT_SUPPORT
ifeq "$(LG_WEBOS_BUILD)" "true"
export MTD_ENCRYPT_SUPPORT := false
else
export MTD_ENCRYPT_SUPPORT := true
endif
endif

#
# Flash adspbin
# ADSPBIN_ROOT: adspbin root directory
# please config it correctly if FLASH_ADSP_BIN is true
#
ifndef FLASH_ADSP_BIN
export FLASH_ADSP_BIN      := true
export ADSPBIN_ROOT        := adspbin8290
endif

#
# Animation Support
#
ifndef ANIMATION_SUPPORT
export ANIMATION_SUPPORT := false
endif
#For ptp/mtp feature
ifndef MTP_SUPPORT
export MTP_SUPPORT := true
endif

#
# EDB Support
#
ifndef EDB_SUPPORT
export EDB_SUPPORT := true
endif

#
# Fastboot Support
#
ifndef SUSPEND_TO_DRAM
export SUSPEND_TO_DRAM := false
endif

ifndef SNAP_SHOT
export SNAP_SHOT := true
endif
ifeq "$(SUSPEND_TO_DRAM)" "true"
    ifndef SUSPEND_TO_DRAM_DEBUG
        export SUSPEND_TO_DRAM_DEBUG := true
    endif
endif

#
# AV Bypass Configuration
#
# 1: Disable AV Bypass
# 2: Enable Monitor Bypass only
# 3: Enable TV & Monitor Bypass concurrently
# 4: Enable TV Bypass only
#
ifndef AV_BYPASS
    ifeq "$(EU_RUNNING_SPEC)" "oceania"
        export AV_BYPASS := 2
    else
        export AV_BYPASS := 4
    endif
endif

ifndef SUPPORT_PIP
export SUPPORT_PIP = true
endif

#
# Multimedia Options (OPTIONS: none / basic / full)
#   "MMP_SUPPORT=none"  : No MMP
#   "MMP_SUPPORT=basic" : MP3 + JPEG only
#   "MMP_SUPPORT=full"  : MP3 + JPEG + Video Playback
#
ifndef MMP_SUPPORT
export MMP_SUPPORT := full
endif

#
# Time-Shift & PVR Options
#
ifeq "$(MMP_SUPPORT)" "full"
ifndef ENABLE_DTV_TIME_SHIFT
    export ENABLE_DTV_TIME_SHIFT := false
endif
ifndef ENABLE_DTV_PVR
    export ENABLE_DTV_PVR := false
endif

##################################################################################
# AV record for ATV and CVBS record ( may extend to support other input sources ).
# NOTE: 
#    If Enable AV timeshift or AV PVR, please confirm the CVBS is separated from 
# Combi source in sys_drv_custom.c !
##################################################################################
ifndef ENABLE_AV_TIME_SHIFT
    export ENABLE_AV_TIME_SHIFT := false
endif
ifndef ENABLE_AV_PVR
    export ENABLE_AV_PVR := false
endif

ifndef ENABLE_SCHED_PVR
    ifeq "$(ENABLE_DTV_PVR)" "true"
        export ENABLE_SCHED_PVR := true
    else
        ifeq "$(ENABLE_AV_PVR)" "true"
            export ENABLE_SCHED_PVR := true
        else
            export ENABLE_SCHED_PVR := false
        endif
    endif
endif
endif
###############################################

#
# Loader logo
#
ifndef LOADER_LOGO_SUPPORT
export LOADER_LOGO_SUPPORT := true
endif

ifeq "$(LOADER_LOGO_SUPPORT)" "true"
export LOADER_LOGO_LONG_TIME := true
endif

#
# Capture Logo
#
ifndef CAPTURE_LOGO_SUPPORT
export CAPTURE_LOGO_SUPPORT := false
endif

#
# upgrade FW with changed acfg layout doesn't modify acfg user settings, default will reset them.
#
ifndef ACFG_UPGRADE_ENHANCE
export ACFG_UPGRADE_ENHANCE := false
endif

ifndef UPGRADE_VERSION_CTRL_FLOW_BY_IMG  
export UPGRADE_VERSION_CTRL_FLOW_BY_IMG	:= false
endif

#
## dump Log to usb
#
ifndef LOG2USB
export LOG2USB = false
endif

#
# MHL Support Options
#
ifndef MHL_SUPPORT
    export MHL_SUPPORT := true
endif

#
# MHL configure
# 0: Disable
# 1: HDMI1=>HDMI1/MHL
# 2: HDMI2=>HDMI2/MHL
# 3: HDMI3=>HDMI3/MHL
# 4: HDMI4=>HDMI4/MHL
#
ifndef MHL_HDMI
    ifeq "$(MHL_SUPPORT)" "true"
        export MHL_HDMI := 1
    else
        export MHL_HDMI := 0
    endif
endif

#
# Factory Uart Support Options
#
ifndef FACTORY_SUPPORT
    export FACTORY_SUPPORT := true
endif

#
# Factory mode through USB Support Options
#
ifeq "$(FACTORY_SUPPORT)" "true"
    ifndef SUPPORT_TTY_USB
        export SUPPORT_TTY_USB := false
    endif
    ifeq "$(SUPPORT_TTY_USB)" "true"
        export KERNEL_CONFIG_DIFF += usbconsole
    endif
endif

#
# USB 2.0 built-in setting
#
ifeq "$(USB20_BUILD_IN)" "true"
    export KERNEL_CONFIG_DIFF += usb20_buildin
endif

#
# USB 3.0 built as module(KO)
#
ifeq "$(USB30_BUILD_MOD)" "true"
    export KERNEL_CONFIG_DIFF += usb30_buildmod
endif

#
# ETHERNET built as module(KO)
#
ifeq "$(ETHERNET_DRV_BUILTIN)" "true"
    export KERNEL_CONFIG_DIFF += star_mac
endif

# Disable log 20s when boot up
ifndef KERNEL_NO_LOG
    export KERNEL_NO_LOG := false
endif

# For low cost IC(5881), enable this define will improve EPG performance.Turnkey default disable.
ifndef EPG_STOP_VIDEO
    export EPG_STOP_VIDEO := false
endif

ifndef EPG_FILTER_EIT_SECTION_IN_DMX
    export EPG_FILTER_EIT_SECTION_IN_DMX := false
endif
# For low cost IC(5881), enable this define will improve change channel performance.Turnkey default disable.
ifndef ENABLE_ZAPPING_ENHANCE
    export ENABLE_ZAPPING_ENHANCE := false
endif
# For change channel performance improvement
ifeq "$(ENABLE_ZAPPING_ENHANCE)" "true"
    export ZAPPING_STOP_DMX    := true
    export ZAPPING_MUTE_VIDEO  := true
    export ZAPPING_WITHOUT_EDB := true
endif

#
# ISO Support
#
ifndef SYS_ISO_SUPPORT
    export SYS_ISO_SUPPORT := true
endif

ifeq "$(SYS_ISO_SUPPORT)" "true"
    export KERNEL_CONFIG_DIFF += iso
endif

#
# MMU for DFB. If enable this define, DFB will share system memory when DFB memory is not enough
#
#ifndef GFX_MMU_SUPPORT
#export GFX_MMU_SUPPORT := true
#endif

ifndef LINUX_GPU_SUPPORT
export LINUX_GPU_SUPPORT := false
endif
#
# Bluetooth Option: default off
#
ifndef BLUETOOTH
export BLUETOOTH := false
endif
ifeq "$(BLUETOOTH)" "true"
export 4_AUDIO_DECODER := true
endif

##### Prepare internal variable (Do not modify)
ifeq "$(ENABLE_DTV_TIME_SHIFT)" "true"
    export TSHFT_CORE_LIB := true
else
    ifeq "$(ENABLE_DTV_PVR)" "true"
        export TSHFT_CORE_LIB := true
    else
        export TSHFT_CORE_LIB := false
    endif
endif

##################################################################################
# AV record for ATV and CVBS record ( may extend to support other input sources ).
# NOTE: 
#    If Enable AV timeshift or AV PVR, please confirm the CVBS is separated from 
# Combi source in sys_drv_custom.c !
##################################################################################
ifeq "$(ENABLE_AV_TIME_SHIFT)" "true"
    AV_RECORD_LIB := true
else
    ifeq "$(ENABLE_AV_PVR)" "true"
        AV_RECORD_LIB := true
    else
        AV_RECORD_LIB := false
    endif
endif
#LGA A5LR don't need PVR/DVR Function.
#export AV_RECORD_LIB

#########################################################################
# EU NET App Configuration 1: DLNA +  WIFI + MyNetworkPlaces
ifeq "$(DAILY_BUILD)" "true"
#    export RELEASE_3RD_POLICY := NET_AP_EU
endif

export RELEASE_3RD_POLICY := none

#
# Network
#
ifndef INET_SUPPORT
    export INET_SUPPORT := true
endif

ifeq "$(INET_SUPPORT)" "true"
#
## Wi-Fi Display (WFD) Option
#
# add for build error , consultant : Hong Wang; Wen Xu; Liyong Zhang
    ifndef RELEASE_3RD_LIB_WIFI
  #  	export RELEASE_3RD_LIB_WIFI := true
    endif
#end
    export USE_SMART_FILE := true
## wakeup on Lan for zhiyong.tao
     ifndef WAKE_ON_LAN
        export WAKE_ON_LAN := true
    endif
    ifeq "$(RELEASE_3RD_LIB_WIFI)" "true"
        ifneq "$(MMP_SUPPORT)" "none"
            ifndef ENABLE_WIFI_DISPLAY
                export ENABLE_WIFI_DISPLAY := true
            endif
        endif
    endif

#
# Samba Support
#
    ifndef SMB_SUPPORT
#        export SMB_SUPPORT := true
    endif

#
# IPV6 Support
#
    ifndef IPV6_SUPPORT
        export IPV6_SUPPORT := true
    endif

#
# Dialserver Support
#
#    ifndef DIAL_SUPPORT
#        export DIAL_SUPPORT := true
#    endif

#
# Netflix Support
#
    ifndef NETFLIX_CMPB
#        export NETFLIX_CMPB := true
    endif

    ifndef NETFLIX_3D
#        export NETFLIX_3D := true
    endif

    ifeq "$(NETFLIX_CMPB)" "true"
        export WEBP_SUPPORT      := true
        export MNG_SUPPORT       := true
        export OGG_SUPPORT       := true
        export SEAMLESS_SUPPORT  := true
        ifndef NETFLIX_APP_SUSPEND_SUPPORT
        export NETFLIX_APP_SUSPEND_SUPPORT := true
        endif
    endif

#
# Skype Support
#
    ifndef SKYPE_SUPPORT
#        export SKYPE_SUPPORT := true
    endif
    
    ifeq "$(SKYPE_SUPPORT)" "true"
        ifndef V4L_SUPPORT
            export V4L_SUPPORT := true
        endif
        export USB_CAM          := MAX
        export MULTI_MM_SUPPORT := true
    endif

# Support Opera HTML5
    ifndef OPERA_HTML5_SUPPORT
#        export OPERA_HTML5_SUPPORT := true
    endif

    ifeq "$(OPERA_HTML5_SUPPORT)" "true"
        ifndef MTKSTREAM_SUPPORT
            export MTKSTREAM_SUPPORT := true
        endif
        ifndef HLS_SUPPORT
            export HLS_SUPPORT := true
        endif
        ifndef WIDEVINE_SUPPORT
            export WIDEVINE_SUPPORT := true
        endif
        ifeq "$(RELEASE_3RD_POLICY)"  "NET_AP_EU"
            ifndef MSS_SUPPORT
                export MSS_SUPPORT := true
            endif
        endif
        ifndef DASH_SUPPORT
            export DASH_SUPPORT := true
        endif
        ifndef CMPB_DASH_SUPPORT
            export CMPB_DASH_SUPPORT := true
        endif
		ifndef SEAMLESS_SUPPORT
            export SEAMLESS_SUPPORT := true
        endif
    endif

# MHP Support
    ifndef SYS_MHP_SUPPORT
#        export SYS_MHP_SUPPORT := false
    endif

    ifeq "$(SYS_MHP_SUPPORT)" "true"
        export DASH_SUPPORT := true
        export MTKSTREAM_SUPPORT := true
        export CMPB_DASH_SUPPORT := true
        export BACKGROUND_PSI_SI_MONITOR := true
    endif

# HBBTV Support
    ifndef HBBTV_SUPPORT
#        export HBBTV_SUPPORT := false
    endif

    ifeq "$(HBBTV_SUPPORT)" "true"
        ifndef HBBTV_SOLUTION
            export HBBTV_SOLUTION := opera
        endif
        ifndef DEMO_FAKE_HBBTV_IC
            export DEMO_FAKE_HBBTV_IC := false
        endif
        ifndef CMPB_DASH_SUPPORT
            export CMPB_DASH_SUPPORT := true
        endif
		ifndef SEAMLESS_SUPPORT
            export SEAMLESS_SUPPORT := true
        endif
        ifndef DASH_SUPPORT
            export DASH_SUPPORT := true
        endif
        ifndef MTKSTREAM_SUPPORT
            export MTKSTREAM_SUPPORT := true
        endif
    	ifndef MTK_DRM_SUPPORT
    	    export MTK_DRM_SUPPORT := true
        endif
        ifneq "$(HBBTV_SOLUTION)" "opera"
        export SYS_DSMCC_SUPPORT := true
        endif
        export BACKGROUND_PSI_SI_MONITOR := true
    endif
endif

ifeq "$(DASH_SUPPORT)" "true"
	ifndef IPCC_SUPPORT
		export IPCC_SUPPORT := true
	endif
endif

#
# WIFI  default enalbe WIFI
#
ifeq "$(RELEASE_3RD_LIB_WIFI)" "true"
export CONFIG_NEW_WIFI_SUPPLICANT := true
#export WIFI_DONGLE := RT557X
export WIFI_DONGLE := MT7603U
export WIFI_WOW_SUPPORT :=true
endif

# HDCP Option: default on with WFD, and must work with TRUSTZONE
ifeq "$(ENABLE_WIFI_DISPLAY)" "true"
    DEFINES += -DMW_FAKE_THREAD
    ifeq "$(TRUSTZONE)" "true"
        ifndef ENABLE_HDCP2
            export ENABLE_HDCP2 := true
        endif
    endif
endif

ifndef ENABLE_HDCP2
	export ENABLE_HDCP2 := true
endif

# SKB
ifndef ENABLE_SKB
export ENABLE_SKB := true
endif

# IPCC and MSS support
ifeq "$(MMP_SUPPORT)" "full"
    ifeq "$(INET_SUPPORT)" "true"
        ifndef IPCC_SUPPORT
            export IPCC_SUPPORT := true
        endif
        ifeq "$(RELEASE_3RD_POLICY)"  "NET_AP_EU"
            ifndef MSS_SUPPORT
                export MSS_SUPPORT := true
            endif
        endif
    endif
endif

ifeq "$(HLS_SUPPORT)" "true"
    export SEAMLESS_SUPPORT := true
endif

#
# DockUI draw with DFB
#
ifndef DOCKUI_DFB
#export DOCKUI_DFB := true
endif

ifeq "$(DEINIT)" "1"
DEFINES    += -DHBBTV_DEINIT
endif

#
# Firmware Upgrade
#
# none  : No Firmware Upgrade
# mtk   : Turkey USB Upgrade + Network Upgrade
# uli   : UpdateLogic
#
ifndef FW_UPG_SUPPORT
ifeq "$(LG_WEBOS_BUILD)" "true"
export FW_UPG_SUPPORT := LGMTK
else 
export FW_UPG_SUPPORT := mtk
endif 
endif

export ULI_SDK_VER = v2.0.26.2
export ULI_CEM_MG = 00199D-0101

# sys_start.c would be removed

#
# Hotkey wakeup 
#
ifeq "$(RELEASE_3RD_POLICY)" "NET_AP_EU"
    ifndef HOTKEY_WAKEUP_SUPPORT
        export HOTKEY_WAKEUP_SUPPORT := true
    endif
endif

#
# DivX DRM Configuration
#
# 1: HT 2.2,        SDK 4.0 patch 3.02
# 2: HT 3.0,        SDK 5.1
# 3: HD 1.1,        SDK 5.1
# 4: HD 1.2,        SDK 6.1
# 5: HD Plus 1.0,   SDK 6.1
# 6: HD Plus 1.1.1, SDK 6.1 addendum
# 7: HD 1.3,        SDK 6.1 addendum
# 8: HT 3.1,        SDK 6.1 addendum
#
ifndef DIVX_DRM
    ifneq "$(LITE)" "true"
        ifneq "$(MMP_SUPPORT)" "none"
            export DIVX_DRM := 7
        endif
    endif
endif

ifeq ($(DIVX_DRM), 1)
    DEFINES += -DDIVX_DRM_40
else ifeq ($(DIVX_DRM), 2)
    DEFINES += -DDIVX_DRM_51
else ifeq ($(DIVX_DRM), 3)
    DEFINES += -DDIVX_DRM_51
else ifeq ($(DIVX_DRM), 4)
    DEFINES += -DDIVX_DRM_60
else ifeq ($(DIVX_DRM), 5)
    DEFINES += -DDIVX_DRM_60
else ifeq ($(DIVX_DRM), 6)
    DEFINES += -DDIVX_DRM_61
else ifeq ($(DIVX_DRM), 7)
    DEFINES += -DDIVX_DRM_61
else ifeq ($(DIVX_DRM), 8)
    DEFINES += -DDIVX_DRM_61
endif

ifndef DIVX_PLUS_SUPPORT
	export DIVX_PLUS_SUPPORT := true
endif

ifndef DIVX_HD_SUPPORT
	export DIVX_HD_SUPPORT := true
endif

ifeq "$(DIVX_PLUS_SUPPORT)" "true"
    export SEAMLESS_SUPPORT :=true
endif

#export USE_WIFI_WLAN_CTL ?= true
#
# Flingo ACR Option: default on
#
ifndef SUPPORT_VSS_FLINGO
export SUPPORT_VSS_FLINGO := true
endif

#
# Add local compile defines
#
LOADER_MAX_SIZE = 262144

ifndef FBM_MEM_CFG
export FBM_MEM_CFG = 5363_64MBx2
endif

ifeq "$(AV_RECORD_LIB)" "true"
export MUXER_SUPPORT := true
endif
ifeq "$(MSI)" "true"
export MUXER_SUPPORT := true
endif
ifeq  "$(INTERNAL_SKYPE)" "true"
export MUXER_SUPPORT := true
export INTERNAL_SKYPE := true
endif
#
#
# USB upgrade version
#
export USB_UPG_VERSION := $(VERSION)

export MODEL_NAME BUILD_NAME SERIAL_NUMBER CUSTOMER VERSION TARGET_IC DEFINES

# Teletext support
ifndef SYS_TTX_SUPPORT
    ifeq "$(EU_RUNNING_SPEC)" "taiwan"
        export SYS_TTX_SUPPORT := false
    else
        export SYS_TTX_SUPPORT := true
    endif
endif

# Subtitle support
ifndef SYS_SUBTITLE_SUPPORT
    export SYS_SUBTITLE_SUPPORT := true
endif

# CI Engine
ifndef SYS_CI_SUPPORT
    ifeq "$(EU_RUNNING_SPEC)" "taiwan"
        export SYS_CI_SUPPORT := false
    else
        export SYS_CI_SUPPORT := true
    endif
endif

ifeq "$(SYS_CI_SUPPORT)" "true"
    export CI_ENGINE := oceanblue_ci
    ifndef CI_PLUS_SUPPORT
        export CI_PLUS_SUPPORT := true
    endif
    ifndef CI_PLUS_V13_SUPPORT
       export CI_PLUS_V13_SUPPORT := true
    endif
    ifeq "$(CI_PLUS_SUPPORT)" "true"
        ifndef CIPLUS_PRODUCT_KEY
            export CIPLUS_PRODUCT_KEY := true
        endif
        ifndef DEMO_FAKE_IC
            export DEMO_FAKE_IC := false
        endif
        ifndef COMPATIBLE_CAM_V12
            export COMPATIBLE_CAM_V12 := false
        endif
        ifndef CI_TZ_SUPPORT
            export CI_TZ_SUPPORT := true
        endif
        ifeq "$(CI_TZ_SUPPORT)" "false"
            export CIPLUS_PERM_STORAGE := false
        else
            #      when CI_TZ_SUPPORT is true,you can change CI+ key storage position here                                #
            # "true" indicate the key will be stored in perm,"false" indicate the key will be stored in flash raw partition #
            # if CI_TZ_SUPPORT is false,the key must be stored in flash raw partition                                     #
            export CIPLUS_PERM_STORAGE := false
        endif
        ifndef CI_PLUS_SAS
            export CI_PLUS_SAS := true
        endif
    endif
    ifndef EXTERNAL_CI_SUPPORT
        export EXTERNAL_CI_SUPPORT := false
    endif
endif
ifndef DUAL_AUDIO_DECODERS
    export DUAL_AUDIO_DECODERS := false
endif
# H.264 feature
ifndef SYS_H264_SUPPORT
    export SYS_H264_SUPPORT := true
endif

# EWS_PA support
ifndef EWS_PA_SUPPORT
ifeq "$(EU_RUNNING_SPEC)" "oceania"
    export EWS_PA_SUPPORT := true
endif
endif

# MHEG5 support
ifndef SYS_MHEG5_SUPPORT
    ifeq "$(EU_RUNNING_SPEC)" "taiwan"
        export SYS_MHEG5_SUPPORT := false
    else
 #       export SYS_MHEG5_SUPPORT := true
    endif
endif
ifeq "$(SYS_MHEG5_SUPPORT)" "true"
    # MHEG5 HD
    export MHEG5_HD_SUPPORT := true

    # MHEG5 Interaction Channel
    ifeq "$(INET_SUPPORT)" "true"
        export MHEG5_IC_SUPPORT := true
    endif

    # MHEG5 NVM
    export MHEG5_NVM_SUPPORT := true

    ifeq "$(MHEG5_IC_SUPPORT)" "true"
        export SYS_MHEG5_COEXIST_WITH_SUBTITLE := true
    endif

    ifeq "$(EU_RUNNING_SPEC)" "oceania"
        export SYS_AUS_FREEVIEWHD_SUPPORT := true
        ifeq "$(SYS_AUS_FREEVIEWHD_SUPPORT)" "true"
            export SYS_MHEG5_AUS_SUPPORT := true
        endif

        export SYS_NZ_FREEVIEWHD_SUPPORT  := true
        ifeq "$(SYS_NZ_FREEVIEWHD_SUPPORT)" "true"
            export SYS_MHEG5_NZ_SUPPORT := true
        endif
    else
        export SYS_UK_FREEVIEWHD_SUPPORT := true
        ifeq "$(SYS_UK_FREEVIEWHD_SUPPORT)" "true"
            export SYS_MHEG5_UK_SUPPORT := true
        endif
    endif
endif

# OAD
ifndef SYS_OAD_SUPPORT
    ifeq "$(EU_RUNNING_SPEC)" "taiwan"
        export SYS_OAD_SUPPORT := false
    else
        export SYS_OAD_SUPPORT := true
    endif
endif

# BGM
ifndef SYS_BGM_SUPPORT
    export SYS_BGM_SUPPORT := true
endif

# FLIP/MIRROR support
# default is false
ifndef SYS_FLIP_SUPPORT
    export SYS_FLIP_SUPPORT := true
endif

ifndef SYS_MIRROR_SUPPORT
    export SYS_MIRROR_SUPPORT := true
endif

# Tcon FW  update support
# default is false
ifndef SUPPORT_TCON_UPDATE
    export SUPPORT_TCON_UPDATE := false
endif
#4k2k support
#capri not support 4K,please Do not open it.
ifndef SUPPORT_4K2K
    export SUPPORT_4K2K := false
endif

ifeq "$(MTK_DRM_SUPPORT)" "true"
	ifndef ENABLE_INTERTRUST_SUPPORT 
	    export ENABLE_INTERTRUST_SUPPORT := true
	endif
	#ifndef ENABLE_PLAYREADY_SUPPORT 
	#    export ENABLE_PLAYREADY_SUPPORT := true
	#endif
endif


ifndef ENABLE_PLAYREADY_SUPPORT
export ENABLE_PLAYREADY_SUPPORT := true
endif

ifndef SYS_MS10_SUPPORT
export SYS_MS10_SUPPORT = true
endif

export AUD_SUPPORT_3_DECODER :=1

export AUD_APROC_RENDER := true

#
# Dolby tech. license
#
# NON = no Dolby
# MS10 = MS10 (DDC + DDT)
# DCV = Dolby Digital Plus (with converter)
# DD = Dolby Digital
# DP = Dolby Pulse (with DDENC)
# DDCO = DDCO (with HE-AAC)
# DCV+DDCO = DCV + DDCO
# DD+DDCO = DD + DDCO
# DD+DP = DD + DP
ifndef AUD_DOLBY_SUPPORT
export AUD_DOLBY_SUPPORT = MS10
endif

# OAD version check
#
# CUST_OAD_OUI         should accurate 3 bytes, need prefix 0 when digit is not enough
# CUST_OAD_HW_MODEL    should accurate 2 bytes, need prefix 0 when digit is not enough
# CUST_OAD_HW_VERSION  should accurate 2 bytes, need prefix 0 when digit is not enough
# CUST_OAD_SW_MODEL    should accurate 2 bytes, need prefix 0 when digit is not enough
# CUST_OAD_SW_VERSION  should accurate 2 bytes, need prefix 0 when digit is not enough
ifeq "$(SYS_OAD_SUPPORT)" "true"
export CUST_OAD_OUI := 0x000ce7
export CUST_OAD_HW_MODEL := 0x5882
export CUST_OAD_HW_VERSION := 0x0001
export CUST_OAD_SW_MODEL := 0x1002
export CUST_OAD_SW_VERSION := 0x0090

DEFINES += -DCUST_OAD_OUI=$(CUST_OAD_OUI)
DEFINES += -DCUST_OAD_HW_MODEL=$(CUST_OAD_HW_MODEL)
DEFINES += -DCUST_OAD_HW_VERSION=$(CUST_OAD_HW_VERSION)
DEFINES += -DCUST_OAD_SW_MODEL=$(CUST_OAD_SW_MODEL)
DEFINES += -DCUST_OAD_SW_VERSION=$(CUST_OAD_SW_VERSION)

export SYS_DSMCC_SUPPORT := true
endif

ifndef PKG_ENCRYPT_MODE
export PKG_ENCRYPT_MODE = AES
endif

# [Driver debug use] capture dump support for ncstools
ifndef CDC_DUMP_SUPPORT
export CDC_DUMP_SUPPORT := false
endif

# [Driver debug use] capture dump support for ncstools
ifeq "$(CDC_DUMP_SUPPORT)" "true"
export KERNEL_CONFIG_DIFF += pq_tool
endif

ifeq "$(ETHERNET_DRV_BUILTIN)" "true"
export KERNEL_CONFIG_DIFF += star_mac
endif

ifndef OSS_NOTICE
export OSS_NOTICE := true
endif

ifndef STANDALONE_CLI_MODULE
export STANDALONE_CLI_MODULE := false
endif

ifeq "$(DRV_VALIDATION_BUILD)" "true"
    export WARNING_TO_ERROR := false
endif

#
## form sys_build common folder
#
ifndef SYS_COM_DIR
export SYS_COM_DIR := common
endif

export SYS_COM_FILE_LINK_LIST = sys_network.c \
                                ubi_basic.cfg \
                                ubi_perm.cfg \
                                ubi_3rd.cfg \
                                ubi_3rd_rw.cfg

 #
## form HDMI 2.0 hdcp bin
#
ifndef HDMI_2_0_HDCP_BIN_SUPPORT
export HDMI_2_0_HDCP_BIN_SUPPORT := false
endif         
ifeq "$(L_ENV)" "true"
else
#
# Include customer mak file
#
-include ../../../target/$(TARGET)/customer.mak
endif

