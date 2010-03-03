#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"
#include "examplefile.h"

/*----------------------------------------------
 * HPCT Test Suite
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

  hpct_log_setlevel(HPCT_NOLOG);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= hpct_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Read specific variables and echo locally */

  flag *= hpct_input_fread_float("strangeways",&reyn);

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
      exit (0);
  else 
    exit (1);

}
