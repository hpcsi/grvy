#!/bin/bash
#-----------------------------------------------------------------------bl-
#--------------------------------------------------------------------------
# 
# libGRVY - a utility library for scientific computing.
#
# Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Version 2.1 GNU Lesser General
# Public License as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc. 51 Franklin Street, Fifth Floor, 
# Boston, MA  02110-1301  USA
#
#-----------------------------------------------------------------------el-
# 
#  Regression/Test Suite for libGRVY.
# 
#  $Id$
# --------------------------------------------------------------------------

GDUMP=../src/gdump

REF_DIR=$GRVY_INPUT_EXAMPLE_DIR/ref_files
TEST_FILE=.test.h5
TEST_DIR=perf_test

# Compiled binaries must exist

if test ! -x $GDUMP ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

if test ! -s $TEST_FILE ; then
    echo "${0}: No output generated from $GDUMP"
    exit 1
fi

# Previous test generated new hdf file using gadd; now dump the
# performance data using $GDUMP

$GDUMP -q -c -o $TEST_DIR $TEST_FILE || exit 1

# Verify we have expected output files

if test ! -s $TEST_DIR/mymachine/example1 ;then
    echo "${0}: $GDUMP did not create $TEST_DIR/mymachine/example1"
    exit 1
fi

if test ! -s $TEST_DIR/yourmachine/example2 ;then
    echo "${0}: $GDUMP did not create $TEST_DIR/yourmachine/example2"
    exit 1
fi

# Compare the result to reference output

cat $TEST_DIR/mymachine/example1   | grep -v "^#"  | awk '{$2=$3=$4="";print}' >& $TEST_DIR/mymachine/example1.pruned
cat $TEST_DIR/yourmachine/example2 | grep -v "^#"  | awk '{$2=$3=$4="";print}' >& $TEST_DIR/yourmachine/example2.pruned

diff $TEST_DIR/mymachine/example1.pruned $REF_DIR/$TEST_DIR/mymachine/example1.pruned >& /dev/null || exit 1
diff $TEST_DIR/mymachine/example1.pruned $REF_DIR/$TEST_DIR/mymachine/example1.pruned >& /dev/null || exit 1

# Clean up

if test -d $TEST_DIR ; then
    rm -rf $TEST_DIR
fi

#------------------------------------
# Run binary with additional options
#------------------------------------

$GDUMP -q -a -d "%" -o $TEST_DIR $TEST_FILE || exit 1
$GDUMP --debug -e -s $TEST_FILE >& /dev/null || exit 1

# -------------------------------------
# Now, run the binary with help option
# -------------------------------------

$GDUMP -h >& $TEST_FILE || exit 1

if test ! -s $TEST_FILE ; then
    echo "${0}: No output generated from $GDUMP"
    exit 1
fi

diff $TEST_FILE $REF_DIR/gdump.help || exit 1

# Clean up

if test -e $TEST_FILE ; then
    rm -f $TEST_FILE
fi

if test -d $TEST_DIR ; then
    rm -rf $TEST_DIR
fi

exit 0



