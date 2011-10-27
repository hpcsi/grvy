#!/bin/bash
#--------------------------------------------------------------------------
# GRVY Test Suite
#
# Diff two directories for consistency.
#--------------------------------------------------------------------------

if [ $# -ne 2 ]; then
    echo "Usage: $0 dir1 dir2"
    exit 1
fi

if [ ! -d $1 ];then
    echo "$1 is not a directory"
    exit 1
fi

if [ ! -d $2 ];then
    echo "$2 is not a directory"
    exit 1
fi

# a decent one liner if I say so myself

diff <(cd $1 && find . | sort) <(cd $2 && find . | sort) #>& /dev/null





