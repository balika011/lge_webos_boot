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
# $RCSfile: path.mak,v $
# $Revision: #1 $
# $Date: 2015/01/09 $
# $Author: dtvbm11 $
#
# Description:
#         Root path definitions for all components. It is assumed that
#         PROJECT_ROOT has alreday been set.
#
#############################################################################

#
# Get host specific definitions
#
include $(PROJECT_ROOT)/host.mak

export PROJECT_ROOT


#
# Set a common target
#
ifndef TARGET
ifdef QA_TARGET
TARGET = $(QA_TARGET)
endif

ifdef UT_TARGET
TARGET = $(UT_TARGET)
endif

ifdef TOOL_TARGET
TARGET = $(TOOL_TARGET)
endif
endif


#
# Current path.
#
THIS_PATH      = $(shell bash -c pwd -L)
#ifeq ($(strip $(findstring project_x, $(THIS_PATH))),)
COMP_PATH      = $(word 2, $(subst $(LINUX_PORTING_DTV_LINUX_PRJ_PATH)/,$(LINUX_PORTING_DTV_LINUX_PRJ_PATH)/ , $(THIS_PATH)))
#else
#COMP_PATH      = $(word 2, $(subst $(PROJECT_ROOT)/,$(PROJECT_ROOT)/ , $(THIS_PATH)))
#endif

QA_COMP_PATH   := $(word 2, $(subst $(QA_ROOT)/,$(QA_ROOT)/ , $(THIS_PATH)))
UT_COMP_PATH   := $(word 2, $(subst $(UT_ROOT)/,$(UT_ROOT)/ , $(THIS_PATH)))
TOOL_COMP_PATH := $(word 2, $(subst $(TOOL_ROOT)/,$(TOOL_ROOT)/ , $(THIS_PATH)))


export THIS_PATH
export COMP_PATH

ifeq "$(LINUX_SOLUTION)" "true"
    # 3rd library path
    ifndef VM_LINUX_ROOT
        VM_LINUX_ROOT := $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell pwd)))
    endif

    ifndef DTV_LINUX_ROOT
        DTV_LINUX_ROOT := $(VM_LINUX_ROOT)/dtv_linux
    endif

    ifndef CHILING_ROOT
        CHILING_ROOT := $(VM_LINUX_ROOT)/chiling
    endif

	TOOL_CHAIN ?= 4.2.1

	export VFP_SUFFIX  = 
ifeq "$(TOOL_CHAIN)" "4.5.1"
	ifeq "$(ENABLE_VFP)" "true"
		export VFP_SUFFIX = _vfp
	else 
		ifeq "$(ENABLE_CA9)" "true"
			export VFP_SUFFIX = _vfp_ca9
		endif
	endif	
else ifeq "$(TOOL_CHAIN)" "4.8.2"
  ifeq "$(ENABLE_CA9)" "true"
				export VFP_SUFFIX = _vfp_ca9
		else ifeq "$(ENABLE_CA9_NEON)" "true"
				export VFP_SUFFIX = _neon_ca9
		endif	
else
	ifeq "$(ENABLE_VFP)" "true"
	export VFP_SUFFIX  = _vfp
	endif
