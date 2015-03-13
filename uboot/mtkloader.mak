###############################################################################
# chiling/uboot/mtkloader.mak                                                 
#                                                                             
# SMP core init                                                               
#                                                                             
# Copyright (c) 2010-2012 MediaTek Inc.                                       
# $Author: p4admin $                                                    
#                                                                             
# This program is free software; you can redistribute it and/or modify        
# it under the terms of the GNU General Public License version 2 as           
# published by the Free Software Foundation.                                  
#                                                                             
# This program is distributed in the hope that it will be useful, but WITHOUT 
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   
# more details.                                                               
###############################################################################

unexport MW_MODEL
unexport LINUX_SOLUTION
unexport ROOTFS_NAME
unexport KEY_FROM_DFB
unexport AV_BYPASS 
unexport UBOOT_VERSION
unexport APP_IF
unexport RAMDISK_ROOTFS
unexport MMP_SUPPORT
unexport RELEASE_3RD_LIB_LIST
unexport TARGET
unexport CUSTOM
unexport CUST_MODEL
unexport MTAL_SUPPORT
#unexport TOOL_CHAIN
unexport SUPPORT_PIP
unexport OPTIMIZE_LVL
unexport AV_BYPASS
unexport MMP_SUPPORT
unexport INET_SUPPORT
unexport FW_UPG_SUPPORT
unexport DIVX_DRM
unexport USB_UPG_VERSION
unexport SERIAL_NUMBER VERSION
unexport PROJECT_ROOT TARGET_ROOT UBOOT_LIBRARY MW_LIB_DIR LINUX_DRV_ROOT THUMB
unexport UBOOT_ROOT
#unexport CROSS_COMPILE
#unexport CPPFLAGS
unexport CFLAGS
unexport AFLAGS
unexport ROM_CODE
unexport CC_SECURE_ROM_DEF
unexport CPPFLAGS CFLAGS AFLAGS 
unexport CC_LOADER_DEF
unexport CC_PRELOADER_DEF
unexport COMMON_INC OSAI_INC
unexport AS
unexport LD
unexport CC
unexport AR
unexport OBJCOPY

export DEFINES=
#
# Executable name
#
ifndef BUILD_LINUX_LOADER
BUILD_LINUX_LOADER := true
endif
export BUILD_LINUX_LOADER

MODEL_NAME ?= $(MODEL)
export EXE_NAME := $(MODEL_NAME)

ifeq "$(BUILD_CFG)" "debug"
export EXE_NAME := $(EXE_NAME)_dbg

else
ifeq "$(BUILD_CFG)" "cli"
export EXE_NAME := $(EXE_NAME)_cli
endif
endif

export MAP_NAME := $(EXE_NAME)$(MAP_SUFFIX)

RLS_CUSTOM_BUILD ?= false


ROOTDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

VM_LINUX_ROOT ?= $(word 1, $(subst /vm_linux/,/vm_linux /, $(ROOTDIR)))

MTKLOADER_SUBPATH ?= _build/_mtkloader/project_x

ifdef MTKLOADER_ROOT
export PROJECT_ROOT = $(MTKLOADER_ROOT)
else
export PROJECT_ROOT = $(VM_LINUX_ROOT)/chiling/uboot/$(MTKLOADER_SUBPATH)
endif

#LG_CHG zaewon.lee
-include $(VM_LINUX_ROOT)/project_x/host.mak
export WARNING_TO_ERROR=false

ifeq "$(BUILD_CFG)" "debug"
export MODE ?= debug
endif

ifeq "$(BUILD_CFG)" "cli"
export MODE ?= debug
endif

export MODE ?= release

ifeq "$(SECURE_BOOT)" "y"
export SECURE=ALL
endif

export BRANCH_NAME  = $(word 2,$(BUILD_NAME))
export WARNING_LOG  = $(THIS_ROOT)/build_$(TARGET_IC).log
export ERROR_LOG    = $(THIS_ROOT)/build_fail.log

export CHK_ERR_WARN_SCRIPT = $(PROJECT_ROOT)/tools/pbuild/chk_warn_err.sh 
export CHK_ERR_WARN_PARM   = $(PROJECT_ROOT)/tools/pbuild $(BRANCH_NAME) $(WARNING_LOG) $(ERROR_LOG)
	
#
# Find the local cfg.mak, if existed, include it
#

CFG_MAK_NAME := cfg.mak

INC_CFG_MAK := $(shell if [ -e ./$(CFG_MAK_NAME) ];then echo "./$(CFG_MAK_NAME)"; else echo ""; fi)

ifneq "$(INC_CFG_MAK)" ""
include $(INC_CFG_MAK)
endif

export TARGET_DRIVER := $(subst mt,,$(TARGET_IC))_driver
export FAST_SUPPORT  := true
#LG_CHG zaewon.lee
-include $(VM_LINUX_ROOT)/project_x/target/$(TARGET_IC)/drv_opt/$(COMPANY)/$(MODEL).def


ifeq "$(BOOT_TYPE)" "ROM2NAND"
DEFINES += -DCC_NAND_LOADER -DCC_NAND_ENABLE -DCC_NAND_BOOT
BOOT = nand
endif

ifeq "$(BOOT_TYPE)" "ROM2EMMC"
DEFINES += -DCC_EMMC_BOOT
BOOT = emmc
endif

