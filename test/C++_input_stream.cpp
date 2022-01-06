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
// Regression/Test Suite for libGRVY.
//
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <grvy.h>
#include <sstream>
#include <fstream>
#include "examplefile.h"

using namespace GRVY;
using namespace std;

const char *test_name = "C++ input parsing from stream: var check";

int compare_int    (int var1,    int var2);
int compare_float  (float var1,  float var2);
int compare_double (double var1, double var2);

int main(int argc, char **argv)
{
  GRVY_Input_Class iparse;	// Input parsing object

  float reyn,mach,aoa,A[3];
  int iter_max,turbulent;
  char *input_example_file_path;
  char *gridfile;
  char *key1, *key2;

  int    ivar1, ivar2, ivec[4];
  float  fvar1, fvar2, fvec[3];
  double dvar1, dvar2, dvec[3];

  int flag = 1;

  /* Silence warning/error messages */

  grvy_log_setlevel(GRVY_NOLOG);

  /* Verify read call fails prior to file open */

  flag *= (1 != iparse.Read_Var("reyn",&reyn));

  /* read the file into a stream */

  input_example_file_path = build_example_file_path("input-example.txt");
  std::stringstream buffer;
  std::ifstream file(input_example_file_path);

  buffer << file.rdbuf();

  flag *= iparse.Load(buffer);

  /* Read specific variables and echo locally */

  flag *= iparse.Read_Var("reyn",&reyn);
  flag *= iparse.Read_Var("mach",&mach);

  flag *= iparse.Read_Var("aoa",&aoa);
  flag *= iparse.Read_Var("iter_max",&iter_max);
  flag *= iparse.Read_Var("gridfile",&gridfile);

  // Read from the solver section...

  flag *= iparse.Read_Var      ("solver/turbulence",&turbulent);
  flag *= iparse.Read_Var_iVec ("solver/keywords",&key1,0);
  flag *= iparse.Read_Var_iVec ("solver/keywords",&key2,1);
  flag *= iparse.Read_Var_Vec  ("turbulence/A",A,3);

  // Read from the verify section and verify int reads...

  flag *= iparse.Read_Var      ("verify/ivar",&ivar1);
  flag *= iparse.Read_Var_iVec ("verify/ivec",&ivar2,2);
  flag *= iparse.Read_Var_Vec  ("verify/ivec",ivec,4);

  if( ivec[0] != 11 || ivec[1] != 12 || ivec[2] != 13 || ivec[3] != 14)
    {
      flag = 0;
      grvy_printf(GRVY_INFO,"Read int_ivec mismatch\n");
    }

  flag *= compare_int(ivar1,10);
  flag *= compare_int(ivar2,13);

  // Verify int read with default value provided and entry in input file

  flag *= iparse.Read_Var      ("verify/ivar",&ivar1,-100);
  flag *= compare_int(ivar1,10);

  // Verify int read with default value provided and no entry in input file

  flag *= iparse.Read_Var      ("nothinghere/ivar",&ivar1,-100);
  flag *= compare_int(ivar1,-100);

  // Verify float reads...

  flag *= iparse.Read_Var       ("verify/fvar",&fvar1);
  flag *= iparse.Read_Var_iVec  ("verify/fvec",&fvar2,1);
  flag *= iparse.Read_Var_Vec   ("verify/fvec",fvec,3);

  if( fvec[0] != 101. || fvec[1] != 102. || fvec[2] != 103.)
    {
      flag = 0;
      printf("\nRead float_ivec mismatch\n\n");
    }

  flag *= compare_float(fvar1,100.);
  flag *= compare_float(fvar2,102.);

  // Verify float read with default value provided and entry in input file

  flag *= iparse.Read_Var      ("verify/fvar",&fvar1,-100.0f);
  flag *= compare_float(fvar1,100.0f);

  // Verify float read with default value provided and no entry in input file

  flag *= iparse.Read_Var      ("nothinghere/fvar",&fvar1,-100.0f);
  flag *= compare_float(fvar1,-100.0f);

  // Verify double reads...

  flag *= iparse.Read_Var      ("verify/dvar",&dvar1);
  flag *= iparse.Read_Var_iVec ("verify/dvec",&dvar2,1);
  flag *= iparse.Read_Var_Vec  ("verify/dvec",dvec,3);

  if( dvec[0] != 1.1e20 || dvec[1] != 1.2e20 || dvec[2] != 1.3e20 )
    {
      flag = 0;
      printf("\nRead double_ivec mismatch\n\n");
    }

  flag *= compare_double(dvar1,1.e20);
  flag *= compare_double(dvar2,1.2e20);

  // Verify double read with default value provided and entry in input file

  flag *= iparse.Read_Var      ("verify/dvar",&dvar1,-6.23e3);
  flag *= compare_double(dvar1,1.e20);

  // Verify double read with default value provided and no entry in input file

  flag *= iparse.Read_Var      ("nothinghere/dvar",&dvar1,-6.23e3);
  flag *= compare_double(dvar1,-6.23e3);

  // Verify C++ string reads

  string GridFile;

  flag *= iparse.Read_Var("gridfile",&GridFile);

  if(GridFile.compare("./grid_plate.nc") != 0)
    {
      grvy_printf(GRVY_DEBUG,"string mismatch\n");
      exit(1);
    }

  // Verify string read with default provided and value in input file.

  flag *= iparse.Read_Var("gridfile",&GridFile,"uhoh");

  if(GridFile.compare("./grid_plate.nc") != 0)
    {
      grvy_printf(GRVY_DEBUG,"string mismatch when default provided\n");
      exit(1);
    }

  // Verify string read with default provided but no value in input file).

  flag *= iparse.Read_Var("nothinghere/astring",&GridFile,"gotcha");

  if(GridFile.compare("gotcha") != 0)
    {
      grvy_printf(GRVY_DEBUG,"string mismatch when default provided an no value in input file\n");
      exit(1);
    }

  // Verify C++ boolean reads

  bool restart_flag;

  flag *= iparse.Read_Var("restart",&restart_flag,false);

  if(!restart_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch with value provided in input file\n");
      exit(1);
    }

  flag *= iparse.Read_Var("restart",&restart_flag,true);

  if(!restart_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch with value provided in input file\n");
      exit(1);
    }

  flag *= iparse.Read_Var("nothinghere",&restart_flag,true);

  if(!restart_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch when no value provided in input file (truth test)\n");
      exit(1);
    }

  flag *= iparse.Read_Var("nothinghere",&restart_flag,false);

  if(restart_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch when no value provided in input file (false test)\n");
      exit(1);
    }

  bool adapt_flag;

  flag *= iparse.Read_Var("adapt",&adapt_flag,false);

  if(adapt_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch with adapt value provided in input file\n");
      exit(1);
    }

  flag *= iparse.Read_Var("adapt",&restart_flag,true);

  if(adapt_flag)
    {
      grvy_printf(GRVY_DEBUG,"boolean mismatch with adapt value provided in input file\n");
      exit(1);
    }

  /* Close the file */

  flag *= iparse.Close();

  if(flag == 0)
      exit (1);
  else
    exit (0);

}

int compare_int(int var1,int var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %i, expecting %i\n\n",var1,var2);
      return 0;
    }
  return 1;
}

int compare_float(float var1,float var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %f, expecting %f\n\n",var1,var2);
      return 0;
    }
  return 1;
}

int compare_double(double var1,double var2)
{
  if(var1 != var2)
    {
      printf("\nRead variable mismatch: read %e, expecting %e\n\n",var1,var2);
      return 0;
    }
  return 1;
}
