#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "hpct.h"

/*------------------------------------------------
 * HPCT Test Suite
 * Works in conjunction with wrapper_C_scratch_dir
 *
 * $Id$
 *------------------------------------------------*/

const char *test_name = "C scratch directory creation";

/* Useful for debugging */
#define CHECKFLAG fprintf(stderr, "\nflag=%d at line %d\n", flag, __LINE__);

int main(int argc, char **argv)
{
  int flag = 1;

  /* Silence warning/error messages */

  hpct_input_toggle_messages(0);
  
  /* Unable to create a directory on invalid template */
  {
    char invalid_template[] = "ScratchDir_doesNotHaveSixTrailingExes";
    char *template_backup;
    DIR *dir_handle;

    /* Ensure invalid_template directory does not exist to begin */
    dir_handle = opendir(invalid_template);
    flag *= (dir_handle == NULL && errno == ENOENT);

    /* Check call fails and the template was not modified */
    template_backup = strdup(invalid_template);
    flag           *= (0 != hpct_create_scratch_dir(invalid_template));
    flag           *= (0 == strcmp(template_backup, invalid_template));
    free(template_backup);

    /* Ensure invalid_template directory does not exist afterwards */
    dir_handle = opendir(invalid_template);
    flag *= (dir_handle == NULL && errno == ENOENT);
  }

  /* Create a directory on valid template, and check for existence */
  {
    struct stat st;
    char valid_template[]   = "ScratchDir_hasSixTrailingExesXXXXXX";
    char *template_backup;

    template_backup = strdup(valid_template);
    flag           *= (0 == hpct_create_scratch_dir(valid_template));
    flag           *= (0 != strcmp(template_backup, valid_template));
    free(template_backup);

    /* Ensure valid_template directory exists afterwards */
    flag *= (0 == lstat(valid_template, &st));
    flag *= S_ISDIR(st.st_mode);
  }

  /* Create a directory on valid template, and add some files to it */
  /* All this stuff should be removed; checked by wrapper_C_scratch_dir */
  {
    char valid_template[]   = "ScratchDir_hasSixTrailingExesXXXXXX";
    char file_template[]    = "SomeFileX";
    char link_template[]    = "SomeLinkX";
    char dir_template[]     = "SomeDirX";
    struct stat st;
    int i, j;

    /* Ensure valid_template directory exists afterwards */

    flag *= (0 == hpct_create_scratch_dir(valid_template));
    flag *= (0 == lstat(valid_template, &st));
    flag *= S_ISDIR(st.st_mode);

    /* Change to the directory and create some entries */

    flag *= (0 == chdir(valid_template));

    /* Create some subdirectories and change to them   */

    for (j = 0; j < 3; ++j)
      {
	dir_template[strlen(dir_template) - 1] = '0' + j;
	flag *= (0 == mkdir(dir_template, 0777));
	flag *= (0 == chdir(dir_template));

	for (i = 0; i < 10; ++i)
	  {
	    int fd;

	    file_template[strlen(file_template) - 1] = '0' + i;
	    link_template[strlen(link_template) - 1] = '0' + i;

	    /* Create some files */
	    flag *= (-1 != (fd = open(file_template, O_CREAT)));
	    flag *= (-1 != close(fd));

	    /* Create some extra links and symlinks to those files */
	    if (i > 7)
		flag *= (0 == link(file_template, link_template));
	    else if (i > 4)
		flag *= (0 == symlink(file_template, link_template));
	  }

	flag *= (0 == chdir(".."));
      }

    /* Finally, create a scratch directory under the scratch directory */
    for (i = strlen(valid_template) - 6; i < strlen(valid_template); ++i)
      {
	valid_template[i] = 'X';
      }
    flag *= (0 == hpct_create_scratch_dir(valid_template));

    /* Change back to the parent directory */
    flag *= (0 == chdir(".."));
  }

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
