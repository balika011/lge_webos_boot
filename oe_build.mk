#******************************************************************************#
#	DIGITAL TV RESEARCH LAB., LG ELECTRONICS INC., SEOUL, KOREA                #
#	Copyright(c) 1998-2007 by LG Electronics Inc.                              #
#                                                                              #
#   All rights are reserved.                                                   #
#   No part of this work covered by the copyright hereon may be                #
#   reproduced, stored in a retrieval system, in any form or                   #
#   by any means, electronic, mechanical, photocopying, recording              #
#   or otherwise, without the prior permission of LG Electronics.              #
#                                                                              #
#   ---------------------------------------------------------------------------#
#                                                                              #
#	FILE NAME	:	makefile                                                   #
#	VERSION		:	1.0                                                        #
#	AUTHOR		:	Baekwon, Choi(√÷πË±«, bk1472@lge.com)                      #
#	DATE        :	2008/06/32                                                 #
#	DESCRIPTION	:	This is global making for CFE Boot Project                 #
#******************************************************************************#

#Just for verfiyication building in mtk side
.NOTPARALLEL:
##############start purplearrow add, no need to source
##############################################################################
## Setup some environment variables.
##############################################################################
export OE_ENV=false
export L_ENV=true
export LG_SIGN_FLOW := false
export L_DUAL_BOOT := true
export UBOOT_HACK := true
export LG_WEBOS_BUILD := true

export SHRINK_MTK_LOADER_SIZE := true 

ifndef BUILD_CFG
export BUILD_CFG := rel
endif

ifndef LGE_PROTO_PCBA
export LGE_PROTO_PCBA := true
endif

ifndef BUILD_SPC
export BUILD_SPC := EU
endif

include $(shell pwd)/kdriver/Makefile.mak
export KDRIVER_ROOT = $(shell pwd)/kdriver

include  $(KDRIVER_ROOT)/host.mak

DEST_OS=linux
TARGET=5882
TARGET_CPU=arm
#TOOL_CHAIN=4.8.2
#TOOL_CHAIN=webos
#ENABLE_CA9=true


export TOOL_DIR = /mtkoss/gnuarm/arm-lg115x-linux-gnueabi-4.8-2014.08-1-x86_64
TOOL_CHAIN_ROOT=${TOOL_DIR}
#SIGN_DIR=~/secureboot_a2_dist
    SIGN_DIR=$(PWD)/uboot/tools/sign_dir

#SIGN_DIR=~/secureboot_a2_dist
export CROSS_TOOLCHAIN_SYSROOT ?= $(TOOL_CHAIN_ROOT)/usr
export CROSS_TOOLCHAIN_SYSROOT_LIB ?= $(TOOL_CHAIN_ROOT)/lib
export TOOL_CHAIN_BIN_PATH				:= $(TOOL_CHAIN_ROOT)/bin
export LIBC_PATH_482 := $(shell pwd)/library/arm-lg115x-linux-gnueabi-4.8-2014.08-1-x86_64/without-multi-arch-lib

export CROSS_COMPILE := /mtkoss/gnuarm/arm-lg115x-linux-gnueabi-4.8-2014.08-1-x86_64/bin/arm-lg115x-linux-gnueabi-



HOST_NAME=`hostname`

FLAG_IS_DIST=YES
SET_DIST_OUT_DIR=1
SIGN_DIST=YES
export SET_DIST_OUT_DIR

TOOL_BIN=${TOOL_DIR}/bin/TOOL_BIN_NOT_DEFINE
TOOL_LIB=${TOOL_DIR}/armv7a-mediatek-linux-gnueabi/lib/TOOL_LIB_NOT_DEFINE
IMG_TOOL_BIN=${TOOL_DIR}/image-tools/IMG_TOOL_BIN_NOT_DEFINE
SDE_BIN=/opt/toolchains/sde5/bin/SDE_BIN_NOT_DEFNIE


SIGN_TOOL_DIR=${SIGN_DIR}/tool
SIGN_KEY_DIR=${SIGN_DIR}/key
SIGN_SCRIPT=${SIGN_TOOL_DIR}/sign.sh
SIGN_USE_PARTIAL=YES


PROJECT_NAME=GP4-MTK
PLATFORM_CHIP=mtk5882
PLATFORM_CHIP_REV=A2
PLATFORM_NAME=${PLATFORM_CHIP}
PLATFORM=${PLATFORM_CHIP}
PLATFORM_ALLOW=mtk5882:
USE_EMMC=YES
#USE_EMMC_HYNIX="NO"
#USE_EMMC_4GB="YES"

USE_KERNEL_V3=YES
DIST_REV_NO=34205M

