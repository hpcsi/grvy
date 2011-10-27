//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011 The PECOS Development Team
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
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <grvy.h>

int main(int argc, char **argv)
{
  int flag = 1;
  char template[]   = "tmpdir-XXXXXX";

  /* Silence info/warn/error messages */

  //  grvy_log_setlevel(GRVY_NOLOG);
  grvy_log_setlevel(GRVY_DEBUG);

  /* Generate unique sandbox dir for testing */

  flag *= (0 == grvy_create_unique_dir(template));

  /* Failure should occur if we attempt to copy a file */
  {
    grvy_printf(GRVY_DEBUG,"<-- Testing non-directory input path\n");
    flag *= (1 == grvy_copy_dir("C_check_file_path",template));
    grvy_printf(GRVY_DEBUG,"Done -->\n");
  }

  /* Failure should occur if input/destination dirs are identical */
  {
    grvy_printf(GRVY_DEBUG,"<-- Testing identical input/destination dirs\n");
    flag *= (1 == grvy_copy_dir(template,template));
    grvy_printf(GRVY_DEBUG,"Done -->\n");
  }

  /* Failure should occur if destination path is a file */
  {
    grvy_printf(GRVY_DEBUG,"<-- Testing destination path is a file\n");
    flag *= (1 == grvy_copy_dir(template,"C_check_copy_dir.o"));
    grvy_printf(GRVY_DEBUG,"Done -->\n");
  }

  /* Success should occur for existing directory copied to empty existing destination */
  {
    grvy_printf(GRVY_DEBUG,"<-- Testing copy from empty existing destination\n");
    flag *= (0 == grvy_copy_dir("ref_files/adir",template));
    grvy_printf(GRVY_DEBUG,"Done -->\n");
  }

  /* Cleanup */

  //  flag *= (0 == rmdir(template));

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
