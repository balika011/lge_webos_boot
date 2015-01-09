#!/usr/bin/perl
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

use lib "$ARGV[0]/perl_lib";
use Spreadsheet::WriteExcel;
use Parse::RecDescent;
use File::Copy;
use MIME::Lite; #for e-mail utility

#show title
my $TITLE = "
========================================================================
 [Program]          p4_xls.pl
 [Version]          V1.0
 [Revision Date]    2010-09-09
 [Author]           Brianpc Huang, brianpc.huang\@mediatek.com, 21895, 2010-09-09
 [Modified]         Brianpc Huang,
 [Last Update]      2010-09-09
 [Copyright]
    Copyright (C) 2004 MediaTek Incorporation. All Rights Reserved.
========================================================================
";

my $DTV_HR_FILE = "/proj/brianpchuang/test_sql/db/hr_dh_account.csv";
my $DEBUG_MODE =0;
my @aryAuthorGroup;
my @aryAuthor;
my $nWarningNo;
my $strModelName;
my $strBranchName;
my %hashGroup   = ();
my %hashAuthor  = ();

my $DFG_CC_LIST   = "brianpc.huang\@mediatek.com,hong.wang\@mediatek.com,lele.wang\@mediatek.com,DTV_SI_CSI";

# force overwrite owner hash tables, handle special case
my %force_module_2_owner=(

    # Jason's request
    "panel_table_cust.c"                        => "cosh.hsu",
    "nav_teletext"                              => "jonathan.kao",
    "nav_ttx_view_rc.c"                         => "jonathan.kao",

    # Brian's request
    "eeprom_if.c"           => "rocky.huang",
    "nor_part.c"            => "jerrick.liu",
    "block_mw.c"            => "leon.lin",
    "dsp_intf.c"            => "jessica.wu",
    "nand_sdm.c"            => "lawrance.liu",
    "pd_glue_dvbt.c"        => "jc.wu",
    "tve_if.c"              => "ky.lin",
    "aud_drv.c"             => "jessica.wu",
    "aud_if.c"              => "jessica.wu",
    "aud_dsp_cfg.c"         => "sunman.chang",

    "project_x/middleware/strm_mngr/video_hdlr/ntsc_vbi_psr" => "jianfeng.rong",

    # Scott's request
    "project_x/middleware/mutil/midxbuld/asf_idxbuld"  => "lianming.lin",
    "project_x/middleware/mutil/midxbuld/avi_idxbuld"  => "guanghui.wang",
    "project_x/middleware/mutil/midxbuld/mp4_idxbuld"  => "junhai.qiu",
    "project_x/middleware/mutil/midxbuld/mp3_idxbuld"  => "guanghui.wang",
    "project_x/middleware/mutil/minfo/asf_info"        => "lianming.lin",
    "project_x/middleware/mutil/minfo/avi_info"        => "guanghui.wang",
    "project_x/middleware/mutil/minfo/mp4_info"        => "junhai.qiu",
    "project_x/middleware/mutil/minfo/mp3_info"        => "guanghui.wang",
    "project_x/middleware/mutil/minfo/ps_info"         => "zink.hsin",
    "project_x/middleware/mutil/mfmtrecg/asf_mfmtrecg" => "lianming.lin",
    "project_x/middleware/mutil/mfmtrecg/avi_mfmtrecg" => "guanghui.wang",
    "project_x/middleware/mutil/mfmtrecg/mp4_mfmtrecg" => "junhai.qiu",
    "project_x/middleware/mutil/mfmtrecg/mp3_mfmtrecg" => "guanghui.wang",
    "project_x/middleware/prc_mngr"                    => "zhicheng.wang",

    "project_x/middleware/ci"                          => "flora.fu",
    "project_x/middleware/vbi_fltr/vbif_hdlr_ttx"      => "lei.xie",
    "project_x/middleware/vbi_fltr"                    => "lei.xie",

    "project_x/middleware/conn_mngr/cm_atsc_util.c"    => "jun.zhang",



    # YT Chen's request
    "pcmcia_cmd.c"                           => "lawrance.liu",
    "drv_scaler.c"                           => "hotkey.yang",
    "drv_lbox.c"                             => "benson.lee",
    "drv_di_int.c"                           => "andrew.chuang",
    "drv_nr_int.c"                           => "greg.peng",
    "chksum.c"                               => "jipson.liu",
    "drv_contrast_int.c"                     => "andrew.chuang",
    "ether_oid.c"                            => "kenny.hsieh",

    # Jason Chen's request
    "menu_page_ch_edit.c"                    => "xutong.hu",
    "nav_stream_filter.c"                    => "nick.chen",

    # Brian's request
    "dtv_linux/dtv_process/util"                                => "lianming.lin",
    "dtv_linux/project_x_linux/rpc/rpc_wrapper/lol"             => "jingbo.kuang",
    "dtv_linux/project_x_linux/rpc/rpc_wrapper/nwl"             => "jingbo.kuang",
	"dtv_linux/project_x_linux/rpc/rpc_wrapper"                 => "xia.zhang",
	"dtv_linux/project_x_linux/middleware/handle"               => "lianming.lin",
	"dtv_linux/project_x_linux/rpc/ri_common.c"                 => "lianming.lin",
	"dtv_linux/project_x_linux/rpc/rpc_handler/rh_mm_svctx.c"   => "lianming.lin",
	"dtv_linux/project_x_linux/rpc/rpc_handler/rh_init.c"       => "lianming.lin",
	"dtv_linux/project_x_linux/rpc/rpc_handler/rh_svl.c"        => "tiezheng.zhong",
	"dtv_linux/project_x_linux/rpc/rpc_handler/rh_dt.c"         => "tiezheng.zhong",
	
	"project_x/middleware/wgl/cc_isdb"      =>  "zhicheng.wang",
	"project_x/middleware/tbl_mngr/dsmcc_eng/dsmcc_dii.c" => "stanley.chang",

	"evctx_dvb_eng.c"                       =>  "binl.liu",
	"sb_dvb_svl.c"                          =>  "zhihuai.zhang",
	"dvb_si_txt_cnv_6937.c"                 =>  "binl.liu",
	"rh_sb.c"                               =>  "cheng.liu",
	

	
    "DUMMY_FILE"                             => "dummy.owner"
);

