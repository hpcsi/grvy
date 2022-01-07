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
#include <grvy.h>
#include "examplefile.h"

const char *test_name = "C input parsing: test ascii input file dump";

int main(int argc, char **argv)
{
  int flag = 1;
  char *input_example_file_path;
  char tmp_string[1024];
  char *input_dir      = getenv("GRVY_INPUT_EXAMPLE_DIR");
  char dir_template1[] = "grvy-scratch-XXXXXX";
  char dir_template2[] = "grvy-scratch-XXXXXX";

  grvy_log_setlevel(GRVY_WARN);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path("input-example.txt");

  flag *= grvy_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Define registered variable which may not be present in input file */

  grvy_input_register_int   ("sec1/aint",   1);
  grvy_input_register_float ("sec1/afloat", 2.2);
  grvy_input_register_double("sec1/adouble",3.3e3);
  grvy_input_register_char  ("sec1/astring","plankton");

  /* Dump the input contents to a temporary file */

  flag *= (0 == grvy_create_scratch_dir(dir_template1));
  sprintf(tmp_string,"%s/%s",dir_template1,"solution.txt");
  flag *= grvy_input_fdump_file("# ",tmp_string);

  sprintf(tmp_string,"diff %s/ref_files/solution.C.txt %s/solution.txt 2>&1 >/dev/null",
	  input_dir,dir_template1);
  flag *= (0 == system(tmp_string));

  /* Use stdout option and dump file */

  flag *= (0 == grvy_create_scratch_dir(dir_template2));
  //flag *= (0 == grvy_create_unique_dir(dir_template2));
  sprintf(tmp_string,"%s/%s",dir_template2,"solution.stdout.txt");

  freopen(tmp_string,"w",stdout);
  flag *= grvy_input_fdump_delim("# ");

  sprintf(tmp_string,"diff %s/ref_files/solution.C.txt %s/solution.stdout.txt 2>&1 >/dev/null",
	  input_dir,dir_template2);
  flag *= (0 == system(tmp_string));


  freopen("/dev/null","w",stdout);
  flag *= grvy_input_fdump();

  /* Close the file */

  flag *= grvy_input_fclose();

  if(flag == 0)
      exit (1);
  else
    exit (0);

}

