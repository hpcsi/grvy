//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

const char *test_name = "C input parsing: no file existence";

int main(int argc, char **argv)
{
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *key1, *key2;
  int flag = 1;

  //  grvy_input_toggle_messages(0);
  grvy_log_setlevel(GRVY_NOLOG);

  /* Initialize/read the file */

  flag *= grvy_input_fopen("./tori-rulez");

  /* Close the file */

  flag *= grvy_input_fclose();

  if(flag == 0)
    exit(0);
  else
    exit(1);
}
