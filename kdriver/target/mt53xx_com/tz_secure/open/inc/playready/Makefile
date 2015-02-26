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

IC_NAME     = $(subst mt,,$(TARGET_IC))
DRV_ROOT    = ../../mt$(IC_NAME)
OBJDIR      = $(METADATA_OUTPUT_ROOT)/obj/tz_secure
TARGET      = mt$(IC_NAME)_tz.axf

-include $(DRIVER_CFG_DIR)/cfg.mak
-include $(DRV_ROOT)/drv_opt/$(COMPANY)/$(MODEL).def

ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5398 mt5880 mt5881))
DMX_FOLDER          = dmx9880
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5399))
DMX_FOLDER          = dmx5399
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5396 mt5368 mt5389))
DMX_FOLDER          = dmx6896
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5890 mt5882))
DMX_FOLDER          = dmx8290
endif
endif
endif
endif

DRM_FOLDER          = drm
OPEN_ARMLIB_DIR     = open/armlib
ARMLIB_DIR          = armlib
OPEN_SECURITY_DIR   = open/security
SECURITY_DIR        = security
VDEC_FOLDER         = vdec_v2
CRYPTO_DIR          = security/playready

# toolchain
CC      ?= $(CROSS_COMPILE)gcc
LD      ?= $(CROSS_COMPILE)ld
OBJCOPY ?= $(CROSS_COMPILE)objcopy

ifeq "$(ODB)" "true"
CC      ?= odbc comp $(CROSS_COMPILE)gcc
endif

#LD_FLAG = -Bstatic --shared --unresolved-symbols=report-all --version-script=./open/tz_reloc.map -T $(OBJDIR)/tz.sc -Map $(OBJDIR)/$(subst axf,map,$(TARGET)) -o $(OBJDIR)/$(TARGET)
LD_FLAG = -Bstatic --unresolved-symbols=report-all --version-script=./open/tz_reloc.map -T $(OBJDIR)/tz.sc -Map $(OBJDIR)/$(subst axf,map,$(TARGET)) -o $(OBJDIR)/$(TARGET)

ASM_FLAG  = -g -fpic -fno-unwind-tables -DCC_MT$(IC_NAME) -D__GCC__
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5881))
ASM_FLAG += -march=armv6z -mtune=arm1176jzf-s -mfloat-abi=soft -msoft-float -D__LINUX_ARM_ARCH__=6 -D__CC_GCC__
else
ASM_FLAG += -march=armv7-a -mtune=cortex-a9 -mfloat-abi=soft -D__LINUX_ARM_ARCH__=7
endif

CC_FLAG   = -O2 -g -Wall -Wno-long-long -std=gnu99 -fpic -fno-unwind-tables -fsigned-char -fshort-wchar -Wno-strict-aliasing
CC_FLAG  += -DNDEBUG -D_CPU_LITTLE_ENDIAN_ -D__GCC__
CC_FLAG  += -DLTC_SOURCE -DLTC_NO_FILE -DNPOPT_ENABLE_EXPORT_ACTION
CC_FLAG  += -DCC_MT$(IC_NAME) -DCC_SECURE_WORLD
CC_FLAG  += -DCC_CUSOMIZE_WV_KEY
CC_FLAG  += -DCC_CUSTOMIZE_DIVX_KEY
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5881))
CC_FLAG += -march=armv6z -mtune=arm1176jzf-s -mfloat-abi=soft -msoft-float -D__LINUX_ARM_ARCH__=6 -D__CC_GCC__
else
CC_FLAG  += -DCC_ENABLE_L2C -DCC_CA9 -march=armv7-a -mtune=cortex-a9 -mfloat-abi=soft -D__LINUX_ARM_ARCH__=7
endif

DEFINES += -DCC_WIDEVINE_L1_SUPPORT

CC_FLAG  += $(DEFINES)

ifneq "$(ENABLE_SMP_IC)" "true"
CC_FLAG  += -DCC_DISABLE_SMP
endif

ifeq "$(BUILD_CFG)" "rel"
CC_FLAG  += -DCC_TEE_STRICT_PROTECT_SUPPORT
endif

ifeq "$(TOM_MATH)" "fast"
CC_FLAG  += -DTFM_DESC
else
CC_FLAG  += -DLTM_DESC
endif

# TEE memory check
#CC_FLAG  += -DCC_TEE_FORCE_MEM_CHK

