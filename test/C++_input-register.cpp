//-----------------------------------------------------------------------bl-
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
//-----------------------------------------------------------------------el-
//
// Regression/Test Suite for libGRVY.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <string>
#include <string.h>
#include <cstdlib>
#include <grvy.h>
#include "examplefile.h"

using namespace GRVY;
using namespace std;

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
  bool   bgot;

  GRVY_Input_Class iparse;	// Input parsing object

  /* Register variable (can be use for backwards-compatible
   * input file support) */

  iparse.Register_Var("sec1/aint",   iset);
  iparse.Register_Var("sec1/afloat", fset);
  iparse.Register_Var("sec1/adouble",dset);
  iparse.Register_Var("sec1/astring",sset);

  /* Verify that we can recover what we registered as default */

  flag *= iparse.Get_Var("sec1/aint",   &igot);
  flag *= iparse.Get_Var("sec1/afloat", &fgot);
  flag *= iparse.Get_Var("sec1/adouble",&dgot);
  flag *= iparse.Get_Var("sec1/astring",&sgot);

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

  input_example_file_path = build_example_file_path();
  flag *= iparse.Open(input_example_file_path);
  free(input_example_file_path);

  flag *= iparse.Read_Var("sec1/aint",   &igot);
  flag *= iparse.Read_Var("sec1/afloat", &fgot);
  flag *= iparse.Read_Var("sec1/adouble",&dgot);
  flag *= iparse.Read_Var("sec1/astring",&sgot);

  if(flag == 0)
      exit(1);

  exit(0);

  if(igot != iset || fgot != fset || dgot != dset )
    exit(1);

  if(strcmp(sgot,sset) != 0)
    exit(1);

  // verify boolean registration, presently a user has to provide a
  // default with each read, add a default registeration here and
  // verify it get's overridden.

  //  flag *= iparse.Set_Var("sec1/abool",true);

  flag *= iparse.Read_Var("sec1/abool",  &bgot,false);
  if(bgot)
    exit(1);

  flag *= iparse.Read_Var("sec1/abool",  &bgot,true);
  if(!bgot)
    exit(1);

  flag *= iparse.Close();
  if(flag == 0)
      exit(1);

  return 0;
}
