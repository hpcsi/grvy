#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id$
 *----------------------------------------------*/

const char *test_name = "C input parsing: var check";

int main(int argc, char **argv)
{
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *key1, *key2;
  int flag = 1;

  //  printf("--> %s\n",test_name);

  /* Initialize/read the file */

  flag *= hpct_input_fopen("./input-example.txt");

  /* Read specific variables and echo locally */

  flag *= hpct_input_fread_float("reyn",&reyn);
  flag *= flag*hpct_input_fread_float("mach",&mach);

  flag *= hpct_input_fread_float("aoa",&aoa);
  flag *= hpct_input_fread_int("iter_max",&iter_max);
  flag *= hpct_input_fread_char("gridfile",&gridfile);

  /* Note that HPCT will allocate memory for the
   * string; if you are a purist and want to cleanup, you should
   * call free() when done with the variable */

  free(gridfile);

  /* Read from the solver section */

  flag *= hpct_input_fread_int("solver/turbulence",&turbulent);
  flag *= hpct_input_fread_char_ivec("solver/keywords",&key1,0);
  flag *= hpct_input_fread_char_ivec("solver/keywords",&key2,1);
  flag *= hpct_input_fread_float_vec("turbulence/A",A,3);

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
      exit (1);
  else 
    exit (0);

}