CC_INC = -I./open/inc \
-I./open/inc/hci \
-I./open/inc/skb \
-I$(DRV_ROOT)/$(IC_NAME)_driver/inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/private_inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_cust \
-I$(DRV_ROOT)/$(IC_NAME)_driver/drv_inc \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp \
-I$(DRV_ROOT)/$(IC_NAME)_driver/bsp/dram \
-I$(DRV_ROOT)/$(IC_NAME)_driver/tz_normal \
-I$(DMX_FOLDER)/cypher/aes \
-I$(DRM_FOLDER)/secure_world_inc \
-I$(DRM_FOLDER)/mtk_skb_driver \
-I$(DRM_FOLDER)/libtomcrypt-1.16/src/headers \
-I$(DRM_FOLDER)/libtommath-0.41 \
-I$(DRM_FOLDER)/mtk_hci \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC/include \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC/include/local \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC/src/cmn/include \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC/src/platform/sec/hci/include \
-I$(DRM_FOLDER)/mtk_hci/nautilus_4.2_RC/src/sec/common/crypto/include \
-I$(DRM_FOLDER)/widevine/inc \
-I$(DRV_ROOT)/../../middleware \
-I$(DRV_ROOT)/../../middleware/os/inc/ \
-I$(INC_ROOT)/drv_inc \
-I$(CHILING_ROOT)/app_if/mtal/mtal_inc \
-I$(SECURITY_DIR)/netflix/include \
-I$(SECURITY_DIR)/netflix/StorageAgent \
-I$(SECURITY_DIR)/netflix/TeePortingInterface \
-I.

ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5396 mt5368 mt5389))
CC_INC += \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv6896/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv6896/inc/sys 
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5398 mt5880))
CC_INC += \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8098/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8098/inc/sys 
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5399 mt5882))
CC_INC += \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv5399/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv5399/inc/sys 
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5881))
CC_INC += \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv5881/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv5881/inc/sys 
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5890))
CC_INC += \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8290/inc/hw \
-I$(DRV_ROOT)/$(IC_NAME)_driver/nptv8290/inc/sys 
endif
endif
endif
endif
endif

ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5398 mt5880 mt5881 mt5399 mt5890 mt5882))
CC_INC += \
-I$(DMX_FOLDER)/include \
-I$(DMX_FOLDER)/gcpu 
else
ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5396 mt5368 mt5389))
CC_INC += \
-I$(DMX_FOLDER) 
endif
endif


C_SRC = \
tz_main.c \
tz_reloc.c \
common/common.c \
common/printf.c \
common/ctype.c \
common/s_heap_func.c \
common/str_util.c \
bsp/arm/bootp.c \
services/tz_svcmgr.c \
services/aes.c \
services/dmx_descrambler.c \
services/svc_dmx.c \
services/svc_gcpu.c \
services/svc_drm.c \
services/svc_tzctl.c \
$(DRM_FOLDER)/mtk_util.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/ciphers/aes/aes.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/rmd128.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/rmd256.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/md2.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/md4.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/rmd160.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/md5.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/rmd320.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/sha1.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/tiger.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/sha2/sha512.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/sha2/sha256.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/hmac/hmac_init.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/hmac/hmac_process.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/hmac/hmac_done.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/hmac/hmac_memory.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/omac/omac_init.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/omac/omac_process.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/omac/omac_done.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/mac/omac/omac_memory.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/math/multi.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/math/rand_prime.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/math/ltm_desc.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/math/gmp_desc.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/math/tfm_desc.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_encrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_getiv.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_start.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_decrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_setiv.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/cbc/cbc_done.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_setiv.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_encrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_done.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_decrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_start.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_getiv.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ctr/ctr_test.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ecb/ecb_done.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ecb/ecb_encrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ecb/ecb_start.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/modes/ecb/ecb_decrypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/bit/der_length_bit_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/bit/der_encode_bit_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/bit/der_decode_bit_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/choice/der_decode_choice.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_decode_ia5_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_length_ia5_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/ia5/der_encode_ia5_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/integer/der_encode_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/integer/der_decode_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/integer/der_length_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_decode_object_identifier.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_encode_object_identifier.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/object_identifier/der_length_object_identifier.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/octet/der_length_octet_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/octet/der_decode_octet_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/octet/der_encode_octet_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_length_printable_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_decode_printable_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/printable_string/der_encode_printable_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_ex.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_free.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_length_sequence.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_multi.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_flexi.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_decode_sequence_multi.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_sequence_sibling_count.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/sequence/der_encode_sequence_ex.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_setof.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/set/der_encode_set.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_decode_short_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_length_short_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/short_integer/der_encode_short_integer.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_encode_utf8_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_decode_utf8_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utf8/der_length_utf8_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_encode_boolean.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_length_boolean.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/boolean/der_decode_boolean.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/t61/der_length_t61_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/t61/der_encode_t61_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/t61/der_decode_t61_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_decode_utctime.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_length_utctime.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/utctime/der_encode_utctime.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_decode_generalized_time.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/asn1/der/generalized_time/der_length_generalized_time.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/dh/dh.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_make_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_encrypt_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_exptmod.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_verify_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_export.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_sign_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_free.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_import.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/rsa/rsa_decrypt_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_mgf1.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_os2ip.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_encode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_encode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_i2osp.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_encode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_v1_5_decode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_oaep_decode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/pkcs1/pkcs_1_pss_decode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/sober128.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/rng_make_prng.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/rng_get_bytes.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/sprng.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/rc4.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/yarrow.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/prngs/fortuna.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/zeromem.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/error_to_string.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/burn_stack.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_register_prng.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_any.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_hash_is_valid.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_oid.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_id.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_register_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_fsa.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_cipher_is_valid.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_register_cipher.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_prng.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_cipher.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_prng_descriptor.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_cipher_descriptor.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_hash_id.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_prng.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_argchk.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_hash_descriptor.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_prng_is_valid.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_unregister_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/crypt/crypt_find_cipher_any.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/base64/base64_decode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/misc/base64/base64_encode.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/helper/hash_file.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/helper/hash_filehandle.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/helper/hash_memory_multi.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/hashes/helper/hash_memory.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mul2add.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_sizes.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_points.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_test.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_decrypt_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_make_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_dbl_point.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_get_size.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_import.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_encrypt_key.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_sign_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_shared_secret.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_map.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_ansi_x963_export.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod_timing.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_projective_add_point.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_verify_hash.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_import.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_free.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_mulmod.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ecc_export.c \
$(DRM_FOLDER)/libtomcrypt-1.16/src/pk/ecc/ltc_ecc_is_valid_idx.c \
$(DMX_FOLDER)/cypher/sha/sha1.c \
$(DMX_FOLDER)/cypher/sha/sha256.c

ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5890 mt5882))
C_SRC += bsp/arm/hal_armv7.c
else
C_SRC += bsp/arm/hal_ca9.c
endif

