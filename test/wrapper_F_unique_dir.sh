#!/bin/bash
# Checks for preconditions/postconditions of the F_unique_dir tests

# Compiled binary must exist
if test ! -x F_unique_dir ; then
    echo "${0}: Expected binary not executable"
    exit 1
fi

# Clean up any old test output directories
for dir in TempDir_*
do
    if test -d "${dir}" ; then
        echo "${0}: Deleting stale scratch directory from previous test: $dir"
        rm -rf "${dir}"
    fi
done

# Run the binary and make sure it exits with zero status
./F_unique_dir || exit 1

# Make sure there is exactly one lingering directory from the test
status=1
count=0
for dir in TempDir_*
do
    if test -d "${dir}" ; then
        count=$((count+1))
        if test $count -eq 1 ; then
          rm -rf "${dir}"
          status=0
        else
          echo "${0}: encountered $count directories, 1 expected: $dir"
          rm -rvf "${dir}"
          status=1
        fi
    fi
done
exit ${status}