# force overwrite owner hash tables, handle special case for cvs or some leave colleague
my %force_owner_2_owner=(
    "dwu"                    => "daniel.wu",
    "grayhuang"              => "gray.huang",
    "ttsai"                  => "thomas.tsai",
    "dian.yin"               => "chao.yan",
    "richard.lu"             => "jeremy.wu",
    "jchen"                  => "jason.chen",
    "chengjie.huang"         => "heming.zhang",
    "wanghong.li"            => "yuankang.li",
    "cs.kuo"                 => "zhiwei.chen"
);


main();
sub main
{

	my $strWarnFile;
	
	if (@ARGV && $ARGV[1]) {
		$strWarnFile  = $ARGV[1];
	}

	if (@ARGV && $ARGV[2]) {
		$strModelName  = $ARGV[2];
	}

	if (@ARGV && $ARGV[3]) {
		$strBranchName  = $ARGV[3];
	}

	myPrint( __LINE__ ,"strWarnFile:\t$strWarnFile");
	
	my $strXlsFile = $strWarnFile;
	$strXlsFile =~ s/.log/.$strModelName.xls/g;
	my $strHrGroupFile = $strWarnFile;
	$strHrGroupFile =~ s/.log/.$strModelName.log.group/g;
	my $strHrAuthorFile = $strWarnFile;
	$strHrAuthorFile =~ s/.log/.$strModelName.log.author/g;
	
	$nWarningNo = csv2xls($strWarnFile,$strXlsFile);
	my %hashTemp = map { $_ => 1 } @aryAuthorGroup;
	my @aryAuthorGroup_S = sort keys %hashTemp;
	my $strAuthorString = join (',', @aryAuthorGroup_S);
	$strAuthorString =~ s/\//_/g;
	putLog($strHrGroupFile, $strAuthorString);

	my %hashTemp2 = map { $_ => 1 } @aryAuthor;
	my @aryAuthor_S = sort keys %hashTemp2;
	my $strAuthorString2 = join (',', @aryAuthor_S);
	$strAuthorString2 =~ s/\//_/g;
	putLog($strHrAuthorFile, $strAuthorString2);
	
	notify_mail($strHrAuthorFile, $strHrGroupFile,$strXlsFile);

	return 0;
}