ifeq "$(WIDEVINE_EME_SUPPORT)" "true"
C_SRC += \
$(DRM_FOLDER)/widevine_eme/oemcrypto_session_tz.c \
$(DRM_FOLDER)/widevine_eme/oemcrypto_kexbox_tz.c \
$(DRM_FOLDER)/widevine_eme/oemcrypto_key_tz.c \
$(DRM_FOLDER)/widevine_eme/pkcs8.c \
$(DRM_FOLDER)/widevine_eme/asn1.c \
$(DRM_FOLDER)/widevine_eme/bignum.c \
$(DRM_FOLDER)/widevine_eme/rsa.c \
$(DRM_FOLDER)/widevine_eme/wvcrc.c 

CC_INC += -I$(DRM_FOLDER)/widevine_eme
CC_FLAG += -DCC_ENABLE_WIDEVINE_EME
endif

ifeq "$(TOM_MATH)" "fast"
C_SRC += \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqrmod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_48.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_3.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_small_set.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_32.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_6.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_7.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_generic.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_17.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_4.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_24.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_8.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_20.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_28.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_12.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_64.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr_comba_9.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/sqr/fp_sqr.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_lcm.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_gcd.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_isprime.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_prime_random_ex.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_prime_miller_rabin.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/numtheory/fp_invmod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_4.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_24.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_7.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_48.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_28.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_small_set.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mulmod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_9.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_64.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_2.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_2d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_8.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_32.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_3.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_12.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_20.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_17.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mul/fp_mul_comba_6.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mont/fp_montgomery_reduce.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mont/fp_montgomery_calc_normalization.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/mont/fp_montgomery_setup.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/misc/fp_ident.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/misc/fp_set.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/misc/fp_get_string.c \
#$(DRM_FOLDER)/tomsfastmath-0.14/src/generators/comba_sqr_gen.c \
#$(DRM_FOLDER)/tomsfastmath-0.14/src/generators/comba_mont_gen.c \
#$(DRM_FOLDER)/tomsfastmath-0.14/src/generators/comba_mult_gen.c \
#$(DRM_FOLDER)/tomsfastmath-0.14/src/generators/comba_mult_smallgen.c \
#$(DRM_FOLDER)/tomsfastmath-0.14/src/generators/comba_sqr_smallgen.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/exptmod/fp_2expt.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/exptmod/fp_exptmod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/divide/fp_mod_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/divide/fp_div.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/divide/fp_div_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/divide/fp_mod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_rshd.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_cnt_lsb.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_lshd.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_mod_2d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_div_2.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_div_2d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bit/fp_count_bits.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_reverse.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_read_radix.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_to_signed_bin.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_signed_bin_size.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_read_unsigned_bin.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_read_signed_bin.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_radix_size.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_unsigned_bin_size.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_s_rmap.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_to_unsigned_bin.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/bin/fp_toradix.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_cmp_mag.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_cmp.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/s_fp_add.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_add_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/s_fp_sub.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_sub.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_sub_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_cmp_d.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_addmod.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_add.c \
$(DRM_FOLDER)/tomsfastmath-0.14/src/addsub/fp_submod.c
else
C_SRC += \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init_copy.c \
$(DRM_FOLDER)/libtommath-0.41/bn_fast_s_mp_sqr.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mul_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_copy.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_set_int.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_2k.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_sqr.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_rabin_miller_trials.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_exch.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_zero.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_toradix.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_radix_smap.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init_multi.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_toradix_n.c \
$(DRM_FOLDER)/libtommath-0.41/bncore.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mod_2d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_cmp_mag.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_to_signed_bin_n.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_cmp_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_fwrite.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_gcd.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_get_int.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_neg.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_xor.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_rshd.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mod_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_fast_s_mp_mul_digs.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init_size.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_dr_setup.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_read_signed_bin.c \
$(DRM_FOLDER)/libtommath-0.41/bn_error.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_set.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_div_2d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_sub.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_to_unsigned_bin_n.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_cmp.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_to_unsigned_bin.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_2k_setup_l.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_montgomery_setup.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_exteuclid.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_div.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_setup.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_exptmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init_set.c \
$(DRM_FOLDER)/libtommath-0.41/bn_prime_tab.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_n_root.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_read_unsigned_bin.c \
$(DRM_FOLDER)/libtommath-0.41/bn_fast_s_mp_mul_high_digs.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mul.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_fermat.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mulmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_expt_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_mul_digs.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_exptmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_sqr.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_karatsuba_sqr.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_init_set_int.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_get_string.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_signed_bin_size.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_next_prime.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_div_3.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_sqrmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_invmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_add.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_grow.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_sub_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_dr_is_modulus.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_is_prime.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_is_square.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_miller_rabin.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_addmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_clamp.c \
$(DRM_FOLDER)/libtommath-0.41/bn_fast_mp_invmod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_to_signed_bin.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_montgomery_reduce.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_lshd.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_fread.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_lcm.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_mul_high_digs.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mul_2d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_fast_mp_montgomery_reduce.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_toom_mul.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_and.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_is_divisible.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_abs.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_2k_setup.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_unsigned_bin_size.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_2k_l.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_exptmod_fast.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_add_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_or.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_dr_reduce.c \
$(DRM_FOLDER)/libtommath-0.41/bn_reverse.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_sqrt.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_montgomery_calc_normalization.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_cnt_lsb.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_radix_size.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_is_2k.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_shrink.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_invmod_slow.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_mul_2.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_submod.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_count_bits.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_toom_sqr.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_reduce_is_2k_l.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_2expt.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_clear.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_div_2.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_read_radix.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_add.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_karatsuba_mul.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_div_d.c \
$(DRM_FOLDER)/libtommath-0.41/bn_s_mp_sub.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_clear_multi.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_prime_random_ex.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_rand.c \
$(DRM_FOLDER)/libtommath-0.41/bn_mp_jacobi.c
endif

