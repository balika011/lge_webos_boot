#
# (C) Copyright 2000-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

#########################################################################

ifeq ($(CURDIR),$(SRCTREE))
dir :=
else
MYCURDIR = $(subst board,$(UBOOT_DIR)/board,$(CURDIR))
dir := $(subst $(SRCTREE)/,,$(MYCURDIR))
endif

ifneq ($(OBJTREE),$(SRCTREE))
# Create object files for SPL in a separate directory
ifeq ($(CONFIG_SPL_BUILD),y)
obj := $(if $(dir),$(SPLTREE)/$(dir)/,$(SPLTREE)/)
else
obj := $(if $(dir),$(OBJTREE)/$(dir)/,$(OBJTREE)/)
endif
src := $(if $(dir),$(SRCTREE)/$(dir)/,$(SRCTREE)/)

$(shell mkdir -p $(obj))
else
# Create object files for SPL in a separate directory
ifeq ($(CONFIG_SPL_BUILD),y)
obj := $(if $(dir),$(SPLTREE)/$(dir)/,$(SPLTREE)/)

$(shell mkdir -p $(obj))
else
obj :=
endif
src :=
endif

# clean the slate ...
PLATFORM_RELFLAGS =
PLATFORM_CPPFLAGS =
PLATFORM_LDFLAGS =

#########################################################################

HOSTCFLAGS	= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer \
		  $(HOSTCPPFLAGS) $(DEFINES)
HOSTSTRIP	= strip

#
# Mac OS X / Darwin's C preprocessor is Apple specific.  It
# generates numerous errors and warnings.  We want to bypass it
# and use GNU C's cpp.	To do this we pass the -traditional-cpp
# option to the compiler.  Note that the -traditional-cpp flag
# DOES NOT have the same semantics as GNU C's flag, all it does
# is invoke the GNU preprocessor in stock ANSI/ISO C fashion.
#
# Apple's linker is similar, thanks to the new 2 stage linking
# multiple symbol definitions are treated as errors, hence the
# -multiply_defined suppress option to turn off this error.
#

ifeq ($(HOSTOS),darwin)
# get major and minor product version (e.g. '10' and '6' for Snow Leopard)
DARWIN_MAJOR_VERSION	= $(shell sw_vers -productVersion | cut -f 1 -d '.')
DARWIN_MINOR_VERSION	= $(shell sw_vers -productVersion | cut -f 2 -d '.')

os_x_before	= $(shell if [ $(DARWIN_MAJOR_VERSION) -le $(1) -a \
	$(DARWIN_MINOR_VERSION) -le $(2) ] ; then echo "$(3)"; else echo "$(4)"; fi ;)

# Snow Leopards build environment has no longer restrictions as described above
HOSTCC		 = $(call os_x_before, 10, 5, "cc", "gcc")
HOSTCFLAGS	+= $(call os_x_before, 10, 4, "-traditional-cpp")
HOSTLDFLAGS	+= $(call os_x_before, 10, 5, "-multiply_defined suppress")
else
HOSTCC		= gcc
endif

ifeq ($(HOSTOS),cygwin)
HOSTCFLAGS	+= -ansi
endif

# We build some files with extra pedantic flags to try to minimize things
# that won't build on some weird host compiler -- though there are lots of
# exceptions for files that aren't complaint.

HOSTCFLAGS_NOPED = $(filter-out -pedantic,$(HOSTCFLAGS))
HOSTCFLAGS	+= -pedantic

#########################################################################
#
# Option checker (courtesy linux kernel) to ensure
# only supported compiler options are used
#
CC_OPTIONS_CACHE_FILE := $(OBJTREE)/include/generated/cc_options.mk

$(if $(wildcard $(CC_OPTIONS_CACHE_FILE)),,\
	$(shell mkdir -p $(dir $(CC_OPTIONS_CACHE_FILE))))

-include $(CC_OPTIONS_CACHE_FILE)

cc-option-sys = $(shell if $(CC) $(CFLAGS) $(1) -S -o /dev/null -xc /dev/null \
		> /dev/null 2>&1; then \
		echo 'CC_OPTIONS += $(strip $1)' >> $(CC_OPTIONS_CACHE_FILE); \
		echo "$(1)"; fi)