export DEST_OS TARGET TARGET_CPU TARGET_TOOL CROSS_COMPILE
export TOOL_DIR TOOL_BIN TOOL_LIB IMG_TOOL_BIN TOOL_CHAIN_ROOT SDE_BIN DIST_REV_NO
export PROJECT_NAME PLATFORM_CHIP PLATFORM_CHIP_REV PLATFORM_NAME PLATFORM PLATFORM_ALLOW
export TOOL_CHAIN USE_EMMC USE_EMMC_HYNIX USE_EMMC_4GB USE_KERNEL_V3
export SIGN_DIR SIGN_TOOL_DIR SIGN_KEY_DIR SIGN_SCRIPT SIGN_DIST SIGN_USE_PARTIAL

##############end purplearrow add
#==============================================================================
#	Include real configuration file located in project directory
#------------------------------------------------------------------------------
ECHO			 = echo
COPY_TFTP		 =	$(PWD)/../tools/copy_tftp
BOOT_RESULT_DIR	 = ./result
BOOT_TFTP_DIR	 = ../tftp

#==============================================================================
#		Tools Definition
#==============================================================================
CD				 = cd
RM				 = rm -rf
LN				 = ln -s
CP				 = cp
CP_FLAG			 = -rf
MK_EPAK			 = uboot/tools/mkepk

#==============================================================================
#		Variables
#==============================================================================
TOP_DIR			 =	.
DEBUG			?= 0
TIME			?= 0
CHP_REV			:= $(PLATFORM_CHIP_REV)
CHP_REV			?= A1 # PLATFORM_CHIP_REV is not defined
BOOT_NAME		= $(BOOT)-boot
BOOT_NAME_PT	= $(BOOT_NAME)-PT
BOOT_NAME_DV	= $(BOOT_NAME)-DV
SOC_UBOOT		= uboot
BOOT_ROOT 		= $(TOP_DIR)
CONFIG_DDR_H	 = ../drv_mtk5396/kdriver/target/mt5396/5396_driver/bsp/dram/ddr.h
CONFIG_DDR_C	 = ../drv_mtk5396/kdriver/target/mt5396/5396_driver/bsp/dram/ddr_cal3.c
export COMPANY        := LGE

ifeq ($(USE_EMMC), YES)
BOOT 			= emmc
BOOT_TYPE		= ROM2EMMC
else
ifeq ($(USE_EMMC), NO)
$(error *** "*** YOU SHOULD USE EMMC NOW!.")
#BOOT 			= nand
#BOOT_TYPE		= ROM2NAND
else
$(error *** "*** USE_EMMC is not defined, Please, adapt latest xxx.env file.")
endif
endif
NOLOG			:= n
SECURE_BOOT    	:= y
FULL_VERIFY_OFF	:= n
UBOOT_VERSION 	:= 2011.12



ifndef DRV_CUSTOM
DRV_CUSTOM = $(MODEL)
endif

ifndef FW_UPG_SUPPORT
export FW_UPG_SUPPORT := LGMTK
endif
export NO_DLM := true

#if UNSECURE = y, we can enter boot prompt even if It is RELEASE_LEVEL.
#if UNSECURE = n, we can't enter boot prompt in RELEASE_LEVEL.
UNSECURE		:= n
export SECURE_BOOT UBOOT_VERSION MODEL TARGET_IC BOOT BOOT_TYPE UNSECURE DRV_CUSTOM FULL_VERIFY_OFF

BUILD_VERS 		= $(shell grep -i "^[0-9]*\\.[0-9]*\\..*$i" $(TOP_DIR)/boot_history.txt | head -1 | awk '{ print $$1 }' | tr -d ":")
MODEL_NAME		= MTK_BOOT
COMPILE_DATE	= $(shell date +%D)

# SILENT LIST : add target to here
.SILENT:	all prepare_build help clean normal pt dv help re

all   : prepare_build normal

ifeq "$(SIGN_DIST)" "NO"
	cd $(BOOT_RESULT_DIR) && rm -f boot.bin; ln -s $(BOOT_NAME_DV).bin boot.bin
	cd $(BOOT_RESULT_DIR) && rm -f boot.pak; ln -s $(BOOT_NAME_DV).pak boot.pak
	$(COPY_TFTP) $(PLATFORM_NAME) $(BOOT_RESULT_DIR)
	$(ECHO) -e "\n============================================================="
	$(ECHO) -e " PLEASE DON'T FORGET TO COMMIT ALL BOOT BINARIES !!\n"
	$(ECHO) -e "   $(BOOT_RESULT_DIR)/$(BOOT_NAME_DV).bin"
	$(ECHO) -e "   $(BOOT_RESULT_DIR)/$(BOOT_NAME_DV).pak"
	$(ECHO) -e "==============================================================="