ifeq "$(ENABLE_HCI)" "true"
C_SRC += \
$(DRM_FOLDER)/mtk_hci/hci.c \
$(DRM_FOLDER)/mtk_hci/hci_b64.c \
$(DRM_FOLDER)/mtk_hci/hci_crypto.c \
$(DRM_FOLDER)/mtk_hci/hci_drm.c \
$(DRM_FOLDER)/mtk_hci/hci_import.c \
$(DRM_FOLDER)/mtk_hci/hci_key.c \
$(DRM_FOLDER)/mtk_hci/hci_key_cache.c \
$(DRM_FOLDER)/mtk_hci/hci_licgen.c \
$(DRM_FOLDER)/mtk_hci/hci_lock.c \
$(DRM_FOLDER)/mtk_hci/hci_mem.c \
$(DRM_FOLDER)/mtk_hci/hci_move.c \
$(DRM_FOLDER)/mtk_hci/hci_ms3.c \
$(DRM_FOLDER)/mtk_hci/hci_notsupported.c \
$(DRM_FOLDER)/mtk_hci/hci_obfuscate.c \
$(DRM_FOLDER)/mtk_hci/hci_pers.c \
$(DRM_FOLDER)/mtk_hci/hci_ppproc.c \
$(DRM_FOLDER)/mtk_hci/hci_ppproc_default.c \
$(DRM_FOLDER)/mtk_hci/hci_ppproc_drbd.c \
$(DRM_FOLDER)/mtk_hci/hci_ppproc_qmu.c \
$(DRM_FOLDER)/mtk_hci/hci_setup.c \
$(DRM_FOLDER)/mtk_hci/hci_binstr.c \
$(DRM_FOLDER)/mtk_hci/custom_entry.c \
$(DRM_FOLDER)/mtk_hci/aes_api.c \
$(DRM_FOLDER)/mtk_hci/crypto_api.c \
$(DRM_FOLDER)/mtk_hci/rsa_api.c \
$(DRM_FOLDER)/mtk_hci/hmac_api.c \
$(DRM_FOLDER)/mtk_hci/sha1_api.c \
$(DRM_FOLDER)/mtk_hci/sha256_api.c \
$(DRM_FOLDER)/mtk_hci/rand_api.c \
$(DRM_FOLDER)/mtk_hci/asn.c \
$(DRM_FOLDER)/mtk_hci/pkcs8.c
endif

