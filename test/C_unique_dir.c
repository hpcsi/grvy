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

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <grvy.h>

const char *test_name = "C temporary directory creation";

int main(int argc, char **argv)
{
  int flag = 1;

  /* Silence warning/error messages */

  grvy_log_setlevel(GRVY_NOLOG);
  
  /* Unable to create a directory on invalid template */

  {
    char invalid_template[] = "TempDir_doesNotHaveSixTrailingExes";
    char *template_backup;
    DIR *dir_handle;

    /* Ensure invalid_template directory does not exist to begin */

    dir_handle = opendir(invalid_template);
    flag *= (dir_handle == NULL && errno == ENOENT);

    /* Check call fails and the template was not modified */

    template_backup = strdup(invalid_template);
    flag           *= (0 != grvy_create_unique_dir(invalid_template));
    flag           *= (0 == strcmp(template_backup, invalid_template));
    free(template_backup);

    /* Check call fails with NULL template */

    flag           *= (0 != grvy_create_unique_dir(NULL));

    /* Ensure invalid_template directory does not exist afterwards */

    dir_handle = opendir(invalid_template);
    flag *= (dir_handle == NULL && errno == ENOENT);
  }

  /* Create a directory on valid template, and check for existence */
  {
    char valid_template[]   = "TempDir_hasSixTrailingExesXXXXXX";
    char *template_backup;

    template_backup = strdup(valid_template);
    flag           *= (0 == grvy_create_unique_dir(valid_template));
    flag           *= (0 != strcmp(template_backup, valid_template));
    free(template_backup);

    /* Ensure valid_template directory exists afterwards and remove it */
    flag *= (0 == rmdir(valid_template));
  }

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
