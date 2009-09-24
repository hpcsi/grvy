#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id$
 *----------------------------------------------*/

const char *test_name = "C temporary directory creation";

int main(int argc, char **argv)
{
  char invalid_template[] = "doesNotHaveSixTrailingExes";
  char valid_template[]   = "hasSixTrailingExesXXXXXX";
  char *template_backup;
  int flag = 1;

  /* Unable to create a directory on NULL input */
  /* FIXME */

  /* Unable to create a directory on invalid template */
  template_backup = strdup(invalid_template);
  flag           *= (0 != hpct_create_temp_dir(invalid_template));
  flag           *= (0 == strcmp(template_backup, invalid_template));
  free(template_backup);

  /* Create a directory on valid template, and check for existence */
  /* FIXME */

  if(flag == 0)
    exit (1);
  else
    exit (0);
}
