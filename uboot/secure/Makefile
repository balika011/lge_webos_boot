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

ifeq ($(BOOT),nor)
SC_FILE = secure_nor.sc
else
SC_FILE = secure_nand.sc
endif

ifeq "$(CUST_CLR_IMG)" "true"

CUST_CLR_ROOT      = $(THIS_ROOT)/secure_rls
CUST_CLR_TOOL_PATH = $(CUST_CLR_ROOT)/tools

ifdef BUILD_CFG
CUST_CLR_PATH = $(CUST_CLR_ROOT)/cli/bin
else
CUST_CLR_PATH = $(CUST_CLR_ROOT)/rel/bin
endif
endif

CFLAGS= -std=c99 -pedantic -Wall -Wpointer-arith -Wstrict-prototypes -O0 -g
CC= gcc



SIGGEN_KEY := $(shell if [ -e $(SIGN_TOOL_DIR)/customer_pub_mtka5lr_key -a -e $(SIGN_TOOL_DIR)/customer_priv_mtka5lr_key ]; then echo "$(SIGN_TOOL_DIR)/customer_pub_mtka5lr_key $(SIGN_TOOL_DIR)/customer_priv_mtka5lr_key"; else echo "";fi)
AES_KEY2 := $(SIGN_TOOL_DIR)/aes_key_set2\
#$(warning SIGN_TOOL_DIR = $(SIGN_TOOL_DIR))
#$(error SIGGEN_KEY = $(SIGGEN_KEY))
T_MTK = $(SIGN_TOOL_DIR)/sign

clean distclean:
	rm -f  uboot.* *.map *preloader*
	
package: preprocess packimg siggen
	chmod +x ./packimg	
	chmod +x ./lzhsenc	
	rm -f u-boot_pack.bin
	rm -f u-boot_pack_secure.lzhs
	cp ../$(UBOOT_DIR)/u-boot.bin .
	cp ../preloader/$(MODEL)_preloader_$(BOOT).bin .
	./siggen $(MODEL)_preloader_$(BOOT).bin $(MODEL)_Preloader_$(BOOT).bin 9216 0 Customer_Key_Encrypt_Data preloader_signature
	./lzhsenc u-boot.bin u-boot_pack_secure.lzhs
	./siggen u-boot_pack_secure.lzhs U-boot_pack.lzhs 245248 0 Customer_Key_Encrypt_Data uboot_signature
	if [ -f $(SC_FILE) ]; then \
		/bin/sed 's/MODEL/$(MODEL)/g' $(SC_FILE) > temp1.sc; \
		/bin/sed 's/BOOT/$(BOOT)/g' temp1.sc > temp2.sc; \
		./packimg -f temp2.sc; \
	fi
	mv -f $(MODEL)_secure_$(BOOT)boot.bin ..

5365_package: packimg
	chmod +x ./packimg	
	chmod +x ./lzhsenc
#	chmod +x ./t_mtk
#	chmod +x ./siggen
ifneq "$(LG_SIGN_FLOW)" "true"
	chmod +x $(T_MTK)
endif
	rm -f 14KB_preloader.bin Customer_Key_Encrypt_Data Customer_Key_Signature SHA1_Signature
	rm -f u-boot_pack.bin
	rm -f u-boot_pack_secure.lzhs
	cp ../$(UBOOT_DIR)/u-boot.bin .
	cp ../pack/mtkloader.bin .
	./lzhsenc u-boot.bin u-boot_pack_secure.lzhs
ifeq "$(CUST_CLR_IMG)" "true"
	cp -f u-boot_pack_secure.lzhs $(CUST_CLR_PATH)
endif
ifeq "$(SECURE_CUSTKEY)" "y"
ifeq "$(SECURE_BOOT_V2)" "true"
	$(T_MTK) u-boot_pack_secure.lzhs 0 2 0 $(AES_KEY2) $(SIGGEN_KEY)
else
	$(T_MTK) u-boot_pack_secure.lzhs 0 2 0 $(SIGGEN_KEY)
endif
else
	$(T_MTK) u-boot_pack_secure.lzhs 0 1
endif
	@rm -f MTK_PRIVATE_KEY_0
	@rm -f MTK_PUBLIC_KEY_0
	/bin/sed 's/MODEL/$(MODEL)/g' mt_secure_emmc.sc > temp1.sc;
	/bin/sed 's/BOOT/$(BOOT)/g' temp1.sc > temp2.sc;
	./packimg -f temp2.sc;
ifeq "$(CUST_CLR_IMG)" "true"
	cp -f temp2.sc $(CUST_CLR_TOOL_PATH)
	cp -f packimg $(CUST_CLR_TOOL_PATH)
endif
	mv -f $(MODEL)_secure_$(BOOT)boot.bin ..
	cp -f u-boot_pack_secure.lzhs ..
	$(warning (yjdbg.3) packing for tzfw.bin)
	@cp ../board/$(TARGET_IC)/env.o ./
	./packimg -f tzfw.sc
	# please help to move to right place
#	../../../tools/mkepk -c tzfw.pak tzfw.bin tzfw AA 0 0 0 RELEASE
	@cp tzfw.bin ../../result
	
preprocess:
	@cp -au ../pack/packimg.c .
	@cp -au ../pack/lzhsenc .
	@cp -au ../pack/zboot.img .
       	    					
packimg: preprocess
	$(CC) -m32 -g -o packimg packimg.c
