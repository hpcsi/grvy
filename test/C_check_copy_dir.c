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
  char template2[]  = "tmpdir-XXXXXX";
  char *input_dir   = getenv("GRVY_INPUT_EXAMPLE_DIR");
  char tmp_string[1024];

  /* Silence info/warn/error messages */

  grvy_log_setlevel(GRVY_NOLOG);
  //grvy_log_setlevel(GRVY_DEBUG);

  /* Generate unique sandbox dirs for testing */

  flag *= (0 == grvy_create_scratch_dir(template) );
  flag *= (0 == grvy_create_scratch_dir(template2));

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
    sprintf(tmp_string,"%s/ref_files/adir",input_dir);
    flag *= (0 == grvy_copy_dir(tmp_string,template));
    grvy_printf(GRVY_DEBUG,"Done -->\n");

    /* verify result */

    sprintf(tmp_string,"%s/diff_dir.sh %s/ref_files/adir %s",input_dir,input_dir,template);
    flag *= (0 == system(tmp_string));
  }

  /* Success should occur for existing directory copied to non-existing destination dir */
  {

    flag *= (0 == rmdir(template2));

    grvy_printf(GRVY_DEBUG,"<-- Testing copy from non-existent destination\n");
    sprintf(tmp_string,"%s/ref_files/adir",input_dir);
    flag *= (0 == grvy_copy_dir(tmp_string,template2));
    grvy_printf(GRVY_DEBUG,"Done -->\n");

    /* verify result */

    sprintf(tmp_string,"%s/diff_dir.sh %s/ref_files/adir %s",input_dir,input_dir,template2);
    flag *= (0 == system(tmp_string));
  }

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