#
# csv2xls() sub function
#
sub csv2xls
{
    my $csv_file = $_[0];
    my $xls_file = $_[1];

	myPrint( __LINE__ ,"csv_file:\t$csv_file");
	myPrint( __LINE__ ,"xls_file:\t$xls_file");
	
    # open the Comma Separated Variable file
    if ( -e "$csv_file")
    {
    	myPrint( __LINE__ ,"Exist: $csv_file");
    }
    open (CSVFILE, "<$csv_file") or die "Fail to open $csv_file, $!";

    # create a new Excel workbook
    my $workbook  = Spreadsheet::WriteExcel->new($xls_file);
    my $worksheet = $workbook->add_worksheet("Checking Results");
    # set format
    my $format = $workbook->add_format();
    $format->set_font('Arial Unicode MS');
    $format->set_size(10);
    $format->set_border(1);
    $format->set_text_wrap();

    my $format_ex = $workbook->add_format();
    $format_ex->set_font('Arial Unicode MS');
    $format_ex->set_size(10);
    $format_ex->set_border(1);
    $format_ex->set_bg_color('silver');
    $format_ex->set_text_wrap();

    # row and column are zero indexed
    my $row = 1;
    my $col = 0;

	$worksheet->write(0, 0, 'Author'   , $format_ex);
	$worksheet->write(0, 1, 'Type'     , $format_ex);
	$worksheet->write(0, 2, 'File'     , $format_ex);
	$worksheet->write(0, 3, 'Message'  , $format_ex);
	$worksheet->write(0, 4, 'Group'    , $format_ex);

	
    while (<CSVFILE>)
    {
        chomp($_);
        my @fld = split(/;;;/, $_);
        my $strAuthur = @fld[0];
        my $strFile   = @fld[2];
		$strAuthur =~ s/mhf_//g;
		$strAuthur =~ s/msz_//g;

		my ($file_path, $file_name) = $strFile =~ m/(.*)\/(.*)?/;

		foreach my $key ( keys %force_module_2_owner ) 
        {
        	my $value = $force_module_2_owner{$key};
            if ( index($strFile, $key) != -1
                 || index($file_path, $key) != -1
                 || index($file_name, $key) != -1
               )
            {
                $strAuthur = $value;
                last;
            }
        }

        foreach my $key ( keys %force_owner_2_owner ) 
        {
        	my $value = $force_owner_2_owner{$key};
            if ( index($strAuthur, $key) != -1)
            {
                $strAuthur = $value;
                last;
            }
        }
        
        my $group = get_group($strAuthur);
        if (!$hashGroup{$group})
		{
			$hashGroup{$group} =1;
		}
		else
		{
			$hashGroup{$group} ++;
		}

        if (!$hashAuthor{$strAuthur})
		{
			$hashAuthor{$strAuthur} =1;
		}
		else
		{
			$hashAuthor{$strAuthur} ++;
		}
		
		$worksheet->write($row, 0, $strAuthur, $format);
        $col = 1;
        for(my $i = 1; $i < scalar @fld; $i++) {
        	my $token= @fld[$i];
            $worksheet->write($row, $i, $token, $format);
            $col++;
        }
        $worksheet->write($row, $col, $group, $format);
        
        $row++;
    }
    # set column width
    $worksheet->set_column(0, 0, 20);
    $worksheet->set_column(1, 1, 10);
    $worksheet->set_column(2, 2, 50);
    $worksheet->set_column(3, 3, 60);
    $worksheet->set_column(4, 4, 30);

    # set auto filter
    $worksheet->autofilter(0, 0, 0, $col);

    $workbook->close();
	close CSVFILE;
    return ($row - 1);
}