endif

    THIRD_PARTY_ROOT := $(VM_LINUX_ROOT)/third_party/library/gnuarm-$(TOOL_CHAIN)$(VFP_SUFFIX)

    #electric-fence
    EFENCE_LIB_DIR   := $(THIRD_PARTY_ROOT)/electric-fence
    
    # DLNA
    DLNA_LIB_DIR     := $(THIRD_PARTY_ROOT)/dlna/lib

    # OPENSSL
    OPENSSL_LIB_PATH := $(THIRD_PARTY_ROOT)/openssl/openssl-1.0.0/lib
    OPENSSL_INC_PATH := $(THIRD_PARTY_ROOT)/openssl/openssl-1.0.0/include

    # libcurl
    LIBCURL_LIB_PATH := $(THIRD_PARTY_ROOT)/curl/curl-7.21.4/lib
    LIBCURL_INC_PATH := $(THIRD_PARTY_ROOT)/curl/curl-7.21.4/include
    
    # CARES
    LIBCARES_LIB_PATH := $(THIRD_PARTY_ROOT)/c-ares/cares-1.7.4/lib
    LIBCARES_INC_PATH := $(THIRD_PARTY_ROOT)/c-ares/cares-1.7.4/include

    # expat
    EXPAT_LIB_PATH   := $(THIRD_PARTY_ROOT)/expat/expat_2.0.1/lib
    EXPAT_INC_PATH   := $(THIRD_PARTY_ROOT)/expat/expat_2.0.1/include

    # SECURESTORAGE
    SECURESTORAGE_LIB_PATH   := $(THIRD_PARTY_ROOT)/securestorage/lib
    SECURESTORAGE_INC_PATH   := $(THIRD_PARTY_ROOT)/securestorage/include

    # NET UTILITY
    NETUTIL_LIB_PATH   := $(THIRD_PARTY_ROOT)/netutil/lib
    NETUTIL_INC_PATH   := $(THIRD_PARTY_ROOT)/netutil/include
    
    # UpdateLogic
    UPDATETV_LIB_PATH   := $(THIRD_PARTY_ROOT)/ULI
    UPDATETV_INC_PATH   := $(THIRD_PARTY_ROOT)/ULI/include

    # ALSA    
    ALSA_INC_PATH           := $(THIRD_PARTY_ROOT)/alsa/include
    ALSA_LIB_PATH           := $(THIRD_PARTY_ROOT)/alsa/lib

    # DBus
    DBUS_LIB_PATH           := $(THIRD_PARTY_ROOT)/dbus/lib
    DBUS_INC_PATH           := $(THIRD_PARTY_ROOT)/dbus/include
    DBUS_ARCH_DEPS_INC_PATH := $(THIRD_PARTY_ROOT)/dbus/lib/dbus-1.0/include

	# directfb
    DIRECTFB_PATH   := $(VM_LINUX_ROOT)/chiling/driver/directfb

 # IPERF
    IPERF_LIB_DIR     := $(THIRD_PARTY_ROOT)/iperf
    
    #MHEG5
	MHEG5_LIB_DIR	:= $(THIRD_PARTY_ROOT)/mheg5
	
	export DIRECTFB_PATH

    export THIRD_PARTY_ROOT

    export DLNA_LIB_DIR

    export OPENSSL_LIB_PATH
    export OPENSSL_INC_PATH

    export LIBCURL_LIB_PATH
    export LIBCURL_INC_PATH

    export LIBCARES_LIB_PATH
    export LIBCARES_INC_PATH

    export EXPAT_LIB_PATH
    export EXPAT_INC_PATH

    export SECURESTORAGE_LIB_PATH
    export SECURESTORAGE_INC_PATH

    export NETUTIL_LIB_PATH
    export NETUTIL_INC_PATH
    
    export UPDATETV_LIB_PATH
    export UPDATETV_INC_PATH

    export ALSA_INC_PATH
    export ALSA_LIB_PATH    

    export DBUS_LIB_PATH
    export DBUS_INC_PATH
    export DBUS_ARCH_DEPS_INC_PATH
    
    export IPERF_LIB_DIR
    
    export MHEG5_LIB_DIR
endif

#
# Set version / customer fields if required
#
ifndef BUILD_NAME
BUILD_NAME := dev
endif

ifndef CUSTOMER
CUSTOMER := cmtk / mtk
endif

ifndef VERSION
VERSION := 1.00
endif

BUILD_NAME_DEF := "STR_DEF ($(BUILD_NAME))"
CUSTOMER_DEF   := "STR_DEF ($(CUSTOMER))"
VERSION_DEF    := "STR_DEF ($(VERSION))"


