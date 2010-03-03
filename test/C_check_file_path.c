#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id$
 *----------------------------------------------*/

/* Useful for debugging */
#define CHECKFLAG fprintf(stderr, "\nflag=%d at line %d\n", flag, __LINE__);

const char *test_name = "C check file path";

int main(int argc, char **argv)
{
  int flag = 1;

  /* Silence info/warn/error messages */

  hpct_log_setlevel(HPCT_NOLOG);

  /* Failure should occur if we attempt to clobber this binary */
  {
    flag *= (-1 == hpct_check_file_path("C_check_file_path/foo"));
  }

  /* Success should occur for universally existent file paths */
  {
    flag *= (0 == hpct_check_file_path("////"));
    flag *= (0 == hpct_check_file_path("."));
    flag *= (0 == hpct_check_file_path(".."));
  }

  /* Success should occur for paths relative to the current directory */
  {
    flag *= (0 == hpct_check_file_path("./CheckFilePathString"));
    flag *= (0 == hpct_check_file_path("../CheckFilePathString"));
  }

  /* Create a single new directory */
  {
    /* Ensure the directory doesn't already exist */
    DIR *dir_handle = opendir("CheckFilePathDir");
    flag *= (dir_handle == NULL && errno == ENOENT);

    flag *= (0 == hpct_check_file_path("CheckFilePathDir/foo"));

    /* Ensure directory exists afterwards and remove it */
    flag *= (0 == rmdir("CheckFilePathDir"));
  }

  /* Create a several new directories */
  {
    /* Ensure the directory doesn't already exist */
    DIR *dir_handle = opendir("CheckFilePathDir");
    flag *= (dir_handle == NULL && errno == ENOENT);

    flag *= (0 == hpct_check_file_path("CheckFilePathDir/A/B/C/foo"));

    /* Ensure directory structure exists afterwards and remove it */
    flag *= (0 == rmdir("CheckFilePathDir/A/B/C"));
    flag *= (0 == rmdir("CheckFilePathDir/A/B"));
    flag *= (0 == rmdir("CheckFilePathDir/A"));
    flag *= (0 == rmdir("CheckFilePathDir"));
  }

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
