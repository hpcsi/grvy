#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <grvy.h>

/*----------------------------------------------
 * GRVY Test Suite
 *
 * Variable registration - missing var check
 *
 * $Id: input.c 2422 2009-04-19 19:33:42Z karl $
 *----------------------------------------------*/

int main(int argc, char **argv)
{

  int flag = 0;
  int igot;

  grvy_input_toggle_messages(0);

  /* Verify error on unknown variable query */

  flag *= grvy_input_register_get_int   ("solver/aint",&igot);

  if(flag == 1)
      exit(1);

  return 0;
}
