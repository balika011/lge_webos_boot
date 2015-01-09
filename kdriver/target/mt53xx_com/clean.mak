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
# $RCSfile: clean.mak,v $
# $Revision: #1 $
# $Date: 2015/01/09 $
# $Author: dtvbm11 $
# $CCRevision: /main/DTV_X/DTV_X_HQ_int/10 $
# $SWAuthor: Clear Case Administrator $
# $MD5HEX: 87dde420b8da95b7168c7a5048609de5 $
#
# Description:
#         Clean rules.
#
#############################################################################


include $(PROJECT_ROOT)/target/mt53xx_com/files.mak


#
# Adjust SUB_COMPONENTS and OPT_SUB_COMPONENTS if required
#
ifeq ($(strip $(NO_SUB_COMP)),TRUE)
SUB_COMPONENTS =
endif

ifeq ($(strip $(NO_OPT_SUB_COMP)),TRUE)
OPT_SUB_COMPONENTS =
endif


#
# Specify the various goals
#
.PHONY : clean clean_target clean_a clean_o clean_d clean_lib clean_lint clean_cfg_mak clean_force $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)


clean_force : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	@$(RM) $(RM_FLAG) $(CFG_MAK_NAME) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(OBJ_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(TARGET_LIB_DIR)) $(NULL_DEV)
	@$(RM) $(RM_REC_FLAG) $(call path_cnvt,$(LIB_LIB_DIR)) $(NULL_DEV)


clean clean_target : clean_a clean_o clean_d clean_lint $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS) clean_cfg_mak
	$(call if_dir_exist_fct,$(call path_cnvt,$(TARGET_LIB_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(TARGET_LIB_DIR)/$(THIS_LIB)) $(NULL_DEV); )


clean_a :
	$(call if_dir_exist_fct,$(call path_cnvt,$(LIB_LIB_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(ABS_LIB)) $(NULL_DEV); )


clean_o : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	$(call if_dir_exist_fct,$(call path_cnvt,$(OBJ_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(REL_ABS_OBJ)) $(NULL_DEV); )


clean_d : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	$(call if_dir_exist_fct,$(call path_cnvt,$(DEPEND_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(ABS_DEPEND)) $(NULL_DEV); )


clean_lib : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	$(call if_dir_exist_fct,$(call path_cnvt,$(TARGET_LIB_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(TARGET_LIB_DIR)/$(THIS_LIB)) $(NULL_DEV); )


clean_lint : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	$(call if_dir_exist_fct,$(call path_cnvt,$(LINT_DIR)), \
	    $(RM) $(RM_FLAG) $(call path_cnvt,$(ABS_LINT_OBJ)) $(NULL_DEV); )


clean_cfg_mak : $(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS)
	@$(RM) $(RM_FLAG) $(CFG_MAK_NAME) $(NULL_DEV)

#
# Cycle through all sub- and optional sub components.
#
$(SUB_COMPONENTS) $(OPTIONAL_SUB_COMPONENTS) :
	$(call if_dir_exist_fct,$(call path_cnvt,$(call adjust_target_comp_path,$@)), \
		$(MAKE) $(MAKE_FLAG) -C $(call adjust_target_comp_path,$@) $(MAKECMDGOALS);)
