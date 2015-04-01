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
use strict;
my $strTgt ="";
my $strTmpFile = "";
my $strCmd ="";
my $msg ="";
my $rc="";
my @arySrc;


while (@ARGV && $ARGV[0])
{
	my $arg_no = scalar @ARGV;
	if ($arg_no == 1)
	{
		$strTgt = $ARGV[0];
	}
	else
	{
		push (@arySrc, $ARGV[0]);
	}
	shift;
};

main();

sub main
{
	my $cmd_now = "date +\"%Y%m%d_%H%M%S_%N\" 2>&1";
	my $strCrtTime;
	chomp($strCrtTime =`$cmd_now`);
	$strTmpFile = "/tmp/mtk_cp_tmp_$strCrtTime.tgz";

	foreach my $strSrc (@arySrc) {
		if (-d $strSrc)
		{
			if ($strSrc !~ /\/CVS/)
			{
				dir_copy($strSrc);
			}
		}
		elsif (-f $strSrc) 
		{
			$strCmd = "cp -f $strSrc $strTgt";
			$msg = `$strCmd 2>&1`;
			$rc = $?;
			die("$rc: $msg") if ($rc);
		}
		else
		{
			die("$strSrc is not directory neither files");
		}
	}
}


sub dir_copy
{
	my ($strSrc) = @_;
	
	$strCmd = "find $strSrc -name CVS";
	$msg = `$strCmd 2>&1`;
	if ($msg eq "")
	{
		$strCmd = "cp -rf $strSrc $strTgt";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
	}
	else
	{
		$strCmd = "pushd $strSrc; find . -name CVS|xargs tar zcvf $strTmpFile; popd";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
		
		$strCmd = "find $strSrc -name CVS|xargs rm -rf";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
		
		$strCmd = "cp -rf $strSrc $strTgt";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
		
		$strCmd = "tar zxvf $strTmpFile -C $strSrc";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
		
		$strCmd = "rm -rf $strTmpFile";
		$msg = `$strCmd 2>&1`;
		$rc = $?;
		die("$rc: $msg") if ($rc);
	}
}
