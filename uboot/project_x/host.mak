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
############################################################################
# $RCSfile: host.mak,v $
# $Revision: #1 $
# $Date: 2015/01/09 $
# $Author: dtvbm11 $
#
# Description:
#         Host specific defnitions
#
#############################################################################

ifeq "$(PROJECT_X_HOST)" "dos"
#
#############################################################################
# Host is DOS/ WINDOWS
#############################################################################
#
# Lint
#
LINT      =
LINT_FLAG =


#
# Make command and options
#
ORIG_MAKE = make
MAKE      = make
MAKE_FLAG =
#JOBS  ?= 12
#FMAKE     = make -j $(JOBS)
FMAKE     = make -j 4

#
# Copy command and options
#
CP      = copy
CP_FLAG = /Y


#
# Clean command and options
#
RM          = del
RM_FLAG     = /F/S/Q
RM_REC_FLAG = /F/S/Q
RMDIR       = rmdir
RMDIR_FLAG  = /S/Q


#
# Create directory command and options
#
MKDIR      = mkdir
MKDIR_FLAG =


#
# Catalog command and options
#
CAT      = type
CAT_FLAG =


#
# Lexer command and options
#
LEX      =
LEX_FLAG =


#
# Perl script command and options
#
PERL      = perl
PERL_FLAG =


#
# Zip command and options
#
ZIP      = zip
ZIP_FLAG =


#
# Specify the NULL device
#
NULL_DEV = 1> nul


#
# The following functions are used to translate include path and
# any other path
#
inc_path_cnvt    = $(subst /,\,$(1))
lib_path_cnvt    = $(subst /,\,$(1))
path_cnvt        = $(subst /,\,$(1))


#
# The following functions are used to check on directories and execute
# a set of function.
#
if_dir_exist_fct     = if exist $(1) $(subst ;, &,$(2))
if_dir_not_exist_fct = if not exist $(1) $(subst ;, &,$(2))


#
# The following functions is used to modify a newly created dependancy file.
#
mod_dep_file_fct = @$(CP) $(CP_FLAG) $(call path_cnvt,$(1)) $(call path_cnvt,$(2)) $(NULL_DEV)


else
ifeq "$(PROJECT_X_HOST)" "cygwin"
#
#############################################################################
# Host is CYGWIN
#############################################################################
#
# Lint
#
LINT       = splint
LINT_FLAGS = -booltype BOOL


#
# Make command and options
#
ORIG_MAKE = make
MAKE      = make
MAKE_FLAG =
#JOBS  ?= $(shell cat /proc/cpuinfo |grep processor|wc -l)
#FMAKE     = make -j $(JOBS)
FMAKE     = make -j 4

#
# Copy command and options
#
CP      = /bin/cp
CP_FLAG = -f


#
# Clean command and options
#
RM          = /bin/rm
RM_FLAG     = -f
RM_REC_FLAG = -rf
RMDIR       = /bin/rmdir
RMDIR_FLAG  = --ignore-fail-on-non-empty -p


#
# Create directory command and options
#
MKDIR      = mkdir
MKDIR_FLAG = -p


#
# Catalog command and options
#
CAT      = cat
CAT_FLAG =


#
# Lexer command and options
#
LEX      = /usr/bin/flex
LEX_FLAG =


#
# Perl script command and options
#
PERL      = perl
PERL_FLAG =


#
# Lockfile script command and options
#
LOCKFILE = lockfile -1 -l 30


#
# Zip command and options
#
ZIP      = zip
ZIP_FLAG =


#
# Specify the NULL device
#
NULL_DEV =


#
# The following functions are used to translate include path and
# any other path
#
inc_path_cnvt = $(1)
lib_path_cnvt = $(1)
path_cnvt     = $(1)


#
# The following functions are used to check on directories and execute
# a set of function.
#
if_dir_exist_fct     = @if [ -d $(1) ]; then $(2) fi
if_dir_not_exist_fct = @if [ ! -d $(1) ]; then $(2) fi


#
# The following functions is used to modify a newly created dependancy file.
#
mod_dep_file_fct = /bin/sed 's,\($*\)\.o[ :]*,\1.o $(2) : ,g' < $(1) > $(2)
mod_dep_file_01_fct = @/bin/sed 's,.*\($*\)\.o\(.*\)[ :],\1.o $(1) : ,g' -i $(1)

#
# The following functions is used to check directory whether empty or not
#
if_dir_empty_fct     = if [ "`find $(1) -type f|wc|awk '{print $$1}'`" == "0" ]; then $(2); fi

else
#
#############################################################################
# Default host is always LINUX
#############################################################################
#

PROJECT_X_HOST := linux


#
# Lint
#
LINT       = splint
LINT_FLAGS = -booltype BOOL +weak -castfcnptr -fixedformalarray -abstract -globuse \
             -retvalother -syntax -unrecogcomments -initallelements -fullinitblock \
             -redef -sysunrecog -unrecog -nestcomment \
             +csv $(call path_cnvt,$(LINT_DIR)/$@.csv) +csvoverwrite

