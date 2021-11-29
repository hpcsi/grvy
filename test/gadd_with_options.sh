#!/bin/bash
#-----------------------------------------------------------------------bl-
#--------------------------------------------------------------------------
# 
# libGRVY - a utility library for scientific computing.
#
# Copyright (C) 2008-2013,2018-2021 The PECOS Development Team
# Additional Copyright (C) 2018 individual authors
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
# --------------------------------------------------------------------------
# --------------------------------------------------------------------------


GADD=../src/gadd

# Compiled binaries must exist

if test ! -x $GADD ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

if test -e $TEST_FILE ;then
    rm -f $TEST_FILE
fi

# Verify we can run with options not tested by other regressions

$GADD --debug --version >& /dev/null || exit 1

exit 0