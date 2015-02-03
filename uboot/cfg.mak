

#### HDMI 2.0 HDCP BIN ####
ifeq "$(HDMI_2_0_HDCP_BIN_SUPPORT)" "true"
DEFINES += -DCC_HDMI_2_0_HDCP_BIN
endif

ifeq "$(LGE_PROTO_PCBA)" "true"
DEFINES += -DCC_LGE_PROTO_PCBA
endif

#support one pkg cover different dram size
ifeq "$(ONE_PKG_MULTI_DRAM_SIZE_SUPPORT)" "true"
DEFINES += -DCC_ONE_PKG_MULTI_DRAM_SIZE_SUPPORT
endif

##### Video screenshot #####
ifeq "$(SUPPORT_VSS_FLINGO)" "true"
    DEFINES += -DCC_SUPPORT_VSS
    export CC_SUPPORT_VSS = true
endif

DEFINES += -DCC_WIDEVINE_L1_SUPPORT
DEFINES += -DCC_FBM_SECURE_FEEDER

#
# Seamless Support Option: DivX Cert, Netflix, Dash, Live_stream, YouTube HTML5 are seamless applications and need to enable CC_ENABLE_SEAMLESS_FOR_2D
#

ifeq "$(DRAM_DEBUG)" "true"
DEFINES += -DDRAM_DEBUG
endif

ifeq "$(ENABLE_PART_VER_CHK)" "true"
DEFINES += -DCC_DEF_PART_VER_CHK_FUNC
endif

ifeq "$(SNAP_SHOT)" "true" 
DEFINES += -DCC_LG_SNAP_SHOT
endif

####WIFI WOW development ####
ifeq "$(WIFI_WOW_SUPPORT)" "true"
DEFINES += -DCC_WIFI_WOWLAN_SUPPORT
ifeq "$(WIFI_DONGLE)" "MT7603U"
DEFINES += -DCC_WIFI_WOWLAN_LOW_ACTIVE
endif
endif

ifeq "$(LINUX_AUTO_TEST)" "true"
DEFINES += -DCC_MTK_AUTO_TEST
endif

ifeq "$(DYNAMIC_FBM_MAP)" "true"
DEFINES += -DCC_FBM_MAPPING_ONE_BY_ONE
DEFINES += -DCC_VDEC_FBM_DYNAMIC_MAPPING
DEFINES += -DCC_JPEG_FBM_DYNAMIC_MAPPING
DEFINES += -DCC_PQ_TOOL_8M
endif

ifeq "$(SEAMLESS_SUPPORT)" "true"
DEFINES += -DCC_ENABLE_SEAMLESS_FOR_2D
endif

ifeq "$(SUPPORT_TTY_USB)" "true"
DEFINES += -DCC_UBOOT_ADD_USB_LOG_MODE
DEFINES += -DCC_UBOOT_ADD_USB_LOG_MODE_ADDR=0xFFF
endif

ifeq "$(USIG)" "true"
DEFINES += -DCC_LOADER_VERIFY_PKG
endif

ifeq "$(PKG_ENCRYPT_MODE)" "AES"
DEFINES += -DDLM_USE_AES_MODE
endif

ifeq "$(NETFLIX_3D)" "true"
DEFINES += -DCC_ENABLE_SEAMLESS_FOR_MVC
endif

# Loader logo
ifeq "$(LOADER_LOGO_SUPPORT)" "true"
    DEFINES += -DCC_LOADER_LOGO_SUPPORT
    # Loader Logo improvement
    #ifeq "$(SUPPORT_4K2K)" "false"
        DEFINES += -DCC_SUPPORT_BL_DLYTIME_CUT
   # endif
    ifeq "$(LOADER_LOGO_LONG_TIME)" "true"
        DEFINES += -DCC_LOADER_LOGO_LONG_TIME
    endif
endif

#
# Capture Logo
#
ifeq "$(CAPTURE_LOGO_SUPPORT)" "true"
DEFINES += -DCC_BOOT_VDEC_LOGO
endif

ifeq "$(DRAM_256M)" "true"
DEFINES += -DCC_DRAM_256M
endif

ifeq "$(MHL_SUPPORT)" "true"
DEFINES += -DSYS_MHL_SUPPORT
DEFINES += -DCC_MHL_3D_SUPPORT
endif

ifeq "$(TRUSTZONE)" "true"
DEFINES += -DCC_TRUSTZONE_SUPPORT
    ifeq "$(SVP_SUPPORT)" "true"
        DEFINES += -DCC_SVP_SUPPORT
        DEFINES += -DCC_FBM_SECURE_FEEDER
    endif
     ifeq "$(TVP_SUPPORT)" "true"
        DEFINES += -DCC_TVP_SUPPORT
   endif
    ifeq "$(CI_TZ_SUPPORT)" "true"
        DEFINES += -DCC_CI_PLUS_TZ_SUPPORT
    endif

