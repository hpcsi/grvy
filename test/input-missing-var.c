#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

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

  /* Silence warning/error messages */

  hpct_input_toggle_messages(0);

  /* Initialize/read the file */

  flag *= hpct_input_fopen("./input-example.txt");

  /* Read specific variables and echo locally */

  flag *= hpct_input_fread_float("tommysucks",&reyn);

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
      exit (0);
  else 
    exit (1);

}
