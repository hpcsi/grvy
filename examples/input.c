#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
/* Simple C example to illustrate basic input 
 * file parsing via HPCT.
 *
 * $Id$
 *----------------------------------------------*/

int main(int argc, char **argv)
{
  int igot;
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *koomie;
  char *key1, *key2;

  /* Initialize/read the file */

  igot = hpct_input_fopen("./pecos-input.txt");

  /* Read specific variables and echo locally */

  if(hpct_input_fread_float("reyn",&reyn))
    printf("--> %-10s = %f\n","reyn",reyn);

  if(hpct_input_fread_float("mach",&mach))
    printf("--> %-10s = %f\n","mach",mach);

  if(hpct_input_fread_float("aoa",&aoa))
    printf("--> %-10s = %f\n","aoa",aoa);

  if(hpct_input_fread_int("iter_max",&iter_max))
    printf("--> %-10s = %i\n","iter_max",iter_max);

  if(hpct_input_fread_char("gridfile",&gridfile))
    printf("--> %-10s = %s\n","gridfile",gridfile);

  /* Note that HPCT will allocate memory for the
   * string; if you are a purist and want to cleanup, you should
   * call free() when done with the variable */

  free(gridfile);

  /* Read from the solver section */

  if(hpct_input_fread_int("solver/turbulence",&turbulent))
    printf("--> %-10s = %i\n","turbulent",turbulent);

  if(hpct_input_fread_char_ivec("solver/keywords",&key1,0)) 
    printf("--> %-10s = %s \n","solver/keyword 1",key1);

  if(hpct_input_fread_char_ivec("solver/keywords",&key2,1))
    printf("--> %-10s = %s \n","solver/keyword 2",key2);

  if(hpct_input_fread_float_vec("turbulence/Abcd",A,3))
    printf("--> %-10s = %f %f %f\n","turbulence/A",A[0],A[1],A[2]);

  /* Attempt to read unknown variable */

  if(hpct_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  /* Disable error messages if you want to control them yourself
   * If you query the unknown variable again, stdout should be quiet */

  hpct_log_setlevel(HPCT_NOLOG);

  if(hpct_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  hpct_log_setlevel(HPCT_INFO);

  /* Register variable default values (can be used for
   * backwards-compatible input file support) */

  hpct_input_register_int   ("solver/fast_button",1);
  hpct_input_register_float ("solver/afloat",456.789);
  hpct_input_register_double("solver/adouble",1.23e10);
  hpct_input_register_char  ("solver/astring","shoplifters-unite");

  /* Dump the whole file to stdout */

  printf("\n ------ Full Dump ------\n\n");
  hpct_input_fdump();
  printf(" ---- End Full Dump ----\n\n");

  printf("\n ------ Full Dump (delimited) ------\n\n");
  hpct_input_fdump_delim("# ");
  printf(" ---- End Full Dump ----\n\n");

  /* Dump the whole file to a file */

  printf("\n ------ Full Dump to test.out ------\n\n");
  hpct_input_fdump_file("% ","test.out");
  printf(" -------    End Full Dump    -------\n\n");

  int    igot2;
  float  fgot;
  double dgot;
  char  *sgot;

  if(hpct_input_register_get_int("solver/fast_button",&igot2))
    printf("registered int    = %i\n",igot2);
  if(hpct_input_register_get_float("solver/afloat",&fgot))
    printf("registered float  = %f\n",fgot);
  if(hpct_input_register_get_double("solver/adouble",&dgot))
    printf("registered double = %e\n",dgot);
  if(hpct_input_register_get_char("solver/astring",&sgot))
    printf("registered string = %s\n",sgot);

  /* Read in variable that has a registered default but is not present
     in the input file */

  if(hpct_input_fread_int("solver/fast_button",&igot))
    printf("fread_int: fast_button = %i\n",igot);

  fgot=-1.;
  dgot=-1.;

  if(hpct_input_fread_float("solver/afloat",&fgot))
    printf("fread_float: afloat = %f\n",fgot);

  if(hpct_input_fread_double("solver/adouble",&dgot))
    printf("fread_double: adouble = %e\n",dgot);

  sgot=NULL;

  if(hpct_input_fread_char("solver/astring",&sgot))
    printf("fread_char: astring = %s\n",sgot);
  
  /* Close the file */

  hpct_input_fclose();

  return 0;
}