#
# Default target is linux_pc. However, if a makefile is called from within
# a target then use that target as TARGET_ROOT. In case a target is being
# built from within a target directory then TARGET must be set to the same
# target or must not be defined at all.
#
ifndef TARGET_ROOT
ifneq ($(strip $(findstring /project_x/target/, $(THIS_PATH))),)
PATH_TARGET := $(strip $(firstword $(subst /, ,$(word 2, $(subst /project_x/target/,/project_x/target/ , $(THIS_PATH))))))

ifeq "$(origin TARGET)" "command line"
ifneq "$(TARGET)" "$(PATH_TARGET)"
$(error Conflicting targets. Currently in target "$(PATH_TARGET)" and specified target "$(TARGET)")
endif
endif

TARGET := $(PATH_TARGET)

TARGET_ROOT := $(word 1, $(subst /project_x/target/,/project_x/target /, $(THIS_PATH)))/$(TARGET)
else
ifndef TARGET
TARGET := linux_pc
endif

TARGET_ROOT := $(PROJECT_ROOT)/target/$(TARGET)
endif
endif

#
# TARGET_BASE references to ".../project_x/target".
#
ifndef TARGET_BASE
ifneq ($(strip $(findstring /project_x/target/, $(TARGET_ROOT))),)
TARGET_BASE := $(firstword $(subst /project_x/target/,/project_x/target /, $(TARGET_ROOT)))
endif
endif

#
# Default custom is dev. In case CUSTOM_ROOT is not set yet, check the current path for
# '/project_x/custom/'. If that pattern is found, use the path to set CUSTOM_ROOT. The
# first directory name after '/project_x/custom' (if available), will be stored in CUSTOM.
# If there is no first directy name after '/project_x/custom/' then set 'dev' in CUSTOM.
# And eventually, set the CUSTOM_ROOT variable. In case CUSTOM_ROOT is alreday set, check
# the current path for '/project_x/custom'. If that pattern is present, then adjust CUSTOM
# as described before.
#
DEFAULT_CUSTOM := dev

ifndef CUSTOM_ROOT
ifneq ($(strip $(findstring /project_x/custom/, $(THIS_PATH))),)
CUSTOM := $(strip $(firstword $(subst /, , $(word 2, $(subst /project_x/custom/,/project_x/custom/ , $(THIS_PATH))))))

ifeq ($(strip $(CUSTOM)),)
CUSTOM := $(DEFAULT_CUSTOM)
endif

CUSTOM_ROOT := $(firstword $(subst /project_x/custom/,/project_x/custom /, $(THIS_PATH)))/$(CUSTOM)
else

ifeq ($(strip $(CUSTOM)),)
CUSTOM := $(DEFAULT_CUSTOM)
endif

CUSTOM_ROOT := $(PROJECT_ROOT)/custom/$(CUSTOM)
endif
else
ifneq ($(strip $(findstring /project_x/custom/, $(THIS_PATH))),)
CUSTOM := $(strip $(firstword $(subst /, , $(word 2, $(subst /project_x/custom/,/project_x/custom/ , $(THIS_PATH))))))

ifeq ($(strip $(CUSTOM)),)
CUSTOM := $(DEFAULT_CUSTOM)
endif

CUSTOM_ROOT := $(firstword $(subst /project_x/custom/,/project_x/custom /, $(THIS_PATH)))/$(CUSTOM)
endif
endif


#
# Ensure that $(CUSTOM_ROOT)/common.mak is avilable. If not, add the next sub-directory
# from $(THIS_PATH) to $(CUSTOM_ROOT). However, $(THIS_PATH) must contain $(CUSTOM_ROOT)
# else there is an error.
#
ifeq ($(strip $(wildcard $(CUSTOM_ROOT)/common.mak)),)
CUSTOM_DIR := $(strip $(word 2,$(subst $(CUSTOM_ROOT)/,$(CUSTOM_ROOT)/ , $(THIS_PATH))))

ifneq ($(strip $(CUSTOM_DIR)),)
CUSTOM_ROOT := $(CUSTOM_ROOT)/$(word 1, $(subst /, ,$(CUSTOM_DIR)))