#
# Make command and options
#
ORIG_MAKE = make
MAKE      = make
MAKE_FLAG =
#JOBS  ?= $(shell cat /proc/cpuinfo |grep processor|wc -l)
#FMAKE     = make -j $(JOBS)
FMAKE     = make -j 4

#
# Copy command and options
#
CP      = /bin/cp
CP_FLAG = -fp


#
# Clean command and options
#
RM          = /bin/rm
RM_FLAG     = -f
RM_REC_FLAG = -rf
RMDIR       = /bin/rmdir
RMDIR_FLAG  = --ignore-fail-on-non-empty -p


#
# Create directory command and options
#
MKDIR      = mkdir
MKDIR_FLAG = -p


#
# Catalog command and options
#
CAT      = /bin/cat
CAT_FLAG =


#
# Lexer command and options
#
LEX      = /usr/bin/lex
LEX_FLAG =


#
# Perl script command and options
#
PERL      = perl
PERL_FLAG =


#
# Lockfile script command and options
#
LOCKFILE = lockfile -1 -l 30


#
# Zip command and options
#
ZIP      = zip
ZIP_FLAG =


#
# Specify the NULL device
#
NULL_DEV =

#
# Date command and options
#
DATE = date
DATE_FLAG = +%Y%m%d_%H%M%S
SDATE_FLAG = +%s

#
# Symbolic Link command and options
#
LN = ln
LN_FLAG = -sf

#
# The following functions are used to translate include path and
# any other path
#
inc_path_cnvt = $(1)
lib_path_cnvt = $(1)
path_cnvt     = $(1)


#
# The following functions are used to check on directories and execute
# a set of function.
#
if_dir_exist_fct     = @if [ -d $(1) ]; then $(2) fi
if_dir_not_exist_fct = @if [ ! -d $(1) ]; then $(2) fi

#
# The following functions are used to check on files and execute
# a set of function.
#
if_file_exist_fct     = if [ -e $(1) ]; then $(2) fi
if_file_not_exist_fct = if [ ! -e $(1) ]; then $(2) fi

#
# The following functions are used to check on make successful or not, 
# a set of function.
#
set_build_chk_fct     = if [ $$? -eq 0 ]; then if [ -e $(1) ]; then sh $(1) $(2); fi; exit 0; else if [ -e $(1) ]; then sh $(1) $(2); fi; exit 1; fi
	
#
# The following functions are used to do archieve, 
# a set of function.
#
#sh ${TOOL_ROOT}/archieve_lib/ar_lib.sh

gen_archieve_fct =	for i in $(1)/*.content; do if [ -e $$i ]; then echo $(2) $(3) `echo $$i|sed -r 's/\.content//'` `cat $$i`;sh ${TOOL_ROOT}/archieve_lib/ar_lib.sh $(2) $(3) `echo $$i|sed -r 's/\.content//'` $$i; $(RM) $$i; fi done

#
# The following functions is used to modify a newly created dependancy file.
#
mod_dep_file_fct = /bin/sed 's,\($*\)\.o[ :]*,\1.o $(2) : ,g' < $(1) > $(2)
mod_dep_file_01_fct = @/bin/sed 's,.*\($*\)\.o\(.*\)[ :],\1.o $(1) : ,g' -i $(1)

#
# The following functions is used to check directory whether empty or not
#
if_dir_empty_fct     = if [ "`find $(1) -type f|wc|awk '{print $$1}'`" == "0" ]; then $(2); fi


endif
endif


adjust_custom_comp_path = $(if $(findstring /,$(1)),$(CUSTOM_BASE)/$(1),$(1))
adjust_target_comp_path = $(if $(findstring /,$(1)),$(TARGET_BASE)/$(1),$(1))

#
# The following functions are used to calculate the execute time of
# a set of function.
#
cal_exec_time_fct     = @echo BUILD_$(1)_END_TIME=`$(DATE) $(DATE_FLAG)`;\
                        if [ -e .build_start_time_$(1) ]; then expr `$(DATE) $(SDATE_FLAG)` - `cat .build_start_time_$(1)` > dtv_linux_build_time_$(1); \
                                              echo BUILD_$(1)_EXEC_TIME:`cat  dtv_linux_build_time_$(1)`; \
                                              rm -rf dtv_linux_build_time_$(1) .build_start_time_$(1); fi
                        
                                              
set_start_time_fct    = @$(DATE) $(SDATE_FLAG) > .build_start_time_$(1); \
                        echo BUILD_$(1)_START_TIME=`$(DATE) $(DATE_FLAG)`

ifndef SYMLINK_LOG
SYMLINK_LOG := /dev/null	
endif
if_file_notexist_w_sym_fct     = if [ -e $(1) -a ! -e $(2) -a ! -h $(2) ]; then $(LN) $(LN_FLAG) $(1) $(2); if [ "$(SYMLINK_LOG)" != "" ]; then echo  $(2) >> $(SYMLINK_LOG); fi ; fi
