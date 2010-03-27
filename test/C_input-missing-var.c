#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include "examplefile.h"

/*----------------------------------------------
 * GRVY Test Suite
 *
 * $Id$
 *----------------------------------------------*/

const char *test_name = "C input parsing: missing var check";

int main(int argc, char **argv)
{
  float reyn;
  int flag = 1;
  char *input_example_file_path;

  /* Silence warning/error/info messages */

  grvy_log_setlevel(GRVY_NOLOG);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= grvy_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Read specific variables and echo locally */

  flag *= grvy_input_fread_float("strangeways",&reyn);

  /* Close the file */

  flag *= grvy_input_fclose();

  if(flag == 0)
      exit (0);
  else 
    exit (1);

}
