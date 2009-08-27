#include <stdio.h>
#include <stdlib.h>
#include "hpct.h"

/*----------------------------------------------
/* Simple C example illustrating creation of 
 * parent directories for output files via HPCT.
 *
 * $Id$
 *----------------------------------------------*/

int main(int argc, char **argv)
{

  char *filepath1 = "./afile";
  char *filepath2 = "bfile";
  char *filepath3 = "adir/bdir/cfile";
  char *filepath4 = "/tmp/moontower";
  char *filepath5 = "../sneaky/adir/dfile";

  FILE *fp;

  printf("\nPossibly creating parent directories for the following files\n");
  printf("--> %s\n",filepath1);
  printf("--> %s\n",filepath2);
  printf("--> %s\n",filepath3);
  printf("--> %s\n",filepath4);
  printf("--> %s\n",filepath5);

  /* Create parent directories if necessary */

  hpct_check_file_path(filepath1);
  hpct_check_file_path(filepath2);
  hpct_check_file_path(filepath3);
  hpct_check_file_path(filepath4);
  hpct_check_file_path(filepath5);

  /* Write to the files */

  fp = fopen(filepath1,"w");
  fprintf(fp,"tori is sweet\n");
  fclose(fp);

  fp = fopen(filepath2,"w");
  fprintf(fp,"tori is stinky\n");
  fclose(fp);

  fp = fopen(filepath3,"w");
  fprintf(fp,"tori needs a job\n");
  fclose(fp);

  fp = fopen(filepath4,"w");
  fprintf(fp,"tori plays the bongos\n");
  fclose(fp);

  fp = fopen(filepath5,"w");
  fprintf(fp,"tori frowns upon straight ticket voters\n");
  fclose(fp);

  printf("\nSUCCESS\n");

  return 0;
}
