#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id$
 *----------------------------------------------*/

const char *test_name = "C input parsing: no file existence";

int main(int argc, char **argv)
{
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *key1, *key2;
  int flag = 1;

  //  hpct_input_toggle_messages(0);
  hpct_log_setlevel(HPCT_NOLOG);

  /* Initialize/read the file */

  flag *= hpct_input_fopen("./tori-rulez");

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
    exit(0);
  else
    exit(1);
}