ifeq ($(CONFIG_CC_OPT_CACHE_DISABLE),y)
cc-option = $(strip $(if $(call cc-option-sys,$1),$1,$2))
else
cc-option = $(strip $(if $(findstring $1,$(CC_OPTIONS)),$1,\
		$(if $(call cc-option-sys,$1),$1,$2)))
endif

#
# Include the make variables (CC, etc...)
#
AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
CPP	= $(CC) -E
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
LDR	= $(CROSS_COMPILE)ldr
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)RANLIB
DTC	= dtc

#########################################################################

# Load generated board configuration
sinclude $(OBJTREE)/include/autoconf.mk
sinclude $(OBJTREE)/include/config.mk

# Some architecture config.mk files need to know what CPUDIR is set to,
# so calculate CPUDIR before including ARCH/SOC/CPU config.mk files.
# Check if arch/$ARCH/cpu/$CPU exists, otherwise assume arch/$ARCH/cpu contains
# CPU-specific code.
CPUDIR=arch/$(ARCH)/cpu/$(CPU)
ifneq ($(SRCTREE)/$(CPUDIR),$(wildcard $(SRCTREE)/$(CPUDIR)))
CPUDIR=arch/$(ARCH)/cpu
endif

sinclude $(TOPDIR)/arch/$(ARCH)/config.mk	# include architecture dependend rules
sinclude $(TOPDIR)/$(CPUDIR)/config.mk		# include  CPU	specific rules

ifdef	SOC
sinclude $(TOPDIR)/$(CPUDIR)/$(SOC)/config.mk	# include  SoC	specific rules
endif
ifdef	VENDOR
BOARDDIR = $(VENDOR)/$(BOARD)
else
BOARDDIR = $(BOARD)
endif
ifdef	BOARD
sinclude $(TOPDIR)/board/$(BOARDDIR)/config.mk	# include board specific rules
endif

#########################################################################

# We don't actually use $(ARFLAGS) anywhere anymore, so catch people
# who are porting old code to latest mainline but not updating $(AR).
ARFLAGS = $(error update your Makefile to use cmd_link_o_target and not AR)
RELFLAGS= $(PLATFORM_RELFLAGS)
DBGFLAGS= -g # -DDEBUG
OPTFLAGS= -Os #-fomit-frame-pointer

OBJCFLAGS += --gap-fill=0xff

gccincdir := $(shell $(CC) -print-file-name=include)

CPPFLAGS := $(DBGFLAGS) $(OPTFLAGS) $(RELFLAGS)		\
	-D__KERNEL__

# Enable garbage collection of un-used sections for SPL
ifeq ($(CONFIG_SPL_BUILD),y)
CPPFLAGS += -ffunction-sections -fdata-sections
LDFLAGS_FINAL += --gc-sections
endif

ifneq ($(CONFIG_SYS_TEXT_BASE),)
CPPFLAGS += -DCONFIG_SYS_TEXT_BASE=$(CONFIG_SYS_TEXT_BASE)
endif

ifneq ($(CONFIG_SPL_TEXT_BASE),)
CPPFLAGS += -DCONFIG_SPL_TEXT_BASE=$(CONFIG_SPL_TEXT_BASE)
endif

ifeq ($(CONFIG_SPL_BUILD),y)
CPPFLAGS += -DCONFIG_SPL_BUILD
endif

ifeq ($(CONFIG_THREAD),true)
CPPFLAGS += -DCONFIG_USE_IRQ
CPPFLAGS += -DCONFIG_MULTICORES_PLATFORM
endif
ifneq ($(RESET_VECTOR_ADDRESS),)
CPPFLAGS += -DRESET_VECTOR_ADDRESS=$(RESET_VECTOR_ADDRESS)
endif

ifneq ($(OBJTREE),$(SRCTREE))
CPPFLAGS += -I$(OBJTREE)/include2 -I$(OBJTREE)/include
endif

