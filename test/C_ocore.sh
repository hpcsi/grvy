#!/bin/bash
#--------------------------------------------------------------------------
# GRVY Test Suite
#
# Wrapper for C_ocore regression test.
# --------------------------------------------------------------------------

EXE=C_ocore

# Compiled binary must exist

if test ! -x $EXE ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

# Make sure we have MPI 
if ! which mpiexec > /dev/null ; then
    echo "WARNING: Unable to find mpiexec; skipping C_ocore test"
    exit 0
fi

# Run the binary and make sure it exits with zero status

mpiexec -np 2 ./$EXE >& /dev/null || exit 1

exit $?




