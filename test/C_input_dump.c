#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include "examplefile.h"

/* -------------------------------------------------------------------------
 * GRVY Test Suite
 *
 * $Id$
 *--------------------------------------------------------------------------*/

const char *test_name = "C input parsing: test ascii input file dump";

int main(int argc, char **argv)
{
  int flag = 1;
  char *input_example_file_path;
  char tmp_string[256];
  char dir_template[] = "grvy-scratch-XXXXXX";

  grvy_input_toggle_messages(1);

  /* Initialize/read the file */

  input_example_file_path = build_example_file_path();
  flag *= grvy_input_fopen(input_example_file_path);
  free(input_example_file_path);

  /* Dump the input contents to a temporary file */

  flag *= (0 == grvy_create_scratch_dir(dir_template));
  sprintf(tmp_string,"%s/%s",dir_template,"solution.txt");
  flag *= grvy_input_fdump_file("# ",tmp_string);

  sprintf(tmp_string,"diff ref_files/solution.txt %s/solution.txt 2>&1 >/dev/null",dir_template);

  flag *= (0 == system(tmp_string));

  /* Close the file */

  flag *= grvy_input_fclose();

  if(flag == 0)
      exit (1);
  else
    exit (0);

}

