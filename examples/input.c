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
// input.c: Example illustrating basic input file parsing via libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

int main(int argc, char **argv)
{
  int igot;
  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *gridfile;
  char *koomie;
  char *key1, *key2;

  /* Initialize/read the file */

  igot = grvy_input_fopen("./pecos-input.txt");

  /* Read specific variables and echo locally */

  if(grvy_input_fread_float("reyn",&reyn))
    printf("--> %-10s = %f\n","reyn",reyn);

  if(grvy_input_fread_float("mach",&mach))
    printf("--> %-10s = %f\n","mach",mach);

  if(grvy_input_fread_float("aoa",&aoa))
    printf("--> %-10s = %f\n","aoa",aoa);

  if(grvy_input_fread_int("iter_max",&iter_max))
    printf("--> %-10s = %i\n","iter_max",iter_max);

  if(grvy_input_fread_char("gridfile",&gridfile))
    printf("--> %-10s = %s\n","gridfile",gridfile);

  /* Note that GRVY will allocate memory for the
   * string; if you are a purist and want to cleanup, you should
   * call free() when done with the variable */

  free(gridfile);

  /* Read from the solver section */

  if(grvy_input_fread_int("solver/turbulence",&turbulent))
    printf("--> %-10s = %i\n","turbulent",turbulent);

  if(grvy_input_fread_char_ivec("solver/keywords",&key1,0)) 
    printf("--> %-10s = %s \n","solver/keyword 1",key1);

  if(grvy_input_fread_char_ivec("solver/keywords",&key2,1))
    printf("--> %-10s = %s \n","solver/keyword 2",key2);

  if(grvy_input_fread_float_vec("turbulence/A",A,3))
    printf("--> %-10s = %f %f %f\n","turbulence/A",A[0],A[1],A[2]);

  /* Attempt to read unknown variable */

  if(grvy_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  /* Disable error messages if you want to control them yourself
   * If you query the unknown variable again, stdout should be quiet */

  grvy_log_setlevel(GRVY_NOLOG);

  if(grvy_input_fread_char("koomie",&koomie))
    printf("--> %-10s = %s\n","koomie",koomie);

  free(koomie);

  grvy_log_setlevel(GRVY_INFO);

  /* Register variable default values (can be used for
   * backwards-compatible input file support) */

  grvy_input_register_int   ("solver/fast_button",1);
  grvy_input_register_float ("solver/afloat",456.789);
  grvy_input_register_double("solver/adouble",1.23e10);
  grvy_input_register_char  ("solver/astring","shoplifters-unite");

  /* Dump the whole file to stdout */

  printf("\n ------ Full Dump ------\n\n");
  grvy_input_fdump();
  printf(" ---- End Full Dump ----\n\n");

  printf("\n ------ Full Dump (delimited) ------\n\n");
  grvy_input_fdump_delim("# ");
  printf(" ---- End Full Dump ----\n\n");

  /* Dump the whole file to a file */

  printf("\n ------ Full Dump to test.out ------\n\n");
  grvy_input_fdump_file("% ","test.out");
  printf(" -------    End Full Dump    -------\n\n");

  int    igot2;
  float  fgot;
  double dgot;
  char  *sgot;

  if(grvy_input_register_get_int("solver/fast_button",&igot2))
    printf("registered int    = %i\n",igot2);
  if(grvy_input_register_get_float("solver/afloat",&fgot))
    printf("registered float  = %f\n",fgot);
  if(grvy_input_register_get_double("solver/adouble",&dgot))
    printf("registered double = %e\n",dgot);
  if(grvy_input_register_get_char("solver/astring",&sgot))
    printf("registered string = %s\n",sgot);

  /* Read in variable that has a registered default but is not present
     in the input file */

  if(grvy_input_fread_int("solver/fast_button",&igot))
    printf("fread_int: fast_button = %i\n",igot);

  fgot=-1.;
  dgot=-1.;

  if(grvy_input_fread_float("solver/afloat",&fgot))
    printf("fread_float: afloat = %f\n",fgot);

  if(grvy_input_fread_double("solver/adouble",&dgot))
    printf("fread_double: adouble = %e\n",dgot);

  sgot=NULL;

  if(grvy_input_fread_char("solver/astring",&sgot))
    printf("fread_char: astring = %s\n",sgot);
  
  /* Close the file */

  grvy_input_fclose();

  return 0;
}
