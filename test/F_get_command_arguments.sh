#!/bin/bash
# Checks for preconditions/postconditions of the F_get_command_arguments tests

# Compiled binary must exist
if test ! -x F_get_command_arguments ; then
    echo "${0}: Expected binary not executable"
    exit 1
fi

# Run the binary with sample arguments and make sure it exits with zero status
./F_get_command_arguments a b c d e f || exit 1

exit 0
