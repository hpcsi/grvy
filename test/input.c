#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
 * HPCT Test Suite
 *
 * $Id$
 *----------------------------------------------*/

const char *test_name = "C input parsing: var check";

int compare_int    (int var1,    int var2);
int compare_float  (float var1,  float var2);
int compare_double (double var1, double var2);

int main(int argc, char **argv)
{
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *key1, *key2;

  int    ivar1, ivar2, ivec[4];
  float  fvar1, fvar2, fvec[3];
  double dvar1, dvar2, dvec[3];

  int flag = 1;

  hpct_input_toggle_messages(1);

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

  /* Read from the verify section and verify the results */

  flag *= hpct_input_fread_int      ("verify/ivar",&ivar1);
  flag *= hpct_input_fread_int_ivec ("verify/ivec",&ivar2,2);
  flag *= hpct_input_fread_int_vec  ("verify/ivec",ivec,4);

  if( ivec[0] != 11 || ivec[1] != 12 || ivec[2] != 13 || ivec[3] != 14)
    {
      flag = 0;
      printf("\nRead int_ivec mismatch\n\n");
    }

  flag *= compare_int(ivar1,10);
  flag *= compare_int(ivar2,13);

  flag *= hpct_input_fread_float      ("verify/fvar",&fvar1);
  flag *= hpct_input_fread_float_ivec ("verify/fvec",&fvar2,1);
  flag *= hpct_input_fread_float_vec  ("verify/fvec",fvec,3);

  if( fvec[0] != 101. || fvec[1] != 102. || fvec[2] != 103.)
    {
      flag = 0;
      printf("\nRead float_ivec mismatch\n\n");
    }

  flag *= compare_float(fvar1,100.);
  flag *= compare_float(fvar2,102.);

  flag *= hpct_input_fread_double      ("verify/dvar",&dvar1);
  flag *= hpct_input_fread_double_ivec ("verify/dvec",&dvar2,1);
  flag *= hpct_input_fread_double_vec  ("verify/dvec",dvec,3);

  if( dvec[0] != 1.1e20 || dvec[1] != 1.2e20 || dvec[2] != 1.3e20 )
    {
      flag = 0;
      printf("\nRead double_ivec mismatch\n\n");
    }

  flag *= compare_double(dvar1,1.e20);
  flag *= compare_double(dvar2,1.2e20);

  /* Close the file */

  flag *= hpct_input_fclose();

  if(flag == 0)
      exit (1);
  else 
    exit (0);

}

int compare_int(int var1,int var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %i, expecting %i\n\n",var1,var2);
      return 0;
    }
  return 1;
}

int compare_float(float var1,float var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %f, expecting %f\n\n",var1,var2);
      return 0;
    }
  return 1;
}

int compare_double(double var1,double var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %e, expecting %e\n\n",var1,var2);
      return 0;
    }
  return 1;
}
