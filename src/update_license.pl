#!/usr/bin/env perl
# --------------------------------------------------------------------
# Simple support script for license management with PECOS developed
# software. Intended for use with autotools based projects. In
# particular, if a ".in" version of an input src file exists, the .in
# version will be updated in favor of names src file..
# 
# Originally: August 2010 (ks)
# -----------------------------------------------------------------

use warnings;

my $license_begin_delim='----------bl-$';
my $license_end_delim='----------el-$';

if (@ARGV >= 2) {
    $license_file = shift@ARGV;
} else {
    print "\nUsage: update_license.pl [LICENSE-FILE] [SOURCE FILE]...\n\n";
    exit 0;
}

# Verify license file existence and cache contents.

if ( ! -s $license_file ) {
    print "\n** Error: unable to open valid license file ($license_file).\n\n";
    exit(1);
}

open ($LIC,$license_file) || die "Cannot open $license_file\n";
my @license_text = <$LIC>;
close($LIC);

# Scan all provided input files and look for designated license
# begin/end delimiters.  When found, update with provided license file
# contents.

my $found_delim = 0;

while (@ARGV) 
{
    $found_delim = 0;

    $infile_test = shift @ARGV;

    # autoconf support - look for ".in" version of the src file

    if ( -e "$infile_test.in" ) {
	$infile = "$infile_test.in";
    } else {
	$infile = "$infile_test";
    }

    open($IN, "<$infile") || die "Cannot open $infile\n";
    open ($TMPFILE,">",".$infile.tmp") || die "Cannot create tmp file .$infile.tmp";

    while (<$IN>) {
	if(/$license_begin_delim/../$license_end_delim/) {
	    $found_delim=1;
	    if (/--bl-$/) {
		print $TMPFILE @license_text;
	    }
	} else {
	    print $TMPFILE $_;
	}
    }

    close($IN);
    close($TMPFILE);

    if($found_delim) {
	print "[license_tool]: updating license in file $infile\n";
	rename(".$infile.tmp",$infile) || die "Cannot rename updated file\n";
    } else {
	print "[license_tool]: warning: no license delimiters found in file $infile\n";
	unlink(".$infile.tmp") || die "Unable to remove temporary file\n";
    }
}






