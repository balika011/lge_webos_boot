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
# $Revision: #27 $
# $Date: 2009/09/04 $
# $Author: brianpc.huang $
#
# Description:
#         For each main program to define the executable file name.
#
#         Specify the source files to be compile in SRC.
#############################################################################

ifneq ($(strip $(findstring /drv_build/, $(PWD))),)
export DRIVER_DEMO = y
export DIAG_DIR = $(firstword $(subst /drv_build/mt5882/driver/, /, $(PWD)))/target/mt53xx_com/diag
else
export DIAG_DIR = $(PROJECT_ROOT)/target/mt53xx_com/diag
endif


THIS_COMPONENT = diag

ifdef LINUX_DRV_ROOT
export DRV_ROOT = $(LINUX_DRV_ROOT)
else
    ifdef UT_DRV_ROOT
        export DRV_ROOT = ..
        ifndef DRIVER_DIR
            export DRIVER_DIR = 5882_driver
        endif
    else
	export DRV_ROOT = $(firstword $(subst /drv_build/mt5882/driver/,/drv_build/mt5882/driver /, $(PWD)))
	ifndef DRIVER_DIR
	export DRIVER_DIR = 5882_driver
	endif
    endif
endif

#############################################################################
# Standard components
#############################################################################

SUB_COMPONENTS =

ifeq "$(BOOT_CLI)" ""
ifeq "$(TV_DRV_VFY)" "true"
	SRC += UDVT_cmd.c
  SRC += vdec_vfy_cmd.c
	SRC += imgresz_vfy_cmd.c
	SRC += png_vfy_cmd.c
  SRC += jpg_vfy_cmd.c
  SRC += gfx3d_vfy_cmd.c
else
########## !!!!! Put new driver command here. !!!!!
SRC += os_cmd.c
SRC += osd_cmd.c osd_diag_cmd.c
#SRC += gdma_cmd.c
SRC += gfx_cmd.c

ifeq "$(LINUX_SLT_SUPPORT)" "true"
SRC += gfx3d_vfy_cmd.c
endif

#SRC += gfx_emu.c
#SRC += gfx_benchmark.c
SRC += imgrz_cmd.c imgrz_emu.c
SRC += dmx_cmd.c

ifeq "$(DMX_EMULATION)" "true"
SRC += dmx_emu_cmd.c
SRC += fvr_cmd.c
DEFINES += -DCC_DMX_EMULATION
endif

SRC += jpg_cmd.c jpg_cmd_cases.c
SRC += png_cmd.c
SRC += webp_cmd.c
SRC += aud2_cmd.c
SRC += mpv_cmd.c
SRC += pmx_cmd.c
SRC += nim_cmd.c
SRC += vdec_cmd.c
SRC += vomx_cmd.c
#SRC += vdec_ide.c
ifdef CC_SUPPORT_VENC
SRC += venc_cmd.c
endif
ifdef CC_SUPPORT_MUXER
SRC += muxer_cmd.c
endif
SRC += vdp_cmd.c
SRC += ir_cmd.c
SRC += timeprofile_cmd.c
SRC += pdwnc_cmd.c
SRC += rtc_cmd.c
SRC += sif_cmd.c
SRC += pwm_cmd.c
SRC += fbm_cmd.c
SRC += gpio_cmd.c
SRC += av_cmd.c
SRC += dbs_cmd.c
SRC += eeprom_cmd.c
SRC += nptv_cmd.c
SRC += nptv_mon_out_cmd.c
SRC += nptv_bypass_cmd.c
SRC += nptv_srm_cmd.c
SRC += nptv_dec_cmd.c
SRC += nptv_dvi_cmd.c
SRC += cec_cmd.c
SRC += nptv_avlink_cmd.c
SRC += nptv_hdmi_cmd.c
SRC += nptv_hvpos_cmd.c
SRC += nptv_pq_cmd.c
#SRC += nptv_mjc_cmd.c
SRC += nptv_scart_cmd.c
SRC += nptv_scpos_cmd.c
SRC += nptv_fpga_cmd.c
SRC += nptv_scpip_gfx_cmd.c
SRC += nptv_scpip_venc_cmd.c
SRC += nptv_vrm_cmd.c
SRC += nptv_tvd_cmd.c
SRC += nptv_vbi_cmd.c
SRC += nptv_vga_cmd.c
SRC += nptv_vfe_cmd.c
SRC += nptv_ddcci_cmd.c
SRC += nptv_tdtv_cmd.c
SRC += nptv_dgo_cmd.c
SRC += mid_cmd.c
SRC += tve_cmd.c
SRC += tcon_cmd.c
#SRC += swdmx_cmd.c
SRC += linux_cmd.c
SRC += pcmcia_cmd.c
SRC += nptv_di_cmd.c
SRC += nptv_ptgen_cmd.c
SRC += bim_cmd.c
SRC += memtest_cmd.c
SRC += ldm_cmd.c

