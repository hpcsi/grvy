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

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <unistd.h>
#include <grvy.h>

/* Useful for debugging */

#define CHECKFLAG fprintf(stderr, "\nflag=%d at line %d\n", flag, __LINE__);

const char *test_name = "C check file path";

int main(int argc, char **argv)
{
  int flag = 1;

  /* Silence info/warn/error messages */

  grvy_log_setlevel(GRVY_NOLOG);

  /* Failure should occur if we attempt to clobber this binary */
  {
    flag *= (-1 == grvy_check_file_path("C_check_file_path/foo"));
  }

  /* Success should occur for universally existent file paths */
  {
    flag *= (0 == grvy_check_file_path("////"));
    flag *= (0 == grvy_check_file_path("."));
    flag *= (0 == grvy_check_file_path(".."));
  }

  /* Success should occur for paths relative to the current directory */
  {
    flag *= (0 == grvy_check_file_path("./CheckFilePathString"));
    flag *= (0 == grvy_check_file_path("../CheckFilePathString"));
  }

  /* Create a single new directory */
  {
    /* Ensure the directory doesn't already exist */
    DIR *dir_handle = opendir("CheckFilePathDir");
    flag *= (dir_handle == NULL && errno == ENOENT);

    flag *= (0 == grvy_check_file_path("CheckFilePathDir/foo"));

    /* Ensure directory exists afterwards and remove it */
    flag *= (0 == rmdir("CheckFilePathDir"));
  }

  /* Create several new directories */
  {
    /* Ensure the directory doesn't already exist */
    DIR *dir_handle = opendir("CheckFilePathDir");
    flag *= (dir_handle == NULL && errno == ENOENT);

    flag *= (0 == grvy_check_file_path("CheckFilePathDir/A/B/C/foo"));

    /* Ensure directory structure exists afterwards and remove it */
    flag *= (0 == rmdir("CheckFilePathDir/A/B/C"));
    flag *= (0 == rmdir("CheckFilePathDir/A/B"));
    flag *= (0 == rmdir("CheckFilePathDir/A"));
    flag *= (0 == rmdir("CheckFilePathDir"));
  }

  /* Create new directories based on absolute path */

  {
    char  pwd[MAXPATHLEN];
    char path[MAXPATHLEN];
    getcwd(pwd,MAXPATHLEN);
    sprintf(path,"%s/CheckFilePathDir/A/B/foo",pwd);

    flag *= (0 == grvy_check_file_path(path));

    /* Ensure directory structure exists afterwards and remove it */

    flag *= (0 == rmdir("CheckFilePathDir/A/B"));
    flag *= (0 == rmdir("CheckFilePathDir/A"));
    flag *= (0 == rmdir("CheckFilePathDir"));
  }

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