ifeq ($(strip $(wildcard $(CUSTOM_ROOT)/common.mak)),)
CUSTOM_ROOT := $(CUSTOM_ROOT)/$(word 2, $(subst /, ,$(CUSTOM_DIR)))
endif

ifeq ($(strip $(wildcard $(CUSTOM_ROOT)/common.mak)),)
$(error Cannot find "$(CUSTOM_ROOT)/common.mak".)
endif

else
$(error Cannot set CUSTOM_ROOT. Path "$(CUSTOM_ROOT)" is not part of "$(THIS_PATH)".)
endif
endif


#
# CUSTOM_BASE references to ".../project_x/custom".
#
ifndef CUSTOM_BASE
ifneq ($(strip $(findstring /project_x/custom/, $(CUSTOM_ROOT))),)
CUSTOM_BASE := $(firstword $(subst /project_x/custom/,/project_x/custom /, $(CUSTOM_ROOT)))
endif
endif


#
# Do not reset the C_INC_DIR variable
#
ifndef C_INC_DIR
C_INC_DIR := $(CUSTOM_ROOT)/c_inc
endif

export CUSTOM_ROOT
export CUSTOM_BASE
export C_INC_DIR


# Determine the CUSTOM_OUTPUT, if is building not from a sysbuild, the default CUSTOM will be "dev".

CUSTOM_OUTPUT := $(CUSTOM)

ifeq "$(CUSTOM_OUTPUT)" ""
    CUSTOM_OUTPUT := $(DEFAULT_CUSTOM)
endif

# Overwrite the CUSTOM_OUTPUT if CUSTOMER and MODEL_NAME are defined

ifdef MODEL_NAME
    ifdef CUSTOMER
        ifeq "$(CUSTOMER)" "cmtk / mtk"
            CUSTOMER := mtk
        endif
        CUSTOM_OUTPUT := $(CUSTOMER)/$(MODEL_NAME)
    endif
endif

# Include the Object Type determination

include $(PROJECT_ROOT)/object_type.mak

