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
# System config file
include ../config.mak

export MODE ?= debug
HDR_TOOL = mt5391_pack

ifeq ($(BOOT),nor)
SC_FILE = zboot_nor.sc
else
SC_FILE = zboot_nand.sc
endif

clean distclean:
	rm -f  uboot.* *.map *preloader*

package: packimg
	@chmod +x ./packimg
	@chmod +x ./lzhsenc
	@rm -f u-boot_pack.bin
	@rm -f u-boot_pack.lzhs
	@cp ../$(UBOOT_DIR)/u-boot.bin .

	cp ../preloader/$(MODEL)_preloader_$(BOOT).bin .
	@./lzhsenc u-boot.bin u-boot_pack.lzhs
	@if [ -f $(SC_FILE) ]; then \
		/bin/sed 's/MODEL/$(MODEL)/g' $(SC_FILE) > temp1.sc; \
		/bin/sed 's/BOOT/$(BOOT)/g' temp1.sc > temp2.sc; \
		./packimg -f temp2.sc; \
	fi
	@mv -f $(MODEL)_$(BOOT)boot.bin ..

5365_package: packimg
	@chmod +x ./packimg
	@chmod +x ./lzhsenc
	@chmod +x $(HDR_TOOL)
	@rm -f u-boot_pack.bin
	@rm -f u-boot_pack.lzhs
	@cp ../$(UBOOT_DIR)/u-boot.bin .
	@rm -rf .dummy .dummy.hdr
	@touch .dummy
	./$(HDR_TOOL) $(MODEL)_$(MODE)_mtkloader_$(BOOT).bin .dummy .dummy.hdr 2 "$(COMPANY)" "$(MW_MODEL)" "2011.12" 524288 2
	@rm -rf .dummy .dummy.bin
	@cp $(MODEL)_$(MODE)_mtkloader_$(BOOT).bin.hdr.bin mtkloader.bin
	@./lzhsenc u-boot.bin u-boot_pack.lzhs
	/bin/sed 's/MODEL/$(MODEL)/g' mt_emmc.sc > temp1.sc;
	/bin/sed 's/BOOT/$(BOOT)/g' temp1.sc > temp2.sc;
	./packimg -f temp2.sc;
	@mv -f $(MODEL)_$(BOOT)boot.bin ..
	@cp -f mtkloader.bin ..
	@cp -f u-boot_pack.lzhs ../u-boot_pack.lzhs

packimg: packimg.c
	gcc -m32 -g -o packimg packimg.c

cmmrm: cmmrm.c
	gcc -m32 -g -o cmmrm cmmrm.c

make_cmmrm: make_cmmrm.c
	gcc -m32 -g -o make_cmmrm make_cmmrm.c
