//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
//
// Regression/Test Suite for libGRVY.
//
// $Id: C_log_printf.c 12363 2010-08-15 14:52:36Z karl $
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

const char *test_name = "C version: unit tests";

int main(int argc, char **argv)
{

  // Silence stdout 

  grvy_log_setlevel(GRVY_NOLOG);

  // simply makes sure version_stdout() function is cooperating.

  grvy_version_stdout();

  return(0);
}