endif


ifeq "$(LOADER_ENABLE_CONSOLE_LOCK)" "true"
    DEFINES += -DCC_CONSOLE_LOCK
endif

ifeq "$(LOADER_ENABLE_SECURE_STORAGE)" "true"
    DEFINES += -DCC_SECURESTORAGE_SUPPORT
endif

ifeq "$(SYS_MHP_SUPPORT)" "true"
    DEFINES += -DCC_MHP_SUPPORT
endif

# app_driver_share table
ifeq "$(APP_DRV_TABLE_SHARE)" "true"
DEFINES += -DAPP_DRV_TABLE_SHARE
endif

ifeq "$(SYM_SUPPORT)" "true"
NO_SYM_DEBUG := FALSE
else
NO_SYM_DEBUG := TRUE
endif
ifeq "$(BUILD_CFG)" "debug"
NO_SYM_DEBUG := FALSE
endif

ifeq "$(FW_UPG_SUPPORT)" "LGMTK"
#disable these defines. If you need mtk usb upgrade, enable them
#DEFINES += -DLOADER_USB_UPGRADE
#DEFINES += -DAUTO_USB_UPGRADE_ENABLE=1
#DEFINES += -DDLM_USE_AES_MODE
#DEFINES += -DDLM_TMTK_LG
endif

ifeq "$(FW_UPG_SUPPORT)" "mtk"
DEFINES += -DLOADER_USB_UPGRADE
endif

# Just for release version
ifeq "$(BUILD_CFG)" "rel"
DEFINES += -DCC_UBOOT_QUIET_BOOT
DEFINES += -DNDEBUG
endif

#export USB_UPGRADE = nand
#export FAST_SUPPORT = false
DEFINES += -DAUTO_USB_UPGRADE_ENABLE=1
#DEFINES += -DCC_SHOW_OAD_MODEL_VERSION
DEFINES += -DOSAI_FOR_NUCLEUS

DEFINES += -DCC_VUDU_SUPPORT

ifeq "$(TMS_SUPPORT)" "true"
DEFINES += -DTIME_MEASUREMENT
endif

ifeq "$(DUAL_TUNER)" "true"
DEFINES += -DCC_DUAL_TUNER_SUPPORT
DEFINES += -DCC_DUAL_TUNER_DUMMY
DEFINES += -DCC_EXTERNAL_DEMOD
DEFINES += -DCC_SECOND_B2R_SUPPORT
endif

ifeq "$(SUSPEND_TO_DRAM)" "true"
DEFINES += -DCC_SUPPORT_STR
DEFINES += -DCC_SUPPORT_STR_CORE_OFF
DEFINES += -DCC_SUPPORT_STR_TEMP
endif

ifeq "$(SUSPEND_TO_DRAM_DEBUG)" "true"
DEFINES += -DCC_SUPPORT_STR_DEBUG
endif

ifneq "$(SDM_SUPPORT)" "false"
DEFINES += -DLINUX_NAND_USE_SDM
endif

ifeq "$(MTD_ENCRYPT_SUPPORT)" "true"
DEFINES += -DCC_MTD_ENCRYPT_SUPPORT
endif

ifeq "$(NETFLIX_CMPB)" "true"
DEFINES += -DCC_NETFLIX_CMPB_SUPPORT
DEFINES += -DNETFLIX_SUPPORT
DEFINES += -DCC_NETFLIX_CE3_SUPPORT
endif

ifeq "$(MULTI_MM_SUPPORT)" "true"
DEFINES += -DCC_FBM_TWO_FBP
DEFINES += -DCC_SKYPE_FINE_INSTANCE
DEFINES += -DCC_SECOND_B2R_SUPPORT
endif

DEFINES += -DLINUX_TURNKEY_SOLUTION
export LINUX_SOLUTION = true

DEFINES += -DKEY_FROM_DFB
export KEY_FROM_DFB = y

ifeq "$(AV_RECORD_LIB)" "true"
DEFINES += -DCC_SUPPORT_RECORD_AV
endif

ifeq "$(MUXER_SUPPORT)" "true"
##### Video encoder #####
DEFINES += -DCC_SUPPORT_VENC
export CC_SUPPORT_VENC = true

##### Muxer #####
#DEFINES += -DCC_SUPPORT_MUXER
#export CC_SUPPORT_MUXER = true
endif