ifeq "$(BOOT_TYPE)" "ROM2NOR"
DEFINES += -DCC_NAND_LOADER -DCC_NAND_ENABLE
BOOT = nor
endif

DEFINES += -DCC_LOAD_UBOOT
export NO_SYM_DEBUG ?= TRUE

ifeq "$(MODE)" "release"
DEFINES += -DNDEBUG
else
DEFINES += -DCC_DEBUG
endif

ifeq "$(NOLOG)" "y"
DEFINES += -DNOLOG
endif

PARALLEL  ?= 16
RLS_CUSTOM_BUILD ?= false

if_file_notexist_w_sym_fct     = if [ -e $(1) -o -h $(1) ] && [ ! -e $(2) -a ! -h $(2) ]; then echo "symlink: `basename $(1)`";$(LN) $(LN_FLAG) $(1) $(2); fi

ifndef BUILD_CFG
    OBJECT_TYPE := rel
else
    OBJECT_TYPE := $(BUILD_CFG)
endif


RLS_LIB_ROOT = $(KDRIVER_ROOT)/mtk_obj/$(CUSTOMER)/$(MODEL_NAME)/$(OBJECT_TYPE)/mtkloader

# --no-print-directory -s

all: check_version do_drv_inc lib_to_obj
	@$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/x_inc, $(PROJECT_ROOT)/target/$(TARGET_IC)/x_inc);
	$(MAKE) $(if $(filter -j,$(MAKEFLAGS)),,-j $(PARALLEL)) -C $(PROJECT_ROOT)/target/$(TARGET_IC)/mtkloader TARGET=$(TARGET_IC) LINUX_SOLUTION=false UBOOT_LIBRARY=
	@$(CP) $(PROJECT_ROOT)/target/$(TARGET_IC)/mtkloader/$(EXE_NAME)_mtkloader.bin $(VM_LINUX_ROOT)/chiling/uboot/pack/$(MODEL)_$(MODE)_mtkloader_$(BOOT).bin
ifeq "$(TRUSTZONE)" "true"
ifneq "$(RLS_CUSTOM_BUILD)" "true"
	$(MAKE) -C $(PROJECT_ROOT)/target/$(TARGET_IC)/trustzone all
	@$(CP) $(PROJECT_ROOT)/target/$(TARGET_IC)/trustzone/$(TARGET_IC)_tz.bin $(VM_LINUX_ROOT)/chiling/uboot/secure/tz.bin
	$(PROJECT_ROOT)/tools/mt5391_pack/lzhsenc $(VM_LINUX_ROOT)/chiling/uboot/secure/tz.bin $(VM_LINUX_ROOT)/chiling/uboot/secure/tz.bin.lzhs
endif
else
	echo dummy > $(VM_LINUX_ROOT)/chiling/uboot/secure/tz.bin.lzhs
endif
	@if [ -d $(RLS_LIB_ROOT)/loader ]; then \
		cd $(RLS_LIB_ROOT)/loader ; \
		find .  -type d | sed s'/^\.$$//' | xargs rm -rvf  >/dev/null; \
	fi
	@if [ -d $(RLS_LIB_ROOT)/preloader ]; then \
		cd $(RLS_LIB_ROOT)/preloader ; \
		find .  -type d |  sed s'/^\.$$//' |  xargs rm -rvf >/dev/null; \
	fi

lib_to_obj_fct = if [ -e $1 ]; then \
		for i in `cat $1 | awk -F "," '{print $$2}' | sort -u `; do \
		 	module=`echo $$i | sed 's/\.a//'`; \
			mkdir -p $$module; $(AR) -x $$i; mv *.o $$module ; \
			for j in `cat $1 | grep -w $$i | awk -F "," '{print $$1}'` ; do \
 				mkdir -p `dirname $$j` ; \
				cp $$module/`basename $$j` `dirname $$j`; \
			done; \
		done; \
		fi

lib_to_obj:
	if [ -d $(RLS_LIB_ROOT)/loader ]; then \
		cd $(RLS_LIB_ROOT)/loader; \
		$(call lib_to_obj_fct, loader_map.txt) ; \
	fi
	if [ -d $(RLS_LIB_ROOT)/preloader ]; then \
		cd $(RLS_LIB_ROOT)/preloader; \
		$(call lib_to_obj_fct, preloader_map.txt) ; \
	fi

do_drv_inc :
	@$(call if_file_notexist_w_sym_fct, $(DRV_INC_ROOT), $(PROJECT_ROOT)/target/$(TARGET_IC)/$(TARGET_DRIVER)/drv_inc);

check_version :
ifneq ($(BOOT_SUBMISSION), )
DEFINES += -DBUILD_VERS="TO_STR($(BOOT_SUBMISSION))"
endif
	
clean:
ifeq "$(TRUSTZONE)" "true"
ifeq "$(L_ENV)" "true"
else
	$(MAKE) -C $(PROJECT_ROOT)/target/$(TARGET_IC)/trustzone clean
endif
endif
	@$(RM) $(RM_FLAG) -r $(PROJECT_ROOT)
	@if [ -d "`dirname $(PROJECT_ROOT)`" ]; then \
		$(RMDIR) $(RMDIR_FLAG) `dirname $(PROJECT_ROOT)`; \
	fi