ifndef TARGET_LIB_DIR
TARGET_LIB_DIR := $(TARGET_ROOT)/lib_target/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
endif
ifndef LIB_LIB_DIR
LIB_LIB_DIR    := $(TARGET_ROOT)/lib_lib/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
endif
OBJ_DIR        := $(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(COMP_PATH)
PRIV_OBJ_DIR   := $(TARGET_PRIV_ROOT)/obj/$(GLOBAL_CHIP_SET)/$(OBJECT_TYPE)/$(COMP_PATH)
ASM_DIR        := $(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(COMP_PATH)
DEPEND_DIR     := $(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(COMP_PATH)
LINT_DIR       := $(TARGET_ROOT)/lint/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(COMP_PATH)
X_INC_DIR      := $(TARGET_ROOT)/x_inc
D_INC_DIR      := $(TARGET_BASE)/d_inc
ALL_OBJ_DIR        := $(TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)
ALL_LIB_LIB_DIR    := $(TARGET_ROOT)/lib_lib/$(CUSTOM_OUTPUT)
ALL_MW_LIB_DIR     := $(TARGET_ROOT)/lib_mw/$(CUSTOM_OUTPUT)
ALL_DTV_LINUX_LIB_DIR     := $(TARGET_ROOT)/lib_dtv_process/$(CUSTOM_OUTPUT)
ALL_CUSTOM_LIB_DIR := $(TARGET_ROOT)/lib_custom/$(CUSTOM_OUTPUT)
ALL_TARGET_LIB_DIR := $(TARGET_ROOT)/lib_target/$(CUSTOM_OUTPUT)

export TARGET
export TARGET_LIB_DIR
export LIB_LIB_DIR
export TARGET_ROOT
export TARGET_BASE
export OBJ_DIR
export PRIV_OBJ_DIR
export ASM_DIR
export DEPEND_DIR
export LINT_DIR
export X_INC_DIR
export D_INC_DIR
export ALL_OBJ_DIR
export ALL_LIB_LIB_DIR
export ALL_MW_LIB_DIR
export ALL_DTV_LINUX_LIB_DIR
export ALL_CUSTOM_LIB_DIR
export ALL_TARGET_LIB_DIR

#
# Default QA target is linux_pc
#
ifndef QA_ROOT
QA_ROOT := $(PROJECT_ROOT)/qa
endif

ifndef QA_TARGET_ROOT
ifndef QA_TARGET
QA_TARGET := $(TARGET)
endif

QA_TARGET_ROOT := $(QA_ROOT)/test_target/$(QA_TARGET)
endif

QA_UTIL         := $(QA_ROOT)/lib_src/util
QA_TEST_SRC_DIR := $(QA_ROOT)/test_src
QA_BIN_DIR      := $(QA_TARGET_ROOT)/bin
QA_LIB_DIR      := $(QA_TARGET_ROOT)/qa_lib
QA_OBJ_DIR      := $(QA_TARGET_ROOT)/obj/$(QA_COMP_PATH)
QA_EXT_OBJ_ROOT := $(QA_TARGET_ROOT)/obj/test_src
QA_DEPEND_DIR   := $(QA_TARGET_ROOT)/obj/$(QA_COMP_PATH)
QA_LINT_DIR     := $(QA_TARGET_ROOT)/lint/$(QA_COMP_PATH)

export QA_ROOT
export QA_UTIL
export QA_TEST_SRC_DIR
export QA_BIN_DIR
export QA_LIB_DIR
export QA_OBJ_DIR
export QA_EXT_OBJ_DIR
export QA_DEPEND_DIR
export QA_LINT_DIR
export QA_TARGET


#
# Default UT target is linux_pc
#
ifndef UT_ROOT
UT_ROOT := $(PROJECT_ROOT)/ut
endif

ifndef UT_TARGET_ROOT
ifndef UT_TARGET
UT_TARGET := $(TARGET)
endif

UT_TARGET_ROOT := $(UT_ROOT)/target/$(UT_TARGET)
endif

UT_BIN_DIR      := $(UT_TARGET_ROOT)/bin/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
UT_OBJ_DIR      := $(UT_TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(UT_COMP_PATH)
UT_DEPEND_DIR   := $(UT_TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(UT_COMP_PATH)
UT_LINT_DIR     := $(UT_TARGET_ROOT)/lint/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(UT_COMP_PATH)

export UT_ROOT
export UT_BIN_DIR
export UT_OBJ_DIR
export UT_DEPEND_DIR
export UT_LINT_DIR
export UT_TARGET_ROOT
export UT_TARGET


#
# Default TOOL target is linux_pc
#
ifndef TOOL_TARGET_ROOT
ifndef TOOL_TARGET
TOOL_TARGET := $(TARGET)
endif

TOOL_TARGET_ROOT := $(PROJECT_ROOT)/tools/target/$(TOOL_TARGET)
endif

TOOL_ROOT       := $(PROJECT_ROOT)/tools
TOOL_BIN_DIR    := $(TOOL_TARGET_ROOT)/bin/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
TOOL_OBJ_DIR    := $(TOOL_TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(TOOL_COMP_PATH)
TOOL_DEPEND_DIR := $(TOOL_TARGET_ROOT)/obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(TOOL_COMP_PATH)
TOOL_LINT_DIR   := $(TOOL_TARGET_ROOT)/lint/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(TOOL_COMP_PATH)

export TOOL_ROOT
export TOOL_BIN_DIR
export TOOL_OBJ_DIR
export TOOL_DEPEND_DIR
export TOOL_LINT_DIR
export TOOL_TARGET
export TOOL_TARGET_ROOT


#
# Specify some tools
#
PRC_CB      = $(TOOL_ROOT)/prc_cb/prc_cb
SECT_GEN    = $(TOOL_ROOT)/sect_gen/sect_gen
GEN_CFG     = $(TOOL_ROOT)/gen_cfg/gen_cfg_$(PROJECT_X_HOST)

export PRC_CB
export PRC_CB_FLAG

export SECT_GEN
export SECT_GEN_FLAG

export GEN_CFG
export GEN_CFG_FLAG



#
# Set path to middleware
#
ifndef MW_ROOT
MW_ROOT := $(PROJECT_ROOT)/middleware
endif

ifndef MW_LIB_DIR
MW_LIB_DIR     := $(TARGET_ROOT)/lib_mw/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
endif
ifndef CUSTOM_LIB_DIR
CUSTOM_LIB_DIR := $(TARGET_ROOT)/lib_custom/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
endif
ifndef DTV_LINUX_LIB_DIR
DTV_LINUX_LIB_DIR     := $(TARGET_ROOT)/lib_dtv_process/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)
endif

export MW_ROOT
export MW_LIB_DIR
export CUSTOM_LIB_DIR
export DTV_LINUX_LIB_DIR


#
# Export common variables
#
THIS_COMPONENT := $(strip $(THIS_COMPONENT))
export THIS_COMPONENT
export SUB_COMPONENTS
export OPTIONAL_SUB_COMPONENTS
export SEL_SUB_COMPONENTS
export SRC
export ASM_SRC
export OBJ
export DEFINES
export ASM_DEFINES
export CC_INC
export LIB

export BUILD_NAME
export BUILD_NAME_DEF
export CUSTOMER
export CUSTOMER_DEF
export VERSION
export VERSION_DEF
export CUSTOMER_VERSION

export TEST_NAME
export TOOL_NAME

export NO_SUB_COMP
export NO_OPT_SUB_COMP


#
# Include the local 'cfg.mak'
#
include $(PROJECT_ROOT)/cfg_inc.mak


#
# Handle defines specified in an external file
#
ifneq ($(strip $(DEF_FILE_1)),)
ifneq ($(strip $(FLAG_DEF_FILE_1)),TRUE)
DEFINES += $(shell $(CAT) $(CAT_FLAG) $(DEF_FILE_1))

FLAG_DEF_FILE_1=TRUE
endif
endif

ifneq ($(strip $(DEF_FILE_2)),)
ifneq ($(strip $(FLAG_DEF_FILE_2)),TRUE)
DEFINES += $(shell $(CAT) $(CAT_FLAG) $(DEF_FILE_2))

FLAG_DEF_FILE_2=TRUE
endif
endif

export FLAG_DEF_FILE_1
export FLAG_DEF_FILE_2


#
# Map suffix definitions
#
MAP_SUFFIX := _map.txt

export MAP_SUFFIX

#
# Tools for PRIV BUILD
#
PRIV_BUILD_SHELL = ${TOOL_ROOT}/priv_build/priv_build.sh
export PRIV_BUILD_SHELL


#
# Set up for Screen Output
#
SCREEN_TOKEN=$(PROJECT_ROOT)/.screen
export SCREEN_TOKEN

#if there is ATD/atd string in BRDCST_IF,it will define USE_ATD_IF_DEMOD
ifneq ($(findstring ATD,$(shell echo $(BRDCST_IF) | tr 'a-z' 'A-Z')),)
DEFINES += -DUSE_ATD_IF_DEMOD
endif

#
# ROOT for Release Folder
#
export RLS_OBJ_ROOT = $(PROJECT_ROOT)/mtk_obj/$(CUSTOM_OUTPUT)/$(OBJECT_TYPE)/$(COMP_PATH)



ifeq "$(RLS_CUSTOM_BUILD)" "true"
ifneq ($(strip $(findstring _build/_mtkloader/project_x, $(PROJECT_ROOT))),)
MY_OBJECT_TYPE := $(subst /$(APPEND_OBJ_TYPE),/mtkloader/$(APPEND_OBJ_TYPE),$(OBJECT_TYPE))
export RLS_OBJ_ROOT = $(PROJECT_ROOT)/mtk_obj/$(CUSTOM_OUTPUT)/$(MY_OBJECT_TYPE)/$(COMP_PATH)
endif
endif