ifeq "$(CPU_TEST)" "y"
SRC += dhrystone_cmd.c
DEFINES += -DCPU_TEST_ENABLE=1
endif
endif

endif

ifdef LINUX_DRV_ROOT
ifeq "$(CDC_DUMP_SUPPORT)" "true"
SRC += musb_cmd.c
DEFINES += -DCC_CDC_SUPPORT
else
ifeq "$(LINUX_SLT_SUPPORT)" "true"
SRC += musb_cmd.c
endif
endif
endif

ifndef LINUX_DRV_ROOT
SRC += lzhs_cmd.c
SRC += musb_cmd.c


#ifeq "$(BOOT_TYPE)" "ROM2NAND"
#SRC += nand_cmd.c
#endif



SRC += nor_cmd.c
SRC += uart_cmd.c
SRC += msdc_cmd.c
#SRC += psr_cmd.c
#SRC += pod_cmd.c pod_emu.c
ifdef ENABLE_SMARTCARD
SRC += smc_cmd.c smc_emu.c
endif
#ifdef CC_INET_SUPPORT
SRC += net_cmd.c
#endif
endif #LINUX_DRV_ROOT
#ifdef SUPPORT_EXTRA_CLI_CMD
#SRC += extra_cmd.c
#endif
ifdef LINUX_DRV_ROOT
SRC += diag_cmain.c
SRC += net_cmd.c
ifdef ENABLE_MULTIMEDIA
SRC += playmgr_cmd.c
endif
else
ifdef DRIVER_DEMO
SRC += diag_cmain.c
SRC += nav_cmd.c
SRC += psipsr_cmd.c
ifdef ENABLE_MULTIMEDIA
SRC += playmgr_cmd.c
endif

else
SRC +=	diag_init.c
endif
endif


ifdef ENABLE_MULTIMEDIA
# Options enable dump data
ifeq "$(SWDMX_DUMP)" "true"
DEFINES += -DSWDMX_DUMP_DATA
endif
SRC += swdmx_cmd.c
SRC += feeder_cmd.c
endif

#ifdef DRV_SUPPORT_EXTMJC
SRC += extmjc_cmd.c
#endif

SRC += gcpu_cmd.c

ifeq "$(findstring DTD, $(BRDCST_IF))" "DTD"
DEFINES += -DDTD_INCLUDE
endif

ifeq "$(MODEL)" "slt5882"
SRC += slt_cmd.c
endif

SRC += cbus_cmd.c

ifeq "$(LINUX_SLT_SUPPORT)" "true"
SRC += nav_cmd.c
SRC += slt_cmd.c
endif
#############################################################################
# Optional components
#############################################################################

#############################################################################
# Global definitions
#############################################################################
# DEFINES will work at the whole project.
DEFINES += -DCC_MT5882
ifeq "$(FPGA_SUPPORT)" "true"
DEFINES += -DCC_FPGA_SUPPORT
endif

#DEFINES += -DVDEC_TIME_PROFILE

ifndef LINUX_DRV_ROOT
# SUPPORT_FLASH_PQ is defined in drv_opt .def file
#DEFINES += -DSUPPORT_FLASH_PQ
endif

# CC_INC will work at the whole project.
CC_INC +=

# USB module include path.
USBDIR =  $(DRV_ROOT)/$(DRIVER_DIR)/usb
NETDIR =  $(DRV_ROOT)/$(DRIVER_DIR)/net
CC_INC += -I$(USBDIR)/include \
          -I$(USBDIR)/include/class \
          -I$(USBDIR)/src/systems/ucos \
          -I$(USBDIR)/src/systems/ucos/afs \
          -I$(USBDIR)/src/cd \
          -I$(USBDIR)/src/drivers/msd \
          -I$(USBDIR)/src/drivers/aud \
          -I$(USBDIR)/src/drivers/sicd \
          -I$(USBDIR)/src/drivers/hub

CC_INC += -I$(NETDIR)

