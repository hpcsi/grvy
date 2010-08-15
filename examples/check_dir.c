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
// check_dir.c: Example illustrating creation of parent directories for 
//              output files via libGRVY in C.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>

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

  grvy_check_file_path(filepath1);
  grvy_check_file_path(filepath2);
  grvy_check_file_path(filepath3);
  grvy_check_file_path(filepath4);
  grvy_check_file_path(filepath5);

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
