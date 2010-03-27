#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"
#include "examplefile.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id: C_input-missing-var.c 5607 2009-09-29 00:50:24Z rhys $
 *----------------------------------------------*/

const char *test_name = "C input parsing: missing var check";

int main(int argc, char **argv)
{
  float reyn;
  int flag = 1;
  char *input_example_file_path;

  /* Silence warning/error messages */

  hpct_input_toggle_messages(0);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= hpct_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Read specific variables and echo locally */

  flag *= hpct_input_fread_float("tommysucks",&reyn);

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
      exit (0);
  else 
    exit (1);

}
