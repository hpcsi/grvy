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

const char *test_name = "C++ input parsing: test ascii input file dump";

using namespace GRVY;

int main(int argc, char **argv)
{
  int flag = 1;
  char *input_example_file_path;
  char tmp_string[256];
  char dir_template1[] = "grvy-scratch-XXXXXX";
  char dir_template2[] = "grvy-scratch-XXXXXX";

  GRVY_Input_Class iparse;	// Input parsing object

  grvy_log_setlevel(GRVY_WARN);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= iparse.Open(input_example_file_path);
  free(input_example_file_path);

  /* Define registered variable which may not be present in input file */

  iparse.Register_Var("sec1/aint",   1);
  iparse.Register_Var("sec1/afloat", 2.2);
  iparse.Register_Var("sec1/adouble",3.3e3);
  iparse.Register_Var("sec1/astring","plankton");
  iparse.Register_Var("sec1/abool",  false);

  /* Dump the input contents to a temporary file */

  flag *= (0 == grvy_create_scratch_dir(dir_template1));
  //flag *= (0 == grvy_create_unique_dir(dir_template1));
  sprintf(tmp_string,"%s/%s",dir_template1,"solution.txt");
  flag *= iparse.Fdump("# ",tmp_string);

  sprintf(tmp_string,"diff ref_files/solution.txt %s/solution.txt 2>&1 >/dev/null",dir_template1);
  flag *= (0 == system(tmp_string));

  /* Close the file */

  flag *= iparse.Close();

  if(flag == 0)
      exit (1);
  else
    exit (0);

}

