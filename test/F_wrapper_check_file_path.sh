#!/bin/bash
# Checks for preconditions/postconditions of the F_check_file_path tests

# Compiled binary must exist
if test ! -x F_check_file_path ; then
    echo "${0}: Expected binary not executable"
    exit 1
fi

# Clean up any old test output directories
for dir in CheckFilePathDir_*
do
    if test -d "${dir}" ; then
        echo "${0}: Deleting stale scratch directory from previous test: $dir"
        rm -rf "${dir}"
    fi
done

# Run the binary and make sure it exits with zero status
./F_check_file_path || exit 1

if ! rmdir "CheckFilePathDir/A/B/C" ; then
  echo "${0}: Expected directory did not exist"
  exit 1
fi
if ! rmdir "CheckFilePathDir/A/B" ; then
  echo "${0}: Expected directory did not exist"
  exit 1
fi
if ! rmdir "CheckFilePathDir/A" ; then
  echo "${0}: Expected directory did not exist"
  exit 1
fi
if ! rmdir "CheckFilePathDir" ; then
  echo "${0}: Expected directory did not exist"
  exit 1
fi

exit 0