ifeq "$(ENABLE_SKB)" "true"
C_SRC += \
$(DRM_FOLDER)/mtk_skb_driver/SkbGenericCipher.c \
$(DRM_FOLDER)/mtk_skb_driver/SkbGenericEngine.c \
$(DRM_FOLDER)/mtk_skb_driver/SkbGenericSecureData.c \
$(DRM_FOLDER)/mtk_skb_driver/SkbGenericTransform.c \
$(DRM_FOLDER)/mtk_skb_driver/tomcrypt_asn1_missing.c
endif

C_SRC += $(DRM_FOLDER)/sec_buf/sec_buf.c

C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_ex.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_tz.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_glue.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_keyctrlblk.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_sessionctx.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_sessionkeytbl.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_usage_table.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_noncetbl.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_keybox.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_key.c
C_SRC += $(DRM_FOLDER)/widevine/mtkcrypto_engine.c
C_SRC += $(DRM_FOLDER)/widevine/mtkutil_vector.c
C_SRC += $(DRM_FOLDER)/widevine/mtkutil_div6432.c
C_SRC += $(DRM_FOLDER)/widevine/mtkutil_sort.c
C_SRC += $(DRM_FOLDER)/widevine/mtkutil_map.c
C_SRC += $(DRM_FOLDER)/widevine/mtkutil_net.c
C_SRC += $(DRM_FOLDER)/widevine/wvcrc.c
C_SRC += $(DRM_FOLDER)/widevine/string_conversions.c

C_SRC += $(DRM_FOLDER)/rpmb/rpmb.c
C_SRC += bsp/timer.c

ifeq "$(ENABLE_FUJ_MARLIN)" "true"
C_SRC += \
$(DRM_FOLDER)/marlin3/DecoderDRM_TZ.c \
$(DRM_FOLDER)/marlin3/MediaDRM_TZ.c
endif

ifeq "$(TVP_SUPPORT)" "true"
DEFINES += -DCC_TVP_SUPPORT

C_SRC += tz_mid$(IC_NAME)/mid.c 
C_SRC += common/fbm.c
C_SRC += bsp/bim_if.c
endif


A_SRC = \
monitor/gcc_tz_init.S \
monitor/gcc_tz_vectors.S \
monitor/gcc_tz_mon.S \
$(ARMLIB_DIR)/aeabi_sdivfast_rs.S \
$(ARMLIB_DIR)/memcmp.S \
$(ARMLIB_DIR)/qsortnoex.S \
$(ARMLIB_DIR)/rt_memcpy_v6.S \
$(ARMLIB_DIR)/rt_memmove_v6.S \
$(ARMLIB_DIR)/strcmpv6.S

O_SRC = \
$(ARMLIB_DIR)/aeabi_idiv0.o \
$(ARMLIB_DIR)/aeabi_ldiv0.o \
$(ARMLIB_DIR)/aeabi_memset.o \
$(ARMLIB_DIR)/llsdiv.o \
$(ARMLIB_DIR)/lludiv5.o \
$(ARMLIB_DIR)/memset.o \
$(ARMLIB_DIR)/rt_memclr.o \
$(ARMLIB_DIR)/rt_memclr_w.o \
$(ARMLIB_DIR)/rt_memcpy_w.o \
$(ARMLIB_DIR)/rt_memmove_w.o \
$(ARMLIB_DIR)/strncpy.o

C_SRC += \
$(SECURITY_DIR)/tee_common.c

C_SRC += \
$(SECURITY_DIR)/securestorage/tee_securestorage.c

C_SRC += \
$(SECURITY_DIR)/netflix/tee_nflx.c \
$(SECURITY_DIR)/netflix/utils/ManufSS.c	\
$(SECURITY_DIR)/netflix/utils/b64.c	\
$(SECURITY_DIR)/netflix/utils/teeUtils.c	\
$(SECURITY_DIR)/netflix/utils/teeTomUtils.c	\
$(SECURITY_DIR)/netflix/utils/teeCryptoLibTom.c	\
$(SECURITY_DIR)/netflix/TeePortingInterface/tpiStorage.c	\
$(SECURITY_DIR)/netflix/CommAgent/TeeEntry.c	\
$(SECURITY_DIR)/netflix/CryptoAgent/CryptoAgent.c	\
$(SECURITY_DIR)/netflix/CryptoAgent/CryptoAgentDispatch.c	\
$(SECURITY_DIR)/netflix/CryptoAgent/KeyMgmt.c	\
$(SECURITY_DIR)/netflix/MgmtAgent/TeeMgr.c	\
$(SECURITY_DIR)/netflix/StorageAgent/StorageAgent.c	\
$(SECURITY_DIR)/netflix/StorageAgent/StorageAgentDispatch.c	\
$(SECURITY_DIR)/netflix/DrmAgent/DrmAgent.c	\
$(SECURITY_DIR)/netflix/DrmAgent/DrmAgentDispatch.c	\