CC_INC += -I$(NPTV_ROOT)/inc \
	  -I$(NPTV_ROOT)/inc/hw \
	  -I$(NPTV_ROOT)/inc/drv \
	  -I$(NPTV_ROOT)/inc/sys \
	  -I$(NPTV_ROOT)/inc/empty \
	  -I$(DRV_ROOT)/$(DRIVER_DIR)/brdcst_if/tuner_src/TunerHAL \
      -I$(DRV_ROOT)/$(DRIVER_DIR)/fm \
      -I$(DRV_ROOT)/$(DRIVER_DIR)/fm/fat

export OPT_FLAG = -O0

ifdef DRIVER_DEMO
CC_INC += -I$(DRV_ROOT)/demo
CC_INC += -I$(DRV_ROOT)/drv_if

# UART module include path.
UARTDIR =  $(DRV_ROOT)/$(DRIVER_DIR)/uart
CC_INC += -I$(UARTDIR)
endif

ifdef DRV_INC_DIR
CC_INC += -I$(DRV_INC_DIR)
endif

ifndef DRV_INC_DIR
DRV_INC_DIR := .
endif

#############################################################################
# Local definitions
#############################################################################
# LOCAL_DEFINES only works at main program directory.

LOCAL_DEFINES +=


PREMAIN = premain
LOCAL_LINK = local_link

export ADD_TO_SYMLINK_FILES = aud_cmd.h   dhrystone_cmd.h  mw_cli_list.h  osd_cmd.h  psipsr_cmd.h  tve_cmd.h \
				ir_kp.h          nptv_cmd.h     pmx_cmd.h  rtc_cmd.h playmgr_cmd.c smc_emu.h nptv_scpip_gfx_cmd.c


ifneq "$(ONLY_CLI)" "y"
ifdef DRIVER_DEMO
ifeq "$(SYSBUILD_DEMO)" "true"
include $(DRV_ROOT)/driver.mak
else
include $(DRV_ROOT)/main.mak
endif
else

ifneq ($(KERNELRELEASE),)
ifeq "$(STANDALONE_CLI_MODULE)" "true"
# Make as a standalone module.
DEFINES         += -DSTANDALONE_CLI
obj-m           := driver_cli.o
driver_cli-y    := $(patsubst %.c,%.o,$(SRC))
SRC             :=
LIB_CONTENT     := $(THIS_ROOT)/driver_ko_cli.content
endif
endif

include $(DRV_ROOT)/driver.mak
endif

#include $(DRV_ROOT)/brdcst_if.mak
endif

premain:
	echo "" > $(DRV_INC_DIR)/cli_list.h.tmp
	grep CLI_MAIN_COMMAND_ITEM $(SRC) | awk -F'(' '{ print $$2 }' | awk -F')' '{ print "extern CLI_EXEC_T *CLI_"$$1"_function(void);" }' >> $(DRV_INC_DIR)/cli_list.h.tmp
	echo "static const CLI_GET_CMD_TBL_FUNC _pfCliGetTbl[] = {" >> $(DRV_INC_DIR)/cli_list.h.tmp
	grep CLI_MAIN_COMMAND_ITEM $(SRC) | awk -F'(' '{ print $$2 }' | awk -F')' '{ print "CLI_"$$1"_function," }' >> $(DRV_INC_DIR)/cli_list.h.tmp
	echo "};" >> $(DRV_INC_DIR)/cli_list.h.tmp
	@if [ ! -e $(DRV_INC_DIR)/cli_list.h ]; then \
		cp $(DRV_INC_DIR)/cli_list.h.tmp $(DRV_INC_DIR)/cli_list.h; \
	fi
	@if [ "`diff -q $(DRV_INC_DIR)/cli_list.h.tmp $(DRV_INC_DIR)/cli_list.h`" != "" ]; then \
		cp $(DRV_INC_DIR)/cli_list.h.tmp $(DRV_INC_DIR)/cli_list.h; \
	fi
	@rm -rf $(DRV_INC_DIR)/cli_list.h.tmp

clean_premain:
	@rm -rf $(DRV_INC_DIR)/cli_list.h.tmp $(DRV_INC_DIR)/cli_list.h
	
local_link:
#	@echo $(shell ls ${DIAG_DIR}/nptv5399/)
	@for i in $(shell ls ${DIAG_DIR}/nptv5399/); do \
		if [ ! -e $$i -a ! -h $$i ]; \
		then \
			($(call if_file_notexist_w_sym_fct, ${DIAG_DIR}/nptv5399/$$i, $(DRV_ROOT)/diag/$$i); exit 0) ; \
		fi \
	done

clean_brdcst_if:
	find $(DRV_BUILD_ROOT)/$(DRV_BUILD_BASE)/driver/build/ -name brdcst_if | xargs rm -rf
