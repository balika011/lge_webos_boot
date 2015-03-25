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
# $RCSfile: customer.mak,v $
# $Revision: #3 $
# $Date: 2015/03/25 $
# $Author: p4admin $
# $MD5HEX: 61118bc277d8ac078b83c14a0d509c1a $
#
# Description:
#         Makefile to build a "MT-5351" demo board build. The following
#         targets are supported:
#
#             all:           Compiles middleware, custom, target and this
#                            directory and creates a final image.
#############################################################################



ifndef IMAGE_ROOT_PATH
IMAGE_ROOT_PATH := $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell bash -c pwd -P)))/project_x/tools/image-tools
endif
ifndef LGE_A5LR_TOOL_PATH
LGE_A5LR_TOOL_PATH := $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell bash -c pwd -P)))/project_x/tools/a5lr_tools
endif
ifeq "$(DEBUG_MAKE)" "true"
include $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell bash -c pwd -P)))/dtv_linux/mak/gmsl-1.0.12/gmsl
include $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell bash -c pwd -P)))/dtv_linux/mak/gmd-1.0.2/gmd
endif
#
#Define showstring instead of warning
#
ifeq (1,$(V))
define showstring
$(warning $(1))
endef
else
define showstring
endef
endif

#
# Set the Use Script
#

RM      = /bin/rm
RM_FLAG = -rf

RMDIR   = /bin/rmdir
RMDIR_FLAG = --ignore-fail-on-non-empty -p

LN      = /bin/ln
LN_FLAG = -sf

CP      = /bin/cp
CP_FLAG = -rf
RLS_CP_FLAG = -au


MV      = /bin/mv
MV_FLAG =


export FCLEAN = $(word 1, $(subst /vm_linux/,/vm_linux /, $(shell pwd)))/project_x/tools/fast_clean/fclean.sh
export SHELL = /bin/bash

#
# Make command and options
#
MAKE      = make SHELL=$(SHELL)
MAKE_FLAG =
export JOBS  ?= $(shell cat /proc/cpuinfo |grep processor|wc -l)
FMAKE     = $(MAKE) -j $(JOBS)


SUPPORT_MAKE_VERSION := 3.81
MAKE_VERSION := $(shell make -v|grep 'GNU Make'|awk '{print $$3}')

# check for broken versions of make
ifeq (0,$(shell expr $(MAKE_VERSION) \>= 3.81))
$(warning ********************************************************************************)
$(warning *  You are using version $(MAKE_VERSION) of make.)
$(warning *  You must upgrade to version $(SUPPORT_MAKE_VERSION) or greater.)
$(warning ********************************************************************************)
$(error stopping)
endif


#
# Create directory command and options
#
MKDIR      = mkdir
MKDIR_FLAG = -p

#
# tar command and options
#
TAR        = tar
TAR_FLAG   = zcf
UNTAR_FLAG = zxf
UNTAR_BZ_FLAG = jxf


#
# gzip command and options
#
GZIP        = gzip


#
# find command and options
#
FIND        = /usr/bin/find

REMOVE_SH := $(CHILING_ROOT)/script/base/_remove_sub.sh
RELEASE_DRIVER_SCRIPT     := $(CHILING_ROOT)/script/base/_release_driver.sh
RELEASE_DRIVER_OBJ_SCRIPT := $(CHILING_ROOT)/script/base/_release_driver_obj.sh

#
# Date command and options
#
DATE = date
DATE_FLAG = +%Y%m%d_%H%M%S
SDATE_FLAG = +%s

#
# file/directory change mode command
#
CHMOD      = /bin/chmod

LOCKFILE = lockfile -1 -l 30

#
# The shell define
#
export SHELL=/bin/bash

#
# mtk tool chain
#
export GENEXT2FS       = $(IMAGE_ROOT_PATH)/genext2fs
export MKCRAMFS        = $(IMAGE_ROOT_PATH)/mkcramfs
export MKIMAGE         = $(IMAGE_ROOT_PATH)/mkimage
export MKSQUASHFS      = $(IMAGE_ROOT_PATH)/mksquashfs$(MKFS_VERSION)
export PACKLINUX       = $(IMAGE_ROOT_PATH)/packlinux
ifeq "$(USE_MKNFSBIMG_V3)" "true"
export MKNFSBIMG       = $(IMAGE_ROOT_PATH)/mknfsbimg3
export NFSBVERITY      = $(IMAGE_ROOT_PATH)/veritysetup
else
export MKNFSBIMG       = $(IMAGE_ROOT_PATH)/mknfsbimg
export NFSBVERITY      = $(IMAGE_ROOT_PATH)/verity
endif

#add for generating kernel.pak
ifneq "$(L_ENV)" "true"
export MK_EPAK      = 	$(LGE_A5LR_TOOL_PATH)/mkepk
export SIGN_DIR			=		$(LGE_A5LR_TOOL_PATH)/sign_dir
export SIGN_TOOL_DIR =	$(SIGN_DIR)/tool
export SIGN_TOOL		= 	$(SIGN_TOOL_DIR)/sign_tool
export SIGGEN_KEY1	= 	$(SIGN_TOOL_DIR)/customer_pub_mtka5lr_key  
export SIGGEN_KEY2	= 	$(SIGN_TOOL_DIR)/customer_priv_mtka5lr_key
endif

export COMPILE_DATE		= $(shell date +%D)
export MTK_MKIMAGE := $(MKIMAGE)
export LG_KERNEL_MKIMAGE := $(IMAGE_ROOT_PATH)/lgmkimage
  
export GEN_CFG := $(TOOL_ROOT)/gen_cfg/gen_cfg_linux
#
# The following functions are used to check on files and execute
# a set of function.
#