C_SRC += \
$(SECURITY_DIR)/ddi/tee_ddi.c


ifeq "$(ENABLE_PLAYREADY_SUPPORT)" "true"
CC_FLAG += -DCC_ENABLE_PLAYREADY_SUPPORT

C_SRC += \
$(SECURITY_DIR)/playready/tee_playready.c

CC_INC += -I./open/inc/playready/prhal/ \
-I./open/inc/playready/prhal/inc/    \
-I./open/inc/playready//prhal/oemcommoninc/    \
-I./open/inc/playready//prhal/AES/   \
-I./open/inc/playready//prhal/xmr/   \
-I./open/inc/playready/oem/common/cbc/ \
-I./open/inc/playready/oem/common/inc/ \
-I./open/inc/playready/inc/ \
-I./open/inc/playready/oem/common/hal/CoreAPI/ \
-I./open/inc/playready/oem/common/hal/oemimpl/ \
-I./open/inc/playready/xmr/ \
-I./open/inc/playready/ \

C_SRC += \
$(SECURITY_DIR)/playready/oem/common/bignum/bignumnoinline.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/bignum.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/modular.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/ecurve.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/kmuladd.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/modmulch1.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/modexp.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/field.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/ecppq.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/kinvert.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/lucas.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/mpgcdex.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/kdiv.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/divisibility.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/mprand.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/mpaddsubcmp.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/kinitpr.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/ecex2001.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/mpalloc.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/modsqrt.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/ecexpon.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/mpmul22.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/multlow.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/digitogcd.c  \
$(SECURITY_DIR)/playready/oem/common/bignum/divide.c  \
$(SECURITY_DIR)/playready/oem/common/sha/sha1/oemsha1.c  \
$(SECURITY_DIR)/playready/oem/common/sha/hash/oemshahash.c  \
$(SECURITY_DIR)/playready/oem/common/sha/sha256/oemsha256.c  \
$(SECURITY_DIR)/playready/oem/common/sha/hmac/oemshahmac.c  \
$(SECURITY_DIR)/playready/oem/common/ecc/base/oemeccp256.c  \
$(SECURITY_DIR)/playready/oem/common/ecc/base/oemeccp160.c  \
$(SECURITY_DIR)/playready/oem/common/ecc/baseimpl/oemeccp160impl.c  \
$(SECURITY_DIR)/playready/oem/common/ecc/baseimpl/oemeccp256impl.c  \
$(SECURITY_DIR)/playready/oem/common/cipher/oemcipher.c  \
$(SECURITY_DIR)/playready/oem/common/des/deskey.c  \
$(SECURITY_DIR)/playready/oem/common/parsers/oemparsers_revinfo.c  \
$(SECURITY_DIR)/playready/oem/common/parsers/oemparsers_bcert.c  \
$(SECURITY_DIR)/playready/oem/common/parsers/oemparsers_bcrl.c  \
$(SECURITY_DIR)/playready/oem/common/parsers/oemparsers_utf8.c  \
$(SECURITY_DIR)/playready/oem/common/profile/oemprofile_common.c  \
$(SECURITY_DIR)/playready/oem/common/AES/oemaesmulti.c  \
$(SECURITY_DIR)/playready/oem/common/AES/oemaes.c  \
$(SECURITY_DIR)/playready/oem/common/AES/aes128.c  \
$(SECURITY_DIR)/playready/oem/common/AES/aesbox.c  \
$(SECURITY_DIR)/playready/oem/common/cbc/lib/oemcbc.c  \
$(SECURITY_DIR)/playready/oem/common/lib/oemimpl_common.c  \
$(SECURITY_DIR)/playready/oem/common/lib/oemndr.c  \
$(SECURITY_DIR)/playready/oem/common/rc4/lib/rc4.c  \
$(SECURITY_DIR)/playready/oem/common/md5/oemmd5.c  \
$(SECURITY_DIR)/playready/oem/common/hal/CoreAPI/oemhaldevcerts.c  \
$(SECURITY_DIR)/playready/oem/common/hal/CoreAPI/oemhal.c  \
$(SECURITY_DIR)/playready/oem/common/hal/CoreAPI/oemderivedkeys.c  \
$(SECURITY_DIR)/playready/oem/common/hal/CoreAPI/oemsparsexmrparser.c  \
$(SECURITY_DIR)/playready/oem/common/hal/oemimpl/oemhaloemimpl.c  \
$(SECURITY_DIR)/playready/oem/linux/trace/oemtrace.c  \
$(SECURITY_DIR)/playready/oem/linux/oemdebug.c  \
$(SECURITY_DIR)/playready/oem/linux/oemimplrand.c  \
$(SECURITY_DIR)/playready/oem/linux/oemimpl.c  \
$(SECURITY_DIR)/playready/oem/linux/oemcriticalsection.c  \
$(SECURITY_DIR)/playready/modules/utilities/general/drmutilitiesgeneral.c \
$(SECURITY_DIR)/playready/modules/strsafe/real/drmstrsafeimplreal.c   \
$(SECURITY_DIR)/playready/modules/base64/real/drmbase64implreal.c   \
$(SECURITY_DIR)/playready/modules/crt/drmcrtcommon.c \
$(SECURITY_DIR)/playready/modules/crt/real/crtimplreal.c \
$(SECURITY_DIR)/playready/modules/crt/real/crtutilreal.c \
$(SECURITY_DIR)/playready/modules/stkalloc/real/drmstkallocimplreal.c \
$(SECURITY_DIR)/playready/constants/drmconstants.c \
$(SECURITY_DIR)/playready/constants/drmutilitiesgeneralconstants.c \
$(SECURITY_DIR)/playready/xmr/drmxmrparser.c   \
$(SECURITY_DIR)/playready/xmr/drmxmr.c   \
$(SECURITY_DIR)/playready/xmr/drmxmrbuilder.c   \
$(SECURITY_DIR)/playready/pr25_hal_interface.c

