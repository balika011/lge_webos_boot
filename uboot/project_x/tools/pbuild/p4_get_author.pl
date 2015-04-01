#!/usr/bin/perl
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
#
# Author: Chien-Ping Cheng (cp_cheng\@mtk.com.tw) on 2004-11-08
#
# Description:
# This script will
# 1. auto_build and send email
#***********************************************************************
#use module
use strict;
use Getopt::Std;
#use MIME::Lite; #for e-mail utility

#show title
my $TITLE = "
========================================================================
 [Program]          p4_get_author.pl
 [Version]          V1.0
 [Revision Date]    2009-04-02
 [Author]           Brianpc Huang, brianpc_huang\@mediatek.com, 21895, 2009-04-02
 [Modified]         Brianpc Huang,
 [Last Update]      2007-03-29
 [Copyright]
    Copyright (C) 2004 MediaTek Incorporation. All Rights Reserved.
========================================================================
";


my $STR_GET_AUTHOR = "p4 filelog -m 1 -s %s 2>/dev/null|head -n 2|tail -n 1|awk '{print \$9}'|sed -e 's/\@.*//g'";

my @aryExcludeList = ("project_x/middleware/inet/openssl");

my $strCustRlsFlag="";
my $DEBUG_MODE =0;
my @aryAuthor;


main();
sub main
{
	print "  CHK\tLOG\n";
	my $strBranch = "";
	my $strWarnErrFile = "";
	my $strErrFile ="";
	my $strWarningFile ="";
	
	if (@ARGV && $ARGV[0]) {
		$strBranch  = $ARGV[0];
	}

	if (@ARGV && $ARGV[1]) {
		$strWarnErrFile  = $ARGV[1];
	}

	if (@ARGV && $ARGV[2]) {
		$strErrFile  = $ARGV[2];
	}
	
	if (@ARGV && $ARGV[3]) {
		$strCustRlsFlag  = $ARGV[3];
	}
	
	myPrint( __LINE__ ,"strBranch:\t$strBranch");
	
	die "Not find Branch or WarnErrFile" if (($strBranch eq "") or ($strWarnErrFile eq "") or ($strErrFile eq ""));
	$strWarningFile = $strErrFile ;
	$strWarningFile =~ s/_fail/_warning/g;
	
	
	my @aryMsg;
	my @aryFullMsg;
	
	if (! chk_P4_status())
	{
		# generate Warning/Error message without author
		@aryMsg = gen_report($strWarnErrFile,\@aryFullMsg);
	}
	else
	{
		# generate Warning/Error message with author
		@aryMsg = gen_report_w_author($strBranch,$strWarnErrFile,\@aryFullMsg);
	}

	print "\n========== Build Result Info, Branch = $strBranch ===============\n" if (scalar @aryMsg >0);
	for(my $i = 0; $i < scalar @aryMsg; $i++) {
		my $strMsg     = @aryMsg[$i];
		my $Fullmsg = @aryFullMsg[$i];
		my $strAuthor = @aryAuthor[$i];

		my ($strAbsPath, $msg) = $strMsg =~ m/(.*?);;;(.*)/;
		$msg =~ s/\(/\\\(/g;
		$msg =~ s/\)/\\\)/g;
		
		if ( ($msg =~ /Error/) or ($msg =~ /error:/) )
		{
			putLog($strErrFile, "$strAuthor;;;$Fullmsg");
		}
		else
		{
			putLog($strWarningFile, "$strAuthor;;;$Fullmsg");
		}
		if ($strAuthor ne '')
		{
			print "$strAuthor:\t";
		}
		print "$msg\n";
	}
	print "\n\n" if (scalar @aryMsg >0);
	
	my $strRst = join ("\n", @aryMsg);
	return 0;
	
}

sub gen_report_w_author
{
	my ($strBranch,$strWarnErrFile,$argFullMsg) = @_;
	my @aryMsg = gen_report($strWarnErrFile,$argFullMsg);
	my @aryRstMsg;
	foreach my $strMsg (@aryMsg) 
	{
		my ($strAbsPath, $msg) = $strMsg =~ m/(.*?);;;(.*)/;
		next if (like_array(\@aryExcludeList,$msg));
		my $strAuthor = "";
		$strAuthor = get_author($strBranch,$strAbsPath) if ($strCustRlsFlag eq '');
		push (@aryRstMsg,"$strAbsPath,$strAuthor:\t$msg");
		push (@aryAuthor, $strAuthor);
	}
	return @aryRstMsg;
}

sub get_author
{
	my ($strBranch,$strAbsPath) = @_;
	my $strAuthor = "";
	my $strCmd1 = sprintf ($STR_GET_AUTHOR, $strAbsPath); 
	$strCmd1 =~ s/\(/\\\(/g;
	$strCmd1 =~ s/\)/\\\)/g;

	$strAuthor = `$strCmd1`;
	chomp($strAuthor);
	return $strAuthor if ($strAuthor ne "");
	
	$strAuthor = get_p4_depot_author($strBranch,$strAbsPath);
	#myPrint( __LINE__ ,"strAuthor:\t$strAuthor");
	return $strAuthor if ($strAuthor ne "");
	return "super";
}