CPPFLAGS += -I$(TOPDIR)/include
CPPFLAGS += -I$(DRV_INC_DIR)
CPPFLAGS += -I$(DRVLIB_DRVCUST_DIR)
CPPFLAGS += -fno-builtin -ffreestanding -nostdinc	\
	-isystem $(gccincdir) -pipe $(PLATFORM_CPPFLAGS)

ifdef BUILD_TAG
CFLAGS := $(CPPFLAGS) -w -Wstrict-prototypes \
	-DBUILD_TAG='"$(BUILD_TAG)"'
else
CFLAGS := $(CPPFLAGS) -w -Wstrict-prototypes
endif

CFLAGS_SSP := $(call cc-option,-fno-stack-protector)
CFLAGS += $(CFLAGS_SSP)
# Some toolchains enable security related warning flags by default,
# but they don't make much sense in the u-boot world, so disable them.
CFLAGS_WARN := $(call cc-option,-Wno-format-nonliteral) \
	       $(call cc-option,-Wno-format-security)
CFLAGS += $(CFLAGS_WARN)
CFLAGS += -DCONFIG_USE_IRQ
# $(CPPFLAGS) sets -g, which causes gcc to pass a suitable -g<format>
# option to the assembler.
AFLAGS_DEBUG :=

# turn jbsr into jsr for m68k
ifeq ($(ARCH),m68k)
ifeq ($(findstring 3.4,$(shell $(CC) --version)),3.4)
AFLAGS_DEBUG := -Wa,-gstabs,-S
endif
endif

AFLAGS := $(AFLAGS_DEBUG) -D__ASSEMBLY__ $(CPPFLAGS)

LDFLAGS += $(PLATFORM_LDFLAGS)
LDFLAGS_FINAL += -Bstatic

LDFLAGS_u-boot += -T $(obj)u-boot.lds $(LDFLAGS_FINAL)
ifneq ($(CONFIG_SYS_TEXT_BASE),)
LDFLAGS_u-boot += -Ttext $(CONFIG_SYS_TEXT_BASE)
endif

LDFLAGS_u-boot-spl += -T $(obj)u-boot-spl.lds $(LDFLAGS_FINAL)
ifneq ($(CONFIG_SPL_TEXT_BASE),)
LDFLAGS_u-boot-spl += -Ttext $(CONFIG_SPL_TEXT_BASE)
endif

# Location of a usable BFD library, where we define "usable" as
# "built for ${HOST}, supports ${TARGET}".  Sensible values are
# - When cross-compiling: the root of the cross-environment
# - Linux/ppc (native): /usr
# - NetBSD/ppc (native): you lose ... (must extract these from the
#   binutils build directory, plus the native and U-Boot include
#   files don't like each other)
#
# So far, this is used only by tools/gdb/Makefile.

ifeq ($(HOSTOS),darwin)
BFD_ROOT_DIR =		/usr/local/tools
else
ifeq ($(HOSTARCH),$(ARCH))
# native
BFD_ROOT_DIR =		/usr
else
#BFD_ROOT_DIR =		/LinuxPPC/CDK		# Linux/i386
#BFD_ROOT_DIR =		/usr/pkg/cross		# NetBSD/i386
BFD_ROOT_DIR =		/opt/powerpc
endif
endif

ifeq ($(PCI_CLOCK),PCI_66M)
CFLAGS := $(CFLAGS) -DPCI_66M
endif

ifeq ($(TARGET_IC),mt5365)
CFLAGS := $(CFLAGS) -DCC_MT5365
endif

ifeq ($(TARGET_IC),mt5392b)
CFLAGS := $(CFLAGS) -DCC_MT5392B
endif

ifeq ($(TARGET_IC),mt5363)
CFLAGS := $(CFLAGS) -DCC_MT5363
endif

ifeq ($(TARGET_IC),mt5395)
CFLAGS := $(CFLAGS) -DCC_MT5395
endif

ifeq "$(TOOL_CHAIN)" "4.5.1"
CFLAGS += "-fno-unwind-tables"
endif

ifeq "$(TOOL_CHAIN)" "4.8.2"
CFLAGS += "-fno-unwind-tables"
endif