endif


C_SRC += \
$(SECURITY_DIR)/vudu/tee_vudu.c \
$(SECURITY_DIR)/vudu/tee_des.c \
$(SECURITY_DIR)/vudu/tee_aes.c \

C_SRC += \
$(SECURITY_DIR)/keystore/arithmatic.c \
$(SECURITY_DIR)/keystore/auth.c \
$(SECURITY_DIR)/keystore/tee_dtcp_api.c \
$(SECURITY_DIR)/keystore/tee_crypto_key.c


ifeq "$(ULI_PROVISION_SUPPORT)" "true"
C_SRC += $(SECURITY_DIR)/uli/tee_uli.c
CC_FLAG += -DTEE_ULI_CEM_MG='"tee_uli_$(ULI_CEM_MG).h"'
CC_FLAG += -DCC_ENABLE_ULI_IN_TEE
ifneq "$(NFLX_NO_ULI)" "true" 
CC_FLAG += -DNFLX_ULI_SUPPORT
endif
endif




# WFD HDCP2
ifeq "$(ENABLE_HDCP2)" "true"
CC_INC += -I$(DRM_FOLDER)/hdcp2
C_SRC += \
$(DRM_FOLDER)/hdcp2/drm_hdcp2.c \
$(DRM_FOLDER)/hdcp2/drm_hdcp2x_testkey.c
CC_FLAG += -DCC_ENABLE_HDCP2
endif

ifeq "$(CI_TZ_SUPPORT)" "true"
C_SRC += \
$(DRM_FOLDER)/ciplus/svc_ciplus.c \
$(DRM_FOLDER)/ciplus/ci_trust_zone.c \
$(DRM_FOLDER)/ciplus/ccactx.c \
$(DRM_FOLDER)/ciplus/cccert.c \
$(DRM_FOLDER)/ciplus/ccfunc.c \
$(DRM_FOLDER)/ciplus/ccmsg.c \
$(DRM_FOLDER)/ciplus/ccsac.c \
$(DRM_FOLDER)/ciplus/sectk.c \
$(DRM_FOLDER)/ciplus/cclru.c \
$(DRM_FOLDER)/ciplus/celo.c \
$(DRM_FOLDER)/ciplus/ccbn_tfm.c
endif

ifeq "$(IPTVES_SUPPORT)" "true"
CC_FLAG += -DCC_ENABLE_IPTVES -DBUILD_LEVEL=3
# SONY IPTVES

C_SRC += $(DRM_FOLDER)/iptves_sec/sec/keymng/src/keymng.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/drm/src/drm.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cache/src/cache.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/sac/src/sac.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/certmng/src/certmng.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/sdi/src/sdi.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/sp/src/sp.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/asn/src/asn.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/ecm/src/ecm.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/nvs/src/nvs.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/cert.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/cert_verify.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/crl_inf.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/crl_verify.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/cert_inf.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/crl_parse.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/sec/cert/src/cert_parse.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/prof.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/mutex.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/base.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/mem.c 
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/dbg.c 
#C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/fio.c
C_SRC += $(DRM_FOLDER)/iptves_sec/base/src/time.c

