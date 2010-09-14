#!/bin/bash
#--------------------------------------------------------------------------
# GRVY Test Suite
#
# Wrapper for F_timer which silences stdout.
#--------------------------------------------------------------------------

EXE=F_timer

# Compiled binary must exist

if test ! -x $EXE ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

# Run the binary and make sure it exits with zero status

./$EXE > /dev/null || exit 1




