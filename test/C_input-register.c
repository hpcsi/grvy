//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008-2013,2018-2022 The PECOS Development Team
// Additional Copyright (C) 2018 individual authors
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
//-----------------------------------------------------------------------el-
//
// Regression/Test Suite for libGRVY.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grvy.h>
#include "examplefile.h"

int main(int argc, char **argv)
{

  int flag     = 1;

  int    iset  = 123;
  float  fset  = 456.7;
  double dset  = 123.e10;
  char  sset[] = "shoplifters-unite"; 
  char *input_example_file_path;

  int    igot;
  float  fgot;
  double dgot;
  char  *sgot;

  /* Register variable (can be use for backwards-compatible
   * input file support) */

  grvy_input_register_int   ("sec1/aint",   iset);
  grvy_input_register_float ("sec1/afloat", fset);
  grvy_input_register_double("sec1/adouble",dset);
  grvy_input_register_char  ("sec1/astring",sset);

  /* Verify that we can recover what we registered as default */

  flag *= grvy_input_register_get_int   ("sec1/aint",&igot);
  flag *= grvy_input_register_get_float ("sec1/afloat",&fgot);
  flag *= grvy_input_register_get_double("sec1/adouble",&dgot);
  flag *= grvy_input_register_get_char  ("sec1/astring",&sgot);

  if(flag == 0)
      exit(1);

  if(igot != iset || fgot != fset || dgot != dset)
    exit(1);

  if(strcmp(sgot,sset) != 0)
    exit(1);

  /* Verify that we can recover registered default variables when they
     are not present in the input file */

  grvy_log_setlevel(GRVY_NOLOG);

  igot=-1;
  fgot=-1.;
  dgot=-1.;
  sgot=NULL;

  input_example_file_path = build_example_file_path("input-example.txt");
  flag *= grvy_input_fopen(input_example_file_path);
  free(input_example_file_path);

  flag *= grvy_input_fread_int   ("sec1/aint",&igot);
  flag *= grvy_input_fread_float ("sec1/afloat",&fgot);
  flag *= grvy_input_fread_double("sec1/adouble",&dgot);
  flag *= grvy_input_fread_char  ("sec1/astring",&sgot);
  flag *= grvy_input_fclose      ();

  if(flag == 0)
      exit(1);

  if(igot != iset || fgot != fset || dgot != dset)
    exit(1);

  if(strcmp(sgot,sset) != 0)
    exit(1);

  return 0;
}