sub myPrint
{
	my $strLine        = $_[0];
	my $strString      = $_[1];
	print "$strLine>> $strString\n" if ($DEBUG_MODE);
}

sub get_group
{
	my ($strAuthur) =@_;
	return 'super_group' if ($strAuthur eq "super");
	my $strGrepString =`grep -w $strAuthur $DTV_HR_FILE 2>&1|head -n 1`;
	if ($strGrepString eq "")
	{
		return 'super_group';
	}
	else
	{
		my @data = split(/,/, $strGrepString);
		my $group =@data[2];
		chomp($group);
		push (@aryAuthorGroup,"$group/Manager");

		my $mail =@data[1];
		push (@aryAuthor,$mail);
		return $group;
	}
	
}

sub putLog
{
	my ($strFile, $strMsg) = @_;
	open(LOG_FILE, ">$strFile") or die("Failed to open file $strFile .");
	print LOG_FILE $strMsg;
	print LOG_FILE "\n";
	close LOG_FILE;
}

sub send_email {

	my ($strMailFrom, $strMailList, $strMailSubject, $strMailBody, $strAttach, $strCcList) = @_;
	my $msg = MIME::Lite->new(
		From    => $strMailFrom,
		To      => $strMailList,
		Cc      => $strCcList,
		Subject => $strMailSubject,
		Data    => $strMailBody
	);
	
	if (-e $strAttach )
	{
		$msg->attach(
			Type => 'application/vnd.ms-excel',
			Path => $strAttach,
			Disposition => 'attachment'
		);
	}
	MIME::Lite->send('smtp', '172.21.100.30', Timeout => 60);
	$msg->send;
} # end of sub

sub notify_mail 
{
	my ($strAuthorFile, $strGroupFile,$strXlsFile) = @_;
	my $strAttach;
	my $strMailFrom = "brianpc.huang\@mediatek.com";
	my $strMAIL_LIST = `cat $strAuthorFile 2>&1`;
	chomp($strMAIL_LIST);
	my $strCC_LIST   = `cat $strGroupFile 2>&1`;
	chomp($strCC_LIST);

	$strCC_LIST .= ",$DFG_CC_LIST";

	my $strMailSubject = "[Warning_Check] Warning Found in $strModelName on branch $strBranchName";
	my $strMailBody = "Warning Number Count: $nWarningNo\n\n";
	$strMailBody .= "Branch Name: $strBranchName\n\n";
	$strMailBody .= "Model Name: $strModelName\n\n";
	
	$strMailBody .= "Group Counting\n================================================\n";

	my @aryHashGroup;
	foreach my $key ( keys %hashGroup ) 
	{
		push (@aryHashGroup,"$key : $hashGroup{$key}");
	}
	@aryHashGroup = sort @aryHashGroup;
	my $strHashGroupMsg = join ("\n", @aryHashGroup);

	$strMailBody .= "$strHashGroupMsg \n";

	$strMailBody .= "\n\n\n";

	$strMailBody .= "RD Counting\n================================================\n";

	my @aryHashAuthor;
	foreach my $key ( keys %hashAuthor ) 
	{
		push (@aryHashAuthor,"$key : $hashAuthor{$key}");
	}
	@aryHashAuthor = sort @aryHashAuthor;
	my $strHashRDMsg = join ("\n", @aryHashAuthor);

	$strMailBody .= "$strHashRDMsg \n";

	print "Warning Count\n================================================\n";
	print "$strHashRDMsg\n";
	print "Total: $nWarningNo\n";

	send_email($strMailFrom, $strMAIL_LIST, $strMailSubject, $strMailBody, $strXlsFile, $strCC_LIST);
}

