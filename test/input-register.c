#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * Input variable registration
 *
 * $Id: input.c 2422 2009-04-19 19:33:42Z karl $
 *----------------------------------------------*/

int main(int argc, char **argv)
{

  int flag     = 1;

  int    iset  = 123;
  float  fset  = 456.7;
  double dset  = 123.e10;
  char  sset[] = "shoplifters-unit"; 

  int    igot;
  float  fgot;
  double dgot;
  char  *sgot;

  /* Register variable (can be use for backwards-compatible
   * input file support) */

  hpct_input_register_int   ("solver/aint",   iset);
  hpct_input_register_float ("solver/afloat", fset);
  hpct_input_register_double("solver/adouble",dset);
  hpct_input_register_char  ("solver/astring",sset);

  /* Verify that we can recover what we registered as default */

  flag *= hpct_input_register_get_int   ("solver/aint",&igot);
  flag *= hpct_input_register_get_float ("solver/afloat",&fgot);
  flag *= hpct_input_register_get_double("solver/adouble",&dgot);
  flag *= hpct_input_register_get_char  ("solver/astring",&sgot);

  if(flag == 0)
      exit(1);

  if(igot != iset || fgot != fset || dgot != dset)
    exit(1);

  if(strcmp(sgot,sset) != 0)
    exit(1);
  
  return 0;
}
