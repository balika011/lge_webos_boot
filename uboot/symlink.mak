##############################################################################
# No Warranty                                                                #
# Except as may be otherwise agreed to in writing, no warranties of any      #
# kind, whether express or implied, are given by MTK with respect to any MTK #
# Deliverables or any use thereof, and MTK Deliverables are provided on an   #
# "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        #
# including any implied warranties of merchantability, non-infringement and  #
# fitness for a particular purpose and any warranties arising out of course  #
# of performance, course of dealing or usage of trade.  Parties further      #
# acknowledge that Company may, either presently and/or in the future,       #
# instruct MTK to assist it in the development and the implementation, in    #
# accordance with Company's designs, of certain softwares relating to        #
# Company's product(s) (the "Services").  Except as may be otherwise agreed  #
# to in writing, no warranties of any kind, whether express or implied, are  #
# given by MTK with respect to the Services provided, and the Services are   #
# provided on an "AS IS" basis.  Company further acknowledges that the       #
# Services may contain errors, that testing is important and Company is      #
# solely responsible for fully testing the Services and/or derivatives       #
# thereof before they are used, sublicensed or distributed.  Should there be #
# any third party action brought against MTK, arising out of or relating to  #
# the Services, Company agree to fully indemnify and hold MTK harmless.      #
# If the parties mutually agree to enter into or continue a business         #
# relationship or other arrangement, the terms and conditions set forth      #
# hereunder shall remain effective and, unless explicitly stated otherwise,  #
# shall prevail in the event of a conflict in the terms in any agreements    #
# entered into between the parties.                                          #
##############################################################################
###########################################################################
#
#  Copyright (c) 2004, MediaTek, Inc.
#  All rights reserved
#
#  Unauthorized use, practice, perform, copy, distribution, reproduction,
#  or disclosure of this information in whole or in part is prohibited.
###########################################################################
# $RCSfile: Makefile,v $
# $Revision: #1 $
# $Date: 2015/01/09 $
# $Author: dtvbm11 $
# $CCRevision: /main/DTV_X_ATSC/6 $
# $SWAuthor: Joshuah Huang $
# $MD5HEX: e2c57029ea8dd3e986bb5c3b4fa60554 $
#
# Description:
#         For each main program to define the executable file name.
#
#         Specify the source files to be compile in SRC.
#############################################################################

ROOTDIR	:= $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

VM_LINUX_ROOT ?= $(word 1, $(subst /vm_linux/,/vm_linux /, $(ROOTDIR)))
include $(VM_LINUX_ROOT)/project_x/host.mak

if_file_notexist_w_sym_fct     = if [ -e $(1) -o -h $(1) ] && [ ! -e $(2) -a ! -h $(2) ]; then echo "symlink: `basename $(1)`";$(LN) $(LN_FLAG) $(1) $(2); fi

TARGET_IC      ?= mt5365
TARGET_DRIVER  ?= $(subst mt,,$(TARGET_IC))_driver


TARGET_ROOT    ?= $(VM_LINUX_ROOT)/chiling/driver/target/$(TARGET_IC)
MTKLOADER_ROOT ?= $(VM_LINUX_ROOT)/chiling/uboot/_build_mtkloader/project_x
UTARGET_ROOT   ?= $(MTKLOADER_ROOT)/target/$(TARGET_IC)
UTOOLS_ROOT    ?= $(MTKLOADER_ROOT)/tools
TOOLS_ROOT     ?= $(VM_LINUX_ROOT)/project_x/tools

TOOLS_LIST = $(shell cd $(TOOLS_ROOT);find pbuild -type f)

DRV_FILE_LIST = $(UTARGET_ROOT)/drv_file_list.txt

DRV_LIST = $(shell if [ -e $(DRV_FILE_LIST).need_to_do_link ]; then cat $(DRV_FILE_LIST); fi)