ifeq "$(ENABLE_CA9)" "true"
CC_FLAG += -march=armv7-a -mtune=cortex-a9
endif

ifeq ($(TARGET_IC),mt5396)
CFLAGS := $(CFLAGS) -DCC_MT5396
endif

ifeq ($(TARGET_IC),mt5368)
CFLAGS := $(CFLAGS) -DCC_MT5368
endif

ifeq ($(TARGET_IC),mt5389)
CFLAGS := $(CFLAGS) -DCC_MT5389
endif

ifeq ($(TARGET_IC),mt5398)
CFLAGS := $(CFLAGS) -DCC_MT5398
endif

ifeq ($(TARGET_IC),mt5880)
CFLAGS := $(CFLAGS) -DCC_MT5880
endif

ifeq ($(TARGET_IC),mt5881)
CFLAGS := $(CFLAGS) -DCC_MT5881
endif

ifeq ($(TARGET_IC),mt5399)
CFLAGS := $(CFLAGS) -DCC_MT5399
endif

# TODO: remove CC_MT5399
ifeq ($(TARGET_IC),mt5890)
CFLAGS := $(CFLAGS) -DCC_MT5399 -DCC_MT5890
endif

ifeq ($(TARGET_IC),mt5891)
CFLAGS := $(CFLAGS) -DCC_MT5399 -DCC_MT5890 -DCC_MT5891
endif

ifeq ($(TARGET_IC),mt5882)
CFLAGS := $(CFLAGS) -DCC_MT5882
CFLAGS += -I$(COMMON_INC) -I$(OSAI_INC)
endif

ifeq ($(TARGET_IC),mt5883)
CFLAGS := $(CFLAGS) -DCC_MT5883
endif

CFLAGS += $(DEFINES)
AFLAGS += $(DEFINES)

CFLAGS += -pipe
CPPFLAGS += -pipe

#########################################################################

export	HOSTCC HOSTCFLAGS HOSTLDFLAGS PEDCFLAGS HOSTSTRIP CROSS_COMPILE \
	AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP MAKE
export	CONFIG_SYS_TEXT_BASE PLATFORM_CPPFLAGS PLATFORM_RELFLAGS CPPFLAGS CFLAGS AFLAGS

#########################################################################

# Allow boards to use custom optimize flags on a per dir/file basis
BCURDIR = $(subst $(SRCTREE)/,,$(CURDIR:$(obj)%=%))
ALL_AFLAGS = $(AFLAGS) $(AFLAGS_$(BCURDIR)/$(@F)) $(AFLAGS_$(BCURDIR))
ALL_CFLAGS = $(CFLAGS) $(CFLAGS_$(BCURDIR)/$(@F)) $(CFLAGS_$(BCURDIR))
EXTRA_CPPFLAGS = $(CPPFLAGS_$(BCURDIR)/$(@F)) $(CPPFLAGS_$(BCURDIR))
ALL_CFLAGS += $(EXTRA_CPPFLAGS)

# The _DEP version uses the $< file target (for dependency generation)
# See rules.mk
EXTRA_CPPFLAGS_DEP = $(CPPFLAGS_$(BCURDIR)/$(addsuffix .o,$(basename $<))) \
		$(CPPFLAGS_$(BCURDIR))
$(obj)%.s:	%.S
	$(CPP) $(ALL_AFLAGS) -o $@ $<
$(obj)%.o:	%.S
	$(CC)  $(ALL_AFLAGS) -o $@ $< -c
$(obj)%.o:	%.c
	$(CC)  $(ALL_CFLAGS) -o $@ $< -c
$(obj)%.i:	%.c
	$(CPP) $(ALL_CFLAGS) -o $@ $< -c
$(obj)%.s:	%.c
	$(CC)  $(ALL_CFLAGS) -o $@ $< -c -S

#########################################################################

# If the list of objects to link is empty, just create an empty built-in.o
cmd_link_o_target = $(if $(strip $1),\
		      $(LD) $(LDFLAGS) -r -o $@ $1,\
		      rm -f $@; $(AR) rcs $@ )

#########################################################################