DEFINES += -DCC_SUPPORT_PIPELINE
export CC_SUPPORT_PIPELINE = true

##### Network #####
ifneq ($(strip $(findstring -DDLNA_SUPPORT, $(DEFINES))),)
DEFINES += -DCC_DLNA_SUPPORT
#DEFINES += -DDMX_SUPPORT_NDDRM
#DEFINES += -DDMX_SUPPORT_DTCP
endif

ifeq "$(ENABLE_SKB)" "true"
DEFINES += -DCC_ENABLE_SKB
endif

ifeq "$(ENABLE_HCI)" "true"
DEFINES += -DCC_ENABLE_HCI
endif

#NAND Flash size setting
ifeq "$(NAND128M)" "true"
DEFINES += -DCC_USE_128MB_NAND
endif

#For ROM-2-NAND boot
#export BOOT=nand
#export LARGE_NAND=y
#export BOOT_TYPE=ROM2NAND

############################
# Flash Configuration #
############################
ifeq "$(BOOT_TYPE)" "ROM2NAND"
DEFINES += -DCC_NAND_ENABLE
endif

ifeq "$(BOOT_TYPE)" "ROM2EMMC"
DEFINES += -DCC_MSDC_ENABLE
endif

#PANEL
ifndef DEFAULT_PANEL_SELECT
DEFINES += -DDEFAULT_PANEL_SELECT=PANEL_CMO_19_A1
else
DEFINES += -DDEFAULT_PANEL_SELECT=$(DEFAULT_PANEL_SELECT)
endif

ifeq "$(DEFAULT_PANEL_SELECT)" "PANEL_DEFAULT"
DEFINES += -DCC_AUTOTEST
endif

# H.264 - drv option
#ifeq "$(SYS_H264_SUPPORT)" "true"
DEFINES += -DCC_H264_SUPPORT
export CC_H264_SUPPORT = y
#endif

# IPv6
ifeq "$(IPV6_SUPPORT)" "true"
    DEFINES += -DCC_IPV6_SUPPORT
endif

##### Multimedia #####
ifeq "$(MMP_SUPPORT)" "basic"
    DEFINES += -DJPG_ENABLE
    # This ENABLE_MULTIMEDIA is used by mw_if/Makefile
    DEFINES += -DENABLE_MULTIMEDIA
    export ENABLE_MULTIMEDIA = true
else 
    ifeq "$(MMP_SUPPORT)" "full"
        DEFINES += -DJPG_ENABLE
        DEFINES += -DCC_H264_SUPPORT
        ifeq "$(DIVX_PLUS_SUPPORT)" "true"
            DEFINES += -DDIVX_PLUS_CER
        endif
        DEFINES += -DCC_B2R_CHAPTER_CHG
        ifeq "$(TSHFT_CORE_LIB)" "true"
            export TIME_SHIFT_SUPPORT = true
            DEFINES += -DTIME_SHIFT_SUPPORT
            DEFINES += -DSYS_RECORD_SUPPORT
        endif
        DEFINES += -DCC_AUD_MMP_FULL_SUPPORT
        # This ENABLE_MULTIMEDIA is used by mw_if/Makefile
        DEFINES += -DENABLE_MULTIMEDIA
        export ENABLE_MULTIMEDIA = true
    endif
endif

# UART
DEFINES += -DCC_SET_UART_TRANSPARENT
DEFINES += -DIRRC_LOG

DEFINES += -DTUNER_SUPPORT_MULTI_SYS_TV
DEFINES += -DSUPPORT_MULTI_SYSTEM
DEFINES += -DTUNER_SUPPORT_SCART_OUT
DEFINES += -DCC_DUAL_IMAGE_FLASH
DEFINES += -DCC_OSD_USE_FBM
DEFINES += -DNO_POD_INIT

#FRC Cadenace Detection
ifeq "$(FRC_SUPPORT)" "force120_web"
    DEFINES += -DCC_FRC_SUPPORT_FORCE120_WEB
else 
    ifeq "$(FRC_SUPPORT)" "force100"
        DEFINES += -DCC_FRC_SUPPORT_FORCE100
    else 
        ifeq "$(FRC_SUPPORT)" "f48"
            DEFINES += -DCC_FRC_SUPPORT_F48
        endif
    endif
endif

# [Driver debug use] capture dump support for ncstools
ifeq "$(CDC_DUMP_SUPPORT)" "true"
DEFINES += -DCC_CDC_SUPPORT
endif

#Bypass setting
ifneq "$(AV_BYPASS)" "1"
    DEFINES += -DCC_SUPPORT_TVE_CTRL_BY_MW
    DEFINES += -DCC_SUPPORT_TVE