all: $(DRV_LIST) $(TOOLS_LIST) 
	@$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/target/d_inc  ,$(MTKLOADER_ROOT)/target/d_inc);
	@$(call if_file_notexist_w_sym_fct, $(TARGET_ROOT)/symlink.mak               ,$(UTARGET_ROOT)/symlink.mak);
       
	@if [ ! -d "$(MTKLOADER_ROOT)/middleware/scripts" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/middleware/scripts; \
	fi
	@if [ ! -d "$(MTKLOADER_ROOT)/middleware/dlm" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/middleware/dlm; \
	fi
#	@if [ ! -d "$(MTKLOADER_ROOT)/target/mt53xx_com" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/target/mt53xx_com; \
	fi
#	@if [ ! -d "$(MTKLOADER_ROOT)/custom/dev" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/custom/dev; \
	fi
	
#	@if [ ! -d "$(MTKLOADER_ROOT)/tools/archieve_lib" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/tools/archieve_lib; \
	fi

#	@if [ ! -d "$(MTKLOADER_ROOT)/tools/mt5391_pack" ]; then \
		mkdir -p $(MTKLOADER_ROOT)/tools/mt5391_pack; \
	fi

#	@for i in `cd $(VM_LINUX_ROOT)/project_x/; ls *.mak`; do \
		$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/$$i                    ,$(MTKLOADER_ROOT)/$$i); \
	done 

#	@for i in `cd $(KDRIVER_ROOT)/middleware/; ls *.mak`; do \
		$(call if_file_notexist_w_sym_fct, $(KDRIVER_ROOT)/middleware/$$i         ,$(MTKLOADER_ROOT)/middleware/$$i); \
	done
#	@for i in `cd $(KDRIVER_ROOT)/middleware/scripts/; ls`; do \
		$(call if_file_notexist_w_sym_fct, $(KDRIVER_ROOT)/middleware/scripts/$$i ,$(MTKLOADER_ROOT)/middleware/scripts/$$i); \
	done

#	@for i in `cd $(KDRIVER_ROOT)/middleware/dlm/; ls`; do \
		$(call if_file_notexist_w_sym_fct, $(KDRIVER_ROOT)/middleware/dlm/$$i      ,$(MTKLOADER_ROOT)/middleware/dlm/$$i); \
	done

#	@for i in `cd $(VM_LINUX_ROOT)/project_x/custom/dev/; ls *.mak`; do \
		$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/custom/dev/$$i          ,$(MTKLOADER_ROOT)/custom/dev/$$i); \
	done
	
#	@for i in `cd $(VM_LINUX_ROOT)/project_x/target/mt53xx_com/; ls *.mak`; do \
		$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/target/mt53xx_com/$$i   ,$(MTKLOADER_ROOT)/target/mt53xx_com/$$i); \
	done

#	@for i in `cd $(VM_LINUX_ROOT)/project_x/tools/archieve_lib/; ls`; do \
		$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/tools/archieve_lib/$$i  ,$(MTKLOADER_ROOT)/tools/archieve_lib/$$i); \
	done

#	@for i in `cd $(VM_LINUX_ROOT)/project_x/tools/mt5391_pack/; ls`; do \
		$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/tools/mt5391_pack/$$i  ,$(MTKLOADER_ROOT)/tools/mt5391_pack/$$i); \
	done
	

	@$(call if_file_notexist_w_sym_fct, $(VM_LINUX_ROOT)/project_x/mtk_obj ,$(MTKLOADER_ROOT)/mtk_obj)

	@if [ -e $(DRV_FILE_LIST).tmp ]; then \
		if [ "`diff -q $(DRV_FILE_LIST).tmp $(DRV_FILE_LIST) 2>/dev/null`" != '' ]; then \
			cp -au $(DRV_FILE_LIST).tmp $(DRV_FILE_LIST); \
		fi; \
		rm -rf $(DRV_FILE_LIST).tmp; \
	fi

	

$(DRV_LIST):
	@if [ ! -d "$(UTARGET_ROOT)/$(dir $@)" ]; then \
		mkdir -p $(UTARGET_ROOT)/$(dir $@); \
	fi
	@if [ ! -e $(UTARGET_ROOT)/$@ -a ! -h $(UTARGET_ROOT)/$@ ]; then \
		if [ "$(suffix $@)" == ".o" ]; then \
			if [ ! -e "$(TARGET_ROOT)/$(@:.o=.c)" ]; then \
				$(call if_file_notexist_w_sym_fct, $(TARGET_ROOT)/$@  ,$(UTARGET_ROOT)/$@); \
			fi; \
		else \
			$(call if_file_notexist_w_sym_fct, $(TARGET_ROOT)/$@  ,$(UTARGET_ROOT)/$@); \
		fi; \
	fi


$(TOOLS_LIST):
	@if [ ! -d "$(UTOOLS_ROOT)/$(dir $@)" ]; then \
		mkdir -p $(UTOOLS_ROOT)/$(dir $@); \
	fi
	@if [ ! -e $(UTOOLS_ROOT)/$@ -a ! -h $(UTOOLS_ROOT)/$@ ]; then \
		$(call if_file_notexist_w_sym_fct, $(TOOLS_ROOT)/$@  ,$(UTOOLS_ROOT)/$@); \
	fi

