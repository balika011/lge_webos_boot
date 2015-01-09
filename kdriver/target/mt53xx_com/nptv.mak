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
# $RCSfile: nptv.mak,v $
# $Revision: #1 $
# $Date: 2015/01/09 $
# $Author: dtvbm11 $
#
# Description:
#         common makefile to all nptv components.
#
#############################################################################

ifndef MODEL
MODEL = common
endif
ifndef MODE
MODE = debug
endif

#NPTV_ROOT = $(DRV_ROOT)/5391_driver/nptv
# don't know where to use the variable

ifeq "$(TARGET)" "mt5387"
NPTV_ROOT = $(DRV_ROOT)/5387_driver/nptv
endif

ifeq "$(TARGET)" "mt5363"
NPTV_ROOT = $(DRV_ROOT)/5363_driver/nptv
endif

ifeq "$(TARGET)" "mt5365"
NPTV_ROOT = $(DRV_ROOT)/5365_driver/nptv6595
endif

ifeq "$(TARGET)" "mt5395"
NPTV_ROOT = $(DRV_ROOT)/5395_driver/nptv6595
endif

ifeq "$(TARGET)" "mt5389"
NPTV_ROOT = $(DRV_ROOT)/5389_driver/nptv6896
endif

ifeq "$(TARGET)" "mt5368"
NPTV_ROOT = $(DRV_ROOT)/5368_driver/nptv6896
endif

ifeq "$(TARGET)" "mt5396"
NPTV_ROOT = $(DRV_ROOT)/5396_driver/nptv6896
endif

ifeq "$(TARGET)" "mt5398"
NPTV_ROOT = $(DRV_ROOT)/5398_driver/nptv8098
endif

ifeq "$(TARGET)" "mt5399"
NPTV_ROOT = $(DRV_ROOT)/5399_driver/nptv5399
endif

ifeq "$(TARGET)" "mt5890"
NPTV_ROOT = $(DRV_ROOT)/5890_driver/nptv8290
endif

ifeq "$(TARGET)" "mt5891"
NPTV_ROOT = $(DRV_ROOT)/5891_driver/nptv8290
endif

ifeq "$(TARGET)" "mt5861"
NPTV_ROOT = $(DRV_ROOT)/5861_driver/nptv8290
endif

ifeq "$(TARGET)" "mt5882"
NPTV_ROOT = $(DRV_ROOT)/5882_driver/nptv5399
endif

ifeq "$(TARGET)" "mt5883"
NPTV_ROOT = $(DRV_ROOT)/5883_driver/nptv5399
endif

# for driver build only
ifeq "$(TARGET)" "5398_driver"
NPTV_ROOT = $(DRV_ROOT)/5398_driver/nptv8098
endif

ifeq "$(TARGET)" "mt5880"
NPTV_ROOT = $(DRV_ROOT)/5880_driver/nptv8098
endif

ifeq "$(TARGET)" "mt5860"
NPTV_ROOT = $(DRV_ROOT)/5860_driver/nptv8098
endif

ifeq "$(TARGET)" "mt5881"
NPTV_ROOT = $(DRV_ROOT)/5881_driver/nptv5881
endif

ifeq "$(TARGET)" "5399_driver"
NPTV_ROOT = $(DRV_ROOT)/5399_driver/nptv5399
endif

ifeq "$(TARGET)" "5890_driver"
NPTV_ROOT = $(DRV_ROOT)/5890_driver/nptv8290
endif

ifeq "$(TARGET)" "5891_driver"
NPTV_ROOT = $(DRV_ROOT)/5891_driver/nptv8290
endif

ifeq "$(TARGET)" "5861_driver"
NPTV_ROOT = $(DRV_ROOT)/5861_driver/nptv8290
endif

ifeq "$(TARGET)" "5882_driver"
NPTV_ROOT = $(DRV_ROOT)/5882_driver/nptv5399
endif

ifeq "$(TARGET)" "5883_driver"
NPTV_ROOT = $(DRV_ROOT)/5883_driver/nptv5399
endif

# include $(DRV_ROOT)/target/$(MODEL).def


CC_INC += -I$(NPTV_ROOT)/inc \
	-I$(NPTV_ROOT)/inc/hw \
	-I$(NPTV_ROOT)/inc/drv \
	-I$(NPTV_ROOT)/inc/sys \


# ASMDEFINES = $(ASM_DEFINES)

# ifeq "$(MODE)" "release"
# DEFINES += -DNDEBUG

# 	ifndef OPT_FLAG
# 	OPT_FLAG = -O2
# 	endif
# endif

# export OPT_FLAG

# include $(DRV_ROOT)/common.mak