ifndef SYMLINK_LOG
export SYMLINK_LOG := /dev/null
endif

if_file_notexist_w_sym_fct   = if [ -e $(1) -o -h $(1) ]; then if [ ! -e $(2) -a ! -h $(2) ]; then $(LN) $(LN_FLAG) $(1) $(2); if [ "$(SYMLINK_LOG)" != "" ]; then echo  $(2) >> $(SYMLINK_LOG); fi ; fi ; fi

#
# The following functions are used to check on files and execute
# a set of function.
#
if_file_exist_fct     = @if [ -e $(1) ]; then $(2) fi
if_file_not_exist_fct = @if [ ! -e $(1) ]; then $(2) fi


#
# The following functions are used to check on directories and execute
# a set of function.
#
if_dir_exist_fct     = @if [ -d $(1) ]; then $(2) fi
if_dir_not_exist_fct = @if [ ! -d $(1) ]; then $(2) fi


#
# The following functions are used to calculate the execute time of
# a set of function.
#
cal_exec_time_fct     = echo BUILD_$(1)_END_TIME=`$(DATE) $(DATE_FLAG)`;\
                        if [ -e .build_start_time_$(1) ]; then expr `$(DATE) $(SDATE_FLAG)` - `cat .build_start_time_$(1)` > dtv_linux_build_time_$(1); \
                                              echo BUILD_$(1)_EXEC_TIME:`cat  dtv_linux_build_time_$(1)`; \
                                              rm -rf dtv_linux_build_time_$(1) .build_start_time_$(1); fi
                        
                                              
set_start_time_fct    = $(DATE) $(SDATE_FLAG) > .build_start_time_$(1); \
                        echo BUILD_$(1)_START_TIME=`$(DATE) $(DATE_FLAG)`

export CHK_ERR_WARN_SCRIPT_ROOT  = $(TOOL_ROOT)/pbuild
export CHK_ERR_WARN_SCRIPT = $(CHK_ERR_WARN_SCRIPT_ROOT)/chk_warn_err.sh 
export ERROR_LOG    = $(THIS_ROOT)/build_fail.log
export BRANCH_NAME  = $(word 2,$(BUILD_NAME))
export FIND_AUTHOR ?= false
export CHECK_WARNING_BUILD ?= false

ifeq "$(RLS_CUSTOM_BUILD)" "true"
export FIND_AUTHOR = false
endif
ifeq "$(CHECK_WARNING_BUILD)" "true"
export FIND_AUTHOR ?= true
endif

export CHECK_WARNING ?= false

chk_err_fct =   sed -e '/\( [Ee]rror[ :]\|first defined here\|multiple definition of\|Stop\.\|No rule to make target\)/!d' $(1) > $(1).tmp.txt; \
                if [ -s $(1).tmp.txt ]; then    \
                    grep -v 'error = clazz' $(1).tmp.txt > $(1).tmp2.txt; \
                    if [ -s $(1).tmp2.txt ]; then \
                      mv $(1).tmp2.txt $(1).tmp.txt; \
                    else \
                      rm -rf $(1).tmp.txt; \
                    fi;\
                fi; \
				if [ -s $(1).tmp.txt ]; then	\
					/bin/echo -e '\n\n=========================== BUILD_ERROR_MESSAGES ==========================='; \
					cat $(1).tmp.txt;	\
					rm -f $(1) $(1).tmp.txt;	\
					exit 1;	\
				fi; \
				rm -f $(1) $(1).tmp.txt
          
chk_err_warn_fct     = if [ -e $(1) ]; then /bin/bash $(CHK_ERR_WARN_SCRIPT) $(CHK_ERR_WARN_SCRIPT_ROOT) $(BRANCH_NAME) $(1) $(ERROR_LOG) $(FIND_AUTHOR) $(CHECK_WARNING) $(2);  if [ -e $(ERROR_LOG) ]; then  exit 1; else $(call chk_err_fct, $(1));	 fi ; fi
                                              

cust_link = \
	if [ -d $(1) -a -d $(2) ];  then \
		for i in `cd $(1); find . -type f | sed  's/\.\///'` ; do  \
			if [ -f $(2)/$$i -a ! -L $(2)/$$i ] ; then  \
				ORIG_SRC_FILE=`echo $(2)/$$i | sed 's,$(VM_LINUX_ROOT),$(CUST_ORIG_SRC_ROOT),'` ; \
				$(MKDIR) $(MKDIR_FLAG) `dirname $$ORIG_SRC_FILE` ; \
				$(MV) $(MV_FLAG) $(2)/$$i $$ORIG_SRC_FILE ; \
				$(LN) $(LN_FLAG) $(1)/$$i  $(2)/$$i ; \
				echo $(2)/$$i >> $(CUST_LINK_LOG) ; \
			elif [ ! -e $(2)/$$i	] ; then  \
				$(MKDIR) $(MKDIR_FLAG) `dirname $(2)/$$i ` ; \
				$(LN) $(LN_FLAG) $(1)/$$i  $(2)/$$i ; \
				echo $(2)/$$i >> $(CUST_LINK_LOG) ; \
			fi ; \
		done ; \
	fi

clean_cust_link = \
	if [ -e $(CUST_LINK_LOG) ]; then \
		for i in `cat $(CUST_LINK_LOG) | grep $(1)` ; do \
			$(RM) $(RM_FLAG) $$i; \
			ORIG_SRC_FILE=`echo $$i | sed 's,$(VM_LINUX_ROOT),$(CUST_ORIG_SRC_ROOT),'` ; \
			if [ -e $$ORIG_SRC_FILE ] ; then  \
				$(MV) $(MV_FLAG) $$ORIG_SRC_FILE $$i ; \
			fi ; \
		done; \
	fi
