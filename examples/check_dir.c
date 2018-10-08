//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// libGRVY - a utility library for scientific computing.
//
// Copyright (C) 2008,2009,2010,2011,2012,2013 The PECOS Development Team
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
// check_dir.c: Example illustrating creation of parent directories for 
//              output files via libGRVY in C.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include <assert.h>

int main(int argc, char **argv)
{

  char *filepath1 = "./afile";
  char *filepath2 = "bfile";
  char *filepath3 = "adir/bdir/cfile";
  char *filepath4 = "/tmp/foo/moontower";
  char *filepath5 = "../sneaky/adir/dfile";

  FILE *fp;

  grvy_printf(GRVY_INFO,"\nPossibly creating parent directories for the following files\n");
  grvy_printf(GRVY_INFO,"--> %s\n",filepath1);
  grvy_printf(GRVY_INFO,"--> %s\n",filepath2);
  grvy_printf(GRVY_INFO,"--> %s\n",filepath3);
  grvy_printf(GRVY_INFO,"--> %s\n",filepath4);
  grvy_printf(GRVY_INFO,"--> %s\n",filepath5);

  /* Create parent directories if necessary */

  grvy_check_file_path(filepath1);
  grvy_check_file_path(filepath2);
  grvy_check_file_path(filepath3);
  grvy_check_file_path(filepath4);
  grvy_check_file_path(filepath5);

  /* Write to the files */

  fp = fopen(filepath1,"w"); 
  assert(fp != NULL);
  fprintf(fp,"tori is sweet\n");
  fclose(fp);

  fp = fopen(filepath2,"w");
  assert(fp != NULL);
  fprintf(fp,"tori is stinky\n");
  fclose(fp);

  fp = fopen(filepath3,"w");
  assert(fp != NULL);
  fprintf(fp,"tori needs a job\n");
  fclose(fp);

  fp = fopen(filepath4,"w");
  assert(fp != NULL);
  fprintf(fp,"tori plays the bongos\n");
  fclose(fp);

  fp = fopen(filepath5,"w");
  assert(fp != NULL);
  fprintf(fp,"tori frowns upon straight ticket voters\n");
  fclose(fp);

  /* Recursively copy contents of existing directory to a unique
   * temporary directory
   */

  char newdir[] = "tempXXXXXXX";
  grvy_create_unique_dir(newdir);

  grvy_printf(GRVY_INFO,"\nRecursively copying contents of adir/ to %s/\n",newdir);

  assert(grvy_copy_dir("adir",newdir) == 0);

  grvy_printf(GRVY_INFO,"\nSUCCESS\n");

  return 0;
}