endif

prepare_build:
	if [ ! -d $(BOOT_RESULT_DIR) ]; then \
		mkdir -p $(BOOT_RESULT_DIR);\
	fi
	# Change to Large NAND
	sed -e 's/\/\/#define LARGE_NAND_PAGESIZE/#define LARGE_NAND_PAGESIZE/g' -i $(BOOT_ROOT)/$(SOC_UBOOT)/u-boot-$(UBOOT_VERSION)/include/configs/$(TARGET_IC).h

normal: dv

dv: prepare_build
#	-svn up $(CONFIG_DDR_H)
#	-svn up $(CONFIG_DDR_C)
	-make clean
	$(ECHO) -e "\n============================================================="
	$(ECHO) -e " Building normal DV boot..."
	$(ECHO) -e "=============================================================\n"
	$(MAKE) -C $(SOC_UBOOT) all SECURE_BOOT=$(SECURE_BOOT) NOLOG=$(NOLOG) BUILD_VERS=$(BUILD_VERS) TRUSTZONE=$(TRUSTZONE) BOARD_TYPE=DV SIGN_DIST=$(SIGN_DIST) FULL_VERIFY_OFF=$(FULL_VERIFY_OFF)
ifneq "$(SECURE_BOOT)" "y"
	$(CP) $(CP_FLAG) $(BOOT_ROOT)/$(SOC_UBOOT)/$(MODEL)_$(BOOT)boot.bin $(BOOT_RESULT_DIR)/$(BOOT_NAME_DV).bin
	$(CHMOD) +x $(MK_EPAK)
	$(MK_EPAK) -c $(BOOT_RESULT_DIR)/boot.pak $(BOOT_ROOT)/$(SOC_UBOOT)/u-boot_pack_secure.lzhs	\
				boot $(MODEL_NAME) $(BUILD_VERS) $(COMPILE_DATE) 0x0 RELEASE
else
	$(CP) $(CP_FLAG) $(BOOT_ROOT)/$(SOC_UBOOT)/$(MODEL)_secure_$(BOOT)boot.bin $(BOOT_RESULT_DIR)/$(BOOT_NAME_DV).bin
	$(CHMOD) +x $(MK_EPAK)
	$(MK_EPAK) -c $(BOOT_RESULT_DIR)/boot.pak $(BOOT_ROOT)/$(SOC_UBOOT)/u-boot_pack.lzhs	\
				boot $(MODEL_NAME) $(BUILD_VERS) $(COMPILE_DATE) 0x0 RELEASE
endif
	$(MK_EPAK) -c $(BOOT_RESULT_DIR)/secureboot.pak $(BOOT_ROOT)/$(SOC_UBOOT)/mtkloader.bin	\
				secureboot $(MODEL_NAME) $(BUILD_VERS) $(COMPILE_DATE) 0x0 RELEASE
	$(MK_EPAK) -c $(BOOT_RESULT_DIR)/tzfw.pak $(BOOT_RESULT_DIR)/tzfw.bin	\
				tzfw $(MODEL_NAME) $(BUILD_VERS) $(COMPILE_DATE) 0x0 RELEASE

re:
	# release image
	$(ECHO) *** copy $(addprefix $(BOOT_RESULT_DIR)/,$(BOOT_NAME)) to $(BOOT_TFTP_DIR)
	$(CP) $(BOOT_RESULT_DIR)/$(BOOT_NAME)* $(BOOT_TFTP_DIR)


install: clean all re
	$(ECHO)	""
	$(ECHO) "###############################################################"
	$(ECHO)	" Please, commit these things."
	$(ECHO)	" 1. boot source on os/$(PLATFORM_NAME)_linux/boot"
	$(ECHO)	" 2. boot image  on os/$(PLATFORM_NAME)_linux/tftp"
	$(ECHO)	""
	$(ECHO) "-- check modified files. (This list does not include added files) --"
	svn st -uq . $(BOOT_TFTP_DIR)
	$(ECHO) "###############################################################"

clean:
	$(MAKE) -C $(SOC_UBOOT) clean

packing_uboot: prepare_build
	make clean
	@$(ECHO) -e "\n============================================================="
	@$(ECHO) -e " Packing uboot ..."
	@$(ECHO) -e "=============================================================\n"
	$(MAKE) -C $(SOC_UBOOT) -f packing_uboot.mak all  SECURE_BOOT=$(SECURE_BOOT)

help:
	$(ECHO) ""
	$(ECHO) "============================================================="
	$(ECHO) " make : "
	$(ECHO) "============================================================="
