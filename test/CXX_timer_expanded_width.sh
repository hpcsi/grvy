#!/bin/bash
#--------------------------------------------------------------------------
# GRVY Test Suite
#
# Wrapper for C_timer_max_width to verify correct max width
# settings.
#--------------------------------------------------------------------------

EXE=CXX_timer_expanded_width
REF_FILE=$GRVY_INPUT_EXAMPLE_DIR/ref_files/summarize.expanded.txt
TEST_FILE=CXX_timer_width.output

# Compiled binary must exist

if test ! -x $EXE ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

# Reference file must exit

if test ! -s $REF_FILE ; then
    echo "${0}: Expected reference file does not exist"
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

./$EXE | head -3 >& $TESTDIR/$TEST_FILE || exit 1

if test ! -s $TESTDIR/$TEST_FILE ; then
    echo "${0}: No output generated from $EXE"
    exit 1
fi

# Compare the result to reference output

diff $REF_FILE $TESTDIR/$TEST_FILE >& /dev/null
diffstatus=$?

# Clean up

if test -e $TESTDIR/$TEST_FILE ; then
    rm -f $TESTDIR/$TEST_FILE
fi

if test -d $TESTDIR ; then
    rmdir $TESTDIR
fi

exit ${diffstatus}