sub get_p4_depot_author
{
	my ($strBranch,$strAbsPath) =@_;
	my $str2ndPath = `p4 dirs //DTV/*`;
	my @ary2ndPath = split /\n/, $str2ndPath; 

	my $strP4Path ="";
	my $strAuthor ="";
	foreach my $strP1 (@ary2ndPath)
	{
		my $strPath = `p4 dirs $strP1/$strBranch 2>&1`;
		if ($strPath !~/no such file/)
		{
			$strP4Path = $strPath;
			chomp($strP4Path);
			last;
		}
	}
	
	return $strAuthor if ($strP4Path eq "");
	
	my ($strPrjPath) = $strAbsPath =~ m/(project_x.*)/;

	my $strCmd1 = sprintf ($STR_GET_AUTHOR, "$strP4Path/$strPrjPath");
	$strCmd1 =~ s/\(/\\\(/g;
	$strCmd1 =~ s/\)/\\\)/g;

	$strAuthor = `$strCmd1`;
	chomp($strAuthor);
	return $strAuthor;
		
}

sub gen_report
{
	my ($strWarnErrFile,$argFullMsg) =@_;
	die "$strWarnErrFile does not exist" if (! -e $strWarnErrFile);
	
	my @aryPath;
	my @aryMsgList;
	my $strFindFile = "";

	open(IN_FILE, "<$strWarnErrFile")  or die("Failed to open file $strWarnErrFile.");
	foreach (<IN_FILE>)
	{
		my $strFile  = $_;
		chomp($strFile);
		myPrint( __LINE__ ,"strFile:\t$strFile");
		my ($strFileC, $strFileO) = $_ =~ m/(.*),(.*)/;
		$strFileO = trim($strFileO);
		if ( -l $strFileC)
		{
			myPrint( __LINE__ ,"$strFileC is a symlink");
			my $strLink01 = `readlink $strFileC`;
			chomp($strLink01);
			myPrint( __LINE__ ,"$strFileC link to $strLink01");
			$strFileC = $strLink01;
			if ( -l $strFileC)
			{
				myPrint( __LINE__ ,"$strFileC is a symlink");
				my $strLink02 = `readlink $strFileC`;
				chomp($strLink02);
				myPrint( __LINE__ ,"$strFileC link to $strLink02");
				$strFileC = $strLink02;
				if ( -l $strFileC)
				{
					myPrint( __LINE__ ,"$strFileC is a symlink");
					my $strLink03 = `readlink $strFileC`;
					chomp($strLink03);
					myPrint( __LINE__ ,"$strFileC link to $strLink03");
					$strFileC = $strLink03;
				}
			}
		}
		myPrint( __LINE__ ,"strFileC:\t$strFileC");
		myPrint( __LINE__ ,"strFileO:\t$strFileO");
		my $nSizeFileO = -s $strFileO;
		if ($nSizeFileO != 0)
		{
			myPrint( __LINE__ ,"strFileO,nSizeFileO:\t$strFileO,$nSizeFileO");
			my ($strVm,$strPrjPath) = $strFileC =~ m/(vm_linux|vm_proj_x)\/(.*)/;
			my $strMsg  =`sed ':a; /\$/N; s/\\n//; ta' $strFileO`;
			$strMsg=~ s/\s+/ /g;
			my $fragment =  substr ($strMsg, 0, 100);
			my $large_fragment =  substr ($strMsg, 0, 1024);

			myPrint( __LINE__ ,"strMsg:\t$strMsg");

			my $strType ="";
			$strType = "Error" if (($strMsg =~ /(\bError|\berror)/) and ($strMsg !~ / 0 errors/) );
			$strType = "Warn" if (($strMsg =~ /\bWarn/) or ($strMsg =~ /\bwarning/) );
			
			my $msg ="$strFileC;;;$strType;;;$strPrjPath;;;$fragment";
			my $strFullMsg ="$strType;;;$strPrjPath;;;$large_fragment";
			myPrint( __LINE__ ,"msg:\t$msg");
			push (@aryMsgList, $msg);
			push (@$argFullMsg, $strFullMsg);
		}
	}
	close IN_FILE;
	
	return @aryMsgList;
		
}

sub chk_P4_status
{
	my $msg = `p4 set 2>&1`;
	return 0 if ($msg =~ /command not found/);
	return 0 if ($msg =~ /cannot execute binary file/);
	my $msg2 = `p4 info 2>&1`;
	return 0 if ($msg2 =~ /Perforce client error/);

	my $strPath = `pwd`;
	chomp($strPath);
	return 0 if ($strPath =~ /\/release\//);
 
	return 1;
}

sub myPrint
{
	my $strLine        = $_[0];
	my $strString      = $_[1];
	print "$strLine>> $strString\n" if ($DEBUG_MODE);
}

sub like_array
{
	my $ary    = $_[0];
	my $target = $_[1];
	
	for(my $i = 0; $i < scalar @$ary; $i++) {
		my $src = @$ary[$i];
		$src =~ s/\//\\\//;
		return 1 if ($target =~/@$ary[$i]/);
	} 
	return 0;
}

sub trim {
	my $strString = @_[0];
	for ($strString) {           # trim white space in $variable, cheap
		s/^\s+//;
		s/\s+$//;
	}
	return $strString;
} # end of sub



sub putLog
{
	my ($strFile, $strMsg) = @_;
	open(LOG_FILE, ">>$strFile") or myDie(__LINE__,"Failed to open file $strFile .");
	print LOG_FILE $strMsg;
	print LOG_FILE "\n";
	close LOG_FILE;
}
