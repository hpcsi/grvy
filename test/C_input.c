//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//--------------------------------------------------------------------------
//
// Regression/Test Suite for libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include "examplefile.h"

const char *test_name = "C input parsing: var check";

int compare_int    (int var1,    int var2);
int compare_float  (float var1,  float var2);
int compare_double (double var1, double var2);

int main(int argc, char **argv)
{
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *input_example_file_path;
  char *gridfile;
  char *key1, *key2;

  int    ivar1, ivar2, ivec[4];
  float  fvar1, fvar2, fvec[3];
  double dvar1, dvar2, dvec[3];

  int flag = 1;

  /* Silence warning/error messages */

  grvy_log_setlevel(GRVY_NOLOG);

  /* Verify read call fails prior to file open */

  flag *= (1 != grvy_input_fread_float("reyn",&reyn));

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= grvy_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Read specific variables and echo locally */

  flag *= grvy_input_fread_float("reyn",&reyn);
  flag *= flag*grvy_input_fread_float("mach",&mach);

  flag *= grvy_input_fread_float("aoa",&aoa);
  flag *= grvy_input_fread_int("iter_max",&iter_max);
  flag *= grvy_input_fread_char("gridfile",&gridfile);

  /* Note that GRVY will allocate memory for the
   * string; if you are a purist and want to cleanup, you should
   * call free() when done with the variable */

  free(gridfile);

  /* Read from the solver section */

  flag *= grvy_input_fread_int("solver/turbulence",&turbulent);
  flag *= grvy_input_fread_char_ivec("solver/keywords",&key1,0);
  flag *= grvy_input_fread_char_ivec("solver/keywords",&key2,1);
  flag *= grvy_input_fread_float_vec("turbulence/A",A,3);

  /* Read from the verify section and verify the results */

  flag *= grvy_input_fread_int      ("verify/ivar",&ivar1);
  flag *= grvy_input_fread_int_ivec ("verify/ivec",&ivar2,2);
  flag *= grvy_input_fread_int_vec  ("verify/ivec",ivec,4);

  if( ivec[0] != 11 || ivec[1] != 12 || ivec[2] != 13 || ivec[3] != 14)
    {
      flag = 0;
      printf("\nRead int_ivec mismatch\n\n");
    }

  flag *= compare_int(ivar1,10);
  flag *= compare_int(ivar2,13);

  flag *= grvy_input_fread_float      ("verify/fvar",&fvar1);
  flag *= grvy_input_fread_float_ivec ("verify/fvec",&fvar2,1);
  flag *= grvy_input_fread_float_vec  ("verify/fvec",fvec,3);

  if( fvec[0] != 101. || fvec[1] != 102. || fvec[2] != 103.)
    {
      flag = 0;
      printf("\nRead float_ivec mismatch\n\n");
    }

  flag *= compare_float(fvar1,100.);
  flag *= compare_float(fvar2,102.);

  flag *= grvy_input_fread_double      ("verify/dvar",&dvar1);
  flag *= grvy_input_fread_double_ivec ("verify/dvec",&dvar2,1);
  flag *= grvy_input_fread_double_vec  ("verify/dvec",dvec,3);

  if( dvec[0] != 1.1e20 || dvec[1] != 1.2e20 || dvec[2] != 1.3e20 )
    {
      flag = 0;
      printf("\nRead double_ivec mismatch\n\n");
    }

  flag *= compare_double(dvar1,1.e20);
  flag *= compare_double(dvar2,1.2e20);

  /* Close the file */

  flag *= grvy_input_fclose();

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
