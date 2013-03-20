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
# --------------------------------------------------------------------------

GADD=../src/gadd
REF_DIR=$GRVY_INPUT_EXAMPLE_DIR/ref_files
TEST_FILE=.test.h5

# Compiled binaries must exist

if test ! -x $GADD ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

if test -e $TEST_FILE ;then
    rm -f $TEST_FILE
fi


# -------------------------------------
# First, run the binary with help option
# -------------------------------------

$GADD -h >& $TEST_FILE || exit 1

if test ! -s $TEST_FILE ; then
    echo "${0}: No output generated from $GADD"
    exit 1
fi

diff $TEST_FILE $REF_DIR/gadd.help || exit 1

# Clean up

if test -e $TEST_FILE ; then
    rm -f $TEST_FILE
fi

# --------------------------------------------------------------
# Second, run the binary and make sure it generates output
# (will be used as input for subsequent gdump test)
# --------------------------------------------------------------

$GADD -q -c "acomment" -m "mymachine"   -j "12345" -p 42 -r "r2222" -f 65 13.45  "example1" $TEST_FILE || exit 1
$GADD -q -c "commentb" -m "mymachine"   -j "54321" -p  1 -r "r2223" -f 65  2.45  "example1" $TEST_FILE || exit 1
$GADD -e --debug -c "totoro"   -m "yourmachine" -j "98765" -p 99 -r "norev"    8.3e2  "example2" \
    $TEST_FILE >& /dev/null || exit 1

if test ! -s $TEST_FILE ; then
    echo "${0}: No output generated from $GADD"
    exit 1
fi

# Third, make sure we get an error of we cannot write to the file

TESTDIR=.grvy.testing.$RANDOM
mkdir $TESTDIR
if test ! -d $TESTDIR ; then
    echo "${0}: Unable to create temporary directory"
    exit 1
fi

chmod 400 $TESTDIR
! ($GADD -q -j "54321" -p  1 -r "r2223" -f 65  2.45  "example1" $TESTDIR/$TEST_FILE>& /dev/null)   || exit 1

if test -e $TESTDIR/$TEST_FILE ; then
   rm -f $TESTDIR/$TEST_FILE
fi

if test -d $TESTDIR ; then
   rmdir $TESTDIR
fi

#if test -e $TEST_FILE ;then
#    rm -f $TEST_FILE
#fi



exit 0