C_SRC += $(DRM_FOLDER)/iptves_sec/outside/crypto_api_iptves.c
C_SRC += $(DRM_FOLDER)/iptves_sec/outside/crypto_ecc_iptves.c
C_SRC += $(DRM_FOLDER)/iptves_sec/outside/http_api_iptves.c
C_SRC += $(DRM_FOLDER)/iptves_sec/outside/rend_api_iptves.c
C_SRC += $(DRM_FOLDER)/iptves_sec/outside/time_api_iptves.c
#C_SRC += $(DRM_FOLDER)/iptves_sec/outside/test.c


CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/keymng/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/drm/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/cache/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/sac/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/certmng/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/sdi/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/sp/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/asn/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/ecm/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/nvs/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/sec/cert/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/base/include 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/include/outside 
CC_INC += -I$(DRM_FOLDER)/iptves_sec/include


endif


JOBS ?= 24

ifeq ($(TARGET_IC), $(filter $(TARGET_IC), mt5881)) 
# Remy with ARM11 arch
C_SRC:=$(C_SRC:hal_ca9.c=hal_1176.c)
endif

OBJ    = $(A_SRC:.S=.o) $(C_SRC:.c=.o)
OBJ   := $(addprefix $(OBJDIR)/,$(OBJ))
O_SRC := $(addprefix $(OBJDIR)/,$(O_SRC))

.PHONY : all bin symlink clean

all: symlink
	$(MAKE) -j $(JOBS) bin
	@echo "build OK"

symlink:
#	@if [ ! -e bsp ]; then \
		(ln -s $(DRV_ROOT)/$(IC_NAME)_driver/bsp .; \
		echo bsp >> $(OBJ_ROOT)/tz_symlink.txt; \
		exit 0); \
	fi
#	@if [ ! -e $(DMX_FOLDER) ]; then \
		(ln -s $(DRV_ROOT)/$(IC_NAME)_driver/$(DMX_FOLDER) .; \
		echo $(DMX_FOLDER) >> $(OBJ_ROOT)/tz_symlink.txt; \
		exit 0); \
	fi
	@ln -sf $(DRV_ROOT)/$(IC_NAME)_driver/bsp .;
	@ln -sf $(DRV_ROOT)/$(IC_NAME)_driver/$(DMX_FOLDER) .;
	@ln -sf $(DRV_ROOT)/$(IC_NAME)_driver/tz_mid$(IC_NAME) .;

	@if [ ! -d $(OBJDIR)/$(ARMLIB_DIR) ]; then \
		mkdir -p $(OBJDIR)/$(ARMLIB_DIR); \
	fi
	@echo "Copy armlib *.o files to obj folder"
	@cp -fv $(OPEN_ARMLIB_DIR)/*.o $(OBJDIR)/$(ARMLIB_DIR);

bin: $(TARGET)
	@$(OBJCOPY) ${OBJCFLAGS} -O binary $(OBJDIR)/$(TARGET) $(OBJDIR)/$(subst axf,bin,$(TARGET))
	@ln -sf $(OBJDIR)/$(TARGET) mt53xx_tz.axf 

$(TARGET): $(OBJ)
	@echo "Linking $(TARGET)..."
	@$(CC) -x c -o $(OBJDIR)/tz.ldsx $(CC_FLAG) $(CC_INC) -E open/tz.lds
	@awk '/OUTPUT_FORMAT/,p' $(OBJDIR)/tz.ldsx > $(OBJDIR)/tz.sc
	$(LD) $(LD_FLAG) --start-group $(OBJ) $(O_SRC) --end-group


$(OBJDIR)/%.o :: %.c
	@echo "--- Building `basename $<` ---"
	@if [ ! -d `dirname $@` ] ; then \
		mkdir -p `dirname $@`; \
	fi
	@$(CC) -c $(CC_FLAG) $(CC_INC) -o $@ $<

$(OBJDIR)/%.o :: %.S
	@echo "--- Building `basename $<` ---"
	@if [ ! -d `dirname $@` ] ; then \
		mkdir -p `dirname $@`; \
	fi
	@$(CC) -c $(ASM_FLAG) -o $@ $<

clean:
#	@if [ -e $(OBJ_ROOT)/tz_symlink.txt ]; then \
		rm -rf `cat $(OBJ_ROOT)/tz_symlink.txt`; \
	fi
	rm -f bsp $(DMX_FOLDER) tz_mid$(IC_NAME)
	rm -rf $(OBJDIR) *.axf *.bin *.bak *.lst *.sym *.scatx *.sc *.map *.ldsx