endif
#for 5882 monitor bypass, use monitor out
ifeq "$(AV_BYPASS)" "2"
    DEFINES += -DCC_SUPPORT_MONITOR_OUT_TVE
endif

#
#PIP/POP. target use this define.
#
ifeq "$(SUPPORT_PIP)" "true"
DEFINES += -DMW_PIP_POP_SUPPORT
endif

#chip spec
DEFINES += -DCHIP_SPEC_SUPPORT

##### xml ####
DEFINES += -DXML_SUPPORT

# use MT8295 or MT5135 define external CI
ifeq "$(SYS_CI_SUPPORT)" "true"
    DEFINES += -DDVBT_CI_ENABLE
    ifeq "$(EXTERNAL_CI_SUPPORT)" "true"
        DEFINES += -DCC_EXTERNAL_CI
    else
        DEFINES += -DCC_INTERNAL_CI
    endif
    ##### CI ENGINE #####
    ifeq "$(CI_ENGINE)" "oceanblue_ci"
        ifeq "$(CI_PLUS_SUPPORT)" "true"
            DEFINES += -DCI_PLUS_SUPPORT
            ifeq "$(CI_PLUS_V13_SUPPORT)" "true"
                ifeq "$(TSHFT_CORE_LIB)" "true"
                    DEFINES += -DSUPPORT_CIPLUS_RECORD
                endif
                DEFINES += -DCI_PLUS_V13_SUPPORT
            endif
        endif
    endif
endif

#ifneq "$FLASH_PQ" "false"
#DEFINES += -DSUPPORT_FLASH_PQ
#endif

#ifneq "$FLASH_AQ" "false"
#DEFINES += -DSUPPORT_FLASH_AQ
#endif

ifeq "$(FLASH_ADSP_BIN)" "true"
DEFINES += -DADSP_BIN_SUPPORT
endif

##### FLIP/MIRROR #####
ifeq "$(SYS_FLIP_SUPPORT)" "true"
DEFINES += -DCC_FLIP_MIRROR_SUPPORT
#DEFINES += -DCC_FLIP_MIRROR_FROM_EEPROM
#DEFINES += -DCC_FLIP_ENABLE
endif

ifeq "$(SYS_MIRROR_SUPPORT)" "true"
DEFINES += -DCC_FLIP_MIRROR_SUPPORT
#DEFINES += -DCC_FLIP_MIRROR_FROM_EEPROM
#DEFINES += -DCC_MIRROR_ENABLE
endif

#
# 3D Support
#
ifeq "$(3D_SUPPORT)" "true"
    DEFINES += -DCC_SCPOS_3DTV_SUPPORT
    DEFINES += -DCC_REALD_3D_SUPPORT
    DEFINES += -DCC_B2R_3D_SUPPROT
    DEFINES += -DCC_3D_SUPPORT
    ifeq "$(3D_MM_DS_SUPPORT)" "true"
        DEFINES += -DCC_MVC_SUPPORT
        DEFINES += -DCC_KR_3DDTV
        DEFINES += -DCC_FBM_TWO_FBP
        #DEFINES += -DCC_FBM_INT_TWO_FBP
        DEFINES += -DCC_FBM_TWO_FBP_SHARED_WITH_DFB
        DEFINES += -DCC_3D_MM_DS_SUPPORT
        #DEFINES += -DCC_FBM_FULL_3D_SUPPORT
    endif

	ifeq "$(3D_SUPPORT_SG_PANEL)" "true"
		DEFINES += -DSUPPORT_3D_EXT_COMP_TRL=1
	else
		DEFINES += -DSUPPORT_3D_EXT_COMP_TRL=0
	endif
    ifeq "$(3D_PHOTO_ON_VDP)" "true"
        DEFINES += -DIS_SUPPORT_3D_PHOTO
        DEFINES += -DCC_FBM_3D_OSD
        DEFINES += -DCC_DFB_SUPPORT_VDP_LAYER
    endif
endif

ifeq "$(SYS_UK_FREEVIEWHD_SUPPORT)" "true"
    #AD Pan/Fade
    DEFINES += -DCC_AUD_PANFADE_SUPPORT
endif

ifeq "$(SYS_DVBS_SUPPORT)" "true"
    DEFINES += -DSYS_DVBS_SUPPORT
    
    ##### DVBS Default DB support
    ifeq "$(DEFAULT_DB_SUPPORT)" "true"
        DEFINES += -DDEFAULT_DB_SUPPORT
    endif
