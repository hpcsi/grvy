#!/bin/bash
# Checks for preconditions/postconditions of the F_scratch_dir tests

# Compiled binary must exist
if test ! -x F_scratch_dir ; then
    echo "${0}: Expected binary not executable"
    exit 1
fi

# Clean up any old test output directories
for dir in ScratchDir_*
do
    if test -d "${dir}" ; then
        echo "${0}: Deleting stale scratch directory from previous test: $dir"
        rm -rf "${dir}"
    fi
done

# Run the binary and make sure it exits with zero status
./F_scratch_dir || exit 1

# Make sure there are no lingering directories from the test
status=0
for dir in ScratchDir_*
do
    if test -d "${dir}" ; then
        echo "${0}: Deleting lingering scratch directory from this test: $dir"
        rm -rvf "${dir}"
        status=1
    fi
done
exit ${status}
