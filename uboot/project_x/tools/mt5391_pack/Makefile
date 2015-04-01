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
# *-----------------------------------------------------------------------------
# * $RCSfile: Makefile,v $
# * $Revision: #16 $
# * $Date: 2013/01/28 $
# * $Author: tenny.liu $
# * $CCRevision: /main/DTV_X_HQ_int/1 $
# * $SWAuthor: Iolo Tsai $
# * $MD5HEX: 1deae8033587314bd02ece651472302b $
# *
# * Description:
# *         makefile to build source codes.
# *---------------------------------------------------------------------------*/

ifndef PROJECT_ROOT
PROJECT_ROOT := $(word 1, $(subst /project_x/,/project_x /, $(shell pwd)))
endif

include $(PROJECT_ROOT)/path.mak


#
# Tool name
#
TOOL_NAME = mt5391_pack_$(PROJECT_X_HOST)


#
# Specify all the sources
#
SRC =

OBJ =

PACKPROG = packprog

LZHSENC = lzhsenc

DEFINES += -g

ifdef INC_PATH
CC_INC += -I../../$(INC_PATH) -I../../middleware/dlm
else
CC_INC += -I../../target/mt5381/loader -I../../target/mt5382p/loader -I../../middleware/dlm -I../../target/mt5391/loader -I../../target/mt5392b/loader
endif

CFLAGS += -DNO_MIDDLEWARE -DNO_TYPE_DEF -m32

RLS_CUSTOM_BUILD ?= false
ifeq "$(RLS_CUSTOM_BUILD)" "true"
RLS_CUSTOM_BUILD := $(shell if [ -e mt5391_pack.c ]; then echo 'false'; else echo 'true'; fi)
endif


.PHONY: $(PACKPROG)

ifeq "$(RLS_CUSTOM_BUILD)" "false"
all: $(PACKPROG) $(LZHSENC) change_mode
else
all: change_mode
endif
	chmod +x mt5391_pack_$(PROJECT_X_HOST)
	chmod +x lzhsenc
	chmod +x codefile

$(PACKPROG): mt5391_pack.c packimg.c
	echo "build packprog"
	$(CC) -m32 -g $(CC_INC) $(CFLAGS)  -o $(TOOL_NAME) mt5391_pack.c
	$(CC) -m32 -g $(CC_INC) $(CFLAGS)  -o flip_pack_linux flip_pack.c
	$(CC) -m32 -g $(CC_INC) $(CFLAGS)  -o packimg packimg.c

ifeq "$(RLS_CUSTOM_BUILD)" "false"
change_mode: $(PACKPROG)
else
change_mode:
endif
	chmod +x $(TOOL_NAME)
	chmod +x packimg
		
$(LZHSENC): lzhsenc.c
	echo "build lzhsenc"
	$(CC) -m32 -g -static -o lzhsenc lzhsenc.c

ifeq "$(RLS_CUSTOM_BUILD)" "false"
clean:
	$(RM) $(RM_FLAG) $(TOOL_NAME)
else
clean:
	@echo "clean tools"
endif
