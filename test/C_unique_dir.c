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

const char *test_name = "C temporary directory creation";

int main(int argc, char **argv)
{
  int flag = 1;

  /* Silence warning/error messages */

  hpct_input_toggle_messages(0);

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
    flag           *= (0 != hpct_create_unique_dir(invalid_template));
    flag           *= (0 == strcmp(template_backup, invalid_template));
    free(template_backup);

    /* Ensure invalid_template directory does not exist afterwards */

    dir_handle = opendir(invalid_template);
    flag *= (dir_handle == NULL && errno == ENOENT);
  }

  /* Create a directory on valid template, and check for existence */
  {
    char valid_template[]   = "TempDir_hasSixTrailingExesXXXXXX";
    char *template_backup;

    template_backup = strdup(valid_template);
    flag           *= (0 == hpct_create_unique_dir(valid_template));
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
