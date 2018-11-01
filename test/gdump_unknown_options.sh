#!/bin/bash
#-----------------------------------------------------------------------bl-
#--------------------------------------------------------------------------
# 
# libGRVY - a utility library for scientific computing.
#
# Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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


GDUMP=../src/gdump

# Compiled binaries must exist

if test ! -x $GDUMP ; then
    echo "${0}: Expected binary does not exist or is not executable"
    exit 1
fi

if test -e $TEST_FILE ;then
    rm -f $TEST_FILE
fi

# Verify we get an error with unknown option

! ( $GDUMP --hello-sweetie >& /dev/null ) || exit 1

# Verify that we get an error if we don't provide required arguments

! ($GDUMP >& /dev/null ) || exit 1

exit 0