endif
################ Enable Tuner Fast Boot ##########
ifeq "$(findstring DVBTC, $(BRDCST_IF))" "DVBTC"
DEFINES += -DCC_DEMOD_FASTACQ
#DEFINES += -DCC_ATD_SUPPORT_FASTACQ
endif
##################################################
ifeq "$(SYS_MHP_SUPPORT)" "true"
DEFINES += -DCC_FBM_TWO_FBP
DEFINES += -DCC_FBM_TWO_FBP_SHARED_WITH_DFB
endif

ifeq "$(SUPPORT_4K2K)" "true"
DEFINES += -DCC_SUPPORT_4K2K
DEFINES += -DCC_FBM_SUPPORT_4K2K
ifeq "$(MM_DRV_RM)" "true"
DEFINES += -DCC_DUAL_CORE_DEC
DEFINES += -DCC_VDEC_RM_SUPPORT
DEFINES += -DCC_SUPPORT_VDEC_PREPARSE

endif
endif

DEFINES += -DCC_B2R_RES_SUPPORT

ifneq "$(AV_BYPASS)" "1"
ifeq "$(MM_DRV_RM)" "true"
DEFINES += -DCC_B2R_SUPPORT_BYPASS
endif
endif

ifeq "$(SUPPORT_TCON_UPDATE)" "true"
DEFINES += -DCC_SUPPORT_TCON_FW_UPDATE
endif

###############################################
# Enable HDMI Fast Boot.                      #
# Note: It can't be applied in all PCBs       #
###############################################
DEFINES += -DCC_SUPPORT_HDMI_FBOOTING

###############################################
#               cec arc ctrl                  #
###############################################
DEFINES += -DCC_HDMI_ARC_SPDIF_CTRL
DEFINES += -DCECNFY5VSTATUS

#DMX
DEFINES += -DDMX_NT_UCODE_DISABLE
DEFINES += -DCC_FBM_SUPPORT_DMXPID
ifeq "$(EDB_SUPPORT)" "true"
ifeq "$(EPG_FILTER_EIT_SECTION_IN_DMX)" "true"
DEFINES += -DCC_EIT_SECTION_FILTER
endif
endif
##### Video encoder #####
#DEFINES += -DCC_SUPPORT_VENC
#export CC_SUPPORT_VENC = true

#DEFINES += -DCC_SCHED_RR_POLICY

ifeq "$(BASIC_IN_ROOTFS)" "true"
DEFINES += -DCC_DISABLE_BASIC_PART
endif

DEFINES += -DCONFIG_OSD_COMPRESSION
# DEFINES += -DCONFIG_OSD_PLANE1_COMPRESSION
DEFINES += -DCONFIG_OSD_PLANE2_COMPRESSION
# DEFINES += -DCONFIG_OSD_PLANE3_COMPRESSION
# DEFINES += -DCONFIG_OSD_PLANE4_COMPRESSION
# DEFINES += -DCONFIG_OSD_PLANE5_COMPRESSION
ifeq "$(GFX_MMU_SUPPORT)" "true"
DEFINES += -DCC_GFX_MMU
endif
ifeq "$(3RD_RO)" "true"
DEFINES += -DCC_3RD_RO
endif

ifeq "$(LINUX_GPU_SUPPORT)" "true"
DEFINES += -DCC_LINUX_GPU_SUPPORT
endif
ifeq "$(HOTKEY_WAKEUP_SUPPORT)" "true"
DEFINES += -DCC_HOTKEY_WAKEUP_SUPPORT
endif

# For improve change channel performance: B2R slow sync mode when change channel.
DEFINES += -DCC_VDP_DTV_CHANGE_CHANNEL_SLOW_SYNC_ENABLE

ifeq "$(UPGRADE_VERSION_CTRL_FLOW_BY_IMG)" "true"
DEFINES += -DCC_UPGRADE_VERSION_CTRL_FLOW_BY_IMG
endif

# For sony special bitstream, progressive force to interlace.
#DEFINES += -DCC_FORCE_ITIMING_FRM251080

# Disable log 20s when boot up
ifeq "$(KERNEL_NO_LOG)" "true"
DEFINES += -DCC_KERNEL_NO_LOG
endif

ifeq "$(SECURE_BOOT_V2)" "true"
DEFINES += -DCC_SECURE_BOOT_V2
endif

ifeq "$(BOOT_NO_LZHS)" "true"
DEFINES += -DCC_BOOT_NO_LZHS
endif

ifeq "$(SECURE_BOOT_SCRAMBLE)" "true"
DEFINES += -DCC_SECURE_BOOT_SCRAMBLE
endif
ifeq "$(CLI_LOG)" "true"
DEFINES += -DCC_SHOW_MORE_CLI_LOG
endif


