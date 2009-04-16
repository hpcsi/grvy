#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
/* Simple C example to illustre basic input 
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

  /* Dump the whole file to stdout */

  printf("\n ------ Full Dump ------\n\n");
  hpct_input_fdump();
  printf(" ---- End Full Dump ----\n\n");

  printf("\n ------ Full Dump (delimited) ------\n\n");
  hpct_input_fdump_delim("# ");
  printf(" ---- End Full Dump ----\n\n");

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

  if(hpct_input_fread_float_vec("turbulence/A",A,3))
    printf("--> %-10s = %f %f %f\n","turbulence/A",A[0],A[1],A[2]);

  /* Attempt to read unknown variable */

  if(hpct_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  /* Disable error messages if you want to control them yourself
   * If you query the unknown variable again it should be quiet */

  hpct_input_toggle_messages(0);

  if(hpct_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  /* Close the file */

  hpct_input_fclose();

  return 0;
}
