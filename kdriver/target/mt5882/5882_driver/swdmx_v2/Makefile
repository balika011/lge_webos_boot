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
# $Revision: #1 $
# $Date: 2009/08/11 $
# $Author: dtvbm11 $
#
# Description:
#         Leave-level makefile to build the subcomponent of driver library.
#
#         Specify the source files to be compile in SRC.
#############################################################################

REF_TARGET = 53xx_com

THIS_COMPONENT = swdmx

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
export DRV_ROOT = ../..
endif

# Options for disable container
#DEFINES += -DCC_PS_DISABLE
#DEFINES += -DCC_PS_MPEG1_DISABLE
#DEFINES += -DCC_PS_MPEG2_DISABLE
#DEFINES += -DCC_TS_DISABLE
#DEFINES += -DCC_TS_MPEG1_DISABLE
#DEFINES += -DCC_TS_MPEG2_DISABLE
#DEFINES += -DCC_TS_VC1_DISABLE
#DEFINES += -DCC_TS_H264_DISABLE
#DEFINES += -DCC_ES_DISABLE
#DEFINES += -DCC_ES_MPEG1_DISABLE
#DEFINES += -DCC_ES_MPEG2_DISABLE
#DEFINES += -DCC_ES_MPEG4_DISABLE
#DEFINES += -DCC_ES_VC1_DISABLE
#DEFINES += -DCC_ES_H264_DISABLE
#DEFINES += -DCC_ES_MP2_DISABLE
#DEFINES += -DCC_ES_MP3_DISABLE
#DEFINES += -DCC_ES_LPCM_DISABLE
#DEFINES += -DCC_ES_WMA_DISABLE
#DEFINES += -DCC_AVI_DISABLE
#DEFINES += -DCC_AVI_MPEG1_DISABLE
#DEFINES += -DCC_AVI_MPEG2_DISABLE
#DEFINES += -DCC_AVI_MPEG4_DISABLE
#DEFINES += -DCC_AVI_DX311_DISABLE
#DEFINES += -DCC_AVI_H264_DISABLE
#DEFINES += -DCC_AVI_VC1_DISABLE
#DEFINES += -DCC_AVI_MJPEG_DISABLE
#DEFINES += -DCC_MP4_DISABLE
#DEFINES += -DCC_MP4_MPEG1_DISABLE
#DEFINES += -DCC_MP4_MPEG2_DISABLE
#DEFINES += -DCC_MP4_MPEG4_DISABLE
#DEFINES += -DCC_MP4_H264_DISABLE
#DEFINES += -DCC_MP4_VC1_DISABLE
#DEFINES += -DCC_MP4_MJPEG_DISABLE
#DEFINES += -DCC_WMV_DISABLE
#DEFINES += -DCC_WMV_MPEG1_DISABLE
#DEFINES += -DCC_WMV_MPEG2_DISABLE
#DEFINES += -DCC_WMV_MPEG4_DISABLE
#DEFINES += -DCC_WMV_H264_DISABLE
#DEFINES += -DCC_WMV_VC1_DISABLE
#DEFINES += -DCC_WMV_MJPEG_DISABLE
#DEFINES += -DCC_MKV_DISABLE
#DEFINES += -DCC_MKV_MPEG1_DISABLE
#DEFINES += -DCC_MKV_MPEG2_DISABLE
#DEFINES += -DCC_MKV_MPEG4_DISABLE
#DEFINES += -DCC_MKV_H264_DISABLE
#DEFINES += -DCC_MKV_VC1_DISABLE
#DEFINES += -DCC_MKV_MJPEG_DISABLE
#DEFINES += -DCC_MKV_VP8_DISABLE
#DEFINES += -DCC_RM_DISABLE
#DEFINES += -DCC_FLV_DISABLE
#DEFINES += -DCC_FLV_H264_DISABLE
#DEFINES += -DCC_FLV_SORENSON_DISABLE
#DEFINES += -DCC_FLV_MJPG_DISABLE
#DEFINES += -DCC_CMPB_DISABLE

ifeq "$(BUILD_CFG)" "debug"
EN_LOG := true
else
ifeq "$(BUILD_CFG)" "cli"
EN_LOG := true
else
EN_LOG := false
endif
endif

ifeq "$(EN_SWDMX_LOG)" "true"
EN_LOG := true
endif

ifeq "$(EN_LOG)" "true"
DEFINES += -DCC_SWDMX_INFO_LOG
endif

SRC = swdmx_if.c        \
      swdmx_drvif.c

ifndef CC_PS_DISABLE
SRC += swdmx_mpg.c
SRC += swdmx_mpg_idx.c
SRC += swdmx_ps.c
endif
ifndef CC_ES_DISABLE
SRC += swdmx_wmv.c
SRC += swdmx_mpg.c
SRC += swdmx_mpg_idx.c
endif
ifndef CC_TS_DISABLE
SRC += swdmx_mpg.c
SRC += swdmx_mpg_idx.c
SRC += swdmx_mpg_dmx.c
SRC += swdmx_mpg_psipsr.c
SRC += swdmx_mpg_pvr.c
endif
ifndef CC_AVI_DISABLE
SRC += swdmx_avi.c
endif
ifndef CC_WMV_DISABLE
SRC += swdmx_wmv.c
endif
ifndef CC_MP4_DISABLE
SRC += swdmx_mp4.c
endif
ifndef CC_MKV_DISABLE
SRC += swdmx_mkv.c
endif
ifndef CC_RM_DISABLE
SRC += swdmx_rm.c
endif
ifndef CC_FLV_DISABLE
SRC += swdmx_flv.c
endif
ifndef CC_CMPB_DISABLE
SRC += swdmx_cmpb.c
endif
#ifndef CC_FLASH_DISABLE
#SRC += swdmx_flash.c
#endif

ifndef CC_OGG_DISABLE
SRC += swdmx_ogg.c
endif

ifndef CC_APE_DISABLE
SRC += swdmx_ape.c
endif

ADD_TO_SYMLINK_FILES =swdmx_ogg.h swdmx_flv.h swdmx_avi.h swdmx_debug.h swdmx_drvif.h swdmx_if.h swdmx_mkv.h swdmx_mp4.h swdmx_mpg.h swdmx_mpg_dmx.h swdmx_mpg_idx.h swdmx_mpg_psipsr.h swdmx_mpg_psipsr_if.h swdmx_mpg_pvr.h swdmx_pvr_if.h swdmx_wmv.h swdmx_rm.h swdmx_cmpb.h swdmx_ps.h swdmx_ape.h

OBJ =

SUB_COMPONENTS =

OPTIONAL_SUB_COMPONENTS =

DEFINES += -DSYNC_PES_HEADER


CC_INC +=

# Options enable dump data
ifeq "$(SWDMX_DUMP)" "true"
DEFINES += -DSWDMX_DUMP_DATA
ADD_TO_SYMLINK_FILES += swdmx_debug_util.h
SRC += swdmx_debug_util.c
endif
#HDCP2
ifeq "$(ENABLE_HDCP2)" "true"
    ifeq "$(TRUSTZONE)" "true"
        DEFINES += -DCC_ENABLE_HDCP2
    endif
endif
#############################################################################
#
# Include the makefile common to all components
#

include $(DRV_ROOT)/driver.mak
