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
# $Revision: #6 $
# $Date: 2009/09/03 $
# $Author: luis.chen $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

THIS_COMPONENT = 5882_preloader
export TARGET = $(THIS_COMPONENT)

ifndef DRV_ROOT
export DRV_ROOT = ..
endif

export 5882_PRELOADER = y

SRC =

OBJ =

SUB_COMPONENTS = drv_inc bsp drv_cust chip dmx8290

# driver diag need optional directory list
OPT_COMPONENTS +=

# middleware program need optional directory list
OPTIONAL_SUB_COMPONENTS =

CC_INC += 
DEFINES += -DCC_MTK_PRELOADER -DCC_MTK_LOADER

ifeq "$(BUILD_LINUX_LOADER)" "true"
ASM_DEFINES += -DCC_MTK_PRELOADER -DCC_MTK_LOADER -D__GCC__
else
ASM_DEFINES += --pd "CC_MTK_PRELOADER SETL {TRUE}" --pd "CC_MTK_LOADER SETL {TRUE}"
endif

ifeq "$(SUSPEND_TO_DRAM)" "true"
ifeq "$(TRUSTZONE)" "true"
ifeq "$(BUILD_LINUX_LOADER)" "true"
# for gcc_jump_resume.S
ASM_DEFINES += -DCC_TRUSTZONE_SUPPORT
endif
endif
endif

LINK_LIST = inc private_inc $(SUB_COMPONENTS)

#############################################################################
#
# Export preloader library specific variable
#

LIBRARY_PATH := $(subst 5882_driver,5882,$(word 2,$(subst target/mt5882/, , $(subst drv_build/mt5882/driver/, , $(shell pwd)))))

#############################################################################
#
# Include the makefile common to all components
#
export APPEND_OBJ_TYPE := preloader

PREDRV = predriver

include $(DRV_ROOT)/driver.mak

predriver:
	#rm -rf $(LINK_LIST)
	@for i in $(LINK_LIST); do \
		if [ ! -e $$i -a ! -h $$i ]; \
		then \
			($(call if_file_notexist_w_sym_fct, ../5882_driver/$$i, $(DRV_ROOT)/5882_preloader/$$i); exit 0) ; \
		fi; \
	done

