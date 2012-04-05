#!/bin/bash
#--------------------------------------------------------------------------
# GRVY Test Suite
#
# Wrapper for C_timer_no_finalize to verify summarize works even if no
# prior finalize is called.
#--------------------------------------------------------------------------

EXE=C_timer_no_finalize
TEST_FILE=C_timer_no_finalize.output

# Compiled binary must exist

if test ! -x $EXE ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

# Create housing spot for test results

TESTDIR=.grvy.testing.$RANDOM

mkdir $TESTDIR
if test ! -d $TESTDIR ; then
    echo "${0}: Unable to create temporary directory"
    exit 1
fi

# Run the binary and make sure it exits with zero status

./$EXE >& $TESTDIR/$TEST_FILE || exit 1

if test ! -s $TESTDIR/$TEST_FILE ; then
    echo "${0}: No output generated from $EXE"
    exit 1
fi

# Look for any Nans/infs...

cat $TESTDIR/$TEST_FILE | egrep -i "inf|nan" >& /dev/null
diffstatus=$?

# Clean up

if test -e $TESTDIR/$TEST_FILE ; then
    rm -f $TESTDIR/$TEST_FILE
fi

if test -d $TESTDIR ; then
    rmdir $TESTDIR
fi

if [ $diffstatus -eq 0 ];then
    exit 1
else
    exit 0
fi



