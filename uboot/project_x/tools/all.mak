#
# Copyright (c) MediaTek Inc.
#
# This program is distributed under a dual license of GPL v2.0 and
# MediaTek proprietary license. You may at your option receive a license
# to this program under either the terms of the GNU General Public
# License (GPL) or MediaTek proprietary license, explained in the note
# below.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
#
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
# $RCSfile: all.mak,v $
# $Revision: #2 $
# $Date: 2015/04/01 $
# $Author: p4admin $
# $CCRevision: /main/DTV_X/DTV_X_HQ_int/2 $
# $SWAuthor: Iolo Tsai $
# $MD5HEX: e28ed720dfe9ee1aaf20a243fb0e434d $
#
# Description:
#         Rules to create libraries, object files and dependency files.
#
#############################################################################

include $(TARGET_ROOT)/target.mak

#
# Set the actual compiler include path and define
#
ABS_INCLUDE = -I$(MW_ROOT) $(TARGET_INC) $(sort $(CC_INC))
ABS_DEFINES = $(sort $(DEFINES))

ABS_LIBS      := $(TARGET_LIBS)
ABS_LIBS_PATH := $(TARGET_LIBS_PATH)
ABS_EXE       := $(TOOL_BIN_DIR)/$(TOOL_NAME)$(TARGET_EXE_SUFFIX)


#
# Specify the various goals
#
.PHONY : all


all : $(LEX_C_FILE) $(OBJ)
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(TOOL_BIN_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(TOOL_BIN_DIR);)
	@echo ""
	@echo "---------------- Building executable $(TOOL_NAME)$(TARGET_EXE_SUFFIX) ----------------"
	$(LD) $(LD_FLAG) $(LD_OUTPUT) $(call path_cnvt,$(ABS_EXE)) $(call path_cnvt,$(ABS_OBJ)) $(call lib_path_cnvt,$(ABS_LIBS_PATH)) $(ABS_LIBS)
	$(CP) $(CP_FLAG) $(call path_cnvt,$(ABS_EXE)) .


#
# Set the search path's
#
vpath
vpath %.o $(TOOL_OBJ_DIR)
vpath %.d $(TOOL_DEPEND_DIR)


#
# And here the rules begin.
#
$(LEX_C_FILE) : %.c : %.l
	@echo ""
	@echo "---------------- Building `basename $@` ----------------"
	$(LEX) $(LEX_FLAG) -o$@ $<


$(TOOL_DEPEND_DIR)/%.d : %.c
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(TOOL_DEPEND_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(TOOL_DEPEND_DIR));)
	@echo ""
	@echo "---------------- Building `basename $@` ----------------"
	$(CC) $(CC_DEP_GEN_FLAG) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $(call path_cnvt,$<) $(call dep_out_fct,$(@F)) > $(call path_cnvt,$@.tmp)
	$(call mod_dep_file_fct,$@.tmp,$@)
	@$(RM) $(RM_FLAG) $(call path_cnvt,$@.tmp) $(NULL_DEV)


%.o : %.c
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(TOOL_OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(TOOL_OBJ_DIR));)
	@echo ""
	@echo "---------------- Building `basename $@` ----------------"
	$(CC) -c $(CC_FLAG) $(call inc_path_cnvt,$(ABS_INCLUDE)) $(ABS_DEFINES) $(call path_cnvt,$<) -o $(call path_cnvt,$(TOOL_OBJ_DIR)/$@)


_obj_/%.o : %.o
	$(call if_dir_not_exist_fct,$(call path_cnvt,$(TOOL_OBJ_DIR)), \
	    $(MKDIR) $(MKDIR_FLAG) $(call path_cnvt,$(TOOL_OBJ_DIR));)
	@echo ""
	@echo "---------------- Copying `basename $@` ----------------"
	$(CP) $(CP_FLAG) $(call path_cnvt,$<) $(call path_cnvt,$(TOOL_OBJ_DIR))


ifneq ($(strip $(ABS_DEPEND)),)
-include $(ABS_DEPEND)
endif

